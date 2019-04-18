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
 *  ======== PowerCC26XX.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common   = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;
let isCName  = Common.isCName;

/* Array of power configurables for this device family */
let config = [
    {
        name        : "enablePolicy",
        displayName : "Enable Policy",
        description : "Enable the power policy to run when the CPU is idle.",
        onChange    : onChangeEnablePolicy,
        default     : true
    },

    /* Policy function handling */
    {
        name        : "policyFunction",
        displayName : "Policy Function",
        description : "Power policy function called from the idle loop.",
        default     : "PowerCC26XX_standbyPolicy",
        onChange    : onChangePolicyFxn,
        options     :
        [
            { name: "PowerCC26XX_standbyPolicy" },
            { name: "PowerCC26XX_doWFI" },
            { name: "Custom" }
        ]
    },

    {
        name        : "policyCustomFunction",
        displayName : "Policy Custom Function",
        description : "User provided Power policy function. Usage not typical.",
        default     : "customPolicyFxn",
        hidden      : true
    },

    {
        name        : "policyInitFunction",
        displayName : "Policy Init Function",
        default     : "Not Required",
        onChange    : onChangePolicyInitFxn,
        description : "The initialization function for the Power policy.",
        options     :
        [
            { name: "Not Required" },
            { name: "Custom" }
        ]
    },

    {
        name        : "policyInitCustomFunction",
        displayName : "Policy Init Custom Function",
        description : "User provided Power policy init function. " +
                      "Usage not typical.",
        default     : "customPolicyInitFxn",
        hidden      : true
    },

    {
        name        : "calibrateFunction",
        displayName : "Calibrate Function",
        description : "Optional function. ",
        default     : "PowerCC26XX_calibrate",
        options     :
        [
            { name: "PowerCC26XX_calibrate" },
            { name: "PowerCC26XX_noCalibrate" },
        ]
    },

    {
        name        : "vddrRechargeMargin",
        displayName : "vddr Recharge Margin",
        default     : 0
    },

    {
        name        : "calibrateRCOSC_LF",
        displayName : "Calibrate RCOSC_LF",
        default     : true
    },

    {
        name        : "calibrateRCOSC_HF",
        displayName : "Calibrate RCOSC_HF",
        default     : true
    }
];

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base Power configuration
 */
let devSpecific = {
    getClockFrequencies : getClockFrequencies,
    moduleStatic        : {
        config   : config,
        validate : validate
    },
    templates           : {
        boardc      : "/ti/drivers/power/PowerCC26XX.Board.c.xdt",
        board_initc : "/ti/drivers/power/PowerCC26XX.Board_init.c.xdt"
    }
};

/*
 *  ======== onChangeEnablePolicy ========
 *  onChange callback function for the enablePolicy config
 */
function onChangeEnablePolicy(inst, ui)
{
    let subState = (!inst.enablePolicy);
    ui.policyInitFunction.hidden  = subState;
    ui.policyFunction.hidden = subState;

    onChangePolicyInitFxn(inst,ui);
    onChangePolicyFxn(inst,ui);
}

/*
 *  ======== onChangePolicyInitFxn ========
 *  onChange callback function for the policyInitFunction config
 */
function onChangePolicyInitFxn(inst, ui)
{
    let subState = (inst.policyInitFunction !== 'Custom');
    ui.policyInitCustomFunction.hidden = subState;
}

/*
 *  ======== onChangePolicyFxn ========
 *  onChange callback function for the policyFunction config
 */
function onChangePolicyFxn(inst, ui)
{
    let subState = (inst.policyFunction !== 'Custom');
    ui.policyCustomFunction.hidden = subState;
}

/*
 *  ======== getClockFrequencies ========
 *  Return the value of the CC26XX main CPU clock frequency
 */
function getClockFrequencies(clock)
{
    return [ 48000000 ];
}

/*
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  param inst - Power instance to be validated
 *  param vo   - Issue reporting object
 *
 */
function validate(inst, vo)
{
    if (inst.enablePolicy) {
        if (inst.policyInitFunction === 'Custom') {
            if (!isCName(inst.policyInitCustomFunction)) {
                logError(vo, inst, "policyInitCustomFunction",
                    "Not a valid C identifier");
            }
            if (inst.policyInitCustomFunction === '') {
                logError(vo, inst, "policyInitCustomFunction",
                    "Must contain a valid function name if the " +
                    "Policy Init Function field == 'Custom'");
            }
        }
        if (inst.policyFunction === 'Custom') {
            if (!isCName(inst.policyCustomFunction)) {
                logError(vo, inst, "policyCustomFunction",
                    "is not a valid C identifier");
            }
            if (inst.policyCustomFunction === '') {
                logError(vo, inst, "policyCustomFunction",
                    "Must contain a valid function name if the " +
                    "Policy Function field == 'Custom'");
            }
        }
    }
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic Power module to
 *  allow us to augment/override as needed for the CC26XX
 */
function extend(base)
{
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
