#ifndef _NPU_RT_GRAPH_H
#define _NPU_RT_GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "driver_api.h"
#include "npu_defines.h"

#define RT_GRAPH_MAX_CONSUMER_NUM   2
#define RT_GRAPH_MAX_PRODUCER_NUM   1

#define TENSOR_PARAM_INIT_ADDR 0xffffffff

// define tensor type
#define RT_TENSOR_CONST 0
#define RT_TENSOR_VAR 1

// define scheduler node type
#define RT_S_BASIC 0
#define RT_S_PARALLEL 1
#define RT_S_SEQUENTIAL 2

// define rt_node type
#define RT_NPU_HARD 0
#define RT_POOLING 1
#define RT_CONCAT 2
#define RT_UPSAMPLE 3
#define RT_ELTWISE 4
#define RT_PRELU 5
#define RT_INTERP 6
#define RT_DWCONV 7
#define RT_INPUT 8
#define RT_YOLO 9
#define RT_RESHAPE 10
#define RT_SIGMOID 11


// ################## struct definitions ######################

typedef struct sp_param {

    AL_U8 sp_c_num ;
    AL_U8 sp_h_num ;
    // use sub_nd_id in rt_node to find the right sub_param
    tensor_param_t** sub_params ;
    AL_U32* addr_offsets ;

} sp_param_t ;

typedef struct rt_tensor {
    
    AL_U16 index ;
    char* name ;
    AL_U16 producer_num ; 
    AL_S16* producer ;
    AL_U16 consumer_num ; 
    AL_S16* consumer;

    AL_U8 tensor_type ; // tensor_type: { const, var }
    AL_U8 allocated ; // memory allocated status: { un-allocated, allocated }

    tensor_param_t* param ;

    // record whether the tensor's producer is splited
    AL_U8 p_split_num ;
    sp_param_t* p_sp_params ;

    // record whether the tensor's consumer is splited
    AL_U8 c_split_num ;
    sp_param_t* c_sp_params ;

} rt_tensor_t ;

struct rt_graph ;
typedef struct rt_node {

    AL_U16 index ;
    char* name ;

    // node type
    AL_U16 node_type ; 
    AL_U8 is_sub_nd ; // for split
    AL_U8 c_sp_idx ; // idx of the c_sp_params of the i_tensor 
    AL_U16 sub_nd_id ; // for find sub_param and addr_offset
    AL_VOID* param ; // the corresponding param in driver_api.h

    AL_U8 input_num ;
    AL_U16* input_tensors ;
    AL_U8 output_num ;
    AL_U16* output_tensors ;

    struct rt_graph* graph ;

} rt_node_t ;

// it stands for npu node, including npu hard node and soft node
typedef struct basic_node {

    AL_S16 index ; // point to rt_node in rt_graph
    AL_S16 parent ; // idx of scheduler node

} basic_node_t ;

// it stands for parallel execution units
typedef struct parallel_group {

    AL_U16 node_num ;
    AL_S16* nodes ;
    AL_S16 parent ;
    AL_U8 is_split ;

} parallel_group_t ;

// it stands for sequential execution order of analyzer nodes
typedef struct sequential_list {

    AL_U16 node_num ;
    AL_S16* nodes ; // index vector of scheduler node, can be basic_node_t or pg
    AL_S16 parent ;

} sequential_list_t ;

typedef struct scheduler_node {

    AL_U8 node_type ; //node_type: { basic_node, parallel_group, sequential_list }
    AL_S16 index ;

    union {
        basic_node_t*              nd ;
        parallel_group_t*          pg ;
        sequential_list_t*         sl ;
    } ;

    struct rt_graph* graph ;

} scheduler_node_t ;

typedef struct rt_graph {

    AL_U16 tensor_num ;
    rt_tensor_t** tensor_list ;

    AL_U16 node_num ;
    rt_node_t** node_list ;

    AL_U16 scheduler_node_num ;
    scheduler_node_t** scheduler_node_list ;

    // define resources of basic nodes, index indicates basic node type
    // AL_U16* basic_node_resources ; 

} rt_graph_t ;

