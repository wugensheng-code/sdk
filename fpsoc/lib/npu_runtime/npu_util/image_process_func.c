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

#include "image_process_func.h"

#include "npu_driver_io.h"

#include "npu_util.h"
#include "soft_global_register.h"
#include "yuv2rgb_register.h"

#define LOG_TAG "IMAGE_PROCESS_FUNC"
#include "elog.h"

AL_S8 run_yuv2rgb (int h, int w, int format, AL_U32 src_addr, AL_U32 dst_addr, npu_input_param_t* input_param) {

    int input_zero_point;
    int data_type; // 0: uint8, 1: int8

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
        log_e("unsupported NPU data type: %d\n", input_param->output_tensor->data_type);
        return -1;
    }

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
            log_e("unsupported YUV format: %d\n", format);
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

    /* start HW */
    soft_init_cfg.cfg_init_addr.val.cfg_lyr_strt = 1;
	soft_npu_write(SOFTNPU__SOFT_SHARE__CFG_INIT_ADDR__ADDR, soft_init_cfg.cfg_init_addr._v_);

    // int res = soft_npu_read(SOFTNPU__SOFT_SHARE__INTERRUPT_STATUS__ADDR) ;
    // while(res!=0x1) {
    //     res = soft_npu_read(SOFTNPU__SOFT_SHARE__INTERRUPT_STATUS__ADDR) ;
    // }
    // printf("yuv2rgb finish\n");

    if (wait_for_yuv2rgb_irq()) {
        log_e("wait for yuv2rgb irq failed\n");
        return -1;
    }

    return 0;
}