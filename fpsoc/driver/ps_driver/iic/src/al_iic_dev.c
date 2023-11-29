/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_iic_dev.h"

#define IS_SAME_INITCONFIGS(Dest, Src)                                          \
(                                                                               \
    (Dest).Mode == (Src).Mode && (Dest).AddrMode == (Src).AddrMode &&           \
    (Dest).SpeedMode == (Src).SpeedMode && (Dest).SlaveAddr == (Src).SlaveAddr  \
)

static AL_IIC_InitStruct IicDefInitConfigs =
{
    .Mode           = AL_IIC_MODE_MASTER,
    .AddrMode       = AL_IIC_ADDR_7BIT,
    .SpeedMode      = AL_IIC_STANDARD_MODE,
};

extern AL_IIC_HwConfigStruct AlIic_HwConfig[AL_IIC_NUM_INSTANCE];

/**
 * This function look up hardware config structure.
 * @param   DeviceId is hardware module id
 * @return
 *          - AL_IIC_HwConfigStruct for hardware config
 * @note
*/
AL_IIC_HwConfigStruct *AlIic_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AL_IIC_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_IIC_NUM_INSTANCE; Index++) {
        if (AlIic_HwConfig[Index].DeviceId == DevId) {
            ConfigPtr = &AlIic_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

/**
 * This function check whether the iic tx is busy.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 *          - AL_BOOL for iic tx status
 * @note
*/
AL_BOOL AlIic_Dev_IsTxBusy(AL_IIC_DevStruct *Iic)
{
    return (AL_BOOL)(Iic->State & AL_IIC_STATE_TX_BUSY);
}

/**
 * This function check whether the iic rx is busy.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 *          - AL_BOOL for iic rx status
 * @note
*/
AL_BOOL AlIic_Dev_IsRxBusy(AL_IIC_DevStruct *Iic)
{
    return (AL_BOOL)(Iic->State & AL_IIC_STATE_RX_BUSY);
}

/**
 * This function set iic tx status to busy.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
AL_VOID AlIic_Dev_SetTxBusy(AL_IIC_DevStruct *Iic)
{
    Iic->State |= AL_IIC_STATE_TX_BUSY;

    if (Iic->EventCallBack) {
        AL_IIC_EventStruct IicEvent = {
            .Events        = AL_IIC_EVENT_READY_TO_TX
        };
        Iic->EventCallBack(&IicEvent, Iic->EventCallBackRef);
    }
}

/**
 * This function set iic rx status to busy.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
AL_VOID AlIic_Dev_SetRxBusy(AL_IIC_DevStruct *Iic)
{
    Iic->State |= AL_IIC_STATE_RX_BUSY;

    if (Iic->EventCallBack) {
        AL_IIC_EventStruct IicEvent = {
            .Events        = AL_IIC_EVENT_READY_TO_RX
        };
        Iic->EventCallBack(&IicEvent, Iic->EventCallBackRef);
    }
}

/**
 * This function clear iic tx busy status.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
AL_VOID AlIic_Dev_ClrTxBusy(AL_IIC_DevStruct *Iic)
{
    Iic->State &= (~AL_IIC_STATE_TX_BUSY);
}

/**
 * This function clear iic rx busy status.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
AL_VOID AlIic_Dev_ClrRxBusy(AL_IIC_DevStruct *Iic)
{
    Iic->State &= (~AL_IIC_STATE_RX_BUSY);
}

/**
 * This function config the clock.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   SpeedMode Speed Mode
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_InitSclHighLowCout(AL_IIC_DevStruct *Iic, AL_IIC_SpeedModeEnum SpeedMode)
{
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);
    AL_U16 ClockMHz = (Iic->HwConfig.ClockHz) / 1000000;

    if (SpeedMode == AL_IIC_STANDARD_MODE) {
        AL_U16 SsSclHigh, SsSclLow;

        SsSclHigh = ((AL_U16) (((AL_IIC_SS_MIN_SCL_HIGH * ClockMHz) / 1000) + (25-7)));
        SsSclLow = ((AL_U16) (((AL_IIC_SS_MIN_SCL_LOW * ClockMHz) / 1000) + 25));

        AlIic_ll_SetStdSpeedSclHighCount(IicBaseAddr, SsSclHigh);
        AlIic_ll_SetStdSpeedSclLowCount(IicBaseAddr, SsSclLow);
    } else if (SpeedMode == AL_IIC_FAST_MODE) {
        AL_U16 FsSclHigh, FsSclLow;

        FsSclHigh = ((AL_U16) (((AL_IIC_FS_MIN_SCL_HIGH * ClockMHz) / 1000) + (25-7)));
        FsSclLow = ((AL_U16) (((AL_IIC_FS_MIN_SCL_LOW * ClockMHz) / 1000) + 25));

        AlIic_ll_SetFastSpeedSclHighCount(IicBaseAddr, FsSclHigh);
        AlIic_ll_SetFastSpeedSclLowCount(IicBaseAddr, FsSclLow);
    } else {
        AL_U16 HsSclHigh, HsSclLow;

        HsSclHigh = ((AL_U16) (((AL_IIC_HS_MIN_SCL_HIGH_100PF * ClockMHz) / 1000) + (25-7)));
        HsSclLow  = ((AL_U16) (((AL_IIC_HS_MIN_SCL_LOW_100PF * ClockMHz) / 100) + 25));

        AlIic_ll_SetHighSpeedSclHighCount(IicBaseAddr, HsSclHigh);
        AlIic_ll_SetHighSpeedSclLowCount(IicBaseAddr, HsSclLow);
    }
}

/**
 * This function check the configure to be config.
 * @param   InitConfig The Initial configuration
 * @return
 * @note
*/
static AL_S32 AlIic_Dev_CheckConfigParam(AL_IIC_InitStruct *InitConfig)
{
    AL_ASSERT ((InitConfig->Mode == AL_IIC_MODE_SLAVE) || ((InitConfig->Mode == AL_IIC_MODE_MASTER) &&
              ((InitConfig->SpeedMode == AL_IIC_STANDARD_MODE || InitConfig->SpeedMode == AL_IIC_FAST_MODE ||
                InitConfig->SpeedMode == AL_IIC_HIGH_SPEED_MODE))), AL_IIC_ERR_ILLEGAL_PARAM);

    AL_ASSERT(((InitConfig->AddrMode == AL_IIC_ADDR_7BIT) || (InitConfig->AddrMode == AL_IIC_ADDR_10BIT)),
              AL_IIC_ERR_ILLEGAL_PARAM);

    return AL_OK;
}

/**
 * This function initialize IIC registers according to the specified parameters in AL_IIC_InitStruct.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   HwConfig IIC hardware configure
 * @param   InitConfig pointer to a AL_IIC_InitStruct structure
 *          that contains the configuration information for the specified IIC peripheral
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlIic_Dev_Init(AL_IIC_DevStruct *Iic, AL_IIC_HwConfigStruct *HwConfig, AL_IIC_InitStruct *InitConfig)
{
    AL_S32 Ret;
    AL_REG IicBaseAddr;
    AL_U32 RxFifoDepth;
    AL_U32 TxFifoDepth;

    AL_ASSERT((Iic != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    if (Iic->State & AL_IIC_STATE_READY) {
        if ((InitConfig != AL_NULL) && IS_SAME_INITCONFIGS(Iic->Configs, *InitConfig)) {
            return AL_OK;
        }
    }

    /* check the InitConfig */
    if (InitConfig != AL_NULL) {
        Ret = AlIic_Dev_CheckConfigParam(InitConfig);
        if (Ret != AL_OK) {
            return Ret;
        }

        Iic->Configs = *InitConfig;
    } else {
        Iic->Configs = IicDefInitConfigs;
    }

    Iic->HwConfig = *HwConfig;
    IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    AlIic_ll_SetEnable(IicBaseAddr, AL_IIC_FUNC_DISABLE);

    if (InitConfig->Mode == AL_IIC_MODE_MASTER) {
        AlIic_ll_SetSlaveDisable(IicBaseAddr, AL_IIC_FUNC_ENABLE);
        AlIic_ll_SetRestartEnable(IicBaseAddr, AL_IIC_FUNC_ENABLE);
        AlIic_ll_SetMasterAddrMode(IicBaseAddr, InitConfig->AddrMode);

        AlIic_ll_SetSpeedMode(IicBaseAddr, InitConfig->SpeedMode);
        AlIic_ll_SetMasterEnable(IicBaseAddr, AL_IIC_FUNC_ENABLE);
        AlIic_Dev_InitSclHighLowCout(Iic, InitConfig->SpeedMode);

        Iic->CmdOption = AL_IIC_CMD_OPTION_STOP;
    } else {
        AlIic_ll_SetSlaveAddr(IicBaseAddr, InitConfig->SlaveAddr);
        AlIic_ll_SetSlaveDisable(IicBaseAddr, AL_IIC_FUNC_DISABLE);
        AlIic_ll_SetSlaveAddrMode(IicBaseAddr, InitConfig->AddrMode);
        AlIic_ll_SetMasterEnable(IicBaseAddr, AL_IIC_FUNC_DISABLE);
    }

    AlIic_ll_MaskAllIntr(IicBaseAddr);

    /* Bus clear feture */
    AlIic_ll_SetBusClrFeatureCtrl(IicBaseAddr, AL_IIC_FUNC_ENABLE);
    AlIic_ll_SetSclStuckLowTimeout(IicBaseAddr, DEFAULT_SDA_SCL_AT_LOW_TIMEOUT);
    AlIic_ll_SetSdaStuckLowTimeout(IicBaseAddr, DEFAULT_SDA_SCL_AT_LOW_TIMEOUT);
    AlIic_ll_SetSclStuckAtLowIntr(IicBaseAddr, AL_IIC_FUNC_ENABLE);

    AlIic_ll_SetSdaTxHold(IicBaseAddr, 4);
    AlIic_ll_SetSdaRxHold(IicBaseAddr, 1);


    /* Get fifo depth, by default, the threshold is set to half the depth */
    RxFifoDepth = AlIic_ll_GetRxBufferDepth(IicBaseAddr);
    TxFifoDepth = AlIic_ll_GetTxBufferDepth(IicBaseAddr);
    AlIic_ll_SetRxFifoThr(IicBaseAddr, RxFifoDepth/2);
    AlIic_ll_SetTxFifoThr(IicBaseAddr, TxFifoDepth/2);

    AlIic_ll_SetEnable(IicBaseAddr, AL_IIC_FUNC_ENABLE);

    Iic->State |= AL_IIC_STATE_READY;

    return AL_OK;
}

/**
 * This function used when master send or receive data,
 * to config slave address.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   SlaveAddr Slave address
 * @return
 * @note
*/
static inline AL_VOID AlIic_Dev_MasterSetTar(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr)
{
    /* I2C_DYNAMIC_TAR_UPDATE not enabled, need disable IIC first */
    AlIic_ll_SetEnable((AL_REG)(Iic->HwConfig.BaseAddress), AL_IIC_FUNC_DISABLE);

    AlIic_ll_SetTar((AL_REG)(Iic->HwConfig.BaseAddress), SlaveAddr);

    AlIic_ll_SetEnable((AL_REG)(Iic->HwConfig.BaseAddress), AL_IIC_FUNC_ENABLE);
}

/**
 * This function used when master send data in block mode, enable/disable interrupt.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   State AL_IIC_FUNC_DISABLE or AL_IIC_FUNC_DISABLE
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_EnableMasterSendIntr(AL_IIC_DevStruct *Iic, AL_IIC_FunctionEnum State)
{
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    AL_U32 IntrMask = AlIic_ll_GetIntrMask(IicBaseAddr);

    if (State == AL_IIC_FUNC_ENABLE)
        IntrMask |= (AL_IIC_INTR_TX_EMPTY_MASK | AL_IIC_INTR_TX_ABRT_MASK | AL_IIC_INTR_TX_OVER_MASK);
    else
        IntrMask &= ~(AL_IIC_INTR_TX_EMPTY_MASK | AL_IIC_INTR_TX_ABRT_MASK | AL_IIC_INTR_TX_OVER_MASK);

    AlIic_ll_SetIntrMask(IicBaseAddr, IntrMask);
}

/**
 * This function master send an amount of data in blocking & interrupt mode
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   SlaveAddr Slave address
 * @param   SendBuf Pointer to data buffer
 * @param   SendSize Amount of data to be sent
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlIic_Dev_MasterSendData(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_ASSERT((Iic != AL_NULL) && (Iic->Configs.Mode == AL_IIC_MODE_MASTER) &&
              (SendBuf != AL_NULL) && (SendSize > 0) && (SlaveAddr < AL_IIC_MAX_SLAVE_ADDR), AL_IIC_ERR_ILLEGAL_PARAM);

    AL_ASSERT(((Iic->State & AL_IIC_STATE_READY) == AL_IIC_STATE_READY), AL_IIC_ERR_NOT_READY);
    AL_ASSERT((!AlIic_Dev_IsTxBusy(Iic)), AL_IIC_ERR_BUSY);

    /* Change Status */
    AlIic_Dev_SetTxBusy(Iic);

    AlIic_Dev_MasterSetTar(Iic, SlaveAddr);

    Iic->OpState = AL_IIC_OP_STATE_MASTER_TX;

    Iic->SendBuffer.BufferPtr      = SendBuf;
    Iic->SendBuffer.RequestedCnt   = SendSize;
    Iic->SendBuffer.HandledCnt     = 0;

    AlIic_Dev_EnableMasterSendIntr(Iic, AL_IIC_FUNC_ENABLE);

    return AL_OK;
}

