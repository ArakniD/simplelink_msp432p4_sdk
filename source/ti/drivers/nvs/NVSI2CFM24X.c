
/*
 *  ======== NVSI2CFM24X.c ========
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/drivers/NVS.h>
#include <ti/drivers/nvs/NVSI2CFM24X.h>

#include <ti/drivers/I2C.h>

/*  The short hand used below is;
 *  S. = Master Start
 *  .A = Slave Ack
 *  .P = Master Stop
 *  .MA = Master Ack
 *  .NA = No Ack
 *
 *  The I2C FRAM commands used by this driver are as follows:
 */
#define I2CFRAM_SLAVE_ADDRESS         0xA0
#define I2CFRAM_SLAVE_A16_SHFT        0x0F
#define I2CFRAM_SLAVE_A16_MSK         0x02
#define I2CFRAM_SLAVE_READ            0x01
#define I2CFRAM_SLAVE_WRITE           0x00
#define I2CFRAM_SLAVE_RW_MSK          0x01
#define I2CFRAM_SLAVE_DEVIO_MSK       0x0C
#define I2CFRAM_SLAVE_DEVIO_SHFT      0x02
#define I2CFRAM_SLAVE_MB(ADR,DEVID)   (I2CFRAM_SLAVE_ADDRESS | \
                                       ((ADR >> I2CFRAM_SLAVE_A16_SHFT) & I2CFRAM_SLAVE_A16_MSK) | \
                                       ((DEVID << I2CFRAM_SLAVE_DEVIO_SHFT) & I2CFRAM_SLAVE_DEVIO_MSK) ) >> I2CFRAM_SLAVE_RW_MSK
// e.g. I2CFRAM_SLAVE(0x1F00, I2CFRAM_SLAVE_READ
// S.0xF8.A, 0xID.A, S.0x86.A.P
#define I2CFRAM_RSVD_ID               (0xF8 >> 1)
#define I2CFRAM_SLEEP_MODE            (0x86 >> 1)

/* highest supported I2C instance index */
#define MAX_I2C_INDEX               2

static int_fast16_t extFramPowerStandby(NVS_Handle nvsHandle);

extern NVS_Config NVS_config[];
extern const uint8_t NVS_count;

/* NVS function table for NVSI2CFM24X implementation */
const NVS_FxnTable NVSI2CFM24X_fxnTable = {
    NVSI2CFM24X_close,
    NVSI2CFM24X_control,
    NVSI2CFM24X_erase,
    NVSI2CFM24X_getAttrs,
    NVSI2CFM24X_init,
    NVSI2CFM24X_lock,
    NVSI2CFM24X_open,
    NVSI2CFM24X_read,
    NVSI2CFM24X_unlock,
    NVSI2CFM24X_write
};

/* manage I2 indexes */
static I2C_Handle i2cHandles[MAX_I2C_INDEX + 1];
static uint8_t i2cHandleUsers[MAX_I2C_INDEX + 1];
/*
 * currently active (protected within Semaphore_pend() block)
 * I2C handle, and CSN pin
 */
static I2C_Handle I2CHandle;

/*
 *  Semaphore to synchronize access to flash region.
 */
static SemaphoreP_Handle  writeSem;

/*
 * Private static functions
 */
static int_fast16_t doRead(NVS_Handle handle, size_t offset, void *buffer, size_t bufferSize);
static int_fast16_t doErase(NVS_Handle handle, size_t offset, size_t size);
static int_fast16_t doWriteVerify(NVS_Handle handle, size_t offset, void *src,
        size_t srcBufSize, void *dst, size_t dstBufSize, bool preFlag);
static int_fast16_t checkEraseRange(NVS_Handle handle, size_t offset, size_t size);
static int_fast16_t extFlashI2CWrite(const uint8_t *buf, size_t len, uint_least8_t slave, uint_least8_t readSlave);
static int_fast16_t extFlashI2CRead(uint8_t *adrBuf, size_t adrLen, uint8_t *buf, size_t len, uint_least8_t slave);
static int_fast16_t extFramPowerStandby(NVS_Handle nvsHandle);

