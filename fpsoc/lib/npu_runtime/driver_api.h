
/* it's better for driver to be as simple and fast as possible */

#ifndef _NPU_DRIVER_API_H
#define _NPU_DRIVER_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

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

    uint8_t data_type ;
    uint8_t layout ;
    uint8_t dim_num ;
    int32_t dims[RT_MAX_SHAPE_DIM_NUM] ;

    uint32_t addr ;

    size_t size ; // the size of tensor data

} tensor_param_t ;


/* following struct(s) is(are) for npu hard node */

typedef struct {

    int32_t kernel_h ;
    int32_t kernel_w ;
    int32_t stride_h ;
    int32_t stride_w ;
    int32_t pad_h0 ;
    int32_t pad_h1 ;
    int32_t pad_w0 ;
    int32_t pad_w1 ;
    int32_t dilation_h ;
    int32_t dilation_w ;
    int32_t group ;
    int32_t input_channel ;
    int32_t output_channel ;
} conv_param_t ;

typedef struct {

    /* act type select */
    uint8_t func_sel ;

    /* in/out quant param */
    uint8_t  up_scale ;
    uint16_t scale_i ;
    uint16_t scale_o ;
    uint16_t scale_p ;
    uint16_t scale_n ;
    uint8_t rshift_i ;
    uint8_t rshift_o ;
    uint8_t rshift_p ;
    uint8_t rshift_n ;

    int32_t output_zero_point ;
} act_param_t ;

typedef struct {

    /* input/output tensor info, assigned during run time */
    uint8_t input_num ;
    tensor_param_t** input_tensors ;
    tensor_param_t* output_tensor ;

    /* bias data. if it's null, there is no bias tensor. */
    int32_t* bias_data ; // fc needed fixed

    /* conv param */
    conv_param_t param ;
    uint8_t conv_type ;
    uint8_t mode_load_data ;

    /* fixed-point parameters */
    int fx_param_count ; // at least 1 should be present
    uint16_t* fx_scales ;  
    uint8_t* fx_rshifts ;
    int32_t input_zero_point ;
    int32_t weight_zero_point ;
    int32_t output_zero_point ;
    
    /* for hardware */
    int32_t ifm_dims[4] ; //n,c,h,w
    int32_t ofm_dims[4] ; //n,c,h,w

    /* for act node */
    uint8_t has_act ;
    act_param_t* act_param ;

    /* for pooling node */
    uint8_t has_pooling ;

} npu_hard_param_t ;


/* following struct(s) is(are) for pooling node */

typedef struct {

    /* input/output tensor info */
    //uint8_t input_num ;
    tensor_param_t* input_tensor ;
    // uint8_t output_num ;
    tensor_param_t* output_tensor ;

    /* for quant */
    uint16_t scale_io ;
    uint8_t rshift_io ;
    int16_t zp_i ;
    int16_t zp_o ;

    /* pooling param */
    int32_t pool_method ; // 0:max    1:avg
    int32_t kernel_h ;
    int32_t kernel_w ;
    int32_t stride_h ;
    int32_t stride_w ;
    int32_t pad_h0 ;
    int32_t pad_h1 ;
    int32_t pad_w0 ;
    int32_t pad_w1 ;

} npu_pooling_param_t ;


/* following struct(s) is(are) for concat node */

typedef struct {

    /* input/output tensor info */
    uint8_t input_num ;
    tensor_param_t** input_tensors ;
    // uint8_t output_num ;
    tensor_param_t* output_tensor ;

    /* concat param */
    int32_t axis ;
    
    /* ifm order select */
    uint8_t ifm_order ; // 0: ifm order by increase address, 1: ifm order by decrease address

    /* for quant: array length for input is input_num */
    uint16_t* scale_i ; 
    uint8_t* rshift_i ;
    int16_t* zp_i ;
    int16_t zp_o ;

} npu_concat_param_t ;


/* following struct(s) is(are) for upsample node */

typedef struct {

    /* input/output tensor info */
    //uint8_t input_num ;
    tensor_param_t* input_tensor ;
    // uint8_t output_num ;
    tensor_param_t* output_tensor ;

    /* for quant */
    uint16_t scale_io ;
    uint8_t rshift_io ;
    int16_t zp_i ;
    int16_t zp_o ;

} npu_upsample_param_t ;


/* following struct(s) is(are) for eltwise node */

typedef struct {

    /* input/output tensor info */
    uint8_t input_num ;
    tensor_param_t** input_tensors ;
    // uint8_t output_num ;
    tensor_param_t* output_tensor ;

    /* eltwise param */
    uint8_t type ;

    /* state select */
    uint8_t state ;

    /* ifm order select */
    uint8_t ifm_order ; // 0: ifm order by increase address, 1: ifm order by decrease address

    /* for quant: array length for input is input_num */
    uint16_t* scale_io ;
    uint8_t* rshift_io ;
    int16_t* zp_i ;
    int16_t zp_o ;

} npu_eltwise_param_t ;


/* following struct(s) is(are) for prelu node */

typedef struct {

    /* input/output tensor info */
    tensor_param_t* input_tensor ;
    // uint8_t output_num ;
    tensor_param_t* output_tensor ;

    /* prelu param TODO: */
    // slope data ?

    /* for quant */
    // ?: uint16_t scale_io ;
    // ?: uint8_t rshift_io ;
    int16_t zp_i ;
    int16_t zp_o ;

} npu_prelu_param_t ;


