

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
//#include <sys/mman.h>
#endif

#include <fcntl.h>
#include <string.h>

#include "rgb_deserializer.h"
#include "rt_graph_bin_format.h"
#include "npu_rt_graph.h"
#include "npu_defines.h"
#include "file_ops.h"

#define LOG_TAG "GRAPH_DESERIALIZER"
#include "elog.h"

static char* strdup_name(char* buf, int size)
{
    char* p = (char*)malloc(size + 1);
    memcpy(p, buf, size);
    p[size] = 0x0;
    return p;
}

static inline const RGB_Header* get_rgb_file_header(const char* base) {
    return (const RGB_Header*)(base);
}

static inline const RGB_Vector_offsets* get_rgb_file_vo_tensors(const char* base, const RGB_Header* header) {
    return (const RGB_Vector_offsets*)(base + header->offset_vo_tensors);
}

static inline const RGB_Vector_offsets* get_rgb_file_vo_nodes(const char* base, const RGB_Header* header) {
    return (const RGB_Vector_offsets*)(base + header->offset_vo_nodes);
}

static inline const RGB_Vector_offsets* get_rgb_file_vo_scheduler_nodes(const char* base, const RGB_Header* header) {
    return (const RGB_Vector_offsets*)(base + header->offset_vo_scheduler_nodes);
}


