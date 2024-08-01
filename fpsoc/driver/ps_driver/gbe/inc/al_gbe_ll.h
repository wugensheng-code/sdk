/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_GBE_LL_H_
#define __AL_GBE_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_gbe_hw.h"
#include "al_reg_io.h"

typedef enum
{
    AL_GBE_FUNC_DISABLE     = 0,
    AL_GBE_FUNC_ENABLE      = 1
} AL_GBE_FunctionEnum;

typedef enum
{
    AL_GBE_PRE_LEN_7BYTES       = 0,
    AL_GBE_PRE_LEN_5BYTES       = 1,
    AL_GBE_PRE_LEN_3BYTES       = 2,
} AL_GBE_PreambleLenEnum;

typedef enum
{
    AL_GBE_HALF_DUPLEX_MODE     = 0,
    AL_GBE_FULL_DUPLEX_MODE     = 1
} AL_GBE_DuplexModeEnum;

typedef enum
{
    AL_GBE_SPEED_1G         = 0,
    AL_GBE_SPEED_2_5G       = 1,
    AL_GBE_SPEED_10M        = 2,
    AL_GBE_SPEED_100M       = 3,
    AL_GBE_SPEED_AUTONEG    = 4,
} AL_GBE_SpeedEnum;

typedef enum
{
    AL_GBE_FILTER_ALL_CTL_PACKETS                           = 0,
    AL_GBE_FORWARD_CTL_PACKETS_EXCEPT_PAUSE_PACKETS         = 1,
    AL_GBE_FORWARD_CTL_PACKETS_ALL                          = 2,
    AL_GBE_FORWARD_PASS_FILTER_CTL_PACKETS                  = 3,
} AL_GBE_PassCtlPacketsModeEnum;

typedef enum
{
    AL_GBE_INTER_PACKET_GAP_96BIT   = 0,
    AL_GBE_INTER_PACKET_GAP_88BIT   = 1,
    AL_GBE_INTER_PACKET_GAP_80BIT   = 2,
    AL_GBE_INTER_PACKET_GAP_72BIT   = 3,
    AL_GBE_INTER_PACKET_GAP_64BIT   = 4,
    AL_GBE_INTER_PACKET_GAP_56BIT   = 5,
    AL_GBE_INTER_PACKET_GAP_48BIT   = 6,
    AL_GBE_INTER_PACKET_GAP_40BIT   = 7,
} AL_GBE_InterPacketGapEnum;

typedef enum
{
    AL_GBE_WatchdogTimeout2K        = 0,
    AL_GBE_WatchdogTimeout3K        = 1,
    AL_GBE_WatchdogTimeout4K        = 2,
    AL_GBE_WatchdogTimeout5K        = 3,
    AL_GBE_WatchdogTimeout6K        = 4,
    AL_GBE_WatchdogTimeout7K        = 5,
    AL_GBE_WatchdogTimeout8K        = 6,
    AL_GBE_WatchdogTimeout9K        = 7,
    AL_GBE_WatchdogTimeout10K       = 8,
    AL_GBE_WatchdogTimeout11K       = 9,
    AL_GBE_WatchdogTimeout12K       = 10,
    AL_GBE_WatchdogTimeout13K       = 11,
    AL_GBE_WatchdogTimeout14K       = 12,
    AL_GBE_WatchdogTimeout15K       = 13,
    AL_GBE_WatchdogTimeout16383BYTE = 14,
} AL_GBE_WatchdogTimeoutEnum;

typedef enum
{
    AL_GBE_PAUSE_LOW_MINUS_4SOLT_TIMES        = 0,
    AL_GBE_PAUSE_LOW_MINUS_28SOLT_TIMES       = 1,
    AL_GBE_PAUSE_LOW_MINUS_36SOLT_TIMES       = 2,
    AL_GBE_PAUSE_LOW_MINUS_144SOLT_TIMES      = 3,
    AL_GBE_PAUSE_LOW_MINUS_256SOLT_TIMES      = 4,
    AL_GBE_PAUSE_LOW_MINUS_512SOLT_TIMES      = 5,
} AL_GBE_PauseLowThresholdEnum;

typedef enum
{
    AL_GBE_DMA_TX_RX_WEIGHT_ROUND_ROBIN     = 0,
    AL_GBE_DMA_TX_RX_FIXED_PROORITY         = 1,
} AL_GBE_DmaTxRxArbSchemeEnum;

typedef enum
{
    AL_GBE_DMA_PRIORITY_RATIO_1_1           = 0,
    AL_GBE_DMA_PRIORITY_RATIO_2_1           = 1,
    AL_GBE_DMA_PRIORITY_RATIO_3_1           = 2,
    AL_GBE_DMA_PRIORITY_RATIO_4_1           = 3,
    AL_GBE_DMA_PRIORITY_RATIO_5_1           = 4,
    AL_GBE_DMA_PRIORITY_RATIO_6_1           = 5,
    AL_GBE_DMA_PRIORITY_RATIO_7_1           = 6,
    AL_GBE_DMA_PRIORITY_RATIO_8_1           = 7,
} AL_GBE_DmaTxRxPriorityRatioEnum;

