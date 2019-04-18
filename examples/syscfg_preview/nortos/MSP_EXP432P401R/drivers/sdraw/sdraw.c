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
 *  ======== sdraw.c ========
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SD.h>
#include <ti/display/Display.h>

/* Example/Board Header files */
#include "Board.h"

/* Buffer size used for the file copy process */
#define BUFFSIZE 1024

 /* Starting sector to write/read to */
#define STARTINGSECTOR 0

#define BYTESPERKILOBYTE 1024

/*
 * Set this constant to 1 in order to write to the SD card.
 * WARNING: Running this example with WRITEENABLE set to 1 will cause
 * any filesystem present on the SD card to be corrupted!
 */
#define WRITEENABLE 0

static Display_Handle display;

unsigned char textarray[BUFFSIZE];

unsigned char cpy_buff[BUFFSIZE];

/*
 *  ======== mainThread ========
 *  Task to perform a raw write and read from the SD card.
 *  Note: Running this application will cause any filesystem on the
 *      SD card to become corrupted!
 */
void *mainThread(void *arg0)
{
    int_fast8_t   result;
    uint_fast32_t cardCapacity;
    uint_fast32_t totalSectors;
    uint_fast32_t sectorSize;
    uint_fast32_t sectors;
    int           i;
    SD_Handle     sdHandle;

    Display_init();
    GPIO_init();
    SD_init();

    /* Configure the LED pin */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Open the display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        /* Failed to open display driver */
        while (1);
    }

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    Display_printf(display, 0, 0, "Starting the SD example\n");

    /* Initialize the array to write to the SD card */
    for (i = 0; i < BUFFSIZE; i++) {
        textarray[i] = i & 0xFF;
    }

    /* Mount and register the SD Card */
    sdHandle = SD_open(Board_SD0, NULL);
    if (sdHandle == NULL) {
        Display_printf(display, 0, 0, "Error starting the SD card\n");
        while (1);
    }

    result = SD_initialize(sdHandle);
    if (result != SD_STATUS_SUCCESS) {
        Display_printf(display, 0, 0, "Error initializing the SD card\n");
        while (1);
    }

    totalSectors = SD_getNumSectors(sdHandle);
    sectorSize = SD_getSectorSize(sdHandle);
    cardCapacity = (totalSectors / BYTESPERKILOBYTE) * sectorSize;

    Display_printf(display, 0, 0,
        "There are %u total sectors on the SD card.\n", totalSectors);
    Display_printf(display, 0, 0,
        "The Read/Write sector size is %u bytes\n", sectorSize);
    Display_printf(display, 0, 0,
        "The total card capacity is %u KB\n", cardCapacity);

    /* Calculate number of sectors taken up by the array by rounding up */
    sectors = (sizeof(textarray) + sectorSize - 1) / sectorSize;

#if (WRITEENABLE)
    Display_printf(display, 0, 0, "Writing the array...\n");

    result = SD_write(sdHandle, textarray, STARTINGSECTOR, sectors);
    if (result != SD_STATUS_SUCCESS) {
        Display_printf(display, 0, 0, "Error writing to the SD card\n");
        while (1);
    }
#endif

    Display_printf(display, 0, 0, "Reading the array...\n");
    result = SD_read(sdHandle, cpy_buff, STARTINGSECTOR, sectors);
    if (result != SD_STATUS_SUCCESS) {
        Display_printf(display, 0, 0, "Error reading from the SD card\n");
        while (1);
    }

    /* Compare data read from the SD card with expected values */
    for (i = 0; i < BUFFSIZE; i++) {
        if (cpy_buff[i] != textarray[i]) {
            Display_printf(display, 0, 0,
                    "Data read from SD card differed from expected value\n");
            Display_printf(display, 0, 0,
                    "    Expected value for index %d: %d, got %d\n", i,
                    textarray[i], cpy_buff[i]);
            Display_printf(display, 0, 0, "Run the example with WRITEENABLE "
                    "= 1 to write expected values to the SD card\n");
            break;
        }
    }

    if (i == BUFFSIZE) {
        Display_printf(display, 0, 0,
                "Data read from SD card matched expected values\n");
    }

    SD_close(sdHandle);

    return (NULL);
}
