/*
 * LEDLP5562.c
 *
 *  Created on: 7 Nov. 2018
 *      Author: Lucas.Oldfield
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <ti/devices/DeviceFamily.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/drivers/LED.h>
#include <ti/drivers/led/LEDLP5562.h>

#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CMSP432.H>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSP432.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>

/* driverlib header files */
#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

// Register mapping
#define LP5562_ENGINE      0x00
#define  ENG_1               4
#define  ENG_2               2
#define  ENG_3               0
#define LP5562_ENG_EN      0x40
#define LP5562_ENG_LOG     0x80
#define  ENG_EXEC_MODE(ENG, EXEC) ((EXEC & 0x03) << (ENG))

/* OPMODE Register 01h */
#define LP5562_REG_OP_MODE      0x01
#define LP5562_MODE_ENG1_M      0x30
#define LP5562_MODE_ENG2_M      0x0C
#define LP5562_MODE_ENG3_M      0x03
#define LP5562_LOAD_ENG1        0x10
#define LP5562_LOAD_ENG2        0x04
#define LP5562_LOAD_ENG3        0x01
#define LP5562_RUN_ENG1         0x20
#define LP5562_RUN_ENG2         0x08
#define LP5562_RUN_ENG3         0x02
#define LP5562_ENG1_IS_LOADING(mode)    \
    ((mode & LP5562_MODE_ENG1_M) == LP5562_LOAD_ENG1)
#define LP5562_ENG2_IS_LOADING(mode)    \
    ((mode & LP5562_MODE_ENG2_M) == LP5562_LOAD_ENG2)
#define LP5562_ENG3_IS_LOADING(mode)    \
    ((mode & LP5562_MODE_ENG3_M) == LP5562_LOAD_ENG3)
#define  OP_ENG_MODE(ENG, ENG_MODE) ((ENG_MODE & 0x03) << (ENG << 1)) // Example OP_ENG_MODE(OP_ENG_1, OP_ENG_MODE_LOAD) | ...

/* BRIGHTNESS Registers */
#define LP5562_REG_R_PWM            0x04
#define LP5562_REG_G_PWM            0x03
#define LP5562_REG_B_PWM            0x02
#define LP5562_REG_W_PWM            0x0E

/* CURRENT Registers */
#define LP5562_REG_R_CURRENT        0x07
#define LP5562_REG_G_CURRENT        0x06
#define LP5562_REG_B_CURRENT        0x05
#define LP5562_REG_W_CURRENT        0x0F

/* CONFIG Register 08h */
#define LP5562_REG_CONFIG           0x08
#define LP5562_PWM_HF               0x40
#define LP5562_PWRSAVE_EN           0x20
#define LP5562_CLK_INT              0x01    /* Internal clock */
#define LP5562_CLK_AUTO             0x02    /* Auto select block */
#define LP5562_DEFAULT_CFG          (LP5562_PWM_HF | LP5562_PWRSAVE_EN | LP5562_CLK_AUTO)

/* RESET Register 0Dh */
#define LP5562_REG_RESET            0x0D
#define LP5562_RESET                0xFF

/* PROGRAM ENGINE Registers */
#define LP5562_REG_PROG_MEM_ENG1    0x10
#define LP5562_REG_PROG_MEM_ENG2    0x30
#define LP5562_REG_PROG_MEM_ENG3    0x50

/* ENGINE PROGRAM COUNTER Registers */
#define LP5562_REG_ENG1_PC          0x09
#define LP5562_REG_ENG2_PC          0x0A
#define LP5562_REG_ENG3_PC          0x0B

#define LP5562_STATUS               0x0C
#define LP5562_LED_MAP              0x70
#define  LED_MAP_W              3
#define  LED_MAP_R              2
#define  LED_MAP_G              1
#define  LED_MAP_B              0
#define  LED_MAP(COLOUR,MAP)    ((MAP & 0x03) << (COLOUR << 1)) // Example LED_MAP(LED_MAP_W, LED_MAP_DIRECT) | ..

/* highest supported I2C instance index */
#define MAX_I2C_INDEX               4

extern LED_Config LED_config[];
extern const uint8_t LED_count;

