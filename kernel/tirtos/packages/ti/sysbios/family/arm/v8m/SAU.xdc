/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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
 *  ======== SAU.xdc ========
 */

package ti.sysbios.family.arm.v8m;

import xdc.rov.ViewInfo;

/*!
 *  ======== SAU ========
 *  Security Attribution Unit (SAU) Manager.
 *
 *  This module manages the Security Attribution Unit (SAU) present in ARMv8
 *  Cortex-V8M devices. It enables the application to partition the memory
 *  into different regions and set security attributes for each region.
 *
 *  The number of memory regions supported is device specific and may vary
 *  on different devices.
 *
 *  @a(Examples)
 *  Example showing how to set attributes for a given memory region using
 *  *.cfg script:
 *
 *  @p(code)
 *  var SAU = xdc.useModule('ti.sysbios.family.arm.v8m.SAU');
 *  SAU.enableSAU = true;
 *
 *  // define NSC region
 *  SAU.configureRegionMeta(0, 0x2C410000, 0x00010000, SAU.RegionType_NSC);
 *  // define NS code/data region
 *  SAU.configureRegionMeta(1, 0x2C420000, 0x00020000, SAU.RegionType_NS);
 *  // allow NS access to SCB
 *  SAU.configureRegionMeta(2, 0xE0000000, 0x10000000, SAU.RegionType_NS);
 *  @p
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center">
 *    </colgroup>
 *
 *    <tr>        <th> Function </th>
 *    <th> Hwi </th><th> Swi </th><th> Task </th><th> Main </th><th> Startup </th></tr>
 *    <tr><td> {@link #configureRegion} </td>
 *    <td>  Y  </td><td>  Y  </td><td>  Y   </td><td>  Y   </td><td>   Y   </td></tr>
 *    <tr><td> {@link #disable} </td>
 *    <td>  Y  </td><td>  Y  </td><td>  Y   </td><td>  Y   </td><td>   Y   </td></tr>
 *    <tr><td> {@link #disableRegion} </td>
 *    <td>  Y  </td><td>  Y  </td><td>  Y   </td><td>  Y   </td><td>   Y   </td></tr>
 *    <tr><td> {@link #enable} </td>
 *    <td>  Y  </td><td>  Y  </td><td>  Y   </td><td>  Y   </td><td>   Y   </td></tr>
 *    <tr><td> {@link #enableRegion} </td>
 *    <td>  Y  </td><td>  Y  </td><td>  Y   </td><td>  Y   </td><td>   Y   </td></tr>
 *    <tr><td> {@link #isEnabled} </td>
 *    <td>  Y  </td><td>  Y  </td><td>  Y   </td><td>  Y   </td><td>   Y   </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup. </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *             <li> In your module startup.</li>
 *           </ul>
 *       </ul>
 *    </td></tr>
 *
 *  </table>
 *  @p
 */

@DirectCall
module SAU
{
    // -------- ROV views --------

    /*! @_nodoc */
    metaonly struct RegionsView {
        UInt8       RegionIdx;
        Bool        Enabled;
        String      BaseAddress;
        String      EndAddress;
        String      RegionType;
    };

