/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_IIC_LL_H_
#define __AL_IIC_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_iic_hw.h"
#include "al_reg_io.h"

typedef enum
{
    AL_IIC_FUNC_DISABLE     = 0,
    AL_IIC_FUNC_ENABLE      = 1
} AL_IIC_FunctionEnum;

typedef enum {
    AL_IIC_ADDR_7BIT        = 0,
    AL_IIC_ADDR_10BIT       = 1
} AL_IIC_AddrModeEnum ;

typedef enum {
    AL_IIC_STANDARD_MODE    = 1,
    AL_IIC_FAST_MODE        = 2,
    AL_IIC_HIGH_SPEED_MODE  = 3
} AL_IIC_SpeedModeEnum;

typedef enum
{
    AL_IIC_ABRT_7B_ADDR_NOACK       = (0x1 << 0),
    AL_IIC_ABRT_10ADDR1_NOACK       = (0x1 << 1),
    AL_IIC_ABRT_10ADDR2_NOACK       = (0x1 << 2),
    AL_IIC_ABRT_TXDATA_NOACK        = (0x1 << 3),
    AL_IIC_ABRT_GCALL_NOACK         = (0x1 << 4),
    AL_IIC_ABRT_GCALL_READ          = (0x1 << 5),
    AL_IIC_ABRT_HS_ACKDET           = (0x1 << 6),
    AL_IIC_ABRT_SBYTE_ACKDET        = (0x1 << 7),
    AL_IIC_ABRT_HS_NORSTRT          = (0x1 << 8),
    AL_IIC_ABRT_SBYTE_NORSTRT       = (0x1 << 9),
    AL_IIC_ABRT_10B_RD_NORSTRT      = (0x1 << 10),
    AL_IIC_ABRT_MASTER_DIS          = (0x1 << 11),
    AL_IIC_ARB_LOST                 = (0x1 << 12),
    AL_IIC_ABRT_SLVFLUSH_TXFIFO     = (0x1 << 13),
    AL_IIC_ABRT_SLV_ARBLOST         = (0x1 << 14),
    AL_IIC_ABRT_SLVRD_INTX          = (0x1 << 15),
    AL_IIC_ABRT_USER_ABRT           = (0x1 << 16),
    AL_IIC_ABRT_SDA_STUCK_AT_LOW    = (0x1 << 17),
    AL_IIC_ABRT_DEVICE_NOACK        = (0x1 << 18),
    AL_IIC_ABRT_SLAVEADDR_NOACK     = (0x1 << 19),
    AL_IIC_ABRT_DEVICE_WRITE        = (0x1 << 20),
} AL_IIC_AbrtSrcEnum;

static inline AL_VOID AlIic_ll_SetEnable(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_ENABLE__OFFSET, I2C__IC_ENABLE__ENABLE__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetSdaStuckRecoveryEnable(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_ENABLE__OFFSET, I2C__IC_ENABLE__SDA_STUCK_RECOVERY_ENABLE__SHIFT, State);
}

static inline AL_IIC_FunctionEnum AlIic_ll_GetSdaStuckRecoveryEnableStatus(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_ENABLE__OFFSET, I2C__IC_ENABLE__SDA_STUCK_RECOVERY_ENABLE__SHIFT);
}

static inline AL_VOID AlIic_ll_SetSlaveDisable(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_CON__OFFSET, I2C__IC_CON__IC_SLAVE_DISABLE__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetRestartEnable(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_CON__OFFSET, I2C__IC_CON__IC_RESTART_EN__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetMasterAddrMode(AL_REG BaseAddr, AL_IIC_AddrModeEnum Mode)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_CON__OFFSET, I2C__IC_CON__IC_10BITADDR_MASTER__SHIFT, Mode);
}

static inline AL_IIC_AddrModeEnum AlIic_ll_GetMasterAddrMode(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_CON__OFFSET, I2C__IC_CON__IC_10BITADDR_MASTER__SHIFT);
}

