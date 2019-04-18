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
 *  ======== PWM.syscfg.js ========
 */
"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/* get /ti/drivers family name from device object */
let family = Common.device2Family(system.deviceData, "PWM");

/* generic configuration parameters for PWM instances */
let pwmConfig = []; /* nothing (yet) beyond generic driver configs */

/* Intro splash on GUI */
let longDescription = "The PWM driver in TI-RTOS facilitates the generation "
    + "of Pulse Width Modulated signals via simple and "
    + "portable APIs. PWM instances must be opened by "
    + "calling PWM_open() while passing in a PWM index "
    + "and a parameters data structure."
    + "\nThe driver APIs serve as an interface to a typical "
    + "TI-RTOS application. The specific peripheral "
    + "implementations are responsible for creating all "
    + "OS specific primitives to allow for thread-safe "
    + "operation."
    + "\nWhen a PWM instance is opened, the period, duty "
    + "cycle and idle level are configured and the PWM is "
    + "stopped (waveforms not generated until PWM_start() is "
    + "called). The maximum period and duty supported is "
    + "device dependent; refer to the implementation "
    + "specific documentation for values."
    + "\nPWM outputs are active-high, meaning the duty will "
    + "control the duration of high output on the pin (at "
    + "0% duty, the output is always low, at 100% duty, the "
    + "output is always high).";

/*
 *  ======== validate ========
 *  Validate this inst's configuration
 *
 *  param inst       - PWM instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    Common.validateNames(inst, validation);
}

/*
 *  ======== base ========
 *  Define the base PWM properties and methods
 */
let base = {
    displayName: "PWM",
    description: "Pulse Width Modulation (PWM) Output Driver",
    longDescription: longDescription,
    documentation: "/tidrivers/doxygen/html/_p_w_m_8h.html",
    defaultInstanceName: "Board_PWM",
    config: pwmConfig,
    validate: validate
};

/* Get family-specific PWM module. Some PWM drivers are based on a timer
 * peripheral while others are based on dedicated PWM peripherals.
 */
let devicePWM;
if (family.match("MSP432E4")) {
    devicePWM = system.getScript("/ti/drivers/pwm/PWM" + family);
}
else {
    devicePWM = system.getScript("/ti/drivers/pwm/PWMTimer" + family);
}

exports = devicePWM.extend(base);
