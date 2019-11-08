/*
 * Copyright (c) 2017-2019, Texas Instruments Incorporated
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
 *  ======== powerperformance.c ========
 */

/* Board Header file */
#include "ti_drivers_config.h"

/* Required when including driverlib header files*/
#ifdef CONFIG_MSP_EXP432P401R
#ifndef __MSP432P401R__
#define __MSP432P401R__
#endif
#elif defined(CONFIG_MSP_EXP432P4111)
#ifndef __MSP432P4111__
#define __MSP432P4111__
#endif
#endif

/* Driver Header files */
#include <ti/drivers/Power.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/power/PowerMSP432.h>

/* Display */
#include <ti/display/Display.h>
static Display_Handle display;

/* Driverlib Header files */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

unsigned int notifyFxn(unsigned int eventType, unsigned int eventArg,
    unsigned int clientArg);
Power_NotifyObj notifyObj;

void runPerfLevel(unsigned int level);

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    Board_init();
    GPIO_init();
    Display_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Open the display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        /* Failed to open display driver */
        while (1);
    }

    Display_printf(display, 0, 0, "Initial perf level = %d", Power_getPerformanceLevel());
    Display_printf(display, 0, 0, "MCLK = %d", CS_getMCLK());
    Display_printf(display, 0, 0, "HSMCLK = %d", CS_getHSMCLK());
    Display_printf(display, 0, 0, "SMCLK = %d", CS_getSMCLK());
    Display_printf(display, 0, 0, "ACLK = %d", CS_getACLK());

    /* register for start/done change performance level notifications */
    Power_registerNotify(&notifyObj, (PowerMSP432_START_CHANGE_PERF_LEVEL |
    PowerMSP432_DONE_CHANGE_PERF_LEVEL), (Power_NotifyFxn)notifyFxn, 0);

    runPerfLevel(1);
    runPerfLevel(0);
    runPerfLevel(3);
    runPerfLevel(2);

    while(1);
}

/*
 *  ======== runPerfLevel ========
 */
void runPerfLevel(unsigned int level)
{
    unsigned int status;

    status = Power_setPerformanceLevel(level);

    Display_printf(display, 0, 0, "= = = = = = = = =");
    Display_printf(display, 0, 0, "Set perf %d status = %d", level, status);
    Display_printf(display, 0, 0, "Perf level = %d", Power_getPerformanceLevel());
    Display_printf(display, 0, 0, "MCLK = %d", CS_getMCLK());
    Display_printf(display, 0, 0, "HSMCLK = %d", CS_getHSMCLK());
    Display_printf(display, 0, 0, "SMCLK = %d", CS_getSMCLK());
    Display_printf(display, 0, 0, "ACLK = %d", CS_getACLK());
}

/*
 *  ======== notifyFxn ========
 */
unsigned int notifyFxn(unsigned int eventType, unsigned int eventArg,
 unsigned int clientArg)
{
    if(eventType == PowerMSP432_START_CHANGE_PERF_LEVEL) {
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
    }
    else {
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
    }
    return(Power_NOTIFYDONE);
}
