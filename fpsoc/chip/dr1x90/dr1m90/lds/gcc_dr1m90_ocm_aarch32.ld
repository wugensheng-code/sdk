/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(vectors)

MEMORY
{
    ram    : ORIGIN = 0x61000300, LENGTH = 0x3c000
}

SECTIONS
{
    __stack_size = DEFINED(__stack_size) ? __stack_size : 8K;

    .text :
    {
        PROVIDE(_text_start  = .);
        *(.vectors*)
        *al_aarch32_start.S.o(.text*)
        *(*.text*)
        PROVIDE(_text_end  = .);
    }  > ram AT>ram
    PROVIDE(_text_end  = .);

    .ilalign         :
    {
        . = ALIGN(4);
        PROVIDE( _ilm_lma = . );
    } > ram AT>ram

    .ialign         :
    {
        PROVIDE( _ilm = . );
    } > ram AT>ram

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
    }  > ram AT>ram
    PROVIDE(_rodata_end   = .);

    PROVIDE_HIDDEN (__exidx_start = .);
    PROVIDE_HIDDEN (__exidx_end = .);

    . = ALIGN(4096);
    .pagetable :
    {
        . = ALIGN(4096);
        PROVIDE(_pagetable_start = .);
        *(.page_table)
        PROVIDE(_pagetable_end   = .);
    }  > ram AT>ram

    .lalign         :
    {
        . = ALIGN(4);
        PROVIDE( _data_lma = LOADADDR(.data) );
    } >ram AT>ram

    .dalign         :
    {
        . = ALIGN(4);
        PROVIDE( _data = ADDR(.data));
    } >ram AT>ram

    . = ALIGN(4096);
    .cli_section :
    {
    . = ALIGN(4096);
    _cli_section_start = .;
        KEEP(*(cli_section))
        _cli_section_end   = .;
    }  > ram AT>ram

    . = ALIGN(4096);
    .unity_section :
    {
        . = ALIGN(4096);
        _unity_test_start = .;
        KEEP(*(unity_section))
        _unity_test_end   = .;
    }  > ram AT>ram

    . = ALIGN(4096);
    PROVIDE(_data_start = .);
    .data :
    {
        . = ALIGN(4096);
        *(.data*)
        _no_cache_section_start = (. & 0x200000);
        _no_cache_section_end   = (. & 0x200000);
    }  > ram AT>ram

    /* all bss segment */
    . = ALIGN(4096);
    _bss_start = .;
    .bss :
    {
        . = ALIGN(4096);
        *(.bss*)
    }  > ram AT>ram
    . = ALIGN(4096);
    _bss_end = .;
    _end = .;
    _heap_start = .;

#if 0
	__stack_end = . ;
	. = . + 0x8000;
	__stack_start = .;
#endif


    .stack ORIGIN(ram) + LENGTH(ram) - __stack_size (NOLOAD) :
    {
        PROVIDE(_heap_end = .);
        PROVIDE(stack_start = .);
        . = ALIGN(16);
        . = __stack_size;
        PROVIDE(stack_top = .);
        PROVIDE(_sp = .);
        . = ALIGN(16);

    } > ram AT>ram
}
