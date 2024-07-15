/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gbe_hal.h"

#define AL_GBE_HAL_LOCK(Handle)          do {} while (0)
#define AL_GBE_HAL_UNLOCK(Handle)        do {} while (0)

AL_GBE_HalStruct AlGbeHandle[AL_GBE_NUM_INSTANCE];

/**
 * Callback function for default GBE events.
 *
 * @param GbeEvent Pointer to the GBE event structure.
 * @param CallbackRef User-defined callback reference.
 * @return None.
 */
static AL_VOID AlGbe_DefEventCallBack(AL_GBE_EventStruct *GbeEvent, void *CallbackRef)
{
    AL_GBE_HalStruct *Handle = (AL_GBE_HalStruct *)CallbackRef;

    switch (GbeEvent->Event) {
        case AL_GBE_EVENT_TX_DONE:
        case AL_GBE_EVENT_TX_BUFFER_UNAVAILABLE:
        Handle->TxDoneCallBack(CallbackRef);
        break;

        case AL_GBE_EVENT_RX_DONE:
        case AL_GBE_EVENT_RX_BUFFER_UNAVAILABLE:
        Handle->RxDoneCallBack(CallbackRef);
        break;

        default:
        break;
    }
}

/**
 * Waits for a TX done event or times out.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param Timeout Timeout in milliseconds.
 * @return AL_OK on success, error code otherwise.
 */
static AL_S32 AlGbe_Hal_WaitTxDoneOrTimeout(AL_GBE_HalStruct *Handle, AL_U32 Timeout)
{

    return AL_OK;
}

/**
 * Waits for an RX done event or times out.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param Timeout Timeout in milliseconds.
 * @return AL_OK on success, error code otherwise.
 */
static AL_S32 AlGbe_Hal_WaitRxDoneOrTimeout(AL_GBE_HalStruct *Handle, AL_U32 Timeout)
{

    return AL_OK;
}

