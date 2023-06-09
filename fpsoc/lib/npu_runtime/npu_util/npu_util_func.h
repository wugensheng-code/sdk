#ifndef NPU_RUNTIME_UTIL_FUNC_H
#define NPU_RUNTIME_UTIL_FUNC_H

#include <stdint.h>
#include <stddef.h>
#include "driver_api.h"

/* ------------  the following funcions are generic functions for users  ------------------ */

npu_input_param_t* get_input_param (AL_VOID * graph) ;

AL_S8 get_input_tensor_addr (AL_VOID* graph, AL_VOID** addr, AL_U32* length) ;

AL_S8 get_output_tensor_addr (AL_VOID* graph, AL_VOID** addr, AL_U32* length) ;

/* ---------------------------------------------------------------------------------------- */

/* ------------  the following funcions are preprocess or postprocess for run_npu_graph  ------------------ */

AL_S8 load_input_data (char* input_fn, AL_VOID* addr, AL_U32 length) ;

AL_S8 process_input_image (char* input_fn, AL_VOID* addr, AL_U32 length,  npu_input_param_t* input_param) ;

AL_S8 draw_yolo_res (char* input_fn, AL_VOID* addr, AL_U32 length,  npu_input_param_t* input_param) ;

AL_S8 dump_yolo_res (char* output_fn, AL_VOID* addr, AL_U32 length,  npu_input_param_t* input_param) ;

/* ---------------------------------------------------------------------------------------- */

#endif