static inline AL_VOID AlIic_ll_SetSlaveAddrMode(AL_REG BaseAddr, AL_IIC_AddrModeEnum Mode)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_CON__OFFSET, I2C__IC_CON__IC_10BITADDR_SLAVE__SHIFT, Mode);
}

static inline AL_VOID AlIic_ll_SetSpeedMode(AL_REG BaseAddr, AL_IIC_SpeedModeEnum Mode)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_CON__OFFSET, I2C__IC_CON__SPEED__SHIFT, I2C__IC_CON__SPEED__SIZE, Mode);
}

static inline AL_IIC_SpeedModeEnum AlIic_ll_GetSpeedMode(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + I2C__IC_CON__OFFSET, I2C__IC_CON__SPEED__SHIFT, I2C__IC_CON__SPEED__SIZE);
}

static inline AL_VOID AlIic_ll_SetMasterEnable(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_CON__OFFSET, I2C__IC_CON__MASTER_MODE__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetTxEmptyCtrl(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_CON__OFFSET, I2C__IC_CON__TX_EMPTY_CTRL__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetBusClrFeatureCtrl(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_CON__OFFSET, I2C__IC_CON__BUS_CLEAR_FEATURE_CTRL__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetTar(AL_REG BaseAddr, AL_U16 Addr)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_TAR__OFFSET, I2C__IC_TAR__IC_TAR__SHIFT, I2C__IC_TAR__IC_TAR__SIZE, Addr);
}

static inline AL_VOID AlIic_ll_SetSlaveAddr(AL_REG BaseAddr, AL_U16 Addr)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_SAR__OFFSET, I2C__IC_SAR__IC_SAR__SHIFT, I2C__IC_SAR__IC_SAR__SIZE, Addr);
}

static inline AL_VOID AlIic_ll_SetStdSpeedSclHighCount(AL_REG BaseAddr, AL_U16 Count)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_SS_SCL_HCNT__OFFSET,
                      I2C__IC_SS_SCL_HCNT__IC_SS_SCL_HCNT__SHIFT, I2C__IC_SS_SCL_HCNT__IC_SS_SCL_HCNT__SIZE, Count);
}

static inline AL_VOID AlIic_ll_SetStdSpeedSclLowCount(AL_REG BaseAddr, AL_U16 Count)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_SS_SCL_LCNT__OFFSET,
                      I2C__IC_SS_SCL_LCNT__IC_SS_SCL_LCNT__SHIFT, I2C__IC_SS_SCL_LCNT__IC_SS_SCL_LCNT__SIZE, Count);
}

static inline AL_VOID AlIic_ll_SetFastSpeedSclHighCount(AL_REG BaseAddr, AL_U16 Count)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_FS_SCL_HCNT__OFFSET,
                      I2C__IC_FS_SCL_HCNT__IC_FS_SCL_HCNT__SHIFT, I2C__IC_FS_SCL_HCNT__IC_FS_SCL_HCNT__SIZE, Count);
}

static inline AL_VOID AlIic_ll_SetFastSpeedSclLowCount(AL_REG BaseAddr, AL_U16 Count)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_FS_SCL_LCNT__OFFSET,
                      I2C__IC_FS_SCL_LCNT__IC_FS_SCL_LCNT__SHIFT, I2C__IC_FS_SCL_LCNT__IC_FS_SCL_LCNT__SIZE, Count);
}

static inline AL_VOID AlIic_ll_SetHighSpeedSclHighCount(AL_REG BaseAddr, AL_U16 Count)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_HS_SCL_HCNT__OFFSET,
                      I2C__IC_HS_SCL_LCNT__IC_FS_SCL_LCNT__SHIFT, I2C__IC_HS_SCL_LCNT__IC_FS_SCL_LCNT__SIZE, Count);
}

