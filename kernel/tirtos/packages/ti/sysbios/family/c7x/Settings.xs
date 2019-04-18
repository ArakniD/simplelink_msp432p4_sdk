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
 *  ======== Settings.xs ========
 *
 */

var settings = {
    device: {
        "C71": {
            hwiDelegate : "ti.sysbios.family.c7x.Hwi",
            timerDelegate : "ti.sysbios.timers.dmtimer.Timer",
            clockTimerDelegate : "ti.sysbios.timers.dmtimer.Timer",
            timerSupportDelegate : "ti.sysbios.family.c64p.tci6488.TimerSupport",
            timestampDelegate : "ti.sysbios.family.c7x.TimestampProvider",
            taskSupportDelegate : "ti.sysbios.family.c7x.TaskSupport",
            intrinsicsSupportDelegate : "ti.sysbios.family.c7x.IntrinsicsSupport",
            cacheDelegate : "ti.sysbios.family.c7x.Cache",
            clockTickPeriod : 1000,
            targets : [ "ti.targets.elf.nda.C71" ]
        },
        "J7ES": {
            hwiDelegate : "ti.sysbios.family.c7x.Hwi",
            timerDelegate : "ti.sysbios.timers.dmtimer.Timer",
            clockTimerDelegate : "ti.sysbios.timers.dmtimer.Timer",
            timerSupportDelegate : "ti.sysbios.family.c64p.tci6488.TimerSupport",
            timestampDelegate : "ti.sysbios.family.c7x.TimestampProvider",
            taskSupportDelegate : "ti.sysbios.family.c7x.TaskSupport",
            intrinsicsSupportDelegate : "ti.sysbios.family.c7x.IntrinsicsSupport",
            cacheDelegate : "ti.sysbios.family.c7x.Cache",
            clockTickPeriod : 1000,
            targets : [ "ti.targets.elf.nda.C71" ]
        },
        "J721E": {
            hwiDelegate : "ti.sysbios.family.c7x.Hwi",
            timerDelegate : "ti.sysbios.timers.dmtimer.Timer",
            clockTimerDelegate : "ti.sysbios.timers.dmtimer.Timer",
            timerSupportDelegate : "ti.sysbios.family.c64p.tci6488.TimerSupport",
            timestampDelegate : "ti.sysbios.family.c7x.TimestampProvider",
            taskSupportDelegate : "ti.sysbios.family.c7x.TaskSupport",
            intrinsicsSupportDelegate : "ti.sysbios.family.c7x.IntrinsicsSupport",
            cacheDelegate : "ti.sysbios.family.c7x.Cache",
            clockTickPeriod : 1000,
            targets : [ "ti.targets.elf.nda.C71" ]
        },
/*
        "TMS320DA830": {
            hwiDelegate : "ti.sysbios.family.c64p.Hwi",
            timerDelegate : "ti.sysbios.timers.timer64.Timer",
            clockTimerDelegate : "ti.sysbios.timers.timer64.Timer",
            timerSupportDelegate : "ti.sysbios.family.c64p.primus.TimerSupport",
            timestampDelegate : "ti.sysbios.family.c64p.TimestampProvider",
            taskSupportDelegate : "ti.sysbios.family.c62.TaskSupport",
            intrinsicsSupportDelegate : "ti.sysbios.family.c62.IntrinsicsSupport",
            cacheDelegate : "ti.sysbios.family.c64p.Cache",
            clockTickPeriod : 1000,
            targets : [ "ti.targets.C674", "ti.targets.elf.C674" ]
        },
        "TMS320C3430": {
            hwiDelegate : "ti.sysbios.family.c64p.Hwi",
            timerDelegate : "ti.sysbios.timers.gptimer.Timer",
            clockTimerDelegate : "ti.sysbios.timers.gptimer.Timer",
            timerSupportDelegate : "ti.sysbios.family.c64p.omap3430.TimerSupport",
            timestampDelegate : "ti.sysbios.family.c64p.TimestampProvider",
            taskSupportDelegate : "ti.sysbios.family.c62.TaskSupport",
            intrinsicsSupportDelegate : "ti.sysbios.family.c62.IntrinsicsSupport",
            cacheDelegate : "ti.sysbios.family.c64p.Cache",
            clockTickPeriod : 1000,
            targets : [ "ti.targets.C64P", "ti.targets.elf.C64P" ]
        },
        "TMS320TI816X": {
            hwiDelegate : "ti.sysbios.family.c64p.Hwi",
            timerDelegate : "ti.sysbios.timers.dmtimer.Timer",
            clockTimerDelegate : "ti.sysbios.timers.dmtimer.Timer",
            timerSupportDelegate : "ti.sysbios.family.c64p.ti81xx.TimerSupport",
            timestampDelegate : "ti.sysbios.family.c64p.TimestampProvider",
            taskSupportDelegate : "ti.sysbios.family.c62.TaskSupport",
            intrinsicsSupportDelegate : "ti.sysbios.family.c62.IntrinsicsSupport",
            cacheDelegate : "ti.sysbios.family.c64p.Cache",
            clockTickPeriod : 1000,
            targets : [ "ti.targets.C674", "ti.targets.elf.C674" ]
        },
        "DRA7XX": {
            hwiDelegate : "ti.sysbios.family.c64p.Hwi",
            timerDelegate : "ti.sysbios.timers.dmtimer.Timer",
            clockTimerDelegate : "ti.sysbios.timers.dmtimer.Timer",
            timerSupportDelegate: "ti.sysbios.family.shared.vayu.TimerSupport",
            timestampDelegate : "ti.sysbios.family.c64p.TimestampProvider",
            taskSupportDelegate : "ti.sysbios.family.c62.TaskSupport",
            intrinsicsSupportDelegate : "ti.sysbios.family.c62.IntrinsicsSupport",
            cacheDelegate : "ti.sysbios.family.c66.Cache",
            clockTickPeriod : 1000,
            targets : [ "ti.targets.elf.C66" ]
        },
*/
    }
}

