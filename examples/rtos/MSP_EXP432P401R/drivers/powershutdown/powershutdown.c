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
 *  ======== powershutdown.c ========
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

/* Driverlib Header files */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

unsigned int notifyFxn(unsigned int eventType, unsigned int eventArg,
    unsigned int clientArg);
Power_NotifyObj notifyObj;

/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 */
void gpioButtonFxn0(uint_least8_t x)
{
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);

    /* Turn off PSS high-side supervisors to consume less power in shutdown */
    MAP_PSS_disableHighSide();

    /*
     *    PowerMSP432_SHUTDOWN_0 => PCM_LPM35_VCORE0
     *    PowerMSP432_SHUTDOWN_1 => PCM_LPM45
     */
    Power_shutdown(PowerMSP432_SHUTDOWN_1, 0);
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    Board_init();
    GPIO_init();

    /* Configure the LED and button pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

    /* Initialize interrupts for all ports that need them */
    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Register for entering shutdown notifications */
    Power_registerNotify(&notifyObj, PowerMSP432_ENTERING_SHUTDOWN,
        (Power_NotifyFxn) notifyFxn, 0);

    while(1);
}

/*
 *  ======== notifyFxn ========
 */
unsigned int notifyFxn(unsigned int eventType, unsigned int eventArg,
 unsigned int clientArg)
{
    /*
     * Put all GPIOs except for BUTTON0 (P1.1) in low power configuration
     * for shutdown. This is done using by MSP432-specific driverlib APIs
     */
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PA, (PIN_ALL16 & ~GPIO_PIN1));
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PE, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PJ, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PA, (PIN_ALL16 & ~GPIO_PIN1));
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PE, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PJ, PIN_ALL16);

#ifdef CONFIG_MSP_EXP432P4111
    /*
     * Set P5.3 as analog input (connected to Temp Sensor on MSP_EXP432P4111
     * LaunchPad)
     */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN3,
        GPIO_TERTIARY_MODULE_FUNCTION);
#endif

    return(Power_NOTIFYDONE);
}
