#include <stdio.h>
#include "al_type.h"
#include "al_reg_io.h"
#include "npu_c_api.h"
#include "jpu_oneshot_api.h"

#define RUN_PLATFORM_BM

#ifdef RUN_PLATFORM_BM
#include "gic_v3.h"
#include "ff.h"
#endif

#define LOG_TAG "MAIN"
#include "elog.h"

#define TOP_NS_BASE_ADDR 0xf8800000
#define GP0_ENABLE (TOP_NS_BASE_ADDR+0x80)
#define CFG_CTRL  (TOP_NS_BASE_ADDR+0x188)
#define TOP_NS__CFG_CTRL_NPU_ADDR (TOP_NS_BASE_ADDR + 0x1b8)
#define SRST_CTRL2 (TOP_NS_BASE_ADDR + 0x01000 + 0x078)
#define PLS_PROT   (TOP_NS_BASE_ADDR+0x080)

int main () {
    printf("entering main\n") ;

    /* ------------- 1. deal with reset register ----------------------------------- */
    /* uncomment the following section when run on fpga platform*/
    int res = AL_REG32_READ(PLS_PROT) ;
    printf("GP PLS_PROT regsiter is %x\n",res);
    AL_REG32_WRITE(PLS_PROT, 0x0); // enable gp port acess to pl
    res = AL_REG32_READ(PLS_PROT) ;
    printf("GP PLS_PROT regsiter is %x\n",res);

    res = AL_REG32_READ(SRST_CTRL2) ;
    printf("read reset regsiter is %x\n",res);
    AL_REG32_WRITE(SRST_CTRL2, res|0x3); // release hp port reset
    res = AL_REG32_READ(SRST_CTRL2) ;
    printf("read reset regsiter is %x\n",res);


    /* ---------------2. init interrupt ---------------------------------------------- */
#ifdef RUN_PLATFORM_BM
   irq_enable();
#endif

    /* ---------------3. run npu runtime graph for once ------------------------------ */
    /* initialize logger */
    elog_init();

    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG);
    // elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_DEBUG, 0);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_LVL | ELOG_FMT_TAG);

    // elog_set_filter_lvl(ELOG_LVL_WARN);
    
    elog_start();
    elog_set_filter_lvl(ELOG_LVL_INFO);

    /* init npu driver */
    if (npu_driver_init()) {
        log_e("fail to init npu driver\n");
        return -1;
    }

    /* init npu interrupt */
    if (npu_intr_init()) {
        log_e("fail to init npu interrupt\n");
        return -1;
    }
    
    graph_t* graph = load_model ("npu_demo/rt.bin", "npu_demo/weight.bin") ;
    if (NULL == graph) {
        log_e("load_model failed\n");
        return -1;
    }

    input_cfg_t input_cfg = get_input_cfg ((void*)graph) ;

    void* addr = 0 ;
    uint32_t length = 0 ;

    // for (int i = 0; i < 1; i++) { 

    if (obtain_input_tensor_addr(graph, &addr, &length)) {
        printf ("fail to get the info of input tensor\n");
        destroy_graph(graph);
        return -1;
    }

    // process_input_image("npu_demo/input_resize.jpg", addr, length, input_param) ;
    // load_input_data("input_0.bin", addr, length);
    char* input_fn = "npu_demo/input_resize.jpg";

    /* --------------------------- JPU decode --------------------------------- */
    DecConfigParam decConfig;
    picInfo decPicInfo;
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

    FILINFO fno ;
    FIL fd ;
    int ret = f_open(&fd, input_fn, FA_READ) ;
    if (ret) {
        log_e("Fail to open file %s, err no: %d\n", input_fn, ret) ;
        destroy_graph(graph);
        return -1 ;
    }
    f_stat(input_fn, &fno) ;
    int file_len = fno.fsize ;
    log_i("start read input pic\n") ;

    void* image_start_addr = malloc(file_len) ;
    UINT br = 0 ;
    ret = f_read(&fd, image_start_addr, file_len, &br) ;
    if (ret) {
        log_e("Fail to read file: %s, err no: %d\n", input_fn, ret) ;
        destroy_graph(graph);
        return -1 ;
    }
    if (file_len != br) {
        log_e("Fail to read expected length from file %s, expected length: %d, actual length: %d\n", input_fn, file_len, br) ;
        destroy_graph(graph);
        return -1 ;
    }

    ret = f_close(&fd) ;
    if (ret) {
        log_e("Fail to close file %s, err no: %d\n", input_fn, ret) ;
        destroy_graph(graph);
        return -1 ;
    }

    uint32_t buf0 = 0;

    succ = JPU_decode_oneShot(&decConfig, image_start_addr, file_len, &buf0, &decPicInfo);

    free(image_start_addr);

    log_i("finish JPU decode\n");

    if (succ) {

        int format = 0;
        /* get format info */
        switch (decPicInfo.subsample) {
            case FORMAT_444:
            {
                format = YUV_444;
                break;
            }
            case FORMAT_422:
            {
                format = YUV_422;
                break;
            }
            case FORMAT_420:
            {
                format = YUV_420;
                break;
            }
            default:
            {
                log_e("unsupported yuv format: %d\n", decPicInfo.subsample);
                destroy_graph(graph);
                return -1;
            } 
        }

        log_i("start run yuv2rgb\n");
        if (execute_yuv2rgb(decPicInfo.picHeight, decPicInfo.picWidth, format, (uint32_t)buf0, (uint32_t)(addr), input_cfg)) {
            log_e("execute_yuv2rgb failed\n");
            destroy_graph(graph);
            return -1;
        } else {
            log_i("execute_yuv2rgb finish\n");
        }

    } else {
        log_e("JPU decode failed\n");
        destroy_graph(graph);
        return -1;
    }

    /* --------------------------- run npu_graph --------------------------------- */
    log_i("run_graph start\n");
    if (run_graph (graph)) {
        log_e("run npu graph failed\n") ;
        destroy_graph(graph);
        return -1;
    }

    log_i("run_graph finish\n");

    /* --------------------------- get inference result --------------------------------- */
    if (obtain_output_tensor_addr(graph, &addr, &length)) {
        printf("fail to get info of output tensor");
        destroy_graph (graph);
        return -1;
    }

    // draw_yolo_res("npu_demo/input.jpg", addr, length, input_param) ;
    if (show_yolo_res(NULL, addr, length, input_cfg)) {
        printf("fail to show yolo results\n");
        destroy_graph (graph);
        return -1;
    }

    // }

    /* deinit npu interrupt */
    if (npu_intr_deinit()) {
        log_e("fail to deinit npu interrupt\n");
        destroy_graph (graph);
        return -1;
    }

    /* deinit npu driver */
    if (npu_driver_deinit()) {
        log_e("fail to deinit npu driver\n");
        destroy_graph(graph);
        return -1;
    }

    destroy_graph(graph);

    /* ---------------6. successfully exit ------------------------------------------- */
    return 0 ;
}
