
/* it's better for driver to be as simple and fast as possible */

#ifndef _NPU_DRIVER_API_H
#define _NPU_DRIVER_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "al_type.h"

#define RT_MAX_SHAPE_DIM_NUM        8 

#define ACT_LRELU 0
#define ACT_RELU 1
#define ACT_MISH 2
#define ACT_HARDSWISH 3
#define ACT_PRELU 4

#define NPU_HARD_CONV 0
#define NPU_HARD_FC 1

#define NPU_TYPE_HARD 0
#define NPU_TYPE_SOFT 1

#define NPU_DATA_FP32 0
#define NPU_DATA_FP16 1
#define NPU_DATA_INT8 2
#define NPU_DATA_UINT8 3
#define NPU_DATA_INT32 4
#define NPU_DATA_INT16 5

#define NPU_TENSOR_LAYOUT_NCHW 0
#define NPU_TENSOR_LAYOUT_NHWC 1


typedef struct {

    AL_U8 data_type ;
    AL_U8 layout ;
    AL_U8 dim_num ;
    AL_S32 dims[RT_MAX_SHAPE_DIM_NUM] ;

    AL_U32 addr ;

    AL_U64 size ; // the size of tensor data

} tensor_param_t ;


/* following struct(s) is(are) for npu hard node */

typedef struct {

    AL_S32 kernel_h ;
    AL_S32 kernel_w ;
    AL_S32 stride_h ;
    AL_S32 stride_w ;
    AL_S32 pad_h0 ;
    AL_S32 pad_h1 ;
    AL_S32 pad_w0 ;
    AL_S32 pad_w1 ;
    AL_S32 dilation_h ;
    AL_S32 dilation_w ;
    AL_S32 group ;
    AL_S32 input_channel ;
    AL_S32 output_channel ;
} conv_param_t ;

typedef struct {

    /* act type select */
    AL_U8 func_sel ;

    /* in/out quant param */
    AL_U8  up_scale ;
    AL_U16 scale_i ;
    AL_U16 scale_o ;
    AL_U16 scale_p ;
    AL_U16 scale_n ;
    AL_U8 rshift_i ;
    AL_U8 rshift_o ;
    AL_U8 rshift_p ;
    AL_U8 rshift_n ;

    AL_S32 output_zero_point ;
} act_param_t ;

typedef struct {

    /* input/output tensor info, assigned during run time */
    AL_U8 input_num ;
    tensor_param_t** input_tensors ;
    tensor_param_t* output_tensor ;

    /* bias data. if it's null, there is no bias tensor. */
    AL_S32* bias_data ; // fc needed fixed

    /* conv param */
    conv_param_t param ;
    AL_U8 conv_type ;
    AL_U8 mode_load_data ;

    /* fixed-point parameters */
    int fx_param_count ; // at least 1 should be present
    AL_U16* fx_scales ;  
    AL_U8* fx_rshifts ;
    AL_S32 input_zero_point ;
    AL_S32 weight_zero_point ;
    AL_S32 output_zero_point ;
    
    /* for hardware */
    AL_S32 ifm_dims[4] ; //n,c,h,w
    AL_S32 ofm_dims[4] ; //n,c,h,w

    /* for act node */
    AL_U8 has_act ;
    act_param_t* act_param ;

    /* for pooling node */
    AL_U8 has_pooling ;

} npu_hard_param_t ;


/* following struct(s) is(are) for pooling node */

typedef struct {

    /* input/output tensor info */
    //AL_U8 input_num ;
    tensor_param_t* input_tensor ;
    // AL_U8 output_num ;
    tensor_param_t* output_tensor ;

    /* for quant */
    AL_U16 scale_io ;
    AL_U8 rshift_io ;
    AL_S16 zp_i ;
    AL_S16 zp_o ;

    /* pooling param */
    AL_S32 pool_method ; // 0:max    1:avg
    AL_S32 kernel_h ;
    AL_S32 kernel_w ;
    AL_S32 stride_h ;
    AL_S32 stride_w ;
    AL_S32 pad_h0 ;
    AL_S32 pad_h1 ;
    AL_S32 pad_w0 ;
    AL_S32 pad_w1 ;

} npu_pooling_param_t ;


