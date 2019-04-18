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
 *  ======== MemProtect.c ========
 */

#include <xdc/std.h>

#include <ti/sysbios/family/arm/MPU.h>
#include <ti/sysbios/hal/MemProtect.h>

#define MPU_REGION_MASK             0xF
#define MPU_VALID                   0x10
#define MPU_ENABLE                  0x1

#define MemProtect_USER_FLAGS_MASK  0x0000000F
#define MemProtect_PRIV_FLAGS_MASK  0x000000F0
#define MemProtect_MEM_FLAGS_MASK   0x0FFFFF00

#define MemProtect_parseFlags       ti_sysbios_hal_MemProtect_parseFlags
#define MemProtect_programMpuEntry  ti_sysbios_hal_MemProtect_programMpuEntry

extern Ptr _privileged_data_begin;
extern Ptr _privileged_data_end;
extern Ptr _privileged_bss_begin;
extern Ptr _privileged_bss_end;
extern Ptr _kernel_wrapper_funcs_begin;
extern Ptr _kernel_wrapper_funcs_end;
extern Ptr _public_code_begin;
extern Ptr _public_code_end;
extern Ptr _public_data_begin;
extern Ptr _public_data_end;
extern Ptr _public_bss_begin;
extern Ptr _public_bss_end;

UInt32 MemProtect_parseFlags(UInt32 flags);
Void MemProtect_programMpuEntry(UInt8 id, UInt32 begin, UInt32 end,
    UInt32 flags);

Int MemProtect_constructDomain(MemProtect_Struct *obj, MemProtect_Acl *acl,
    UInt16 aclLength)
{
    UInt8  i;
    SizeT  length;
    Ptr    baseAddress;
    UInt32 flags, regionAttrs, regionSize;

    /* Validate obj base address is in privileged address space */
    if ((obj == NULL) || (acl == NULL) ||
        (MemProtect_isDataInKernelSpace((Ptr)obj,
         sizeof(MemProtect_Struct)) != TRUE)) {
        return (-1);
    }

    if (aclLength > MemProtect_NUM_USER_ACL_ENTRIES) {
        return (-2);
    }

    for (i = 0; i < aclLength; i++) {
        /* Validate ACL entry */
        // TODO enumerate return codes

        /* Read user supplied ACL entry */
        baseAddress = acl[i].baseAddress;
        length = acl[i].length;

        /* verify base address is a multiple of length */
        if (((UInt32)baseAddress & (length - 1)) != 0) {
            return (-3);
        }

        flags = acl[i].flags;

#if defined(__TI_COMPILER_VERSION__)
        regionSize = (30 - __clz(length)) << 1;
#endif

        regionAttrs = MemProtect_parseFlags(flags);
        if (regionAttrs == (~0)) {
            return (-4);
        }

        /* Validate address range falls in unprivileged address space */
        // TODO

        obj->acl[i].rbar = (UInt32)baseAddress | MPU_VALID |
            (i & MPU_REGION_MASK);
        obj->acl[i].rasr = regionAttrs | regionSize | MPU_ENABLE;
    }
    for (; i < MemProtect_NUM_USER_ACL_ENTRIES; i++) {
        obj->acl[i].rbar = 0;
        obj->acl[i].rasr = 0;
    }

    return (0);
}

Int MemProtect_destructDomain(MemProtect_Struct *obj)
{
    return (0);
}

