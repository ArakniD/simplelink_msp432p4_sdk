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
 *  ======== computation.c ========
 *  This file contains the following:
 * 
 *  computationThread
 *      This thread 'pends' upon the message queue receiving both the
 *      acquisition count and the acquisition address (either the ping or
 *      pong buffer).  Once both are received the thread calls finZeroCrossings
 *      and computes the sum of the squares of the buffer.  Once the last 
 *      acquistion is made and computed the total RMS calculation is made the
 *      calculateFrequency function is called.  After this the thread posts to 
 *      themeasurementComplete semaphore and then again pends for the message 
 *      queue.
 *
 *  findZeroCrossings
 *      This function simply goes through the acquisition array and looks for
 *      zero crossings and then records the index of the crossing.  
 *  calculateFrequency
 *      This function takes the index crossings and based upon the sample
 *      frequency calculates the frequency of the sin wave input.
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

#include "ulpMeasure.h"

/* Global Variables */
extern pthread_mutex_t resultMutex;
extern sem_t measureComplete;

q15_t totalRootMean_q15;

q63_t sumSquares_q63[NUMBER_OF_ADC_BUFFERS];

q63_t totalSumSquares_q63;

uint8_t direction;
uint8_t zcIndex;
uint16_t zeroCrossArray[NUMBER_OF_ZEROCROSSINGS];
float32_t rmsCalculation;
float32_t rmsFrequency;

const float32_t adcReferenceVoltage = ADC_REFERENCE_VOLTAGE;

/* Static Function Declarations */
static void findZeroCrossings(q15_t *, uint32_t);
static void calculateFrequency(void);

/*
 *  ======== copmutationThread ========
 *
 */
void *computationThread(void *arg0)
{
    mqd_t *mqdes = arg0;
    int msg;
    volatile uint16_t ii;
    uint32_t bufferCount;
    uint32_t blockSize;
    float32_t tempRMS_f32;
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
        if((uint32_t)bufferPtr > NUMBER_OF_ADC_BUFFERS)
        {
            if(bufferCount == 0)
            {
                zcIndex = 0;
                /*
                 * ZERO_CROSS will be a function of the ADC output type.
                 * Assuming that the measurement is differential and that
                 * DF = 0, then '0' with represent the zero-crossing.
                 */
                if(bufferPtr[0] > ZERO_CROSS)
                {
                    direction = DOWN;
                }
                else
                {
                    direction = UP;
                }
            }
            findZeroCrossings(bufferPtr,bufferCount);
            blockSize = ADCDMA_BUFFERSIZE;
            /*
             * result is in a 34.40 format
             */
            arm_power_q15(&bufferPtr[0],blockSize,&sumSquares_q63[bufferCount]);

            if(bufferCount == NUMBER_OF_ADC_BUFFERS-1)
            {
                totalSumSquares_q63 = 0;
                for(ii=0;ii < NUMBER_OF_ADC_BUFFERS; ii++)
                {
                    totalSumSquares_q63 += sumSquares_q63[ii];
                }
                /*
                 * Convert q63_t to q15_t
                 * truncate 15 and saturate to 1.15
                 */
                blockSize = blockSize*NUMBER_OF_ADC_BUFFERS;
                arm_sqrt_q15(__SSAT((totalSumSquares_q63 / (q63_t)blockSize) >> 15, 16), &totalRootMean_q15);
                /*
                 * Update data
                 */
                pthread_mutex_lock(&resultMutex);
                if(zcIndex >= NUMBER_OF_ZEROCROSSINGS-1)
                {
                    calculateFrequency();
                    arm_q15_to_float(&totalRootMean_q15,&tempRMS_f32,1);
                    arm_scale_f32(&tempRMS_f32,(float32_t)adcReferenceVoltage,&rmsCalculation,1);
                }
                else
                {
                    rmsCalculation = 0.0;
                    rmsFrequency = 0.0;
                }
                pthread_mutex_unlock(&resultMutex);
                sem_post(&measureComplete);
            }
        }
    }
}

static void findZeroCrossings(q15_t * sampleBuffer, uint32_t bufCnt)
{
    volatile uint16_t ii;

    /*
     * find zero-crossings
     * Expecting 0x8000 to 0x7FFC range.  Where 0x8000 is negative full scale.
     */
    for(ii=0;ii<ADCDMA_BUFFERSIZE;ii++)
    {
        if(zcIndex < NUMBER_OF_ZEROCROSSINGS)
        {
            if(direction == DOWN)
            {
                if(sampleBuffer[ii] < ZERO_CROSS &&
                        sampleBuffer[ii+1] < ZERO_CROSS)
                {
                    zeroCrossArray[zcIndex] = ii + bufCnt*ADCDMA_BUFFERSIZE;
                    direction = UP;
                    zcIndex++;
                }
            }
            else
            {
                if(sampleBuffer[ii] > ZERO_CROSS &&
                        sampleBuffer[ii+1] > ZERO_CROSS)
                {
                    zeroCrossArray[zcIndex] = ii + bufCnt*ADCDMA_BUFFERSIZE;
                    direction = DOWN;
                    zcIndex++;
                }
            }
        }
    }
}

/*
 * calculateFrequency
 * Calculation based upon the indecies of the zero crossings and the sampling rate.
 */
static void calculateFrequency(void)
{
    uint16_t sampleSize;
    /*
     * Determine is one whole cycle is captured (three zero crossings) or if
     * only the half cycle is captured.
     */
    if(zcIndex == NUMBER_OF_ZEROCROSSINGS)
    {
        sampleSize = zeroCrossArray[2] - zeroCrossArray[0];
    }
    else
    {
        //Only captured two zero crossings
        sampleSize = zeroCrossArray[1] - zeroCrossArray[0];
        if(sampleSize < ADCDMA_BUFFERSIZE)
        {
            sampleSize = sampleSize*2;
        }
        else
        {
            /*
             *  Bad result
             */
            sampleSize = 0;
            rmsCalculation = 0;
        }
    }
    rmsFrequency = ((float32_t)SAMPLE_FREQEUNCY)/((float32_t)sampleSize);
}