/* following struct(s) is(are) for interp node */

typedef struct {

    /* input/output tensor info */
    //uint8_t input_num ;
    tensor_param_t* input_tensor ;
    // uint8_t output_num ;
    tensor_param_t* output_tensor ;

    /* for quant */
    uint16_t scale_io ;
    uint8_t rshift_io ;
    int16_t zp_i ;
    int16_t zp_o ;

} npu_interp_param_t ;


/* following struct(s) is(are) for deconv node */

typedef struct {
    uint8_t input_num ;
    tensor_param_t** input_tensors ;
    tensor_param_t* output_tensor ;

    /* bias data. if it's null, there is no bias tensor. */
    int32_t* bias_data ; // fc needed fixed

    conv_param_t param;
    uint8_t conv_type ;
    uint8_t mode_load_data ;

    /* fixed-point parameters */
    int fx_param_count ; // at least 1 should be present
    uint16_t* fx_scales ;
    uint8_t* fx_rshifts ;
    int32_t* fx_multi ;
    int32_t* fx_qshifts ;
    int32_t input_zero_point ;
    int32_t weight_zero_point ;
    int32_t output_zero_point ;

    /* for hardware */
    int32_t ifm_dims[4] ; //n,c,h,w
    int32_t ofm_dims[4] ; //n,c,h,w

    int32_t activation;
    int32_t activation_min;
    int32_t activation_max;

    /* for pooling node */
    uint8_t has_pooling ;

} npu_dwconv_param_t ;


typedef struct {

    // uint8_t input_num;
    tensor_param_t* input_tensor ;
    tensor_param_t* output_tensor ;
    int32_t dim_size;
    // int32_t* re_shape;

    // /* for quant */
    // uint16_t* scale_io ;
    // uint8_t* rshift_io ;
    // int16_t* zp_i ;
    // int16_t zp_o ;

} npu_reshape_param_t ;

typedef struct {

    tensor_param_t* input_tensor ;
    tensor_param_t* output_tensor ;

    /* for quant */
    // uint16_t* scale_io ;
    // uint8_t* rshift_io ;
    float scale_i;
    float scale_o;
    int16_t zp_i ;
    int16_t zp_o ;

} npu_sigmoid_param_t ;

typedef struct {

    /* input/output tensor info */
    // uint8_t output_num ;
    tensor_param_t* output_tensor ;

    /* input param: */
    int16_t input_c ;
    int16_t input_h ;
    int16_t input_w ;
    float mean[3] ;
    float scale[3] ;

    int8_t focus ;
    int16_t letterbox_rows ;
    int16_t letterbox_cols ;
    
    /* for quant */
    float scale_q ;
    int16_t zero_point ;

    /* for yuv2rgb quant */
    int8_t r_mean_rshift ;
    int16_t r_mean_param ;
    int8_t g_mean_rshift ;
    int16_t g_mean_param ;
    int8_t b_mean_rshift ;
    int16_t b_mean_param ;
    int8_t r_quant_rshift ;
    int16_t r_quant_param ;
    int8_t g_quant_rshift ;
    int16_t g_quant_param ;
    int8_t b_quant_rshift ;
    int16_t b_quant_param ;

} npu_input_param_t ;


typedef struct {

    /* input/output tensor info */
    uint8_t input_num ;
    tensor_param_t** input_tensors ;
    tensor_param_t* output_tensor ;

    /* yolov param */
    uint8_t anchor_num ;
    //two dimensional vector: [input_num][anchor_num*2]
    uint16_t** anchors ;
    
    uint16_t iou_thresh ;
    uint16_t prob_thresh ;
    uint8_t class_num ;
    
    // for yolov5s
    uint8_t is_yolo_5s ;
    uint16_t letter_box_w ; // or input_w
    uint16_t letter_box_h ; // or input_h

    /* for quant: array length for input is input_num */
    uint16_t* scale ;
    uint8_t* rshift ;
    int16_t* zp ;

} npu_yolov_param_t ;

// for package info for hard/soft_npu 
typedef struct {
    /* package addr */
    uint32_t addr ;
    uint64_t virt_addr ;

    /* package length */
    uint32_t length ;
} pkg_info_t;


typedef struct {
    pkg_info_t pkg_info ;

    /* extra info for hard npu */
    uint32_t mode_load_data ;
    uint32_t ifm_base_addr ;
    uint32_t flt_base_addr ;
    uint32_t ofm_base_addr ;

} hard_npu_pkg_info_t;

typedef struct {
    pkg_info_t pkg_info ;

    /* extra info for soft npu */
    uint32_t ofm_base_addr ;
    uint32_t ifm0_base_addr ;
    uint32_t ifm1_base_addr ;
    uint32_t ifm2_base_addr ;
    uint32_t ifm3_base_addr ;

    /* first reg content for soft node */
    uint32_t first_reg ;

} soft_npu_pkg_info_t;

// yolo pkg is different from other soft npu pkg, 
    // one yolo node may contain several yolo_pkg_info
typedef struct {
    pkg_info_t pkg_info ;

    /* extra info for yolo node */
    uint32_t ofm_base_addr ;
    uint32_t ifm0_base_addr ;

    /* first reg content for soft node */
    uint32_t first_reg ;

} yolo_pkg_info_t;

#ifdef __cplusplus
}
#endif

#endif