AL_S32 AlIic_Dev_MasterDmaSendData(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr)
{
    AL_ASSERT((Iic != AL_NULL) && (Iic->Configs.Mode == AL_IIC_MODE_MASTER) &&
              (SlaveAddr < AL_IIC_MAX_SLAVE_ADDR), AL_IIC_ERR_ILLEGAL_PARAM);

    AL_ASSERT(((Iic->State & AL_IIC_STATE_READY) == AL_IIC_STATE_READY), AL_IIC_ERR_NOT_READY);
    AL_ASSERT((!AlIic_Dev_IsTxBusy(Iic)), AL_IIC_ERR_BUSY);

    AlIic_Dev_MasterSetTar(Iic, SlaveAddr);

    /* I2C_DYNAMIC_TAR_UPDATE not enabled, need disable IIC first */
    AlIic_ll_SetEnable((AL_REG)(Iic->HwConfig.BaseAddress), AL_IIC_FUNC_DISABLE);

    AlIic_ll_EnableTxDma(Iic->HwConfig.BaseAddress, AL_IIC_FUNC_ENABLE);
    AlIic_ll_SetTxFifoThr(Iic->HwConfig.BaseAddress, 0);
    AlIic_ll_SetDmaTransLevel(Iic->HwConfig.BaseAddress, 0);
    AlIic_ll_SetEnable((AL_REG)(Iic->HwConfig.BaseAddress), AL_IIC_FUNC_ENABLE);

    return AL_OK;
}

