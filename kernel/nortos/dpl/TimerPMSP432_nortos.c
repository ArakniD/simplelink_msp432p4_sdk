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
 *  ======== TimerPMSP432_nortos.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <ti/devices/DeviceFamily.h>

#include <ti/drivers/dpl/HwiP.h>

#include "TimerP.h"


#define MC           0x0020        /* mode, control = continous up counting */
#define TxCLR        0x0004        /* clear timer counter */
#define CCIE         0x0010        /* compare interrupt enable */
#define CCIFG        0x0001        /* compare interrupt pending flag */
#define TSSEL        0x0100        /* source select = ACLK */
#define ID_MASK      0x00C0        /* input divider bit mask */

#define TIMER_CONTROL_START           MC
#define TIMER_CONTROL_CLEAR           TxCLR
#define TIMER_CONTROL_DEFAULT         TSSEL
#define TIMER_COMPARE_INTR_ENABLE     CCIE
#define TIMER_COMPARE_INTR_PENDING    CCIFG

#define TIMER_DELETED       0
#define BAD_TIMER_ID        1
#define NO_TIMER_AVAIL      2
#define NO_HWI_OBJ          3
#define BAD_PERIOD          4

#define READ_LATENCY                    10   /* # MCLK cycles to allow for
                                                back-to-back read of count */

#define SYS_PERI_HALTCTL_REG    0xE004300C   /* peripheral halt control reg */
#define REG(x)  (*((volatile unsigned *)(x)))

#define BASEADDR_TA0 0x40000000  /* Timer_TA0 */
#define INTNUM_TA0 24

/* Input divider (ID) bit field values */
#define ID_1 0x0                     /* 1 */
#define ID_2 0x40                    /* 2 */
#define ID_4 0x80                    /* 4 */
#define ID_8 0xC0                     /* 8 */

/* Input Divider Expansion (IDEX) bit field values */
#define IDEX_1 0x0                   /* 1 */
#define IDEX_2 0x1                   /* 2 */
#define IDEX_3 0x2                   /* 3 */
#define IDEX_4 0x3                   /* 4 */
#define IDEX_5 0x4                   /* 5 */
#define IDEX_6 0x5                   /* 6 */
#define IDEX_7 0x6                   /* 7 */
#define IDEX_8 0x7                   /* 8 */

/* ACLK */
#define CONTROLREGINIT (0x0100 | ID_1)
#define INPUTDIVIDEREXP IDEX_1

#define TIMER_FREQ_ACLK 32768

/*
 *  ======== TimerRegs ========
 */
typedef volatile struct TimerRegs {
    uint16_t control;
    uint16_t cctl_0;
    uint16_t cctl_1;
    uint16_t cctl_2;
    uint16_t cctl_3;
    uint16_t cctl_4;
    uint16_t cctl_5;
    uint16_t cctl_6;
    uint16_t count;
    uint16_t cc_compare_0;
    uint16_t cc_compare_1;
    uint16_t cc_compare_2;
    uint16_t cc_compare_3;
    uint16_t cc_compare_4;
    uint16_t cc_compare_5;
    uint16_t cc_compare_6;
    uint16_t expansion;
    uint16_t reserved[6];
    uint16_t vector;
} TimerRegs;


/*
 *  ======== TimerP_Obj ========
 */
typedef struct _TimerP_Obj {
    TimerP_StartMode        startMode;
    uint32_t                period;
    uintptr_t               arg;
    HwiP_Fxn                tickFxn;
    TimerP_FreqHz           frequency;
    uint64_t                period64;
    uint64_t                savedCurrCount;
    uint64_t                prevThreshold;
    uint64_t                nextThreshold;
    uint64_t                rollovers;
} TimerP_Obj;

static uint32_t TimerP_getCount(TimerP_Obj *obj);

static bool TimerP_initialized = false;
static TimerP_Obj *TimerP_handle;
static HwiP_Struct TimerP_hwiStruct;

/*
 *  ======== TimerP_defaultParams ========
 */
static TimerP_Params TimerP_defaultParams = {
    .startMode = TimerP_StartMode_AUTO,
    .arg = 0,
    .period = 10,
};


/*
 *  ======== TimerP_Params_init ========
 */
