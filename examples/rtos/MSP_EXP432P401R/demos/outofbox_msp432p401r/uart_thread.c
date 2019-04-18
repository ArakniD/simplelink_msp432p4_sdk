/*
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 *  ======== uart_thread.c ========
 */

/* Includes */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* POSIX Header Files */
#include <semaphore.h>

/* Driver Header Files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Board Header Files */
#include "Board.h"

/* Local Header Files */
#include "callbacks.h"
#include "led_thread.h"
#include "uart_thread.h"

/* Global Variables */
volatile int bytesReceived = 7;     // Keeps track of # UART bytes received
volatile char start[5];             // Receives 'START' keyword
volatile int ack = 0;               // Track acknowledgment sent to GUI
volatile int32_t receiveFreq = 0;   // Receives LED blink frequency from GUI
char receiveByte, ackByte = 'A';

pthread_t uartthread_handler;

/*
 *  ======== mainThread ========
 */
void *uartThread(void *arg0)
{
    LedMsg msg;
    int rc;
    UART_Handle uart_handle;
    UART_Params uartParams;
    int32_t receiveByteArray[4] = {-1, -1, -1, -1};

    /* Call driver init functions */
    GPIO_init();
    UART_init();
    Timer_init();

    /* Define UART parameters*/
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart_handle = UART_open(Board_UART0, &uartParams);

    if (uart_handle == NULL) {
        /* UART_open() failed */
        while (1);
    }

    while (1) {
        UART_read(uart_handle, &receiveByte, 1);  //input = RXD byte

        if (ack == 0) {
            UART_write(uart_handle, &ackByte, 1);
            ack = 1;
        }
        /* Wait for 'START' keyword to be received before changing color */
        if (bytesReceived > 6) {
            start[0] = start[1];
            start[1] = start[2];
            start[2] = start[3];
            start[3] = start[4];
            start[4] = receiveByte;

            if (start[0] == 'S' && start[1] == 'T' && start[2] == 'A' && start[3] == 'R' && start[4] == 'T')
                bytesReceived = -1;
        }

        /* Receive/Set color and frequency of LED */
        else {

            if (bytesReceived == 0)
            {
                receiveByteArray[PWM_RED] = (int)(pwmPeriod*(receiveByte/255.0f));
            }
            else if (bytesReceived == 1)
            {
                receiveByteArray[PWM_GREEN] = (int)(pwmPeriod*(receiveByte/255.0f));
            }
            else if (bytesReceived == 2)
            {
                receiveByteArray[PWM_BLUE] = (int)(pwmPeriod*(receiveByte/255.0f));
            }
            else if (bytesReceived == 3)
            {
                receiveFreq = 0;
                receiveFreq |= receiveByte;
            }
            else if (bytesReceived == 4)
                receiveFreq |= (receiveByte << 8);
            else if (bytesReceived == 5)
                receiveFreq |= (receiveByte << 16);
            else if (bytesReceived == 6)
            {
                receiveFreq |= (receiveByte << 24);
                receiveByteArray[3] = receiveFreq/3;
                ack = 0;

                /* Send message to change color */
                msg.cmd = LedCmd_PC_UPDATE;
                msg.buffer = (void *)(&receiveByteArray);
                rc = mq_send(mqLED, (char *)&msg, sizeof(msg), 0);
                if (rc == -1) {
                    // Failed to send to message queue
                }

                rc = sem_post(&semLED);
                if (rc == -1) {
                    while (1);
                }
            }
        }

        if (bytesReceived < 7)
            bytesReceived++;
    }
}
