/*
 * Copyright (c) 2013-2018, Texas Instruments Incorporated
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
 *  ======== Hwi.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/Log.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/interfaces/IHwi.h>
#include <ti/sysbios/BIOS.h>

#include <c6x_migration.h>
#include <c7x.h>

#include "package/internal/Hwi.xdc.h"
#pragma FUNC_EXT_CALLED(Hwi_dispatchC);

extern UInt ti_sysbios_family_c7x_Hwi_disable__E(void);
extern UInt ti_sysbios_family_c7x_Hwi_enable__E(void);
extern Void ti_sysbios_family_c7x_Hwi_restore__E(UInt key);

extern Void ti_sysbios_family_c7x_Hwi_dispatchAlways(Void);
extern Void ti_sysbios_family_xxx_Hwi_switchAndRunFunc(Void (*func)());

#define Hwi_switchAndRunFunc ti_sysbios_family_xxx_Hwi_switchAndRunFunc
#define Hwi_dispatchAlways ti_sysbios_family_c7x_Hwi_dispatchAlways
#define Hwi_vectorsBase ti_sysbios_family_c7x_Hwi_vectorsBase

extern char Hwi_vectorsBase[];
extern __FAR__ char _stack[0x10001];

#ifdef ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport__D
/* disable unused local variable warning during optimized compile */
#pragma diag_suppress=179
#define TASK_DISABLE Task_disable
#define TASK_RESTORE Task_restoreHwi
#else
#define TASK_DISABLE Hwi_taskDisable
#define TASK_RESTORE Hwi_taskRestoreHwi
#endif

#ifdef ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport__D
/* disable unused local variable warning during optimized compile */
#pragma diag_suppress=179
#define SWI_DISABLE Swi_disable
#define SWI_RESTORE Swi_restoreHwi
#else
#define SWI_DISABLE Hwi_swiDisable
#define SWI_RESTORE Hwi_swiRestoreHwi
#endif

/*
 * ECSP stores an 8 KB context for each interrupt, up to a maximum of 8
 * nesting levels (1 per priority level).
 */
#define HWI_ECSP_SIZE (0x10000)

/*
 *  ======== Hwi_Module_startup ========
 */
Int Hwi_Module_startup(Int phase)
{
    Int i;
    Hwi_Object *hwi;

    /* Task and Swi APIs used not called until BIOS_start() */
    /* okay to proceed with initialization */

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    for (i = 0; i < Hwi_hooks.length; i++) {
        if (Hwi_hooks.elem[i].registerFxn != NULL) {
            Hwi_hooks.elem[i].registerFxn(i);
        }
    }
#endif

    /* Initialize the vector table pointer, ESTP */
    __ESTP_S = (ULong)Hwi_module->vectorTableBase;
    __ESTP_SS = (ULong)Hwi_module->vectorTableBase;

    /*
     * Initialize the pointer to the isrStack. These symbols are part of the
     * Hwi_module (instead of floating) in order to support ROM.
     * Leave room for two 32-bit values pushed by xdc_runtime_Startup_reset()
     * (for cases where intentionally reset as resume from power down),
     * and maintain double word alignment.
     */
    Hwi_module->isrStack = Hwi_getIsrStackAddress() - 16;
    __ECSP_S = (UInt64)_stack;
    __ECSP_SS = (UInt64)_stack;
    __TCSP = (UInt64)(_stack + HWI_ECSP_SIZE);

    /* signal that we're executing on the ISR stack */
    Hwi_module->taskSP = (Char *)-1;

    /* initialize event mapping */
    for (i = 0; i < Hwi_NUM_INTERRUPTS; i++) {
        if (Hwi_module->intEvents[i] != -1) {
            Hwi_eventMap(i, Hwi_module->intEvents[i]);
        }
        else {
            /* keep intEvents[] current for ROV */
            Hwi_module->intEvents[i] = Hwi_getEventId(i);
        }
    }

    /* start with a clean slate */
    __set_indexed(__EFCLR, 0, 0xFFFFFFFFFFFFFFFFUL);

    Hwi_enableIER(Hwi_module->ierMask); /* IER per static Hwi settings */

    for (i = 0; i < Hwi_NUM_INTERRUPTS; i++) {
        hwi = Hwi_module->dispatchTable[i];
        if (hwi !=  NULL) {
            Hwi_postInit(hwi, NULL);
        }
    }

    ti_sysbios_family_c7x_Hwi_setCOP__E(7);

    return (Startup_DONE);
}

