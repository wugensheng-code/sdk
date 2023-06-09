#ifndef __NPU_RUNTIME_CMA_MEM_INIT_H__
#define __NPU_RUNTIME_CMA_MEM_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

AL_S8 cma_mem_init(AL_U32* cma_phy_start,AL_VOID** cma_virt_start);

#ifdef __cplusplus
}
#endif

#endif