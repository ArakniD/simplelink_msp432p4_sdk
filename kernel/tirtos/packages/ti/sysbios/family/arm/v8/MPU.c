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
 *  ======== MPU.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Startup.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/family/arm/AsmMacros.h>

#include "package/internal/MPU.xdc.h"

/*
 *  ======== MPU_startup ========
 */
Void MPU_startup()
{
    UInt i;

    /* Disable the MPU (if already disabled, does nothing) */
    MPU_disable();

    /* Initialize MAIR */
    MPU_deviceRegs.MAIR0 = (MPU_MAIR0 & 0xFF) | ((MPU_MAIR1 & 0xFF) << 8) |
        ((MPU_MAIR2 & 0xFF) << 16) | ((MPU_MAIR3 & 0xFF) << 24);
    MPU_deviceRegs.MAIR1 = (MPU_MAIR4 & 0xFF) | ((MPU_MAIR5 & 0xFF) << 8) |
        ((MPU_MAIR6 & 0xFF) << 16) | ((MPU_MAIR7 & 0xFF) << 24);

    /*
     * Initialize all MPU regions
     */
    for (i = 0; i < MPU_numRegions; i += 4) {
        MPU_deviceRegs.RNR = i;
        MPU_deviceRegs.RBAR = MPU_regionEntry[i].rbar;
        MPU_deviceRegs.RLAR = MPU_regionEntry[i].rlar;
        MPU_deviceRegs.RBAR_A1 = MPU_regionEntry[i+1].rbar;
        MPU_deviceRegs.RLAR_A1 = MPU_regionEntry[i+1].rlar;
        MPU_deviceRegs.RBAR_A2 = MPU_regionEntry[i+2].rbar;
        MPU_deviceRegs.RLAR_A2 = MPU_regionEntry[i+2].rlar;
        MPU_deviceRegs.RBAR_A3 = MPU_regionEntry[i+3].rbar;
        MPU_deviceRegs.RLAR_A3 = MPU_regionEntry[i+3].rlar;
    }

    if (MPU_enableBackgroundRegion) {
        MPU_deviceRegs.CTRL = MPU_deviceRegs.CTRL | 0x4;
    }
    else {
        MPU_deviceRegs.CTRL = MPU_deviceRegs.CTRL & (~0x4);
    }

    if (MPU_enableMPU) {
        MPU_enable();
    }
}

/*
 *  ======== MPU_disable ========
 *  Function to disable the MPU.
 */
Void MPU_disable()
{
    UInt   key;

    if (!(MPU_isEnabled())) {
        return;
    }

    key = Hwi_disable();

    /* Ensure all memory transactions have completed */
    __DMB(sy);

    /* disables the MPU */
    MPU_deviceRegs.CTRL = MPU_deviceRegs.CTRL & (~0x1);

    Hwi_restore(key);
}

/*
 *  ======== MPU_enable ========
 *  Function to enable the MPU.
 */
Void MPU_enable()
{
    UInt   key;

    /* if MPU is already enabled then just return */
    if (MPU_isEnabled()) {
        return;
    }

    key = Hwi_disable();

    MPU_deviceRegs.CTRL = MPU_deviceRegs.CTRL | 0x1;

    __DMB(sy);
    __ISB(sy);

    Hwi_restore(key);
}

/*
 *  ======== isEnabled ========
 */
Bool MPU_isEnabled()
{
    return ((MPU_deviceRegs.CTRL & 0x1) != 0);
}

/*
 *  ======== MPU_initRegionAttrs ========
 */
Void MPU_initRegionAttrs(MPU_RegionAttrs *attrs)
{
    Assert_isTrue(attrs != NULL, MPU_A_nullPointer);

    attrs->enable = MPU_defaultAttrs.enable;
    attrs->shareable = MPU_defaultAttrs.shareable;
    attrs->executable = MPU_defaultAttrs.executable;
    attrs->accessPerm = MPU_defaultAttrs.accessPerm;
    attrs->attrIndx = MPU_defaultAttrs.attrIndx;
}

/*
 *  ======== Mmu_setMAIR ========
 */
Void Mmu_setMAIR(UInt attrIndx, UInt attr)
{
    UInt key, mask, shift;

    shift = attrIndx * 8;
    mask = 0xFF << shift;

    key = Hwi_disable();

    __DMB(sy);

    if (attrIndx < 4) {
        MPU_deviceRegs.MAIR0 = MPU_deviceRegs.MAIR0 & (~mask);
        MPU_deviceRegs.MAIR0 |= (attr << shift);
    }
    else {
        MPU_deviceRegs.MAIR1 = MPU_deviceRegs.MAIR1 & (~mask);
        MPU_deviceRegs.MAIR1 |= (attr << shift);
    }

    __DMB(sy);
    __ISB(sy);

    Hwi_restore(key);
}

/*
 *  ======== MPU_setRegion ========
 */
Void MPU_setRegion(UInt8 regionId, Ptr regionBaseAddr,
    Ptr regionEndAddr, MPU_RegionAttrs *attrs)
{
    UInt32 key;
    Bool   enabled;
    UInt32 rbar, rlar;

    Assert_isTrue(attrs != NULL, MPU_A_nullPointer);
    Assert_isTrue(regionId < MPU_numRegions, MPU_A_invalidRegionId);
    Assert_isTrue(((UInt32)regionBaseAddr & 0x1F) == 0, MPU_A_unalignedBaseAddr);
    Assert_isTrue(((UInt32)regionEndAddr & 0x1F) == 0, MPU_A_unalignedEndAddr);

    rbar = (UInt32)regionBaseAddr | (UInt32)(attrs->shareable << 3) |
        (UInt32)(attrs->accessPerm << 1) | (UInt32)((!(attrs->executable)) & 0x1);

    rlar = (UInt32)regionEndAddr | (UInt32)(attrs->attrIndx << 1) |
        (UInt32)(attrs->enable);

    enabled = MPU_isEnabled();

    /* Disable the MPU (if already disabled, does nothing) */
    MPU_disable();

    key = Hwi_disable();

    MPU_deviceRegs.RNR = regionId;
    MPU_deviceRegs.RBAR = rbar;
    MPU_deviceRegs.RLAR = rlar;

    /* Copy register values to module state for use by ROV */
    (MPU_module->regionEntry[regionId]).rbar = rbar;
    (MPU_module->regionEntry[regionId]).rlar = rlar;

    Hwi_restore(key);

    if (enabled) {
        MPU_enable();
    }
}
