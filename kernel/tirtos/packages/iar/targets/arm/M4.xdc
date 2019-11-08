/* 
 *  Copyright (c) 2013-2015 Texas Instruments and others.
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
 *  ======== M4 ========
 *  IAR M4 target
 */
metaonly module M4 inherits ITarget
{
    override readonly config String name = "M4";

    override readonly config String suffix = "rm4";

    override readonly config String isa  = "v7M4";

    override config string platform  = "ti.platforms.tiva:TM4C1294NCPDT:1";

    /*!
     *  ======== cc ========
     *  The command used to compile C/C++ source files into object files
     */
    override readonly config Command cc = {
        cmd: "iccarm",
        opts: "--aeabi --cpu=Cortex-M4 --diag_suppress=Pa050,Go005 --endian=little -e --thumb"
    };

    /*!
     *  ======== asm ========
     *  The command used to assembles assembly source files into object files
     */
    override readonly config Command asm = {
        cmd: "iasmarm",
        opts: "--cpu Cortex-M4 --endian little"
    };

    /*!
     *  ======== lnk ========
     *  The command used to link executables.
     */
    override readonly config Command lnk = {
        cmd: "ilinkarm",
        opts: "--cpu=Cortex-M4"
    };
}
/*
 *  @(#) iar.targets.arm; 1, 0, 0,0; 8-9-2019 17:14:57; /db/ztree/library/trees/xdctargets/xdctargets-v00/src/ xlibrary

 */

