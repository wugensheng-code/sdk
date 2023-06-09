
#include <stdio.h>
#include <malloc.h>
#include "npu_rt_graph.h"
#include "npu_rt_graph_util.h"

#define LOG_TAG "RUNTIME GRAPH"
#include "elog.h"

// ################## rt_tensor related functions ######################

static AL_VOID init_rt_tensor (rt_tensor_t* tensor, int tensor_index) {

    tensor->index = tensor_index;
    tensor->name = NULL ;

    tensor->producer = (AL_S16*)malloc(sizeof(AL_S16) * RT_GRAPH_MAX_PRODUCER_NUM) ;
    for (int i = 0; i < RT_GRAPH_MAX_PRODUCER_NUM; i++)
    {
        tensor->producer[i] = -1 ;
    }
    tensor->producer_num = 0 ;

    tensor->consumer = (AL_S16*)malloc(sizeof(AL_S16) * RT_GRAPH_MAX_CONSUMER_NUM) ;
    for (int i = 0; i < RT_GRAPH_MAX_CONSUMER_NUM; i++)
    {
        tensor->consumer[i] = -1 ;
    }
    tensor->consumer_num = 0 ;

    tensor->tensor_type = RT_TENSOR_VAR ;
    tensor->allocated = 0 ;
    tensor->param = NULL ;

    // for producer split
    tensor->p_split_num = 0 ;
    tensor->p_sp_params = NULL ;

    // for consumer split
    tensor->c_split_num = 0 ;
    tensor->c_sp_params = NULL ;

}

rt_tensor_t* create_rt_tensor (rt_graph_t* graph) {

    rt_tensor_t* rt_tensor = (rt_tensor_t*) malloc(sizeof(rt_tensor_t));

    if (NULL == rt_tensor)
    {
        log_e("Malloc failed\n");
        return NULL;
    }

    init_rt_tensor(rt_tensor, graph->tensor_num);

    rt_tensor_t** new_tensor_list = (rt_tensor_t**)realloc(graph->tensor_list, sizeof(rt_tensor_t*) * (graph->tensor_num + 1));

    if (NULL == new_tensor_list)
    {
        free(rt_tensor);
        log_e("Realloc failed\n");
        return NULL;
    }

    new_tensor_list[graph->tensor_num] = rt_tensor;

    graph->tensor_list = new_tensor_list;
    graph->tensor_num++;

    return rt_tensor;

}

AL_VOID destroy_rt_tensor (rt_graph_t* graph, rt_tensor_t* tensor) {

    if (tensor->name != NULL) {
        free(tensor->name) ;
    }

    if (NULL != tensor->producer)
    {
        free(tensor->producer);
    }

    if (NULL != tensor->consumer)
    {
        free(tensor->consumer);
    }

    if (NULL != tensor->param)
    {
        free(tensor->param);
    }

    for (int i = 0; i < tensor->p_split_num; i++) {
        sp_param_t sp_param = tensor->p_sp_params[i] ;
        int total = sp_param.sp_c_num * sp_param.sp_h_num ;

        for(int j = 0; j < total; j++) {
            free(sp_param.sub_params[j]) ;
        }
        free(sp_param.sub_params);
        free(sp_param.addr_offsets);
    }
    free(tensor->p_sp_params);
  
    for (int i = 0; i < tensor->c_split_num; i++) {
        sp_param_t sp_param = tensor->c_sp_params[i] ;
        int total = sp_param.sp_c_num * sp_param.sp_h_num ;

        for(int j = 0; j < total; j++) {
            free(sp_param.sub_params[j]) ;
        }
        free(sp_param.sub_params);
        free(sp_param.addr_offsets);
    }
    free(tensor->c_sp_params);

    free(tensor);

}

