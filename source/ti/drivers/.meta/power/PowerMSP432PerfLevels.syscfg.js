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
 *  ======== PowerMSP432PerfLevels.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common   = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;

let VLOCLK  = 9400;     // 9.4 kHz
let REFOCLK = 32768;    // selectable to alternative 128000
let LFXTCLK = 32768;    // Assumed freq if enabled.
let MODOSC  = 25000000; // 25 MHz;

let divChoices = [
    { name:   1 },
    { name:   2 },
    { name:   4 },
    { name:   8 },
    { name:  16 },
    { name:  32 },
    { name:  64 },
    { name: 128 }
];

/* Array of Performance Level configurations for MSP432 */
let config =  [
    {
        name        : "mclk",
        displayName : "MCLK",
        description : "MCLK frequency for this Performance Level",
        default     : 3000000,
        readOnly    : true
    },
    {
        name        : "hsmclk",
        displayName : "HSMCLK",
        description : "HSMCLK frequency for this Performance Level",
        default     : 3000000,
        readOnly    : true
    },
    {
        name        : "smclk",
        displayName : "SMCLK",
        description : "SMCLK frequency for this Performance Level",
        default     : 3000000,
        readOnly    : true
    },
    {
        name        : "bclk",
        displayName : "BCLK",
        description : "BCLK frequency for this Performance Level",
        default     : 32768,
        readOnly    : true
    },
    {
        name        : "aclk",
        displayName : "ACLK",
        description : "ACLK frequency for this Performance Level",
        default     : 32768,
        readOnly    : true
    },


    {
        name        : "activeState",
        displayName : "Active State",
        description : "Active State for the Device",
        default     : "LDO",
        options     :
        [
            { name: "LDO"  /* Low Dropout */},
            { name: "DCDC" /* DC-DC */ },
            { name: "LF"   /* Low Frequency */}
        ]
    },
    {
        name        : "coreVoltageLevel",
        displayName : "Core Voltage Level",
        default     : "VCORE0",
        options     :
        [
            { name : "VCORE0" },
            { name : "VCORE1" }
        ]
    },
    {
        name        : "dcoFrequencyRangeSelection",
        displayName : "DCO Frequency Range Selection",
        default     : 3000000,
        onChange    : calcOutClockFreq,
        options     :
        [
            { name: 1500000,   description: "1  MHz to 2  MHz" },
            { name: 3000000,   description: "2  MHz to 4  MHz" },
            { name: 6000000,   description: "4  MHz to 8  MHz" },
            { name: 12000000,  description: "8  MHz to 16 MHz" },
            { name: 24000000,  description: "16 MHz to 32 MHz" },
            { name: 48000000,  description: "32 MHz to 64 MHz" }
        ]
    },

    /* Source Configuration */
    {
        name        : "mclkSource",
        displayName : "MCLK Source",
        default     : "DCO",
        onChange    : calcOutClockFreq,
        options     :
        [
            { name: "DCO"    },
            { name: "HFXT"   },
            { name: "LFXT"   },
            { name: "VLO"    },
            { name: "REFO"   },
            { name: "MODOSC" }
        ]
    },
    {
        name        : "smclkSharedSource",
        displayName : "SMCLK Shared Source",
        description : "HSMCLK / SMCLK Shared Source",
        default     : "DCO",
        onChange    : calcOutClockFreq,
        options     :
        [
            { name: "DCO"    },
            { name: "HFXT"   },
            { name: "LFXT"   },
            { name: "VLO"    },
            { name: "REFO"   },
            { name: "MODOSC" }
        ]
    },
    {
        name        : "bclkSource",
        displayName : "BCLK Source",
        default     : "REFO",
        onChange    : calcOutClockFreq,
        options     :
        [
            { name: "LFXT" },
            { name: "REFO" }
        ]
    },

    {
        name        : "aclkSource",
        displayName : "ACLK Source",
        default     : "REFO",
        onChange    : calcOutClockFreq,
        options     :
        [
            { name: "LFXT" },
            { name: "VLO"  },
            { name: "REFO" }
        ]
    },

    {
        name        : "mclkDivider",
        displayName : "MCLK Divider",
        default     : 1,
        onChange    : calcOutClockFreq,
        options     : divChoices
    },

    /* Divider Configuration */
    {
        name        : "hsmclkDivider",
        displayName : "HSMCLK Divider",
        default     : 1,
        onChange    : calcOutClockFreq,
        options     : divChoices
    },
    {
        name        : "smclkDivider",
        displayName : "SMCLK Divider",
        default     : 1,
        onChange    : calcOutClockFreq,
        options     : divChoices
    },

    {
        name        : "aclkDivider",
        displayName : "ACLK Divider",
        default     : 1,
        onChange    : calcOutClockFreq,
        options     : divChoices
    },

    /* FLASH configuration */
    {
        name        : "flashWaitStates",
        displayName : "Flash Wait-States",
        description : "Number of Flash Wait-States",
        default     : 0
    },

    {
        name        : "enableFlashBuffer",
        displayName : "Enable Flash Buffer",
        description : "Enable Flash Read Buffering",
        default     : false
    }
];

