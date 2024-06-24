
/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_aarch64_macro.h"

/********************************************|***************************
VBAR_ELn + 0x000   Synchronous               |
+ 0x080            IRQ/vIRQ                  |  Current EL with SP0
+ 0x100            FIQ/vFIQ                  |
+ 0x180            SError/vSError            |
*********************************************|***************************
+ 0x200            Synchronous               |
+ 0x280            IRQ/vIRQ                  |  Current EL with SPx
+ 0x300            FIQ/vFIQ                  |
+ 0x380            SError/vSError            |
*************************************************************************
+ 0x400            Synchronous               |
+ 0x480            IRQ/vIRQ                  |
+ 0x500            FIQ/vFIQ                  |  Lower EL using AArch64
+ 0x580            SError/vSError            |
*************************************************************************
+ 0x600            Synchronous               |
+ 0x680            IRQ/vIRQ                  |  Lower EL using AArch32
+ 0x700            FIQ/vFIQ                  |
+ 0x780            SError/vSError            |
***************************************************************************/

.align 11

.globl vectors
vectors:
    /* Current EL Synchronous exception with sp0 */
    .align 7
    /* store lr(x30) before entry exception */
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    bl  _exception_entry
    bl  do_bad_sync
    b   exception_exit

    /* Current EL irq exception with sp0 */
    .align 7
    /* store lr(x30) before entry exception */
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    bl  _exception_entry
    bl  do_bad_irq
    b   exception_exit

    /* Current EL fiq exception with sp0 */
    .align 7
    /* store lr(x30) before entry exception */
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    bl  _exception_entry
    bl  do_bad_fiq
    b   exception_exit

    /* Current EL Error with sp0 */
    .align 7
    /* store lr(x30) before entry exception */
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    bl  _exception_entry
    bl  do_bad_error
    b   exception_exit

    /* Current EL (SP_ELx) Synchronous Handler */
    .align 7
    /* store lr(x30) before entry exception */
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    bl  _exception_entry
    bl  do_sync_handle
    b   exception_exit

    /* Current EL (SP_ELx) irq Handler */
    .align 7
    /* store lr(x30) before entry exception */
    stp x29, x30, [sp, #-16]!
    bl  _exception_entry
    bl  do_irq_handle
    b   exception_exit

     /* Current EL (SP_ELx) FIQ Handler */
    .align 7
    /* store lr(x30) before entry exception */
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    bl  _exception_entry
    bl  do_fiq_handle
    b   exception_exit

    /* Current EL (SP_ELx) Error Handler */
    .align    7
    /* store lr(x30) before entry exception */
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    bl  _exception_entry
    bl  do_error
    b   exception_exit

    /* sync, irq, fiq, error for 64bit and 32bit with sp_lower */
    /* total: 128 x 8 = 1024 */
    /* not used */
    .align 4

/* save x0~x30 to sp */
/* return: x0=sp, x1=esr_eln,x2=elr_eln,(n=1,2,3)*/
_exception_entry:
    stp x27, x28, [sp, #-16]!
    stp x25, x26, [sp, #-16]!
    stp x23, x24, [sp, #-16]!
    stp x21, x22, [sp, #-16]!
    stp x19, x20, [sp, #-16]!
    stp x17, x18, [sp, #-16]!
    stp x15, x16, [sp, #-16]!
    stp x13, x14, [sp, #-16]!
    stp x11, x12, [sp, #-16]!
    stp x9, x10,  [sp, #-16]!
    stp x7, x8,   [sp, #-16]!
    stp x5, x6,   [sp, #-16]!
    stp x3, x4,   [sp, #-16]!
    stp x1, x2,   [sp, #-16]!
    b   save_el_regs            /* jump to the second part */

/*
 * Save exception specific context: ESR and ELR,spsr for all exception levels.
 * This is the second part of the shared routine called into from all entries.
 */
save_el_regs:
    switch_el x11, 3f, 2f, 1f
3:  mrs x1, esr_el3
    mrs x2, elr_el3
    mrs x3, spsr_el3
    b   0f
2:  mrs x1, esr_el2
    mrs x2, elr_el2
    mrs x3, spsr_el2
    b   0f
1:  mrs x1, esr_el1
    mrs x2, elr_el1
    mrs x3, spsr_el1
0:
    stp x2, x0, [sp, #-16]!
    stp x3, x1, [sp, #-16]!
    mov x0, sp
    ret

/*
 * Restore the exception return address, for all exception levels.
 * This is the first part of the shared routine called into from all entries.
 */
exception_exit:
    ldp x3, x1, [sp],#16
    ldp x2, x0, [sp],#16
    switch_el x11, 3f, 2f, 1f
3:  msr elr_el3, x2
    msr spsr_el3, x3
    b   restore_regs
2:  msr elr_el2, x2
    msr spsr_el2, x2
    b   restore_regs
1:  msr elr_el1, x2
    msr spsr_el1, x2
    b   restore_regs        /* jump to the second part */

/*
 * Restore the general purpose registers from the exception stack, then return.
 * This is the second part of the shared routine called into from all entries.
 */
restore_regs:
    ldp    x1, x2,   [sp],#16
    ldp    x3, x4,   [sp],#16
    ldp    x5, x6,   [sp],#16
    ldp    x7, x8,   [sp],#16
    ldp    x9, x10,  [sp],#16
    ldp    x11, x12, [sp],#16
    ldp    x13, x14, [sp],#16
    ldp    x15, x16, [sp],#16
    ldp    x17, x18, [sp],#16
    ldp    x19, x20, [sp],#16
    ldp    x21, x22, [sp],#16
    ldp    x23, x24, [sp],#16
    ldp    x25, x26, [sp],#16
    ldp    x27, x28, [sp],#16
    ldp    x29, x30, [sp],#16

    msr    daifset, (DAIF_IRQ_BIT | DAIF_FIQ_BIT)

    /*
     * Exception Return using the ELR and SPSR for the current Exception level. When executed, the PE restores PSTATE
     * from the SPSR, and branches to the address held in the ELR.
    */
    eret