UInt32 MemProtect_parseFlags(UInt32 flags)
{
    UInt32 userFlags = flags & MemProtect_USER_FLAGS_MASK;
    UInt32 privFlags = flags & MemProtect_PRIV_FLAGS_MASK;
    UInt32 memFlags  = flags & MemProtect_MEM_FLAGS_MASK;
    UInt32 regionAttrs = 0;

    if ((userFlags & 0x3) == MemProtect_USER_READ) {
        if ((privFlags & 0x30) == MemProtect_PRIV_READ) {
            /* Priv Read-only, User Read-only */
            regionAttrs |= (0x7 << 24);
        }
        else if ((privFlags & 0x30) ==
            (MemProtect_PRIV_READ | MemProtect_PRIV_WRITE)) {
            /* Priv Read Write, User Read-only */
            regionAttrs |= (0x2 << 24);
        }
        else {
            return (~0);
        }
    }
    else if ((userFlags & 0x3) == (MemProtect_USER_READ | MemProtect_USER_WRITE)) {
        if ((privFlags & 0x30) ==
            (MemProtect_PRIV_READ | MemProtect_PRIV_WRITE)) {
            /* Priv Read Write, User Read Write */
            regionAttrs |= (0x3 << 24);
        }
        else {
            return (~0);
        }
    }
    else if ((userFlags & 0x3) == MemProtect_USER_WRITE) {
        return (~0);
    }
    else {
        if ((privFlags & 0x30) == MemProtect_PRIV_READ) {
            /* Priv Read-only, User No Access */
            regionAttrs |= (0x5 << 24);
        }
        else if ((privFlags & 0x30) ==
            (MemProtect_PRIV_READ | MemProtect_PRIV_WRITE)) {
            /* Priv Read Write, User No Access */
            regionAttrs |= (0x1 << 24);
        }
        else if ((privFlags & 0x30) == MemProtect_PRIV_WRITE) {
            return (~0);
        }
        else {
            /* Priv No Access, User No Access */
            regionAttrs |= (0x0 << 24);
        }
    }

    if ((userFlags & MemProtect_USER_EXEC) != MemProtect_USER_EXEC) {
        if ((privFlags & MemProtect_PRIV_EXEC) == MemProtect_PRIV_EXEC) {
            /* Cannot enable execution for privilege only */
            return (~0);
        }
        else {
            regionAttrs |= (0x1 << 28);
        }
    }

    if (memFlags & MemProtect_DEVICE) {
        if (memFlags > MemProtect_DEVICE) {
            return (~0);
        }

        if (memFlags & MemProtect_SHAREABLE) {
            /* Shared Device */
            regionAttrs |= (0x5 << 16);
        }
        else {
            /* Non-shared Device */
            regionAttrs |= (0x10 << 16);
        }
    }
    else if (memFlags & MemProtect_DEVICE_UNBUFFERED) {
        if (memFlags > MemProtect_DEVICE_UNBUFFERED) {
            return (~0);
        }

        /* Strongly-ordered */
        regionAttrs |= (0x1 << 18);
    }
    else if (memFlags & MemProtect_NONCACHEABLE) {
        if (memFlags > MemProtect_NONCACHEABLE) {
            return (~0);
        }

        if (memFlags & MemProtect_SHAREABLE) {
            /* Shareable Non-cacheable */
            regionAttrs |= (0xC << 16);
        }
        else {
            /* Non-shared Non-cacheable */
            regionAttrs |= (0x8 << 16);
        }
    }
    else if (memFlags & MemProtect_WRITEBACK) {
        if (memFlags & MemProtect_WRITETHROUGH) {
            return (~0);
        }

        if (memFlags & MemProtect_WRITEALLOCATE) {
            /* Outer and inner write-back; write and read allocate */
            regionAttrs |= (0xB << 16);
        }
        else {
            /* Outer and inner write-back, no write allocate */
            regionAttrs |= (0x3 << 16);
        }

        if (memFlags & MemProtect_SHAREABLE) {
            /* Shareable */
            regionAttrs |= (0x1 << 18);
        }
    }
    else if (memFlags & MemProtect_WRITETHROUGH) {
        if (memFlags & MemProtect_SHAREABLE) {
            /* Shareable Outer and inner write-through, no write allocate */
            regionAttrs |= (0x6 << 16);
        }
        else {
            /* Non-shared Outer and inner write-through, no write allocate */
            regionAttrs |= (0x2 << 16);
        }
    }

    return (regionAttrs);
}

