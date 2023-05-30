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

#ifdef RUN_PLATFORM_LINUX
#include <fcntl.h>
#include <sys/mman.h>

#include "hard_npu_uio.h"
#include "soft_npu_uio.h"
#endif

#include "uio_npu_driver.h"

#ifdef RUN_PLATFORM_BM
#include "ff.h"
#include "npu_arm_op.h"
#include "driver_api.h"
#include "new_op_func.h"

#endif

#include "npu_util_test.h"

/* ------------------------------- global variables -------------------------------------------------------------------- */

volatile int16_t* ts_usage_rec = NULL ; // record the consumer number of allocated tensors, it is initialized in init process.
volatile int16_t* sched_nd_rec = NULL ; // record the children visited status of scheduler nodes, it is initialized in init process.
volatile uint64_t* ts_vaddr_rec = NULL ; // record the virtual addr of allocated tensors, it is initialized in init process.

/* to meet the needs of scheduler, the fifo may need to be changed to linked list */
fifo_t* hard_ready_nodes = NULL ; // store the hard basic nodes which are ready to run, it is initialized in init process.
fifo_t* soft_ready_nodes = NULL ; // store the soft basic nodes which are ready to run, it is initialized in init process.
fifo_t* hard_running_nodes = NULL ; // store the hard basic nodes which are running, it is initialized in init process.
fifo_t* soft_running_nodes = NULL ; // store the soft basic nodes which are running, it is initialized in init process.

/* the following variables are the flags of soft node irq and hard node irq */
volatile int8_t soft_irq = 0 ; // 0: no soft node interrupt, 1: soft node interrupt triggered.
volatile int8_t hard_irq = 0 ; // 0: no hard node interrupt, 1: hard node interrupt triggered.
volatile uint32_t info = 1; /* unmask */
volatile int hard_irq_fd = -1;
volatile int soft_irq_fd = -1;
volatile int hard_node_num = 0 ;
volatile int soft_node_num = 0 ;

/* pthread_mutex_t not support by BM */
#ifndef RUN_PLATFORM_BM
/* the following variables are for mutex of multi-thread */
pthread_mutex_t m_mutex;
#endif
/* the following variables are used for yolo */
uint64_t res_addr = 0 ; // record the yolo result addr, can be obtained directly from graph
int8_t yolo_id = 0 ; // record how many input feature maps have been sent to hardware
uint8_t is_yolo_5s = 0 ; // record whether the yolo node is yolo_5s

/* the following variables are used for debug */
uint8_t run_check_tensor = 0 ; // 0: do nothing, 1: check tensor in memory one by one, the start addr of the golden value is 0x40000000
uint8_t run_dump_tensor = 1 ; // 0: do nothing, 1: dump tensor in sd card as bin files
uint8_t print_ts_info = 0 ; // 0: do nothing, 1: print tensor id/name/addr at running time

int32_t hard_finish_cnt = 0;
int32_t soft_finish_cnt = 0;

/* define the platform to run */
uint8_t run_on_os = 1 ;
uint8_t config_dma_mode = 1 ;

/* define the input/output data process mode */
// 0: INOUT_DATA_MODE_DDR, 1: INOUT_DATA_MODE_SD_ARM, 2:INOUT_DATA_MODE_SD_JPU
int8_t inout_data_mode = 1 ; // default is 0.

/* for draw yolo result */
char* input_pic_fn = NULL ;
int16_t img_w = 0 ;
int16_t img_h = 0 ;

volatile int8_t preprocess_finish_flag = 0;

/* ------------------------------- end of global variables -------------------------------------------------------------------- */



/* ------------------------------- util functions -------------------------------------------------------------------- */

/* check_tensor is used to check tensor content in ddr, compared with golden values stored at addr 0x40000000 */
int8_t check_tensor (rt_tensor_t* t) {
    uint32_t t_addr = t->param->addr ;
    uint32_t test_addr = 0x10000000 + t_addr ;
    uint32_t golden_addr = 0x40000000 + t_addr ;
    int8_t* test_data = (int8_t*) test_addr ;
    int8_t* golden_data = (int8_t*) golden_addr ;

    int32_t t_size = 1 ;
    for (int i = 0; i < t->param->dim_num; i++) {
        t_size *= t->param->dims[i] ;
    }

    int8_t pass = 1 ;
    for (int i = 0; i < t_size; ++i) {
        if (*(test_data + i) != *(golden_data + i)) {
            printf("Error: check %dth data in tensor %d failed, res: %d golden: %d\n",i, t->index, *(test_data + i), *(golden_data + i)) ;
            pass = 0 ;
            break ;
        }
    }

    if (pass == 1) {
        printf("check tensor %d pass!\n", t->index) ;
        return 1 ;
    } else if (pass == 0) {
        printf("check tensor %d failed!\n", t->index) ;
        return 0 ;
    } else {
        printf("unexpected pass value: %d\n", pass) ;
        return -1 ;
    }
}

