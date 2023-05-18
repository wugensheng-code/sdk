#ifndef NEW_OP_FUNC_H
#define NEW_OP_FUNC_H

int8_t npu_reshape_node (npu_reshape_param_t* param);

inline int8_t npu_reshape_node_int8 (npu_reshape_param_t* param);

inline int8_t npu_reshape_node_uint8 (npu_reshape_param_t* param);

int8_t npu_sigmoid_node (npu_sigmoid_param_t* param);

inline int8_t npu_sigmoid_node_int8 (npu_sigmoid_param_t* param);

inline int8_t npu_sigmoid_node_uint8 (npu_sigmoid_param_t* param);

#endif