/**
 * This function used when master send data in block mode, stop send process.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
AL_VOID AlIic_Dev_StopMasterSend(AL_IIC_DevStruct *Iic)
{
    AlIic_Dev_EnableMasterSendIntr(Iic, AL_IIC_FUNC_DISABLE);

    AlIic_Dev_ClrTxBusy(Iic);
}

/**
 * This function master send an amount of data in polling(non interrupt) & blocking mode.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   SlaveAddr Slave address
 * @param   SendBuf Pointer to data buffer
 * @param   SendSize Amount of data to be sent
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlIic_Dev_MasterSendDataPolling(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_U32 HandledCnt = 0;
    AL_U8 Data;
    AL_REG IicBaseAddr;
    AL_U16 DataCmd;

    AL_ASSERT((Iic != AL_NULL) && (Iic->Configs.Mode == AL_IIC_MODE_MASTER) &&
              (SendBuf != AL_NULL) && (SendSize > 0) && (SlaveAddr < AL_IIC_MAX_SLAVE_ADDR), AL_IIC_ERR_ILLEGAL_PARAM);

    AL_ASSERT(((Iic->State & AL_IIC_STATE_READY) == AL_IIC_STATE_READY), AL_IIC_ERR_NOT_READY);
    AL_ASSERT((!AlIic_Dev_IsTxBusy(Iic)), AL_IIC_ERR_BUSY);

    /* Change Status */
    AlIic_Dev_SetTxBusy(Iic);

    AlIic_Dev_MasterSetTar(Iic, SlaveAddr);

    IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    Iic->OpState = AL_IIC_OP_STATE_MASTER_TX;

    IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    while (HandledCnt < SendSize) {

        if (AlIic_ll_IsTxFifoNotFull(IicBaseAddr) == AL_TRUE) {

            Data = SendBuf[HandledCnt];

            if (HandledCnt !=  SendSize - 1 )
                DataCmd = AL_IIC_CMD_WRITE | Data;
            else {
                /* Last byte, need set cmd by option */
                if (Iic->CmdOption == AL_IIC_CMD_OPTION_STOP)
                    DataCmd = AL_IIC_CMD_WRITE | AL_IIC_STOP_ENABLE | Data;
                else if (Iic->CmdOption == AL_IIC_CMD_OPTION_RESTART)
                    DataCmd = AL_IIC_CMD_WRITE | AL_IIC_RESTART_ENABLE | Data;
                else
                    DataCmd = AL_IIC_CMD_WRITE | Data;
            }

            AlIic_ll_SetDataCmd(IicBaseAddr, DataCmd);

            HandledCnt++;
        }

    }

    if (Iic->CmdOption == AL_IIC_CMD_OPTION_STOP) {
        while(AlIic_ll_IsMasterActivity(IicBaseAddr));
    }

    AlIic_Dev_ClrTxBusy(Iic);

    return AL_OK;
}

/**
 * This function used when master receive data in block mode, enable/disable interrupt.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   State AL_IIC_FUNC_DISABLE or AL_IIC_FUNC_DISABLE
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_EnableMasterRecvIntr(AL_IIC_DevStruct *Iic, AL_IIC_FunctionEnum State)
{
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    AL_U32 IntrMask = AlIic_ll_GetIntrMask(IicBaseAddr);

    if (State == AL_IIC_FUNC_ENABLE)
        IntrMask |= (AL_IIC_INTR_TX_EMPTY_MASK | AL_IIC_INTR_RX_FULL_MASK | AL_IIC_INTR_TX_ABRT_MASK |
                     AL_IIC_INTR_RX_UNDER_MASK | AL_IIC_INTR_RX_OVER_MASK);
    else
        IntrMask &= ~(AL_IIC_INTR_TX_EMPTY_MASK | AL_IIC_INTR_RX_FULL_MASK | AL_IIC_INTR_TX_ABRT_MASK |
                      AL_IIC_INTR_RX_UNDER_MASK | AL_IIC_INTR_RX_OVER_MASK);

    AlIic_ll_SetIntrMask(IicBaseAddr, IntrMask);
}

/**
 * This function master receive an amount of data in blocking & interrupt mode
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   SlaveAddr Slave address
 * @param   RecvBuf Pointer to data buffer
 * @param   RecvSize Amount of data to be receive
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlIic_Dev_MasterRecvData(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr, AL_U8 *RecvBuf, AL_U32 RecvSize)
{
    AL_REG IicBaseAddr;
    AL_U8 RxFifoThrLevel;

    AL_ASSERT((Iic != AL_NULL) && (Iic->Configs.Mode == AL_IIC_MODE_MASTER) &&
              (RecvBuf != AL_NULL) && (RecvSize > 0) && (SlaveAddr < AL_IIC_MAX_SLAVE_ADDR), AL_IIC_ERR_ILLEGAL_PARAM);

    AL_ASSERT(((Iic->State & AL_IIC_STATE_READY) == AL_IIC_STATE_READY), AL_IIC_ERR_NOT_READY);
    AL_ASSERT((!AlIic_Dev_IsRxBusy(Iic)), AL_IIC_ERR_BUSY);

    /* Change Status */
    AlIic_Dev_SetRxBusy(Iic);

    AlIic_Dev_MasterSetTar(Iic, SlaveAddr);

    IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    Iic->OpState                    = AL_IIC_OP_STATE_MASTER_RX;

    Iic->RecvBuffer.BufferPtr       = RecvBuf;
    Iic->RecvBuffer.RequestedCnt    = RecvSize;
    Iic->RecvBuffer.HandledCnt      = 0;

    /*
       Use this buffer to record the number of read requests sent,
       equal to the length of the data received.
     */
    Iic->SendBuffer.RequestedCnt    = RecvSize;
    Iic->SendBuffer.HandledCnt      = 0;

    /* Make sure the fifo full interrupt is triggered, and receive data in the interrupt */
    RxFifoThrLevel = AlIic_ll_GetRxFifoThr(IicBaseAddr);
    if (RecvSize <= RxFifoThrLevel) {
        AlIic_ll_SetRxFifoThr(IicBaseAddr, RecvSize - 1);
    }

    AlIic_Dev_EnableMasterRecvIntr(Iic, AL_IIC_FUNC_ENABLE);

    return AL_OK;
}