static int load_graph_tensors(rt_graph_t* graph, const char* base, RGB_Vector_offsets* vo_tensors) {

    for (unsigned int i = 0; i < vo_tensors->v_num; i++) {
		RGB_Tensor* rgb_tensor = (RGB_Tensor*)(base + vo_tensors->offsets[i]) ;
		
		rt_tensor_t* rt_tensor = create_rt_tensor (graph) ;
		if (rt_tensor == NULL) {
			log_e("Create rt_tensor failed\n");
			return -1;
		}
		if (rt_tensor->index != rgb_tensor->tensor_id) {
			log_e("Rt_tensor index check failed, tensor_idx: %d \t rgb_tensor_id: %d\n", rt_tensor->index, rgb_tensor->tensor_id);
			return -1;
		}

		rt_tensor->tensor_type = rgb_tensor->tensor_type ;
		rt_tensor->allocated = rgb_tensor->allocated ;
		rt_tensor->p_split_num = rgb_tensor->p_split_num ;
		rt_tensor->c_split_num = rgb_tensor->c_split_num ;

		// load tensor name
		if (rgb_tensor->offset_s_tname != RGB_NOT_SET) {
			const RGB_String* rgb_str = (RGB_String*)(base + rgb_tensor->offset_s_tname);
            rt_tensor->name = strdup_name(base + rgb_str->offset_data, rgb_str->size);
		} else {
			rt_tensor->name = NULL ;
		}

		// load tensor param
		tensor_param_t* param = (tensor_param_t*)malloc(sizeof(tensor_param_t)) ;
		rt_tensor->param = param ;
		memcpy(rt_tensor->param, &(rgb_tensor->param), sizeof(tensor_param_t)) ;

		// load producer and consumer
		RGB_Vector_indices* v_producers = (RGB_Vector_indices*)(base + rgb_tensor->offset_vi_producer_nodes) ;
		rt_tensor->producer_num = v_producers->v_num ;
		rt_tensor->producer = (AL_U16*)realloc(rt_tensor->producer, sizeof(AL_U16) * v_producers->v_num) ;
		for( unsigned int j = 0; j < v_producers->v_num; j++) {
			rt_tensor->producer[j] = v_producers->indices[j] ;
		}

		RGB_Vector_indices* v_consumers = (RGB_Vector_indices*)(base + rgb_tensor->offset_vi_consumer_nodes) ;
		rt_tensor->consumer_num = v_consumers->v_num ;
		rt_tensor->consumer = (AL_U16*)realloc(rt_tensor->consumer, sizeof(AL_U16) * v_consumers->v_num) ;
		for( unsigned int j = 0; j < v_consumers->v_num; j++) {
			rt_tensor->consumer[j] = v_consumers->indices[j] ;
		}

		// load sub_params and addr offsets for producer
		if (rt_tensor->p_split_num > 0) {
			rt_tensor->p_sp_params = (sp_param_t*) malloc (sizeof(sp_param_t) * rt_tensor->p_split_num) ;
			RGB_Vector_sp_param* sp_params = (RGB_Vector_sp_param*)(base + rgb_tensor->offset_v_p_sp_params) ;
			for (int sp_idx = 0; sp_idx < rt_tensor->p_split_num; sp_idx++) {
				RGB_sp_param sp_param = sp_params->elems[sp_idx] ;
				rt_tensor->p_sp_params[sp_idx].sp_c_num = sp_param.sp_c_num ;
				rt_tensor->p_sp_params[sp_idx].sp_h_num = sp_param.sp_h_num ;
				int total = sp_param.sp_c_num * sp_param.sp_h_num ;

				rt_tensor->p_sp_params[sp_idx].sub_params = (tensor_param_t**) malloc (sizeof(tensor_param_t*) * total) ;
				RGB_Vector_tensor_param* v_t_params = (RGB_Vector_tensor_param*)(base + sp_param.offset_vt_sub_params) ;
							
				rt_tensor->p_sp_params[sp_idx].addr_offsets = (AL_U32*)malloc(sizeof(AL_U32) * total) ;
				RGB_Vector_uint32* v_addr_offset = (RGB_Vector_uint32*)(base + sp_param.offset_vuint32_addr_offsets) ;

				for (int sub_idx = 0; sub_idx < total; sub_idx++) {
					rt_tensor->p_sp_params[sp_idx].sub_params[sub_idx] = (tensor_param_t*)malloc(sizeof(tensor_param_t)) ; 
					memcpy(rt_tensor->p_sp_params[sp_idx].sub_params[sub_idx], &(v_t_params->elems[sub_idx]), sizeof(tensor_param_t)) ;
					rt_tensor->p_sp_params[sp_idx].addr_offsets[sub_idx] = v_addr_offset->elems[sub_idx] ;
				}

			}
		}

		// load sub_params and addr offsets for consumer
		if (rt_tensor->c_split_num > 0) {
			rt_tensor->c_sp_params = (sp_param_t*) malloc (sizeof(sp_param_t) * rt_tensor->c_split_num) ;
			RGB_Vector_sp_param* sp_params = (RGB_Vector_sp_param*)(base + rgb_tensor->offset_v_c_sp_params) ;
			for (int sp_idx = 0; sp_idx < rt_tensor->c_split_num; sp_idx++) {
				RGB_sp_param sp_param = sp_params->elems[sp_idx] ;
				rt_tensor->c_sp_params[sp_idx].sp_c_num = sp_param.sp_c_num ;
				rt_tensor->c_sp_params[sp_idx].sp_h_num = sp_param.sp_h_num ;
				int total = sp_param.sp_c_num * sp_param.sp_h_num ;

				rt_tensor->c_sp_params[sp_idx].sub_params = (tensor_param_t**) malloc (sizeof(tensor_param_t*) * total) ;
				RGB_Vector_tensor_param* v_t_params = (RGB_Vector_tensor_param*)(base + sp_param.offset_vt_sub_params) ;
							
				rt_tensor->c_sp_params[sp_idx].addr_offsets = (AL_U32*)malloc(sizeof(AL_U32) * total) ;
				RGB_Vector_uint32* v_addr_offset = (RGB_Vector_uint32*)(base + sp_param.offset_vuint32_addr_offsets) ;

				for (int sub_idx = 0; sub_idx < total; sub_idx++) {
					rt_tensor->c_sp_params[sp_idx].sub_params[sub_idx] = (tensor_param_t*)malloc(sizeof(tensor_param_t)) ; 
					memcpy(rt_tensor->c_sp_params[sp_idx].sub_params[sub_idx], &(v_t_params->elems[sub_idx]), sizeof(tensor_param_t)) ;
					rt_tensor->c_sp_params[sp_idx].addr_offsets[sub_idx] = v_addr_offset->elems[sub_idx] ;
				}
			}
		}

    }
    return 0;
}

