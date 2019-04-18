/*
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
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
 */
/*
 *  ======== package.xs ========
 */

/*
 *  ======== getLibs ========
 *  This package does not build any libraries.
 */
function getLibs()
{
    var dplBase = xdc.loadPackage("ti.dpl").packageBase;
    var libName = "";
    var retString = "";
    var targetSuffix = "";

    /* Determine libraries required by device name. */
    if (Program.cpu.deviceName.match(/CC13.2/)) {
        libName = "_cc13x2";
    }
    else if (Program.cpu.deviceName.match(/CC13.0/)) {
        libName = "_cc13x0";
    }
    else if (Program.cpu.deviceName.match(/CC26.0R2/)) {
        libName = "_cc26x0r2";
    }
    else if (Program.cpu.deviceName.match(/CC26.2/)) {
        libName = "_cc26x2";
    }
    else if (Program.cpu.deviceName.match(/CC26.0/)) {
        libName = "_cc26x0";
    }
    else if (Program.cpu.deviceName.match(/CC3220/)) {
        libName = "_cc32xx";
    }
    else if (Program.cpu.deviceName.match(/MSP432E4.*/)) {
        libName = "_msp432e4";
    }
    else if (Program.cpu.deviceName.match(/MSP432P4.1.I/)) {
        libName = "_msp432p4x1xi";
    }
    else if (Program.cpu.deviceName.match(/MSP432P4.1.T/)) {
        libName = "_msp432p4x1xt";
    }
    else if (Program.cpu.deviceName.match(/MSP432P401/)) {
        libName = "_msp432p401x";
    }
    else {
        throw ("DPL not found for this device " + Program.cpu.deviceName +
            " and target " + Program.build.target.suffix);
    }

    targetSuffix = ".a" + Program.build.target.suffix + ";";

    retString = dplBase + "/lib/dpl" + libName + targetSuffix;

    /* strip duplicate '//' characters from the library list */
    return (retString.replace(/\/\/+/g, '/'));
}
