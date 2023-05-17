#ifndef NPU_RUNTIME_UTIL_FUNC_H
#define NPU_RUNTIME_UTIL_FUNC_H

#include <stdint.h>
#include <stddef.h>
#include "driver_api.h"

/* ------------  the following funcions are generic functions for users  ------------------ */

npu_input_param_t* get_input_param (void * graph) ;

int8_t get_input_tensor_addr (void* graph, void** addr, uint32_t* length) ;

int8_t get_output_tensor_addr (void* graph, void** addr, uint32_t* length) ;

/* ---------------------------------------------------------------------------------------- */

/* ------------  the following funcions are preprocess or postprocess for run_npu_graph  ------------------ */

int8_t load_input_data (char* input_fn, void* addr, uint32_t length) ;

int8_t process_input_image (char* input_fn, void* addr, uint32_t length,  npu_input_param_t* input_param) ;

int8_t draw_yolo_res (char* input_fn, void* addr, uint32_t length,  npu_input_param_t* input_param) ;

int8_t dump_yolo_res (char* output_fn, void* addr, uint32_t length,  npu_input_param_t* input_param) ;

/* ---------------------------------------------------------------------------------------- */

#endif