int set_rt_tensor_producer (rt_tensor_t* tensor, const int index) {

    if (RT_GRAPH_MAX_PRODUCER_NUM <= tensor->producer_num)
    {
        AL_S16* new_producer = (AL_S16*)realloc(tensor->producer, sizeof(AL_S16) * (tensor->producer_num + 1));
        if (NULL == new_producer)
        {
            log_e("Realloc failed\n");
            return -1;
        }

        tensor->producer = new_producer;
    }

    tensor->producer[tensor->producer_num] = index;
    tensor->producer_num++;

    return 0;

}

int set_rt_tensor_consumer (rt_tensor_t* tensor, const int index) {

    if (RT_GRAPH_MAX_CONSUMER_NUM <= tensor->consumer_num)
    {
        AL_S16* new_consumer = (AL_S16*)realloc(tensor->consumer, sizeof(AL_S16) * (tensor->consumer_num + 1));
        if (NULL == new_consumer)
        {
            log_e("Realloc failed\n");
            return -1;
        }

        tensor->consumer = new_consumer;
    }

    tensor->consumer[tensor->consumer_num] = index;
    tensor->consumer_num++;

    return 0;

}

AL_VOID dump_rt_tensor (rt_graph_t* graph, rt_tensor_t* tensor) {

    log_d("tensor_%d type: %s", tensor->index, get_rt_tensor_type_string(tensor->tensor_type));

    if (tensor->name != NULL) {
        log_d(" name: %s", tensor->name);
    }

    if (tensor->producer_num > 0)
        log_d(" (producer: %d)", tensor->producer_num);

    if (tensor->consumer_num > 0)
        log_d(" (consumer: %d)", tensor->consumer_num);

    log_d(" (data_type: %d) (dim_num: %d)", tensor->param->data_type, tensor->param->dim_num);

    log_d("\n");
}


// ################## rt_node related functions ######################

static AL_VOID init_rt_node(rt_node_t* node, AL_U16 node_type, AL_U16 node_index) {

    node->index = node_index ;
    node->name = NULL ;
    node->node_type = node_type ;
    node->is_sub_nd = 0 ;
    node->c_sp_idx = 0 ;
    node->sub_nd_id = 0 ;
    node->param = NULL ;
    node->input_num = 0 ;
    node->output_num = 0 ;
    node->input_tensors = NULL ;
    node->output_tensors = NULL ;

}

rt_node_t* create_rt_node (rt_graph_t* graph, AL_U16 node_type) {
    
    rt_node_t* node = (rt_node_t*)malloc(sizeof(rt_node_t)) ;
    if (NULL == node)
    {
        log_e("Malloc failed\n");
        return NULL ;
    }

    init_rt_node(node, node_type, graph->node_num) ;


    rt_node_t** new_node_list = (rt_node_t**)realloc(graph->node_list, sizeof(rt_node_t*) * (graph->node_num + 1)) ;

    if (NULL == new_node_list)
    {
	    free(node) ;
        log_e("Realloc failed\n");
        return NULL;
    }

    node->graph = graph;

    new_node_list[graph->node_num] = node;

    graph->node_list = new_node_list;
    graph->node_num++;

    return node;

}

static AL_VOID free_rt_node_param (rt_node_t* node) {
    if (node->param == NULL) {
        return ;
    }
    AL_U16 nd_type = node->node_type ;
    switch (nd_type) {
        case RT_NPU_HARD: 
        {
            hard_npu_pkg_info_t* param = (hard_npu_pkg_info_t*) node->param ;
            free (param) ;
            param = NULL ;
            break ;
        }
        case RT_POOLING: 
        case RT_CONCAT: 
        case RT_UPSAMPLE: 
        case RT_ELTWISE: 
        case RT_PRELU: 
        case RT_INTERP: 
        {
            soft_npu_pkg_info_t* param = (soft_npu_pkg_info_t*) node->param ;
            free (param) ;
            param = NULL ;
            break ;
        }
        case RT_DWCONV: 
        {
            npu_dwconv_param_t* param = (npu_dwconv_param_t*) node->param ;

            free (param) ;
            param = NULL ;

            break ;
        }
        case RT_RESHAPE: 
        {
            npu_reshape_param_t* param = (npu_reshape_param_t*) node->param ;

            free (param) ;
            param = NULL ;

            break ;
        }
        case RT_SIGMOID: 
        {
            npu_sigmoid_param_t* param = (npu_sigmoid_param_t*) node->param ;

            free (param) ;
            param = NULL ;

            break ;
        }
        case RT_INPUT: 
        {
            npu_input_param_t* param = (npu_input_param_t*) node->param ;

            if (param->output_tensor != NULL) {
                free (param->output_tensor) ;
                param->output_tensor = NULL ;
            }

            free (param) ;
            param = NULL ;

            break ;
        }
        case RT_YOLO: 
        {
            yolo_pkg_info_t** param = (yolo_pkg_info_t**) node->param ;
            for(int idx = 0; idx < node->input_num; idx++) {
                free (param[idx]) ;
                param[idx] = NULL ;
            }

            free (param) ;
            param = NULL ;

            break ;
        }
        default: 
        {
            // unsupported node type
        }
    }

    return ;
}

