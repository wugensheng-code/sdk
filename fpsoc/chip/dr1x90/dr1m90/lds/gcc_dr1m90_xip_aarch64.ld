/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

__ROM_BASE = 0x70400000;
__ROM_SIZE = 0x100000;

MEMORY
{
    flash  : ORIGIN = __ROM_BASE, LENGTH = __ROM_SIZE
    ocm    : ORIGIN = 0x61000000, LENGTH = 256K
}

SECTIONS
{
    __stack_size = DEFINED(__stack_size) ? __stack_size : 8K;


    .ilalign         :
    {
        . = ALIGN(4);
        PROVIDE( _ilm_lma = LOADADDR(.text) );
    } > flash AT>flash

    .ialign         :
    {
        PROVIDE( _ilm = ADDR(.text) );
    } > flash AT>flash

    .text :
    {
        PROVIDE(_text_start  = .);
        *al_aarch64_start.S.o(.text)
        *(.text)
        PROVIDE(_text_end  = .);
    }  > flash AT>flash
    PROVIDE(_text_end  = .);

    . = ALIGN(4096);
    PROVIDE(_rodata_start = .);
    .rodata :
    {
        . = ALIGN(4096);
        *(.rodata)
        . = ALIGN(4);
        *(.rdata)
        *(.rodata .rodata.*)
        . = ALIGN(4);
        /* section information for initial. */
        . = ALIGN(4);
        __rt_init_start = .;
        KEEP(*(SORT(.rti_fn*)))
        __rt_init_end = .;
        /* section information for finsh shell */
        . = ALIGN(4);
        __fsymtab_start = .;
        KEEP(*(FSymTab))
        __fsymtab_end = .;
        . = ALIGN(4);
        __vsymtab_start = .;
        KEEP(*(VSymTab))
        __vsymtab_end = .;
        . = ALIGN(4);
        __rt_utest_tc_tab_start = .;
        KEEP(*(UtestTcTab))
        __rt_utest_tc_tab_end = .;
        *(.gnu.linkonce.r.*)
        /* section information for usbh class */
        . = ALIGN(4);
        __usbh_class_info_start__ = .;
        KEEP(*(.usbh_class_info))
        __usbh_class_info_end__ = .;
    }  > flash AT>flash
    PROVIDE(_rodata_end   = .);

    . = ALIGN(4096);
    .pagetable :
    {
        . = ALIGN(4096);
        PROVIDE(_pagetable_start = .);
        *(.page_table)
        PROVIDE(_pagetable_end   = .);
    }  > flash AT>flash

    . = ALIGN(4096);
    .cli_section :
    {
    . = ALIGN(4096);
    _cli_section_start = .;
        KEEP(*(cli_section))
        _cli_section_end   = .;
    }  > ocm AT>flash

    . = ALIGN(4096);
    .unity_section :
    {
        . = ALIGN(4096);
        _unity_test_start = .;
        KEEP(*(unity_section))
        _unity_test_end   = .;
    }  > ocm AT>flash

    PROVIDE (__etext = .);
    PROVIDE (_etext = .);
    PROVIDE (etext = .);
    PROVIDE( _eilm = . );

    . = ALIGN(4096);

    .lalign         :
    {
        . = ALIGN(4);
        PROVIDE( _data_lma = LOADADDR(.data) );
    } >ocm AT>flash

    .dalign         :
    {
        . = ALIGN(4);
        PROVIDE( _data = ADDR(.data));
    } >ocm AT>flash

    PROVIDE(_data_start = .);
    .data :
    {
        . = ALIGN(4096);
        *(.data*)
        _no_cache_section_start = (. & 0x200000);
        _no_cache_section_end   = (. & 0x200000);
    }  > ocm AT>flash

    PROVIDE( _edata = . );
    PROVIDE( edata = . );

    /* all bss segment */
    . = ALIGN(4096);
    _bss_start = .;
    .bss :
    {
        . = ALIGN(4096);
        *(.bss*)
    }  > ocm AT>flash
    . = ALIGN(4096);
    _bss_end = .;
    _end = .;
    _heap_start = .;

    .stack ORIGIN(ocm) + LENGTH(ocm) - __stack_size (NOLOAD) :
    {
        PROVIDE(_heap_end = .);
        PROVIDE(stack_start = .);
        . = ALIGN(16);
        . = __stack_size;
        PROVIDE(stack_top = .);
        PROVIDE(_sp = .);
        . = ALIGN(16);

    } > ocm AT>flash
}
