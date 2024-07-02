/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

//#include "al_aarch32_macro.h"
#include "al_mmu.h"

	.section .text

#ifdef ENABLE_MMU
	.globl enable_mmu
enable_mmu:
	mrc	p15, 0, r1, c1, c0, 0
	bic	r0, r1, #0x00000005
	mcr	p15, 0, r0, c1, c0, 0
	mov	r6, lr
	bl	_invalidate_dcache_all

#if defined(CODE_READONLY)
	ldr	r0, =(_text_start + 0xFFFFF)
	ldr	r1, =_rodata_end
	mov	r2, #0x100000
	sub	r3, r2, #1
	bic	r0, r0, r3
	bic	r5, r1, r3

	ldr	r4, =MMUTableL0
	add	r4, r4, r0, lsr #18
1:
	cmp	r0, r5
	bge	2f
	ldr	r3, [r4]
	orr	r3, r3, #0x8000
	str	r3, [r4], #4
	add	r0, r0, r2
	b	1b
2:
	mov	r2, #0x1000
	sub	r3, r2, #1
	bic	r5, r1, r3
	cmp	r0, r5
	bge	finish
	ldr	r3, =MMUTableL1
	orr	r1, r3, #0x21
	str	r1, [r4]
	mov	r4, #0x100000
	ldr	r1, =mmu_4k_list
	str	r0, [r1], #4
	str	r3, [r1], #4
	str	r4, [r1], #4
	add	r4, r0, #0x100000
	ldr	r1, =0x776
	orr	r1, r1, r0
3:
	str	r1, [r3], #4
	add	r1, r1, r2
	add	r0, r0, r2
	cmp	r0, r5
	blt	3b
	cmp	r0, r4
	bge	finish
	bic	r1, r1, #0x200
	mov	r5, r4
	b	3b
finish:
#endif

	mov	r0, #0
	mcr	p15, 0, r0, c2, c0, 2		/* ttbcr */

	mov	r0, #7
	mcr	p15, 0, r0, c3, c0, 0		/* dacr */

	ldr	r0, =MMUTableL0
	orr	r0, r0, #0x5B			/* Outer-cacheable, WB */
	mcr	p15, 0, r0, c2, c0, 0		/* ttbr0 */

	mrc	p15, 0, r1, c1, c0, 0
	bic	r0, r1, #0x30000000		/* AFE, TRE */
	bic	r0, r0, #0x00180000		/* UWXN, WXN */
	orr	r0, r0, #0x00000005		/* C, M */
	mcr	p15, 0, r0, c1, c0, 0		/* sctlr */
	isb
	dsb
	bx	r6

	.globl mmu_settlb
mmu_settlb:
	stmfd	sp!, {r4-r11, lr}
	ldr	r3, =mmu_4k_list
1:
	ldmia	r3!, {r2, r4, r5}
	cmp	r5, #0
	beq	2f
	cmp	r0, r2
	blt	1b
	add	r5, r2, r5
	cmp	r0, r5
	bge	1b
	bfc	r0, #0, #12
	sub	r2, r0, r2
	add	r4, r4, r2, lsr #10
	lsr	r2, r1, #4
	bfc	r2, #1, #31
	lsr	r1, r1, #1
	bfi	r2, r1, #1, #3
	lsr	r1, r1, #9
	bfi	r2, r1, #4, #8
	orr	r2, r2, r0
	str	r2, [r4]
	bl	_flush_dcache_all
	bl	__asm_invalidate_tlb_all
	mov	r0, #0x1000
	b	set_finish
2:
	bfc	r0, #0, #20
	ldr	r4, =MMUTableL0
	add	r4, r4, r0, lsr #18
	orr	r2, r1, r0
	str	r2, [r4]
	bl	_flush_dcache_all
	bl	__asm_invalidate_tlb_all
	mov	r0, #0x100000
set_finish:
	ldmfd	sp!, {r4-r11, lr}
	bx	lr

	.globl disable_mmu
disable_mmu:
	stmfd	sp!, {r4-r11, lr}
	mrc	p15, 0, r1, c1, c0, 0
	bic	r0, r1, #0x00000005
	mcr	p15, 0, r0, c1, c0, 0
	bl	_flush_dcache_all
	bl	__asm_invalidate_tlb_all
	ldmfd	sp!, {r4-r11, lr}
	bx	lr
#else
	.globl enable_mmu
enable_mmu:
	.globl disable_mmu
disable_mmu:
	.globl mmu_settlb
mmu_settlb:
	bx	lr
#endif

__invalidate_dcache:
	cmp	r0, #0
	beq	i_finished
	lsl	r0, r0, #1
	mrc	p15, 1, r3, c0, c0, 1		/* read CLIDR */
	mov	r10, #0				/* start with level 0 */
