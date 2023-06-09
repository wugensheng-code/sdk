#ifndef __NPU_RUNTIME_DRIVER_IO_H__
#define __NPU_RUNTIME_DRIVER_IO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

AL_VOID hard_npu_write(AL_U32 offset, AL_U32 value);

AL_U32 hard_npu_read(AL_U32 offset);

AL_VOID soft_npu_write(AL_U32 offset, AL_U32 value);

AL_U32 soft_npu_read(AL_U32 offset);

#ifdef __cplusplus
}
#endif

#endif