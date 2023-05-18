#include <string.h>
#include <stdio.h>
#include "driver_api.h"
#include "npu_rt_graph.h"
#include "npu_rt_graph_util.h"



const char* get_rt_tensor_type_string (int type) {
    switch (type) {
        case RT_TENSOR_CONST:
            return "const";
        case RT_TENSOR_VAR:
            return "var";
        default:
            return "unknown";
    }
}

const char* get_rt_tensor_layout_string (int type) {
    switch (type) {
        case NPU_TENSOR_LAYOUT_NCHW:
            return "NCHW";
        case NPU_TENSOR_LAYOUT_NHWC:
            return "NHWC";
        default:
            return "unknown";
    }
}

const char* get_rt_node_type_string (int type) {
    switch (type) {
        case RT_NPU_HARD:
            return "npu hard node";
        case RT_POOLING:
            return "pooling";
        case RT_CONCAT:
            return "concat";
        case RT_UPSAMPLE:
            return "upsample";
        case RT_ELTWISE:
            return "eltwise";
        case RT_PRELU:
            return "prelu";
        case RT_INTERP:
            return "interp";
        case RT_DWCONV:
            return "dwconv";
        case RT_RESHAPE:
            return "reshape";
        case RT_SIGMOID:
            return "sigmoid";
        case RT_INPUT:
            return "input";
        case RT_YOLO:
            return "yolo";
        default:
            return "unknown";
    }
}

const char* get_scheduler_node_type_string (int type) {
    switch (type) {
        case RT_S_BASIC:
            return "basic";
        case RT_S_PARALLEL:
            return "parallel";
        case RT_S_SEQUENTIAL:
            return "sequential";
        default:
            return "unknown";
    }
}

void sn_level_str(char* buf, int level, char blank) {
    strcpy (buf, "") ;
    for (int i = 0; i < level; i++) {
        if (blank == '\t')
            sprintf (buf, "%s%c", buf, blank) ;
        else
            sprintf (buf, "%s%c%c%c%c", buf, blank, blank, blank, blank) ;
    }
}