/*
 *  ======== MemProtect_programMpuEntry ========
 */
Void MemProtect_programMpuEntry(UInt8 id, UInt32 begin, UInt32 end,
    UInt32 flags)
{
    UInt8 i = MemProtect_NUM_USER_ACL_ENTRIES + id;
    UInt32 rbar, rasr, baseAddress, regionSize, regionAttrs;

    baseAddress = begin;
    regionSize = end - begin;
#if defined(__TI_COMPILER_VERSION__)
    regionSize = (30 - __clz(regionSize)) << 1;
#endif
    regionAttrs = MemProtect_parseFlags(flags);

    rbar = (baseAddress & (~0x1F)) | MPU_VALID | (i & MPU_REGION_MASK);
    rasr = regionAttrs | regionSize | MPU_ENABLE;

    /* Program MPU */
    MPU_setRegionRaw(rbar, rasr);
}

/*
 *  ======== MemProtect_startup ========
 */
Void MemProtect_startup()
{
    UInt8 i = 0;

    MemProtect_programMpuEntry(i++, (UInt32)&_kernel_wrapper_funcs_begin,
        (UInt32)&_kernel_wrapper_funcs_end, (MemProtect_PRIV_READ |
        MemProtect_USER_READ | MemProtect_USER_EXEC |
        MemProtect_WRITETHROUGH));

    MemProtect_programMpuEntry(i++, (UInt32)&_public_code_begin,
        (UInt32)&_public_code_end, (MemProtect_PRIV_READ | 
        MemProtect_USER_READ | MemProtect_USER_EXEC |
        MemProtect_WRITETHROUGH));

    MemProtect_programMpuEntry(i++, (UInt32)&_public_data_begin,
        (UInt32)&_public_data_end, (MemProtect_PRIV_READ |
        MemProtect_PRIV_WRITE | MemProtect_USER_READ |
        MemProtect_USER_WRITE | MemProtect_WRITETHROUGH |
        MemProtect_SHAREABLE));

    MemProtect_programMpuEntry(i, (UInt32)&_public_bss_begin,
        (UInt32)&_public_bss_end, (MemProtect_PRIV_READ |
        MemProtect_PRIV_WRITE | MemProtect_USER_READ |
        MemProtect_USER_WRITE | MemProtect_WRITETHROUGH |
        MemProtect_SHAREABLE));

    MPU_enable();
}

/*
 *  ======== MemProtect_switch ========
 */
Void MemProtect_switch(MemProtect_Struct *obj)
{
    UInt i;

    if (obj == NULL) {
        return;
    }

    for (i = 0; i < MemProtect_NUM_USER_ACL_ENTRIES; i++) {
        if ((obj->acl[i].rbar == 0) && (obj->acl[i].rasr == 0)) {
            MPU_setRegionRaw(MPU_VALID | (i & MPU_REGION_MASK), 0);
        }
        else {
            MPU_setRegionRaw(obj->acl[i].rbar, obj->acl[i].rasr);
        }
    }
}

/*
 *  ======== MemProtect_isDataInKernelSpace ========
 *  This function validates if a data range is in kernel address space.
 *  This is used by other kernel code to ensure it does not accept kernel
 *  objects residing in user address space from the application.
 *
 *  Another added benefit of the check is that if the kernel object is in
 *  system stack or FLASH, we will catch that too.
 */
Bool MemProtect_isDataInKernelSpace(Ptr obj, SizeT size)
{
    Bool ret = FALSE;
    Ptr begin = obj;
    Ptr end = (Ptr)((SizeT)obj + size);

    if ((begin >= (Ptr)&_privileged_bss_begin) &&
        (end <= (Ptr)&_privileged_bss_end)) {
        ret = TRUE;
    }
    else if ((begin >= (Ptr)&_privileged_data_begin) &&
             (end <= (Ptr)&_privileged_data_end)) {
        ret = TRUE;
    }

    return (ret);
}
