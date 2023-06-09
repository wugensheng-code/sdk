#include <unistd.h>
#include <pthread.h>

#include "stdio.h"
#include "npu_scheduler.h"
#include "npu_defines.h"
#include "fifo.h"
// #include "npu_driver.h"
#include "image.h"
#include "npu_util.h"
#include "fm_mem_alloc.h"
#include "npu_driver.h"

#include "cpu_aclr_op.h"
#include "cpu_op_func.h"
#include "npu_driver_io.h"
#include "npu_lock.h"

#define LOG_TAG "NPU_SCHEDULER"
#include "elog.h"

/* ------------------------------- global variables -------------------------------------------------------------------- */

volatile AL_S16* ts_usage_rec = NULL ; // record the consumer number of allocated tensors, it is initialized in init process.
volatile AL_S16* sched_nd_rec = NULL ; // record the children visited status of scheduler nodes, it is initialized in init process.
volatile AL_U64* ts_vaddr_rec = NULL ; // record the virtual addr of allocated tensors, it is initialized in init process.

/* to meet the needs of scheduler, the fifo may need to be changed to linked list */
fifo_t* hard_ready_nodes = NULL ; // store the hard basic nodes which are ready to run, it is initialized in init process.
fifo_t* soft_ready_nodes = NULL ; // store the soft basic nodes which are ready to run, it is initialized in init process.
fifo_t* hard_running_nodes = NULL ; // store the hard basic nodes which are running, it is initialized in init process.
fifo_t* soft_running_nodes = NULL ; // store the soft basic nodes which are running, it is initialized in init process.

/* the following variables records the num of hard/soft node in rt_graph */
volatile AL_U32 hard_node_num = 0 ;
volatile AL_U32 soft_node_num = 0 ;

/* the following variables are used for yolo */
AL_S8 yolo_id = 0 ; // record how many input feature maps have been sent to hardware

/* the following variables are used for debug */
AL_U8 run_dump_tensor = 0 ; // 0: do nothing, 1: dump tensor in sd card as bin files
AL_U8 print_ts_info = 0 ; // 0: do nothing, 1: print tensor id/name/addr at running time

volatile AL_U32 hard_finish_cnt = 0;
volatile AL_U32 soft_finish_cnt = 0;

/* define the platform to run */
AL_U8 config_dma_mode = 1;

/* ------------------------------- end of global variables -------------------------------------------------------------------- */

/* ------------------------------- util functions -------------------------------------------------------------------- */

/* after the scheduler node is finished, update the usage of the input tensors, free them if necessary */
AL_S8 update_ts_usage (scheduler_node_t* sn) {

    if (sn->node_type != RT_S_BASIC) {
        log_e ("the scheduler node to upate ts usage must be basic node, but actually is: %d\n", sn->node_type) ;
        return -1 ;
    }

    /* update tensor usage and free tensor mem if all the consumer are executed */
    rt_node_t* rt_nd = get_rt_graph_node (sn->graph, sn->nd->index) ;
    for (unsigned int j = 0; j < rt_nd->input_num; j++) {
        rt_tensor_t* rt_ts = get_rt_graph_tensor (rt_nd->graph, rt_nd->input_tensors[j]) ;
        if (rt_ts->tensor_type == RT_TENSOR_VAR) {
            --ts_usage_rec[rt_ts->index] ;
            if (ts_usage_rec[rt_ts->index] == 0) {
                free_mem_buddy(rt_ts) ;
                rt_ts->allocated = 0 ;
            }
        }
    }

    return 0 ;
}

/* ------------------------------- end of util functions -------------------------------------------------------------------- */



/* ------------------------------- irq handler functions -------------------------------------------------------------------- */

/* function declarations which are used in irq handler functions */
AL_S8 update_ready_nodes (scheduler_node_t* sn) ;
AL_S8 run_scheduler_node (scheduler_node_t* sn) ;

