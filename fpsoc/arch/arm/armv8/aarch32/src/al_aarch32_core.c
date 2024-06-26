/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"
#include "al_utils_def.h"
#include "al_mmu.h"
#include "al_cache.h"

AL_VOID __asm_invalidate_icache_all(AL_VOID);
AL_VOID __asm_flush_dcache_range(AL_UINTPTR, AL_UINTPTR);
AL_VOID __asm_invalidate_dcache_range(AL_UINTPTR, AL_UINTPTR);
AL_VOID __asm_invalidate_dcache_all(AL_VOID);
AL_VOID __asm_flush_dcache_all(AL_VOID);

AL_VOID AlCache_EnableICache(AL_VOID)
{

}

AL_VOID AlCache_DisableICache(AL_VOID)
{

}

AL_VOID AlCache_EnableDCache(AL_VOID)
{

}

AL_VOID AlCache_DisableDCache(AL_VOID)
{

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
    AL_UINTPTR Addr;
    AL_U32 Size;

    AL_ASSERT((Start & 0xFFFFF) == 0, AL_ERR_ILLEGAL_PARAM);
    AL_ASSERT((End & 0xFFFFF) == 0, AL_ERR_ILLEGAL_PARAM);
    AL_ASSERT((Attr == Al_MEM_DMA || Attr == NORM_CACHE), AL_ERR_ILLEGAL_PARAM);

    for (Addr = Start; Addr < End; Addr += Size) {
        Size = mmu_settlb(Addr, (Attr == Al_MEM_DMA) ? DEVICE_MEM : NORM_CACHE);
    }

    return AL_OK;
}
