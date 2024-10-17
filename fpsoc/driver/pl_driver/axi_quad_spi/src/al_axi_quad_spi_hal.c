/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include <stdlib.h>
#include "al_axi_quad_spi_hal.h"
#include "al_axi_quad_spi_dev.h"
#include "al_intr.h"
#include "al_osal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
/* Static axi-qspi controller instance */
static AlAxiQspi_HalStruct AlAxiQspiHandle[AL_AXI_QSPI_NUM_INSTANCE];

/************************** Function Definitions ******************************/

/**
 * Default event callback function for axi-qspi.
 * 
 * @param QspiEvent axi-qspi event structure
 * @param CallBackRef callback reference, which is a pointer to AlAxiQspi_HalStruct
 * @return None
*/
static AL_VOID AlQspi_DefEventCallBack(AlAxiQspi_EventStruct QspiEvent, void *CallBackRef)
{
    AlAxiQspi_HalStruct *Handle = (AlAxiQspi_HalStruct *)CallBackRef;

    switch (QspiEvent.Events)
    {
    case AL_AXI_QSPI_EVENT_TX_DONE:
    case AL_AXI_QSPI_EVENT_DTR_EMPTY:
    case AL_AXI_QSPI_EVENT_TX_HALF:
    case AL_AXI_QSPI_EVENT_RX_DONE:
    case AL_AXI_QSPI_EVENT_DRR_FULL:
    case AL_AXI_QSPI_EVENT_DRR_NOT_EMPTY:
        AlOsal_Mb_Send(&Handle->TxEventQueue, &QspiEvent);
        break;
    case AL_AXI_QSPI_EVENT_MODF:
    case AL_AXI_QSPI_EVENT_SLV_MODF:
    case AL_AXI_QSPI_EVENT_SLV_SEL:
    case AL_AXI_QSPI_EVENT_CPOL_CPHA_ERR:
    case AL_AXI_QSPI_EVENT_SLV_MODE_ERR:
    case AL_AXI_QSPI_EVENT_MSB_ERR:
    case AL_AXI_QSPI_EVENT_LOOPBACK_ERR:
    case AL_AXI_QSPI_EVENT_CMD_ERR:
    case AL_AXI_QSPI_EVENT_DRR_OVER:
    case AL_AXI_QSPI_EVENT_DTR_UNDER:
        AlOsal_Mb_Send(&Handle->TxEventQueue, &QspiEvent);
    default:
        break;
    }
}

/**
 * This function initializes the axi-qspi hardware and registers event callbacks.
 *
 * @param Handle Double pointer to the CAN HAL structure. This will be initialized by the function.
 * @param DevId Device ID for the axi-qspi hardware.
 * @param InitConfig Pointer to the initialization configuration structure.
 * @param CallBack Event callback function pointer.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_U32 AlAxiQspi_Hal_Init(AlAxiQspi_HalStruct **Handle, AL_U32 DevId, AlAxiQspi_InitConfigStruct *InitConfig, AxiQspi_EventCallBack CallBack)
{
    AL_U32 Status;
    AlAxiQspi_HwConfigStruct *HwConfig = AL_NULL;

    HwConfig = AlAxiQspi_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlAxiQspiHandle[DevId];
    }
    else {
        return AL_AXI_QSPI_ERR_ILLEGAL_PARAM;
    }

    Status = AlAxiQspi_Dev_Init(&(*Handle)->Dev, HwConfig, InitConfig);
    if (Status != AL_OK)
    {
        return Status;
    }

    if (CallBack == AL_NULL) {
        AlAxiQspi_Dev_RegisterEventCallBack(&(*Handle)->Dev, AlQspi_DefEventCallBack, (AL_VOID *)*Handle);
    } else {
        AlAxiQspi_Dev_RegisterEventCallBack(&(*Handle)->Dev, CallBack, (AL_VOID *)*Handle);
    }

    (AL_VOID)AlIntr_RegHandler(HwConfig->IntrId, AL_NULL, AlAxiQspi_Dev_IntrHandler, &(*Handle)->Dev);

    Status = AlOsal_Lock_Init(&(*Handle)->AxiQspiLock, "AxiQspiLock");
    if (Status != AL_OK) {
        return Status;
    }

    Status = AlOsal_Mb_Init(&(*Handle)->TxEventQueue, "TxEventQueue");
    if (Status != AL_OK) {
        return Status;
    }

    Status = AlOsal_Mb_Init(&(*Handle)->RxEventQueue, "RxEventQueue");
    if (Status != AL_OK) {
        return Status;
    }

    return AL_OK;
}

/**
 * This function waits for the send done event or timeout.
 * 
 * @param Handle Pointer to the axi-qspi HAL structure.
 * @param Event Pointer to the event structure.
 * @param Timeout Timeout value in milliseconds.
 * @return Returns AL_OK on success, or an error code on failure.
 */