// LED function table for the LEDLP5562 implementation
const LED_FxnTable LEDLP5562_fxnTable = {
    LEDLP5562_Close,
    LEDLP5562_Control,
    LEDLP5562_Init,
    LEDLP5562_Open,
    LEDLP5562_Start,
    LEDLP5562_Stop,
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
 *  Semaphore to synchronize access to LEDs.
 */
static SemaphoreP_Handle  writeSem;

extern void LEDLP5562_Init          (void);
static int perfChangeNotifyFxn      (unsigned int eventType, uintptr_t eventArg,
                                     uintptr_t clientArg);
static int_fast16_t extLedI2CWrite  (const uint8_t *buf, size_t len, uint_least8_t slave, uint_least8_t readSlave);
static int32_t pvLedSetColourPwm    (LED_Handle handle, lp5562_pattern *pattern);
static int32_t pvLedSetEngineMapping(LED_Handle handle, lp5562_engine_map *map);
static int32_t pvLedSetRunMode      (LED_Handle handle, lp55xx_run_pattern *run);
extern void ClockP_usleep(uint32_t usec);

int_fast16_t LEDLP5562_Control    (LED_Handle handle, uint_fast16_t cmd, void *arg)
{
    //LEDLP5562_HWAttrs const *hwAttrs;
    LEDLP5562_Object *object;
    int retval = LED_STATUS_SUCCESS;

    SemaphoreP_pend(writeSem, SemaphoreP_WAIT_FOREVER);

    //hwAttrs = handle->hwAttrs;
    object = handle->object;

    I2CHandle = object->I2CHandle;

    switch( cmd ) {
    case LED_CMD_SET_COLOUR:
        retval = pvLedSetColourPwm(handle, arg);
        break;

    case LED_CMD_SET_CONTROL:
        retval = pvLedSetEngineMapping(handle, arg);
        break;

    case LED_CMD_SET_RUN:
        retval = pvLedSetRunMode(handle, arg);
        break;

    default:
        retval =  LED_STATUS_UNDEFINEDCMD ;
        break;
    }

    SemaphoreP_post(writeSem);
    return retval;
}

/*
 *
 */
void LEDLP5562_Close(LED_Handle handle)
{
    LEDLP5562_HWAttrs const *hwAttrs;
    LEDLP5562_Object *object;

    SemaphoreP_pend(writeSem, SemaphoreP_WAIT_FOREVER);

    hwAttrs = handle->hwAttrs;
    object = handle->object;

    I2CHandle = object->I2CHandle;

    /* close the I2C if we opened it */
    if (hwAttrs->I2CHandle == NULL)
    {
        i2cHandleUsers[hwAttrs->i2cIndex] -= 1;

        /* close I2C if this is the last region that uses it */
        if (i2cHandleUsers[hwAttrs->i2cIndex] == 0)
        {
            I2C_close(object->I2CHandle);
            i2cHandles[hwAttrs->i2cIndex] = NULL;

            /* Remove the power registration */
            Power_unregisterNotify(&object->perfChangeNotify);
        }
    }

    object->opened = false;

    SemaphoreP_post(writeSem);
}


void LEDLP5562_Init(void)
{
    unsigned int key;
    SemaphoreP_Handle sem;

    /* speculatively create a binary semaphore for thread safety */
    sem = SemaphoreP_createBinary(1);
    /* sem == NULL will be detected in 'open' */

    key = HwiP_disable();

    if (writeSem == NULL)
    {
        /* use the binary sem created above */
        writeSem = sem;
        HwiP_restore(key);
    }
    else
    {
        /* init already called */
        HwiP_restore(key);
        /* delete unused Semaphore */
        if (sem)
            SemaphoreP_delete(sem);
    }
}

/*
 *
 */
LED_Handle LEDLP5562_Open(uint_least8_t index, LED_Params *params)
{
    LEDLP5562_Object *object;
    LEDLP5562_HWAttrs const *hwAttrs;
    LED_Handle handle;
    I2C_Params i2cParams;

    /* Confirm that 'init' has successfully completed */
    if (writeSem == NULL)
    {
        LEDLP5562_Init();
        if (writeSem == NULL)
        {
            return (NULL);
        }
    }

    /* verify NVS region index */
    if (index >= LED_count)
    {
        return (NULL);
    }

    handle = &LED_config[index];
    object = LED_config[index].object;
    hwAttrs = LED_config[index].hwAttrs;

    SemaphoreP_pend(writeSem, SemaphoreP_WAIT_FOREVER);

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

            /* Register function to reconfigure peripheral on perf level changes */
            Power_registerNotify(&object->perfChangeNotify,
               PowerMSP432_START_CHANGE_PERF_LEVEL | PowerMSP432_DONE_CHANGE_PERF_LEVEL,
               perfChangeNotifyFxn, (uintptr_t) handle);
        }
        object->I2CHandle = i2cHandles[hwAttrs->i2cIndex];
        /* keep track of how many regions use the same I2C handle */
        i2cHandleUsers[hwAttrs->i2cIndex] += 1;
    }

    /* Enable the LED enable function */
    GPIO_write(hwAttrs->gpioEnable, 1);

    /* Enable the clocking output function */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(hwAttrs->clkPort, hwAttrs->clkPin, hwAttrs->clkPinMode);

    /* set protected global variables */
    I2CHandle = object->I2CHandle;

    object->opened = true;

    SemaphoreP_post(writeSem);

    return (handle);
}

