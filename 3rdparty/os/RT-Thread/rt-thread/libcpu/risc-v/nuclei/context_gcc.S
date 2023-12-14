/*
 * Copyright (c) 2019-Present Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020/03/26     Huaqi        First Nuclei RISC-V porting implementation
 */

#include "al_rv64_sysregs.h"

#ifndef __riscv_32e
#define RT_SAVED_REGNUM         30
#else
#define RT_SAVED_REGNUM         14
#endif

#ifdef __riscv_flen
/* offset 0 * FPREGBYTES - mepc
 * offset 1 * FPREGBYTES - mstatus
 * offset (2 - 32) * FPREGBYTES - float registers
 */
#define portFPU_CONTEXT_WORDS   34
#define portFPU_CONTEXT_SIZE (portFPU_CONTEXT_WORDS * FPREGBYTES)

.macro portasmSAVE_FPU_REGISTERS
    addi    sp, sp, -portFPU_CONTEXT_SIZE
    /* get FS field from mstatus */
    li t0, MSTATUS_FS
    csrr    t1, mstatus
    STORE   t1, 33 * FPREGBYTES(sp)
    and t0, t1, t0
    li t2, MSTATUS_FS_DIRTY
    bne t2, t0, 1f
    /* FS == dirty */
    /* Make room for the additional FPU registers. */
    FPSTORE  f0, 1 * FPREGBYTES(sp)
    FPSTORE  f1, 2 * FPREGBYTES(sp)
    FPSTORE  f2, 3 * FPREGBYTES(sp)
    FPSTORE  f3, 4 * FPREGBYTES(sp)
    FPSTORE  f4, 5 * FPREGBYTES(sp)
    FPSTORE  f5, 6 * FPREGBYTES(sp)
    FPSTORE  f6, 7 * FPREGBYTES(sp)
    FPSTORE  f7, 8 * FPREGBYTES(sp)
    FPSTORE  f8, 9 * FPREGBYTES(sp)
    FPSTORE  f9, 10 * FPREGBYTES(sp)
    FPSTORE  f10, 11 * FPREGBYTES(sp)
    FPSTORE  f11, 12 * FPREGBYTES(sp)
    FPSTORE  f12, 13 * FPREGBYTES(sp)
    FPSTORE  f13, 14 * FPREGBYTES(sp)
    FPSTORE  f14, 15 * FPREGBYTES(sp)
    FPSTORE  f15, 16 * FPREGBYTES(sp)
    FPSTORE  f16, 17 * FPREGBYTES(sp)
    FPSTORE  f17, 18 * FPREGBYTES(sp)
    FPSTORE  f18, 19 * FPREGBYTES(sp)
    FPSTORE  f19, 20 * FPREGBYTES(sp)
    FPSTORE  f20, 21 * FPREGBYTES(sp)
    FPSTORE  f21, 22 * FPREGBYTES(sp)
    FPSTORE  f22, 23 * FPREGBYTES(sp)
    FPSTORE  f23, 24 * FPREGBYTES(sp)
    FPSTORE  f24, 25 * FPREGBYTES(sp)
    FPSTORE  f25, 26 * FPREGBYTES(sp)
    FPSTORE  f26, 27 * FPREGBYTES(sp)
    FPSTORE  f27, 28 * FPREGBYTES(sp)
    FPSTORE  f28, 29 * FPREGBYTES(sp)
    FPSTORE  f29, 30 * FPREGBYTES(sp)
    FPSTORE  f30, 31 * FPREGBYTES(sp)
    FPSTORE  f31, 32 * FPREGBYTES(sp)

    /* must set FS to clean */
    csrc    mstatus, t0
    li  t1, MSTATUS_FS_CLEAN
    csrs    mstatus, t1
1:
    .endm

