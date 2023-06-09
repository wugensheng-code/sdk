#ifndef NEW_OP_FUNC_H
#define NEW_OP_FUNC_H

#include "al_type.h"
#include "driver_api.h"

AL_S8 npu_reshape_node (npu_reshape_param_t* param);

inline AL_S8 npu_reshape_node_int8 (npu_reshape_param_t* param);

inline AL_S8 npu_reshape_node_uint8 (npu_reshape_param_t* param);

AL_S8 npu_sigmoid_node (npu_sigmoid_param_t* param);

inline AL_S8 npu_sigmoid_node_int8 (npu_sigmoid_param_t* param);

inline AL_S8 npu_sigmoid_node_uint8 (npu_sigmoid_param_t* param);

#endif