static AL_VOID* get_param_from_rgb_node (AL_U16 node_type, rgb_uoffset_t offset, const char* base) {

    switch (node_type) {
        case RT_NPU_HARD : 
		{
            hard_npu_pkg_info_t* rgb_hpi = (hard_npu_pkg_info_t*)(base + offset) ;

            hard_npu_pkg_info_t* hpi = (hard_npu_pkg_info_t*)malloc(sizeof(hard_npu_pkg_info_t)) ;

            memcpy (hpi, rgb_hpi, sizeof(hard_npu_pkg_info_t)) ;

			return (AL_VOID*) hpi ;
		}
        case RT_POOLING:
        case RT_CONCAT:
        case RT_UPSAMPLE:
        case RT_PRELU:
        case RT_ELTWISE:
		case RT_INTERP:
		{
			soft_npu_pkg_info_t* rgb_spi = (soft_npu_pkg_info_t*)(base + offset) ;

            soft_npu_pkg_info_t* spi = (soft_npu_pkg_info_t*)malloc(sizeof(soft_npu_pkg_info_t)) ;

            memcpy (spi, rgb_spi, sizeof(soft_npu_pkg_info_t)) ;

			return (AL_VOID*) spi ;
		}
        case RT_DWCONV : {
            RGB_NPUDWConvParam *rgb_dwconv_param = (RGB_NPUDWConvParam *) (base + offset);
            npu_dwconv_param_t *npu_dwconv_param = (npu_dwconv_param_t *) malloc(sizeof(npu_dwconv_param_t));

            memcpy(&(npu_dwconv_param->param), &(rgb_dwconv_param->param), sizeof(conv_param_t));

            // init input/output tensors
            npu_dwconv_param->input_num = 0;
            npu_dwconv_param->input_tensors = NULL;
            npu_dwconv_param->output_tensor = NULL;

            memcpy(&(npu_dwconv_param->param), &(rgb_dwconv_param->param), sizeof(conv_param_t));
            npu_dwconv_param->conv_type = rgb_dwconv_param->conv_type;
            npu_dwconv_param->mode_load_data = rgb_dwconv_param->mode_load_data;
            npu_dwconv_param->input_zero_point = rgb_dwconv_param->input_zero_point;
            npu_dwconv_param->weight_zero_point = rgb_dwconv_param->weight_zero_point;
            npu_dwconv_param->output_zero_point = rgb_dwconv_param->output_zero_point;
            memcpy(npu_dwconv_param->ifm_dims, rgb_dwconv_param->ifm_dims, sizeof(AL_S32) * 4);
            memcpy(npu_dwconv_param->ofm_dims, rgb_dwconv_param->ofm_dims, sizeof(AL_S32) * 4);

            // load bias data
            RGB_Vector_bias *v_bias = (RGB_Vector_bias *) (base + rgb_dwconv_param->offset_vbias);
            npu_dwconv_param->bias_data = (AL_S32 *) malloc(sizeof(AL_S32) * v_bias->v_num);
            for (unsigned int i = 0; i < v_bias->v_num; i++) {
                npu_dwconv_param->bias_data[i] = v_bias->elems[i];
            }

            // load scale and rshift data
            RGB_Vector_scale *v_scale = (RGB_Vector_scale *) (base + rgb_dwconv_param->offset_vscale);
            npu_dwconv_param->fx_param_count = v_scale->v_num;
            npu_dwconv_param->fx_scales = (rgb_scale_t *) malloc(sizeof(rgb_scale_t) * v_scale->v_num);
            for (unsigned int i = 0; i < v_scale->v_num; i++) {
                npu_dwconv_param->fx_scales[i] = v_scale->elems[i];
            }

            RGB_Vector_rshift *v_rshift = (RGB_Vector_rshift *) (base + rgb_dwconv_param->offset_vrshift);
            npu_dwconv_param->fx_rshifts = (rgb_rshift_t *) malloc(sizeof(rgb_rshift_t) * v_rshift->v_num);
            for (unsigned int i = 0; i < v_rshift->v_num; i++) {
                npu_dwconv_param->fx_rshifts[i] = v_rshift->elems[i];
            }

            RGB_Vector_multi *v_multi = (RGB_Vector_multi *) (base + rgb_dwconv_param->offset_vmulti);
            npu_dwconv_param->fx_multi = (rgb_multi_t *) malloc(sizeof(rgb_multi_t) * v_multi->v_num);
            for (unsigned int i = 0; i < v_multi->v_num; i++) {
                npu_dwconv_param->fx_multi[i] = v_multi->elems[i];
            }

            RGB_Vector_qshift *v_qshift = (RGB_Vector_qshift *) (base + rgb_dwconv_param->offset_vqshift);
            npu_dwconv_param->fx_qshifts = (rgb_qshift_t *) malloc(sizeof(rgb_qshift_t) * v_qshift->v_num);
            for (unsigned int i = 0; i < v_qshift->v_num; i++) {
                npu_dwconv_param->fx_qshifts[i] = v_qshift->elems[i];
            }

            npu_dwconv_param->activation_max = rgb_dwconv_param->activation_max;
            npu_dwconv_param->activation_min = rgb_dwconv_param->activation_min;

            return (AL_VOID *)npu_dwconv_param;
        }
		case RT_RESHAPE:{
			RGB_NPUReshapeParam *rgb_reshape_param = (RGB_NPUReshapeParam *) (base + offset);
			npu_reshape_param_t *npu_reshape_param = (npu_reshape_param_t *) malloc(sizeof(npu_reshape_param_t));
			npu_reshape_param->input_tensor = NULL;
			npu_reshape_param->output_tensor = NULL;
			npu_reshape_param->dim_size = rgb_reshape_param->dim_size;
			// memcpy(&(npu_reshape_param), &(rgb_reshape_param->param), sizeof(npu_reshape_param_t));
			return (AL_VOID *)npu_reshape_param;
			
		}
		case RT_SIGMOID:{
			RGB_NPUSigmoidParam *rgb_sigmoid_param = (RGB_NPUSigmoidParam *) (base + offset);
			npu_sigmoid_param_t *npu_sigmoid_param = (npu_sigmoid_param_t *) malloc(sizeof(npu_sigmoid_param_t));
			npu_sigmoid_param->input_tensor = NULL;
			npu_sigmoid_param->output_tensor = NULL;
			npu_sigmoid_param->scale_i = rgb_sigmoid_param->scale_i;
			npu_sigmoid_param->scale_o = rgb_sigmoid_param->scale_o;
			npu_sigmoid_param->zp_i = rgb_sigmoid_param->zp_i;
			npu_sigmoid_param->zp_o = rgb_sigmoid_param->zp_o;
			return (AL_VOID *)npu_sigmoid_param;
		}
        case RT_INPUT:
		{
	        RGB_NPUInputParam* rgb_nip = (RGB_NPUInputParam*)(base + offset) ;
	    
            npu_input_param_t* nip = (npu_input_param_t*)malloc(sizeof(npu_input_param_t)) ;

			// init input/output tensors
			nip->output_tensor = NULL ;

            nip->input_c = rgb_nip->input_c ;
			nip->input_h = rgb_nip->input_h ;
			nip->input_w = rgb_nip->input_w ;
            nip->mean[0] = rgb_nip->mean[0] ;
			nip->mean[1] = rgb_nip->mean[1] ;
			nip->mean[2] = rgb_nip->mean[2] ;
			nip->scale[0] = rgb_nip->scale[0] ;
			nip->scale[1] = rgb_nip->scale[1] ;
			nip->scale[2] = rgb_nip->scale[2] ;

   			nip->focus = rgb_nip->focus ;
            nip->letterbox_rows = rgb_nip->letterbox_rows ;
            nip->letterbox_cols = rgb_nip->letterbox_cols ;

            nip->scale_q = rgb_nip->scale_q ;
            nip->zero_point = rgb_nip->zero_point ;

			nip->r_mean_rshift = rgb_nip->r_mean_rshift ;
			nip->r_mean_param = rgb_nip->r_mean_param ;
			nip->g_mean_rshift = rgb_nip->g_mean_rshift ;
			nip->g_mean_param = rgb_nip->g_mean_param ;
			nip->b_mean_rshift = rgb_nip->b_mean_rshift ;
			nip->b_mean_param = rgb_nip->b_mean_param ;

			nip->r_quant_rshift = rgb_nip->r_quant_rshift ;
			nip->r_quant_param = rgb_nip->r_quant_param ;
			nip->g_quant_rshift = rgb_nip->g_quant_rshift ;
			nip->g_quant_param = rgb_nip->g_quant_param ;
			nip->b_quant_rshift = rgb_nip->b_quant_rshift ;
			nip->b_quant_param = rgb_nip->b_quant_param ;
	    
            return (AL_VOID*) nip ;
		}
        case RT_YOLO:
		{

            RGB_Vector_yolo_info* rgb_vyi = (RGB_Vector_yolo_info*)(base + offset) ;

            yolo_pkg_info_t** ypi_vec = (yolo_pkg_info_t**)malloc(sizeof(yolo_pkg_info_t*) * rgb_vyi->v_num) ;  

            // load yolo_pkg_infos 
			for (unsigned int i = 0; i < rgb_vyi->v_num; i++) {
				yolo_pkg_info_t* ypi = (yolo_pkg_info_t*)malloc(sizeof(yolo_pkg_info_t)) ;
				memcpy (ypi, &(rgb_vyi->elems[i]), sizeof(yolo_pkg_info_t)) ;
				ypi_vec[i] = ypi ;
			}

            return (AL_VOID*) ypi_vec ;
		}
        default:
		{
            log_e("Unsupported node type:%d in runtime graph\n", node_type);
            return NULL;
		}
    }

}