static inline AL_VOID AlIic_ll_SetHighSpeedSclLowCount(AL_REG BaseAddr, AL_U16 Count)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_HS_SCL_LCNT__OFFSET,
                      I2C__IC_HS_SCL_LCNT__IC_FS_SCL_LCNT__SHIFT, I2C__IC_HS_SCL_LCNT__IC_FS_SCL_LCNT__SIZE, Count);
}

static inline AL_VOID AlIic_ll_SetRxUnderIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_RX_UNDER__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetRxOverIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_RX_OVER__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetRxFullIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_RX_FULL__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetTxOverIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_TX_OVER__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetTxEmptyIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_TX_EMPTY__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetRdReqIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_RD_REQ__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetTxAbrtIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_TX_ABRT__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetRxDoneIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_RX_DONE__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetActivityIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_ACTIVITY__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetStopDetIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_STOP_DET__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetStartDetIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_START_DET__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetGenCallIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_GEN_CALL__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetRestartDetIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_RESTART_DET__SHIFT, State);
}

static inline AL_VOID AlIic_ll_SetSclStuckAtLowIntr(AL_REG BaseAddr, AL_IIC_FunctionEnum State)
{
    AL_REG32_SET_BIT(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_SCL_STUCK_AT_LOW__SHIFT, State);
}

static inline AL_VOID AlIic_ll_MaskAllIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_INTR_MASK__OFFSET, I2C__IC_INTR_MASK__M_RX_UNDER__SHIFT,
                      I2C__IC_INTR_MASK__SIZE, AL_IIC_FUNC_DISABLE);
}

static inline AL_U32 AlIic_ll_GetIntrMask(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + I2C__IC_INTR_MASK__OFFSET);
}

static inline AL_VOID AlIic_ll_SetIntrMask(AL_REG BaseAddr, AL_U32 IntrMask)
{
    AL_REG32_WRITE(BaseAddr + I2C__IC_INTR_MASK__OFFSET, IntrMask);
}

static inline AL_VOID AlIic_ll_DisableIntr(AL_REG BaseAddr, AL_U32 IntrMask)
{
    AL_REG32_WRITE(BaseAddr + I2C__IC_INTR_MASK__OFFSET, (AlIic_ll_GetIntrMask(BaseAddr)) & (~(IntrMask)));
}


static inline AL_VOID AlIic_ll_SetRxFifoThr(AL_REG BaseAddr, AL_U8 FifoThrLevel)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_RX_TL__OFFSET, I2C__IC_RX_TL__RX_TL__SHIFT,
                      I2C__IC_RX_TL__RX_TL__SIZE, FifoThrLevel);
}

static inline AL_U8 AlIic_ll_GetRxFifoThr(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + I2C__IC_RX_TL__OFFSET, I2C__IC_RX_TL__RX_TL__SHIFT,
                             I2C__IC_RX_TL__RX_TL__SIZE);
}

static inline AL_U8 AlIic_ll_GetRxFifoLevel(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + I2C__IC_RXFLR__OFFSET, I2C__IC_RXFLR__RXFLR__SHIFT,
                             I2C__IC_RXFLR__RXFLR__SIZE);
}

static inline AL_U8 AlIic_ll_GetTxFifoLevel(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + I2C__IC_TXFLR__OFFSET, I2C__IC_TXFLR__TXFLR__SHIFT,
                             I2C__IC_RXFLR__RXFLR__SIZE);
}

static inline AL_VOID AlIic_ll_SetTxFifoThr(AL_REG BaseAddr, AL_U8 FifoThrLevel)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_TX_TL__OFFSET, I2C__IC_TX_TL__TX_TL__SHIFT,
                      I2C__IC_TX_TL__TX_TL__SIZE, FifoThrLevel);
}