AL_S8 handle_soft_node_irq () {

    /* special process for yolo node */

    /* read yolo switch signal */
    AL_U32 res = soft_npu_read(SOFTNPU__SOFT_SHARE__CFG_INIT_ADDR__ADDR) ;
    AL_U32 mask = 0x800 ;
    res &= mask ;

    if(npu_lock_lock()) {
        log_e("fail to lock\n");
        return -1;
    }

    if (res == 0x800) {
        /* the soft node is yolo */
        /* read yolo layer num */
        AL_U32 layer_num = soft_npu_read(SOFTNPU__SOFT_YOLO__YOLO_C__ADDR) ;
        layer_num = layer_num >> 16 ;
        mask = 0x3 ;
        layer_num &= mask ;

        if (yolo_id < layer_num) {
            any_data_t front_node = get_fifo_data(soft_running_nodes, soft_running_nodes->rdptr) ;
            scheduler_node_t* front_sn = (scheduler_node_t*) front_node.pointer ;
            rt_node_t* yolo_node = get_rt_graph_node (front_sn->graph, front_sn->nd->index) ;
            if (yolo_node->node_type != RT_YOLO) {
                log_e("get yolo node failed\n") ;
                return -1;
            } else {
                yolo_pkg_info_t** param = (yolo_pkg_info_t**) yolo_node->param ;

                if (config_dma_mode) {
                    run_npu_soft_yolo_dma_mode(param[yolo_id++]) ;
                } else {
                    run_npu_soft_yolo_normal_mode(param[yolo_id++]) ;
                }
            }

        } else {

            any_data_t out_data = pop_fifo_data(soft_running_nodes) ;
            scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
            if (update_ts_usage (sn)) {
                log_e("update_ts_usage failed\n");
                return -1;
            }
            if (update_ready_nodes (sn)) {
                log_e("update_ready_nodes failed\n");
                return -1;
            }

            /* assign nodes to npu */
            if ((fifo_item_num(soft_running_nodes) == 0) && (fifo_item_num(soft_ready_nodes) > 0)) {
                any_data_t out_data = pop_fifo_data(soft_ready_nodes) ;
                scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
                push_fifo_data (soft_running_nodes, out_data) ;
                if (run_scheduler_node (sn)) {
                    log_e("run_scheduler_node failed\n");
                    return -1;
                }
            }
            if ((fifo_item_num(hard_running_nodes) == 0) && (fifo_item_num(hard_ready_nodes) > 0)) {
                any_data_t out_data = pop_fifo_data(hard_ready_nodes) ;
                scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
                push_fifo_data (hard_running_nodes, out_data) ;
                if (run_scheduler_node (sn)) {
                    log_e("run_scheduler_node failed\n");
                    return -1;
                }
            }
        }
    } else {
        any_data_t out_data = pop_fifo_data(soft_running_nodes) ;
        scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
        if (update_ts_usage (sn)) {
            log_e("update_ts_usage failed\n");
            return -1;
        }
        if (update_ready_nodes (sn)) {
            log_e("update_ready_nodes failed\n");
            return -1;
        }


        /* for debug */
        rt_node_t* rt_nd = get_rt_graph_node (sn->graph, sn->nd->index) ;
        rt_tensor_t* t_output = get_rt_graph_tensor (rt_nd->graph, rt_nd->output_tensors[0]) ;

        if (run_dump_tensor == 1) {
            dump_tensor (t_output) ;
        }
        if (print_ts_info == 1) {
            log_i ("tensor id is: %d\n", t_output->index) ;
            log_i ("tensor name is: %s\n", t_output->name) ;
            log_i ("tensor addr is: %x\n", t_output->param->addr) ;
        }

        /* assign nodes to npu */
        if ((fifo_item_num(soft_running_nodes) == 0) && (fifo_item_num(soft_ready_nodes) > 0)) {
            any_data_t out_data = pop_fifo_data(soft_ready_nodes) ;
            scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
            push_fifo_data (soft_running_nodes, out_data) ;
            if (run_scheduler_node (sn)) {
                log_e("run_scheduler_node failed\n");
                return -1;
            }
        }
        if ((fifo_item_num(hard_running_nodes) == 0) && (fifo_item_num(hard_ready_nodes) > 0)) {
            any_data_t out_data = pop_fifo_data(hard_ready_nodes) ;
            scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
            push_fifo_data (hard_running_nodes, out_data) ;
            if (run_scheduler_node (sn)) {
                log_e("run_scheduler_node failed\n");
                return -1;
            }
        }
    }

    if(npu_lock_unlock()) {
        log_e("fail to unlock\n");
        return -1;
    }

    return 0 ;
}


