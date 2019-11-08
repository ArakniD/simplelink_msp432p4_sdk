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
 *  ======== callbacks.c ========
 */
/* Includes */
#include <stdbool.h>
#include <stdlib.h>

/* POSIX Header files */
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
#include "temperature_thread.h"

#include "ti/devices/msp432p4xx/inc/msp.h"

/* Global Variables */
bool counting = false;              // TRUE when interval time counter is running.
bool enteringTempMode = false;
bool enteringIntTempMode = false;
bool enteringExtTempMode = false;
bool inTempMode = false;
bool externaltempSensor = false;
bool scrolling = true;
uint32_t timerValueOnTap;           // Timer32 Value on Tap.
uint16_t numIntervals = 0;          // Number of intervals recorded.
uint16_t S1buttonDebounce = 0;      // De-bounce state for button S1.
uint16_t S2buttonDebounce = 0;      // De-bounce state for button S2.
static unsigned int bothButtonHeldCount = 0;

Timer_Handle intervalTimer, blinkTimer, debounceTimer;
Timer_Params timer_params;

/* Timer Callback Functions */

/*  ======== intervalCounter_timerCallback ========
 * Callback function for intervalCounter.
 * This is the 4-Second Timeout.
 * This is also used to record interval times between presses of S1.
 */
void intervalTimer_Callback(Timer_Handle myHandle)
{
    counting = false;               // Stop counting intervals.
    numIntervals = 0;               // Reset button S1 tap counts
    Timer_stop(intervalTimer);      // Stops itself after 4 seconds of inactivity on S1.
}

/*  ======== blinkTimer_Callback ========
 * blinkTimer Callback Function
 * This posts a semaphore to ledThread telling the LEDs to toggle ON/OFF.
 */
void blinkTimer_Callback(Timer_Handle myHandle)
{
    int retc;
    LedMsg msg;

    msg.cmd = LedCmd_TOGGLE;
    retc = mq_send(mqLED, (char *)&msg, sizeof(msg), 0);
    if (retc == -1) {
        // Failed to send to message queue
    }
    retc = sem_post(&semLED);
    if (retc == -1) {
        while (1);
    }

    if (inTempMode)
    {
        int retc;
        retc = sem_post(&semTemp);
        if (retc == -1) {
            while (1);
        }
    }
}

/*
 *  ======== debounceTimer_Callback ========
 * debounceTimer Callback Function
 * This checks the push button states and resets the button debounces when appropriate.
 */
void debounceTimer_Callback(Timer_Handle myHandle)
{
    if(GPIO_read(CONFIG_GPIO_BUTTON0))
    {
        GPIO_clearInt(CONFIG_GPIO_BUTTON0);
        GPIO_enableInt(CONFIG_GPIO_BUTTON0);
        S1buttonDebounce = 0;
    }
    if(GPIO_read(CONFIG_GPIO_BUTTON1))
    {
        GPIO_clearInt(CONFIG_GPIO_BUTTON1);
        GPIO_enableInt(CONFIG_GPIO_BUTTON1);
        S2buttonDebounce = 0;
    }
    if (GPIO_read(CONFIG_GPIO_BUTTON0) && GPIO_read(CONFIG_GPIO_BUTTON1))
    {
        GPIO_write(CONFIG_GPIO_LED0, CONFIG_GPIO_LED_OFF);
        Timer_stop(debounceTimer);

        int rc;
        LcdMsg msg;

        /* Send message to scroll string on LCD */
        msg.cmd = LcdCmd_SCROLL_STRING;

        if (enteringIntTempMode)
        {
            // Switch to using internal temperature sensor
            enteringIntTempMode = false;
            enteringTempMode = false;
            externaltempSensor = false;
            inTempMode = true;

            msg.buffer = "USING INTERNAL TEMP SENSOR";

            rc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
            if (rc == -1) {
                // Failed to send to message queue
            }
            rc = sem_post(&semLCD);
            if (rc == -1) {
                while (1);
            }
        }
        else if (enteringExtTempMode)
        {
            // Switch to using external temperature sensor
            enteringExtTempMode = false;
            enteringTempMode = false;
            externaltempSensor = true;
            inTempMode = true;

            msg.buffer = "USING EXTERNAL TEMP SENSOR";

            rc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
            if (rc == -1) {
                // Failed to send to message queue
            }
            rc = sem_post(&semLCD);
            if (rc == -1) {
                while (1);
            }
        }
    }
    if (!(GPIO_read(CONFIG_GPIO_BUTTON0) || GPIO_read(CONFIG_GPIO_BUTTON1)))
    {
        // Enter temperature mode
        if (!enteringTempMode)
        {
            bothButtonHeldCount++;
            // While both buttons are held, display temperature every 1 seconds
            if (bothButtonHeldCount == 50)
            {
                if (inTempMode)
                {
                    // Exit Temperature Mode
                    inTempMode = false;
                    enteringTempMode = false;
                    enteringIntTempMode = false;
                    enteringExtTempMode = false;
                    scrolling = false;

                    int rc;
                    LcdMsg msg;

                    /* Send message to scroll string on LCD */
                    msg.cmd = LcdCmd_SCROLL_STRING;
                    msg.buffer = "EXITING TEMPERATURE MODE";
                    rc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
                    if (rc == -1) {
                        // Failed to send to message queue
                    }
                    rc = sem_post(&semLCD);
                    if (rc == -1) {
                        while (1);
                    }
                }
                else
                {
                    // Enter Temperature Mode
                    enteringTempMode = true;
                    scrolling = false;

                    int rc;
                    LcdMsg msg;

                    /* Send message to scroll string on LCD */
                    msg.cmd = LcdCmd_SCROLL_STRING;
                    msg.buffer = "ENTERING TEMPERATURE MODE      PRESS LEFT BUTTON FOR EXTERNAL SENSOR      PRESS RIGHT BUTTON FOR INTERNAL SENSOR";
                    rc = mq_send(mqLCD, (char *)&msg, sizeof(msg), 0);
                    if (rc == -1) {
                        // Failed to send to message queue
                    }
                    rc = sem_post(&semLCD);
                    if (rc == -1) {
                        while (1);
                    }
                }
            }
        }
    }
}

