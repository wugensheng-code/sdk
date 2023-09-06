/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <al_aarch64_macro.h>
#include <al_mmu.h>

#ifdef ENABLE_MMU
	.extern MMUTableL0
	.extern MMUTableL1
	.extern MMUTableL2
	.extern _text_start
	.extern _pagetable_start
	.extern mmu_2m_list

	.globl enable_mmu
enable_mmu:
	mov	x15, x30
	ldr	x2, =0x1005
	mvn	x2, x2

	switch_el x3, 0f, 1f, 2f
0:
	mrs	x3, sctlr_el3
	and	x3, x3, x2
	msr	sctlr_el3, x3
	isb
	tlbi	ALLE3
	b	3f
1:
	mrs	x3, sctlr_el2
	and	x3, x3, x2
	msr	sctlr_el2, x3
	isb
	tlbi	ALLE2
	b	3f
2:
	mrs	x3, sctlr_el1
	and	x3, x3, x2
	msr	sctlr_el1, x3
	isb
	tlbi	VMALLE1
3:
	ic	IALLU
	bl	invalidate_dcaches
	dsb	sy
	isb

#if defined(DDR_2M_MAPPING) && defined(CODE_READONLY)
	ldr	x0, =_text_start
	ldr	x1, =_pagetable_start
	mov	x2, #0x200000
	sub	x3, x2, #1
	add	x0, x0, x3
	bic	x0, x0, x3
	bic	x3, x1, x3

	ldr	x4, =MMUTableL0
	ubfx	x5, x0, #30, #9
	add	x4, x4, x5, LSL #3
nextl0:
	ldr	x7, [x4], #8
	and	x5, x7, #3
	cmp	x5, #3
	bne	finish
	sub	x5, x7, #3
	add	x8, x5, #0x1000
	ubfx	x6, x0, #21, #9
	add	x5, x5, x6, LSL #3
nextl1:
	cmp	x5, x8
	bge	nextl0
	cmp	x0, x3
	bge	l2
	ldr	x7, [x5]
	orr	x7, x7, #0x80
	str	x7, [x5], #8
	add	x0, x0, x2
	b	nextl1
l2:
	cmp	x3, x1
	beq	finish
	ldr	x6, =MMUTableL2
	add	x8, x6, #0x1000
	add	x7, x6, #3
	str	x7, [x5]
nextl2ro:
	cmp	x6, x8
	bge	finish
	add	x5, x0, #(NORM_RO_CACHE+2)
	str	x5, [x6], #8
	add	x0, x0, #0x1000
	cmp	x0, x1
	blt	nextl2ro
nextl2:
	cmp	x6, x8
	bge	finish
	add	x5, x0, #(NORM_CACHE+2)
	str	x5, [x6], #8
	add	x0, x0, #0x1000
	b	nextl2
finish:
#endif

	ldr	x0, =MMUTableL0
	ldr	x1, =0x000000BB0400FF44
	ldr	x2, =0x1005

	switch_el x3, mmu_el3, mmu_el2, mmu_el1
mmu_el3:
	msr     TTBR0_EL3, x0
	msr     MAIR_EL3, x1
	ldr     x0,=0x80823519
	msr     TCR_EL3, x0
	mrs	x0, sctlr_el3
	orr	x0, x0, x2
	msr	sctlr_el3, x0
	isb
	b	0f
mmu_el2:
	msr     TTBR0_EL2, x0
	msr     MAIR_EL2, x1
	ldr     x0,=0x80823519
	msr     TCR_EL2, x0
	mrs	x0, sctlr_el2
	orr	x0, x0, x2
	msr	sctlr_el2, x0
	isb
	b	0f

mmu_el1:
	msr     TTBR0_EL1, x0
	msr     MAIR_EL1, x1
	ldr     x0,=0x280803519
	msr     TCR_EL1, x0
	mrs	x0, sctlr_el1
	orr	x0, x0, x2
	msr	sctlr_el1, x0
	isb

0:
	mov	x30, x15
	ret


invalidate_dcaches:
	dmb     ISH
	mrs     x0, CLIDR_EL1
	ubfx    w2, w0, #24, #3        // w2 = CLIDR.LoC
	cmp     w2, #0                 // LoC is 0?
	b.eq    invalidateCaches_end   // No cleaning required and enable MMU
	mov     w1, #0                 // w1 = level iterator

