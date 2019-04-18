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
 *  ======== Semaphore_svc.c ========
 */

#include <xdc/std.h>

#include <ti/sysbios/hal/SysCall.h>

#include "package/internal/Semaphore.xdc.h"

/*
 *  ======== ti_sysbios_knl_Semaphore_construct_SVC ========
 */
Void ti_sysbios_knl_Semaphore_construct_SVC(Semaphore_Struct *obj, Int count,
    const Semaphore_Params *prms)
{
    SysCall_enterPrivMode();
    Semaphore_construct(obj, count, prms);
    SysCall_enterUnprivMode();
}

/*
 *  ======== ti_sysbios_knl_Semaphore_destruct_SVC ========
 */
Void ti_sysbios_knl_Semaphore_destruct_SVC(Semaphore_Struct *obj)
{
    SysCall_enterPrivMode();
    Semaphore_destruct(obj);
    SysCall_enterUnprivMode();
}

/*
 *  ======== ti_sysbios_knl_Semaphore_getCount_SVC ========
 */
Int ti_sysbios_knl_Semaphore_getCount_SVC(Semaphore_Object *sem)
{
    Int ret;

    SysCall_enterPrivMode();
    ret = Semaphore_getCount(sem);
    SysCall_enterUnprivMode();
    return (ret);
}

/*
 *  ======== ti_sysbios_knl_Semaphore_Params_init_SVC ========
 */
Void ti_sysbios_knl_Semaphore_Params_init_SVC(Semaphore_Params *prms)
{
    SysCall_enterPrivMode();
    Semaphore_Params_init(prms);
    SysCall_enterUnprivMode();
}

/*
 *  ======== ti_sysbios_knl_Semaphore_pend_SVC ========
 */
Bool ti_sysbios_knl_Semaphore_pend_SVC(Semaphore_Object *sem, UInt32 timeout)
{
    Bool ret;

    SysCall_enterPrivMode();
    ret = Semaphore_pend(sem, timeout);
    SysCall_enterUnprivMode();
    return (ret);
}

/*
 *  ======== ti_sysbios_knl_Semaphore_post_SVC ========
 */
Void ti_sysbios_knl_Semaphore_post_SVC(Semaphore_Handle handle)
{
    SysCall_enterPrivMode();
    Semaphore_post(handle);
    SysCall_enterUnprivMode();
}
