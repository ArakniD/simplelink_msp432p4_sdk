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
 *  ======== RF.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/* get /ti/drivers family name from device object */
let family = Common.device2Family(system.deviceData, "RF");

/*
 *  ======== base ========
 *  Define the base/common RF properties and methods
 */
let base = {
    displayName: "RF",
    description: "Radio Frequency (RF) Driver",
    longDescription:`
The [__Radio Frequency (RF) driver__][1] provides access to the radio core on
the CC13XX and CC26XX device family. It offers a high-level interface for
command execution and to the radio timer (RAT). The RF driver ensures the
lowest possible power consumption by providing automatic power management
that is fully transparent for the application.

* [Configuration Options][2]

For a detailed explanation of the RF core, please refer to the
[__Technical Reference Manual__][3] or the [__Proprietary RF User Guide__][4].

[1]: /tidrivers/doxygen/html/_r_f_8h.html#details "C API reference"
[2]: /tidrivers/syscfg/html/ConfigDoc.html#RF_Configuration_Options "Configuration options reference"
[3]: /proprietary-rf/technical-reference-manual.html
[4]: /proprietary-rf/proprietary-rf-users-guide.html
`
};

/* get the family-specific RF module */
let deviceRF = system.getScript("/ti/drivers/rf/RF" + family);

/* extend our common exports to include the family-specific content */
exports = deviceRF.extend(base);
