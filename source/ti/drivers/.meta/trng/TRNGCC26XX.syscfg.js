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
 *  ======== TRNGCC26XX.syscfg.js ========
 */

"use strict";

/* $super is used to call generic module's methods */
let $super = {};

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "TRNG peripheral interrupt priority";

let swiPriority = Common.newSwiPri();
swiPriority.name = "softwareInterruptPriority";
swiPriority.displayName = "Software Interrupt Priority";
swiPriority.description = "TRNG software interrupt priority";

let logError = Common.logError;

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base TRNG configuration
 */
let devSpecific = {
    config: [
        intPriority,
        swiPriority,
        {
            name        : "entropyGenerationCycles",
            displayName : "Entropy Generation Cycles",
            description : 'Clock cycles used for TRNG entropy generation. Default '
                          + 'values set to generate 64 bits of entropy in 5ms with '
                          + 'all FROs active.',
            default     : 240000
        }
    ],

    templates : {
        boardc: "/ti/drivers/trng/TRNGCC26XX.Board.c.xdt",
        boardh: "/ti/drivers/trng/TRNG.Board.h.xdt"
    },

    /* override generic validation with ours */
    validate              : validate
};

/*
 *  ======== validate ========
 *  Validate this instance's configuration
 *
 *  param inst       - TRNG instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    if (inst.entropyGenerationCycles < 256) {
        logError(validation, inst, "entropyGenerationCycles", "value must be greater than 256");
    }

    if (inst.entropyGenerationCycles > 16777216) {
        logError(validation, inst, "entropyGenerationCycles", "value must be less than 16777216 (2^24)");
    }

    if ($super.validate) {
        $super.validate(inst, validation);
    }
}

/*
 *  ======== extend ========
 */
function extend(base)
{
    /* save base properies/methods, to use in our methods */
    $super = base;

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
    /* required function, called by base TRNG module */
    extend: extend
};