/*
 *  ======== NVSI2CFM24X_close ========
 */
void NVSI2CFM24X_close(NVS_Handle handle)
{
    NVSI2CFM24X_HWAttrs const *hwAttrs;
    NVSI2CFM24X_Object *object;

    SemaphoreP_pend(writeSem, SemaphoreP_WAIT_FOREVER);

    hwAttrs = handle->hwAttrs;
    object = handle->object;

    I2CHandle = object->I2CHandle;

    /* close the I2C if we opened it */
    if (hwAttrs->I2CHandle == NULL) {
        i2cHandleUsers[hwAttrs->i2cIndex] -= 1;

        /* close I2C if this is the last region that uses it */
        if (i2cHandleUsers[hwAttrs->i2cIndex] == 0) {
            // Put the part in sleep mode
            extFramPowerStandby(handle);

            I2C_close(object->I2CHandle);
            i2cHandles[hwAttrs->i2cIndex] = NULL;
        }
    }

    object->opened = false;

    SemaphoreP_post(writeSem);
}

/*
 *  ======== NVSI2CFM24X_control ========
 */
int_fast16_t NVSI2CFM24X_control(NVS_Handle handle, uint_fast16_t cmd, uintptr_t arg)
{
    NVSI2CFM24X_HWAttrs const *hwAttrs;
    NVSI2CFM24X_Object *object;

    hwAttrs = handle->hwAttrs;
    object = handle->object;

    if (cmd != NVSI2CFM24X_CMD_SLEEP || hwAttrs->framMbit == 0) return (NVS_STATUS_UNDEFINEDCMD);

    /* set protected global variables */
    I2CHandle = object->I2CHandle;

    return (extFramPowerStandby(handle));
}

/*
 *  ======== NVSI2CFM24X_erase ========
 */
int_fast16_t NVSI2CFM24X_erase(NVS_Handle handle, size_t offset, size_t size)
{
    int_fast16_t status;

    SemaphoreP_pend(writeSem, SemaphoreP_WAIT_FOREVER);

    status = doErase(handle, offset, size);

    SemaphoreP_post(writeSem);

    return (status);
}

/*
 *  ======== NVSI2CFM24X_getAttrs ========
 */
void NVSI2CFM24X_getAttrs(NVS_Handle handle, NVS_Attrs *attrs)
{
    NVSI2CFM24X_HWAttrs const *hwAttrs;

    hwAttrs = handle->hwAttrs;

    /* FlashSectorSizeGet() returns the size of a flash sector in bytes. */
    attrs->regionBase  = NVS_REGION_NOT_ADDRESSABLE;
    attrs->regionSize  = hwAttrs->regionSize;
}

/*
 *  ======== NVSI2CFM24X_init ========
 */
void NVSI2CFM24X_init()
{
    unsigned int key;
    SemaphoreP_Handle sem;

    I2C_init();

    /* speculatively create a binary semaphore for thread safety */
    sem = SemaphoreP_createBinary(1);
    /* sem == NULL will be detected in 'open' */

    key = HwiP_disable();

    if (writeSem == NULL) {
        /* use the binary sem created above */
        writeSem = sem;
        HwiP_restore(key);
    }
    else {
        /* init already called */
        HwiP_restore(key);
        /* delete unused Semaphore */
        if (sem) {
            SemaphoreP_delete(sem);
        }
    }
}

/*
 *  ======== NVSI2CFM24X_lock =======
 */
int_fast16_t NVSI2CFM24X_lock(NVS_Handle handle, uint32_t timeout)
{
    if (SemaphoreP_pend(writeSem, timeout) != SemaphoreP_OK) {
        return (NVS_STATUS_TIMEOUT);
    }
    return (NVS_STATUS_SUCCESS);
}

/*
 *  ======== NVSI2CFM24X_open =======
 */