static AL_U32 AlAxiQspi_Hal_WaitSendDoneOrTimeout(AlAxiQspi_HalStruct *Handle, AlAxiQspi_EventStruct *Event, AL_U32 Timeout)
{
    AL_U32 Status = AL_OK;

    Status = AlOsal_Mb_Receive(&Handle->TxEventQueue, Event, Timeout);
    if (Status != AL_OK) {
        return Status;
    }

    while (Handle->Dev.State & AL_AXI_QSPI_STATE_TX_BUSY) {

        if (Handle->Dev.State & AL_AXI_QSPI_STATE_ERROR) {
            Handle->Dev.State &= ~AL_AXI_QSPI_STATE_ERROR;
            Handle->Dev.State &= ~AL_AXI_QSPI_STATE_TX_BUSY;
            return AL_AXI_QSPI_EVENTS_TO_ERRS(Event->Events);
        }
    }

    return AL_OK;
}

/**
 * This function waits for the send and receive done events or timeout.
 * 
 * @param Handle Pointer to the axi-qspi HAL structure.
 * @param Event Pointer to the event structure.
 * @param Timeout Timeout value in milliseconds.
 * @return Returns AL_OK on success, or an error code on failure.
 */
static AL_U32 AlAxiQspi_Hal_WaitTxRxDoneOrTimeout(AlAxiQspi_HalStruct *Handle, AlAxiQspi_EventStruct *Event, AL_U32 Timeout)
{
    AL_U32 Status = AL_OK;

    Status = AlOsal_Mb_Receive(&Handle->TxEventQueue, Event, Timeout);
    if (Status != AL_OK) {
        return Status;
    }

#if 0
    Status = AlOsal_Mb_Receive(&Handle->RxEventQueue, Event, Timeout);
    if (Status != AL_OK) {
        return Status;
    }
#endif

    while ((Handle->Dev.State & AL_AXI_QSPI_STATE_TX_BUSY) || 
           (Handle->Dev.State & AL_AXI_QSPI_STATE_RX_BUSY)) {

        if (Handle->Dev.State & AL_AXI_QSPI_STATE_ERROR) {
            Handle->Dev.State &= ~AL_AXI_QSPI_STATE_ERROR;
            Handle->Dev.State &= ~AL_AXI_QSPI_STATE_TX_BUSY;
            Handle->Dev.State &= ~AL_AXI_QSPI_STATE_RX_BUSY;
            return AL_AXI_QSPI_EVENTS_TO_ERRS(Event->Events);
        }
    }

    return AL_OK;
}

/**
 * This function sends a block of data in interrupt mode.
 * 
 * @param Handle Pointer to the axi-qspi HAL structure.
 * @param Data Pointer to the data buffer.
 * @param Size Size of the data buffer.
 * @param Timeout Timeout value in milliseconds.
 * @return Returns AL_OK on success, or an error code on failure.
*/
AL_U32 AlAxiQspi_Hal_SendDataBlock(AlAxiQspi_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_U32 Status = AL_OK;
    AlAxiQspi_EventStruct QspiEvent = { 0 };

    if (Handle == AL_NULL || Data == AL_NULL || Size == 0) {
        return AL_AXI_QSPI_ERR_ILLEGAL_PARAM;
    }

    Status = AlOsal_Lock_Take(&Handle->AxiQspiLock, Timeout);
    if (Status != AL_OK) {
        return Status;
    }

    Status = AlAxiQspi_Dev_SendData(&Handle->Dev, Data, Size);
    if (Status != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->AxiQspiLock);
        return Status;
    }

    Status = AlAxiQspi_Hal_WaitSendDoneOrTimeout(Handle, &QspiEvent, Timeout);
    if (Status != AL_OK) {
        Status = AL_AXI_QSPI_ERR_TIMEOUT;
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->AxiQspiLock);

    return Status;
}

