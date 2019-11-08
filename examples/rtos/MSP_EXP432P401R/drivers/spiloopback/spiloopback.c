/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
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
 *  ======== spiloopback.c ========
 */
#include <string.h>
#include <stdint.h>
#include <stddef.h>

/* POSIX Header files */
#include <pthread.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/display/Display.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#define SPI_MSG_LENGTH  50

#define THREADSTACKSIZE 1024

#define MAX_LOOP 5

#define MASTER_MSG  "Hello from master, msg#: "
#define SLAVE_MSG   "Hello from slave, msg#: "

static pthread_barrier_t barrier;
static Display_Handle display;

unsigned char masterRxBuffer[SPI_MSG_LENGTH];
unsigned char masterTxBuffer[SPI_MSG_LENGTH];

unsigned char slaveRxBuffer[SPI_MSG_LENGTH];
unsigned char slaveTxBuffer[SPI_MSG_LENGTH];

/*  ======== slaveThread ========
 *  This thread runs on a higher priority, since slave
 *  has to be ready for master. Slave SPI sends a
 *  message to master and also receives message from
 *  master.
 */
void *slaveThread(void *arg0)
{
    SPI_Handle slaveSpi;
    SPI_Params slaveSpiParams;
    SPI_Transaction slaveTransaction;
    int i;
    bool transferOK;

    /* Initialize SPI handle with slave mode */
    SPI_Params_init(&slaveSpiParams);
    slaveSpiParams.frameFormat = SPI_POL0_PHA1;
    slaveSpiParams.mode = SPI_SLAVE;

    slaveSpi = SPI_open(CONFIG_SPI_1, &slaveSpiParams);
    if (slaveSpi == NULL) {
        Display_printf(display, 0, 0, "Error initializing slave SPI\n");
        while (1);
    }
    else {
        Display_printf(display, 0, 0, "Slave SPI initialized\n");
    }

    /* Wait for master to open spi */
    pthread_barrier_wait(&barrier);

    strncpy((char *)slaveTxBuffer, SLAVE_MSG, SPI_MSG_LENGTH);

    for (i = 0; i < MAX_LOOP; i++) {
        /* Initialize slave SPI transaction structure */
        slaveTxBuffer[sizeof(SLAVE_MSG) - 1] = (i % 10) + '0';
        slaveTransaction.count = SPI_MSG_LENGTH;
        slaveTransaction.txBuf = (void *)slaveTxBuffer;
        slaveTransaction.rxBuf = (void *)slaveRxBuffer;

        /* Initiate SPI transfer */
        transferOK = SPI_transfer(slaveSpi, &slaveTransaction);

        if (transferOK) {
            /* Print contents of slave receive buffer */
            Display_printf(display, 0, 0, "Slave: %s\n", slaveRxBuffer);
        }
        else {
            Display_printf(display, 0, 0, "Unsuccessful slave SPI transfer");
        }
    }

    /* Deinitialize SPI */
    SPI_close(slaveSpi);

    return (NULL);
}

/*
 *  ======== masterThread ========
 *  This thread runs at a lower priority after the slave
 *  task to ensure it is ready for a transaction.
 *  Master SPI sends a message to slave and also
 *  receives message from slave.
 */
void *masterThread(void *arg0)
{
    SPI_Handle masterSpi;
    SPI_Params masterSpiParams;
    SPI_Transaction masterTransaction;
    int i;
    bool transferOK;

    /* Initialize SPI handle as default master */
    SPI_Params_init(&masterSpiParams);
    masterSpiParams.frameFormat = SPI_POL0_PHA1;
    masterSpi = SPI_open(CONFIG_SPI_0, &masterSpiParams);
    if (masterSpi == NULL) {
        Display_printf(display, 0, 0, "Error initializing master SPI\n");
        while (1);
    }
    else {
        Display_printf(display, 0, 0, "Master SPI initialized\n");
    }

    pthread_barrier_wait(&barrier);

    strncpy((char *)masterTxBuffer, MASTER_MSG, SPI_MSG_LENGTH);

    for (i = 0; i < MAX_LOOP; i++) {
        /* Initialize master SPI transaction structure */
        masterTxBuffer[sizeof(MASTER_MSG) - 1] = (i % 10) + '0';
        masterTransaction.count = SPI_MSG_LENGTH;
        masterTransaction.txBuf = (void *)masterTxBuffer;
        masterTransaction.rxBuf = (void *)masterRxBuffer;

        /* Turn on user LED, indicating a SPI transfer is in progress */
        GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON);

        /* Initiate SPI transfer */
        transferOK = SPI_transfer(masterSpi, &masterTransaction);

        if (transferOK) {
            /* Print contents of master receive buffer */
            Display_printf(display, 0, 0, "Master: %s\n", masterRxBuffer);
        }
        else {
            Display_printf(display, 0, 0, "Unsuccessful master SPI transfer");
        }

        /* Sleep a short time to allow the LED to flash */
        usleep(500000);

        /* Turn off user LED, indicating the SPI transfer is done */
        GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF);

        /* Sleep for a bit before starting the next SPI transfer  */
        sleep(3);
    }

    /* Deinitialize SPI */
    SPI_close(masterSpi);

    Display_printf(display, 0, 0, "Done\n");

    return (NULL);
}
/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    pthread_t           thread0, thread1;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    /* Call driver init functions. */
    Display_init();
    GPIO_init();
    SPI_init();

    /* Configure the LED pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /*
     *  Create a barrier to allow the master task to open its SPI
     *  handle before the slave task issues a SPI transfer.  Since the
     *  pin muxing is done in the call to SPI_open(), we need to make
     *  sure all pins are configured before the first transfer.  This
     *  will prevent the possibility of erroneous data being transfered.
     */
    pthread_barrier_init(&barrier, NULL, 2);

    /* Open the display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        /* Failed to open display driver */
        while (1);
    }

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    Display_printf(display, 0, 0, "Starting the SPI loop-back example\n");

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

    priParam.sched_priority = 2;
    pthread_attr_setschedparam(&attrs, &priParam);

    /* Create slave thread */
    retc = pthread_create(&thread1, &attrs, slaveThread, (void* )0);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    return (NULL);
}
