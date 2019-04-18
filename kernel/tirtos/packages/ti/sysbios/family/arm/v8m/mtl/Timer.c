/*
 * Copyright (c) 2019, Texas Instruments Incorporated
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
 *  ======== Timer.c ========
 *
 */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/Types.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/arm/v8m/Hwi.h>

#include "package/internal/Timer.xdc.h"

#define TIMER_DELETED   0
#define BAD_TIMER_ID    1
#define NO_TIMER_AVAIL  2
#define NO_HWI_OBJ      3
#define BAD_PERIOD      4

/* SYSRTC structure definitions, extracted from VSOC P3 sysrtc.h */
typedef struct {
    uint32_t RESERVED0[2];
    uint32_t IMASK;          /* !< (@ 0x00011028) Interrupt mask */
    uint32_t RESERVED1;
    uint32_t RIS;            /* !< (@ 0x00011030) Raw interrupt status */
    uint32_t RESERVED2;
    uint32_t MIS;            /* !< (@ 0x00011038) Masked interrupt status */
    uint32_t RESERVED3;
    uint32_t ISET;           /* !< (@ 0x00011040) Interrupt set */
} SYSRTC_CHANNEL_INT_EVENT_Regs;

typedef struct {
    uint32_t RESERVED0[62];
    uint32_t DESC_EX;        /* !< (@ 0x000010F8) IP configuration options */
    uint32_t DESC;           /* !< (@ 0x000010FC) Module Description */
    uint32_t UTIME;          /* !< (@ 0x00001100) us view */
    uint32_t STIME;          /* !< (@ 0x00001104) seconds view */
    uint32_t MTIME;          /* !< (@ 0x00001108) ms view */
    uint32_t RESERVED1;
    uint32_t LFINC;          /* !< (@ 0x00001110) Increment count used */
    uint32_t LOCALTIME;      /* !< (@ 0x00001114) Current local time */
    uint32_t WALLTIME;       /* !< (@ 0x00001118) Current wall time */
    uint32_t WALLTIME_BCD;   /* !< (@ 0x0000111C) Current wall time */
    uint32_t DAYS;           /* !< (@ 0x00001120) Days since epoch */
    uint32_t WALLTIME_FULL;  /* !< (@ 0x00001124) Current wall time and days */
    uint32_t RESERVED2[3];
    uint32_t LFINC_MAN;        /* !< (@ 0x00001134) Manual increment override */
    uint32_t LOCALTIME_EPOCH;  /* !< (@ 0x00001138) Local time epoch */
    uint32_t LOCALTIME_OFFSET; /* !< (@ 0x0000113C) Local time offset */
} SYSRTC_STAT_Regs;

typedef struct {
    uint32_t RESERVED0[256];
    uint32_t FSUB_0;            /* !< (@ 0x00010400) Subscriber port. */
    uint32_t RESERVED1[15];
    uint32_t FPUB_0;            /* !< (@ 0x00010440) Publisher port. */
    uint32_t RESERVED2[15];
    uint32_t CPU_CONNECT_0;     /* !< (@ 0x00010480) CPU connect config byte */
    uint32_t RESERVED3[743];
    SYSRTC_CHANNEL_INT_EVENT_Regs INT_EVENT; /* !< (@ 0x00011020) */
    uint32_t RESERVED4[39];
    uint32_t EVT_MODE;        /* !< (@ 0x000110E0) Event Mode */
    uint32_t RESERVED5[5];
    uint32_t DESC_EX;         /* !< (@ 0x000110F8) IP configuration options */
    uint32_t DESC;            /* !< (@ 0x000110FC) Module Description */
    uint32_t CTL;             /* !< (@ 0x00011100) Status / compare/ capture */
   uint32_t UTIMEVAL;         /* !< (@ 0x00011104) us value */
   uint32_t STIMEVAL;         /* !< (@ 0x00011108) seconds value */
   uint32_t MTIMEVAL;         /* !< (@ 0x0001110C) ms value */
   uint32_t UTIMEDVAL;        /* !< (@ 0x00011110) us value, relative */
   uint32_t STIMEDVAL;        /* !< (@ 0x00011114) seconds value, relative */
   uint32_t MTIMEDVAL;        /* !< (@ 0x00011118) ms value, relative */
   uint32_t PERVAL;           /* !< (@ 0x0001111C) Periodic compare mode */
   uint32_t PERDVAL;          /* !< (@ 0x00011120) Periodic compare mode */
   uint32_t RESERVED6[951];
} SYSRTC_CHANNEL_Regs;

