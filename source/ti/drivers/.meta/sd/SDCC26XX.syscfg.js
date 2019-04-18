/*
 * Copyright (c) 2018-2019 Texas Instruments Incorporated - http://www.ti.com
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
 *
 */

/*
 *  ======== SDCC26XX.syscfg.js ========
 */

"use strict";

let Common = system.getScript("/ti/drivers/Common.js");

let longDescription =`
The [__SD driver__][1] provides a simple interface to perform basic
operations on SD cards.
* [Usage Synopsis][2]
* [Examples][3]
* Configuration - There are no SD configuration parameters for this device
[1]: /tidrivers/doxygen/html/_s_d_8h.html#details "C API reference"
[2]:
/tidrivers/doxygen/html/_s_d_8h.html#ti_drivers_SD_Synopsis "Synopsis"
[3]: /tidrivers/doxygen/html/_s_d_8h.html#ti_drivers_SD_Examples
"C usage examples"
`;

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base SD configuration
 */
let devSpecific = {

    sharedModuleInstances: sharedModuleInstances,

    moduleInstances: moduleInstances,

    filterHardware: filterHardware,

    templates: {
        boardc: "/ti/drivers/sd/SDCC26XX.Board.c.xdt",
        boardh: "/ti/drivers/sd/SD.Board.h.xdt"
    }
};

/*
 *  ======== sharedModuleInstances ========
 */
function sharedModuleInstances(inst)
{
    return ([{
        name: "spiInstance",
        displayName: "SPI",
        moduleName: "/ti/drivers/SPI",
        hardware: inst.$hardware ? inst.$hardware.subComponents.spi : null
    }]);
}

/*
 *  ======== moduleInstances ========
 */
function moduleInstances(inst)
{
    let hw = null;

    if (inst.$hardware &&
        inst.$hardware.subComponents &&
        inst.$hardware.subComponents.select) {
        hw = inst.$hardware.subComponents.select;
    }

    return ([{
        name: "slaveSelect",
        displayName: "Slave Select",
        moduleName: "/ti/drivers/GPIO",
        hardware: hw,
        args: {
            comment: hw ? "" : "%l    /* SD SPI Chip Select */",
            mode: "Output",
            outputType: "Standard",
            initialOutputState:"High"
        }
    }]);
}

/*
 *  ========= filterHardware ========
 *  param component - hardware object describing signals and
 *                    resources
 *
 *  returns Boolean indicating whether or not to allow the component to
 *           be assigned to an instance's $hardware config
 */
function filterHardware(component) {
    if (component.type) {
        if (Common.typeMatches(component.type, ["SD_SPI_FLASH"])) {
            return (true);
        }
    }

    return (false);
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic SD module to
 *  allow us to augment/override as needed for the CC26XX.
 */
function extend(base)
{
    base.config = [];
    base.longDescription = longDescription;

    return (Object.assign({}, base, devSpecific));
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base SD module */
    extend: extend
};
