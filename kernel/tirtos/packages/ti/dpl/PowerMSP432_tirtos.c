/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
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
 *  ======== PowerMSP432_tirtos.c ========
 */

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/arm/msp432/ClockFreqs.h>

#include <ti/devices/DeviceFamily.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>

#include <ti/devices/msp432p4xx/driverlib/cpu.h>

UInt PowerMSP432_taskKey;
UInt PowerMSP432_swiKey;

/*
 *  ======== PowerMSP432_deepSleepPolicy ========
 */
void PowerMSP432_deepSleepPolicy()
{
    UInt constraints;
    Bool slept = false;
    UInt taskKey;
    UInt swiKey;

    /* disable interrupts */
    CPU_cpsid();

    /* disable Swi scheduling */
    swiKey = Hwi_swiDisable();

    /* disable Task scheduling */
    taskKey = Task_disable();

    /* query the declared constraints */
    constraints = Power_getConstraintMask();

    /*
     *  Check if can go to a sleep state, starting with the deepest level.
     *  Do not go to a sleep state if a lesser sleep state is disallowed.
     */

     /* check if can go to DEEPSLEEP_1 */
    if ((constraints & ((1 << PowerMSP432_DISALLOW_SLEEP) |
                        (1 << PowerMSP432_DISALLOW_DEEPSLEEP_0) |
                        (1 << PowerMSP432_DISALLOW_DEEPSLEEP_1))) == 0) {

        /* go to DEEPSLEEP_1 */
        Power_sleep(PowerMSP432_DEEPSLEEP_1);

        /* set 'slept' to true*/
        slept = true;
    }

    /* if didn't sleep yet, now check if can go to DEEPSLEEP_0 */
    if (!slept && ((constraints & ((1 << PowerMSP432_DISALLOW_SLEEP) |
                        (1 << PowerMSP432_DISALLOW_DEEPSLEEP_0))) == 0)) {

        /* go to DEEPSLEEP_0 */
        Power_sleep(PowerMSP432_DEEPSLEEP_0);

        /* set 'slept' to true*/
        slept = true;
    }

    /* if didn't sleep yet, now check if can go to SLEEP */
    if (!slept && ((constraints & (1 << PowerMSP432_DISALLOW_SLEEP)) == 0)) {

        /* go to SLEEP */
        Power_sleep(PowerMSP432_SLEEP);

        /* set 'slept' to true*/
        slept = true;
    }

    /* re-enable interrupts */
    CPU_cpsie();

    /* restore Swi scheduling */
    Hwi_swiRestore(swiKey);

    /* restore Task scheduling */
    Task_restore(taskKey);

    /* if didn't sleep yet, just do WFI */
    if (!slept) {
        __asm(" wfi");
    }
}

/*
 *  ======== PowerMSP432_sleepPolicy ========
 */
void PowerMSP432_sleepPolicy()
{
    UInt constraints;
    Bool slept = false;
    UInt taskKey;
    UInt swiKey;

    /* disable interrupts */
    CPU_cpsid();

    /* disable Swi scheduling */
    swiKey = Hwi_swiDisable();

    /* disable Task scheduling */
    taskKey = Task_disable();

    /* query the declared constraints */
    constraints = Power_getConstraintMask();

    /* sleep, if there is no constraint prohibiting it */
    if ((constraints & (1 << PowerMSP432_DISALLOW_SLEEP)) == 0) {

        /* go to SLEEP */
        Power_sleep(PowerMSP432_SLEEP);

        /* set 'slept' to true*/
        slept = true;
    }

    /* re-enable interrupts */
    CPU_cpsie();

    /* restore Swi scheduling */
    Hwi_swiRestore(swiKey);

    /* restore Task scheduling */
    Task_restore(taskKey);

    /* if didn't sleep yet, just do WFI */
    if (!slept) {
        __asm(" wfi");
    }
}

/*
 *  ======== PowerMSP432_initPolicy ========
 */
void PowerMSP432_initPolicy()
{
}

/*
 *  ======== PowerMSP432_schedulerDisable ========
 */
void PowerMSP432_schedulerDisable()
{
    PowerMSP432_taskKey = Task_disable();
    PowerMSP432_swiKey = Hwi_swiDisable();
}

/*
 *  ======== PowerMSP432_schedulerRestore ========
 */
void PowerMSP432_schedulerRestore()
{
    Hwi_swiRestore(PowerMSP432_swiKey);
    Task_restore(PowerMSP432_taskKey);
}

/*
 *  ======== PowerMSP432_updateFreqs ========
 */
void PowerMSP432_updateFreqs(PowerMSP432_Freqs *freqs)
{
    Types_FreqHz cpuFreq;

    cpuFreq.hi = 0;
    cpuFreq.lo = freqs->MCLK;
    BIOS_setCpuFreq(&cpuFreq);

    ClockFreqs_setFrequency(ClockFreqs_Clock_ACLK, freqs->ACLK);
    ClockFreqs_setFrequency(ClockFreqs_Clock_SMCLK, freqs->SMCLK);
    ClockFreqs_setFrequency(ClockFreqs_Clock_HSMCLK, freqs->HSMCLK);
}
