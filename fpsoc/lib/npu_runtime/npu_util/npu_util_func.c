#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "npu_defines.h"
#include "npu_util_func.h"
#include "npu_rt_graph.h"
#include "fm_mem_alloc.h"
#include "image.h"
#include "driver_api.h"
#include "file_ops.h"

#define LOG_TAG "NPU_UTIL_FUNC"
#include "elog.h"

npu_input_param_t* get_input_param (AL_VOID * graph) {
    rt_graph_t* rt_graph = (rt_graph_t*) graph ;

    // find input node, possible to support multiple input nodes if necessary
    rt_node_t* input_node = get_rt_graph_node (rt_graph, 0) ;

    npu_input_param_t* param = (npu_input_param_t*) input_node->param ;

    rt_tensor_t* ts = get_rt_graph_tensor (rt_graph, input_node->output_tensors[0]) ;

    param->output_tensor = ts->param ;

    return param ;
}

AL_S8 get_input_tensor_addr (AL_VOID* graph, AL_VOID** addr, AL_U32* length) {
    rt_graph_t* rt_graph = (rt_graph_t*) graph ;

    // find input tensor, possible to support multiple input tensors if necessary
    rt_tensor_t* input_tensor = get_rt_graph_tensor(rt_graph, 0) ;

    // alloc mem for input_tensor
    alloc_mem_buddy(input_tensor) ;

    extern AL_S16* ts_usage_rec;
    ts_usage_rec[input_tensor->index] = input_tensor->consumer_num ;

    extern AL_U64* ts_vaddr_rec;
    (*addr) = (AL_VOID*) ts_vaddr_rec[input_tensor->index] ;
    *length = input_tensor->param->size ;

    return 0;
}

AL_S8 get_output_tensor_addr (AL_VOID* graph, AL_VOID** addr, AL_U32* length) {
    rt_graph_t* rt_graph = (rt_graph_t*) graph ;

    // find output tensor, possible to support multiple output tensors if necessary
    rt_tensor_t* output_tensor = get_rt_graph_tensor(rt_graph, (rt_graph->tensor_num - 1)) ;

    extern AL_U64* ts_vaddr_rec;
    (*addr) = (AL_VOID*) ts_vaddr_rec[output_tensor->index] ;
    *length = output_tensor->param->size ;

    return 0;

}


AL_S8 load_input_data (char* input_fn, AL_VOID* addr, AL_U32 length) {

    if (read_file(input_fn, addr, (AL_S64) length)) {
        log_e("load input data failed\n");
        return -1;
    }

    return 0;
}