i_loop1:
	add	r2, r10, r10, lsr #1		/* work out 3xcachelevel */
	mov	r1, r3, lsr r2			/* bottom 3 bits are the Cache type for this level */
	and	r1, r1, #7			/* get those 3 bits alone */
	cmp	r1, #2
	blt	i_skip				/* no cache or only instruction cache at this level */
	mcr	p15, 2, r10, c0, c0, 0		/* write the Cache Size selection register */
	isb					/* isb to sync the change to the CacheSizeID reg */
	mrc	p15, 1, r1, c0, c0, 0		/* reads current Cache Size ID register */
	and	r2, r1, #7			/* extract the line length field */
	add	r2, r2, #4			/* add 4 for the line length offset (log2 16 bytes) */
	ldr	r4, =0x3ff
	ands	r4, r4, r1, lsr #3		/* r4 is the max number on the way size (right aligned) */
	clz	r5, r4				/* r5 is the bit position of the way size increment */
	ldr	r7, =0x7fff
	ands	r7, r7, r1, lsr #13		/* r7 is the max number of the index size (right aligned) */
i_loop2:
	mov	r9, r4				/* r9 working copy of the max way size (right aligned) */
i_loop3:
	orr	r11, r10, r9, lsl r5		/* factor in the way number and cache number into r11 */
	orr	r11, r11, r7, lsl r2		/* factor in the index number */
	mcr	p15, 0, r11, c7, c6, 2		/* invalidate by set/way */
	subs	r9, r9, #1			/* decrement the way number */
	bge	i_loop3
	subs	r7, r7, #1			/* decrement the index */
	bge	i_loop2
i_skip:
	add	r10, r10, #2			/* increment the cache number */
	cmp	r0, r10
	bgt	i_loop1

i_finished:
	mov	r10, #0				/* switch back to cache level 0 */
	mcr	p15, 2, r10, c0, c0, 0		/* select current cache level in cssr */
	dsb
	isb

	bx	lr

__flush_dcache:
	cmp	r0, #0
	beq	f_finished
	lsl	r0, r0, #1
	mrc	p15, 1, r3, c0, c0, 1		/* read CLIDR */
	mov	r10, #0				/* start with level 0 */
f_loop1:
	add	r2, r10, r10, lsr #1		/* work out 3xcachelevel */
	mov	r1, r3, lsr r2			/* bottom 3 bits are the Cache type for this level */
	and	r1, r1, #7			/* get those 3 bits alone */
	cmp	r1, #2
	blt	f_skip				/* no cache or only instruction cache at this level */
	mcr	p15, 2, r10, c0, c0, 0		/* write the Cache Size selection register */
	isb					/* isb to sync the change to the CacheSizeID reg */
	mrc	p15, 1, r1, c0, c0, 0		/* reads current Cache Size ID register */
	and	r2, r1, #7			/* extract the line length field */
	add	r2, r2, #4			/* add 4 for the line length offset (log2 16 bytes) */
	ldr	r4, =0x3ff
	ands	r4, r4, r1, lsr #3		/* r4 is the max number on the way size (right aligned) */
	clz	r5, r4				/* r5 is the bit position of the way size increment */
	ldr	r7, =0x7fff
	ands	r7, r7, r1, lsr #13		/* r7 is the max number of the index size (right aligned) */
f_loop2:
	mov	r9, r4				/* r9 working copy of the max way size (right aligned) */
f_loop3:
	orr	r11, r10, r9, lsl r5		/* factor in the way number and cache number into r11 */
	orr	r11, r11, r7, lsl r2		/* factor in the index number */
	mcr	p15, 0, r11, c7, c14, 2		/* clean & invalidate by set/way */
	subs	r9, r9, #1			/* decrement the way number */
	bge	f_loop3
	subs	r7, r7, #1			/* decrement the index */
	bge	f_loop2
f_skip:
	add	r10, r10, #2			/* increment the cache number */
	cmp	r0, r10
	bgt	f_loop1

f_finished:
	mov	r10, #0				/* switch back to cache level 0 */
	mcr	p15, 2, r10, c0, c0, 0		/* select current cache level in cssr */
	dsb
	isb

	bx	lr

_invalidate_dcache_all:
	dmb
	mrc	p15, 1, r3, c0, c0, 1		/* read CLIDR */
	mov	r0, r3, lsr #24
	and	r0, r0, #7
	b	__invalidate_dcache

_flush_dcache_all:
	dmb
	mrc	p15, 1, r3, c0, c0, 1		/* read CLIDR */
	mov	r0, r3, lsr #24
	and	r0, r0, #7
	b	__flush_dcache

_flush_dcache_range:
	dmb
	mrc	p15, 1, r3, c0, c0, 0
	and	r3, r3, #7
	mov	r2, #16
	lsl	r2, r2, r3			/* line size */

	sub	r3, r2, #1
	bic	r0, r3
1:
	cmp	r0, r1
	bge	2f
	mcr	p15, 0, r0, c7, c14, 1		/* clean & invalidate by va */
	add	r0, r0, r2
	b	1b
