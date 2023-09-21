#include "al_core.h"
#include "al_dma_dev.h"

typedef struct {
    AL_DMA_ChStruct     *Chan;
    AL_DMA_DevStruct    *Dev;
    AL_Lock             Lock;
    AL_MailBox          EventQueue;
} AL_DMA_HalStruct;


AL_S32 AlDma_Hal_Init(AL_DMA_HalStruct **Handle, AL_U32 DevId, AL_DMA_ChCfgStruct *Initconfig);

AL_S32 AlDma_Hal_MemCpyBlock(AL_DMA_HalStruct *Handle, AL_UINTPTR Dst, AL_UINTPTR Src, AL_U32 Len);

AL_S32 AlDma_Hal_PeriCpySingleBlock(AL_DMA_HalStruct *Handle, AL_UINTPTR MemAddr, AL_U32 Len, AL_U32 PerId);

AL_S32 AlDma_Hal_PeriCpyCycleBlock(AL_DMA_HalStruct *Handle, AL_UINTPTR MemAddr, AL_U32 Len, AL_U32 PerId);

AL_S32 AlDma_Hal_MemCpy(AL_UINTPTR Dst, AL_UINTPTR Src, AL_U32 Len);

AL_S32 AlDma_Hal_PeriCpySingle(AL_DMA_HalStruct *Handle, AL_UINTPTR MemAddr, AL_U32 Len, AL_U32 PerId);

AL_S32 AlDma_Hal_PeriCpyCycle(AL_DMA_HalStruct *Handle, AL_UINTPTR MemAddr, AL_U32 Len, AL_U32 PerId);

AL_S32 AlDma_Hal_StopCpy(AL_DMA_HalStruct *Handle);