#ifndef __NPU_RUNTIME_C_API_H__
#define __NPU_RUNTIME_C_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "al_type.h"

typedef AL_VOID* graph_t;
typedef AL_VOID* input_cfg_t;

#define YUV_444 0
#define YUV_422 1
#define YUV_420 2


graph_t load_model (char* rtg_path, char* wt_path);

input_cfg_t get_input_cfg (graph_t graph);

AL_S8 obtain_input_tensor_addr(graph_t graph, AL_VOID** addr, AL_U32* length);

AL_VOID destroy_graph(graph_t graph);

AL_S8 execute_yuv2rgb (int pic_h, int pic_w, int format, AL_U32 src_addr, AL_U32 dst_addr, input_cfg_t input_cfg);

AL_S8 run_graph(graph_t graph);

AL_S8 obtain_output_tensor_addr(graph_t graph, AL_VOID** addr, AL_U32* length);

AL_S8 show_yolo_res(char* pic_path, AL_VOID* addr, AL_U32 length, input_cfg_t input_cfg);

AL_S8 write_yolo_res(char* res_path, AL_VOID* addr, AL_U32 length, input_cfg_t input_cfg);

AL_S8 npu_driver_init();

AL_S8 npu_driver_deinit();

AL_S8 npu_intr_init();

AL_S8 npu_intr_deinit();

#ifdef __cplusplus
}
#endif

#endif
