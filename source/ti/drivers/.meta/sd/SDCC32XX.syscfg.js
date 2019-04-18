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
 *  ======== SDCC32XX.syscfg.js ========
 */

"use strict";

let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "SDHost Interrupt Priority";
intPriority.hidden = true;

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base SD configuration
 */
let devSpecific = {
    config: [
        {
            name: "clockRate",
            displayName: "Clock Rate",
            default: 8000000
        },
        {
            name: "interfaceType",
            displayName: "Interface Type",
            default: "SD Host",
            options: [
                { name: "SD SPI" },
                { name: "SD Host" }
            ],
            onChange: interfaceChange
        },
        intPriority
    ],

    onHardwareChanged: onHardwareChanged,

    pinmuxRequirements: pinmuxRequirements,

    sharedModuleInstances : sharedModuleInstances,

    moduleInstances : moduleInstances,

    modules: Common.autoForceModules(["Board", "Power", "DMA"]),

    filterHardware: filterHardware,

    templates: {
        boardc: "/ti/drivers/sd/SDCC32XX.Board.c.xdt",
        boardh: "/ti/drivers/sd/SD.Board.h.xdt"
    }
};

/*
 *  ======== sharedModuleInstances ========
 */
function sharedModuleInstances(inst)
{
    if (inst.interfaceType === "SD SPI") {
        return ([{
            name: "spiInstance",
            displayName: "SPI",
            moduleName: "/ti/drivers/SPI",
            hardware: inst.$hardware ? inst.$hardware.subComponents.spi : null
        }]);
    }
    return ([]);
}

/*
 *  ======== moduleInstances ========
 */
function moduleInstances(inst)
{
    if (inst.interfaceType === "SD SPI") {
        return ([{
            name: "slaveSelect",
            displayName: "Slave Select",
            moduleName: "/ti/drivers/GPIO",
            hardware: inst.$hardware ? inst.$hardware.subComponents.select : null,
            args: {
                comment: "%l    /* SD SPI Chip Select */",
                mode: "Output",
                outputType: "Standard",
                initialOutputState:"High"
            }
        }]);
    }
    return ([]);
}

/*
 *  ======== pinmuxRequirements ========
 */
function pinmuxRequirements(inst)
{
    if (inst.interfaceType === "SD Host") {
        let sdHost = {
            name: "sdHost",
            displayName: "SD Host Peripheral",
            interfaceName: "SDHost",
            hidden: true,
            resources: [
                {
                    name: "clkPin",
                    displayName: "CLK Pin",
                    interfaceNames: ["CLK"]
                },
                {
                    name: "cmdPin",
                    displayName: "CMD Pin",
                    interfaceNames: ["CMD"]
                },
                {
                    name: "dataPin",
                    displayName: "DATA Pin",
                    interfaceNames: ["DATA"]
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
            ],

            signalTypes: {
                clkPin: ["SDHost_CLK"],
                cmdPin: ["SDHost_CMD"],
                dataPin: ["SDHost_DATA"]
            }
        };
        
        return ([sdHost]);
    }

    return ([]);
}

/*
 *  ========= filterHardware ========
 */
function filterHardware(component)
{
    let clk, cmd, data;

    /* component must have SD Host signals */
    for (let sig in component.signals) {
        if (component.signals[sig].type) {
            let type = component.signals[sig].type;
            if (Common.typeMatches(type,["SDHost_CLK"])) clk = sig;
            if (Common.typeMatches(type,["SDHost_CMD"])) cmd = sig;
            if (Common.typeMatches(type,["SDHost_DATA"])) data = sig;
        }
    }

    /*
     * confirm that this component has all the signals required
     * to be an SDHost
     */
    if (clk && cmd && data) {
        return (true);
    }

    return (false);
}

/*
 *  ======== interfaceChange ========
 */
function interfaceChange(inst, ui)
{
    if (inst.interfaceType === "SD Host") {
        ui.interruptPriority.hidden = false;
    }
    else {
        ui.interruptPriority.hidden = true;
    }
}

/*
 *  ======== onHardwareChanged ========
 */
function onHardwareChanged(inst, ui)
{
    if (inst.$hardware) {
        let component = inst.$hardware;
        for (let sig in component.signals) {
            if (component.signals[sig].type) {
                let type = component.signals[sig].type;
                if (Common.typeMatches(type, ["SDHost_CLK"])) {
                    inst.interfaceType = "SD Host";
                    ui.interfaceType.readOnly = true;
                    return;
                }
                if (Common.typeMatches(type, ["SPI_CLK"])) {
                    inst.interfaceType = "SD SPI";
                    ui.interfaceType.readOnly = true;
                    return;
                }
            }
        }
    }

    ui.interfaceType.readOnly = false;
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic SD module to
 *  allow us to augment/override as needed for the CC32XX.
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
    /* required function, called by base SD module */
    extend: extend
};
