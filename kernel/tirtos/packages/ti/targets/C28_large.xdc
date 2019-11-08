/* 
 *  Copyright (c) 2008-2015 Texas Instruments Incorporated
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 *
 * */

/*
 *  ======== C28_large.xdc ========
 *
 */


/*!
 *  ======== C28_large ========
 *  TI C28 large model little endian
 */
metaonly module C28_large inherits ti.targets.ITarget {
    override readonly config string name                = "C28_large";
    override readonly config string suffix              = "28L";
    override readonly config string isa                 = "28";
    override readonly config xdc.bld.ITarget.Model model    = {dataModel: "large", endian: "little"};
    override readonly config string rts                 = "ti.targets.rts2800";
    final override readonly config Bool alignDirectiveSupported = false;

    /*!
     *  ======== ar ========
     *  Define archiver executable
     *
     *  Options:
     *  @p(dlist)
     *      -`-r`
     *          replace file
     *      -`-q`
     *          quiet mode
     */
    override readonly config xdc.bld.ITarget2.Command ar = {
        cmd: "ar2000",
        opts: "rq"
    };

    /*!
     *  ======== cc ========
     *  Define compiler executable
     *
     *  Options:
     *  @p(dlist)
     *      -`-c`
     *          no linking
     *      -`-v28`
     *          compile for c28x.
     *      -`-ml`
     *          use large memory model
     *      -`-DLARGE_MODEL=1`
     *          required to use va_arg in large model
     */
    override readonly config xdc.bld.ITarget2.Command cc = {
        cmd: "cl2000 -c",
        opts: "-v28 -DLARGE_MODEL=1 -ml"
    };

    /*!
     *  ======== vers ========
     *  Define the vers command
     */
    override readonly config xdc.bld.ITarget2.Command vers = {
        cmd: "cl2000",
        opts: "--compiler_revision"
    };

    /*!
     *  ======== asm ========
     *  Define assembler executable
     *
     *  Options:
     *  @p(dlist)
     *      -`-c`
     *          no linking
     *      -`-v28`
     *          compile for c28x.
     *      -`-ml`
     *          use large memory model
     *      -`-DLARGE_MODEL=1`
     *          because compiler and BIOS are broken; asembler
     *          defines __LARGE_MODEL but BIOS uses LARGE_MODEL
     *
     */
    override readonly config xdc.bld.ITarget2.Command asm = {
        cmd: "cl2000 -c",
        opts: "-v28 -ml -DLARGE_MODEL=1"
    };

    /*!
     *  ======== lnk ========
     *  Define linker executable
     */
    override readonly config xdc.bld.ITarget2.Command lnk = {
        cmd: "cl2000",
        opts: "-z"
    };

    /*!
     *  ======== asmOpts ========
     *  User configurable assembler options.
     *
     *  Defaults:
     *  @p(dlist)
     *      -`-qq`
     *          super quiet mode
     */
    override config xdc.bld.ITarget2.Options asmOpts = {
        prefix: "-qq",
        suffix: ""
    };

    /*!
     *  ======== ccOpts ========
     *  Compiler options
     *  @p(dlist)
     *      -`-qq`
     *          super quiet mode
     *      -`-pdsw225`
     *          generate a warning for implicitly declared functions; i.e.,
     *          functions without prototypes
     *      -`-Dfar= `
     *          ignore keyword far; this allows one to write code that can
     *          be compiled in large model and small model without #ifdef's
     */
    override config xdc.bld.ITarget2.Options ccOpts = {
        prefix: "-qq -pdsw225 -Dfar= ",
        suffix: ""
    };

    /*!
     *  ======== profiles ========
     *  Standard options profiles for the TI tool-chain.
     */
    override config xdc.bld.ITarget.OptionSet profiles[string] = [
        ["debug", {
            compileOpts: {
                copts: "-g",
                defs:  "-D_DEBUG_=1",
            }
        }],
        ["release", {
            compileOpts: {
                copts: "-O2",
            },
        }],
        ["profile", {
            compileOpts: {
                copts: "-gp",
            },
        }],
        ["coverage", {
            compileOpts: {
                copts: "-gp",
            },
        }],
    ];

    /*!
     *  ======== includeOpts ========
     *  Default include search path
     */
    override config string includeOpts = "-I$(rootDir)/include";

    final override readonly config string sectMap[string] =
        ti.targets.C28.sectMap;

    final override readonly config Bool splitMap[string] =
        ti.targets.C28.splitMap;

    override readonly config xdc.bld.ITarget.StdTypes stdTypes = {
        t_IArg          : { size: 2, align: 2 },
        t_Char          : { size: 1, align: 1 },
        t_Double        : { size: 2, align: 2 },
        t_Float         : { size: 2, align: 2 },
        t_Fxn           : { size: 2, align: 2 }, /* far */
        t_Int           : { size: 1, align: 1 },
        t_Int8          : { size: 1, align: 1 },
        t_Int16         : { size: 1, align: 1 },
        t_Int32         : { size: 2, align: 2 },
        t_Int64         : { size: 4, align: 2 },
        t_Long          : { size: 2, align: 2 },
        t_LDouble       : { size: 2, align: 2 },
        t_LLong         : { size: 4, align: 2 },
        t_Ptr           : { size: 2, align: 2 },
        t_Short         : { size: 1, align: 1 },
        t_Size          : { size: 2, align: 2 },
    };

    override readonly config Int bitsPerChar = 16;
}
/*
 *  @(#) ti.targets; 1, 0, 3,0; 8-9-2019 17:15:03; /db/ztree/library/trees/xdctargets/xdctargets-v00/src/ xlibrary

 */

