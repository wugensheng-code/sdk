#ifndef NPU_FM_MEM_ALLOC_H
#define NPU_FM_MEM_ALLOC_H

#include "npu_rt_graph.h"

AL_VOID alloc_mem (rt_tensor_t* ts, AL_U8 align) ;

AL_VOID free_mem (rt_tensor_t* ts) ;

// only for current test solution
AL_VOID alloc_mem_reset () ;

AL_VOID alloc_mem_buddy (rt_tensor_t* ts) ;

AL_VOID free_mem_buddy (rt_tensor_t* ts) ;


#endif