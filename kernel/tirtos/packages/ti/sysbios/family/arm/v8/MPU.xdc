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
 *  ======== MPU.xdc ========
 */

package ti.sysbios.family.arm.v8;

import xdc.rov.ViewInfo;

/*!
 *  ======== MPU ========
 *  Memory Protection Unit (MPU) Manager.
 *
 *  This module manages the Memory Protect Unit (MPU) present in ARMv8
 *  Cortex-M devices. It enables the application to partition the memory
 *  into different regions and set protection attributes for each region.
 *
 *  The number of memory regions supported is device specific and may vary
 *  on different devices.
 *
 *  Programming a memory region requires specifying the base address and
 *  ending address of the region, and the region's protection attributes.
 *  The protection attributes for each region include attributes such as
 *  memory type (device or normal), shareability, cacheability and
 *  read-write access permission.
 *
 *  @a(Memory region attributes)
 *  Memory regions can be configured as different memory types by setting
 *  the {@link #RegionAttrs shareable}, {@link #RegionAttrs accessPerm},
 *  {@link #RegionAttrs executable} and {@link #RegionAttrs attrIndx}
 *  fields of the {@link #RegionAttrs} structure which is passed as an
 *  argument to {@link #setRegion MPU_setRegion()} function.
 *
 *  The two memory types supported by the hardware are "Normal" (cacheable or
 *  non-cacheable) and "Device" memory (gathering, reordering and/or early
 *  acknowledgement). "Device" memory type is recommended for mapping
 *  peripheral address space like memory-mapped registers. This type ensures
 *  that the memory accesses to the peripheral memory are not performed
 *  speculatively, are not repeated, and are always performed in order. The
 *  "Normal" memory type is recommended for mapping memory regions storing
 *  application code and data. The memory type is encoded in a Memory Attribute
 *  Indirection Register (MAIR) and an index to it (see
 *  {@link #RegionAttrs attrIndx}) is passed to
 *  {@link #setRegion MPU_setRegion()} function.
 *
 *  SYS/BIOS has 8 MAIR config params (MAIR0, MAIR2, ...) that are
 *  initialized to default value. In order to have a custom memory
 *  attribute, a user can either change the MAIRn config param in
 *  the application's cfg script or call the {@link #setMAIR MPU_setMAIR()}
 *  runtime API.
 *
 *  For more details on MAIR encodings please refer v8M ARM Architecture
 *  Reference Manual.
 *
 *  @a(Changing shareability attributes of a cacheable memory region)
 *  If changing the shareability attribute of a cacheable memory region,
 *  it is possible for coherency problems to arise. In order to avoid possible
 *  coherency errors, the below sequence should be followed to change the
 *  shareability attributes of the memory region:
 *  - Make the memory region Non-cacheable and outer-shareable
 *  - Clean and invalidate the memory region from the cache
 *  - Change the shareability attribute to the desired value
 *
 *  @a(Examples)
 *  Example showing how to set attributes for a given memory region using
 *  *.cfg script:
 *
 *  @p(code)
 *  var MPU = xdc.useModule('ti.sysbios.family.arm.v8.MPU');
 *  MPU.enableMPU = true;
 *
 *  // Mark memory region as normal outer and inner write-back
 *  // cacheable
 *  var attrs = new MPU.RegionAttrs();
 *  MPU.initRegionAttrsMeta(attrs);
 *  attrs.enable = true;
 *  attrs.shareable = MPU.Shareable_NONE;
 *  attrs.executable = false;
 *  attrs.accessPerm = MPU.AccessPerm_RW_ANY;
 *  attrs.attrIndx = 7; // Use MAIR 7 which has a default of outer and inner
 *                      // write-back cacheable
 *
 *  // Set attributes for memory region of size 32KB starting at
 *  // address 0x20000000 using MPU region Id 0 to store the attributes.
 *  MPU.setRegionMeta(0, 0x20000000, 32768, attrs);
 *  @p
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center">
 *    </colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th>
 *    <th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                               -->
 *    <tr><td> {@link #disable}     </td><td>   Y    </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #enable}      </td><td>   Y    </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #initRegionAttrs}  </td><td>   Y    </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #isEnabled}  </td><td>   Y    </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #setMAIR}  </td><td>   Y    </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #setRegion}  </td><td>   Y    </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
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
module MPU
{
    // -------- ROV views --------

    /*! @_nodoc */
    metaonly struct RegionAttrsView {
        UInt8       RegionId;
        Bool        Enabled;
        String      BaseAddress;
        String      EndAddress;
        String      Shareable;
        Bool        Executable;
        String      AccessPerm;
    };

    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
                ['MpuRegionAttrsView', {
                    type: ViewInfo.MODULE_DATA,
                    viewInitFxn: 'viewMpuRegionAttrs',
                    structName: 'RegionAttrsView'
                }],
           ]
       });

    /*!
     *  Memory Protection Unit (MPU) registers. Symbol "MPU_deviceRegs" is
     *  a physical device
     */
    struct DeviceRegs {
        UInt32 TYPE;            /*! 0xD90 Type Register                      */
        UInt32 CTRL;            /*! 0xD94 Control Register                   */
        UInt32 RNR;             /*! 0xD98 Region Register                    */
        UInt32 RBAR;            /*! 0xD9C Region Base Address Register       */
        UInt32 RLAR;            /*! 0xDA0 Region Base Limit Register         */
        UInt32 RBAR_A1;         /*! 0xDA4 MPU RBAR Alias 1                   */
        UInt32 RLAR_A1;         /*! 0xDA8 MPU RLAR Alias 1                   */
        UInt32 RBAR_A2;         /*! 0xDAC MPU RBAR Alias 2                   */
        UInt32 RLAR_A2;         /*! 0xDB0 MPU RLAR Alias 2                   */
        UInt32 RBAR_A3;         /*! 0xDB4 MPU RBAR Alias 3                   */
        UInt32 RLAR_A3;         /*! 0xDB8 MPU RLAR Alias 3                   */
        UInt32 res0;            /*! 0xDBC Reserved                           */
        UInt32 MAIR0;           /*! 0xDC0 MAIR0                              */
        UInt32 MAIR1;           /*! 0xDC4 MAIR1                              */
    };

    extern volatile DeviceRegs deviceRegs;

    /*!
     *  ======== AccessPerm ========
     *  Access Permissions
     */
    enum AccessPerm {
        AccessPerm_RW_PRIV = 0x0,   /*! Read/write by privileged code only */
        AccessPerm_RW_ANY = 0x1,    /*! Read/write by any privilege level  */
        AccessPerm_RO_PRIV = 0x2,   /*! Read only by privileged code only  */
        AccessPerm_RO_ANY = 0x3     /*! Read only by any privilege level   */
    };

    /*!
     *  ======== Shareable ========
     *  Shareability attribute
     */
    enum Shareable {
        Shareable_NONE = 0x0,
        Shareable_OUTER = 0x2,
        Shareable_INNER = 0x3
    };

    /*!
     *  ======== RegionAttrs ========
     *  Structure for setting protection attributes of a MPU region
     */
    struct RegionAttrs {
        Bool       enable;           /*! Is MPU region enabled              */
        Shareable  shareable;        /*! Memory region shareability         */
        Bool       executable;       /*! Is memory region executable        */
        AccessPerm accessPerm;       /*! Access permission                  */
        UInt8      attrIndx;         /*! Memory attributes index field 0-7  */
    };

    /*!
     *  ======== defaultAttrs ========
     *  Default region attributes structure
     *
     *  The default attributes structure marks the memory region as outer and
     *  inner non-cacheable and non-shareable, with read-write access from
     *  privileged code only.
     */
    config RegionAttrs defaultAttrs = {
        enable: true,                   /* true by default                  */
        shareable: Shareable_NONE,      /* Shareable_NONE by default        */
        executable: true,               /* true by default                  */
        accessPerm: AccessPerm_RW_PRIV, /* allow read/write access from
                                           privileged code only             */
        attrIndx: 0                     /* MAIR 0 by default (MAIR 0 is set
                                           to attribute for strongly ordered
                                           device memory)                   */
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
     *  Assert raised when an invalid region number is passed to MPU_setRegion()
     */
    config xdc.runtime.Assert.Id A_invalidRegionId  = {
        msg: "A_invalidRegionId: MPU Region number passed is invalid."
    };

    /*!
     *  ======== A_unalignedBaseAddr ========
     *  Assert raised when region's base address is not aligned.
     */
    config xdc.runtime.Assert.Id A_unalignedBaseAddr  = {
        msg: "A_unalignedBaseAddr: MPU region base address not aligned."
    };

    /*!
     *  ======== A_unalignedEndAddr ========
     *  Assert raised when region's end address is not aligned.
     */
    config xdc.runtime.Assert.Id A_unalignedEndAddr  = {
        msg: "A_unalignedEndAddr: MPU region end address not aligned."
    };

    /*!
     *  ======== enableMPU ========
     *  Configuration parameter to enable MPU. Disabled by default.
     */
    config Bool enableMPU = false;

    /*!
     *  ======== enableBackgroundRegion ========
     *  Configuration parameter to enable background region.
     *
     *  If the MPU is enabled and background region is also enabled, any
     *  privileged access that does not map to any MPU memory region is
     *  handled using the default memory map.
     *
     *  @p(blist)
     *      -See the "Protected Memory System Architecture (PMSA)" chapter
     *       in the ARM v8M Architecture Reference Manual for more info on
     *       the default memory map.
     *  @p
     */
    config Bool enableBackgroundRegion = true;

    /*!
     *  ======== MAIR0 ========
     *  Memory attribute 0.
     *
     *  Default is memory with non-gathering, non-reordering and no early write
     *  acknowledegement property.
     */
    config UInt8 MAIR0 = 0x00;

    /*!
     *  ======== MAIR1 ========
     *  Memory attribute 1
     *
     *  Default is memory with non-gathering, non-reordering and early write
     *  acknowledegement property.
     */
    config UInt8 MAIR1 = 0x04;

    /*!
     *  ======== MAIR2 ========
     *  Memory attribute 2
     *
     *  Default is memory with non-gathering, reordering and early write
     *  acknowledegement property.
     */
    config UInt8 MAIR2 = 0x08;

    /*!
     *  ======== MAIR3 ========
     *  Memory attribute 3
     *
     *  Default is memory with gathering, reordering and early write
     *  acknowledegement property.
     */
    config UInt8 MAIR3 = 0x0C;

    /*!
     *  ======== MAIR4 ========
     *  Memory attribute 4
     *
     *  Default is normal outer & inner non-cacheable memory.
     */
    config UInt8 MAIR4 = 0x44;

    /*!
     *  ======== MAIR5 ========
     *  Memory attribute 5
     *
     *  Default is normal outer non-cacheable, inner write-back cacheable
     *  non-transient memory.
     */
    config UInt8 MAIR5 = 0x4F;

    /*!
     *  ======== MAIR6 ========
     *  Memory attribute 6
     *
     *  Default is normal outer & inner write-through cacheable non-transient
     *  memory.
     */
    config UInt8 MAIR6 = 0xBB;

    /*!
     *  ======== MAIR7 ========
     *  Memory attribute 7
     *
     *  Default is normal outer and inner write-back cacheable non-transient
     *  memory.
     */
    config UInt8 MAIR7 = 0xFF;

    /*!
     *  @_nodoc
     *  ======== numRegions ========
     *  Number of MPU regions. Default is determined based on device type.
     */
    config UInt8 numRegions;

    /*!
     *  ======== initRegionAttrsMeta ========
     *  Initializes the region attribute structure
     *
     *   @param(attrs)          Pointer to region attribute struct
     */
    metaonly Void initRegionAttrsMeta(RegionAttrs *regionAttrs);

    /*!
     *  ======== setRegionMeta ========
     *  Statically sets the MPU region attributes
     *
     *  @see ti.sysbios.family.arm.r5.MPU
     *
     *  @param(regionId)        MPU region number
     *  @param(regionBaseAddr)  MPU region base address
     *  @param(regionEndAddr)   MPU region end address
     *  @param(attrs)           Protection attributes
     */
    metaonly Void setRegionMeta(UInt8 regionId, Ptr regionBaseAddr,
        Ptr regionEndAddr, RegionAttrs attrs);

    /*!
     *  ======== disable ========
     *  Disables the MPU.
     *
     *  If the MPU is already disabled, then simply return.
     */
    Void disable();

    /*!
     *  ======== enable ========
     *  Enables the MPU.
     *
     *  If the MPU is already enabled, then simply return.
     */
    Void enable();

    /*!
     *  ======== initRegionAttrs() ========
     *  Initializes the region attribute structure
     *
     *  @param(attrs)           Pointer to region attribute struct
     */
    Void initRegionAttrs(RegionAttrs *regionAttrs);

    /*!
     *  ======== isEnabled ========
     *  Determines if the MPU is enabled
     */
    Bool isEnabled();

    /*!
     *  ======== setMAIR ========
     *  Sets the memory attribute encoding in the MAIRn register.
     *
     *  MAIR provides the memory attribute encodings to the possible
     *  {@link #RegionAttrs attrIndx} values in a MPU region entry.
     *
     *  {@link #RegionAttrs attrIndx}[2:0] selects the ATTRn bit-field in
     *  the MAIR register.
     *
     *  Memory Attribute Indirection Register (MAIR) bit assignments:
     *  @p(code)
     *        |31     |    24|23     |     16|15     |      8|7      |      0|
     *         --------------------------------------------------------------
     *  MAIR0 |     ATTR3    |     ATTR2     |     ATTR1     |     ATTR0     |
     *         --------------------------------------------------------------
     *  MAIR1 |     ATTR7    |     ATTR6     |     ATTR5     |     ATTR4     |
     *         --------------------------------------------------------------
     *  @p
     *
     *  SYS/BIOS has 8 MAIR config params (MAIR0, MAIR2, ...) that are
     *  initialized to default value. In order to have a custom memory
     *  attribute, a user can either change the MAIRn config param in
     *  the application's cfg script or call this runtime API.
     *
     *  For more details on MAIR encodings please refer v8M ARM Architecture
     *  Reference Manual.
     *
     *  @param(attrIndx)     Memory attribute index
     *  @param(attr)         Memory attribute encoding
     */
    Void setMAIR(UInt8 attrIndx, UInt8 attr);

    /*!
     *  ======== setRegion ========
     *  Sets the MPU region attributes
     *
     *  @see ti.sysbios.family.arm.v8.MPU
     *
     *  @param(regionId)        MPU region number
     *  @param(regionBaseAddr)  MPU region base address
     *  @param(regionEndAddr)   MPU region end address
     *  @param(attrs)           Protection attributes
     */
    Void setRegion(UInt8 regionId, Ptr regionBaseAddr,
        Ptr regionEndAddr, RegionAttrs *attrs);

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
     *  Array to hold statically configured MPU region entries
     */
    config RegionEntry regionEntry[];

    /*
     *  ======== startup ========
     *  startup function to initialize MPU module
     */
    Void startup();

    /*! Module state */
    struct Module_State {
        RegionEntry regionEntry[];  // Array to hold a copy of MPU region
                                    // settings
    }
}
