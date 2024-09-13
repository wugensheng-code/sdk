/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_AXI_ETHLITE_LL_H_
#define __AL_AXI_ETHLITE_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_axi_ethlite_hw.h"

typedef enum
{
    AL_AXI_ETHLITE_FUNC_DISABLE        = 0,
    AL_AXI_ETHLITE_FUNC_ENABLE         = 1
} AL_AXI_ETHLITE_FunctionEnum;

typedef enum
{
    AL_AXI_ETHLITE_SPEED_10M           = 0x0,
    AL_AXI_ETHLITE_SPEED_100M          = 0x1,
    AL_AXI_ETHLITE_SPEED_AUTONEG       = 0x2
} AL_AXI_ETHLITE_SpeedEnum;

typedef enum
{
    AL_AXI_ETHLITE_MDIO_CMD_WRITE      = 0x1,
    AL_AXI_ETHLITE_MDIO_CMD_READ       = 0x2
} AL_AXI_ETHLITE_MdioCmdEnum;

typedef enum
{
    AL_AXI_ETHLITE_INTR_RX_VALID       = (0x1 << 0),
    AL_AXI_ETHLITE_INTR_TX_IDLE        = (0x1 << 1),
    AL_AXI_ETHLITE_INTR_TX_DONE        = (0x1 << 2),
    AL_AXI_ETHLITE_INTR_RX_FRAME_LOSS  = (0x1 << 6),
    AL_AXI_ETHLITE_INTR_RX_BAD_FRAME   = (0x1 << 7),
} AL_AXI_ETHLITE_IntrEventEnum;

static inline AL_U32 AlAxiEthLite_ll_GetRxPingBufferPtr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AXI_ETHLITE_RX_PING_BUFFER_PTR_OFFSET);
}

static inline AL_VOID AlAxiEthLite_ll_SetRxPingBufferPtr(AL_REG BaseAddr, AL_U32 Status)
{
    AL_REG32_WRITE(BaseAddr + AXI_ETHLITE_RX_PING_BUFFER_PTR_OFFSET, Status | (1 << 30));
}

static inline AL_U32 AlAxiEthLite_ll_GetRxPongBufferPtr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AXI_ETHLITE_RX_PONG_BUFFER_PTR_OFFSET);
}

static inline AL_VOID AlAxiEthLite_ll_SetRxPongBufferPtr(AL_REG BaseAddr, AL_U32 Status)
{
    AL_REG32_WRITE(BaseAddr + AXI_ETHLITE_RX_PONG_BUFFER_PTR_OFFSET, Status | (1 << 30));
}

static inline AL_VOID AlAxiEthLite_ll_ClrRxPingBufferFrameFlag(AL_REG BaseAddr)
{
    AL_REG32_WRITE(BaseAddr + AXI_ETHLITE_RX_PING_BUFFER_PTR_OFFSET,
                  (1 << AXI_ETHLITE_RX_PING_BUFFER_CLR_FRAME_FLAG_SHIFT));
}

static inline AL_BOOL AlAxiEthLite_ll_IsRxPingBufferRecvFrame(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AXI_ETHLITE_RX_PING_BUFFER_PTR_OFFSET,
                            AXI_ETHLITE_RX_PING_BUFFER_RECV_FRAME_FLAG_SHIFT);
}

static inline AL_U16 AlAxiEthLite_ll_GetRxPingBufferRecvFrameLen(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + AXI_ETHLITE_RX_PING_BUFFER_PTR_OFFSET,
                      AXI_ETHLITE_RX_PING_BUFFER_RECV_FRAME_LEN_SHIFT, AXI_ETHLITE_RX_PING_BUFFER_RECV_FRAME_LEN_SIZE);
}

static inline AL_BOOL AlAxiEthLite_ll_IsRxPingBufferRecvBadFrame(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AXI_ETHLITE_RX_PING_BUFFER_PTR_OFFSET,
                            AXI_ETHLITE_RX_PING_BUFFER_BAD_FRAME_FLAG_SHIFT);
}

