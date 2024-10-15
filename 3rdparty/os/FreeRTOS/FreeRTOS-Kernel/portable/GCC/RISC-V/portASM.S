
/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#include "al_rv64_sysregs.h"

#ifndef __riscv_32e
#define portRegNum          30
#else
#define portRegNum          14
#endif

#define portCONTEXT_SIZE    ( portRegNum * REGBYTES )

#define portasmADDITIONAL_CONTEXT_SIZE  0

.section    .text.entry
.align 8

.extern xPortTaskSwitch
.extern pxCurrentTCB
.global prvPortStartFirstTask
.global pxPortInitialiseStack

#ifdef __riscv_flen

#if __riscv_flen == 32
    #define store_fpu           fsw
    #define load_fpu            flw
    #define FREGBYTES           4
#endif
#if __riscv_flen == 64
    #define store_fpu           fsd
    #define load_fpu            fld
    #define FREGBYTES           8
#endif

/* offset 0 * FREGBYTES - mepc
 * offset 1 * FREGBYTES - mstatus
 * offset (2 - 32) * FREGBYTES - float registers
 */
#define portFPU_CONTEXT_WORDS   34
#define portFPU_CONTEXT_SIZE (portFPU_CONTEXT_WORDS * FREGBYTES)

.macro portasmSAVE_FPU_REGISTERS
    addi    sp, sp, -portFPU_CONTEXT_SIZE
    /* get FS field from mstatus */
    li t0, MSTATUS_FS
    csrr    t1, mstatus
    STORE t1, 33 * FREGBYTES(sp)
    and t0, t1, t0
    /* Make room for the additional FPU registers. */
    store_fpu  f0, 1 * FREGBYTES(sp)
    store_fpu  f1, 2 * FREGBYTES(sp)
    store_fpu  f2, 3 * FREGBYTES(sp)
    store_fpu  f3, 4 * FREGBYTES(sp)
    store_fpu  f4, 5 * FREGBYTES(sp)
    store_fpu  f5, 6 * FREGBYTES(sp)
    store_fpu  f6, 7 * FREGBYTES(sp)
    store_fpu  f7, 8 * FREGBYTES(sp)
    store_fpu  f8, 9 * FREGBYTES(sp)
    store_fpu  f9, 10 * FREGBYTES(sp)
    store_fpu  f10, 11 * FREGBYTES(sp)
    store_fpu  f11, 12 * FREGBYTES(sp)
    store_fpu  f12, 13 * FREGBYTES(sp)
    store_fpu  f13, 14 * FREGBYTES(sp)
    store_fpu  f14, 15 * FREGBYTES(sp)
    store_fpu  f15, 16 * FREGBYTES(sp)
    store_fpu  f16, 17 * FREGBYTES(sp)
    store_fpu  f17, 18 * FREGBYTES(sp)
    store_fpu  f18, 19 * FREGBYTES(sp)
    store_fpu  f19, 20 * FREGBYTES(sp)
    store_fpu  f20, 21 * FREGBYTES(sp)
    store_fpu  f21, 22 * FREGBYTES(sp)
    store_fpu  f22, 23 * FREGBYTES(sp)
    store_fpu  f23, 24 * FREGBYTES(sp)
    store_fpu  f24, 25 * FREGBYTES(sp)
    store_fpu  f25, 26 * FREGBYTES(sp)
    store_fpu  f26, 27 * FREGBYTES(sp)
    store_fpu  f27, 28 * FREGBYTES(sp)
    store_fpu  f28, 29 * FREGBYTES(sp)
    store_fpu  f29, 30 * FREGBYTES(sp)
    store_fpu  f30, 31 * FREGBYTES(sp)
    store_fpu  f31, 32 * FREGBYTES(sp)

    /* must set FS to clean */
    csrc    mstatus, t0
    li  t1, MSTATUS_FS_CLEAN
    csrs    mstatus, t1
1:
    .endm

