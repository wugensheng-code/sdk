//
// Created by mqye on 2022/11/17.
//

#ifndef SOC_APU_SDK_NPU_ARM_OP_H
#define SOC_APU_SDK_NPU_ARM_OP_H

#include "npu_rt_graph.h"
#include <malloc.h>
#include <stdlib.h>
#include "driver_api.h"

#include <stdio.h>
#include "npu_init.h"
#include "npu_scheduler.h"
#include "fm_mem_alloc.h"

//AL_VOID depthwise_k3s1p1_int8_a72(AL_S8* input, AL_S8* kernel, AL_S8* out, int* bias, long out_h, long out_w,
//                               long multi, long shift, long input_w, long act_min, long act_max);

AL_VOID depthwise_k3s1p1_int8_a72(AL_S8*, AL_S8*, AL_S8*, int*, long, long, long, long, long, long, long);
AL_VOID depthwise_k3s2p1_int8_a72(AL_S8*, AL_S8*, AL_S8*, int*, long, long, long, long, long, long, long);
AL_VOID depthwise_k7s1p0_int8_a72(AL_S8*, AL_S8*, AL_S8*, int*, long, long, long, long, long, long, long);

// uint8, int9
AL_VOID depthwise_k3s1p1_int16_a72(AL_S8*, AL_S8*, AL_S8*, int*, long, long, long, long, long, long, long);
AL_VOID depthwise_k3s2p1_int16_a72(AL_S8*, AL_S8*, AL_S8*, int*, long, long, long, long, long, long, long);
AL_VOID depthwise_k7s1p0_int16_a72(AL_S8*, AL_S8*, AL_S8*, int*, long, long, long, long, long, long, long);

AL_VOID dw_k5s1(float*, float*, float*, float*, int, int, int);

AL_S8 npu_dwconv_node (npu_dwconv_param_t* param);

inline AL_S8 npu_dwconv_node_int8 (npu_dwconv_param_t* param);

inline AL_S8 npu_dwconv_node_uint8 (npu_dwconv_param_t* param);


// Image Processing
AL_VOID image_resize_dim2(AL_S8*, AL_S8*,long, long, long, long);
AL_VOID image_yuv2rgb();

#endif //SOC_APU_SDK_NPU_ARM_OP_H
