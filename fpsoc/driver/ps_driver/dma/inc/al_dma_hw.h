#ifndef __AL_DMA_HW_H__
#define __AL_DMA_HW_H__


#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

typedef struct
{
    AL_U32                  DeviceId;
    AL_U32                  BaseAddress;
    AL_U32                  ChanCount;
    AL_U16                  IntrNum[AL_DMA_CHAN_COUNT];
    AL_U32                  AbortIntr;
    AL_U32                  LockAddr;
} AL_DMA_HwConfigStruct;


#ifdef __cplusplus
}
#endif

#endif
