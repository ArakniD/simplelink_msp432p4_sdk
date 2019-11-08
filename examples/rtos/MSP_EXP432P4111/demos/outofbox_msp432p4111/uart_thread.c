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
#include <string.h>

/* POSIX Header Files */
#include <semaphore.h>

/* Driver Header Files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Board Header file */
#include "ti_drivers_config.h"

/* Local Header Files */
#include "callbacks.h"
#include "led_thread.h"
#include "lcd_thread.h"
#include "uart_thread.h"
#include <cJSON/cJSON.h>

// cJSON Variables
char rxString[MAX_STR_LENGTH];    // Received JSON strings
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
    int rc;
    UART_Handle uart_handle;
    UART_Params uartParams;

    /* Call driver init functions */
    UART_init();

    /* Define UART parameters*/
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_TEXT;
    uartParams.readDataMode = UART_DATA_TEXT;
    uartParams.readReturnMode = UART_RETURN_NEWLINE;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart_handle = UART_open(CONFIG_UART0, &uartParams);

    if (uart_handle == NULL) {
        /* UART_open() failed */
        while (1);
    }

    while (1) {
        UART_read(uart_handle, &rxString, MAX_STR_LENGTH);  //input = RXD byte

        int32_t receiveArray[4] = {-1, -1, -1, -1};
        rxJSONObject = cJSON_Parse(rxString);
        cJSON *red = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "red");
        if (red)
            receiveArray[0] = (int32_t)(pwmPeriod*(red->valueint/255.0f));

        cJSON *green = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "green");
        if (green)
            receiveArray[1] = (int32_t)(pwmPeriod*(green->valueint/255.0f));

        cJSON *blue = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "blue");
        if (blue)
            receiveArray[2] = (int32_t)(pwmPeriod*(blue->valueint/255.0f));

        cJSON *bpm = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "bpm");
        if (bpm)
            receiveArray[3] = (int32_t)((60.0f / bpm->valueint) * 1000000 / 2);

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
        }

        cJSON *lcdScrollString = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "lcdScrollString");
        if (lcdScrollString)
        {
            /* Send message to scroll string on LCD */
            msg.cmd = LcdCmd_SCROLL_STRING;
            msg.buffer = (void *)(lcdScrollString->valuestring);
            rc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
            if (rc == -1) {
                // Failed to send to message queue
            }
            rc = sem_post(&semLCD);
            if (rc == -1) {
                while (1);
            }
        }

        cJSON *lcdShowString = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "lcdShowString");
        if (lcdShowString)
        {
            /* Send message to display static string on LCD */
            msg.cmd = LcdCmd_SHOW_STRING;
            msg.buffer = (void *)(lcdShowString->valuestring);
            rc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
            if (rc == -1) {
                // Failed to send to message queue
            }
            rc = sem_post(&semLCD);
            if (rc == -1) {
                while (1);
            }
        }

        cJSON *lcdUpdateMemory = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "lcdUpdateMemory");
        if (cJSON_IsArray(lcdUpdateMemory) && cJSON_GetArraySize(lcdUpdateMemory) == 3)
        {
            int buffer[3];
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
        }

        cJSON *lcdUpdateAnimationMemory = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "lcdUpdateAnimationMemory");
        if (cJSON_IsArray(lcdUpdateAnimationMemory) && cJSON_GetArraySize(lcdUpdateAnimationMemory) == 3)
        {
            int buffer[3];
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
        }

        cJSON *lcdStartAnimation = cJSON_GetObjectItemCaseSensitive(rxJSONObject, "lcdStartAnimation");
        if (lcdStartAnimation)
        {
            int startAnimation;
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
        }

        /* Delete JSON object to free up allocated memory */
        cJSON_Delete(rxJSONObject);
    }
}
