OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

MEMORY
{
	ram	: ORIGIN = 0x61000000, LENGTH = 0x40000
}

SECTIONS
{
	__stack_size = DEFINED(__stack_size) ? __stack_size : 4K;

	.text :
	{
		_text_start  = .;
		*startup_aarch64.S.o(.text)
		*(.text)
	}  > ram AT>ram
	PROVIDE(_text_end  = .);

	. = ALIGN(4096);
	PROVIDE(_rodata_start = .);
	.rodata :
	{
		*(.rodata)
        /* section information for usbh class */
        . = ALIGN(4);
        __usbh_class_info_start__ = .;
        KEEP(*(.usbh_class_info))
        __usbh_class_info_end__ = .;
	}  > ram AT>ram
	PROVIDE(_rodata_end   = .);

	.pagetable :
	{
		. = ALIGN(4096);
		PROVIDE(_pagetable_start = .);
		*(.page_table)
		PROVIDE(_pagetable_end   = .);
	}  > ram AT>ram

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

	. = ALIGN(4096);
	PROVIDE(_data_start = .);
	.data :
	{
		*(.data*)
	}  > ram AT>ram

	/* all bss segment */
	. = ALIGN(4096);
	_bss_start = .;
	.bss :
	{
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
		. = __stack_size;
		PROVIDE(stack_top = .);
		PROVIDE(_sp = .);

	} > ram AT>ram
}