AL_S8 handle_hard_node_irq () {

    any_data_t out_data = pop_fifo_data(hard_running_nodes) ;
    scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;

    if(npu_lock_lock()) {
        log_e("fail to lock\n");
        return -1;
    }

    if (update_ts_usage (sn)) {
        log_e("update_ts_usage failed\n");
        return -1;
    }
    if (update_ready_nodes (sn)) {
        log_e("update_ready_nodes failed\n");
        return -1;
    }

    if(npu_lock_unlock()) {
        log_e("fail to unlock\n");
        return -1;
    }

    /* for debug */
    rt_node_t* rt_nd = get_rt_graph_node (sn->graph, sn->nd->index) ;
    rt_tensor_t* t_output = get_rt_graph_tensor (rt_nd->graph, rt_nd->output_tensors[0]) ;

    /* check for split node */
    if (((t_output->p_split_num == 0) && !rt_nd->is_sub_nd) || ((t_output->p_split_num > 0) && (rt_nd->sub_nd_id + 1) == (t_output->p_sp_params[0].sp_c_num * t_output->p_sp_params[0].sp_h_num))) {
        if (run_dump_tensor == 1) {
            dump_tensor (t_output) ;
        }
        if (print_ts_info == 1) {
            log_i ("tensor id is: %d\n", t_output->index) ;
            log_i ("tensor name is: %s\n", t_output->name) ;
            log_i ("tensor addr is: %x\n", t_output->param->addr) ;
        }
    }

    if(npu_lock_lock()) {
        log_e("fail to lock\n");
        return -1;
    }

    /* assign nodes to npu */
    if ((fifo_item_num(soft_running_nodes) == 0) && (fifo_item_num(soft_ready_nodes) > 0)) {
        any_data_t out_data = pop_fifo_data(soft_ready_nodes) ;
        scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
        push_fifo_data (soft_running_nodes, out_data) ;
        if (run_scheduler_node (sn)) {
            log_e("run_scheduler_node failed\n");
            return -1;
        }
    }
    if ((fifo_item_num(hard_running_nodes) == 0) && (fifo_item_num(hard_ready_nodes) > 0)) {
        any_data_t out_data = pop_fifo_data(hard_ready_nodes) ;
        scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
        push_fifo_data (hard_running_nodes, out_data) ;
        if (run_scheduler_node (sn)) {
            log_e("run_scheduler_node failed\n");
            return -1;
        }
    }

    if(npu_lock_unlock()) {
        log_e("fail to unlock\n");
        return -1;
    }

    return 0 ;
}

/* ------------------------------- end of irq handler functions -------------------------------------------------------------------- */



/* ------------------------------- update_ready_nodes related functions -------------------------------------------------------------------- */

/* put scheduler node to hard_ready_nodes fifo or soft_ready_nodes fifo according to the rt_node type in the scheduler node */
AL_S8 put_to_fifo (scheduler_node_t* sn) {

    rt_node_t* rt_nd = get_rt_graph_node (sn->graph, sn->nd->index) ;
    if (NULL == rt_nd) {
        log_e("rt_node_t* is NULL\n");
        return -1;
    }
    if (rt_nd->node_type == RT_NPU_HARD) {
        if (fifo_free_room(hard_ready_nodes) < 1) {
            log_e("hard ready nodes fifo is full, and the free room is: %d\n", fifo_free_room(hard_ready_nodes)) ;
            return -1 ;
        }
        any_data_t in_data;
        in_data.pointer = (AL_VOID*) sn ;
        push_fifo_data (hard_ready_nodes, in_data) ;
    } else {
        if (fifo_free_room(soft_ready_nodes) < 1) {
            log_e("soft ready nodes fifo is full, and the free room is: %d\n", fifo_free_room(soft_ready_nodes)) ;
            return -1 ;
        }
        any_data_t in_data;
        in_data.pointer = (AL_VOID*) sn ;
        push_fifo_data (soft_ready_nodes, in_data) ;
    }

    return 0;
}

