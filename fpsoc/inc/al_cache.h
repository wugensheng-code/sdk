/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_CACHE_H
#define AL_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef ENABLE_MMU
#define CACHE_LINE_ALIGN cacheline_aligned
#else
#define CACHE_LINE_ALIGN
#endif

AL_VOID AlCache_EnableICache(AL_VOID);

AL_VOID AlCache_DisableICache(AL_VOID);

AL_VOID AlCache_EnableDCache(AL_VOID);

AL_VOID AlCache_DisableDCache(AL_VOID);

AL_VOID AlCache_InvalidateIcacheAll(AL_VOID);

AL_VOID AlCache_FlushDcacheRange(AL_UINTPTR Start, AL_UINTPTR End);

AL_VOID AlCache_InvalidateDcacheRange(AL_UINTPTR Start, AL_UINTPTR End);

AL_VOID AlCache_FlushAndInvalidateDcacheRange(AL_UINTPTR FStart, AL_UINTPTR FEnd, AL_UINTPTR IStart, AL_UINTPTR IEnd);

AL_VOID AlCache_InvalidateDcacheAll(AL_VOID);

AL_VOID AlCache_FlushDcacheAll(AL_VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AL_CACHE_H */
