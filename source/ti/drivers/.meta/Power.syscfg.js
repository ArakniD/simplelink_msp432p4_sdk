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
 *  ======== Power.syscfg.js ========
 */

"use strict";

let Common = system.getScript("/ti/drivers/Common.js");
let family = Common.device2Family(system.deviceData, "Power");

/* Intro splash on GUI */
let longDescription = "The Power manager facilitates the transition " +
    "of the MCU from active state to one of the sleep states and vice " +
    "versa. It provides drivers the ability to set and release dependencies " +
    "on hardware resources and keeps a reference count on each resource to " +
    "know when to enable or disable the peripheral clock to the resource. " +
    "It provides drivers the ability to register a callback function upon a " +
    "specific power event. In addition, drivers and apps can set or release " +
    "constraints to prevent the MCU from transitioning into a particular " +
    "sleep state.";

/* The base exports for the Power module */
let base = {
    displayName         : "Power",
    description         : "Power Driver",
    longDescription     : longDescription,
    documentation       : "/tidrivers/doxygen/html/_power_8h.html",
    defaultInstanceName : "Board_Power",
    maxInstances        : 1,
    initPriority        : -1 /* lower numbers init earlier */
};

/* get family-specific Power module */
let devPower = system.getScript("/ti/drivers/power/Power" + family);

exports = devPower.extend(base);
