#include <stdlib.h>
#include "cma_mem_init.h"

#define LOG_TAG "CMA_MEM_INIT"
#include "elog.h"

AL_S8 cma_mem_init(AL_U32* cma_phy_start,AL_VOID** cma_virt_start) {
    AL_U64 cma_size = 102 * 1024 *1024 ; // 102MB
    (*cma_virt_start) = malloc(cma_size);
    if(*cma_virt_start == NULL) {
        log_e("alloc cma mem failed\n");
        return -1;
    }
    (*cma_phy_start) = (AL_U32) (*cma_virt_start);
    
    return 0;
}