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
 *    ======== i2cslaveexample1.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/I2CSlave.h>
#include <ti/display/Display.h>

/* Driver configuration */
#include "ti_drivers_config.h"

static Display_Handle display;
uint8_t commandBuffer[10];
uint8_t mirrorRegister[26] = "x ";

I2CSlave_Handle i2cSlave;

bool processBytes(uint8_t *command)
{
    bool retVal = true;

    switch (command[0])
    {
        /*
         * Command: GETSTATUS Cmd from Master.
         * command[0] = 0x01 [GETSTATUS CMD]
         * Response: I2CSlave_write contents of mirrorRegister[0]
         * Content of mirrorRegister[0] is considered as Status byte
         */
        case 0x01:
            retVal = I2CSlave_write(i2cSlave, &mirrorRegister[0], 1);
            break;

        /*
         * Command: SETSTATUS Cmd from Master.
         * command[0] = 0x02 [SETSTATUS CMD]
         * command[1] = Status value to be updated
         * Response: OverWrite content of mirrorRegister[0] with contents of
         * command[1]
         */
        case 0x02:
            mirrorRegister[0] = command[1];
            break;

         /*
         * Command: READBLOCK Cmd from Master.
         * of mirrorRegister
         * command[0] = 0x03 [READBLOCK CMD]
         * command[1] = start offset of bytes to be read from mirrorRegister
         * command[2] = numberOfBytes to be read
         * buffer
         */
        case 0x03:
            retVal = I2CSlave_write(i2cSlave, &mirrorRegister[command[1]],
                        command[2]);
            break;

         /*
         * Command: WRITEBLOCK Cmd from Master.
         * command[0] = 0x04 [WRITEBLOCK CMD]
         * command[1] = start offset of bytes to be written to mirrorRegister
         * command[2] = numberOfBytes to be written
         * Response: Issue I2CSlave_read() to read command[2] number of bytes
         * from master
         */
        case 0x04:
            retVal = I2CSlave_read(i2cSlave, &mirrorRegister[command[1]],
                    command[2]);
            break;
    }

    return retVal;
}
/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    I2CSlave_Params      i2cSlaveParams;
    int i = 0;

    /* Call driver init functions */
    Display_init();
    I2CSlave_init();

    /* Open the UART display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1);
    }

    /* Initialize all buffers */
    for (i = 0; i < 10; i++) {
        /* Initializing the commandBuffer[0-9] = 0x00 */
        commandBuffer[i] = 0x00;
    }

    /* Create I2CSlave for usage */
    I2CSlave_Params_init(&i2cSlaveParams);
    i2cSlaveParams.transferMode = I2CSLAVE_MODE_BLOCKING;
    i2cSlave = I2CSlave_open(CONFIG_I2CSLAVE_0, &i2cSlaveParams);

    if (NULL == i2cSlave) {
        Display_printf(display, 0, 0, "Error Initializing I2CSlave\n");
    }

    /* Loop forever waiting for commands from master */
    while (1) {
        if (I2CSlave_read(i2cSlave, commandBuffer, 10))
            if (!processBytes(commandBuffer)) {
                Display_printf(display, 0, 0, "I2CSlave Command failed\n");
            }
    }

}
