#ifndef AL_MMC_HAL_H
#define AL_MMC_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_mmc_dev.h"

typedef struct
{
    AL_MMC_DevStruct    *Dev;

#ifdef USE_RTOS
    AL_Lock             Lock;
#endif
} AL_MMC_HalStruct;

AL_S32 AlMmc_Hal_Init(AL_MMC_HalStruct *Handle, AL_MMC_InitStruct *InitConfig, AL_MMC_CallBackStruct *CallBack,
                      AL_U32 DevId);

AL_S32 AlMmc_Hal_Write(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt);

AL_S32 AlMmc_Hal_WriteBlocked(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt, AL_U32 Timeout);

AL_S32 AlMmc_Hal_Read(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt);

AL_S32 AlMmc_Hal_ReadBlocked(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt, AL_U32 Timeout);

AL_S32 AlMmc_Hal_IoCtl(AL_MMC_HalStruct *Handle, AL_MMC_IoCtlCmdEnum Cmd, AL_VOID *Data);

#ifdef __cplusplus
}
#endif

#endif
