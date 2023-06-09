#ifndef __NPU_RUNTIME_DRIVER_INIT_H__
#define __NPU_RUNTIME_DRIVER_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

AL_S8 hard_npu_driver_init();

AL_S8 hard_npu_driver_exit();

AL_S8 soft_npu_driver_init();

AL_S8 soft_npu_driver_exit();

#ifdef __cplusplus
}
#endif

#endif