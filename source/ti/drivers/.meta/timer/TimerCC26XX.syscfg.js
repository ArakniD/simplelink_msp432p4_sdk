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
 *  ======== TimerCC26XX.syscfg.js ========
 */

"use strict";

let Common = system.getScript("/ti/drivers/Common.js");

let longDescription =`The [__Timer driver__][1] allows you to manage a Timer peripheral via simple
and portable APIs.

* [Configuration Options][2]

[1]: /tidrivers/doxygen/html//_g_p_timer_c_c26_x_x_8h.html#details "C API reference"
[2]: /tidrivers/syscfg/html/ConfigDoc.html#Timer_Configuration_Options "Configuration options reference"
`;

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base Timer configuration
 */
let devSpecific = {

    config: [
        {
            name        : "useCase",
            displayName : "Use Case",
            hidden      : true,
            onChange    : onPinTypeChange,
            default     : "None",
            options     : [
                { name: "None" },
                { name: "ADCBuf" },
                { name: "PWM" },
                { name: "Capture" }
            ]
        }
    ],

    templates: {
        boardc : "/ti/drivers/timer/TimerCC26XX.Board.c.xdt",
        boardh : "/ti/drivers/timer/Timer.Board.h.xdt"
    },

    maxInstances       : 8,

    /* override generic pin requirements */
    pinmuxRequirements : pinmuxRequirements,

    /* PIN instances */
    moduleInstances    : moduleInstances,

    filterHardware     : filterHardware,
    onHardwareChanged  : onHardwareChanged
};

/*
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config
 */
function pinmuxRequirements(inst)
{
    let timer = {
        name          : "timer",
        displayName   : "Timer Peripheral",
        interfaceName : "GPTM",
        resources     : []
    };

    switch (inst.useCase) {
        case "None": {
            if (inst.timerType == "16 Bits") {
                timer.canShareWith = "16 Bit Timer";
                timer.maxShareCount = 2;
                timer.resources = []; // no resources required for Timer
            }
            break;
        }
        case "ADCBuf": {
            timer.canShareWith = "ADCBuf";
            timer.maxShareCount = 2;
            /* ADCBufCC26XX requires Timer0 */
            timer.filter = function (peripheral) {
                    return (peripheral.name.match(/GPTM0/));
                },

            timer.resources = []; // no resources required for Timer
            break;
        }
        case "PWM" : {
            timer.canShareWith = "PWM";
            timer.maxShareCount = 2;
            timer.resources = [{
                name: "pwmPin",
                displayName: "PWM Pin",
                interfaceNames: [
                    "TIMER_A_PWM", "TIMER_B_PWM"
                ]
            }];
            timer.signalTypes = { pwmPin: ["PWM"] };
            break;
        }
        case "Capture": {
            timer.canShareWith = "Capture";
            timer.maxShareCount = 2;
            timer.resources = [{
                name: "capturePin",
                displayName: "Capture Pin",
                interfaceNames: [
                    "TIMER_A_CCP", "TIMER_B_CCP"
                ]
            }];
            timer.signalTypes = { capturePin: ["DIN"] };
            break;
        }
    }

    return ([timer]);
}

/*
 *  ======== moduleInstances ========
 *  returns PIN instance
 */
function moduleInstances(inst)
{
    let pinInstances = new Array();

    switch (inst.useCase) {
        case "None":
        case "ADCBuf":
            break;
        case "PWM": {
            pinInstances.push(
                {
                    name: "pwmPinInstance",
                    displayName: "PWM Output PIN Configuration While Pin is Not In Use",
                    moduleName: "/ti/drivers/PIN",
                    collapsed: true,
                    args: { parentMod: "/ti/drivers/Timer",
                            parentInterfaceName: "timer",
                            parentSignalName: "pwmPin",
                            parentSignalDisplayName: "PWM Pin",
                            mode: "Output",
                            outputState: "Low",
                            pull: "None" }
                }
            );
            break;
        }
    }

    return (pinInstances);
}

/*
 *  ======== onPinTypeChange ========
 */
function onPinTypeChange(inst, ui)
{
    switch (inst.useCase) {
        case "None":
            ui.interruptPriority.hidden = false;
            ui.timerType.readOnly = false;
            ui.useCase.hidden = true;
            ui.useCase.readOnly = false;
            break;
        case "PWM":
        case "Capture":
            ui.interruptPriority.hidden = true;
            ui.timerType.readOnly = true;
            ui.useCase.hidden = false;
            ui.useCase.readOnly = true;
            break;
    }
}

/*
 *  ========= filterHardware ========
 *  param component - hardware object describing signals and
 *                    resources
 *
 *  returns Boolean indicating whether or not to allow the component to
 *           be assigned to an instance's $hardware config
 */
function filterHardware(component)
{
    for (let sig in component.signals) {
        let type = component.signals[sig].type;
        if (Common.typeMatches(type, ["PWM", "Capture"])) {
            return (true);
        }
    }

    return (false);
}

/*
 *  ======== onHardwareChanged ========
 *  Handle new hardware selection
 */
function onHardwareChanged(inst, ui)
{
    if (inst.$hardware) {
        let key = Object.keys(inst.$hardware.signals)[0];
        let signal = inst.$hardware.signals[key];
        let type = signal.type;

        if (Common.typeMatches(type, ["PWM"])) {
            inst.useCase = "PWM";
            ui.useCase.readOnly = true;
        }
        if (Common.typeMatches(type, ["Capture"])) {
            inst.useCase = "Capture";
            ui.useCase.readOnly = true;
        }
    }
    else {
        ui.useCase.readOnly = false;
        inst.useCase = "None";
    }

    onPinTypeChange(inst, ui);
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic Timer module to
 *  allow us to augment/override as needed for the CC26XX.
 */
function extend(base)
{
    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* concatenate device-specific configs */
    result.config = base.config.concat(devSpecific.config);

    result.longDescription = longDescription;

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base Timer module */
    extend: extend
};