AL_VOID destroy_rt_node (rt_graph_t* graph, rt_node_t* node) {

    if (node->name != NULL) {
        free(node->name);
    }

    if (0 < node->input_num)
    {
        free(node->input_tensors);
        node->input_tensors = NULL;
    }

    if (0 < node->output_num)
    {
        free(node->output_tensors);
        node->output_tensors = NULL;
    }

    // free node param
    free_rt_node_param (node) ;

    free(node);

}

int set_rt_node_input_tensor (rt_node_t* node, int input_idx, rt_tensor_t* tensor) {

    if (input_idx >= node->input_num)
    {
        AL_S16* new_tensor = (AL_S16*)realloc(node->input_tensors, sizeof(AL_S16) * (input_idx + 1));

        if (NULL == new_tensor)
        {
            log_e("Realloc failed\n");
            return -1;
        }

        for (int i = node->input_num; i < input_idx + 1; i++)
        {
            new_tensor[i] = -1;
        }

        node->input_tensors = (AL_U16*)new_tensor;
        node->input_num = input_idx + 1;
    }

    node->input_tensors[input_idx] = tensor->index;
    if (set_rt_tensor_consumer(tensor, node->index) < 0)
    {
        log_e("Set rt_tensor consumer failed\n");
        return -1;
    }
    return 0;

}

int set_rt_node_output_tensor (rt_node_t* node, int output_idx, rt_tensor_t* tensor) {

    if (output_idx >= node->output_num)
    {
        AL_U16* new_tensor = (AL_U16*)realloc(node->output_tensors, sizeof(AL_S16) * (output_idx + 1));
        if (new_tensor == NULL) {
            log_e("Realloc failed\n");
            return -1;
        }

        for (int i = node->output_num; i < output_idx + 1; i++)
        {
            new_tensor[i] = -1;
        }

        node->output_tensors = new_tensor;
        node->output_num = output_idx + 1;
    }

    node->output_tensors[output_idx] = tensor->index;
    if (set_rt_tensor_producer(tensor, node->index) < 0)
    {
        log_e("Set rt_tensor producer failed\n");
        return -1;
    }

    return 0;
}

AL_VOID dump_rt_node (rt_graph_t* graph, rt_node_t* node) {

    log_d("\nnode: %d type: %s is_sub_nd: %d sub_nd_id: %d\n", node->index, get_rt_node_type_string(node->node_type), node->is_sub_nd, node->sub_nd_id);

    if (node->name != NULL) {
        log_d("\tname: %s\n", node->name);
    }

    if (0 < node->input_num)
    {
        log_d("\tinput tensors: %d\n", node->input_num);
    }

    for (int i = 0; i < node->input_num; i++)
    {
        rt_tensor_t* rt_tensor = get_rt_graph_tensor(graph, node->input_tensors[i]);

        log_d("\t    %d: [id: %d] ", i, rt_tensor->index);

        dump_rt_tensor(graph, rt_tensor);
    }

    if (0 < node->output_num)
        log_d("\toutput tensors: %d\n", node->output_num);

    for (int i = 0; i < node->output_num; i++)
    {
        rt_tensor_t* rt_tensor = get_rt_graph_tensor(graph, node->output_tensors[i]);

        log_d("\t    %d: [id: %d] ", i, rt_tensor->index);

        dump_rt_tensor(graph, rt_tensor);
    }
}

