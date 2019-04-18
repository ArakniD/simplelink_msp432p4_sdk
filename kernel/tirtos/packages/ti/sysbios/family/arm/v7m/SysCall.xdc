/*
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 *  ======== SysCall.xdc ========
 */

package ti.sysbios.family.arm.v7m;

import xdc.runtime.Error;

import ti.sysbios.interfaces.ISysCall;

/*!
 *  ======== SysCall ========
 *  System Call Manager
 */

@ModuleStartup      /* generate a call to startup function */

module SysCall inherits ti.sysbios.interfaces.ISysCall
{
    //  -------- Module Constants --------

    const UInt16 NUM_SYSCALLS = 3;

    //  -------- Module Types --------

    /*! Callback function type definition. */
    typedef Void (*FuncPtr)(UArg);

    //  -------- Module Errors --------

    /*!
     *  Error raised when a User Task attempts to switch privilege mode.
     *
     *  This error is raised by the system call handler if the caller's
     *  PC does not fall within the kernel wrapper function section. This
     *  typically happens if a User Task attempts to directly invoke a
     *  system call to switch the CPU's privileged mode.
     */
    config Error.Id E_requestFailed  = {
        msg: "E_requestFailed: Request (Svc #%d) to switch CPU privilege denied."
    };

    //  -------- Module Config --------

    /*!
     *  ======== requestFailedFunc ========
     *  Callback function for when a system call request is denied.
     *  Default is set to an internal function that raises an error.
     *
     *  If the validation check performed by the system call handler fails,
     *  the request is denied and this function is called.
     */
    config FuncPtr requestFailedFunc;

    //  -------- Module Functions --------

    @Macro
    override Void enterPrivMode();

    @Macro
    override Void enterUnprivMode();

    @Macro
    override Void restorePrivMode();

internal:

    /*
     *  ======== requestFailed ========
     */
    Void requestFailed(UArg svcNum);

    /*
     *  ======== enterPrivModeI ========
     */
    Void enterPrivModeI();

    /*
     *  ======== enterUnprivModeI ========
     */
    Void enterUnprivModeI();

    /*
     *  ======== restorePrivModeI ========
     */
    Void restorePrivModeI();

    /*
     *  ======== svcHandler ========
     */
    Void svcHandler();

    /*! Module state */
    struct Module_State {
        Ptr sysCallTable[];     // System call function table
    }
}
