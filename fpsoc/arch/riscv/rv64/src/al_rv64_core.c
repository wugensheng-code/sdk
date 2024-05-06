/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"
#include "al_rv64_core.h"
#include "al_barrier.h"
#include "al_cache.h"


AL_VOID AlCache_EnableICache(AL_VOID)
{
    ARCH_SYSREG_SET(CSR_MCACHE_CTL, CSR_MCACHE_CTL_IE);
    ISB();
}


AL_VOID AlCache_DisableICache(AL_VOID)
{
    ARCH_SYSREG_CLEAR(CSR_MCACHE_CTL, CSR_MCACHE_CTL_IE);
    ISB();
}


AL_VOID AlCache_EnableDCache(AL_VOID)
{
    ARCH_SYSREG_SET(CSR_MCACHE_CTL, CSR_MCACHE_CTL_DE);
    ISB();
}


AL_VOID AlCache_DisableDCache(AL_VOID)
{
    ARCH_SYSREG_CLEAR(CSR_MCACHE_CTL, CSR_MCACHE_CTL_DE);
    ISB();
}


AL_VOID AlCore_EnableBPU(AL_VOID)
{
    ARCH_SYSREG_SET(CSR_MMISC_CTL, MMISC_CTL_BPU);
    ISB();
}

AL_VOID AlCore_DisableBPU(AL_VOID)
{
    ARCH_SYSREG_CLEAR(CSR_MMISC_CTL, MMISC_CTL_BPU);
    ISB();
}