.macro portasmRESTORE_FPU_REGISTERS
    LOAD t1, 33 * FPREGBYTES(sp)
    /* get FS field from mstatus */
    li t0, MSTATUS_FS
    and t0, t1, t0
    li t2, MSTATUS_FS_DIRTY
    bne t2, t0, 1f
    /* FS == dirty */
    csrs    mstatus, t0
    FPLOAD   f0, 1 * FPREGBYTES(sp)
    FPLOAD   f1, 2 * FPREGBYTES(sp)
    FPLOAD   f2, 3 * FPREGBYTES(sp)
    FPLOAD   f3, 4 * FPREGBYTES(sp)
    FPLOAD   f4, 5 * FPREGBYTES(sp)
    FPLOAD   f5, 6 * FPREGBYTES(sp)
    FPLOAD   f6, 7 * FPREGBYTES(sp)
    FPLOAD   f7, 8 * FPREGBYTES(sp)
    FPLOAD   f8, 9 * FPREGBYTES(sp)
    FPLOAD   f9, 10 * FPREGBYTES(sp)
    FPLOAD   f10, 11 * FPREGBYTES(sp)
    FPLOAD   f11, 12 * FPREGBYTES(sp)
    FPLOAD   f12, 13 * FPREGBYTES(sp)
    FPLOAD   f13, 14 * FPREGBYTES(sp)
    FPLOAD   f14, 15 * FPREGBYTES(sp)
    FPLOAD   f15, 16 * FPREGBYTES(sp)
    FPLOAD   f16, 17 * FPREGBYTES(sp)
    FPLOAD   f17, 18 * FPREGBYTES(sp)
    FPLOAD   f18, 19 * FPREGBYTES(sp)
    FPLOAD   f19, 20 * FPREGBYTES(sp)
    FPLOAD   f20, 21 * FPREGBYTES(sp)
    FPLOAD   f21, 22 * FPREGBYTES(sp)
    FPLOAD   f22, 23 * FPREGBYTES(sp)
    FPLOAD   f23, 24 * FPREGBYTES(sp)
    FPLOAD   f24, 25 * FPREGBYTES(sp)
    FPLOAD   f25, 26 * FPREGBYTES(sp)
    FPLOAD   f26, 27 * FPREGBYTES(sp)
    FPLOAD   f27, 28 * FPREGBYTES(sp)
    FPLOAD   f28, 29 * FPREGBYTES(sp)
    FPLOAD   f29, 30 * FPREGBYTES(sp)
    FPLOAD   f30, 31 * FPREGBYTES(sp)
    FPLOAD   f31, 32 * FPREGBYTES(sp)

    /* must set FS to clean */
    csrc    mstatus, t0
    li  t1, MSTATUS_FS_CLEAN
    csrs    mstatus, t1
1:
    addi    sp, sp, portFPU_CONTEXT_SIZE
    .endm

#else

.macro portasmSAVE_FPU_REGISTERS
.endm
.macro portasmRESTORE_FPU_REGISTERS
.endm

#endif

#define RT_CONTEXT_SIZE         (RT_SAVED_REGNUM * REGBYTES)

    .extern rt_interrupt_from_thread
    .extern rt_interrupt_to_thread


.section    .text


/*
 * void rt_hw_context_switch_to(rt_ubase_t to);
 * a0 --> to_thread
 */
    .globl rt_hw_context_switch_to

/* Start the first task.  This also clears the bit that indicates the FPU is
	in use in case the FPU was used before the scheduler was started - which
	would otherwise result in the unnecessary leaving of space in the stack
	for lazy saving of FPU registers. */
.align 3
rt_hw_context_switch_to:
    /* Setup Interrupt Stack using
       The stack that was used by main()
       before the scheduler is started is
       no longer required after the scheduler is started.
       Interrupt stack pointer is stored in CSR_MSCRATCH */
    la t0, _sp
    csrw CSR_MSCRATCH, t0
    LOAD sp, 0x0(a0)                /* Read sp from first TCB member(a0) */

    /* Restore Fpu registers when fpu is enabled */
    portasmRESTORE_FPU_REGISTERS

    /* Pop PC from stack and set MEPC */
    LOAD t0,  0  * REGBYTES(sp)
    csrw CSR_MEPC, t0
    /* Pop mstatus from stack and set it */
    LOAD t0,  (RT_SAVED_REGNUM - 1)  * REGBYTES(sp)
    csrw CSR_MSTATUS, t0
    /* Interrupt still disable here */
    /* Restore Registers from Stack */
    LOAD x1,  1  * REGBYTES(sp)    /* RA */
    LOAD x5,  2  * REGBYTES(sp)
    LOAD x6,  3  * REGBYTES(sp)
    LOAD x7,  4  * REGBYTES(sp)
    LOAD x8,  5  * REGBYTES(sp)
    LOAD x9,  6  * REGBYTES(sp)
    LOAD x10, 7  * REGBYTES(sp)
    LOAD x11, 8  * REGBYTES(sp)
    LOAD x12, 9  * REGBYTES(sp)
    LOAD x13, 10 * REGBYTES(sp)
    LOAD x14, 11 * REGBYTES(sp)
    LOAD x15, 12 * REGBYTES(sp)
#ifndef __riscv_32e
    LOAD x16, 13 * REGBYTES(sp)
    LOAD x17, 14 * REGBYTES(sp)
    LOAD x18, 15 * REGBYTES(sp)
    LOAD x19, 16 * REGBYTES(sp)
    LOAD x20, 17 * REGBYTES(sp)
    LOAD x21, 18 * REGBYTES(sp)
    LOAD x22, 19 * REGBYTES(sp)
    LOAD x23, 20 * REGBYTES(sp)
    LOAD x24, 21 * REGBYTES(sp)
    LOAD x25, 22 * REGBYTES(sp)
    LOAD x26, 23 * REGBYTES(sp)
    LOAD x27, 24 * REGBYTES(sp)
    LOAD x28, 25 * REGBYTES(sp)
    LOAD x29, 26 * REGBYTES(sp)
    LOAD x30, 27 * REGBYTES(sp)
    LOAD x31, 28 * REGBYTES(sp)
