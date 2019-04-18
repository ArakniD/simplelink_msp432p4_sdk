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
 *  ======== ADCBufMSP432E4.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";

/* Sequencer 0-3 support 8, 4, 4, and 1 channel(s) respectively */
var MAX_SEQ_CHANS = [8, 4, 4, 1];

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base ADCBuf configuration
 */
let devSpecific = {
    maxInstances: 2,

    config: [
        {
            name: "sequencers",
            displayName: "Sequencers",
            description: "Selects the ADC sequencers for which channels will"
                + " be configured. Each sequencer can support 8, 4, 4, and 1"
                + " channels respectively",
            default: [0],
            options: [
                { name: 0 },
                { name: 1 },
                { name: 2 },
                { name: 3 }
            ]
        },
        {
            name: "modulePhase",
            displayName: "Module Phase",
            description: "Specifies the module phase in degrees.",
            default: 0,
            options : [
                {name: 0},   {name: 22.5},  {name: 45},  {name: 67.5},
                {name: 90},  {name: 112.5}, {name: 135}, {name: 157.5},
                {name: 180}, {name: 202.5}, {name: 225}, {name: 247.5},
                {name: 270}, {name: 292.5}, {name: 315}, {name: 337.5}
            ]
        },
        {
            name: "referenceSource",
            displayName: "Reference Source",
            default: "Internal",
            description: "Specifies the ADC reference voltage source.",
            options: [
                { name: "Internal" },
                { name: "External" }
            ]
        },
        {
            name: "enableDMA",
            displayName: "Enable DMA",
            description: "Specifies if the DMA is used.",
            default: true
        },
        {
            name: "enableTimer",
            displayName: "Enable Timer",
            description: "Enabling the timer allows sequencers to use a"
                + " timer peripheral as a trigger source",
            default: true
        },

        intPriority

    ],

    moduleInstances: moduleInstances,

    modules: Common.autoForcePowerAndDMAModules,

    /* override generic requirements with  device-specific reqs (if any) */
    pinmuxRequirements: pinmuxRequirements,

    templates: {
        boardc: "/ti/drivers/adcbuf/ADCBufMSP432E4.Board.c.xdt",
        boardh: "/ti/drivers/adcbuf/ADCBufMSP432E4.Board.h.xdt"
    }
};

/*
 *  ======== pinmuxRequirements ========
 */
function pinmuxRequirements(inst)
{
    let requirements = [];

    let timer = {
        name: "timer",
        displayName: "Trigger Source Timer",
        interfaceName: "Timer",
        resources: []
    };

    let adc = {
        name: "adc",
        displayName: "ADC",
        interfaceName: "ADC",
        canShareWith: inst.$name,
        resources: []
    };

    if (inst.enableTimer === true) {
        requirements.push(timer);
    }

    requirements.push(adc);

    return (requirements);
}

/*
 *  ======== moduleInstances ========
 */
function moduleInstances(inst)
{
    let result = [];
    let triggerSource = "Timer";

    if (inst.enableTimer === false) {
        triggerSource = "Auto";
    }

    /* Create a Sequence instance for each of the unique sequencers. */
    for (let i in inst.sequencers) {
        let sequencer = inst.sequencers[i];
        result.push({
            name: "sequencer" + sequencer,
            displayName: "Sequencer " + sequencer + " ",
            moduleName: "/ti/drivers/adcbuf/ADCBufSeqMSP432E4",
            args: {
                maxChans: MAX_SEQ_CHANS[sequencer],
                number: parseInt(sequencer),
                triggerSource: triggerSource,
                enableDMA: inst.enableDMA
            }
        });
    }

    return (result);
}

/*
 *  ======== extend ========
 */
function extend(base)
{

    /* concatenate device-specific configs */
    devSpecific.config = base.config.concat(devSpecific.config);

    /* merge and overwrite base module attributes */
    return (Object.assign({}, base, devSpecific));
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base ADCBuf module */
    extend: extend
};
