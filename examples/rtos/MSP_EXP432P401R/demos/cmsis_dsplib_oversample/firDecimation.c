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
 *  ======== firDecimation.c ========
 *  This file contains the following:
 * 
 *  computationThread
 *      This thread 'pends' upon the message queue receiving both the
 *      acquisition count and the acquisition address (either the ping or
 *      pong buffer).  Once both are received the thread calls the decimation
 *      filter.  Once the last acquistion is made and computed the thread posts
 *      to the measureComplete semaphore and then again pends for the message
 *      queue.
 *
 */

/* Includes */
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <arm_math.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>
#include <mqueue.h>

#include <ti/drivers/GPIO.h>

/* Driverlib Header files */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "overSampleADC.h"

#define START_OF_SRAM_SPACE         0x20000000

/* Global Variables */
extern pthread_mutex_t resultMutex;
extern sem_t measureComplete;

const float32_t adcReferenceVoltage = ADC_REFERENCE_VOLTAGE;

/*
 * FIR Filter for decimation filter
 */
const q15_t filterCoeffs_q15[NUM_OF_TAPS] =
{
/*
 * fir1(31,1/32)
 * q15_t
 */
    0x006C, 0x0080, 0x00B2, 0x0102, 0x0172, 0x01FF,
    0x02A7, 0x0362, 0x0429, 0x04F4, 0x05BA, 0x0670,
    0x070F, 0x078E, 0x07E6, 0x0814, 0x0814, 0x07E6,
    0x078E, 0x070F, 0x0670, 0x05BA, 0x04F4, 0x0429,
    0x0362, 0x02A7, 0x01FF, 0x0172, 0x0102, 0x00B2,
    0x0080, 0x006C
};

arm_fir_decimate_instance_q15 S;
q15_t pState[NUM_OF_TAPS+BLOCK_SIZE-1];   // 4.1Kbytes

/* Static Function Declarations */
q15_t decimatedOutput_q15[ARRAY_LENGTH];  // 24Kbytes

/*
 *  ======== copmutationThread ========
 *
 */
void *computationThread(void *arg0)
{
    mqd_t *mqdes = arg0;
    int msg;
    uint32_t bufferCount;
    q15_t * bufferPtr;

    while(1)
    {
        /*
         * Looking for two messages in the mail box.  The first
         * indicates whether this is the first half of the data
         * and the second indicates the address of the data.
         */
        mq_receive(*mqdes, (char *)&msg, sizeof(msg), NULL);
        bufferCount = (uint32_t)msg;
        mq_receive(*mqdes, (char *)&msg, sizeof(msg), NULL);
        bufferPtr =(q15_t *)msg;
        /*
         * look for case where the second message was not received,the
         * bufferPtr should be pointing to the SRAM space  > 0x2000_0000
         */
        if((uint32_t)bufferPtr > START_OF_SRAM_SPACE)
        {
            pthread_mutex_lock(&resultMutex);
            arm_fir_decimate_q15(&S,bufferPtr,&decimatedOutput_q15[bufferCount*DECIMATED_BlOCK_LENGTH],BLOCK_SIZE);
            pthread_mutex_unlock(&resultMutex);
            if(bufferCount == NUMBER_OF_ADC_BUFFERS-1)
            {
                sem_post(&measureComplete);
            }
        }
    }
}
