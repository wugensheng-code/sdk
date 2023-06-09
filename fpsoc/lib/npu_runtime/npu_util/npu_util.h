#ifndef NPU_RUNTIME_UTIL_H
#define NPU_RUNTIME_UTIL_H

#include "npu_rt_graph.h"

AL_VOID dump_tensor (rt_tensor_t* t) ;

char* strdup(const char* src) ;

unsigned long get_sys_counter_val(AL_VOID);

#endif