//
// Created by mqye on 2022/11/17.
//
#include "cpu_aclr_op.h"
#include "ff.h"
#include "npu_util.h"

#define CHECK_OVERFLOW if(param->input_zero_point > 128 || param->weight_zero_point > 128)

AL_S8 npu_dwconv_node (npu_dwconv_param_t* param){
    AL_U8 o_dt = param->output_tensor->data_type;
    AL_S8 ret;

    if(o_dt == NPU_DATA_INT8)
        ret = npu_dwconv_node_int8(param);
    else if(o_dt == NPU_DATA_UINT8)
        ret = npu_dwconv_node_uint8(param);
    else{
        printf("Unsupported datatype : %d", o_dt);
        ret = -1;
    }
    return ret;
}

AL_S8 npu_dwconv_node_int8 (npu_dwconv_param_t* param){
    // TODO : Mind special case - multiple inputs.
    AL_S8 *input_buf = NULL;
    AL_S8 *weight_buf = NULL;
    int *bias = NULL;
    AL_S8 *output_data = NULL;

    input_buf = (AL_S8 *) param->input_tensors[0]->addr;
    weight_buf = (AL_S8 *) param->input_tensors[1]->addr;
    bias = (AL_S32 *) param->input_tensors[2]->addr;
    output_data = (AL_S8 *) param->output_tensor[0].addr;

    int input_h = param->ifm_dims[2], input_w = param->ifm_dims[3];
    int output_h = param->ofm_dims[2], output_w = param->ofm_dims[3];

    npu_dwconv_param_t* p = param;
    for (int i = 0; i < param->ifm_dims[1]; ++i) {
        AL_S8 *input_tmp = input_buf + i * input_h * input_w;
        int *bias_tmp = bias ? (bias + i) : NULL;

        if(p->param.kernel_h == 3 && p->param.kernel_w == 3){
            if(p->param.stride_h == 1 && p->param.stride_w == 1)
                depthwise_k3s1p1_int8_a72(
                        input_tmp, weight_buf + p->param.kernel_h * p->param.kernel_w * i,
                        output_data + i * output_h * output_w, bias + i,
                        output_h, output_w, p->fx_multi[i], p->fx_qshifts[i], input_w,
                        p->activation_min, p->activation_max);
            else if(p->param.stride_h == 2 && p->param.stride_w == 2)
                depthwise_k3s2p1_int8_a72(
                        input_tmp, weight_buf + p->param.kernel_h * p->param.kernel_w * i,
                        output_data + i * output_h * output_w, bias + i,
                        output_h, output_w, p->fx_multi[i], p->fx_qshifts[i], input_w,
                        p->activation_min, p->activation_max);
        }
        else if (p->param.kernel_h == 7 && p->param.kernel_w == 7){
            depthwise_k7s1p0_int8_a72(
                    input_tmp, weight_buf + p->param.kernel_h * p->param.kernel_w * i,
                    output_data + i * output_h * output_w, bias + i,
                    output_h, output_w, p->fx_multi[i], p->fx_qshifts[i], input_w,
                    p->activation_min, p->activation_max);
        }


    }

    return 0;
}

AL_S8 npu_dwconv_node_uint8 (npu_dwconv_param_t* param){
    // TODO : Mind special case - multiple inputs.
    AL_U8 *input_buf_uint8 = NULL;
    AL_U8 *weight_buf_uint8 = NULL;
    int *bias = NULL;
    AL_U8 *output_data = NULL;

    input_buf_uint8 = (AL_U8 *) param->input_tensors[0]->addr;
    weight_buf_uint8 = (AL_U8 *) param->input_tensors[1]->addr;
    bias = (AL_S32 *) param->input_tensors[2]->addr;
    output_data = (AL_U8 *) param->output_tensor[0].addr;

    int input_h = param->ifm_dims[2], input_w = param->ifm_dims[3];
    int output_h = param->ofm_dims[2], output_w = param->ofm_dims[3];
    int input_patch_size = input_h * input_w;
    int output_patch_size = output_h * output_w;

    npu_dwconv_param_t* p = param;
    int kernel_size = p->param.kernel_h * p->param.kernel_w;

    CHECK_OVERFLOW
    printf("input_zero_point : %d, weight_zero_point : %d\n", param->input_zero_point, param->weight_zero_point);

    for (int i = 0; i < param->ifm_dims[1]; ++i) {
        AL_S16 *input_tmp = (AL_S16*) malloc(sizeof(AL_S16) * input_patch_size);
        for (int j = 0; j < input_patch_size; ++j) {
            input_tmp[j] = input_buf_uint8[i * input_patch_size + j] - param->input_zero_point;
        }

        AL_S16 *weight_tmp = (AL_S16*) malloc(sizeof(AL_S16) * kernel_size);
        for (int j = 0; j < kernel_size; ++j) {
            weight_tmp[j] = weight_buf_uint8[i * kernel_size + j] - param->weight_zero_point;
        }

        AL_S8 *output_tmp = (AL_S8*) malloc(sizeof(AL_S8) * output_patch_size);

        if(p->param.kernel_h == 3 && p->param.kernel_w == 3) {
            if (p->param.stride_h == 1 && p->param.stride_w == 1)
                depthwise_k3s1p1_int16_a72(
                        input_tmp, weight_tmp, output_tmp, bias + i,
                        output_h, output_w, p->fx_multi[0], p->fx_qshifts[0], input_w * 2,
                        p->activation_min, p->activation_max);
            else if (p->param.stride_h == 2 && p->param.stride_w == 2)
                depthwise_k3s2p1_int16_a72(
                        input_tmp, weight_tmp, output_tmp, bias + i,
                        output_h, output_w, p->fx_multi[0], p->fx_qshifts[0], input_w * 2,
                        p->activation_min, p->activation_max);
        }
        else if (p->param.kernel_h == 7 && p->param.kernel_w == 7){
            depthwise_k7s1p0_int16_a72(
                    input_tmp, weight_tmp, output_tmp, bias + i,
                    output_h, output_w, p->fx_multi[0], p->fx_qshifts[0], input_w * 2,
                    p->activation_min, p->activation_max);

        }
        else{
            printf("Unsupported dwconv kernel size : %d x %d\n", p->param.kernel_h, p->param.kernel_w);
        }

        for (int j = 0; j < output_patch_size; ++j) {
            (output_data + i * output_patch_size) [j] = output_tmp[j] + param->output_zero_point;
        }
    }

    return 0;
}