/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_VDMA_DEV_H_
#define AL_VDMA_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_vdma_ll.h"



#define AL_AXI_VDMA_ERR_NULL_PTR                 (AL_DEF_ERR(AL_VDMA, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_AXI_VDMA_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_VDMA, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_AXI_VDMA_ERR_NOT_READY                (AL_DEF_ERR(AL_VDMA, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY))
#define AL_AXI_VDMA_ERR_NOT_SUPPORT              (AL_DEF_ERR(AL_VDMA, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT))
#define AL_AXI_VDMA_ERR_TIMEOUT                  (AL_DEF_ERR(AL_VDMA, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT))
#define AL_AXI_VDMA_ERR_BUSY                     (AL_DEF_ERR(AL_VDMA, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY))
#define AL_AXI_VDMA_ERR_NOMEM                    (AL_DEF_ERR(AL_VDMA, AL_LOG_LEVEL_ERROR, AL_ERR_NOMEM))
#define AL_AXI_VDMA_ERR_NOMEM                    (AL_DEF_ERR(AL_VDMA, AL_LOG_LEVEL_ERROR, AL_ERR_NOMEM))

#define AL_AXIVDMA_MAX_FRAMESTORE 16


typedef enum
{
    AL_VDMA_MM2S_CHANNEL     = 0,
    AL_VDMA_S2MM_CHANNEL     = 1,
} AlAxiVdma_ChannelEnum;

typedef enum
{
    MM2S_INTR_FRM_CNT            = 12,
    MM2S_INTR_DLY_CNT            = 13,
    MM2S_INTR_ERROR              = 14,
} AlAxiVdma_Mm2sIntrEnum;

typedef enum
{
    S2MM_INTR_FRM_CNT            = 12,
    S2MM_INTR_DLY_CNT            = 13,
    S2MM_INTR_ERROR              = 14,
} AlAxiVdma_S2MmIntrEnum;

typedef enum
{
    MaskIrqSofEarlyErr = 0,
    MaskIrqEolEarlyErr = 1,
    MaskIrqSofLateErr = 2,
    MaskIrqEolLateErr = 3,
} AlAxiVdma_S2MmMaskIntrEnum;

typedef enum
{
    GenlockMode_Master = 0,
    GenlockMode_Slave = 1,
    GenlockMode_DynamicMaster = 2,
    GenlockMode_DynamicSlave = 3,
} AlAxiVdma_GenlockModeEnum;

typedef struct {
    AL_U32  DelayCount;
    AL_U32  FrameCount;
    AL_BOOL  RepeatEn;
    AL_BOOL  ErrIrqEn;
    AL_BOOL  DlyCntIrqEn;
    AL_BOOL  FrmCntIrqEn;
    AL_U32  PntrNum;
    AL_U32  GenlockSrc;
    AL_BOOL  FrameCntEn;
    AL_BOOL  GenlockEn;
    AL_U32  CircularPark;
    AL_U32   RegIndex;
    AL_UINTPTR FrameStoreStartAddr[AL_AXIVDMA_MAX_FRAMESTORE];
    AL_U32  Vsize;
    AL_U32  Hsize;
    AL_U32  FrameDelay;
    AL_U32  Stride;
    AL_BOOL      EnableVerticalFlip;
}AlAxiVdma_ChannelCfg;

typedef enum
{
    AlAxiVdma_Event_Mm2s_FRAMECNT     = 0,
    AlAxiVdma_Event_Mm2s_ERRIRQ       = 1,
    AlAxiVdma_Event_S2Mm_FRAMECNT     = 2,
    AlAxiVdma_Event_S2Mm_ERRIRQ       = 3,
} AlAxiVdma_EventIdEnum;

typedef struct
{
    AlAxiVdma_EventIdEnum       Events;
    AL_U32                      EventData;
} AlAxiVdma_EventStruct;

typedef (*AlAxiVdma_EventCallBack)(AlAxiVdma_EventStruct Event, AL_VOID *CallbackRef);

typedef struct
{
    AlAxiVdma_HwConfigStruct     HwConfig;
    AlAxiVdma_EventCallBack      EventCallBack;
    AL_VOID                     *EventCallBackRef;
} AlAxiVdma_Dev_Struct;



AL_S32 AlAxiVdma_Dev_Init(AlAxiVdma_Dev_Struct *Vdma, AL_U32 DevId);
AL_U32 AlAxiVdma_Dev_StartMm2s(AlAxiVdma_Dev_Struct *Vdma, AlAxiVdma_ChannelCfg *ReadChannelCfg);
AL_U32 AlAxiVdma_Dev_SetMm2sConfig(AlAxiVdma_Dev_Struct *Vdma, AlAxiVdma_ChannelCfg *ReadChannelCfg);
AL_U32 AlAxiVdma_Dev_StartS2Mm(AlAxiVdma_Dev_Struct *Vdma, AlAxiVdma_ChannelCfg *WriteChannelCfg);
AL_U32 AlAxiVdma_Dev_SetS2MmConfig(AlAxiVdma_Dev_Struct *Vdma, AlAxiVdma_ChannelCfg *WriteChannelCfg);
AL_VOID AlAxiVdma_Dev_Mm2sIntrHandler(AL_VOID *Instance);
AL_VOID AlAxiVdma_Dev_S2MmIntrHandler(AL_VOID *Instance);
AL_S32 AlAxiVdma_Dev_RegisterEventCallBack(AlAxiVdma_Dev_Struct *Vdma, AlAxiVdma_EventCallBack Callback, void *CallbackRef);
AL_S32 AlAxiVdma_Dev_UnRegisterEventCallBack(AlAxiVdma_Dev_Struct *Vdma);


#ifdef __cplusplus
}
#endif

#endif
