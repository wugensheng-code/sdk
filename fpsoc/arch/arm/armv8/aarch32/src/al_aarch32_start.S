/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <al_aarch32_sysreg.h>
#include <asm_macros_common.S>

    .globl    vectors
    .globl    reset_handler            /* Reset handler*/

.section .aarch64_start, "ax"
.global aarch64_start
aarch64_start:
    .incbin "aarch64_start.bin"

vector_base vectors
    b    reset_handler              /* Reset handler*/
    b    undefined_handler          /* Undef */
    b    svc_handler                /* Syscall */
    b    prefetch_abort_handler     /* Prefetch abort */
    b    data_abort_handler         /* Data abort */
    b    panic_handler              /* Reserved */
    b    irq_handler                /* IRQ */
    b    fiq_handler                /* FIQ */


func reset_handler
    cps #MODE32_svc     // Change to svc mode.

    mov r0, #0
    mov r1, #0
    mov r2, #0
    mov r3, #0
    mov r4, #0
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    mov r9, #0
    mov r10, #0
    mov r11, #0
    mov r12, #0
    mov r13, #0
    mov r14, #0

    cps #MODE32_fiq        // Change to FIQ mode.
    mov r8, #0
    mov r9, #0
    mov r10, #0
    mov r11, #0
    mov r12, #0
    mov r13, #0
    mov r14, #0

    cps #MODE32_irq    // Change to IRQ mode.
    mov r13, #0
    mov r14, #0

#if 0
    cps #MODE32_sys        // Change to System mode.
    mov r13, #0            // System and User modes reuse the same banking
    mov r14, #0            // of r13 and r14.

    cps #MODE32_und        // Change to Undef mode.
    mov r13, #0
    mov r14, #0

    cps #MODE32_abt        // Change to Abort mode.
    mov r13, #0
    mov r14, #0
#endif

    ldr r1, =0x1000       @stack size for each mode = 4KB
    ldr r3, =stack_top
    cps #MODE32_irq        // Change to irq mode.
    mov sp, r3

    sub r3, r3, r1
    cps #MODE32_fiq        // Change to fiq mode.
    mov sp, r3

    sub r3, r3, r1
    cps #MODE32_svc        // Change to svc mode.
    mov sp, r3

/*
    * Todo:
*/
#if 0
    add r3, r3, r1
    cps #MODE32_abt        // Change to abort mode.
    mov sp, r3

    add r3, r3, r1
    cps #MODE32_und        // Change to undef mode.
    mov sp, r3

    add r3, r3, r1
    cps #MODE32_mon        // Change to monitor mode.
    mov sp, r3

#endif

#if 0
    /*reserve some 128 bytes for exception handler.*/
    ldr r3, =stack_start - 128
    cps #MODE32_sys        // Change to system mode.
    mov sp, r3
#endif

    // Enable access to NEON/FP by enabling access to Coprocessors 10 and 11.
    // Enable Full Access in both privileged and non-privileged modes.
    mov r0, #(0xf << 20)
    mcr p15, 0, r0, c1, c0, 2
    isb

    // switch on the fp and neon hardware.
    mov r1, #(0x1 << 30)
    vmsr fpexc, r1

    ldr r0, =(SCTLR_RESET_VAL & ~(SCTLR_TE_BIT | SCTLR_EE_BIT | \
             SCTLR_V_BIT | SCTLR_DSSBS_BIT))
    //stcopr    r0, SCTLR
    isb

    /* Switch to svc mode */
    cps    #MODE32_svc
    isb

    /* ---------------------------------------------------------------------
     * Set the exception vectors (VBAR/MVBAR).
     * ---------------------------------------------------------------------
     */
    ldr    r0, =vectors
    mcr p15, 0, r0, c12, c0, 0      //VBAR
    isb

    mov r0, #0x07
    mcr p15, 0, r0, c12, c12, 5     //icc_sre

    ldr    r1, =(SCTLR_I_BIT | SCTLR_A_BIT)

    mrc     p15, 0, r0, c1, c0, 0
    orr     r0, r0, r1
    mcr     p15, 0, r0, c1, c0, 0
    isb

#ifdef ENABLE_MMU
    bl	enable_mmu
#endif

#if 0
    ldr    r0, =(SDCR_RESET_VAL | SDCR_SPD(SDCR_SPD_DISABLE) | SDCR_SCCD_BIT)
    stcopr    r0, SDCR
#endif
    /* ---------------------------------------------------------------------
     * Initialise PMCR, setting all fields rather than relying
     * on hw. Some fields are architecturally UNKNOWN on reset.
     *
     * PMCR.LP: Set to one so that event counter overflow, that
     *  is recorded in PMOVSCLR[0-30], occurs on the increment
     *  that changes PMEVCNTR<n>[63] from 1 to 0, when ARMv8.5-PMU
     *  is implemented. This bit is RES0 in versions of the architecture
     *  earlier than ARMv8.5, setting it to 1 doesn't have any effect
     *  on them.
     *  This bit is Reserved, UNK/SBZP in ARMv7.
     *
     * PMCR.LC: Set to one so that cycle counter overflow, that
     *  is recorded in PMOVSCLR[31], occurs on the increment
     *  that changes PMCCNTR[63] from 1 to 0.
     *  This bit is Reserved, UNK/SBZP in ARMv7.
     *
     * PMCR.DP: Set to one to prohibit cycle counting whilst in Secure mode.
     * ---------------------------------------------------------------------
     */
    ldr     r0, =(PMCR_RESET_VAL | PMCR_DP_BIT | PMCR_LC_BIT | \
              PMCR_LP_BIT)
    mcr     p15, 0, r0, c9, c12, 0

    ldr     r0, =_bss_start
    ldr     r1, =_bss_end
    sub     r1, r1, r0
    bl      clear_mem

    /*
     * call c maini function.
     */
    bl      AlChip_Init

    bl      board_init

    /* Call entry function when using RT-Thread */
    bl      entry

    bl      components_init

    bl      main

    b .

endfunc reset_handler

.weak board_init
board_init:
    nop
    bx lr

.weak entry
entry:
    nop
    bx lr
