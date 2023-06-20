#include <unistd.h>
#include <string.h>

#include "image.h"
#include "npu_util.h"
#include "fifo.h"
#include "npu_rt_graph.h"
#include "rgb_deserializer.h"
#include "npu_init.h"
#include "npu_defines.h"
#include "buddy_allocator.h"
#include "fm_mem_alloc.h"

#include "file_ops.h"
#include "npu_driver_init.h"

#define LOG_TAG "NPU_INIT"
#include "elog.h"

#define READY_QUEUE_SIZE 350


rt_graph_t* load_network (char* rt_graph_fn, char* filter_fn) {

    if (init_file_system()) {
        log_e("init file system failed\n");
        return NULL;
    }

    rt_graph_t* graph = load_rt_graph (rt_graph_fn);
    dump_rt_graph (graph) ;

    extern AL_S16* ts_usage_rec;
    ts_usage_rec = (AL_S16*) malloc (sizeof(AL_S16) * graph->tensor_num) ;
    if (ts_usage_rec == NULL) {
        log_e("malloc failed for ts_usage_rec\n") ;
        destroy_rt_graph(graph);
        return NULL;
    }
    // init ts_usage_rec ;
    for (int i = 0; i < graph->tensor_num; i++) {
        ts_usage_rec[i] = 0 ;
    }

    extern AL_S16* sched_nd_rec;
    sched_nd_rec = (AL_S16*) malloc (sizeof(AL_S16) * graph->scheduler_node_num) ;
    if (sched_nd_rec == NULL) {
        log_e("malloc failed for sched_nd_rec\n") ;
        destroy_rt_graph(graph);
        return NULL;
    }
    // init sched_nd_rec ;
    for (int i = 0; i < graph->scheduler_node_num; i++) {
        sched_nd_rec[i] = 0 ;
    }

    extern AL_U64* ts_vaddr_rec;
    ts_vaddr_rec = (AL_S64*) malloc (sizeof(AL_U64) * graph->tensor_num) ;
    if (ts_vaddr_rec == NULL) {
        log_e("malloc failed for ts_usage_rec\n") ;
        destroy_rt_graph(graph);
        return NULL;
    }
    // init ts_usage_rec ;
    for (int i = 0; i < graph->tensor_num; i++) {
        ts_vaddr_rec[i] = 0 ;
    }

    extern fifo_t* hard_ready_nodes ;
    hard_ready_nodes = create_fifo (sizeof(AL_VOID*), READY_QUEUE_SIZE + 1) ;
    extern fifo_t* soft_ready_nodes ;
    soft_ready_nodes = create_fifo (sizeof(AL_VOID*), READY_QUEUE_SIZE + 1) ;
    extern fifo_t* hard_running_nodes ;
    hard_running_nodes = create_fifo (sizeof(AL_VOID*), READY_QUEUE_SIZE + 1) ;
    extern fifo_t* soft_running_nodes ;
    soft_running_nodes = create_fifo (sizeof(AL_VOID*), READY_QUEUE_SIZE + 1) ;

    extern AL_U32 fm_start_addr ; // filter data is stored before fm, after init, fm_start_addr is defined
    extern AL_U32 fm_current_addr ; // record the start addr of next alloc 
    extern AL_U32 cma_phy_start ; // record the phy_addr of the start of cma mem block
    extern AL_VOID* cma_virt_start ; // record the vir_addr of the start of cma mem block
    extern struct buddy* buddy_allocator ;

    /* --------------- init cma_mem -------------------------------------------- */

    if (cma_mem_init(&cma_phy_start, &cma_virt_start)) {
        log_e("cma_mem_init failed\n");
        destroy_rt_graph(graph);
        return NULL;
    }

    fm_start_addr = 0;
    fm_current_addr = 0;

    /* --------------- end of init npu_mem -------------------------------------------- */

    /* --------------- read in weight file -------------------------------------------- */
    AL_S64 file_len = get_file_size(filter_fn);
	if (-1 == file_len) {
		log_e("get file length failed\n");
        destroy_rt_graph(graph);
		return NULL;
	}

	if (read_file(filter_fn, cma_virt_start, file_len)) {
		log_e("read file: %s failed\n", filter_fn);
		destroy_rt_graph(graph);
		return NULL;
	}

    if (file_len > WEIGHT_DATA_LENGTH) {
        log_e("file_len: %lx of weight.bin is more than reserved value: %lx\n", file_len, WEIGHT_DATA_LENGTH);
        destroy_rt_graph(graph);
        return NULL;
    }
    fm_start_addr = WEIGHT_DATA_LENGTH ;

    for (int i = 0; i < graph->tensor_num; i++) {
        rt_tensor_t* t = get_rt_graph_tensor(graph, i) ;
        if (t->tensor_type == RT_TENSOR_CONST) {
            t->param->addr += cma_phy_start;
        }
    }

    extern AL_U32 hard_node_num ;
    extern AL_U32 soft_node_num ;
    hard_node_num = 0 ;
    soft_node_num = 0 ;

    for (int i = 0; i < graph->node_num; i++) {
        rt_node_t* nd = get_rt_graph_node(graph, i) ;
        AL_U8 node_type = nd->node_type ;
        switch (node_type) {
            case RT_NPU_HARD : 
            {
                hard_npu_pkg_info_t* hpi = (hard_npu_pkg_info_t*) nd->param ;
                hpi->pkg_info.virt_addr = hpi->pkg_info.addr + (AL_U64) cma_virt_start ;
                hpi->pkg_info.addr += cma_phy_start ;
                hard_node_num++ ;
                break ;
            }
            case RT_POOLING:
            case RT_CONCAT:
            case RT_UPSAMPLE:
            case RT_PRELU:
            case RT_ELTWISE:
            case RT_INTERP:
            {
                soft_npu_pkg_info_t* spi = (soft_npu_pkg_info_t*) nd->param ;
                spi->pkg_info.virt_addr = spi->pkg_info.addr + (AL_U64) cma_virt_start ;
                spi->pkg_info.addr += cma_phy_start ;
                soft_node_num++ ;
                break ;
            }

            case RT_YOLO:
            {
                yolo_pkg_info_t** ypi = (yolo_pkg_info_t**) nd->param ;

                for (unsigned int i = 0; i < nd->input_num; i++) {
                    ypi[i]->pkg_info.virt_addr = ypi[i]->pkg_info.addr + (AL_U64) cma_virt_start ;
                    ypi[i]->pkg_info.addr += cma_phy_start ;
                    soft_node_num++ ;
                }

                break ;
            }
            case RT_INPUT:
            default:
            {
                break;
            }
        }
    }

    fm_current_addr = fm_start_addr;

    buddy_allocator = buddy_new(BUDDY_LEVEL) ;

    // exclude weight data in the first buddy
    buddy_alloc(buddy_allocator, fm_start_addr) ;

    return graph ;
}

