/*
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
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* POSIX Header files */
#include <pthread.h>

/* Driver Header files */
#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/adcbuf/ADCBufMSP432.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/power/PowerMSP432.h>
#ifdef RTC_WAKEUP
#include <ti/drivers/dpl/HwiP.h>
#endif

/* Driverlib Header files */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Board Header file */
#include "ti_drivers_config.h"
#include "vibrationCapture.h"
#include "commonComms.h"
#ifdef BLE_COMMS
#include "motor_condition_monitor.h"
#endif

#include <semaphore.h>
#ifdef BLE_COMMS
#include <mqueue.h>
#endif

/* DSP Include */
#include "arm_math.h"
#include "arm_const_structs.h"

#ifdef BLE_COMMS
extern mqd_t apQueueSend;
#endif

#ifdef RTC_WAKEUP
void RTC_C_IRQHandler(void);

/* -------------------------------------------------------------------
 * Set up of the current RTC time when the system is first powered up.
 * This is set for April 28th 2017 11:30:00 PM
 * ------------------------------------------------------------------- */
const RTC_C_Calendar currentTime =
{
        00,
        30,
        23,
        06,
        28,
        04,
        2017
};
#endif

/* Callback used for timer in one-shot mode. */
void timerCallback(Timer_Handle myHandle);

void *vibrationThread(void *arg0);

/* Energy Bin buffers*/
Energy_Bin XAxisEnergyBin;
Energy_Bin YAxisEnergyBin;
Energy_Bin ZAxisEnergyBin;

/* ADC Sample Buffers for X, Y and Z axis */
uint16_t sampleBufferX[ADCBUFFERSIZE];
uint16_t sampleBufferY[ADCBUFFERSIZE];
uint16_t sampleBufferZ[ADCBUFFERSIZE];

/* Floating Point Buffer for getting the data */
static float32_t setfloatInput[ADCBUFFERSIZE*2];

/* Floating Point Buffer where FFT output is kept */
float32_t *fftOutput;

/* Variable to count the number of averaging iterations */
uint8_t avgLoop;

/* Mutex for end of conversion and FFT calculation */
sem_t adcConvMutex;

/* Mutex for indicating wakeup of device */
sem_t devWakeMutex;

/* Mutex for power up of AFE time delay */
sem_t afeWakeMutex;

/* Mutex to talk to the Communication stack */
extern sem_t startComms;
extern sem_t endComms;

/* Power Policy Object*/
Power_NotifyObj notifyObj;

/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t ifftFlag     = 0;
uint32_t doBitReverse = 1;

/*
 * This function converts the data into a floating point representation of the
 * data read from the ADC channel
 */
int_fast16_t convertAdjustedDataToFloat(uint32_t adjustedSampleBuffer[],
                                        float32_t adjustedfloatInput[],
                                        uint_fast16_t sampleCount)
{
    int_fast16_t loopCount;

    for(loopCount = 0; loopCount < (sampleCount/2); loopCount++)
    {
        adjustedfloatInput[loopCount*4] = (float32_t)((adjustedSampleBuffer[loopCount] & 0xFFFF))/(float32_t)(6553.6f);
        adjustedfloatInput[loopCount*4 + 1] = 0.0f;
        adjustedfloatInput[loopCount*4 + 2] = (float32_t)(((adjustedSampleBuffer[loopCount] & 0xFFFF0000) >> 16))/(float32_t)(6553.6f);
        adjustedfloatInput[loopCount*4 + 3] = 0.0f;
    }

    return 0;
}

/*
 * This function is called whenever a buffer is full. The content is converted
 * to floating point representation and FFT is performed on the 3 channels. In
 * the end the Conversion Mutex is released for the device to go to LPM3 state.
 *
 */
