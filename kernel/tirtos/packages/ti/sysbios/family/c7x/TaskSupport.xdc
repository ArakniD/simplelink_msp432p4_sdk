/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 *  ======== TaskSupport.xdc ========
 *
 */

package ti.sysbios.family.c7x;

import xdc.runtime.Assert;

/*!
 *  ======== TaskSupport ========
 *  C71 Task Support Module.
 */
module TaskSupport inherits ti.sysbios.interfaces.ITaskSupport
{
    /*!
     *  Default Task stack size.
     *
     *  The C7x CPU uses 2 stacks during interrupt processing.  The CPU
     *  automatically saves some context to the memory block to which the
     *  TCSP register points, and SW will save some context to the Task's
     *  SW stack during interrupt processing.  In order to keep specification
     *  and processing compatible across different CPU targets, SYS/BIOS
     *  uses a single stack memory block for both of these stacks (although
     *  there isn't really any stacking with TCSP since there is a distinct
     *  block for each Task which is used only once).  Due to HW alignment
     *  and size requirements for the TCSP block, there is a strict minimum
     *  of 16KB for the Task stack.
     */
    override readonly config SizeT defaultStackSize = 0x4000;

    /*!
     *  Required stack alignment (in MAUs).
     *
     *  The C7x CPU TCSP register requires this alignment (see
     *  {@link #defaultStackSize} for further information).
     */
    override readonly config UInt stackAlignment = 0x2000;

    /*! Asserted in TaskSupport_start */
    config Assert.Id A_stackSizeTooSmall = {
        msg: "A_stackSizeTooSmall: Task stack size must be >= 16KB."
    };
}