AL_S32 AlIic_Dev_MasterDmaRecvData(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr, AL_U32 RecvSize)
{
    AL_ASSERT((Iic != AL_NULL) && (Iic->Configs.Mode == AL_IIC_MODE_MASTER) &&
              (SlaveAddr < AL_IIC_MAX_SLAVE_ADDR) && (RecvSize > 0), AL_IIC_ERR_ILLEGAL_PARAM);

    AL_ASSERT(((Iic->State & AL_IIC_STATE_READY) == AL_IIC_STATE_READY), AL_IIC_ERR_NOT_READY);
    AL_ASSERT((!AlIic_Dev_IsRxBusy(Iic)), AL_IIC_ERR_BUSY);

    Iic->OpState = AL_IIC_OP_STATE_MASTER_DMA_RX;

    /*
       Use this buffer to record the number of read requests sent,
       equal to the length of the data received.
     */
    Iic->SendBuffer.RequestedCnt    = RecvSize;
    Iic->SendBuffer.HandledCnt      = 0;

    AlIic_Dev_MasterSetTar(Iic, SlaveAddr);

    /* I2C_DYNAMIC_TAR_UPDATE not enabled, need disable IIC first */
    AlIic_ll_SetEnable((AL_REG)(Iic->HwConfig.BaseAddress), AL_IIC_FUNC_DISABLE);

    AlIic_ll_RxDmaEnable(Iic->HwConfig.BaseAddress);
    AlIic_ll_SetRxFifoThr(Iic->HwConfig.BaseAddress, 0);
    AlIic_ll_SetDmaRecevLevel(Iic->HwConfig.BaseAddress, 0);
    AlIic_ll_SetTxEmptyIntr((AL_REG)(Iic->HwConfig.BaseAddress),AL_IIC_FUNC_ENABLE);
    AlIic_ll_SetEnable((AL_REG)(Iic->HwConfig.BaseAddress), AL_IIC_FUNC_ENABLE);

    return AL_OK;
}

/**
 * This function used when master receive data in block mode, stop recvive process.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
AL_VOID AlIic_Dev_StopMasterRecv(AL_IIC_DevStruct *Iic)
{
    AlIic_Dev_EnableMasterRecvIntr(Iic, AL_IIC_FUNC_DISABLE);

    AlIic_Dev_ClrRxBusy(Iic);
}

/**
 * This function master receive an amount of data in polling(non interrupt) & blocking mode.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   SlaveAddr Slave address
 * @param   RecvBuf Pointer to data buffer
 * @param   RecvSize Amount of data to be receive
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlIic_Dev_MasterRecvDataPolling(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr, AL_U8 *RecvBuf, AL_U32 RecvSize)
{
    AL_U8 Data = 0;
    AL_U16 Cmd = 0;
    AL_U32 HandledCnt = 0;
    AL_U32 IssuereadCnt = 0;
    AL_REG IicBaseAddr;

    AL_ASSERT((Iic != AL_NULL) && (Iic->Configs.Mode == AL_IIC_MODE_MASTER) &&
              (RecvBuf != AL_NULL) && (RecvSize > 0) && (SlaveAddr < AL_IIC_MAX_SLAVE_ADDR), AL_IIC_ERR_ILLEGAL_PARAM);

    AL_ASSERT(((Iic->State & AL_IIC_STATE_READY) == AL_IIC_STATE_READY), AL_IIC_ERR_NOT_READY);
    AL_ASSERT((!AlIic_Dev_IsRxBusy(Iic)), AL_IIC_ERR_BUSY);

    /* Change Status */
    AlIic_Dev_SetRxBusy(Iic);

    AlIic_Dev_MasterSetTar(Iic, SlaveAddr);

    IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    Iic->OpState                    = AL_IIC_OP_STATE_MASTER_RX;

    while (HandledCnt < RecvSize) {

        while ((AlIic_ll_IsTxFifoNotFull(IicBaseAddr) == AL_TRUE) &&
               (IssuereadCnt < RecvSize)) {

            if (IssuereadCnt != (RecvSize - 1))
                Cmd = AL_IIC_CMD_READ;
            else {
                /* Last byte, need set cmd by option */
                if (Iic->CmdOption == AL_IIC_CMD_OPTION_STOP)
                    Cmd = AL_IIC_CMD_READ | AL_IIC_STOP_ENABLE;
                else if (Iic->CmdOption == AL_IIC_CMD_OPTION_RESTART)
                    Cmd = AL_IIC_CMD_READ | AL_IIC_RESTART_ENABLE;
                else
                    Cmd = AL_IIC_CMD_WRITE;
            }

            AlIic_ll_SetDataCmd(IicBaseAddr, Cmd);

            IssuereadCnt++;
        }

        while ((AlIic_ll_IsRxFifoNotEmpty(IicBaseAddr) == AL_TRUE) &&
               (HandledCnt < RecvSize)) {

            Data = AlIic_ll_ReadDataCmdData(IicBaseAddr);
            RecvBuf[HandledCnt] = Data;

            HandledCnt++;
        }

    }

    if (Iic->CmdOption == AL_IIC_CMD_OPTION_STOP) {
        while(AlIic_ll_IsMasterActivity(IicBaseAddr));
    }

    AlIic_Dev_ClrRxBusy(Iic);

    return AL_OK;
}

/**
 * This function used when master send data in block mode, enable/disable interrupt.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   State AL_IIC_FUNC_DISABLE or AL_IIC_FUNC_DISABLE
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_EnableSlaveSendIntr(AL_IIC_DevStruct *Iic, AL_IIC_FunctionEnum State)
{
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    AL_U32 IntrMask = AlIic_ll_GetIntrMask(IicBaseAddr);

    if (State == AL_IIC_FUNC_ENABLE)
        IntrMask |= (AL_IIC_INTR_TX_ABRT_MASK | AL_IIC_INTR_RD_REQ_MASK |
                     AL_IIC_INTR_TX_OVER_MASK | AL_IIC_INTR_RX_DONE_MASK);
    else
        IntrMask &= ~(AL_IIC_INTR_TX_ABRT_MASK | AL_IIC_INTR_RD_REQ_MASK |
                     AL_IIC_INTR_TX_OVER_MASK | AL_IIC_INTR_RX_DONE_MASK);

    AlIic_ll_SetIntrMask(IicBaseAddr, IntrMask);
}

/**
 * This function slave send an amount of data in blocking & interrupt mode
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   SendBuf Pointer to data buffer
 * @param   SendSize Amount of data to be sent
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlIic_Dev_SlaveSendData(AL_IIC_DevStruct *Iic, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_ASSERT((Iic != AL_NULL) && (Iic->Configs.Mode == AL_IIC_MODE_SLAVE) &&
              (SendBuf != AL_NULL) && (SendSize > 0), AL_IIC_ERR_ILLEGAL_PARAM);

    AL_ASSERT(((Iic->State & AL_IIC_STATE_READY) == AL_IIC_STATE_READY), AL_IIC_ERR_NOT_READY);
    AL_ASSERT((!AlIic_Dev_IsRxBusy(Iic)), AL_IIC_ERR_BUSY);

    /* Change Status */
    AlIic_Dev_SetTxBusy(Iic);

    Iic->OpState = AL_IIC_OP_STATE_SLAVE_TX;

    Iic->SendBuffer.BufferPtr      = SendBuf;
    Iic->SendBuffer.RequestedCnt   = SendSize;
    Iic->SendBuffer.HandledCnt     = 0;

    AlIic_Dev_EnableSlaveSendIntr(Iic, AL_IIC_FUNC_ENABLE);

    return AL_OK;
}