typedef enum
{
    AL_GBE_DMA_TX_BURST_LENGTH_1BEAT        = (0x1 << 0),
    AL_GBE_DMA_TX_BURST_LENGTH_2BEAT        = (0x1 << 1),
    AL_GBE_DMA_TX_BURST_LENGTH_4BEAT        = (0x1 << 2),
    AL_GBE_DMA_TX_BURST_LENGTH_8BEAT        = (0x1 << 3),
    AL_GBE_DMA_TX_BURST_LENGTH_16BEAT       = (0x1 << 4),
    AL_GBE_DMA_TX_BURST_LENGTH_32BEAT       = (0x1 << 5),
} AL_GBE_DmaTxProgramBurstLengthEnum;

typedef enum
{
    AL_GBE_DMA_AXI_BURST_LENGTH_4           = (0x1 << 1),
    AL_GBE_DMA_AXI_BURST_LENGTH_8           = (0x1 << 2),
    AL_GBE_DMA_AXI_BURST_LENGTH_16          = (0x1 << 3),
    AL_GBE_DMA_AXI_BURST_LENGTH_32          = (0x1 << 4),
    AL_GBE_DMA_AXI_BURST_LENGTH_64          = (0x1 << 5),
    AL_GBE_DMA_AXI_BURST_LENGTH_128         = (0x1 << 6),
    AL_GBE_DMA_AXI_BURST_LENGTH_256         = (0x1 << 7),
} AL_GBE_DmaAxiBurstLengthEnum;

typedef enum
{
    AL_GBE_INTR_TX_COMPLETE             = (0x1 << 0),
    AL_GBE_INTR_TX_PROCESS_STOP         = (0x1 << 1),
    AL_GBE_INTR_TX_BUFFER_UNAVAILABLE   = (0x1 << 2),
    AL_GBE_INTR_RX_COMPLETE             = (0x1 << 6),
    AL_GBE_INTR_RX_BUFFER_UNAVAILABLE   = (0x1 << 7),
    AL_GBE_INTR_RX_PROCESS_STOP         = (0x1 << 8),
    AL_GBE_INTR_RX_WATCHDOG_TIMEOUT     = (0x1 << 9),
    AL_GBE_INTR_EARLY_TX                = (0x1 << 10),
    AL_GBE_INTR_EARLY_RX                = (0x1 << 11),
    AL_GBE_INTR_FATAL_BUS_ERROR         = (0x1 << 12),
    AL_GBE_INTR_CONTEXT_DESC_ERROR      = (0x1 << 13),
    AL_GBE_INTR_ABNORMAL_SUMMARY        = (0x1 << 14),
    AL_GBE_INTR_NORMAL_SUMMARY          = (0x1 << 15),
} AL_GBE_IntrStatusEnum;

typedef enum
{
    AL_GBE_DESC_NORMAL_DESC     = 0,
    AL_GBE_DESC_CONTEXT_DESC    = 1,
} AL_GBE_DescContextTypeStruct;

typedef enum
{
    AL_GBE_DESC_SKIP_LEN_0BIT       = 0x0,
    AL_GBE_DESC_SKIP_LEN_32BIT      = 0x1,
    AL_GBE_DESC_SKIP_LEN_64BIT      = 0x2,
    AL_GBE_DESC_SKIP_LEN_128BIT     = 0x4,
} AL_GBE_DescSkipLenEnum;

typedef enum
{
    AL_GBE_GMII_CMD_RESERVED            = 0,
    AL_GBE_GMII_CMD_WRITE               = 1,
    AL_GBE_GMII_CMD_POST_READ_INCR_ADDR = 2,
    AL_GBE_GMII_CMD_READ                = 3,
} AL_GBE_GmiiOperationCmdEnum;

typedef enum
{
    AL_GBE_CSR_CLOCK_DIV42      = 0,
    AL_GBE_CSR_CLOCK_DIV62      = 1,
    AL_GBE_CSR_CLOCK_DIV16      = 2,
    AL_GBE_CSR_CLOCK_DIV26      = 3,
    AL_GBE_CSR_CLOCK_DIV102     = 4,
    AL_GBE_CSR_CLOCK_DIV124     = 5,
    AL_GBE_CSR_CLOCK_DIV204     = 6,
    AL_GBE_CSR_CLOCK_DIV324     = 7,
} AL_GBE_CsrClockDivEnum;

static inline AL_VOID AlGbe_ll_SetGbeCtlRegister(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr, Value);
}

static inline AL_VOID AlGbe_ll_SetRecvEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__RE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetTransEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__TE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetPreLEn(AL_REG BaseAddr, AL_GBE_PreambleLenEnum Value)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET,
                      GBE__MAC_CONFIGURATION__PRELEN__SHIFT, GBE__MAC_CONFIGURATION__PRELEN__SIZE, Value);
}

