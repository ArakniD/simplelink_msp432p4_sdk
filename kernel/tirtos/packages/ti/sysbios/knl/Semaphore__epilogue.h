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

#ifndef ti_sysbios_knl_Semaphore__epilogue__include
#define ti_sysbios_knl_Semaphore__epilogue__include

#ifdef __cplusplus
extern "C" {
#endif

#if defined(ti_sysbios_Build_useSvcWrappers)

#undef ti_sysbios_knl_Semaphore_construct
#define ti_sysbios_knl_Semaphore_construct ti_sysbios_knl_Semaphore_construct_SVC
extern xdc_Void ti_sysbios_knl_Semaphore_construct_SVC(
    ti_sysbios_knl_Semaphore_Struct *obj, xdc_Int count,
    const ti_sysbios_knl_Semaphore_Params *prms);

#undef ti_sysbios_knl_Semaphore_destruct
#define ti_sysbios_knl_Semaphore_destruct ti_sysbios_knl_Semaphore_destruct_SVC
extern xdc_Void ti_sysbios_knl_Semaphore_destruct_SVC(
    ti_sysbios_knl_Semaphore_Struct *obj);

#undef ti_sysbios_knl_Semaphore_getCount
#define ti_sysbios_knl_Semaphore_getCount ti_sysbios_knl_Semaphore_getCount_SVC
extern xdc_Int ti_sysbios_knl_Semaphore_getCount_SVC(
    ti_sysbios_knl_Semaphore_Handle sem);

#undef ti_sysbios_knl_Semaphore_Params_init
#define ti_sysbios_knl_Semaphore_Params_init ti_sysbios_knl_Semaphore_Params_init_SVC
extern xdc_Void ti_sysbios_knl_Semaphore_Params_init_SVC(
    ti_sysbios_knl_Semaphore_Params *prms);

#undef ti_sysbios_knl_Semaphore_pend
#define ti_sysbios_knl_Semaphore_pend ti_sysbios_knl_Semaphore_pend_SVC
extern xdc_Bool ti_sysbios_knl_Semaphore_pend_SVC(
    ti_sysbios_knl_Semaphore_Handle sem, xdc_UInt32 timeout);

#undef ti_sysbios_knl_Semaphore_post
#define ti_sysbios_knl_Semaphore_post ti_sysbios_knl_Semaphore_post_SVC
extern xdc_Void ti_sysbios_knl_Semaphore_post_SVC(
    ti_sysbios_knl_Semaphore_Handle handle);

#endif

#ifdef __cplusplus
}
#endif

#endif