void adcBufCallback(ADCBuf_Handle handle, ADCBuf_Conversion *conversion,
    void *completedADCBuffer, uint32_t completedChannel) {

    int_fast16_t loopCount;

    if(avgLoop == (AVG_LOOP_COUNT-1))
    {
        /* Power Down the Sensor and Power Down the Switch for the AFE */
        GPIO_write(CONFIG_GPIO_SENSOREN, CONFIG_GPIO_LED_OFF);
        GPIO_write(CONFIG_GPIO_SENSORPWREN, CONFIG_GPIO_LED_OFF);
    }

    /* Adjust raw adc values and convert them to microvolts */
    ADCBuf_adjustRawValues(handle, completedADCBuffer, ADCBUFFERSIZE,
        completedChannel);
    convertAdjustedDataToFloat(completedADCBuffer, setfloatInput, ADCBUFFERSIZE);

    if(0 == completedChannel)
    {
        /* CPU now performs the X Axis-FFT calculation */
#if FFTSIZE == 2048 && defined(DeviceFamily_MSP432P4x1xI)
        arm_cfft_f32(&arm_cfft_sR_f32_len4096, setfloatInput, ifftFlag, doBitReverse);
#elif FFTSIZE == 1024 && defined(DeviceFamily_MSP432P4x1xI)
        arm_cfft_f32(&arm_cfft_sR_f32_len2048, setfloatInput, ifftFlag, doBitReverse);
#elif FFTSIZE == 512
        arm_cfft_f32(&arm_cfft_sR_f32_len1024, setfloatInput, ifftFlag, doBitReverse);
#elif FFTSIZE == 256
        arm_cfft_f32(&arm_cfft_sR_f32_len512, setfloatInput, ifftFlag, doBitReverse);
#endif

        /* Process the data through the Complex Magnitude Module for
         * calculating the magnitude at each bin */
        arm_cmplx_mag_f32(setfloatInput, fftOutput, ADCBUFFERSIZE);

        /* Store the DC Value */
        if(avgLoop == 0)
        {
            XAxisEnergyBin.energyValue[0] = fftOutput[0];
        }
        else
        {
            XAxisEnergyBin.energyValue[0] += fftOutput[0];
        }

        for(loopCount=1; loopCount<FFTSIZE; loopCount++)
        {
            /* Perform the FFT Bin conversion for +ve and -ve frequency
             * wrapping to +ve frequency */
            if(avgLoop == 0)
            {
                XAxisEnergyBin.energyValue[loopCount] = 2*fftOutput[loopCount];
            }
            else
            {
                XAxisEnergyBin.energyValue[loopCount] += 2*fftOutput[loopCount];
            }

            if(loopCount > 1)
            {
                if(XAxisEnergyBin.energyValue[loopCount] > XAxisEnergyBin.maxValue)
                {
                    XAxisEnergyBin.maxValue = XAxisEnergyBin.energyValue[loopCount];
                    XAxisEnergyBin.maxValueBin = loopCount;
                }
            }
            else
            {
                XAxisEnergyBin.maxValue    = XAxisEnergyBin.energyValue[1];
                XAxisEnergyBin.maxValueBin = 1;
            }
        }
    }
    else if(1 == completedChannel)
    {
        /* CPU now performs the X Axis-FFT calculation */
#if FFTSIZE == 2048 && defined(DeviceFamily_MSP432P4x1xI)
        arm_cfft_f32(&arm_cfft_sR_f32_len4096, setfloatInput, ifftFlag, doBitReverse);
#elif FFTSIZE == 1024 && defined(DeviceFamily_MSP432P4x1xI)
        arm_cfft_f32(&arm_cfft_sR_f32_len2048, setfloatInput, ifftFlag, doBitReverse);
#elif FFTSIZE == 512
        arm_cfft_f32(&arm_cfft_sR_f32_len1024, setfloatInput, ifftFlag, doBitReverse);
#elif FFTSIZE == 256
        arm_cfft_f32(&arm_cfft_sR_f32_len512, setfloatInput, ifftFlag, doBitReverse);
#endif

        /* Process the data through the Complex Magnitude Module for
         * calculating the magnitude at each bin */
        arm_cmplx_mag_f32(setfloatInput, fftOutput, ADCBUFFERSIZE);

        /* Store the DC Value */
        if(avgLoop == 0)
        {
            YAxisEnergyBin.energyValue[0] = fftOutput[0];
        }
        else
        {
            YAxisEnergyBin.energyValue[0] += fftOutput[0];
        }

        for(loopCount=1; loopCount<FFTSIZE; loopCount++)
        {
            /* Perform the FFT Bin conversion for +ve and -ve frequency
             * wrapping to +ve frequency */
            if(avgLoop == 0)
            {
                YAxisEnergyBin.energyValue[loopCount] = 2*fftOutput[loopCount];
            }
            else
            {
                YAxisEnergyBin.energyValue[loopCount] += 2*fftOutput[loopCount];
            }

            if(loopCount > 1)
            {
                if(YAxisEnergyBin.energyValue[loopCount] > YAxisEnergyBin.maxValue)
                {
                    YAxisEnergyBin.maxValue = YAxisEnergyBin.energyValue[loopCount];
                    YAxisEnergyBin.maxValueBin = loopCount;
                }
            }
            else
            {
                YAxisEnergyBin.maxValue    = YAxisEnergyBin.energyValue[1];
                YAxisEnergyBin.maxValueBin = 1;
            }
        }
    }
    if(2 == completedChannel)
    {
        /* CPU now performs the X Axis-FFT calculation */
#if FFTSIZE == 2048 && defined(DeviceFamily_MSP432P4x1xI)
        arm_cfft_f32(&arm_cfft_sR_f32_len4096, setfloatInput, ifftFlag, doBitReverse);
#elif FFTSIZE == 1024 && defined(DeviceFamily_MSP432P4x1xI)
        arm_cfft_f32(&arm_cfft_sR_f32_len2048, setfloatInput, ifftFlag, doBitReverse);
#elif FFTSIZE == 512
        arm_cfft_f32(&arm_cfft_sR_f32_len1024, setfloatInput, ifftFlag, doBitReverse);
#elif FFTSIZE == 256
        arm_cfft_f32(&arm_cfft_sR_f32_len512, setfloatInput, ifftFlag, doBitReverse);
#endif

        /* Process the data through the Complex Magnitude Module for
         * calculating the magnitude at each bin */
        arm_cmplx_mag_f32(setfloatInput, fftOutput, ADCBUFFERSIZE);

        /* Store the DC Value */
        if(avgLoop == 0)
        {
            ZAxisEnergyBin.energyValue[0] = fftOutput[0];
        }
        else
        {
            ZAxisEnergyBin.energyValue[0] += fftOutput[0];
        }

        for(loopCount=1; loopCount<FFTSIZE; loopCount++)
        {
            /* Perform the FFT Bin conversion for +ve and -ve frequency
             * wrapping to +ve frequency */
            if(avgLoop == 0)
            {
                ZAxisEnergyBin.energyValue[loopCount] = 2*fftOutput[loopCount];
            }
            else
            {
                ZAxisEnergyBin.energyValue[loopCount] += 2*fftOutput[loopCount];
            }

            if(loopCount > 1)
            {
                if(ZAxisEnergyBin.energyValue[loopCount] > ZAxisEnergyBin.maxValue)
                {
                    ZAxisEnergyBin.maxValue = ZAxisEnergyBin.energyValue[loopCount];
                    ZAxisEnergyBin.maxValueBin = loopCount;
                }
            }
            else
            {
                ZAxisEnergyBin.maxValue    = ZAxisEnergyBin.energyValue[1];
                ZAxisEnergyBin.maxValueBin = 1;
            }
        }

        /* Average out the values for AVG_LOOP_COUNT times */
        if(avgLoop == (AVG_LOOP_COUNT-1))
        {
            for(loopCount=0; loopCount<FFTSIZE; loopCount++)
            {
                XAxisEnergyBin.energyValue[loopCount] = (XAxisEnergyBin.energyValue[loopCount])/AVG_LOOP_COUNT;
                YAxisEnergyBin.energyValue[loopCount] = (YAxisEnergyBin.energyValue[loopCount])/AVG_LOOP_COUNT;
                ZAxisEnergyBin.energyValue[loopCount] = (ZAxisEnergyBin.energyValue[loopCount])/AVG_LOOP_COUNT;
            }

            XAxisEnergyBin.maxValue = XAxisEnergyBin.maxValue/AVG_LOOP_COUNT;
            YAxisEnergyBin.maxValue = YAxisEnergyBin.maxValue/AVG_LOOP_COUNT;
            ZAxisEnergyBin.maxValue = ZAxisEnergyBin.maxValue/AVG_LOOP_COUNT;
        }

        /* Post the Conversion Complete Mutex */
        sem_post(&adcConvMutex);
    }
}

