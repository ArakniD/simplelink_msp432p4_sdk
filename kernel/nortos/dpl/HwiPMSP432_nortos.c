/*
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 *  ======== HwiPMSP432_nortos.c ========
 */

#include <stdlib.h>

/* Driver lib includes */
#include <ti/devices/DeviceFamily.h>

//#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/DebugP.h>

#define MAX_INTERRUPTS 80
#define NVIC_INT_CTRL_REG (*((volatile uint32_t *)0xe000ed04))

/* Masks off all bits but the VECTACTIVE bits in the ICSR register. */
#define VECTACTIVE_MASK (0xFFUL)

#define INT_PRI_LEVEL0          0x00000000
#define INT_PRI_LEVEL1          0x00000020
#define INT_PRI_LEVEL2          0x00000040
#define INT_PRI_LEVEL3          0x00000060
#define INT_PRI_LEVEL4          0x00000080
#define INT_PRI_LEVEL5          0x000000A0
#define INT_PRI_LEVEL6          0x000000C0
#define INT_PRI_LEVEL7          0x000000E0

typedef struct _HwiP_Obj {
    uint32_t intNum;
    HwiP_Fxn fxn;
    uintptr_t arg;
} HwiP_Obj;

void HwiP_dispatch(void);

static HwiP_Obj* HwiP_dispatchTable[MAX_INTERRUPTS] = {
    0
};

typedef struct HwiP_NVIC {
    uint32_t RES_00;
    uint32_t ICTR;
    uint32_t RES_08;
    uint32_t RES_0C;
    uint32_t STCSR;
    uint32_t STRVR;
    uint32_t STCVR;
    uint32_t STCALIB;
    uint32_t RES_20[56];
    uint32_t ISER[8];
    uint32_t RES_120[24];
    uint32_t ICER[8];
    uint32_t RES_1A0[24];
    uint32_t ISPR[8];
    uint32_t RES_220[24];
    uint32_t ICPR[8];
    uint32_t RES_2A0[24];
    uint32_t IABR[8];
    uint32_t RES_320[56];
    uint8_t IPR[240];
    uint32_t RES_4F0[516];
    uint32_t CPUIDBR;
    uint32_t ICSR;
    uint32_t VTOR;
    uint32_t AIRCR;
    uint32_t SCR;
    uint32_t CCR;
    uint8_t SHPR[12];
    uint32_t SHCSR;
    uint8_t MMFSR;
    uint8_t BFSR;
    uint16_t UFSR;
    uint32_t HFSR;
    uint32_t DFSR;
    uint32_t MMAR;
    uint32_t BFAR;
    uint32_t AFSR;
    uint32_t PFR0;
    uint32_t PFR1;
    uint32_t DFR0;
    uint32_t AFR0;
    uint32_t MMFR0;
    uint32_t MMFR1;
    uint32_t MMFR2;
    uint32_t MMFR3;
    uint32_t ISAR0;
    uint32_t ISAR1;
    uint32_t ISAR2;
    uint32_t ISAR3;
    uint32_t ISAR4;
    uint32_t RES_D74[5];
    uint32_t CPACR;
    uint32_t RES_D8C[93];
    uint32_t STI;
    uint32_t RES_F04[12];
    uint32_t FPCCR;
    uint32_t FPCAR;
    uint32_t FPDSCR;
    uint32_t MVFR0;
    uint32_t MVFR1;
    uint32_t RES_F48[34];
    uint32_t PID4;
    uint32_t PID5;
    uint32_t PID6;
    uint32_t PID7;
    uint32_t PID0;
    uint32_t PID1;
    uint32_t PID2;
    uint32_t PID3;
    uint32_t CID0;
    uint32_t CID1;
    uint32_t CID2;
    uint32_t CID3;
} HwiP_NVIC;

static volatile HwiP_NVIC *HwiP_nvic = (HwiP_NVIC *)0xE000E000;

uintptr_t HwiP_key = 0;

int HwiP_swiPIntNum = FAULT_PENDSV;

/*
 *  ======== HwiP_clearInterrupt ========
 */
void HwiP_clearInterrupt(int interruptNum)
{
    Interrupt_unpendInterrupt((uint32_t)interruptNum);
}

/*
 *  ======== HwiP_construct ========
 */
HwiP_Handle HwiP_construct(HwiP_Struct *handle, int interruptNum,
                           HwiP_Fxn hwiFxn, HwiP_Params *params)
{
    HwiP_Params defaultParams;
    HwiP_Obj *obj = (HwiP_Obj *)handle;

    if (obj != NULL) {
        if (params == NULL) {
            params = &defaultParams;
            HwiP_Params_init(&defaultParams);
        }

        if ((params->priority & 0xFF) == 0xFF) {
            /* SwiP_nortos.c uses INT_PRI_LEVEL7 as its scheduler */
            params->priority = INT_PRI_LEVEL6;
        }

        if (interruptNum != HwiP_swiPIntNum &&
            params->priority == INT_PRI_LEVEL7) {
            DebugP_log0("HwiP_construct: can't use reserved INT_PRI_LEVEL7");

            obj = NULL;
        }
        else {
            HwiP_dispatchTable[interruptNum] = obj;
            obj->fxn = hwiFxn;
            obj->arg = params->arg;
            obj->intNum = (uint32_t)interruptNum;

            Interrupt_registerInterrupt((uint32_t)interruptNum,
                    (void(*)(void))HwiP_dispatch);

            Interrupt_setPriority((uint32_t)interruptNum,
                    (uint8_t)params->priority);

            if (params->enableInt) {
                Interrupt_enableInterrupt((uint32_t)interruptNum);
            }
        }
    }

    return ((HwiP_Handle)obj);
}

