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

#ifndef ti_sysbios_family_arm_m3_TimestampProvider__epilogue__include
#define ti_sysbios_family_arm_m3_TimestampProvider__epilogue__include

#ifdef __cplusplus
extern "C" {
#endif

#if defined(ti_sysbios_Build_useSvcWrappers)


#undef ti_sysbios_family_arm_m3_TimestampProvider_get32
#define ti_sysbios_family_arm_m3_TimestampProvider_get32 ti_sysbios_family_arm_m3_TimestampProvider_get32_SVC
extern xdc_Bits32 ti_sysbios_family_arm_m3_TimestampProvider_get32_SVC(xdc_Void);

#undef ti_sysbios_family_arm_m3_TimestampProvider_get64
#define ti_sysbios_family_arm_m3_TimestampProvider_get64 ti_sysbios_family_arm_m3_TimestampProvider_get64_SVC
extern xdc_Void ti_sysbios_family_arm_m3_TimestampProvider_get64_SVC(xdc_runtime_Types_Timestamp64 *result);

#undef ti_sysbios_family_arm_m3_TimestampProvider_getFreq
#define ti_sysbios_family_arm_m3_TimestampProvider_getFreq ti_sysbios_family_arm_m3_TimestampProvider_getFreq_SVC
extern xdc_Void ti_sysbios_family_arm_m3_TimestampProvider_getFreq_SVC(xdc_runtime_Types_FreqHz *freq);


#endif

#ifdef __cplusplus
}
#endif

#endif