AL_S8 process_input_image (char* input_fn, AL_VOID* addr, AL_U32 length,  npu_input_param_t* input_param) {

    /* input parameters */
    int img_h = input_param->input_h;
    int img_w = input_param->input_w;
    int img_c = input_param->input_c;
    const float mean[3] = {input_param->mean[0], input_param->mean[1], input_param->mean[2]};
    const float scale[3] = {input_param->scale[0], input_param->scale[1], input_param->scale[2]};
    float input_scale = input_param->scale_q;
    int input_zero_point = input_param->zero_point;

    image img_input ;

    AL_S64 file_len = get_file_size(input_fn);
    if (-1 == file_len) {
        log_e("get file length failed\n");
        return -1;
    }

    log_i("start read input pic\n") ;

    #define IMAGE_START_ADDR 0x40000000
    char* image_start_addr = IMAGE_START_ADDR ;
    if (read_file(input_fn, (AL_VOID*)(image_start_addr), file_len)) {
        log_e("read file: %s failed\n", input_fn);
        return -1;
    }

    img_input = load_image_from_memory(image_start_addr,file_len, 0, 0, 3) ;

    if (input_param->letterbox_rows == 0 && input_param->letterbox_cols == 0) {
        image resized = resize_image(img_input, img_w, img_h) ;
        free_image(img_input) ;
        img_input = resized ;
        log_i("finish load_image_from_memory\n") ;
    } else {
       // process letterbox
       img_w = input_param->letterbox_cols ;
       img_h = input_param->letterbox_rows ;
       img_c = 3 ; // hardcode here, maybe need one more parameter from user.

       /* letterbox process to support different letterbox size */
       float scale_letterbox = 0 ;
       int resize_rows = 0 ;
       int resize_cols = 0 ;
       if ((input_param->letterbox_rows * 1.0 / img_input.h) < (input_param->letterbox_cols * 1.0 / img_input.w)) {
           scale_letterbox = input_param->letterbox_rows * 1.0 / img_input.h ;
       } else {
           scale_letterbox = input_param->letterbox_cols * 1.0 / img_input.w ;
       }
       resize_cols = (int)(scale_letterbox * img_input.w);
       resize_rows = (int)(scale_letterbox * img_input.h);

       /* resize image */
       image resized = resize_image(img_input, resize_cols, resize_rows);
       free_image(img_input);
       img_input = resized;
   
       // Generate a black image for letterbox 
       image img_new = make_image(img_w, img_h, img_c);
       int top = (img_h - resize_rows) / 2;
       int bot = (img_h - resize_rows + 1) / 2;
       int left = (img_w - resize_cols) / 2;
       int right = (img_w - resize_cols + 1) / 2;
       // Letterbox filling
       for (int c = 0; c < img_c; c++) {
           for (int h = 0; h < img_h; h++) {
               for (int w = 0; w < img_w; w++) {
                   if (!(w < left || h < top || w > (left + resize_cols) || h > (top + resize_rows))) {
                       img_new.data[c*img_h*img_w + h*img_w + w] = img_input.data[c*resize_rows*resize_cols + (h-top)*resize_cols + (w-left)] ;
                   }
               }
           }
       }
       free_image(img_input);
       img_input = img_new;
    }
   
    float* img_data = img_input.data;
    AL_U8* input_data = (AL_U8*) addr;
    AL_U32 img_size = (img_input.c * img_input.h * img_input.w) ;
    if (length != img_size) {
        log_e("the input image size is not the same as input tensor size. input image size: %d input tensor size: %u\n", img_size, length) ;
        return -1 ;
    }

    if (1 == input_param->focus) {
        // process focus
        for (int i = 0; i < 2; i++) // corresponding to rows
        {
            for (int g = 0; g < 2; g++) // corresponding to cols
            {
                for (int c = 0; c < img_c; c++)
                {
                    for (int h = 0; h < img_h/2; h++)
                    {
                        for (int w = 0; w < img_w/2; w++)
                        {
                            int in_index  = i + g * img_w + c * img_w * img_h +
                                            h * 2 * img_w + w * 2;
                            int out_index = i * 2 * img_c * (img_w/2) * (img_h/2) +
                                            g * img_c * (img_w/2) * (img_h/2) +
                                            c * (img_w/2) * (img_h/2) +
                                            h * (img_w/2) +
                                            w;

                            float input_fp32 = (img_input.data[in_index]*255 - mean[c]) * scale[c];

                            /* quant to int8/uint8 */
                            int udata = (round)(input_fp32 / input_scale + (float)input_zero_point);

                            if (input_param->output_tensor->data_type == NPU_DATA_UINT8) {
                                if (udata > 255)
                                    udata = 255;
                                else if (udata < 0)
                                    udata = 0;
                            } else if (input_param->output_tensor->data_type == NPU_DATA_INT8) {
                                if (udata > 127)
                                    udata = 127;
                                else if (udata < -128)
                                    udata = -128;
                            } else {
                                log_e("unsupported data type for input_op: %d", input_param->output_tensor->data_type) ;
                                return -1 ;
                            }

                            input_data[out_index] = udata;
                        }
                    }
                }
            }
        }
    } else {
        /* quant data */
        log_i("start quant_data\n") ;
        for (int c = 0; c < img_c; c++)
        {
            for (int h = 0; h < img_h; h++)
            {
                for (int w = 0; w < img_w; w++)
                {
                    int idx = c * img_h * img_w + h * img_w + w;
                    float input_fp32 = (img_data[idx]*255 - mean[c]) * scale[c];

                    /* quant to int8/uint8 */
                    int udata = (round)(input_fp32 / input_scale + (float)input_zero_point);
                    if (input_param->output_tensor->data_type == NPU_DATA_UINT8) {
                        if (udata > 255)
                            udata = 255;
                        else if (udata < 0)
                            udata = 0;

                        input_data[idx] = udata;
                    } else if (input_param->output_tensor->data_type == NPU_DATA_INT8) {
                        if (udata > 127)
                            udata = 127;
                        else if (udata < -128)
                            udata = -128;

                        input_data[idx] = udata;
                    } else {
                        log_e("unsupported data type for input_op: %d", input_param->output_tensor->data_type) ;
                        return -1 ;
                    }
                }
            }
        }
       log_i("finish quant_data\n") ;
    }

    log_i("free img_input") ;
    free_image(img_input) ;

    log_i("finish gen_input_data\n") ;

    return 0 ;
}

