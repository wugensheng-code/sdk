#ifndef AL_CACHE_H
#define AL_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

AL_VOID AlCache_EnableICache(AL_VOID);

AL_VOID AlCache_DisableICache(AL_VOID);

AL_VOID AlCache_EnableDCache(AL_VOID);

AL_VOID AlCache_DisableDCache(AL_VOID);

AL_VOID AlCache_InvalidateIcacheAll(AL_VOID);

AL_VOID AlCache_FlushDcacheRange(AL_UINTPTR Start, AL_UINTPTR End);

AL_VOID AlCache_InvalidateDcacheRange(AL_UINTPTR Start, AL_UINTPTR End);

AL_VOID AlCache_InvalidateDcacheAll(AL_VOID);

AL_VOID AlCache_FlushDcacheAll(AL_VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AL_CACHE_H */
