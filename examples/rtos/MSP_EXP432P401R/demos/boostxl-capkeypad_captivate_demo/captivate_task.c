/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
/*******************************************************************************
 *                              INCLUDES
 ******************************************************************************/
/* Standard Defines */
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* TI Defines */
#include <ti/display/Display.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <pthread.h>

/* Local Defines */
#include "npi_task.h"

#include "captivate_task.h"
#include "npi_message.h"

/*******************************************************************************
 *                                CONSTANTS
 ******************************************************************************/

/*******************************************************************************
 *                             LOCAL VARIABLES
 ******************************************************************************/
/* Task configuration and semaphores */
pthread_t captivateTask;

/* Buffer that contains the keypad information */
uint8_t captivateMsgBuffer[8];

/* Output display handle that will be used to print out all debug/log
 * statements */
extern Display_Handle displayOut;

/*******************************************************************************
 *                                  LOCAL FUNCTIONS
 ******************************************************************************/
static void CAPTIVATE_demoUpdate(void);

/*******************************************************************************
 *                                 PUBLIC FUNCTIONS
 ******************************************************************************/
/*******************************************************************************
 * @fn      CAPTIVATE_createTask
 *
 * @brief   Task creation function for the capTIvate handling
 *
 * @param   None.
 *
 * @return  None.
 ******************************************************************************/
void CAPTIVATE_createTask(void)
{
    pthread_attr_t pAttrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    pthread_attr_init(&pAttrs);
    priParam.sched_priority = CAPTIVATE_TASK_PRIORITY;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);

    if (retc != 0)
    {
        while (1);
    }

    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc |= pthread_attr_setstacksize(&pAttrs, CAPTIVATE_STACK_SIZE);

    if (retc != 0)
    {
        while (1);
    }

    /* Creating the task */
    retc = pthread_create(&captivateTask, &pAttrs, CAPTIVATE_taskFxn,
                          NULL);
    if (retc != 0)
    {
        while (1);
    }
}

/*******************************************************************************
 * @fn      CAPTIVATE_taskFxn
 *
 * @brief   Application task entry point for managing the capTIvate
 *          functionality
 *
 * @param   a0, a1 - not used.
 *
 * @return  None.
 ******************************************************************************/
void *CAPTIVATE_taskFxn(void *arg0)
{
    /* Write to the display terminal */
    Display_print0(displayOut, 0, 0,"--------- BOOSTXL CAPKEYPAD Example ---------");
    Display_print0(displayOut, 0, 0, "Waiting for CAPKEYPAD input... ");

	while (1)
	{
	    sem_wait(&npiGetDataSemaphore);
	    CAPTIVATE_demoUpdate();

	} /* end of while loop */
}

/*******************************************************************************
 * @fn      CAPTIVATE_demoUpdate
 *
 * @brief   Grab touch panel message and run the demo functions to act on it
 *
 * @param   None.
 *
 * @return  None.
 ******************************************************************************/
static void CAPTIVATE_demoUpdate(void)
{
    /* CapTIvate PayloadMessage                                                              */
    /*  [0]       [1]       [2]       [3]           [4]           [5]        [6]      [7]    */
    /* Keypad    Keypad #  Keypress  GuardChannel  GuardChannel  Proximity  Buttton  Button  */
    /* activity            Pressed   activity      active        activity   activity Pressed */
    /* flag                          flag                        flag       flag             */
    NPI_parsePayload(&rxMsg, &captivateMsgBuffer[0]);

    /* Keypad activity flag */
    if (captivateMsgBuffer[0])
    {
        /* Display Keypad pressed to the terminal */
        Display_print1(displayOut, 0, 0, "> %d",captivateMsgBuffer[1]);
    }

    /* Guard Channel and Proximity Sensor Activity flag */
    if (captivateMsgBuffer[3])
    {
    }

    /* Button activity flag */
    if (captivateMsgBuffer[6])
    {
    }
}