static inline AL_VOID AlGbe_ll_SetCarrierSenseBeforeTransmitEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__ECRSFD__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetLoopbackModeEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__LM__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDuplexMode(AL_REG BaseAddr, AL_GBE_DuplexModeEnum Mode)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__DM__SHIFT, Mode);
}

static inline AL_VOID AlGbe_ll_SetSpeed(AL_REG BaseAddr, AL_GBE_SpeedEnum Speed)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET,
                      GBE__MAC_CONFIGURATION__FES__SHIFT, GBE__MAC_CONFIGURATION__PORT_SPEED__SIZE, Speed);
}

static inline AL_VOID AlGbe_ll_SetJumboPacketEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__JE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetJabberDisable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__JD__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetWatchdogDisable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__WD__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetAutoPadCrcStripEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__ACS__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetCrcStripForTypePacketEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__CST__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetSupport2KPacketEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__S2KP__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetGiantPacketSizeLimitCtlEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__GPSLCE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetInterPacketGap(AL_REG BaseAddr, AL_GBE_InterPacketGapEnum Value)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET,
                     GBE__MAC_CONFIGURATION__IPG__SHIFT, GBE__MAC_CONFIGURATION__IPG__SIZE, Value);
}

static inline AL_VOID AlGbe_ll_SetChecksumOffloadEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__IPC__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetArpOffloadEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_CONFIGURATION__OFFSET, GBE__MAC_CONFIGURATION__ARPEN__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetGiantPacketSizeLimit(AL_REG BaseAddr, AL_U16 Size)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_EXT_CONFIGURATION__OFFSET,
                      GBE__MAC_EXT_CONFIGURATION__GPSL__SHIFT, GBE__MAC_EXT_CONFIGURATION__GPSL__SIZE, Size);
}

static inline AL_VOID AlGbe_ll_SetDisableCrcCheckRecvPackets(AL_REG BaseAddr,AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_EXT_CONFIGURATION__OFFSET, GBE__MAC_EXT_CONFIGURATION__DCRCC__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetSlowProtDectEnable(AL_REG BaseAddr,AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_EXT_CONFIGURATION__OFFSET, GBE__MAC_EXT_CONFIGURATION__DCRCC__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetUnicastSlowProtDectEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_EXT_CONFIGURATION__OFFSET, GBE__MAC_EXT_CONFIGURATION__DCRCC__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetExtendInterPacketGapEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_EXT_CONFIGURATION__OFFSET, GBE__MAC_EXT_CONFIGURATION__EIPGEN__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetExtendInterPacketGap(AL_REG BaseAddr, AL_U8 Value)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_EXT_CONFIGURATION__OFFSET,
                     GBE__MAC_EXT_CONFIGURATION__EIPG__SHIFT, GBE__MAC_EXT_CONFIGURATION__EIPG__SIZE, Value);
}

static inline AL_VOID AlGbe_ll_SetPromiscuousModeEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_PACKET_FILTER__OFFSET, GBE__MAC_PACKET_FILTER__PR__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetHashUnicastEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_PACKET_FILTER__OFFSET, GBE__MAC_PACKET_FILTER__HUC__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetHashMulticastEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_PACKET_FILTER__OFFSET, GBE__MAC_PACKET_FILTER__HMC__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDestAddrInverseFilterEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_PACKET_FILTER__OFFSET, GBE__MAC_PACKET_FILTER__DAIF__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetPassAllMulticastEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_PACKET_FILTER__OFFSET, GBE__MAC_PACKET_FILTER__PM__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetBroadcasPacketsDisable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_PACKET_FILTER__OFFSET, GBE__MAC_PACKET_FILTER__DBF__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetPassCtlPacketsMode(AL_REG BaseAddr, AL_GBE_PassCtlPacketsModeEnum Mode)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_PACKET_FILTER__OFFSET,
    GBE__MAC_PACKET_FILTER__PCF__SHIFT, GBE__MAC_PACKET_FILTER__PCF__SIZE, Mode);
}

static inline AL_VOID AlGbe_ll_SetSrcAddrInverseFilterEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_PACKET_FILTER__OFFSET, GBE__MAC_PACKET_FILTER__SAIF__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetSrcAddrFilterEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_PACKET_FILTER__OFFSET, GBE__MAC_PACKET_FILTER__SAF__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetHashOrPerfectEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_PACKET_FILTER__OFFSET, GBE__MAC_PACKET_FILTER__HPF__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetRecvAllEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_PACKET_FILTER__OFFSET, GBE__MAC_PACKET_FILTER__RA__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetWatchdogTimeout(AL_REG BaseAddr, AL_GBE_WatchdogTimeoutEnum Timeout)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_WATCHDOG_TIMEOUT__OFFSET,
    GBE__MAC_WATCHDOG_TIMEOUT__WTO__SHIFT, GBE__MAC_WATCHDOG_TIMEOUT__WTO__SIZE, Timeout);
}

