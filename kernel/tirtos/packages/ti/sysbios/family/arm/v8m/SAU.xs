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
 *  ======== SAU.xs ========
 */

var device = null;
var SAU = null;
var Startup = null;

if (xdc.om.$name == "cfg" || typeof(genCdoc) != "undefined") {
    var deviceTable = {
        "FVP_MPS2": {
            numRegions : 8
        }
    };

    /* Cortex-M33 devices */
    deviceTable["MTL1_VSOC"] = deviceTable["FVP_MPS2"];
}

/*
 *  ======== deviceSupportCheck ========
 *  Check validity of device
 */
function deviceSupportCheck()
{
    /* look for exact match first */
    for (device in deviceTable) {
        if (device == Program.cpu.deviceName) {
            return (device);
        }
    }

    /* now look for a wildcard match */
    for (device in deviceTable) {
        if (Program.cpu.deviceName.match(device)) {
            return (device);
        }
    }

    return (null);
}

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    SAU = this;

    /* set fxntab default */
    SAU.common$.fxntab = false;

    device = deviceSupportCheck();

    if (device != null) {
        SAU.numRegions = deviceTable[device].numRegions;
        SAU.regionEntry.length = SAU.numRegions;
        for (var i = 0; i < SAU.numRegions; i++) {
            SAU.regionEntry[i].rbar = 0;
            SAU.regionEntry[i].rlar = 0;
        }
    }
    else {
        SAU.numRegions = 0;
        SAU.regionEntry.length = 0;
    }
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Startup = xdc.useModule('xdc.runtime.Startup');

    if (device == null) {
        /*
         * no match, print all catalog devices supported
         * and then raise an error
         */
        var catalog = xdc.loadPackage(Program.cpu.catalogName);

        /* build associative array of supported devices */
        var supportedDevices = new Object();

        for (var i = 0; i < catalog.$modules.length; i++) {
            catalogName = catalog.$modules[i].$name.substring(
                Program.cpu.catalogName.length + 1);

            for (device in deviceTable) {
                if (catalogName.match(device)) {
                    supportedDevices[catalogName] = catalogName;
                }
            }
        }

        /* copy it into a sortable array */
        var sd = new Array();

        for (var i in supportedDevices) {
            sd[sd.length++] = i;
        }

        /* sort it for a nicer report */
        sd.sort();

        print("The " + Program.cpu.deviceName +
              " device is not currently supported.");
        print("The following devices are supported for the " +
            Program.build.target.name + " target:");

        for (var i=0; i<sd.length; i++) {
            print("\t" + sd[i]);
        }

        throw new Error ("Unsupported device!");
    }
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    mod.regionEntry.length = SAU.numRegions;

    for (var i = 0; i < SAU.numRegions; i++) {
        mod.regionEntry[i].rbar = SAU.regionEntry[i].rbar;
        mod.regionEntry[i].rlar = SAU.regionEntry[i].rlar;
    }

    Startup.firstFxns.$add(SAU.startup);
}

/*
 *  ======== confgureRegionMeta ========
 */
function configureRegionMeta(regionId, regionBase, regionSize, regionType)
{
    var index;

    if (SAU.numRegions == 0) {
        SAU.$logError("SAU_setRegionMeta: Number of SAU regions " +
              "(SAU.numRegions) is set to 0. The number of regions must be " +
              "non-zero.", SAU);
    }

    if ((regionId >= 0) && (regionId < SAU.numRegions)) {
        var alignMask = 0x1F;

        if (convertToUInt32(regionBase & alignMask) != 0) {
            SAU.$logWarning("Region base address 0x" +
                regionBase.toString(16) + " not aligned.",
                SAU);
        }

        let regionEnd = (regionBase + regionSize - 1) & 0xffffffe0;

        if (convertToUInt32(regionEnd & alignMask) != 0) {
            SAU.$logWarning("Region end address 0x" +
                regionEndr.toString(16) + " not aligned.",
                SAU);
        }

        SAU.regionEntry[regionId].rbar = convertToUInt32(regionBase);

        SAU.regionEntry[regionId].rlar = convertToUInt32(regionEnd);

        if (regionType == SAU.RegionType_NSC) {
            SAU.regionEntry[regionId].rlar |= 0x2;
        }

        /* enable the region by default */
        SAU.regionEntry[regionId].rlar |= 0x1;
    }
    else {
        SAU.$logError("SAU Region number (" + regionId + ") passed is " +
            "invalid.", SAU);
    }
}