/*
 *  ======== deviceSupportCheck ========
 *  Check validity of device
 */
function deviceSupportCheck()
{
    for (device in settings.device) {
        if (device == Program.cpu.deviceName) {
            return;
        }
    }
    print("The " + Program.cpu.deviceName + " device is not currently supported.");
    print("The following devices are supported for the " + Program.build.target.name + " target:");
    for (device in settings.device) {
        print("\t" + device);
    }
    throw new Error ("Unsupported device!");
}

/*
 *  ======== getDefaultCoreDelegate ========
 */
function getDefaultCoreDelegate()
{
    return (null);
}

/*
 *  ======== getDefaultHwiDelegate ========
 */
function getDefaultHwiDelegate()
{
    deviceSupportCheck();
    return (settings.device[Program.cpu.deviceName].hwiDelegate);
}

/*
 *  ======== getDefaultTimerDelegate ========
 */
function getDefaultTimerDelegate()
{
    deviceSupportCheck();
    return(settings.device[Program.cpu.deviceName].timerDelegate);
}

/*
 *  ======== getDefaultClockTimerDelegate ========
 */
function getDefaultClockTimerDelegate()
{
    deviceSupportCheck();
    return(settings.device[Program.cpu.deviceName].clockTimerDelegate);
}

/*
 *  ======== getDefaultTimerSupportDelegate ========
 */
function getDefaultTimerSupportDelegate()
{
    deviceSupportCheck();
    return(settings.device[Program.cpu.deviceName].timerSupportDelegate);
}

/*
 *  ======== getDefaultTimestampDelegate ========
 */
function getDefaultTimestampDelegate()
{
    deviceSupportCheck();
    return(settings.device[Program.cpu.deviceName].timestampDelegate);
}

/*
 *  ======== getDefaultTaskSupportDelegate ========
 */
function getDefaultTaskSupportDelegate()
{
    deviceSupportCheck();
    return(settings.device[Program.cpu.deviceName].taskSupportDelegate);
}

/*
 *  ======== getDefaultIntrinsicsSupportDelegate ========
 */
function getDefaultIntrinsicsSupportDelegate()
{
    deviceSupportCheck();
    return(settings.device[Program.cpu.deviceName].intrinsicsSupportDelegate);
}

/*
 *  ======== getDefaultCacheDelegate ========
 */
function getDefaultCacheDelegate()
{
    deviceSupportCheck();
    if  (settings.device[Program.cpu.deviceName].cacheDelegate == null) {
        return ("ti.sysbios.hal.CacheNull");
    }
    else {
        return (settings.device[Program.cpu.deviceName].cacheDelegate);
    }
}

/*
 *  ======== getDefaultSecondsDelegate ========
 */
function getDefaultSecondsDelegate()
{
    return (null);
}

/*
 *  ======== getDefaultSysCallDelegate ========
 */
function getDefaultSysCallDelegate()
{
    return (null);
}

/*
 *  ======== getDefaultPowerDelegate ========
 */
function getDefaultPowerDelegate()
{
    return ("ti.sysbios.hal.PowerNull");
}

/*
 *  ======== getDefaultClockTickPeriod ========
 */
function getDefaultClockTickPeriod()
{
    deviceSupportCheck();
    return(settings.device[Program.cpu.deviceName].clockTickPeriod);
}

/*
 *  ======== getDefaultBootModule ========
 */
function getDefaultBootModule()
{
    return (null);
}

/*
 *  ======== getDefaultMmuModule ========
 */
function getDefaultMmuModule()
{
    return (null);
}

/*
 *  ======== getFamilySettingsXml ========
 */
function getFamilySettingsXml()
{
    return("family/c7x/Settings.xml");
}