static inline AL_VOID AlGbe_ll_SetProgrammableWatchdogEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_WATCHDOG_TIMEOUT__OFFSET, GBE__MAC_WATCHDOG_TIMEOUT__PWE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetHashTable0(AL_REG BaseAddr, AL_U32 Value)
{

}

static inline AL_VOID AlGbe_ll_SetFlowCtlBusyorBackpressureActiveEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_Q0_TX_FLOW_CTRL__OFFSET, GBE__MAC_Q0_TX_FLOW_CTRL__FCB_BPA__SHIFT, State);
}

static inline AL_GBE_FunctionEnum AlGbe_ll_GetFlowCtlBusyorBackpressureActiveState(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + GBE__MAC_Q0_TX_FLOW_CTRL__OFFSET, GBE__MAC_Q0_TX_FLOW_CTRL__FCB_BPA__SHIFT);
}

static inline AL_VOID AlGbe_ll_SetTxFlowCtlEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_Q0_TX_FLOW_CTRL__OFFSET, GBE__MAC_Q0_TX_FLOW_CTRL__TFE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetPauseLowThreshlod(AL_REG BaseAddr, AL_GBE_PauseLowThresholdEnum Value)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_Q0_TX_FLOW_CTRL__OFFSET,
                      GBE__MAC_Q0_TX_FLOW_CTRL__PLT__SHIFT, GBE__MAC_Q0_TX_FLOW_CTRL__PLT__SIZE, Value);
}

static inline AL_VOID AlGbe_ll_SetZeroQuantaPauseDisable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_Q0_TX_FLOW_CTRL__OFFSET, GBE__MAC_Q0_TX_FLOW_CTRL__DZPQ__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetPauseTime(AL_REG BaseAddr, AL_U16 Value)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_Q0_TX_FLOW_CTRL__OFFSET,
                      GBE__MAC_Q0_TX_FLOW_CTRL__PT__SHIFT, GBE__MAC_Q0_TX_FLOW_CTRL__PT__SIZE, Value);
}

static inline AL_VOID AlGbe_ll_SetRxFlowCtlEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_RX_FLOW_CTRL__OFFSET, GBE__MAC_RX_FLOW_CTRL__RFE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetUnicastPausePacketDetEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_RX_FLOW_CTRL__OFFSET, GBE__MAC_RX_FLOW_CTRL__UP__SHIFT, State);
}

static inline AL_U32 AlGbe_ll_ReadMdioAddr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GBE__MAC_MDIO_ADDRESS__OFFSET);
}

static inline AL_VOID AlGbe_ll_WriteMdioAddr(AL_REG BaseAddr, AL_U32 RegValue)
{
    AL_REG32_WRITE(BaseAddr + GBE__MAC_MDIO_ADDRESS__OFFSET, RegValue);
}

static inline AL_BOOL AlGbe_ll_IsGmiiBusy(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + GBE__MAC_MDIO_ADDRESS__OFFSET, GBE__MAC_MDIO_ADDRESS__GB__SHIFT);
}

static inline AL_VOID AlGbe_ll_SetCsrClockRange(AL_REG BaseAddr, AL_GBE_CsrClockDivEnum Value)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_MDIO_ADDRESS__OFFSET,
                     GBE__MAC_MDIO_ADDRESS__CR__SHIFT, GBE__MAC_MDIO_ADDRESS__CR__SIZE, Value);
}

static inline AL_VOID AlGbe_ll_WriteMdioGmiiData(AL_REG BaseAddr, AL_U16 Value)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_MDIO_DATA__OFFSET,
                     GBE__MAC_MDIO_DATA__GD__SHIFT, GBE__MAC_MDIO_DATA__GD__SIZE, Value);
}

static inline AL_U32 AlGbe_ll_ReadMdioGmiiData(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GBE__MAC_MDIO_DATA__OFFSET);
}

static inline AL_VOID AlGbe_ll_SetMacAddrLow(AL_REG BaseAddr, AL_U32 Addr)
{
    AL_REG32_WRITE(BaseAddr + GBE_MAC_ADDRESS0_LOW__OFFSET, Addr);
}

static inline AL_VOID AlGbe_ll_SetMacAddrHigh(AL_REG BaseAddr, AL_U16 Addr)
{
    AL_REG32_SET_BITS(BaseAddr + GBE_MAC_ADDRESS0_HIGH__OFFSET,
                      GBE_MAC_ADDRESS0_HIGH__ADDRHI__SHIFT, GBE_MAC_ADDRESS0_HIGH__ADDRHI__SIZE, Addr);
}