typedef struct {
  uint32_t RESERVED[1024];
  SYSRTC_STAT_Regs  STAT;            /* !< (@ 0x00001000) */
  uint32_t RESERVED0[15280];
  SYSRTC_CHANNEL_Regs  CHANNEL[32];  /* !< (@ 0x00010000) */
} SYSRTC_Regs;

#define SYSRTC_BASE      0x40100000U /*!< Base address of module SYSRTC */
#define SYSRTC_STAT_OFS  0x00001000U

SYSRTC_STAT_Regs * RTC_stat = (SYSRTC_STAT_Regs *)(SYSRTC_BASE +
    SYSRTC_STAT_OFS);
SYSRTC_Regs * RTC_regs = (SYSRTC_Regs *) SYSRTC_BASE;

/*
 *  ======== Timer_getNumTimers ========
 *  Get number of timer peripherals available on the device.
 */
UInt Timer_getNumTimers()
{
    return (Timer_numTimerDevices);
}

/*
 *  ======== Timer_getStatus ========
 *  Get the FREE/INUSE status of the timer.
 */
Timer_Status Timer_getStatus(UInt timerId)
{
    Assert_isTrue(timerId < Timer_numTimerDevices, NULL);

    if (Timer_module->availMask & (0x1 << timerId)) {
        return (Timer_Status_FREE);
    }
    else {
        return (Timer_Status_INUSE);
    }
}

/*
 *  ======== Timer_getMaxTicks ========
 */
UInt32 Timer_getMaxTicks(Timer_Object *obj)
{
    return (0);
}

/*
 *  ======== Timer_setAvailMask ========
 */
Bool Timer_setAvailMask(UInt mask)
{
    UInt i;
    UInt key;
    UInt tmpMask;

    key = Hwi_disable();
    tmpMask = mask;
    for (i = 0; i < Timer_numTimerDevices; i++) {
        /* Check if mask is setting any currently used timer as available */
        if ((tmpMask & 0x1) && (Timer_module->handles[i] != NULL)) {
            Hwi_restore(key);
            return (FALSE);
        }
        tmpMask = tmpMask >> 1;
    }
    Timer_module->availMask = mask;
    Hwi_restore(key);

    return (TRUE);
}

/*
 *  ======== Timer_setNextTick ========
 */
Void Timer_setNextTick(Timer_Object *obj, UInt32 ticks)
{
}

/*
 *  ======== Timer_setThreshold ========
 */
Void Timer_setThreshold(Timer_Object *obj, UInt32 next, Bool wrap)
{
    RTC_regs->CHANNEL[0].UTIMEVAL = next;
}

/*
 *  ======== Timer_Module_startup ========
 *  Calls postInit for all statically-created & constructed
 *  timers to initialize them.
 */
Int Timer_Module_startup(Int status)
{
    Timer_Object *obj;
    Int i;

    if (Timer_startupNeeded) {
        for (i = 0; i < Timer_numTimerDevices; i++) {
            obj = Timer_module->handles[i];
            /* if timer was statically created/constructed */
            if ((obj != NULL) && (obj->staticInst)) {
                Timer_postInit(obj, NULL);
            }
        }
    }

    return (Startup_DONE);
}

/*
 *  ======== Timer_startup ========
 *  Here after call to main().  Called from BIOS_start().
 */
