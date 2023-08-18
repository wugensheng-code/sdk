#include "al_type.h"
#include "al_rv_core.h"


AL_VOID AlCache_EnableICache(AL_VOID)
{
    __RV_CSR_SET(CSR_MCACHE_CTL, CSR_MCACHE_CTL_IE);
    isb();
}


AL_VOID AlCache_DisableICache(AL_VOID)
{
    __RV_CSR_CLEAR(CSR_MCACHE_CTL, CSR_MCACHE_CTL_IE);
    isb();
}


AL_VOID AlCache_EnableDCache(AL_VOID)
{
    __RV_CSR_SET(CSR_MCACHE_CTL, CSR_MCACHE_CTL_DE);
    isb();
}


AL_VOID AlCache_DisableDCache(AL_VOID)
{
    __RV_CSR_CLEAR(CSR_MCACHE_CTL, CSR_MCACHE_CTL_DE);
    isb();
}

#define __enable_FPU()          __RV_CSR_SET(CSR_MSTATUS, MSTATUS_FS)

#define __disable_FPU()         __RV_CSR_CLEAR(CSR_MSTATUS, MSTATUS_FS)
