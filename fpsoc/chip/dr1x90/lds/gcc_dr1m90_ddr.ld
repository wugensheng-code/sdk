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
	}  > ram AT>ram
	PROVIDE(_text_end  = .);

	/* read only data segment */
	. = ALIGN(16);
	PROVIDE(_rodata_start = .);
	.rodata :
	{
		*(.rodata)
	}  > ram AT>ram
	PROVIDE(_rodata_end   = .);

	/* all data segment */
	. = ALIGN(16);
	PROVIDE(_data_start = .);
	.data :
	{
		*(.data*)
	}  > ram AT>ram
	PROVIDE(_data_end = .);

	/* all bss segment */
	. = ALIGN(16);
	PROVIDE(_bss_start = .);
	.bss :
	{
		*(.bss*)
	}  > ram AT>ram
	PROVIDE(_bss_end = .);

	/* end */
	. = ALIGN(16);
	_end = .;
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
