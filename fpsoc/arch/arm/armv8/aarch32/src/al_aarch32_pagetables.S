#ifdef ENABLE_MMU
#include "al_mmu.h"
	.globl MMUTableL0
	.globl MMUTableL1
	.globl mmu_4k_list

	.section .page_table,"a"
	.balign 0x4000
MMUTableL0:
	.set SECT, 0x00000000
	.rept (0x61100000-SECT)/0x100000
	.4byte SECT + NORM_CACHE		/* 0x00000000 - 0x61100000 */
	.set SECT, SECT + 0x100000
	.endr

	.rept (0x63E00000-SECT)/0x100000
	.4byte SECT + RESERVED			/* 0x61100000 - 0x63E00000 */
	.set SECT, SECT + 0x100000
	.endr

	.rept (0x63F00000-SECT)/0x100000
	.4byte SECT + NORM_NONCACHE		/* 0x63E00000 - 0x63F00000 */
	.set SECT, SECT + 0x100000
	.endr

	.rept (0x64000000-SECT)/0x100000
	.4byte SECT + DEVICE_MEM		/* 0x63F00000 - 0x64000000 */
	.set SECT, SECT + 0x100000
	.endr

	.rept (0x65000000-SECT)/0x100000
	.4byte SECT + DEVICE_MEM		/* 0x64000000 - 0x65000000 */
	.set SECT, SECT + 0x100000
	.endr

	.rept (0x70000000-SECT)/0x100000
	.4byte SECT + RESERVED			/* 0x65000000 - 0x70000000 */
	.set SECT, SECT + 0x100000
	.endr

	.rept (0x80000000-SECT)/0x100000
	.4byte SECT + NORM_CACHE		/* 0x70000000 - 0x80000000 */
	.set SECT, SECT + 0x100000
	.endr

	.rept ((0xFFFFFFFF-SECT)/0x100000+1)
	.4byte SECT + DEVICE_MEM		/* 0x80000000 - 0x100000000 */
	.set SECT, SECT + 0x100000
	.endr

#if defined(CODE_READONLY)
	.balign 0x400
MMUTableL1:
	.rept 256
	.4byte 0
	.endr
#endif

mmu_4k_list:
	.long 0, 0, 0
	.long 0, 0, 0
#endif
.end
