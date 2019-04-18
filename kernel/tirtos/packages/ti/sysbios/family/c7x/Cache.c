/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
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
 *  ======== Cache.c ========
 */
#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Startup.h>
#include <ti/sysbios/hal/Hwi.h>

#include <c6x_migration.h>
#include <c7x.h>

#include "package/internal/Cache.xdc.h"

#define L2CFG_MODE_MASK         0x00000007      /* L2 Mode mask */

/*
 *  ======== Cache_startup ========
 *  Sets the default cache size and MAR register values.
 */
Void Cache_startup()
{
    Cache_setSize((Cache_Size *)&(Cache_initSize));
}

/*
 *  ======== Cache_enable ========
 *  Set the size to the Cache_initSize.
 */
Void Cache_enable(Bits16 type)
{
    Cache_Size size;

    Cache_getSize(&size);

    if (type == Cache_Type_L1D) {
        size.l1dSize = Cache_initSize.l1dSize;
        Cache_setSize(&size);
    }
    else if (type == Cache_Type_L2) {
        size.l2Size = Cache_initSize.l2Size;
        Cache_setSize(&size);
    }
}

/*
 *  ======== Cache_disable ========
 *  For L1P and L1D set the size to 0.
 *  This API should not be called to disable L2 Cache.
 *  To disable L2 use Cache_setSize().
 */
Void Cache_disable(Bits16 type)
{
    Cache_Size size;

    Cache_getSize(&size);

    if (type == Cache_Type_L1D) {
        size.l1dSize = Cache_L1Size_0K;
        Cache_setSize(&size);
    }
    else if (type == Cache_Type_L2) {
        size.l2Size = Cache_L2Size_0K;
        Cache_setSize(&size);
    }
}

/*
 *  ======== Cache_setSize ========
 *  Set the L1D, L1P, and L2 cache sizes
 */
Void Cache_setSize(Cache_Size *size)
{
    UInt        mask;

    /* critical section -- disable interrupts */
    mask = Hwi_disable();

    /*
     *  Set size of L2 cache.
     *  Read back CFG, this stalls CPU until the mode change completes.
     */
    Cache_setL2CFG(size->l2Size);

    Cache_getL2CFG();

    /*
     *  Set size of L1D cache.
     *  Set size of L1P cache.
     *  Read back CFG, this stalls CPU until the mode change completes.
     */
    Cache_setL1DCFG(size->l1dSize);

    Cache_getL1DCFG();

    /* end of critical section -- restore interrupts */
    Hwi_restore(mask);
}

/*
 *  ======== Cache_getSize ========
 *  Returns the L1D, L1P, and L2 cache sizes.
 *  If value of L2 size is greater than Cache_L2_1024K then return
 *  Cache_L2_1024K.
 *  If value of L1 size is greater than Cache_L1_32K then return
 *  Cache_L1_32K.
 *  If value of L1 size is less than Cache_L1_8K then return
 *  Cache_L1_8K.
 */
Void Cache_getSize(Cache_Size *size)
{
    UInt        tmpSize;

    /*
     *  Read the L2 CFG register
     *  Return Cache_L2Size_1024K if register value is equal or greater than
     *  Cache_L2Size_1024K. This is the largest size defined in the .xdc file.
     */
    tmpSize = Cache_getL2CFG() & L2CFG_MODE_MASK;
    if (tmpSize > Cache_L2Size_1024K) {
        tmpSize = Cache_L2Size_1024K;
    }
    size->l2Size = (Cache_L2Size)tmpSize;

    /*
     *  Read the L1D register
     *  Its possible the register value is greater than 4 in which case
     *  we simply return 4 since values greater than 4 is equivalent to 4.
     *  Its possible the register value is less than 2 in which case
     *  we simply return 2 since values less than 2 is equivalent to 2.
     */
    tmpSize = Cache_getL1DCFG();
    if (tmpSize > Cache_L1Size_32K) {
        tmpSize = Cache_L1Size_32K;
    }
    else if (tmpSize < Cache_L1Size_8K) {
        tmpSize = Cache_L1Size_8K;
    }
    size->l1dSize = (Cache_L1Size)tmpSize;

    /*
     * L1P is a non-configurable 32KB cache.
     */
    size->l1pSize = Cache_L1Size_32K;
}

