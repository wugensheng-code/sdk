
#ifndef __RT_GRAPH_BIN_FORMAT_H__
#define __RT_GRAPH_BIN_FORMAT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "driver_api.h"
#include "npu_defines.h"

/* rgb is the shortcut of runtime graph bin */
#define RGB_NOT_SET 0x00

/* Type define */
typedef AL_U32 rgb_uoffset_t ;  /* offset is 4-byte unsigned integer */
typedef AL_U32 rgb_size_t ;     /* size is 4-byte unsigned integer */
typedef AL_U8 rgb_bool_t ;      /* bool is 1-byte unsigned integer */
typedef AL_S16 rgb_index_t ;     /* index is 4-byte signed integer */
typedef AL_U16 rgb_scale_t ;     /* scale is 2-byte unsigned integer */
typedef AL_U8 rgb_rshift_t ;     /* rshift is 1-byte unsigned integer */
typedef AL_S32 rgb_multi_t ;     /* rshift is 4-byte signed integer */
typedef AL_S32 rgb_qshift_t ;     /* rshift is 4-byte signed integer */
typedef AL_S16 rgb_zp_t ;     /* zero point is 2-byte signed integer */
typedef AL_S32 rgb_bias_t ;     /* bias is 4-byte signed integer */
typedef AL_S32 rgb_anchor_t ;     /* anchor for yolo is 4-byte signed integer */

/* --------------------- -------- RGB objects -------------------------------- */

typedef struct
{
    rgb_uoffset_t offset_vo_tensors ;                /* offset of RGB_Vector_offsets <tensors> */
    rgb_uoffset_t offset_vo_nodes ;                  /* offset of RGB_Vector_offsets <nodes> */
    rgb_uoffset_t offset_vo_scheduler_nodes ;        /* offset of RGB_Vector_offsets <scheduler_nodes> */
} RGB_Header;

typedef struct
{
    AL_U32 sp_c_num;
    AL_U32 sp_h_num;
    rgb_uoffset_t offset_vt_sub_params ;         /* offset of RGB_Vector_tensor_param */
    rgb_uoffset_t offset_vuint32_addr_offsets ;  /* offset of RGB_Vector_uint32 */
} RGB_sp_param ;

typedef struct
{
    AL_U32 tensor_id ;
    AL_U32 tensor_type ;
    AL_U32 allocated ;
    tensor_param_t param ;
    AL_U32 p_split_num ;
    AL_U32 c_split_num ;
    rgb_uoffset_t offset_s_tname;                /* offset of string <tensor name> */
    rgb_uoffset_t offset_vi_producer_nodes ;     /* offset of RGB_Vector_indices <indices of producer nodes> */
    rgb_uoffset_t offset_vi_consumer_nodes ;     /* offset of RGB_Vector_indices <indices of consumer nodes> */
    rgb_uoffset_t offset_v_p_sp_params ;         /* offset of RGB_Vector_sp_param */
    rgb_uoffset_t offset_v_c_sp_params ;         /* offset of RGB_Vector_sp_param */
} RGB_Tensor;

typedef struct
{
    AL_U32 node_id ;                         /* node id */
    AL_U32 node_type ;                       /* npu node type */
    AL_U8 is_sub_nd ;
    AL_U8 c_sp_idx ;
    AL_U16 sub_nd_id ;
    rgb_uoffset_t offset_s_nname;              /* offset of string <node name> */
    rgb_uoffset_t offset_vi_input_tensors ;    /* offset of RGB_Vector_indices <indices of input tensors> */
    rgb_uoffset_t offset_vi_output_tensors ;   /* offset of RGB_Vector_indices <indices of output tensors> */
    rgb_uoffset_t offset_param ;               /* offset of node param */
} RGB_Node;

typedef struct
{
    AL_U32 node_id ;                         /* node id */
    AL_U32 node_type ;                       /* scheduler node type */
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
    AL_U32 elems[0];
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
    AL_U8 conv_type ;
    AL_U8 mode_load_data ;

    /* fixed-point parameters */
    AL_S32 input_zero_point ;
    AL_S32 weight_zero_point ;
    AL_S32 output_zero_point ;
    
    /* for hardware */
    AL_S32 ifm_dims[4] ; //n,c,h,w
    AL_S32 ofm_dims[4] ; //n,c,h,w

    /* for act node */
    AL_U8 has_act ;

    /* for pooling node */
    AL_U8 has_pooling ;

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
    AL_U8 conv_type ;
    AL_U8 mode_load_data ;

    /* fixed-point parameters */
    AL_S32 input_zero_point ;
    AL_S32 weight_zero_point ;
    AL_S32 output_zero_point ;

    /* for hardware */
    AL_S32 ifm_dims[4] ; //n,c,h,w
    AL_S32 ofm_dims[4] ; //n,c,h,w

    /* pointers */
    rgb_uoffset_t offset_vbias ; /* offset of RGB_Vector_bias */
    rgb_uoffset_t offset_vscale ; /* offset of RGB_Vector_scale */
    rgb_uoffset_t offset_vrshift ; /* offset of RGB_Vector_rshift */
    rgb_uoffset_t offset_vmulti ; /* offset of RGB_Vector_multi */
    rgb_uoffset_t offset_vqshift ; /* offset of RGB_Vector_qshift */
    rgb_uoffset_t offset_act_param ;

    /* int8 params */
    AL_S32* q_shift;
    AL_S32* multi;
    AL_S32 activation_min;
    AL_S32 activation_max;

} RGB_NPUDWConvParam ;

