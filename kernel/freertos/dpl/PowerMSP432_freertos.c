/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 *  ======== PowerMSP432_freertos.c ========
 */

#include "FreeRTOS.h"
#include "task.h"

#include <ti/devices/DeviceFamily.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>

#include <ti/devices/msp432p4xx/driverlib/cpu.h>

/*
 *  ======== PowerMSP432_deepSleepPolicy ========
 */
void PowerMSP432_deepSleepPolicy()
{
    uint32_t constraints;
    bool slept = false;

    /* disable interrupts */
    CPU_cpsid();

    /* disable Task scheduling */
    vTaskSuspendAll();

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

    /* restore Task scheduling */
    xTaskResumeAll();

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
    uint32_t constraints;
    bool slept = false;

    /* disable interrupts */
    CPU_cpsid();

    /* disable Task scheduling */
    vTaskSuspendAll();

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

    /* restore Task scheduling */
    xTaskResumeAll();

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
#if defined(__DISALLOW_SLEEP__)
    Power_setConstraint(PowerMSP432_DISALLOW_SLEEP)
#endif
}

/*
 *  ======== PowerMSP432_schedulerDisable ========
 */
void PowerMSP432_schedulerDisable()
{
    vTaskSuspendAll();
}

/*
 *  ======== PowerMSP432_schedulerRestore ========
 */
void PowerMSP432_schedulerRestore()
{
    xTaskResumeAll();
}
void init_swo(uint32_t cpuCoreFreqHz, uint32_t SWOSpeed) /* cpuCoreFreqHz = 300000000 */
{
    uint32_t SWOPrescaler = (cpuCoreFreqHz/SWOSpeed) - 1; /* SWOSpeed in Hz, note that cpuCoreFreqHz is expected to be match the CPU core clock */
    /* Configure the TPIU - pages and info from ATSAMS70N20 datasheet */
    CoreDebug->DEMCR = (1UL<<24);           /* page 83, step 1 - "Debug exception and monitor register": bit 24 is TRCENA which enables DWT and ITM units. page 766 at https://static.docs.arm.com/ddi0403/eb/DDI0403E_B_armv7m_arm.pdf */
    TPI->SPPR = 0x00000002;                 /* page 83, step 2 - "Selected PIN Protocol Register": Select which protocol to use for trace output (2: SWO UART/NRZ, 1: SWO Manchester encoding) */
    TPI->FFCR = 0x00000100;                 /* page 83, step 3 - Formatter and Flush Control Register */
    TPI->ACPR = SWOPrescaler;               /* page 83, step 4 - "Async Clock Prescaler Register". Scale the baud rate of the asynchronous output */
    // This value lines up with 1MBit baud rate
    //TPI->ACPR = 0x22;
    ITM->LAR = 0xC5ACCE55;                  /* ITM Lock Access Register, C5ACCE55 enables more write access to Control Register 0xE00 :: 0xFFC */
    ITM->TCR |= ((1UL<<16) | (1UL<<4) | (1UL<<2) | (1UL<<0));   /* pg 776, ITM Trace Control Register at https://static.docs.arm.com/ddi0403/eb/DDI0403E_B_armv7m_arm.pdf;
                                                                    - set TraceBusID to 1,
                                                                    - enable SWOENA (async clocking of timestamp counter
                                                                    - enable SYNCENA (Synchronization packet transmission for a synchronous TPIU)
                                                                    - enable ITMENA (enable ITM)*/
    ITM->TER = (0xFFFF);                    /* ITM Trace Enable Register. Each bit location corresponds to a virtual stimulus register; when a bit is set, a write to the appropriate stimulus location results in a packet being generated, except when the FIFO is full.
                                                - only ITM_TER0 is implemented as ITM_TER according to one register in .h file. Registers ITM_TER1 to ITM_TER7 seem unimplemented
                                                - ITM_TER0.0 - ITM_STIM0 (ITM->PORT[0])
                                                - ITM_TER0.1 - ITM_STIM1 (ITM->PORT[1])
                                                - ITM_TER0.31 - ITM_STIM31 (ITM->PORT[31]) */
    DWT->CTRL = 0x400003FE; /* DWT_CTRL */
    /* Check if Trace Control Register (ITM->TCR at 0xE0000E80) is set */
    if ((ITM->TCR & (1UL<<0)) == 0) /* check Trace Control Register if ITM trace is enabled (ITM->TCR.ITMENA)*/
        while(1){} /* not enabled? */
    /* Check if the requested channel stimulus port (ITM->TER at 0xE0000E00) is enabled */
    if ((ITM->TER & (1UL<<0))==0) /* check Trace Enable Register if requested port is enabled */
        while(1){} /* requested port not enabled? */
}

/*
 *  ======== PowerMSP432_updateFreqs ========
 */
void PowerMSP432_updateFreqs(PowerMSP432_Freqs *freqs)
{
    /* Setup the SWOCLK based on profile */
    init_swo(freqs->MCLK , freqs->SWOCLK);
}

/* Tickless Hook */
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
#if (configUSE_TICKLESS_IDLE != 0)
    Power_idleFunc();
#endif
}
