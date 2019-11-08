/* --COPYRIGHT--,BSD
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
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

#include <stdint.h>
#include <stdio.h>
/* For usleep() */
#include <unistd.h>

/* POSIX Header files */
#include <pthread.h>

/* Driver Header files */
#include <ti/drivers/UART.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/power/PowerMSP432.h>

#include <ti/display/Display.h>

/* Driverlib Header files */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "vibrationCapture.h"
#include "commonComms.h"

#include <semaphore.h>

/* Mutex semaphores */
sem_t startComms;
sem_t endComms;

#ifdef BLE_COMMS
sem_t vibrationMutex;

void commsTask(void)
{
    /* Create Mutex for Communication between sensing and Comms Stack */
    sem_init(&startComms, 1, 0);
    sem_init(&endComms, 1, 0);

    /* Create vibration Mutex */
    createResource(&vibrationMutex);
}

int16_t createResource(sem_t *sem)
{
    int16_t retc;

    retc = sem_init(sem, 0, 1);

    return(retc);
}

bool acquireResource(sem_t *sem)
{
    struct timespec timeOut;

    /* 100ms timeout */
    timeOut.tv_nsec = 100000000;

    /* Acquire resource */
    if (sem_timedwait(sem, &timeOut) != 0)
    {
        return false;
    }

    return true;
}

void releaseResource(sem_t *sem)
{
    /* Release resource */
    sem_post(sem);
}

#else
void *commsThread(void *arg0);

/* Communication Task to start the Thread for communicating between sensing and
 * the data transfer method */
void commsTask(void)
{
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    /* Set priority and stack size attributes */
    pthread_attr_init(&attrs);
    priParam.sched_priority = COMMS_TASK_PRIORITY;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    pthread_attr_setschedparam(&attrs, &priParam);

    retc |= pthread_attr_setstacksize(&attrs, COMMS_TASK_STACK_SIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    retc = pthread_create(&thread, &attrs, commsThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }
}

void *commsThread(void *arg0)
{
    int_fast16_t loop;

    Display_Handle display;

    Display_init();

    /* Open the display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        /* Failed to open display driver */
        while (1);
    }

    /* Create Mutex for Communication between sensing and Comms Stack */
    sem_init(&startComms, 1, 0);
    sem_init(&endComms, 1, 0);

    /* Loop for communication stack interaction */
    for(;;)
    {
        /* Wait for the processing to get over */
        sem_wait(&startComms);

        for(loop=0 ; loop<FFTSIZE ; loop++)
        {
            Display_printf(display, 0, 0, "%f, %f, %f,",XAxisEnergyBin.energyValue[loop],
                           YAxisEnergyBin.energyValue[loop],
                           ZAxisEnergyBin.energyValue[loop]);
        }

        /* Tell Sensing stack that we are done with transmitting the data */
        sem_post(&endComms);
    }
}
#endif

