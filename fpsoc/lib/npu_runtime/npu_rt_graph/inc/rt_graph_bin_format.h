
#ifndef __RT_GRAPH_BIN_FORMAT_H__
#define __RT_GRAPH_BIN_FORMAT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "driver_api.h"

/* rgb is the shortcut of runtime graph bin */
#define RGB_NOT_SET 0x00

/* Type define */
typedef uint32_t rgb_uoffset_t ;  /* offset is 4-byte unsigned integer */
typedef uint32_t rgb_size_t ;     /* size is 4-byte unsigned integer */
typedef uint8_t rgb_bool_t ;      /* bool is 1-byte unsigned integer */
typedef int16_t rgb_index_t ;     /* index is 4-byte signed integer */
typedef uint16_t rgb_scale_t ;     /* scale is 2-byte unsigned integer */
typedef uint8_t rgb_rshift_t ;     /* rshift is 1-byte unsigned integer */
typedef int32_t rgb_multi_t ;     /* rshift is 4-byte signed integer */
typedef int32_t rgb_qshift_t ;     /* rshift is 4-byte signed integer */
typedef int16_t rgb_zp_t ;     /* zero point is 2-byte signed integer */
typedef int32_t rgb_bias_t ;     /* bias is 4-byte signed integer */
typedef int32_t rgb_anchor_t ;     /* anchor for yolo is 4-byte signed integer */

/* --------------------- -------- RGB objects -------------------------------- */

typedef struct
{
    rgb_uoffset_t offset_vo_tensors ;                /* offset of RGB_Vector_offsets <tensors> */
    rgb_uoffset_t offset_vo_nodes ;                  /* offset of RGB_Vector_offsets <nodes> */
    rgb_uoffset_t offset_vo_scheduler_nodes ;        /* offset of RGB_Vector_offsets <scheduler_nodes> */
} RGB_Header;

typedef struct
{
    uint32_t sp_c_num;
    uint32_t sp_h_num;
    rgb_uoffset_t offset_vt_sub_params ;         /* offset of RGB_Vector_tensor_param */
    rgb_uoffset_t offset_vuint32_addr_offsets ;  /* offset of RGB_Vector_uint32 */
} RGB_sp_param ;

typedef struct
{
    uint32_t tensor_id ;
    uint32_t tensor_type ;
    uint32_t allocated ;
    tensor_param_t param ;
    uint32_t p_split_num ;
    uint32_t c_split_num ;
    rgb_uoffset_t offset_s_tname;                /* offset of string <tensor name> */
    rgb_uoffset_t offset_vi_producer_nodes ;     /* offset of RGB_Vector_indices <indices of producer nodes> */
    rgb_uoffset_t offset_vi_consumer_nodes ;     /* offset of RGB_Vector_indices <indices of consumer nodes> */
    rgb_uoffset_t offset_v_p_sp_params ;         /* offset of RGB_Vector_sp_param */
    rgb_uoffset_t offset_v_c_sp_params ;         /* offset of RGB_Vector_sp_param */
} RGB_Tensor;

typedef struct
{
    uint32_t node_id ;                         /* node id */
    uint32_t node_type ;                       /* npu node type */
    uint8_t is_sub_nd ;
    uint8_t c_sp_idx ;
    uint16_t sub_nd_id ;
    rgb_uoffset_t offset_s_nname;              /* offset of string <node name> */
    rgb_uoffset_t offset_vi_input_tensors ;    /* offset of RGB_Vector_indices <indices of input tensors> */
    rgb_uoffset_t offset_vi_output_tensors ;   /* offset of RGB_Vector_indices <indices of output tensors> */
    rgb_uoffset_t offset_param ;               /* offset of node param */
} RGB_Node;

typedef struct
{
    uint32_t node_id ;                         /* node id */
    uint32_t node_type ;                       /* scheduler node type */
    rgb_uoffset_t offset_node_content ;        /* offset of node content */
} RGB_Scheduler_Node;

typedef struct
{
    rgb_size_t size;           /* string size */
    rgb_uoffset_t offset_data; /* offset of string data */
} RGB_String;
/* ------------------------ ------- Vectors --------------------------------- */

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    rgb_uoffset_t offsets[0];
} RGB_Vector_offsets; // for two dimensional vectors

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    rgb_index_t indices[0];
} RGB_Vector_indices;

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    uint32_t elems[0];
} RGB_Vector_uint32;

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    tensor_param_t elems[0];
} RGB_Vector_tensor_param;

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    RGB_sp_param elems[0];
} RGB_Vector_sp_param;

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    rgb_bias_t elems[0];
} RGB_Vector_bias;


typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    rgb_anchor_t elems[0];
} RGB_Vector_anchors;

// vectors for quant params

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    rgb_scale_t elems[0];
} RGB_Vector_scale;

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    rgb_rshift_t elems[0];
} RGB_Vector_rshift;

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    rgb_multi_t elems[0];
} RGB_Vector_multi;

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    rgb_qshift_t elems[0];
} RGB_Vector_qshift;

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    rgb_zp_t elems[0];
} RGB_Vector_zp;

typedef struct
{
    rgb_size_t v_num; /* number of vector elements */
    yolo_pkg_info_t elems[0];
} RGB_Vector_yolo_info;

/* --------------------------- node params ----------------------------- */

