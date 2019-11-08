/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
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
 *    ======== i2cmasterexample1.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

/* Driver configuration */
#include "ti_drivers_config.h"

static Display_Handle display;

/* Buffers used in this code example */
uint8_t txBuffer[10];
uint8_t rxBuffer[10];
uint8_t mirrorRegister[26] = "x Hello this is master";

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    bool            retVal = false;
    I2C_Handle      i2c;
    I2C_Params      i2cParams;
    I2C_Transaction i2cTransaction;

    /* Call driver init functions */
    Display_init();
    I2C_init();

    /* Open the UART display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1);
    }

    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz;
    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);

    if (i2c == NULL) {
        Display_printf(display, 0, 0, "Error Initializing I2C!\n");
        while (1) {}
    }

    Display_printf(display, 0, 0, "Master: %s\n", mirrorRegister);

    /*
     * Send SETSTATUS Cmd to Slave.
     * txBuffer[0] = 0x02; [SETSTATUS CMD]
     * txBuffer[1] = 'x'; Status byte to be overwritten
     */
     txBuffer[0] = 0x02;
     txBuffer[1] = 'x';
     i2cTransaction.slaveAddress = 0x48;
     i2cTransaction.writeBuf = txBuffer;
     i2cTransaction.writeCount = 2;
     i2cTransaction.readBuf = rxBuffer;
     i2cTransaction.readCount = 0;

     /* Re-try writing to slave till I2C_transfer returns true */
     do {
         retVal = I2C_transfer(i2c, &i2cTransaction);
     } while(!retVal);

    /*
     * Send GETSTATUS Cmd to Slave.
     * txBuffer[0] = 0x01; [GETSTATUS CMD]
     * rxBuffer[0] = status returned by slave
     */
    txBuffer[0] = 0x01;
    i2cTransaction.slaveAddress = 0x48;
    i2cTransaction.writeCount = 1;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.readCount = 1;
    i2cTransaction.readBuf = rxBuffer;

    /* Re-try writing to slave till I2C_transfer returns true */
    do {
        retVal = I2C_transfer(i2c, &i2cTransaction);
    } while(!retVal);

    if (rxBuffer[0] != 'x') {
        Display_printf(display, 0, 0, "ERROR: I2C transfer corruption!\n");
    }

   /*
    * Send SETSTATUS Cmd to Slave.
    * txBuffer[0] = 0x02 [SETSTATUS CMD]
    * txBuffer[1] = 'o' Status byte to be overwritten
    */
    txBuffer[0] = 0x02;
    txBuffer[1] = 'o';

    i2cTransaction.slaveAddress = 0x48;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readCount = 0;

    retVal = I2C_transfer(i2c, &i2cTransaction);

    /*
     * Send WRITEBLOCK Cmd to Slave to expect txBuffer[2]
     * of mirrorRegister
     * txBuffer[0] = 0x04 [WRITEBLOCK CMD]
     * txBuffer[1] = 0x02; start address of slave buffer
     * txBuffer[2] = 19; numberOfBytes to be written
     */
    txBuffer[0] = 0x04;
    txBuffer[1] = 0x02;
    txBuffer[2] = 19;

    i2cTransaction.slaveAddress = 0x48;
    i2cTransaction.writeCount = 3;
    i2cTransaction.readCount = 0;

    /* Re-try writing to slave till I2C_transfer returns true */
    do {
        retVal = I2C_transfer(i2c, &i2cTransaction);
    } while(!retVal);

    /* Write the actual 0x13 bytes */
    i2cTransaction.slaveAddress = 0x48;
    i2cTransaction.writeBuf = "Hello this is slave";
    i2cTransaction.writeCount = 19;
    i2cTransaction.readCount = 0;

    /* Re-try writing to slave till I2C_transfer returns true */
    do {
        retVal = I2C_transfer(i2c, &i2cTransaction);
    } while(!retVal);

    /*
     * Send READBLOCK CMD to Slave mirrorRegister
     * txBuffer[0] = 0x03; [READBLOCK CMD]
     * txBuffer[1] = 0x00; start offset of bytes to be read from slave
     * txBuffer[2] = 26; numberOfBytes to be read
     */
    txBuffer[0] = 0x03;
    txBuffer[1] = 0x00;
    txBuffer[2] = 26;

    /*
     * Response from slave for READBLOCK Cmd
     * numberOfBytes to be read in this case is
     * txBuffer[2]
     */
    i2cTransaction.slaveAddress = 0x48;
    i2cTransaction.writeCount = 3;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.readBuf = mirrorRegister;
    i2cTransaction.readCount = txBuffer[2];

    /* Re-try reading from slave till I2C_transfer returns true */
    do {
        retVal = I2C_transfer(i2c, &i2cTransaction);
    } while(!retVal);

    Display_printf(display, 0, 0, "I2C master/slave transfer complete\n");

    Display_printf(display, 0, 0, "Slave: %s\n", mirrorRegister);

    /* Deinitialized I2C */
    I2C_close(i2c);
    Display_printf(display, 0, 0, "I2C closed!\n");

    return (0);
}