/* after the scheduler node is finished, update the usage of the input tensors, free them if necessary */
int8_t update_ts_usage (scheduler_node_t* sn) {

    if (sn->node_type != RT_S_BASIC) {
        printf ("the scheduler node to upate ts usage must be basic node, but actually is: %d\n", sn->node_type) ;
        return -1 ;
    }

    /* update tensor usage and free tensor mem if all the consumer are executed */
    rt_node_t* rt_nd = get_rt_graph_node (sn->graph, sn->nd->index) ;
    for (unsigned int j = 0; j < rt_nd->input_num; j++) {
        rt_tensor_t* rt_ts = get_rt_graph_tensor (rt_nd->graph, rt_nd->input_tensors[j]) ;
        if (rt_ts->tensor_type == RT_TENSOR_VAR) {
            --ts_usage_rec[rt_ts->index] ;
            if (ts_usage_rec[rt_ts->index] == 0) {
                // free_mem(rt_ts)
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
int8_t update_ready_nodes (scheduler_node_t* sn) ;
int8_t run_scheduler_node (scheduler_node_t* sn) ;

/* soft node irq handler */
void soft_node_irq_handler(void) {

    // printf("entering soft node ISR\n") ;

    /* special process for yuv2rgb */
    uint32_t res = soft_npu_read(SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR) ;
    uint32_t mask = 0x01 ;
    res &= mask ;
    if (0x01 == res) {
        rgb_postprocess();
        preprocess_finish_flag = 1;
        soft_npu_write(SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR, 0x00); // select device
        soft_npu_write(SOFTNPU__SOFT_SHARE__CLEAR_INTERRUPT__ADDR, 0x01) ;
        return 0;
    }


    /* set soft_irq flag */
    soft_irq = 1 ;

    /* clear interrupt */

#ifdef RUN_PLATFORM_LINUX

    size_t nb = write(soft_irq_fd, &info, sizeof(info));
    if (nb != (size_t)sizeof(info)) {
        printf("write soft_npu failed\n");
        return;
    }

#endif

    soft_npu_write(SOFTNPU__SOFT_SHARE__CLEAR_INTERRUPT__ADDR, 0x1) ;

}

int8_t handle_soft_node_irq () {

    /* special process for yolo node */

    /* read yolo switch signal */
    uint32_t res = soft_npu_read(SOFTNPU__SOFT_SHARE__CFG_INIT_ADDR__ADDR) ;
    uint32_t mask = 0x800 ;
    res &= mask ;

#ifdef RUN_PLATFORM_LINUX

    pthread_mutex_lock(&m_mutex);

#endif

    if (res == 0x800) {
        /* the soft node is yolo */
        /* read yolo layer num */
        uint32_t layer_num = soft_npu_read(SOFTNPU__SOFT_YOLO__YOLO_C__ADDR) ;
        layer_num = layer_num >> 16 ;
        mask = 0x3 ;
        layer_num &= mask ;

        if (yolo_id < layer_num) {
            any_data_t front_node = get_fifo_data(soft_running_nodes, soft_running_nodes->rdptr) ;
            scheduler_node_t* front_sn = (scheduler_node_t*) front_node.pointer ;
            rt_node_t* yolo_node = get_rt_graph_node (front_sn->graph, front_sn->nd->index) ;
            if (yolo_node->node_type != RT_YOLO) {
                printf("get yolo node failed\n") ;
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
            update_ts_usage (sn) ;
            update_ready_nodes (sn) ;

            /* free input tensors in param */
            rt_node_t* nd = get_rt_graph_node (sn->graph, sn->nd->index) ;
            npu_yolov_param_t* param = (npu_yolov_param_t*) nd->param ;
            free (param->input_tensors) ;

            /* assign nodes to npu */
            if ((fifo_item_num(soft_running_nodes) == 0) && (fifo_item_num(soft_ready_nodes) > 0)) {
                any_data_t out_data = pop_fifo_data(soft_ready_nodes) ;
                scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
                push_fifo_data (soft_running_nodes, out_data) ;
                run_scheduler_node (sn) ;
            }
            if ((fifo_item_num(hard_running_nodes) == 0) && (fifo_item_num(hard_ready_nodes) > 0)) {
                any_data_t out_data = pop_fifo_data(hard_ready_nodes) ;
                scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
                push_fifo_data (hard_running_nodes, out_data) ;
                run_scheduler_node (sn) ;
            }
        }
    } else {
        any_data_t out_data = pop_fifo_data(soft_running_nodes) ;
        scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
        update_ts_usage (sn) ;
        update_ready_nodes (sn) ;


        /* for debug */
        rt_node_t* rt_nd = get_rt_graph_node (sn->graph, sn->nd->index) ;
        rt_tensor_t* t_output = get_rt_graph_tensor (rt_nd->graph, rt_nd->output_tensors[0]) ;

        if (run_dump_tensor == 1) {
            dump_tensor (t_output) ;
        }
        if (run_check_tensor == 1) {
            if (check_tensor (t_output) != 1) {
                printf ("Error: check tensor failed\n") ;
                // return -1 ;
            }
        }
        if (print_ts_info == 1) {
            printf ("tensor id is: %d\n", t_output->index) ;
            printf ("tensor name is: %s\n", t_output->name) ;
            printf ("tensor addr is: %x\n", t_output->param->addr) ;
        }

        /* assign nodes to npu */
        if ((fifo_item_num(soft_running_nodes) == 0) && (fifo_item_num(soft_ready_nodes) > 0)) {
            any_data_t out_data = pop_fifo_data(soft_ready_nodes) ;
            scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
            push_fifo_data (soft_running_nodes, out_data) ;
            run_scheduler_node (sn) ;
        }
        if ((fifo_item_num(hard_running_nodes) == 0) && (fifo_item_num(hard_ready_nodes) > 0)) {
            any_data_t out_data = pop_fifo_data(hard_ready_nodes) ;
            scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
            push_fifo_data (hard_running_nodes, out_data) ;
            run_scheduler_node (sn) ;
        }
    }

#ifdef RUN_PLATFORM_LINUX

    pthread_mutex_unlock(&m_mutex);

#endif

    return 0 ;
}

/* hard node irq handler */
void hard_node_irq_handler(void) {
    // printf("entering hard node ISR\n") ;

    /* set hard_irq flag */
    hard_irq = 1 ;

    /* clear interrupt */

#ifdef RUN_PLATFORM_LINUX

    size_t nb = write(hard_irq_fd, &info, sizeof(info));
    if (nb != (size_t)sizeof(info)) {
        printf("write hard_npu failed\n");
        return;
    }

#endif

    hard_npu_write(HARDNPU__CCU__CLEAR_INTERRUPT__ADDR, 0x1) ;
}

int8_t handle_hard_node_irq () {

    any_data_t out_data = pop_fifo_data(hard_running_nodes) ;
    scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;

#ifdef RUN_PLATFORM_LINUX

    pthread_mutex_lock(&m_mutex);

#endif

    update_ts_usage (sn) ;
    update_ready_nodes (sn) ;

#ifdef RUN_PLATFORM_LINUX

    pthread_mutex_unlock(&m_mutex);

#endif

    /* for debug */
    rt_node_t* rt_nd = get_rt_graph_node (sn->graph, sn->nd->index) ;
    rt_tensor_t* t_output = get_rt_graph_tensor (rt_nd->graph, rt_nd->output_tensors[0]) ;

    /* check for split node */
    if (((t_output->p_split_num == 0) && !rt_nd->is_sub_nd) || ((t_output->p_split_num > 0) && (rt_nd->sub_nd_id + 1) == (t_output->p_sp_params[0].sp_c_num * t_output->p_sp_params[0].sp_h_num))) {
        if (run_dump_tensor == 1) {
            dump_tensor (t_output) ;
        }
        if (run_check_tensor == 1) {
            if (check_tensor (t_output) != 1) {
                printf ("Error: check tensor failed\n") ;
                // return -1 ;
            }
        }
        if (print_ts_info == 1) {
            printf ("tensor id is: %d\n", t_output->index) ;
            printf ("tensor name is: %s\n", t_output->name) ;
            printf ("tensor addr is: %x\n", t_output->param->addr) ;
        }
    }

#ifdef RUN_PLATFORM_LINUX

    pthread_mutex_lock(&m_mutex);

#endif

    /* assign nodes to npu */
    if ((fifo_item_num(soft_running_nodes) == 0) && (fifo_item_num(soft_ready_nodes) > 0)) {
        any_data_t out_data = pop_fifo_data(soft_ready_nodes) ;
        scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
        push_fifo_data (soft_running_nodes, out_data) ;
        run_scheduler_node (sn) ;
    }
    if ((fifo_item_num(hard_running_nodes) == 0) && (fifo_item_num(hard_ready_nodes) > 0)) {
        any_data_t out_data = pop_fifo_data(hard_ready_nodes) ;
        scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
        push_fifo_data (hard_running_nodes, out_data) ;
        run_scheduler_node (sn) ;
    }

#ifdef RUN_PLATFORM_LINUX

    pthread_mutex_unlock(&m_mutex);

#endif

    return 0 ;
}

/* ------------------------------- end of irq handler functions -------------------------------------------------------------------- */



/* ------------------------------- update_ready_nodes related functions -------------------------------------------------------------------- */

/* put scheduler node to hard_ready_nodes fifo or soft_ready_nodes fifo according to the rt_node type in the scheduler node */
int8_t put_to_fifo (scheduler_node_t* sn) {

    rt_node_t* rt_nd = get_rt_graph_node (sn->graph, sn->nd->index) ;
    if (rt_nd->node_type == RT_NPU_HARD) {
        if (fifo_free_room(hard_ready_nodes) < 1) {
            printf("hard ready nodes fifo is full, and the free room is: %d\n", fifo_free_room(hard_ready_nodes)) ;
            return -1 ;
        }
        any_data_t in_data;
        in_data.pointer = (void*) sn ;
        push_fifo_data (hard_ready_nodes, in_data) ;
    } else {
        if (fifo_free_room(soft_ready_nodes) < 1) {
            printf("soft ready nodes fifo is full, and the free room is: %d\n", fifo_free_room(soft_ready_nodes)) ;
            return -1 ;
        }
        any_data_t in_data;
        in_data.pointer = (void*) sn ;
        push_fifo_data (soft_ready_nodes, in_data) ;
    }

    return 0;
}

/* a group of split hard_node is ready to run, the params of the split hard node need to preprocess */
int8_t prepare_split_node_to_fifo (scheduler_node_t* sn) {
    // this kind of parallel group is generated by me and has a fixed pattern.
    // temporary solution for 1 npu

    /* prepare split parameters */
    scheduler_node_t* child_sl = get_rt_graph_scheduler_node(sn->graph, sn->pg->nodes[0]) ;
    scheduler_node_t* child_bn = get_rt_graph_scheduler_node(child_sl->graph, child_sl->sl->nodes[0]) ;
    rt_node_t* rt_nd = get_rt_graph_node(child_bn->graph, child_bn->nd->index) ;
    rt_tensor_t* t_input = get_rt_graph_tensor (rt_nd->graph, rt_nd->input_tensors[0]) ;
    uint8_t c_sp_idx = rt_nd->c_sp_idx ;
    uint8_t sp_c_num = t_input->c_sp_params[c_sp_idx].sp_c_num ;
    uint8_t sp_h_num = t_input->c_sp_params[c_sp_idx].sp_h_num ;
    uint8_t ic = 0 ;
    uint8_t ih = 0 ;
    uint8_t load_fm = 1 ;
    uint8_t load_flt = 1 ;

    /* 1> check whether input tensors are allocated */
    for (unsigned int j = 0; j < rt_nd->input_num; j++) {
        rt_tensor_t* rt_ts = get_rt_graph_tensor (rt_nd->graph, rt_nd->input_tensors[j]) ;
        if (!(rt_ts->allocated)) {
            printf("\nError: input tensor: %d of rt_nd: %d is not allocated\n", j, rt_nd->index) ;
            return -1 ;
        }
    }

    /* 2> if the output tensor is not allocated yet, allocate the output tensor */
    for (unsigned int j = 0; j < rt_nd->output_num; j++) {
        rt_tensor_t* rt_ts = get_rt_graph_tensor (rt_nd->graph, rt_nd->output_tensors[j]) ;
        if(!(rt_ts->allocated)) {
            if (1 == run_on_os) {
                if (rt_nd->node_type == RT_YOLO) {
                    // alloc_mem_linux (rt_ts, 8) ;
                    alloc_mem_buddy(rt_ts) ;
                } else {
                    // alloc_mem_linux (rt_ts, 1) ;
                    alloc_mem_buddy(rt_ts) ;
                }
                // alloc_mem_os (rt_ts) ;
            } else {
                if (rt_nd->node_type == RT_YOLO) {
                    alloc_mem (rt_ts, 8) ;
                } else {
                    alloc_mem (rt_ts, 1) ;
                }
            }
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
        put_to_fifo(child_bn) ;
    }

    return 0 ;
}

/* a basic node is ready to run, the param of the basic node need to preprocess */
int8_t prepare_basic_node_to_fifo (scheduler_node_t* sn) {

    rt_node_t* nd = get_rt_graph_node (sn->graph, sn->nd->index) ;

    uint16_t node_type = nd->node_type ;

    if (node_type == RT_INPUT) {
        put_to_fifo (sn) ;
        return 0 ;
    }

    /* 1> check whether input tensors are allocated */
    for (unsigned int j = 0; j < nd->input_num; j++) {
        rt_tensor_t* rt_ts = get_rt_graph_tensor (nd->graph, nd->input_tensors[j]) ;
        if (!(rt_ts->allocated)) {
            printf("\nError: input tensor: %d of rt_nd: %d is not allocated\n", j, nd->index) ;
            return -1 ;
        }
    }

    /* 2> if the output tensor is not allocated yet, allocate the output tensor */
    for (unsigned int j = 0; j < nd->output_num; j++) {
        rt_tensor_t* rt_ts = get_rt_graph_tensor (nd->graph, nd->output_tensors[j]) ;
        if(!(rt_ts->allocated)) {
            if (1 == run_on_os) {
                if (nd->node_type == RT_YOLO) {
                    // alloc_mem_linux (rt_ts, 8) ;
                    alloc_mem_buddy(rt_ts) ;
                } else {
                    // alloc_mem_linux (rt_ts, 1) ;
                    alloc_mem_buddy(rt_ts) ;
                }
                // alloc_mem_os (rt_ts) ;
            } else {
                if (nd->node_type == RT_YOLO) {
                    alloc_mem (rt_ts, 8) ;
                } else {
                    alloc_mem (rt_ts, 1) ;
                }
            }
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

            put_to_fifo (sn) ;

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
                        printf("the input_num is more than 4 of node: %s, idx: %u\n", nd->name, i) ;
                        break ;
                    }
                }
            }

            put_to_fifo (sn) ;

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

            printf ("dwconv: node id: %d out addr: %x\n", nd->index, t->param->addr) ;

            npu_dwconv_node(param);
            dump_tensor(t);

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
            printf ("reshape: node id: %d out addr: %x\n", nd->index, t->param->addr) ;

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
            printf ("sigmoid: node id: %d out addr: %x\n", nd->index, t->param->addr) ;

            npu_sigmoid_node(param);
            update_ready_nodes (sn) ;
            update_ts_usage (sn) ;
            break;
        }
        case RT_INPUT:
        {
            // npu_input_param_t* param = (npu_input_param_t*) nd->param ;

            // /* prepare output tensor info */
            // rt_tensor_t* t = get_rt_graph_tensor(nd->graph, nd->output_tensors[0]) ;
            // param->output_tensor = t->param ;
            // param->output_tensor->addr = 0 ;

            put_to_fifo (sn) ;

            break ;
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

            // is_yolo_5s = param->is_yolo_5s ;

            put_to_fifo (sn) ;

            break ;
        }
        default:
        {
            printf("unsupported rt_node type %d \n", node_type) ;
        }
    }

    return 0 ;
}

/* function declaration used in find_ready_nodes_for_pg */
int8_t find_ready_nodes_for_sl (scheduler_node_t* sn) ;

/* find the basic nodes under the pg that are ready to run */
int8_t find_ready_nodes_for_pg (scheduler_node_t* sn) {

    if (sn->node_type != RT_S_PARALLEL) {
        printf("the scheduler node type is not parallel group for find_ready_nodes_for_pg: %d\n", sn->node_type) ;
        return -1 ;
    }
    for (int8_t i = 0; i < sn->pg->node_num; i++) {
        scheduler_node_t* sn_sl = get_rt_graph_scheduler_node(sn->graph, sn->pg->nodes[i]) ;

        /* the children of parallel group are sequential lists */
        if (sn_sl->node_type != RT_S_SEQUENTIAL) {
            printf("the child i: %d of parallel group is not sequential list type: %d", i, sn_sl->node_type) ;
            return -1 ;
        }

        /* recursively call find_ready_nodes_for_sl */
        find_ready_nodes_for_sl (sn_sl) ;
    }
    return 0 ;
}

/* find the basic node under the sl that is ready to run */
int8_t find_ready_nodes_for_sl (scheduler_node_t* sn) {

    if (sn->node_type != RT_S_SEQUENTIAL) {
        printf("the scheduler node type is not sequential list for find_ready_nodes_for_sl: %d\n", sn->node_type) ;
        return -1 ;
    }

    /* get the first child node in the sequential list */
    scheduler_node_t* first_child = get_rt_graph_scheduler_node(sn->graph, sn->sl->nodes[0]) ;

    /* the children of sequential list can be basic node and parallel group */
    if (first_child->node_type == RT_S_BASIC) {
        prepare_basic_node_to_fifo (first_child) ;
    } else if (first_child->node_type == RT_S_PARALLEL && first_child->pg->is_split == 1) {
        prepare_split_node_to_fifo (first_child) ;
    } else if (first_child->node_type == RT_S_PARALLEL) {
        /* recursively call find_ready_nodes_for_pg */
        find_ready_nodes_for_pg (first_child) ;
    } else {
        printf("unsupported node type for the first child of sequential list: %d\n", first_child->node_type) ;
        return -1 ;
    }
    return 0 ;
}

/* the scheduler node finished, find new ready nodes */
int8_t update_ready_nodes (scheduler_node_t* sn) {

    /* find the parent id of the finished scheduler node */
    int16_t parent_id = -2 ;
    if (sn->node_type == RT_S_BASIC) {
        parent_id = sn->nd->parent ;
    } else if (sn->node_type == RT_S_SEQUENTIAL) {
        parent_id = sn->sl->parent ;
    } else if (sn->node_type == RT_S_PARALLEL) {
        parent_id = sn->pg->parent ;
    } else {
        printf("unsupported scheduler node type: %d\n", sn->node_type) ;
        return -1 ;
    }

    /* the parent id of the root node is -1 */
    if (parent_id == -1) {
        printf("meet the root scheduler node\n") ;
        return 0 ;
    }

    /* get the parent scheduler node */
    scheduler_node_t* parent = get_rt_graph_scheduler_node(sn->graph, parent_id) ;

    if (parent == NULL) {

        printf ("find parent failed\n") ;
        return -1 ;

    } else if (parent->node_type == RT_S_SEQUENTIAL) {

        /* the finished num of the children nodes increase 1*/
        sched_nd_rec[parent->index]++ ;

        if (sched_nd_rec[parent->index] == parent->sl->node_num) {

            /* if all the children are finished, the parent is finished */
            update_ready_nodes (parent) ;

        } else {

            /* find the node after the finished node in the sequential list */
            scheduler_node_t* child = get_rt_graph_scheduler_node(parent->graph, parent->sl->nodes[sched_nd_rec[parent->index]]) ;

            if (child->node_type == RT_S_BASIC ) {
                prepare_basic_node_to_fifo (child) ;
            } else if (child->node_type == RT_S_PARALLEL && child->pg->is_split == 1) {
                prepare_split_node_to_fifo (child) ;
            } else if (child->node_type == RT_S_PARALLEL) {
                find_ready_nodes_for_pg (child) ;
            } else {
                printf("unsupported scheduler type as the child of sequential list: %d\n", child->node_type) ;
                return -1 ;
            }

        }

    } else if (parent->node_type == RT_S_PARALLEL) {

        /* the finished num of the children nodes increase 1*/
        sched_nd_rec[parent->index]++ ;

        if (sched_nd_rec[parent->index] == parent->pg->node_num) {

            /* if all the children are finished, the parent is finished */
            update_ready_nodes (parent) ;

        }

    } else {

        printf ("unexpected parent type: %d, snode_id: %d parent_id: %d\n", parent->node_type, sn->index, parent_id) ;

    }

    return 0 ;
}

/* ------------------------------- end of update_ready_nodes related functions -------------------------------------------------------------------- */



/* ------------------------------- run_scheduler_node related functions -------------------------------------------------------------------- */

/* update output tensor addr, then config the basic node on hardware  */
void run_basic_node (rt_node_t* nd) {
    uint16_t node_type = nd->node_type ;

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
            // npu_input_param_t* param = (npu_input_param_t*) nd->param ;

            // /* get img_w/img_h */
            // if (0 == param->letterbox_rows && 0 == param->letterbox_cols) {
            //     img_w = param->input_w ;
            //     img_h = param->input_h ;
            // } else {
            //     img_w = param->letterbox_cols ;
            //     img_h = param->letterbox_rows ;
            // }

            // rt_tensor_t* t = get_rt_graph_tensor(nd->graph, nd->output_tensors[0]) ;
            // param->output_tensor->addr = t->param->addr ;

            // printf("\nrun npu_input_node\n") ;

            // if (run_on_os) {
            //    if (INOUT_DATA_MODE_DDR == inout_data_mode) {
            //         load_input_data(param) ;
            //     } else if (INOUT_DATA_MODE_SD_ARM == inout_data_mode) {
            //         process_input_image (input_pic_fn, param, run_on_os) ;
            //     } else {
            //         printf("unsupported inout_data_mode: %d\n", inout_data_mode);
            //     }
            // } else {
            //     if (INOUT_DATA_MODE_DDR == inout_data_mode) {
            //         // npu_input_node (param) ; // do nothing
            //     } else if (INOUT_DATA_MODE_SD_ARM == inout_data_mode) {
            //         process_input_image (input_pic_fn, param, run_on_os) ;
            //     } else {
            //         printf("unsupported inout_data_mode: %d\n", inout_data_mode);
            //     }
            // }


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
            printf("unsupported rt_node type %d \n", node_type) ;
        }
    }
}

/* config the scheduler node on the hardware */
int8_t run_scheduler_node (scheduler_node_t* sn) {
    if (sn->node_type == RT_S_BASIC) {
        rt_node_t* rt_nd = get_rt_graph_node (sn->graph, sn->nd->index) ;

        /* 3> config the rt_node of the basic scheduler on hardware */
        // printf("\nnode id: %d\n", rt_nd->index) ;
        run_basic_node (rt_nd) ;

    } else {
        printf("unsupported scheduler node type to run: %d\n", sn->node_type) ;
        return -1 ;
    }

    return 0 ;
}

/* ------------------------------- end of run_scheduler_node related functions -------------------------------------------------------------------- */

/* ------------------------------- thread related functions -------------------------------------------------------------------- */

#ifdef RUN_PLATFORM_LINUX

void* thread_func_hard_npu(void* arg) {

    uint32_t info = 1; /* unmask */

    /* wait for interrupt from hard npu*/
    while (1) {
        size_t nb = read(hard_irq_fd, &info, sizeof(info));
        if (nb == (size_t)sizeof(info)) {
            // reset interrupt in hard npu
            hard_npu_write(HARDNPU__CCU__CLEAR_INTERRUPT__ADDR, 0x1) ;

            hard_finish_cnt += 1;

            /* Do something in response to the interrupt. */
            printf("hard_npu Interrupt #%u!\n", info);

            // reset interrupt in UIO
            info = 1;
            nb = write(hard_irq_fd, &info, sizeof(info));
            if (nb != (size_t)sizeof(info)) {
                printf("write hard_npu failed\n");
                return NULL;
            }


            handle_hard_node_irq();

            // if ((fifo_item_num(soft_running_nodes) == 0) && (fifo_item_num(soft_ready_nodes) == 0) &&
            //         (fifo_item_num(hard_running_nodes) == 0) && (fifo_item_num(hard_ready_nodes) == 0)) {
            if(hard_finish_cnt == hard_node_num) {
                // reach the condition to exit this thread.
                pthread_exit(NULL);
                // return NULL;
            }
        } else {
            printf("read from hard_npu_device failed, expected size: %d, read size: %d\n", sizeof(info), nb);
        }

    }

    return NULL;
}

void* thread_func_soft_npu(void* arg) {

    uint32_t info = 1; /* unmask */

    /* wait for interrupt from soft npu*/
    while (1) {
        size_t nb = read(soft_irq_fd, &info, sizeof(info));
        if (nb == (size_t)sizeof(info)) {
            // reset interrupt in soft npu
            soft_npu_write(SOFTNPU__SOFT_SHARE__CLEAR_INTERRUPT__ADDR, 0x1) ;

            soft_finish_cnt += 1;

            /* Do something in response to the interrupt. */
            printf("soft_npu Interrupt #%u!\n", info);

            // reset interrupt in UIO
            info = 1;
            nb = write(soft_irq_fd, &info, sizeof(info));
            if (nb != (size_t)sizeof(info)) {
                printf("write soft_npu failed\n");
                return NULL;
            }

            handle_soft_node_irq();

            // if ((fifo_item_num(soft_running_nodes) == 0) && (fifo_item_num(soft_ready_nodes) == 0) &&
            //         (fifo_item_num(hard_running_nodes) == 0) && (fifo_item_num(hard_ready_nodes) == 0)) {
            if(soft_finish_cnt == soft_node_num) {
                // reach the condition to exit this thread.
                pthread_exit(NULL);
                // return NULL;
            }
        } else {
            printf("read from soft_npu_device failed, expected size: %d, read size: %d\n", sizeof(info), nb);
        }

    }

    return NULL;
}

#endif



/* ------------------------------- end of thread related functions -------------------------------------------------------------------- */

/* ------------------------------- scheduler main function -------------------------------------------------------------------- */

// Tips for future work:
// 1. if more hard npu available, need to assign the ready nodes according to the hardware resource available.
// 2. for more advanced scheduler, perform prediction feature:
//      assign the ready nodes according to the hardware resource available and running time needed by the ready nodes.
// 3. in multi-thread or multi-process system, make sure update_ready_nodes is thread safe.
// 4. queue implemented by fifo may change to queue implemented by linked list.
int8_t run_npu_graph (rt_graph_t* graph) {
    //TODO: reset fifo

    /* reset the num of children that are finished */
    memset(sched_nd_rec, 0, sizeof(int16_t) * graph->scheduler_node_num) ;

    /* find the first ready node*/
    scheduler_node_t* entry_nd = graph->scheduler_node_list[0] ;
    if (entry_nd->node_type == RT_S_PARALLEL) {
        find_ready_nodes_for_pg (entry_nd) ;
    } else {
        printf("entry node of rt_graph should be parallel type, but actully is: %d\n", entry_nd->node_type) ;
        return -1 ;
    }

    /* run the first ready node. the entry node of a net, must be soft node, more specific, input node.*/
    if (fifo_item_num(soft_ready_nodes) > 0) {
        any_data_t out_data = pop_fifo_data(soft_ready_nodes) ;
        scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
        push_fifo_data (soft_running_nodes, out_data) ;
        // run node
        if (run_scheduler_node (sn)) {
            printf ("run scheduler node failed: %d\n", sn->index) ;
            return -1 ;
        }
        pop_fifo_data(soft_running_nodes) ;

        /* input node is not implemented in npu, special process here */
        update_ts_usage (sn) ;
        update_ready_nodes (sn) ;
        // assign nodes to npu
        if ((fifo_item_num(soft_running_nodes) == 0) && (fifo_item_num(soft_ready_nodes) > 0)) {
            any_data_t out_data = pop_fifo_data(soft_ready_nodes) ;
            scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
            push_fifo_data (soft_running_nodes, out_data) ;
            run_scheduler_node (sn) ;
        }
        if ((fifo_item_num(hard_running_nodes) == 0) && (fifo_item_num(hard_ready_nodes) > 0)) {
            any_data_t out_data = pop_fifo_data(hard_ready_nodes) ;
            scheduler_node_t* sn = (scheduler_node_t*) out_data.pointer ;
            push_fifo_data (hard_running_nodes, out_data) ;
            run_scheduler_node (sn) ;
        }

#ifdef RUN_PLATFORM_LINUX

        pthread_mutex_init(&m_mutex, NULL);

        pthread_t tid1, tid2;
        pthread_create(&tid1, NULL, thread_func_hard_npu, NULL);
        pthread_create(&tid2, NULL, thread_func_soft_npu, NULL);

        char* rev = NULL;
        pthread_join(tid1, (void*)rev);
        pthread_join(tid2, (void*)rev);

        pthread_mutex_destroy(&m_mutex);

#endif

    } else {
        printf ("the first node of the rt_graph is not soft node\n") ;
        return -1 ;
    }

#ifdef RUN_PLATFORM_BM

    /* if on linux or rtos operating system, the while loop can be replaced with multi-thread or multi-process */
    while (fifo_item_num(soft_ready_nodes) > 0 || fifo_item_num(hard_ready_nodes) > 0 || fifo_item_num(soft_running_nodes) > 0 || fifo_item_num(hard_running_nodes) > 0) {
        if (soft_irq) {
            /* reset soft_irq flag */
            soft_irq = 0 ;

            handle_soft_node_irq () ;
        }
        if (hard_irq) {
            /* reset hard_irq flag */
            hard_irq = 0 ;

            handle_hard_node_irq () ;
        }
    }

#endif

#ifdef RUN_PLATFORM_LINUX

    hard_npu_exit();
    soft_npu_exit();

#endif

    // /* gen yolo result */
    // // now we only support yolo series, to support more result generation later on
    // res_addr = ts_vaddr_rec[graph->tensor_num - 1] ;
    // int16_t* data = (int16_t*) (res_addr) ;
    // int16_t yolo_len = *(data) ;
    // if (yolo_len%12 != 0) {
    //     printf("data length modulo 12 is not zero: %d\n", yolo_len) ;
    // }
    // int16_t num_res = yolo_len/12 ;
    // if (num_res < 0 || num_res > 340) {
    //     printf("ERROR: invalid yolo num_res: %d\n", num_res) ;
    // }
    // data += 4 ; //skip lenth

    // const char* out_image_file = "out.jpg" ;

    // float red = 255 ;
    // float green = 0 ;
    // float blue = 0 ;
    // float rgb[3];
    // rgb[0] = red / 255 ;
    // rgb[1] = green / 255 ;
    // rgb[2] = blue / 255 ;

    // static const char* class_names[] = {
    // "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
    // "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
    // "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
    // "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
    // "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
    // "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
    // "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
    // "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
    // "hair drier", "toothbrush"};

    // image img_input ;
    // image **alphabet = NULL;
    // if (NULL != input_pic_fn) {
    //    // FILINFO fno ;
    //    // FIL fd ;
    //    // int ret = f_open(&fd, input_pic_fn, FA_READ) ;
    //    // if (ret) {
    //    //     printf("Fail to open file %s, err no: %d\n", input_pic_fn, ret) ;
    //    //     return NULL ;
    //    // }
    //    // f_stat(input_pic_fn, &fno) ;
    //    // int file_len = fno.fsize ;

    //    // #define IMAGE_START_ADDR 0x40000000
    //    // char* image_start_addr = IMAGE_START_ADDR ;
    //    // UINT br = 0 ;
    //    // ret = f_read(&fd, (void*)image_start_addr, file_len, &br) ;
    //    // if (ret) {
    //    //     printf("Fail to read file: %s, err no: %d\n", input_pic_fn, ret) ;
    //    //     return NULL ;
    //    // }
    //    // ret = f_close(&fd) ;
    //    // if (ret) {
    //    //     printf("Fail to close file %s, err no: %d\n", input_pic_fn, ret) ;
    //    //     return 0 ;
    //    // }

    //     //img_input = load_image_from_memory(image_start_addr,file_len,0,0,3);

    //     //alphabet = load_alphabet();

    // } else {
    //     img_input = make_empty_image(0,0,0);
    // }

    // for (int16_t i = 0; i < num_res; i++) {
    //     // printf("\n prob: %\tlabel: %d\trect.x_l: %d\trect.y_l: %d\trect.x_h: %d\trect.y_h: %d\n",*(data),*(data+1),*(data+2),*(data+3),*(data+4),*(data+5)) ;
    //     float prob = (float)(*(data))/1024/16 ;
    //     int prob_int = prob*100 ;
    //     int16_t label = *(data+1) ;
    //     int16_t x0 = *(data+2) ;
    //     x0 = x0*img_w/32/1024 ;
    //     int16_t y0 = *(data+3) ;
    //     y0 = y0*img_h/32/1024 ;
    //     int16_t w = *(data+4) ;
    //     w = w*img_w/32/1024 ;
    //     int16_t h = *(data+5) ;
    //     h = h*img_h/32/1024 ;
    //     int16_t x_l = x0 - w/2 ;
    //     int16_t y_l = y0 - h/2 ;
    //     int16_t x_h = x0 + w/2 ;
    //     int16_t y_h = y0 + h/2 ;

    //     printf("\n prob: %d%%\tlabel: %d\trect.x_l: %d\trect.y_l: %d\trect.x_h: %d\trect.y_h: %d\n",(int)(prob*100),label,x_l,y_l,x_h,y_h) ;
    //     data += 6 ;

    //     /* put label */
    //     if (NULL != input_pic_fn) {
    //         /* yolov3 tiny draw the result */
    //         int raw_h = img_input.h ;
    //         int raw_w = img_input.w ;

    //         float ratio_x = (float)raw_w / (float)img_w;
    //         float ratio_y = (float)raw_h / (float)img_h;

    //         x_l = x_l * ratio_x;
    //         y_l = y_l * ratio_y;
    //         x_h = x_h * ratio_x;
    //         y_h = y_h * ratio_y;

    //         if (x_l < 0) x_l = 0 ;
    //         if (x_h > (raw_w -1)) x_h = raw_w - 1 ;
    //         if (y_l < 0) y_l = 0 ;
    //         if (y_h > (raw_h -1)) y_h = raw_h - 1 ;

    //         draw_box(img_input, x_l, y_l, x_h, y_h, rgb[0], rgb[1], rgb[2]) ;

    //         char text[256];
    //         sprintf(text, "%s %d%%", class_names[label], (int)(prob*100));
    //         image img_label = get_label(alphabet, text, img_input.h * 0.03);

    //         int x = x_l ;
    //         int y = y_l ;

    //         draw_label(img_input, y+img_label.h, x, img_label, rgb);
    //     }
    // }

    // if (NULL != input_pic_fn) {
    //     //FILINFO fno ;
    //     //FIL fd ;
    //     //int ret = f_open(&fd, out_image_file, (FA_CREATE_ALWAYS | FA_WRITE) ) ;
    //     //if (ret) {
    //     //    printf("fail to open file %s, err no: %d\n", out_image_file, ret) ;
    //     //    return NULL ;
    //     //}

    //     //unsigned char *data = calloc(img_input.w*img_input.h*img_input.c, sizeof(char));
    //     //int i,k;
    //     //for(k = 0; k < img_input.c; ++k){
    //     //    for(i = 0; i < img_input.w*img_input.h; ++i){
    //     //        data[i*img_input.c+k] = (unsigned char) (255*img_input.data[i + k*img_input.w*img_input.h]);
    //     //    }
    //     //}
    //     //stbi_write_jpg_to_func(stbi_write_func_sd, (void*)(&fd), img_input.w, img_input.h, img_input.c, data, 80);

    //     //ret = f_close(&fd) ;
    //     //if (ret) {
    //     //    printf("cannot close file %s\n", out_image_file) ;
    //     //    return 0 ;
    //     //}
    //     //
    //     //free(data) ;

    //     //free_image(img_input) ;

    //     printf("finish draw_yolo_result\n") ;
    // }

    return 0 ;
}

/* ------------------------------- end of scheduler main function -------------------------------------------------------------------- */
