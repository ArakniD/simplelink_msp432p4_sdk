/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
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
 *  ======== capturepwmdisplay.c ========
 */
/* Driver Header files */
#include <ti/drivers/Capture.h>
#include <ti/display/Display.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <stddef.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/* Callback used for blinking LED on timer completion */
void captureCallback(Capture_Handle handle, uint32_t interval);

/* Local Variables */
static Display_Handle display;
volatile uint32_t curInterval;
static SemaphoreP_Handle captureSem;

/*
 *  ======== mainThread ========
 *  Task that will capture two rising edges and output the time between the
 *  two edges
 */
void *mainThread(void *arg0)
{
    SemaphoreP_Params semParams;
    Capture_Params captureParams;
    Capture_Handle capture;
    PWM_Params pwmParams;
    PWM_Handle pwm0, pwm1;

    /* units in microseconds */
    uint32_t   pwmPeriod = 100000;
    uint32_t   duty = 50000;

    /* Driver Init Functions */
    Capture_init();
    Display_init();
    PWM_init();

    /* Open Display for Output */
    display = Display_open(Display_Type_UART, NULL);

    if (display == NULL)
    {
        /* Failed to open display driver */
        while (1);
    }

    /* PWM Params init */
    PWM_Params_init(&pwmParams);
    pwmParams.dutyUnits = PWM_DUTY_US;
    pwmParams.dutyValue = 0;
    pwmParams.periodUnits = PWM_PERIOD_US;
    pwmParams.periodValue = pwmPeriod;

    /* Open PWM0 */
    pwm0 = PWM_open(CONFIG_PWM_0, &pwmParams);

    if (!pwm0)
    {
        Display_printf(display, 0, 0, "Failed to initialized PWM0.\n");
        while (1);
    }

    PWM_start(pwm0);

    /* Open PWM1 */
    pwm1 = PWM_open(CONFIG_PWM_1, &pwmParams);

    if (!pwm1)
    {
        Display_printf(display, 0, 0, "Failed to initialized PWM1.\n");
        while (1);
    }

    PWM_start(pwm1);

    /* Semaphore to wait for capture data */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    captureSem = SemaphoreP_create(0, &semParams);

    if (captureSem == NULL)
    {
        Display_printf(display, 0, 0, "Could not allocate semaphore!\n");
        while(1);
    }

    /* Setting up the Capture driver to detect two rising edges and report
     * the result in microseconds
     */
    Capture_Params_init(&captureParams);
    captureParams.mode = Capture_RISING_EDGE;
    captureParams.periodUnit = Capture_PERIOD_US;
    captureParams.callbackFxn = captureCallback;

    capture = Capture_open(CONFIG_CAPTURE_0, &captureParams);
    if (capture == NULL)
    {
        Display_printf(display, 0, 0, "Failed to initialized Capture!\n");
        while(1);
    }

    Display_printf(display, 0, 0, "About to Capture!\n");

    /* set the PWM duty and start the capture */
    PWM_setDuty(pwm0, duty);
    PWM_setDuty(pwm1, duty);
    Capture_start(capture);

    while(1)
    {
        /* The value printed should be close to the period of the pwm */
        SemaphoreP_pend(captureSem, SemaphoreP_WAIT_FOREVER);
        Display_printf(display, 0, 0,
                "Period: %d\n", curInterval);
    }
}

/* Callback function that displays the interval */
void captureCallback(Capture_Handle handle, uint32_t interval)
{
    curInterval = interval;
    SemaphoreP_post(captureSem);
}
