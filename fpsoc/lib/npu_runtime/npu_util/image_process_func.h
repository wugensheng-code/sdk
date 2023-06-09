#ifndef NPU_RUNTIME_UTIL_TEST_H
#define NPU_RUNTIME_UTIL_TEST_H

#include <stdint.h>
#include <stddef.h>
#include "driver_api.h"


/* ------------  the following funcions are util functions for preprocess/postprocess 
                            and are related to function modules on FPGA ------------------ */

AL_S8 run_yuv2rgb (int h, int w, int format, AL_U32 src_addr, AL_U32 dst_addr, npu_input_param_t* input_param) ;


/* ---------------------------------------------------------------------------------------- */

#endif