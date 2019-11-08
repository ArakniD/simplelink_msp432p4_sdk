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
#include <ti/drivers/I2C.h>
#include <ti/drivers/GPIO.h>
/* Board Header file */
#include "ti_drivers_config.h"

#include "captivate_task.h"
#include "npi_task.h"
#include "npi_master.h"
#include "npi_message.h"

/*******************************************************************************
 *                                CONSTANTS
 ******************************************************************************/
/* NPI States */
typedef enum
{
    NPI_INIT,
    NPI_IDLE,
    NPI_READ,
    NPI_WRITE
} npi_States_t;

/*******************************************************************************
 *                             LOCAL VARIABLES
 ******************************************************************************/
pthread_t npiTask;
sem_t npiGetDataSemaphore;
sem_t npiWaitForData;
sem_t npiIrqSem;

npi_States_t npi_state;

npiMessage_t rxMsg;
npiMessage_t txMsg;

/* I2C captivate handle */
I2C_Handle i2cCaptivateHandle;

void NPI_irqHandler(uint_least8_t index);

/*******************************************************************************
 *                                 PUBLIC FUNCTIONS
 ******************************************************************************/
/*******************************************************************************
 * @fn      NPI_createTask
 *
 * @brief   Task creation function for the NPI handling
 *
 * @param   None.
 *
 * @return  None.
 ******************************************************************************/
void NPI_createTask(void)
{
    pthread_attr_t pAttrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    pthread_attr_init(&pAttrs);
    priParam.sched_priority = NPI_TASK_PRIORITY;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);

    if (retc != 0)
    {
        while (1);
    }

    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc |= pthread_attr_setstacksize(&pAttrs, NPI_STACK_SIZE);

    if (retc != 0)
    {
        while (1);
    }

    /* Creating the tasks */
    retc = pthread_create(&npiTask, &pAttrs, NPI_taskFxn,
                          NULL);
    if (retc != 0)
    {
        while (1);
    }

    sem_init(&npiGetDataSemaphore, 1, 0);
    sem_init(&npiWaitForData, 1, 0);
    sem_init(&npiIrqSem, 1, 0);
}

/*******************************************************************************
 * @fn      NPI_taskFxn
 *
 * @brief   Application task entry point for managing the NPI Messages
 *
 * @param   a0, a1 - not used.
 *
 * @return  None.
 ******************************************************************************/
void *NPI_taskFxn(void *arg0)
{
    I2C_Params i2cParams;

    /* Captivate IRQ init */
    GPIO_setCallback(CONFIG_CAPTIVATE_IRQ, (GPIO_CallbackFxn) NPI_irqHandler);
    GPIO_clearInt(CONFIG_CAPTIVATE_IRQ);
    GPIO_enableInt(CONFIG_CAPTIVATE_IRQ);

    /* Initialize I2C interface */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2cCaptivateHandle = I2C_open(CONFIG_I2C_CAPTIVATE, &i2cParams);

    while(1) {
        switch(npi_state) {
        case NPI_INIT:
            npi_state = NPI_IDLE;
            break;
        case NPI_IDLE:
            sem_wait(&npiIrqSem);
            npi_state = NPI_READ;
            break;

        case NPI_READ:
            NPIMaster_readMsg(&rxMsg);

            if (!NPI_validMsg(&rxMsg))
            {
                npi_state = NPI_IDLE;
                break;
            }

            if (rxMsg.type == CAPT_TYPE)
            {
                if (rxMsg.command == INFO_CMD)
                {
                    NPIMaster_setCaptInfo(&rxMsg);
                }
                else if (rxMsg.command == CAPT_MSG)
                {
                    /* Notify app task */
                    sem_post(&npiGetDataSemaphore);
                }
            }

            npi_state = NPI_IDLE;
            break;

        /* WRITE case currently not used */
        case NPI_WRITE:
            NPIMaster_writeMsg(&txMsg);
            npi_state = NPI_IDLE;
            break;
        default:
            break;
        }
    }
}

/*******************************************************************************
 * @fn      NPI_irqHandler
 *
 * @brief   Handler to wake up system, respond to IRQ/touch panel activity
 *
 * @param   None.
 *
 * @return  None.
 ******************************************************************************/
void NPI_irqHandler(uint_least8_t index)
{
    uint32_t delayDebounce = 0;

    GPIO_disableInt(CONFIG_CAPTIVATE_IRQ);

    /* Delay for switch debounce */
    for (delayDebounce = 0; delayDebounce < 100; delayDebounce++);

    sem_post(&npiIrqSem);
    GPIO_clearInt(CONFIG_CAPTIVATE_IRQ);
    GPIO_enableInt(CONFIG_CAPTIVATE_IRQ);
}
