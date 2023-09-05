
/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_AARCH64_MACRO_H__
#define __AL_AARCH64_MACRO_H__

#include <al_aarch64_sysreg.h>

/*
 * Register aliases.
 */
lr	.req	x30

/*
 * Branch according to exception level
 */
.macro    switch_el, xreg, el3_label, el2_label, el1_label
    mrs \xreg, CurrentEL
    cmp \xreg, 0xc
    b.eq \el3_label
    cmp \xreg, 0x8
    b.eq \el2_label
    cmp \xreg, 0x4
    b.eq \el1_label
.endm

/* switch to el1 from el3 */
.macro    switch_el3_to_el1_el0
    /* (1) The Execution state for EL1 is AArch64, No-Secure */
#ifdef SUPPORT_NONSECURE
    ldr x21, =(SCR_RW_BIT | SCR_NS_BIT)
#else
    ldr x21, =(SCR_RW_BIT)
#endif
    msr scr_el3, x21

    ldr x21, =(0x1<<31)
    msr hcr_el2, x21

    /* (2) set sctlr_el1, disable mmu, LITTLE_ENDIAN */
    mrs x21, sctlr_el1
    bic x21, x21, SCTLR_ELx_M
    bic x21, x21, SCTLR_EL1_E0E
    bic x21, x21, SCTLR_ELx_EE
    msr sctlr_el1, x21

    /* (3) set spsr_el3, return to el1 */
    ldr x21, =(SPSR_AIF | SPSR_M_EL1H)

#ifdef ENABLE_APU_ABORT_AT_ERET
    bic x21, x21, #(0x01 << 8)
#endif

    msr spsr_el3, x21

    /* (4) set el1_entry adress to elr_el3 */
    adr x21, el1_entry
    msr elr_el3, x21
    /* (5) return, jump tp el1_entry adress */
    eret
.endm

#endif /* __AL_AARCH64_MACRO_H__ */