typedef struct
{
    /* reshape param */
    AL_S32 dim_size ;
    // npu_reshape_param_t param ;

} RGB_NPUReshapeParam;

typedef struct
{
    /* for quant */
    // AL_U16 scale_io ;
    // AL_U8 rshift_io ;
    // npu_sigmoid_param_t param ;
    float scale_i;
    float scale_o;
    AL_S16 zp_i ;
    AL_S16 zp_o ;

} RGB_NPUSigmoidParam;

typedef struct
{
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

} RGB_NPUPoolingParam ;

typedef struct
{
    /* concat param */
    AL_S32 axis ;

    AL_U8 ifm_order ;
    
    /* for quant: array length for input is input_num */
    AL_S16 zp_o ;

    /* for pointers */
    rgb_uoffset_t offset_vscale ; /* offset of RGB_Vector_scale*/
    rgb_uoffset_t offset_vrshift ; /* offset of RGB_Vector_rshift*/
    rgb_uoffset_t offset_vzp ; /* offset of RGB_Vector_zp*/

} RGB_NPUConcatParam;

typedef struct
{
    /* for quant */
    AL_U16 scale_io ;
    AL_U8 rshift_io ;
    AL_S16 zp_i ;
    AL_S16 zp_o ;

} RGB_NPUUpsampleParam;

typedef struct
{
   /* eltwise param */
    AL_U8 type ;

    /* for quant: array length for input is input_num */
    AL_S16 zp_o ;

    AL_U8 ifm_order ;
    AL_U8 state ;

    /* for pointers */
    rgb_uoffset_t offset_vscale ; /* offset of RGB_Vector_scale*/
    rgb_uoffset_t offset_vrshift ; /* offset of RGB_Vector_rshift*/
    rgb_uoffset_t offset_vzp ; /* offset of RGB_Vector_zp*/

} RGB_NPUEltwiseParam;

typedef struct
{
    /* prelu param */
    AL_U16 slope_size;

    /* for quant */
    AL_U8 up_scale;
    AL_U16 scale_o;
    AL_U8 rshift_o;
    AL_S16 zp_i;
    AL_S16 zp_o;

    /* pointers */
    rgb_uoffset_t offset_vscale;
    rgb_uoffset_t offset_vrshift;

}RGB_NPUPReluParam;

typedef struct
{
    /* for quant */
    AL_U16 scale_io ;
    AL_U8 rshift_io ;
    AL_S16 zp_i ;
    AL_S16 zp_o ;

} RGB_NPUInterpParam;

typedef struct
{
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

} RGB_NPUInputParam;

typedef struct
{
    /* yolov param */
    AL_U8 anchor_num ;
    
    AL_U16 iou_thresh ;
    AL_U16 prob_thresh ;
    AL_U8 class_num ;
    
    // for yolov5s
    AL_U8 is_yolo_5s ;
    AL_U16 letter_box_w ; // or input_w
    AL_U16 letter_box_h ; // or input_h

    /* for pointers */
    rgb_uoffset_t offset_vo_vanchors ; /* offset of RGB_Vector_offsets <RGB_Vector_anchors> */

    rgb_uoffset_t offset_vscale ; /* offset of RGB_Vector_scale*/
    rgb_uoffset_t offset_vrshift ; /* offset of RGB_Vector_rshift*/
    rgb_uoffset_t offset_vzp ; /* offset of RGB_Vector_zp*/

} RGB_NPUYoloParam;

/* ---------------------------scheduler node content ----------------------------- */

typedef struct {

    AL_S16 index ; // point to rt_node in rt_graph
    AL_S16 parent ; // idx of scheduler node

} RGB_BasicBlock ;

typedef struct {
    
    AL_S16 parent ;
    AL_U8 is_split ;
    rgb_uoffset_t offset_vi_nodes ; // child node idxs

} RGB_ParallelGroup ;

typedef struct {
    
    AL_S16 parent ;
    rgb_uoffset_t offset_vi_nodes ; // child node idxs

} RGB_SequentialList ;

#ifdef __cplusplus
}
#endif

#endif
