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
 * */

/* XDC Module Headers */
#include <xdc/std.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Diags.h>

/* BIOS Module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/hal/Timer.h>

/* Board Header file */
#include <ti/drivers/Board.h>

#define LOAD_STEPS    20
#define PERIOD        100                /* 100 ms (0.1 seconds) */
#define NUMPERSEC     (1000 / PERIOD)
#define TASKSTACKSIZE 512

/*
 * Thread Load Tables
 */

UInt hwiLoadPercent[LOAD_STEPS] =  {0, 25, 50, 75, 95,
                                    0,  0,  0,  0,  0,
                                    0,  0,  0,  0,  0,
                                    0,  5, 10, 15, 20};

UInt swiLoadPercent[LOAD_STEPS] =  {0,  0,  0,  0,  0,
                                    0, 25, 50, 75, 95,
                                    0,  0,  0,  0,  0,
                                    0, 10, 15, 20, 25};

UInt taskLoadPercent[LOAD_STEPS] = {0,  0,  0,  0,  0,
                                    0,  0,  0,  0,  0,
                                    0, 25, 50, 75, 95,
                                    0, 15, 20, 25, 30};

ULong hwiLoadValue[LOAD_STEPS];
ULong swiLoadValue[LOAD_STEPS];
ULong taskLoadValue[LOAD_STEPS];

ULong hwiLoadVal = 0;
ULong swiLoadVal = 0;
ULong taskLoadVal = 0;

Int loadIndex = 0;

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];
Swi_Struct swi0Struct;
Swi_Handle swi0Handle;
Semaphore_Struct sem0Struct;
Semaphore_Handle sem0Handle;
Timer_Handle timer0;
Timer_Struct timer0Struct;

Void cpuLoadInit(Void);
Void doLoad(ULong count);
Void hwiLoad(Void);
Void step(Void);
Void swiLoad(UArg arg1, UArg arg2);
Void taskLoad(UArg arg1, UArg arg2);
Void timerFunc(UArg arg);

/*
 *  ======== main ========
 */
Int main(Void)
{
    /* Call driver init functions */
    Board_init();

    /* Construct BIOS objects */
    Task_Params taskParams;
    Swi_Params swiParams;
    Semaphore_Params semParams;
    Timer_Params timerParams;

    /* Construct BIOS objects */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)taskLoad, &taskParams, NULL);

    Semaphore_Params_init(&semParams);
    Semaphore_construct(&sem0Struct, 0, &semParams);

    /* Obtain instance handle */
    sem0Handle = Semaphore_handle(&sem0Struct);

    Swi_Params_init(&swiParams);
    Swi_construct(&swi0Struct, (Swi_FuncPtr)swiLoad, &swiParams, NULL);
    swi0Handle = Swi_handle(&swi0Struct);

    /*
     * Configure a Timer to interrupt every 100ms
     * timerFunc() provides Hwi load and posts a Swi and Semaphore
     * to provide Swi and Task loads and adjusts the loads every 5 seconds.
     */
    Timer_Params_init(&timerParams);
    timerParams.startMode = Timer_StartMode_AUTO;
    timerParams.period = 100000;        /* 100,000 uSecs = 100ms */
    Timer_construct(&timer0Struct, Timer_ANY, (Timer_FuncPtr)timerFunc, &timerParams, NULL);

    Log_print0(Diags_USER1, "stairstep example started.");

    /* Set up the Hwi, Swi, and Task thread load tables */
    cpuLoadInit();

    BIOS_start();

    /* Fall into BIOS idle loop */
    return (0);
}

/*
 *  ======== cpuloadInit ========
 */
Void cpuLoadInit(Void)
{
    Types_FreqHz freq;
    ULong maxLoad;
    Int i;

    /* freq is maximum timestamp counts per second (100% cpuload) */
    Timestamp_getFreq(&freq);
    maxLoad = freq.lo / NUMPERSEC;  /* since we run load NUMPERSEC times */

    /*
     * calculate loadValues for each thread type for
     * each load interval (5 seconds)
     */
    for (i = 0; i < LOAD_STEPS; i++) {
        hwiLoadValue[i] = hwiLoadPercent[i] * (maxLoad / 100);
        swiLoadValue[i] = swiLoadPercent[i] * (maxLoad / 100);
        taskLoadValue[i] = taskLoadPercent[i] * (maxLoad / 100);
    }
}

/*
 *  ======== doLoad ========
 *  stall in a loop until timestamp equals initial
 *  value plus count.
 */
Void doLoad(ULong count)
{
    ULong now, end;

    /* compute load loop endpoint */
    end = Timestamp_get32() + count;

    /* loop until we reach termination timestamp */
    do {
        now = Timestamp_get32();
    } while ((end - now) < count);
}

/*
 *  ======== hwiLoad ========
 */
Void hwiLoad(Void)
{
    static ULong oldLoad = ~0;

    /* display confirmation of load changes */
    if (oldLoad != hwiLoadVal ) {
        oldLoad = hwiLoadVal;
        Log_print1(Diags_USER1, "Hwi load: new load = %d%%",
                hwiLoadPercent[loadIndex]);
    }

    if (hwiLoadVal) {
        doLoad(hwiLoadVal);
    }
}

/*
 *  ======== step ========
 *  Update the load values for all thread types
 */
Void step(Void)
{
    loadIndex++;

    if (loadIndex == LOAD_STEPS) {
        loadIndex = 0;
    }

    hwiLoadVal = hwiLoadValue[loadIndex];
    swiLoadVal = swiLoadValue[loadIndex];
    taskLoadVal = taskLoadValue[loadIndex];
}

/*
 *  ======== swiLoad ========
 */
Void swiLoad(UArg arg1, UArg arg2)
{
    static ULong oldLoad = ~0;

    /* display confirmation of load changes */
    if (oldLoad != swiLoadVal) {
        oldLoad = swiLoadVal;
        Log_print1(Diags_USER1, "Swi load: new load = %d%%",
            swiLoadPercent[loadIndex]);
    }

    if (swiLoadVal) {
        doLoad(swiLoadVal);
    }
}

/*
 *  ======== taskLoad ========
 */
Void taskLoad(UArg arg1, UArg arg2)
{
    static ULong oldLoad = ~0;

    while (TRUE) {
        Semaphore_pend(sem0Handle, BIOS_WAIT_FOREVER);

        /* display confirmation of load changes */
        if (oldLoad != taskLoadVal) {
            oldLoad = taskLoadVal;
            Log_print1(Diags_USER1, "Task load: new load = %d%%",
                taskLoadPercent[loadIndex]);
        }

        if (taskLoadVal) {
            doLoad(taskLoadVal);
        }
    }
}

/*
 *  ======== timerFunc ========
 *  This function runs every PERIOD ms in the context of a Hwi thread.
 */
Void timerFunc(UArg arg)
{
    static UInt tickCount = 0;

    /* change load values every 5 seconds */
    if (++tickCount >= (5 * NUMPERSEC)) {
        tickCount = 0;
        step();
    }

    /* Make Swi load thread ready to run */
    Swi_post(swi0Handle);

    /* Make Task load thread ready to run */
    Semaphore_post(sem0Handle);

    /* Do Hwi thread load now */
    hwiLoad();
}
