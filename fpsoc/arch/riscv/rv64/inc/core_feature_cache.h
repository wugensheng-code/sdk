#ifndef __CORE_FEATURE_CACHE_H__
#define __CORE_FEATURE_CACHE_H__

#ifdef __cplusplus
 extern "C" {
#endif


__STATIC_FORCEINLINE void EnableICache (void)
{
    __RV_CSR_SET(CSR_MCACHE_CTL, CSR_MCACHE_CTL_IE);
}


__STATIC_FORCEINLINE void DisableICache (void)
{
    __RV_CSR_CLEAR(CSR_MCACHE_CTL, CSR_MCACHE_CTL_IE);
}


__STATIC_FORCEINLINE void EnableDCache (void)
{
    __RV_CSR_SET(CSR_MCACHE_CTL, CSR_MCACHE_CTL_DE);
}


__STATIC_FORCEINLINE void DisableDCache (void)
{
    __RV_CSR_CLEAR(CSR_MCACHE_CTL, CSR_MCACHE_CTL_DE);
}

#define __enable_FPU()          __RV_CSR_SET(CSR_MSTATUS, MSTATUS_FS)

#define __disable_FPU()         __RV_CSR_CLEAR(CSR_MSTATUS, MSTATUS_FS)


#ifdef __cplusplus
}
#endif
#endif
