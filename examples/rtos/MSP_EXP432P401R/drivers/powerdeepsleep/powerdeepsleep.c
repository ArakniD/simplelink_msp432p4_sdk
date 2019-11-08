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
 *  ======== powerdeepsleep.c ========
 */

/* Board Header file */
#include "ti_drivers_config.h"

/* Required when including driverlib header files */
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
#include <ti/drivers/power/PowerMSP432.h>
#include <ti/drivers/GPIO.h>

/* Driverlib Header files */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 */
void gpioButtonFxn0(uint_least8_t x)
{
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF);

    /*
     * Put all GPIOs in their lowest power configuration. This is done by
     * MSP432-specific driverlib calls
     */
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PA, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PE, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PJ, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PA, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PE, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PJ, PIN_ALL16);

#ifdef CONFIG_MSP_EXP432P4111
    /*
     * Set P5.3 as analog input (connected to Temp Sensor on
     * MSP_EXP432P4111 LaunchPad)
     */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5,
        GPIO_PIN3, GPIO_TERTIARY_MODULE_FUNCTION);
#endif

    /*
     * Turn off PSS high-side supervisors to consume lower power in deep sleep
     */
    MAP_PSS_disableHighSide();

    /* Select PowerMSP432_deepSleepPolicy to enable transition
     * into the PowerMSP432_DEEPSLEEP_1 (LPM4) state
     */
    Power_setPolicy((Power_PolicyFxn)PowerMSP432_deepSleepPolicy);

    /*
     * Refer to device datasheet for wake-up sources from LPM4
     */
}


/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    Board_init();
    GPIO_init();

    /* Configure the LED and button pins */
    GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON);

    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

    return NULL;
}