/*
 *  ======== Cache_wbAll ========
 *  Perform a global write back.  There is no effect on L1P cache.  All cache
 *  lines are left valid in L1D cache and the data in L1D cache is written
 *  back L2 or external.  All cache lines are left valid in L2 cache and the
 *  data in L2 cache is written back to external.
 */
Void Cache_wbAll()
{
    Cache_setL2WB(1);
}

/*
 *  ======== Cache_wbL1dAll ========
 *  Performs a global write back of L1D cache
 */
Void Cache_wbL1dAll()
{
    Cache_setL1DWB(1);
}

/*
 *  ======== Cache_wbInvAll ========
 *  Performs a global write back and invalidate.  All cache lines are
 *  invalidated in L1P cache.  All cache lines are written back to L2 or
 *  or external then invalidated in L1D cache.  All cache lines are
 *  written back to external and then invalidated in L2 cache.
 */
Void Cache_wbInvAll()
{
    Cache_setL2WBINV(1);
}

/*
 *  ======== Cache_wbInvL1dAll ========
 *  Performs a global write back then invalidate of L1D cache
 */
Void Cache_wbInvL1dAll()
{
    Cache_setL1DWBINV(1);
}

/*
 *  ======== Cache_inv ========
 *  Invalidate the range of memory within the specified starting address and
 *  byte count.  The range of addresses operated on gets quantized to whole
 *  cache lines in each cache.  All cache lines in range are invalidated in L1P
 *  cache.  All cache lines in range are invalidated in L1D cache.
 *  All cache lines in range are invaliated in L2 cache.
 */
Void Cache_inv(Ptr blockPtr, SizeT byteCnt, Bits16 type, Bool wait)
{
    __se_cache_op((void *)blockPtr, __DCICNS, byteCnt);

    if (wait) {
        Cache_wait();
    }
}

/*
 *  ======== Cache_wb ========
 *  Writes back the range of memory within the specified starting address
 *  and byte count.  The range of addresses operated on gets quantized to
 *  whole cache lines in each cache.  There is no effect on L1P cache.
 *  All cache lines within the range are left valid in L1D cache and the data
 *  within the range in L1D cache will be written back to L2 or external.
 *  All cache lines within the range are left valid in L2 cache and the data
 *  within the range in L2 cache will be written back to external.
 */
Void Cache_wb(Ptr blockPtr, SizeT byteCnt, Bits16 type, Bool wait)
{
    __se_cache_op((void *)blockPtr, __DCCCNS, byteCnt);

    if (wait) {
        Cache_wait();
    }
}

/*
 *  ======== Cache_wbInv ========
 *  Writes back and invalidates the range of memory within the specified
 *  starting address and byte count.  The range of addresses operated on gets
 *  quantized to whole cache lines in each cache.  All cache lines within range
 *  are invalidated in L1P cache.  All cache lines within the range are
 *  written back to L2 or external and then invalidated in L1D cache
 *  All cache lines within the range are written back to external and then
 *  invalidated in L2 cache.
 */
Void Cache_wbInv(Ptr blockPtr, SizeT byteCnt, Bits16 type, Bool wait)
{
    __se_cache_op((void *)blockPtr, __DCCICNS, byteCnt);

    if (wait) {
        Cache_wait();
    }
}

/*
 *  ======== Cache_wait ========
 *  Wait for the L2 count to complete.  This function needs only to wait
 *  for L2 word count since all block cache operations in BIOS are done
 *  through the L2 registers and all global cache operations must already
 *  wait until the operation completes.  Note:  Its sufficient to wait
 *  on one of the L2 count registers since all 3 count registers are
 *  mirrors of one another and map to the same bits.
 */
Void Cache_wait()
{
    /*
     *  Stall CPU while memory system is busy.
     */
    _mfence();

    /* do a 2nd mfence as per single mfence silicon errata */
    _mfence();
}
