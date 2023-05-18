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

#include "jpu_oneshot_api.h"
#include "r_SOFT_YUV2RGB_test.h"

#ifdef RUN_PLATFORM_LINUX
#include <dirent.h>
#endif

#ifdef RUN_PLATFORM_BM
#include "ff.h"
#endif

#ifdef RUN_PLATFORM_LINUX
#include "hard_npu_uio.h"
#include "soft_npu_uio.h"
#else
#include "bare_metal_io.h"
#endif

#include "npu_util.h"

// #define BUFFER_SIZE (96 * 1024)
#define BUFFER_SIZE (2048 * 1024)

#define Y_BS_ADDR_HI 0x0
#define U_BS_ADDR_HI 0x0
#define V_BS_ADDR_HI 0x0
#define R_BS_ADDR_HI 0x0
#define G_BS_ADDR_HI 0x0
#define B_BS_ADDR_HI 0x0

#define SOFT_BURST_LEN 16
#define SOFT_OUTSTANDING 16
#define MATCH_REAL_BURST 0

#define YUV_MODE_444 0
#define YUV_MODE_422 1
#define YUV_MODE_420 2

/* define YUV2RGB base addr */
#define SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR 0x200
#define SOFTNPU__SOFT_YUV2RGB__Y_BS_ADDR_LO__ADDR 0x300
#define SOFTNPU__SOFT_YUV2RGB__U_BS_ADDR_LO__ADDR 0x304
#define SOFTNPU__SOFT_YUV2RGB__V_BS_ADDR_LO__ADDR 0x308
#define SOFTNPU__SOFT_YUV2RGB__R_BS_ADDR_LO__ADDR 0x30c
#define SOFTNPU__SOFT_YUV2RGB__G_BS_ADDR_LO__ADDR 0x310
#define SOFTNPU__SOFT_YUV2RGB__B_BS_ADDR_LO__ADDR 0x314
#define SOFTNPU__SOFT_YUV2RGB__IMG_BS_ADDR_HI__ADDR 0x318
#define SOFTNPU__SOFT_YUV2RGB__Y_SIZE__ADDR 0x31c
#define SOFTNPU__SOFT_YUV2RGB__Y_TSIZE__ADDR 0x320
#define SOFTNPU__SOFT_YUV2RGB__U_SIZE__ADDR 0x324
#define SOFTNPU__SOFT_YUV2RGB__U_TSIZE__ADDR 0x328
#define SOFTNPU__SOFT_YUV2RGB__V_SIZE__ADDR 0x32c
#define SOFTNPU__SOFT_YUV2RGB__V_TSIZE__ADDR 0x330
#define SOFTNPU__SOFT_YUV2RGB__OUT_SIZE__ADDR 0x334
#define SOFTNPU__SOFT_YUV2RGB__OUT_TSIZE__ADDR 0x338
#define SOFTNPU__SOFT_YUV2RGB__CFG_DT__ADDR 0x33c
#define SOFTNPU__SOFT_YUV2RGB__R_MEAN_DT__ADDR 0x340
#define SOFTNPU__SOFT_YUV2RGB__G_MEAN_DT__ADDR 0x344
#define SOFTNPU__SOFT_YUV2RGB__B_MEAN_DT__ADDR 0x348
#define SOFTNPU__SOFT_YUV2RGB__R_QUANT_DT__ADDR 0x34c
#define SOFTNPU__SOFT_YUV2RGB__G_QUANT_DT__ADDR 0x350
#define SOFTNPU__SOFT_YUV2RGB__B_QUANT_DT__ADDR 0x354
#define SOFTNPU__SOFT_YUV2RGB__RGB_OUTPUT_ZERO_DT__ADDR 0x358

#define SOFTNPU__SOFT_SHARE__INTERRUPT_STATUS__ADDR 0xa0
#define SOFTNPU__SOFT_SHARE__CLEAR_INTERRUPT__ADDR 0xa4
#define SOFTNPU__SOFT_SHARE__ENABLE_INTERRUPT__ADDR 0xa8

#define SOFTNPU__SOFT_SHARE__CFG_INIT_ADDR__ADDR 0x0

static int img_h;
static int img_w;
static int img_c;
static int letterbox_rows;
static int letterbox_cols;
static int8_t has_focus;
static float mean[3];
static float scale[3];
static float input_scale;
static int input_zero_point;
static int data_type; // 0: uint8, 1: int8

static int8_t r_mean_rshift ;
static int16_t r_mean_param ;
static int8_t g_mean_rshift ;
static int16_t g_mean_param ;
static int8_t b_mean_rshift ;
static int16_t b_mean_param ;
static int8_t r_quant_rshift ;
static int16_t r_quant_param ;
static int8_t g_quant_rshift ;
static int16_t g_quant_param ;
static int8_t b_quant_rshift ;
static int16_t b_quant_param ;

