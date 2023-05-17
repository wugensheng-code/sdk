#ifndef NPU_FM_MEM_ALLOC_H
#define NPU_FM_MEM_ALLOC_H

#include "npu_rt_graph.h"

void alloc_mem (rt_tensor_t* ts, uint8_t align) ;

void free_mem (rt_tensor_t* ts) ;

// only for current test solution
void alloc_mem_reset () ;

void alloc_mem_buddy (rt_tensor_t* ts) ;

void free_mem_buddy (rt_tensor_t* ts) ;


#endif