static inline AL_VOID AlGbe_ll_SetMtlTxQueueFlushEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MTL_TXQ0_OPERATION_MODE__OFFSET, GBE__MTL_TXQ0_OPERATION_MODE__FTQ__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetMtlTxQueueStoreForwardEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MTL_TXQ0_OPERATION_MODE__OFFSET, GBE__MTL_TXQ0_OPERATION_MODE__TSF__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetMtlRxQueueStoreForwardEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MTL_RXQ0_OPERATION_MODE__OFFSET, GBE__MTL_RXQ0_OPERATION_MODE__RSF__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetMtlRxQueueForwardUndersizeGoodPacketsEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MTL_RXQ0_OPERATION_MODE__OFFSET, GBE__MTL_RXQ0_OPERATION_MODE__FUP__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetMtlRxQueueForwardErrorPacketsEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MTL_RXQ0_OPERATION_MODE__OFFSET, GBE__MTL_RXQ0_OPERATION_MODE__FEP__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetMtlRxQueueDisableDropTcpIpChecksumErrorpackets(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MTL_RXQ0_OPERATION_MODE__OFFSET, GBE__MTL_RXQ0_OPERATION_MODE__DIS_TCP_EF__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_ResetMac(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_MODE__OFFSET, GBE__DMA_MODE__SWR__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_GBE_FunctionEnum AlGbe_ll_GetMacResetStatus(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + GBE__DMA_MODE__OFFSET, GBE__DMA_MODE__SWR__SHIFT);
}

static inline AL_VOID AlGbe_ll_SetDmaTxRxArbitrationScheme(AL_REG BaseAddr, AL_GBE_DmaTxRxArbSchemeEnum Scheme)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_MODE__OFFSET, GBE__DMA_MODE__SWR__SHIFT, Scheme);
}

static inline AL_VOID AlGbe_ll_SetDmaTxPriorityEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_MODE__OFFSET, GBE__DMA_MODE__TXPR__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaTxRxPriorityRatio(AL_REG BaseAddr, AL_GBE_DmaTxRxPriorityRatioEnum PriorityRatio)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__DMA_MODE__OFFSET,
                      GBE__DMA_MODE__PR__SHIFT, GBE__DMA_MODE__PR__SIZE, PriorityRatio);
}

static inline AL_VOID AlGbe_ll_SetDmaFixedBurstLengthEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_SYSBUS_MODE__OFFSET, GBE__DMA_SYSBUS_MODE__FB__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaFixedBurstLength(AL_REG BaseAddr, AL_GBE_DmaAxiBurstLengthEnum BurstLen)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__DMA_SYSBUS_MODE__OFFSET,
                      GBE__DMA_SYSBUS_MODE__BLEN4__SHIFT, GBE__DMA_SYSBUS_MODE__BLEN__SIZE, BurstLen);
}

static inline AL_VOID AlGbe_ll_SetAddrAlignedBeatsEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_SYSBUS_MODE__OFFSET, GBE__DMA_SYSBUS_MODE__AAL__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetMixedBurstEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_SYSBUS_MODE__OFFSET, GBE__DMA_SYSBUS_MODE__MB__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetRebuildIncrxBurstEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_SYSBUS_MODE__OFFSET, GBE__DMA_SYSBUS_MODE__RB__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaMaximumSegmentSize(AL_REG BaseAddr, AL_U16 Size)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__DMA_CH0_CONTROL__OFFSET, GBE__DMA_CH0_CONTROL__MSS__SHIFT,
                      GBE__DMA_CH0_CONTROL__MSS__SIZE, Size);
}

static inline AL_VOID AlGbe_ll_SetDma8xPblEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_CONTROL__OFFSET, GBE__DMA_CH0_CONTROL__PBLX8__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDescSkipLen(AL_REG BaseAddr, AL_GBE_DescSkipLenEnum SkipLen)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__DMA_CH0_CONTROL__OFFSET,
                      GBE__DMA_CH0_CONTROL__DSL__SHIFT, GBE__DMA_CH0_CONTROL__DSL__SIZE, SkipLen);
}

static inline AL_VOID AlGbe_ll_SetDmaStartTransEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_TX_CONTROL__OFFSET, GBE__DMA_CH0_TX_CONTROL__ST__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaOperateOnSecondPacketEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_TX_CONTROL__OFFSET, GBE__DMA_CH0_TX_CONTROL__OSF__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaTcpSegmentEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_TX_CONTROL__OFFSET, GBE__DMA_CH0_TX_CONTROL__TSE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaTxBurstLen(AL_REG BaseAddr, AL_GBE_DmaTxProgramBurstLengthEnum BurstLen)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__DMA_CH0_TX_CONTROL__OFFSET,
                      GBE__DMA_CH0_TX_CONTROL__TXPBL__SHIFT, GBE__DMA_CH0_TX_CONTROL__TXPBL__SIZE, BurstLen);
}

static inline AL_VOID AlGbe_ll_SetDmaStartRecvEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_RX_CONTROL__OFFSET, GBE__DMA_CH0_RX_CONTROL__SR__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaRxBurstLen(AL_REG BaseAddr, AL_GBE_DmaTxProgramBurstLengthEnum BurstLen)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__DMA_CH0_RX_CONTROL__OFFSET,
                      GBE__DMA_CH0_RX_CONTROL__RXPBL__SHIFT, GBE__DMA_CH0_RX_CONTROL__RXPBL__SIZE, BurstLen);
}