/**
 * This function used when slave send data in block mode, stop recvive process.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
AL_VOID AlIic_Dev_StopSlaveSend(AL_IIC_DevStruct *Iic)
{
    AlIic_Dev_EnableSlaveSendIntr(Iic, AL_IIC_FUNC_DISABLE);

    AlIic_Dev_ClrTxBusy(Iic);
}

/**
 * This function used when slave receive data in block mode, enable/disable interrupt.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   State AL_IIC_FUNC_DISABLE or AL_IIC_FUNC_DISABLE
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_EnableSlaveRecvIntr(AL_IIC_DevStruct *Iic, AL_IIC_FunctionEnum State)
{
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    AL_U32 IntrMask = AlIic_ll_GetIntrMask(IicBaseAddr);

    if (State == AL_IIC_FUNC_ENABLE)
        IntrMask |= (AL_IIC_INTR_RX_FULL_MASK | AL_IIC_INTR_RX_UNDER_MASK | AL_IIC_INTR_RX_OVER_MASK);
    else
        IntrMask &= ~(AL_IIC_INTR_RX_FULL_MASK | AL_IIC_INTR_RX_UNDER_MASK | AL_IIC_INTR_RX_OVER_MASK);

    AlIic_ll_SetIntrMask(IicBaseAddr, IntrMask);
}

/**
 * This function slave receive an amount of data in blocking & interrupt mode
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   RecvBuf Pointer to data buffer
 * @param   RecvSize Amount of data to be sent
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlIic_Dev_SlaveRecvData(AL_IIC_DevStruct *Iic, AL_U8 *RecvBuf, AL_U32 RecvSize)
{
    AL_REG IicBaseAddr;
    AL_U8 RxFifoThrLevel;

    AL_ASSERT((Iic != AL_NULL) && (Iic->Configs.Mode == AL_IIC_MODE_SLAVE) &&
              (RecvBuf != AL_NULL) && (RecvSize > 0), AL_IIC_ERR_ILLEGAL_PARAM);

    AL_ASSERT(((Iic->State & AL_IIC_STATE_READY) == AL_IIC_STATE_READY), AL_IIC_ERR_NOT_READY);
    AL_ASSERT((!AlIic_Dev_IsRxBusy(Iic)), AL_IIC_ERR_BUSY);

    /* Change Status */
    AlIic_Dev_SetRxBusy(Iic);

    IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    Iic->OpState = AL_IIC_OP_STATE_SLAVE_RX;

    Iic->RecvBuffer.BufferPtr      = RecvBuf;
    Iic->RecvBuffer.RequestedCnt   = RecvSize;
    Iic->RecvBuffer.HandledCnt     = 0;

    /* Make sure the fifo full interrupt is triggered, and receive data in the interrupt */
    RxFifoThrLevel = AlIic_ll_GetRxFifoThr(IicBaseAddr);
    if (RecvSize <= RxFifoThrLevel) {
        AlIic_ll_SetRxFifoThr(IicBaseAddr, RecvSize - 1);
    }

    AlIic_Dev_EnableSlaveRecvIntr(Iic, AL_IIC_FUNC_ENABLE);

    return AL_OK;
}

/**
 * This function used when slave receive data in block mode, stop recvive process.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
AL_VOID AlIic_Dev_StopSlaveRecv(AL_IIC_DevStruct *Iic)
{
    AlIic_Dev_EnableSlaveRecvIntr(Iic, AL_IIC_FUNC_DISABLE);

    AlIic_Dev_ClrRxBusy(Iic);
}

/**
 * This function used when master send data in block&interrupt mode,
 * send data in the interrupt.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_MasterSendDataHandler(AL_IIC_DevStruct *Iic)
{
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);
    AL_U8 Data;
    AL_U16 DataCmd;

    while ((AlIic_ll_IsTxFifoNotFull(IicBaseAddr) == AL_TRUE) &&
           (Iic->SendBuffer.HandledCnt < Iic->SendBuffer.RequestedCnt)) {

        Data = Iic->SendBuffer.BufferPtr[Iic->SendBuffer.HandledCnt];

        if (Iic->SendBuffer.HandledCnt != Iic->SendBuffer.RequestedCnt - 1 )
            DataCmd = AL_IIC_CMD_WRITE | Data;
        else {
            /* Last byte, need set cmd by option */
            if (Iic->CmdOption == AL_IIC_CMD_OPTION_STOP)
                DataCmd = AL_IIC_CMD_WRITE | AL_IIC_STOP_ENABLE | Data;
            else if (Iic->CmdOption == AL_IIC_CMD_OPTION_RESTART)
                DataCmd = AL_IIC_CMD_WRITE | AL_IIC_RESTART_ENABLE | Data;
            else
                DataCmd = AL_IIC_CMD_WRITE | Data;
        }

        AlIic_ll_SetDataCmd(IicBaseAddr, DataCmd);

        Iic->SendBuffer.HandledCnt++;
    }

    if (Iic->SendBuffer.HandledCnt == Iic->SendBuffer.RequestedCnt) {
        if (Iic->EventCallBack) {
            AL_IIC_EventStruct IicEvent = {
                .Events        = AL_IIC_EVENT_TX_DONE,
                .EventData     = Iic->SendBuffer.HandledCnt,
            };
            (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
        }

        /*
          It is possible for an abrt interrupt to occur, so only mask the empty interrupt
          and call AlIic_Dev_StopMasterSend after tx is done.
        */
        AlIic_ll_DisableIntr(IicBaseAddr, AL_IIC_INTR_TX_EMPTY_MASK);
    }
}