/*
 *  ======== Hwi_Instance_init ========
 */
Int Hwi_Instance_init(Hwi_Object *hwi, Int intNum,
                      Hwi_FuncPtr fxn, const Hwi_Params *params,
                      Error_Block *eb)
{
    Int status;

    /* there are 64 "interrupt" events in C7x */
    if (intNum < 0 || intNum > 63) {
        Error_raise(eb, Hwi_E_invalidIntNum, intNum, 0);
        return (1);
    }

    if (Hwi_module->dispatchTable[intNum] != NULL) {
        Error_raise(eb, Hwi_E_alreadyDefined, intNum, 0);
        return (1);
    }

    Hwi_module->dispatchTable[intNum] = hwi;
// There is no vector table on C7x.  Instead, all interrupts vector to
// ESTP + 0x800, where a dispatcher needs to look at AHPEE for interrupt
// number in service and call the configured ISR.
//    Hwi_plug(intNum, Hwi_dispatchAlways);

    Hwi_reconfig(hwi, fxn, params);
    hwi->intNum = intNum;

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    if (Hwi_hooks.length > 0) {
        /* Allocate environment space for each hook instance. */
        hwi->hookEnv = Memory_calloc(Hwi_Object_heap(),
                Hwi_hooks.length * sizeof(Ptr), 0, eb);

        if (hwi->hookEnv == NULL) {
            return (2);
        }
    }
#endif

    hwi->irp = 0;

    status = Hwi_postInit(hwi, eb);

    if (Error_check(eb)) {
        return (3 + status);
    }

    return (0);
}

/*
 *  ======== Hwi_postInit ========
 *  Function to be called during module startup to complete the
 *  initialization of any statically created or constructed Hwi.
 *  returns (0) and clean 'eb' on success
 *  returns 'eb' *and* 'n' for number of successful createFxn() calls iff
 *      one of the createFxn() calls fails
 */
Int Hwi_postInit (Hwi_Object *hwi, Error_Block *eb)
{
    Int i;

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    for (i = 0; i < Hwi_hooks.length; i++) {
        hwi->hookEnv[i] = (Ptr)0;
        if (Hwi_hooks.elem[i].createFxn != NULL) {
            Hwi_hooks.elem[i].createFxn((IHwi_Handle)hwi, eb);

            if (Error_check(eb)) {
                return (i);
            }
        }
    }
#endif

    Hwi_setPriority(hwi->intNum, hwi->priority);

    return (0);
}

/*
 *  ======== Hwi_Instance_finalize ========
 *  Here on Hwi_delete() or failed Hwi_create().
 */
Void Hwi_Instance_finalize(Hwi_Object *hwi, Int status)
{
    Int i, cnt;
    UInt intNum;

    if (status == 1) {  /* failed early in Hwi_Instance_init() */
        return;
    }

    for (intNum = 0; intNum < Hwi_NUM_INTERRUPTS; intNum++) {
        if (Hwi_module->dispatchTable[intNum] == hwi) {
            break;
        }
    }

    if (intNum == Hwi_NUM_INTERRUPTS) {
        Error_raise(NULL, Hwi_E_handleNotFound, hwi, 0);
        return;   /* never reached, but keep Coverity happy */
    }

    Hwi_disableInterrupt(intNum);
    Hwi_module->dispatchTable[intNum] = NULL;

    if (status == 2) {  /* failed mid-way into Hwi_Instance_init() */
        return;
    }

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    if (Hwi_hooks.length > 0) {
        if (status == 0) {
            cnt = Hwi_hooks.length;
        }
        else {
            cnt = status - 3;    /* failed within hook init */
        }

        for (i = 0; i < cnt; i++) {
            if (Hwi_hooks.elem[i].deleteFxn != NULL) {
                Hwi_hooks.elem[i].deleteFxn((IHwi_Handle)hwi);
            }
        }

        Memory_free(Hwi_Object_heap(), hwi->hookEnv,
                Hwi_hooks.length * sizeof(Ptr));
    }
#endif
}

/*
 *  ======== Hwi_unPluggedInterrupt ========
 *  Here on interrupt unPlugged by Hwi_delete().
 */
