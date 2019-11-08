/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 *  ======== main_tirtos.c ========
 */
/* Includes */
#include <stdint.h>
#include <stdbool.h>

/* POSIX Header Files */
#include <pthread.h>

/* RTOS Header Files */
#include <ti/sysbios/BIOS.h>

/* Driver Header Files */
#include <ti/drivers/GPIO.h>

/* Example/Board Header Files */
#include <ti/drivers/Board.h>

/* Local Header Files */
#include "led_thread.h"
#include "lcd_thread.h"
#include "temperature_thread.h"
#include "uart_thread.h"

/* Stack size in bytes */
#define THREADSTACKSIZE    1024

/*
 *  ======== main ========
 */
int main(void)
{
    pthread_attr_t      pAttrs;
    struct sched_param  priParam;
    struct mq_attr      attr;
    int                 retc;
    int                 detachState;

    /* Call driver init functions */
    Board_init();

    /*
     *  Message Queue to send messages to LED thread.
     *  It is non-blocking to allow it to be called by ISRs (e.g. GPIO and
     *  Timer callbacks). Note: senders need to use semLED to actually
     *  wake up the LED thread.
     */
    attr.mq_maxmsg = LED_MSG_NUM;
    attr.mq_msgsize = LED_MSG_SIZE;
    attr.mq_flags = 0;
    mqLED = mq_open("led", O_RDWR | O_CREAT | O_NONBLOCK, 0664, &attr);
    if (mqLED == (mqd_t)-1) {
        /* mq_open() failed */
        while (1);
    }

    /*
     *  Message Queue to send messages to LCD thread.
     *  It is non-blocking to allow it to be called by ISRs (e.g. GPIO and
     *  Timer callbacks). Note: senders need to use semLCD to actually
     *  wake up the LCD thread.
     */
    attr.mq_maxmsg = LCD_MSG_NUM;
    attr.mq_msgsize = LCD_MSG_SIZE;
    attr.mq_flags = 0;
    mqLCD = mq_open("lcd", O_RDWR | O_CREAT | O_NONBLOCK, 0664, &attr);
    if (mqLCD == (mqd_t)-1) {
        /* mq_open() failed */
        while (1);
    }

    /* Semaphore to notify LED thread that msg is present. */
    retc = sem_init(&semLED, 0, 0);
    if (retc == -1) {
        while (1);
    }

    retc = sem_init(&semLCD, 0, 0);
    if (retc == -1) {
        while (1);
    }

    retc = sem_init(&semTemp, 0, 0);
    if (retc == -1) {
        while (1);
    }

    /* Set priority and stack size attributes */
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 1;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc |= pthread_attr_setstacksize(&pAttrs, 2048);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    /* Create LED Thread with priority = 4 */
    priParam.sched_priority = 4;
    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc = pthread_create(&ledthread_handler, &pAttrs, ledThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    retc |= pthread_attr_setstacksize(&pAttrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    /* Create LCD Thread with priority = 3 */
    priParam.sched_priority = 3;
    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc = pthread_create(&lcdthread_handler, &pAttrs, lcdThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    /* Create UART Thread with priority = 2 */
    priParam.sched_priority = 2;
    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc = pthread_create(&uartthread_handler, &pAttrs, uartThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    /* Create TempSensor Thread with priority = 1 */
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc = pthread_create(&temperatureThread_handler, &pAttrs, temperatureThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    BIOS_start();

    return (0);
}
