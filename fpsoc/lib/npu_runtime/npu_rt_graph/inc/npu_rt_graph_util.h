#ifndef _NPU_RT_GRAPH_UTIL_H
#define _NPU_RT_GRAPH_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

const char* get_rt_tensor_type_string (int type) ;

const char* get_rt_tensor_layout_string (int type) ;

const char* get_rt_node_type_string (int type) ;

const char* get_scheduler_node_type_string (int type) ;

// generate indent according to level
void sn_level_str (char* buf, int level, char blank) ;




#ifdef __cplusplus
}
#endif

#endif
