/*
 * Copyright (c) 2012-2017, Texas Instruments Incorporated
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

#ifndef ti_sysbios_knl_Task__epilogue__include
#define ti_sysbios_knl_Task__epilogue__include

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== Task_selfMacro ========
 */

#define ti_sysbios_knl_Task_selfMacro() (ti_sysbios_knl_Task_self())

#if defined(ti_sysbios_Build_useSvcWrappers)

#undef ti_sysbios_knl_Task_construct
#define ti_sysbios_knl_Task_construct ti_sysbios_knl_Task_construct_SVC
extern xdc_Void ti_sysbios_knl_Task_construct_SVC(
    ti_sysbios_knl_Task_Struct *obj, ti_sysbios_knl_Task_FuncPtr fxn,
    const ti_sysbios_knl_Task_Params *prms, xdc_runtime_Error_Block *eb);

#undef ti_sysbios_knl_Task_destruct
#define ti_sysbios_knl_Task_destruct ti_sysbios_knl_Task_destruct_SVC
extern xdc_Void ti_sysbios_knl_Task_destruct_SVC(
    ti_sysbios_knl_Task_Struct *obj);

#undef ti_sysbios_knl_Task_disable
#define ti_sysbios_knl_Task_disable ti_sysbios_knl_Task_disable_SVC
extern xdc_UInt ti_sysbios_knl_Task_disable_SVC(void);

#undef ti_sysbios_knl_Task_enable
#define ti_sysbios_knl_Task_enable ti_sysbios_knl_Task_enable_SVC
extern xdc_Void ti_sysbios_knl_Task_enable_SVC(void);

#undef ti_sysbios_knl_Task_getPri
#define ti_sysbios_knl_Task_getPri ti_sysbios_knl_Task_getPri_SVC
extern xdc_Int ti_sysbios_knl_Task_getPri_SVC(
    ti_sysbios_knl_Task_Handle handle);

#undef ti_sysbios_knl_Task_getPrivileged
#define ti_sysbios_knl_Task_getPrivileged ti_sysbios_knl_Task_getPrivileged_SVC
extern xdc_Bool ti_sysbios_knl_Task_getPrivileged_SVC(
    ti_sysbios_knl_Task_Handle handle);

#undef ti_sysbios_knl_Task_Params_init
#define ti_sysbios_knl_Task_Params_init ti_sysbios_knl_Task_Params_init_SVC
extern xdc_Void ti_sysbios_knl_Task_Params_init_SVC(
    ti_sysbios_knl_Task_Params *prms);

#undef ti_sysbios_knl_Task_restore
#define ti_sysbios_knl_Task_restore ti_sysbios_knl_Task_restore_SVC
extern xdc_Void ti_sysbios_knl_Task_restore_SVC(xdc_UInt key);

#undef ti_sysbios_knl_Task_setPri
#define ti_sysbios_knl_Task_setPri ti_sysbios_knl_Task_setPri_SVC
extern xdc_UInt ti_sysbios_knl_Task_setPri_SVC(
    ti_sysbios_knl_Task_Handle handle, xdc_Int priority);

#undef ti_sysbios_knl_Task_sleep
#define ti_sysbios_knl_Task_sleep ti_sysbios_knl_Task_sleep_SVC
extern xdc_Void ti_sysbios_knl_Task_sleep_SVC(xdc_UInt32 timeout);

#undef ti_sysbios_knl_Task_yield
#define ti_sysbios_knl_Task_yield ti_sysbios_knl_Task_yield_SVC
extern xdc_Void ti_sysbios_knl_Task_yield_SVC(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
