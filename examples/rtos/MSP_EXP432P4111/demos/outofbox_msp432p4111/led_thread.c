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
 *  ======== led_thread.c ========
 */

/* Includes */
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <ti/devices/msp432p4xx/inc/msp.h>

/* POSIX Header files */
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/Timer.h>

/* Board Header file */
#include "ti_drivers_config.h"

/* Local Header Files */
#include "callbacks.h"
#include "led_thread.h"
#include "lcd_thread.h"
#include "uart_thread.h"

/* Global Variables */
pthread_t ledthread_handler;
sem_t semLED;
mqd_t  mqLED;

PWM_Handle pwms[PWM_COLOR_COUNT];
uint16_t   pwmPeriod = 3000;                            // PWM Period in MicroSeconds.
uint16_t   dutys[PWM_COLOR_COUNT];                      // Duty cycle in MicroSeconds.
uint32_t   ledTogglePeriods[LED_COLOR_MODE_COUNT] =     // Period(MicroSeconds) for toggling LED in each of the 4 color modes.
        {500000, 500000, 500000, 500000, 500000};               // Half of the LED's blink period (represents the ON or OFF time)

/* Static Function Declarations */
static void changeLedTogglePeriod(int colorMode, Timer_Params *timer_params, char *str);
static void changeLedColor(char colorMode);
static void setState(bool ledActive);
static uint32_t calculateNewTogglePeriod(int tapPeriod);

/*
 *  ======== ledThread ========
 *  Creates the PWM modules for RGB color setting.
 *  Handles changing the blink rate, color and ON/OFF state of the LED.
 */