static uint32_t input_addr;
static uint32_t input_length;
static void* cur_out_addr;

static int pic_h;
static int pic_w;

int8_t run_yuv2rgb (int h, int w, int format, uint32_t src_addr, uint32_t dst_addr) {

    r_SOFT_INIT_CFG soft_init_cfg;
    memset(&soft_init_cfg, 0x00, sizeof(r_SOFT_INIT_CFG));

    /* config initial values */
    soft_init_cfg.cfg_init_addr.val.cfg_burst_len = SOFT_BURST_LEN - MATCH_REAL_BURST;
    soft_init_cfg.cfg_init_addr.val.cfg_os_num = SOFT_OUTSTANDING;
    soft_init_cfg.cfg_init_addr.val.cfg_rdfm_sw = 1;
    soft_init_cfg.cfg_init_addr.val.cfg_work_mode = data_type;

    r_SOFT_YUV2RGB yuv2rgb_params;
    memset(&yuv2rgb_params, 0x00, sizeof(r_SOFT_YUV2RGB));

    /* config YUV2RGB data type */
    yuv2rgb_params.cfg_dt.val.YUV_mode = format;
    yuv2rgb_params.cfg_dt.val.max_req_len = soft_init_cfg.cfg_init_addr.val.cfg_burst_len - 1;
    yuv2rgb_params.cfg_dt.val.RGB_output_mode = 1; // 0: quant off  1: quant on

    /* config YUV size */
    yuv2rgb_params.Y_size.val.Y_h = h;
    yuv2rgb_params.Y_size.val.Y_w = w;
    yuv2rgb_params.Y_tsize.val.Y_tsize = h * w;

    switch (format) {
        case YUV_MODE_444:
        {
            yuv2rgb_params.U_size.val.U_h = h;
            yuv2rgb_params.U_size.val.U_w = w;
            yuv2rgb_params.U_tsize.val.U_tsize = h * w;
            yuv2rgb_params.V_size.val.V_h = h;
            yuv2rgb_params.V_size.val.V_w = w;
            yuv2rgb_params.V_tsize.val.V_tsize = h * w;
            break;
        }
        case YUV_MODE_422:
        {
            int u_h = (h % 2) ? h/2 + 1 : h/2;
            int u_w = w ;
            yuv2rgb_params.U_size.val.U_h = u_h;
            yuv2rgb_params.U_size.val.U_w = u_w;
            yuv2rgb_params.U_tsize.val.U_tsize = u_h * u_w;
            yuv2rgb_params.V_size.val.V_h = u_h;
            yuv2rgb_params.V_size.val.V_w = u_w;
            yuv2rgb_params.V_tsize.val.V_tsize = u_h * u_w;
            break;
        }
        case YUV_MODE_420:
        {
            int u_h = (h % 2) ? h/2 + 1 : h/2;
            int u_w = (w % 2) ? w/2 + 1 : w/2;
            yuv2rgb_params.U_size.val.U_h = u_h;
            yuv2rgb_params.U_size.val.U_w = u_w;
            yuv2rgb_params.U_tsize.val.U_tsize = u_h * u_w;
            yuv2rgb_params.V_size.val.V_h = u_h;
            yuv2rgb_params.V_size.val.V_w = u_w;
            yuv2rgb_params.V_tsize.val.V_tsize = u_h * u_w;
            break;
        }
        default:
        {
            printf("unsupported YUV format: %d\n", format);
            return -1;
        }
    }

    /* config YUV addr */
    yuv2rgb_params.Y_bs_addr_LO.val.Y_bs_addr_LO = src_addr;
    yuv2rgb_params.U_bs_addr_LO.val.U_bs_addr_LO = src_addr + yuv2rgb_params.Y_tsize.val.Y_tsize;
    yuv2rgb_params.V_bs_addr_LO.val.V_bs_addr_LO = yuv2rgb_params.U_bs_addr_LO.val.U_bs_addr_LO + yuv2rgb_params.U_tsize.val.U_tsize;

    /* config RGB addr */
    yuv2rgb_params.R_bs_addr_LO.val.R_bs_addr_LO = dst_addr;
    yuv2rgb_params.G_bs_addr_LO.val.G_bs_addr_LO = dst_addr + w*h;
    yuv2rgb_params.B_bs_addr_LO.val.B_bs_addr_LO = yuv2rgb_params.G_bs_addr_LO.val.G_bs_addr_LO + w*h;

    /* config img_bs_addr_HI */
    yuv2rgb_params.img_bs_addr_HI.val.Y_bs_addr_HI = Y_BS_ADDR_HI;
    yuv2rgb_params.img_bs_addr_HI.val.U_bs_addr_HI = U_BS_ADDR_HI;
    yuv2rgb_params.img_bs_addr_HI.val.V_bs_addr_HI = V_BS_ADDR_HI;
    yuv2rgb_params.img_bs_addr_HI.val.R_bs_addr_HI = R_BS_ADDR_HI;
    yuv2rgb_params.img_bs_addr_HI.val.G_bs_addr_HI = G_BS_ADDR_HI;
    yuv2rgb_params.img_bs_addr_HI.val.B_bs_addr_HI = B_BS_ADDR_HI;    

    /* config RGB out size */
    yuv2rgb_params.Out_size.val.Out_h = h;
    yuv2rgb_params.Out_size.val.Out_w = w;
    yuv2rgb_params.Out_tsize.val.Out_tsize = h*w*3;

    /* config quant param */
    yuv2rgb_params.R_mean_dt.val.R_mean_rshft = r_mean_rshift;
    yuv2rgb_params.R_mean_dt.val.R_mean_param = r_mean_param;
    yuv2rgb_params.G_mean_dt.val.G_mean_rshft = g_mean_rshift;
    yuv2rgb_params.G_mean_dt.val.G_mean_param = g_mean_param;
    yuv2rgb_params.B_mean_dt.val.B_mean_rshft = b_mean_rshift;
    yuv2rgb_params.B_mean_dt.val.B_mean_param = b_mean_param;
    yuv2rgb_params.R_quant_dt.val.R_quant_rshft = r_quant_rshift;
    yuv2rgb_params.R_quant_dt.val.R_quant_param = r_quant_param;
    yuv2rgb_params.G_quant_dt.val.G_quant_rshft = g_quant_rshift;
    yuv2rgb_params.G_quant_dt.val.G_quant_param = g_quant_param;
    yuv2rgb_params.B_quant_dt.val.B_quant_rshft = b_quant_rshift;
    yuv2rgb_params.B_quant_dt.val.B_quant_param = b_quant_param;
    yuv2rgb_params.RGB_output_zero_dt.val.RGB_output_zero_dt = input_zero_point;

    /* config HW */
    soft_npu_write(SOFTNPU__SOFT_SHARE__CFG_INIT_ADDR__ADDR, soft_init_cfg.cfg_init_addr._v_); // initialize
	soft_npu_write(SOFTNPU__SOFT_SHARE__ENABLE_INTERRUPT__ADDR, 0x00000001); // enable interrupt
	soft_npu_write(SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR, 0x00000001); // select device
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__Y_BS_ADDR_LO__ADDR, yuv2rgb_params.Y_bs_addr_LO._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__U_BS_ADDR_LO__ADDR, yuv2rgb_params.U_bs_addr_LO._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__V_BS_ADDR_LO__ADDR, yuv2rgb_params.V_bs_addr_LO._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__R_BS_ADDR_LO__ADDR, yuv2rgb_params.R_bs_addr_LO._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__G_BS_ADDR_LO__ADDR, yuv2rgb_params.G_bs_addr_LO._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__B_BS_ADDR_LO__ADDR, yuv2rgb_params.B_bs_addr_LO._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__IMG_BS_ADDR_HI__ADDR, yuv2rgb_params.img_bs_addr_HI._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__Y_SIZE__ADDR, yuv2rgb_params.Y_size._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__Y_TSIZE__ADDR, yuv2rgb_params.Y_tsize._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__U_SIZE__ADDR, yuv2rgb_params.U_size._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__U_TSIZE__ADDR, yuv2rgb_params.U_tsize._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__V_SIZE__ADDR, yuv2rgb_params.V_size._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__V_TSIZE__ADDR, yuv2rgb_params.V_tsize._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__OUT_SIZE__ADDR, yuv2rgb_params.Out_size._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__OUT_TSIZE__ADDR, yuv2rgb_params.Out_tsize._v_);
	soft_npu_write(SOFTNPU__SOFT_YUV2RGB__CFG_DT__ADDR, yuv2rgb_params.cfg_dt._v_);
    soft_npu_write(SOFTNPU__SOFT_YUV2RGB__R_MEAN_DT__ADDR, yuv2rgb_params.R_mean_dt._v_);
    soft_npu_write(SOFTNPU__SOFT_YUV2RGB__G_MEAN_DT__ADDR, yuv2rgb_params.G_mean_dt._v_);
    soft_npu_write(SOFTNPU__SOFT_YUV2RGB__B_MEAN_DT__ADDR, yuv2rgb_params.B_mean_dt._v_);
    soft_npu_write(SOFTNPU__SOFT_YUV2RGB__R_QUANT_DT__ADDR, yuv2rgb_params.R_quant_dt._v_);
    soft_npu_write(SOFTNPU__SOFT_YUV2RGB__G_QUANT_DT__ADDR, yuv2rgb_params.G_quant_dt._v_);
    soft_npu_write(SOFTNPU__SOFT_YUV2RGB__B_QUANT_DT__ADDR, yuv2rgb_params.B_quant_dt._v_);
    soft_npu_write(SOFTNPU__SOFT_YUV2RGB__RGB_OUTPUT_ZERO_DT__ADDR, yuv2rgb_params.RGB_output_zero_dt._v_);

    // printf("SOFTNPU__SOFT_SHARE__CFG_INIT_ADDR__ADDR, %x\n", soft_init_cfg.cfg_init_addr._v_);
    // printf("SOFTNPU__SOFT_SHARE__ENABLE_INTERRUPT__ADDR, %x\n", 0x00000001);
    // printf("SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR, %x\n", 0x00000001);
    // printf("SOFTNPU__SOFT_YUV2RGB__Y_BS_ADDR_LO__ADDR, %x\n", yuv2rgb_params.Y_bs_addr_LO._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__U_BS_ADDR_LO__ADDR, %x\n", yuv2rgb_params.U_bs_addr_LO._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__V_BS_ADDR_LO__ADDR, %x\n", yuv2rgb_params.V_bs_addr_LO._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__R_BS_ADDR_LO__ADDR, %x\n", yuv2rgb_params.R_bs_addr_LO._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__G_BS_ADDR_LO__ADDR, %x\n", yuv2rgb_params.G_bs_addr_LO._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__B_BS_ADDR_LO__ADDR, %x\n", yuv2rgb_params.B_bs_addr_LO._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__IMG_BS_ADDR_HI__ADDR, %x\n", yuv2rgb_params.img_bs_addr_HI._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__Y_SIZE__ADDR, %x\n", yuv2rgb_params.Y_size._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__Y_TSIZE__ADDR, %x\n", yuv2rgb_params.Y_tsize._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__U_SIZE__ADDR, %x\n", yuv2rgb_params.U_size._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__U_TSIZE__ADDR, %x\n", yuv2rgb_params.U_tsize._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__V_SIZE__ADDR, %x\n", yuv2rgb_params.V_size._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__V_TSIZE__ADDR, %x\n", yuv2rgb_params.V_tsize._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__OUT_SIZE__ADDR, %x\n", yuv2rgb_params.Out_size._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__OUT_TSIZE__ADDR, %x\n", yuv2rgb_params.Out_tsize._v_);
    // printf("SOFTNPU__SOFT_YUV2RGB__CFG_DT__ADDR, %x\n", yuv2rgb_params.cfg_dt._v_);

    /* start HW */
    soft_init_cfg.cfg_init_addr.val.cfg_lyr_strt = 1;
	soft_npu_write(SOFTNPU__SOFT_SHARE__CFG_INIT_ADDR__ADDR, soft_init_cfg.cfg_init_addr._v_);
    // printf("SOFTNPU__SOFT_SHARE__CFG_INIT_ADDR__ADDR, %x\n", soft_init_cfg.cfg_init_addr._v_);

    return 0;
}

