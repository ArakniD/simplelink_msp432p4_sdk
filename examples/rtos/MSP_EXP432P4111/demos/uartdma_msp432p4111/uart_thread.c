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
#include <stdio.h>
#include <string.h>

#include <FreeRTOS.h>

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
#include "lcd_thread.h"
#include "uart_thread.h"
#include "dma_thread.h"
#include <cJSON/cJSON.h>

// cJSON Variables
char rxString[MAX_STR_LENGTH];    // Received JSON strings
char response[MAX_STR_LENGTH];
cJSON * rxJSONObject;

pthread_t uartthread_handler;

/*
 *  ======== uartThread ========
 *  Receives and parses UART messages from the PC GUI and relegate
 *  actions to various threads.
 */
void *uartThread(void *arg0)
{
    LedMsg msg;
    DmaMsg dma;
    int rc;
    int ok = 0;
    int skip = 0;
    UART_Handle uart_handle;
    UART_Params uartParams;
    int32_t receiveArray[4] = {-1, -1, -1, -1};
    uint8_t bytes[32];
#if 0
    const cJSON_Hooks rtos_hooks =
    {
     .free_fn =
    };
    cJSON_InitHooks();
#endif
    /* Call driver init functions */
    UART_init();

    /* Define UART parameters*/
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_TEXT;
    uartParams.readDataMode = UART_DATA_TEXT;
    uartParams.readReturnMode = UART_RETURN_NEWLINE;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart_handle = UART_open(Board_UART0, &uartParams);

    if (uart_handle == NULL) {
        /* UART_open() failed */
        while (1);
    }

    UART_write(uart_handle, "Startup\n", 8);
    memset(rxString, 0 , MAX_STR_LENGTH);

    while (1) {
        UART_read(uart_handle, &rxString, MAX_STR_LENGTH);  //input = RXD byte


        rxJSONObject = cJSON_Parse(rxString);
        cJSON *red = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "red");
        if (red)
            receiveArray[0] = (int32_t) (pwmPeriod * (red->valueint / 255.0f));
        else
            receiveArray[0] = -1;

        cJSON *green = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "green");
        if (green)
            receiveArray[1] =
                    (int32_t) (pwmPeriod * (green->valueint / 255.0f));
        else
            receiveArray[1] = -1;
        cJSON *blue = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "blue");
        if (blue)
            receiveArray[2] = (int32_t) (pwmPeriod * (blue->valueint / 255.0f));
        else
            receiveArray[2] = -1;
        cJSON *bpm = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "bpm");
        if (bpm)
            receiveArray[3] = (int32_t) ((60.0f / bpm->valueint) * 1000000 / 2);
        else
            receiveArray[3] = -1;

        if (red || green || blue || bpm)
        {
            /* Send message to update LED color and blinkrate */
            msg.cmd = LedCmd_PC_UPDATE;
            msg.buffer = (void *)(&receiveArray);
            rc = mq_send(mqLED, (char *)&msg, sizeof(msg), 0);
            if (rc == -1) {
                // Failed to send to message queue
            }
            rc = sem_post(&semLED);
            if (rc == -1) {
                while (1);
            }
            ok = 1;
        }

        /* Accept a DMA send test command and send over to the message queue that we need to
         * send up to X bytes, and that we DO or DO not expect any RTS/CTS events
         */
        cJSON *dmaBaud = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "dmaChangePortSettings");
        if (cJSON_IsArray(dmaBaud) && cJSON_GetArraySize(dmaBaud) == 2)
        {
            static int buffer[2];
            buffer[0] = cJSON_GetArrayItem(dmaBaud, 0)->valueint; // Baud Rate
            buffer[1] = cJSON_GetArrayItem(dmaBaud, 1)->valueint; // Read timeout
            /* Send message to scroll string on LCD */
            dma.cmd = dmaCmd_ChangePortSettings;
            dma.buffer = (void *)(buffer);
            dma.terminalHandle = uart_handle;

            /* Send the message to the main thread first */
            rc = mq_send(mqDMAMaster, (char *)&dma, sizeof(dma), 0);
            if (rc == -1) {
                // Failed to send to message queue
            }
            rc = sem_post(&semDMAMaster);
            if (rc == -1) {
                while (1);
            }
            ok = 1;
        }

        /* Pause the receive handling of Loop or Master
         */
        cJSON *dmaPauseRx = cJSON_GetObjectItemCaseSensitive(
                rxJSONObject, "dmaPauseReceiving");
        if (dmaPauseRx && dmaPauseRx->type == cJSON_Number)
        {
            /* Send message to scroll string on LCD */
            dma.cmd = dmaCmd_PauseReceiving;
            dma.buffer = NULL;
            dma.terminalHandle = uart_handle;

            if (dmaPauseRx->valueint == 1)
            {
                /* Value 1 is the loopback */
                rc = mq_send(mqDMALoopback, (char *) &dma, sizeof(dma), 0);
                if (rc == -1)
                {
                    // Failed to send to message queue
                }
                rc = sem_post(&semDMALoopback);
                if (rc == -1)
                {
                    while (1)
                        ;
                }

            }
            else
            {
                /*Value 0 is the master */
                rc = mq_send(mqDMAMaster, (char *) &dma, sizeof(dma), 0);
                if (rc == -1)
                {
                    // Failed to send to message queue
                }
                rc = sem_post(&semDMAMaster);
                if (rc == -1)
                {
                    while (1)
                        ;
                }
            }
            ok = 1;
        }

        /* Resume the RX handling
         */
        cJSON *dmaResumeRx = cJSON_GetObjectItemCaseSensitive(
                rxJSONObject, "dmaResumeReceiving");
        if (dmaResumeRx && dmaResumeRx->type == cJSON_Number)
        {
            /* Send message to scroll string on LCD */
            dma.cmd = dmaCmd_ResumeReceiving;
            dma.buffer = NULL;
            dma.terminalHandle = uart_handle;

            if (dmaResumeRx->valueint == 1)
            {
                /* Value 1 is the loopback */
                rc = mq_send(mqDMALoopback, (char *) &dma, sizeof(dma), 0);
                if (rc == -1)
                {
                    // Failed to send to message queue
                }
                rc = sem_post(&semDMALoopback);
                if (rc == -1)
                {
                    while (1)
                        ;
                }

            }
            else
            {
                /*Value 0 is the master */
                rc = mq_send(mqDMAMaster, (char *) &dma, sizeof(dma), 0);
                if (rc == -1)
                {
                    // Failed to send to message queue
                }
                rc = sem_post(&semDMAMaster);
                if (rc == -1)
                {
                    while (1)
                        ;
                }
            }
            ok = 1;
        }

        /* Send a number of bytes down the pipe. This is always master and the number of
         * bytes is specified by the valueInt
         */
        cJSON *dmaSendStream = cJSON_GetObjectItemCaseSensitive(
                rxJSONObject, "dmaSendStream");
        if (dmaSendStream && dmaSendStream->type == cJSON_Number)
        {
            static int length;
            length = dmaSendStream->valueint;
            skip = 1;

            /* Send message to scroll string on LCD */
            dma.cmd = dmaCmd_SendStream;
            dma.buffer = (void *)&length;
            dma.terminalHandle = uart_handle;

            /*Value 0 is the master */
            rc = mq_send(mqDMAMaster, (char *) &dma, sizeof(dma), 0);
            if (rc == -1)
            {
                // Failed to send to message queue
            }
            rc = sem_post(&semDMAMaster);
            if (rc == -1)
            {
                while (1)
                    ;
            }
            ok = 1;
        }

        cJSON *lcdScrollString = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "lcdScrollString");
        if (lcdScrollString)
        {
            strcpy(bytes, lcdScrollString->valuestring);
            /* Send message to scroll string on LCD */
            msg.cmd = LcdCmd_SCROLL_STRING;
            msg.buffer = (void *)(bytes);
            rc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
            if (rc == -1) {
                // Failed to send to message queue
            }
            rc = sem_post(&semLCD);
            if (rc == -1) {
                while (1);
            }
            ok = 1;
        }

        cJSON *lcdShowString = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "lcdShowString");
        if (lcdShowString)
        {
            strcpy(bytes, lcdShowString->valuestring);
            /* Send message to display static string on LCD */
            msg.cmd = LcdCmd_SHOW_STRING;
            msg.buffer = (void *)(bytes);
            rc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
            if (rc == -1) {
                // Failed to send to message queue
            }
            rc = sem_post(&semLCD);
            if (rc == -1) {
                while (1);
            }
            ok = 1;
        }

        cJSON *lcdUpdateMemory = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "lcdUpdateMemory");
        if (cJSON_IsArray(lcdUpdateMemory) && cJSON_GetArraySize(lcdUpdateMemory) == 3)
        {
            static int buffer[3];
            buffer[0] = cJSON_GetArrayItem(lcdUpdateMemory, 0)->valueint;
            buffer[1] = cJSON_GetArrayItem(lcdUpdateMemory, 1)->valueint;
            buffer[2] = cJSON_GetArrayItem(lcdUpdateMemory, 2)->valueint;

            /* Send message to update LCD memory register */
            msg.cmd = LcdCmd_UPDATE_MEMORY;
            msg.buffer = (void *)(buffer);
            rc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
            if (rc == -1) {
                // Failed to send to message queue
            }
            rc = sem_post(&semLCD);
            if (rc == -1) {
                while (1);
            }
            ok = 1;
        }

        cJSON *lcdUpdateAnimationMemory = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "lcdUpdateAnimationMemory");
        if (cJSON_IsArray(lcdUpdateAnimationMemory) && cJSON_GetArraySize(lcdUpdateAnimationMemory) == 3)
        {
            static int buffer[3];
            buffer[0] = cJSON_GetArrayItem(lcdUpdateAnimationMemory, 0)->valueint;
            buffer[1] = cJSON_GetArrayItem(lcdUpdateAnimationMemory, 1)->valueint;
            buffer[2] = cJSON_GetArrayItem(lcdUpdateAnimationMemory, 2)->valueint;

            /* Send message to update LCD animation memory register */
            msg.cmd = LcdCmd_UPDATE_ANIMATION_MEMORY;
            msg.buffer = (void *)(buffer);
            rc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
            if (rc == -1) {
                // Failed to send to message queue
            }
            rc = sem_post(&semLCD);
            if (rc == -1) {
                while (1);
            }
            ok = 1;
        }

        cJSON *lcdStartAnimation = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "lcdStartAnimation");
        if (lcdStartAnimation)
        {
            static int startAnimation;
            if (lcdStartAnimation->type == cJSON_True)
                startAnimation = true;
            else
                startAnimation = false;

            /* Send message to start/stop LCD animation */
            msg.cmd = LcdCmd_START_ANIMATION;
            msg.buffer = (void *)(&startAnimation);
            rc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
            if (rc == -1) {
                // Failed to send to message queue
            }
            rc = sem_post(&semLCD);
            if (rc == -1) {
                while (1);
            }
            ok = 1;
        }

        /* Delete JSON object to free up allocated memory */
        cJSON_Delete(rxJSONObject);

        if (!skip) {
            if (ok)
            {
                sprintf(response, "{\"Return\":\"success\"}\n");
                UART_write(uart_handle, response, strlen(response));
            } else {
                sprintf(response, "{\"Return\":\"fail\",\n \"input\":\"%s\"}\n", rxString);
                UART_write(uart_handle, response, strlen(response));
            }

            ok = 0;
            memset(rxString, 0 , MAX_STR_LENGTH);
        } else {
            skip = 0;
        }
    }
}
