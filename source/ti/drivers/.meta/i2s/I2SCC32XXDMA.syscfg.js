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
 *  ======== I2SCC32XXDMA.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "I2S peripheral interrupt priority";

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base I2S configuration
 */
let devSpecific = {
    config: [
        intPriority
    ],

    maxInstances: 1,

    pinmuxRequirements: pinmuxRequirements,

    templates: {
        boardc : "/ti/drivers/i2s/I2SCC32XXDMA.Board.c.xdt",
        boardh : "/ti/drivers/i2s/I2SCC32XXDMA.Board.h.xdt"
    },

    /* bring in DMA and Power modules */
    modules: Common.autoForcePowerAndDMAModules,

    filterHardware: filterHardware
};

/*
 *  ======== pinmuxRequirements ========
 */
function pinmuxRequirements(inst)
{
    let i2s = {
        name: "i2s",
        displayName: "I2S Peripheral",
        description: "I2S Multichannel Audio Serial Port",
        interfaceName: "McASP",
        resources: [
            {
                name: "clkPin",
                displayName: "CLK Pin",
                description: "Audio port clock output (McACLK)",
                interfaceNames: ["McACLK"]
            },
            {
                name: "clkxPin",
                displayName: "CLKX Pin",
                description: "Audio port clock (McACLKX)",
                interfaceNames: ["McACLKX"]
            },
            {
                name: "data0Pin",
                displayName: "Data 0 Pin",
                description: "Audio port data 0 pin (McAXR0)",
                interfaceNames: ["McAXR0"]
            },
            {
                name: "data1Pin",
                displayName: "Data 1 Pin",
                description: "Audio port data 1 pin (McAXR1)",
                interfaceNames: ["McAXR1"]
            },
            {
                name: "fsPin",
                displayName: "FS Pin",
                description: "Audio port frame sync pin (McAFSX)",
                interfaceNames: ["McAFSX"]
            },
            {
                name: "dmaTxChannel",
                displayName: "DMA TX Channel",
                interfaceNames: ["DMA_TX"]
            },
            {
                name: "dmaRxChannel",
                displayName: "DMA RX Channel",
                interfaceNames: ["DMA_RX"]
            }
        ]
    };

    i2s.signalTypes = {
        clkPin:   ["I2S_ACLK"],
        clkxPin:  ["I2S_ACLKX"],
        data0Pin: ["I2S_AXR0"],
        data1Pin: ["I2S_AXR1"],
        fsPin:    ["I2S_AFSX"]
    };

    return ([i2s]);
}

/*
 *  ========= filterHardware ========
 *  Check 'component' signals for compatibility with I2S
 *
 *  @param component - hardware object describing signals and
 *                     resources they're attached to
 *  @returns matching pinmuxRequirement object if I2S is supported.
 */
function filterHardware(component)
{
    let aclk, aclkx, afsx, axr0, axr1;

    for (let sig in component.signals) {
        let type = component.signals[sig].type;
        if (Common.typeMatches(type, ["I2S_ACLK"])) aclk = sig;
        if (Common.typeMatches(type, ["I2S_ACLKX"])) aclkx = sig;
        if (Common.typeMatches(type, ["I2S_AFSX"])) afsx = sig;
        if (Common.typeMatches(type, ["I2S_AXR0"])) axr0 = sig;
        if (Common.typeMatches(type, ["I2S_AXR1"])) axr1 = sig;
    }

    if (aclk && aclkx && afsx && axr0 && axr1) {
        return ({
            i2sData: {
                McACLK: [aclk],
                McACLKX: [aclkx],
                McAFSX: [afsx],
                McAXR0: [axr0],
                McAXR1: [axr1]
            }
        });
    }

    return (null);
}

/*
 *  ======== extend ========
 */
function extend(base)
{
    return (Object.assign({}, base, devSpecific));
}

/*
 *  ======== exports ========
 */
exports = {
    extend: extend
};
