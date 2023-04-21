#ifndef AL_CAN_HAL_H
#define AL_CAN_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_can_dev.h"

typedef struct
{
    AL_CAN_DevStruct    *Dev;

#ifdef USE_RTOS
    AL_Lock             Lock;
#endif
}AL_CAN_HalStruct;

AL_S32 AlCan_Hal_Init(AL_CAN_HalStruct *Handle, AL_CAN_InitStruct *InitConfig, \
    AL_CAN_CallBackStruct *CallBack, AL_U32 DevId);
AL_S32 AlCan_Hal_SendFrame(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame);
AL_S32 AlCan_Hal_RecvFrame(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame);
AL_S32 AlCan_Hal_SendFrameBlock(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame, AL_U32 Timeout);
AL_S32 AlCan_Hal_RecvFrameBlock(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame, AL_U32 Timeout);
AL_S32 AlCan_Hal_RecvFrameDma(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame);

#ifdef __cplusplus
}
#endif

#endif