/*
 *  ======== disableRegionMeta ========
 */
function disableRegionMeta(regionId)
{
    SAU.regionEntry[regionId].rlar &= 0xfffffffe;
}

/*
 *  ======== convertToUInt32 ========
 */
function convertToUInt32(value)
{
    /*
     *  Provide this workaround here due to javascript.
     *  Note: Javascript converts numbers to 32-bit signed numbers before
     *        evaluating bitwise operators.
     */
    if (value < 0) {
        value = value + 0xffffffff + 1;
    }

    return (value);
}

/*
 *  ======== viewSauRegions ========
 */
function viewSauRegions(view)
{
    var sauRegionEntry;
    var Program = xdc.useModule('xdc.rov.Program');
    var SAU = xdc.useModule('ti.sysbios.family.arm.v8m.SAU');
    var SAUCfg = Program.getModuleConfig(SAU.$name);

    /* Scan the raw view in order to obtain the module state. */
    var rawView;
    try {
        rawView = Program.scanRawView('ti.sysbios.family.arm.v8m.SAU');
    }
    catch (e) {
        SAU.$logWarning("Caught exception while retrieving raw view: " + e,
                this);
    }

    /* Get the module state */
    var mod = rawView.modState;

    /* Retrieve the SAU entry table from the module state */
    try {
        sauRegionEntry = Program.fetchArray(
            {
                type: 'xdc.rov.support.ScalarStructs.S_UInt32',
                isScalar: true
            },
            mod.regionEntry,
            (SAUCfg.numRegions * 2),
            false);
    }
    catch (e) {
        SAU.$logWarning(
            "Caught exception while trying to retrieve descriptor table: " +
            e, this);
    }

    for (var i = 0; i < SAUCfg.numRegions; i++) {
        var elem = Program.newViewStruct('ti.sysbios.family.arm.v8m.SAU',
                'SauRegions');

        var rbar =  sauRegionEntry[i*2];
        var rlar = sauRegionEntry[i*2 + 1];

        elem.RegionIdx = i;
        elem.Enabled = rlar & 0x1;

        if (elem.Enabled) {
            elem.BaseAddress = "0x" + convertToUInt32(rbar & (~0x1F)).toString(16);
            elem.EndAddress = "0x" + convertToUInt32((rlar & (~0x1F)) + 0x1F).toString(16);
            elem.RegionType = rlar & 0x2 ? "Secure, Non-Secure Callable" : "Non-Secure";
        }
        else {
            elem.BaseAddress = "";
            elem.EndAddress = "";
            elem.RegionType = "";;
        }

        /* Add the element to the list. */
        view.elements.$add(elem);
    }
}

/*
 *  ======== viewSauRegisters ========
 */
function viewSauRegisters(view)
{
    var Program = xdc.useModule('xdc.rov.Program');

    try {
        this.SAU = Program.fetchArray({type: 'xdc.rov.support.ScalarStructs.S_UInt', isScalar: true}, 0xe000edd0, 8, false);
    }
    catch (e) {
        var errorString = "Problem fetching SAU registers: " + e.toString();
        Program.displayError(view, "SREGION", errorString);
        Program.displayError(view, "ENABLE", errorString);
        Program.displayError(view, "ALLNS", errorString);
        Program.displayError(view, "SFSR", errorString);
        Program.displayError(view, "SFAR", errorString);
        return;
    }

    var regs = Program.newViewStruct('ti.sysbios.family.arm.v8m.SAU',
                'SauRegisters');

    regs.SREGION = this.SAU[1] & 0xff;
    regs.ENABLE = this.SAU[0] & 0x1;
    regs.ALLNS = this.SAU[0] & 0x2 ? "All Non-Secure" : "All Secure";
    regs.SFSR = "0x" + this.SAU[5].toString(16);
    regs.SFAR = "0x" + this.SAU[6].toString(16);

    /* Add the regs to the view. */
    view.elements.$add(regs);
}
