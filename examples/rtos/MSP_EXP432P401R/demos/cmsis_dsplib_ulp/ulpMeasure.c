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
 *  ======== ulpMeasure.c ========
 *  This file contains the following:
 *  
 *  ADCBuf callback
 *      The ADCBuf callback function is called everytime either the 'ping' or 
 *      'pong' of the DMA is completed.  Within the callback the number of 
 *      callbacks is monitored and passed along with the address of the most
 *      recent data array through the message queue to the calculation thread
 *      which computes the RMS and frequency of the signal across multiple ADC
 *      acquistions/DMA transfers.  The idea is that the acquisition size can
 *      exceed the 1024 DMA size limit by managing the transfers and performing
 *      cumulative computations until the desired data set size is acquired.
 *      The usage of the 'no block' message queue flag ensures that the queue 
 *      does not pend within the ISR.
 * 
 *  measurementThread
 *      The measurement thread calls the initialization of the RTC as well as
 *      initializes the ADC/DMA and the diplay driver.  Two semaphores are used
 *      to control the flow of the program: startMeasure and completeMeasure.  
 *      The startMeasure is posted from the RTC callback (hwi) and starts the 
 *      measurement, the next measurement will not start until the RTC posts
 *      again (1 second interval).  The measurementComplete is posted from 
 *      computationThread and indicates that both the ADC/DMA as well as the 
 *      computation is complete and that the data can be presented to the 
 *      display.  The ADC and the display are both closed and the power 
 *      constraint is released to allow the device to enter deepsleep0 (LPM3)
 *      while waiting for the next RTC hwi.
 *      
 *  RTC initialization and callback
 *      The RTC initialization sets the deepsleep power policy to allow LPM3.
 *      The RTC time interval is set to 1 second so that once a second the 
 *      data acquisition and computation are performed.  Once the measurement
 *      and computation are complete the device is allowed to return to LPM3
 *      to conserve power.
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
#include "Board.h"
#include "ulpMeasure.h"

/*
 * Pend upon startMeasure which will be posted from RTC.  Post to
 * computeMeasure when the data collection is completed.
 */
sem_t startMeasure;
sem_t measureComplete;

/* Power Policy Object*/
Power_NotifyObj notifyObj;

extern Display_Handle display;
extern pthread_mutex_t resultMutex;

extern volatile float rmsCalculation;
extern volatile float rmsFrequency;

q15_t pingSampleBuffer[ADCDMA_BUFFERSIZE];
q15_t pongSampleBuffer[ADCDMA_BUFFERSIZE];
uint32_t ADCDMA_SampleCount;

/*******************************************************************************
 *                         LOCAL FUNCTIONS
 ******************************************************************************/
static void RTC_C_Init(void);
static void RTC_C_IRQHandler(uintptr_t arg);

/*
 * This function is called whenever a buffer is full.
 * The content of the buffer is then converted into human-readable format and
 * sent to the PC via UART.
 *
 */
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
     * The reception takes place in computation.c
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
 *  ======== measurementThread ========
 */
void *measurementThread(void *arg0)
{
    ADCBuf_Handle adcBuf;
    ADCBuf_Params adcBufParams;
    ADCBuf_Conversion continuousConversion;
    float32_t rmsCalculationTemp;
    float32_t rmsFrequencyTemp;

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
    continuousConversion.adcChannel = Board_ADCBUF0CHANNEL3;
    continuousConversion.sampleBuffer = pingSampleBuffer;
    continuousConversion.sampleBufferTwo = pongSampleBuffer;
    continuousConversion.samplesRequestedCount = ADCDMA_BUFFERSIZE;

    RTC_C_Init();

    while(1)
    {
        sem_wait(&startMeasure);
        ADCDMA_SampleCount = 0;
        /*
         * re-open and close ADC driver with each aquisition to reinitialize
         * the DMA ping-pong
         */
        adcBuf = ADCBuf_open(Board_ADCBUF0, &adcBufParams);
        if (!adcBuf){
            /* AdcBuf did not open correctly. */
            while(1);
        }
        ADCBuf_control(adcBuf, ADCBufMSP432_CMD_ENTER_ADC_ULTRA_LOW_POWER_MODE, 0);
        /*
         * this function enables the reference burst feature.  This means that the
         * reference is only available during the S&H time and that the measurement
         * time is 'increased' by the time required for the reference to be ready.
         *
         */
        MAP_ADC14_setResolution(ADC_12BIT);
        MAP_ADC14_setResultFormat(ADC_SIGNED_BINARY);
        MAP_ADC14_disableReferenceBurst();
        /* Start converting. */
        if (ADCBuf_convert(adcBuf, &continuousConversion, 1) !=
            ADCBuf_STATUS_SUCCESS)
        {
            /* Did not start conversion process correctly. */
            while(1);
        }
        sem_wait(&measureComplete);
        ADCBuf_close(adcBuf);

        display = Display_open(Display_Type_UART, NULL);
        if (display == NULL) {
            /* Failed to open display driver */
            while(1);
        }

        pthread_mutex_lock(&resultMutex);
        
        rmsCalculationTemp = rmsCalculation;
        rmsFrequencyTemp = rmsFrequency;
        
        Display_printf(display, 0, 0, "Frequency: %f\n RMS: %f\n", rmsFrequencyTemp, rmsCalculationTemp);
        pthread_mutex_unlock(&resultMutex);
        Display_close(display);
        Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
    }
}

