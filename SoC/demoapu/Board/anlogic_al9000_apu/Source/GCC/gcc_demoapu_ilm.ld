OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)


SECTIONS
{
	. = 0x61000000;
	__stack_size = 2k;

	_text_start = .;
	.text :
	{
		*/startup64.o(.text)
		*(.text)
	}
	_text_end   = .;

	. = ALIGN(4096);
	_rodata_start = .;
	.rodata :
	{
		*(.rodata)
	}
	_rodata_end   = .;

	. = ALIGN(4096);
	_data_start = .;
	.data :
	{
		*(.data)
	}

	/* stack: 64k */
	. = ALIGN(4096);
	_stack_start = .;
	. = . + 0x10000;
	stack_top = .;
	_stack_end = .;

	 /* all bss segment */
	_bss_start = .;
	.bss :
	{
		*(.bss COMMON)
	}
	_bss_end = .;

	_end = .;
	end = .;
	. = ALIGN(4096);
	.stack :
	{
		_heap_end = .;
		. = __stack_size;
		_sp = .;
	}
}
