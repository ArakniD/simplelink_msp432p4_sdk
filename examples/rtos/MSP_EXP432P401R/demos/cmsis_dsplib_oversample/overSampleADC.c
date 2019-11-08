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
 *  ======== overSampleADC.c ========
 *  This file contains the following:
 *  
 *  ADCBuf callback
 *      The ADCBuf callback function is called everytime either the 'ping' or 
 *      'pong' of the DMA is completed.  Within the callback the number of 
 *      callbacks is monitored and passed along with the address of the most
 *      recent data array through the message queue to the calculation thread.
 *      The idea is that the acquisition size can exceed the 1024 DMA size
 *      limit by managing the transfers and performing cumulative computations
 *      until the desired data set size is acquired. The usage of the
 *      'no block' message queue flag ensures that the queue does not pend
 *      within the ISR.
 * 
 *  measurementThread
 *      The measurement thread initializes the GPIO, ADC/DMA and the diplay
 *      driver.  Two semaphores are used to control the flow of the program:
 *      startMeasure and completeMeasure.
 *      The startMeasure is posted from the GPIO callback (hwi) and starts the
 *      measurement, the next measurement will not start until the GPIO posts
 *      again.  The measurementComplete is posted from computationThread and
 *      indicates that both the ADC/DMA as well as the computation is complete
 *      and that the data can be presented to the display.  The ADC and the
 *      display are both closed and the power constraint is released to allow
 *      the device to enter sleep (LPM0)while waiting for the next GPIO hwi.
 *      
 *
 */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* CMSIS Header File */
#include <arm_math.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>
#include <mqueue.h>
#include <errno.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>
#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/adcbuf/ADCBufMSP432.h>

/* Driverlib Header files */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <ti/sysbios/hal/Hwi.h>
#include <ti/display/Display.h>

/* Board Header file */
#include "ti_drivers_config.h"

#include "overSampleADC.h"

/*
 * Pend upon startMeasure which will be posted from GPIO callback.  Post to
 * measureComplete when the data collection is completed.
 */
sem_t startMeasure;
sem_t measureComplete;

extern Display_Handle display;
extern pthread_mutex_t resultMutex;

extern const q15_t filterCoeffs_q15[NUM_OF_TAPS];
extern arm_fir_decimate_instance_q15 S;
extern q15_t pState[NUM_OF_TAPS+BLOCK_SIZE-1];   // 4.1Kbytes

extern q15_t decimatedOutput_q15[ARRAY_LENGTH];  // 8Kbytes

q15_t pingSampleBuffer[ADCDMA_BUFFERSIZE];
q15_t pongSampleBuffer[ADCDMA_BUFFERSIZE];
uint32_t ADCDMA_SampleCount;
bool bMeasureActive;

void adcBufCallback(ADCBuf_Handle handle, ADCBuf_Conversion *conversion,
    void *completedADCBuffer, uint32_t completedChannel)
{
    ADCBufMSP432_Object  *object = handle->object;
    mqd_t *mqdes = (mqd_t *)conversion->arg;
    int     msg;
    int retc;

    /*
     * need to use the mailbox to tell the computeMeasure which group is
     * being supplied, the first or the second.
     *
     * The reception takes place in firDecimation.c
     */

    msg = (int)ADCDMA_SampleCount;
    retc = mq_send(*mqdes, (char*)&msg, sizeof(msg),0);
    if(retc != EAGAIN)
    {
        msg = (int)completedADCBuffer;
        retc = mq_send(*mqdes, (char*)&msg, sizeof(msg),0);
        if(retc != EAGAIN)
        {
            ADCDMA_SampleCount++;
        }
    }
    if(ADCDMA_SampleCount == NUMBER_OF_ADC_BUFFERS)
    {
        MAP_Timer_A_stopTimer(object->timerAddr);
    }
}

/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON0.
 */
void gpioButtonFxn0(uint_least8_t index)
{
    if(bMeasureActive == false)
    {
        /*
         * Setup decimation filter
         */
        arm_fir_decimate_init_q15(&S,NUM_OF_TAPS,DECIMATION_FACTOR,(q15_t *)filterCoeffs_q15,pState,BLOCK_SIZE);
        sem_post(&startMeasure);
        bMeasureActive = true;
    }
}

/*
 *  ======== measurementThread ========
 */
void *measurementThread(void *arg0)
{
    volatile uint16_t ii;
    ADCBuf_Handle adcBuf;
    ADCBuf_Params adcBufParams;
    ADCBuf_Conversion continuousConversion;

    int retc;

    retc = sem_init(&startMeasure, 0, 0);
    if (retc == -1) {
        while (1);
    }
    retc = sem_init(&measureComplete, 0, 0);
    if (retc == -1) {
        while (1);
    }

    /* Set up an ADCBuf peripheral in ADCBuf_RECURRENCE_MODE_CONTINUOUS */
    ADCBuf_Params_init(&adcBufParams);
    adcBufParams.callbackFxn = adcBufCallback;
    adcBufParams.recurrenceMode = ADCBuf_RECURRENCE_MODE_CONTINUOUS;
    adcBufParams.returnMode = ADCBuf_RETURN_MODE_CALLBACK;
    adcBufParams.samplingFrequency = SAMPLE_FREQEUNCY;

    /* Configure the conversion struct */
    continuousConversion.arg = arg0;
    continuousConversion.adcChannel = CONFIG_ADCBUF0CHANNEL_0;
    continuousConversion.sampleBuffer = pingSampleBuffer;
    continuousConversion.sampleBufferTwo = pongSampleBuffer;
    continuousConversion.samplesRequestedCount = ADCDMA_BUFFERSIZE;

    GPIO_setConfig(CONFIG_GPIO_LED1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    /* install Button callback */
    GPIO_setCallback(CONFIG_GPIO_BUTTON0, gpioButtonFxn0);

    /* Enable interrupts */
    GPIO_enableInt(CONFIG_GPIO_BUTTON0);
    adcBuf = ADCBuf_open(CONFIG_ADCBUF0, &adcBufParams);
    if (!adcBuf){
        /* AdcBuf did not open correctly. */
        while(1);
    }
    MAP_ADC14_setResultFormat(ADC_SIGNED_BINARY);

    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        /* Failed to open display driver */
        while(1);
    }

    while(1)
    {
        bMeasureActive = false;
        sem_wait(&startMeasure);
        ADCDMA_SampleCount = 0;
        /* Start converting. */
        if (ADCBuf_convert(adcBuf, &continuousConversion, 1) !=
            ADCBuf_STATUS_SUCCESS)
        {
            /* Did not start conversion process correctly. */
            while(1);
        }
        sem_wait(&measureComplete);
        Display_printf(display, 0, 0, "Decimated Array:\n");
        pthread_mutex_lock(&resultMutex);
        for(ii=0;ii<ARRAY_LENGTH;ii++)
        {
            Display_printf(display, 0, 0, "%d\n",decimatedOutput_q15[ii]);
        }
        pthread_mutex_unlock(&resultMutex);
    }
}