// ################## scheduler_node related functions ######################

basic_node_t* create_basic_node (AL_S16 rt_node_idx, AL_S16 parent_idx) {

    basic_node_t* node = (basic_node_t*)malloc(sizeof(basic_node_t)) ;
    if (node == NULL) {
        log_e("Malloc failed\n");
        return NULL ;
    }

    node->index = rt_node_idx ;
    node->parent = parent_idx ;

    return node ;

}


parallel_group_t* create_parallel_group (AL_S16 parent_idx) {

    parallel_group_t* pg = (parallel_group_t*)malloc(sizeof(parallel_group_t)) ;
    if (pg == NULL) {
        log_e("Malloc failed\n");
        return NULL;
    }

    pg->parent = parent_idx ;
    pg->node_num = 0 ;
    pg->nodes = NULL ;
    pg->is_split = 0 ;

    return pg ;

}

int add_child_pg (parallel_group_t* pg, scheduler_node_t* child_node) {

    if (pg == NULL) {
        log_e("Parallel group is null point in add_child for parallel group.\n");
        return -1;
    } else {
        AL_S16* nodes = NULL ;

        if (pg->node_num == 0) {
            nodes = (AL_S16*)malloc(sizeof(AL_S16) * (pg->node_num + 1)) ;
        } else {
            nodes = (AL_S16*)realloc(pg->nodes, sizeof(AL_S16) * (pg->node_num + 1)) ;
        }
            
        if (nodes == NULL) {
            log_e("Alloc failed in add_child for parallel group.\n") ;
            return -1 ;
        } else {
            pg->nodes = nodes ;
            pg->nodes[pg->node_num] = child_node->index ;
            pg->node_num += 1 ;
        }
    }

    return 0 ;
    
}


sequential_list_t* create_sequential_list (AL_S16 parent_idx) {

    sequential_list_t* sl = (sequential_list_t*)malloc(sizeof(sequential_list_t));
    if (sl == NULL) {
        log_e("Malloc failed\n");
        return NULL;
    }

    sl->parent = parent_idx;
    sl->node_num = 0;
    sl->nodes = NULL;

    return sl;
    
}

int add_child_sl (sequential_list_t* sl, scheduler_node_t* child_node) {

    if (sl == NULL) {
        log_e("Sequential list is null point in add_child for sequential list.\n") ;
        return -1 ;
    } else {
        AL_S16* nodes = NULL ;

        if (sl->node_num == 0) {
            nodes = (AL_S16*)malloc(sizeof(AL_S16) * (sl->node_num + 1)) ;
        } else {
            nodes = (AL_S16*)realloc(sl->nodes, sizeof(AL_S16) * (sl->node_num + 1)) ;
        }
            
        if (nodes == NULL) {
            log_e("Alloc failed in add_child for sequential list.\n") ;
            return -1 ;
        } else {
            sl->nodes = nodes ;
            sl->nodes[sl->node_num] = child_node->index ;
            sl->node_num += 1 ;
        }
    }

    return 0 ;
    
}