/* following struct(s) is(are) for concat node */

typedef struct {

    /* input/output tensor info */
    AL_U8 input_num ;
    tensor_param_t** input_tensors ;
    // AL_U8 output_num ;
    tensor_param_t* output_tensor ;

    /* concat param */
    AL_S32 axis ;
    
    /* ifm order select */
    AL_U8 ifm_order ; // 0: ifm order by increase address, 1: ifm order by decrease address

    /* for quant: array length for input is input_num */
    AL_U16* scale_i ; 
    AL_U8* rshift_i ;
    AL_S16* zp_i ;
    AL_S16 zp_o ;

} npu_concat_param_t ;


/* following struct(s) is(are) for upsample node */

typedef struct {

    /* input/output tensor info */
    //AL_U8 input_num ;
    tensor_param_t* input_tensor ;
    // AL_U8 output_num ;
    tensor_param_t* output_tensor ;

    /* for quant */
    AL_U16 scale_io ;
    AL_U8 rshift_io ;
    AL_S16 zp_i ;
    AL_S16 zp_o ;

} npu_upsample_param_t ;


/* following struct(s) is(are) for eltwise node */

typedef struct {

    /* input/output tensor info */
    AL_U8 input_num ;
    tensor_param_t** input_tensors ;
    // AL_U8 output_num ;
    tensor_param_t* output_tensor ;

    /* eltwise param */
    AL_U8 type ;

    /* state select */
    AL_U8 state ;

    /* ifm order select */
    AL_U8 ifm_order ; // 0: ifm order by increase address, 1: ifm order by decrease address

    /* for quant: array length for input is input_num */
    AL_U16* scale_io ;
    AL_U8* rshift_io ;
    AL_S16* zp_i ;
    AL_S16 zp_o ;

} npu_eltwise_param_t ;


/* following struct(s) is(are) for prelu node */

typedef struct {

    /* input/output tensor info */
    tensor_param_t* input_tensor ;
    // AL_U8 output_num ;
    tensor_param_t* output_tensor ;

    /* prelu param TODO: */
    // slope data ?

    /* for quant */
    // ?: AL_U16 scale_io ;
    // ?: AL_U8 rshift_io ;
    AL_S16 zp_i ;
    AL_S16 zp_o ;

} npu_prelu_param_t ;


/* following struct(s) is(are) for interp node */

typedef struct {

    /* input/output tensor info */
    //AL_U8 input_num ;
    tensor_param_t* input_tensor ;
    // AL_U8 output_num ;
    tensor_param_t* output_tensor ;

    /* for quant */
    AL_U16 scale_io ;
    AL_U8 rshift_io ;
    AL_S16 zp_i ;
    AL_S16 zp_o ;

} npu_interp_param_t ;


/* following struct(s) is(are) for deconv node */

typedef struct {
    AL_U8 input_num ;
    tensor_param_t** input_tensors ;
    tensor_param_t* output_tensor ;

    /* bias data. if it's null, there is no bias tensor. */
    AL_S32* bias_data ; // fc needed fixed

    conv_param_t param;
    AL_U8 conv_type ;
    AL_U8 mode_load_data ;

    /* fixed-point parameters */
    int fx_param_count ; // at least 1 should be present
    AL_U16* fx_scales ;
    AL_U8* fx_rshifts ;
    AL_S32* fx_multi ;
    AL_S32* fx_qshifts ;
    AL_S32 input_zero_point ;
    AL_S32 weight_zero_point ;
    AL_S32 output_zero_point ;

    /* for hardware */
    AL_S32 ifm_dims[4] ; //n,c,h,w
    AL_S32 ofm_dims[4] ; //n,c,h,w

    AL_S32 activation;
    AL_S32 activation_min;
    AL_S32 activation_max;

    /* for pooling node */
    AL_U8 has_pooling ;

} npu_dwconv_param_t ;