.macro portasmRESTORE_FPU_REGISTERS
    LOAD  t1, 33 * FREGBYTES(sp)
    /* get FS field from mstatus */
    li t0, MSTATUS_FS
    and t0, t1, t0
     /* FS == dirty */
    csrs    mstatus, t0
    load_fpu   f0, 1 * FREGBYTES(sp)
    load_fpu   f1, 2 * FREGBYTES(sp)
    load_fpu   f2, 3 * FREGBYTES(sp)
    load_fpu   f3, 4 * FREGBYTES(sp)
    load_fpu   f4, 5 * FREGBYTES(sp)
    load_fpu   f5, 6 * FREGBYTES(sp)
    load_fpu   f6, 7 * FREGBYTES(sp)
    load_fpu   f7, 8 * FREGBYTES(sp)
    load_fpu   f8, 9 * FREGBYTES(sp)
    load_fpu   f9, 10 * FREGBYTES(sp)
    load_fpu   f10, 11 * FREGBYTES(sp)
    load_fpu   f11, 12 * FREGBYTES(sp)
    load_fpu   f12, 13 * FREGBYTES(sp)
    load_fpu   f13, 14 * FREGBYTES(sp)
    load_fpu   f14, 15 * FREGBYTES(sp)
    load_fpu   f15, 16 * FREGBYTES(sp)
    load_fpu   f16, 17 * FREGBYTES(sp)
    load_fpu   f17, 18 * FREGBYTES(sp)
    load_fpu   f18, 19 * FREGBYTES(sp)
    load_fpu   f19, 20 * FREGBYTES(sp)
    load_fpu   f20, 21 * FREGBYTES(sp)
    load_fpu   f21, 22 * FREGBYTES(sp)
    load_fpu   f22, 23 * FREGBYTES(sp)
    load_fpu   f23, 24 * FREGBYTES(sp)
    load_fpu   f24, 25 * FREGBYTES(sp)
    load_fpu   f25, 26 * FREGBYTES(sp)
    load_fpu   f26, 27 * FREGBYTES(sp)
    load_fpu   f27, 28 * FREGBYTES(sp)
    load_fpu   f28, 29 * FREGBYTES(sp)
    load_fpu   f29, 30 * FREGBYTES(sp)
    load_fpu   f30, 31 * FREGBYTES(sp)
    load_fpu   f31, 32 * FREGBYTES(sp)

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

/**
 * \brief  Global interrupt disabled
 * \details
 *  This function disable global interrupt.
 * \remarks
 *  - All the interrupt requests will be ignored by CPU.
 */
.macro DISABLE_MIE
    csrc CSR_MSTATUS, MSTATUS_MIE
.endm

/**
 * \brief  Macro for context save
 * \details
 * This macro save ABI defined caller saved registers in the stack.
 * \remarks
 * - This Macro could use to save context when you enter to interrupt
 * or exception
*/
/* Save caller registers */
.macro SAVE_CONTEXT
    csrrw sp, CSR_MSCRATCHCSWL, sp
    /* Allocate stack space for context saving */
#ifndef __riscv_32e
    addi sp, sp, -20*REGBYTES
#else
    addi sp, sp, -14*REGBYTES
#endif /* __riscv_32e */

    STORE x1, 0*REGBYTES(sp)
    STORE x4, 1*REGBYTES(sp)
    STORE x5, 2*REGBYTES(sp)
    STORE x6, 3*REGBYTES(sp)
    STORE x7, 4*REGBYTES(sp)
    STORE x10, 5*REGBYTES(sp)
    STORE x11, 6*REGBYTES(sp)
    STORE x12, 7*REGBYTES(sp)
    STORE x13, 8*REGBYTES(sp)
    STORE x14, 9*REGBYTES(sp)
    STORE x15, 10*REGBYTES(sp)
#ifndef __riscv_32e
    STORE x16, 14*REGBYTES(sp)
    STORE x17, 15*REGBYTES(sp)
    STORE x28, 16*REGBYTES(sp)
    STORE x29, 17*REGBYTES(sp)
    STORE x30, 18*REGBYTES(sp)
    STORE x31, 19*REGBYTES(sp)
#endif /* __riscv_32e */
.endm

/**
 * \brief  Macro for restore caller registers
 * \details
 * This macro restore ABI defined caller saved registers from stack.
 * \remarks
 * - You could use this macro to restore context before you want return
 * from interrupt or exeception
 */
/* Restore caller registers */
.macro RESTORE_CONTEXT
    LOAD x1, 0*REGBYTES(sp)
    LOAD x4, 1*REGBYTES(sp)
    LOAD x5, 2*REGBYTES(sp)
    LOAD x6, 3*REGBYTES(sp)
    LOAD x7, 4*REGBYTES(sp)
    LOAD x10, 5*REGBYTES(sp)
    LOAD x11, 6*REGBYTES(sp)
    LOAD x12, 7*REGBYTES(sp)
    LOAD x13, 8*REGBYTES(sp)
    LOAD x14, 9*REGBYTES(sp)
    LOAD x15, 10*REGBYTES(sp)
#ifndef __riscv_32e
    LOAD x16, 14*REGBYTES(sp)
    LOAD x17, 15*REGBYTES(sp)
    LOAD x28, 16*REGBYTES(sp)
    LOAD x29, 17*REGBYTES(sp)
    LOAD x30, 18*REGBYTES(sp)
    LOAD x31, 19*REGBYTES(sp)

    /* De-allocate the stack space */
    addi sp, sp, 20*REGBYTES
#else
    /* De-allocate the stack space */
    addi sp, sp, 14*REGBYTES
#endif /* __riscv_32e */
    csrrw sp, CSR_MSCRATCHCSWL, sp
.endm

/**
 * \brief  Macro for save necessary CSRs to stack
 * \details
 * This macro store MCAUSE, MEPC, MSUBM to stack.
 */