static int8_t run_quant(int in_h, int in_w, int in_c) {

    printf("start quant_data\n");
    char* res_addr = (char*) malloc(in_h*in_w*in_c);
    char* src_addr = (char*) cur_out_addr;
    // unsigned char* src_addr = (unsigned char*) cur_out_addr;

    // size_t rgb_file_size = pic_h * pic_w * 3;
    // char name[30] = "rgb_data";

    // FATFS fatfs;
    // TCHAR  *Path = "0:/";
    // FRESULT Res = f_mount(&fatfs, Path, 0);
    // if (Res != FR_OK) {
    //     printf("f_mount failed\n");
    // }

    // FIL   fil;
    // FRESULT Res = f_open(&fil, name, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
    // if (Res) {
    // printf("open file: %s failed\n", name);
    // printf("res num: %d\n", Res) ;
    // }

    // int num_wr = 0 ;
    // f_write (&fil, src_addr, rgb_file_size, &num_wr) ;

    // if(num_wr != rgb_file_size) {
    // printf("write failed: rgb_file_size: %d\t num_wr: %d\n", rgb_file_size, num_wr) ;
    // }

    // f_close (&fil) ;

    for (int c = 0; c < in_c; c++) {
        for (int h = 0; h < in_h; h++) {
            for (int w = 0; w < in_w; w++) {
                int idx = c * in_h * in_w + h * in_w + w;
                float input_fp32 = ((float)(src_addr[idx]) - mean[c]) * scale[c];

                /* quant to int8/uint8 */
                int udata = (round)(input_fp32 / input_scale + (float)input_zero_point);
                if (data_type == 0) {
                    if (udata > 255)
                        udata = 255;
                    else if (udata < 0)
                        udata = 0;

                    res_addr[idx] = udata;
                } else if (data_type == 1) {
                    if (udata > 127)
                        udata = 127;
                    else if (udata < -128)
                        udata = -128;

                    res_addr[idx] = udata;
                } else {
                    printf("unsupported data type for input_op: %d\n", data_type) ;
                    return -1 ;
                }
            }
        }
    }
    free(src_addr);
    cur_out_addr = (void*)res_addr;

    printf("finish quant_data\n") ;

    return 0;
}

