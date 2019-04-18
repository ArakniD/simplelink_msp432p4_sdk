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
 *  ======== Timer.xs ========
 *
 */

var Timer = null;
var BIOS = null;
var Hwi = null;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }
    Timer = this;

    /*
     * set fxntab to false because ti.sysbios.hal.Timer can be used
     * instead of an abstract instance
     */
    Timer.common$.fxntab = false;

    /* initialize timer fields */
    Timer.anyMask = (1 << 8) - 1;
    Timer.numTimerDevices = 8;
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Hwi = xdc.useModule("ti.sysbios.family.arm.v8m.Hwi");
    BIOS = xdc.useModule('ti.sysbios.BIOS');
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    mod.availMask = (1 << 8) - 1;
    mod.device.length = 8;
    mod.handles.length = 8;

    if (params.anyMask > mod.availMask) {
        Timer.$logError("Incorrect anyMask (" + params.anyMask
            + "). Should be <= " + mod.availMask + ".", Timer);
    }

    mod.device[0].intNum = 24;     /* Channel 0 */
    mod.handles[0] = null;

    mod.device[1].intNum = 25;     /* Channel 1 */
    mod.handles[1] = null;

    mod.device[2].intNum = 26;     /* Channel 2 */
    mod.handles[2] = null;

    mod.device[3].intNum = 27;     /* Channel 3 */
    mod.handles[3] = null;

    mod.device[4].intNum = 28;     /* Channel 4 */
    mod.handles[4] = null;

    mod.device[5].intNum = 29;     /* Channel 5 */
    mod.handles[5] = null;

    mod.device[6].intNum = 30;     /* Channel 6 */
    mod.handles[6] = null;

    mod.device[7].intNum = 31;     /* Channel 7 */
    mod.handles[7] = null;

    /*
     * plug Timer.startup into BIOS.startupFxns array
     */
    var BIOS = xdc.module ('ti.sysbios.BIOS');
    BIOS.addUserStartupFunction(Timer.startup);
}

/*
 *  ======== instance$static$init ========
 */
function instance$static$init(obj, id, tickFxn, params)
{
    var modObj = this.$module.$object;

    /* set flag because static instances need to be started */
    Timer.startupNeeded = true;
    obj.staticInst = true;

    if (id >= Timer.numTimerDevices) {
        if (id != Timer.ANY) {
            Timer.$logFatal("Invalid Timer ID: " + id
                            + "; the id must be -1, 0, ..., "
                            + (Timer.numTimerDevices - 1), this);
        }
    }

    if (id == Timer.ANY) {
        var mask = Timer.anyMask & modObj.availMask;
        for (var i = 0; i < Timer.numTimerDevices; i++) {
            if (mask & (1 << i)) {
                modObj.availMask &= ~(1 << i);
                obj.id = i;
                break;
            }
        }
    }
    else if (modObj.availMask & (1 << id)) {
        modObj.availMask &= ~(1 << id);
        obj.id = id;
    }

    if (obj.id == undefined) {
        var alist = [];
        var mask = Timer.anyMask & modObj.availMask;
        for (var i = 0; i < Timer.numTimerDevices; i++) {
            if (mask & (1 << i)) {
                alist.push(i);
            }
        }
        var avail = alist.length == 0
            ? "no timers are available"
            : ("the current set of  available timer ids includes: "
               + alist.join(", ") + ", or Timer.ANY (-1)");
        Timer.$logFatal("Timer device id " + id + " is unavailable; "
                        + avail, this);
    }

    obj.runMode = params.runMode;
    obj.startMode = params.startMode;
    obj.intNum = modObj.device[obj.id].intNum;
    obj.arg = params.arg;
    obj.tickFxn = tickFxn;
    obj.extFreq.lo = 1000000; /* rate of UTIME view */
    obj.extFreq.hi = 0;

    if (params.periodType == Timer.PeriodType_MICROSECS) {
        if (setPeriodMicroSecs(obj, params.period) == false) {
            Timer.$logError("Timer doesn't support configured period.",
                                this, params.period);
        }
    }
    else {
        obj.period = params.period;
    }
    obj.periodType = Timer.PeriodType_COUNTS;

    obj.prevThreshold = 0;
    obj.savedCurrCount = 0;
    obj.nextThreshold = 0;

    if (obj.tickFxn) {
        if (!params.hwiParams) {
            params.hwiParams = new Hwi.Params;
        }
        var hwiParams = params.hwiParams;

        if (Hwi.inUseMeta(obj.intNum) == true) {
            Timer.$logError("Timer interrupt " + obj.intNum +
                                " already in use!", obj);
            return;
        }

        hwiParams.arg = obj.id;

        if (obj.runMode == Timer.RunMode_CONTINUOUS) {
            obj.hwi = Hwi.create(obj.intNum, Timer.periodicStub, hwiParams);
        }
        else {
            obj.hwi = Hwi.create(obj.intNum, Timer.oneShotStub, hwiParams);
        }
    }
    else {
        obj.hwi = null;
    }

    modObj.handles[obj.id] = this;
}

/*
 *  ======== setPeriodMicroSecs ========
 */
function setPeriodMicroSecs(obj, period)
{
    obj.period = period;

    if (obj.period > 0xffffffff) {
        return false;
    }
    else {
        return true;
    }
}

/*
 *  ======== getEnumString ========
 *  Get the enum value string name, not 0, 1, 2 or 3, etc.  For an enumeration
 *  type property.
 *
 *  Example usage:
 *  if obj contains an enumeration type property "Enum enumProp"
 *
 *  view.enumString = getEnumString(obj.enumProp);
 *
 */
function getEnumString(enumProperty)
{
    /*
     *  Split the string into tokens in order to get rid of the huge package
     *  path that precedes the enum string name. Return the last 2 tokens
     *  concatenated with "_"
     */
    var enumStrArray = String(enumProperty).split(".");
    var len = enumStrArray.length;
    return (enumStrArray[len - 1]);
}

/*
 *  ======== viewInitBasic ========
 *  Initialize the 'Basic' Timer instance view.
 */
function viewInitBasic(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var halTimer = xdc.useModule('ti.sysbios.hal.Timer');

    view.halTimerHandle =  halTimer.viewGetHandle(obj.$addr);
    view.label      = Program.getShortName(obj.$label);
    view.id         = obj.id;
    view.runMode    = getEnumString(obj.runMode);
    view.startMode  = getEnumString(obj.startMode);
    view.periodType = getEnumString(obj.periodType);
    view.period     = obj.period;
    view.intNum     = obj.intNum;
    view.tickFxn    = Program.lookupFuncName(Number(obj.tickFxn));
    view.arg        = obj.arg;
    view.hwiHandle  = "0x" + Number(obj.hwi).toString(16);
}

/*
 *  ======== viewInitDevice ========
 */
function viewInitDevice(view, obj)
{
    return;
}

/*
 *  ======== viewInitModule ========
 *  Initialize the Timer 'Module' view.
 */
function viewInitModule(view, obj)
{
    view.availMask = Number(obj.availMask).toString(2);
}