Void Timer_startup()
{
    Timer_Object *obj;
    Int i;

    if (Timer_startupNeeded) {
        for (i = 0; i < Timer_numTimerDevices; i++) {
            obj = Timer_module->handles[i];
            /* if timer was statically created/constructed */
            if ((obj != NULL) && (obj->staticInst)) {
                if (obj->startMode == Timer_StartMode_AUTO) {
                    Timer_start(obj);
                }
            }
        }
    }
}

/*
 *  ======== Timer_getHandle ========
 */
Timer_Handle Timer_getHandle(UInt id)
{
    Assert_isTrue((id < Timer_numTimerDevices), NULL);
    return (Timer_module->handles[id]);
}

/*
 *  ======== Timer_Instance_init ========
 * 1. Select timer based on id
 * 2. Mark timer as in use
 * 3. Init obj using params
 * 4. Create Hwi if tickFxn !=NULL
 * 5. Initialize timer
 * 6. Configure timer (wrt emulation, frequency etc)
 * 7. Set period
 * 8. Timer_start()
 */
Int Timer_Instance_init(Timer_Object *obj, Int id, Timer_FuncPtr tickFxn, const Timer_Params *params, Error_Block *eb)
{
    Hwi_Params hwiParams;
    UInt tempId = 0xffff;
    Int status;
    UInt key;
    Int i;

    if (id >= Timer_numTimerDevices) {
        if (id != Timer_ANY) {
            Error_raise(eb, Timer_E_invalidTimer, id, 0);
            return (BAD_TIMER_ID);
        }
    }

    key = Hwi_disable();

    if (id == Timer_ANY) {
        for (i = 0; i < Timer_numTimerDevices; i++) {
            if ((Timer_anyMask & (1 << i))
                && (Timer_module->availMask & (1 << i))) {
                Timer_module->availMask &= ~(1 << i);
                tempId = i;
                break;
            }
        }
    }
    else if (Timer_module->availMask & (1 << id)) {
        Timer_module->availMask &= ~(1 << id);
        tempId = id;
    }

    Hwi_restore(key);

    obj->staticInst = FALSE;

    if (tempId == 0xffff) {
        Error_raise(eb, Timer_E_notAvailable, id, 0);
        return (NO_TIMER_AVAIL);
    }
    else {
        obj->id = tempId;
    }

    obj->runMode = params->runMode;
    obj->startMode = params->startMode;
    obj->period = params->period;
    obj->periodType = params->periodType;
    obj->arg = params->arg;
    obj->intNum = Timer_module->device[obj->id].intNum;
    obj->tickFxn = tickFxn;
    obj->prevThreshold = 0;
    obj->nextThreshold = 0;
    obj->savedCurrCount = 0;
    obj->extFreq.lo = params->extFreq.lo;
    obj->extFreq.hi = params->extFreq.hi;

    if (obj->tickFxn) {
        if (params->hwiParams) {
            Hwi_Params_copy(&hwiParams, (params->hwiParams));
        }
        else {
            Hwi_Params_init(&hwiParams);
        }

        hwiParams.arg = (UArg)obj->id;

        if (obj->runMode == Timer_RunMode_CONTINUOUS) {
            obj->hwi = Hwi_create (obj->intNum, Timer_periodicStub,
                &hwiParams, eb);
        }
        else {
            obj->hwi = Hwi_create (obj->intNum, Timer_oneShotStub,
                &hwiParams, eb);
        }

        if (obj->hwi == NULL) {
            return (NO_HWI_OBJ);
        }
    }
    else {
        obj->hwi = NULL;
    }

    Timer_module->handles[obj->id] = obj;

    Timer_initDevice(obj);

    if (obj->periodType == Timer_PeriodType_MICROSECS) {
        if (!Timer_setPeriodMicroSecs(obj, obj->period)) {
            Error_raise(eb, Timer_E_cannotSupport, obj->period, 0);
            Hwi_restore(key);
            return (BAD_PERIOD);
        }
    }

    status = Timer_postInit(obj, eb);

    if (status) {
        return (status);
    }

    if (obj->startMode == Timer_StartMode_AUTO) {
        Timer_start(obj);
    }

    return (0);
}