.macro SAVE_CSR_CONTEXT
    /* Store CSR mcause to stack using pushmcause */
    csrrwi  x0, CSR_PUSHMCAUSE, 11
    /* Store CSR mepc to stack using pushmepc */
    csrrwi  x0, CSR_PUSHMEPC, 12
    /* Store CSR msub to stack using pushmsub */
    csrrwi  x0, CSR_PUSHMSUBM, 13
.endm

/**
 * \brief  Macro for restore necessary CSRs from stack
 * \details
 * This macro restore MSUBM, MEPC, MCAUSE from stack.
 */
.macro RESTORE_CSR_CONTEXT
    LOAD x5,  13*REGBYTES(sp)
    csrw CSR_MSUBM, x5
    LOAD x5,  12*REGBYTES(sp)
    csrw CSR_MEPC, x5
    LOAD x5,  11*REGBYTES(sp)
    csrw CSR_MCAUSE, x5
.endm

/**
 * \brief  Exception/NMI Entry
 * \details
 * This function provide common entry functions for exception/nmi.
 * \remarks
 * This function provide a default exception/nmi entry.
 * ABI defined caller save register and some CSR registers
 * to be saved before enter interrupt handler and be restored before return.
 */
.section .text.trap
/* In CLIC mode, the exeception entry must be 64bytes aligned */
.align 6
.global exc_entry
exc_entry:
    /* Save the caller saving registers (context) */
    SAVE_CONTEXT
    /* Save the necessary CSR registers */
    SAVE_CSR_CONTEXT

    /*
     * Set the exception handler function arguments
     * argument 1: mcause value
     * argument 2: current stack point(SP) value
     */
    csrr a0, mcause
    mv a1, sp
    /*
     * TODO: Call the exception handler function
     * By default, the function template is provided in
     * system_Device.c, you can adjust it as you want
     */
    call core_exception_handler

    /* Restore the necessary CSR registers */
    RESTORE_CSR_CONTEXT
    /* Restore the caller saving registers (context) */
    RESTORE_CONTEXT

    /* Return to regular code */
    mret

/* Start the first task.  This also clears the bit that indicates the FPU is
    in use in case the FPU was used before the scheduler was started - which
    would otherwise result in the unnecessary leaving of space in the stack
    for lazy saving of FPU registers. */
.align 3
prvPortStartFirstTask:
    /* Setup Interrupt Stack using
       The stack that was used by main()
       before the scheduler is started is
       no longer required after the scheduler is started.
       Interrupt stack pointer is stored in CSR_MSCRATCHCSWL */
    la t0, _sp
    csrw CSR_MSCRATCH, t0
    LOAD sp, pxCurrentTCB           /* Load pxCurrentTCB. */
    LOAD sp, 0x0(sp)                /* Read sp from first TCB member */

    /* Pop PC from stack and set MEPC */
    LOAD t0,  0  * REGBYTES(sp)
    csrw CSR_MEPC, t0

    LOAD t0,  (portRegNum + portFPU_CONTEXT_WORDS - 1)  * REGBYTES(sp)
    fscsr t0


    /* Pop mstatus from stack and set it */
    LOAD t0,  (portRegNum + portFPU_CONTEXT_WORDS - 2)  * REGBYTES(sp)
    csrw CSR_MSTATUS, t0
    /* Interrupt still disable here */

    portasmRESTORE_FPU_REGISTERS
    
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

    addi sp, sp, portCONTEXT_SIZE

    mret

.align 2
.global eclic_msip_handler
eclic_msip_handler:
    addi sp, sp, -portCONTEXT_SIZE
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
    STORE t0,  (portRegNum - 2)  * REGBYTES(sp)
    
    frcsr t0
    STORE t0, (portRegNum - 1)  * REGBYTES(sp)
    fscsr zero
    li  t1, MSTATUS_FS_CLEAN
    csrs    mstatus, t1

    /* Push additional registers */
    portasmSAVE_FPU_REGISTERS

    /* Store sp to task stack */
    LOAD t0, pxCurrentTCB
    STORE sp, 0(t0)

    csrr t0, CSR_MEPC
    STORE t0, 0(sp)
    jal xPortTaskSwitch

    /* Switch task context */
    LOAD t0, pxCurrentTCB           /* Load pxCurrentTCB. */
    LOAD sp, 0x0(t0)                /* Read sp from first TCB member */

    /* Pop PC from stack and set MEPC */
    LOAD t0,  0  * REGBYTES(sp)
    csrw CSR_MEPC, t0

    /* Pop additional registers */
    portasmRESTORE_FPU_REGISTERS

    LOAD t0,  (portRegNum - 1)  * REGBYTES(sp)
    fscsr t0

    /* Pop mstatus from stack and set it */
    LOAD t0,  (portRegNum - 2)  * REGBYTES(sp)
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

    addi sp, sp, portCONTEXT_SIZE
    mret