Void Hwi_unPluggedInterrupt() {
    while(TRUE);
}

/*
 *  ======== Hwi_eventMap ========
 *  Function maps an event to a Hwi vector so that the event will be the
 *  source of the interrupt of the vector.
 */
Void Hwi_eventMap(Int vectId, Int eventId)
{
    UInt mask;          /* Interrupt mask value */

    if (vectId < 0 || vectId > 63) {
        return;
    }

    mask = ti_sysbios_family_c7x_Hwi_disable__E();

    /* Program CLEC to map external eventId to internal interrupt (event) */

    /* clear any residual interrupt */
    __set_indexed(__EFCLR, 0, 1L << vectId);

    ti_sysbios_family_c7x_Hwi_restore__E(mask);
}

/*
 *  ======== Hwi_getEventId ========
 */
Int Hwi_getEventId(UInt vectId)
{
    if (vectId > 63) {
        return -1;
    }

    /* TODO */

    return 0;
}

/*
 * Below APIs provided for backward compatability.
 */

/*
 *  ======== Hwi_disableIER ========
 */
ULong Hwi_disableIER(ULong mask)
{
    UInt key;
    ULong oldEER;

    key = ti_sysbios_family_c7x_Hwi_disable__E();
    oldEER = __get_indexed(__EER, 0);
    __set_indexed(__EECLR, 0, mask);
    ti_sysbios_family_c7x_Hwi_restore__E(key);

    return oldEER;
}

/*
 *  ======== Hwi_enableIER ========
 */
ULong Hwi_enableIER(ULong mask)
{
    UInt key;
    ULong oldEER;

    key = ti_sysbios_family_c7x_Hwi_disable__E();
    oldEER = __get_indexed(__EER, 0);
    __set_indexed(__EESET, 0, mask);
    ti_sysbios_family_c7x_Hwi_restore__E(key);

    return oldEER;
}

/*
 *  ======== Hwi_restoreIER ========
 */
ULong Hwi_restoreIER(ULong mask)
{
    UInt key;
    ULong oldEER;

    key = ti_sysbios_family_c7x_Hwi_disable__E();
    oldEER = __get_indexed(__EER, 0);
    __set_indexed(__EESET, 0, mask);
    ti_sysbios_family_c7x_Hwi_restore__E(key);

    return oldEER;
}

/*
 *  ======== Hwi_setPriority ========
 */
Void Hwi_setPriority(UInt intNum, UInt priority)
{
    __set_indexed(__EPRI, intNum, priority << 5);
}

/*
 *  ======== Hwi_disableInterrupt ========
 */
UInt Hwi_disableInterrupt(UInt intNum)
{
    ULong mask = 1L << intNum;

    /* Hwi_disableIER() returns old EER */
    return ((Hwi_disableIER(mask) & mask) != 0L);
}

/*
 *  ======== Hwi_enableInterrupt ========
 */
UInt Hwi_enableInterrupt(UInt intNum)
{
    ULong mask = 1L << intNum;

    return ((Hwi_enableIER(mask) & mask) != 0L);
}

/*
 *  ======== Hwi_RestoreInterrupt ========
 */
Void Hwi_restoreInterrupt(UInt intNum, UInt key)
{
    if (key) {
        Hwi_enableInterrupt(intNum);
    }
    else {
        Hwi_disableInterrupt(intNum);
    }
}

/*
 *  ======== Hwi_clearInterrupt ========
 */
Void Hwi_clearInterrupt(UInt intNum)
{
    __set_indexed(__EFCLR, 0, 1L << intNum);
}

/*
 *  ======== Hwi_getHandle ========
 */
Hwi_Handle Hwi_getHandle(UInt intNum)
{
    return (Hwi_module->dispatchTable[intNum]);
}

/*
 *  ======== Hwi_getFunc ========
 */
Hwi_FuncPtr Hwi_getFunc(Hwi_Object *hwi, UArg *arg)
{
    *arg = hwi->arg;

    return (hwi->fxn);
}

/*
 *  ======== setFunc ========
 */
Void Hwi_setFunc(Hwi_Object *hwi, Void (*fxn)(), UArg arg)
{
    hwi->fxn = fxn;
    hwi->arg = arg;
}

/*
 *  ======== Hwi_getIrp ========
 */