/*
 *  ======== Timer_Instance_finalize ========
 */
Void Timer_Instance_finalize(Timer_Object *obj, Int status)
{
    UInt key;

    /* note: fall through in switch below is intentional */
    switch (status) {
        /* Timer_delete() */
        case TIMER_DELETED:

        /* setPeriodMicroSecs failed */
        case BAD_PERIOD:
            Timer_initDevice(obj);
            if (obj->hwi) {
                Hwi_delete(&obj->hwi);
            }

        /* Hwi create failed */
        case NO_HWI_OBJ:

        /* timer not available */
        case NO_TIMER_AVAIL:

        /* invalid timer id */
        case BAD_TIMER_ID:

        default:
            break;
    }

    key = Hwi_disable();
    Timer_module->availMask |= (0x1 << obj->id);
    Timer_module->handles[obj->id] = NULL;
    Hwi_restore(key);
}

/*
 *  ======== Timer_reconfig ========
 *
 * 1. Init obj using params
 * 2. Timer_init()
 * 3. Configure timer (wrt emulation, frequency, etc.)
 * 4. Set period
 * 5. Timer_start()
 *
 */
Void Timer_reconfig (Timer_Object *obj, Timer_FuncPtr tickFxn, const
    Timer_Params *params, Error_Block *eb)
{
    obj->runMode = params->runMode;
    obj->startMode = params->startMode;
    obj->periodType = params->periodType;
    if (obj->periodType == Timer_PeriodType_MICROSECS) {
        if (!Timer_setPeriodMicroSecs(obj, params->period)) {
            Error_raise(eb, Timer_E_cannotSupport, params->period, 0);
        }
    }
    else {
        obj->period = params->period;
    }

    obj->arg = params->arg;
    obj->tickFxn = tickFxn;

    if (params->extFreq.lo) {              /* (extFreq.hi is ignored) */
        obj->extFreq.lo = params->extFreq.lo;
    }

    Timer_postInit(obj, eb);

    if (obj->startMode == Timer_StartMode_AUTO) {
        Timer_start(obj);
    }
}

/*
 *  ======== Timer_start ========
 *
 * 1. Hwi_disable()
 * 2. Reset the timer channel
 * 3. Clear any pending channel events
 * 4. Enable events from channel to CPU
 * 5. Set first compare threshold to 'start' timer
 * 6. Hwi_restore()
 *
 */
Void Timer_start(Timer_Object *obj)
{
    UInt32 previous;
    UInt32 next;
    UInt key;

    key = Hwi_disable();

    /* reset the channel */
    RTC_regs->CHANNEL[obj->id].CTL = 0;

    /* clear any pending events */
    /* !!! TO DO */

    /* enable events from channel */
    RTC_regs->CHANNEL[obj->id].CPU_CONNECT_0 = 0x2; /* !!! fixed to CPU0 */
    RTC_regs->CHANNEL[obj->id].INT_EVENT.IMASK = 0x1;

    /* compute and save thresholds, set compare value to start channel */
    obj->prevThreshold = RTC_stat->UTIME;
    obj->nextThreshold = obj->prevThreshold + obj->period;
    RTC_regs->CHANNEL[obj->id].UTIMEVAL = obj->nextThreshold;

    Hwi_restore(key);
}

/*
 *  ======== Timer_stop ========
 */
Void Timer_stop(Timer_Object *obj)
{
    RTC_regs->CHANNEL[obj->id].CTL = 0;
    RTC_regs->CHANNEL[obj->id].INT_EVENT.IMASK = 0;
}

/*
 *  ======== Timer_setPeriod ========
 *
 * 1. Stop timer
 * 2. Set period value in timer obj
 *
 */
Void Timer_setPeriod(Timer_Object *obj, UInt32 period)
{
    obj->period = period;
}