/* a group of split hard_node is ready to run, the params of the split hard node need to preprocess */
AL_S8 prepare_split_node_to_fifo (scheduler_node_t* sn) {
    // this kind of parallel group is generated by me and has a fixed pattern.
    // temporary solution for 1 npu 

    /* prepare split parameters */
    scheduler_node_t* child_sl = get_rt_graph_scheduler_node(sn->graph, sn->pg->nodes[0]) ;
    scheduler_node_t* child_bn = get_rt_graph_scheduler_node(child_sl->graph, child_sl->sl->nodes[0]) ;
    rt_node_t* rt_nd = get_rt_graph_node(child_bn->graph, child_bn->nd->index) ;
    rt_tensor_t* t_input = get_rt_graph_tensor (rt_nd->graph, rt_nd->input_tensors[0]) ;
    AL_U8 c_sp_idx = rt_nd->c_sp_idx ;
    AL_U8 sp_c_num = t_input->c_sp_params[c_sp_idx].sp_c_num ;
    AL_U8 sp_h_num = t_input->c_sp_params[c_sp_idx].sp_h_num ;
    AL_U8 ic = 0 ;
    AL_U8 ih = 0 ;
    AL_U8 load_fm = 1 ;
    AL_U8 load_flt = 1 ;

    /* 1> check whether input tensors are allocated */
    for (unsigned int j = 0; j < rt_nd->input_num; j++) {
        rt_tensor_t* rt_ts = get_rt_graph_tensor (rt_nd->graph, rt_nd->input_tensors[j]) ;
        if (!(rt_ts->allocated)) {
            log_e("input tensor: %d of rt_nd: %d is not allocated\n", j, rt_nd->index) ;
            return -1 ;
        }
    }

    /* 2> if the output tensor is not allocated yet, allocate the output tensor */
    for (unsigned int j = 0; j < rt_nd->output_num; j++) {
        rt_tensor_t* rt_ts = get_rt_graph_tensor (rt_nd->graph, rt_nd->output_tensors[j]) ;
        if(!(rt_ts->allocated)) {
            alloc_mem_buddy(rt_ts) ;
            ts_usage_rec[rt_ts->index] = rt_ts->consumer_num ;
        }
    }

    for (int i = 0; i < sn->pg->node_num; i++) {

        /* find the param of each sub node */
        child_sl = get_rt_graph_scheduler_node(sn->graph, sn->pg->nodes[i]) ;
        child_bn = get_rt_graph_scheduler_node(child_sl->graph, child_sl->sl->nodes[0]) ;
        rt_nd = get_rt_graph_node(child_bn->graph, child_bn->nd->index) ;

        hard_npu_pkg_info_t* param = (hard_npu_pkg_info_t*) rt_nd->param ;

        /* find input tensor */
        rt_tensor_t* t = get_rt_graph_tensor(rt_nd->graph, rt_nd->input_tensors[0]) ;
        param->ifm_base_addr = t->param->addr + t->c_sp_params[c_sp_idx].addr_offsets[rt_nd->sub_nd_id] ;

         /* find weight tensor */
        t = get_rt_graph_tensor(rt_nd->graph, rt_nd->input_tensors[1]) ;
        param->flt_base_addr = t->param->addr ;

        /* find output tensor */
        t = get_rt_graph_tensor(rt_nd->graph, rt_nd->output_tensors[0]) ;
        param->ofm_base_addr = t->param->addr + t->p_sp_params[0].addr_offsets[rt_nd->sub_nd_id] ;

        /* set load fm && filter attribute */
        /* declare loda data mode, 0:load FM&Filter 1:only load FM 2:load Filter */
        if (i == 0) {
            /* always load FM&&Filter for the first sub node of spilt group */
            param->mode_load_data = 0 ;
        } else {
            if (sp_c_num == 1) {
                // only need to update FM
                param->mode_load_data = 1 ;
            } else if (sp_h_num == 1) {
                // only need to update Filter
                param->mode_load_data = 2 ;
            } else {
                if (ic == 0) {
                    // need to update FM&&Filter
                    param->mode_load_data = 0 ;
                } else {
                    // only need to update Filter
                    param->mode_load_data = 2 ;
                }
            }
        }

        ic++ ;
        if (ic == sp_c_num) {
            ic = 0 ;
            ih++ ;
        }

        /* put the scheduler node to ready fifo */
        if (put_to_fifo (child_bn)) {
            log_e("put_to_fifo failed\n");
            return -1;
        }
    }

    return 0 ;
}

