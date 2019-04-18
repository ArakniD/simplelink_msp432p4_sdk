/******************************************************************************
*
* Copyright (C) 2012 - 2017 Texas Instruments Incorporated - http://www.ti.com/
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
*  Redistributions of source code must retain the above copyright
*  notice, this list of conditions and the following disclaimer.
*
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the
*  documentation and/or other materials provided with the
*  distribution.
*
*  Neither the name of Texas Instruments Incorporated nor the names of
*  its contributors may be used to endorse or promote products derived
*  from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* GCC linker script for Texas Instruments MSP432P401R
*
* File creation date: 04/18/17
*
******************************************************************************/


/* SRAM_CODE is placed from 0x0100_1200 to 0x0100_1800  but is aliased to 0x2000_1200 to 0x2000_1800*/
/* SRAM_DATA1 is placed from 0x2000_0000 to 0x2000_1200  */
/* Ram functions are placed in SRAM_DATA2 is from 0x2000_1200 to 0x2000_1800  */
/* Valiables for computation are placed in SRAM_DATA3 is from 0x2000_1800 to 0x2000_6000  */



MEMORY
{
    MAIN_FLASH (RX) : ORIGIN = 0x00000000, LENGTH = 0x00040000
    INFO_FLASH (RX) : ORIGIN = 0x00200000, LENGTH = 0x00004000
    SRAM_CODE  (RWX): ORIGIN = 0x01001200, LENGTH = 0x00000600
    SRAM_SLEEP  (RW) : ORIGIN = 0x20000000, LENGTH = 0x00001200
    SRAM_ACTIVE (RW) : ORIGIN = 0x20001800, LENGTH = 0x00004800
}

REGION_ALIAS("REGION_TEXT", MAIN_FLASH);
REGION_ALIAS("REGION_INFO", INFO_FLASH);
REGION_ALIAS("REGION_BSS", SRAM_SLEEP);
REGION_ALIAS("REGION_DATA", SRAM_SLEEP);
REGION_ALIAS("REGION_STACK", SRAM_SLEEP);
REGION_ALIAS("REGION_HEAP", SRAM_SLEEP);
REGION_ALIAS("REGION_RAM_EXE", SRAM_CODE);
REGION_ALIAS("REGION_RAM_ACTIVE",SRAM_ACTIVE);
REGION_ALIAS("REGION_ARM_EXIDX", MAIN_FLASH);
REGION_ALIAS("REGION_ARM_EXTAB", MAIN_FLASH);

SECTIONS {

    /* section for the interrupt vector area                                 */
    PROVIDE (_intvecs_base_address =
        DEFINED(_intvecs_base_address) ? _intvecs_base_address : 0x0);

    .intvecs (_intvecs_base_address) : AT (_intvecs_base_address) {
        KEEP (*(.intvecs))
    } > REGION_TEXT

    /* The following three sections show the usage of the INFO flash memory  */
    /* INFO flash memory is intended to be used for the following            */
    /* device specific purposes:                                             */
    /* Flash mailbox for device security operations                          */
    PROVIDE (_mailbox_base_address = 0x200000);

    .flashMailbox (_mailbox_base_address) : AT (_mailbox_base_address) {
        KEEP (*(.flashMailbox))
    } > REGION_INFO

    /* TLV table for device identification and characterization              */
    PROVIDE (_tlv_base_address = 0x00201000);

    .tlvTable (_tlv_base_address) (NOLOAD) : AT (_tlv_base_address) {
        KEEP (*(.tlvTable))
    } > REGION_INFO

    /* BSL area for device bootstrap loader                                  */
    PROVIDE (_bsl_base_address = 0x00202000);

    .bslArea (_bsl_base_address) : AT (_bsl_base_address) {
        KEEP (*(.bslArea))
    } > REGION_INFO

    PROVIDE (_vtable_base_address =
        DEFINED(_vtable_base_address) ? _vtable_base_address : 0x20000000);

    .vtable (_vtable_base_address) : AT (_vtable_base_address) {
        KEEP (*(.vtable))
    } > REGION_DATA

    .text : {
        CREATE_OBJECT_SYMBOLS
        KEEP (*(.text))
        *(.text.*)
        . = ALIGN(0x4);
        KEEP (*(.ctors))
        . = ALIGN(0x4);
        KEEP (*(.dtors))
        . = ALIGN(0x4);
        __init_array_start = .;
        KEEP (*(.init_array*))
        __init_array_end = .;
        KEEP (*(.init))
        KEEP (*(.fini*))
    } > REGION_TEXT AT> REGION_TEXT

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    } > REGION_TEXT AT> REGION_TEXT

    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > REGION_ARM_EXIDX AT> REGION_ARM_EXIDX

    .ARM.extab : {
        KEEP (*(.ARM.extab* .gnu.linkonce.armextab.*))
    } > REGION_ARM_EXTAB AT> REGION_ARM_EXTAB

    __etext = .;

    .data : {
        __data_load__ = LOADADDR (.data);
        __data_start__ = .;
        KEEP (*(.data))
        KEEP (*(.data*))
        . = ALIGN (4);
        __data_end__ = .;
    } > REGION_DATA AT> REGION_TEXT

    .bss : {
        __bss_start__ = .;
        *(.shbss)
        KEEP (*(.bss))
        *(.bss.*)
        *(COMMON)
        . = ALIGN (4);
        __bss_end__ = .;
    } > REGION_BSS AT> REGION_BSS

    .heap : {
        __heap_start__ = .;
        end = __heap_start__;
        _end = end;
        __end = end;
        KEEP (*(.heap))
        __heap_end__ = .;
        __HeapLimit = __heap_end__;
    } > REGION_HEAP AT> REGION_HEAP

    .stack (NOLOAD) : ALIGN(0x8) {
        _stack = .;
        KEEP(*(.stack))
    } > REGION_STACK AT> REGION_STACK
	
	__StackTop = ORIGIN(REGION_STACK) + LENGTH(REGION_STACK);
    PROVIDE(__stack = __StackTop);

/* This is an output section description */
/*
 * section [address] [(type)] : [AT(lma)]
 *   {
 *      output-section-command
 *      output-section-command
 *      ...
 *   } [>region] [AT>lma_region] [:phdr :phdr ...] [=fillexp]
 */
    myData :
    {
        *(myData)
        . = ALIGN (4);
    } > REGION_RAM_ACTIVE AT> REGION_RAM_ACTIVE

    .TI.ramfunk :
    {
        *(.TI.ramFunk)
    } > REGION_RAM_EXE AT> REGION_TEXT

}
