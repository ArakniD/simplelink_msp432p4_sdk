/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
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
 *  ======== compress_thread.c ========
 */
#include <ti/devices/msp432p4xx/inc/msp432.h>

/* Standard Includes */
#include <stdint.h>

/* POSIX Header files */
#include <semaphore.h>

/* Driver Header files */
#include <ti/display/Display.h>
#include <ti/drivers/NVS.h>

/* LZ4 Header files */
#include <ti/compression/lz4/lz4.h>

/* Board Header file */
#include "Board.h"

/* Local Header files */
#include "compress_thread.h"
#include "sensor_thread.h"

/* LZ4 Compression Parameters */
#define DATA_SIZE       (sizeof(packets))
#define HASHSIZE        4096
#define HASHLOG2SIZE    12

uint8_t hashMemory[HASHSIZE] = {0};
uint8_t compressedPackets[LZ4_COMPRESS_MAX_SIZE(DATA_SIZE)] = {0};
volatile uint32_t compressedSize;

void *compressThread(void *arg0)
{
    /* NVS parameters */
    NVS_Handle nvsHandle;
    NVS_Attrs regionAttrs;
    NVS_Params nvsParams;

    NVS_init();
    NVS_Params_init(&nvsParams);
    nvsHandle = NVS_open(Board_NVSINTERNAL, &nvsParams);
    if (nvsHandle == NULL) {
        while (1);
    }
    NVS_getAttrs(nvsHandle, &regionAttrs);

    /* LZ4 compression parameters */
    LZ4_status status;
    LZ4_compressParams compressParams;

    compressParams.src = (const void *)packets;
    compressParams.dst = (void *)compressedPackets;
    compressParams.length = DATA_SIZE;
    compressParams.hashTable = (void *)hashMemory;
    compressParams.hashLog2Size = HASHLOG2SIZE;
    compressParams.addBlockChecksum = false;
    compressParams.addContentChecksum = true;
    compressParams.addContentSize = true;

    /* Wait until data logging finishes, then compress the data and write it to flash */
    while (1) {
        sem_wait(&semCompress);

        Display_printf(displayHandle, 0, 0, "Compressing 32 sensor data packets...");
        compressedSize = LZ4_compress(&compressParams, &status);
        Display_printf(displayHandle, 0, 0, "Original size: %d    Compressed size: %d", DATA_SIZE, compressedSize);

        /* Write the compressed data to flash (persists through power cycles)  */
        /* 16,384 bytes are allocated for NVS starting at base address 0x3B000 */
        Display_printf(displayHandle, 0, 0, "Writing compressed data to flash...\n");
        NVS_write(nvsHandle, 0, (void *) compressedPackets, sizeof(compressedPackets),
            NVS_WRITE_ERASE | NVS_WRITE_POST_VERIFY);
    }
}