void *ledThread(void *arg0)
{
    PWM_Params pwm_params;
    Timer_Params timer_params;
    LedMsg msg;
    bool ledActive = true;
    int currentColorMode = LED_RED_MODE;
    char str[7];
    int retc;

    /* Enable GPIO CallBack Functions */
    /* Call driver init functions */
    GPIO_init();
    Timer_init();
    /* install Button callback */
    GPIO_setCallback(CONFIG_GPIO_BUTTON0, gpioButton0_Callback);

    /* Enable interrupts */
    GPIO_enableInt(CONFIG_GPIO_BUTTON0);

    /*
     *  If more than one input pin is available for your device, interrupts
     *  will be enabled on CONFIG_GPIO_BUTTON1.
     */
    if (CONFIG_GPIO_BUTTON0 != CONFIG_GPIO_BUTTON1) {
        /* Install Button callback */
        GPIO_setCallback(CONFIG_GPIO_BUTTON1, gpioButton1_Callback);
        GPIO_enableInt(CONFIG_GPIO_BUTTON1);
    }

    /* Create Timers with timer callbacks */
    //Seed Random number generator
    srand(TLV->RANDOM_NUM_1);

    /* Set-Up intervalTimer */
    Timer_Params_init(&timer_params);
    timer_params.period = 4000000;
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = intervalTimer_Callback;
    intervalTimer = Timer_open(CONFIG_TIMER0, &timer_params);
    if (intervalTimer == NULL) {
       /* Failed to initialized Timer */
       while(1);
    }

    /* Set-Up blinkTimer */
    Timer_Params_init(&timer_params);
    timer_params.period = ledTogglePeriods[currentColorMode];
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = blinkTimer_Callback;
    blinkTimer = Timer_open(CONFIG_TIMER1, &timer_params);
    if (blinkTimer == NULL) {
       /* Failed to initialized Timer */
       while(1);
    }
    Timer_start(blinkTimer);

    /* Set-Up debounceTimer */
    Timer_Params_init(&timer_params);
    timer_params.period = 20000;
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = debounceTimer_Callback;
    debounceTimer = Timer_open(CONFIG_TIMER3, &timer_params);
    if (debounceTimer == NULL) {
       /* Failed to initialized Timer */
       while(1);
    }
    /* Call driver init functions. */
    PWM_init();

    /* Set the parameters for PWM modules. */
    PWM_Params_init(&pwm_params);
    pwm_params.dutyUnits = PWM_DUTY_US;
    pwm_params.dutyValue = 0;
    pwm_params.periodUnits = PWM_PERIOD_US;
    pwm_params.periodValue = pwmPeriod;

    /* Open/Start the Red LED PWM Module */
    pwms[PWM_RED] = PWM_open(CONFIG_PWM0, &pwm_params);
    if (pwms[PWM_RED] == NULL) {
      /* CONFIG_PWM0 did not open */
      while (1);
    }

    /* Open/Start the Green LED PWM Module */
    if (CONFIG_PWM1 != CONFIG_PWM0)
    {
      pwms[PWM_GREEN] = PWM_open(CONFIG_PWM1, &pwm_params);
      if (pwms[PWM_GREEN] == NULL) {
          /* CONFIG_PWM1 did not open */
          while (1);
      }
    }

    /* Open the Blue LED PWM Module */
    if (CONFIG_PWM2 != CONFIG_PWM1)
    {
      pwms[PWM_BLUE] = PWM_open(CONFIG_PWM2, &pwm_params);
      if (pwms[PWM_BLUE] == NULL) {
          /* CONFIG_PWM2 did not open */
          while (1);
      }
    }

    /* Set duty cycles so LED is 100% Red. */
    changeLedColor(currentColorMode);
    setState(ledActive);

    while (1) {
        /* Wait for a change to occur */
        retc = sem_wait(&semLED);
        if (retc == 0) {
            retc = mq_receive(mqLED, (char *)&msg, sizeof(msg), NULL);
            if (retc != -1) {

                switch (msg.cmd) {
                    case LedCmd_COLOR_MODE:
                        /* Cycle to the next color mode */
                        currentColorMode++;
                        if (currentColorMode > LED_RANDOM_MODE)
                            currentColorMode = 0;
                        changeLedColor(currentColorMode);
                        changeLedTogglePeriod(currentColorMode, &timer_params, str);
                        break;
                    case LedCmd_PC_UPDATE:
                        /* Update LED color/blinkrate from PC GUI */
                        if (currentColorMode != LED_PC_CONTROLLED_MODE)
                        {
                            currentColorMode = LED_PC_CONTROLLED_MODE;
                            changeLedTogglePeriod(currentColorMode, &timer_params, str);
                        }
                        if (((int32_t *)(msg.buffer))[0] != -1)
                            dutys[PWM_RED] = ((uint32_t *)(msg.buffer))[0];
                        if (((int32_t *)(msg.buffer))[1] != -1)
                            dutys[PWM_GREEN] = ((uint32_t *)(msg.buffer))[1];
                        if (((int32_t *)(msg.buffer))[2] != -1)
                            dutys[PWM_BLUE] = ((uint32_t *)(msg.buffer))[2];
                        changeLedColor(currentColorMode);

                        if (((int32_t *)(msg.buffer))[3] != -1)
                        {
                            ledTogglePeriods[currentColorMode] = ((uint32_t *)(msg.buffer))[3];
                            changeLedTogglePeriod(currentColorMode, &timer_params, str);
                        }
                        break;
                    case LedCmd_BLINK_RATE:
                        /* Update blinkrate base on button S1 tapping */
                        ledTogglePeriods[currentColorMode] = calculateNewTogglePeriod((int)(msg.buffer));
                        changeLedTogglePeriod(currentColorMode, &timer_params, str);
                        break;
                    case LedCmd_TOGGLE:
                        /* Toggle LED (Timer PWM output) on/off */
                        ledActive = !ledActive;
                        if (currentColorMode == LED_RANDOM_MODE && ledActive)
                            changeLedColor(LED_RANDOM_MODE);
                        setState(ledActive);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

/*
 * Update the LED toggle Timer period to the specified colorMode's stored period
 */
static void changeLedTogglePeriod(int colorMode, Timer_Params *timer_params, char *str)
{
    if (timer_params->period != ledTogglePeriods[colorMode])
    {
        Timer_close(blinkTimer);
        Timer_Params_init(timer_params);
        timer_params->period = ledTogglePeriods[colorMode];
        timer_params->periodUnits = Timer_PERIOD_US;
        timer_params->timerMode = Timer_CONTINUOUS_CALLBACK;
        timer_params->timerCallback = blinkTimer_Callback;
        blinkTimer = Timer_open(CONFIG_TIMER1, timer_params);
        if (blinkTimer == NULL) {
            /* Failed to initialized Timer */
            while(1);
        }
        Timer_start(blinkTimer);
    }

    LedMsg msg;
    int retc;
    unsigned int bpm = (unsigned int)(roundf(((1000000.0f) / (2.0f * ledTogglePeriods[colorMode])) * 60.0f));
    if (colorMode != LED_PC_CONTROLLED_MODE && bpm > 999)
        bpm /= 10;
    switch (colorMode) {
        case LED_RED_MODE:
            sprintf(str, "%-3dRED", bpm);
            break;
        case LED_GREEN_MODE:
            sprintf(str, "%-3dGRN", bpm);
            break;
        case LED_BLUE_MODE:
            sprintf(str, "%-3dBLU", bpm);
            break;
        case LED_RANDOM_MODE:
            sprintf(str, "%-3dRND", bpm);
            break;
        case LED_PC_CONTROLLED_MODE:
            sprintf(str, "%-4dPC", bpm);
            break;
        default:
            sprintf(str, "%-3d", bpm);
            break;
    }

    /* Send message to change color */
    msg.cmd = LcdCmd_SHOW_STRING;
    msg.buffer = (void *)(str);
    retc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
    if (retc == -1) {
        // Failed to send to message queue
    }
    retc = sem_post(&semLCD);
    if (retc == -1) {
        while (1);
    }
}

/*
 * Update the LED Color (Timer PWM period for RED, GRN, & BLU) to the specified colorMode
 */
static void changeLedColor(char colorMode)
{
    switch (colorMode) {
        case LED_RED_MODE:
            dutys[PWM_RED] = pwmPeriod;
            dutys[PWM_GREEN] = 0;
            dutys[PWM_BLUE] = 0;
            break;
        case LED_GREEN_MODE:
            dutys[PWM_RED] = 0;
            dutys[PWM_GREEN] = pwmPeriod;
            dutys[PWM_BLUE] = 0;
            break;
        case LED_BLUE_MODE:
            dutys[PWM_RED] = 0;
            dutys[PWM_GREEN] = 0;
            dutys[PWM_BLUE] = pwmPeriod;
            break;
        case LED_RANDOM_MODE:
            dutys[PWM_RED] = (uint16_t)((rand() % 255) / 255.0f *pwmPeriod);
            dutys[PWM_GREEN] = (uint16_t)((rand() % 255) / 255.0f *pwmPeriod);
            dutys[PWM_BLUE] = (uint16_t)((rand() % 255) / 255.0f *pwmPeriod);
            break;
        case LED_PC_CONTROLLED_MODE:
            break;
        default:
            break;
    }
    /* Set new PWM duty cycle for RED, GRN, & BLU */
    PWM_setDuty(pwms[PWM_RED], dutys[PWM_RED]);
    PWM_setDuty(pwms[PWM_GREEN], dutys[PWM_GREEN]);
    PWM_setDuty(pwms[PWM_BLUE], dutys[PWM_BLUE]);
}

/*
 * Turn LED (Timer PWM outputs for RED, GRN, & BLU) on/off
 */
static void setState(bool ledActive)
{
    if(ledActive) {
        /* Turn On LED */
        PWM_start(pwms[PWM_RED]);
        PWM_start(pwms[PWM_GREEN]);
        PWM_start(pwms[PWM_BLUE]);
    }
    else {
        /* Turn Off LED */
        PWM_stop(pwms[PWM_RED]);
        PWM_stop(pwms[PWM_GREEN]);
        PWM_stop(pwms[PWM_BLUE]);
    }
}

/*
 * Calculate the average button S1 tapping frequency
 */
static uint32_t calculateNewTogglePeriod(int timerValueOnTap)
{
    static uint32_t newTogglePeriod;

    /* Convert captured count value into microseconds. */
    uint32_t tapPeriod = timerValueOnTap/48;

    numIntervals++;

    /* Take the average of all interval times, divided by two. */
    newTogglePeriod = (uint32_t)(((newTogglePeriod * (numIntervals-1)) + tapPeriod/2.0f) / (numIntervals));

    return newTogglePeriod;
}