static int8_t resize_img (char* src_addr, char* dst_addr, int org_h, int org_w, int org_c, int new_h, int new_w) {
    int r, c, k;
    float w_scale = (float)(org_w) / (new_w);
    float h_scale = (float)(org_h) / (new_h);
    for(k = 0; k < org_c; ++k){
        for(r = 0; r < new_h; ++r){
            float sy = ((float)(r) + 0.5)*h_scale - 0.5;
            if (sy < 0) 
                sy = 0;
            if (sy > org_h - 1)
                sy = org_h -1;
            int iy = (int) sy;
            float dy = sy - iy;
            int iy_h = iy + 1;
            if (iy == org_h -1) 
                iy_h = iy;

            for(c = 0; c < new_w; ++c){
                float sx = ((float)(c)+0.5)*w_scale - 0.5;
                if (sx < 0)
                    sx = 0;
                if (sx > org_w - 1) 
                    sx = org_w - 1;
                int ix = (int) sx;
                float dx = sx - ix;
                int ix_h = ix + 1;
                if (ix == org_w -1) 
                    ix_h = ix;

                float val = 0;
                val = (1.0 - dx) * (1.0 - dy) * src_addr[k*org_h*org_w + iy*org_w + ix] + (1.0 - dx) * (dy) * src_addr[k*org_h*org_w + iy_h*org_w + ix] \
                            + (dx) * (1.0 - dy) * src_addr[k*org_h*org_w + iy*org_w + ix_h] + dx * dy * src_addr[k*org_h*org_w + iy_h*org_w + ix_h];

                int val_int = val;
                dst_addr[k*new_h*new_w + r*new_w + c] = val_int;
            }
        }
    }
    return 0;
}