Hwi_Irp Hwi_getIrp(Hwi_Object *hwi)
{
    return (hwi->irp);
}

/*
 *  ======== Hwi_post ========
 */
Void Hwi_post(UInt intNum)
{
    __set_indexed(__EFSET, 0, 1L << intNum);
}

/*
 *  ======== Hwi_getTaskSP ========
 */
Char *Hwi_getTaskSP()
{
    return (Hwi_module->taskSP);
}

/*
 *  ======== Hwi_reconfig ========
 *  Reconfigure a dispatched interrupt.
 */
Void Hwi_reconfig(Hwi_Object *hwi, Hwi_FuncPtr fxn, const Hwi_Params *params)
{
    UInt intNum;

    for (intNum = 0; intNum < Hwi_NUM_INTERRUPTS; intNum++) {
        if (Hwi_module->dispatchTable[intNum] == hwi) {
            break;
        }
    }

    if (intNum == Hwi_NUM_INTERRUPTS) {
        Error_raise(NULL, Hwi_E_handleNotFound, hwi, 0);
        return;
    }


    Hwi_disableInterrupt(intNum);

    hwi->fxn = fxn;
    hwi->arg = params->arg;

    if (params->priority == -1) {
        hwi->priority = Hwi_DEFAULT_INT_PRIORITY;
    }
    else {
        hwi->priority = params->priority;
    }

    switch (params->maskSetting) {
        case Hwi_MaskingOption_NONE:
            hwi->disableMask = 0;
            hwi->restoreMask = 0;
            break;
        case Hwi_MaskingOption_ALL:
            hwi->disableMask = 0xFFFFFFFFFFFFFFFFUL;
            hwi->restoreMask = 0xFFFFFFFFFFFFFFFFUL;
            break;
        default:
        case Hwi_MaskingOption_SELF:
            hwi->disableMask = 1L << intNum;
            hwi->restoreMask = 1L << intNum;
            break;
        case Hwi_MaskingOption_BITMASK:
            hwi->disableMask = params->disableMask;
            hwi->restoreMask = params->restoreMask;
            break;
    }

    if (params->eventId != -1) {
        Hwi_eventMap(intNum, params->eventId);
    }

    /* keep intEvents[] current for ROV */
    Hwi_module->intEvents[intNum] = Hwi_getEventId(intNum);

    if (params->enableInt) {
        Hwi_enableInterrupt(intNum);
    }
}

/*
 *  ======== switchFromBootStack ========
 *  Indicate that we are leaving the boot stack and
 *  are about to switch to a task stack.
 */
Void Hwi_switchFromBootStack()
{
    Hwi_module->taskSP = 0;
}

/*
 *  ======== Hwi_getIsrStackAddress ========
 */
Char *Hwi_getIsrStackAddress()
{
    __extern __FAR__ UInt8 __TI_STACK_SIZE;
    UInt64 isrStack;

    isrStack = (UInt64)_stack;
    isrStack += (UInt64)_symval(&__TI_STACK_SIZE);
    isrStack -= 0x1;

    isrStack &= ~0x7;   /* align to long word */

    return ((Char *)isrStack);
}

/*
 *  ======== Hwi_getStackInfo ========
 *  Used to get Hwi stack usage info.
 */
Bool Hwi_getStackInfo(Hwi_StackInfo *stkInfo, Bool computeStackDepth)
{
    __extern __FAR__ UInt8 __TI_STACK_SIZE;

    Char *isrSP;
    Bool stackOverflow;

    /* Copy the stack base address and size */
    stkInfo->hwiStackSize = (SizeT)_symval(&__TI_STACK_SIZE) - HWI_ECSP_SIZE;
    stkInfo->hwiStackBase = _stack + HWI_ECSP_SIZE;

    isrSP = stkInfo->hwiStackBase;

    /* Check for stack overflow */
    stackOverflow = (*isrSP != (Char)0xbe ? TRUE : FALSE);

    if (computeStackDepth && !stackOverflow) {
        /* Compute stack depth */
        do {
        } while(*isrSP++ == (Char)0xbe);
        stkInfo->hwiStackPeak = stkInfo->hwiStackSize - (SizeT)(--isrSP - (Char *)stkInfo->hwiStackBase);
    }
    else {
        stkInfo->hwiStackPeak = 0;
    }

    return stackOverflow;
}

