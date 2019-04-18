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
 *  ======== SPIMSP432E4DMA.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "dmaInterruptPriority";
intPriority.displayName = "DMA Interrupt Priority";

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base SPI configuration
 */
let devSpecific = {
    config: [
        {
            name: "spiMode",
            displayName: "SPI Mode",
            description: "Whether this SPI will be used as a master or slave.",
            default: "Master",
            options: [
            { name: "Master" },
            { name: "Slave" }
            ]
        },
        intPriority
    ],

    /* override generic requirements with  device-specific reqs (if any) */
    pinmuxRequirements: pinmuxRequirements,

    templates: {
        boardc: "/ti/drivers/spi/SPIMSP432E4DMA.Board.c.xdt",
        boardh: "/ti/drivers/spi/SPI.Board.h.xdt"
    }
};

/*
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config
 */
function pinmuxRequirements(inst)
{
    let misoRequired = false;
    let mosiRequired = false;
    let txRequired   = true;
    let rxRequired   = true;
    let isMaster     = true;

    if (inst.spiMode == "Slave"){
        isMaster = false;
    }

    switch (inst.duplex) {
        case "Full":
            misoRequired = true;
            mosiRequired = true;
            break;
        case "Master TX Only":
            misoRequired = false;
            mosiRequired = true;
            rxRequired   = false;
            break;
        case "Slave RX Only":
            misoRequired = false;
            mosiRequired = true;
            txRequired   = false;
            break;
        case "Master RX Only":
            misoRequired = true;
            mosiRequired = false;
            txRequired   = false;
            break;
        case "Slave TX Only":
            misoRequired = true;
            mosiRequired = false;
            rxRequired   = false;
            break;
    }

    let spi = {
        name: "spi",
        displayName: "SPI Peripheral",
        interfaceName: "SSI",
        canShareWith: "SPI",
        resources: [
            {
                name: "sclkPin",
                displayName: "SCLK Pin",
                description: "Serial SPI Pin",
                interfaceNames: ["CLK"]
            }
        ]
    };

    /*
     * XDAT0 is always TX and XDAT1 is always RX, therefore MOSI and MISO
     * depend on if the SPI is a master or slave
     */
    if (isMaster) {
        if (misoRequired) {
            spi.resources.push({
                name: "misoPin",
                displayName: "MISO Pin",
                description: "Master Input Slave Output pin",
                interfaceNames: ["XDAT1"]});
        }

        if (mosiRequired) {
            spi.resources.push({
                name: "mosiPin",
                displayName: "MOSI Pin",
                description: "Master Output Slave Input pin",
                interfaceNames: ["XDAT0"]});
        }
    }
    else {
        if (misoRequired) {
            spi.resources.push({
                name: "misoPin",
                displayName: "MISO Pin",
                description: "Master Input Slave Output pin",
                interfaceNames: ["XDAT1"]});
        }

        if (mosiRequired) {
            spi.resources.push({
                name: "mosiPin",
                displayName: "MOSI Pin",
                description: "Master Output Slave Input pin",
                interfaceNames: ["XDAT0"]});
        }
    }

    /* add SS pin if one of the four pin modes is selected */
    if (inst.mode != "Three Pin") {
        spi.resources.push({
                    name: "ssPin",
                    displayName: "SS Pin",
                    interfaceNames: ["FSS"]
                });
    }

    if (rxRequired) {
        spi.resources.push({
            name: "dmaRxChannel",
            displayName: "DMA RX Channel",
            description: "DMA channel used to receive data.",
            interfaceNames: ["DMA_RX"]});
    }

    if (txRequired) {
        spi.resources.push({
            name: "dmaTxChannel",
            displayName: "DMA TX Channel",
            description: "DMA channel used to receive data.",
            interfaceNames: ["DMA_TX"]});
    }

    spi.signalTypes = {
        sclkPin: ["SPI_SCLK"],
        mosiPin: ["SPI_MOSI"],
        misoPin: ["SPI_MISO"],
        ssPin:   ["DOUT", "SPI_SS"]
    };

    return ([spi]);
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic SPI module to
 *  allow us to augment/override as needed for the MSP432E4
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
    /* required function, called by base SPI module */
    extend: extend
};
