/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_aarch64_macro.h"
#include "al_aarch64_sysreg.h"

.align 3

.extern clear_bss
.extern AlGic_Init
.extern AlSys_StartTimer

.globl _start
.globl vectors
.globl _sp
.globl SystemCoreClock

.set vector_base, vectors


/* Initial State: MMU Disabled, i/dCache Disabled */
_start:

    /* disable intr */
    msr daifset, 0xF
    isb
    /* disable mmu */
    tlbi alle3
    ic iallu
    dsb sy
    isb
    mrs x0, sctlr_el3
    ldr x1, =0x1005
    bic x0, x0, x1
    msr sctlr_el3, x0
    isb
    /* init register */
    mov x0, #0
	mov x1, #0
	mov x2, #0
	mov x3, #0
	mov x4, #0
	mov x5, #0
	mov x6, #0
	mov x7, #0
	mov x8, #0
	mov x9, #0
	mov x10, #0
	mov x11, #0
	mov x12, #0
	mov x13, #0
	mov x14, #0
	mov x15, #0
	mov x16, #0
	mov x17, #0
	mov x18, #0
	mov x19, #0
	mov x20, #0
	mov x21, #0
	mov x22, #0
	mov x23, #0
	mov x24, #0
	mov x25, #0
	mov x26, #0
	mov x27, #0
	mov x28, #0
	mov x29, #0
	mov x30, #0
    b setup_vector

/* set up vector table */
setup_vector:
    /* Gets the current exception level, and jumps to the corresponding label to execute
     * x0 = CurrentEL，
     * 0xc --> el3_entry
     * 0x8 --> el2_entry
     * 0x4 --> el1_entry
    **/
    switch_el x0, el3_entry, el2_entry, el1_entry

    b .

/* execute el3 */
el3_entry:

    /* Set vector table base address */
    adr x0, vector_base
    msr vbar_el3, x0

    /* set up stack point */
    ldr x28, =_sp
    mov sp, x28

    /* Enable FP/SIMD */
    msr cptr_el3, xzr

    /* SMP EN */
    mrs x0, S3_1_c15_c2_1
	orr x0, x0, #0x40
	msr S3_1_c15_c2_1, x0
	isb

    /* enable hardware coherency between cores */
    mov x0, #(0x01 << 6)
    msr actlr_el3, x0    //Enable L2ACTLR write access from EL2
    msr actlr_el2, x0    //Enable L2ACTLR write access from Non-secure EL1

    /* set icc_sre_el3 */
    mov x0, #(0x1 << 0)         //SRE, Enable system register
    orr x0, x0, #(0x1 << 1)     //DFB, Disable FIQ bypass
    orr x0, x0, #(0x1 << 2)     //DIB, Disable IRQ bypass
    orr x0, x0, #(0x1 << 3)     //Enable, Enable lower EL access SRE_EL1 and SRE_EL2
    msr S3_6_c12_c12_5, x0
    isb

    /* set icc_igrpen1_el3 */
    mov x0, #(0x1 << 0)         //Enable group 1 interrupts for Non-secure state
    orr x0, x0, #(0x1 << 1)     //Enable group 1 interrupts for Secure state
    msr S3_6_c12_c12_7, x0

    /* set icc_igrpen0_el1 */
    mov x0, #(0x1 << 0)         //Enable group 0 interrupts
    msr S3_0_c12_c12_6, x0

    /* Set counter frequency */
    ldr    x0, =SystemCoreClock
    ldr    x0, [x0]
    msr    cntfrq_el0, x0

    mrs    x0, scr_el3
    orr    x0, x0, #(0x1 << 1)      /* Enable IRQ */
    orr    x0, x0, #(0x1 << 2)      /* Enable FIQ */
    orr    x0, x0, #(0x1 << 3)      /* Enable EA */
    bic    x0, x0, #0x1             /* clear scr_el3.NS */
    msr    scr_el3, x0

#ifdef SUPPORT_NONSECURE
    bl clear_bss
    bl AlGic_Init
    bl AlSys_StartTimer
#endif

/* switch to el1 */
#ifdef SWITCH_TO_EL1_EL0_FROM_EL3
    switch_el3_to_el1_el0
#endif

    b   main_entry

.align 3

/* el2 */
el2_entry:
    adr x0, vector_base
    msr vbar_el2, x0

    /* Enable FP/SIMD */
    mov x0, #0x33ff
    msr cptr_el2, x0

    /* set up stack point */
    ldr x28, =_sp
    mov sp, x28

    b   main_entry

.align 3

/* el1 */
el1_entry:
    /* Set up stack point */
    ldr x28, =_sp
    mov sp, x28

    adr x0, vector_base
    msr vbar_el1, x0

    /* Enable FP/SIMD */
    mov x0, #3 << 20
    msr cpacr_el1, x0

#ifdef ENABLE_ICACHE
    mrs x0, sctlr_el1
    orr x0, x0, #(1 << 12)  //enable icache
    msr sctlr_el1, x0
#else
    mrs x0, sctlr_el1
    bic x0, x0, #(1 << 12)  //disable icache
    msr sctlr_el1, x0
#endif
    isb

    b main_entry

main_entry:

#ifdef ENABLE_ICACHE
    mrs x0, sctlr_el3
    orr x0, x0, #(1 << 12)  //enable icache
    msr sctlr_el3, x0
#else
    mrs x0, sctlr_el3
    bic x0, x0, #(1 << 12)  //disable icache
    msr sctlr_el3, x0
#endif
    isb

/*Enable MMU & Dcache*/
#ifdef ENABLE_MMU
    bl enable_mmu
#endif

#ifndef SUPPORT_NONSECURE
    bl clear_bss
#endif

    bl AlChip_Init

    bl board_init

    /* Call entry function when using RT-Thread */
    bl entry

    bl components_init

    bl main

    /* hang */
    b .

.global board_init
.weak board_init
board_init:
    nop
    ret

.weak entry
entry:
    nop
    ret

_psci_dcache_level:
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
1:  dc	cisw, x9		/* clean & invalidate by set/way */
2:  subs    x6, x6, #1		/* decrement the way */
    b.ge    loop_way
    subs    x4, x4, #1		/* decrement the set */
    b.ge    loop_set

    ret

.global psci_cpu_pwrdown
psci_cpu_pwrdown:
    msr daifset, #0xf
    mrs x0, sctlr_el3
    movn x1, #(1 << 2)
    and x1, x0, x1
    msr sctlr_el3, x1
    mov x0, xzr
    mov x1, xzr
    bl  _psci_dcache_level
    isb
    dsb sy
0:  wfe
    b    0b
