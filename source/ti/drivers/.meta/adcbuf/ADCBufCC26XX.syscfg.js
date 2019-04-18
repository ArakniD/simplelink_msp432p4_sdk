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
 *  ======== ADCBufCC26XX.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Hardware Interrupt Priority";

let swiPriority = Common.newSwiPri();
swiPriority.name = "softwareInterruptPriority";
swiPriority.displayName = "Software Interrupt Priority";

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base ADCBuf configuration
 */
let devSpecific = {
    maxInstances: 1,

    config: [
        intPriority,
        swiPriority,
        {
            name: "channels",
            displayName: "Channels (External)",
            description: "Number of external channels to configure. Select '0'"
                + " if you only wish to use one of the Battery, Decoupling,"
                + " or Ground channel(s).",
            default: 1,
            options: [
                { name: 0 }, { name: 1 }, { name: 2 }, { name: 3 }, { name: 4 },
                { name: 5 }, { name: 6 }, { name: 7 }, { name: 8 }
            ]
        },
        {
            name: "batteryChannel",
            displayName: "Battery Channel",
            description: "Adds a channel to sample the internal"
                + " battery voltage, VDDS.",
            default: false,
        },
        {
            name: "decouplingChannel",
            displayName: "Decoupling Channel",
            description: "Adds a channel to sample the decoupling capacitor"
                + " voltage, DCOUPL. The digital core is supplied by a 1.28-V"
                + " regulator connected to VDDR. The output of this"
                + " regulator requires an external decoupling capacitor"
                + " for proper operation; this capcitor is connected to"
                + " DCOUPL.",
            default: false,
        },
        {
            name: "groundChannel",
            displayName: "Ground Channel",
            description: "Adds a channel to sample the ground voltage, VSS.",
            default: false,
        }
    ],

    moduleInstances: moduleInstances,

    modules: Common.autoForceDMAModule,

    validate: validate,

    templates: {
        boardc: "/ti/drivers/adcbuf/ADCBufCC26XX.Board.c.xdt",
        boardh: "/ti/drivers/adcbuf/ADCBufCC26XX.Board.h.xdt"
    }
};

/*
 *  ======== validate ========
 */
function validate(inst, vo)
{
    if (inst.channels == 0) {
        if (inst.batteryChannel == false &&
            inst.decouplingChannel == false &&
            inst.groundChannel == false) {
            logError(vo, inst, "channels", "Must select atleast one"
                + " channel to configure.");
        }
    }
}

/*
 *  ======== moduleInstances ========
 */
function moduleInstances(inst)
{
    let modInstances = new Array();

    modInstances.push(
        {
            name: "timerInstance",
            displayName: "Timer Instance",
            moduleName: "/ti/drivers/Timer",
            hidden: false,
            args: {
                timerType: "16 Bits",
                useCase: "ADCBuf"
            }
        }
    );

    for (let i = 0; i < inst.channels; i++) {
        modInstances.push({
            name: "adcBufChannel" + i,
            displayName: "ADCBuf Channel " + i,
            moduleName: "/ti/drivers/adcbuf/ADCBufChanCC26XX",
            hardware: inst.$hardware,
            args: {
                channelNumber: i
            }
        });
    }

    return (modInstances);
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic ADCBuf module to
 *  allow us to augment/override as needed for the CC26XX
 */
function extend(base)
{

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