/*
 *  ======== debounceGPIO ========
 *  Called by GPIO callback functions to begin Debounce procedure.
 */
void debounceGPIO(uint_least8_t index)
{
    GPIO_write(CONFIG_GPIO_LED0, CONFIG_GPIO_LED_ON);

    /* Stop/Close the DebounceTimer.
     * Ensure timer_params are set correctly (could have changed).
     * Open/Start DebounceTimer with proper settings.
     */
    Timer_close(debounceTimer);
    timer_params.period = 20000;
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = debounceTimer_Callback;
    debounceTimer = Timer_open(CONFIG_TIMER3, &timer_params);
    if (debounceTimer == NULL) {
        /* Failed to initialized Timer */
        while(1);
    }
    Timer_start(debounceTimer);
}

/*
 *  ======== gpioButton0_Callback ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON0.
 *  When S1 is pressed, this function updates the blinkRate controlled by
 *  the period of the timer32.
 */
void gpioButton0_Callback(uint_least8_t index)
{
    if(S1buttonDebounce == 0)
    {
        S1buttonDebounce = 1;
        bothButtonHeldCount = 0;
        scrolling = false;

        if (enteringTempMode || inTempMode)
        {
            enteringExtTempMode = true;
        }
        else
        {
            int retc;
            LedMsg msg;

            if(counting)    // Only try to capture interval times after second press since last timeout.
            {
                timerValueOnTap = Timer_getCount(intervalTimer);

                /* Send message to signal tap */
                msg.cmd    = LedCmd_BLINK_RATE;
                msg.buffer = (void *)(timerValueOnTap);
                retc = mq_send(mqLED, (char *)&msg, sizeof(msg), 0);
                if (retc == -1) {
                    while (1);
                }
                retc = sem_post(&semLED);
                if (retc == -1) {
                    while (1);
                }
            }
            else        // This is the first press since 4-second timeout event. There is no interval to record yet.
            {
                counting = true;
            }

            /* Restart the 4-Second timeout.  Also restarts for interval capture. */
            Timer_close(intervalTimer);
            timer_params.period = 4000000;
            timer_params.periodUnits = Timer_PERIOD_US;
            timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
            timer_params.timerCallback = intervalTimer_Callback;
            intervalTimer = Timer_open(CONFIG_TIMER0, &timer_params);
            if (intervalTimer == NULL) {
                /* Failed to initialized Timer */
                while(1);
            }
            Timer_start(intervalTimer);
        }

        /* Debounce Function */
        debounceGPIO(CONFIG_GPIO_BUTTON0);
    }
}

/*
 *  ======== gpioButton1_Callback ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON1.
 *  When S2 is pressed, change current color selection
 *  Post to semaphore that PWM should be updated.
 */
void gpioButton1_Callback(uint_least8_t index)
{
    if(S2buttonDebounce == 0)
    {
        S2buttonDebounce = 1;
        bothButtonHeldCount = 0;
        scrolling = false;

        if (enteringTempMode || inTempMode)
        {
            enteringIntTempMode = true;
        }
        else
        {
            int retc;
            LedMsg msg;

            /* Send message to change color */
            msg.cmd = LedCmd_COLOR_MODE;
            retc = mq_send(mqLED, (char *)&msg, sizeof(msg), 0);
            if (retc == -1) {
                while (1);
            }
            retc = sem_post(&semLED);
            if (retc == -1) {
                while (1);
            }
        }

        /* Debounce Function */
        debounceGPIO(CONFIG_GPIO_BUTTON1);
    }
}
