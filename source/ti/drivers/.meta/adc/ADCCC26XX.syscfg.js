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
 *  ======== ADCCC26XX.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base ADC configuration
 */
let devSpecific = {

    maxInstances: 11,

    config: [
        {
            name: "referenceSource",
            displayName: "Reference Source",
            default: "Fixed",
            description: "Specifies the ADC's reference source.",
            options: [
                { name: "Fixed" },
                { name: "VDDS" }
            ]
        },
        {
            name: "resolution",
            displayName: "Resolution",
            description: "Specifies the ADC's resolution",
            default: "12 Bits",
            hidden: true,
            options: [
                { name: "12 Bits" }
            ]
        },
        {
            name: "samplingDuration",
            displayName: "Sampling Duration",
            description: "Specifies the ADC's sampling duration.",
            default: "2.7 us",
            options: [
                { name: "2.7 us" },
                { name: "5.3 us" },
                { name: "10.6 us" },
                { name: "21.3 us" },
                { name: "42.6 us" },
                { name: "85.3 us" },
                { name: "170 us" },
                { name: "341 us" },
                { name: "682 us" },
                { name: "1.37 ms" },
                { name: "2.73 ms" },
                { name: "5.46 ms" },
                { name: "10.9 ms" }
            ]
        },
        {
            name: "triggerSource",
            displayName: "Trigger Source",
            description: "Specifies the trigger to start an ADC sample.",
            default: "Manual",
            readOnly: true,
            hidden: true,
            options: [
                { name: "Manual" }
            ]
        },
        {
            name: "inputScaling",
            displayName: "Input Scaling",
            description: "Enable or disable scaling on the ADC input pin.",
            default: true
        },
        {
            name: "adjustSampleValue",
            displayName: "Adjust Sample Value",
            description: "Specifies if the returned sample value is adjusted.",
            default: false
        },
        {
            name: "internalSignal",
            displayName: "Internal Signal",
            description: "Specifies internal signal(s) to use as a"
                + " sample source.",
            default: "None",
            options: [
                { name: "None" },
                {
                    name: "Battery Channel",
                    description: "Adds a channel to sample the internal"
                        + " battery voltage, VDDS."
                },
                {
                    name: "Decoupling Channel",
                    description: "Adds a channel to sample the decoupling"
                        + " capacitor voltage, DCOUPL.",
                    longDescription:`
The digital core is supplied by a 1.28-V regulator connected to VDDR.
The output of this regulator requires an external decoupling capacitor
for proper operation; this capcitor is connected to DCOUPL.
`
                },
                {
                    name: "Ground Channel",
                    description: "Adds a channel to sample the ground voltage,"
                        + " VSS."
                }
            ]
        }
    ],

    /* override generic requirements with device-specific reqs (if any) */
    pinmuxRequirements: pinmuxRequirements,

    modules: Common.autoForceModules(["Board", "Power"]),

    /* PIN instances */
    moduleInstances: moduleInstances,

    templates: {
        boardc: "/ti/drivers/adc/ADCCC26XX.Board.c.xdt",
        boardh: "/ti/drivers/adc/ADC.Board.h.xdt"
    }
};

/*
 *  ======== pinmuxRequirements ========
 *  Returns peripheral pin requirements of the specified instance
 */
function pinmuxRequirements(inst)
{

    if (inst.internalSignal !== "None") {
        return ([]);
    }

    let adc = {
        name: "adc",
        hidden: true,
        displayName: "ADC Peripheral",
        interfaceName: "ADC",
        canShareWith: "ADC",
        resources: [
            {
                name: "adcPin",
                hidden: false,
                displayName: "ADC Pin",
                interfaceNames: [
                    "PIN0", "PIN1", "PIN2", "PIN3",
                    "PIN4", "PIN5", "PIN6", "PIN7"
                ]
            }
        ],
        signalTypes: { adcPin: ["AIN"] }
    };

    return ([adc]);
}

/*
 *  ======== moduleInstances ========
 *  returns PIN instances
 */
function moduleInstances(inst)
{

    if (inst.internalSignal !== "None") {
        return ([]);
    }

    let pinInstances = new Array();

    pinInstances.push(
        {
            name: "adcPinInstance",
            displayName: "ADC Input PIN Configuration While Pin is Not In Use",
            moduleName: "/ti/drivers/PIN",
            collapsed: true,
            args: {
                parentMod: "/ti/drivers/ADC",
                parentInterfaceName: "adc",
                parentSignalName: "adcPin",
                parentSignalDisplayName: "ADC Pin",
                mode: "Input",
                pull: "None"
            }
        }
    );

    return (pinInstances);
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic ADC module to
 *  allow us to augment and override as needed.
 */
function extend(base)
{
    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* concatenate device-specific configs */
    result.config = base.config.concat(devSpecific.config);

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base ADC module */
    extend: extend
};