/* a basic node is ready to run, the param of the basic node need to preprocess */
AL_S8 prepare_basic_node_to_fifo (scheduler_node_t* sn) {

    rt_node_t* nd = get_rt_graph_node (sn->graph, sn->nd->index) ;
    if (NULL == nd) {
        log_e("rt_node_t* is NULL\n");
        return -1;
    }

    AL_U16 node_type = nd->node_type ;

    if (node_type == RT_INPUT) {
        if (put_to_fifo (sn)) {
            log_e("put_to_fifo failed\n");
            return -1;
        } else {
            return 0;
        }
    }

    /* 1> check whether input tensors are allocated */
    for (unsigned int j = 0; j < nd->input_num; j++) {
        rt_tensor_t* rt_ts = get_rt_graph_tensor (nd->graph, nd->input_tensors[j]) ;
        if (!(rt_ts->allocated)) {
            log_e("\nError: input tensor: %d of rt_nd: %d is not allocated\n", j, nd->index) ;
            return -1 ;
        }
    }

    /* 2> if the output tensor is not allocated yet, allocate the output tensor */
    for (unsigned int j = 0; j < nd->output_num; j++) {
        rt_tensor_t* rt_ts = get_rt_graph_tensor (nd->graph, nd->output_tensors[j]) ;
        if(!(rt_ts->allocated)) {
            alloc_mem_buddy(rt_ts) ;
            ts_usage_rec[rt_ts->index] = rt_ts->consumer_num ;
        }
    }

    // for future work: if support concat optimization, get the correct output tensor offset
    switch (node_type) {
        case RT_NPU_HARD:
        {
            hard_npu_pkg_info_t* param = (hard_npu_pkg_info_t*) nd->param ;
            param->mode_load_data = 0 ;

            rt_tensor_t* t = NULL ;

            t = get_rt_graph_tensor(nd->graph, nd->input_tensors[0]) ;
            param->ifm_base_addr = t->param->addr ;

            t = get_rt_graph_tensor(nd->graph, nd->input_tensors[1]) ;
            param->flt_base_addr = t->param->addr ;

            t = get_rt_graph_tensor(nd->graph, nd->output_tensors[0]) ;
            param->ofm_base_addr = t->param->addr ;

            // printf ("p_basic_node: node id: %d out addr: %x\n", nd->index, t->param->addr) ;

            if (put_to_fifo (sn)) {
                log_e("put_to_fifo failed\n");
                return -1;
            }

            break ;
        }
        case RT_POOLING:
        case RT_CONCAT:
        case RT_UPSAMPLE:
        case RT_PRELU:
        case RT_INTERP:
        case RT_ELTWISE:
        {
            soft_npu_pkg_info_t* param = (soft_npu_pkg_info_t*) nd->param ;

            rt_tensor_t* t = NULL ;

            t = get_rt_graph_tensor(nd->graph, nd->output_tensors[0]) ;
            param->ofm_base_addr = t->param->addr ;

            for (unsigned int i = 0; i < nd->input_num; i++) {
                t = get_rt_graph_tensor(nd->graph, nd->input_tensors[i]) ;
                switch (i) {
                    case 0 :
                    {
                        param->ifm0_base_addr = t->param->addr ;
                        break ;
                    }
                    case 1 :
                    {
                        param->ifm1_base_addr = t->param->addr ;
                        break ;
                    }
                    case 2 :
                    {
                        param->ifm2_base_addr = t->param->addr ;
                        break ;
                    }
                    case 3 :
                    {
                        param->ifm3_base_addr = t->param->addr ;
                        break ;
                    }
                    default :
                    {
                        log_e("the input_num is more than 4 of node: %s, idx: %u\n", nd->name, i) ;
                        return -1;
                    }
                }
            }

            if (put_to_fifo (sn)) {
                log_e("put_to_fifo failed\n");
                return -1;
            }

            break ;
        }
        case RT_DWCONV:
        {
            npu_dwconv_param_t* param = (npu_dwconv_param_t*) nd->param;

            /* prepare input tensor info */
            param->input_num = nd->input_num ;
            param->input_tensors = (tensor_param_t**)malloc(sizeof(tensor_param_t) * nd->input_num) ;
            rt_tensor_t* t = NULL ;
            for (unsigned int i = 0; i < nd->input_num; i++) {
                t = get_rt_graph_tensor(nd->graph, nd->input_tensors[i]) ;
                param->input_tensors[i] = t->param ;
            }

            /* prepare output tensor info */
            t = get_rt_graph_tensor(nd->graph, nd->output_tensors[0]) ;
            param->output_tensor = t->param ;

            log_v ("dwconv: node id: %d out addr: %x\n", nd->index, t->param->addr) ;

            npu_dwconv_node(param);

            if (run_dump_tensor == 1) {
                dump_tensor(t);
            }

//            put_to_fifo (sn) ;
            update_ready_nodes (sn) ;
            update_ts_usage (sn) ;
            break;
        }
        case RT_RESHAPE:
        {
            npu_reshape_param_t* param = (npu_reshape_param_t*) nd->param;

            /* prepare input tensor info */
            // param->input_tensor = (tensor_param_t**)malloc(sizeof(tensor_param_t) * nd->input_num) ;
            rt_tensor_t* t = NULL ;
            t = get_rt_graph_tensor(nd->graph, nd->input_tensors[0]) ;
            param->input_tensor = (tensor_param_t*)malloc(sizeof(tensor_param_t)) ;
             param->output_tensor = (tensor_param_t*)malloc(sizeof(tensor_param_t)) ;
            param->input_tensor = t->param ;
            t = get_rt_graph_tensor(nd->graph, nd->output_tensors[0]) ;
            param->output_tensor = t->param ;
            log_v ("reshape: node id: %d out addr: %x\n", nd->index, t->param->addr) ;

            npu_reshape_node(param);
            update_ready_nodes (sn) ;
            update_ts_usage (sn) ;
            break;
        }
        case RT_SIGMOID:
        {
            npu_sigmoid_param_t* param = (npu_sigmoid_param_t*) nd->param;

            /* prepare input tensor info */
            // param->input_tensor = (tensor_param_t**)malloc(sizeof(tensor_param_t) * nd->input_num) ;
            rt_tensor_t* t = NULL ;
            t = get_rt_graph_tensor(nd->graph, nd->input_tensors[0]) ;
            param->input_tensor = t->param ;
            t = get_rt_graph_tensor(nd->graph, nd->output_tensors[0]) ;
            param->output_tensor = t->param ;
            log_v ("sigmoid: node id: %d out addr: %x\n", nd->index, t->param->addr) ;

            npu_sigmoid_node(param);
            update_ready_nodes (sn) ;
            update_ts_usage (sn) ;
            break;
        }
        case RT_YOLO:
        {
            yolo_pkg_info_t** param = (yolo_pkg_info_t**) nd->param ;

            rt_tensor_t* out_t = get_rt_graph_tensor(nd->graph, nd->output_tensors[0]) ;

            rt_tensor_t* t = NULL ;
            for (unsigned int i = 0; i < nd->input_num; i++) {
                t = get_rt_graph_tensor(nd->graph, nd->input_tensors[i]) ;
                param[i]->ifm0_base_addr = t->param->addr ;
                param[i]->ofm_base_addr = out_t->param->addr ;
            }

            if (put_to_fifo (sn)) {
                log_e("put_to_fifo failed\n");
                return -1;
            }

            break ;
        }
        default:
        {
            log_e("unsupported rt_node type %d \n", node_type) ;
        }
    }

    return 0 ;
}