invalidateCaches_flush_level:
	add     w3, w1, w1, lsl #1     // w3 = w1 * 3 (right-shift for cache type)
	lsr     w3, w0, w3             // w3 = w0 >> w3
	ubfx    w3, w3, #0, #3         // w3 = cache type of this level
	cmp     w3, #2                 // No cache at this level?
	b.lt    invalidateCaches_next_level

	lsl     w4, w1, #1
	msr     CSSELR_EL1, x4         // Select current cache level in CSSELR
	isb                            // ISB required to reflect new CSIDR
	mrs     x4, CCSIDR_EL1         // w4 = CSIDR

	ubfx    w3, w4, #0, #3
	add    	w3, w3, #2             // w3 = log2(line size)
	ubfx    w5, w4, #13, #15
	ubfx    w4, w4, #3, #10        // w4 = Way number
	clz     w6, w4                 // w6 = 32 - log2(number of ways)

invalidateCaches_flush_set:
	mov     w8, w4                 // w8 = Way number
invalidateCaches_flush_way:
	lsl     w7, w1, #1             // Fill level field
	lsl     w9, w5, w3
	orr     w7, w7, w9             // Fill index field
	lsl     w9, w8, w6
	orr     w7, w7, w9             // Fill way field
	dc      CISW, x7               // Invalidate by set/way to point of coherency
	subs    w8, w8, #1             // Decrement way
	b.ge    invalidateCaches_flush_way
	subs    w5, w5, #1             // Descrement set
	b.ge    invalidateCaches_flush_set

invalidateCaches_next_level:
	add     w1, w1, #1             // Next level
	cmp     w2, w1
	b.gt    invalidateCaches_flush_level

invalidateCaches_end:
	ret

	.globl mmu_settlb
mmu_settlb:
	mov	x13, x30
	lsr	x0, x0, #21
	lsl	x0, x0, #21
	ldr	x2, =mmu_2m_list
0:
	ldr	x4, [x2], #8
	ldr	x5, [x2], #8
	ldr	x6, [x2], #8
	cmp	x6, #0
	beq	block4g
	cmp	x0, x4
	blt	0b
	add	x6, x6, x4
	cmp	x0, x6
	bge	0b
	sub	x6, x0, x4
	add	x5, x5, x6, LSR #18
	orr	x6, x0, x1
	str	x6, [x5]
	bl	__asm_flush_dcache_all
	bl	__asm_invalidate_tlb_all
	mov	x0, #0x200000
	b	mmu_settlb_exit
block4g:
	lsr	x0, x0, #30
	lsl	x0, x0, #30
	ldr	x2, =MMUTableL0
	ubfx	x3, x0, #27, #12
	add	x3, x2, x3
	orr	x4, x0, x1
	str	x4, [x3]
	bl	__asm_flush_dcache_all
	bl	__asm_invalidate_tlb_all
	mov	x0, #0x40000000
mmu_settlb_exit:
	mov	x30, x13
	ret
#else
	.globl mmu_settlb
mmu_settlb:
	ret
#endif

/*
 * void __asm_dcache_level(level)
 *
 * flush or invalidate one level cache.
 *
 * x0: cache level
 * x1: 0 clean & invalidate, 1 invalidate only
 * x2~x9: clobbered
 */
__asm_dcache_level:
	lsl	x12, x0, #1
	msr	csselr_el1, x12		/* select cache level */
	isb				/* sync change of cssidr_el1 */
	mrs	x6, ccsidr_el1		/* read the new cssidr_el1 */
	and	x2, x6, #7		/* x2 <- log2(cache line size)-4 */
	add	x2, x2, #4		/* x2 <- log2(cache line size) */
	mov	x3, #0x3ff
	and	x3, x3, x6, lsr #3	/* x3 <- max number of #ways */
	clz	w5, w3			/* bit position of #ways */
	mov	x4, #0x7fff
	and	x4, x4, x6, lsr #13	/* x4 <- max number of #sets */
	/* x12 <- cache level << 1 */
	/* x2 <- line length offset */
	/* x3 <- number of cache ways - 1 */
	/* x4 <- number of cache sets - 1 */
	/* x5 <- bit position of #ways */

loop_set:
	mov	x6, x3			/* x6 <- working copy of #ways */
loop_way:
	lsl	x7, x6, x5
	orr	x9, x12, x7		/* map way and level to cisw value */
	lsl	x7, x4, x2
	orr	x9, x9, x7		/* map set number to cisw value */
	tbz	w1, #0, 1f
	dc	isw, x9
	b	2f
1:	dc	cisw, x9		/* clean & invalidate by set/way */
2:	subs	x6, x6, #1		/* decrement the way */
	b.ge	loop_way
	subs	x4, x4, #1		/* decrement the set */
	b.ge	loop_set

	ret

/*
 * void __asm_flush_dcache_all(int invalidate_only)
 *
 * x0: 0 clean & invalidate, 1 invalidate only
 *
 * flush or invalidate all data cache by SET/WAY.
 */
