/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AARCH64_ARCH_H
#define AL_AARCH64_ARCH_H

#include "al_aarch64_sysreg.h"
#include "al_core.h"

#define icc_asgi1r      S3_0_C12_C11_6
#define icc_sgi1r       S3_0_C12_C11_5
#define icc_sre_el1     S3_0_C12_C12_5
#define icc_sre_el2     S3_4_C12_C9_5
#define icc_sre_el3     S3_6_C12_C12_5
#define icc_pmr_el1     S3_0_C4_C6_0
#define icc_rpr_el1     S3_0_C12_C11_3
#define icc_igrpen1_el3 S3_6_c12_c12_7
#define icc_igrpen1_el1 S3_0_C12_C12_7
#define icc_igrpen0_el1 S3_0_c12_c12_6
#define icc_hppir0_el1  S3_0_c12_c8_2
#define icc_hppir1_el1  S3_0_c12_c12_2
#define icc_iar0_el1    S3_0_c12_c8_0
#define icc_iar1_el1    S3_0_c12_c12_0
#define icc_eoir0_el1   S3_0_c12_c8_1
#define icc_eoir1_el1   S3_0_c12_c12_1
#define icc_sgi0r_el1   S3_0_c12_c11_7
#define icc_bpr1_el1    S3_0_c12_c12_3
#define icc_ctlr_el1    S3_0_C12_C12_4

#define ISA_SYSREG_READ     "mrs"
#define ISA_SYSREG_WRITE    "msr"
#define ISA_SYSREG_WRITE_CONST    ISA_SYSREG_WRITE


#include "al_sysops.h"

__STATIC_FORCEINLINE void enable_irq(void)
{
    /*
     * The compiler memory barrier will prevent the compiler from
     * scheduling non-volatile memory access after the write to the
     * register.
     *
     * This could happen if some initialization code issues non-volatile
     * accesses to an area used by an interrupt handler, in the assumption
     * that it is safe as the interrupts are disabled at the time it does
     * that (according to program order). However, non-volatile accesses
     * are not necessarily in program order relatively with volatile inline
     * assembly statements (and volatile accesses).
     */
    __COMPILER_BARRIER();
    ARCH_SYSREG_WRITE_CONST(daifclr, DAIF_IRQ_BIT);
    ISB();
}

__STATIC_FORCEINLINE void enable_fiq(void)
{
    __COMPILER_BARRIER();
    ARCH_SYSREG_WRITE_CONST(daifclr, DAIF_FIQ_BIT);
    ISB();
}

__STATIC_FORCEINLINE void enable_all_intr(void)
{
    __COMPILER_BARRIER();
    ARCH_SYSREG_WRITE_CONST(daifclr, DAIF_IRQ_BIT | DAIF_FIQ_BIT);
    ISB();
}

__STATIC_FORCEINLINE void enable_serror(void)
{
    __COMPILER_BARRIER();
    ARCH_SYSREG_WRITE_CONST(daifclr, DAIF_ABT_BIT);
    ISB();
}

__STATIC_FORCEINLINE void enable_debug_exceptions(void)
{
    __COMPILER_BARRIER();
    ARCH_SYSREG_WRITE_CONST(daifclr, DAIF_DBG_BIT);
    ISB();
}

__STATIC_FORCEINLINE void disable_irq(void)
{
    __COMPILER_BARRIER();
    ARCH_SYSREG_WRITE_CONST(daifset, DAIF_IRQ_BIT);
    ISB();
}

__STATIC_FORCEINLINE void disable_fiq(void)
{
    __COMPILER_BARRIER();
    ARCH_SYSREG_WRITE_CONST(daifset, DAIF_FIQ_BIT);
    ISB();
}

__STATIC_FORCEINLINE void disable_all_intr(void)
{
    __COMPILER_BARRIER();
    ARCH_SYSREG_WRITE_CONST(daifset, DAIF_IRQ_BIT | DAIF_FIQ_BIT);
    ISB();
}

__STATIC_FORCEINLINE void disable_serror(void)
{
    __COMPILER_BARRIER();
    ARCH_SYSREG_WRITE_CONST(daifset, DAIF_ABT_BIT);
    ISB();
}

__STATIC_FORCEINLINE void disable_debug_exceptions(void)
{
    __COMPILER_BARRIER();
    ARCH_SYSREG_WRITE_CONST(daifset, DAIF_DBG_BIT);
    ISB();
}


#endif /* AL_AARCH64_ARCH_H */