static inline AL_VOID AlGbe_ll_SetDmaRxPacketFlushEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_RX_CONTROL__OFFSET, GBE__DMA_CH0_RX_CONTROL__RPF__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaRxBufferSize(AL_REG BaseAddr, AL_U16 BufferSize)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__DMA_CH0_RX_CONTROL__OFFSET,
                      GBE__DMA_CH0_RX_CONTROL__RBSZ_13_Y__SHIFT, GBE__DMA_CH0_RX_CONTROL__RBSZ_13_Y__SIZE, BufferSize);
}

static inline AL_VOID AlGbe_ll_SetDmaChannelTxDescListAddr(AL_REG BaseAddr, AL_U32 Addr)
{
    AL_REG32_WRITE(BaseAddr + GBE__DMA_CH0_TXDESC_LIST_ADDRESS__OFFSET, Addr);
}

static inline AL_VOID AlGbe_ll_SetDmaChannelRxDescListAddr(AL_REG BaseAddr, AL_U32 Addr)
{
    AL_REG32_WRITE(BaseAddr + GBE__DMA_CH0_RXDESC_LIST_ADDRESS__OFFSET, Addr);
}

static inline AL_VOID AlGbe_ll_SetDmaTxDescTailPointer(AL_REG BaseAddr, AL_U32 Addr)
{
    AL_REG32_WRITE(BaseAddr + GBE__DMA_CH0_TXDESC_TAIL_POINTER__OFFSET, Addr);
}

static inline AL_VOID AlGbe_ll_SetDmaRxDescTailPointer(AL_REG BaseAddr, AL_U32 Addr)
{
    AL_REG32_WRITE(BaseAddr + GBE__DMA_CH0_RXDESC_TAIL_POINTER__OFFSET, Addr);
}

static inline AL_VOID AlGbe_ll_SetDmaChannelTxDescRingLen(AL_REG BaseAddr, AL_U16 RingLen)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__DMA_CH0_TXDESC_RING_LENGTH__OFFSET, GBE__DMA_CH0_TXDESC_RING_LENGTH__TDRL__SHIFT,
                      GBE__DMA_CH0_TXDESC_RING_LENGTH__TDRL__SIZE, RingLen);
}

static inline AL_VOID AlGbe_ll_SetDmaChannelRxDescRingLen(AL_REG BaseAddr, AL_U16 RingLen)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__DMA_CH0_RXDESC_RING_LENGTH__OFFSET, GBE__DMA_CH0_RXDESC_RING_LENGTH__RDRL__SHIFT,
                      GBE__DMA_CH0_RXDESC_RING_LENGTH__RDRL__SIZE, RingLen);
}

static inline AL_VOID AlGbe_ll_SetDmaTransIntrEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_INTERRUPT_ENABLE__OFFSET, GBE__DMA_CH0_INTERRUPT_ENABLE__TIE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaTransBufferUnavailableIntrEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_INTERRUPT_ENABLE__OFFSET, GBE__DMA_CH0_INTERRUPT_ENABLE__TBUE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaRecvIntrEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_INTERRUPT_ENABLE__OFFSET, GBE__DMA_CH0_INTERRUPT_ENABLE__RIE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaRecvBufferUnavailableIntrEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_INTERRUPT_ENABLE__OFFSET, GBE__DMA_CH0_INTERRUPT_ENABLE__RBUE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaFatalBusErrorIntrEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_INTERRUPT_ENABLE__OFFSET, GBE__DMA_CH0_INTERRUPT_ENABLE__FBEE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaAbnormalSummaryIntrEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_INTERRUPT_ENABLE__OFFSET, GBE__DMA_CH0_INTERRUPT_ENABLE__AIE__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetDmaNormalSummaryIntrEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_INTERRUPT_ENABLE__OFFSET, GBE__DMA_CH0_INTERRUPT_ENABLE__NIE__SHIFT, State);
}

static inline AL_U32 AlGbe_ll_GetDmaChannelStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET);
}

static inline AL_U32 AlGbe_ll_IsDmaChannelFatalBusError(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__FBE__SHIFT);
}

static inline AL_VOID AlGbe_ll_ClrDmaIntr(AL_REG BaseAddr, AL_U32 IntrStatus)
{
    AL_REG32_WRITE(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, IntrStatus);
}

