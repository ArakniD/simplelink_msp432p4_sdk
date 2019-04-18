/*
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== I2SCC26XX.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base I2S configuration
 */
let devSpecific = {
    maxInstances: 1,

    config: [
        {
            name        : "enableMCLK",
            displayName : "Enable MCLK",
            default     : false
        }
    ],

    templates: {
        boardc : "/ti/drivers/i2s/I2SCC26XX.Board.c.xdt",
        boardh : "/ti/drivers/i2s/I2S.Board.h.xdt"
    },

    pinmuxRequirements: pinmuxRequirements,
    moduleInstances: moduleInstances,
    modules: Common.autoForceModules(["Board", "Power"])
};

/*
 *  ======== moduleInstances ========
 *  returns PIN instances
 */
function moduleInstances(inst)
{
    let pinInstances = new Array();

    if(inst.sd0DataDirection == "Output") {
        pinInstances.push({
            name: "sd0PinInstance",
            displayName: "I2S SD0 PIN Driver Instance - Output",
            moduleName: "/ti/drivers/PIN",
            readOnly: true,
            args: {
                parentMod: "/ti/drivers/I2S",
                parentInterfaceName: "i2s",
                parentSignalName: "SD0Pin",
                parentSignalDisplayName: "SD0",
                mode: "Output",
                outputState: "Low",
                pull: "None" }
        });
    }

    if(inst.sd0DataDirection == "Input") {
        pinInstances.push({
            name: "sd0PinInstance",
            displayName: "I2S SD0 PIN Driver Instance - Input",
            moduleName: "/ti/drivers/PIN",
            readOnly: true,
            args: {
                parentMod: "/ti/drivers/I2S",
                parentInterfaceName: "i2s",
                parentSignalName: "SD0Pin",
                parentSignalDisplayName: "SD0",
                mode: "Input",
                pull: "None" }
        });
    }

    if(inst.sd1DataDirection == "Input") {
        pinInstances.push({
            name: "sd1PinInstance",
            displayName: "I2S SD1 PIN Driver Instance - Input",
            moduleName: "/ti/drivers/PIN",
            readOnly: true,
            args: {
                parentMod: "/ti/drivers/I2S",
                parentInterfaceName: "i2s",
                parentSignalName: "SD1Pin",
                parentSignalDisplayName: "SD1",
                mode: "Input",
                pull: "None" }
        });
    }

    if(inst.sd1DataDirection == "Output") {
        pinInstances.push({
            name: "sd1PinInstance",
            displayName: "I2S SD1 PIN Driver Instance - Output",
            moduleName: "/ti/drivers/PIN",
            readOnly: true,
            args: {
                parentMod: "/ti/drivers/I2S",
                parentInterfaceName: "i2s",
                parentSignalName: "SD1Pin",
                parentSignalDisplayName: "SD1",
                mode: "Output",
                outputState: "Low",
                pull: "None"  }
        });
    }

    if(inst.masterSlaveSelection == "Master") {
        pinInstances.push({
            name: "sckPinInstance",
            displayName: "I2S SCK PIN Driver Instance - Output",
            moduleName: "/ti/drivers/PIN",
            readOnly: true,
            args: {
                parentMod: "/ti/drivers/I2S",
                parentInterfaceName: "i2s",
                parentSignalName: "SCKPin",
                parentSignalDisplayName: "SCK",
                mode: "Output",
                outputState: "Low",
                pull: "None" }
        });

        pinInstances.push({
            name: "wsPinInstance",
            displayName: "I2S WS PIN Driver Instance - Output",
            moduleName: "/ti/drivers/PIN",
            readOnly: true,
            args: {
                parentMod: "/ti/drivers/I2S",
                parentInterfaceName: "i2s",
                parentSignalName: "WSPin",
                parentSignalDisplayName: "WS",
                mode: "Output",
                outputState: "Low",
                pull: "None"
            }
        });

        if(inst.enableMCLK) {
            pinInstances.push({
                name: "mclkPinInstance",
                displayName: "I2S MCLK PIN Driver Instance - Output",
                moduleName: "/ti/drivers/PIN",
                readOnly: true,
                args: {
                    parentMod: "/ti/drivers/I2S",
                    parentInterfaceName: "i2s",
                    parentSignalName: "MCLKPin",
                    parentSignalDisplayName: "MCLK",
                    mode: "Output",
                    outputState: "Low",
                    pull: "None"
                }
            });
        }
    }
    else {
        pinInstances.push({
            name: "sckPinInstance",
            displayName: "I2S SCK PIN Driver Instance - Input",
            moduleName: "/ti/drivers/PIN",
            readOnly: true,
            args: {
                parentMod: "/ti/drivers/I2S",
                parentInterfaceName: "i2s",
                parentSignalName: "SCKPin",
                parentSignalDisplayName: "SCK",
                mode: "Input",
                pull: "None"
            }
        });

        pinInstances.push({
            name: "wsPinInstance",
            displayName: "I2S WS PIN Driver Instance - Input",
            moduleName: "/ti/drivers/PIN",
            readOnly: true,
            args: {
                parentMod: "/ti/drivers/I2S",
                parentInterfaceName: "i2s",
                parentSignalName: "WSPin",
                parentSignalDisplayName: "WS",
                mode: "Input",
                pull: "None"
            }
        });

        if(inst.enableMCLK) {
            pinInstances.push({
                name: "mclkPinInstance",
                displayName: "I2S MCLK PIN Driver Instance - Input",
                moduleName: "/ti/drivers/PIN",
                readOnly: true,
                args: {
                    parentMod: "/ti/drivers/I2S",
                    parentInterfaceName: "i2s",
                    parentSignalName: "MCLKPin",
                    parentSignalDisplayName: "MCLK",
                    mode: "Input",
                    pull: "None"
                }
            });
        }
    }

    return (pinInstances);
}

