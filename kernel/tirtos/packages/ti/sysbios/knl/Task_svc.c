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
 *  ======== Task_svc.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>

#include <ti/sysbios/hal/SysCall.h>

#include "package/internal/Task.xdc.h"

/*
 *  ======== ti_sysbios_knl_Task_construct_SVC ========
 */
Void ti_sysbios_knl_Task_construct_SVC(Task_Struct *obj, Task_FuncPtr fxn,
    const Task_Params *prms, Error_Block *eb)
{
    SysCall_enterPrivMode();
    Task_construct(obj, fxn, prms, eb);
    SysCall_enterUnprivMode();
}

/*
 *  ======== ti_sysbios_knl_Task_destruct_SVC ========
 */
Void ti_sysbios_knl_Task_destruct_SVC(Task_Struct *obj)
{
    SysCall_enterPrivMode();
    Task_destruct(obj);
    SysCall_enterUnprivMode();
}

/*
 *  ======== ti_sysbios_knl_Task_disable_SVC ========
 */
UInt ti_sysbios_knl_Task_disable_SVC()
{
    UInt ret;

    SysCall_enterPrivMode();
    ret = Task_disable();
    SysCall_enterUnprivMode();
    return (ret);
}

/*
 *  ======== ti_sysbios_knl_Task_enable_SVC ========
 */
Void ti_sysbios_knl_Task_enable_SVC()
{
    SysCall_enterPrivMode();
    Task_enable();
    SysCall_enterUnprivMode();
}

/*
 *  ======== ti_sysbios_knl_Task_getPri_SVC ========
 */
Int ti_sysbios_knl_Task_getPri_SVC(Task_Object *tsk)
{
    Int ret;

    SysCall_enterPrivMode();
    ret = Task_getPri(tsk);
    SysCall_enterUnprivMode();
    return (ret);
}

/*
 *  ======== ti_sysbios_knl_Task_getPrivileged_SVC ========
 */
Bool ti_sysbios_knl_Task_getPrivileged_SVC(Task_Object *tsk)
{
    Bool ret;

    SysCall_enterPrivMode();
    ret = Task_getPrivileged(tsk);
    SysCall_enterUnprivMode();
    return (ret);
}

/*
 *  ======== ti_sysbios_knl_Task_Params_init_SVC ========
 */
Void ti_sysbios_knl_Task_Params_init_SVC(Task_Params *prms) {
    SysCall_enterPrivMode();
    Task_Params_init(prms);
    SysCall_enterUnprivMode();
}

/*
 *  ======== ti_sysbios_knl_Task_restore_SVC ========
 */
Void ti_sysbios_knl_Task_restore_SVC(UInt tskKey)
{
    SysCall_enterPrivMode();
    Task_restore(tskKey);
    SysCall_enterUnprivMode();
}

/*
 *  ======== ti_sysbios_knl_Task_setPri_SVC ========
 */
Int ti_sysbios_knl_Task_setPri_SVC(Task_Object *tsk, Int priority)
{
    Int ret;

    SysCall_enterPrivMode();
    ret = Task_setPri(tsk, priority);
    SysCall_enterUnprivMode();
    return (ret);
}

/*
 *  ======== ti_sysbios_knl_Task_sleep_SVC ========
 */
Void ti_sysbios_knl_Task_sleep_SVC(UInt32 period)
{
    SysCall_enterPrivMode();
    Task_sleep(period);
    SysCall_enterUnprivMode();
}

/*
 *  ======== ti_sysbios_knl_Task_yield_SVC ========
 */
Void ti_sysbios_knl_Task_yield_SVC()
{
    SysCall_enterPrivMode();
    Task_yield();
    SysCall_enterUnprivMode();
}
