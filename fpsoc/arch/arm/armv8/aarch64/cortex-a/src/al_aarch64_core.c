/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"
#include "al_utils_def.h"
#include "al_mmu.h"
#include "al_cache.h"
#include "al_aarch64_sysreg.h"
#include "al_aarch64_core.h"

AL_VOID __asm_invalidate_icache_all(AL_VOID);
AL_VOID __asm_flush_dcache_range(AL_UINTPTR, AL_UINTPTR);
AL_VOID __asm_invalidate_dcache_range(AL_UINTPTR, AL_UINTPTR);
AL_VOID __asm_flush_and_invalidate_same_dcache_range(AL_UINTPTR, AL_UINTPTR);
AL_VOID __asm_flush_and_invalidate_diff_dcache_range(AL_UINTPTR, AL_UINTPTR, AL_UINTPTR, AL_UINTPTR);
AL_VOID __asm_invalidate_dcache_all(AL_VOID);
AL_VOID __asm_flush_dcache_all(AL_VOID);
AL_VOID disable_mmu(AL_VOID);

AL_VOID AlCache_EnableICache(AL_VOID)
{
    __asm_invalidate_icache_all();
    set_sctlr(get_sctlr() | SCTLR_ELx_I);
}

AL_VOID AlCache_DisableICache(AL_VOID)
{
    set_sctlr(get_sctlr() & ~SCTLR_ELx_I);
}

AL_VOID AlCache_EnableDCache(AL_VOID)
{
    set_sctlr(get_sctlr() | SCTLR_ELx_C);
}

AL_VOID AlCache_DisableDCache(AL_VOID)
{
    __asm_flush_dcache_all();
    set_sctlr(get_sctlr() & ~SCTLR_ELx_C);
}

AL_VOID AlCache_InvalidateIcacheAll(AL_VOID)
{
    __asm_invalidate_icache_all();
}

AL_VOID AlCache_FlushDcacheRange(AL_UINTPTR Start, AL_UINTPTR End)
{
    __asm_flush_dcache_range(Start, End);
}

AL_VOID AlCache_InvalidateDcacheRange(AL_UINTPTR Start, AL_UINTPTR End)
{
    __asm_invalidate_dcache_range(Start, End);
}

AL_VOID AlCache_FlushAndInvalidateSameDcacheRange(AL_UINTPTR Start, AL_UINTPTR End)
{
    __asm_flush_and_invalidate_same_dcache_range(Start, End);
}

AL_VOID AlCache_FlushAndInvalidateDiffDcacheRange(AL_UINTPTR FStart, AL_UINTPTR FEnd, AL_UINTPTR IStart, AL_UINTPTR IEnd)
{
    __asm_flush_and_invalidate_diff_dcache_range(FStart, FEnd, IStart, IEnd);
}

AL_VOID AlCache_InvalidateDcacheAll(AL_VOID)
{
    __asm_invalidate_dcache_all();
}

AL_VOID AlCache_FlushDcacheAll(AL_VOID)
{
    __asm_flush_dcache_all();
}

AL_VOID AlCache_DisableMmu(AL_VOID)
{
    disable_mmu();
}

AL_S32 AlCache_SetMemoryAttr(AL_UINTPTR Start, AL_UINTPTR End, AL_MemAttr Attr)
{
#ifdef DDR_2M_MAPPING
    AL_UINTPTR Addr;
    AL_U32 Size;

    AL_ASSERT(MEM_2M_ALIGN(Start), AL_ERR_ILLEGAL_PARAM);
    AL_ASSERT(MEM_2M_ALIGN(End), AL_ERR_ILLEGAL_PARAM);
    AL_ASSERT((Attr == Al_MEM_DMA || Attr == NORM_CACHE), AL_ERR_ILLEGAL_PARAM);

    for (Addr = Start; Addr < End; Addr += Size) {
        Size = mmu_settlb(Addr, (Attr == Al_MEM_DMA) ? DEVICE_MEM : NORM_CACHE);
    }

    return AL_OK;
#else
    return AL_OK;
#endif
}


AL_S32 AlCore_GetCoreId(AL_VOID)
{
    return (ARCH_SYSREG_READ(mpidr_el1) & 0xFF);
}