static inline AL_VOID AlAxiEthLite_ll_ClrRxPongBufferFrameFlag(AL_REG BaseAddr)
{
    AL_REG32_WRITE(BaseAddr + AXI_ETHLITE_RX_PONG_BUFFER_PTR_OFFSET,
                  (1 << AXI_ETHLITE_RX_PONG_BUFFER_CLR_FRAME_FLAG_SHIFT));
}

static inline AL_BOOL AlAxiEthLite_ll_IsRxPongBufferRecvFrame(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AXI_ETHLITE_RX_PONG_BUFFER_PTR_OFFSET,
                            AXI_ETHLITE_RX_PONG_BUFFER_RECV_FRAME_FLAG_SHIFT);
}

static inline AL_U16 AlAxiEthLite_ll_GetRxPongBufferRecvFrameLen(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + AXI_ETHLITE_RX_PONG_BUFFER_PTR_OFFSET,
                      AXI_ETHLITE_RX_PONG_BUFFER_RECV_FRAME_LEN_SHIFT, AXI_ETHLITE_RX_PONG_BUFFER_RECV_FRAME_LEN_SIZE);
}

static inline AL_BOOL AlAxiEthLite_ll_IsRxPongBufferRecvBadFrame(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AXI_ETHLITE_RX_PONG_BUFFER_PTR_OFFSET,
                            AXI_ETHLITE_RX_PONG_BUFFER_BAD_FRAME_FLAG_SHIFT);
}

static inline AL_U32 AlAxiEthLite_ll_GetTxPingBufferPtr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AXI_ETHLITE_TX_PING_BUFFER_PTR_OFFSET);
}

static inline AL_BOOL AlAxiEthLite_ll_IsTxPingBufferBusy(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AXI_ETHLITE_TX_PING_BUFFER_PTR_OFFSET, AXI_ETHLITE_TX_PING_BUFFER_TX_BUSY_SHIFT);
}

static inline AL_VOID AlAxiEthLite_ll_SetTxPingBufferFrameLen(AL_REG BaseAddr, AL_U16 FrameLen)
{
    AL_REG32_SET_BITS(BaseAddr + AXI_ETHLITE_TX_PING_BUFFER_PTR_OFFSET, AXI_ETHLITE_TX_PING_BUFFER_FRAME_LEN_SHIFT,
                      AXI_ETHLITE_TX_PING_BUFFER_FRAME_LEN_SIZE, FrameLen);
}

static inline AL_VOID AlAxiEthLite_ll_EnableTxPingBufferSend(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_TX_PING_BUFFER_PTR_OFFSET,
                     AXI_ETHLITE_TX_PING_BUFFER_TX_ENABLE_SHIFT, AL_AXI_ETHLITE_FUNC_ENABLE);
}

static inline AL_VOID AlAxiEthLite_ll_SetTxPingBufferFrameLenAndEnableTx(AL_REG BaseAddr, AL_U32 FrameLen)
{
    AL_REG32_WRITE(BaseAddr + AXI_ETHLITE_TX_PING_BUFFER_PTR_OFFSET,
                  (FrameLen | (1 << AXI_ETHLITE_TX_PONG_BUFFER_TX_ENABLE_SHIFT)));
}

static inline AL_U32 AlAxiEthLite_ll_GetTxPongBufferPtr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AXI_ETHLITE_TX_PONG_BUFFER_PTR_OFFSET);
}

static inline AL_BOOL AlAxiEthLite_ll_IsTxPongBufferBusy(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AXI_ETHLITE_TX_PONG_BUFFER_PTR_OFFSET, AXI_ETHLITE_TX_PONG_BUFFER_TX_BUSY_SHIFT);
}

static inline AL_VOID AlAxiEthLite_ll_SetTxPongBufferFrameLen(AL_REG BaseAddr, AL_U16 FrameLen)
{
    AL_REG32_SET_BITS(BaseAddr + AXI_ETHLITE_TX_PONG_BUFFER_PTR_OFFSET, AXI_ETHLITE_TX_PONG_BUFFER_FRAME_LEN_SHIFT,
                      AXI_ETHLITE_TX_PONG_BUFFER_FRAME_LEN_SIZE, FrameLen);
}

