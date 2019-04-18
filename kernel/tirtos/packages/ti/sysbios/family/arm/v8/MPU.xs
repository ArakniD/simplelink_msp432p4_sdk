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
 *  ======== MPU.xs ========
 */

var device = null;
var MPU = null;
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

    MPU = this;

    /* set fxntab default */
    MPU.common$.fxntab = false;

    device = deviceSupportCheck();

    if (device != null) {
        MPU.numRegions = deviceTable[device].numRegions;
        MPU.regionEntry.length = MPU.numRegions;
        for (var i = 0; i < MPU.numRegions; i++) {
            MPU.regionEntry[i].rbar = 0;
            MPU.regionEntry[i].rlar = 0;
        }
    }
    else {
        MPU.numRegions = 0;
        MPU.regionEntry.length = 0;
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
    mod.regionEntry.length = MPU.numRegions;

    for (var i = 0; i < MPU.numRegions; i++) {
        mod.regionEntry[i].rbar = MPU.regionEntry[i].rbar;
        mod.regionEntry[i].rlar = MPU.regionEntry[i].rlar;
    }

    Startup.firstFxns.$add(MPU.startup);
}

/*
 *  ======== initRegionAttrsMeta ========
 */
function initRegionAttrsMeta(attrs)
{
    /* Set all fields to their default value */
    attrs.enable = MPU.defaultAttrs.enable;
    attrs.shareable = MPU.defaultAttrs.shareable;
    attrs.executable = MPU.defaultAttrs.executable;
    attrs.accessPerm = MPU.defaultAttrs.accessPerm;
    attrs.attrIndx = MPU.defaultAttrs.attrIndx;
}

/*
 *  ======== setiRegionMeta ========
 */
function setRegionMeta(regionId, regionBaseAddr, regionEndAddr, attrs)
{
    var index;

    if (MPU.numRegions == 0) {
        MPU.$logError("MPU_setRegionMeta: Number of MPU regions " +
              "(MPU.numRegions) is set to 0. The number of regions must be " +
              "non-zero.", MPU);
    }

    /* Set enable field to default if undefined */
    if (attrs.enable === undefined) {
        attrs.enable = MPU.defaultAttrs.enable;
    }

    /* Set shareable field to default if undefined */
    if (attrs.shareable === undefined) {
        attrs.shareable = MPU.defaultAttrs.shareable;
    }

    /* Set executable field to default if undefined */
    if (attrs.executable === undefined) {
        attrs.executable = MPU.defaultAttrs.executable;
    }

    /* Set accessPerm field to default if undefined */
    if (attrs.accessPerm === undefined) {
        attrs.accessPerm = MPU.defaultAttrs.accessPerm;
    }

    /* Set attrIndx field to default if undefined */
    if (attrs.attrIndx === undefined) {
        attrs.attrIndx = MPU.defaultAttrs.attrIndx;
    }

    if ((regionId >= 0) && (regionId < MPU.numRegions)) {
        var alignMask = 0x1F;

        if (convertToUInt32(regionBaseAddr & alignMask) != 0) {
            MPU.$logWarning("Region base address 0x" +
                regionBaseAddr.toString(16) + " not aligned.",
                MPU);
        }

        if (convertToUInt32(regionEndAddr & alignMask) != 0) {
            MPU.$logWarning("Region end address 0x" +
                regionEndAddr.toString(16) + " not aligned.",
                MPU);
        }

        MPU.regionEntry[regionId].rbar = convertToUInt32(regionBaseAddr) |
            convertToUInt32((attrs.shareable << 3) | (attrs.accessPerm << 1) |
            !(attrs.executable));

        MPU.regionEntry[regionId].rlar = convertToUInt32(regionEndAddr) |
            convertToUInt32((attrs.attrIndx << 1) | attrs.enable);
    }
    else {
        MPU.$logError("MPU Region number (" + regionId + ") passed is " +
            "invalid.", MPU);
    }
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
 *  ======== viewPopulateRegionAttrs ========
 */
function viewPopulateRegionAttrs(regionIdx, elem, rbar, rlar)
{
    elem.RegionIdx = regionIdx;
    elem.Enabled = rlar & 0x1;

    if (elem.Enabled) {
        elem.BaseAddress = "0x" + convertToUInt32(rbar & (~0x1F)).toString(16);
        elem.EndAddress = "0x" + convertToUInt32(rlar & (~0x1F)).toString(16);

        var shareable = ((rbar >>> 2) & 0x3);
        switch (shareable) {
            case 0:
                    elem.Shareable = "Non-shareable";
                    break;
            case 2:
                    elem.Shareable = "Outer shareable";
                    break;
            case 3:
                    elem.Shareable = "Inner shareable";
                    break;
            default:
                    elem.Shareable = "Unknown";
        }

        elem.Executable = !(rbar & 0x1);

        /* Determine MPU region access permissions */
        var accessPerm = (rbar >>> 1) & 0x3;
        switch (accessPerm) {
            case 0:
                    elem.AccessPerm = "Read/Write by privileged code only";
                    break;
            case 1:
                    elem.AccessPerm = "Read/Write by any privilege level";
                    break;
            case 2:
                    elem.AccessPerm = "Read only by privileged code only";
                    break;
            case 3:
                    elem.AccessPerm = "Read only by any privilege level";
                    break;
        }
    }
    else {
        elem.BaseAddress = "";
        elem.EndAddress = "";
        elem.Shareable = "";;
        elem.Executable = undefined;
        elem.AccessPerm = "";
    }
}

/*
 *  ======== viewMpuRegionAttrs ========
 */
function viewMpuRegionAttrs(view)
{
    var mpuRegionEntry;
    var Program = xdc.useModule('xdc.rov.Program');
    var MPU = xdc.useModule('ti.sysbios.family.arm.v8.MPU');
    var MPUCfg = Program.getModuleConfig(MPU.$name);

    /* Scan the raw view in order to obtain the module state. */
    var rawView;
    try {
        rawView = Program.scanRawView('ti.sysbios.family.arm.v8.MPU');
    }
    catch (e) {
        this.$logWarning("Caught exception while retrieving raw view: " + e,
                this);
    }

    /* Get the module state */
    var mod = rawView.modState;

    /* Retrieve the MPU entry table */
    try {
        mpuRegionEntry = Program.fetchArray(
            {
                type: 'xdc.rov.support.ScalarStructs.S_UInt32',
                isScalar: true
            },
            mod.regionEntry,
            (MPUCfg.numRegions * 2),
            false);
    }
    catch (e) {
        this.$logWarning(
            "Caught exception while trying to retrieve descriptor table: " +
            e, this);
    }

    for (var i = 0; i < MPUCfg.numRegions; i++) {
        var elem = Program.newViewStruct('ti.sysbios.family.arm.v8.MPU',
                'MpuRegionAttrsView');

        viewPopulateRegionAttrs(i, elem, mpuRegionEntry[i*2],
            mpuRegionEntry[i*2 + 1]);

        /* Add the element to the list. */
        view.elements.$add(elem);
    }
}
