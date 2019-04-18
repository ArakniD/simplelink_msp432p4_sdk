/*
 * Copyright (c) 2018-2019, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== Display.syscfg.js ========
 */

"use strict";

let Common   = system.getScript("/ti/drivers/Common.js");
let UART     = system.getScript("/ti/drivers/UART");
let logError = Common.logError;

let config = [
    {
        name        : "displayType",
        displayName : "Display Type",
        default     : "UART",
        onChange   : onDisplayTypeChange,
        options : [
            {name: "UART"},
            {name: "LCD"},
            {name: "Host"}
        ]
    },
    {
        name        : "lcdSize",
        displayName : "LCD Size",
        description : "Size of the LCD in pixels.",
        default     : 128,
        hidden      : true
    },
    {
        name        : "uartBufferSize",
        displayName : "UART Buffer Size",
        description : "UART display buffer size in bytes",
        default     : 1024
    },
    {
        name        : "enableANSI",
        displayName : "Enable ANSI",
        description: "Enables or disables ANSI cursor support.",
        default     : false
    },
    {
        name        : "maxPrintLength",
        displayName : "Maximum Print Length",
        default     : 256,
        hidden      : true
    },
    {
        name        : "mutexTimeout",
        displayName : "Mutex Timeout",
        description : "Timeout to acquire mutex for synchronizing access to UART.",
        default     : "Never Timeout",
        onChange    : onChangeMutexTimeout,
        options     : [
            { name : "Never Timeout" },
            { name : "Never Wait" },
            { name : "Custom", description: "Custom number of RTOS ticks" }
        ]
    },
    {
        name        : "mutexTimeoutValue",
        displayName : "Mutex Timeout Value",
        hidden      : true,
        default     : 1
    },
    {
        name        : "baudRate",
        displayName : "Baud Rate",
        description : "UART baud rate",
        default     : 115200,
        options     : [
            { name:   1200 },
            { name:   2400 },
            { name:   4800 },
            { name:   9600 },
            { name:  19200 },
            { name:  38400 },
            { name:  57600 },
            { name: 115200 },
            { name: 230400 },
            { name: 460800 },
            { name: 921600 }
        ]
    }
];

/*
 * ======== moduleInstances ========
 */
function moduleInstances(inst)
{
    if (inst.displayType === "UART") {
        return ([
            {
                name       : "uart",
                moduleName : "/ti/drivers/UART",
                hardware   : inst.$hardware
            }
        ]);
    }

    if (inst.displayType === "LCD") {
        return ([
            {
                name       : "lcdEnable",
                moduleName : "/ti/drivers/GPIO",
                hardware   : inst.$hardware ? inst.$hardware.subComponents.LCD_ENABLE : null,
                args       : { mode: "Dynamic" }
            },
            {
                name       : "lcdPower",
                moduleName : "/ti/drivers/GPIO",
                hardware   : inst.$hardware ? inst.$hardware.subComponents.LCD_POWER : null,
                args       : { mode: "Dynamic" }
            },
            {
                name       : "lcdSS",
                moduleName : "/ti/drivers/GPIO",
                hardware   : inst.$hardware ? inst.$hardware.subComponents.LCD_SS : null,
                args       : { mode: "Dynamic" }
            }
        ]);
    }

    return ([]);
}

/*
 * ======== sharedModuleInstances ========
 */
function sharedModuleInstances(inst)
{
    if (inst.displayType == "LCD") {
        return ([
            {
                name       : "spi",
                moduleName : "/ti/drivers/SPI",
                hardware   : inst.$hardware ? inst.$hardware.subComponents.LCD_SPI : null
            }
        ]);
    }

    return ([]);
}

/*
 * ======== validate ========
 */
function validate(inst, validation)
{
    if (inst.mutexTimeoutValue <= 0) {
        logError(validation, inst, 'mutexTimeoutValue', 
                 'Must be a positive integer.');
    }

    if (inst.uartBufferSize < 32) {
        logError(validation, inst, 'uartBufferSize', 
                 'Must be greater than 32 bytes.');
    }

    if (inst.maxPrintLength <= 0) {
        logError(validation, inst, 'maxPrintLength',
                 'Must be a positive integer.');
    }

    if (inst.lcdSize <= 0) {
        logError(validation, inst, 'lcdSize', 'Must be a positive integer.');
    }

    /* Ensure display baud rate is supported by the uart's baud rate list */
    if (inst.displayType === "UART" && inst.uart.hasOwnProperty('baudRates')) {
        if (! inst.uart.baudRates.includes(inst.baudRate)) {
            let suffix = "";
            if (inst.uart.$hardware) {
                suffix = "(" + inst.uart.$hardware.name + ")";
            }
            logError(validation, inst, 'baudRate',
                inst.baudRate + " baud is not supported by the UART "
                + inst.uart.$name + suffix
            );
        }
    }
}