#ifdef GPIO_WAKEUP
/* Call back function when GPIO SW1 is pressed. This wakes up the system from
 * LPM3 state and starts the next ADC sample acquisition and FFT processing.
 * */
void gpioButtonFxn0(uint_least8_t x)
{
    sem_post(&devWakeMutex);
}
#endif

/* Vibration Task to start the Thread for Vibration data capture */
void vibrationTask(void)
{
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    /* Set priority and stack size attributes */
    pthread_attr_init(&attrs);
    priParam.sched_priority = VIBRATION_TASK_PRIORITY;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    pthread_attr_setschedparam(&attrs, &priParam);

    retc |= pthread_attr_setstacksize(&attrs, VIBRATION_TASK_STACK_SIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    retc = pthread_create(&thread, &attrs, vibrationThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }
}

/* Callback function for Power Policy Manager to perform specific tasks on
 * LMP3 Entry and Wake. During LPM3 Entry, the GPIO for AFE Power Enable and
 * Sensor Enable are made low to reduce current consumption. Also since
 * enableparking is disabled, the unused IO's are configured to output low
 * state so that current consumption is reduced. During LPM3 exit the GPIO for
 * AFE Power Enable and Sensor Enable are made High.
 * */
uint8_t notifyDeepSleepFxn(uint8_t eventType, uint8_t eventArg, uint8_t clientArg)
{
    if(PowerMSP432_ENTERING_DEEPSLEEP == eventType)
    {
        /* Set the unused IOs to low state */
#ifdef GPIO_WAKEUP
#ifdef BLE_COMMS
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PA, (PIN_ALL16 & ~(GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN13)));
#else
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PA, (PIN_ALL16 & ~(GPIO_PIN1|GPIO_PIN2|GPIO_PIN3)));
#endif
#else
#ifdef BLE_COMMS
    	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PA, (PIN_ALL16 & ~(GPIO_PIN2|GPIO_PIN3|GPIO_PIN13)));
