#ifndef NPU_INIT_H
#define NPU_INIT_H

#include "npu_rt_graph.h"

rt_graph_t* load_network (char* rt_graph_fn, char* filter_fn) ;

rt_graph_t* load_network_with_input_pic (char* rt_graph_fn, char* filter_fn, char* pic_fn) ;

rt_graph_t* load_network_from_mem () ;


#endif