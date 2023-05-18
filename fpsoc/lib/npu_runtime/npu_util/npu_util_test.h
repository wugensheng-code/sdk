#ifndef NPU_RUNTIME_UTIL_TEST_H
#define NPU_RUNTIME_UTIL_TEST_H

#include <stdint.h>
#include <stddef.h>
#include "driver_api.h"


/* ------------  the following funcions are preprocess or postprocess for run_npu_graph  ------------------ */

int8_t process_input_image_test (char* input_fn, void* addr, uint32_t length,  npu_input_param_t* input_param) ;

int8_t draw_yolo_res_test (char* input_fn, void* addr, uint32_t length,  npu_input_param_t* input_param) ;

/* ---------------------------------------------------------------------------------------- */

/* ------------  the following funcions are util functions for preprocess/postprocess ------------------ */

int8_t run_yuv2rgb (int h, int w, int format, uint32_t src_addr, uint32_t dst_addr) ;

int8_t rgb_postprocess () ;

/* ---------------------------------------------------------------------------------------- */

#endif