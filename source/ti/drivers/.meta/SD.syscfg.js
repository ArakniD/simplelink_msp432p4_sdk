/*
 * Copyright (c) 2018-2019, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== SD.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let family = Common.device2Family(system.deviceData, "SD");

/* generic configuration parameters for SD instances */
let config = [
    {
        name: "useFatFS",
        displayName: "Use FatFS",
        description: "Enables the driver to use the SDFatFS driver interface",
        longDescription:`When enabled, the [__SDFatFS__][1] driver interface
will be accessible by the application.

[1]: /tidrivers/doxygen/html/_s_d_fat_f_s_8h.html#details "C API reference"
`,
        default: false
    }
];

/*
 *  ======== validate ========
 *  Validate this inst's configuration
 *
 *  param inst       - SD instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    Common.validateNames(inst, validation);
}

/*
 *  ======== base ========
 *  Define the base properties and methods
 */
let base = {
    displayName: "SD",
    description: "Secure Digital (SD) Driver",
    longDescription: `
The [__SD driver__][1] provides a simple interface to perform basic
operations on SD cards.
* [Usage Synopsis][2]
* [Examples][3]
* [Configuration][4]
[1]: /tidrivers/doxygen/html/_s_d_8h.html#details "C API reference"
[2]:
/tidrivers/doxygen/html/_s_d_8h.html#ti_drivers_SD_Synopsis "Synopsis"
[3]: /tidrivers/doxygen/html/_s_d_8h.html#ti_drivers_SD_Examples
"C usage examples"
[4]: /tidrivers/syscfg/html/ConfigDoc.html#SD_Configuration_Options "Configuration options reference"
`,
    documentation: "/tidrivers/doxygen/html/_s_d_8h.html",
    defaultInstanceName: "Board_SD",
    config: Common.addNameConfig(config, "/ti/drivers/SD", "Board_SD"),
    validate: validate,
    modules: Common.autoForceModules(["Board", "Power"])
};

/* extend the base exports to include family-specific content */
let deviceSD = system.getScript("/ti/drivers/sd/SD" + family);
exports = deviceSD.extend(base);
