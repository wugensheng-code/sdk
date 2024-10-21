#ifndef CONTEXT_H
#define CONTEXT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "al_rv64_sysregs.h"



#if __riscv_flen == 32
    #define STORE_FPU           fsw
    #define LOAD_FPU            flw
    #define FREGBYTES           4
#endif
#if __riscv_flen == 64
    #define STORE_FPU           fsd
    #define LOAD_FPU            fld
    #define FREGBYTES           8
#endif


#ifndef __riscv_32e
#define GENERAL_CONTEXT_SIZE 30  * REGBYTES
#else
#define GENERAL_CONTEXT_SIZE 14  * REGBYTES
#endif


#ifdef __riscv_flen

    #define MSTATUS_FS           0x00006000 /* Floating-point Status */
    #define MSTATUS_FS_OFF       0x00000000
    #define MSTATUS_FS_INITIAL   0x00002000
    #define MSTATUS_FS_CLEAN     0x00004000
    #define MSTATUS_FS_DIRTY     0x00006000


#if __riscv_flen == 32
    #define STORE_FPU           fsw
    #define LOAD_FPU            flw
    #define FREGBYTES           4
#endif
#if __riscv_flen == 64
    #define STORE_FPU           fsd
    #define LOAD_FPU            fld
    #define FREGBYTES           8
#endif

#define FPU_CONTEXT 32

/* 
 * offset (0 - 31) * FREGBYTES - float registers
 */
#define FPU_CONTEXT_SIZE (FPU_CONTEXT * FREGBYTES)

.macro SAVE_FPU_REGISTERS
    addi    sp, sp, -FPU_CONTEXT_SIZE
    /* get FS field from mstatus */
    li t0, MSTATUS_FS
    csrr    t1, mstatus
    // store_x t1, 33 * FREGBYTES(sp)
    and t0, t1, t0
    li t2, MSTATUS_FS_DIRTY
    // bne t2, t0, 1f
    /* FS == dirty */
    /* Make room for the additional FPU registers. */
    STORE_FPU  f0, 1 * FREGBYTES(sp)
    STORE_FPU  f1, 2 * FREGBYTES(sp)
    STORE_FPU  f2, 3 * FREGBYTES(sp)
    STORE_FPU  f3, 4 * FREGBYTES(sp)
    STORE_FPU  f4, 5 * FREGBYTES(sp)
    STORE_FPU  f5, 6 * FREGBYTES(sp)
    STORE_FPU  f6, 7 * FREGBYTES(sp)
    STORE_FPU  f7, 8 * FREGBYTES(sp)
    STORE_FPU  f8, 9 * FREGBYTES(sp)
    STORE_FPU  f9, 10 * FREGBYTES(sp)
    STORE_FPU  f10, 11 * FREGBYTES(sp)
    STORE_FPU  f11, 12 * FREGBYTES(sp)
    STORE_FPU  f12, 13 * FREGBYTES(sp)
    STORE_FPU  f13, 14 * FREGBYTES(sp)
    STORE_FPU  f14, 15 * FREGBYTES(sp)
    STORE_FPU  f15, 16 * FREGBYTES(sp)
    STORE_FPU  f16, 17 * FREGBYTES(sp)
    STORE_FPU  f17, 18 * FREGBYTES(sp)
    STORE_FPU  f18, 19 * FREGBYTES(sp)
    STORE_FPU  f19, 20 * FREGBYTES(sp)
    STORE_FPU  f20, 21 * FREGBYTES(sp)
    STORE_FPU  f21, 22 * FREGBYTES(sp)
    STORE_FPU  f22, 23 * FREGBYTES(sp)
    STORE_FPU  f23, 24 * FREGBYTES(sp)
    STORE_FPU  f24, 25 * FREGBYTES(sp)
    STORE_FPU  f25, 26 * FREGBYTES(sp)
    STORE_FPU  f26, 27 * FREGBYTES(sp)
    STORE_FPU  f27, 28 * FREGBYTES(sp)
    STORE_FPU  f28, 29 * FREGBYTES(sp)
    STORE_FPU  f29, 30 * FREGBYTES(sp)
    STORE_FPU  f30, 31 * FREGBYTES(sp)
    STORE_FPU  f31, 32 * FREGBYTES(sp)

    /* must set FS to clean */
    csrc    mstatus, t0
    li  t1, MSTATUS_FS_CLEAN
    csrs    mstatus, t1