/* function declaration used in find_ready_nodes_for_pg */
AL_S8 find_ready_nodes_for_sl (scheduler_node_t* sn) ;

/* find the basic nodes under the pg that are ready to run */
AL_S8 find_ready_nodes_for_pg (scheduler_node_t* sn) {
    if (sn == NULL) {
        log_e("input sn is NULL\n");
        return -1;
    }
    if (sn->node_type != RT_S_PARALLEL) {
        log_e("the scheduler node type is not parallel group for find_ready_nodes_for_pg: %d\n", sn->node_type) ;
        return -1 ;
    }
    for (AL_S8 i = 0; i < sn->pg->node_num; i++) {
        scheduler_node_t* sn_sl = get_rt_graph_scheduler_node(sn->graph, sn->pg->nodes[i]) ;
        if (sn_sl == NULL) {
            log_e("sn_sl is NULL\n");
            return -1;
        }
        /* the children of parallel group are sequential lists */
        if (sn_sl->node_type != RT_S_SEQUENTIAL) {
            log_e("the child i: %d of parallel group is not sequential list type: %d", i, sn_sl->node_type) ;
            return -1 ;
        }

        /* recursively call find_ready_nodes_for_sl */
        if (find_ready_nodes_for_sl (sn_sl)) {
            log_e("find_ready_nodes_for_sl failed in find_ready_nodes_for_pg\n");
            return -1;
        }
    }
    return 0 ;
}

/* find the basic node under the sl that is ready to run */
AL_S8 find_ready_nodes_for_sl (scheduler_node_t* sn) {
    if (sn == NULL) {
        log_e("input sn is NULL\n");
        return -1;
    }
    if (sn->node_type != RT_S_SEQUENTIAL) {
        log_e("the scheduler node type is not sequential list for find_ready_nodes_for_sl: %d\n", sn->node_type) ;
        return -1 ;
    }

    /* get the first child node in the sequential list */
    scheduler_node_t* first_child = get_rt_graph_scheduler_node(sn->graph, sn->sl->nodes[0]) ;
    if (first_child == NULL) {
        log_e("the first child of sl is NULL\n");
        return -1;
    }

    /* the children of sequential list can be basic node and parallel group */
    if (first_child->node_type == RT_S_BASIC) {
        if (prepare_basic_node_to_fifo (first_child)) {
            log_e("prepare_basic_node_to_fifo failed\n");
            return -1;
        }
    } else if (first_child->node_type == RT_S_PARALLEL && first_child->pg->is_split == 1) {
        if (prepare_split_node_to_fifo (first_child)) {
            log_e("prepare_split_node_to_fifo failed\n");
            return -1;
        }
    } else if (first_child->node_type == RT_S_PARALLEL) {
        /* recursively call find_ready_nodes_for_pg */
        if (find_ready_nodes_for_pg (first_child)) {
            log_e("find_ready_nodes_for_pg failed\n");
            return -1;
        }
    } else {
        log_e("unsupported node type for the first child of sequential list: %d\n", first_child->node_type) ;
        return -1 ;
    }
    return 0 ;
}

