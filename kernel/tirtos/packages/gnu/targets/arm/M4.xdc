/*
 *  Copyright 2019 by Texas Instruments Incorporated.
 *
 */

/*
 *  Copyright (c) 2008-2017 Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 */
import xdc.bld.ITarget2;

/*!
 *  ======== M4.xdc ========
 *  Embedded Cortex M4, little endian, bare metal target
 *
 *  This module defines an embedded bare metal target on Cortex M4. The target
 *  generates code compatible with the "v7M" architecture.
 *
 */
metaonly module M4 inherits IM {
    override readonly config string name                = "M4"; 
    override readonly config string suffix              = "m4g";
    override readonly config string isa                 = "v7M4";

    override readonly config ITarget2.Command cc = {
        cmd: "$(rootDir)/bin/$(GCCTARG)-gcc -c -MD -MF $@.dep",
        opts: "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -mabi=aapcs -g"
    };

    readonly config ITarget2.Command ccBin = {
        cmd: "bin/arm-none-eabi-gcc -c -MD -MF $@.dep",
        opts: "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -mabi=aapcs -g"
    };

    override readonly config ITarget2.Command asm = {
        cmd: "$(rootDir)/bin/$(GCCTARG)-gcc -c -x assembler-with-cpp",
        opts: "-Wa,-mcpu=cortex-m4 -Wa,-mthumb"
    };

    readonly config ITarget2.Command asmBin = {
        cmd: "bin/arm-none-eabi-gcc -c -x assembler-with-cpp",
        opts: "-Wa,-mcpu=cortex-m4 -Wa,-mthumb"
    };

    override config ITarget2.Options lnkOpts = {
        prefix: "-mthumb -march=armv7e-m -nostartfiles -Wl,-static -Wl,--gc-sections ",
        suffix: "-L$(packageBase)/libs/install-native/$(GCCTARG)/lib/thumb/v7e-m -Wl,--start-group -lgcc -lc -lm -Wl,--end-group --specs=nano.specs -Wl,-Map=$(XDCCFGDIR)/$@.map"
    };

    /*!
     *  ======== includeOpts ========
     *  Additional user configurable target-specific include path options
     *
     *  This target uses newlib-nano run-time.
     */
    override config string includeOpts = "-I$(packageBase)/libs/install-native/$(GCCTARG)/include/newlib-nano -I$(packageBase)/libs/install-native/$(GCCTARG)/include";
}
/*
 *  @(#) gnu.targets.arm; 1, 0, 0,0; 8-9-2019 17:14:52; /db/ztree/library/trees/xdctargets/xdctargets-v00/src/ xlibrary

 */

