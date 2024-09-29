/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

MEMORY
{
    ram    : ORIGIN = 0x61000000, LENGTH = 0x3F000
}

SECTIONS
{
    __stack_size = DEFINED(__stack_size) ? __stack_size : 8K;

    .text :
    {
        PROVIDE(_text_start  = .);
        *al_aarch64_start.S.o(.text)
        *(.text)
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
        __rt_utest_tc_tab_start = .;
        KEEP(*(UtestTcTab))
        __rt_utest_tc_tab_end = .;
        *(.gnu.linkonce.r.*)
        /* section information for usbh class */
        . = ALIGN(4);
        __usbh_class_info_start__ = .;
        KEEP(*(.usbh_class_info))
        __usbh_class_info_end__ = .;
    }  > ram AT>ram
    PROVIDE(_rodata_end   = .);

    .ctors          :
    {
        /* gcc uses crtbegin.o to find the start of
        the constructors, so we make sure it is
        first.  Because this is a wildcard, it
        doesn't matter if the user does not
        actually link against crtbegin.o; the
        linker won't look for a file to match a
        wildcard.  The wildcard also means that it
        doesn't matter which directory crtbegin.o
        is in.  */
        KEEP (*crtbegin.o(.ctors))
        KEEP (*crtbegin?.o(.ctors))
        /* We don't want to include the .ctor section from
        the crtend.o file until after the sorted ctors.
        The .ctor section from the crtend file contains the
        end of ctors marker and it must be last */
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))

        PROVIDE(__ctors_start__ = .);
        /* new GCC version uses .init_array */
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE(__ctors_end__ = .);
    } >ram AT>ram


    .eh_frame : {
       *(.eh_frame)
    } >ram AT>ram

    .eh_framehdr : {
       __eh_framehdr_start = .;
       *(.eh_framehdr)
       __eh_framehdr_end = .;
    } >ram AT>ram

    .gcc_except_table : {
       *(.gcc_except_table)
    } >ram AT>ram

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

    PROVIDE (__etext = .);
    PROVIDE (_etext = .);
    PROVIDE (etext = .);
    PROVIDE( _eilm = . );

    . = ALIGN(4096);
    PROVIDE(_data_start = .);
    .data :
    {
        . = ALIGN(4096);
        *(.data*)
        _no_cache_section_start = (. & 0x200000);
        _no_cache_section_end   = (. & 0x200000);
    }  > ram AT>ram

    PROVIDE( _edata = . );
    PROVIDE( edata = . );

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
