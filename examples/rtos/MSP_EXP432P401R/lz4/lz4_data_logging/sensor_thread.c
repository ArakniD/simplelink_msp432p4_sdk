/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== sensor_thread.c ========
 */
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* POSIX Header files */
#include <semaphore.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

/* Board Header file */
#include "Board.h"

/* Local Header files */
#include "sensor_thread.h"

#define  CELSIUS_PER_LSB  0.03125F /* Degrees celsius per LSB (TMP007)   */
#define  LUX_BASE_RES     0.01F    /* Result conversion factor (OPT3001) */
#define  TIME             1        /* Read data every 1 second           */

/* I2C Defines */
I2C_Handle      i2c;
I2C_Params      i2cParams;
I2C_Transaction i2cTransaction;

/* RX and TX Buffers for I2C Protocol */
uint8_t         txBuffer[3] = {0};
uint8_t         rxBuffer[3] = {0};

/* Display Driver Parameters */
Display_Handle displayHandle;
Display_Params displayParams;

/* Sensor Defines */
// TMP007
uint16_t rawDieTemp;
uint16_t rawObjTemp;
double_t tDieTemp;
double_t tObjTemp;

// OPT3001
uint16_t rawLux;
double_t convertedLux;

/* Time is Friday, September 12th 1958 12:00:00 PM */
const RTC_C_Calendar startTime =
{
        0x00,    // second         (0-59)
        0x00,    // minute         (0-59)
        0x12,    // hour           (0-23)
        0x05,    // day of week     (0-6)
        0x12,    // day of month   (1-31)
        0x09,    // month          (1-12)
        0x1958   // year         (0-4095)
};

struct data_packet_t packets[32];

static bool sensorTmp007Read(uint16_t *rawDieTemp, uint16_t *rawObjTemp);
static void sensorTmp007Convert(uint16_t rawDieTemp, uint16_t rawObjTemp,
                                double_t *tDieTemp, double_t *tObjTemp);

static void sensorOpt3001Init();
static bool sensorOpt3001Read(uint16_t *rawLux);
static void sensorOpt3001Convert(uint16_t rawLux, double_t *convertedLux);

/*
 *  ======== mainThread ========
 */
void *sensorThread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();
    I2C_init();

    /* Configure & open Display driver */
    Display_Params_init(&displayParams);
    displayHandle = Display_open(Display_Type_UART, &displayParams);
    if (displayHandle == NULL) {
        while (1);
    }

    /* Open I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(Board_I2C_TMP, &i2cParams);
    if (i2c == NULL) {
        while (1);
    }

    /* Initialize the OPT3001 sensor */
    sensorOpt3001Init();

    /* Configuring pins for peripheral/crystal usage and LED for output */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Starting LFXT in non-bypass mode without a timeout. */
    CS_startLFXT(CS_LFXT_DRIVE3);

    /* Initialize RTC calendar for timestamps */
    RTC_C_initCalendar(&startTime, RTC_C_FORMAT_BCD);
    RTC_C_startClock();

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    /* Read data into packets for some amount of time, then post semaphore */
    /* to allow another thread to compress the data and write it to flash  */
    while (1) {
        int i;
        for (i = 0; i < 32; i++) {
            sleep(TIME);
            GPIO_toggle(Board_GPIO_LED0);
            Display_printf(displayHandle, 0, 0, "%d", i+1);

            RTC_C_Calendar currentTime = RTC_C_getCalendarTime();
            packets[i].timestamp = currentTime;
            Display_printf(displayHandle, 0, 0, "%x/%x/%x - %x:%02x:%02x",
                           currentTime.month, currentTime.dayOfmonth, currentTime.year,
                           currentTime.hours, currentTime.minutes, currentTime.seconds);

            /* =================== Temperature Sensor =================== */
            if (sensorTmp007Read(&rawDieTemp, &rawObjTemp)) {

                sensorTmp007Convert(rawDieTemp, rawObjTemp, &tDieTemp, &tObjTemp);

                double_t tObjTempFahrenheit = tObjTemp * 9.0/5.0 + 32.0; // convert to degrees F
                double_t tDieTempFahrenheit = tDieTemp * 9.0/5.0 + 32.0;
                double_t tObjTempKelvin = tObjTemp + 273.15F;            // convert to K
                double_t tDieTempKelvin = tDieTemp + 273.15F;

                packets[i].tObjTemp = tObjTemp;
                packets[i].tDieTemp = tDieTemp;
                packets[i].tObjTempFahrenheit = tObjTempFahrenheit;
                packets[i].tDieTempFahrenheit = tDieTempFahrenheit;
                packets[i].tObjTempKelvin = tObjTempKelvin;
                packets[i].tDieTempKelvin = tDieTempKelvin;

                Display_printf(displayHandle, 0, 0, "ObjTemp: %f C    %f F    %f K",
                               tObjTemp, tObjTempFahrenheit, tObjTempKelvin);
                Display_printf(displayHandle, 0, 0, "DieTemp: %f C    %f F    %f K",
                               tDieTemp, tDieTempFahrenheit, tDieTempKelvin);
            }
            else {
                packets[i].tObjTemp = 0;
                packets[i].tDieTemp = 0;
                packets[i].tObjTempFahrenheit = 0;
                packets[i].tDieTempFahrenheit = 0;
                packets[i].tObjTempKelvin = 0;
                packets[i].tDieTempKelvin = 0;
                Display_printf(displayHandle, 0, 0, "Could not read from TMP007 sensor");
            }

            /* ====================== Light Sensor ====================== */
            if (sensorOpt3001Read(&rawLux)) {
                sensorOpt3001Convert(rawLux, &convertedLux);
                packets[i].convertedLux = convertedLux;
                Display_printf(displayHandle, 0, 0, "Lux:     %f lx\n", convertedLux);
            }
            else {
                packets[i].convertedLux = 0;
                Display_printf(displayHandle, 0, 0, "Could not read from OPT3001 sensor\n");
            }
        }

        sem_post(&semCompress); // post the semaphore after collecting all the data
    }
}