#else
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PA, (PIN_ALL16 & ~(GPIO_PIN2|GPIO_PIN3)));
#endif
#endif
#ifdef BLE_COMMS
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PB, (PIN_ALL16 & ~(GPIO_PIN2|GPIO_PIN3|GPIO_PIN8|GPIO_PIN10)));
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PC, (PIN_ALL16 & ~(GPIO_PIN8|GPIO_PIN9|GPIO_PIN15)));
#else
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PB, (PIN_ALL16 & ~(GPIO_PIN8|GPIO_PIN10)));
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PC, (PIN_ALL16 & ~(GPIO_PIN9)));
#endif
     	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PD, PIN_ALL16);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PE, PIN_ALL16);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PJ, PIN_ALL16);
#ifdef GPIO_WAKEUP
#ifdef BLE_COMMS
        MAP_GPIO_setAsOutputPin(GPIO_PORT_PA, (PIN_ALL16 & ~(GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN13)));
#else
        MAP_GPIO_setAsOutputPin(GPIO_PORT_PA, (PIN_ALL16 & ~(GPIO_PIN1|GPIO_PIN2|GPIO_PIN3)));
#endif
#else
#ifdef BLE_COMMS
        MAP_GPIO_setAsOutputPin(GPIO_PORT_PA, (PIN_ALL16 & ~(GPIO_PIN2|GPIO_PIN3|GPIO_PIN13)));
#else
        MAP_GPIO_setAsOutputPin(GPIO_PORT_PA, (PIN_ALL16 & ~(GPIO_PIN2|GPIO_PIN3)));
#endif
#endif
#ifdef BLE_COMMS
        MAP_GPIO_setAsOutputPin(GPIO_PORT_PB, (PIN_ALL16 & ~(GPIO_PIN2|GPIO_PIN3|GPIO_PIN8|GPIO_PIN10)));
        MAP_GPIO_setAsOutputPin(GPIO_PORT_PC, (PIN_ALL16 & ~(GPIO_PIN8|GPIO_PIN9|GPIO_PIN15)));
