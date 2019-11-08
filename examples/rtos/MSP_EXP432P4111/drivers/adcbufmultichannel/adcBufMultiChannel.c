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
 *  ======== adcbufMultiChannel.c ========
 */
/* DriverLib Includes */
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <semaphore.h>

/* Driver Header files */
#include <ti/drivers/ADCBuf.h>

/* Display Header files */
#include <ti/display/Display.h>
#include <ti/display/DisplayUart.h>

/* Board Header file */
#include "ti_drivers_config.h"

#define ADCBUFFERSIZE    (5)

uint16_t sampleBufferOne[ADCBUFFERSIZE];
uint16_t sampleBufferTwo[ADCBUFFERSIZE];
uint16_t sampleBufferThree[ADCBUFFERSIZE];
uint16_t sampleBufferFour[ADCBUFFERSIZE];
uint32_t buffersCompletedCounter = 0;
uint16_t outputBuffer[ADCBUFFERSIZE];

/* Display Driver Handle */
Display_Handle displayHandle;

/* ADCBuf semaphore */
sem_t adcbufSem;

/*
 * This function is called whenever a buffer is full.
 * The content of the buffer is then converted into human-readable format and
 * sent to the PC via UART.
 *
 */
void adcBufCallback(ADCBuf_Handle handle, ADCBuf_Conversion *conversion,
    void *completedADCBuffer, uint32_t completedChannel) {
    uint_fast16_t i;
    uint16_t *completedBuffer = (uint16_t *) completedADCBuffer;
 
    for (i = 0; i < ADCBUFFERSIZE; i++) {
        outputBuffer[i] = completedBuffer[i];
    }
 
    /* post adcbuf semaphore */
    sem_post(&adcbufSem);
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    Display_Params displayParams;
    ADCBuf_Handle adcBuf;
    ADCBuf_Params adcBufParams;
    ADCBuf_Conversion continuousConversion[2];
    uint_fast16_t i;
    int32_t status;

    /* Call driver init functions */
    ADCBuf_init();
    Display_init();

    /* Configure & open Display driver */
    Display_Params_init(&displayParams);
    displayParams.lineClearMode = DISPLAY_CLEAR_BOTH;
    displayHandle = Display_open(Display_Type_UART, &displayParams);
    if (displayHandle == NULL) {
        Display_printf(displayHandle, 0, 0, "Error creating displayHandle\n");
        while (1);
    }
    
    status = sem_init(&adcbufSem, 0, 0);
    if (status != 0) {
        Display_printf(displayHandle, 0, 0, "Error creating adcbufSem\n");
        while(1);
    }

    Display_printf(displayHandle, 0, 0, "Starting the ADCBuf Multi-Channel example");

    /* Set up an ADCBuf peripheral in ADCBuf_RECURRENCE_MODE_CONTINUOUS */
    ADCBuf_Params_init(&adcBufParams);
    adcBufParams.callbackFxn = adcBufCallback;
    adcBufParams.recurrenceMode = ADCBuf_RECURRENCE_MODE_CONTINUOUS;
    adcBufParams.returnMode = ADCBuf_RETURN_MODE_CALLBACK;
    adcBufParams.samplingFrequency = 200;
    adcBuf = ADCBuf_open(CONFIG_ADCBUF0, &adcBufParams);

    /* Configure the conversion struct for two channels on same sequencer */
    continuousConversion[0].arg = NULL;
    continuousConversion[0].adcChannel = CONFIG_ADCBUF0CHANNEL_0;
    continuousConversion[0].sampleBuffer = sampleBufferOne;
    continuousConversion[0].sampleBufferTwo = sampleBufferTwo;
    continuousConversion[0].samplesRequestedCount = ADCBUFFERSIZE;

    continuousConversion[1].arg = NULL;
    continuousConversion[1].adcChannel = CONFIG_ADCBUF0CHANNEL_1;
    continuousConversion[1].sampleBuffer = sampleBufferThree;
    continuousConversion[1].sampleBufferTwo = sampleBufferFour;
    continuousConversion[1].samplesRequestedCount = ADCBUFFERSIZE;

    if (!adcBuf){
        /* AdcBuf did not open correctly. */
        while(1);
    }

    /* Start converting sequencer 0. */
    if (ADCBuf_convert(adcBuf, continuousConversion, 2) !=
        ADCBuf_STATUS_SUCCESS) {
        /* Did not start conversion process correctly. */
        while(1);
    }

    /*
     * Go to sleep in the foreground thread forever. The data will be collected
     * and transfered in the background thread
     */
    while(1) {
        sem_wait(&adcbufSem);
        
        /*
         * Start with a header message and print current buffer values
         */
        Display_printf(displayHandle, 0, 0, "\r\nBuffer %u finished:",
            (unsigned int)buffersCompletedCounter++);

        for (i = 0; i < ADCBUFFERSIZE; i++) {
            Display_printf(displayHandle, 0, 0,
                "    %u,", outputBuffer[i]);
        }
    }

}