scheduler_node_t* create_scheduler_node (rt_graph_t* graph, AL_U16 node_type, AL_VOID* content) {
    
    scheduler_node_t* node = (scheduler_node_t*)malloc(sizeof(scheduler_node_t)) ;
    if (NULL == node)
    {
        log_e("Malloc failed\n") ;
        return NULL ;
    }

    node->node_type = node_type ;
    if (node_type == 0) {
        node->nd = (basic_node_t*) content ;
    } else if (node_type == 1) {
        node->pg = (parallel_group_t*) content ;
    } else if (node_type == 2) {
        node->sl = (sequential_list_t*) content ;
    } else {
        log_e("Unsupported node type for scheduler node.\n") ;
        free (node) ;
        return NULL ;
    }

    node->index = graph->scheduler_node_num ;

    scheduler_node_t** new_node_list = (scheduler_node_t**)realloc(graph->scheduler_node_list, sizeof(scheduler_node_t*) * (graph->scheduler_node_num + 1)) ;

    if (NULL == new_node_list)
    {
	    free(node) ;
        log_e("Realloc failed\n") ;
        return NULL;
    }

    node->graph = graph;

    new_node_list[graph->scheduler_node_num] = node;

    graph->scheduler_node_list = new_node_list;
    graph->scheduler_node_num++;

    return node;

}

AL_VOID destroy_scheduler_node (rt_graph_t* graph, scheduler_node_t* node) {

    if (node->node_type == 0) {
        free (node->nd) ;
    } else if (node->node_type == 1) {
        free (node->pg) ;
    } else if (node->node_type == 2) {
        free (node->sl) ;
    } else {
        log_e("Unexpected node type in destroy scheduler node.\n") ;
    }

    free(node);

}

int add_child_sn (scheduler_node_t* node, scheduler_node_t* child_node) {

    int ret = 0 ;

    if (node->node_type == 1) {
        parallel_group_t* pg = node->pg ;
        ret = add_child_pg (pg, child_node) ;
    } else if (node->node_type == 2) {
        sequential_list_t* sl = node->sl ;
        ret = add_child_sl (sl, child_node) ;
    } else {
        log_e("Unexpected node type in add_child for scheduler node.\n") ;
        ret = -1 ;
    }

    return ret;

}

int get_child_num (scheduler_node_t* node) {

    int ret = -1 ;
    if (node == NULL) {
        log_e("The scheduler node pointer is null in get_child_num.\n") ;
        return ret ;
    } else {
        if (node->node_type == RT_S_BASIC) {
            ret = 0 ;
        } else if (node->node_type == RT_S_PARALLEL) {
            parallel_group_t* pg = node->pg ;
            if (pg == NULL) {
                log_e("Pg in scheduler node is null.\n") ;
                return ret ;
            } else {
                ret = pg->node_num ;
            }
        } else if (node->node_type == RT_S_SEQUENTIAL) {
            sequential_list_t* sl = node->sl ;
            if (sl == NULL) {
                log_e("Sl in scheduler node is null.\n") ;
                return ret ;
            } else {
                ret = sl->node_num ;
            }
        } else {
            log_e("Unsupported node type for scheduler node.\n") ;
            return ret ;
        }
    }
    return ret ;
}

int set_parent (scheduler_node_t* node, AL_S16 parent_idx) {

    int ret = 0 ;
    if (node == NULL) {
        log_e("The scheduler node pointer is null in set_parent.\n") ;
        return -1 ;
    } else {
        if (node->node_type == RT_S_BASIC) {
            //check_pointer (node->nd) ;
            if (node->nd == NULL) {
                log_e("Nd in scheduler node is NULL\n");
                return -1;
            }
            node->nd->parent = parent_idx ;
        } else if (node->node_type == RT_S_PARALLEL) {
            //check_pointer (node->pg) ;
            if (node->pg == NULL) {
                log_e("Pg in node is NULL\n");
                return -1;
            }
            node->pg->parent = parent_idx ;
        } else if (node->node_type == RT_S_SEQUENTIAL) {
            //check_pointer (node->sl) ;
            if (node->sl == NULL) {
                log_e("Sl in node is NULL\n");
                return -1;
            }
            node->sl->parent = parent_idx ;
        } else {
            log_e("Unsupported node type for scheduler node.\n") ;
            return -1 ;
        }
    }
    return ret ;
}