static struct predictions {
    float prob;
    int label;
    int x_l;
    int y_l;
    int x_h;
    int y_h;
};

#define char_width      8
#define char_height     16
#define label_width     (char_width)
#define label_height    (char_height)

unsigned char Alphabet[][16] =  
{
    // Calibri
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC1,0xDF,0xC7,0xD9,0xDD,0xC1,0xFF,0xFF,0xFF,0xFF},/*"a",0*/
    // {0xFF,0xFF,0xFF,0xFD,0xFD,0xFD,0xC5,0xB9,0xBD,0xBD,0xB9,0xC5,0xFF,0xFF,0xFF,0xFF},/*"b",1*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0xFD,0xFD,0xFD,0xFD,0xC3,0xFF,0xFF,0xFF,0xFF},/*"c",2*/
    // {0xFF,0xFF,0xFF,0xBF,0xBF,0xBF,0xA3,0x9D,0xBD,0xBD,0x9D,0xA3,0xFF,0xFF,0xFF,0xFF},/*"d",3*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0x9D,0x81,0xFD,0xFD,0xC3,0xFF,0xFF,0xFF,0xFF},/*"e",4*/
    // {0xFF,0xFF,0xFF,0xE3,0xFB,0xF9,0xF0,0xF9,0xF9,0xF9,0xF9,0xF9,0xFF,0xFF,0xFF,0xFF},/*"f",5*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x81,0xDD,0xDD,0xE1,0xFD,0xC1,0x9C,0xC1,0xFF,0xFF},/*"g",6*/
    // {0xFF,0xFF,0xFF,0xFD,0xFD,0xFD,0xC5,0x99,0xBD,0xBD,0xBD,0xBD,0xFF,0xFF,0xFF,0xFF},/*"h",7*/
    // {0xFF,0xFF,0xFF,0xFF,0xFD,0xFF,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFF,0xFF,0xFF,0xFF},/*"i",8*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x81,0xDD,0xDD,0xE1,0xFD,0xC1,0x9C,0xC1,0xFF,0xFF},/*"g",9*/
    // {0xFF,0xFF,0xFF,0xFD,0xFD,0xFD,0xCD,0xE5,0xF1,0xE5,0xED,0xDD,0xFF,0xFF,0xFF,0xFF},/*"k",10*/
    // {0xFF,0xFF,0xFF,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFF,0xFF,0xFF,0xFF},/*"l",11*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x45,0x99,0x9D,0x9D,0x9D,0x9D,0xFF,0xFF,0xFF,0xFF},/*"m",12*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC5,0x99,0xBD,0xBD,0xBD,0xBD,0xFF,0xFF,0xFF,0xFF},/*"n",13*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0xBD,0xBD,0xBD,0xBD,0xC3,0xFF,0xFF,0xFF,0xFF},/*"o",14*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC5,0xB9,0xBD,0xBD,0xB9,0xC5,0xFD,0xFD,0xFF,0xFF},/*"p",15*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xA3,0x9D,0xBD,0xBD,0x9D,0xA3,0xBF,0xBF,0xFF,0xFF},/*"q",16*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE5,0xF9,0xFD,0xFD,0xFD,0xFD,0xFF,0xFF,0xFF,0xFF},/*"r",17*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE1,0xFD,0xF9,0xE7,0xEF,0xE1,0xFF,0xFF,0xFF,0xFF},/*"s",18*/
    // {0xFF,0xFF,0xFF,0xFF,0xF9,0xF9,0xE0,0xF9,0xF9,0xF9,0xF9,0xE3,0xFF,0xFF,0xFF,0xFF},/*"t",19*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBD,0xBD,0xBD,0xBD,0x9D,0xA3,0xFF,0xFF,0xFF,0xFF},/*"u",20*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDC,0xDD,0xCD,0xEB,0xEB,0xF3,0xFF,0xFF,0xFF,0xFF},/*"v",21*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDC,0xCD,0xAD,0xAB,0xB3,0x73,0xFF,0xFF,0xFF,0xFF},/*"w",22*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDD,0xE9,0xF3,0xE3,0xE9,0xDD,0xFF,0xFF,0xFF,0xFF},/*"x",23*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDC,0xDD,0xC9,0xEB,0xEB,0xF7,0xF7,0xFB,0xFF,0xFF},/*"y",24*/
    // {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE1,0xE7,0xF7,0xFB,0xFD,0xE1,0xFF,0xFF,0xFF,0xFF},/*"z",25*/

    // Courier New
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0xBD,0x83,0xBD,0x9D,0x23,0xFF,0xFF,0xFF,0xFF},/*"a",0*/
    {0xFF,0xFF,0xFF,0xFF,0xFC,0xFD,0xC5,0xB9,0xBD,0xBD,0xB9,0xC4,0xFF,0xFF,0xFF,0xFF},/*"b",1*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xA3,0x9D,0xFD,0xFD,0xBD,0xC3,0xFF,0xFF,0xFF,0xFF},/*"c",2*/
    {0xFF,0xFF,0xFF,0xFF,0x9F,0xBF,0xA3,0x9D,0xBD,0xBD,0x9D,0x23,0xFF,0xFF,0xFF,0xFF},/*"d",3*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0xBD,0x81,0xFD,0xFD,0x83,0xFF,0xFF,0xFF,0xFF},/*"e",4*/
    {0xFF,0xFF,0xFF,0xFF,0x8F,0xF7,0x81,0xF7,0xF7,0xF7,0xF7,0x81,0xFF,0xFF,0xFF,0xFF},/*"f",5*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x9D,0xBD,0xBD,0x9D,0xA3,0xBF,0xC3,0xFF,0xFF},/*"g",6*/
    {0xFF,0xFF,0xFF,0xFF,0xF9,0xFB,0xCB,0xB3,0xBB,0xBB,0xBB,0x11,0xFF,0xFF,0xFF,0xFF},/*"h",7*/
    {0xFF,0xFF,0xFF,0xFF,0xEF,0xFF,0xE3,0xEF,0xEF,0xEF,0xEF,0x83,0xFF,0xFF,0xFF,0xFF},/*"i",8*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x9D,0xBD,0xBD,0x9D,0xA3,0xBF,0xC3,0xFF,0xFF},/*"g",9*/
    {0xFF,0xFF,0xFF,0xFF,0xF9,0xFB,0x0B,0xDB,0xE3,0xEB,0xDB,0x19,0xFF,0xFF,0xFF,0xFF},/*"k",10*/
    {0xFF,0xFF,0xFF,0xFF,0xE7,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0x83,0xFF,0xFF,0xFF,0xFF},/*"l",11*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xB4,0x49,0x6D,0x6D,0x6D,0x48,0xFF,0xFF,0xFF,0xFF},/*"m",12*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC9,0xB3,0xBB,0xBB,0xBB,0x11,0xFF,0xFF,0xFF,0xFF},/*"n",13*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0xBD,0xBD,0xBD,0xBD,0xC3,0xFF,0xFF,0xFF,0xFF},/*"o",14*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC9,0xB3,0xBB,0xBB,0xBB,0xC3,0xFB,0xF1,0xFF,0xFF},/*"p",15*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x9D,0xBD,0xBD,0x9D,0xA3,0xBF,0x1F,0xFF,0xFF},/*"q",16*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x89,0xF3,0xFB,0xFB,0xFB,0xC1,0xFF,0xFF,0xFF,0xFF},/*"r",17*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x83,0xBD,0xC3,0xBF,0xBD,0xC1,0xFF,0xFF,0xFF,0xFF},/*"s",18*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFB,0xC1,0xFB,0xFB,0xFB,0xBB,0xC7,0xFF,0xFF,0xFF,0xFF},/*"t",19*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x99,0xBB,0xBB,0xBB,0x9B,0x27,0xFF,0xFF,0xFF,0xFF},/*"u",20*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x18,0xBD,0xDB,0xDB,0xE7,0xE7,0xFF,0xFF,0xFF,0xFF},/*"v",21*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x11,0xBB,0xAB,0xAB,0xAB,0xD7,0xFF,0xFF,0xFF,0xFF},/*"w",22*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x99,0xDB,0xE7,0xE7,0xDB,0x99,0xFF,0xFF,0xFF,0xFF},/*"x",23*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x11,0xBB,0xBB,0xD7,0xD7,0xEF,0xEF,0xE3,0xFF,0xFF},/*"y",24*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x83,0xDB,0xEF,0xF7,0xBB,0x83,0xFF,0xFF,0xFF,0xFF},/*"z",25*/
}; 