/*
 *  ======== Timer_setPeriodMicroSecs ========
 */
Bool Timer_setPeriodMicroSecs(Timer_Object *obj, UInt32 period)
{
    obj->period = period;
    return (TRUE);
}

/*
 *  ======== Timer_trigger ========
 */
Void Timer_trigger(Timer_Object *obj, UInt32 insts)
{
}

/*
 *  ======== Timer_getPeriod ========
 */
UInt32 Timer_getPeriod(Timer_Object *obj)
{
    return(obj->period);
}

/*
 *  ======== Timer_getCount ========
 */
UInt32 Timer_getCount(Timer_Object *obj)
{
    return (RTC_stat->UTIME);
}

/*
 *  ======== Timer_dynamicStub ========
 */
Void Timer_dynamicStub(UArg arg)
{
}

/*
 *  ======== Timer_oneShotStub ========
 */
Void Timer_oneShotStub(UArg arg)
{
    Timer_Object *obj;

    obj = Timer_module->handles[(UInt)arg];
    Timer_stop(obj);
    obj->tickFxn(obj->arg);
}

/*
 *  ======== Timer_periodicStub ========
 */
Void Timer_periodicStub(UArg arg)
{
    Timer_Object *obj;

    obj = Timer_module->handles[(UInt)arg];

    /* compute and save thresholds, set compare value to start channel */
    obj->prevThreshold = obj->nextThreshold;
    obj->nextThreshold = obj->prevThreshold + obj->period;
    RTC_regs->CHANNEL[obj->id].UTIMEVAL = obj->nextThreshold;

    /* call the tick function */
    obj->tickFxn(obj->arg);
}

/*
 *  ======== Timer_getExpiredCounts ========
 *  Returns expired counts (32-bits) since the last serviced timer interrupt.
 */
UInt32 Timer_getExpiredCounts(Timer_Object *obj)
{
    return (0);
}

/*
 *  ======== Timer_getExpiredCounts64 ========
 *  Returns expired counts (64-bits) since the last serviced timer interrupt.
 */
UInt64 Timer_getExpiredCounts64(Timer_Object *obj)
{
    return (0);
}

/*
 *  ======== Timer_getCurrentTick ========
 *  Used by the Clock module for TickMode_DYNAMIC to query the corresponding
 *  Clock tick, as derived from the current timer count.
 */
UInt32 Timer_getCurrentTick(Timer_Object *obj, Bool saveFlag)
{
    return (0);
}

/*
 *  ======== Timer_getExpiredTicks ========
 *  Used by the Clock module for TickMode_DYNAMIC to determine the number of
 *  virtual ticks that have occurred since the last serviced timer interrupt.
 */
UInt32 Timer_getExpiredTicks(Timer_Object *obj, UInt32 tickPeriod)
{
    return (0);
}

/*
 *  ======== Timer_getFreq ========
 */
Void Timer_getFreq(Timer_Object *obj, Types_FreqHz *freq)
{
    freq->lo = obj->extFreq.lo;
    freq->hi = obj->extFreq.hi;
}

/*
 *  ======== Timer_getFunc ========
 */
Timer_FuncPtr Timer_getFunc(Timer_Object *obj, UArg *arg)
{
    *arg = obj->arg;
    return (obj->tickFxn);
}

/*
 *  ======== Timer_setFunc ========
 */
Void Timer_setFunc(Timer_Object *obj, Timer_FuncPtr fxn, UArg arg)
{
    obj->tickFxn = fxn;
    obj->arg = arg;
}

/* ======== Timer_initDevice ========
 * Disable and reset the RTC.
 */
Void Timer_initDevice(Timer_Object *obj)
{
}

/*
 *  ======== Timer_postInit ========
 */
Int Timer_postInit (Timer_Object *obj, Error_Block *eb)
{
    UInt key;

    key = Hwi_disable();

    Timer_initDevice(obj);

    Timer_setPeriod(obj, obj->period);

    Hwi_restore(key);

    return (0);
}