AL_VOID dump_scheduler_node (rt_graph_t* graph, scheduler_node_t* node, int level) {

    char* node_buf = (char*)malloc(sizeof(char) * 1024 * 4) ;

    sn_level_str (node_buf, level, ' ') ;

    if (node->node_type == RT_S_BASIC) {
        rt_node_t* rt_nd = get_rt_graph_node (graph, node->nd->index) ;
        log_d("\n%s+ basic node: %s sn index: %d sn_parent: %d \n", node_buf, get_rt_node_type_string(rt_nd->node_type), node->index, node->nd->parent);
    } else if (node->node_type == RT_S_PARALLEL) {
        log_d("\n%s+ parallel: %d cmds sn index: %d sn_parent: %d ", node_buf, node->pg->node_num, node->index, node->pg->parent);
        for (int i = 0; i < node->pg->node_num; i++) {
            scheduler_node_t* sn = get_rt_graph_scheduler_node (graph, node->pg->nodes[i]) ;
            dump_scheduler_node (graph, sn, level + 1) ;
        }
    } else if (node->node_type == RT_S_SEQUENTIAL) {
        log_d("\n%s+ sequential: %d cmds sn index: %d sn_parent: %d ", node_buf, node->sl->node_num, node->index, node->sl->parent);
        for (int i = 0; i < node->sl->node_num; i++) {
            scheduler_node_t* sn = get_rt_graph_scheduler_node (graph, node->sl->nodes[i]) ;
            dump_scheduler_node (graph, sn, level + 1) ;
        }
    } else {
        log_d("ERROR: \nunsupported scheduler node type\n");
    }

    free(node_buf) ;

}


// ################## rt_graph related functions ######################

rt_graph_t* create_rt_graph () {

    rt_graph_t* graph = (rt_graph_t*)malloc(sizeof(rt_graph_t));
    if (NULL == graph)
    {
        log_e("Malloc failed\n");
        return NULL;
    }

    init_rt_graph(graph);

    return graph;

}

AL_VOID init_rt_graph (rt_graph_t* graph) {

    graph->tensor_list = NULL;
    graph->node_list = NULL;
    graph->scheduler_node_list = NULL;
    //graph->basic_node_resources = NULL;

    graph->tensor_num = 0;
    graph->node_num = 0;
    graph->scheduler_node_num = 0;

}

AL_VOID destroy_rt_graph (rt_graph_t* graph) {

    if (NULL == graph) {
        return;
    }

    //!< 1, destroy tensors
    for (int i = 0; i < graph->tensor_num; i++)
    {
        destroy_rt_tensor(graph, graph->tensor_list[i]);
    }

    //!< 2, destroy nodes
    for (int i = 0; i < graph->node_num; i++)
    {
        destroy_rt_node(graph, graph->node_list[i]);
    }

    //!< 3, destroy scheduler nodes
    for (int i = 0; i < graph->scheduler_node_num; i++)
    {
        destroy_scheduler_node(graph, graph->scheduler_node_list[i]);
    }

    free(graph->tensor_list);
    free(graph->node_list);
    free(graph->scheduler_node_list);
    //free(graph->basic_node_resources);

    free(graph);
}

rt_tensor_t* get_rt_graph_tensor (rt_graph_t* graph, int index) {
    return graph->tensor_list[index];
}

rt_node_t* get_rt_graph_node (rt_graph_t* graph, int index) {
    return graph->node_list[index];
}

scheduler_node_t* get_rt_graph_scheduler_node (rt_graph_t* graph, int index) {
    return graph->scheduler_node_list[index];
}

AL_VOID dump_rt_graph (rt_graph_t* graph) {

    log_d("\n graph node_num %u tensor_num: %u \n", graph->node_num, graph->tensor_num);

    for (int i = 0; i < graph->node_num; i++)
    {
        rt_node_t* node = graph->node_list[i] ;
        dump_rt_node(graph, node);
    }

    log_d("\n dump scheduler node structure:\n");

    if (graph->scheduler_node_num > 0) {
        scheduler_node_t* first = graph->scheduler_node_list[0] ;
        dump_scheduler_node (graph, first, 0) ;
    }

}
