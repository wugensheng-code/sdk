/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_RV_CORE__
#define __AL_RV_CORE__

#ifdef __cplusplus
 extern "C" {
#endif

#include "al_type.h"
#include "al_compiler.h"
#include "al_rv64_sysregs.h"

#define ISA_SYSREG_READ     "csrr"
#define ISA_SYSREG_WRITE    "csrw"
#define ISA_SWAP            "csrrw"
#define ISA_READ_SET        "csrrs"
#define ISA_SET             "csrs"
#define ISA_READ_CLEAR      "csrrc"
#define ISA_CLEAR           "csrc"

#include "al_arch_sysops.h"

__STATIC_FORCEINLINE void enable_irq(void)
{
    ARCH_SYSREG_SET(CSR_MSTATUS, MSTATUS_MIE);
}

__STATIC_FORCEINLINE void set_irq_mask(AL_U32 Mask)
{
    ARCH_SYSREG_SET(CSR_MSTATUS, Mask);
}

__STATIC_FORCEINLINE AL_U32 get_irq_mask(void)
{
    return (AL_U32)ARCH_SYSREG_READ(CSR_MSTATUS);
}

__STATIC_FORCEINLINE void disable_irq(void)
{
    ARCH_SYSREG_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
}

__STATIC_FORCEINLINE void enable_fpu(void)
{
    ARCH_SYSREG_SET(CSR_MSTATUS, MSTATUS_FS);
}

__STATIC_FORCEINLINE void disable_fpu(void)
{
    ARCH_SYSREG_CLEAR(CSR_MSTATUS, MSTATUS_FS);
}

__STATIC_FORCEINLINE void __WFI(void)
{
    ARCH_SYSREG_CLEAR(CSR_WFE, WFE_WFE);
    __ASM volatile("wfi");
}


__STATIC_FORCEINLINE void __WFE(void)
{
    ARCH_SYSREG_SET(CSR_WFE, WFE_WFE);
    __ASM volatile("wfi");
    ARCH_SYSREG_CLEAR(CSR_WFE, WFE_WFE);
}

__STATIC_FORCEINLINE void __FENCE_I(void)
{
    __ASM volatile("fence.i");
}

#define __FENCE(p, s)                                            \
        __ASM volatile ("fence " #p "," #s                       \
        :                                                        \
        :                                                        \
        : "memory")                                              \

#define __RWMB()   __FENCE(iorw,iorw)

#ifdef __cplusplus
}
#endif
#endif
