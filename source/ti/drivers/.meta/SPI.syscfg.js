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
 *  ======== SPI.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;

/* get /ti/drivers family name from device object */
let family = Common.device2Family(system.deviceData, "SPI");

/* generic configuration parameters for SPI instances */
let config = [
    {
        name: "mode",
        displayName: "Mode",
        default: "Three Pin",
        options: [
            { name: "Three Pin" },
            { name: "Four Pin SS Active Low" },
            { name: "Four Pin SS Active High" }
        ]
    },
    {
        name: "defaultTxBufferValue",
        displayName: "Default TX Buffer Value",
        description: "Specifies the default transmit buffer value. This value"
            + " must be provided in hexadecimal format. 0 and ~0 are"
            + " also acceptable inputs.",
        default: "~0"
    },
    {
        name: "minDmaTransferSize",
        displayName: "Min DMA Transfer Size",
        description: "Specifies the minimum transfer size in bytes for which"
            + " the DMA will be used.",
        default: 10
    },
    {
        name: "duplex",
        displayName: "Duplex",
        hidden : false,
        description: "Useful in situations where the SPI bus master will"
            + " transmit data to the slaves but is not interested in the data"
            + " returned by the slaves OR where the master will not"
            + " transmit meaningful data but is interested in receiving data"
            + " from slaves. Slaves must ignore incoming data from master.",
        default: "Full",
        options: [
            { name: "Full" },
            { name: "Transmit Only" },
            { name: "Receive Only" }

        ]
    }
];

/*
 *  ========= filterHardware ========
 *  param component - hardware object describing signals and
 *                    resources
 *
 *  returns Boolean indicating whether or not to allow the component to
 *           be assigned to an instance's $hardware config
 */
function filterHardware(component)
{
    if (Common.typeMatches(component.type, ["SPI_BUS", "SPI_DEVICE"])) {
        return (true);
    }

    return (false);
}

/*
 *  ======== validate ========
 *  Validate this inst's configuration
 *
 *  @param inst       - SPI instance to be validated
 *  @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    Common.validateNames(inst, validation);

    /* Allow an input of 0 or ~0 */
    if (inst.defaultTxBufferValue.match( /((^0$)|(^~0$))/ )) {
        return;
    }

    if (inst.defaultTxBufferValue.match((/^0[X][\dA-F]{1,8}$/i)) === null) {
        logError(validation, inst, 'defaultTxBufferValue',
            'Must be in hex format.');
    }
}

/*
 *  ======== base ========
 *  Define the base properties and methods
 */
let base = {
    displayName: "SPI",
    description: "Serial Peripheral Interface (SPI) Bus Driver",
    longDescription: "The Serial Peripheral Interface (SPI) "
    + "driver is a generic, full-duplex driver that transmits "
    + "and receives data on a SPI bus. SPI is sometimes called SSI "
    + "(Synchronous Serial Interface). The SPI protocol defines "
    + "the format of a data transfer over the SPI bus, but it "
    + "leaves flow control, data formatting, and handshaking "
    + "mechanisms to higher-level software layers.",
    documentation: "/tidrivers/doxygen/html/_s_p_i_8h.html",
    defaultInstanceName: "Board_SPI",
    config: config,
    filterHardware: filterHardware,
    validate: validate,
    busModule: true,
    allowStaticOnly: true
};

/* extend the base exports to include family-specific content */
let deviceSPI = system.getScript("/ti/drivers/spi/SPI" + family + "DMA");
exports = deviceSPI.extend(base);