static int load_graph_nodes(rt_graph_t* graph, const char* base, RGB_Vector_offsets* vo_nodes) {

    for (unsigned int i = 0; i < vo_nodes->v_num; i++) {
		RGB_Node* rgb_node = (RGB_Node*)(base + vo_nodes->offsets[i]) ;
		
		rt_node_t* rt_node = create_rt_node (graph, rgb_node->node_type) ;
		if (rt_node == NULL) {
			log_e("Create rt_node failed\n");
			return -1;
		}
		if (rt_node->index != rgb_node->node_id) {
			log_e("Rt_node index check failed\n") ;
			return -1;
		}
		rt_node->is_sub_nd = rgb_node->is_sub_nd ;
		rt_node->c_sp_idx = rgb_node->c_sp_idx ;
		rt_node->sub_nd_id = rgb_node->sub_nd_id ;

		// load tensor name
		if (rgb_node->offset_s_nname != RGB_NOT_SET) {
			const RGB_String* rgb_str = (RGB_String*)(base + rgb_node->offset_s_nname);
			rt_node->name = strdup_name(base + rgb_str->offset_data, rgb_str->size);
		} else {
			rt_node->name = NULL ;
		}

		// load input tensors and output tensors
		RGB_Vector_indices* v_input = (RGB_Vector_indices*)(base + rgb_node->offset_vi_input_tensors) ;
		rt_node->input_num = v_input->v_num ;
		rt_node->input_tensors = (AL_U16*)malloc(sizeof(AL_U16) * v_input->v_num) ;
		for( unsigned int j = 0; j < v_input->v_num; j++) {
			rt_node->input_tensors[j] = v_input->indices[j] ;
		}

		RGB_Vector_indices* v_output = (RGB_Vector_indices*)(base + rgb_node->offset_vi_output_tensors) ;
		rt_node->output_num = v_output->v_num ;
		rt_node->output_tensors = (AL_U16*)malloc(sizeof(AL_U16) * v_output->v_num) ;
		for( unsigned int j = 0; j < v_output->v_num; j++) {
			rt_node->output_tensors[j] = v_output->indices[j] ;
		}

		// load param
		rt_node->param = get_param_from_rgb_node (rgb_node->node_type, rgb_node->offset_param, base) ;
		if (rt_node->param == NULL) {
			return -1;
		}

    }

    return 0;
}

