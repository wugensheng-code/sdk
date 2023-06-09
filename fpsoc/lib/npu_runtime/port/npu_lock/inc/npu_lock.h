#ifndef __NPU_RUNTIME_MUTEX_H__
#define __NPU_RUNTIME_MUTEX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

AL_S8 npu_lock_init();

AL_S8 npu_lock_destroy();

AL_S8 npu_lock_lock();

AL_S8 npu_lock_unlock();


#ifdef __cplusplus
}
#endif

#endif