static AL_VOID Write_Alphbet_RGB(image img, int x, int y, char alphabet) {
    int i, j;
    int delta_x, delta_y;
    char shift_data;

    // Writing from left to right ,and up to down, diff with Alphabet[][]
	for(j = y; j < (y+char_height); j++)
    {
        delta_y = y + char_height - j - 1;

        for(i = x; i < (x+char_width); i++)
        {
            delta_x = x + char_width - i - 1;

            shift_data = (Alphabet[alphabet-97][delta_y] >> (7-delta_x)) & 0x01;   // a ascii code is 97            

            if(((y+delta_y)<img.h)&&((y+delta_y)>=0)){
                if (shift_data) {  
                    img.data[0*img.h*img.w + (y+delta_y)*img.w + i] = 1.0;
                    img.data[1*img.h*img.w + (y+delta_y)*img.w + i] = 1.0;
                    img.data[2*img.h*img.w + (y+delta_y)*img.w + i] = 1.0;    
                }         					// background color
                else {
                    img.data[0*img.h*img.w + (y+delta_y)*img.w + i] = 1.0;
                    img.data[1*img.h*img.w + (y+delta_y)*img.w + i] = 0.0;
                    img.data[2*img.h*img.w + (y+delta_y)*img.w + i] = 0.0; 
                }           					// front color
            }else{
                //printf("overflow");
            }
        }
    }

    return;
}