NVS_Handle NVSI2CFM24X_open(uint_least8_t index, NVS_Params *params)
{
    NVSI2CFM24X_Object *object;
    NVSI2CFM24X_HWAttrs const *hwAttrs;
    NVS_Handle handle;
    I2C_Params i2cParams;

    /* Confirm that 'init' has successfully completed */
    if (writeSem == NULL) {
        NVSI2CFM24X_init();
        if (writeSem == NULL) {
            return (NULL);
        }
    }

    /* verify NVS region index */
    if (index >= NVS_count) {
        return (NULL);
    }

    SemaphoreP_pend(writeSem, SemaphoreP_WAIT_FOREVER);

    handle = &NVS_config[index];
    object = NVS_config[index].object;
    hwAttrs = NVS_config[index].hwAttrs;

    if (object->opened == true) {
        SemaphoreP_post(writeSem);
        return (NULL);
    }

    if (hwAttrs->I2CHandle) {
        /* use the provided I2C Handle */
        object->I2CHandle = *hwAttrs->I2CHandle;
    }
    else {
        if (hwAttrs->i2cIndex > MAX_I2C_INDEX) {
            SemaphoreP_post(writeSem);
            return (NULL);
        }
        /* Open I2C if this driver hasn't already opened this I2C instance */
        if (i2cHandles[hwAttrs->i2cIndex] == NULL) {
            I2C_Handle i2c;

            I2C_Params_init(&i2cParams);
            i2cParams.transferMode = I2C_MODE_BLOCKING;
            i2cParams.bitRate = I2C_400kHz;

            /* Attempt to open I2C. */
            i2c = I2C_open(hwAttrs->i2cIndex, &i2cParams);

            if (i2c == NULL) {
                SemaphoreP_post(writeSem);
                return (NULL);
            }

            i2cHandles[hwAttrs->i2cIndex] = i2c;
        }
        object->I2CHandle = i2cHandles[hwAttrs->i2cIndex];
        /* keep track of how many regions use the same I2C handle */
        i2cHandleUsers[hwAttrs->i2cIndex] += 1;
    }

    /* set protected global variables */
    I2CHandle = object->I2CHandle;

    object->opened = true;

    SemaphoreP_post(writeSem);

    return (handle);
}

/*
 *  ======== NVSI2CFM24X_read =======
 */
int_fast16_t NVSI2CFM24X_read(NVS_Handle handle, size_t offset, void *buffer,
        size_t bufferSize)
{
    NVSI2CFM24X_HWAttrs const *hwAttrs;
    int retval = NVS_STATUS_SUCCESS;

    hwAttrs = handle->hwAttrs;

    /* Validate offset and bufferSize */
    if (offset + bufferSize > hwAttrs->regionSize) {
        return (NVS_STATUS_INV_OFFSET);
    }

    /*
     *  Get exclusive access to the region.  We don't want someone
     *  else to erase the region while we are reading it.
     */
    SemaphoreP_pend(writeSem, SemaphoreP_WAIT_FOREVER);

    retval = doRead(handle, offset, buffer, bufferSize);

    SemaphoreP_post(writeSem);

    return (retval);
}

/*
 *  ======== NVSI2CFM24X_unlock =======
 */
void NVSI2CFM24X_unlock(NVS_Handle handle)
{
    SemaphoreP_post(writeSem);
}

/*
 * Address header for small and large parts
 */
typedef struct I2CFM24X_Hdr {
    /* Address MSB */
    uint8_t  addr_msb;
    /* Address LSB */
    uint8_t  addr_lsb;
} I2CFM24X_Hdr;

/*
 *  ======== NVSI2CFM24X_write =======
 */