static inline void lp5562_wait_opmode_done(void)
{
    /* operation mode change needs to be longer than 153 us */
    ClockP_usleep(300);
}

static inline void lp5562_wait_enable_done(void)
{
    /* it takes more 488 us to update ENABLE register */
    ClockP_usleep(500);
}

/*
 *  ======== perfChangeNotifyFxn ========
 *
 *  Called by Power module before and after performance level is changed.
 */
static int perfChangeNotifyFxn(unsigned int eventType, uintptr_t eventArg,
                               uintptr_t clientArg)
{
    LED_Handle handle = (LED_Handle) clientArg;
    LEDLP5562_HWAttrs const *hwAttrs;
    //LEDLP5562_Object *object;

    hwAttrs = handle->hwAttrs;
    //object = handle->object;

    if (eventType & (PowerMSP432_ENTERING_SLEEP | PowerMSP432_ENTERING_DEEPSLEEP | PowerMSP432_ENTERING_SHUTDOWN ))
    {
        /* Disable the module as well? */

        /* Disable the LED enable function */
        GPIO_write(hwAttrs->gpioEnable, 0);

        /* Disable the clocking output function */
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
                hwAttrs->clkPort, hwAttrs->clkPin,
                GPIO_PRIMARY_MODULE_FUNCTION);
    }
    else if (eventType & ( PowerMSP432_AWAKE_SLEEP | PowerMSP432_AWAKE_DEEPSLEEP ) )
    {
        /* Re-enable the module as well? */

        /* Enable the LED enable function */
        GPIO_write(hwAttrs->gpioEnable, 1);

        /* Enable the clocking output function */
        MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(hwAttrs->clkPort,
                                                        hwAttrs->clkPin,
                                                        hwAttrs->clkPinMode);
    }

    return (Power_NOTIFYDONE);
}

/*!
 * Set the PWM colour registers and record the current colour
 * setup in the registers. This allows not double setting values
 * that have not changed
 */
