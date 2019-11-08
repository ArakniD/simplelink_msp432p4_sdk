/* 
 *  Copyright (c) 2008 Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 *
 * */

/*!
 *  ======== M4.xdc ========
 *  Cortex M4, no floating point unit, little endian thumb2 (ELF)
 */
metaonly module M4 inherits ti.targets.arm.elf.IM4 {
    override readonly config string name        = "M4";
    override readonly config string suffix      = "em4";
    override readonly config string rts         = "ti.targets.arm.rtsarm";

    /*
     *  ======== compatibleSuffixes ========
     */
    override config String compatibleSuffixes[] = ["em3"];

    override readonly config xdc.bld.ITarget2.Command cc = {
        cmd:  "armcl -c",
        opts: "--endian=little -mv7M4 --float_support=vfplib --abi=eabi"
    };

    override readonly config xdc.bld.ITarget2.Command asm = {
        cmd:  "armcl -c",
        opts: "--endian=little -mv7M4 --float_support=vfplib --abi=eabi"
    };
}
/*
 *  @(#) ti.targets.arm.elf; 1, 0, 0,0; 8-9-2019 17:15:05; /db/ztree/library/trees/xdctargets/xdctargets-v00/src/ xlibrary

 */

