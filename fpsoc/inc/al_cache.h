/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_CACHE_H_
#define __AL_CACHE_H_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "al_type.h"
#include "al_compiler.h"

#ifdef ENABLE_MMU
#define CACHE_LINE_ALIGN cacheline_aligned
#else
#define CACHE_LINE_ALIGN
#endif

typedef enum AL_MemAttr {
    Al_MEM_DMA,
    Al_MEM_NORM_CACHE,
} AL_MemAttr;

AL_VOID AlCache_EnableICache(AL_VOID);

AL_VOID AlCache_DisableICache(AL_VOID);

AL_VOID AlCache_EnableDCache(AL_VOID);

AL_VOID AlCache_DisableDCache(AL_VOID);

AL_VOID AlCache_InvalidateIcacheAll(AL_VOID);

AL_VOID AlCache_FlushDcacheRange(AL_UINTPTR Start, AL_UINTPTR End);

AL_VOID AlCache_InvalidateDcacheRange(AL_UINTPTR Start, AL_UINTPTR End);

AL_VOID AlCache_FlushAndInvalidateSameDcacheRange(AL_UINTPTR Start, AL_UINTPTR End);

AL_VOID AlCache_FlushAndInvalidateDiffDcacheRange(AL_UINTPTR FStart, AL_UINTPTR FEnd, AL_UINTPTR IStart, AL_UINTPTR IEnd);

AL_VOID AlCache_InvalidateDcacheAll(AL_VOID);

AL_VOID AlCache_FlushDcacheAll(AL_VOID);

AL_VOID AlCache_DisableMmu(AL_VOID);

AL_S32 AlCache_SetMemoryAttr(AL_UINTPTR Start, AL_UINTPTR End, AL_MemAttr Attr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AL_CACHE_H */
