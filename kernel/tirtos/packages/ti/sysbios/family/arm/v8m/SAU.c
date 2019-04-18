/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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
 *  ======== SAU.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Startup.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/family/arm/AsmMacros.h>

#include "package/internal/SAU.xdc.h"

/*
 *  ======== SAU_startup ========
 */
Void SAU_startup()
{
    UInt i;

    /* Disable the SAU (if already disabled, does nothing) */
    SAU_disable();

    /*
     * Initialize all SAU regions
     */
    for (i = 0; i < SAU_numRegions; i++) {
        SAU_deviceRegs.RNR = i;
        SAU_deviceRegs.RBAR = SAU_module->regionEntry[i].rbar;   
        SAU_deviceRegs.RLAR = SAU_module->regionEntry[i].rlar;   
    }

    if (SAU_allSecureWhenSauDisabled) {
        SAU_deviceRegs.CTRL = SAU_deviceRegs.CTRL & (~1);
    }
    else {
        SAU_deviceRegs.CTRL = SAU_deviceRegs.CTRL | 1;
    }

    if (SAU_enableSAU) {
        SAU_enable();
    }
}

/*
 *  ======== SAU_disable ========
 *  Function to disable the SAU.
 */
Void SAU_disable()
{
    UInt   key;

    if (!(SAU_isEnabled())) {
        return;
    }

    key = Hwi_disable();

    /* Ensure all memory transactions have completed */
    __DMB(sy);

    /* disables the SAU */
    SAU_deviceRegs.CTRL = SAU_deviceRegs.CTRL & (~0x1);

    Hwi_restore(key);
}

/*
 *  ======== SAU_enable ========
 *  Function to enable the SAU.
 */
Void SAU_enable()
{
    UInt   key;

    /* if SAU is already enabled then just return */
    if (SAU_isEnabled()) {
        return;
    }

    key = Hwi_disable();

    SAU_deviceRegs.CTRL = SAU_deviceRegs.CTRL | 0x1;

    __DMB(sy);
    __ISB(sy);

    Hwi_restore(key);
}

/*
 *  ======== isEnabled ========
 */
Bool SAU_isEnabled()
{
    return ((SAU_deviceRegs.CTRL & 0x1) != 0);
}

/*
 *  ======== SAU_configureRegion ========
 */
Void SAU_configureRegion(UInt8 regionId, Ptr regionBase,
    SizeT regionSize, SAU_RegionType regionType)
{
    UInt32 key;
    Bool   enabled;
    UInt32 rbar, rlar;
    UInt32 regionEnd;

    regionEnd = (UInt32)regionBase + regionSize;

    Assert_isTrue(regionId < SAU_numRegions, SAU_A_invalidRegionId);
    Assert_isTrue(((UInt32)regionBase & 0x1F) == 0, SAU_A_unalignedBaseAddr);
    Assert_isTrue(((UInt32)regionEnd & 0x1F) == 0, SAU_A_unalignedEndAddr);

    rbar = (UInt32)regionBase;
    rlar = (regionEnd - 1) & 0xffffffe0;

    if (regionType == SAU_RegionType_NSC) {
        rlar |= 0x2;
    }

    /* enable the region by default */
    rlar |= 0x1;

    enabled = SAU_isEnabled();

    /* Disable the SAU (if already disabled, does nothing) */
    SAU_disable();

    key = Hwi_disable();

    SAU_deviceRegs.RNR = regionId;
    SAU_deviceRegs.RBAR = rbar;
    SAU_deviceRegs.RLAR = rlar;

    /* Copy register values to module state for use by ROV */
    (SAU_module->regionEntry[regionId]).rbar = rbar;
    (SAU_module->regionEntry[regionId]).rlar = rlar;

    Hwi_restore(key);

    if (enabled) {
        SAU_enable();
    }
}

/*
 *  ======== SAU_disableRegion ========
 */
Void SAU_disableRegion(UInt8 regionId)
{
    UInt32 key;

    key = Hwi_disable();
    SAU_deviceRegs.RNR = regionId;
    SAU_deviceRegs.RLAR &= 0xfffffffe;
    Hwi_restore(key);
}

/*
 *  ======== SAU_enableRegion ========
 */
Void SAU_enableRegion(UInt8 regionId)
{
    UInt32 key;

    key = Hwi_disable();
    SAU_deviceRegs.RNR = regionId;
    SAU_deviceRegs.RLAR |= 0x1;
    Hwi_restore(key);
}