#else
        MAP_GPIO_setAsOutputPin(GPIO_PORT_PB, (PIN_ALL16 & ~(GPIO_PIN8|GPIO_PIN10)));
        MAP_GPIO_setAsOutputPin(GPIO_PORT_PC, (PIN_ALL16 & ~(GPIO_PIN9)));
#endif
        MAP_GPIO_setAsOutputPin(GPIO_PORT_PD, PIN_ALL16);
        MAP_GPIO_setAsOutputPin(GPIO_PORT_PE, PIN_ALL16);
        MAP_GPIO_setAsOutputPin(GPIO_PORT_PJ, PIN_ALL16);


        /* Turn off PSS high-side supervisors to consume less power in shutdown */
        MAP_PSS_disableHighSide();
    }
    else
    {
#ifdef BLE_COMMS
        if(GPIO_read(CONFIG_SRDY) == 0)
        {
            Power_setConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
        }
#endif

        /* Enable ADC pins in ADC mode before powering up the AFE supply, to
         * avoid bus contention */
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                GPIO_PIN1, GPIO_TERTIARY_MODULE_FUNCTION);
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                GPIO_PIN2, GPIO_TERTIARY_MODULE_FUNCTION);
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    }

    return 0;
}

#ifdef RTC_WAKEUP
/* Function to set the next wakeup time after N Hours and M minutes */
void predictive_maintenance_setWakeTime(uint8_t Hours, uint8_t Minutes)
{
    RTC_C_Calendar newTime;
    uint_fast8_t excessHours = 0;

    /* Get the current time */
    newTime = MAP_RTC_C_getCalendarTime();

    /* Check if the wakeup period parameters are valid
     * Hour interval cannot be more than 24
     * Minute interval cannot be more than 60
     * Hour and Minute both cannot be 0 */
    if((Hours > 24) || (Minutes > 60) || ((Hours == 0) && (Minutes == 0)))
    {
        while(1);
    }

    /* Get the current time and check if there is a roll over for minute or
     * hour. If there is a hour roll over, then change the next match time
     * for 0 hrs. If there is a minute roll over, then increment the hour
     * and then set the minute match time. If there is no roll over for
     * either minute or hour then program the next match time with hour
     * unchanged and minute added. */

    newTime.minutes = newTime.minutes + Minutes;

    if(newTime.minutes > 59)
    {
        newTime.minutes = newTime.minutes - 60;
        excessHours = 1;
    }

    newTime.hours = newTime.hours + Hours + excessHours;

    if(newTime.hours > 23)
    {
        newTime.hours = newTime.hours - 24;
    }

    MAP_RTC_C_setCalendarAlarm(newTime.minutes,
                               newTime.hours,
                               RTC_C_ALARMCONDITION_OFF,
                               RTC_C_ALARMCONDITION_OFF);
}
#endif

/* The Vibration Thread which handles the scheduling of the ADC data
 * acquisition and FFT conversion */