static inline AL_VOID AlAxiEthLite_ll_EnableTxPongBufferSend(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_TX_PONG_BUFFER_PTR_OFFSET,
                     AXI_ETHLITE_TX_PONG_BUFFER_TX_ENABLE_SHIFT, AL_AXI_ETHLITE_FUNC_ENABLE);
}

static inline AL_VOID AlAxiEthLite_ll_SetTxPongBufferFrameLenAndEnableTx(AL_REG BaseAddr, AL_U32 FrameLen)
{
    AL_REG32_WRITE(BaseAddr + AXI_ETHLITE_TX_PONG_BUFFER_PTR_OFFSET,
                  (FrameLen | (1 << AXI_ETHLITE_TX_PONG_BUFFER_TX_ENABLE_SHIFT)));
}

static inline AL_VOID AlAxiEthLite_ll_ClrRXFrameLossCount(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_RX_FRAME_LOSS_COUNT_OFFSET,
                     AXI_ETHLITE_RX_FRAME_LOSS_CLR_ENABLE, AL_AXI_ETHLITE_FUNC_ENABLE);
}

static inline AL_VOID AlAxiEthLite_ll_EnableRxValidIntr(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_INTR_ENABLE_OFFSET, AXI_ETHLITE_INTR_RX_VALID_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableTxIdleIntr(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_INTR_ENABLE_OFFSET, AXI_ETHLITE_INTR_TX_IDLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableTxDoneIntr(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_INTR_ENABLE_OFFSET, AXI_ETHLITE_INTR_TX_DONE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableRxFrameLossIntr(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_INTR_ENABLE_OFFSET, AXI_ETHLITE_INTR_RX_FRAME_LOSS_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableRxRecvBadFrameIntr(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_INTR_ENABLE_OFFSET, AXI_ETHLITE_INTR_RX_BAD_FRAME_SHIFT, State);
}

static inline AL_U32 AlAxiEthLite_ll_GetIntrEvent(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AXI_ETHLITE_INTR_EVENT_OFFSET);
}

static inline AL_VOID AlAxiEthLite_ll_ClrIntr(AL_REG BaseAddr, AL_U32 IntrEvent)
{
    AL_REG32_WRITE(BaseAddr + AXI_ETHLITE_INTR_EVENT_OFFSET, IntrEvent);
}

static inline AL_U32 AlAxiEthLite_ll_GetIntrPending(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AXI_ETHLITE_INTR_PENDING_OFFSET);
}

static inline AL_VOID AlAxiEthLite_ll_SetMdioClockDiv(AL_REG BaseAddr, AL_U8 ClockDiv)
{
    AL_REG32_SET_BITS(BaseAddr + AXI_ETHLITE_MDIO_CONFIG_OFFSET, AXI_ETHLITE_MDIO_CLOCK_DIV_SHIFT,
                      AXI_ETHLITE_MDIO_CLOCK_DIV_SIZE, ClockDiv);
}

static inline AL_VOID AlAxiEthLite_ll_EnableMdio(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MDIO_CONFIG_OFFSET, AXI_ETHLITE_MDIO_ENABLE_SHIFT, State);
}

static inline AL_U32 AlAxiEthLite_ll_ReadMdioOperation(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AXI_ETHLITE_MDIO_OPERATION_OFFSET);
}

static inline AL_VOID AlAxiEthLite_ll_WriteMdioOperation(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + AXI_ETHLITE_MDIO_OPERATION_OFFSET, Value);
}

static inline AL_BOOL AlAxiEthLite_ll_IsMdioBusy(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AXI_ETHLITE_MDIO_OPERATION_OFFSET,
                            AXI_ETHLITE_MDIO_OPERATION_TRANS_ENABLE_SHIFT);
}