AL_S8 dump_yolo_res (char* output_fn, AL_VOID* addr, AL_U32 length,  npu_input_param_t* input_param) {

    AL_VOID* context = get_wr_fd(output_fn);
    if (NULL == context) {
        log_e("open file: %s failed\n", output_fn);
        return -1;
    }

    /* gen yolo result */
    // now we only support yolo series, to support more result generation later on 
    AL_S16* data = (AL_S16*) (addr) ;
    AL_S16 yolo_len = *(data) ;
    if (yolo_len%12 != 0) {
        log_e("data length modulo 12 is not zero: %d\n", yolo_len) ;
        return -1;
    }
    AL_S16 num_res = yolo_len/12 ;
    if (num_res < 0 || num_res > 340) {
        log_e("ERROR: invalid yolo num_res: %d\n", num_res) ;
        return -1;
    }
    data += 4 ; //skip lenth 

    write_func(context, &num_res, sizeof(num_res));

    int img_h = input_param->input_h;
    int img_w = input_param->input_w;
    int img_c = input_param->input_c;

    if (!(input_param->letterbox_rows == 0 && input_param->letterbox_cols == 0)) {
        img_w = input_param->letterbox_cols ;
        img_h = input_param->letterbox_rows ;
        img_c = 3 ; // hardcode here, maybe need one more parameter from user.
    } ;

    for (AL_S16 i = 0; i < num_res; i++) {
        // printf("\n prob: %\tlabel: %d\trect.x_l: %d\trect.y_l: %d\trect.x_h: %d\trect.y_h: %d\n",*(data),*(data+1),*(data+2),*(data+3),*(data+4),*(data+5)) ;
        float prob = (float)(*(data))/1024/16 ;
        AL_S16 prob_int = prob*100 ;
        AL_S16 label = *(data+1) ;
        AL_S16 x0 = *(data+2) ;
        x0 = x0*img_w/32/1024 ;
        AL_S16 y0 = *(data+3) ;
        y0 = y0*img_h/32/1024 ;
        AL_S16 w = *(data+4) ;
        w = w*img_w/32/1024 ;
        AL_S16 h = *(data+5) ;
        h = h*img_h/32/1024 ;
        AL_S16 x_l = x0 - w/2 ;
        AL_S16 y_l = y0 - h/2 ;
        AL_S16 x_h = x0 + w/2 ;
        AL_S16 y_h = y0 + h/2 ;

        log_i("\n prob: %d%%\tlabel: %d\trect.x_l: %d\trect.y_l: %d\trect.x_h: %d\trect.y_h: %d\n",(int)(prob*100),label,x_l,y_l,x_h,y_h) ;

        write_func(context, &prob_int, sizeof(prob_int));
        write_func(context, &label, sizeof(label));
        write_func(context, &x_l, sizeof(x_l));
        write_func(context, &y_l, sizeof(y_l));
        write_func(context, &x_h, sizeof(x_h));
        write_func(context, &y_h, sizeof(y_h));

        data += 6 ;
    }

    if (close_fp(context)) {
        log_e("close file: %s failed\n", output_fn);
        return -1;
    }

    return 0;

}