void *vibrationThread(void *arg0)
{
#ifdef RTC_WAKEUP
    /* Hwi variables */
    HwiP_Handle hwiPRTC = NULL;
    HwiP_Params hwiPRTCParams;
#endif

    /* Power Up AFE Timer Handle */
    Timer_Handle timer0;
    Timer_Params params;

    /* ADC Buff Handle */
    ADCBuf_Handle adcBuf;
    ADCBuf_Params adcBufParams;
    ADCBufMSP432_ParamsExtension adcBufParamsEx;
    ADCBuf_Conversion oneshotVibChannels[ADC_NO_OF_CH];

    /* Set up an ADCBuf peripheral in ADCBuf_RECURRENCE_MODE_ONE_SHOT */
    ADCBuf_Params_init(&adcBufParams);
    adcBufParams.callbackFxn = adcBufCallback;
    adcBufParams.recurrenceMode = ADCBuf_RECURRENCE_MODE_ONE_SHOT;
    adcBufParams.returnMode = ADCBuf_RETURN_MODE_CALLBACK;
    adcBufParams.samplingFrequency = ADC_CH_SAMPLING_FREQ*ADC_NO_OF_CH;
    adcBufParamsEx.samplingDuration = ADCBufMSP432_SamplingDuration_PULSE_WIDTH_16;
    adcBufParams.custom = &adcBufParamsEx;
    adcBuf = ADCBuf_open(CONFIG_ADCBUF0, &adcBufParams);

    /* Configure the conversion struct */
    oneshotVibChannels[0].arg = NULL;
    oneshotVibChannels[0].adcChannel = CONFIG_ADCBUF0CHANNEL_0;
    oneshotVibChannels[0].sampleBuffer = sampleBufferX;
    oneshotVibChannels[0].sampleBufferTwo = NULL;
    oneshotVibChannels[0].samplesRequestedCount = ADCBUFFERSIZE;

    oneshotVibChannels[1].arg = NULL;
    oneshotVibChannels[1].adcChannel = CONFIG_ADCBUF0CHANNEL_1;
    oneshotVibChannels[1].sampleBuffer = sampleBufferY;
    oneshotVibChannels[1].sampleBufferTwo = NULL;
    oneshotVibChannels[1].samplesRequestedCount = ADCBUFFERSIZE;

    oneshotVibChannels[2].arg = NULL;
    oneshotVibChannels[2].adcChannel = CONFIG_ADCBUF0CHANNEL_2;
    oneshotVibChannels[2].sampleBuffer = sampleBufferZ;
    oneshotVibChannels[2].sampleBufferTwo = NULL;
    oneshotVibChannels[2].samplesRequestedCount = ADCBUFFERSIZE;

    if (!adcBuf){
        /* AdcBuf did not open correctly. */
        while(1);
    }

    /* Create Mutex for ADC Conversion before starting the next cycle */
    sem_init(&adcConvMutex, 1, 0);

    /* Create Mutex for Device wakeup due to GPIO switch release or RTC
     * wake up */
    sem_init(&devWakeMutex, 1, 0);

    /* Create Mutex for Device sleep mode entry during power up sequencing
     * of the AFE to reduce current consumption */
    sem_init(&afeWakeMutex, 1, 0);

#ifdef GPIO_WAKEUP
    /* Initialize interrupts for all ports that need them */
    GPIO_setCallback(CONFIG_GPIO_BUTTON0, gpioButtonFxn0);
    GPIO_enableInt(CONFIG_GPIO_BUTTON0);
#endif

    Power_setPolicy((Power_PolicyFxn)PowerMSP432_deepSleepPolicy);

    Power_setConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_1);

    /* Register for entering shutdown notifications */
    Power_registerNotify(&notifyObj, PowerMSP432_ENTERING_DEEPSLEEP | PowerMSP432_AWAKE_DEEPSLEEP,
        (Power_NotifyFxn) notifyDeepSleepFxn, 0);

#ifdef RTC_WAKEUP
    /* Initialize RTC for current time and wakeup due to time match from
     * calendar mode. */
    MAP_RTC_C_initCalendar(&currentTime, RTC_C_FORMAT_BINARY);

    MAP_RTC_C_setPrescaleValue(RTC_C_PRESCALE_0, 0);
    MAP_RTC_C_definePrescaleEvent(RTC_C_PRESCALE_0, RTC_C_PSEVENTDIVIDER_64);
    MAP_RTC_C_clearInterruptFlag(RTC_C_CLOCK_ALARM_INTERRUPT);
    MAP_RTC_C_enableInterrupt(RTC_C_CLOCK_ALARM_INTERRUPT);

    MAP_RTC_C_startClock();

    /* Enable interrupts from RTC Module. */
    HwiP_Params_init(&hwiPRTCParams);
    hwiPRTCParams.arg = 0;
    hwiPRTCParams.priority = 0x20;
    hwiPRTC = HwiP_create(INT_RTC_C, (HwiP_Fxn)RTC_C_IRQHandler, &hwiPRTCParams);

    if (hwiPRTC == NULL)
    {
        while(1);
    }