static int8_t run_resize() {
    char* src_addr = (char*)(cur_out_addr);
    char* dst_addr = (char*) malloc(img_h*img_w*img_c);
    if (letterbox_rows == 0 && letterbox_cols == 0) {
        resize_img (src_addr,dst_addr, pic_h, pic_w, img_c, img_h, img_w);
    } else {
       // process letterbox
       img_w = letterbox_cols ;
       img_h = letterbox_rows ;
       img_c = 3 ; // hardcode here, maybe need one more parameter from user.

       /* letterbox process to support different letterbox size */
       float scale_letterbox = 0 ;
       int resize_rows = 0 ;
       int resize_cols = 0 ;
       if ((letterbox_rows * 1.0 / pic_h) < (letterbox_cols * 1.0 / pic_w)) {
           scale_letterbox = letterbox_rows * 1.0 / pic_h ;
       } else {
           scale_letterbox = letterbox_cols * 1.0 / pic_w ;
       }
       resize_cols = (int)(scale_letterbox * pic_w);
       resize_rows = (int)(scale_letterbox * pic_h);

       /* resize image */
       char* tmp_addr = (char*) malloc(resize_rows*resize_cols*img_c);
       resize_img (src_addr,tmp_addr, pic_h, pic_w, img_c, resize_rows, resize_cols);
   
       // Generate a black image for letterbox 
       int top = (img_h - resize_rows) / 2;
       int bot = (img_h - resize_rows + 1) / 2;
       int left = (img_w - resize_cols) / 2;
       int right = (img_w - resize_cols + 1) / 2;
       // Letterbox filling
       for (int c = 0; c < img_c; c++) {
           for (int h = 0; h < img_h; h++) {
               for (int w = 0; w < img_w; w++) {
                   if (!(w < left || h < top || w > (left + resize_cols) || h > (top + resize_rows))) {
                       dst_addr[c*img_h*img_w + h*img_w + w] = tmp_addr[c*resize_rows*resize_cols + (h-top)*resize_cols + (w-left)] ;
                   } else {
                       dst_addr[c*img_h*img_w + h*img_w + w] = 0;
                   }
               }
           }
       }
       free(tmp_addr);
    }

    free(src_addr);
    cur_out_addr = (void*)dst_addr;
    return 0;
}

static int8_t run_focus() {
    char* src_addr = (char*)(cur_out_addr);
    char* dst_addr = (char*) malloc(img_h*img_w*img_c);
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
                        dst_addr[out_index] = src_addr[in_index];
                    }
                }
            }
        }
    }
    free(src_addr);
    cur_out_addr = (void*)dst_addr;
    return 0;
}

int8_t rgb_postprocess () {
    printf("finish yuv2rgb\n");
    // run_quant(pic_h, pic_w, 3);
    // printf("start resize\n");
    // run_resize();
    // printf("finish resize\n");

    // run_quant(img_h, img_w, img_c);

    if (1 == has_focus) {
        printf("start focus\n");
        run_focus();
        printf("finish focus\n");
    }
    // memcpy((void*)input_addr, cur_out_addr, input_length);
    // free(cur_out_addr);
    
    return 0;
}

