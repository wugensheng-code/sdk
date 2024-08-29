/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_axi_qspi_dev.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2024/08/27
 * @brief   axi-qspi device driver
 */

#ifndef __AL_AXI_QSPI_DEV_H__
#define __AL_AXI_QSPI_DEV_H__

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_axi_qspi_hw.h"
#include "al_type.h"
#include "al_errno.h"

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_AXI_QSPI_ERR_ILLEGAL_PARAM        AL_DEF_ERR(AL_AxiQspi, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM)
#define AL_AXI_QSPI_ERR_BUSY                 AL_DEF_ERR(AL_AxiQspi, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY)
#define AL_AXI_QSPI_ERR_TIMEOUT              AL_DEF_ERR(AL_AxiQspi, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT)
#define AL_AXI_QSPI_ERR_NOT_SUPPORT          AL_DEF_ERR(AL_AxiQspi, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT)
#define AL_AXI_QSPI_ERR_NOT_READY            AL_DEF_ERR(AL_AxiQspi, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY)
#define AL_AXI_QSPI_EVENTS_TO_ERRS(Event)    (AL_DEF_ERR(AL_AxiQspi, AL_LOG_LEVEL_ERROR, (Event << AL_ERR_MAX)))

/**
 * @brief  Define some events for axi-qspi
 */
typedef enum
{
    AL_AXI_QSPI_EVENT_MODF            = BIT(0),
    AL_AXI_QSPI_EVENT_SLV_MODF        = BIT(1),
    AL_AXI_QSPI_EVENT_DTR_EMPTY       = BIT(2),
    AL_AXI_QSPI_EVENT_DTR_UNDER       = BIT(3),
    AL_AXI_QSPI_EVENT_DRR_FULL        = BIT(4),
    AL_AXI_QSPI_EVENT_DRR_OVER        = BIT(5),
    AL_AXI_QSPI_EVENT_TX_HALF         = BIT(6),
    AL_AXI_QSPI_EVENT_SLV_SEL         = BIT(7),
    AL_AXI_QSPI_EVENT_DRR_NOT_EMPTY   = BIT(8),
    AL_AXI_QSPI_EVENT_CPOL_CPHA_ERR   = BIT(9),
    AL_AXI_QSPI_EVENT_SLV_MODE_ERR    = BIT(10),
    AL_AXI_QSPI_EVENT_MSB_ERR         = BIT(11),
    AL_AXI_QSPI_EVENT_LOOPBACK_ERR    = BIT(12),
    AL_AXI_QSPI_EVENT_CMD_ERR         = BIT(13),


    AL_AXI_QSPI_EVENT_TX_DONE         = BIT(14),
    AL_AXI_QSPI_EVENT_RX_DONE         = BIT(15),
} AlAxiQspi_EventIdEnum;

/**
 * @brief  Define the state of axi-qspi
 */
typedef enum
{
    AL_AXI_QSPI_STATE_NOT_INIT = 0,
    AL_AXI_QSPI_STATE_READY    = BIT(0),
    AL_AXI_QSPI_STATE_TX_BUSY  = BIT(1),
    AL_AXI_QSPI_STATE_RX_BUSY  = BIT(2),
    AL_AXI_QSPI_STATE_ERROR    = BIT(3),
} AlAxiQspi_StateEnum;


/**************************** Type Definitions *******************************/
/**
 * @brief  Define the message structure for the mailbox used by axi-qspi
 */
typedef struct
{
    AlAxiQspi_EventIdEnum       Events;
    AL_U32                      EventData;
} AlAxiQspi_EventStruct;

/**
 * @brief  Define the callback handle type for events
 */
typedef AL_VOID (*AxiQspi_EventCallBack)(AlAxiQspi_EventStruct QspiEvent, AL_VOID *CallbackRef);

/**
 * @brief  Define the init configuration structure for axi-qspi
 */
typedef struct
{
    AL_U32 IsLoopback;
    AL_U32 IsMaster;
    AL_U32 CPHA;
    AL_U32 CPOL;
    AL_U32 ManualSlvSel;       /* Only for standard mode */
    AL_U32 LsbFirst;           /* Only for standard mode */
} AlAxiQspi_InitConfigStruct;

/**
 * @brief  Define a structure for transfer.
 */
typedef struct
{
    AL_U8 *SendBufPtr;
    AL_U32 SendSize;              /* The size that user want to be sent */
    AL_U32 SendCnt;

    AL_U8 *RecvBufPtr;
    AL_U32 RecvSize;              /* The size that user want to be received */
    AL_U32 RecvCnt;

    AL_U32 DummySize;             /* Used for future */

    AL_U32 TotalTransSize;        /* Total size that the axi-qspi sent or received */
    AL_U32 TotalSendCnt;
    AL_U32 TotalRecvCnt;

    AL_U32 TargetSlave;           /* Slave to be selected */
} AlAxiQspi_TransInfoStruct;

/**
 * @brief  Define the device structure for axi-qspi
 */
typedef struct
{
    AlAxiQspi_HwConfigStruct HwConfig;
    AlAxiQspi_InitConfigStruct InitConfig;
    
    AlAxiQspi_TransInfoStruct TransInfo;
    AxiQspi_EventCallBack EventCallBack;
    AL_VOID *EventCallBackRef;

    volatile AlAxiQspi_StateEnum State;
    AL_U32 IntrStatus;
} AlAxiQspi_DevStruct;


/************************** Function Prototypes ******************************/
AlAxiQspi_HwConfigStruct *AlAxiQspi_Dev_LookupConfig(AL_U32 DevId);
AL_VOID AlAxiQspi_Dev_IntrHandler(AL_VOID *Instance);
AL_U32 AlAxiQspi_Dev_Init(AlAxiQspi_DevStruct *Dev, AlAxiQspi_HwConfigStruct *HwConfig, AlAxiQspi_InitConfigStruct *InitConfig);
AL_S32 AlAxiQspi_Dev_RegisterEventCallBack(AlAxiQspi_DevStruct *Dev, AxiQspi_EventCallBack CallBack, AL_VOID *CallBackRef);
AL_U32 AlAxiQspi_Dev_SendData(AlAxiQspi_DevStruct *Dev, AL_U8 *SendBuf, AL_U32 SendSize);
AL_U32 AlAxiQspi_Dev_TransferDataPolled(AlAxiQspi_DevStruct *Dev, AL_U8 *SendBuf, AL_U32 SendSize, 
                                        AL_U8 *RecvBuf, AL_U32 RecvSize);
AL_U32 AlAxiQspi_Dev_TransferData(AlAxiQspi_DevStruct *Dev, AL_U8 *SendBuf, AL_U32 SendSize, 
                                  AL_U8 *RecvBuf, AL_U32 RecvSize);

#ifdef __cplusplus
}
#endif

#endif