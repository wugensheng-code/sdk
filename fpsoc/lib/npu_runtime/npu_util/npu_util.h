#ifndef NPU_RUNTIME_UTIL_H
#define NPU_RUNTIME_UTIL_H

#include "npu_rt_graph.h"

void dump_tensor (rt_tensor_t* t) ;

char* strdup(const char* src) ;

// int8_t process_input_image (char* pic_fn, npu_input_param_t* input_param, uint8_t run_platform) ;

// int8_t load_input_data (npu_input_param_t* input_param) ;

void mem_addr(unsigned long vaddr, unsigned long *paddr) ;

int get_uio_dev_count(char *root);

unsigned long get_sys_counter_val(void);

#endif