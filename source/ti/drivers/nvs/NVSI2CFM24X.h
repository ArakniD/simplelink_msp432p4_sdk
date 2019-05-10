/*!*****************************************************************************
 *  @file       NVSI2CFM24X.h
 *  @brief      Non-Volatile Storage driver implementation
 *              for I2C FRAM peripherals
 *
 *  # Overview #
 *
 *  The NVSI2CFM24X module allows you to manage I2C FRAM memory.
 *  This driver works with most I2C based FRAM memory
 *
 *      Cypress     FM24CL64  64kbit
 *      Cypress     FM24V10   1Mbit
 *
 *  FRAM is unique that no erase or block write is required and
 *  every cell is accessible randomly and sequentially.
 *
 *  The short hand used below is;
 *  S. = Master Start
 *  .A = Slave Ack
 *  .P = Master Stop
 *  .MA = Master Ack
 *  .NA = No Ack
 *
 *  The I2C FRAM commands used by this driver are as follows:
 *
 *  @core
 *  #define I2CFRAM_SLAVE_ADDRESS         0xA0
 *  #define I2CFRAM_SLAVE_A16_SHFT        0x0F
 *  #define I2CFRAM_SLAVE_A16_MSK         0x02
 *  #define I2CFRAM_SLAVE_READ            0x01
 *  #define I2CFRAM_SLAVE_WRITE           0x00
 *  #define I2CFRAM_SLAVE_RW_MSK          0x01
 *  #define I2CFRAM_SLAVE_DEVIO_MSK       0x0C
 *  #define I2CFRAM_SLAVE_DEVIO_SHFT      0x02
 *  #define I2CFRAM_SLAVE_MB(ADR,DEVID,IO)   (I2CFRAM_SLAVE_ADDRESS | \
 *                                        ((ADR >> I2CFRAM_SLAVE_A16_SHFT) & I2CFRAM_SLAVE_A16_MSK) | \
 *                                        (IO & I2CFRAM_SLAVE_RW_MSK) | \
 *                                        ((DEVID << I2CFRAM_SLAVE_DEVIO_SHFT) & I2CFRAM_SLAVE_DEVIO_MSK) )
 *  #define I2CFRAM_SLAVE_KB(DEVID,IO)    (I2CFRAM_SLAVE_ADDRESS | \
 *                                        (IO & I2CFRAM_SLAVE_RW_MSK) | \
 *                                        ((DEVID << (I2CFRAM_SLAVE_DEVIO_SHFT-1)) & (I2CFRAM_SLAVE_DEVIO_MSK | 0x02)) )
 *  // e.g. I2CFRAM_SLAVE(0x1F00, I2CFRAM_SLAVE_READ
 *  #define I2CFRAM_SLEEP_MODE            0xF8 // S.0xF8.A, 0xID.A, S.0x86.A.P
 *
 * Use I2CFRAM_SLAVE_MB for >=1MBit devices
 * Use I2CFRAM_SLAVE_KB for < 1MBit devices
 *
 *  @endcode
 *
 *  Writing to the cell is performed by byte or multi byte request. Send the slave ID (which contains the R/W)
 *  bit, address (2 bytes) followed by the data. End the transmission with a STOP byte such as;
 *
 *  S.I2CFRAM_SLAVE_MB(0x1F00, 0, I2CFRAM_SLAVE_WRITE).A,ADRESS MSB.A,ADDRESS LSB.A,DATA.A,...,P
 *
 *  Writing just the address is performed like;
 *
 *  S.I2CFRAM_SLAVE_MB(0x1F00, 0, I2CFRAM_SLAVE_WRITE).A,ADRESS MSB.A,ADDRESS LSB.A.P
 *
 *  Reading from the current address is performed like;
 *
 *  S.I2CFRAM_SLAVE_MB(0x1F00, 0, I2CFRAM_SLAVE_READ).A,DATA.MA,DATA..NA.p
 */


#ifndef DRIVERS_NVS_NVSI2CFM24X_H_
#define DRIVERS_NVS_NVSI2CFM24X_H_

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/I2C.h>

