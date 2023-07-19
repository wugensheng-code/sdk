#ifndef AL_QSPI_HAL_H
#define AL_QSPI_HAL_H

#ifdef __cplusplus
extern "C" {
#endif
#include "al_qspi_dev.h"

struct AL_QSPI_DevStruct;

typedef struct
{
    AL_QSPI_DevStruct            *Dev;

// #ifdef USE_RTOS
//     AL_Lock                      Lock;
// #endif

} AL_QSPI_HalStruct;

AL_S32 AlQspi_Hal_Init(AL_QSPI_HalStruct *Handle, AL_QSPI_ConfigsStruct *InitConfig, QSPI_EventCallBack Callback,
                       AL_VOID *CallbackRef,  AL_U32 DevId);
AL_S32 AlQspi_Hal_SendDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);
AL_S32 AlQspi_Hal_RecvDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);
AL_S32 AlQspi_Hal_TranferDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                   AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout);
AL_S32 AlQspi_Hal_IoCtl(AL_QSPI_HalStruct *Handle, AL_Qspi_IoCtlCmdEnum Cmd, AL_VOID *Data);

#ifdef __cplusplus
}
#endif
#endif
