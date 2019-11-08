/*
 * Copyright (c) 2016-2017, Texas Instruments Incorporated
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

/* Standard Defines */
#include <stdbool.h>
#include <stdlib.h>

/* TI Defines */
#include <semaphore.h>
#include <pthread.h>

/* Driver Header files */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>
#include <ti/drivers/GPIO.h>

/* Driverlib Header files */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Board Header file */
#include "sleep_task.h"
/* Board Header file */
#include "ti_drivers_config.h"


/*******************************************************************************
 *                             LOCAL VARIABLES
 ******************************************************************************/
pthread_t sleepTask;

/*******************************************************************************
 *                                  LOCAL FUNCTIONS
 ******************************************************************************/
static void GPIO_irqHandler(void);
static void *SLEEP_taskFxn(void *arg0);

/*******************************************************************************
 *                                 PUBLIC FUNCTIONS
 ******************************************************************************/
/*******************************************************************************
 * @fn      SLEEP_createTask
 *
 * @brief   Task creation function to send the device into sleep mode
 *
 * @param   None.
 *
 * @return  None.
 ******************************************************************************/
void SLEEP_createTask(void)
{
    pthread_attr_t pAttrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    pthread_attr_init(&pAttrs);
    priParam.sched_priority = SLEEP_TASK_PRIORITY;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);

    if (retc != 0)
    {
        while (1);
    }

    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc |= pthread_attr_setstacksize(&pAttrs, SLEEP_STACK_SIZE);

    if (retc != 0)
    {
        while (1);
    }

    /* Creating the tasks */
    retc = pthread_create(&sleepTask, &pAttrs, SLEEP_taskFxn,
                          NULL);
    if (retc != 0)
    {
        while (1);
    }
}

/*******************************************************************************
 * @fn      SLEEP_taskFxn
 *
 * @brief   Application task entry point for managing the sleep transitions
 *
 * @param   a0, a1 - not used.
 *
 * @return  None.
 ******************************************************************************/
static void *SLEEP_taskFxn(void *arg0)
{
    GPIO_write(CONFIG_GPIO_LED1, CONFIG_GPIO_LED_ON);

    GPIO_setCallback(CONFIG_GPIO_BUTTON0, (GPIO_CallbackFxn)GPIO_irqHandler);
    GPIO_enableInt(CONFIG_GPIO_BUTTON0);

    return (NULL);
}

/*******************************************************************************
 * @fn      GPIO_irqHandler
 *
 * @brief   Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON0.
 *
 * @param   None.
 *
 * @return  None.
 ******************************************************************************/
static void GPIO_irqHandler(void)
{
    GPIO_clearInt(CONFIG_GPIO_BUTTON0);
    GPIO_disableInt(CONFIG_GPIO_BUTTON0);
    GPIO_write(CONFIG_GPIO_LED1, CONFIG_GPIO_LED_OFF);

#ifdef __MSP_EXP432P4111_H
    /* Set P5.3 as analog input (connected to Temp Sensor on MSP_EXP432P4111 LaunchPad) */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN3, GPIO_TERTIARY_MODULE_FUNCTION);
#endif

    /* Turn off PSS high-side supervisors to consume lower power after deep sleep */
    MAP_PSS_disableHighSide();

    /* Select PowerMSP432_deepSleepPolicy to enable transition
     * into the PowerMSP432_DEEPSLEEP_1 (LPM4)state
     */
    Power_setConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_1);
    Power_setPolicy((Power_PolicyFxn)PowerMSP432_deepSleepPolicy);

    /* Refer to Device datasheet for Wake-up Sources From Low Power Modes to wake up from
     * PowerMSP432_DEEPSLEEP_1
     */
}
