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
	ram	: ORIGIN = 0x00400000, LENGTH = 0x0f000000
}

SECTIONS
{
	__stack_size = DEFINED(__stack_size) ? __stack_size : 64M;
	/* all text segment = startup64.o data segment + others text segment */
	.text :
	{
		PROVIDE(_text_start  = .);
		*/startup64.o(.text)
		*boot/arm/*(.text)
		*(.text)
		PROVIDE(_text_end  = .);
	}  > ram AT>ram

	/* read only data segment */
	.rodata :
	{
		. = ALIGN(16);
		PROVIDE(_rodata_start = .);
		*(.rodata)
		PROVIDE(_rodata_end   = .);
        /* section information for usbh class */
        . = ALIGN(4);
        __usbh_class_info_start__ = .;
        KEEP(*(.usbh_class_info))
        __usbh_class_info_end__ = .;
	}  > ram AT>ram

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
		_cli_section_start = .;
		KEEP(*(cli_section))
		_cli_section_end   = .;
	}  > ram AT>ram

	. = ALIGN(4096);
	.unity_section :
	{
		_unity_test_start = .;
		KEEP(*(unity_section))
		_unity_test_end   = .;
	}  > ram AT>ram

	/* all data segment */
	.data :
	{
		. = ALIGN(16);
		PROVIDE(_data_start = .);
		*(.data*)
		PROVIDE(_data_end = .);
	}  > ram AT>ram

	.noncacheable :
	{
		. = ALIGN(2M);
		_no_cache_section_start = .;
		KEEP(*(.noncacheable))
		_no_cache_section_end   = .;
		. = ALIGN(2M);
	}  > ram AT>ram

	/* all bss segment */
	.bss (NOLOAD):
	{
		. = ALIGN(16);
		PROVIDE(_bss_start = .);
		*(*bss .bss*)
		*(COMMON)
		PROVIDE(_bss_end = .);
	}  > ram AT>ram

	/* end */
	. = ALIGN(16);
	_end = .;
	_heap_start = .;

	.stack ORIGIN(ram) + LENGTH(ram) - __stack_size (NOLOAD) :
	{
		PROVIDE(_heap_end = .);
		PROVIDE(stack_start = .);
		. = __stack_size;
		PROVIDE(stack_top = .);
		PROVIDE(_sp = .);

	} > ram AT>ram
}
