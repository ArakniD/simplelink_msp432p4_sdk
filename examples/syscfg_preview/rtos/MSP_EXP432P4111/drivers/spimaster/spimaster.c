/*
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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
 *  ======== spimaster.c ========
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/display/Display.h>

/* Example/Board Header files */
#include "Board.h"

#define THREADSTACKSIZE (1024)

#define SPI_MSG_LENGTH  (30)
#define MASTER_MSG      ("Hello from master, msg#: ")

#define MAX_LOOP        (10)

static Display_Handle display;

unsigned char masterRxBuffer[SPI_MSG_LENGTH];
unsigned char masterTxBuffer[SPI_MSG_LENGTH];

/* Semaphore to block master until slave is ready for transfer */
sem_t masterSem;

/*
 *  ======== slaveReadyFxn ========
 *  Callback function for the GPIO interrupt on Board_SPI_SLAVE_READY.
 */
void slaveReadyFxn(uint_least8_t index)
{
    sem_post(&masterSem);
}

/*
 *  ======== masterThread ========
 *  Master SPI sends a message to slave while simultaneously receiving a
 *  message from the slave.
 */
void *masterThread(void *arg0)
{
    SPI_Handle      masterSpi;
    SPI_Params      spiParams;
    SPI_Transaction transaction;
    uint32_t        i;
    bool            transferOK;
    int32_t         status;

    /*
     * Board_SPI_MASTER_READY & Board_SPI_SLAVE_READY are GPIO pins connected
     * between the master & slave.  These pins are used to synchronize
     * the master & slave applications via a small 'handshake'.  The pins
     * are later used to synchronize transfers & ensure the master will not
     * start a transfer until the slave is ready.  These pins behave
     * differently between spimaster & spislave examples:
     *
     * spimaster example:
     *     * Board_SPI_MASTER_READY is configured as an output pin.  During the
     *       'handshake' this pin is changed from low to high output.  This
     *       notifies the slave the master is ready to run the application.
     *       Afterwards, the pin is used by the master to notify the slave it
     *       has opened Board_SPI_MASTER.  When Board_SPI_MASTER is opened, this
     *       pin will be pulled low.
     *
     *     * Board_SPI_SLAVE_READY is configured as an input pin. During the
     *       'handshake' this pin is read & a high value will indicate the slave
     *       ready to run the application.  Afterwards, a falling edge interrupt
     *       will be configured on this pin.  When the slave is ready to perform
     *       a transfer, it will pull this pin low.
     *
     * Below we set Board_SPI_MASTER_READY & Board_SPI_SLAVE_READY initial
     * conditions for the 'handshake'.
     */
    GPIO_setConfig(Board_SPI_MASTER_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_SPI_SLAVE_READY, GPIO_CFG_INPUT);

    /*
     * Handshake - Set Board_SPI_MASTER_READY high to indicate master is ready
     * to run.  Wait Board_SPI_SLAVE_READY to be high.
     */
    GPIO_write(Board_SPI_MASTER_READY, 1);
    while (GPIO_read(Board_SPI_SLAVE_READY) == 0) {}

    /* Handshake complete; now configure interrupt on Board_SPI_SLAVE_READY */
    GPIO_setConfig(Board_SPI_SLAVE_READY, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setCallback(Board_SPI_SLAVE_READY, slaveReadyFxn);
    GPIO_enableInt(Board_SPI_SLAVE_READY);

    /*
     * Create synchronization semaphore; the master will wait on this semaphore
     * until the slave is ready.
     */
    status = sem_init(&masterSem, 0, 0);
    if (status != 0) {
        Display_printf(display, 0, 0, "Error creating masterSem\n");

        while(1);
    }

    /* Open SPI as master (default) */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_POL0_PHA1;
    masterSpi = SPI_open(Board_SPI_MASTER, &spiParams);
    if (masterSpi == NULL) {
        Display_printf(display, 0, 0, "Error initializing master SPI\n");
        while (1);
    }
    else {
        Display_printf(display, 0, 0, "Master SPI initialized\n");
    }

    /*
     * Master has opened Board_SPI_MASTER; set Board_SPI_MASTER_READY high to
     * inform the slave.
     */
    GPIO_write(Board_SPI_MASTER_READY, 0);

    /* Copy message to transmit buffer */
    strncpy((char *) masterTxBuffer, MASTER_MSG, SPI_MSG_LENGTH);

    for (i = 0; i < MAX_LOOP; i++) {
        /*
         * Wait until slave is ready for transfer; slave will pull
         * Board_SPI_SLAVE_READY low.
         */
        sem_wait(&masterSem);

        /* Initialize master SPI transaction structure */
        masterTxBuffer[sizeof(MASTER_MSG) - 1] = (i % 10) + '0';
        memset((void *) masterRxBuffer, 0, SPI_MSG_LENGTH);
        transaction.count = SPI_MSG_LENGTH;
        transaction.txBuf = (void *) masterTxBuffer;
        transaction.rxBuf = (void *) masterRxBuffer;

        /* Toggle user LED, indicating a SPI transfer is in progress */
        GPIO_toggle(Board_GPIO_LED1);

        /* Perform SPI transfer */
        transferOK = SPI_transfer(masterSpi, &transaction);
        if (transferOK) {
            Display_printf(display, 0, 0, "Master received: %s", masterRxBuffer);
        }
        else {
            Display_printf(display, 0, 0, "Unsuccessful master SPI transfer");
        }

        /* Sleep for a bit before starting the next SPI transfer  */
        sleep(3);
    }

    SPI_close(masterSpi);

    /* Example complete - set pins to a known state */
    GPIO_disableInt(Board_SPI_SLAVE_READY);
    GPIO_setConfig(Board_SPI_SLAVE_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
    GPIO_write(Board_SPI_MASTER_READY, 0);

    Display_printf(display, 0, 0, "\nDone");

    return (NULL);
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    pthread_t           thread0;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    /* Call driver init functions. */
    Display_init();
    GPIO_init();
    SPI_init();

    /* Configure the LED pins */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_LED1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Open the display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        /* Failed to open display driver */
        while (1);
    }

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    Display_printf(display, 0, 0, "Starting the SPI master example");
    Display_printf(display, 0, 0, "This example requires external wires to be "
        "connected to the header pins. Please see the Board.html for details.\n");

    /* Create application threads */
    pthread_attr_init(&attrs);

    detachState = PTHREAD_CREATE_DETACHED;
    /* Set priority and stack size attributes */
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    /* Create master thread */
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread0, &attrs, masterThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    return (NULL);
}