/**
 * This function transfer data in polled mode. User can send only, or receive only or 
 * send first and then receive data.
 * 
 * @param Handle Pointer to the axi-qspi HAL structure.
 * @param SendBuf Pointer to the send buffer, it can be AL_NULL.
 * @param SendSize Size of the send buffer.
 * @param RecvBuf Pointer to the receive buffer, it can be AL_NULL.
 * @param RecvSize Size of the receive buffer.
 * @param Timeout Timeout value in milliseconds.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_U32 AlAxiQspi_Hal_TransferDataPolledBlock(AlAxiQspi_HalStruct *Handle, AL_U8 *SendBuf, AL_U32 SendSize, 
                                             AL_U8 *RecvBuf, AL_U32 RecvSize, AL_U32 Timeout)
{
    AL_U32 Status = AL_OK;
    AlAxiQspi_EventStruct QspiEvent = { 0 };

    if (Handle == AL_NULL || (SendBuf == AL_NULL && SendSize != 0) || (RecvBuf == AL_NULL && RecvSize != 0)) {
        return AL_AXI_QSPI_ERR_ILLEGAL_PARAM;
    }

    Status = AlOsal_Lock_Take(&Handle->AxiQspiLock, Timeout);
    if (Status != AL_OK) {
        return Status;
    }

    Status = AlAxiQspi_Dev_TransferDataPolled(&Handle->Dev, SendBuf, SendSize, RecvBuf, RecvSize);
    if (Status != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->AxiQspiLock);
        return Status;
    }

    Status = AlAxiQspi_Hal_WaitTxRxDoneOrTimeout(Handle, &QspiEvent, Timeout);
    if (Status != AL_OK) {
        Status = AL_AXI_QSPI_ERR_TIMEOUT;
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->AxiQspiLock);

    return Status;
}


/**
 * This function transfer data in interrupt mode. User can send only, or receive only or 
 * send first and then receive data.
 * 
 * @param Handle Pointer to the axi-qspi HAL structure.
 * @param SendBuf Pointer to the send buffer, it can be AL_NULL.
 * @param SendSize Size of the send buffer.
 * @param RecvBuf Pointer to the receive buffer, it can be AL_NULL.
 * @param RecvSize Size of the receive buffer.
 * @param Timeout Timeout value in milliseconds.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_U32 AlAxiQspi_Hal_TransferDataBlock(AlAxiQspi_HalStruct *Handle, AL_U8 *SendBuf, AL_U32 SendSize, 
                                             AL_U8 *RecvBuf, AL_U32 RecvSize, AL_U32 Timeout)
{
    AL_U32 Status = AL_OK;
    AlAxiQspi_EventStruct QspiEvent = { 0 };

    if (Handle == AL_NULL || (SendBuf == AL_NULL && SendSize != 0) || (RecvBuf == AL_NULL && RecvSize != 0)) {
        return AL_AXI_QSPI_ERR_ILLEGAL_PARAM;
    }

    Status = AlOsal_Lock_Take(&Handle->AxiQspiLock, Timeout);
    if (Status != AL_OK) {
        return Status;
    }

    Status = AlAxiQspi_Dev_TransferData(&Handle->Dev, SendBuf, SendSize, RecvBuf, RecvSize);
    if (Status != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->AxiQspiLock);
        return Status;
    }

    Status = AlAxiQspi_Hal_WaitTxRxDoneOrTimeout(Handle, &QspiEvent, Timeout);
    if (Status != AL_OK) {
        Status = AL_AXI_QSPI_ERR_TIMEOUT;
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->AxiQspiLock);

    return Status;
}