static inline AL_VOID AlIic_ll_GetTxFifoThr(AL_REG BaseAddr, AL_U8 FifoThrLevel)
{
    (AL_VOID)AL_REG32_GET_BITS(BaseAddr + I2C__IC_TX_TL__OFFSET, I2C__IC_TX_TL__TX_TL__SHIFT,
                      I2C__IC_TX_TL__TX_TL__SIZE);
}

static inline AL_VOID AlIic_ll_SetDataCmd(AL_REG BaseAddr, AL_U32 DataCmdValue)
{
    AL_REG32_WRITE(BaseAddr + I2C__IC_DATA_CMD__OFFSET, DataCmdValue);
}

static inline AL_U8 AlIic_ll_ReadDataCmdData(AL_REG BaseAddr)
{
    return (AL_U8)AL_REG32_READ(BaseAddr + I2C__IC_DATA_CMD__OFFSET);
}

static inline AL_U32 AlIic_ll_GetIntrStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + I2C__IC_INTR_STAT__OFFSET);
}

static inline AL_U32 AlIic_ll_GetRawIntrStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + I2C__IC_RAW_INTR_STAT__OFFSET);
}

static inline AL_BOOL AlIic_ll_IsActivity(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_STATUS__OFFSET, I2C__IC_STATUS__ACTIVITY__SHIFT);
}

static inline AL_BOOL AlIic_ll_IsMasterActivity(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_STATUS__OFFSET, I2C__IC_STATUS__MST_ACTIVITY__SHIFT);
}

static inline AL_BOOL AlIic_ll_IsSlaveActivity(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_STATUS__OFFSET, I2C__IC_STATUS__SLV_ACTIVITY__SHIFT);
}

static inline AL_BOOL AlIic_ll_IsTxFifoNotFull(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_STATUS__OFFSET, I2C__IC_STATUS__TFNF__SHIFT);
}

static inline AL_BOOL AlIic_ll_IsRxFifoNotEmpty(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + I2C__IC_STATUS__OFFSET, I2C__IC_STATUS__RFNE__SHIFT);
}

static inline AL_BOOL AlIic_ll_IsSdaStuckNotRecovery(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_STATUS__OFFSET, I2C__IC_STATUS__SDA_STUCK_NOT_RECOVERED__SHIFT);
}

static inline AL_BOOL AlIic_ll_ClrRxUnder(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + I2C__IC_CLR_RX_UNDER__OFFSET, I2C__IC_CLR_RX_UNDER__CLR_RX_UNDER__SHIFT);
}

static inline AL_U32 AlIic_ll_ClrRxOver(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_CLR_RX_OVER__OFFSET, I2C__IC_CLR_RX_OVER__CLR_RX_OVER__SHIFT);
}

static inline AL_U32 AlIic_ll_ClrTxOver(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_CLR_TX_OVER__OFFSET, I2C__IC_CLR_TX_OVER__CLR_TX_OVER__SHIFT);
}

static inline AL_U32 AlIic_ll_ClrRdReq(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_CLR_RD_REQ__OFFSET, I2C__IC_CLR_RD_REQ__CLR_RD_REQ__SHIFT);
}

static inline AL_U32 AlIic_ll_ClrTxAbrt(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_CLR_TX_ABRT__OFFSET, I2C__IC_CLR_TX_ABRT__CLR_TX_ABRT__SHIFT);
}

static inline AL_U32 AlIic_ll_ClrRxDone(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_CLR_RX_DONE__OFFSET, I2C__IC_CLR_RX_DONE__CLR_RX_DONE__SHIFT);
}

static inline AL_U32 AlIic_ll_ClrActivity(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_CLR_ACTIVITY__OFFSET, I2C__IC_CLR_ACTIVITY__CLR_ACTIVITY__SHIFT);
}

static inline AL_U32 AlIic_ll_ClrStopDet(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_CLR_STOP_DET__OFFSET, I2C__IC_CLR_STOP_DET__CLR_STOP_DET__SHIFT);
}