#endif

    addi sp, sp, RT_CONTEXT_SIZE

    mret

.align 2
.global eclic_msip_handler
eclic_msip_handler:
    addi sp, sp, -RT_CONTEXT_SIZE
    STORE x1,  1  * REGBYTES(sp)    /* RA */
    STORE x5,  2  * REGBYTES(sp)
    STORE x6,  3  * REGBYTES(sp)
    STORE x7,  4  * REGBYTES(sp)
    STORE x8,  5  * REGBYTES(sp)
    STORE x9,  6  * REGBYTES(sp)
    STORE x10, 7  * REGBYTES(sp)
    STORE x11, 8  * REGBYTES(sp)
    STORE x12, 9  * REGBYTES(sp)
    STORE x13, 10 * REGBYTES(sp)
    STORE x14, 11 * REGBYTES(sp)
    STORE x15, 12 * REGBYTES(sp)
#ifndef __riscv_32e
    STORE x16, 13 * REGBYTES(sp)
    STORE x17, 14 * REGBYTES(sp)
    STORE x18, 15 * REGBYTES(sp)
    STORE x19, 16 * REGBYTES(sp)
    STORE x20, 17 * REGBYTES(sp)
    STORE x21, 18 * REGBYTES(sp)
    STORE x22, 19 * REGBYTES(sp)
    STORE x23, 20 * REGBYTES(sp)
    STORE x24, 21 * REGBYTES(sp)
    STORE x25, 22 * REGBYTES(sp)
    STORE x26, 23 * REGBYTES(sp)
    STORE x27, 24 * REGBYTES(sp)
    STORE x28, 25 * REGBYTES(sp)
    STORE x29, 26 * REGBYTES(sp)
    STORE x30, 27 * REGBYTES(sp)
    STORE x31, 28 * REGBYTES(sp)
#endif
    /* Push mstatus to stack */
    csrr t0, CSR_MSTATUS
    STORE t0,  (RT_SAVED_REGNUM - 1)  * REGBYTES(sp)

    /* Push additional registers */

    /* Store sp to task stack */
    LOAD t0, rt_interrupt_from_thread
    STORE sp, 0(t0)

    /* Restore Fpu registers when fpu is enabled */
    portasmRESTORE_FPU_REGISTERS

    csrr t0, CSR_MEPC
    STORE t0, 0(sp)

    jal rt_hw_taskswitch

    /* Switch task context */
    LOAD t0, rt_interrupt_to_thread
    LOAD sp, 0x0(t0)

    /* Pop PC from stack and set MEPC */
    LOAD t0,  0  * REGBYTES(sp)
    csrw CSR_MEPC, t0

    /* Pop additional registers */

    /* Pop mstatus from stack and set it */
    LOAD t0,  (RT_SAVED_REGNUM - 1)  * REGBYTES(sp)
    csrw CSR_MSTATUS, t0
    /* Interrupt still disable here */
    /* Restore Registers from Stack */
    LOAD x1,  1  * REGBYTES(sp)    /* RA */
    LOAD x5,  2  * REGBYTES(sp)
    LOAD x6,  3  * REGBYTES(sp)
    LOAD x7,  4  * REGBYTES(sp)
    LOAD x8,  5  * REGBYTES(sp)
    LOAD x9,  6  * REGBYTES(sp)
    LOAD x10, 7  * REGBYTES(sp)
    LOAD x11, 8  * REGBYTES(sp)
    LOAD x12, 9  * REGBYTES(sp)
    LOAD x13, 10 * REGBYTES(sp)
    LOAD x14, 11 * REGBYTES(sp)
    LOAD x15, 12 * REGBYTES(sp)
#ifndef __riscv_32e
    LOAD x16, 13 * REGBYTES(sp)
    LOAD x17, 14 * REGBYTES(sp)
    LOAD x18, 15 * REGBYTES(sp)
    LOAD x19, 16 * REGBYTES(sp)
    LOAD x20, 17 * REGBYTES(sp)
    LOAD x21, 18 * REGBYTES(sp)
    LOAD x22, 19 * REGBYTES(sp)
    LOAD x23, 20 * REGBYTES(sp)
    LOAD x24, 21 * REGBYTES(sp)
    LOAD x25, 22 * REGBYTES(sp)
    LOAD x26, 23 * REGBYTES(sp)
    LOAD x27, 24 * REGBYTES(sp)
    LOAD x28, 25 * REGBYTES(sp)
    LOAD x29, 26 * REGBYTES(sp)
    LOAD x30, 27 * REGBYTES(sp)
    LOAD x31, 28 * REGBYTES(sp)
#endif

    addi sp, sp, RT_CONTEXT_SIZE
    mret