static AL_VOID* get_content_from_rgb_scheduler_node (AL_U8 node_type, rgb_uoffset_t offset, const char* base) {
    switch (node_type) {
        case RT_S_BASIC: 
		{
            RGB_BasicBlock* rgb_basic_block = (RGB_BasicBlock*)(base + offset) ;
	    	basic_node_t* nd = create_basic_node (rgb_basic_block->index, rgb_basic_block->parent) ;
			if (nd == NULL) {
				log_e("Create basic node failed\n");
				return NULL;
			}

            return (AL_VOID*) nd ;
		}
        case RT_S_PARALLEL:
		{
            RGB_ParallelGroup* rgb_pg = (RGB_ParallelGroup*)(base + offset) ;
			parallel_group_t* pg = create_parallel_group (rgb_pg->parent) ;
			if (pg == NULL) {
				log_e("Create parallel group failed\n");
				return NULL;
			}
			pg->is_split = rgb_pg->is_split ;

			// load child nodes
			RGB_Vector_indices* vi = (RGB_Vector_indices*)(base + rgb_pg->offset_vi_nodes) ;
			pg->node_num = vi->v_num ;
			pg->nodes = (AL_S16*)malloc(sizeof(AL_S16) * vi->v_num) ;
			for (unsigned int i = 0; i < vi->v_num; i++) {
				pg->nodes[i] = vi->indices[i] ;
			}

            return (AL_VOID*) pg ;
		}
        case RT_S_SEQUENTIAL:
		{
            RGB_SequentialList* rgb_sl = (RGB_SequentialList*)(base + offset) ;
			sequential_list_t* sl = create_sequential_list (rgb_sl->parent) ;

			// load child nodes
			RGB_Vector_indices* vi = (RGB_Vector_indices*)(base + rgb_sl->offset_vi_nodes) ;
			sl->node_num = vi->v_num ;
			sl->nodes = (AL_S16*)malloc(sizeof(AL_S16) * vi->v_num) ;
			for (unsigned int i = 0; i < vi->v_num; i++) {
				sl->nodes[i] = vi->indices[i] ;
			}

            return (AL_VOID*) sl ;
		}
        default:
		{
            log_e("Unsupported scheduler node type:%d in runtime graph\n", node_type);
            return NULL;
		}
    }

}