static inline AL_U32 AlIic_ll_ClrStartDet(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_CLR_START_DET__OFFSET, I2C__IC_CLR_START_DET__CLR_START_DET__SHIFT);
}

static inline AL_U32 AlIic_ll_ClrGenCall(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_CLR_GEN_CALL__OFFSET, I2C__IC_CLR_GEN_CALL__CLR_GEN_CALL__SHIFT);
}

static inline AL_U32 AlIic_ll_ClrSclStuckDet(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + I2C__IC_CLR_SCL_STUCK_DET__OFFSET, I2C__IC_CLR_SCL_STUCK_DET__CLR_SCL_STUCK_DET__SHIFT);
}

static inline AL_IIC_AbrtSrcEnum AlIic_ll_GetAbrtSrc(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + I2C__IC_TX_ABRT_SOURCE__OFFSET,
                      I2C__IC_TX_ABRT_SOURCE__ABRT_7B_ADDR_NOACK__SHIFT, I2C__IC_TX_ABRT_SOURCE__SIZE);
}

static inline AL_VOID AlIic_ll_SetSpkLen(AL_REG BaseAddr, AL_U8 Count)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_FS_SPKLEN__OFFSET,
                      I2C__IC_FS_SPKLEN__IC_FS_SPKLEN__SHIFT, I2C__IC_FS_SPKLEN__IC_FS_SPKLEN__SIZE, Count);
}

static inline AL_VOID AlIic_ll_SetSdaSetUp(AL_REG BaseAddr, AL_U8 Count)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_SDA_SETUP__OFFSET,
                      I2C__IC_SDA_SETUP__SDA_SETUP__SHIFT, I2C__IC_SDA_SETUP__SDA_SETUP__SIZE, Count);
}

static inline AL_VOID AlIic_ll_SetSdaTxHold(AL_REG BaseAddr, AL_U8 Count)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_SDA_HOLD__OFFSET,
                      I2C__IC_SDA_HOLD__IC_SDA_TX_HOLD__SHIFT, I2C__IC_SDA_HOLD__IC_SDA_TX_HOLD__SIZE, Count);
}

static inline AL_VOID AlIic_ll_SetSdaRxHold(AL_REG BaseAddr, AL_U8 Count)
{
    AL_REG32_SET_BITS(BaseAddr + I2C__IC_SDA_HOLD__OFFSET,
                      I2C__IC_SDA_HOLD__IC_SDA_RX_HOLD__SHIFT, I2C__IC_SDA_HOLD__IC_SDA_RX_HOLD__SIZE, Count);
}

static inline AL_VOID AlIic_ll_SetSclStuckLowTimeout(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + I2C__IC_SCL_STUCK_AT_LOW_TIMEOUT__OFFSET, Value);
}

static inline AL_VOID AlIic_ll_SetSdaStuckLowTimeout(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + I2C__IC_SDA_STUCK_AT_LOW_TIMEOUT__OFFSET, Value);
}

static inline AL_U32 AlIic_ll_GetRxBufferDepth(AL_REG BaseAddr)
{
    return (AL_REG32_GET_BITS(BaseAddr + I2C__IC_COMP_PARAM_1__OFFSET, I2C__IC_COMP_PARAM_1__RX_BUFFER_DEPTH__SHIFT,
                             I2C__IC_COMP_PARAM_1__RX_BUFFER_DEPTH__SIZE) + 1);
}

static inline AL_U32 AlIic_ll_GetTxBufferDepth(AL_REG BaseAddr)
{
    return (AL_REG32_GET_BITS(BaseAddr + I2C__IC_COMP_PARAM_1__OFFSET, I2C__IC_COMP_PARAM_1__TX_BUFFER_DEPTH__SHIFT,
                             I2C__IC_COMP_PARAM_1__TX_BUFFER_DEPTH__SIZE) + 1);
}

#ifdef __cplusplus
}
#endif

#endif /* AL_IIC_LL_H */