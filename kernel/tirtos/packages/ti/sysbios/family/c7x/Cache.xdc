/*
 * Copyright (c) 2019, Texas Instruments Incorporated
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
 *  ======== Cache.xdc ========
 */

package ti.sysbios.family.c7x;

import xdc.rov.ViewInfo;

/*!
 *  ======== Cache ========
 *  Cache Module
 *
 *  This Cache module provides C66 family-specific implementations of the
 *  APIs defined in {@link ti.sysbios.interfaces.ICache ICache}.  It also
 *  provides additional C66 specific cache functions.
 *
 *  Unconstrained Functions
 *  All functions
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center"></colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th><th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                                                                                                                 -->
 *    <tr><td> {@link #disable}    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #enable}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #getMar*}    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #getMode*}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #getSize*}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #inv}        </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #invL1pAll*} </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #setMar*}    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #setMode*}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #setSize*}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wait}       </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wb}         </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wbAll*}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wbL1dAll}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wbInv}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wbInvAll}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wbInvL1dAll}</td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup after this module is started (e.g. Mod_Module_startupDone() returns TRUE). </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *             <li> In your module startup before this module is started (e.g. Mod_Module_startupDone() returns FALSE).</li>
 *           </ul>
 *       <li> <b>*</b>: These APIs are intended to be made at initialization time, but are not restricted to this. </li>
 *       </ul>
 *    </td></tr>
 *
 *  </table>
 *  @p
 */

module Cache inherits ti.sysbios.interfaces.ICache
{
    // -------- Module Types --------

    /*!
     *  ======== ModuleView ========
     *  @_nodoc
     */
    metaonly struct ModuleView {
        String  L1PCacheSize;
        String  L1PMode;
        String  L1DCacheSize;
        String  L1DMode;
        String  L2CacheSize;
        String  L2Mode;
    };

    /*!
     *  ======== rovViewInfo ========
     *  @_nodoc
     */
    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
                ['Module',
                    {
                        type: ViewInfo.MODULE,
                        viewInitFxn: 'viewInitModule',
                        structName: 'ModuleView'
                    }
                ],
            ]
        });

    /*! Lists of cache modes for L1/L2 caches */
    enum Mode {
        Mode_FREEZE,    /*! No new cache lines are allocated */
        Mode_BYPASS,    /*! All access result in long-distance access */
        Mode_NORMAL     /*! Normal operation of cache */
    };

    /*! Level 1 cache size type definition. Can be used for both L1D & L1P */
    enum L1Size {
        L1Size_0K = 0,  /*! Amount of cache is 0K, Amount of SRAM is 32K */
        L1Size_4K = 1,  /*! Amount of cache is 4K, Amount of SRAM is 28K */
        L1Size_8K = 2,  /*! Amount of cache is 8K, Amount of SRAM is 24K */
        L1Size_16K = 3, /*! Amount of cache is 16K, Amount of SRAM is 16K */
        L1Size_32K = 4  /*! Amount of cache is 32K, Amount of SRAM is 0K */
    };

    /*! Level 2 cache size type definition. */
    enum L2Size {
        L2Size_0K = 0,   /*! L2 is all SRAM */
        L2Size_32K = 1,  /*! Amount of cache is 32K */
        L2Size_64K = 2,  /*! Amount of cache is 64K */
        L2Size_128K = 3, /*! Amount of cache is 128K */
        L2Size_256K = 4, /*! Amount of cache is 256K */
        L2Size_512K = 5, /*! Amount of cache is 512K */
        L2Size_1024K = 6 /*! Amount of cache is 1024K */
    };

    const UInt32 PC  = 1;     /*! Permit Caching                     */
    const UInt32 WTE = 2;     /*! Write through enabled              */
    const UInt32 PCX = 4;     /*! Permit caching in external cache   */
    const UInt32 PFX = 8;     /*! Prefetchable by external engines   */

    /*! Structure for specifying all cache sizes. */
    struct Size {
        L1Size l1pSize;         /*! L1 Program cache size */
        L1Size l1dSize;         /*! L1 Data data size */
        L2Size l2Size;          /*! L2 cache size */
    };

    /*! Default sizes of caches.
     * @_nodoc
     */
    config Size initSize = {
        l1pSize: L1Size_32K,
        l1dSize: L1Size_32K,
        l2Size: L2Size_0K
    };

    /*! @_nodoc
     *
     *  This parameter is used to break up large blocks into multiple
     *  small blocks which are done atomically.  Each block of the
     *  specified size waits for the cache operation to finish before
     *  starting the next block.  Setting this size to 0, means the
     *  cache operations are not done atomically.
     */
    config UInt32 atomicBlockSize = 1024;

    /*!
     *  ======== disable ========
     *  Disables the 'type' cache(s)
     *
     *  Disabling of L2 cache is currently not supported.
     */
    override Void disable(Bits16 type);

    /*!
     *  ======== getSize ========
     *  Get sizes of all caches
     *
     *  @param(size)    pointer to structure of type Cache_Size
     */
    Void getSize(Size *size);

    /*!
     *  ======== setSize ========
     *  Set sizes of all caches
     *
     *  @param(size)    pointer to structure of type Cache_Size
     */
    Void setSize(Size *size);

    /*!
     *  ======== wbAll ========
     *  Write back all caches
     *
     *  Perform a global write back.  There is no effect on L1P cache.
     *  All cache lines are left valid in L1D cache and dirty lines in L1D cache
     *  are written back to L2 or external.  All cache lines are left valid in
     *  L2 cache and dirty lines in L2 cache are written back to external.
     *  This function does not wait for write back operation to perculate
     *  through the whole memory system before returing. Call Cache_wait(),
     *  after this function if necessary.
     */
    override Void wbAll();

    /*!
     *  ======== wbL1dAll ========
     *  Write back L1D cache
     *
     *  Perform a global write back of L1D cache. There is no effect on L1P
     *  or L2 cache.  All cache lines are left valid in L1D cache and the
     *  dirty lines in L1D cache are written back to L2 or external.
     *  This function does not wait for write back operation to perculate
     *  through the whole memory system before returing. Call Cache_wait(),
     *  after this function if necessary.
     */
    Void wbL1dAll();

    /*!
     *  ======== wbInvAll ========
     *  Write back invalidate all caches
     *
     *  Performs a global write back and invalidate.  All cache lines are
     *  invalidated in L1P cache.  All dirty cache lines are written back to L2
     *  or external and then invalidated in L1D cache.  All dirty cache lines
     *  are written back to external and then invalidated in L2 cache.
     *  This function does not wait for write back operation to perculate
     *  through the whole memory system before returing. Call Cache_wait(),
     *  after this function if necessary.
     */
    override Void wbInvAll();

    /*!
     *  ======== wbInvL1dAll ========
     *  Write back invalidate L1D cache
     *
     *  Performs a global write back and invalidate of L1D cache.
     *  All dirty cache lines are written back to L2 or
     *  external and then invalidated in L1D cache.
     *  This function does not wait for write back operation to perculate
     *  through the whole memory system before returing. Call Cache_wait(),
     *  after this function if necessary.
     */
    Void wbInvL1dAll();

    Void setL2CFG(UInt size);
    ULong getL2CFG();
    Void setL1DCFG(UInt size);
    ULong getL1DCFG();
    Void setL2WB(UInt flag);
    Void setL2WBINV(UInt flag);
    Void setL1DWB(UInt flag);
    Void setL1DWBINV(UInt flag);

internal:

    /*
     *  ======== startup ========
     *  startup function to enable cache early during climb-up
     */
    Void startup();
}