/* Callback function for Power Policy Manager to perform specific tasks on
 * LMP3 Entry and Wake. During LPM3 Entry, the GPIO for AFE Power Enable and
 * Sensor Enable are made low to reduce current consumption. Also since
 * enableparking is disabled, the unused IO's are configured to output low
 * state so that current consumption is reduced. During LPM3 exit the GPIO for
 * AFE Power Enable and Sensor Enable are made High.
 * */
unsigned int notifyDeepSleepFxn(unsigned int eventType, unsigned int eventArg,
 unsigned int clientArg)
{
    if(PowerMSP432_ENTERING_DEEPSLEEP == eventType)
    {

    #ifdef __MSP_EXP432P4111_H
        /* Set P5.3 as analog input (connected to Temp Sensor on MSP_EXP432P4111 LaunchPad) */
        MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN3, GPIO_TERTIARY_MODULE_FUNCTION);
    #endif

        /* Turn off PSS high-side supervisors to consume lower power after deep sleep */
        MAP_PSS_disableHighSide();
    }
    return 0;
}

void RTC_C_Init(void)
{
    Hwi_Handle myHwi;
    Hwi_Params hwiParams;

    /* Configuring pins for peripheral/crystal LFXT */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Starting LFXT in non-bypass mode without a timeout */
    MAP_CS_startLFXT(false);
    MAP_CS_initClockSignal(CS_BCLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

    Power_setPolicy((Power_PolicyFxn)PowerMSP432_deepSleepPolicy);

    /*
     * allowing down to deepsleep_0 = LPM3
     */
    Power_setConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_1);

    /* Register for entering shutdown notifications */
    Power_registerNotify(&notifyObj, PowerMSP432_ENTERING_DEEPSLEEP | PowerMSP432_AWAKE_DEEPSLEEP,
        (Power_NotifyFxn)notifyDeepSleepFxn, 0);

    /* RTC, 1s period */
    MAP_RTC_C_setPrescaleValue(RTC_C_PRESCALE_0,0x00);
    MAP_RTC_C_setPrescaleValue(RTC_C_PRESCALE_1,0x00);
    RTC_C->PS1CTL = RTC_C_PS1CTL_RT1IP_7;
    MAP_RTC_C_enableInterrupt(RTC_C_PRESCALE_TIMER1_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_RTC_C);

    /* Create RTOS hwi */
    Hwi_Params_init(&hwiParams);
    hwiParams.arg = 5;
    hwiParams.priority = 0x40;
    myHwi = Hwi_create(INT_RTC_C, RTC_C_IRQHandler, &hwiParams, NULL);

    if (myHwi == NULL)
    {
        while (1);
    }

    /* Start the RTC */
    MAP_RTC_C_startClock();
}

void RTC_C_IRQHandler(uintptr_t arg)
{
    uint32_t status;

    status = MAP_RTC_C_getEnabledInterruptStatus();
    MAP_RTC_C_clearInterruptFlag(status);

    /*
     * reset scalers for the next second
     */
    if(status & RTC_C_PRESCALE_TIMER1_INTERRUPT)
    {
        MAP_RTC_C_holdClock();
        MAP_RTC_C_setPrescaleValue(RTC_C_PRESCALE_0,0x00);
        MAP_RTC_C_setPrescaleValue(RTC_C_PRESCALE_1,0x00);
        /* Re-start RTC Clock */
        MAP_RTC_C_startClock();
        /* Post to the Task */
        sem_post(&startMeasure);
    }
}