__asm_dcache_all:
	mov	x1, x0
	dsb	sy
	mrs	x10, clidr_el1		/* read clidr_el1 */
	lsr	x11, x10, #24
	and	x11, x11, #0x7		/* x11 <- loc */
	cbz	x11, finished		/* if loc is 0, exit */
	mov	x15, lr
	mov	x0, #0			/* start flush at cache level 0 */
	mrs	x14, DAIF
	orr	x2, x14, #0xc0
	msr	DAIF, x2
	/* x0  <- cache level */
	/* x10 <- clidr_el1 */
	/* x11 <- loc */
	/* x15 <- return address */

loop_level:
	lsl	x12, x0, #1
	add	x12, x12, x0		/* x0 <- tripled cache level */
	lsr	x12, x10, x12
	and	x12, x12, #7		/* x12 <- cache type */
	cmp	x12, #2
	b.lt	skip			/* skip if no cache or icache */
	bl	__asm_dcache_level	/* x1 = 0 flush, 1 invalidate */
skip:
	add	x0, x0, #1		/* increment cache level */
	cmp	x11, x0
	b.gt	loop_level

	mov	x0, #0
	msr	csselr_el1, x0		/* restore csselr_el1 */
	dsb	sy
	isb
	msr	DAIF, x14
	mov	lr, x15

finished:
	ret

	.globl __asm_flush_dcache_all
__asm_flush_dcache_all:
	mov	x0, #0
	b	__asm_dcache_all

	.globl __asm_invalidate_dcache_all
__asm_invalidate_dcache_all:
	mov	x0, #0x1
	b	__asm_dcache_all

/*
 * void __asm_flush_dcache_range(start, end)
 *
 * clean & invalidate data cache in the same range
 *
 * x0: start address
 * x1: end address
 */
	.globl __asm_flush_dcache_range
__asm_flush_dcache_range:
	mrs	x3, ctr_el0
	lsr	x3, x3, #16
	and	x3, x3, #0xf
	mov	x2, #4
	lsl	x2, x2, x3		/* cache line size */
	mrs	x14, DAIF
	orr	x13, x14, #0xc0
	msr	DAIF, x13

	/* x2 <- minimal cache line size in cache system */
	sub	x3, x2, #1
	bic	x0, x0, x3
1:	dc	civac, x0	/* clean & invalidate data or unified cache */
	add	x0, x0, x2
	cmp	x0, x1
	b.lo	1b
	dsb	sy
	msr	DAIF, x14
	ret
/*
 * void __asm_invalidate_dcache_range(start, end)
 *
 * invalidate data cache in the range
 *
 * x0: start address
 * x1: end address
 */
	.globl __asm_invalidate_dcache_range
__asm_invalidate_dcache_range:
	mrs	x3, ctr_el0
	ubfm	x3, x3, #16, #19
	mov	x2, #4
	lsl	x2, x2, x3		/* cache line size */
	mrs	x14, DAIF
	orr	x13, x14, #0xc0
	msr	DAIF, x13

	/* x2 <- minimal cache line size in cache system */
	sub	x3, x2, #1
	bic	x0, x0, x3
1:	dc	ivac, x0	/* invalidate data or unified cache */
	add	x0, x0, x2
	cmp	x0, x1
	b.lo	1b
	dsb	sy
	msr	DAIF, x14
	ret

/*
 * void __asm_flush_and_invalidate_dcache_range(f_start, f_end, i_start, i_end)
 *
 * clean & invalidate data cache in the different range
 *
 * x0: start address
 * x1: end address
 */
	.globl __asm_flush_and_invalidate_dcache_range
__asm_flush_and_invalidate_dcache_range:
	mrs	x5, ctr_el0
	lsr	x5, x5, #16
	and	x5, x5, #0xf
	mov	x4, #4
	lsl	x4, x4, x5		/* cache line size */
	mrs	x14, DAIF
	orr	x13, x14, #0xc0
	msr	DAIF, x13

	/* x4 <- minimal cache line size in cache system */
	sub	x5, x4, #1
	bic	x0, x0, x5
1:	dc	cvac, x0	/* clean data or unified cache */
	add	x0, x0, x4
	cmp	x0, x1
	b.lo	1b
	bic x2, x2, x5
2:	dc ivac, x2
	add x2, x2, x4
	cmp x2, x3
	b.lo	2b
	dsb	sy
	msr	DAIF, x14
	ret

/*
 * void __asm_invalidate_icache_all(void)
 *
 * invalidate all tlb entries.
 */
	.globl __asm_invalidate_icache_all
__asm_invalidate_icache_all:
	ic	ialluis
	isb	sy
	ret

	.globl __asm_invalidate_tlb_all
__asm_invalidate_tlb_all:
	switch_el x9, 3f, 2f, 1f
3: 	tlbi alle3
	dsb sy
	isb
	b 0f
2: 	tlbi alle2
	dsb sy
	isb
	b 0f
1: 	tlbi vmalle1
	dsb sy
	isb
0:
	ret
.end
