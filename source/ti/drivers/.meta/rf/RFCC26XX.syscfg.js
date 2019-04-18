/*
 * Copyright (c) 2018 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== RFCC26XX.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "RF module hardware interrupt priority";

let swiPriority = Common.newSwiPri();
swiPriority.name = "softwareInterruptPriority";
swiPriority.displayName = "Software Interrupt Priority";
swiPriority.description = "RF module software interrupt priority";

let globalCallbackLongDesc = "The RF driver provides an interface "
    + "to register a global, client independent callback.  This callback "
    + "is typically used to control board related configurations, such "
    + "as antenna switches.  If the board has antenna switches, a "
    + "default callback (rfDriverCallback) is provided.  By setting "
    + "globalCallbackFunction to NULL, the default callback, if it exists, "
    + "will be registered.";

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base PIN1 configuration
 */
let devSpecific = {

    moduleStatic: {
        displayName: "RF Global",
        config: [
            intPriority,
            swiPriority,
            {
                name: "xoscNeeded",
                displayName: "XOSC Needed",
                description: "Keep XOSC Dependency While in Standby",
                default: true
            },
            {
                name: "globalCallbackFunction",
                displayName: "Global Callback Function",
                longDescription: globalCallbackLongDesc,
                default: "NULL"
            },
            {
                name: "globalEventMask",
                displayName: "Global Event Mask",
                minSelections: 0,
                default: [],
                options: [
                    { name: "RF_GlobalEventRadioSetup" },
                    { name: "RF_GlobalEventRadioPowerDown" }
                ]
            }
        ],

        filterHardware: filterHardware,
        onHardwareChanged: onHardwareChanged,
        pinmuxRequirements: pinmuxRequirements,
        validate: validate,

        /* bring in Power module */
        modules: Common.autoForcePowerModule,

        moduleInstances: moduleInstances,

        /* Array to hold RF antenna pins, if any */
        devicePins : []
    },

    /* override device-specific templates */
    templates: {
        boardc : "/ti/drivers/rf/RFCC26XX.Board.c.xdt",
        boardh : "/ti/drivers/rf/RFCC26XX.Board.h.xdt"
    }
};

/*
 *  ======== filterRFPin ========
 */
function filterRFPin(devicePin, peripheralPin)
{
    let RF = system.getScript("/ti/drivers/RF");
    let rfPins = RF.moduleStatic.devicePins;

    for (let i = 0; i < rfPins.length; i++) {
        if (rfPins[i].ball == devicePin.ball) {
            return (true);
        }
    }

    return (false);
}

/*
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config
 */
function pinmuxRequirements(inst)
{
    if (!inst.$hardware) {
        return ([]);
    }

    let rfArray = [];

    for (let i = 0; i < inst.$module.devicePins.length ; i++) {
        rfArray[i] = {
            name: "rfAntennaPin" + i,
            displayName: "RF Antenna Pin",
            hidden: true,
            interfaceName: "GPIO",
            signalTypes: ["RF"],
            filter: filterRFPin
        };
    }

    return (rfArray);
}

/*
 *  ======== validate ========
 *  Validate RF module's configuration
 */
function validate(inst, validation)
{
    /* check that globalCallbackFunction is a C identifier */
    let token = inst.globalCallbackFunction;
    if (!Common.isCName(token)) {
        Common.logError(validation, inst, "globalCallbackFunction",
                "'" + token + "' is not a valid a C identifier");
    }
}

/*
 *  ======== onHardwareChange ========
 */
function onHardwareChanged(mod, ui)
{
    if (mod.$hardware) {
        mod.$module.devicePins = [];
        let component = mod.$hardware;

        for (let sig in component.signals) {
            /* Build up array of pins */
            let sigObj = component.signals[sig];
            /* Use the IOID name of the pin, e.g., "IOID_30"  */
            let io = sigObj.devicePin.description.replace("DIO", "IOID");
            let pin = {name: sigObj.name, ioid: io,
                       ball: sigObj.devicePin.ball};

            /* Save the pins for generating defines in Board.h */
            mod.$module.devicePins.push(pin);
        }

        mod.globalCallbackFunction = "rfDriverCallback";
        mod.globalEventMask = [
                "RF_GlobalEventRadioSetup",
                "RF_GlobalEventRadioPowerDown"];
    }
    else {
        mod.$module.devicePins = [];
        mod.globalCallbackFunction = "NULL";
        mod.globalEventMask = [];
    }
}

/*
 *  ======== filterHardware ========
 *  component - a hardware componenet
 *
 *  Use this function to get the pins used by the RF antenna.
 */
function filterHardware(component)
{
    /*
     *  Check for an "RF" component in the board file.  If there is an
     *  RF component, collect the pins so we can pass them to the PIN
     *  module.
     */
    if (Common.typeMatches(component.type, ["RF"])) {
        return (true);
    }
    return (false);
}

/*
 *  ======== moduleInstances ========
 *  returns an array of PIN instances
 */
function moduleInstances(mod)
{
    let pinInstance = new Array();

    if (mod.$hardware) {
        let component = mod.$hardware;
        let i = 0;

        for (let sig in component.signals) {
            let sigObj = component.signals[sig];
            /* Use the IOID name of the pin, e.g., "IOID_30"  */
            let io = sigObj.devicePin.description.replace("DIO", "IOID");

            pinInstance.push({
                    name: "pinInstance" + i,
                    displayName: "PIN Configuration While Pin is Not In Use",
                    moduleName: "/ti/drivers/PIN",
                    collapsed: true,
                    args: {
                        $name: "Board_RF_" + sigObj.name,
                        ioid: io,
                        mode: "Output",
                        outputStrength: "Maximum",
                        outputState: "Low",
                        outputType: "Push/Pull"
                    }
            });
            i += 1;
        }
    }

    return (pinInstance);
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic RF module to
 *  allow us to augment/override as needed for the CC26XX
 */
function extend(base)
{
    /* merge and overwrite base module attributes */
    return (Object.assign({}, base, devSpecific));
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by generic RF module */
    extend: extend
};
