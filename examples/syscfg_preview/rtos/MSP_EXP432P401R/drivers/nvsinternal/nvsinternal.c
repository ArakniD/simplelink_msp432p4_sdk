/*
 * Copyright (c) 2017-2018, Texas Instruments Incorporated
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
 *  ======== nvsinternal.c ========
 */

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/* Driver Header files */
#include <ti/display/Display.h>
#include <ti/drivers/NVS.h>

/* Example/Board Header files */
#include "Board.h"

#define FOOTER "=================================================="

/* Buffer placed in RAM to hold bytes read from non-volatile storage. */
static char buffer[64];

/*
 * Some devices have a minimum FLASH write size of 4-bytes (1 word). Trying
 * to write a non-multiple of 4 amount of data will fail. This array is
 * rounded up (to next multiple of 4) to meet this requirement. Refer to NVS
 * driver documentation for more details.
 */
static const char signature[52] =
    {"SimpleLink SDK Non-Volatile Storage (NVS) Example."};

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    NVS_Handle nvsHandle;
    NVS_Attrs regionAttrs;
    NVS_Params nvsParams;

    Display_Handle displayHandle;

    Display_init();
    NVS_init();

    displayHandle = Display_open(Display_Type_UART, NULL);
    if (displayHandle == NULL) {
        /* Display_open() failed */
        while (1);
    }

    NVS_Params_init(&nvsParams);
    nvsHandle = NVS_open(Board_NVSINTERNAL, &nvsParams);

    if (nvsHandle == NULL) {
        Display_printf(displayHandle, 0, 0, "NVS_open() failed.");

        return (NULL);
    }

    Display_printf(displayHandle, 0, 0, "\n");

    /*
     * This will populate a NVS_Attrs structure with properties specific
     * to a NVS_Handle such as region base address, region size,
     * and sector size.
     */
    NVS_getAttrs(nvsHandle, &regionAttrs);

    /* Display the NVS region attributes */
    Display_printf(displayHandle, 0, 0, "Region Base Address: 0x%x",
            regionAttrs.regionBase);
    Display_printf(displayHandle, 0, 0, "Sector Size: 0x%x",
            regionAttrs.sectorSize);
    Display_printf(displayHandle, 0, 0, "Region Size: 0x%x\n",
            regionAttrs.regionSize);


    /*
     * Copy "sizeof(signature)" bytes from the NVS region base address into
     * buffer. An offset of 0 specifies the offset from region base address.
     * Therefore, the bytes are copied from regionAttrs.regionBase.
     */
    NVS_read(nvsHandle, 0, (void *) buffer, sizeof(signature));

    /*
     * Determine if the NVS region contains the signature string.
     * Compare the string with the contents copied into buffer.
     */
    if (strcmp((char *) buffer, (char *) signature) == 0) {

        /* Write signature directly from the NVS region to the UART console. */
        Display_printf(displayHandle, 0, 0, "%s", regionAttrs.regionBase);
        Display_printf(displayHandle, 0, 0, "Erasing flash sector...");

        /* Erase the entire flash sector. */
        NVS_erase(nvsHandle, 0, regionAttrs.sectorSize);
    }
    else {

        /* The signature was not found in the NVS region. */
        Display_printf(displayHandle, 0, 0, "Writing signature to flash...");

        /* Write signature to memory. The flash sector is erased prior
         * to performing the write operation. This is specified by
         * NVS_WRITE_ERASE.
         */
        NVS_write(nvsHandle, 0, (void *) signature, sizeof(signature),
            NVS_WRITE_ERASE | NVS_WRITE_POST_VERIFY);
    }

    Display_printf(displayHandle, 0, 0, "Reset the device.");
    Display_printf(displayHandle, 0, 0, FOOTER);

    return (NULL);
}