/* the scheduler node finished, find new ready nodes */
AL_S8 update_ready_nodes (scheduler_node_t* sn) {

    /* find the parent id of the finished scheduler node */
    AL_S16 parent_id = -2 ;
    if (sn->node_type == RT_S_BASIC) {
        parent_id = sn->nd->parent ;
    } else if (sn->node_type == RT_S_SEQUENTIAL) {
        parent_id = sn->sl->parent ;
    } else if (sn->node_type == RT_S_PARALLEL) {
        parent_id = sn->pg->parent ;
    } else {
        log_e("unsupported scheduler node type: %d\n", sn->node_type) ;
        return -1 ;
    }

    /* the parent id of the root node is -1 */
    if (parent_id == -1) {
        log_i("meet the root scheduler node\n") ;
        return 0 ;
    }

    /* get the parent scheduler node */
    scheduler_node_t* parent = get_rt_graph_scheduler_node(sn->graph, parent_id) ;

    if (parent == NULL) {
        log_e("find parent failed\n") ;
        return -1 ;
    } else if (parent->node_type == RT_S_SEQUENTIAL) {

        /* the finished num of the children nodes increase 1*/
        sched_nd_rec[parent->index]++ ;

        if (sched_nd_rec[parent->index] == parent->sl->node_num) {

            /* if all the children are finished, the parent is finished */
            if (update_ready_nodes (parent)) {
                log_e("update_ready_nodes for sl parent failed\n");
                return -1;
            }

        } else {

            /* find the node after the finished node in the sequential list */
            scheduler_node_t* child = get_rt_graph_scheduler_node(parent->graph, parent->sl->nodes[sched_nd_rec[parent->index]]) ;

            if (child->node_type == RT_S_BASIC ) {
                if (prepare_basic_node_to_fifo (child)) {
                    log_e("prepare_basic_node_to_fifo failed\n");
                    return -1;
                }
            } else if (child->node_type == RT_S_PARALLEL && child->pg->is_split == 1) {
                if (prepare_split_node_to_fifo (child)) {
                    log_e("prepare_split_node_to_fifo failed\n");
                    return -1;
                }
            } else if (child->node_type == RT_S_PARALLEL) {
                if (find_ready_nodes_for_pg (child)) {
                    log_e("find_ready_nodes_for_pg failed\n");
                    return -1;
                }
            } else {
                log_e("unsupported scheduler type as the child of sequential list: %d\n", child->node_type) ;
                return -1 ;
            }

        }

    } else if (parent->node_type == RT_S_PARALLEL) {

        /* the finished num of the children nodes increase 1*/
        sched_nd_rec[parent->index]++ ;

        if (sched_nd_rec[parent->index] == parent->pg->node_num) {

            /* if all the children are finished, the parent is finished */
            if (update_ready_nodes (parent)) {
                log_e("update_ready_nodes for pg parent failed\n");
                return -1;
            }

        }

    } else {

        log_e("unexpected parent type: %d, snode_id: %d parent_id: %d\n", parent->node_type, sn->index, parent_id) ;
        return -1;

    }

    return 0 ;
}

/* ------------------------------- end of update_ready_nodes related functions -------------------------------------------------------------------- */



/* ------------------------------- run_scheduler_node related functions -------------------------------------------------------------------- */

/* update output tensor addr, then config the basic node on hardware  */
AL_S8 run_basic_node (rt_node_t* nd) {
    AL_U16 node_type = nd->node_type ;

    switch (node_type) {
        case RT_NPU_HARD:
        {
            hard_npu_pkg_info_t* param = (hard_npu_pkg_info_t*) nd->param ;

            // printf("\nrun npu_hard_node\n") ;
            if (config_dma_mode) {
                run_npu_hard_dma_mode (param) ;
            } else {
                run_npu_hard_normal_mode (param) ;
            }


            break ;
        }
        case RT_POOLING:
        case RT_CONCAT:
        case RT_UPSAMPLE:
        case RT_PRELU:
        case RT_INTERP:
        case RT_ELTWISE:
        {
            soft_npu_pkg_info_t* param = (soft_npu_pkg_info_t*) nd->param ;

            // printf("\nrun npu_soft_node\n") ;
            if (config_dma_mode) {
                run_npu_soft_dma_mode (param, nd) ;
            } else {
                run_npu_soft_normal_mode (param, nd) ;
            }

            break ;
        }
        case RT_INPUT:
        {
            // rt_tensor_t* t = get_rt_graph_tensor(nd->graph, nd->output_tensors[0]) ;
            // if (run_dump_tensor == 1) {
            //     dump_tensor (t) ;
            // }

            break ;
        }
        case RT_YOLO:
        {
            yolo_pkg_info_t** param = (yolo_pkg_info_t**) nd->param ;

            // printf("\nrun npu_yolov_node\n") ;
            // npu_yolov_node (param) ;
            yolo_id = 0 ;
            if (config_dma_mode) {
                run_npu_soft_yolo_dma_mode(param[yolo_id++]) ;
            } else {
                run_npu_soft_yolo_normal_mode(param[yolo_id++]) ;
            }

            break ;
        }
        default:
        {
            log_e("unsupported rt_node type %d \n", node_type) ;
            return -1;
        }
    }
    return 0;
}