void TimerP_Params_init(TimerP_Params *params)
{
    /* structure copy */
    *params = TimerP_defaultParams;
}

/*
 *  ======== TimerP_construct ========
 */
TimerP_Handle TimerP_construct(TimerP_Struct *handle, TimerP_Fxn timerFxn,
        TimerP_Params *params)
{
    TimerP_Obj *obj = (TimerP_Obj *)handle;
    uintptr_t   hwiKey;

    if (handle != NULL) {
        hwiKey = HwiP_disable();

        if (!TimerP_initialized) {
            HwiP_Params hwiParams;

            HwiP_Params_init(&hwiParams);
            hwiParams.priority = 1;    // TODO
            HwiP_construct(&TimerP_hwiStruct, INTNUM_TA0, TimerP_dynamicStub,
                           &hwiParams);

            TimerP_handle = (TimerP_Obj *)handle;
            TimerP_handle->rollovers = 0;

            TimerP_initDevice(TimerP_handle);

            TimerP_initialized = true;
        }

        HwiP_restore(hwiKey);

        if (params == NULL) {
            params = (TimerP_Params *)&TimerP_defaultParams;
        }

        obj->startMode = params->startMode;
        obj->arg = params->arg;

        /*
         *  Period is in usec.
         */
        obj->period = ((TIMER_FREQ_ACLK / 1000) * params->period) / 1000;
        obj->tickFxn = timerFxn;
        obj->savedCurrCount = 0;
        obj->prevThreshold = obj->period;
        obj->nextThreshold = obj->period;

        TimerP_initDevice(obj);
        TimerP_setPeriod(obj, obj->period);

        if (obj->startMode == TimerP_StartMode_AUTO) {
            TimerP_start(obj);
        }
    }

    return ((TimerP_Handle)handle);
}

/*
 *  ======== TimerP_create ========
 */
TimerP_Handle TimerP_create(TimerP_Fxn timerFxn, TimerP_Params *params)
{
    TimerP_Handle handle;

    handle = (TimerP_Handle)malloc(sizeof(TimerP_Obj));

    /* TimerP_construct will check handle for NULL, no need here */
    handle = TimerP_construct((TimerP_Struct *)handle, timerFxn, params);

    return (handle);
}

/*
 *  ======== TimerP_getMaxTicks ========
 */
uint32_t TimerP_getMaxTicks(TimerP_Handle handle)
{
    TimerP_Obj *obj = (TimerP_Obj *)handle;

    return (0x0000FFFF / obj->period);
}

/*
 *  ======== TimerP_setNextTick ========
 */