int_fast16_t NVSI2CFM24X_write(NVS_Handle handle, size_t offset, void *buffer,
                      size_t bufferSize, uint_fast16_t flags)
{
    NVSI2CFM24X_Object *object;
    I2CFM24X_Hdr       *header;
    NVSI2CFM24X_HWAttrs const *hwAttrs;
    int_fast16_t rangeStatus;
    size_t foffset;
    uint32_t status = true;
    int retval = NVS_STATUS_SUCCESS;
    uint8_t slave_id;

    hwAttrs = handle->hwAttrs;
    object = handle->object;

    /* sanity test the erase args */
    rangeStatus = checkEraseRange(handle, offset, bufferSize);

    if (rangeStatus != NVS_STATUS_SUCCESS) {
       return (rangeStatus);
    }

    /* Get exclusive access to the Flash region */
    SemaphoreP_pend(writeSem, SemaphoreP_WAIT_FOREVER);

    /* set protected global variables */
    I2CHandle = object->I2CHandle;

    foffset = (size_t)hwAttrs->regionBaseOffset + offset;

    /* FRAM has no limit to the read/write size.
    * effectively unlimited
    */
    header = (I2CFM24X_Hdr*) hwAttrs->verifyBuf;
    slave_id = I2CFRAM_SLAVE_MB(foffset,0);
    header->addr_msb = (foffset >> 8  ) & 0xff;
    header->addr_lsb = (foffset & 0xff);

    /* Copy in the data to be transfered into the verify buffer */
    memcpy(hwAttrs->verifyBuf + sizeof(I2CFM24X_Hdr), buffer, bufferSize);

    if (status && extFlashI2CWrite(hwAttrs->verifyBuf, sizeof(I2CFM24X_Hdr) + bufferSize, slave_id, 0) != NVS_STATUS_SUCCESS) {
        status = false;
    }
    else if (flags & NVS_WRITE_POST_VERIFY) {
        if ((hwAttrs->verifyBuf == NULL) || (hwAttrs->verifyBufSize == 0)) {
            SemaphoreP_post(writeSem);
            return (NVS_STATUS_ERROR);
        }

        retval = doWriteVerify(handle, offset, buffer, bufferSize,
                     hwAttrs->verifyBuf, bufferSize, false); // hwAttrs->verifyBufSize
    }

    SemaphoreP_post(writeSem);

    return (retval);
}

/*
 *  ======== checkEraseRange ========
 */
static int_fast16_t checkEraseRange(NVS_Handle handle, size_t offset, size_t size)
{
    NVSI2CFM24X_HWAttrs const *hwAttrs;

    hwAttrs = handle->hwAttrs;

    if (offset >= hwAttrs->regionSize) {
        return (NVS_STATUS_INV_OFFSET);   /* offset is past end of region */
    }

    if (offset + size > hwAttrs->regionSize) {
        return (NVS_STATUS_INV_SIZE);     /* size is too big */
    }

    return (NVS_STATUS_SUCCESS);
}

/*
 *  ======== doWriteVerify =======
 */
static int_fast16_t doWriteVerify(NVS_Handle handle, size_t offset, void *src,
           size_t srcBufSize, void *dst, size_t dstBufSize, bool preFlag)
{
    size_t i, j;
    uint8_t *srcBuf, *dstBuf;
    bool bad;
    int_fast16_t retval;

    srcBuf = src;
    dstBuf = dst;

    j = dstBufSize;

    for (i = 0; i < srcBufSize; i++, j++) {
        if (j == dstBufSize) {
            retval = doRead(handle, offset + i, dstBuf, j);
            if (retval != NVS_STATUS_SUCCESS) {
                break;
            }
            j = 0;
        }
        if (preFlag) {
            bad = srcBuf[i] != (srcBuf[i] & dstBuf[j]);
        }
        else {
            bad = srcBuf[i] != dstBuf[j];
        }
        if (bad) return (NVS_STATUS_INV_WRITE);
    }
    return (NVS_STATUS_SUCCESS);
}

/*
 *  ======== doErase ========
 */