// ################## rt_tensor related functions ######################

rt_tensor_t* create_rt_tensor (rt_graph_t* graph) ;

AL_VOID destroy_rt_tensor (rt_graph_t* graph, rt_tensor_t* tensor) ;

int set_rt_tensor_producer (rt_tensor_t* tensor, const int index) ;

int set_rt_tensor_consumer (rt_tensor_t* tensor, const int index) ;

AL_VOID dump_rt_tensor (rt_graph_t* graph, rt_tensor_t* tensor) ;


// ################## rt_node related functions ######################

rt_node_t* create_rt_node (rt_graph_t* graph, AL_U16 node_type) ;

AL_VOID destroy_rt_node (rt_graph_t* graph, rt_node_t* node) ;

int set_rt_node_input_tensor (rt_node_t* node, int input_idx, rt_tensor_t* tensor) ;

int set_rt_node_output_tensor (rt_node_t* node, int output_idx, rt_tensor_t* tensor);

AL_VOID dump_rt_node (rt_graph_t* graph, rt_node_t* node);


// ################## scheduler_node related functions ######################

basic_node_t* create_basic_node (AL_S16 rt_node_idx, AL_S16 parent_idx) ;


parallel_group_t* create_parallel_group (AL_S16 parent_idx) ;

int add_child_pg (parallel_group_t* pg, scheduler_node_t* child_node) ;


sequential_list_t* create_sequential_list (AL_S16 parent_idx) ;

int add_child_sl (sequential_list_t* sl, scheduler_node_t* child_node) ;


scheduler_node_t* create_scheduler_node (rt_graph_t* graph, AL_U16 node_type, AL_VOID* content) ;

AL_VOID destroy_scheduler_node (rt_graph_t* graph, scheduler_node_t* node) ;

int add_child_sn (scheduler_node_t* node, scheduler_node_t* child_node) ;

int get_child_num (scheduler_node_t* node) ;

int set_parent (scheduler_node_t* pg, AL_S16 parent_idx) ;

AL_VOID dump_scheduler_node (rt_graph_t* graph, scheduler_node_t* node, int level);


// ################## rt_graph related functions ######################

rt_graph_t* create_rt_graph () ;

AL_VOID init_rt_graph (rt_graph_t* graph) ;

AL_VOID destroy_rt_graph (rt_graph_t* graph) ;

rt_tensor_t* get_rt_graph_tensor (rt_graph_t* graph, int index) ;

rt_node_t* get_rt_graph_node (rt_graph_t* graph, int index) ;

scheduler_node_t* get_rt_graph_scheduler_node (rt_graph_t* graph, int index) ;

AL_VOID dump_rt_graph (rt_graph_t* graph) ;


// ####################################################################

// TODO: memory allocator
// TODO: scheduler

/* class npu_rt_graph {

private:

    AL_U16 tensor_num ;
    rt_tensor_t** tensors ;
    AL_U8* tensor_alloc_stat ; // 0: unallocated, 1: allocated

    AL_U16 node_num ;
    rt_node_t** nodes ;

    AL_U16 analyzer_node_num ;
    analyzer_node** analyzer_nodes ;

    // for memory management
    mem_allocator allocator ;

    // define resources of basic nodes, index indicates basic node type
    AL_U16* basic_node_resources ;

    // for node status
    AL_U16* ready_node_num ;
    AL_U16** ready_nodes ;
    AL_U16* running_node_num ;
    AL_U16** running_nodes ;
    AL_U16 total_running_node_num ; // indicates whether the graph is finished

public:

    // build npu_rt_graph
    AL_S8 load_graph (char* bin_path) ;

    // execute npu_rt_graph
    AL_S8 execute () ;

private:

    analyzer_node* get_entry_node () ;

    // allocate mem space for tensor
    AL_S8 alloc (rt_tensor_t* tensor) ;

    // free mem space for tensor
    AL_S8 free (rt_tensor_t* tensor) ;


} ; */

#ifdef __cplusplus
}
#endif

#endif