/*
 * ======== onChangeMutexTimeout ========
 *
 */
function onChangeMutexTimeout(inst, ui)
{
    if (inst.mutexTimeout == "Custom") {
        ui.mutexTimeoutValue.hidden = false;
    }
    else {
        ui.mutexTimeoutValue.hidden = true;
        inst.mutexTimeoutValue = 1;
    }
}

/*
 *  ======== onDisplayTypeChange ========
 *  Show/hide appropriate config options for each type of display
 */
function onDisplayTypeChange(inst, ui)
{
    if (inst.displayType == "LCD") {
        ui.enableANSI.hidden = true;
        ui.maxPrintLength.hidden = true;
        ui.uartBufferSize.hidden = true;
        ui.baudRate.hidden = true;
        ui.lcdSize.hidden = false;
        ui.mutexTimeout.hidden = true;
        ui.mutexTimeoutValue.hidden = true;
    }
    else if (inst.displayType == "Host") {
        ui.enableANSI.hidden = true;
        ui.maxPrintLength.hidden = false;
        ui.uartBufferSize.hidden = true;
        ui.baudRate.hidden = true;
        ui.lcdSize.hidden = true;
        ui.mutexTimeout.hidden = true;
        ui.mutexTimeoutValue.hidden = true;
    }
    else {
        ui.enableANSI.hidden = false;
        ui.maxPrintLength.hidden = true;
        ui.uartBufferSize.hidden = false;
        ui.baudRate.hidden = false;
        ui.lcdSize.hidden = true;
        ui.mutexTimeout.hidden = false;
        onChangeMutexTimeout(inst, ui);
    }
}

/*
 *  ========= filterHardware ========
 */
function filterHardware(component)
{
    let ret = false;

    if (component.name) {
        if (component.name.match("MSP430BOOST_SHARP96")) {
            return (true);
        }
        if (component.name.match("BOOSTXL_SHARP128")) {
            return (true);
        }
    }

    ret = UART.filterHardware(component);

    return (ret);
}

/*
 * ======== onHardwareChanged ========
 */
function onHardwareChanged(inst, ui)
{
    if (inst.$hardware) {
        if (inst.$hardware.name) {
            if (inst.$hardware.name.match("MSP430BOOST_SHARP96")) {
                ui.displayType.readOnly = false;
                inst.displayType = "LCD";
                onDisplayTypeChange(inst, ui);
                inst.lcdSize = 96;
                ui.displayType.readOnly = true;

                return;
            }
            if (inst.$hardware.name.match("BOOSTXL_SHARP128")) {
                ui.displayType.readOnly = false;
                inst.displayType = "LCD";
                onDisplayTypeChange(inst, ui);
                inst.lcdSize = 128;
                ui.displayType.readOnly = true;

                return;
            }
        }

        if (Common.typeMatches(inst.$hardware.type, ["UART"])) {
            ui.displayType.readOnly = false;
            inst.displayType = "UART";
            onDisplayTypeChange(inst, ui);
            ui.displayType.readOnly = true;

            return;
        }
    }
    else {
        ui.displayType.readOnly = false;
        inst.displayType = "UART";
        onDisplayTypeChange(inst, ui);

        return;
    }
}

/*
 *  ======== base ========
 *  Define the base properties and methods
 */
let base = {
    displayName           : "Display",
    description           : "Display Driver",
    longDescription: `
The [__Display driver__][1] allows you to manage Display instances via simple
and portable APIs.

* [Usage Synopsis][2]
* [Examples][3]
* [Configuration Options][4]

[1]: /tidrivers/doxygen/html/_display_8h.html#details "C API reference"
[2]: /tidrivers/doxygen/html/_display_8h.html#ti_drivers_Display_Synopsis "Basic C usage summary"
[3]: /tidrivers/doxygen/html/_display_8h.html#ti_drivers_Display_Examples "C usage examples"
[4]: /tidrivers/syscfg/html/ConfigDoc.html#Display_Configuration_Options "Configuration options reference"
`,
    documentation: "/tidrivers/doxygen/html/_display_8h.html",
    defaultInstanceName   : "Board_Display",
    config                : Common.addNameConfig(config, "ti/drivers/Display","Board_Display"),
    validate              : validate,
    maxInstances          : 3,
    filterHardware        : filterHardware,
    onHardwareChanged     : onHardwareChanged,
    moduleInstances       : moduleInstances,
    sharedModuleInstances : sharedModuleInstances,
    templates             : {
        boardc: "/ti/display/Display.Board.c.xdt"
    }
};

exports = base;