static inline AL_VOID AlGbe_ll_ClrTxCompletrIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__TI__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_VOID AlGbe_ll_ClrRxCompletrIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__RI__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_VOID AlGbe_ll_ClrNormalSummaryIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__NIS__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_VOID AlGbe_ll_ClrAbnormalSummaryIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__AIS__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_VOID AlGbe_ll_ClrTxBufferUnavailableIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__TBU__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_VOID AlGbe_ll_ClrEarlyTxIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__ETI__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_VOID AlGbe_ll_ClrRxBufferUnavailableIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__RBU__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_VOID AlGbe_ll_ClrRxProcessStopIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__RPS__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_VOID AlGbe_ll_ClrRxWatchDogTimeoutIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__RWT__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_VOID AlGbe_ll_ClrFatalBusErrorIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__FBE__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_VOID AlGbe_ll_ClrCtxDescErrorIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__DMA_CH0_STATUS__OFFSET, GBE__DMA_CH0_STATUS__CDE__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_VOID AlGbe_ll_SetDescTdes0(AL_REG BaseAddr, AL_U16 RingLen)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__DMA_CH0_TXDESC_RING_LENGTH__OFFSET, GBE__DMA_CH0_TXDESC_RING_LENGTH__TDRL__SHIFT,
                      GBE__DMA_CH0_TXDESC_RING_LENGTH__TDRL__SIZE, RingLen);
}

static inline AL_VOID AlGbe_ll_SetTdesc2Buffer1Len(AL_REG BaseAddr, AL_U16 BufferLen)
{
    AL_REG32_SET_BITS(BaseAddr, GBE__DESC_TX_DESC2__HL_B1L__SHIFT, GBE__DESC_TX_DESC2__B1L__SIZE, BufferLen);
}

static inline AL_VOID AlGbe_ll_SetTdesc2Buffer2Len(AL_REG BaseAddr, AL_U16 BufferLen)
{
    AL_REG32_SET_BITS(BaseAddr, GBE__DESC_TX_DESC2__B2L__SHIFT, GBE__DESC_TX_DESC2__B2L__SIZE, BufferLen);
}

static inline AL_VOID AlGbe_ll_SetTdesc2CompleteIntr(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_TX_DESC2__IOC__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetTdesc2TxTimeStampEnable(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_TX_DESC2__TTSE_TMWD__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetTdesc3FrameLen(AL_REG BaseAddr, AL_U16 FrameLen)
{
    AL_REG32_SET_BITS(BaseAddr, GBE__DESC_TX_DESC3__FL_TPL__SHIFT, GBE__DESC_TX_DESC3__FL_TPL__SIZE, FrameLen);
}

static inline AL_VOID AlGbe_ll_SetTdesc3LastDesc(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_TX_DESC3__LD__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetTdesc3FirstDesc(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_TX_DESC3__FD__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetTdesc3ContextType(AL_REG BaseAddr, AL_GBE_DescContextTypeStruct ContextType)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_TX_DESC3__CTXT__SHIFT, ContextType);
}

static inline AL_VOID AlGbe_ll_SetTdesc3OwnByDma(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_TX_DESC3__OWN__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetChksumInsertCtrl(AL_REG BaseAddr, AL_U16 Control)
{
    AL_REG32_SET_BITS(BaseAddr, GBE__DESC_TX_DESC3__CIC_TPL__SHIFT, GBE__DESC_TX_DESC3__CIC_TPL__SIZE, Control);
}

static inline AL_BOOL AlGbe_ll_IsTxDescOwnByDma(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr, GBE__DESC_TX_DESC3__OWN__SHIFT);
}

static inline AL_BOOL AlGbe_ll_GetWbTxDesc3TxTimeStatus(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr, GBE__DESC_WRITE_BACK_TX_DESC3__TTSS__SHIFT);
}

static inline AL_VOID AlGbe_ll_ClearWbTxDesc3TxTimeStatus(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_WRITE_BACK_TX_DESC3__TTSS__SHIFT, AL_GBE_FUNC_DISABLE);
}

static inline AL_BOOL AlGbe_ll_IsWbTxDescOwnByDma(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr, GBE__DESC_WRITE_BACK_TX_DESC3__OWN__SHIFT);
}

static inline AL_VOID AlGbe_ll_SetRdesc3Buff1Valid(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_RX_DESC3__BUF1V__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetRdesc3Buff2Valid(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_RX_DESC3__BUF2V__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetRdesc3CompleteIntr(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_RX_DESC3__IOC__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetRdesc3OwnByDma(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_RX_DESC3__OWN__SHIFT, State);
}

static inline AL_BOOL AlGbe_ll_IsWbRxDesc1TimeStampAvaliable(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr, 14);
}

static inline AL_BOOL AlGbe_ll_IsWbRxDescOwnByDma(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr, GBE__DESC_WRITE_BACK_RX_DESC3__OWN__SHIFT);
}

static inline AL_BOOL AlGbe_ll_IsWbRxDescCrcError(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr, GBE__DESC_WRITE_BACK_RX_DESC3__CE__SHIFT);
}

static inline AL_BOOL AlGbe_ll_IsWbRxDescFirstDesc(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr, GBE__DESC_WRITE_BACK_RX_DESC3__FD__SHIFT);
}

