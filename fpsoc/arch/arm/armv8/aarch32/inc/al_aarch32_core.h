/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_AARCH32_CORE_H_
#define __AL_AARCH32_CORE_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "al_type.h"
#include "al_compiler.h"
#include "al_barrier.h"
#include "al_aarch32_sysreg.h"

#define ISA_COPROCR_READ          "mrc"
#define ISA_COPROCR_WRITE         "mcr"

#define ISA_COPROCR_READ_64       "mrrc"
#define ISA_COPROCR_WRITE_64      "mcrr"

#define ISA_SYSREG_READ           "mrs"
#define ISA_SYSREG_WRITE          "msr"


#define ISA_SYSREG_WRITE_CONST    ISA_SYSREG_WRITE

#include "al_arch_sysops.h"

__STATIC_FORCEINLINE void enable_irq(void)
{
    asm volatile(
        "    cpsie i            "
        :
        :
        : "memory", "cc");
    ISB();
}

__STATIC_FORCEINLINE void enable_fiq(void)
{
    asm volatile(
        "    cpsie f            "
        :
        :
        : "memory", "cc");
    ISB();
}

__STATIC_FORCEINLINE void enable_all_intr(void)
{
    asm volatile(
        "    cpsie aif            "
        :
        :
        : "memory", "cc");
}

__STATIC_FORCEINLINE void enable_serror(void)
{
    asm volatile(
        "    cpsie a            "
        :
        :
        : "memory", "cc");
    ISB();
}

__STATIC_FORCEINLINE void enable_debug_exceptions(void)
{
    asm volatile(
        "    cpsie d            "
        :
        :
        : "memory", "cc");
    ISB();
}

__STATIC_FORCEINLINE void disable_irq(void)
{
    asm volatile(
        "    cpsid i            "
        :
        :
        : "memory", "cc");
    ISB();
}

__STATIC_FORCEINLINE void disable_fiq(void)
{
    asm volatile(
        "    cpsid f            "
        :
        :
        : "memory", "cc");
    ISB();
}

__STATIC_FORCEINLINE void disable_all_intr(void)
{
    asm volatile(
        "    cpsid aif            "
        :
        :
        : "memory", "cc");
    ISB();
}

__STATIC_FORCEINLINE void disable_serror(void)
{
    asm volatile(
        "    cpsid a            "
        :
        :
        : "memory", "cc");
    ISB();
}

__STATIC_FORCEINLINE void disable_debug_exceptions(void)
{
    asm volatile(
        "    cpsid d            "
        :
        :
        : "memory", "cc");
    ISB();
}

__STATIC_FORCEINLINE int get_intr_mask(void)
{

}

__STATIC_FORCEINLINE void set_intr_mask(int Mask)
{

}

__STATIC_FORCEINLINE int get_current_el(void)
{

}

__STATIC_FORCEINLINE int get_sctlr(void)
{

}

__STATIC_FORCEINLINE void set_sctlr(unsigned long val)
{

}

#ifdef __cplusplus
}
#endif
#endif

