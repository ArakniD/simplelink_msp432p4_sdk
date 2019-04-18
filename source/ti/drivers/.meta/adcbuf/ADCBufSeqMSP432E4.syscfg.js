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
 *  ======== ADCBufSeqMSP432E4.syscfg.js ========
 */

"use strict";

let Common = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;

/* Each sequencer uses a unique DMA channel per ADC peripheral */
var SEQUENCER_DMA = ["DMA_SS0", "DMA_SS1", "DMA_SS2", "DMA_SS3"];

/*
 *  ======== exports ========
 */
exports = {
    config: [
        {
            name: "priority",
            displayName: "Priority",
            descriptions: "Specify this sequencer's priority. The highest"
                + " priority is 0.",
            default: 0,
            options: [
                { name: 0 },
                { name: 1 },
                { name: 2 },
                { name: 3 }
            ]
        },
        {
            name: "triggerSource",
            displayName: "Trigger Source",
            default: "Timer",
            options: [
                { name: "Timer" },
                { name: "Auto" }
            ]
        },
        {
            name: "channels",
            displayName: "Channels",
            description: "Specifies the number of channels to configure for"
                + " this sequencer.",
            default: 1,
            onChange: onChannelChange
        },
        {
            /* Used to internally pass enableDMA argument */
            name: "enableDMA",
            default: true,
            hidden: true
        },
        {
            /* Used to internally record this instance's max channels */
            name: "maxChans",
            default: -1,
            hidden: true
        },
        {
            /* Used to internally record this instance's sequencer number */
            name: "number",
            default: -1,
            hidden: true
        }
    ],

    moduleInstances : moduleInstances,

    pinmuxRequirements: pinmuxRequirements,

    validate: validate
};

/*
 *  ======== pinmuxRequirements ========
 */
function pinmuxRequirements(inst)
{
    let requirements = [];

    if (inst.number === -1) {
        return (requirements);
    }

    let adc = {
        name: "adc",
        displayName: "ADC",
        interfaceName: "ADC",
        canShareWith: inst.$ownedBy.$name,
        hidden: true,
        resources: []
    };

    if (inst.enableDMA === true) {
        adc.resources.push({
            name: "dmaChannel",
            displayName: "DMA Channel",
            interfaceNames: [SEQUENCER_DMA[inst.number]],
            hidden: true,
            readOnly: true
        });
    }

    requirements.push(adc);

    return (requirements);
}

/*
 *  ======== validate ========
 */
function validate(inst, validation)
{
    let parent = inst.$ownedBy;
    if (parent) {
        if (parent.enableTimer === false) {
            if (inst.triggerSource === "Timer") {
                logError(validation, inst, "triggerSource",
                    "Timer is not enabled for sequencers.");
            }
        }
    }
}

/*
 *  ======== onChanneChange ========
 */
function onChannelChange(inst, ui)
{
    if (inst.maxChans != -1) {
        if (inst.channels > inst.maxChans) {
            inst.channels = inst.maxChans;
        }
    }
}

/*
 *  ======== moduleInstances ========
 */
function moduleInstances(inst)
{
    let result = [];
    if (inst.maxChans != -1) {
        for (let i = 0; i < inst.channels; i++) {
            result.push({
                name: "channel" + i,
                displayName: "Sequencer " + inst.number + " Channel " + i,
                moduleName: "/ti/drivers/adcbuf/ADCBufChanMSP432E4",
                hardware: inst.$hardware
            });
        }
    }
    return (result);
}