static int_fast16_t doErase(NVS_Handle handle, size_t offset, size_t size)
{
    NVSI2CFM24X_Object *object;
    I2CFM24X_Hdr       *header;
    NVSI2CFM24X_HWAttrs const *hwAttrs;
    int_fast16_t rangeStatus;
    size_t foffset;
    uint8_t slave_id;

    hwAttrs = handle->hwAttrs;
    object = handle->object;

    /* sanity test the erase args */
    rangeStatus = checkEraseRange(handle, offset, size);

    if (rangeStatus != NVS_STATUS_SUCCESS) {
       return (rangeStatus);
    }

    /* set protected global variables */
    I2CHandle = object->I2CHandle;

    foffset = (size_t)hwAttrs->regionBaseOffset + offset;

    /* FRAM has no limit to the read/write size.
    * effectively unlimited
    */
    header = (I2CFM24X_Hdr*) hwAttrs->verifyBuf;
    slave_id = I2CFRAM_SLAVE_MB(foffset,0);
    header->addr_msb = (foffset >> 8  ) & 0xff;
    header->addr_lsb = (foffset & 0xff);

    /* Set the buffer to 0xFF please */
    memset(hwAttrs->verifyBuf + sizeof(I2CFM24X_Hdr), 0xFF, size);

    return extFlashI2CWrite(hwAttrs->verifyBuf, sizeof(I2CFM24X_Hdr) + size, slave_id, 0);
}

/*
 *  ======== doRead =======
 */
static int_fast16_t doRead(NVS_Handle handle, size_t offset, void *buffer, size_t bufferSize)
{
    NVSI2CFM24X_Object *object;
    NVSI2CFM24X_HWAttrs const *hwAttrs;
    I2CFM24X_Hdr header;
    size_t foffset;
    uint8_t slave_id;

    hwAttrs = handle->hwAttrs;
    object = handle->object;

    /* set protected global variables */
    I2CHandle = object->I2CHandle;

    foffset = offset + hwAttrs->regionBaseOffset;

    /*
     * Setup the current address
    */
    slave_id = I2CFRAM_SLAVE_MB(foffset, 0);
    header.addr_msb = (foffset >> 8  ) & 0xff;
    header.addr_lsb = (foffset & 0xff);

    return extFlashI2CRead((uint8_t*)&header, sizeof(header), buffer, bufferSize, slave_id);
}

/*
 *  ======== extFlashI2CWrite =======
 */
static int_fast16_t extFlashI2CWrite(const uint8_t *buf, size_t len, uint_least8_t slave, uint_least8_t readSlave)
{
    I2C_Transaction masterTransaction;

    masterTransaction.readBuf       = NULL;
    masterTransaction.readCount     = 0;
    masterTransaction.slaveAddress  = slave;
    masterTransaction.readSlaveAddress = readSlave;
    masterTransaction.writeCount    = len;
    masterTransaction.writeBuf      = (void*)buf;

    return (I2C_transfer(I2CHandle, &masterTransaction) ? NVS_STATUS_SUCCESS : NVS_STATUS_ERROR);
}

/*
 *  ======== extFlashI2CRead =======
 *  adrBuf is the address to send for the read, NULL if continuation from previous
 *  address counter
 */
static int_fast16_t extFlashI2CRead(uint8_t *adrBuf, size_t adrLen, uint8_t *buf, size_t len, uint_least8_t slave)
{
    I2C_Transaction masterTransaction;

    masterTransaction.writeBuf = adrBuf;
    masterTransaction.writeCount = adrLen;
    masterTransaction.readSlaveAddress = 0;
    masterTransaction.slaveAddress = slave;
    masterTransaction.readCount = len;
    masterTransaction.readBuf = buf;

    return (I2C_transfer(I2CHandle, &masterTransaction) ? NVS_STATUS_SUCCESS : NVS_STATUS_ERROR);
}

/*
 * ========= extFramPowerStandby ======
 * Sends the chip into a standby power state
 */
static int_fast16_t extFramPowerStandby(NVS_Handle nvsHandle)
{
    NVSI2CFM24X_Object *object;
    uint8_t rsvd_slave_id;
    uint8_t read_slave_id;
    uint8_t slave_id;

    object = nvsHandle->object;

    /* set protected global variables */
    I2CHandle = object->I2CHandle;

    /*
    * Setup the current address
    */
    rsvd_slave_id = I2CFRAM_RSVD_ID;
    slave_id = I2CFRAM_SLAVE_MB(0, 0) << 1;
    read_slave_id = I2CFRAM_SLEEP_MODE;

   return extFlashI2CWrite((uint8_t*)&slave_id, 1, rsvd_slave_id, read_slave_id);

}