void TimerP_setNextTick(TimerP_Handle handle, uint32_t ticks)
{
    TimerP_Obj *obj = (TimerP_Obj *)handle;
    TimerRegs  *timer;
    uint32_t    newPeriod = obj->period * ticks;
    uint16_t    next;
    uint16_t    previous;
    uint16_t    now;

    /* get timer base address */
    timer = (TimerRegs *)BASEADDR_TA0;

    previous = obj->prevThreshold;

    /* next is relative to savedCurrCount */
    next = (uint16_t)(obj->savedCurrCount + newPeriod);

    /* set the compare threshold for next interrupt */
    timer->cc_compare_0 = next;

    /*
     * MSP432 timers will only interrupt if the timer counts *to* the threshold.
     * So we must check to make sure that the threshold wasn't set too late -
     * if it was set late, the timer count will need to wrap before the next
     * interrupt is triggered.  To avoid this, after writing the next
     * interrupt threshold, we read the current count, and if the interrupt
     * threshold was set too late per the current count, we again write
     * to the timer to trigger an immediate interrupt.
     *
     * For the cases where the 'next' threshold is more than the 'previous',
     * the timer will not wrap before the next target tick interrupt.  This
     * creates three regions shown below (A, B, C), where the current 'now'
     * count will reside:
     *
     *  FFFF |                  -
     *       |                  B
     *       |<----- next       -
     *       |                  A
     *       |<----- previous   -
     *       |                  C
     *    0  |                  -
     *
     * A - 'now' is greater than equal to 'previous', and less than 'next':
     * this is the 'normal' region, where the threshold has been set before the
     * next threshold, and no adjustment is needed.
     *
     * B - 'now' is greater than equal to 'next': the threshold was set
     * late, trigger an interrupt now.
     *
     * C - 'now' is less than 'previous': the threshold was set late, trigger
     * an interrupt now.
     *
     * For the cases where the 'next' threshold is less than the 'previous',
     * the timer count is expected to wrap before the next tick interrupt.
     * This creates the three regions shown below (D, E, F), where the current
     * 'now' count will reside:
     *
     *  FFFF |                  -
     *       |                  D
     *       |<----- previous   -
     *       |                  F
     *       |<----- next       -
     *       |                  E
     *    0  |                  -
     *
     * D - 'now' is greater than equal to 'previous': this is a 'normal'
     * region, where the threshold has been set before the next threshold,
     * and no adjustment is needed.
     *
     * E - 'now' is less than 'next': this is another 'normal' region,
     * where the threshold has been set early enough, and no adjustment is
     * needed.
     *
     * F - 'now' is greater than or equal to 'next' and less than
     * 'previous': the threshold was set too late, trigger an interrupt now.
     */

    /* get current timer count */
    now = (uint16_t)TimerP_getCount(obj);

    /* if not expecting wrap thru zero ... */
    if (next > previous) {

        /* case "B": now is greater than or equal to next, interrupt now */
        if (now >= next) {
            timer->cctl_0 |= CCIFG;
        }

        /* case "C": now < previous, count has wrapped, interrupt now */
        else {
            if (now < previous) {
                timer->cctl_0 |= CCIFG;
            }
        }
    }

    /* else, expecting wrapping thru zero ... */
    else {
        /* case F: now is between next and previous, interrupt now */
        if ((now < previous) && (now >= next)) {
            timer->cctl_0 |= CCIFG;
        }
    }
}

/*
 *  ======== TimerP_getCount ========
 */
uint32_t TimerP_getCount(TimerP_Obj *obj)
{
    TimerRegs *timer;
    uint32_t   first;
    uint32_t   second;

    timer = (TimerRegs *)BASEADDR_TA0;

    /* read twice, expecting equal values, or within read latency */
    do {
        first = timer->count;
        second = timer->count;
    } while ((first != second) && ((second - first) > READ_LATENCY));

    return (first & 0xffff);
}

/*
 *  ======== TimerP_getCount64 ========
 */
uint64_t TimerP_getCount64(TimerP_Handle handle)
{
    TimerP_Obj *obj = (TimerP_Obj *)handle;
    uint64_t    rollovers;
    uint64_t    currCount;
    uintptr_t   key;

    key = HwiP_disable();

    currCount = TimerP_getCount(obj);
    rollovers = obj->rollovers;

    if (currCount < obj->prevThreshold) {
        rollovers += 1;
    }

    HwiP_restore(key);

    currCount = (rollovers << 16) + currCount;

    return (currCount);
}

/*
 *  ======== TimerP_getFreq ========
 */
void TimerP_getFreq(TimerP_Handle handle, TimerP_FreqHz *freq)
{
    freq->lo = TIMER_FREQ_ACLK;
    freq->hi = 0;
}

/*
 *  ======== TimerP_startup ========
 */
void TimerP_startup()
{
    TimerP_Obj *obj;

    obj = TimerP_handle;
    /* if timer was statically created/constructed */
    if (obj != NULL) {
        if (obj->startMode == TimerP_StartMode_AUTO) {
            TimerP_start(obj);
        }
    }
}

/*
 *  ======== TimerP_start ========
 */
void TimerP_start(TimerP_Handle handle)
{
    TimerP_Obj *obj = (TimerP_Obj *)handle;
    TimerRegs  *timer;
    uint32_t    key;

    timer = (TimerRegs *)BASEADDR_TA0;

    key = HwiP_disable();

    /* reset timer counts */
    timer->control |= TIMER_CONTROL_CLEAR;

    /* clear timer interrupt flag */
    timer->cctl_0 &= ~TIMER_COMPARE_INTR_PENDING;

    /* zero previous threshold */
    obj->prevThreshold = 0;

    /* set compare threshold for next interrupt */
    timer->cc_compare_0 = obj->period;

    /* enable capture/compare 0 interrupts */
    timer->cctl_0 |= TIMER_COMPARE_INTR_ENABLE;

    /* start timer */
    timer->control |= TIMER_CONTROL_START;

    HwiP_restore(key);
}