int8_t process_input_image_test (char* input_fn, void* addr, uint32_t length,  npu_input_param_t* input_param) {

    if (NULL == input_fn) {
        printf("input_fn is NULL\n");
        return -1;
    }

    /* set global variables */
    img_h = input_param->input_h;
    img_w = input_param->input_w;
    img_c = input_param->input_c;
    letterbox_rows = input_param->letterbox_rows;
    letterbox_cols = input_param->letterbox_cols;
    has_focus = input_param->focus;
    mean[0] = input_param->mean[0];
    mean[1] = input_param->mean[1];
    mean[2] = input_param->mean[2];
    scale[0] = input_param->scale[0];
    scale[1] = input_param->scale[1];
    scale[2] = input_param->scale[2];
    input_scale = input_param->scale_q;
    input_zero_point = input_param->zero_point;

    r_mean_rshift = input_param->r_mean_rshift ;
    r_mean_param = input_param->r_mean_param ;
    g_mean_rshift = input_param->g_mean_rshift ;
    g_mean_param = input_param->g_mean_param ;
    b_mean_rshift = input_param->b_mean_rshift ;
    b_mean_param = input_param->b_mean_param ;
    r_quant_rshift = input_param->r_quant_rshift ;
    r_quant_param = input_param->r_quant_param ;
    g_quant_rshift = input_param->g_quant_rshift ;
    g_quant_param = input_param->g_quant_param ;
    b_quant_rshift = input_param->b_quant_rshift ;
    b_quant_param = input_param->b_quant_param ;


    if (NPU_DATA_UINT8 == input_param->output_tensor->data_type) {
        data_type = 0;
    } else if (NPU_DATA_INT8 == input_param->output_tensor->data_type) {
        data_type = 1;
    } else {
        data_type = -1;
        printf("unsupported NPU data type: %d\n", input_param->output_tensor->data_type);
        return -1;
    }

    input_addr = (uint32_t)(addr);
    input_length = length;

#ifdef HW_PREPROCESS

    DecConfigParam decConfig;
    EncConfigParam encConfig;
    picInfo decPicInfo;
    uint32_t rsize = 0;
    BOOL succ = FALSE;

    memset(&decConfig, 0x00, sizeof(DecConfigParam));
    decConfig.subsample = FORMAT_420;
    decConfig.bsSize = 1024 * 160;
    decConfig.StreamEndian = JDI_LITTLE_ENDIAN;
    decConfig.FrameEndian  = JDI_LITTLE_ENDIAN;
    decConfig.cbcrInterleave = CBCR_SEPARATED;
    decConfig.packedFormat   = PACKED_FORMAT_NONE;
    decConfig.roiEnable = FALSE;
    decConfig.rotation  = 0;
    decConfig.mirror    = MIRDIR_NONE;
    decConfig.iHorScaleMode = 0;
    decConfig.iVerScaleMode = 0;

#ifdef RUN_PLATFORM_LINUX 
    // load input image to ddr, where HW can visit
#else

    FILINFO fno ;
    FIL fd ;
    int ret = f_open(&fd, input_fn, FA_READ) ;
    if (ret) {
        printf("Fail to open file %s, err no: %d\n", input_fn, ret) ;
        return NULL ;
    }
    f_stat(input_fn, &fno) ;
    int file_len = fno.fsize ;
    printf("start read input pic\n") ;

    void* image_start_addr = malloc(file_len) ;
    UINT br = 0 ;
    ret = f_read(&fd, image_start_addr, file_len, &br) ;
    if (ret) {
        printf("Fail to read file: %s, err no: %d\n", input_fn, ret) ;
        return NULL ;
    }
    if (file_len != br) {
        printf("Fail to read expected length from file %s, expected length: %d, actual length: %d\n", input_fn, file_len, br) ;
        return NULL ;
    }

    ret = f_close(&fd) ;
    if (ret) {
        printf("Fail to close file %s, err no: %d\n", input_fn, ret) ;
        return 0 ;
    }
#endif

    // void* buf0 = malloc(BUFFER_SIZE);
    uint32_t buf0 = 0;

    unsigned long cnt_val0 = get_sys_counter_val();
    printf("cnt_val0: %ld\n", cnt_val0);
    succ = JPU_decode_oneShot(&decConfig, image_start_addr, file_len, &buf0, &decPicInfo);
    unsigned long cnt_val1 = get_sys_counter_val();
    printf("cnt_val1: %ld\n", cnt_val1);

    unsigned long cnt_val2 = cnt_val1 - cnt_val0;
    printf("JPU used cycles: %ld\n", cnt_val2);

    if (succ) {

        free(image_start_addr);

        int format = 0;
        /* get format info */
        switch (decPicInfo.subsample) {
            case FORMAT_444:
            {
                format = 0;
                break;
            }
            case FORMAT_422:
            {
                format = 1;
                break;
            }
            case FORMAT_420:
            {
                format = 2;
                break;
            }
            default:
            {
                printf("unsupported yuv format: %d\n", decPicInfo.subsample);
                return -1;
            } 
        }

        
        pic_h = decPicInfo.picHeight;
        pic_w = decPicInfo.picWidth;
        // void* rgb_out_addr = malloc(decPicInfo.picHeight * decPicInfo.picWidth * 3);
        // cur_out_addr = rgb_out_addr;
        // cur_out_addr = buf0;

        /* save yuv_data */
        // size_t yuv_file_size = pic_h * pic_w + pic_h/2 * pic_w/2 + pic_h/2 * pic_w/2;
        // char name[30] = "yuv_data";

        // FATFS fatfs;
        // TCHAR  *Path = "0:/";
        // FRESULT Res = f_mount(&fatfs, Path, 0);
        // if (Res != FR_OK) {
        //     printf("f_mount failed\n");
        // }

        // FIL   fil;
        // FRESULT Res = f_open(&fil, name, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
        // if (Res) {
        // printf("open file: %s failed\n", name);
        // printf("res num: %d\n", Res) ;
        // }

        // int num_wr = 0 ;
        // f_write (&fil, buf0, yuv_file_size, &num_wr) ;

        // if(num_wr != yuv_file_size) {
        // printf("write failed: yuv_file_size: %d\t num_wr: %d\n", yuv_file_size, num_wr) ;
        // }

        // f_close (&fil) ;

        // run_yuv2rgb(decPicInfo.picHeight, decPicInfo.picWidth, format, (uint32_t)buf0, (uint32_t)(rgb_out_addr));
        run_yuv2rgb(decPicInfo.picHeight, decPicInfo.picWidth, format, (uint32_t)buf0, (uint32_t)(input_addr));
        printf("finish config yuv2rgb\n");

        return 0;
    }

    printf("JPU_decode_oneShot failed\n");
    return -1 ;

#else // HW_PREPROCESS not defined
    /* test pic size */
    pic_h = 576;
    pic_w = 768;
    int data_size = pic_h*pic_w*3;

    void* src_addr = malloc(data_size);

#ifdef RUN_PLATFORM_LINUX 
    // load input image to ddr, where HW can visit
#else

    char data_fn[20] = "rgb_data";
    FILINFO fno ;
    FIL fd ;
    int ret = f_open(&fd, data_fn, FA_READ) ;
    if (ret) {
        printf("Fail to open file %s, err no: %d\n", data_fn, ret) ;
        return NULL ;
    }
    f_stat(data_fn, &fno) ;
    int file_len = fno.fsize ;
    if (file_len != data_size) {
        printf("Error: file_len: %d doesn't match data size: %d\n", file_len, data_size);
        return -1;
    }

    printf("start read input data\n") ;
    UINT br = 0 ;
    ret = f_read(&fd, src_addr, file_len, &br) ;
    if (ret) {
        printf("Fail to read file: %s, err no: %d\n", data_fn, ret) ;
        return NULL ;
    }
    if (file_len != br) {
        printf("Fail to read expected length from file %s, expected length: %d, actual length: %d\n", data_fn, file_len, br) ;
        return NULL ;
    }

    ret = f_close(&fd) ;
    if (ret) {
        printf("Fail to close file %s, err no: %d\n", data_fn, ret) ;
        return 0 ;
    }
#endif

    cur_out_addr = src_addr; 
    int8_t res = rgb_postprocess ();
    if(res) 
        return -1;
    return 0;

#endif

}