/*
 *  ======== pinmuxRequirements ========
 */
function pinmuxRequirements(inst)
{

    let sd0 = {
        name: "SD0Pin",
        displayName: "SD0 Pin",
        interfaceNames: ["DATA0"]
    };

    let sd1 = {
        name: "SD1Pin",
        displayName: "SD1 Pin",
        interfaceNames: ["DATA1"]
    };

    let sck = {
        name: "SCKPin",
        displayName: "SCK Pin",
        interfaceNames: ["BCLK"]
    };

    let ws = {
        name: "WSPin",
        displayName: "WS Pin",
        interfaceNames: ["WCLK"]
    };

    let mclk = {
        name: "MCLKPin",
        displayName: "MCLK Pin",
        interfaceNames: ["MCLK"]
    };

    let resources = [];

    if (inst.sd0DataDirection != "Disabled") {
        resources.push(sd0);
    }

    if (inst.sd1DataDirection != "Disabled") {
        resources.push(sd1);
    }

    resources.push(sck);
    resources.push(ws);

    if (inst.enableMCLK == true) {
        resources.push(mclk);
    }

    let i2s = {
        name: "i2s",
        displayName: "I2S Peripheral",
        description: "I2S Multichannel Audio Serial Port",
        interfaceName: "I2S",
        resources     : resources,
        signalTypes   : {
            SD0Pin     : ['I2S_SD0'],
            SD1Pin     : ['I2S_SD1'],
            SCKPin     : ['I2S_SCK'],
            WSPin      : ['I2S_WS'],
            MCLKPin    : ['I2S_MCLK']
        }
    };

    return ([i2s]);
}

/*
 *  ======== extend ========
 */
function extend(base)
{
    /* overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* concatenate device-specific configs */
    result.config = base.config.concat(devSpecific.config);

    return (result);
}

/*
 *  ======== exports ========
 */
exports = {
    extend: extend
};
