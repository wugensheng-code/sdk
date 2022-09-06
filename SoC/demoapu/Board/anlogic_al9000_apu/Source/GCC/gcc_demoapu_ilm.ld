OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

MEMORY
{
	ram	: ORIGIN = 0x61001000, LENGTH = 0x3E000
}

SECTIONS
{
	__stack_size = DEFINED(__stack_size) ? __stack_size : 16K;

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
	}  > ram AT>ram
	PROVIDE(_rodata_end   = .);

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

	PROVIDE(_end = .);
	.stack ORIGIN(ram) + LENGTH(ram) - __stack_size :
	{
		PROVIDE(_heap_end = .);
		. = __stack_size;
		PROVIDE(_sp = .);
	} > ram AT>ram
}