#if defined (__cplusplus)
extern "C" {
#endif


/*!
 *  @brief Command to perform mass erase of entire flash
 *
 *  Sleep Mode
    A low power mode called Sleep Mode is implemented on the
    FM24V10 device. The device will enter this low power state when
    the Sleep command 86h is clocked-in. Sleep Mode entry can be
    entered as follows:
    1. The master sends a START command.
    2. The master sends Reserved Slave ID F8h.
    3. The FM24V10 sends an ACK.
    4. The master sends the I2C-bus slave address of the slave
    device it needs to identify. The last bit is a 'Don't care' value
    (page select and R/W bits). Only one device must
    acknowledge this byte (the one that has the I2C-bus slave
    address).
    5. The FM24V10 sends an ACK.
    6. The master sends a Re-START command.
    7. The master sends Reserved Slave ID 86h.
    8. The FM24V10 sends an ACK.
    9. The master sends STOP to ensure the device enters sleep
    mode.
    Note Errata: Step 9 - Sending STOP is an optional step for
    FM24V10. The FM24V10 starts entering the Sleep mode from
    step 8 and releases the SDA line when in the Sleep mode. The
    LOW to HIGH transition on the SDA line when I2C clock is HIGH
    generates an unintended STOP. For more information, see
    Errata on page 19.
    Once in sleep mode, the device draws IZZ current, but the device
    continues to monitor the I2C pins. Once the master sends a
    Slave Address that the FM24V10 identifies, it will “wakeup” and
    be ready for normal operation within tREC time. As an alternative
    method of determining when the device is ready, the master can
    send read or write commands and look for an ACK. While the
    device is waking up, it will NACK the master until it is ready.
 *
 *  Mass Erase is the only control command supported.
 */
#define NVSI2CFM24X_CMD_SLEEP  (NVS_CMD_RESERVED + 0)

/*!
 *  @internal @brief NVS function pointer table
 *
 *  'NVSI2CFM24X_fxnTable' is a fully populated function pointer table
 *  that can be referenced in the NVS_config[] array entries.
 *
 *  Users can minimize their application code size by providing their
 *  own custom NVS function pointer table that contains only those APIs
 *  used by the application.
 */
extern const NVS_FxnTable NVSI2CFM24X_fxnTable;

/*!
 *  @brief      NVSI2CFM24X attributes
 *
 * NVSI2CFM24X_HWAttrs nvsI2CHWAttrs[2] = {
 *      //
 *      // region 0 is 4kb for reasons
 *      //
 *      {
 *          .regionBaseOffset = 0,
 *          .regionSize = 4096,
 *          .i2cHandle = NULL,
 *          .i2cIndex = 0,
 *          .framMbit = 1
 *      },
 *      //
 *      // region 1 is 128kb for reasons
 *      //
 *      {
 *          .regionBaseOffset = 4096,
 *          .regionSize = 1024*128
  *         .i2cHandle = NULL,
 *          .i2cIndex = 0,
 *          .framMbit = 1
 *      }
 *  };
 */
typedef struct NVSI2CFM24X_HWAttrs {
    size_t      regionBaseOffset;  /*!< Base offset in the device */
    size_t      regionSize;        /*!< Size of the defined region */
    uint8_t     *verifyBuf;        /*!< Write Pre/Post verify buffer */
    size_t      verifyBufSize;     /*!< Write Pre/Post verify buffer size */
    I2C_Handle *I2CHandle;         /*!< ptr to I2C handle if provided by user. */
    uint16_t    i2cIndex;          /*!< I2C instance index from Board file */
    uint16_t    framMbit;          /*!< I2C size is greater than 1MBit addressing */
    uint8_t     slaveAddress;      /*!< I2C slave address */
} NVSI2CFM24X_HWAttrs;

/*
 *  @brief      NVSI2CFM24X Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct NVSI2CFM24X_Object {
    bool        opened;             /* Has this region been opened */
    I2C_Handle  I2CHandle;
    uint32_t    deviceSize;        /*!< Size of the i2c device, added up by all offsets + size */
} NVSI2CFM24X_Object;

/*
 *  @cond NODOC
 *  NVSI2CFM24X driver public APIs
 */

extern void         NVSI2CFM24X_close(NVS_Handle handle);
extern int_fast16_t NVSI2CFM24X_control(NVS_Handle handle, uint_fast16_t cmd,
                        uintptr_t arg);
// Erase is just a write to all 0xFF....
extern int_fast16_t NVSI2CFM24X_erase(NVS_Handle handle, size_t offset,
                        size_t size);
extern void         NVSI2CFM24X_getAttrs(NVS_Handle handle, NVS_Attrs *attrs);
extern void         NVSI2CFM24X_init();
extern NVS_Handle   NVSI2CFM24X_open(uint_least8_t index, NVS_Params *params);
extern int_fast16_t NVSI2CFM24X_read(NVS_Handle handle, size_t offset,
                        void *buffer, size_t bufferSize);
extern int_fast16_t NVSI2CFM24X_write(NVS_Handle handle, size_t offset,
                        void *buffer, size_t bufferSize, uint_fast16_t flags);
extern int_fast16_t NVSI2CFM24X_lock(NVS_Handle handle, uint32_t timeout);
extern void         NVSI2CFM24X_unlock(NVS_Handle handle);
/*! @endcond */

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

/** @}*/
#endif /* DRIVERS_NVS_NVSI2CFM24X_H_ */