typedef struct {

    // AL_U8 input_num;
    tensor_param_t* input_tensor ;
    tensor_param_t* output_tensor ;
    AL_S32 dim_size;
    // AL_S32* re_shape;

    // /* for quant */
    // AL_U16* scale_io ;
    // AL_U8* rshift_io ;
    // AL_S16* zp_i ;
    // AL_S16 zp_o ;

} npu_reshape_param_t ;

typedef struct {

    tensor_param_t* input_tensor ;
    tensor_param_t* output_tensor ;

    /* for quant */
    // AL_U16* scale_io ;
    // AL_U8* rshift_io ;
    float scale_i;
    float scale_o;
    AL_S16 zp_i ;
    AL_S16 zp_o ;

} npu_sigmoid_param_t ;

typedef struct {

    /* input/output tensor info */
    // AL_U8 output_num ;
    tensor_param_t* output_tensor ;

    /* input param: */
    AL_S16 input_c ;
    AL_S16 input_h ;
    AL_S16 input_w ;
    float mean[3] ;
    float scale[3] ;

    AL_S8 focus ;
    AL_S16 letterbox_rows ;
    AL_S16 letterbox_cols ;
    
    /* for quant */
    float scale_q ;
    AL_S16 zero_point ;

    /* for yuv2rgb quant */
    AL_S8 r_mean_rshift ;
    AL_S16 r_mean_param ;
    AL_S8 g_mean_rshift ;
    AL_S16 g_mean_param ;
    AL_S8 b_mean_rshift ;
    AL_S16 b_mean_param ;
    AL_S8 r_quant_rshift ;
    AL_S16 r_quant_param ;
    AL_S8 g_quant_rshift ;
    AL_S16 g_quant_param ;
    AL_S8 b_quant_rshift ;
    AL_S16 b_quant_param ;

} npu_input_param_t ;


typedef struct {

    /* input/output tensor info */
    AL_U8 input_num ;
    tensor_param_t** input_tensors ;
    tensor_param_t* output_tensor ;

    /* yolov param */
    AL_U8 anchor_num ;
    //two dimensional vector: [input_num][anchor_num*2]
    AL_U16** anchors ;
    
    AL_U16 iou_thresh ;
    AL_U16 prob_thresh ;
    AL_U8 class_num ;
    
    // for yolov5s
    AL_U8 is_yolo_5s ;
    AL_U16 letter_box_w ; // or input_w
    AL_U16 letter_box_h ; // or input_h

    /* for quant: array length for input is input_num */
    AL_U16* scale ;
    AL_U8* rshift ;
    AL_S16* zp ;

} npu_yolov_param_t ;

// for package info for hard/soft_npu 
typedef struct {
    /* package addr */
    AL_U32 addr ;
    AL_U64 virt_addr ;

    /* package length */
    AL_U32 length ;
} pkg_info_t;


typedef struct {
    pkg_info_t pkg_info ;

    /* extra info for hard npu */
    AL_U32 mode_load_data ;
    AL_U32 ifm_base_addr ;
    AL_U32 flt_base_addr ;
    AL_U32 ofm_base_addr ;

} hard_npu_pkg_info_t;

typedef struct {
    pkg_info_t pkg_info ;

    /* extra info for soft npu */
    AL_U32 ofm_base_addr ;
    AL_U32 ifm0_base_addr ;
    AL_U32 ifm1_base_addr ;
    AL_U32 ifm2_base_addr ;
    AL_U32 ifm3_base_addr ;

    /* first reg content for soft node */
    AL_U32 first_reg ;

} soft_npu_pkg_info_t;

// yolo pkg is different from other soft npu pkg, 
    // one yolo node may contain several yolo_pkg_info
typedef struct {
    pkg_info_t pkg_info ;

    /* extra info for yolo node */
    AL_U32 ofm_base_addr ;
    AL_U32 ifm0_base_addr ;

    /* first reg content for soft node */
    AL_U32 first_reg ;

} yolo_pkg_info_t;

#ifdef __cplusplus
}
#endif

#endif