/**
 * This function used when master receive data in block&interrupt mode,
 * receive data in the interrupt.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_MasterRecvDataHandler(AL_IIC_DevStruct *Iic)
{
    AL_U8 Data;
    AL_U32 RxRemainCnt;
    AL_U8 RxFifoThrLevel;
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    while ((AlIic_ll_IsRxFifoNotEmpty(IicBaseAddr) == AL_TRUE) &&
           (Iic->RecvBuffer.HandledCnt < Iic->RecvBuffer.RequestedCnt)) {

        Data = AlIic_ll_ReadDataCmdData(IicBaseAddr);
        Iic->RecvBuffer.BufferPtr[Iic->RecvBuffer.HandledCnt] = Data;

        Iic->RecvBuffer.HandledCnt++;
    }

    if (Iic->RecvBuffer.HandledCnt < Iic->RecvBuffer.RequestedCnt) {
        RxRemainCnt = Iic->RecvBuffer.RequestedCnt - Iic->RecvBuffer.HandledCnt;
        RxFifoThrLevel = AlIic_ll_GetRxFifoThr(IicBaseAddr);
        if (RxRemainCnt <= RxFifoThrLevel) {
            AlIic_ll_SetRxFifoThr(IicBaseAddr, RxRemainCnt - 1);
        }

    } else if (Iic->RecvBuffer.HandledCnt == Iic->RecvBuffer.RequestedCnt) {

        if (Iic->EventCallBack) {
            AL_IIC_EventStruct IicEvent = {
                .Events        = AL_IIC_EVENT_RX_DONE,
                .EventData     = Iic->RecvBuffer.HandledCnt,
            };
            (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
        }


        AlIic_ll_DisableIntr(IicBaseAddr, AL_IIC_INTR_RX_FULL_MASK | AL_IIC_INTR_RX_UNDER_MASK |
                                          AL_IIC_INTR_RX_OVER_MASK | AL_IIC_INTR_TX_ABRT_MASK);

        AlIic_Dev_ClrRxBusy(Iic);
    }

}

/**
 * This function used when master receive data in block&interrupt mode,
 * master issue read commands before receive data.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_MasterRecvDataIssueReadCmd(AL_IIC_DevStruct *Iic)
{
    AL_U16 Cmd = 0;
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    while ((AlIic_ll_IsTxFifoNotFull(IicBaseAddr) == AL_TRUE) &&
           (Iic->SendBuffer.HandledCnt < Iic->SendBuffer.RequestedCnt)) {

        if (Iic->SendBuffer.HandledCnt != (Iic->SendBuffer.RequestedCnt - 1))
            Cmd = AL_IIC_CMD_READ;
        else {
            /* Last byte, need set cmd by option */
            if (Iic->CmdOption == AL_IIC_CMD_OPTION_STOP)
                Cmd = AL_IIC_CMD_READ | AL_IIC_STOP_ENABLE;
            else if (Iic->CmdOption == AL_IIC_CMD_OPTION_RESTART)
                Cmd = AL_IIC_CMD_READ | AL_IIC_RESTART_ENABLE;
            else
                Cmd = AL_IIC_CMD_WRITE;
        }

        AlIic_ll_SetDataCmd(IicBaseAddr, Cmd);

        Iic->SendBuffer.HandledCnt++;
    }

    if (Iic->SendBuffer.HandledCnt == Iic->SendBuffer.RequestedCnt) {
        AlIic_ll_DisableIntr(IicBaseAddr, AL_IIC_INTR_TX_EMPTY_MASK);
    }

    /* After issue read, need to read data immediately. If no, the fifo may be full and data may be lost */
    AlIic_Dev_MasterRecvDataHandler(Iic);
}

static AL_VOID AlIic_Dev_MasterDmaRecvDataIssueReadCmd(AL_IIC_DevStruct *Iic)
{
    AL_U16 Cmd = 0;
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    while ((AlIic_ll_IsTxFifoNotFull(IicBaseAddr) == AL_TRUE) &&
           (Iic->SendBuffer.HandledCnt < Iic->SendBuffer.RequestedCnt)) {

        if (Iic->SendBuffer.HandledCnt != (Iic->SendBuffer.RequestedCnt - 1))
            Cmd = AL_IIC_CMD_READ;
        else {
            /* Last byte, need set cmd by option */
            if (Iic->CmdOption == AL_IIC_CMD_OPTION_STOP)
                Cmd = AL_IIC_CMD_READ | AL_IIC_STOP_ENABLE;
            else if (Iic->CmdOption == AL_IIC_CMD_OPTION_RESTART)
                Cmd = AL_IIC_CMD_READ | AL_IIC_RESTART_ENABLE;
            else
                Cmd = AL_IIC_CMD_WRITE;
        }

        AlIic_ll_SetDataCmd(IicBaseAddr, Cmd);

        Iic->SendBuffer.HandledCnt++;
    }

    if (Iic->SendBuffer.HandledCnt == Iic->SendBuffer.RequestedCnt) {
        AlIic_ll_DisableIntr(IicBaseAddr, AL_IIC_INTR_TX_EMPTY_MASK);
    }
}

/**
 * This function used when slave receive data in block&interrupt mode,
 * receive data in the interrupt.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_SlaveRecvDataHandler(AL_IIC_DevStruct *Iic)
{
    AL_U8 Data;
    AL_U8 RxFifoThrLevel;
    AL_U32 RxRemainCnt;
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    while ((AlIic_ll_IsRxFifoNotEmpty(IicBaseAddr) == AL_TRUE) &&
           (Iic->RecvBuffer.HandledCnt < Iic->RecvBuffer.RequestedCnt)) {

        Data = AlIic_ll_ReadDataCmdData(IicBaseAddr);
        Iic->RecvBuffer.BufferPtr[Iic->RecvBuffer.HandledCnt] = Data;

        Iic->RecvBuffer.HandledCnt++;
    }

    if (Iic->RecvBuffer.HandledCnt < Iic->RecvBuffer.RequestedCnt) {
        RxRemainCnt = Iic->RecvBuffer.RequestedCnt - Iic->RecvBuffer.HandledCnt;
        RxFifoThrLevel = AlIic_ll_GetRxFifoThr(IicBaseAddr);
        if (RxRemainCnt <= RxFifoThrLevel) {
            AlIic_ll_SetRxFifoThr(IicBaseAddr, RxRemainCnt - 1);
        }
    } else if (Iic->RecvBuffer.HandledCnt == Iic->RecvBuffer.RequestedCnt ) {

        if (Iic->EventCallBack) {
            AL_IIC_EventStruct IicEvent = {
                .Events        = AL_IIC_EVENT_RX_DONE,
                .EventData     = Iic->RecvBuffer.HandledCnt,
            };
            (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
        }

        AlIic_Dev_StopSlaveRecv(Iic);
    }
}

/**
 * This function used when slave send data in block&interrupt mode,
 * send data in the interrupt.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_SlaveSendDataHandler(AL_IIC_DevStruct *Iic)
{
    AL_U8 Data;
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    /* Just send one byte */
    if ((AlIic_ll_IsTxFifoNotFull(IicBaseAddr) == AL_TRUE) &&
        (Iic->SendBuffer.HandledCnt < Iic->SendBuffer.RequestedCnt)) {

        Data = Iic->SendBuffer.BufferPtr[Iic->SendBuffer.HandledCnt];

        AlIic_ll_SetDataCmd(IicBaseAddr, AL_IIC_CMD_WRITE | Data);

        Iic->SendBuffer.HandledCnt++;
    }

    if (Iic->SendBuffer.HandledCnt == Iic->SendBuffer.RequestedCnt) {

        if (Iic->EventCallBack) {
            AL_IIC_EventStruct IicEvent = {
                .Events        = AL_IIC_EVENT_RD_REQ,
                .EventData     = 0,
            };
            (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
        }

        AlIic_ll_DisableIntr(IicBaseAddr, AL_IIC_INTR_TX_OVER_MASK | AL_IIC_INTR_TX_ABRT_MASK |
                                          AL_IIC_INTR_RD_REQ_MASK);

        /* Clear Status */
        AlIic_Dev_ClrTxBusy(Iic);
    }
}

