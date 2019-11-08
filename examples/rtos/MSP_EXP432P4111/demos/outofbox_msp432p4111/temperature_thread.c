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
 *  ======== temperature_thread.c ========
 */

/* Includes */
#include <callbacks.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/* POSIX Header files */
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

/* Driver Header files */
#include <ti/drivers/ADC.h>

/* Board Header file */
#include "ti_drivers_config.h"

/* Local Header Files */
#include "callbacks.h"
#include "lcd_thread.h"
#include "temperature_thread.h"
#include "uart_thread.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Global Variables */
pthread_t temperatureThread_handler;
sem_t semTemp;

/*
 *  ======== temperatureThread ========
 *  Configures and triggers ADC measurement on either
 *  external or internal temperature sensors
 */
void *temperatureThread(void *arg0)
{
    LcdMsg msg;
    int retc;
    ADC_Handle   adc;
    ADC_Params   params;
    int_fast16_t res;
    uint16_t adcValue;

    ADC_init();

    ADC_Params_init(&params);
    adc = ADC_open(CONFIG_ADC0, &params);

    if (adc == NULL) {
        while (1);
    }

    while (1) {
        /* Wait for a change to occur */
        retc = sem_wait(&semTemp);
        if (retc == 0) {

            if (externaltempSensor)
            {
                // Sample and convert from external temperature sensor
                res = ADC_convert(adc, &adcValue);

                if (res == ADC_STATUS_SUCCESS) {
                    /* Calculates degree Celsius from the raw 14-bit ADC value */
                    float tempC = -0.193f * (adcValue/16383.0f) * 3300 + 212.009f;

                    msg.cmd = LcdCmd_SHOW_TEMPERATURE;
                    msg.buffer = (void *)(&tempC);
                    retc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
                    if (retc == -1) {
                        // Failed to send to message queue
                    }
                    retc = sem_post(&semLCD);
                    if (retc == -1) {
                        while (1);
                    }
                }
            }
            else
            {
                /*
                 * DriverLib ADC Temperature Sensor Workaround
                 *
                 * Use DriverLib API to configure ADC due to TI Drivers not supporting internal ADC channels yet.
                 * TODO: update to TI Drivers APIs once TI Drivers is updated to support internal ADC channels.
                 */

                // Setting reference voltage to 2.5 and enabling temperature sensor
                REF_A_enableTempSensor();
                REF_A_setReferenceVoltage(REF_A_VREF2_5V);
                REF_A_enableReferenceVoltage();

                // Enable ADC Temp Sensor
                ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,
                            ADC_TEMPSENSEMAP);

                // Configure ADC MEM0 to sample from internal temp sensor channel A22
                ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS,
                            ADC_INPUT_A22, false);

                ADC14_enableConversion();

                ADC14_toggleConversionTrigger();

                while (!(ADC14_getInterruptStatus() & ADC_INT0));

                adcValue = ADC14_getResult(ADC_MEM0);

                /*
                 * End DriverLib Workaround
                 */

                // Variables to store the ADC temperature reference calibration value
                uint32_t adcRefTempCal_2_5v_30;
                uint32_t adcRefTempCal_2_5v_85;

                // Read the ADC temperature reference calibration value
                adcRefTempCal_2_5v_30 = SysCtl_A_getTempCalibrationConstant(SYSCTL_A_2_5V_REF,
                                                                            SYSCTL_A_30_DEGREES_C);
                adcRefTempCal_2_5v_85 = SysCtl_A_getTempCalibrationConstant(SYSCTL_A_2_5V_REF,
                                                                            SYSCTL_A_85_DEGREES_C);

                float tempC = (((float) adcValue - adcRefTempCal_2_5v_30) * (85.0f - 30.0f)) /
                        (adcRefTempCal_2_5v_85 - adcRefTempCal_2_5v_30) + 30.0f;

                msg.cmd = LcdCmd_SHOW_TEMPERATURE;
                msg.buffer = (void *)(&tempC);
                retc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
                if (retc == -1) {
                    // Failed to send to message queue
                }
                retc = sem_post(&semLCD);
                if (retc == -1) {
                    while (1);
                }
            }
        }
    }
}