static int32_t pvLedSetColourPwm(LED_Handle handle, lp5562_pattern *pattern)
{
    LEDLP5562_HWAttrs const *hwAttrs;
    LEDLP5562_Object *object;
    int retval = LED_STATUS_SUCCESS;

    hwAttrs = handle->hwAttrs;
    object = handle->object;

    uint8_t whiteLevelR[2] = {LP5562_REG_R_PWM  , pattern->r};
    uint8_t whiteLevelG[2] = {LP5562_REG_G_PWM  , pattern->g};
    uint8_t whiteLevelB[2] = {LP5562_REG_B_PWM  , pattern->b};
    uint8_t whiteLevelW[2] = {LP5562_REG_W_PWM  , pattern->w};

    /* Enable the LED Driver */
    GPIO_write(hwAttrs->gpioEnable, 1);

    retval = (!retval && pattern->r != object->currentColour.r) ? extLedI2CWrite(whiteLevelR, sizeof(whiteLevelR), hwAttrs->slaveAddress, 0) : retval;
    retval = (!retval && pattern->g != object->currentColour.g) ? extLedI2CWrite(whiteLevelG, sizeof(whiteLevelG), hwAttrs->slaveAddress, 0) : retval;
    retval = (!retval && pattern->b != object->currentColour.b) ? extLedI2CWrite(whiteLevelB, sizeof(whiteLevelB), hwAttrs->slaveAddress, 0) : retval;
    retval = (!retval && pattern->w != object->currentColour.w) ? extLedI2CWrite(whiteLevelW, sizeof(whiteLevelW), hwAttrs->slaveAddress, 0) : retval;

    /* Copy over the current colour if we're successful */
    if (retval == LED_STATUS_SUCCESS)
        object->currentColour = *pattern;

    return retval;
}

/*!
 * Setup the LED mapping for engine to LED
 */
static int32_t pvLedSetEngineMapping(LED_Handle handle, lp5562_engine_map *map)
{
    LEDLP5562_HWAttrs const *hwAttrs;
    LEDLP5562_Object *object;
    int retval = LED_STATUS_SUCCESS;

    hwAttrs = handle->hwAttrs;
    object = handle->object;

    uint8_t ledMap[2] = {LP5562_LED_MAP ,
                         LED_MAP(LED_MAP_W, map->w)
                       | LED_MAP(LED_MAP_R, map->r)
                       | LED_MAP(LED_MAP_G, map->g)
                       | LED_MAP(LED_MAP_B, map->b) };

    /* Enable the LED Driver */
    GPIO_write(hwAttrs->gpioEnable, 1);

    retval = extLedI2CWrite(ledMap, sizeof(ledMap), hwAttrs->slaveAddress, 0);

    if (retval == LED_STATUS_SUCCESS)
        object->currentMap = *map;

    return retval;
}

/*!
 * Set the run-mode of the LED engines
 */
static int32_t pvLedSetRunMode(LED_Handle handle, lp55xx_run_pattern *run)
{
    LEDLP5562_HWAttrs const *hwAttrs;
    LEDLP5562_Object *object;
    int retval = LED_STATUS_SUCCESS;

    hwAttrs = handle->hwAttrs;
    object = handle->object;

    //uint8_t opMode[2]      = {LP5562_REG_OP_MODE, 0};

    uint8_t engine[2]      = {LP5562_ENGINE  , LP5562_ENG_LOG | LP5562_ENG_EN };

    engine[1] |= ENG_EXEC_MODE(ENG_1, run->e1);
    engine[1] |= ENG_EXEC_MODE(ENG_2, run->e2);
    engine[1] |= ENG_EXEC_MODE(ENG_3, run->e3);

    //opMode[1] |= OP_MODE(0, run->e1 == )

    /* Enable the LED Driver */
    GPIO_write(hwAttrs->gpioEnable, 1);

    retval = extLedI2CWrite(engine, sizeof(engine), hwAttrs->slaveAddress, 0);

    lp5562_wait_enable_done();

    if (retval == LED_STATUS_SUCCESS)
        object->currentRun = *run;

    return retval;
}

/*!
 * Startup the LP5562 chip with all necessary registers and initial control modes
 */
