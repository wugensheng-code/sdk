#ifndef RGB_DESERIALIZER_H
#define RGB_DESERIALIZER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "npu_rt_graph.h"

rt_graph_t* load_rt_graph(const char* fname);

rt_graph_t* load_rt_graph_from_mem(const AL_VOID* addr);

#ifdef __cplusplus
}
#endif

#endif