#endif

    /* Setting up the timer in continuous callback mode that calls the callback
     * function for 279 ms to power up 3.3V AFE rail and 62 ms to power up the
     * 3-axis sensor.
     */
    Timer_Params_init(&params);

    /*
     * Go to sleep in the foreground thread forever. The data will be collected
     * and transfered in the background thread
     */
    for(;;)
    {
#ifdef DeviceFamily_MSP432P4x1xI
        /* Set constraint to prevent the device from going to LPM3.0 state */
        Power_setConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif

        /* Power Up the Switch for the AFE */
        GPIO_write(CONFIG_GPIO_SENSORPWREN, CONFIG_GPIO_LED_ON);

        params.period = 40000;
        params.periodUnits = Timer_PERIOD_US;
        params.timerMode = Timer_ONESHOT_CALLBACK;
        params.timerCallback = timerCallback;
        timer0 = Timer_open(CONFIG_TIMER0, &params);

        if (timer0 == NULL) {
            /* Failed to initialized timer */
            while (1);
        }
        if (Timer_start(timer0) == Timer_STATUS_ERROR) {
            /* Failed to start timer */
            while (1);
        }
        sem_wait(&afeWakeMutex);
        Timer_close(timer0);

        /* Power Up the Sensor */
        GPIO_write(CONFIG_GPIO_SENSOREN, CONFIG_GPIO_LED_ON);

        params.period = 10000;
        params.periodUnits = Timer_PERIOD_US;
        params.timerMode = Timer_ONESHOT_CALLBACK;
        params.timerCallback = timerCallback;
        timer0 = Timer_open(CONFIG_TIMER0, &params);

        if (timer0 == NULL) {
            /* Failed to initialized timer */
            while (1);
        }
        if (Timer_start(timer0) == Timer_STATUS_ERROR) {
            /* Failed to start timer */
            while (1);
        }
        sem_wait(&afeWakeMutex);
        Timer_close(timer0);

        avgLoop = 0;

#ifdef DeviceFamily_MSP432P4x1xI
        /* Set constraint to prevent the device from going to LPM3.0 state */
        Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif

        /* Allocate memory buffer for FFT computation */
        fftOutput = malloc(ADCBUFFERSIZE*sizeof(float32_t));

		do
        {
            /* Start converting using ADCBuf Drivers */
            if (ADCBuf_convert(adcBuf, oneshotVibChannels, ADC_NO_OF_CH) !=
                ADCBuf_STATUS_SUCCESS) {
                /* Did not start conversion process correctly. */
                while(1);
            }

            /* Wait for ADC Conversion to Complete */
            sem_wait(&adcConvMutex);

            /* Increment the loop count */
            avgLoop++;
        }
        while(avgLoop < AVG_LOOP_COUNT);

		/* Free the Buffer */
        free(fftOutput);

        /* Send Mutex to start data transmission and wait for data transmission
         * to complete. Before sending the data prevent LPM3 transition and only
         * when it is complete allow LPM3 transition */
        Power_setConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
        sem_post(&startComms);
        sem_wait(&endComms);
        Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
        Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);

#ifdef RTC_WAKEUP
        predictive_maintenance_setWakeTime(0, 1);
#endif
        /* Wait for GPIO To wakeup the device from LPM3 */
        sem_wait(&devWakeMutex);
#ifdef BLE_COMMS
        uint32_t eventPend = AP_EVT_HOSTMCU_WAKE;
        mq_send(apQueueSend, (void*)&eventPend, sizeof(uint32_t), 1);
#endif

    }
}

#ifdef RTC_WAKEUP
/* Interrupt handler for RTC Match Event */
void RTC_C_IRQHandler(void)
{
    /* Clear the interrupt status for alarm and timer-0 interrupt */
    MAP_RTC_C_clearInterruptFlag(RTC_C_CLOCK_ALARM_INTERRUPT);

    /* Post the semaphore to process the ADCBuf Task and FFT */
    sem_post(&devWakeMutex);

}
#endif

void timerCallback(Timer_Handle myHandle)
{
    /* Post the semaphore to sequence the power up of the AFE */
    sem_post(&afeWakeMutex);
}
