/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_IIC_DEV_H_
#define AL_AXI_IIC_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_errno.h"
#include "al_axi_iic_ll.h"

#define AXI_IIC_ID 0x020210

typedef enum
{
    AL_AXI_IIC_IDLE             = 0,
    AL_AXI_IIC_SLAVE_RX_DONE    = 1,
    AL_AXI_IIC_SLAVE_TX_DONE    = 2,
    AL_AXI_IIC_SLAVE_RX_ING     = 3,
    AL_AXI_IIC_SLAVE_TX_ING     = 4,
    AL_AXI_IIC_MASTER_TX_ING    = 5,
    AL_AXI_IIC_MASTER_RX_ING    = 6,
    AL_AXI_IIC_MASTER_TX_DONE   = 7,
    AL_AXI_IIC_MASTER_RX_DONE   = 8
    
} AlAxiIic_StatusEnum;

typedef struct
{
    AlAxiIic_FunctionEnum         PhaseStart;
    AlAxiIic_FunctionEnum         PhaseAddr;
    AlAxiIic_FunctionEnum         PhaseData;
    AlAxiIic_FunctionEnum         PhaseStop;
    AL_U32                          SlaveAddr;
} AlAxiIic_TransactionStruct;

typedef struct
{
    AL_U8   *BuffPtr;
    AL_U32  DataCnt;
    AL_U32  ReqCnt;
} AlAxiIic_BufferStruct;

typedef enum
{
    AL_AXI_IIC_STANDARD_MODE        = 0,
    AL_AXI_IIC_FAST_MODE            = 1,
    AL_AXI_IIC_HIGH_SPEED_MODE      = 2
} AlAxiIic_SpeedModeEnum;

typedef struct
{
    AlAxiIic_ModeEnum               IIC_Mode;
    AlAxiIic_AddressModeEnum        IIC_AddressMode;
    AlAxiIic_SpeedModeEnum          SpeedMode;          // master mode only
    AL_U32                          Taclk;
    AL_U16                          SlaveAddr;          // slave mode only
} AlAxiIic_InitStruct;

typedef enum
{
    EVENT_RX_DONE     = 0,
    EVENT_TX_DONE     = 1,
} AlAxiIic_EventEnum;

typedef struct
{
    AlAxiIic_EventEnum      Event;
    AL_U32                  EventData;
} AlAxiIic_EventStruct;

typedef (*AlAxiIic_EventCallBack)(AlAxiIic_EventStruct *Event, AL_VOID *CallbackRef);

typedef struct
{
    AlAxiIic_HwConfigStruct         HwConfig;
    AlAxiIic_InitStruct             InitConfig;
    AL_U32                          FifoSize;
    AlAxiIic_BufferStruct           RecvBuffer;
    AlAxiIic_BufferStruct           TransBuffer;
    AlAxiIic_StatusEnum             Status;
    AlAxiIic_EventCallBack          EventCallBack;
    AL_VOID                         *EventCallBackRef;
} AlAxiIic_DevStruct;

typedef enum
{
    AL_AXI_IIC_STOP ,
    AL_AXI_IIC_NSTOP
} AlAxiIic_StopModeEnum;

AL_S32 AlAxiIic_Dev_CheckID(AlAxiIic_DevStruct *Iic);
AL_S32 AlAxiIic_Dev_Init(AlAxiIic_DevStruct *Iic, AlAxiIic_HwConfigStruct *HwConfig, AlAxiIic_InitStruct *InitConfig);
AL_S32 AlAxiIic_Dev_MasterSendData(AlAxiIic_DevStruct *Iic, AL_U32 SlaveAddr, AL_U8 *SendBuf, AL_U32 SendSize);
AL_S32 AlAxiIic_Dev_MasterRecvData(AlAxiIic_DevStruct *Iic, AL_U32 SlaveAddr, AL_U8 *RecvBuf, AL_U32 RecvSize);
AL_S32 AlAxiIic_Dev_SlaveSendData(AlAxiIic_DevStruct *Iic, AL_U8 *SendBuf, AL_U32 SendSize);
AL_S32 AlAxiIic_Dev_SlaveRecvData(AlAxiIic_DevStruct *Iic, AL_U8 *RecvBuf);
AL_S32 AlAxiIic_Dev_StartTrans(AlAxiIic_DevStruct *Iic);
AL_S32 AlAxiIic_Dev_Reset(AlAxiIic_DevStruct *Iic);
AL_S32 AlAxiIic_Dev_ClrFifo(AlAxiIic_DevStruct *Iic);
AL_VOID AlAxiIic_Dev_IrqHandler(AL_VOID *Instance);
AL_VOID AlAxiIic_Dev_CreateTrans(AlAxiIic_DevStruct *Iic, AlAxiIic_StopModeEnum StopMode);
AL_S32 AlAxiIic_Dev_SetSpeedMode(AL_U32 BaseAddr, AlAxiIic_SpeedModeEnum SpeedMode, AL_U32 Taclk);
AL_S32 AlAxiIic_Dev_RegisterEventCallBack(AlAxiIic_DevStruct *Handle, AlAxiIic_EventCallBack CallBack, AL_VOID *CallBackRef);

#ifdef __cplusplus
}
#endif

#endif