typedef struct
{
    /* conv param */
    conv_param_t param ;
    uint8_t conv_type ;
    uint8_t mode_load_data ;

    /* fixed-point parameters */
    int32_t input_zero_point ;
    int32_t weight_zero_point ;
    int32_t output_zero_point ;
    
    /* for hardware */
    int32_t ifm_dims[4] ; //n,c,h,w
    int32_t ofm_dims[4] ; //n,c,h,w

    /* for act node */
    uint8_t has_act ;

    /* for pooling node */
    uint8_t has_pooling ;

    /* pointers */
    rgb_uoffset_t offset_vbias ; /* offset of RGB_Vector_bias*/
    rgb_uoffset_t offset_vscale ; /* offset of RGB_Vector_scale*/
    rgb_uoffset_t offset_vrshift ; /* offset of RGB_Vector_rshift*/
    rgb_uoffset_t offset_act_param ;

} RGB_NPUHardParam ;

typedef struct
{
    /* conv param */
    conv_param_t param ;
    uint8_t conv_type ;
    uint8_t mode_load_data ;

    /* fixed-point parameters */
    int32_t input_zero_point ;
    int32_t weight_zero_point ;
    int32_t output_zero_point ;

    /* for hardware */
    int32_t ifm_dims[4] ; //n,c,h,w
    int32_t ofm_dims[4] ; //n,c,h,w

    /* pointers */
    rgb_uoffset_t offset_vbias ; /* offset of RGB_Vector_bias */
    rgb_uoffset_t offset_vscale ; /* offset of RGB_Vector_scale */
    rgb_uoffset_t offset_vrshift ; /* offset of RGB_Vector_rshift */
    rgb_uoffset_t offset_vmulti ; /* offset of RGB_Vector_multi */
    rgb_uoffset_t offset_vqshift ; /* offset of RGB_Vector_qshift */
    rgb_uoffset_t offset_act_param ;

    /* int8 params */
    int32_t* q_shift;
    int32_t* multi;
    int32_t activation_min;
    int32_t activation_max;

} RGB_NPUDWConvParam ;

typedef struct
{
    /* reshape param */
    int32_t dim_size ;
    // npu_reshape_param_t param ;

} RGB_NPUReshapeParam;

typedef struct
{
    /* for quant */
    // uint16_t scale_io ;
    // uint8_t rshift_io ;
    // npu_sigmoid_param_t param ;
    float scale_i;
    float scale_o;
    int16_t zp_i ;
    int16_t zp_o ;

} RGB_NPUSigmoidParam;

typedef struct
{
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

} RGB_NPUPoolingParam ;

typedef struct
{
    /* concat param */
    int32_t axis ;

    uint8_t ifm_order ;
    
    /* for quant: array length for input is input_num */
    int16_t zp_o ;

    /* for pointers */
    rgb_uoffset_t offset_vscale ; /* offset of RGB_Vector_scale*/
    rgb_uoffset_t offset_vrshift ; /* offset of RGB_Vector_rshift*/
    rgb_uoffset_t offset_vzp ; /* offset of RGB_Vector_zp*/

} RGB_NPUConcatParam;

typedef struct
{
    /* for quant */
    uint16_t scale_io ;
    uint8_t rshift_io ;
    int16_t zp_i ;
    int16_t zp_o ;

} RGB_NPUUpsampleParam;

typedef struct
{
   /* eltwise param */
    uint8_t type ;

    /* for quant: array length for input is input_num */
    int16_t zp_o ;

    uint8_t ifm_order ;
    uint8_t state ;

    /* for pointers */
    rgb_uoffset_t offset_vscale ; /* offset of RGB_Vector_scale*/
    rgb_uoffset_t offset_vrshift ; /* offset of RGB_Vector_rshift*/
    rgb_uoffset_t offset_vzp ; /* offset of RGB_Vector_zp*/

} RGB_NPUEltwiseParam;

typedef struct
{
    /* prelu param */
    uint16_t slope_size;

    /* for quant */
    uint8_t up_scale;
    uint16_t scale_o;
    uint8_t rshift_o;
    int16_t zp_i;
    int16_t zp_o;

    /* pointers */
    rgb_uoffset_t offset_vscale;
    rgb_uoffset_t offset_vrshift;

}RGB_NPUPReluParam;

typedef struct
{
    /* for quant */
    uint16_t scale_io ;
    uint8_t rshift_io ;
    int16_t zp_i ;
    int16_t zp_o ;

} RGB_NPUInterpParam;

typedef struct
{
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

} RGB_NPUInputParam;

typedef struct
{
    /* yolov param */
    uint8_t anchor_num ;
    
    uint16_t iou_thresh ;
    uint16_t prob_thresh ;
    uint8_t class_num ;
    
    // for yolov5s
    uint8_t is_yolo_5s ;
    uint16_t letter_box_w ; // or input_w
    uint16_t letter_box_h ; // or input_h

    /* for pointers */
    rgb_uoffset_t offset_vo_vanchors ; /* offset of RGB_Vector_offsets <RGB_Vector_anchors> */

    rgb_uoffset_t offset_vscale ; /* offset of RGB_Vector_scale*/
    rgb_uoffset_t offset_vrshift ; /* offset of RGB_Vector_rshift*/
    rgb_uoffset_t offset_vzp ; /* offset of RGB_Vector_zp*/

} RGB_NPUYoloParam;

/* ---------------------------scheduler node content ----------------------------- */

typedef struct {

    int16_t index ; // point to rt_node in rt_graph
    int16_t parent ; // idx of scheduler node

} RGB_BasicBlock ;

typedef struct {
    
    int16_t parent ;
    uint8_t is_split ;
    rgb_uoffset_t offset_vi_nodes ; // child node idxs

} RGB_ParallelGroup ;

typedef struct {
    
    int16_t parent ;
    rgb_uoffset_t offset_vi_nodes ; // child node idxs

} RGB_SequentialList ;

#ifdef __cplusplus
}
#endif

#endif