    /*! @_nodoc */
    metaonly struct RegsView {
        UInt8       SREGION;
        Bool        ENABLE;
        String      ALLNS;
        Ptr         SFSR;
        Ptr         SFAR;
    };

    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
                [
                    'SauRegions', {
                        type: ViewInfo.MODULE_DATA,
                        viewInitFxn: 'viewSauRegions',
                        structName: 'RegionsView'
                    }
                ],
                [
                    'SauRegisters', {
                        type: ViewInfo.MODULE_DATA,
                        viewInitFxn: 'viewSauRegisters',
                        structName: 'RegsView'
                    }
                ],
           ]
       });

    /*!
     *  Security Attribution Unit (SAU) registers. Symbol "SAU_deviceRegs" is
     *  the physical device
     */
    struct DeviceRegs {
        UInt32 CTRL;            /*! 0xE000EDD0 Control Register                   */
        UInt32 TYPE;            /*! 0xE000EDD4 Type Register                      */
        UInt32 RNR;             /*! 0xE000EDD8 Region Number Register             */
        UInt32 RBAR;            /*! 0xE000EDDC Region Base Address Register       */
        UInt32 RLAR;            /*! 0xE000EDE0 Region Base Limit Register         */
        UInt32 SFSR;            /*! 0xE000EDE4 Secure Fault Status Register       */
        UInt32 SFAR;            /*! 0xE000EDE8 Secure Fault Address Register      */
    };

    /*! @_nodoc global SAU_deviceRegs struct */
    extern volatile DeviceRegs deviceRegs;

    /*!
     *  ======== RegionType ========
     */
    enum RegionType {
        RegionType_NS,              /*! Region is Non-Secure */
        RegionType_NSC              /*! Region is Secure, Non-Secure Callable */
    };

    /*!
     *  ======== A_nullPointer ========
     *  Assert raised when a pointer is null
     */
    config xdc.runtime.Assert.Id A_nullPointer  = {
        msg: "A_nullPointer: Pointer is null"
    };

    /*!
     *  ======== A_invalidRegionId ========
     *  Assert raised when an invalid region number is passed to
     *  SAU_configureRegion(), SAU_enableRegion(), SAU_disableRegion()
     */
    config xdc.runtime.Assert.Id A_invalidRegionId  = {
        msg: "A_invalidRegionId: SAU Region number passed is invalid."
    };

    /*!
     *  ======== A_unalignedBaseAddr ========
     *  Assert raised when region's base address is not aligned.
     */
    config xdc.runtime.Assert.Id A_unalignedBaseAddr  = {
        msg: "A_unalignedBaseAddr: SAU region base address not aligned."
    };

    /*!
     *  ======== A_unalignedEndAddr ========
     *  Assert raised when region's end address is not aligned.
     */
    config xdc.runtime.Assert.Id A_unalignedEndAddr  = {
        msg: "A_unalignedEndAddr: SAU region end address not aligned."
    };

    /*!
     *  ======== enableSAU ========
     *  Configuration parameter to enable SAU. Disabled by default.
     */
    config Bool enableSAU = false;

    /*!
     *  ======== allSecureWhenSauDisabled ========
     *  Configuration parameter to set all memory type when SAU is disabled
     *
     *  If the SAU is disabled, this parameter determines if all of
     *  memory is configured as Secure or Non Secure.
     *
     *  Default is all memory is marked as Secure if the SAU is disabled.
     */
    config Bool allSecureWhenSauDisabled = true;

    /*!
     *  @_nodoc
     *  ======== numRegions ========
     *  Number of SAU regions. Default is determined based on device type.
     */
    config UInt8 numRegions;

    /*!
     *  ======== configureRegionMeta ========
     *  Statically sets the SAU region attributes
     *
     *  @see ti.sysbios.family.arm.r5.SAU
     *
     *  By default, the region is enabled.
     *  Call disableRegionMeta(regionId) to disable the region.
     *
     *  @param(regionId)        SAU region number
     *  @param(regionBase)      SAU region base address
     *  @param(regionSize)      SAU region size in bytes
     *  @param(regionType)      RegionType
     */
    metaonly Void configureRegionMeta(UInt8 regionId, Ptr regionBase,
        SizeT regionSize, RegionType regionType);

    /*!
     *  ======== disableRegionMeta ========
     *  disable a SAU region
     *
     *  @param(regionId)        SAU region number
     */
    metaonly Void disableRegionMeta(UInt8 regionId);

    /*!
     *  ======== disable ========
     *  Disables the SAU.
     *
     *  If the SAU is already disabled, then simply return.
     */
    Void disable();

    /*!
     *  ======== enable ========
     *  Enables the SAU.
     *
     *  If the SAU is already enabled, then simply return.
     */
    Void enable();

    /*!
     *  ======== isEnabled ========
     *  Determines if the SAU is enabled
     */
    Bool isEnabled();

    /*!
     *  ======== configureRegion ========
     *  Configure a SAU region
     *
     *  @see ti.sysbios.family.arm.v8.SAU
     *
     *  By default, the region is enabled.
     *  Call disableRegion(regionId) to disable the region.
     *
     *  @param(regionId)        SAU region number
     *  @param(regionBase)      SAU region base address
     *  @param(regionSize)      SAU region size in bytes
     *  @param(regionType)      RegionType (NS/NSC)
     */
    Void configureRegion(UInt8 regionId, Ptr regionBase,
        SizeT regionSize, RegionType regionTupe);

    /*!
     *  ======== enableRegion ========
     *  Enable a SAU region
     *
     *  @param(regionId)        SAU region number
     */
    Void enableRegion(UInt8 regionId);

    /*!
     *  ======== disableRegion ========
     *  Disable a SAU region
     *
     *  @param(regionId)        SAU region number
     */
    Void disableRegion(UInt8 regionId);

internal:

    /*
     *  ======== RegionEntry ========
     */
    struct RegionEntry {
        UInt32 rbar;
        UInt32 rlar;
    };

    /*
     *  ======== regionEntry ========
     *  Array to hold statically configured SAU region entries
     */
    metaonly config RegionEntry regionEntry[];

    /*
     *  ======== startup ========
     *  startup function to initialize SAU module
     */
    Void startup();

    /*! Module state */
    struct Module_State {
        volatile RegionEntry regionEntry[];  // Array to hold a copy of SAU region
                                             // settings
    }
}
