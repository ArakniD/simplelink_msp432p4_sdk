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
 *  ======== UARTCC26XX.syscfg.js ========
 */

"use strict";

/* $super is used to call generic module's methods */
let $super = {};

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "UART peripheral interrupt priority";

let swiPriority = Common.newSwiPri();
swiPriority.name = "softwareInterruptPriority";
swiPriority.displayName = "Software Interrupt Priority";
swiPriority.description = "UART module software interrupt priority";

let logError = Common.logError;
let logInfo  = Common.logInfo;

let devId = system.deviceData.deviceId;
let numUarts = (devId.match(/CC26.2/) || devId.match(/CC13.2/)) ? 2 : 1;

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base UART configuration
 */
let devSpecific = {
    config: [
        {
            name        : "flowControl",
            displayName : "Flow Control",
            default     : false,
            description : "Enable hardware flow control"
        },
        {
            name        : "ringBufferSize",
            displayName : "Ring Buffer Size",
            description : "Number of bytes in the ring buffer",
            default     : 32
        },
        intPriority,
        swiPriority,

        /* RX and TX Fifo level thresholds */
        {
            name        : "txInterruptFifoThreshold",
            displayName : "TX Interrupt FIFO Threshold",
            description : "Select the FIFO level for the TX interrupt",
            default     : '1/8',
            options     : [
                { name : "1/8" },
                { name : "2/8" },
                { name : "4/8" },
                { name : "6/8" },
                { name : "7/8" }
            ]
        },
        {
            name        : "rxInterruptFifoThreshold",
            displayName : "RX Interrupt FIFO Threshold",
            description : "Select the FIFO level for the RX interrupt",
            default     : '4/8',
            options     : [
                { name:   "1/8" },
                { name:   "2/8" },
                { name:   "4/8" },
                { name:   "6/8" },
                { name:   "7/8" }
            ]
        }
    ],

    /* override generic pin requirements */
    pinmuxRequirements    : pinmuxRequirements,

    /* PIN instances */
    moduleInstances: moduleInstances,

    /* override device-specific templates */
    templates: {
        boardc : "/ti/drivers/uart/UARTCC26XX.Board.c.xdt",
        boardh : "/ti/drivers/uart/UART.Board.h.xdt"
    },

    /* override generic validation with ours */
    validate              : validate,
    filterHardware        : filterHardware,
    maxInstances          : numUarts
};

/*
 *  ======== pinmuxRequirements ========
 *  Control RX, TX pin usage by the user specified dataDirection.
 *  Add CTS, RTS Pins if use specified flowControl is selected.
 *
 *  param inst      - UART instance
 *
 *  returns req[] - array of requirements needed by inst
 */
function pinmuxRequirements(inst)
{
    let cts = {
        name           : "ctsPin",    /* config script name */
        displayName    : "CTS Pin",   /* GUI name */
        interfaceNames : ["CTS"]      /* pinmux tool name */
    };

    let rts = {
        name           : "rtsPin",
        displayName    : "RTS Pin",
        interfaceNames : ["RTS"]
    };

    let tx = {
        name              : "txPin",  /* config script name */
        displayName       : "TX Pin", /* GUI name */
        interfaceNames    : ["TXD"]   /* pinmux tool name */
    };

    let rx = {
        name              : "rxPin",
        displayName       : "RX Pin",
        interfaceNames    : ["RXD"]
    };

    let resources = [];

    if (inst.dataDirection != 'Receive Only') {
        resources.push(tx);
    }

    if (inst.dataDirection != 'Send Only') {
        resources.push(rx);
    }

    if (inst.flowControl === true) {
        resources.push(cts);
        resources.push(rts);
    }

    let uart = {
        name          : "uart",
        displayName   : "UART Peripheral",
        interfaceName : "UART",
        resources     : resources,
        signalTypes   : {
            txPin     : ['UART_TXD'],
            rxPin     : ['UART_RXD'],
            ctsPin    : ['UART_CTS'],
            rtsPin    : ['UART_RTS']
        }
    };

    return [uart];
}

/*
 *  ======== filterHardware ========
 *  Check 'component' signals for compatibility with UART
 *
 *  param component - hardware object describing signals and
 *                     resources they're attached to
 *
 *  returns Boolean indicating whether or not to allow the component to
 *           be assigned to an instance's $hardware config
 */
function filterHardware(component)
{
    return (Common.typeMatches(component.type, ["UART"]));
}

/*
 *  ======== moduleInstances ========
 *  returns PIN instances
 */
function moduleInstances(inst)
{
    let pinInstances = new Array();

    if (inst.dataDirection != "Receive Only") {
        pinInstances.push(
            {
                name: "txPinInstance",
                displayName: "TX PIN Configuration While Pin is Not In Use",
                moduleName: "/ti/drivers/PIN",
                collapsed: true,
                args: { parentMod: "/ti/drivers/UART",
                        parentInterfaceName: "uart",
                        parentSignalName: "txPin",
                        parentSignalDisplayName: "TX",
                        mode: "Output",
                        outputState: "Low",
                        pull: "None" }
            }
        );
    }

    if (inst.dataDirection != "Send Only") {
        pinInstances.push({
                name: "rxPinInstance",
                displayName: "RX PIN Configuration While Pin is Not In Use",
                moduleName: "/ti/drivers/PIN",
                collapsed: true,
                args: { parentMod: "/ti/drivers/UART",
                        parentInterfaceName: "uart",
                        parentSignalName: "rxPin",
                        parentSignalDisplayName: "RX",
                        mode: "Input",
                        pull: "Pull Down" }
            }
        );
    }

    return (pinInstances);
}

/*
 *  ======== validate ========
 *  Validate this instance's configuration
 *
 *  param inst       - UART instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    if (inst.ringBufferSize < 0) {
        logError(validation, inst, "ringBufferSize", "value must be positive");
    }

    /* don't allow an unreasonably large ring buffer size */
    if (inst.ringBufferSize > 1024) {
        logInfo(validation, inst, "ringBufferSize",
            "consider reducing size for space optimization");
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
    /* required function, called by base UART module */
    extend: extend
};
