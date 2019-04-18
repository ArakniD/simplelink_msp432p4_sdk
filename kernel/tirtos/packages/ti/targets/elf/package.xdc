/* 
 *  Copyright (c) 2008-2018 Texas Instruments and others. - http://www.ti.com
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 *
 * 
 */
/*
 *  ======== package.xdc ========
 */

requires xdc.bld;

/*!
 *  ======== ti.targets.elf ========
 *  Package of targets for TI code generation tools for ELF
 *
 */
package ti.targets.elf [1,0,0] {
    interface ITarget;
    module C64P, C674;
    module C66;
    module C28_float;
}
/*
 *  @(#) ti.targets.elf; 1, 0, 0,2; 2-9-2019 16:22:50; /db/ztree/library/trees/xdctargets/xdctargets-t01/src/ xlibrary

 */

