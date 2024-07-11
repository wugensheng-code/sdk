/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_iic_dev.h"

#define IS_SAME_INITCONFIGS(Dest, Src)                                          \
(                                                                               \
    (Dest).Mode == (Src).Mode && (Dest).AddrMode == (Src).AddrMode &&           \
    (Dest).SlaveAddr == (Src).SlaveAddr                                         \
)

static AL_IIC_InitStruct IicDefInitConfigs =
{
    .Mode           = AL_IIC_MODE_MASTER,
    .AddrMode       = AL_IIC_ADDR_7BIT,
};

extern AL_IIC_HwConfigStruct AlIic_HwConfig[AL_IIC_NUM_INSTANCE];

/**
 *
 * Looks up the hardware configuration for a given I2C device ID.
 * This function searches through the available I2C hardware configurations
 * and returns a pointer to the configuration if found.
 *
 * @param DevId The device ID of the I2C device to look up.
 *
 * @return A pointer to the hardware configuration structure if found; NULL otherwise.
 *
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
 *
 * Checks if the I2C device's transmit buffer is busy.
 * This function examines the device state to determine if data is currently
 * being transmitted.
 *
 * @param Iic Pointer to the I2C device structure.
 * @return AL_TRUE if the transmit buffer is busy; AL_FALSE otherwise.
 */
AL_BOOL AlIic_Dev_IsTxBusy(AL_IIC_DevStruct *Iic)
{
    return (AL_BOOL)(Iic->State & AL_IIC_STATE_TX_BUSY);
}

/**
 * Checks if the I2C device's receive buffer is busy.
 * This function examines the device state to determine if data is currently
 * being received.
 *
 * @param Iic Pointer to the I2C device structure.
 * @return AL_TRUE if the receive buffer is busy; AL_FALSE otherwise.
 */
AL_BOOL AlIic_Dev_IsRxBusy(AL_IIC_DevStruct *Iic)
{
    return (AL_BOOL)(Iic->State & AL_IIC_STATE_RX_BUSY);
}

/**
 * Sets the I2C device's transmit buffer to busy.
 * This function updates the device state to indicate that data is being transmitted.
 * If an event callback is registered, it triggers the callback with the
 * AL_IIC_EVENT_READY_TO_TX event.
 *
 * @param Iic Pointer to the I2C device structure.
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
 * Sets the I2C device's receive buffer to busy.
 * This function updates the device state to indicate that data is being received.
 * If an event callback is registered, it triggers the callback with the
 * AL_IIC_EVENT_READY_TO_RX event.
 *
 * @param Iic Pointer to the I2C device structure.
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
 * Clears the I2C device's transmit buffer busy status.
 * This function updates the device state to indicate that the transmit buffer is no longer busy.
 *
 * @param Iic Pointer to the I2C device structure.
 */
AL_VOID AlIic_Dev_ClrTxBusy(AL_IIC_DevStruct *Iic)
{
    Iic->State &= (~AL_IIC_STATE_TX_BUSY);
}

/**
 * Clears the I2C device's receive buffer busy status.
 * This function updates the device state to indicate that the receive buffer is no longer busy.
 *
 * @param Iic Pointer to the I2C device structure.
 */
AL_VOID AlIic_Dev_ClrRxBusy(AL_IIC_DevStruct *Iic)
{
    Iic->State &= (~AL_IIC_STATE_RX_BUSY);
}

/**
 * Initializes the SCL high and low count for a specified speed mode.
 * This function calculates and sets the SCL high and low period counts based on the
 * specified speed mode and the I2C clock frequency.
 *
 * @param Iic Pointer to the I2C device structure.
 * @param SpeedMode The speed mode (standard, fast, or high speed) to initialize the SCL counts for.
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
 * Checks the initialization configuration parameters for validity.
 * This function verifies that the provided initialization parameters are within
 * acceptable ranges and configurations.
 *
 * @param InitConfig Pointer to the initialization configuration structure.
 * @return AL_OK if the parameters are valid; an error code otherwise.
 */