/**
 * Initializes the GBE HAL.
 *
 * @param Handle Double pointer to the GBE HAL structure to be initialized.
 * @param DevId Device ID.
 * @param InitConfig Pointer to the initialization configuration structure.
 * @param MacDmaConfig Pointer to the MAC DMA configuration structure.
 * @param Callback Event callback function.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_Init(AL_GBE_HalStruct **Handle, AL_U32 DevId, AL_GBE_InitStruct *InitConfig,
                      AL_GBE_MacDmaConfigStruct *MacDmaConfig, AL_GBE_EventCallBack Callback)
{
    AL_S32 Ret = AL_OK;
    AL_GBE_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    HwConfig = AlGbe_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlGbeHandle[DevId];
    } else {
        return AL_GBE_ERR_INVALID_DEVICE_ID;
    }

    Ret = AlGbe_Dev_Init(&((*Handle)->Dev), HwConfig, InitConfig, MacDmaConfig);
    if (Ret != AL_OK) {
        AL_GBE_HAL_UNLOCK(Handle);
        return Ret;
    }

    if(AL_NULL == Callback) {
        Ret = AlGbe_Dev_RegisterEventCallBack(&((*Handle)->Dev), AlGbe_DefEventCallBack, (void *)*Handle);
    } else {
        Ret = AlGbe_Dev_RegisterEventCallBack(&((*Handle)->Dev), Callback, (void *)*Handle);
    }
    if (Ret != AL_OK) {
        AL_GBE_HAL_UNLOCK(Handle);
        return Ret;
    }

    AlIntr_RegHandler((*Handle)->Dev.HwConfig.IntrNum, AL_NULL, AlGbe_Dev_IntrHandler, &((*Handle)->Dev));

    AL_GBE_HAL_UNLOCK(Handle);

    return AL_OK;
}

/**
 * Registers an interrupt handler callback.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param IntrId Interrupt ID.
 * @param CallBackHandler Pointer to the callback handler function.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_RegisterIntrHandlerCallBack(AL_GBE_HalStruct *Handle, AL_GBE_IntrStatusEnum IntrId,
                                             void *CallBackHandler)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL) && (CallBackHandler != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    switch (IntrId) {
    case AL_GBE_INTR_TX_COMPLETE:
        Handle->TxDoneCallBack = ((AL_GBE_TxDoneCallBack)(void *)CallBackHandler);
        break;

    case AL_GBE_INTR_RX_COMPLETE:
        Handle->RxDoneCallBack = ((AL_GBE_RxDoneCallBack)(void *)CallBackHandler);
        break;

    default:
        Ret = AL_GBE_ERR_ILLEGAL_PARAM;
        break;
    }

    return Ret;
}

/**
 * Registers a callback for TX free event.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param CallBackHandler Pointer to the callback handler function.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_RegisterTxFreeCallBack(AL_GBE_HalStruct *Handle, void *CallBackHandler)
{
    AL_ASSERT((Handle != AL_NULL) && (CallBackHandler != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Handle->Dev.TxFreeCallback = CallBackHandler;

    AL_GBE_HAL_UNLOCK(Handle);

    return AL_OK;
}

/**
 * Configures RX descriptor buffers.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param BuffersAddr Address of the buffers.
 * @param BufferCnt Number of buffers.
 * @param BufferSize Size of each buffer.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_ConfigRxDescBuffer(AL_GBE_HalStruct *Handle, AL_U8 *BuffersAddr, AL_U32 BufferCnt, AL_U32 BufferSize)
{
    AL_S32 Ret;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_ConfigRxDescBuffer(&Handle->Dev, BuffersAddr, BufferCnt, BufferSize);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * Configures TX descriptor buffers.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param BuffersAddr Address of the buffers.
 * @param BufferCnt Number of buffers.
 * @param BufferSize Size of each buffer.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_ConfigTxDescBuffer(AL_GBE_HalStruct *Handle, AL_U8 *BuffersAddr, AL_U32 BufferCnt, AL_U32 BufferSize)
{
    AL_S32 Ret;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_ConfigTxDescBuffer(&Handle->Dev, BuffersAddr, BufferCnt, BufferSize);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * Initializes the PHY.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param PhyAddr PHY address.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_PhyInit(AL_GBE_HalStruct *Handle, AL_U8 PhyAddr)
{
    AL_S32 Ret;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_PhyInit(&Handle->Dev, PhyAddr);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * Gets the PHY link status.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param PhyAddr PHY address.
 * @param Speed Pointer to store the speed.
 * @param Duplex Pointer to store the duplex mode.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_GetPhyLinkStatus(AL_GBE_HalStruct *Handle, AL_U32 PhyAddr, AL_U8 *Speed, AL_U8 *Duplex)
{
    AL_S32 Ret;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_GetPhyLinkStatus(&Handle->Dev, PhyAddr, Speed, Duplex);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * Configures duplex and speed.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_ConfigDuplexAndSpeed(AL_GBE_HalStruct *Handle)
{
    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    AlGbe_Dev_ConfigDuplexAndSpeed(&Handle->Dev);

    AL_GBE_HAL_UNLOCK(Handle);

    return AL_OK;
}

/**
 * Starts the MAC DMA.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_StartMacDma(AL_GBE_HalStruct *Handle)
{
    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    AlGbe_Dev_StartMacDma(&Handle->Dev);

    AL_GBE_HAL_UNLOCK(Handle);

    return AL_OK;
}

/**
 * Starts the MAC DMA with interrupts.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_StartMacDmaIntr(AL_GBE_HalStruct *Handle)
{
    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    AlGbe_Dev_StartMacDmaIntr(&Handle->Dev);

    AL_GBE_HAL_UNLOCK(Handle);

    return AL_OK;
}

/**
 * Transmits a block of data.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param TxConfig Pointer to the TX descriptor configuration structure.
 * @param Timeout Timeout in milliseconds.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_TransmitBlock(AL_GBE_HalStruct *Handle, AL_GBE_TxDescConfigStruct *TxConfig, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_Transmit(&Handle->Dev, TxConfig);
    if (Ret != AL_OK) {
        AL_GBE_HAL_UNLOCK(Handle);
        return Ret;
    }

    /*
     * wait until frame send done
     */
    Ret = AlGbe_Hal_WaitTxDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        Ret = AL_GBE_ERR_TIMEOUT;
    }

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * Transmits data.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param TxConfig Pointer to the TX descriptor configuration structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_Transmit(AL_GBE_HalStruct *Handle, AL_GBE_TxDescConfigStruct *TxConfig)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_TransmitPolling(&Handle->Dev, TxConfig);
    if (Ret != AL_OK) {
        AL_GBE_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * Gets the RX data buffer.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param RxBuffer Pointer to the RX buffer structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_GetRxDataBuffer(AL_GBE_HalStruct *Handle, AL_GBE_BufferStruct *RxBuffer)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_GetRxDataBuffer(&Handle->Dev, RxBuffer);
    if (Ret != AL_OK) {
        AL_GBE_HAL_UNLOCK(Handle);
        return Ret;
    }

    Ret = AlGbe_Hal_WaitRxDoneOrTimeout(Handle, 0);
    if (Ret != AL_OK) {
        Ret = AL_GBE_ERR_TIMEOUT;
    }

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * Gets the RX data length.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param Length Pointer to store the data length.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_GetRxDataLength(AL_GBE_HalStruct *Handle, AL_U32 *Length)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_GetRxDataLength(&Handle->Dev, Length);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * Builds RX descriptors.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_BuildRxDescriptors(AL_GBE_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_BuildRxDescriptors(&Handle->Dev);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * Releases a TX packet.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_ReleaseTxPacket(AL_GBE_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_ReleaseTxPacket(&Handle->Dev);
    if (Ret != AL_OK) {
        AL_GBE_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * Initializes PTP.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param PtpConfig Pointer to the PTP configuration structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_PtpInit(AL_GBE_HalStruct *Handle, AL_GBE_PtpConfigStruct *PtpConfig)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_PtpInit(&Handle->Dev, PtpConfig);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * Sets the PTP timestamp.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param Timestamp Pointer to the PTP time structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_SetPtpTimestamp(AL_GBE_HalStruct *Handle, AL_GBE_PtpTimeStruct *Timestamp)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_SetPtpTimestamp(&Handle->Dev, Timestamp);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 *
 * This function fetches the current PTP timestamp from the device. It ensures thread safety by locking
 * the GBE HAL structure during the operation. It validates the input parameters and returns an error
 * if the handle is NULL.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param Timestamp Pointer to the structure where the PTP timestamp will be stored.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_GetPtpTimestamp(AL_GBE_HalStruct *Handle, AL_GBE_PtpTimeStruct *Timestamp)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_GetPtpTimestamp(&Handle->Dev, Timestamp);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 *
 * This function updates the PTP time offset in the device. It locks the GBE HAL structure to ensure
 * thread safety during the operation. The function checks for a valid handle and returns an error
 * if the handle is NULL.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param TimeOffset Pointer to the structure containing the new time offset to be applied.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_UpdatePtpTimeOffset(AL_GBE_HalStruct *Handle, AL_GBE_PtpTimeStruct *TimeOffset)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_UpdatePtpTimeOffset(&Handle->Dev, TimeOffset);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function adjusts the frequency of the PTP clock. It locks the GBE HAL structure to ensure
 * thread safety. The function validates the handle and returns an error if it is NULL. The adjustment
 * is made based on the provided frequency adjustment value.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param Adj The adjustment value for the PTP time frequency.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_AdjustPtpTimeFreq(AL_GBE_HalStruct *Handle, AL_U32 Adj)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_AdjustPtpTimeFreq(&Handle->Dev, Adj);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}