static inline AL_BOOL AlGbe_ll_IsWbRxDescLastDesc(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr, GBE__DESC_WRITE_BACK_RX_DESC3__LD__SHIFT);
}

static inline AL_BOOL AlGbe_ll_IsWbRxDesc3StatusValid(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr, GBE__DESC_WRITE_BACK_RX_DESC3__RS1V__SHIFT);
}

static inline AL_BOOL AlGbe_ll_GetWbRecvContextDesc(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr, GBE__DESC_WRITE_BACK_RX_DESC3__CTXT__SHIFT);
}

static inline AL_VOID AlGbe_ll_ClearWbRecvContextDesc(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr, GBE__DESC_WRITE_BACK_RX_DESC3__CTXT__SHIFT, AL_GBE_FUNC_DISABLE);
}

static inline AL_U16 AlGbe_ll_GetWbRdesc3Packetlen(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr, GBE__DESC_WRITE_BACK_RX_DESC3__PL__SHIFT, GBE__DESC_WRITE_BACK_RX_DESC3__PL__SIZE);
}

static inline AL_VOID AlGbe_ll_EnableTimeStamp(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSENA__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_SetSubSecondIncrementValue(AL_REG BaseAddr, AL_U8 Value)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_SUB_SECOND_INCREMENT__OFFSET,
                      GBE__MAC_SUB_SECOND_INCREMENT__SSINC__SHIFT, GBE__MAC_SUB_SECOND_INCREMENT__SSINC__SIZE, Value);
}

static inline AL_VOID AlGbe_ll_SetSystemTimeSecondsUpdate(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + GBE__MAC_SYSTEM_TIME_SECONDS_UPDATE__OFFSET, Value);
}

static inline AL_VOID AlGbe_ll_SetSystemTimeNanosecondsUpdate(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + GBE__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__OFFSET, Value);
}

static inline AL_VOID AlGbe_ll_EnableInitializeTimestamp(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSINIT__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_GBE_FunctionEnum AlGbe_ll_IsInitializeTimestampEnabled(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSINIT__SHIFT);
}

static inline AL_VOID AlGbe_ll_EnableUpdateTimestamp(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSUPDT__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_GBE_FunctionEnum AlGbe_ll_IsUpdateTimestampEnabled(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSUPDT__SHIFT);
}

static inline AL_VOID AlGbe_ll_EnableUpdateAddendRegister(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSADDREG__SHIFT, AL_GBE_FUNC_ENABLE);
}

static inline AL_GBE_FunctionEnum AlGbe_ll_IsUpdateAddendRegisterEnabled(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSADDREG__SHIFT);
}

static inline AL_VOID AlGbe_ll_EnablePtpV2(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSVER2ENA__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_EnablePtpPacketsOverIpv4Udp(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSIPV4ENA__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_EnableTimestampSnapshotForEventMessage(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSEVNTENA__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_EnableTMessageSnapshotForRelevantMaster(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSMSTRENA__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_PtpSelectPacketsForTakingSnapshot(AL_REG BaseAddr, AL_U8 Value)
{
    AL_REG32_SET_BITS(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET,
                      GBE__MAC_TIMESTAMP_CONTROL__SNAPTYPSEL__SHIFT, GBE__MAC_TIMESTAMP_CONTROL__SNAPTYPSEL__SIZE, Value);
}

static inline AL_VOID AlGbe_ll_EnablePtpPacketsOverIpv6Udp(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSIPV6ENA__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_EnablePtpPacketsOverEthernet(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSIPENA__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_EnableTimestampDigitalorBinaryRollover(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSCTRLSSR__SHIFT, State);
}

static inline AL_VOID AlGbe_ll_EnableTimestampForAllPackets(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSENALL__SHIFT, State);
}

static inline AL_VOID  AlGbe_ll_EnableFineTimestampUpdate(AL_REG BaseAddr, AL_GBE_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MAC_TIMESTAMP_CONTROL__OFFSET, GBE__MAC_TIMESTAMP_CONTROL__TSCFUPDT__SHIFT, State);
}

static inline AL_U32 AlGbe_ll_GetSystemTimeSeconds(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GBE__MAC_SYSTEM_TIME_SECONDS__OFFSET);
}

static inline AL_U32 AlGbe_ll_GetSystemTimeNanoseconds(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GBE__MAC_SYSTEM_TIME_NANOSECONDS__OFFSET);
}

static inline AL_VOID AlGbe_ll_SetTimestampAddend(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + GBE__MAC_TIMESTAMP_ADDEND__OFFSET, Value);
}

static inline AL_VOID AlGbe_ll_SetMmcCntFreez(AL_REG BaseAddr, AL_FUNCTION State)
{
    AL_REG32_SET_BIT(BaseAddr + GBE__MMC_CONTROL__OFFSET, GBE__MMC_CONTROL__CNTFREEZ__SHIFT, State);
}

#ifdef __cplusplus
}
#endif

#endif /* AL_GBE_LL_H */