static inline AL_U16 AlAxiEthLite_ll_MdioReadData(AL_REG BaseAddr)
{
    return (AL_U16)AL_REG32_READ(BaseAddr + AXI_ETHLITE_MDIO_READ_DATA_OFFSET);
}

static inline AL_VOID AlAxiEthLite_ll_MdioWriteData(AL_REG BaseAddr, AL_U16 Data)
{
    AL_REG32_WRITE(BaseAddr + AXI_ETHLITE_MDIO_WRITE_DATA_OFFSET, Data);
}

static inline AL_VOID AlAxiEthLite_ll_EnableTxHalfDuplex(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_TX_HALF_DUMPLE_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableTxVlan(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_TX_VLAN_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableTxFcs(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_TX_FCS_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableTxJumboFrame(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_TX_JUMBO_FRAME_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableTxInterFrameGapAdjust(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET,
                     AXI_ETHLITE_TX_INTER_FRAME_GAP_ADJUST_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableTxFlowControl(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_TX_FLOW_CONTROL_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableTx(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_TX_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableRxHalfDuplex(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_RX_HALF_DUMPLE_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableRxVlan(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_RX_VLAN_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableRxFcs(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_RX_FCS_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableRxJumboFrame(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_RX_JUMBO_FRAME_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableRxControlFrameLenCheck(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET,
                     AXI_ETHLITE_RX_CONTROL_FRAME_LEN_CHECK_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableRxLenTypeCheck(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_RX_FRAME_LEN_TYPE_CHECK_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableRxFlowControl(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_RX_FLOW_CONTROL_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_EnableRx(AL_REG BaseAddr, AL_AXI_ETHLITE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_RX_ENABLE_SHIFT, State);
}

static inline AL_VOID AlAxiEthLite_ll_SetTxInterFrameGapValue(AL_REG BaseAddr, AL_U16 Value)
{
    AL_REG32_SET_BITS(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET,
                      AXI_ETHLITE_TX_INTER_FRAME_GAP_VALUE_SHIFT, AXI_ETHLITE_TX_INTER_FRAME_GAP_VALUE_SIZE, Value);
}

static inline AL_VOID AlAxiEthLite_ll_SetSpeed(AL_REG BaseAddr, AL_AXI_ETHLITE_SpeedEnum Speed)
{
    AL_REG32_SET_BITS(BaseAddr + AXI_ETHLITE_MAC_CONFIG_OFFSET, AXI_ETHLITE_SPEED_SHIFT, AXI_ETHLITE_SPEED_SIZE, Speed);
}

static inline AL_VOID AlAxiEthLite_ll_ResetMac(AL_REG BaseAddr, AL_U32 ResetValue)
{
    AL_REG32_WRITE(BaseAddr + AXI_ETHLITE_RESET_OFFSET, ResetValue);
}

static inline AL_U32 AlAxiEthLite_ll_GetIpInfo(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AXI_ETHLITE_IP_INFO_OFFSET);
}

static inline AL_BOOL AlAxiEthLite_ll_IsMdioExist(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AXI_ETHLITE_IP_INFO_OFFSET, AXI_ETHLITE_MDIO_EXIST_SHIFT);
}

static inline AL_U16 AlAxiEthLite_ll_GetTxBuffSize(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + AXI_ETHLITE_IP_INFO_OFFSET,
                             AXI_ETHLITE_TX_BUFFER_DEPTH_SHIFT, AXI_ETHLITE_TX_BUFFER_DEPTH_SIZE);
}

static inline AL_U16 AlAxiEthLite_ll_GetRxBuffSize(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + AXI_ETHLITE_IP_INFO_OFFSET,
                             AXI_ETHLITE_RX_BUFFER_DEPTH_SHIFT, AXI_ETHLITE_RX_BUFFER_DEPTH_SIZE);
}

#ifdef __cplusplus
}
#endif

#endif /* __AL_AXI_ETHLITE_LL_H_ */