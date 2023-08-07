#ifndef AL_GBE_HAL_H
#define AL_GBE_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_gbe_dev.h"

typedef struct
{
    AL_GBE_DevStruct    *Dev;

#if 0
    AL_Mutex                     TxLock;
    AL_Mutex                     RxLock; 
    AL_Semaphore                 TxDoneSem;
    AL_Semaphore                 RxDoneSem;
#endif
} AL_GBE_HalStruct;

AL_S32 AlGbe_Hal_Init(AL_GBE_HalStruct *Handle, AL_U32 DevId, AL_GBE_InitStruct *InitConfig,
                      AL_GBE_MacDmaConfigStruct *MacDmaConfig, AL_GBE_EventCallBack Callback);

AL_S32 AlGbe_Hal_ConfigRxDescBuffer(AL_GBE_HalStruct *Handle, AL_U32 DescIndex, AL_U8 *Buffer1Addr, AL_U8 *Buffer2Addr);

AL_S32 AlGbe_Hal_PhyInit(AL_GBE_HalStruct *Handle, AL_U8 PhyAddr);

AL_S32 AlGbe_Hal_ConfigDuplexAndSpeed(AL_GBE_HalStruct *Handle);

AL_S32 AlGbe_Hal_StartMacDmaIntr(AL_GBE_HalStruct *Handle);

AL_S32 AlGbe_Hal_StartMacDma(AL_GBE_HalStruct *Handle);

AL_S32 AlGbe_Hal_TransmitBlock(AL_GBE_HalStruct *Handle, AL_GBE_TxDescConfigStruct *TxConfig, AL_U32 Timeout);

AL_S32 AlGbe_Hal_Transmit(AL_GBE_HalStruct *Handle, AL_GBE_TxDescConfigStruct *TxConfig);

AL_S32 AlGbe_Hal_GetRxDataBuffer(AL_GBE_HalStruct *Handle, AL_GBE_BufferStruct *RxBuffer);

AL_S32 AlGbe_Hal_GetRxDataLength(AL_GBE_HalStruct *Handle, AL_U32 *Length);

AL_S32 AlGbe_Hal_BuildRxDescriptors(AL_GBE_HalStruct *Handle);

AL_S32 AlGbe_Hal_ReleaseTxPacket(AL_GBE_HalStruct *Handle);

#ifdef __cplusplus
}
#endif

#endif /* AL_GBE_HAL_H */