/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_AXI_DMA_DEV_H_
#define __AL_AXI_DMA_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_axi_dma_ll.h"
/**************************** Type Definitions ******************************/
typedef enum
{
    EVENT_S2MM_DONE     = 0,
    EVENT_MM2S_DONE     = 1,
    EVENT_S2MM_ERROR    = 2,
    EVENT_MM2S_ERROR    = 3,
    EVENT_S2MM_CYCLIC   = 4,
    EVENT_MM2S_CYCLIC   = 5,
} AlAxiDma_EventEnum;

typedef enum {
    DMA_INTERNAL_ERROR = 1UL << 4,
    DMA_SLAVE_ERROR    = 1UL << 5,
    DMA_DECODE_ERROR   = 1UL << 6,
    SG_INTERNAL_ERROR  = 1UL << 8,
    SG_SLAVE_ERROR     = 1UL << 9,
    SG_DECODE_ERROR    = 1UL << 10,
} AlAxiDma_ErrorState;

typedef struct
{
    AlAxiDma_EventEnum      Event;
    AL_U32                  EventData;
} AlAxiDma_EventStruct;

typedef (*AlAxiDma_EventCallBack)(AlAxiDma_EventStruct *Event, AL_VOID *CallbackRef);

typedef struct {
    AL_U32 next_desc;        // Pointer to the next descriptor
    AL_U32 next_desc_msb;    // Most significant 32 bits of the next descriptor pointer
    AL_U32 buffer_addr;      // Address of the buffer
    AL_U32 buffer_addr_msb;  // Most significant 32 bits of the buffer address
    AL_U32 reserved1;        // Reserved for future use or alignment
    AL_U32 reserved2;        // Reserved for future use or alignment
    AL_U32 control;          // Control field to manage DMA operations
    AL_U32 status;           // Status field to indicate DMA operation status
    AL_U32 app[5];           // Application-specific fields
    AL_U32 padding[3];       // Padding fields to ensure the total size is 64 bytes
} AlAxiDma_Descriptor;

typedef struct {
    AL_U32                     DeviceId;
    AL_U32                     S2mm_HasCyclic;
    AL_U32                     S2mm_HasKeyHole;
    AL_U32                     S2mm_Threshold;
    AL_U32                     Mm2s_HasCyclic;
    AL_U32                     Mm2s_HasKeyHole;
    AL_U32                     Mm2s_Threshold;
    AlAxiDma_Descriptor        *descriptors;
} AlAxiDma_InitStruct;

typedef struct {
    AlAxiDma_HwConfigStruct     HwConfig;
    AlAxiDma_InitStruct         InitConfig;
    AlAxiDma_EventCallBack      EventCallBack;
    AL_VOID                     *EventCallBackRef;
    AL_U64                      RegBase;
    AL_U32                      S2mmCyclicCount;
    AL_U32                      S2mmCyclicCompleted;
    AL_U32                      Mm2sCyclicCount;
    AL_U32                      Mm2sCyclicCompleted;
    AlAxiDma_Descriptor         *descriptors;
} AlAxiDma_DevStruct;

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_AxiDma_EVENT_START_BIT    6
#define AL_AxiDma_ERR_NULL_PTR                 (AL_DEF_ERR(AL_AxiDma, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_AxiDma_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_AxiDma, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_AxiDma_ERR_BUSY                     (AL_DEF_ERR(AL_AxiDma, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY))
#define AL_AxiDma_ERR_NOT_SUPPORT              (AL_DEF_ERR(AL_AxiDma, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT))
#define AL_AxiDma_EVENTS_TO_ERRS(Events)       (AL_DEF_ERR(AL_AxiDma, AL_LOG_LEVEL_ERROR, Events << AL_AxiDma_EVENT_START_BIT))

/************************** Function Prototypes ******************************/
AlAxiDma_HwConfigStruct *AlAxiDma_Dev_LookupConfig(AL_U32 DevId);
AL_S32 AlAxiDma_Dev_Init(AlAxiDma_DevStruct *Dma, AlAxiDma_HwConfigStruct *HwConfig, AlAxiDma_InitStruct *InitConfig);
AL_S32 AlAxiDma_Dev_DirectMode_Transfer(AlAxiDma_DevStruct *Dma, AL_U8 *Buffer, AL_U32 Length, AlAxiDma_TransDirEnum Direction, AlAxiDma_TransferMethodEnum Method);
AL_S32 AlAxiDma_Dev_DirectMode_Start(AlAxiDma_DevStruct *Dma, AL_U8 *Buffer, AL_U32 Length, AlAxiDma_TransDirEnum Direction, AlAxiDma_TransferMethodEnum Method);
AL_VOID AlAxiDma_Dev_Reset(AlAxiDma_DevStruct *InstancePtr);
AL_BOOL AlAxiDma_Dev_Busy(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction);
AL_S32 AlAxiDma_Dev_ResetIsDone(AlAxiDma_DevStruct *Dma);
AL_S32 AlAxiDma_Dev_RegisterEventCallBack(AlAxiDma_DevStruct *Dma, AlAxiDma_EventCallBack CallBack, AL_VOID *CallBackRef);
AL_VOID AlAxiDma_Dev_Stop_Transfer(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction);
AL_VOID AlAxiDma_Dev_Transfer_DoneHandler(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction);
AL_VOID AlAxiDma_Dev_Transfer_ErrorHandler(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction);
AL_VOID AlAxiDma_Dev_Mm2s_IntrHandler(AL_VOID *Instance);
AL_VOID AlAxiDma_Dev_S2mm_IntrHandler(AL_VOID *Instance);
AL_S32 AlAxiDma_Dev_SetupDescriptors(AlAxiDma_DevStruct *Dma, AL_U32 Index, AL_U8 *Buffer, AL_U32 Length, AL_U32 NumBuffers, AlAxiDma_TransDirEnum Direction);
AL_S32 AlAxiDma_Dev_SgMode_Transfer(AlAxiDma_DevStruct *Dma, AL_U32 NumBuffers, AlAxiDma_TransDirEnum Direction, AlAxiDma_TransferMethodEnum Method);

#ifdef __cplusplus
}
#endif

#endif