/**
 * This function used when slave send data in block&interrupt mode,
 * This occurs on the last byte of the transmission, indicating that the transmission is done.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_SlaveSendDataDoneHandler(AL_IIC_DevStruct *Iic)
{
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    if (Iic->EventCallBack) {
        AL_IIC_EventStruct IicEvent = {
            .Events        = AL_IIC_EVENT_TX_DONE,
            .EventData     =  Iic->SendBuffer.HandledCnt,
        };
        (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
    }

    /*
      When the DW_apb_i2c is acting as a slave-transmitter, this
      bit is set to 1 if the master does not acknowledge a
      transmitted byte. This occurs on the last byte of the
      transmission, indicating that the transmission is done.
     */
    AlIic_ll_SetRxDoneIntr(IicBaseAddr, AL_IIC_FUNC_DISABLE);
    AlIic_Dev_ClrRxBusy(Iic);
}

/**
 * This function used in block&interrupt mode, This occurs when error tx process.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_TxAbrtHandler(AL_IIC_DevStruct *Iic)
{
    AL_IIC_AbrtSrcEnum AbrtSrc = AlIic_ll_GetAbrtSrc(Iic->HwConfig.BaseAddress);

    if (Iic->EventCallBack) {
        AL_IIC_EventStruct IicEvent = {
            .Events        = AL_IIC_EVENT_TX_ABRT,
            .EventData     = AbrtSrc,
        };
        (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
    }

    if (AbrtSrc == AL_IIC_ABRT_SDA_STUCK_AT_LOW) {
        AlIic_ll_SetSdaStuckRecoveryEnable(Iic->HwConfig.BaseAddress, AL_IIC_FUNC_ENABLE);

        /* Poll for 9 scl clocks */
        while(AlIic_ll_GetSdaStuckRecoveryEnableStatus(Iic->HwConfig.BaseAddress));

        if (AlIic_ll_IsSdaStuckNotRecovery(Iic->HwConfig.BaseAddress)) {
            AL_LOG(AL_LOG_LEVEL_ALERT,"SDA at low timeout, Bus not recovered, need reset entire IIC\r\n");
        } else {
            AL_LOG(AL_LOG_LEVEL_NOTICE,"SDA at low timeout, Bus recovered\r\n");
        }
    }

}

/**
 * This function used in block&interrupt mode, error interrupt handler.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_ErrorIntrHandler(AL_IIC_DevStruct *Iic, AL_IIC_EventIdEnum EventId)
{
    /* Error occurred, Stop RX */
    if (EventId == AL_IIC_EVENT_RX_UNDER || EventId == AL_IIC_EVENT_RX_OVER ) {
        if (Iic->OpState == AL_IIC_OP_STATE_MASTER_RX) {
            AlIic_Dev_StopMasterRecv(Iic);
        } else {
            AlIic_Dev_StopSlaveRecv(Iic);
        }
    }

    /* Error occurred, Stop TX */
    if (EventId == AL_IIC_EVENT_TX_OVER ) {
        if (Iic->OpState == AL_IIC_OP_STATE_MASTER_TX) {
            AlIic_Dev_StopMasterSend(Iic);
        } else {
            AlIic_Dev_StopSlaveSend(Iic);
        }
    }

    if (Iic->EventCallBack) {
        AL_IIC_EventStruct IicEvent = {
            .Events         = EventId,
            .EventData      = 0,
        };
        (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
    }
}

/**
 * This function used in block&interrupt mode, normal interrupt handler.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
static AL_VOID AlIic_Dev_NormalIntrHandler(AL_IIC_DevStruct *Iic, AL_IIC_EventIdEnum EventId)
{
    if (Iic->EventCallBack) {
        AL_IIC_EventStruct IicEvent = {
            .Events         = EventId,
            .EventData      = 0,
        };
        (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
    }
}

#define IIC_IN_RX_UNDER_INTR(Status)            (Status & AL_IIC_INTR_RX_UNDER)
#define IIC_IN_RX_OVER_INTR(Status)             (Status & AL_IIC_INTR_RX_OVER)
#define IIC_IN_RX_FULL_INTR(Status)             (Status & AL_IIC_INTR_RX_FULL)
#define IIC_IN_TX_OVER_INTR(Status)             (Status & AL_IIC_INTR_TX_OVER)
#define IIC_IN_THRE_EMPTY_INTR(Status)          (Status & AL_IIC_INTR_TX_EMPTY)
#define IIC_IN_RD_REQ_INTR(Status)              (Status & AL_IIC_INTR_RD_REQ)
#define IIC_IN_TX_ABRT_INTR(Status)             (Status & AL_IIC_INTR_TX_ABRT)
#define IIC_IN_RX_DONE_INTR(Status)             (Status & AL_IIC_INTR_RX_DONE)
#define IIC_IN_ACTIVITY_INTR(Status)            (Status & AL_IIC_INTR_ACTIVITY)
#define IIC_IN_STOP_DET_INTR(Status)            (Status & AL_IIC_INTR_STOP_DET)
#define IIC_IN_START_DET_INTR(Status)           (Status & AL_IIC_INTR_START_DET)
#define IIC_IN_GEN_CALL_INTR(Status)            (Status & AL_IIC_INTR_GEN_CALL)
#define IIC_IN_RESTART_DET_INTR(Status)         (Status & AL_IIC_INTR_RESTART_DET)
#define IIC_INTR_MASTER_ON_HOLD_INTR(Status)    (Status & AL_IIC_INTR_MASTER_ON_HOLD)
#define IIC_INTR_SCL_STUCK_ATLOW_INTR(Status)   (Status & AL_IIC_INTR_SCL_STUCK_ATLOW)

/**
 * IIC interrupt handler
 * @param   instance Pointer to a AL_IIC_DevStruct structure
 * @return
 * @note
*/
AL_VOID AlIic_Dev_IntrHandler(AL_VOID *instance)
{
    AL_IIC_DevStruct *Iic = (AL_IIC_DevStruct *)instance;
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);
    AL_IIC_IntrStatusEnum IntrStatus = AlIic_ll_GetIntrStatus(IicBaseAddr);

    if (IIC_IN_RX_UNDER_INTR(IntrStatus)) {
        AlIic_Dev_ErrorIntrHandler(Iic, AL_IIC_EVENT_RX_UNDER);
        AlIic_ll_ClrRxUnder(IicBaseAddr);

    } else if (IIC_IN_RX_OVER_INTR(IntrStatus)) {
        AlIic_Dev_ErrorIntrHandler(Iic, AL_IIC_EVENT_RX_OVER);
        AlIic_ll_ClrRxOver(IicBaseAddr);

    } else if (IIC_IN_TX_OVER_INTR(IntrStatus)) {
        AlIic_Dev_ErrorIntrHandler(Iic, AL_IIC_EVENT_TX_OVER);
        AlIic_ll_ClrTxOver(IicBaseAddr);

    } else if (IIC_IN_RX_FULL_INTR(IntrStatus)) {
        /* Clear by hardware */
        if (Iic->OpState == AL_IIC_OP_STATE_MASTER_RX) {
            AlIic_Dev_MasterRecvDataHandler(Iic);
        } else if (Iic->OpState == AL_IIC_OP_STATE_SLAVE_RX) {
            AlIic_Dev_SlaveRecvDataHandler(Iic);
        }

    } else if (IIC_IN_THRE_EMPTY_INTR(IntrStatus)) {
        if (Iic->OpState == AL_IIC_OP_STATE_MASTER_TX) {
            AlIic_Dev_MasterSendDataHandler(Iic);
        } else if (Iic->OpState == AL_IIC_OP_STATE_MASTER_RX) {
            AlIic_Dev_MasterRecvDataIssueReadCmd(Iic);
        } else if (Iic->OpState == AL_IIC_OP_STATE_MASTER_DMA_RX) {
            AlIic_Dev_MasterDmaRecvDataIssueReadCmd(Iic);
        }

    } else if (IIC_IN_RD_REQ_INTR(IntrStatus)) {
        /* Just for slave transmitter */
        AlIic_Dev_SlaveSendDataHandler(Iic);
        AlIic_ll_ClrRdReq(IicBaseAddr);

    } else if (IIC_IN_TX_ABRT_INTR(IntrStatus)) {
        /* For Master or slave transmit, there is any data remaining in the Tx FIFO */
        AlIic_Dev_TxAbrtHandler(Iic);
        AlIic_ll_ClrTxAbrt(IicBaseAddr);

    } else if (IIC_IN_RX_DONE_INTR(IntrStatus)) {
        /* Just for slave transmitter, occurs on the last byte of the transmission */
        AlIic_Dev_SlaveSendDataDoneHandler(Iic);
        AlIic_ll_ClrRxDone(IicBaseAddr);

    } else if (IIC_IN_ACTIVITY_INTR(IntrStatus)) {
        /*
          This is a normal interrupt , Indicate that there was
          activity on the bus. do nothing, just clear it
         */
        AlIic_Dev_NormalIntrHandler(Iic, AL_IIC_EVENT_ACTIVITY);
        AlIic_ll_ClrActivity(IicBaseAddr);

    } else if (IIC_IN_STOP_DET_INTR(IntrStatus)) {
        /*
          Indicates whether a STOP condition has occurred on the I2C
          interface regardless of whether DW_apb_i2c is operating in
          slave or master mode. do nothing, jusr clear it
         */
        AlIic_Dev_NormalIntrHandler(Iic, AL_IIC_EVENT_STOP_DET);
        AlIic_ll_ClrStopDet(IicBaseAddr);

    } else if (IIC_IN_START_DET_INTR(IntrStatus)) {
        /*
          Indicates whether a START or RESTART condition has
          occurred on the I2C interface regardless of whether
          DW_apb_i2c is operating in slave or master mode. do nothing, jusr clear it
         */
        AlIic_Dev_NormalIntrHandler(Iic, AL_IIC_EVENT_START_DET);
        AlIic_ll_ClrStartDet(IicBaseAddr);

    } else if (IIC_IN_GEN_CALL_INTR(IntrStatus)) {
        AlIic_Dev_NormalIntrHandler(Iic, AL_IIC_EVENT_GEN_CALL);
        AlIic_ll_ClrGenCall(IicBaseAddr);

    } else if (IIC_INTR_SCL_STUCK_ATLOW_INTR(IntrStatus)) {
        AlIic_ll_ClrSclStuckDet(IicBaseAddr);
        AL_LOG(AL_LOG_LEVEL_ALERT,"SCL at low, Need reset entire IIC\r\n");
    }

}

