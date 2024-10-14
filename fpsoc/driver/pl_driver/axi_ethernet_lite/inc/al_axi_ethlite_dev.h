/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_AXI_ETHLITE_DEV_H_
#define __AL_AXI_ETHLITE_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_axi_ethlite_ll.h"

typedef struct
{
    AL_U8                           MacAddr[6];
    AL_AXI_ETHLITE_SpeedEnum        Speed;
} AL_AXI_ETHLITE_InitStruct;

typedef struct {
    AL_U8                   TxIfgVal;
    AL_U8                   RxEnable;
    AL_U8                   RxFlowCtlEnable;
    AL_U8                   RxLenTypChkEnable;
    AL_U8                   RxCtlFrameLenChkEnable;
    AL_U8                   RxJumboEnable;
    AL_U8                   RxFcsChkEnable;
    AL_U8                   RxVlanEnable;
    AL_U8                   RxHfDuplexEnable;
    AL_U8                   TxEnable;
    AL_U8                   TxFlowCtlEnable;
    AL_U8                   TxIfgAdjEnable;
    AL_U8                   TxJumboEnable;
    AL_U8                   TxFcsGenEnable;
    AL_U8                   TxVlanEnable;
    AL_U8                   TxHfDuplexEnable;
} AL_AXI_ETHLITE_DefaultInitStruct;

typedef enum
{
    AL_AXI_ETHLITE_EVENT_RX_VALID          = (0x1 << 0),
    AL_AXI_ETHLITE_EVENT_RX_IDLE           = (0x1 << 1),
    AL_AXI_ETHLITE_EVENT_TX_DONE           = (0x1 << 2),
    AL_AXI_ETHLITE_EVENT_RX_FRAME_LOSS     = (0x1 << 3),
    AL_AXI_ETHLITE_EVENT_RX_BAD_FRAME      = (0x1 << 4),
} AL_AXI_ETHLITE_EventIdEnum;

typedef struct
{
    AL_AXI_ETHLITE_EventIdEnum      Event;
    AL_U32                          EventData;
} AL_AXI_ETHLITE_EventStruct;

typedef AL_VOID (*AL_AXI_ETHLITE_EventCallBack)(AL_AXI_ETHLITE_EventStruct *Event, AL_VOID *CallbackRef);

typedef struct
{
    AL_AXI_ETHLITE_HwConfigStruct       HwConfig;
    AL_AXI_ETHLITE_InitStruct           InitConfig;
    AL_AXI_ETHLITE_EventCallBack        EventCallBack;
    AL_VOID                             *EventCallBackRef;

    AL_BOOL                             NextTxBufIsPong;
    AL_U16                              MaxTxPktSize;
    AL_BOOL                             NextRxBufIsPong;
    AL_U16                              MaxRxPktSize;
} AL_AXI_ETHLITE_DevStruct;

typedef enum
{
    AL_AXI_ETHLITE_PHY_READ_ERROR                    = 0x100,
    AL_AXI_ETHLITE_PHY_WRITE_ERROR                   = 0x101,
    AL_AXI_ETHLITE_PHY_NOT_DETECT                    = 0x102,
    AL_AXI_ETHLITE_ERR_PHY_UNSUPPORT                 = 0x103,

} AL_AXI_ETHLITE_ErrorCodeEnum;

#define AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM        (AL_DEF_ERR(AL_AXI_ETHLITE, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_AXI_ETHLITE_ERR_PHY_READ             (AL_DEF_ERR(AL_AXI_ETHLITE, AL_LOG_LEVEL_ERROR, AL_AXI_ETHLITE_PHY_READ_ERROR))
#define AL_AXI_ETHLITE_ERR_PHY_WRITE            (AL_DEF_ERR(AL_AXI_ETHLITE, AL_LOG_LEVEL_ERROR, AL_AXI_ETHLITE_PHY_WRITE_ERROR))
#define AL_AXI_ETHLITE_ERR_PHY_NOT_DETECT       (AL_DEF_ERR(AL_AXI_ETHLITE, AL_LOG_LEVEL_ERROR, AL_AXI_ETHLITE_PHY_NOT_DETECT))

#define AL_AXI_ETHLITE_MAX_FRAME_SIZE               1518

AL_AXI_ETHLITE_HwConfigStruct *AlAxiEthLite_Dev_LookupConfig(AL_U32 DevId);

AL_S32 AlAxiEthLite_Dev_Init(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_AXI_ETHLITE_HwConfigStruct *HwConfig,
                             AL_AXI_ETHLITE_InitStruct *InitConfig);

AL_S32 AlAxiEthLite_Dev_RegisterEventCallBack(AL_AXI_ETHLITE_DevStruct *AxiEth,
                                              AL_AXI_ETHLITE_EventCallBack Callback, void *CallbackRef);

AL_S32 AlAxiEthLite_Dev_SendFrame(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U8 *SendBuffer, AL_U16 SendLen);

AL_U16 AlAxiEthLite_Dev_RecvFrame(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U8 *RecvBuffer, AL_U16 *RecvLen);

AL_BOOL AlAxiEthLite_Dev_IsTxBufferAvailable(AL_AXI_ETHLITE_DevStruct *AxiEth);

AL_S32 AlAxiEthLite_Dev_SendFrameIntr(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U8 *SendBuffer, AL_U32 SendLen);

AL_U32 AlAxiEthLite_Dev_RecvFrameIntr(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U8 *RecvBuffer, AL_U16 *RecvLen);

AL_S32 AlAxiEthLite_Dev_EnableIntr(AL_AXI_ETHLITE_DevStruct *AxiEth);

AL_S32 AlAxiEthLite_Dev_DisableleIntr(AL_AXI_ETHLITE_DevStruct *AxiEth);

AL_VOID AlAxiEthLite_Dev_IntrHandler(AL_VOID *Instance);

AL_S32 AlAxiEthLite_Dev_ConfigDuplexAndSpeed(AL_AXI_ETHLITE_DevStruct *AxiEth,
                                             AL_AXI_ETHLITE_SpeedEnum Speed, AL_U8 Duplex);

AL_S32 AlAxiEthLite_Dev_WritePhyRegister(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U32 PHYAddr,
                                         AL_U32 PHYReg, AL_U16 RegValue);

AL_S32 AlAxiEthLite_Dev_ReadPhyRegister(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U32 PHYAddr,
                                        AL_U32 PHYReg, AL_U16 *RegValue);

#ifdef __cplusplus
}
#endif

#endif /* __AL_AXI_ETHLITE_DEV_H_ */
