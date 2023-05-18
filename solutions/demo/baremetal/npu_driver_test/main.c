/*
 * @Author: Wenhao Wang 
 * @Date: 2022-05-25 23:14:17 
 * @Last Modified by: Wenhao Wang
 * @Last Modified time: 2022-05-We 11:27:58
 */
#include "config.h"
#include "gic.h"
#include "delay.h"
#include "npu_driver.h"
#include "npu_driver_utils.h"

extern done;

void test_hard_npu_uint8_mb ()
{
    npu_hard_param_t* param = (npu_hard_param_t*)malloc(sizeof(npu_hard_param_t)) ;
    
    /* prepare config pkt */
    int32_t ifm_dims[4] = {1, 3, 110, 110} ;
    memcpy(param->ifm_dims, ifm_dims, sizeof(int32_t) * 4) ;
    int32_t ofm_dims[4] = {1, 64, 55, 55} ;
    memcpy(param->ofm_dims, ofm_dims, sizeof(int32_t) * 4) ;
    
    /* prepare bias data */
    int32_t bias_data[64] = {
	1821, -205, -8538, -8655, -25, -10623, -862, 435, -4600, 807, -3279, -3374, -413, -9182, 1320, -2421, 
	3392, -3933, -2690, -40, -1247, -2251, -9069, 2389, -14200, -1061, -85, -4410, -7785, -41, -828, 605, 
	-14191, -4830, -9713, -9169, -5034, -3066, -2080, 2235, 12546, 10560, 4339, -7608, 698, 3245, -5595, 1262, 
	-2502, 2938, 6661, -5846, -144, 1329, -179, 4890, -15, -1588, -4252, 721, -2219, -13256, 1332, 635, } ;
    param->bias_data = (int32_t*)malloc(sizeof(int32_t) * 64) ;
    memcpy(param->bias_data, bias_data, sizeof(int32_t) * 64) ;

    /* grid0 */
    al_bin_header_v* conv_bin_header = al_load_verification_bin_header(0x40000000);
    // al_verification_bin_header_display(conv_bin_header) ;
    al_prepare_hard_npu_conv_param (param, conv_bin_header, NPU_DATA_UINT8) ;
    param->input_tensors[0]->addr = 0x0 ;
    param->input_tensors[1]->addr = 0x0 ;
    param->output_tensor->addr = 0x8dcc ;

    /* run node */
    npu_hard_node (param) ;

    /* verify */
    int32_t ofm_num = param->ofm_dims[1] * param->ofm_dims[2] * param->ofm_dims[3] ;
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + 0x8dcc;
    char* ofm_sim = 0x10000000 + 0x8dcc;
    for (int i = 0 ; i < ofm_num; i++) {
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

void test_hard_npu_uint8 ()
{
    npu_hard_param_t* param = (npu_hard_param_t*)malloc(sizeof(npu_hard_param_t)) ;
    
    /* prepare config pkt */
    int32_t ifm_dims[4] = {1, 3, 416, 416} ;
    memcpy(param->ifm_dims, ifm_dims, sizeof(int32_t) * 4) ;
    int32_t ofm_dims[4] = {1, 16, 208, 208} ;
    memcpy(param->ofm_dims, ofm_dims, sizeof(int32_t) * 4) ;
    
    /* prepare bias data */
    int32_t bias_data[16] = {5372, -25877, 665, 1043, -6699, 1740, -7074, 5843, -56, 1427, 1333, 1164, 2301, -108, 2074, 2893} ;
    param->bias_data = (int32_t*)malloc(sizeof(int32_t) * 16) ;
    memcpy(param->bias_data, bias_data, sizeof(int32_t) * 16) ;

    /* grid0 */
    al_bin_header_v* conv_bin_header = al_load_verification_bin_header(0x40000000);
    // al_verification_bin_header_display(conv_bin_header) ;
    al_prepare_hard_npu_conv_param (param, conv_bin_header, NPU_DATA_UINT8) ;
    al_bin_header_v* act_bin_header = al_load_verification_bin_header(0x41000000);
    // al_verification_bin_header_display(act_bin_header) ;
    al_prepare_hard_npu_act_param (param, act_bin_header, NPU_DATA_UINT8) ;
    al_bin_header_v* pool_bin_header = al_load_verification_bin_header(0x42000000);
    // al_verification_bin_header_display(pool_bin_header) ;
    al_prepare_hard_npu_pool_param (param, pool_bin_header, NPU_DATA_UINT8) ;
    param->input_tensors[0]->addr = 0x0 ;
    param->input_tensors[1]->addr = 0x0 ;
    param->output_tensor->addr = 0x7ec00 ;
    npu_hard_node (param) ;

    /* grid1 */
    conv_bin_header = al_load_verification_bin_header(0x43000000);
    // al_verification_bin_header_display(conv_bin_header) ;
    al_prepare_hard_npu_conv_param (param, conv_bin_header, NPU_DATA_UINT8) ;
    act_bin_header = al_load_verification_bin_header(0x44000000);
    // al_verification_bin_header_display(act_bin_header) ;
    al_prepare_hard_npu_act_param (param, act_bin_header, NPU_DATA_UINT8) ;
    pool_bin_header = al_load_verification_bin_header(0x45000000);
    // al_verification_bin_header_display(pool_bin_header) ;
    al_prepare_hard_npu_pool_param (param, pool_bin_header, NPU_DATA_UINT8) ;
    param->input_tensors[0]->addr = 0xdea0 ;
    param->input_tensors[1]->addr = 0x0 ;
    param->output_tensor->addr = 0x82410 ;
    npu_hard_node (param) ;

    /* grid2 */
    conv_bin_header = al_load_verification_bin_header(0x46000000);
    // al_verification_bin_header_display(conv_bin_header) ;
    al_prepare_hard_npu_conv_param (param, conv_bin_header, NPU_DATA_UINT8) ;
    act_bin_header = al_load_verification_bin_header(0x47000000);
    // al_verification_bin_header_display(act_bin_header) ;
    al_prepare_hard_npu_act_param (param, act_bin_header, NPU_DATA_UINT8) ;
    pool_bin_header = al_load_verification_bin_header(0x48000000);
    // al_verification_bin_header_display(pool_bin_header) ;
    al_prepare_hard_npu_pool_param (param, pool_bin_header, NPU_DATA_UINT8) ;
    param->input_tensors[0]->addr = 0x1c220 ;
    param->input_tensors[1]->addr = 0x0 ;
    param->output_tensor->addr = 0x85cf0 ;

    /* run node */
    npu_hard_node (param) ;

    /* verify */
    int32_t ofm_num = param->ofm_dims[1] * param->ofm_dims[2] * param->ofm_dims[3] ;
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + 0x7ec00;
    char* ofm_sim = 0x10000000 + 0x7ec00;
    for (int i = 0 ; i < ofm_num; i++) {
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

void test_hard_npu_int8 ()
{
    npu_hard_param_t* param = (npu_hard_param_t*)malloc(sizeof(npu_hard_param_t)) ;
    
    /* prepare config pkt */
    int32_t ifm_dims[4] = {1, 3, 416, 416} ;
    memcpy(param->ifm_dims, ifm_dims, sizeof(int32_t) * 4) ;
    int32_t ofm_dims[4] = {1, 16, 208, 208} ;
    memcpy(param->ofm_dims, ofm_dims, sizeof(int32_t) * 4) ;
    
    /* prepare bias data */
    int32_t bias_data[16] = {2991, -7767, 1113, 1233, -2262, 307, -1768, 2998, -13, 554, 539, 466, 2266, -38, 783, 3572} ;
    param->bias_data = (int32_t*)malloc(sizeof(int32_t) * 16) ;
    memcpy(param->bias_data, bias_data, sizeof(int32_t) * 16) ;

    /* grid0 */
    al_bin_header_v* conv_bin_header = al_load_verification_bin_header(0x40000000);
    // al_verification_bin_header_display(conv_bin_header) ;
    al_prepare_hard_npu_conv_param (param, conv_bin_header, NPU_DATA_INT8) ;
    al_bin_header_v* act_bin_header = al_load_verification_bin_header(0x41000000);
    // al_verification_bin_header_display(act_bin_header) ;
    al_prepare_hard_npu_act_param (param, act_bin_header, NPU_DATA_INT8) ;
    al_bin_header_v* pool_bin_header = al_load_verification_bin_header(0x42000000);
    // al_verification_bin_header_display(pool_bin_header) ;
    al_prepare_hard_npu_pool_param (param, pool_bin_header, NPU_DATA_INT8) ;
    param->input_tensors[0]->addr = 0x0 ;
    param->input_tensors[1]->addr = 0x0 ;
    param->output_tensor->addr = 0x7ec00 ;
    npu_hard_node (param) ;

    /* grid1 */
    conv_bin_header = al_load_verification_bin_header(0x43000000);
    // al_verification_bin_header_display(conv_bin_header) ;
    al_prepare_hard_npu_conv_param (param, conv_bin_header, NPU_DATA_INT8) ;
    act_bin_header = al_load_verification_bin_header(0x44000000);
    // al_verification_bin_header_display(act_bin_header) ;
    al_prepare_hard_npu_act_param (param, act_bin_header, NPU_DATA_INT8) ;
    pool_bin_header = al_load_verification_bin_header(0x45000000);
    // al_verification_bin_header_display(pool_bin_header) ;
    al_prepare_hard_npu_pool_param (param, pool_bin_header, NPU_DATA_INT8) ;
    param->input_tensors[0]->addr = 0xdea0 ;
    param->input_tensors[1]->addr = 0x0 ;
    param->output_tensor->addr = 0x82410 ;
    npu_hard_node (param) ;

    /* grid2 */
    conv_bin_header = al_load_verification_bin_header(0x46000000);
    // al_verification_bin_header_display(conv_bin_header) ;
    al_prepare_hard_npu_conv_param (param, conv_bin_header, NPU_DATA_INT8) ;
    act_bin_header = al_load_verification_bin_header(0x47000000);
    // al_verification_bin_header_display(act_bin_header) ;
    al_prepare_hard_npu_act_param (param, act_bin_header, NPU_DATA_INT8) ;
    pool_bin_header = al_load_verification_bin_header(0x48000000);
    // al_verification_bin_header_display(pool_bin_header) ;
    al_prepare_hard_npu_pool_param (param, pool_bin_header, NPU_DATA_INT8) ;
    param->input_tensors[0]->addr = 0x1c220 ;
    param->input_tensors[1]->addr = 0x0 ;
    param->output_tensor->addr = 0x85cf0 ;

    /* run node */
    npu_hard_node (param) ;

    /* verify */
    int32_t ofm_num = param->ofm_dims[1] * param->ofm_dims[2] * param->ofm_dims[3] ;
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + 0x7ec00;
    char* ofm_sim = 0x10000000 + 0x7ec00;
    for (int i = 0 ; i < ofm_num; i++) {
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

void test_soft_npu_pool ()
{
    npu_pooling_param_t* param = (npu_pooling_param_t*)malloc(sizeof(npu_pooling_param_t)) ;

    /* prepare config pkt */
    al_bin_header_v* bin_header = al_load_verification_bin_header(0x40000000) ;
    // al_verification_bin_header_display(bin_header) ;
    al_prepare_soft_npu_pool_param (param, bin_header, NPU_DATA_UINT8) ;
    param->input_tensor->addr = 0x1bba00 ;
    param->output_tensor->addr = 0x1e5e00 ;

    /* run node */
    npu_pooling_node (param) ;

    /* verify */
    int32_t ofm_num = bin_header->tensor_info[bin_header->tensor_count-1].elem_num ;
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + 0x1e5e00 ;
    char* ofm_sim = 0x10000000 + 0x1e5e00 ;
    for (int i = 0 ; i < ofm_num; i++) {
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

void test_soft_npu_concat ()
{
    npu_concat_param_t* param = (npu_concat_param_t*)malloc(sizeof(npu_concat_param_t)) ;

    /* prepare config pkt */
    al_bin_header_v* bin_header = al_load_verification_bin_header(0x40000000) ;
    al_prepare_soft_npu_concat_param (param, bin_header, NPU_DATA_UINT8) ;
    param->input_tensors[0]->addr = 0x1bba00 ;
    param->input_tensors[1]->addr = 0x2746d7 ;
    param->output_tensor->addr = 0x2898d7 ;
    param->ifm_order = 1 ;

    /* run node */
    npu_concat_node (param) ;

    /* verify */
    int32_t ofm_num = bin_header->tensor_info[bin_header->tensor_count-1].elem_num ;
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + 0x2898d7 ;
    char* ofm_sim = 0x10000000 + 0x2898d7 ;
    for (int i = 0 ; i < ofm_num; i++) {
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

void test_soft_npu_upsample ()
{
    npu_upsample_param_t* param = (npu_upsample_param_t*)malloc(sizeof(npu_upsample_param_t)) ;

    /* prepare config pkt */
    al_bin_header_v* bin_header = al_load_verification_bin_header(0x40000000) ;
    al_prepare_soft_npu_upsample_param (param, bin_header, NPU_DATA_UINT8) ;
    param->input_tensor->addr = 0x26f257 ;
    param->output_tensor->addr = 0x2746d7 ;

    /* run node */
    npu_upsample_node (param) ;

    /* verify */
    int32_t ofm_num = bin_header->tensor_info[bin_header->tensor_count-1].elem_num ;
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + 0x2746d7 ;
    char* ofm_sim = 0x10000000 + 0x2746d7 ;
    for (int i = 0 ; i < ofm_num; i++) {
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

void test_soft_npu_eltwise ()
{
    npu_eltwise_param_t* param = (npu_eltwise_param_t*)malloc(sizeof(npu_eltwise_param_t)) ;

    /* prepare config pkt */
    al_bin_header_v* bin_header = al_load_verification_bin_header(0x40000000) ;
    al_prepare_soft_npu_eltwise_param (param, bin_header, NPU_DATA_UINT8) ;
    param->input_tensors[0]->addr = 0x5c6c00 ;
    param->input_tensors[1]->addr = 0x9bcc00 ;
    param->output_tensor->addr = 0xc60c00 ;
    param->ifm_order = 1 ;

    /* run node */
    npu_eltwise_node (param) ;

    /* verify */
    int32_t ofm_num = bin_header->tensor_info[bin_header->tensor_count-1].elem_num ;
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + 0xc60c00 ;
    char* ofm_sim = 0x10000000 + 0xc60c00 ;
    for (int i = 0 ; i < ofm_num; i++) {
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

void test_soft_npu_prelu_uint8 ()
{
    npu_prelu_param_t* param = (npu_prelu_param_t*)malloc(sizeof(npu_prelu_param_t)) ;
    
    /* prepare config pkt */
    al_bin_header_v* bin_header = al_load_verification_bin_header(0x40000000) ;
    al_prepare_soft_npu_prelu_param (param, bin_header, NPU_DATA_UINT8) ;
    param->input_tensor->addr = 0x8dcc ;
    param->output_tensor->addr = 0x3820c ;

    /* run node */
    npu_prelu_node (param) ;

    /* verify */
    int32_t ofm_num = bin_header->tensor_info[bin_header->tensor_count-1].elem_num ;
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + 0x3820c ;
    char* ofm_sim = 0x10000000 + 0x3820c ;
    for (int i = 0 ; i < ofm_num; i++) {
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

void test_soft_npu_prelu_int8 ()
{
    npu_prelu_param_t* param = (npu_prelu_param_t*)malloc(sizeof(npu_prelu_param_t)) ;
    
    /* prepare config pkt */
    al_bin_header_v* bin_header = al_load_verification_bin_header(0x40000000) ;
    al_prepare_soft_npu_prelu_param (param, bin_header, NPU_DATA_INT8) ;
    param->input_tensor->addr = 0x8dcc ;
    param->output_tensor->addr = 0x3820c ;

    /* run node */
    npu_prelu_node (param) ;

    /* verify */
    int32_t ofm_num = bin_header->tensor_info[bin_header->tensor_count-1].elem_num ;
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + 0x3820c ;
    char* ofm_sim = 0x10000000 + 0x3820c ;
    for (int i = 0 ; i < ofm_num; i++) {
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

void test_soft_npu_interp ()
{
    npu_interp_param_t* param = (npu_interp_param_t*)malloc(sizeof(npu_interp_param_t)) ;

    /* prepare config pkt */
    al_bin_header_v* bin_header = al_load_verification_bin_header(0x40000000) ;
    al_prepare_soft_npu_interp_param (param, bin_header, NPU_DATA_UINT8) ;
    param->input_tensor->addr = 0x18ce000 ;
    param->output_tensor->addr = 0x18e7000 ;

    /* run node */
    npu_interp_node (param) ;

    /* verify */
    int32_t ofm_num = bin_header->tensor_info[bin_header->tensor_count-1].elem_num ;
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + 0x18e7000 ;
    char* ofm_sim = 0x10000000 + 0x18e7000 ;
    for (int i = 0 ; i < ofm_num; i++) {
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

void test_soft_npu_deconv ()
{

}

void test_soft_npu_yolo ()
{
    npu_yolov_param_t* param = (npu_yolov_param_t*)malloc(sizeof(npu_yolov_param_t)) ;
    
    int yolo_num = 2 ;
    /* prepare config pkt */
    al_bin_header_v** bin_header = (al_bin_header_v**)malloc(sizeof(al_bin_header_v*)) ; 
    bin_header[0] = al_load_verification_bin_header(0x40000000) ;
    bin_header[1] = al_load_verification_bin_header(0x41000000) ;
    
    al_prepare_soft_npu_yolo_param (param, bin_header, NPU_DATA_UINT8, yolo_num) ;
    param->input_tensors[0]->addr = 0x264a00 ;
    param->input_tensors[1]->addr = 0x2f32d7 ;

    param->output_tensor = (tensor_param_t*)malloc(sizeof(tensor_param_t)) ;
    param->output_tensor->addr = 0x31d438 ;
    param->output_tensor->dim_num = 1 ;
    param->output_tensor->dims[0] = 44 ;
    
    param->scale = malloc(sizeof(uint16_t) * yolo_num) ;
    param->rshift = malloc(sizeof(uint8_t) * yolo_num) ;
    param->scale[0] = 11070 ;
    param->scale[1] = 12442 ;
    param->rshift[0] = 16 ;
    param->rshift[1] = 16 ;

    param->anchor_num = 3 ;
    param->anchors = (uint16_t**)malloc(sizeof(uint16_t*) * param->input_num) ;
    param->anchors[0] = (uint16_t*)malloc(sizeof(uint16_t) * param->anchor_num * 2) ;
    param->anchors[1] = (uint16_t*)malloc(sizeof(uint16_t) * param->anchor_num * 2) ;
    param->anchors[0][0] = 81 ;
    param->anchors[0][1] = 82 ;
    param->anchors[0][2] = 135 ;
    param->anchors[0][3] = 169 ;
    param->anchors[0][4] = 344 ;
    param->anchors[0][5] = 319 ;
    param->anchors[1][0] = 10 ;
    param->anchors[1][1] = 14 ;
    param->anchors[1][2] = 23 ;
    param->anchors[1][3] = 27 ;
    param->anchors[1][4] = 37 ;
    param->anchors[1][5] = 58 ;

    param->iou_thresh = 0.25f * (1 << 14) ;
    param->prob_thresh = 0.4f * (1 << 14) ;
    param->class_num = 80 ;

    param->is_yolo_5s = 0 ;
    param->letter_box_w = 416 ;
    param->letter_box_h = 416 ;

    npu_yolov_node(param) ;

    /* verify */
    int32_t ofm_num = param->output_tensor->dims[0] ;
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + 0x31d438;
    char* ofm_sim = 0x10000000 + 0x31d438;
    for (int i = 0 ; i < ofm_num; i++) {
        printk("wb data sim: %x, at addr %d\n", ofm_sim[i], i);
        // if (ofm_sim[i] != ofm_golden[i]) {
        //     printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
        //     fail = 1 ;
        //     break ;
        // }
    }
    // if (fail) {
    //     printk("test fail!!!\n");
    // } else {
    //     printk("test pass!!!\n");
    // }
}

#define OFM_ADDR 0x0031d433
#define OFM_NUMS 0x0000002c

void test_driver_normal()
{
    /* load driver */
    driver_entry_t* driver = al_load_driver_bin_file(0x40000000, 0x41000000);
    
    /* run node */
    write_npu_driver (driver) ;

    /* verify */
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + OFM_ADDR;
    char* ofm_sim = 0x10000000 + OFM_ADDR;
    for (int i = 0 ; i < OFM_NUMS; i++) {
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

void test_driver_yolo()
{
    /* load driver */
    driver_entry_t* driver0 = al_load_driver_bin_file(0x40000000, 0x41000000);
    driver_entry_t* driver1 = al_load_driver_bin_file(0x42000000, 0x43000000);
    
    /* run node */
    write_npu_driver (driver0) ;
    write_npu_driver (driver1) ;

    /* verify */
    int fail = 0 ;
    char* ofm_golden = 0x30000000 + OFM_ADDR;
    char* ofm_sim = 0x10000000 + OFM_ADDR;
    for (int i = 0 ; i < OFM_NUMS; i++) {
        printk("wb data sim: %x, at addr %d\n", ofm_sim[i], i);
        continue;
        if (ofm_sim[i] != ofm_golden[i]) {
            printk("wb data fail, sim: %x golden: %x, at addr %d\n", ofm_sim[i], ofm_golden[i], i);
            fail = 1 ;
            break ;
        }
    }
    if (fail) {
        printk("test fail!!!\n");
    } else {
        printk("test pass!!!\n");
    }
}

/**
 * @desc  : main function call by startup64.S
 * boot flow: setup_vector --> clear bss --> lowlevel_init --> set sp --> main
 * @param {*}
 * @return {*}
 */
int main() {
    
    /* init arm system*/
    printf("npu system test\n");

    #define TOP_NS_BASE_ADDR 0xf8800000
    #define GP0_ENABLE (TOP_NS_BASE_ADDR+0x80)
    #define CFG_CTRL  (TOP_NS_BASE_ADDR+0x188)
    #define TOP_NS__CFG_CTRL_NPU_ADDR (TOP_NS_BASE_ADDR + 0x1b8)
    #define SRST_CTRL2 (TOP_NS_BASE_ADDR + 0x01000 + 0x078)
    #define PLS_PROT   (TOP_NS_BASE_ADDR+0x080)

    // int res = npu_mem_read(SRST_CTRL2)  ;
    // printk("read reset register is %x\n", res) ;
    // npu_mem_write(SRST_CTRL2, res|0x3) ;
    // res = npu_mem_read(SRST_CTRL2) ;
    // printk("read reset register is %x\n", res) ;

    gic_init();

    hardnpu_irq_init();
    request_irq(ALSOC_HARDNPU_IRQ, hardnpu_irq_handler);
    softnpu_irq_init();
    request_irq(ALSOC_SOFTNPU_IRQ, softnpu_irq_handler);

    irq_enable();

    // test_driver_normal () ;
    test_driver_yolo () ;
    // test_hard_npu_uint8_mb();
    // test_hard_npu_uint8 () ;
    // test_hard_npu_int8 () ;
    // test_soft_npu_pool () ;
    // test_soft_npu_concat () ;
    // test_soft_npu_upsample () ;
    // test_soft_npu_eltwise () ;
    // test_soft_npu_prelu_uint8 () ;
    // test_soft_npu_prelu_int8 () ;
    // test_soft_npu_interp () ;
    // test_soft_npu_deconv () ;
    // test_soft_npu_yolo () ;
    
    printf("npu system finish\n");
    return 0;
}