/* config the scheduler node on the hardware */
AL_S8 run_scheduler_node (scheduler_node_t* sn) {
    if (sn->node_type == RT_S_BASIC) {
        rt_node_t* rt_nd = get_rt_graph_node (sn->graph, sn->nd->index) ;

        /* config the rt_node of the basic scheduler on hardware */
        // printf("\nnode id: %d\n", rt_nd->index) ;
        if (run_basic_node (rt_nd)) {
            log_e("run_basic_node failed\n");
            return -1;
        }

    } else {
        log_e("unsupported scheduler node type to run: %d\n", sn->node_type) ;
        return -1 ;
    }

    return 0 ;
}

/* ------------------------------- end of run_scheduler_node related functions -------------------------------------------------------------------- */

/* ------------------------------- scheduler main function -------------------------------------------------------------------- */

// Tips for future work: 
// 1. if more hard npu available, need to assign the ready nodes according to the hardware resource available. 
// 2. for more advanced scheduler, perform prediction feature:
//      assign the ready nodes according to the hardware resource available and running time needed by the ready nodes.
// 3. in multi-thread or multi-process system, make sure update_ready_nodes is thread safe.
// 4. queue implemented by fifo may change to queue implemented by linked list.
AL_S8 run_npu_graph (rt_graph_t* graph) {

    /* reset the num of children that are finished */
    memset(sched_nd_rec, 0, sizeof(AL_S16) * graph->scheduler_node_num) ;

    hard_finish_cnt = 0;
    soft_finish_cnt = 0;

    /* find the first ready node*/
    scheduler_node_t* entry_nd = graph->scheduler_node_list[0] ;
    if (entry_nd->node_type == RT_S_PARALLEL) {
        find_ready_nodes_for_pg (entry_nd) ;
    } else {
        log_e("entry node of rt_graph should be parallel type, but actully is: %d\n", entry_nd->node_type) ;
        return -1 ;
    }

    /* run the first ready node. the entry node of a net, must be soft node, more specific, input node.*/
    if (fifo_item_num(soft_ready_nodes) > 0) {
        any_data_t out_data = pop_fifo_data(soft_ready_nodes) ;
        scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
        push_fifo_data (soft_running_nodes, out_data) ;
        // run node
        if (run_scheduler_node (sn)) {
            log_e("run scheduler node failed: %d\n", sn->index) ;
            return -1 ;
        }
        pop_fifo_data(soft_running_nodes) ;

        /* input node is not implemented in npu, special process here */
        if (update_ts_usage (sn)) {
            log_e("update_ts_usage failed\n");
            return -1;
        }
        if (update_ready_nodes (sn)) {
            log_e("update_ready_nodes failed\n");
            return -1;
        }
        // assign nodes to npu
        if ((fifo_item_num(soft_running_nodes) == 0) && (fifo_item_num(soft_ready_nodes) > 0)) {
            any_data_t out_data = pop_fifo_data(soft_ready_nodes) ;
            scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
            push_fifo_data (soft_running_nodes, out_data) ;
            if (run_scheduler_node (sn)) {
                log_e("run_scheduler_node failed\n");
                return -1;
            }
        }
        if ((fifo_item_num(hard_running_nodes) == 0) && (fifo_item_num(hard_ready_nodes) > 0)) {
            any_data_t out_data = pop_fifo_data(hard_ready_nodes) ;
            scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
            push_fifo_data (hard_running_nodes, out_data) ;
            if (run_scheduler_node (sn)) {
                log_e("run_scheduler_node failed\n");
                return -1;
            }
        }

        if (npu_lock_init()) {
            log_e("npu_lock_init failed\n");
            return -1;
        }

        if (run_main()) {
            log_e("run main failed\n");
            return -1;
        }

        if (npu_lock_destroy()) {
            log_e("npu_lock_destroy failed\n");
            return -1;
        }

    } else {
        log_e("the first node of the rt_graph is not soft node\n") ;
        return -1 ;
    }

    return 0 ;
}

/* ------------------------------- end of scheduler main function -------------------------------------------------------------------- */