int32_t LEDLP5562_Start(LED_Handle handle)
{
    LEDLP5562_HWAttrs const *hwAttrs;
    LEDLP5562_Object *object;
    int retval = LED_STATUS_SUCCESS;
    int i;

    hwAttrs = handle->hwAttrs;
    object = handle->object;
    /*
     *  Get exclusive access to the region.  We don't want someone
     *  else to erase the region while we are reading it.
     */
    SemaphoreP_pend(writeSem, SemaphoreP_WAIT_FOREVER);

    /* set protected global variables */
    I2CHandle = object->I2CHandle;

    /* Setup the LED map command */
    uint8_t ledMap[2]  = {LP5562_LED_MAP , 0 };
    uint8_t ledPWM[2]  = {0, 0};
    uint8_t config[2]  = {LP5562_REG_CONFIG, LP5562_DEFAULT_CFG};

    /* Enable the LED Driver */
    GPIO_write(hwAttrs->gpioEnable, 1);

    retval = !retval ? extLedI2CWrite(config, sizeof(config), hwAttrs->slaveAddress, 0) : retval;
    /* Set all PWM values to zero */
    ledPWM[0] = LP5562_REG_B_PWM ;
    retval = !retval ? extLedI2CWrite(ledPWM, sizeof(ledPWM), hwAttrs->slaveAddress, 0) : retval;
    ledPWM[0] = LP5562_REG_G_PWM ;
    retval = !retval ? extLedI2CWrite(ledPWM, sizeof(ledPWM), hwAttrs->slaveAddress, 0) : retval;
    ledPWM[0] = LP5562_REG_R_PWM ;
    retval = !retval ? extLedI2CWrite(ledPWM, sizeof(ledPWM), hwAttrs->slaveAddress, 0) : retval;
    ledPWM[0] = LP5562_REG_W_PWM ;
    retval = !retval ? extLedI2CWrite(ledPWM, sizeof(ledPWM), hwAttrs->slaveAddress, 0) : retval;

    /* Write out sequence enable and log enable */
    uint8_t engine[2]      = {LP5562_ENGINE , LP5562_ENG_EN | LP5562_ENG_LOG };

    retval = !retval ? extLedI2CWrite(engine, sizeof(engine), hwAttrs->slaveAddress, 0) : retval;
    lp5562_wait_enable_done();

    /* pattern load before everything */
    if (hwAttrs->patterns != NULL)
    {
        uint8_t opModeLoad[2] = { LP5562_REG_OP_MODE, 0 };

        /* Stop the engine */
        // Reg Enable default (0x00, 0xC0) {already done above}
        // stop engines (0x01, 0x00)
        retval = !retval ? extLedI2CWrite(opModeLoad, sizeof(opModeLoad), hwAttrs->slaveAddress, 0) : retval;
        lp5562_wait_opmode_done();
        // reg_sel select pwm
        // Write LED map to RGB
        retval = !retval ? extLedI2CWrite(ledMap, sizeof(ledMap), hwAttrs->slaveAddress, 0) : retval;
        // reg_op_mode direct

        opModeLoad[1]  = OP_ENG_MODE(0, hwAttrs->patterns->size_r ? LP55XX_ENGINE_LOAD : LP55XX_ENGINE_DIRECT);
        opModeLoad[1] |= OP_ENG_MODE(1, hwAttrs->patterns->size_g ? LP55XX_ENGINE_LOAD : LP55XX_ENGINE_DIRECT);
        opModeLoad[1] |= OP_ENG_MODE(2, hwAttrs->patterns->size_b ? LP55XX_ENGINE_LOAD : LP55XX_ENGINE_DIRECT);

        // Prepare the engines to be loaded
        retval = !retval ? extLedI2CWrite(opModeLoad, sizeof(opModeLoad), hwAttrs->slaveAddress, 0) : retval;

        lp5562_wait_opmode_done();

        // Load program data for engine 1
        if (hwAttrs->patterns->b != NULL && hwAttrs->patterns->size_b >= 2 && !retval)
        {
            uint8_t cmd[2];
            cmd[0] = hwAttrs->patterns->b[0];

            for (i=1;i<hwAttrs->patterns->size_b;i++)
            {
                cmd[1] = hwAttrs->patterns->b[i];
                retval = extLedI2CWrite(cmd, 2, hwAttrs->slaveAddress, 0);
                cmd[0]++;
            }
        }

        // Load program data for engine 2
        if (hwAttrs->patterns->g != NULL && hwAttrs->patterns->size_g >= 2 && !retval)
        {
           uint8_t cmd[2];
           cmd[0] = hwAttrs->patterns->g[0];

           for (i=1;i<hwAttrs->patterns->size_g;i++)
           {
               cmd[1] = hwAttrs->patterns->g[i];
               retval = extLedI2CWrite(cmd, 2, hwAttrs->slaveAddress, 0);
               cmd[0]++;
           }
        }

        // Load program data for engine 3
        if (hwAttrs->patterns->r != NULL && hwAttrs->patterns->size_r >= 2 && !retval)
        {
           uint8_t cmd[2];
           cmd[0] = hwAttrs->patterns->r[0];

           for (i=1;i<hwAttrs->patterns->size_r;i++)
           {
               cmd[1] = hwAttrs->patterns->r[i];
               retval = extLedI2CWrite(cmd, 2, hwAttrs->slaveAddress, 0);
               cmd[0]++;
           }
        }

        opModeLoad[1]  = OP_ENG_MODE(0, hwAttrs->patterns->size_r ? LP55XX_ENGINE_RUN  : LP55XX_ENGINE_DIRECT);
        opModeLoad[1] |= OP_ENG_MODE(1, hwAttrs->patterns->size_g ? LP55XX_ENGINE_RUN  : LP55XX_ENGINE_DIRECT);
        opModeLoad[1] |= OP_ENG_MODE(2, hwAttrs->patterns->size_b ? LP55XX_ENGINE_RUN  : LP55XX_ENGINE_DIRECT);

        // Set the loaded engines into RUN mode
        retval = !retval ? extLedI2CWrite(opModeLoad, sizeof(opModeLoad), hwAttrs->slaveAddress, 0) : retval;

        lp5562_wait_opmode_done();
    }
    else
    {
        uint8_t opModeLoad[2] = { LP5562_REG_OP_MODE, 0 };

        opModeLoad[1]  = OP_ENG_MODE(0, LP55XX_ENGINE_DIRECT);
        opModeLoad[1] |= OP_ENG_MODE(1, LP55XX_ENGINE_DIRECT);
        opModeLoad[1] |= OP_ENG_MODE(2, LP55XX_ENGINE_DIRECT);

        // Set the loaded engines into direct mode
        retval = !retval ? extLedI2CWrite(opModeLoad, sizeof(opModeLoad), hwAttrs->slaveAddress, 0) : retval;
        lp5562_wait_opmode_done();
    }

    /* Send all the setup commands here */
    const uint8_t current_reg[4] =
    { LP5562_REG_B_CURRENT, LP5562_REG_G_CURRENT, LP5562_REG_R_CURRENT, LP5562_REG_W_CURRENT };

    // Use the config array of LEDs to define the engine mapping
    for (i = 0; i<hwAttrs->num_channels; i++)
    {
        uint8_t current[2];

        current[0] = current_reg[i];
        current[1] = hwAttrs->led_config[i].max_current;

        ledMap[1] |= LED_MAP(hwAttrs->led_config[i].chan_nr, hwAttrs->led_config[i].engine_index);

        /* Set the LED current for this channel */
        retval = extLedI2CWrite(current, sizeof(current), hwAttrs->slaveAddress, 0);
    }

    /* Set the LED mapping for the chip */
    retval = extLedI2CWrite(ledMap, sizeof(ledMap), hwAttrs->slaveAddress, 0);

    SemaphoreP_post(writeSem);

    return (retval);
}

void         LEDLP5562_Stop       (LED_Handle handle)
{
    LEDLP5562_HWAttrs const *hwAttrs;
    hwAttrs = handle->hwAttrs;

    /* Disable the LED enable function */
    GPIO_write(hwAttrs->gpioEnable, 0);

    /* Disable the clocking output function */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(hwAttrs->clkPort, hwAttrs->clkPin, GPIO_PRIMARY_MODULE_FUNCTION);
}

/*
 *  ======== extLedI2CWrite =======
 */
static int_fast16_t extLedI2CWrite(const uint8_t *buf, size_t len, uint_least8_t slave, uint_least8_t readSlave)
{
    I2C_Transaction masterTransaction;

    masterTransaction.readBuf       = NULL;
    masterTransaction.readCount     = 0;
    masterTransaction.slaveAddress  = slave;
    masterTransaction.readSlaveAddress = readSlave;
    masterTransaction.writeCount    = len;
    masterTransaction.writeBuf      = (void*)buf;

    return (I2C_transfer(I2CHandle, &masterTransaction) ? LED_STATUS_SUCCESS : LED_STATUS_ERROR);
}
