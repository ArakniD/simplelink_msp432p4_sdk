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
 *  ======== PowerCC32XX.syscfg.js ========
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
        default     : false
    },

    /* Policy function handling */
    {
        name        : "policyFunction",
        displayName : "Policy Function",
        description : "Power policy function called from the idle loop.",
        onChange    : onChangePolicyFxn,
        default     : "PowerCC32XX_sleepPolicy",
        options     :
        [
            { name: "PowerCC32XX_sleepPolicy" },
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
        description : "The initialization function for the Power policy.",
        default     : "PowerCC32XX_initPolicy",
        onChange    : onChangePolicyInitFxn,
        options     :
        [
            { name: "PowerCC32XX_initPolicy" },
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
        name        : "enterLPDSHookFunction",
        displayName : "Enter LPDS Hook Function",
        description : "Optional hook function to call on entry into LPDS",
        placeholder : "Enter a function name to enable",
        default     : ""
    },

    {
        name        : "resumeLPDSHookFunction",
        displayName : "Resume LPDS Hook Function",
        description : "Optional hook function to call on resuming from LPDS",
        placeholder : "Enter a function name to enable",
        default     : ""
    },


    {
        name        : "enableNetworkWakeupLPDS",
        displayName : "Enable Network Wakeup LPDS",
        description : "Enable Network Activity as a Wakeup Source for LPDS",
        default     : true
    },

    {
        name         : "ramRetentionMaskLPDS",
        displayName  : "RAM Retention Mask LPDS",
        description  : "SRAM retention mask for LPDS",
        hidden       : true,
        default      : ["SRAM_COL_1", "SRAM_COL_2", "SRAM_COL_3", "SRAM_COL_4"],
        minSelections: 0,
        options      :
        [
            { name: "SRAM_COL_1" },
            { name: "SRAM_COL_2" },
            { name: "SRAM_COL_3" },
            { name: "SRAM_COL_4" }
        ]
    },

    {
        name        : "keepDebugActiveDuringLPDS",
        displayName : "Keep Debug Active During LPDS",
        description : "Keep debug interface active during LPDS",
        default     : false
    },

    {
        name        : "enableGPIOWakeupLPDS",
        displayName : "Enable GPIO Wakeup LPDS",
        description : "Enable GPIO as a Wakeup Source for LPDS",
        onChange    : onChangeEnableGPIOWakeupLPDS,
        default     : true
    },

    {
        name        : "wakeupGPIOSourceLPDS",
        displayName : "Wakeup GPIO Source LPDS",
        description : "GPIO Source for wakeup from LPDS",
        default     : "GPIO13",
        hidden      : false,  /* because enableGPIOWakeupLPDS is true */
        options     :
        [
            { name: "GPIO2" },
            { name: "GPIO4" },
            { name: "GPIO11" },
            { name: "GPIO13" },
            { name: "GPIO17" },
            { name: "GPIO24" },
            { name: "GPIO26" }
        ]
    },
    {
        name        : "wakeupGPIOTypeLPDS",
        displayName : "Wakeup GPIO Type LPDS",
        description : "GPIO Trigger Type for wakeup from LPDS",
        hidden      : false,  /* because enableGPIOWakeupLPDS is true */
        default     : "FALL_EDGE",
        options     :
        [
            { name: "LOW_LEVEL"  },
            { name: "HIGH_LEVEL" },
            { name: "FALL_EDGE"  },
            { name: "RISE_EDGE"  }
        ]
    },
    {
        name        : "wakeupGPIOLPDSFunction",
        displayName : "Wakeup GPIO LPDS Function",
        description : "Optianal hook function to call on LPDS wakeup "
                      + "triggered by GPIO",
        onChange    : onChangeWakeupGPIOFxnLPDS,
        placeholder : "Enter a function name to enable",
        hidden      : false,  /* because enableGPIOWakeupLPDS is true */
        default     : ""
    },
    {
        name        : "wakeupGPIOLPDSFunctionArg",
        displayName : "Wakeup GPIO LPDS Function Arg",
        description : "Integer argument to Function called on GPIO "
                      + "triggered wakeup from LPDS",
        hidden      : true,
        default     : 0
    },

    {
        name        : "enableGPIOWakeupShutdown",
        displayName : "Enable GPIO Wakeup Shutdown",
        description : "Enable GPIO as a Wakeup Source for Shutdown",
        onChange    : onChangeEnableGPIOWakeupShutdown,
        default     : true
    },

    {
        name        : "wakeupGPIOSourceShutdown",
        displayName : "Wakeup GPIO Source Shutdown",
        description : "GPIO Source for wakeup from Shutdown",
        hidden      : false,  /* because enableGPIOWakeupShutdown is true */
        default     : "GPIO13",
        options     :
        [
            { name: "GPIO2" },
            { name: "GPIO4" },
            { name: "GPIO11" },
            { name: "GPIO13" },
            { name: "GPIO17" },
            { name: "GPIO24" },
            { name: "GPIO26" }
        ]
    },

    {
        name        : "wakeupGPIOTypeShutdown",
        displayName : "Wakeup GPIO Type Shutdown",
        description : "GPIO Trigger Type for wakeup from Shutdown",
        hidden      : false,  /* because enableGPIOWakeupShutdown is true */
        default     : "RISE_EDGE",
        options     :
        [
            { name: "LOW_LEVEL"  },
            { name: "HIGH_LEVEL" },
            { name: "FALL_EDGE"  },
            { name: "RISE_EDGE"  }
        ]
    },

    {
        name         : "ioRetentionShutdown",
        displayName  : "IO retention Shutdown",
        description  : "IO retention mask for Shutdown",
        default      : [ "GRP_0", "GRP_1", "GRP_2", "GRP_3" ],
        minSelections: 0,
        options      :
        [
            { name: "GRP_0" },
            { name: "GRP_1" },
            { name: "GRP_2" },
            { name: "GRP_3" }
        ]
    }
];

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base Power configuration
 */
let devSpecific = {
    getClockFrequencies : getClockFrequencies,

    moduleStatic : {
        config          : config,
        moduleInstances : modulesInstances,
        validate        : validate
    },

    templates           : {
        boardc      : "/ti/drivers/power/PowerCC32XX.Board.c.xdt",
        board_initc : "/ti/drivers/power/PowerCC32XX.Board_init.c.xdt"
    }
};

/*
 *  ======== onChangeWakeupGPIOFxnLPDS ========
 *  onChange callback function for the wakeupGPIOLPDSFunction config
 */
function onChangeWakeupGPIOFxnLPDS(inst, ui)
{
    let subState = (inst.wakeupGPIOLPDSFunction == '');
    ui.wakeupGPIOLPDSFunctionArg.hidden = subState;
}

/*
 *  ======== onChangeEnableGPIOWakeupLPDS ========
 *  onChange callback function for the enableGPIOWakeupLPDS config
 */
function onChangeEnableGPIOWakeupLPDS(inst, ui)
{
    let subState = (!inst.enableGPIOWakeupLPDS);
    ui.wakeupGPIOTypeLPDS.hidden     = subState;
    ui.wakeupGPIOSourceLPDS.hidden   = subState;
    ui.wakeupGPIOLPDSFunction.hidden = subState;

    if (inst.enableGPIOWakeupLPDS &&
            inst.wakeupGPIOLPDSFunction != '') {
        ui.wakeupGPIOLPDSFunctionArg.hidden = false;
    }
    else {
        ui.wakeupGPIOLPDSFunctionArg.hidden = true;
    }
}

/*
 *  ======== onChangeEnableGPIOWakeupShutdown ========
 *  onChange callback function for the enableGPIOWakeupShutdown config
 */
function onChangeEnableGPIOWakeupShutdown(inst, ui)
{
    let subState = (!inst.enableGPIOWakeupShutdown);
    ui.wakeupGPIOTypeShutdown.hidden   = subState;
    ui.wakeupGPIOSourceShutdown.hidden = subState;
}

/*
 *  ======== onChangeEnablePolicy ========
 *  onChange callback function for the enablePolicy config
 */
function onChangeEnablePolicy(inst, ui)
{
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
    let subState = inst.policyFunction !== 'Custom';

    ui.policyCustomFunction.hidden = subState;
}

/*
 *  ======== moduleInstances ========
 *  Return a array of dependent modules for PowerCC32XX
 */
function modulesInstances(inst)
{
    return ([{
        name : 'parkPins',
        moduleName : '/ti/drivers/power/PowerCC32XXPins',
        collapsed : true
    }]);
}

/*
 *  ======== getClockFrequencies ========
 *  Return the value of the CC26XX main CPU clock frequency
 */
function getClockFrequencies(clock)
{
    return [ 80000000 ];
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

    if (!isCName(inst.enterLPDSHookFunction)) {
        logError(vo, inst, 'enterLPDSHookFunction', 'Not a valid C identifier');
    }

    if (!isCName(inst.resumeLPDSHookFunction)) {
        logError(vo, inst, 'resumeLPDSHookFunction', 'Not a valid C identifier');
    }

    if (!isCName(inst.wakeupGPIOLPDSFunction)) {
        logError(vo, inst, 'wakeupGPIOLPDSFunction', 'Not a valid C identifier');
    }
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic Power module to
 *  allow us to augment/override as needed for the CC32XX
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