2:
	dsb	sy
	isb
	bx	lr

_invalid_dcache_range:
	dmb
	mrc	p15, 1, r3, c0, c0, 0
	and	r3, r3, #7
	mov	r2, #16
	lsl	r2, r2, r3			/* line size */

	sub	r3, r2, #1
	bic	r0, r3
1:
	cmp	r0, r1
	bge	2f
	mcr	p15, 0, r0, c7, c6, 1		/* invalidate by va */
	add	r0, r0, r2
	b	1b
2:
	dsb	sy
	isb
	bx	lr

	.globl __asm_invalidate_dcache_all
__asm_invalidate_dcache_all:
	stmfd	sp!, {r4-r11, lr}
	mrs	r6, cpsr
	cpsid	aif
	bl	_invalidate_dcache_all
	msr	cpsr_c, r6
	ldmfd	sp!, {r4-r11, lr}
	bx	lr

	.globl __asm_flush_dcache_all
__asm_flush_dcache_all:
	stmfd	sp!, {r4-r11, lr}
	mrs	r6, cpsr
	cpsid	aif
	bl	_flush_dcache_all
	msr	cpsr_c, r6
	ldmfd	sp!, {r4-r11, lr}
	bx	lr

/*
 * void __asm_flush_dcache_range(start, end)
 *
 * clean data cache in the range
 *
 * r0: start address
 * r1: end address
 */
	.globl __asm_flush_dcache_range
__asm_flush_dcache_range:
	stmfd	sp!, {r4-r11, lr}
	mrs	r6, cpsr
	cpsid	aif
	bl	_flush_dcache_range
	msr	cpsr_c, r6
	ldmfd	sp!, {r4-r11, lr}
	bx	lr

/*
 * void __asm_invalidate_dcache_range(start, end)
 *
 * invalidate data cache in the range
 *
 * r0: start address
 * r1: end address
 */
	.globl __asm_invalidate_dcache_range
__asm_invalidate_dcache_range:
	stmfd	sp!, {r4-r11, lr}
	mrs	r6, cpsr
	cpsid	aif
	bl	_invalid_dcache_range
	msr	cpsr_c, r6
	ldmfd	sp!, {r4-r11, lr}
	bx	lr

	.globl __asm_invalidate_tlb_all
__asm_invalidate_tlb_all:
	mcr p15, 0, r0, c8, c7, 0	/* TLBIALL  */
	mcr p15, 0, r0, c8, c6, 0	/* DTLBIALL */
	mcr p15, 0, r0, c8, c5, 0	/* ITLBIALL */
	dsb
	isb
	bx	lr

/*
 * void __asm_flush_and_invalidate_same_dcache_range(start, end)
 *
 * clean & invalidate data cache in the same range
 *
 * r0: start address
 * r1: end address
 */
	.globl __asm_flush_and_invalidate_same_dcache_range
__asm_flush_and_invalidate_same_dcache_range:
	stmfd	sp!, {r4-r11, lr}
	mrs	r6, cpsr
	cpsid	aif
	mov r2, r0
	mov r3, r1
	bl	_flush_dcache_range
	mov r0, r2
	mov r1, r3
	bl	_invalid_dcache_range
	msr	cpsr_c, r6
	ldmfd	sp!, {r4-r11, lr}
	bx	lr

/*
 * void __asm_flush_and_invalidate_same_dcache_range(start, end)
 *
 * clean & invalidate data cache in the same range
 *
 * r0: flush start address
 * r1: flush end address
 * r2: invalidate start address
 * r3: invalidate end address
 */
	.globl __asm_flush_and_invalidate_diff_dcache_range
__asm_flush_and_invalidate_diff_dcache_range:
	stmfd	sp!, {r4-r11, lr}
	mrs	r6, cpsr
	cpsid	aif
	bl	_flush_dcache_range
	mov r0, r2
	mov r1, r3
	bl	_invalid_dcache_range
	msr	cpsr_c, r6
	ldmfd	sp!, {r4-r11, lr}
	bx	lr

/*
 * void __asm_invalidate_icache_all(void)
 *
 * invalidate all tlb entries.
 */
	.globl __asm_invalidate_icache_all
__asm_invalidate_icache_all:
	mcr p15, 0, r0, c7, c5, 0	/* ICIALLU  */
	mcr p15, 0, r0, c7, c5, 6	/* BPIALL   */
	dsb
	isb
	bx	lr

	.globl psci_cpu_pwrdown
psci_cpu_pwrdown:
    cpsid   aif
    mrc     p15, 0, r1, c1, c0, 0
    bic     r0, r1, #0x00000005
    mcr     p15, 0, r0, c1, c0, 0
    mov     r0, #1
    bl      __flush_dcache
    isb
    dsb     sy
0:  wfe
    b       0b

.end

