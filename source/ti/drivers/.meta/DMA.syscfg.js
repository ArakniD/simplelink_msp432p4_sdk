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
 *  ======== DMA.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common      = system.getScript("/ti/drivers/Common.js");

/* get /ti/drivers family name from device object */
let family      = Common.device2Family(system.deviceData, "DMA");

let intPriority = Common.newIntPri()[0];
intPriority.displayName = "Interrupt Priority";
intPriority.name = "interruptPriority";
intPriority.description = "DMA interrupt priority.";

let config = [
    {
        /*
         *  This name is used by UDMACC26XX.syscfg.js.  If it is
         *  changed, make sure to update UDMACC26XX.syscfg.js.
         */
        name        : "dmaErrorFunction",
        displayName : "DMA Error Function",
        description : "Specifies function invoked when a DMA error occurs.",
        default     : "dmaErrorFxn"
    },
    intPriority
];

/*
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  @param inst       - DMA instance to be validated
 *  @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    if (!Common.isCName(inst.$module.$static.dmaErrorFunction)) {
        Common.logError(validation, inst, 'dmaErrorFunction',
            'Not a valid C identifier.');
    }
}


/*
 *  ======== base ========
 *  Define the base DMA properties and methods
 */
let base = {
    displayName : "DMA",
    description : "On-chip DMA resource allocation",
    longDescription : "The DMA module provides support to other drivers that"
        + " need to allocate exclusive access to a DMA resource to support"
        + " CPU-less peripheral data transfers.",
    defaultInstanceName : "Board_DMA",
    validate : validate,
    maxInstances : 1,
    moduleStatic : {
        config
    }
};

/* extend the base exports to include family-specific content */
let devDMA = system.getScript("/ti/drivers/dma/UDMA" + family);
exports = devDMA.extend(base);