/*
 *  ======== TimerP_stop ========
 */
void TimerP_stop(TimerP_Handle handle)
{
    TimerP_Obj *obj = (TimerP_Obj *)handle;

    /* not implemented for this timer */
    obj->savedCurrCount = 0;
}

/*
 *  ======== TimerP_setPeriod ========
 */
void TimerP_setPeriod(TimerP_Handle handle, uint32_t period)
{
    TimerP_Obj *obj = (TimerP_Obj *)handle;
    obj->period = period;
}

/*
 *  ======== TimerP_getCurrentTick ========
 *  Used by the Clock module for TickMode_DYNAMIC to query the corresponding
 *  Clock tick, as derived from the current timer count.
 *  Must be called with interrupts disabled.
 */
uint32_t TimerP_getCurrentTick(TimerP_Handle handle, bool saveFlag)
{
    TimerP_Obj *obj = (TimerP_Obj *)handle;
    uint64_t    rollovers;
    uint32_t    currCount;
    uint32_t    tick;
    uint32_t    s1;
    uint32_t    r;

    currCount = TimerP_getCount(obj);

    rollovers = obj->rollovers;

    if (currCount < obj->prevThreshold) {
        rollovers += 1;
    }

    /*
     *  This has been copied from the SYS/BIOS msp432 timer code, which
     *  is based on the MSP430 timer code.  The tick calculation was
     *  done to reduce long division.
     */
    s1 = rollovers / obj->period;            /* upper 32 of 48 bits */
    r = rollovers - s1 * obj->period;        /* remainder of upper division */
    tick = (((uint32_t)r << 16) + currCount) / obj->period; /* lower 16-bits */
    tick = ((uint32_t)(s1 & 0xFFFF) << 16) + (tick & 0xFFFF);

    /* save for later use by TimerP_setNextTick() */
    if (saveFlag != 0) {
        obj->savedCurrCount = tick * obj->period;
    }

    return (tick);
}

/* ======== TimerP_initDevice ========
 *
 * 1. Stop timer (set control registers back to default value)
 * 2. Disable timer interrupt
 * 3. Clear any pending interrupt
 * 4. Clear counters
 *
 */
void TimerP_initDevice(TimerP_Handle handle)
{
//    TimerP_Obj *obj = (TimerP_Obj *)handle;
    TimerRegs  *timer;
    uintptr_t   key;

    timer = (TimerRegs *)BASEADDR_TA0;

    key = HwiP_disable();

    /* stop timer; set default control register */
    timer->control = MC | CONTROLREGINIT;
    timer->expansion = INPUTDIVIDEREXP;

    /* clear timer interrupt flag */
    timer->cctl_0 &= ~TIMER_COMPARE_INTR_PENDING;

    /* reset timer counts */
    timer->control |= TIMER_CONTROL_CLEAR;

    /* freeze the timer when the CPU is halted */
    REG(SYS_PERI_HALTCTL_REG) |= 1;  /* Assuming timer id of 0 */

    HwiP_restore(key);
}

/*
 *  ======== TimerP_dynamicStub ========
 */
void TimerP_dynamicStub(uintptr_t arg)
{
    TimerP_Obj    *obj;
    TimerRegs     *timer;

    obj = TimerP_handle;

    timer = (TimerRegs *)BASEADDR_TA0;

    /* clear timer interrupt flag */
    timer->cctl_0 &= ~TIMER_COMPARE_INTR_PENDING;

    /* for DYNAMIC, mode latch prevThreshold and detect rollovers */
    /*
     * If the current threshold is less than the prevThreshold
     * then a rollover has occurred.
     *
     * If the current threshold is equal to prevThreshold then the timer
     * count wrapped around and generated a new interrupt; increment
     * the rollover count to resume ticking.
     */
    if (obj->prevThreshold >= timer->cc_compare_0) {
        obj->rollovers += 1;
    }

    /* save previous threshold value */
    obj->prevThreshold = timer->cc_compare_0;

    /* call the tick function */
    obj->tickFxn(obj->arg);
}