/*
 *  ======== HwiP_create ========
 */
HwiP_Handle HwiP_create(int interruptNum, HwiP_Fxn hwiFxn, HwiP_Params *params)
{
    HwiP_Handle handle;
    HwiP_Handle retHandle;

    handle = (HwiP_Handle)malloc(sizeof(HwiP_Obj));

    /*
     * Even though HwiP_construct will check handle for NULL and not do
     * anything, we should check it here so that we can know afterwards
     * that construct failed with non-NULL pointer and that we need to
     * free the handle.
     */
    if (handle != NULL) {
        retHandle = HwiP_construct((HwiP_Struct *)handle, interruptNum, hwiFxn,
                                   params);
        if (retHandle == NULL) {
            free(handle);
            handle = NULL;
        }
    }

    return (handle);
}

/*
 *  ======== HwiP_delete ========
 */
void HwiP_delete(HwiP_Handle handle)
{
    HwiP_Obj *obj = (HwiP_Obj *)handle;

    Interrupt_disableInterrupt(obj->intNum);
    Interrupt_unregisterInterrupt(obj->intNum);

    free(handle);
}

/*
 *  ======== HwiP_disable ========
 */
uintptr_t HwiP_disable(void)
{
    if (Interrupt_disableMaster()) {
        /* Interrupts were already disabled */
        return (false);
    }
    else {
        return (true);
    }
}

/*
 *  ======== HwiP_disableInterrupt ========
 */
void HwiP_disableInterrupt(int interruptNum)
{
    Interrupt_disableInterrupt((uint32_t)interruptNum);
}

/*
 *  ======== HwiP_dispatch ========
 */
void HwiP_dispatch(void)
{
    /* Determine which interrupt has fired */
    uint32_t intNum  = HwiP_nvic->ICSR & 0x000000ff;
    HwiP_Obj* obj = HwiP_dispatchTable[intNum];
    if (obj) {
        (obj->fxn)(obj->arg);
    }
}

/*
 *  ======== HwiP_enable ========
 */
void HwiP_enable(void)
{
    Interrupt_enableMaster();
}

/*
 *  ======== HwiP_enableInterrupt ========
 */
void HwiP_enableInterrupt(int interruptNum)
{
    Interrupt_enableInterrupt((uint32_t)interruptNum);
}

/*
 *  ======== HwiP_inISR ========
 */
bool HwiP_inISR(void)
{
    /* This is the same as HwiP_nvic & 0x000000ff */
    if ((NVIC_INT_CTRL_REG & VECTACTIVE_MASK) == 0) {
        /* Not currently in an ISR */
        return (false);
    }

    return (true);
}

/*
 *  ======== HwiP_inSwi ========
 */
bool HwiP_inSwi(void)
{
    uint32_t intNum  = HwiP_nvic->ICSR & 0x000000ff;
    if (intNum == HwiP_swiPIntNum) {
        /* Currently in a Swi */
        return (true);
    }

    return (false);
}

/*
 *  ======== HwiP_Params_init ========
 */
void HwiP_Params_init(HwiP_Params *params)
{
    if (params != NULL) {
        params->arg = 0;
        params->priority = ~0;
        params->enableInt = true;
    }
}

/*
 *  ======== HwiP_plug ========
 */
void HwiP_plug(int interruptNum, void *fxn)
{
    Interrupt_registerInterrupt((uint32_t)interruptNum, (void(*)(void))fxn);
}

/*
 *  ======== HwiP_post ========
 */
void HwiP_post(int interruptNum)
{
    Interrupt_pendInterrupt(interruptNum);
}

/*
 *  ======== HwiP_restore ========
 */
void HwiP_restore(uintptr_t enableFlag)
{
    if (enableFlag) {
        Interrupt_enableMaster();
    }
}

/*
 *  ======== HwiP_setFunc ========
 */
void HwiP_setFunc(HwiP_Handle hwiP, HwiP_Fxn fxn, uintptr_t arg)
{
    HwiP_Obj *obj = (HwiP_Obj *)hwiP;

    uintptr_t key = HwiP_disable();

    obj->fxn = fxn;
    obj->arg = arg;

    HwiP_restore(key);
}


/*
 *  ======== HwiP_setPriority ========
 */
void HwiP_setPriority(int interruptNum, uint32_t priority)
{
    Interrupt_setPriority((uint32_t)interruptNum, priority);
}

/*
 *  ======== HwiP_staticObjectSize ========
 */
size_t HwiP_staticObjectSize(void)
{
    return (sizeof(HwiP_Obj));
}