/*
 *  ======== getClockSourceFreq ========
 */
function getClockSourceFreq(clk, perfInst)
{
    let pow = system.getScript("/ti/drivers/Power").$static;

    switch (clk) {
        case 'VLO'   : { return VLOCLK;                                  }
        case 'REFO'  : { return REFOCLK;                                 }
        case 'MODOSC': { return MODOSC;                                  }
        case 'DCO'   : { return perfInst.dcoFrequencyRangeSelection;     }
        case 'HFXT'  : { return pow.configurePinHFXT ? pow.HFXTFREQ : 0; }
        case 'LFXT'  : { return pow.configurePinLFXT ? LFXTCLK      : 0; }
    }
    return 0;
}

/*
 *  ======== calcOutClockFreq ========
 */
function calcOutClockFreq(inst, ui)
{
    let divm  = inst.mclkDivider;
    let divhs = inst.hsmclkDivider;
    let divs  = inst.smclkDivider;
    let diva  = inst.aclkDivider;

    inst.mclk    = getClockSourceFreq(inst.mclkSource, inst) / divm;
    inst.hsmclk  = getClockSourceFreq(inst.smclkSharedSource, inst) / divhs;
    inst.smclk   = getClockSourceFreq(inst.smclkSharedSource, inst) / divs;
    inst.bclk    = getClockSourceFreq(inst.bclkSource, inst);
    inst.aclk    = getClockSourceFreq(inst.aclkSource, inst) / diva;
}

/*
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  param inst - PowerMSP432PerfLevels instance to be validated
 *  param vo   - Issue reporting object
 */
function validate(inst, vo)
{
    /* activeState, coreVoltageLevel, dcoFrequencyRangeSelection,
     * flashWaitStates, enableFlashBuffer
     * DIV:  M, HS, S, A
     * SEL:  M, S, B, A
     * CLK:  M, HSM, SM, B, A
     */

    if (inst.ALCK > 128000) {
        logError(vo, inst, "aclkSource", 'ACLK frequency must be 128kHz or lower.');
    }

    if (inst.BLCK > 32768) {
        logError(vo, inst, "bclkSource", 'BCLK frequency must be 32768 or lower.');
    }

    if (inst.smclk > 24000000) {
        logError(vo, inst, ["smclkSharedSource", "smclkDivider"],
                 'SMCLK frequency must be 24000000 or lower.');
    }

    if (inst.coreVoltageLevel === "VCORE0") {
        if (inst.mclk > 24000000) {
            logError(vo, inst, "coreVoltageLevel",
                'MCLK frequency must be 24000000 or lower with VCORE0.');
        }
        if (inst.smclk > 12000000) {
            logError(vo, inst, "coreVoltageLevel",
                'SMCLK frequency must be 12000000 or lower, with VCORE0.');
        }
    }

    if (inst.activeState === 'LF') {
        if (inst.mclk   > 128000) {
            logError(vo, inst, ["activeState"],
            'MCLK frequency must be 128000 or lower, if active state is LF.');
        }
        if (inst.smclk  > 128000) {
            logError(vo, inst, ["activeState"],
            'SMCLK frequency must be 128000 or lower, if active state is LF.');
        }
        if (inst.hsmclk > 128000) {
            logError(vo, inst, ["activeState"],
            'HSMCLK frequency must be 128000 or lower, if active state is LF.');
        }
    }

    if (getClockSourceFreq(inst.mclkSource, inst) === 0) {
        logError(vo, inst, ["mclkSource"],
            'Selected clock source has not been configured.');
    }

    if (getClockSourceFreq(inst.smclkSharedSource, inst) === 0) {
        logError(vo, inst, ["smclkSharedSource"],
            'Selected clock source has not been configured.');
    }

    if (getClockSourceFreq(inst.bclkSource, inst) === 0) {
        logError(vo, inst, ["bclkSource"],
            'Selected clock source has not been configured.');
    }

    if (getClockSourceFreq(inst.aclkSource, inst) === 0) {
        logError(vo, inst, ["aclkSource"],
            'Selected clock source has not been configured.');
    }
}

/*
 *  ======== exports ========
 *  The exported objects for this module
 */
exports = {
    name        : "perfLevel",
    displayName : "Performance Levels",
    description : "MSP432 performance levels used by Power",
    validate    : validate,
    maxInstances: 8,
    config      : config
};
