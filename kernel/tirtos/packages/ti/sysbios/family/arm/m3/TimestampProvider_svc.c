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
 *  ======== TimestampProvider_svc.c ========
 */

#include <xdc/std.h>

#include <xdc/runtime/Types.h>

#include <ti/sysbios/hal/SysCall.h>

#include "package/internal/TimestampProvider.xdc.h"

/*
 *  ======== ti_sysbios_family_arm_m3_TimestampProvider_get32_SVC ========
 */
Bits32 ti_sysbios_family_arm_m3_TimestampProvider_get32_SVC(Void)
{
    Bits32 ts;

    SysCall_enterPrivMode();
    ts = TimestampProvider_get32();
    SysCall_enterUnprivMode();

    return (ts);
}

/*
 *  ======== ti_sysbios_family_arm_m3_TimestampProvider_get64_SVC ========
 */
Void ti_sysbios_family_arm_m3_TimestampProvider_get64_SVC(Types_Timestamp64 *result)
{
    SysCall_enterPrivMode();
    TimestampProvider_get64(result);
    SysCall_enterUnprivMode();
}

/*
 *  ======== ti_sysbios_family_arm_m3_TimestampProvider_getCounterFreq_SVC  ========
 */
Void ti_sysbios_family_arm_m3_TimestampProvider_getFreq_SVC(Types_FreqHz *freq)
{
    SysCall_enterPrivMode();
    TimestampProvider_getFreq(freq);
    SysCall_enterUnprivMode();
}