/**
 * This function register a User IIC Callback.
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   CallBack pointer to the Callback function
 * @param   CallbackRef pointer to the Callback function params
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlIic_Dev_RegisterEventCallBack(AL_IIC_DevStruct *Iic, AL_IIC_EventCallBack Callback, void *CallbackRef)
{
    AL_ASSERT((Iic != AL_NULL) && (Callback != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    Iic->EventCallBack        = Callback;
    Iic->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

/**
 * This function master set command option, like stop、restart etc
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   CmdOption Command to be set
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlIic_Dev_MasterSetCmdOption(AL_IIC_DevStruct *Iic, AL_IIC_CmdOptionEnum CmdOption)
{
    AL_ASSERT(Iic != AL_NULL, AL_IIC_ERR_ILLEGAL_PARAM);

    switch (CmdOption)
    {
    case AL_IIC_CMD_OPTION_NO_STOP_RESTART:
    case AL_IIC_CMD_OPTION_STOP:
    case AL_IIC_CMD_OPTION_RESTART:
        Iic->CmdOption = CmdOption;
        break;

    default:
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    return AL_OK;
}

/**
 * This function master get current command option, like stop、restart etc
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @return
 *          - Command for function success
 *          - AL_IIC_CMD_OPTION_NONE for function failure
 * @note
*/
AL_IIC_CmdOptionEnum AlIic_Dev_MasterGetCmdOption(AL_IIC_DevStruct *Iic)
{
    AL_ASSERT(Iic != AL_NULL, AL_IIC_CMD_OPTION_NONE);

    return Iic->CmdOption;
}

/**
 * This function excute operations to set
 * @param   Iic Pointer to a AL_IIC_DevStruct structure
 * @param   Cmd is io ctl cmd to AL_IIC_IoCtlCmdEnum
 * @param   Data Pointer to cmd args
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlIic_Dev_IoCtl(AL_IIC_DevStruct *Iic, AL_IIC_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT(Iic != AL_NULL, AL_IIC_ERR_ILLEGAL_PARAM);

    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    /* Need disable IIC first */
    AlIic_ll_SetEnable(IicBaseAddr, AL_IIC_FUNC_DISABLE);

    switch (Cmd)
    {
    case AL_IIC_IOCTL_SET_ADDR_MODE: {
        AL_IIC_AddrModeEnum AddrMode = *(AL_IIC_AddrModeEnum *)Data;
        AlIic_ll_SetMasterAddrMode(IicBaseAddr, AddrMode);
        break;
    }
    case AL_IIC_IOCTL_GET_ADDR_MODE: {
        AL_IIC_AddrModeEnum *AddrMode = (AL_IIC_AddrModeEnum *)Data;
        *AddrMode = AlIic_ll_GetMasterAddrMode(IicBaseAddr);
        break;
    }
    case AL_IIC_IOCTL_SET_SPEED_MODE: {
        AL_IIC_SpeedModeEnum SpeedMode = *(AL_IIC_SpeedModeEnum *)Data;
        AlIic_ll_SetSpeedMode(IicBaseAddr, SpeedMode);
        AlIic_Dev_InitSclHighLowCout(Iic, SpeedMode);
        break;
    }
    case AL_IIC_IOCTL_GET_SPEED_MODE: {
        AL_IIC_SpeedModeEnum *SpeedMode = (AL_IIC_SpeedModeEnum *)Data;
        *SpeedMode = AlIic_ll_GetSpeedMode(IicBaseAddr);
        break;
    }
    default:
        return AL_IIC_ERR_IOCTL_CMD_NOT_SUPPORT;
        break;
    }

    /* Need Enable IIC here */
    AlIic_ll_SetEnable(IicBaseAddr, AL_IIC_FUNC_ENABLE);

    return Ret;
}