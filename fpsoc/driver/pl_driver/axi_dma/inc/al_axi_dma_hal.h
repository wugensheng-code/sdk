/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_AXI_DMA_HAL_H_
#define __AL_AXI_DMA_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_axi_dma_dev.h"

/**************************** Type Definitions ******************************/
typedef struct
{
    AlAxiDma_DevStruct      Dma;
    AL_Lock                 Mm2s_Lock;
    AL_Lock                 S2mm_Lock;
    AL_MailBox              Mm2s_EventQueue;
    AL_MailBox              S2mm_EventQueue;
} AlAxiDma_HalStruct;

typedef struct {
    AL_U8                   **Buffers;
    AL_U32                  *BufferLengths;
    AL_U32                  NumBuffers;
    AL_U32                  S2mmCyclicCount;
    AL_U32                  Mm2sCyclicCount;
    AlAxiDma_TransDirEnum   Direction;
} ALAXIDMA_TransferMsg;

typedef struct {
        AL_U32              errorBit;
        const char          *errorMsg;
} ErrorInfo;

/************************** Function Prototypes ******************************/
AL_S32 AlAxiDma_Hal_Init(AlAxiDma_HalStruct **Dev, AL_U32 DevId, AlAxiDma_InitStruct *InitConfig, AlAxiDma_EventCallBack EventCallBack);
AL_S32 AlAxiDma_Hal_DirectMode_TransferBlock(AlAxiDma_HalStruct *Handle, AL_U8 *Buffer, AL_U32 Length, AL_U32 Direction, AL_U32 Timeout);
AL_S32 AlAxiDma_Hal_DirectMode_TransferPolling(AlAxiDma_HalStruct *Handle, AL_U8 *Buffer, AL_U32 Length, AL_U32 Direction);
AL_S32 AlAxiDma_Hal_SetupDescriptors(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg);
AL_S32 AlAxiDma_Hal_SgMode_TransferBlock(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg, AL_U32 Timeout);
AL_S32 AlAxiDma_Hal_SgMode_TransferPolling(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg);
AL_S32 AlAxiDma_Hal_SetDescAppWord(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg, AL_U32 Offset, AL_U32 Word);
AL_S32 AlAxiDma_Hal_GetDescAppWord(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg, AL_U32 Offset);

#ifdef __cplusplus
}
#endif

#endif
