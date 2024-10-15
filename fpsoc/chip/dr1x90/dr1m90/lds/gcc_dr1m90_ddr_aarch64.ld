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
    ram    : ORIGIN = 0x8000000, LENGTH = 0x1000000
}

SECTIONS
{
    __stack_size = DEFINED(__stack_size) ? __stack_size : 6M;
    __async_print_ring_buf_size = DEFINED(async_print_ring_buf_size) ? async_print_ring_buf_size : 1M;
    /* all text segment = startup64.o data segment + others text segment */
    .text :
    {
        PROVIDE(_text_start  = .);
        *al_aarch64_start.S.o(.text)
        *(.text)
        PROVIDE(_text_end  = .);
    }  > ram AT>ram

    .ilalign         :
    {
        . = ALIGN(4);
        PROVIDE( _ilm_lma = . );
    } > ram AT>ram

    .ialign         :
    {
        PROVIDE( _ilm = . );
    } > ram AT>ram

    /* read only data segment */
    .rodata :
    {
        . = ALIGN(16);
        PROVIDE(_rodata_start = .);
        *(.rodata)
        PROVIDE(_rodata_end   = .);
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

    .pagetable :
    {
        . = ALIGN(4096);
        PROVIDE(_pagetable_start = .);
        *(.page_table)
        PROVIDE(_pagetable_end   = .);
    }  > ram AT>ram

    .fini           :
    {
        KEEP (*(SORT_NONE(.fini)))
    } >ram AT>ram

    . = ALIGN(4);

    .preinit_array  :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >ram AT>ram

    .init_array     :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >ram AT>ram

    .fini_array     :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >ram AT>ram

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
    } >ram AT>ram

    .dtors          :
    {
        KEEP (*crtbegin.o(.dtors))
        KEEP (*crtbegin?.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
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

    /* all data segment */
    . = ALIGN(16);
    .data :
    {
        . = ALIGN(16);
        PROVIDE(_data_start = .);
        *(.data*)
        PROVIDE(_data_end = .);
    }  > ram AT>ram

    . = ALIGN(2M);
    .noncacheable (NOLOAD):
    {
        . = ALIGN(2M);
        _no_cache_section_start = .;
        KEEP(*(.noncacheable))
        . = ALIGN(2M);
        _no_cache_section_end   = .;
    }  > ram AT>ram

    . = ALIGN(64);
    .async_print_ring_buf (NOLOAD):
    {
        . = ALIGN(64);
        PROVIDE(_async_print_ring_buf_start = .);
        . = __async_print_ring_buf_size;
        PROVIDE(_async_print_ring_buf_end = .);
        . = ALIGN(64);
    }  > ram AT>ram

    PROVIDE( _edata = . );
    PROVIDE( edata = . );

    . = ALIGN(16);
    /* all bss segment */
    .bss (NOLOAD):
    {
        . = ALIGN(16);
        PROVIDE(_bss_start = .);
        *(*bss .bss*)
        *(COMMON)
        PROVIDE(_bss_end = .);
        . = ALIGN(16);
    }  > ram AT>ram

    /* section information for openamp rsc table */
    .resource_table :
    {
        . = ALIGN(4);
        PROVIDE(_resource_table_start = .);
        *(.resource_table)
        PROVIDE(_resource_table_end = .);
    }  > ram AT>ram

    /* end */
    . = ALIGN(16);
    _end = .;
    _heap_start = .;

    .stack (ORIGIN(ram) + LENGTH(ram) - __stack_size):
    {
        . = ALIGN(16);
        PROVIDE(_heap_end = .);
        PROVIDE(stack_start = .);
        . = __stack_size;
        PROVIDE(stack_top = .);
        PROVIDE(_sp = .);
        . = ALIGN(16);

    } > ram AT>ram
}