AL_S8 draw_yolo_res (char* input_fn, AL_VOID* addr, AL_U32 length,  npu_input_param_t* input_param) {

    /* gen yolo result */
    // now we only support yolo series, to support more result generation later on 
    AL_S16* data = (AL_S16*) (addr) ;
    AL_S16 yolo_len = *(data) ;
    if (yolo_len%12 != 0) {
        log_e("data length modulo 12 is not zero: %d\n", yolo_len) ;
        return -1;
    }
    AL_S16 num_res = yolo_len/12 ;
    if (num_res < 0 || num_res > 340) {
        log_e("invalid yolo num_res: %d\n", num_res) ;
        return -1;
    }
    data += 4 ; //skip lenth 

    struct predictions* pred_res = (struct predictions*) malloc (sizeof(struct predictions) * num_res);

    const char* out_image_file = "npu_demo/yolo_res.jpg" ;

    float red = 255 ;
    float green = 0 ;
    float blue = 0 ;
    float rgb[3];
    rgb[0] = red / 255 ;
    rgb[1] = green / 255 ;
    rgb[2] = blue / 255 ;

    static const char* class_names[] = {
    "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
    "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
    "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
    "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
    "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
    "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
    "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
    "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
    "hair drier", "toothbrush"};

    int img_h = input_param->input_h;
    int img_w = input_param->input_w;
    int img_c = input_param->input_c;

    if (!(input_param->letterbox_rows == 0 && input_param->letterbox_cols == 0)) {
        img_w = input_param->letterbox_cols ;
        img_h = input_param->letterbox_rows ;
        img_c = 3 ; // hardcode here, maybe need one more parameter from user.
    } ;

    for (AL_S16 i = 0; i < num_res; i++) {
        float prob = (float)(*(data))/1024/16 ;
        int prob_int = prob*100 ;
        AL_S16 label = *(data+1) ;
        AL_S16 x0 = *(data+2) ;
        x0 = x0*img_w/32/1024 ;
        AL_S16 y0 = *(data+3) ;
        y0 = y0*img_h/32/1024 ;
        AL_S16 w = *(data+4) ;
        w = w*img_w/32/1024 ;
        AL_S16 h = *(data+5) ;
        h = h*img_h/32/1024 ;
        AL_S16 x_l = x0 - w/2 ;
        AL_S16 y_l = y0 - h/2 ;
        AL_S16 x_h = x0 + w/2 ;
        AL_S16 y_h = y0 + h/2 ;

        printf("\n prob: %d%%\tlabel: %d\trect.x_l: %d\trect.y_l: %d\trect.x_h: %d\trect.y_h: %d\n",(int)(prob*100),label,x_l,y_l,x_h,y_h) ;
        pred_res[i].prob = prob;
        pred_res[i].label = label;
        pred_res[i].x_l = x_l;
        pred_res[i].y_l = y_l;
        pred_res[i].x_h = x_h;
        pred_res[i].y_h = y_h;
        data += 6 ;
    }

    if (NULL != input_fn) {

        image img_input ;
        image **alphabet = NULL;

        AL_S64 file_len = get_file_size(input_fn);
        if (-1 == file_len) {
            log_e("fail to get file size\n");
            free(pred_res);
            return -1;
        }

        #define IMAGE_START_ADDR 0x40000000
        char* image_start_addr = IMAGE_START_ADDR ;

        if (read_file(input_fn, (AL_VOID*)image_start_addr, file_len)) {
            log_e("fail to read file: %s\n", input_fn);
            free(pred_res);
            return -1;
        }

        img_input = load_image_from_memory(image_start_addr,file_len,0,0,3);


        // alphabet = load_alphabet();

        /* put label */
        for (int i = 0; i < num_res; i++) {
            /* yolov3 tiny draw the result */
            int raw_h = img_input.h ;
            int raw_w = img_input.w ;

            float ratio_x = (float)raw_w / (float)img_w;
            float ratio_y = (float)raw_h / (float)img_h;

            float prob = pred_res[i].prob;
            int label = pred_res[i].label;
            int x_l = pred_res[i].x_l * ratio_x;
            int y_l = pred_res[i].y_l * ratio_y;
            int x_h = pred_res[i].x_h * ratio_x;
            int y_h = pred_res[i].y_h * ratio_y;

            if (x_l < 0) x_l = 0 ;
            if (x_h > (raw_w -1)) x_h = raw_w - 1 ;
            if (y_l < 0) y_l = 0 ;
            if (y_h > (raw_h -1)) y_h = raw_h - 1 ;

            draw_box(img_input, x_l, y_l, x_h, y_h, rgb[0], rgb[1], rgb[2]) ;

            char* ch = class_names[pred_res[i].label];

            for(int idx = 0; idx < strlen(ch); idx++)
            {
                char c = ch[idx];

                Write_Alphbet_RGB(img_input, x_l+idx*char_width, y_l, c);
            }

            // char text[256];
            // sprintf(text, "%s %d%%", class_names[label], (int)(prob*100));
            // image img_label = get_label(alphabet, text, img_input.h * 0.03);

            // int x = x_l ;
            // int y = y_l ;

            // draw_label(img_input, y+img_label.h, x, img_label, rgb);
        }

        AL_VOID* context = get_wr_fd(out_image_file);
        if (NULL == context) {
            log_e("get fd failed\n");
            free(pred_res);
            return -1 ;
        }

        unsigned char *data = calloc(img_input.w*img_input.h*img_input.c, sizeof(char));
        int i,k;
        for(k = 0; k < img_input.c; ++k){
            for(i = 0; i < img_input.w*img_input.h; ++i){
                data[i*img_input.c+k] = (unsigned char) (255*img_input.data[i + k*img_input.w*img_input.h]);
            }
        }
        stbi_write_jpg_to_func(write_func, context, img_input.w, img_input.h, img_input.c, data, 80);

        if (close_fp(context)) {
            log_e("cannot close file %s\n", out_image_file) ;
            free(data) ;
            free(pred_res);
            return -1 ;
        }
        
        free(data) ;


        free_image(img_input) ;

        log_i("finish draw_yolo_result\n") ;
    }

    free(pred_res);

    return 0 ;
}
