/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
//****************************************************************************
//
// HAL_I2C.c - Hardware abstraction layer for I2C with MSP432P401R
//
//****************************************************************************

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "HAL_I2C.h"


/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
		48000000,                                // SMCLK = 3MHz
        EUSCI_B_I2C_SET_DATA_RATE_400KBPS,      // Desired I2C Clock of 400khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

void I2C_initGPIO()
{
    /* Select I2C function for I2C_SCL(P6.5) & I2C_SDA(P6.4) */
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P6,
            GPIO_PIN5,
			GPIO_PRIMARY_MODULE_FUNCTION);

    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P6,
            GPIO_PIN4,
			GPIO_PRIMARY_MODULE_FUNCTION);
}


/***************************************************************************//**
 * @brief  Configures I2C
 * @param  none
 * @return none
 ******************************************************************************/

void I2C_init(void)
{
        /* Initialize USCI_B0 and I2C Master to communicate with slave devices*/
    I2C_initMaster(EUSCI_B1_BASE, &i2cConfig);

    /* Disable I2C module to make changes */
    I2C_disableModule(EUSCI_B1_BASE);

    /* Enable I2C Module to start operations */
    I2C_enableModule(EUSCI_B1_BASE);

    return;
}


/***************************************************************************//**
 * @brief  Writes data to the sensor
 * @param  pointer  Address of register you want to modify
 * @param  writeByte Data to be written to the specified register
 * @return none
 ******************************************************************************/

bool I2C_write8 (unsigned char pointer, unsigned char writeByte, unsigned int timeout)
{
    /* Set master to transmit mode PL */
	I2C_setMode(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_MODE);

    /* Clear any existing interrupt flag PL */
	I2C_clearInterruptFlag(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    /* Initiate start and send first character */
    if (!I2C_masterSendMultiByteStartWithTimeout(EUSCI_B1_BASE,
        pointer, timeout))
    	return 0;

    if (!I2C_masterSendMultiByteFinishWithTimeout(EUSCI_B1_BASE,
        writeByte, timeout))
    	return 0;

    return 1;
}


/***************************************************************************//**
 * @brief  Writes data to the sensor
 * @param  pointer  Address of register you want to modify
 * @param  writeWord Data to be written to the specified register
 * @return none
 ******************************************************************************/

bool I2C_write16 (unsigned char pointer, unsigned short writeWord, unsigned int timeout)
{
    /* Set master to transmit mode PL */
	I2C_setMode(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_MODE);

    /* Clear any existing interrupt flag PL */
	I2C_clearInterruptFlag(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    /* Initiate start and send first character */
    if (!I2C_masterSendMultiByteStartWithTimeout(EUSCI_B1_BASE,
        pointer, timeout))
    	return 0;

    /* Send the MSB of writeByte to SENSOR */
    if (!I2C_masterSendMultiByteNextWithTimeout(EUSCI_B1_BASE,
        (unsigned char)(writeWord&0xFF), timeout))
    	return 0;

    if (!I2C_masterSendMultiByteFinishWithTimeout(EUSCI_B1_BASE,
        (unsigned char)(writeWord>>8), timeout))
    	return 0;

    return 1;
}


/***************************************************************************//**
 * @brief  Reads data from the sensor
 * @param  pointer Address of register to read from
 * @return Register contents
 ******************************************************************************/

bool I2C_read8(unsigned char pointer, char * result, unsigned int timeout)
{
    /* Set master to transmit mode PL */
	I2C_setMode(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_MODE);

    /* Clear any existing interrupt flag PL */
	I2C_clearInterruptFlag(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    /* Initiate start and send first character */
    if (!I2C_masterSendSingleByteWithTimeout(EUSCI_B1_BASE,
        pointer, timeout))
    	return 0;

    /*
     * Generate Start condition and set it to receive mode.
     * This sends out the slave address and continues to read
     * until you issue a STOP
     */
//    I2C_masterReceiveStart(EUSCI_B1_BASE);
//
//    /* Read from I2C RX register */
//    if(!I2C_masterReceiveMultiByteFinishWithTimeout(EUSCI_B1_BASE, &val, timeout))
//    	return 0;
//
//    /* Return temperature value */
//    *result = val;

    *result = I2C_masterReceiveSingleByte(EUSCI_B1_BASE);

    return 1;
}


/***************************************************************************//**
 * @brief  Reads data from the sensor
 * @param  pointer Address of register to read from
 * @return Register contents
 ******************************************************************************/

bool I2C_read16(unsigned char pointer, short * result, unsigned int timeout)
{
    uint8_t val = 0;
    uint8_t valScratch = 0;
    short r = 0;

    /* Set master to transmit mode PL */
    I2C_setMode(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_MODE);

    /* Clear any existing interrupt flag PL */
    I2C_clearInterruptFlag(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    /* Initiate start and send first character */
    if (!I2C_masterSendSingleByteWithTimeout(EUSCI_B1_BASE, pointer, timeout))
    	return 0;

    /*
     * Generate Start condition and set it to receive mode.
     * This sends out the slave address and continues to read
     * until you issue a STOP
     */
    I2C_masterReceiveStart(EUSCI_B1_BASE);

    /* Wait for RX buffer to fill */
    while(!(I2C_getInterruptStatus(EUSCI_B1_BASE,
        EUSCI_B_I2C_RECEIVE_INTERRUPT0)));

    /* Read from I2C RX register */
    valScratch = I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);

    /* Receive second byte then send STOP condition */
    if (!I2C_masterReceiveMultiByteFinishWithTimeout(EUSCI_B1_BASE, &val, timeout))
    	return 0;

    /* Shift val to top MSB */
    r = (val << 8);

    /* Read from I2C RX Register and write to LSB of r */
    r |= valScratch;

    /* Return temperature value */
    *result = r;

    return 1;
}


void I2C_setslave(unsigned short slaveAdr)
{
    /* Specify slave address for I2C */
	EUSCI_B_I2C_setSlaveAddress(EUSCI_B1_BASE,
        slaveAdr);

    /* Enable and clear the interrupt flag */
	EUSCI_B_I2C_clearInterruptFlag(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_RECEIVE_INTERRUPT0);
    return;
}
