OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

MEMORY
{
	ram		: ORIGIN = 0x61000000, LENGTH = 0x40000
}

SECTIONS
{
	__stack_size = DEFINED(__stack_size) ? __stack_size : 16K;

	.text :
	{
		*/startup64.o(.text)
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
		*(.data)
	}  > ram AT>ram

	/* all bss segment */
	PROVIDE(_bss_start = .);
	.bss :
	{
		*(.bss COMMON)
	}  > ram AT>ram
	PROVIDE(_bss_end = .);

	. = ALIGN(4096);
	PROVIDE(_end = .);
	.stack ORIGIN(ram) + LENGTH(ram) - __stack_size :
	{
		PROVIDE(_heap_end = .);
		. = __stack_size;
		PROVIDE(_sp = .);
	} > ram AT>ram
}