static AL_S32 AlIic_Dev_CheckConfigParam(AL_IIC_InitStruct *InitConfig)
{
    AL_ASSERT (((InitConfig->Mode == AL_IIC_MODE_SLAVE) ||
               (InitConfig->Mode == AL_IIC_MODE_MASTER)), AL_IIC_ERR_ILLEGAL_PARAM);

    AL_ASSERT(((InitConfig->AddrMode == AL_IIC_ADDR_7BIT) || (InitConfig->AddrMode == AL_IIC_ADDR_10BIT)),
              AL_IIC_ERR_ILLEGAL_PARAM);

    return AL_OK;
}

/**
 * Initializes the I2C device with specified hardware and initialization configurations.
 * This function sets up the I2C device based on the provided hardware and initialization
 * configurations, including setting the speed mode, address mode, and FIFO thresholds.
 *
 * @param Iic Pointer to the I2C device structure to initialize.
 * @param HwConfig Pointer to the hardware configuration structure.
 * @param InitConfig Pointer to the initialization configuration structure.
 * @return AL_OK if the device was successfully initialized; an error code otherwise.
 */
AL_S32 AlIic_Dev_Init(AL_IIC_DevStruct *Iic, AL_IIC_HwConfigStruct *HwConfig, AL_IIC_InitStruct *InitConfig)
{
    AL_S32 Ret;
    AL_REG IicBaseAddr;
    AL_U32 RxFifoDepth;
    AL_U32 TxFifoDepth;
    AL_IIC_SpeedModeEnum SpeedMode;

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

        if (HwConfig->RateHz <= AL_IIC_RATE_100K) {
            SpeedMode = AL_IIC_STANDARD_MODE;
        } else if (HwConfig->RateHz <= AL_IIC_RATE_400K) {
            SpeedMode = AL_IIC_FAST_MODE;
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Unsupported rate\r\n");
            return AL_IIC_ERR_ILLEGAL_PARAM;
        }

        AlIic_ll_SetSpeedMode(IicBaseAddr, SpeedMode);
        AlIic_ll_SetMasterEnable(IicBaseAddr, AL_IIC_FUNC_ENABLE);
        AlIic_Dev_InitSclHighLowCout(Iic, SpeedMode);

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
 * Configures the slave address for I2C master mode operations.
 * This function sets the target slave address for I2C transactions initiated by the master.
 * It temporarily disables the I2C device to update the target address.
 *
 * @param Iic Pointer to the I2C device structure.
 * @param SlaveAddr The slave address to set for master mode operations.
 */
static inline AL_VOID AlIic_Dev_MasterSetTar(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr)
{
    /* I2C_DYNAMIC_TAR_UPDATE not enabled, need disable IIC first */
    AlIic_ll_SetEnable((AL_REG)(Iic->HwConfig.BaseAddress), AL_IIC_FUNC_DISABLE);

    AlIic_ll_SetTar((AL_REG)(Iic->HwConfig.BaseAddress), SlaveAddr);

    AlIic_ll_SetEnable((AL_REG)(Iic->HwConfig.BaseAddress), AL_IIC_FUNC_ENABLE);
}

/**
 * Enables or disables interrupts for master mode send operations.
 * This function updates the interrupt mask to enable or disable interrupts related
 * to transmit operations, such as TX_EMPTY, TX_ABRT, and TX_OVER.
 *
 * @param Iic Pointer to the I2C device structure.
 * @param State Specifies whether to enable or disable the send interrupts.
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
 *
 * This function initiates a transmission of data from the master to the specified slave device.
 * It sets up the necessary parameters and enables the master send interrupt to handle the transmission.
 * The function checks for the validity of the input parameters, the readiness of the device, and
 * whether the device is currently busy in transmission. It then sets the device's operation state,
 * target slave address, and data buffer for transmission.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param SlaveAddr The address of the slave device.
 * @param SendBuf Pointer to the data buffer to be sent.
 * @param SendSize The size of the data to be sent.
 * @return AL_S32 Returns AL_OK if the operation was successful, otherwise an error code.
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

/**
 *
 * This function initiates a DMA-based transmission of data from the master to the specified slave device.
 * It disables the IIC device before setting up DMA parameters and re-enables it after configuration.
 * The function ensures the input parameters are valid, the device is ready, and not currently busy.
 * It sets the device's operation state and target slave address for the DMA transmission.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param SlaveAddr The address of the slave device.
 * @return AL_S32 Returns AL_OK if the operation was successful, otherwise an error code.
 */
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
 *
 * This function disables the master send interrupt and clears the transmission busy flag,
 * effectively stopping any ongoing data transmission initiated by the master.
 *
 * @param Iic Pointer to the IIC device structure.
 */
AL_VOID AlIic_Dev_StopMasterSend(AL_IIC_DevStruct *Iic)
{
    AlIic_Dev_EnableMasterSendIntr(Iic, AL_IIC_FUNC_DISABLE);

    AlIic_Dev_ClrTxBusy(Iic);
}

/**
 *
 * This function sends data to the specified slave device using a polling method rather than interrupts.
 * It checks for the validity of the input parameters, the readiness of the device, and whether the device
 * is currently busy. The function then enters a loop, sending data byte by byte, and waits for the
 * transmission to complete if a STOP condition is requested.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param SlaveAddr The address of the slave device.
 * @param SendBuf Pointer to the data buffer to be sent.
 * @param SendSize The size of the data to be sent.
 * @return Returns AL_OK if the operation was successful, otherwise an error code.
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
 *
 * This function configures the interrupt mask for the master receive operation. It enables or disables
 * interrupts related to transmission empty, reception full, transmission abort, reception underflow,
 * and reception overflow based on the specified state.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param State The desired state of the receive interrupts (enable or disable).
 * @return None.
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
 *
 * This function initiates the reception of data from the specified slave device to the master.
 * It sets up the necessary parameters and enables the master receive interrupt to handle the reception.
 * The function checks for the validity of the input parameters, the readiness of the device, and
 * whether the device is currently busy in reception. It then sets the device's operation state,
 * target slave address, and data buffer for reception.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param SlaveAddr The address of the slave device.
 * @param RecvBuf Pointer to the buffer where received data will be stored.
 * @param RecvSize The size of the data to be received.
 * @return Returns AL_OK if the operation was successful, otherwise an error code.
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

/**
 *
 * This function initiates a DMA-based reception of data from the specified slave device to the master.
 * It disables the IIC device before setting up DMA parameters for reception and re-enables it after configuration.
 * The function ensures the input parameters are valid, the device is ready, and not currently busy in reception.
 * It sets the device's operation state and target slave address for the DMA reception.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param SlaveAddr The address of the slave device.
 * @param RecvSize The size of the data to be received.
 * @return Returns AL_OK if the operation was successful, otherwise an error code.
 */
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
 *
 * This function disables the master receive interrupt and clears the reception busy flag,
 * effectively stopping any ongoing data reception initiated by the master.
 *
 * @param Iic Pointer to the IIC device structure.
 * @return None.
 */
AL_VOID AlIic_Dev_StopMasterRecv(AL_IIC_DevStruct *Iic)
{
    AlIic_Dev_EnableMasterRecvIntr(Iic, AL_IIC_FUNC_DISABLE);

    AlIic_Dev_ClrRxBusy(Iic);
}

/**
 *
 * This function is responsible for receiving data from an I2C slave device in master mode.
 * It ensures that the device is in the correct state and configuration before initiating
 * the receive operation. The function polls the TX FIFO to issue read commands and the RX FIFO
 * to retrieve the received data.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param SlaveAddr The address of the slave device from which data is to be received.
 * @param RecvBuf Pointer to the buffer where received data will be stored.
 * @param RecvSize The number of bytes to receive.
 *
 * @return Returns AL_OK if the operation is successful, otherwise returns an error code.
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
 *
 * This function configures the interrupt mask to enable or disable interrupts related to slave transmission.
 * It is used internally to control the flow of slave transmission operations
 * by enabling interrupts for conditions like transmit abort, read request, transmit over, and receive done.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param State Specifies whether to enable or disable the slave send interrupts.
 * @return None.
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
 *
 * Initiates the transmission of data from an I2C slave device.
 * It sets up the device's internal state and buffer pointers for the transmission
 * and enables the necessary interrupts to handle the data sending process.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param SendBuf Pointer to the buffer containing the data to be sent.
 * @param SendSize The number of bytes to send.
 *
 * @return Returns AL_OK if the operation is successful, otherwise returns an error code.
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
 *
 * This function disables the interrupts related to slave sending operations and
 * clears the busy state of the device, effectively stopping any ongoing data transmission
 * in slave mode.
 *
 * @param Iic Pointer to the IIC device structure.
 * @return None.
 */
AL_VOID AlIic_Dev_StopSlaveSend(AL_IIC_DevStruct *Iic)
{
    AlIic_Dev_EnableSlaveSendIntr(Iic, AL_IIC_FUNC_DISABLE);

    AlIic_Dev_ClrTxBusy(Iic);
}

/**
 *
 * Configures the interrupt mask to enable or disable interrupts for conditions related to
 * slave reception. This includes interrupts for RX FIFO full, underflow, and overflow.
 * It is used to control the flow of data reception in slave mode.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param State Specifies whether to enable or disable the slave receive interrupts.
 * @return None.
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
 *
 * Prepares the I2C slave device to receive data by setting up the internal state,
 * buffer pointers, and configuring the RX FIFO threshold. It enables the necessary interrupts
 * to handle the data reception process.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param RecvBuf Pointer to the buffer where received data will be stored.
 * @param RecvSize The number of bytes to receive.
 *
 * @return Returns AL_OK if the operation is successful, otherwise returns an error code.
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
 *
 * Disables the interrupts related to slave reception operations and clears the busy state
 * of the device, effectively stopping any ongoing data reception in slave mode.
 *
 * @param Iic Pointer to the IIC device structure.
 * @return None.
 */
AL_VOID AlIic_Dev_StopSlaveRecv(AL_IIC_DevStruct *Iic)
{
    AlIic_Dev_EnableSlaveRecvIntr(Iic, AL_IIC_FUNC_DISABLE);

    AlIic_Dev_ClrRxBusy(Iic);
}

/**
 *
 * This function is called to handle the transmission of data in master mode.
 * It polls the TX FIFO to send data until all data in the buffer is transmitted.
 * It also handles the last byte transmission differently based on the command option set
 * for stop or restart condition.
 *
 * @param Iic Pointer to the IIC device structure.
 * @return None.
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
 * Handles the reception of data in master mode. This function reads data from the RX FIFO
 * until it's empty or the requested amount of data has been received. It also adjusts the RX FIFO
 * threshold based on the remaining data to be received. If all requested data has been received,
 * it triggers the RX_DONE event and disables relevant interrupts.
 *
 * @param Iic Pointer to the IIC device structure.
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
 * Issues read commands in master mode. This function fills the TX FIFO with read commands
 * until it's full or the requested amount of data has been issued for reading. It handles
 * the last read command differently based on the command option (STOP, RESTART, or WRITE).
 * After issuing all read commands, it immediately starts receiving data by calling
 * AlIic_Dev_MasterRecvDataHandler.
 *
 * @param Iic Pointer to the IIC device structure.
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

/**
 * Issues read commands in master mode for DMA-based transfers. This function is similar to
 * AlIic_Dev_MasterRecvDataIssueReadCmd but is intended for use with DMA by issuing read commands
 * without immediately starting the data reception process.
 *
 * @param Iic Pointer to the IIC device structure.
 */
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
 * Handles the reception of data in slave mode. This function reads data from the RX FIFO
 * until it's empty or the requested amount of data has been received. It also adjusts the RX FIFO
 * threshold based on the remaining data to be received. If all requested data has been received,
 * it triggers the RX_DONE event and stops the slave receiver.
 *
 * @param Iic Pointer to the IIC device structure.
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
 * Handles the transmission of data in slave mode. This function writes a single byte of data to the TX FIFO if it's not full and the requested amount of data has not been fully sent. If all data has been sent, it triggers the RD_REQ event and disables relevant interrupts.
 *
 * @param Iic Pointer to the IIC device structure.
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
 * Handles the completion of data transmission in slave mode. This function is called when the master
 * does not acknowledge a transmitted byte, indicating that the transmission is done.
 * It triggers the TX_DONE event and disables the RX_DONE interrupt.
 *
 * @param Iic Pointer to the IIC device structure.
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
 * Handles the TX abort event. This function is called when a transmission is aborted.
 * It retrieves the abort source, triggers the TX_ABRT event, and attempts to recover
 * if the abort was caused by SDA being stuck at low.
 *
 * @param Iic Pointer to the IIC device structure.
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
 * Handles error interrupts. This function stops data transmission or reception in case of an error
 * (e.g., RX_UNDER, RX_OVER, TX_OVER) and triggers the corresponding event.
 *
 * @param Iic Pointer to the IIC device structure.
 * @param EventId The ID of the event that occurred.
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
 * Handles normal interrupts for the I2C device. This function is called when there is an activity on the bus that does not require any specific action other than notifying the user through a callback.
 *
 * @param Iic Pointer to the I2C device structure.
 * @param EventId The event ID that triggered the interrupt.
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
 * The main interrupt handler for the I2C device. This function checks the interrupt status and calls the appropriate handler based on the type of interrupt that occurred.
 *
 * @param instance A void pointer to the I2C device structure.
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
 * Registers a callback function to be called when an event occurs on the I2C bus.
 *
 * @param Iic Pointer to the I2C device structure.
 * @param Callback The function to be called when an event occurs.
 * @param CallbackRef A user-defined reference that will be passed to the callback function.
 * @return AL_OK if the callback was successfully registered; otherwise, an error code.
 */
AL_S32 AlIic_Dev_RegisterEventCallBack(AL_IIC_DevStruct *Iic, AL_IIC_EventCallBack Callback, void *CallbackRef)
{
    AL_ASSERT((Iic != AL_NULL) && (Callback != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    Iic->EventCallBack        = Callback;
    Iic->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

/**
 * Sets the command option for the I2C master. This function configures how the I2C master behaves with respect to STOP and RESTART conditions.
 *
 * @param Iic Pointer to the I2C device structure.
 * @param CmdOption The command option to set.
 * @return AL_OK if the command option was successfully set; otherwise, an error code.
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
 * Gets the current command option setting for the I2C master.
 *
 * @param Iic Pointer to the I2C device structure.
 * @return The current command option setting.
 */
AL_IIC_CmdOptionEnum AlIic_Dev_MasterGetCmdOption(AL_IIC_DevStruct *Iic)
{
    AL_ASSERT(Iic != AL_NULL, AL_IIC_CMD_OPTION_NONE);

    return Iic->CmdOption;
}

/**
 * Performs a control operation on the I2C device. This function allows setting and getting various parameters of the I2C device, such as address mode and transfer rate.
 *
 * @param Iic Pointer to the I2C device structure.
 * @param Cmd The control command to execute.
 * @param Data Pointer to the data needed for the command or where to store the command output.
 * @return AL_OK if the operation was successful; otherwise, an error code.
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
    case AL_IIC_IOCTL_SET_RATE: {
        AL_IIC_SpeedModeEnum SpeedMode;
        AL_U32 Rate = *(AL_U32 *)Data;

        if (Rate <= AL_IIC_RATE_100K) {
            SpeedMode = AL_IIC_STANDARD_MODE;
        } else if (Rate <= AL_IIC_RATE_400K) {
            SpeedMode = AL_IIC_FAST_MODE;
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Unsupported rate\r\n");
            Ret = AL_IIC_ERR_ILLEGAL_PARAM;
            break;
        }

        AlIic_ll_SetSpeedMode(IicBaseAddr, SpeedMode);
        AlIic_Dev_InitSclHighLowCout(Iic, SpeedMode);

        break;
    }
    case AL_IIC_IOCTL_GET_RATE: {
        AL_IIC_SpeedModeEnum SpeedMode = AlIic_ll_GetSpeedMode(IicBaseAddr);
        AL_U32 *Rate = (AL_U32 *)Data;

        if (SpeedMode == AL_IIC_STANDARD_MODE) {
            *Rate = AL_IIC_RATE_100K;
        } else if (SpeedMode == AL_IIC_FAST_MODE) {
            *Rate = AL_IIC_RATE_400K;
        } else {
            *Rate = AL_IIC_RATE_3400K;
        }

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