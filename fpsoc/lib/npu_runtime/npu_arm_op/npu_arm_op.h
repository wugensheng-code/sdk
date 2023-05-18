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
#include "gic.h"

//void depthwise_k3s1p1_int8_a72(int8_t* input, int8_t* kernel, int8_t* out, int* bias, long out_h, long out_w,
//                               long multi, long shift, long input_w, long act_min, long act_max);

void depthwise_k3s1p1_int8_a72(int8_t*, int8_t*, int8_t*, int*, long, long, long, long, long, long, long);
void depthwise_k3s2p1_int8_a72(int8_t*, int8_t*, int8_t*, int*, long, long, long, long, long, long, long);
void depthwise_k7s1p0_int8_a72(int8_t*, int8_t*, int8_t*, int*, long, long, long, long, long, long, long);

// uint8, int9
void depthwise_k3s1p1_int16_a72(int8_t*, int8_t*, int8_t*, int*, long, long, long, long, long, long, long);
void depthwise_k3s2p1_int16_a72(int8_t*, int8_t*, int8_t*, int*, long, long, long, long, long, long, long);
void depthwise_k7s1p0_int16_a72(int8_t*, int8_t*, int8_t*, int*, long, long, long, long, long, long, long);

void dw_k5s1(float*, float*, float*, float*, int, int, int);

int8_t npu_dwconv_node (npu_dwconv_param_t* param);

inline int8_t npu_dwconv_node_int8 (npu_dwconv_param_t* param);

inline int8_t npu_dwconv_node_uint8 (npu_dwconv_param_t* param);


// Image Processing
void image_resize_dim2(int8_t*, int8_t*,long, long, long, long);
void image_yuv2rgb();

#endif //SOC_APU_SDK_NPU_ARM_OP_H
