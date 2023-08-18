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

    mrs x0, sctlr_el1
    orr x0, x0, #(1 << 12)  //enable icache
    msr sctlr_el1, x0

    b main_entry

main_entry:
    isb

#ifdef ENABLE_MMU
    bl enable_mmu
#endif

#ifndef SUPPORT_NONSECURE
    bl clear_bss
#endif

    bl board_init

    /* Processor specific initialization */
    bl _premain_init

    /* jump to main */

    /* Call entry function when using RT-Thread */
    bl  entry

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