static void stbi_write_func_sd(void *context, void *data, int size) {

#ifdef RUN_PLATFORM_BM

    FIL* fp = (FIL*)(context) ;
    int num_wr = 0 ;
    FRESULT res = f_write(fp, data, size, &num_wr) ;
    if (size != num_wr) {
        printf("Write failed in stbi_write_func_sd, size: %d num_wr: %d\n", size, num_wr) ;
    }

#endif

    return ;
}

int8_t draw_yolo_res_test (char* input_fn, void* addr, uint32_t length,  npu_input_param_t* input_param) {

    /* gen yolo result */
    // now we only support yolo series, to support more result generation later on 
    int16_t* data = (int16_t*) (addr) ;
    int16_t yolo_len = *(data) ;
    if (yolo_len%12 != 0) {
        printf("data length modulo 12 is not zero: %d\n", yolo_len) ;
    }
    int16_t num_res = yolo_len/12 ;
    if (num_res < 0 || num_res > 340) {
        printf("ERROR: invalid yolo num_res: %d\n", num_res) ;
    }
    data += 4 ; //skip lenth 

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

    image img_input ;
    image **alphabet = NULL;
    if (NULL != input_fn) {
#ifdef RUN_PLATFORM_LINUX
        img_input = load_image_color(input_fn, 0, 0);
#else
        FILINFO fno ;
        FIL fd ;
        int ret = f_open(&fd, input_fn, FA_READ) ;
        if (ret) {
            printf("Fail to open file %s, err no: %d\n", input_fn, ret) ;
            return NULL ;
        }
        f_stat(input_fn, &fno) ;
        int file_len = fno.fsize ;

        #define IMAGE_START_ADDR 0x40000000
        char* image_start_addr = IMAGE_START_ADDR ;
        UINT br = 0 ;
        ret = f_read(&fd, (void*)image_start_addr, file_len, &br) ;
        if (ret) {
            printf("Fail to read file: %s, err no: %d\n", input_fn, ret) ;
            return NULL ;
        }
        ret = f_close(&fd) ;
        if (ret) {
            printf("Fail to close file %s, err no: %d\n", input_fn, ret) ;
            return 0 ;
        }

        img_input = load_image_from_memory(image_start_addr,file_len,0,0,3);

#endif

        alphabet = load_alphabet();

    } else {
        img_input = make_empty_image(0,0,0);
    }

    for (int16_t i = 0; i < num_res; i++) {
        // printf("\n prob: %\tlabel: %d\trect.x_l: %d\trect.y_l: %d\trect.x_h: %d\trect.y_h: %d\n",*(data),*(data+1),*(data+2),*(data+3),*(data+4),*(data+5)) ;
        float prob = (float)(*(data))/1024/16 ;
        int prob_int = prob*100 ;
        int16_t label = *(data+1) ;
        int16_t x0 = *(data+2) ;
        x0 = x0*img_w/32/1024 ;
        int16_t y0 = *(data+3) ;
        y0 = y0*img_h/32/1024 ;
        int16_t w = *(data+4) ;
        w = w*img_w/32/1024 ;
        int16_t h = *(data+5) ;
        h = h*img_h/32/1024 ;
        int16_t x_l = x0 - w/2 ;
        int16_t y_l = y0 - h/2 ;
        int16_t x_h = x0 + w/2 ;
        int16_t y_h = y0 + h/2 ;

        printf("\n prob: %d%%\tlabel: %d\trect.x_l: %d\trect.y_l: %d\trect.x_h: %d\trect.y_h: %d\n",(int)(prob*100),label,x_l,y_l,x_h,y_h) ;
        data += 6 ;

        /* put label */
        if (NULL != input_fn) {
            /* yolov3 tiny draw the result */
            int raw_h = img_input.h ;
            int raw_w = img_input.w ;

            float ratio_x = (float)raw_w / (float)img_w;
            float ratio_y = (float)raw_h / (float)img_h;

            x_l = x_l * ratio_x;
            y_l = y_l * ratio_y;
            x_h = x_h * ratio_x;
            y_h = y_h * ratio_y;

            if (x_l < 0) x_l = 0 ;
            if (x_h > (raw_w -1)) x_h = raw_w - 1 ;
            if (y_l < 0) y_l = 0 ;
            if (y_h > (raw_h -1)) y_h = raw_h - 1 ;

            draw_box(img_input, x_l, y_l, x_h, y_h, rgb[0], rgb[1], rgb[2]) ;

            char text[256];
            sprintf(text, "%s %d%%", class_names[label], (int)(prob*100));
            image img_label = get_label(alphabet, text, img_input.h * 0.03);

            int x = x_l ;
            int y = y_l ;

            draw_label(img_input, y+img_label.h, x, img_label, rgb);
        }
    }

    if (NULL != input_fn) {
        
#ifdef RUN_PLATFORM_LINUX

        save_image(img_input, out_image_file) ;

#else

        FILINFO fno ;
        FIL fd ;
        int ret = f_open(&fd, out_image_file, (FA_CREATE_ALWAYS | FA_WRITE) ) ;
        if (ret) {
            printf("fail to open file %s, err no: %d\n", out_image_file, ret) ;
            return NULL ;
        }

        unsigned char *data = calloc(img_input.w*img_input.h*img_input.c, sizeof(char));
        int i,k;
        for(k = 0; k < img_input.c; ++k){
            for(i = 0; i < img_input.w*img_input.h; ++i){
                data[i*img_input.c+k] = (unsigned char) (255*img_input.data[i + k*img_input.w*img_input.h]);
            }
        }
        stbi_write_jpg_to_func(stbi_write_func_sd, (void*)(&fd), img_input.w, img_input.h, img_input.c, data, 80);

        ret = f_close(&fd) ;
        if (ret) {
            printf("cannot close file %s\n", out_image_file) ;
            return 0 ;
        }
        
        free(data) ;

#endif

        free_image(img_input) ;

        printf("finish draw_yolo_result\n") ;
    }

    return 0 ;
}
