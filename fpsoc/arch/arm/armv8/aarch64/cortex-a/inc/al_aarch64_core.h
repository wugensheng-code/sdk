/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_AARCH64_CORE_H_
#define __AL_AARCH64_CORE_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "al_aarch64_sysreg.h"
#include "al_compiler.h"
#include "al_barrier.h"
#include "al_type.h"

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


#include "al_arch_sysops.h"

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
    ARCH_SYSREG_WRITE_CONST(daifclr, DAIF_IRQ_BIT | DAIF_FIQ_BIT | DAIF_ABT_BIT);
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

__STATIC_FORCEINLINE int get_intr_mask(void)
{
    return (int)ARCH_SYSREG_READ(daif);
}

__STATIC_FORCEINLINE void set_intr_mask(int Mask)
{
    __COMPILER_BARRIER();
    ARCH_SYSREG_WRITE(daif, Mask);
    ISB();
}

__STATIC_FORCEINLINE int get_current_el(void)
{
    return (ARCH_SYSREG_READ(CurrentEL) >> 2) & 0x3;
}

__STATIC_FORCEINLINE int get_sctlr(void)
{
    unsigned int el;
    unsigned long val;

    el = get_current_el();

    if (el == 1) {
        val = ARCH_SYSREG_READ(sctlr_el1);
    } else if (el == 2) {
        val = ARCH_SYSREG_READ(sctlr_el2);
    } else {
        val = ARCH_SYSREG_READ(sctlr_el3);
    }

    return val;
}

__STATIC_FORCEINLINE void set_sctlr(unsigned long val)
{
    unsigned int el;

    el = get_current_el();

    __COMPILER_BARRIER();

    if (el == 1) {
        ARCH_SYSREG_WRITE(sctlr_el1, val);
    } else if (el == 2) {
        ARCH_SYSREG_WRITE(sctlr_el2, val);
    } else {
        ARCH_SYSREG_WRITE(sctlr_el3, val);
    }

    ISB();
}

#ifdef __cplusplus
}
#endif
#endif

