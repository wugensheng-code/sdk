/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"

void __asm_invalidate_icache_all(void);
void __asm_flush_dcache_range(AL_UINTPTR, AL_UINTPTR);
void __asm_invalidate_dcache_range(AL_UINTPTR, AL_UINTPTR);
void __asm_invalidate_dcache_all(void);
void __asm_flush_dcache_all(void);


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
