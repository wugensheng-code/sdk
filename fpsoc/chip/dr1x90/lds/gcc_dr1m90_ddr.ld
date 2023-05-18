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
	}  > ram AT>ram

	/* all data segment */
	.data :
	{
		. = ALIGN(16);
		PROVIDE(_data_start = .);
		*(.data*)
		PROVIDE(_data_end = .);
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