1:
    .endm


.macro RESTORE_FPU_REGISTERS
    // load_x  t1, 33 * FREGBYTES(sp)
    /* get FS field from mstatus */
    li t0, MSTATUS_FS
    and t0, t1, t0
    li t2, MSTATUS_FS_DIRTY
    // bne t2, t0, 1f
     /* FS == dirty */
    csrs    mstatus, t0
    LOAD_FPU   f0, 1 * FREGBYTES(sp)
    LOAD_FPU   f1, 2 * FREGBYTES(sp)
    LOAD_FPU   f2, 3 * FREGBYTES(sp)
    LOAD_FPU   f3, 4 * FREGBYTES(sp)
    LOAD_FPU   f4, 5 * FREGBYTES(sp)
    LOAD_FPU   f5, 6 * FREGBYTES(sp)
    LOAD_FPU   f6, 7 * FREGBYTES(sp)
    LOAD_FPU   f7, 8 * FREGBYTES(sp)
    LOAD_FPU   f8, 9 * FREGBYTES(sp)
    LOAD_FPU   f9, 10 * FREGBYTES(sp)
    LOAD_FPU   f10, 11 * FREGBYTES(sp)
    LOAD_FPU   f11, 12 * FREGBYTES(sp)
    LOAD_FPU   f12, 13 * FREGBYTES(sp)
    LOAD_FPU   f13, 14 * FREGBYTES(sp)
    LOAD_FPU   f14, 15 * FREGBYTES(sp)
    LOAD_FPU   f15, 16 * FREGBYTES(sp)
    LOAD_FPU   f16, 17 * FREGBYTES(sp)
    LOAD_FPU   f17, 18 * FREGBYTES(sp)
    LOAD_FPU   f18, 19 * FREGBYTES(sp)
    LOAD_FPU   f19, 20 * FREGBYTES(sp)
    LOAD_FPU   f20, 21 * FREGBYTES(sp)
    LOAD_FPU   f21, 22 * FREGBYTES(sp)
    LOAD_FPU   f22, 23 * FREGBYTES(sp)
    LOAD_FPU   f23, 24 * FREGBYTES(sp)
    LOAD_FPU   f24, 25 * FREGBYTES(sp)
    LOAD_FPU   f25, 26 * FREGBYTES(sp)
    LOAD_FPU   f26, 27 * FREGBYTES(sp)
    LOAD_FPU   f27, 28 * FREGBYTES(sp)
    LOAD_FPU   f28, 29 * FREGBYTES(sp)
    LOAD_FPU   f29, 30 * FREGBYTES(sp)
    LOAD_FPU   f30, 31 * FREGBYTES(sp)
    LOAD_FPU   f31, 32 * FREGBYTES(sp)

    /* must set FS to clean */
    csrc    mstatus, t0
    li  t1, MSTATUS_FS_CLEAN
    csrs    mstatus, t1
1:
    addi    sp, sp, FPU_CONTEXT_SIZE
    .endm

#else

#define FPU_CONTEXT 0

#endif /* __riscv_flen */

#define ALL_CONTEXT_SIZE FPU_CONTEXT_SIZE + GENERAL_CONTEXT_SIZE

/* need to save mepc after calling this macro */
.macro SAVE_ALL
    addi sp, sp, -GENERAL_CONTEXT_SIZE
    /* Allocate stack space for context saving */

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
#endif /* __riscv_32e */
    /* Push mstatus to stack */
    csrr t0, CSR_MSTATUS
    STORE t0,  29  * REGBYTES(sp)

#ifdef __riscv_flen
    SAVE_FPU_REGISTERS
#endif

.endm

/* need to restore mepc before calling this macro */
.macro RESTORE_ALL

#ifdef __riscv_flen
    RESTORE_FPU_REGISTERS
#endif

    /* Pop mstatus from stack and set it */
    LOAD t0,  29  * REGBYTES(sp)
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

    addi sp, sp, GENERAL_CONTEXT_SIZE
    mret

.endm

#ifdef __cplusplus
}
#endif

#endif /* CONTEXT_H */
