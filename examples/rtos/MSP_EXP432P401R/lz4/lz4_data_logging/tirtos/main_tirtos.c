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
 *  ======== main_tirtos.c ========
 */
#include <stdint.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>

/* RTOS Header files */
#include <ti/sysbios/BIOS.h>

/* Example/Board Header files */
#include "Board.h"

extern void *sensorThread(void *arg0);
extern void *compressThread(void *arg0);

/* Stack size in bytes */
#define THREADSTACKSIZE    1024

/* Semaphore to control data compression */
sem_t semCompress;

/*
 *  ======== main ========
 */
int main(void)
{
    pthread_t           sensorThread_handler;
    pthread_t           compressThread_handler;
    pthread_attr_t      pAttrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    /* Initialize Semaphore */
    int rc;
    rc = sem_init(&semCompress, 0, 0);
    if (rc == -1) {
        while (1);
    }

    /* Call driver init functions */
    Board_init();

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

    retc |= pthread_attr_setstacksize(&pAttrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    /* Create Sensor Thread with priority = 1 */
    retc = pthread_create(&sensorThread_handler, &pAttrs, sensorThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    /* Create Compress Thread with priority = 2 */
    priParam.sched_priority = 2;
    pthread_attr_setschedparam(&pAttrs, &priParam);
    retc = pthread_create(&compressThread_handler, &pAttrs, compressThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    BIOS_start();

    return (0);
}
