/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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
#include <mqueue.h>
#include <semaphore.h>

/* RTOS header files */
#include <ti/sysbios/BIOS.h>

/* Driver Header files */
#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/GPIO.h>

/* Display Header file */
#include <ti/display/Display.h>

/* Driver configuration */
#include <ti/drivers/Board.h>

/* Stack size in bytes */
#define THREADSTACKSIZE    1024

#define MSG_SIZE    sizeof(int)
#define MSG_NUM     4

mqd_t mqdesSend;
mqd_t mqdesReceive;

/* Mutex to protect the reading/writing of the temperature variables */
pthread_mutex_t resultMutex;

extern void *measurementThread(void *arg0);
extern void *computationThread(void *arg0);

Display_Handle display;

/*
 *  ======== main ========
 */
int main(void)
{
    pthread_t           thread,freqRmsCalc;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    struct mq_attr      mqAttrs;
    int                 retc;
    int                 detachState;

    /* Call driver init functions */
    Board_init();
    GPIO_init();
    ADCBuf_init();
    Display_init();

    /*
     * Create message queue freqRmsCalc.  This first instance is blocking
     * by default, so that the computation thread will pend until a message
     * is written to the queue.
     */
    mqAttrs.mq_maxmsg = MSG_NUM;
    mqAttrs.mq_msgsize = MSG_SIZE;
    mqAttrs.mq_flags = 0;
    mqdesReceive = mq_open("freqRmsCalc", O_RDWR | O_CREAT,
                    0664, &mqAttrs);
    if (mqdesReceive == (mqd_t)-1) {
      /* mq_open() failed */
      while (1);
    }

    /* Set priority and stack size attributes */
    pthread_attr_init(&attrs);
    priParam.sched_priority = 1;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    pthread_attr_setschedparam(&attrs, &priParam);

    /*
     * The computation thread is associated with the blocking message queue instance.
     */
    retc = pthread_create(&freqRmsCalc, &attrs, computationThread, (void*)&mqdesReceive);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    /*
     *  A seperate instance of the queue is created which is non-blocking so
     * that the message send, which takes place in the ISR, will not wait in
     * the ISR until the queue is available to send.  It will return an error
     * if the message was not sent.
     */ 
    mqdesSend = mq_open ("freqRmsCalc",
                         O_RDWR | O_CREAT | O_NONBLOCK ,
                         0664, &mqAttrs);

    if (mqdesSend == (mqd_t)-1)
    {
      /* mq_open() failed */
      while (1);
    }

    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    retc = pthread_create(&thread, &attrs, measurementThread, (void*)&mqdesSend);
    if (retc != 0) {
        /*
         * pthread_create() failed
         */
        while (1);
    }

    /* 
     * Create a mutex that will protect measurement variables which are used by
     * both threads.  This feature of POSIX is not required for this example
     * but provided for demonstration purposes.
     */
    retc = pthread_mutex_init(&resultMutex, NULL);
    if (retc != 0) {
        /* pthread_mutex_init() failed */
        while (1);
    }

    /* Start the TI-RTOS scheduler */
    BIOS_start();

    return (0);
}