static int load_graph_scheduler_nodes(rt_graph_t* graph, const char* base, RGB_Vector_offsets* vo_scheduler_nodes) {

    for (unsigned int i = 0; i < vo_scheduler_nodes->v_num; i++) {
		RGB_Scheduler_Node* rgb_snode = (RGB_Scheduler_Node*)(base + vo_scheduler_nodes->offsets[i]) ;
		
		// create content
		AL_VOID* content = get_content_from_rgb_scheduler_node(rgb_snode->node_type, rgb_snode->offset_node_content, base); 
		if (content == NULL) {
			return -1;
		}

		scheduler_node_t* rt_snode = create_scheduler_node (graph, rgb_snode->node_type, content) ;
		if (rt_snode == NULL) {
			log_e("Create scheduler node failed\n");
			return -1;
		}
		if (rt_snode->index != rgb_snode->node_id) {
			log_e("Scheduler node index check failed\n") ;
			return -1;
		}
    }

    return 0;
}


rt_graph_t* load_rt_graph(const char* fname) {

	AL_VOID* mem_base = NULL;

	AL_S64 file_len = get_file_size(fname);
	if (-1 == file_len) {
		log_e("get file length failed\n");
		return NULL;
	}

	mem_base = (AL_VOID*)malloc(file_len);	
	if (mem_base == NULL) {
		log_e("malloc failed\n");
		return NULL;
	}

	if (read_file(fname, mem_base, file_len)) {
		log_e("read file: %s failed\n", fname);
		free(mem_base);
		return NULL;
	}

    rt_graph_t* graph = create_rt_graph() ;

    RGB_Header* header = get_rgb_file_header(mem_base) ;
    RGB_Vector_offsets* vo_tensors = get_rgb_file_vo_tensors (mem_base, header) ;
    RGB_Vector_offsets* vo_nodes = get_rgb_file_vo_nodes (mem_base, header) ;
    RGB_Vector_offsets* vo_scheduler_nodes = get_rgb_file_vo_scheduler_nodes (mem_base, header) ;

    if (load_graph_tensors(graph, mem_base, vo_tensors) < 0)
        goto error;

    if (load_graph_nodes(graph, mem_base, vo_nodes) < 0)
        goto error;

    if (load_graph_scheduler_nodes(graph, mem_base, vo_scheduler_nodes) < 0)
        goto error;

    return graph;

error:
    return NULL;

}

rt_graph_t* load_rt_graph_from_mem (const AL_VOID* addr) {
 	rt_graph_t* graph = create_rt_graph() ;

    RGB_Header* header = get_rgb_file_header( addr) ;
    RGB_Vector_offsets* vo_tensors = get_rgb_file_vo_tensors ( addr, header) ;
    RGB_Vector_offsets* vo_nodes = get_rgb_file_vo_nodes ( addr, header) ;
    RGB_Vector_offsets* vo_scheduler_nodes = get_rgb_file_vo_scheduler_nodes ( addr, header) ;

    if (load_graph_tensors(graph,  addr, vo_tensors) < 0)
        goto error;

    if (load_graph_nodes(graph,  addr, vo_nodes) < 0)
        goto error;

    if (load_graph_scheduler_nodes(graph,  addr, vo_scheduler_nodes) < 0)
        goto error;

    return graph;

error:
    return NULL;
}


