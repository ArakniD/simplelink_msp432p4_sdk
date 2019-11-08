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
 *  ======== lcd_thread.c ========
 */

/* Includes */
#include <callbacks.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/* POSIX Header files */
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/Timer.h>

/* Local Header Files */
#include "callbacks.h"
#include "lcd_thread.h"
#include "uart_thread.h"
#include "LCD_HAL/lcd_hal.h"

/* Global Variables */
pthread_t lcdthread_handler;
sem_t semLCD;
mqd_t  mqLCD;

/*
 *  ======== lcdThread ========
 *  Handles displaying various texts on the segmented LCD
 */
void *lcdThread(void *arg0)
{
    LcdMsg msg;
    int retc;

    LCD_init();
    LCD_scrollString("MSP432P4111 LAUNCHPAD OOB DEMO", 150000);

    while (1) {
        /* Wait for a change to occur */
        retc = sem_wait(&semLCD);
        if (retc == 0) {
            retc = mq_receive(mqLCD, (char *)&msg, sizeof(msg), NULL);
            if (retc != -1) {
                char str[MAX_STR_LENGTH];
                switch (msg.cmd) {
                    case LcdCmd_SHOW_STRING:
                        /* Displays a static string on the LCD */
                        strcpy(str, msg.buffer);
                        LCD_showString(str);
                        break;
                    case LcdCmd_SCROLL_STRING:
                        /* Scrolls a string from right to left on the LCD */
                        scrolling = true;
                        strcpy(str, msg.buffer);
                        LCD_scrollString(str, 150000);
                        break;
                    case LcdCmd_SHOW_TEMPERATURE:
                        /* Shows temperature in degree Celsius on LCD */
                        LCD_showTemperatureC(*((float *)(msg.buffer)));
                        break;
                    case LcdCmd_UPDATE_MEMORY:
                        /* Updates LCD main memory register */
                        LCD_updateMemory(((int *)(msg.buffer))[0], ((int *)(msg.buffer))[1], ((int *)(msg.buffer))[2]);
                        break;
                    case LcdCmd_UPDATE_ANIMATION_MEMORY:
                        /* Updates LCD animation memory register */
                        LCD_updateAnimationMemory(((int *)(msg.buffer))[0], ((int *)(msg.buffer))[1], ((int *)(msg.buffer))[2]);
                        break;
                    case LcdCmd_START_ANIMATION:
                        /* Start/stop LCD animation */
                        if (*(int *)(msg.buffer))
                            LCD_startAnimation();
                        else
                            LCD_stopAnimation();
                        break;
                    case LcdCmd_CLEAR_MEMORY:
                        /* Clears LCD main, blink, & animation memory registers */
                        LCD_clear();
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
