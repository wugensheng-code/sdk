#include <stdio.h>
#include "npu_init.h"
#include "npu_scheduler.h"
#include "fm_mem_alloc.h"
#include "npu_util_func.h"
#include "npu_defines.h"
#include "io.h"
// #include "image.h"

#include "npu_util_test.h"
#include "npu_util.h"

#ifdef RUN_PLATFORM_BM
#include "gic.h"
#endif

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
    int res = readl(PLS_PROT) ;
    printf("GP PLS_PROT regsiter is %x\n",res);
    writel(0x0,PLS_PROT); // enable gp port acess to pl
    res = readl(PLS_PROT) ;
    printf("GP PLS_PROT regsiter is %x\n",res);

    res = readl(SRST_CTRL2) ;
    printf("read reset regsiter is %x\n",res);
    writel(res|0x3,SRST_CTRL2); // release hp port reset
    res = readl(SRST_CTRL2) ;
    printf("read reset regsiter is %x\n",res);


    /* ---------------2. init interrupt ---------------------------------------------- */
#ifdef RUN_PLATFORM_BM
   gic_init();
   irq_enable();
#endif

    /* ---------------3. run npu runtime graph for once ------------------------------ */
    
     rt_graph_t* graph = load_network ("npu_demo/rt.bin", "npu_demo/weight.bin") ;

     npu_input_param_t* input_param = get_input_param ((void*)graph) ;

     void* addr = 0 ;
     uint32_t length = 0 ;

    // for (int i = 0; i < 1; i++) { 

        get_input_tensor_addr((void*)graph, &addr, &length) ;

        process_input_image_test("npu_demo/input_resize.jpg", addr, length, input_param) ;
        // load_input_data("input_0.bin", addr, length);

#ifdef HW_PREPROCESS
        extern volatile int8_t preprocess_finish_flag;
        while (preprocess_finish_flag == 0) {
            // printf("wait for preprocess\n");
        }
        // reset flag
        preprocess_finish_flag == 0;
#endif

        printf("run_npu_graph start");
        unsigned long cnt_val0 = get_sys_counter_val();
        if (run_npu_graph (graph)) {
            printf("run npu graph failed\n") ;
        }
        unsigned long cnt_val1 = get_sys_counter_val();
        unsigned long cnt_val2 = cnt_val1 - cnt_val0;
        printf("run_npu_graph used cycles: %ld\n", cnt_val2);

        printf("run_npu_graph finish");

        get_output_tensor_addr((void*)graph, &addr, &length) ;

        draw_yolo_res("npu_demo/input.jpg", addr, length, input_param) ;
        // draw_yolo_res(NULL, addr, length, input_param) ;

        // alloc_mem_reset_linux() ;

    // }

    /* ---------------6. successfully exit ------------------------------------------- */
    return 0 ;
}