/*
 *  ======== Hwi_getCoreStackInfo ========
 *  Used to get Hwi stack usage info.
 */
Bool Hwi_getCoreStackInfo(Hwi_StackInfo *stkInfo, Bool computeStackDepth,
    UInt coreId)
{
    return (Hwi_getStackInfo(stkInfo, computeStackDepth));
}

/*
 *  ======== Hwi_getHookContext ========
 */
Ptr Hwi_getHookContext(Hwi_Object *hwi, Int id)
{
    return (hwi->hookEnv[id]);
}

/*
 *  ======== Hwi_setHookContext ========
 */
Void Hwi_setHookContext(Hwi_Object *hwi, Int id, Ptr hookContext)
{
    hwi->hookEnv[id] = hookContext;
}

/*
 *  ======== Hwi_dispatchC ========
 *  Configurable dispatcher.
 */
Void Hwi_dispatchC(Int intNum)
{
    Int tskKey;

    if (Hwi_dispatcherTaskSupport) {
        tskKey = TASK_DISABLE();
    }

    Hwi_switchAndDispatch(intNum);

    if (Hwi_dispatcherTaskSupport) {
        TASK_RESTORE(tskKey);   /* returns with ints disabled */
    }
}

/*
 *  ======== Hwi_dispatchCore ========
 *  Configurable dispatcher.
 */
Void Hwi_dispatchCore(Int intNum)
{
    /*
     * Enough room is reserved above the isr stack to handle
     * as many as 16 32-bit stack resident local variables.
     * If the dispatcher requires more than this, you must
     * handle this in Hwi_Module_startup().
     */

    Hwi_Object *hwi;
    BIOS_ThreadType prevThreadType;
    ULong oldEER, disableMask, restoreMask;
    Int swiKey;
    Int i;
    Hwi_FuncPtr fxn;
    UArg arg;

    /* save away intNum in module state because it might be saved on stack */
    Hwi_module->intNum = intNum;

    /*
     * pre-read local copies of the variables used
     * within to eliminate memory fetch nops
     */
    hwi = Hwi_module->dispatchTable[intNum];
    fxn = hwi->fxn;
    arg = hwi->arg;

    if (Hwi_dispatcherIrpTrackingSupport) {
// C7x has no event return pointer, it's on context stack somewhere
//        hwi->irp = __ERP;
    }

    if (Hwi_dispatcherAutoNestingSupport) {
        disableMask = hwi->disableMask;
        restoreMask = hwi->restoreMask;
    }

    if (Hwi_dispatcherSwiSupport) {
        swiKey = SWI_DISABLE();
    }

    /* set thread type to Hwi */
    prevThreadType = BIOS_setThreadType(BIOS_ThreadType_Hwi);

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    /* call the begin hooks */
    for (i = 0; i < Hwi_hooks.length; i++) {
        if (Hwi_hooks.elem[i].beginFxn != NULL) {
            Hwi_hooks.elem[i].beginFxn((IHwi_Handle)hwi);
        }
    }
#endif

//    Log_write5(Hwi_LM_begin, (IArg)hwi, (IArg)hwi->fxn,
//               (IArg)prevThreadType, (IArg)intNum, hwi->irp);

    /* call the user's isr */
    if (Hwi_dispatcherAutoNestingSupport) {
        oldEER = __get_indexed(__EER, 0);
        __set_indexed(__EECLR, 0, disableMask);

        ti_sysbios_family_c7x_Hwi_enable__E();

        (fxn)(arg);

        ti_sysbios_family_c7x_Hwi_disable__E();
        __set_indexed(__EESET, 0, restoreMask & oldEER);
    }
    else {
        (fxn)(arg);
    }

//    Log_write1(Hwi_LD_end, (IArg)hwi);

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    /* call the end hooks */
    for (i = 0; i < Hwi_hooks.length; i++) {
        if (Hwi_hooks.elem[i].endFxn != NULL) {
            Hwi_hooks.elem[i].endFxn((IHwi_Handle)hwi);
        }
    }
#endif

    /* open up interrupt priorities */
    ti_sysbios_family_c7x_Hwi_setCOP__E(7);

    /* Run Swi scheduler */
    if (Hwi_dispatcherSwiSupport) {
        SWI_RESTORE(swiKey);
    }

    /* restore thread type */
    BIOS_setThreadType(prevThreadType);
}
