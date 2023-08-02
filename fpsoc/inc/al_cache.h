#ifndef AL_CACHE_H
#define AL_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

AL_VOID AlIcache_InvalidateAll(AL_VOID);

AL_VOID AlDcache_FlushRange(AL_UINTPTR Start, AL_UINTPTR End);

AL_VOID AlDcache_InvalidateRange(AL_UINTPTR Start, AL_UINTPTR End);

AL_VOID AlDcache_InvalidateAll(AL_VOID);

AL_VOID AlDcache_FlushAll(AL_VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AL_CACHE_H */
