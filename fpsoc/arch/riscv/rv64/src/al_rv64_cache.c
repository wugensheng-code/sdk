#include "al_type.h"
#include "al_rv_core.h"
#include "al_barrier.h"


AL_VOID AlCache_EnableICache(AL_VOID)
{
    ARCH_SYSREG_SET(CSR_MCACHE_CTL, CSR_MCACHE_CTL_IE);
    ISB();
}


AL_VOID AlCache_DisableICache(AL_VOID)
{
    ARCH_SYSREG_CLEAR(CSR_MCACHE_CTL, CSR_MCACHE_CTL_IE);
    ISB();
}


AL_VOID AlCache_EnableDCache(AL_VOID)
{
    ARCH_SYSREG_SET(CSR_MCACHE_CTL, CSR_MCACHE_CTL_DE);
    ISB();
}


AL_VOID AlCache_DisableDCache(AL_VOID)
{
    ARCH_SYSREG_CLEAR(CSR_MCACHE_CTL, CSR_MCACHE_CTL_DE);
    ISB();
}

#define __enable_FPU()          ARCH_SYSREG_SET(CSR_MSTATUS, MSTATUS_FS)

#define __disable_FPU()         ARCH_SYSREG_CLEAR(CSR_MSTATUS, MSTATUS_FS)
