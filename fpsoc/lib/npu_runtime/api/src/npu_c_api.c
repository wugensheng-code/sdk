#include "npu_c_api.h"

#include "npu_defines.h"
#include "npu_rt_graph.h"
#include "npu_init.h"
#include "npu_scheduler.h"
#include "fm_mem_alloc.h"
#include "npu_util_func.h"
#include "image_process_func.h"
#include "npu_util.h"
#include "npu_driver_init.h"
#include "npu_interrupt.h"

#define LOG_TAG "NPU_C_API"
#include "elog.h"


graph_t load_model (char* rtg_path, char* wt_path) {
    return (AL_VOID*) load_network(rtg_path, wt_path);
}

input_cfg_t get_input_cfg (graph_t graph) {
    return (AL_VOID*) get_input_param(graph);
}

AL_S8 obtain_input_tensor_addr(graph_t graph, AL_VOID** addr, AL_U32* length) {
    return get_input_tensor_addr(graph, addr, length);
}

AL_VOID destroy_graph(graph_t graph) {
    destroy_rt_graph((rt_graph_t*) graph);
}

AL_S8 execute_yuv2rgb (int pic_h, int pic_w, int format, AL_U32 src_addr, AL_U32 dst_addr, input_cfg_t input_cfg) {
    return run_yuv2rgb(pic_h, pic_w, format, src_addr, dst_addr, (npu_input_param_t*) input_cfg);
}

AL_S8 run_graph(graph_t graph) {
    return run_npu_graph((rt_graph_t*) graph);
}

AL_S8 obtain_output_tensor_addr(graph_t graph, AL_VOID** addr, AL_U32* length) {
    return get_output_tensor_addr(graph, addr, length);
}

AL_S8 show_yolo_res(char* pic_path, AL_VOID* addr, AL_U32 length, input_cfg_t input_cfg) {
    return draw_yolo_res(pic_path, addr, length, (npu_input_param_t*) input_cfg);
}

AL_S8 write_yolo_res(char* res_path, AL_VOID* addr, AL_U32 length, input_cfg_t input_cfg) {
    return dump_yolo_res(res_path, addr, length, (npu_input_param_t*) input_cfg);
}

AL_S8 npu_driver_init() {

    if (hard_npu_driver_init()) {
        log_e("failed to init hard npu driver\n");
        return -1;
    }

    if (soft_npu_driver_init()) {
        log_e("failed to init soft npu driver\n");
        return -1;
    }

    return 0;
}

AL_S8 npu_driver_deinit() {

    if (hard_npu_driver_exit()) {
        log_e("failed to deinit hard npu driver\n");
        return -1;
    }

    if (soft_npu_driver_exit()) {
        log_e("failed to deinit soft npu driver\n");
        return -1;
    }

    return 0;
}

AL_S8 npu_intr_init() {
    return intr_init();
}

AL_S8 npu_intr_deinit() {
    return intr_deinit();
}