#ifndef AL_MPU_HAL_H
#define AL_MPU_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_iic_dev.h"

typedef struct
{
    AL_IIC_DevStruct    *Dev;

#ifdef USE_RTOS
    AL_Lock             Lock;
#endif
} AL_IIC_HalStruct;

AL_S32 AlIic_Hal_Init(AL_IIC_HalStruct *Handle, AL_U32 DevId,
                      AL_IIC_InitStruct *InitConfig, AL_IIC_EventCallBack Callback);

AL_S32 AlIic_Hal_MasterSendDataBlock(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlIic_Hal_MasterRecvDataBlock(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlIic_Hal_SlaveSendDataBlock(AL_IIC_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlIic_Hal_SlaveRecvDataBlock(AL_IIC_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_HAL_H */
