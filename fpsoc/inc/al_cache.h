#ifndef AL_CACHE_H
#define AL_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

AL_VOID AlIcache_Enable(AL_VOID);

AL_VOID AlIcache_Disable(AL_VOID);

AL_VOID AlIcache_InvalidateAll(AL_VOID);

AL_VOID AlIDache_Enable(AL_VOID);

AL_VOID AlIDache_Disable(AL_VOID);

AL_VOID AlIDache_FlushRange(AL_UINTPTR start, AL_UINTPTR stop);

AL_VOID AlIDache_InvalidateRange(AL_UINTPTR start, AL_UINTPTR stop);

AL_VOID al_dcache_invalidate_all(AL_VOID);

AL_VOID al_dcache_flush_all(AL_VOID);

AL_VOID al_cache_flush(AL_UINTPTR addr, AL_UINTPTR size);

AL_VOID al_enable_caches(AL_VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AL_CACHE_H */
