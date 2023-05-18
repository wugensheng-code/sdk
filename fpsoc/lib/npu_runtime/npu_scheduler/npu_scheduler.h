#ifndef NPU_SCHEDULER_H
#define NPU_SCHEDULER_H

#include "npu_rt_graph.h"

#define INOUT_DATA_MODE_DDR 0
#define INOUT_DATA_MODE_SD_ARM 1
#define INOUT_DATA_MODE_SD_JPU 2

int8_t run_npu_graph (rt_graph_t* graph) ;



#endif