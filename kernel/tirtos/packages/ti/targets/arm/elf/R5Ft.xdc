/* 
 *  Copyright (c) 2015-2019 Texas Instruments and others.
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
 *  ======== R5F.xdc ========
 *  Cortex R5 little endian with floating point support (vfpv3d16) (ELF)
 */
metaonly module R5Ft inherits ti.targets.arm.elf.IR5 {
    override readonly config string name        = "R5Ft";
    override readonly config string suffix      = "er5ft";
    override readonly config string rts         = "ti.targets.arm.rtsarm";

    override readonly config xdc.bld.ITarget.Model model = {
        endian: "little",
        codeModel: "thumb2",
        shortEnums: true
    };

    override readonly config xdc.bld.ITarget2.Command cc = {
        cmd:  "armcl -c",
        opts: "--code_state=16 --float_support=vfpv3d16 --endian=little -mv7R5 --abi=eabi"
    };

    override readonly config xdc.bld.ITarget2.Command asm = {
        cmd:  "armcl -c",
        opts: "--code_state=16 --float_support=vfpv3d16 --endian=little -mv7R5 --abi=eabi"
    };
}
/*
 *  @(#) ti.targets.arm.elf; 1, 0, 0,0; 8-9-2019 17:15:05; /db/ztree/library/trees/xdctargets/xdctargets-v00/src/ xlibrary

 */