static bool sensorTmp007Read(uint16_t *rawDieTemp, uint16_t *rawObjTemp)
{
    /* Point to the object temp result register and read its 2 bytes */
    txBuffer[0] = TMP007_OBJ_TEMP;
    i2cTransaction.slaveAddress = TMP007_SLAVE_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 2;

    if (I2C_transfer(i2c, &i2cTransaction)) {
        /* Read raw data from object temp result register */
        *rawObjTemp = (rxBuffer[0] << 8) | rxBuffer[1];
    }
    else {
        return false;
    }

    /* Point to the die temp result register and read its 2 bytes */
    txBuffer[0] = TMP007_DIE_TEMP;
    i2cTransaction.slaveAddress = TMP007_SLAVE_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 2;

    if (I2C_transfer(i2c, &i2cTransaction)) {
        /* Read raw data from die temp result register */
        *rawDieTemp = (rxBuffer[0] << 8) | rxBuffer[1];
    }
    else {
        return false;
    }

    return true;
}

static void sensorTmp007Convert(uint16_t rawDieTemp, uint16_t rawObjTemp,
                                double_t *tDieTemp, double_t *tObjTemp)
{
    /* Shift out first 2 don't care bits, convert to Celsius (see TMP007 datasheet) */
    *tObjTemp = ((double_t)(rawObjTemp >> 2)) * CELSIUS_PER_LSB;
    *tDieTemp = ((double_t)(rawDieTemp >> 2)) * CELSIUS_PER_LSB;
}

static void sensorOpt3001Init()
{
    /* Wake up the OPT3001 sensor by writing to it */
    uint16_t configData;
    configData = (uint16_t)OPT3001_CONTINUOUS | (uint16_t)OPT3001_800MS |
                 (uint16_t)OPT3001_FAULT1 | (uint16_t)OPT3001_LATCH | OPT3001_AUTO;

    /* Point to the config register and write default values to it */
    txBuffer[0] = OPT3001_CONFIG;
    txBuffer[1] = configData >> 8;
    txBuffer[2] = configData;
    i2cTransaction.slaveAddress = OPT3001_SLAVE_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 3;
    i2cTransaction.readCount = 0;
    if (!I2C_transfer(i2c, &i2cTransaction)) {
        while (1);
    }
}

static bool sensorOpt3001Read(uint16_t *rawLux)
{
    /* Point to the lux result register and read its 2 bytes */
    txBuffer[0] = OPT3001_RESULT;
    i2cTransaction.slaveAddress = OPT3001_SLAVE_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 2;

    if (I2C_transfer(i2c, &i2cTransaction)) {
        /* Read raw data from lux result register */
        *rawLux = (rxBuffer[0] << 8) | rxBuffer[1];
    }
    else {
        return false;
    }

    return true;
}

static void sensorOpt3001Convert(uint16_t rawLux, double_t *convertedLux)
{
    double_t lsbSize;

    /* The register consists of data bits and exponent bits. Shift out data bits */
    /* from raw data and raise 2 to the resulting power (see OPT3001 datasheet)  */
    lsbSize = (LUX_BASE_RES * (2 << (rawLux >> 12))) / 2;
    /* Mask out exponent bits */
    rawLux &= 0x0FFF;

    /* Return lux */
    *convertedLux = lsbSize * rawLux;
}
