/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_axi_quad_spi_dev.h"
#include "al_axi_quad_spi_ll.h"

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_AXI_QSPI_INTR_MODF                 BIT(0)
#define AL_AXI_QSPI_INTR_SLV_MODF             BIT(1)    
#define AL_AXI_QSPI_INTR_DTR_EMPTY            BIT(2)
#define AL_AXI_QSPI_INTR_DTR_UNDER            BIT(3)
#define AL_AXI_QSPI_INTR_DRR_FULL             BIT(4)
#define AL_AXI_QSPI_INTR_DRR_OVER             BIT(5)
#define AL_AXI_QSPI_INTR_TX_HALF              BIT(6)
#define AL_AXI_QSPI_INTR_SLV_SEL              BIT(7)  
#define AL_AXI_QSPI_INTR_DRR_NOT_EMPTY        BIT(8)
#define AL_AXI_QSPI_INTR_CPOL_CPHA_ERR        BIT(9)
#define AL_AXI_QSPI_INTR_SLV_MODE_ERR         BIT(10)
#define AL_AXI_QSPI_INTR_MSB_ERR              BIT(11)
#define AL_AXI_QSPI_INTR_LOOPBACK_ERR         BIT(12)
#define AL_AXI_QSPI_INTR_CMD_ERR              BIT(13)

#define AL_AXI_QSPI_INTR_SEND_MASK            (AL_AXI_QSPI_INTR_DTR_EMPTY | AL_AXI_QSPI_INTR_TX_HALF)
#define AL_AXI_QSPI_INTR_RECV_MASK            (AL_AXI_QSPI_INTR_DRR_FULL | AL_AXI_QSPI_INTR_DRR_NOT_EMPTY)
#define AL_AXI_QSPI_INTR_ERR_MASK             (AL_AXI_QSPI_INTR_MODF | AL_AXI_QSPI_INTR_SLV_MODF \
                                                | AL_AXI_QSPI_INTR_DTR_UNDER | AL_AXI_QSPI_INTR_DRR_OVER \
                                                | AL_AXI_QSPI_INTR_CPOL_CPHA_ERR | AL_AXI_QSPI_INTR_SLV_MODE_ERR \
                                                | AL_AXI_QSPI_INTR_MSB_ERR | AL_AXI_QSPI_INTR_LOOPBACK_ERR \
                                                | AL_AXI_QSPI_INTR_CMD_ERR)
#define AL_AXI_QSPI_INTR_SLV_MASK             (AL_AXI_QSPI_INTR_SLV_SEL)


#define AL_AXI_QSPI_SEND_INTR(Status)         ((Status) & AL_AXI_QSPI_INTR_SEND_MASK)
#define AL_AXI_QSPI_RECV_INTR(Status)         ((Status) & AL_AXI_QSPI_INTR_RECV_MASK)
#define AL_AXI_QSPI_ERR_INTR(Status)          ((Status) & AL_AXI_QSPI_INTR_ERR_MASK)
#define AL_AXI_QSPI_SLV_INTR(Status)          ((Status) & AL_AXI_QSPI_INTR_SLV_MASK)

/************************** Function Prototypes ******************************/
static AL_VOID AlAxiQspi_Dev_RecvDataHandler(AlAxiQspi_DevStruct *Dev);
static AL_VOID AlAxiQspi_Dev_SendDataHandler(AlAxiQspi_DevStruct *Dev);
static AL_VOID AlAxiQspi_Dev_SlvHandler(AlAxiQspi_DevStruct *Dev);
static AL_VOID AlAxiQspi_Dev_ErrHandler(AlAxiQspi_DevStruct *Dev);

/************************** Variable Definitions *****************************/
extern AlAxiQspi_HwConfigStruct AlAxiQspi_HwCfg[AL_AXI_QSPI_NUM_INSTANCE];


/************************** Function Definitions ******************************/
/**
 * This function looks up the hardware configuration for a axi-qspi device using its device ID.
 *
 * @param DeviceId The device ID of the axi-qspi device.
 * @return Pointer to the hardware configuration structure if found, NULL otherwise.
 */
AlAxiQspi_HwConfigStruct *AlAxiQspi_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index = 0;
    AlAxiQspi_HwConfigStruct *HwConfig = AL_NULL;

    for (Index = 0; Index < AL_AXI_QSPI_NUM_INSTANCE; Index++) {
        if (AlAxiQspi_HwCfg[Index].DeviceId == DevId) {
            HwConfig = &AlAxiQspi_HwCfg[Index];
            break;
        }
    }

    return HwConfig;
}

/**
 * This function registers a callback function for axi-qspi device events.
 *
 * @param Dev Pointer to the axi-qspi device structure.
 * @param CallBack Function pointer to the callback function.
 * @param CallBackRef User-defined reference passed to the callback function.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlAxiQspi_Dev_RegisterEventCallBack(AlAxiQspi_DevStruct *Dev, AxiQspi_EventCallBack CallBack, AL_VOID *CallBackRef)
{
    if (Dev == AL_NULL || CallBack == AL_NULL) {
        return AL_AXI_QSPI_ERR_ILLEGAL_PARAM;
    }

    Dev->EventCallBack = CallBack;
    Dev->EventCallBackRef = CallBackRef;

    return AL_OK;
}

/**
 * This function interrupt handler for axi-qspi device. 
 * It checks the interrupt status and calls the appropriate handler.
 *
 * @param Instance Pointer to the axi-qspi device instance.
 * @return None.
 */
AL_VOID AlAxiQspi_Dev_IntrHandler(AL_VOID *Instance)
{
    AlAxiQspi_DevStruct *Dev = (AlAxiQspi_DevStruct *)Instance;
    AL_U64 BaseAddr = Dev->HwConfig.BaseAddress;

    Dev->IntrStatus = AlAxiQspi_ll_GetIntrStatus(BaseAddr) & 
        AlAxiQspi_ll_GetIntrEnReg(BaseAddr);

    if (AL_AXI_QSPI_RECV_INTR(Dev->IntrStatus)) {
        AlAxiQspi_Dev_RecvDataHandler(Dev);
    }

    if (AL_AXI_QSPI_SEND_INTR(Dev->IntrStatus)) {
        AlAxiQspi_Dev_SendDataHandler(Dev);
    }

    if (AL_AXI_QSPI_SLV_INTR(Dev->IntrStatus)) {
        AlAxiQspi_Dev_SlvHandler(Dev);
    }

    if (AL_AXI_QSPI_ERR_INTR(Dev->IntrStatus)) {
        AlAxiQspi_Dev_ErrHandler(Dev);
    }

    AlAxiQspi_ll_SetIntrStaReg(BaseAddr, Dev->IntrStatus);
}

/**
 * This function handles the received data from axi-qspi device.
 * 
 * @param Dev Pointer to the axi-qspi device structure.
 * @return None.
 */
static AL_VOID AlAxiQspi_Dev_RecvDataHandler(AlAxiQspi_DevStruct *Dev)
{
    AL_U32 RecvData = 0;
    AlAxiQspi_EventStruct QspiEvent = { 0 };

    if (Dev->TransInfo.RecvSize == 0 || Dev->TransInfo.RecvBufPtr == AL_NULL) {
        return;
    }

    if (Dev->HwConfig.SpiMode == AXI_QSPI_STANDARD) {
        while (AlAxiQspi_ll_GetStatus(Dev->HwConfig.BaseAddress, AXI_QSPI_SPI_STATUS_RX_EMPTY_SHIFT) == 0) {
            RecvData = AlAxiQspi_ll_ReadRxBuf(Dev->HwConfig.BaseAddress);
            if (Dev->TransInfo.TotalRecvCnt < Dev->TransInfo.TotalTransSize - Dev->TransInfo.RecvSize) {
                ;
            } else {
                Dev->TransInfo.RecvBufPtr[Dev->TransInfo.RecvCnt] = RecvData;
                Dev->TransInfo.RecvCnt++;
            }

            Dev->TransInfo.TotalRecvCnt++;
        }
    }

    if (Dev->TransInfo.TotalRecvCnt == Dev->TransInfo.TotalTransSize) {
        if (Dev->InitConfig.IsMaster) {
            AlAxiQspi_ll_SetSlaveSelect(Dev->HwConfig.BaseAddress, Dev->TransInfo.TargetSlave, AL_FUNC_DISABLE);
            AlAxiQspi_ll_SetMasterTransInhibit(Dev->HwConfig.BaseAddress, AL_TRUE);
        }

        AlAxiQspi_ll_DisableAllIntr(Dev->HwConfig.BaseAddress);
        AlAxiQspi_ll_SetSpiEnable(Dev->HwConfig.BaseAddress, AL_FUNC_DISABLE);

        Dev->State &= ~AL_AXI_QSPI_STATE_RX_BUSY;
        Dev->State &= ~AL_AXI_QSPI_STATE_TX_BUSY;
        if (Dev->EventCallBack != AL_NULL) {
            QspiEvent.Events |= AL_AXI_QSPI_EVENT_RX_DONE;
            QspiEvent.EventData = Dev->TransInfo.RecvCnt;
            Dev->EventCallBack(QspiEvent, Dev->EventCallBackRef);
        }
        return;
    }
}

/**
 * This function handles the send data from axi-qspi device.
 * It will also handle the received data.
 * 
 * @param Dev Pointer to the axi-qspi device structure.
 * @return None.
 */
static AL_VOID AlAxiQspi_Dev_SendDataHandler(AlAxiQspi_DevStruct *Dev)
{
    AL_U32 Index = 0;
    AL_U32 SendData = 0;
    AL_U32 RecvData = 0;
    AL_U64 BaseAddr = Dev->HwConfig.BaseAddress;
    AL_U32 ByteSteps = (Dev->HwConfig.TransBits >> 3);
    AL_U32 IsFifoExist = Dev->HwConfig.FifoDepth == AXI_QSPI_FIFO_DEPTH_0 ? 0 : 1;
    AL_U8 *BufPtr = Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt;
    AL_U32 FifoSpace = 0;
    AL_U32 FifoFillCnt = 0;
    AlAxiQspi_EventStruct QspiEvent = { 0 };

    if (Dev->TransInfo.RecvCnt < Dev->TransInfo.RecvSize) {
        while (AlAxiQspi_ll_GetStatus(Dev->HwConfig.BaseAddress, AXI_QSPI_SPI_STATUS_RX_EMPTY_SHIFT) == 0) {
            RecvData = AlAxiQspi_ll_ReadRxBuf(Dev->HwConfig.BaseAddress);
            if (Dev->TransInfo.TotalRecvCnt < Dev->TransInfo.TotalTransSize - Dev->TransInfo.RecvSize) {
                ;
            } else {
                if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_8) {
                    *(AL_U8 *)(Dev->TransInfo.RecvBufPtr + Dev->TransInfo.RecvCnt) = (AL_U8)RecvData;
                } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_16) {
                    *(AL_U16 *)(Dev->TransInfo.RecvBufPtr + Dev->TransInfo.RecvCnt) = (AL_U16)RecvData;
                } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_32) {
                    *(AL_U32 *)(Dev->TransInfo.RecvBufPtr + Dev->TransInfo.RecvCnt) = (AL_U32)RecvData;
                }

                Dev->TransInfo.RecvCnt += ByteSteps;
            }

            Dev->TransInfo.TotalRecvCnt += ByteSteps;
        }
    }

    if (Dev->TransInfo.TotalSendCnt == Dev->TransInfo.TotalTransSize && 
        (Dev->IntrStatus & AL_AXI_QSPI_INTR_DTR_EMPTY)) {

        if (Dev->TransInfo.RecvCnt == Dev->TransInfo.RecvSize) {
            Dev->State &= ~AL_AXI_QSPI_STATE_RX_BUSY;
            AlAxiQspi_ll_SetSlaveSelect(BaseAddr, Dev->TransInfo.TargetSlave, AL_FUNC_DISABLE);
            AlAxiQspi_ll_SetMasterTransInhibit(BaseAddr, AL_TRUE);
            AlAxiQspi_ll_DisableAllIntr(BaseAddr);
            AlAxiQspi_ll_SetSpiEnable(BaseAddr, AL_FUNC_DISABLE);
        }

        Dev->State &= ~AL_AXI_QSPI_STATE_TX_BUSY;
        if (Dev->EventCallBack != AL_NULL) {
            QspiEvent.Events |= AL_AXI_QSPI_EVENT_TX_DONE;
            QspiEvent.EventData = Dev->TransInfo.SendCnt;
            Dev->EventCallBack(QspiEvent, Dev->EventCallBackRef);
        }
        return;
    }

    if (IsFifoExist) {
        FifoSpace = Dev->HwConfig.FifoDepth - AlAxiQspi_ll_GetTxFifoLevel(BaseAddr) - 1;
    } else {
        FifoSpace = 1;
    }

    while (FifoFillCnt < FifoSpace) {
        if (Dev->TransInfo.TotalSendCnt == Dev->TransInfo.TotalTransSize) {
            break;
        }

        if (Dev->TransInfo.TotalSendCnt < Dev->TransInfo.SendSize) {
            if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_8) {
                SendData = (AL_U32)(*(AL_U8 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
            } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_16) {
                SendData = (AL_U32)(*(AL_U16 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
            } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_32) {
                SendData = (AL_U32)(*(AL_U32 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
            }

            Dev->TransInfo.SendCnt += ByteSteps;
        } else {
            SendData = 0x0U;
        }

        AlAxiQspi_ll_WriteTxBuf(BaseAddr, SendData);
        Dev->TransInfo.TotalSendCnt += ByteSteps;
        FifoFillCnt++;
    }
}

/**
 * This function handles the slave select event from axi-qspi device.
 * 
 * @param Dev Pointer to the axi-qspi device structure.
 * @return None.
 */
static AL_VOID AlAxiQspi_Dev_SlvHandler(AlAxiQspi_DevStruct *Dev)
{
    AlAxiQspi_EventStruct QspiEvent = { 0 };

    if (Dev->EventCallBack == AL_NULL) {
        return;
    }

    if (Dev->IntrStatus & AL_AXI_QSPI_INTR_SLV_SEL) {
        QspiEvent.Events |= AL_AXI_QSPI_EVENT_SLV_SEL;
        QspiEvent.EventData = 0;
        Dev->EventCallBack(QspiEvent, Dev->EventCallBackRef);
    }
}

/**
 * This function handles the error event from axi-qspi device.
 * 
 * @param Dev Pointer to the axi-qspi device structure.
 * @return None.
 */
static AL_VOID AlAxiQspi_Dev_ErrHandler(AlAxiQspi_DevStruct *Dev)
{
    AL_U64 BaseAddr = Dev->HwConfig.BaseAddress;
    AlAxiQspi_EventStruct QspiEvent = { 0 };

    /* No data to be received, ignore rx fifo overflow */
    if ((Dev->IntrStatus & AL_AXI_QSPI_INTR_DRR_OVER) && (Dev->TransInfo.RecvSize == 0)) {
        return;
    }

    /* Because of the bug of axi-qspi rx fifo overflow, ignore rx fifo overflow error */
    if ((Dev->IntrStatus & AL_AXI_QSPI_INTR_ERR_MASK) == AL_AXI_QSPI_INTR_DRR_OVER) {
        return;
    }

    AlAxiQspi_ll_SetSlaveSelect(BaseAddr, Dev->TransInfo.TargetSlave, AL_FUNC_DISABLE);
    AlAxiQspi_ll_SetMasterTransInhibit(BaseAddr, AL_TRUE);
    AlAxiQspi_ll_DisableAllIntr(BaseAddr);
    AlAxiQspi_ll_SetSpiEnable(BaseAddr, AL_FUNC_DISABLE);

    if (Dev->EventCallBack == AL_NULL) {
        return;
    }
    
    if (Dev->IntrStatus & AL_AXI_QSPI_INTR_MODF) {
        QspiEvent.Events |= AL_AXI_QSPI_EVENT_MODF;
    }
    if (Dev->IntrStatus & AL_AXI_QSPI_INTR_SLV_MODF) {
        QspiEvent.Events |= AL_AXI_QSPI_EVENT_SLV_MODF;
    }
    if (Dev->IntrStatus & AL_AXI_QSPI_INTR_DTR_UNDER) {
        QspiEvent.Events |= AL_AXI_QSPI_EVENT_DTR_UNDER;
    }
    if (Dev->IntrStatus & AL_AXI_QSPI_INTR_DRR_OVER) {
        QspiEvent.Events |= AL_AXI_QSPI_EVENT_DRR_OVER;
    }
    if (Dev->IntrStatus & AL_AXI_QSPI_INTR_CPOL_CPHA_ERR) {
        QspiEvent.Events |= AL_AXI_QSPI_EVENT_CPOL_CPHA_ERR;
    }
    if (Dev->IntrStatus & AL_AXI_QSPI_INTR_SLV_MODE_ERR) {
        QspiEvent.Events |= AL_AXI_QSPI_EVENT_SLV_MODE_ERR;
    }
    if (Dev->IntrStatus & AL_AXI_QSPI_INTR_MSB_ERR) {
        QspiEvent.Events |= AL_AXI_QSPI_EVENT_MSB_ERR;
    }
    if (Dev->IntrStatus & AL_AXI_QSPI_INTR_LOOPBACK_ERR) {
        QspiEvent.Events |= AL_AXI_QSPI_EVENT_LOOPBACK_ERR;
    }
    if (Dev->IntrStatus & AL_AXI_QSPI_INTR_CMD_ERR) {
        QspiEvent.Events |= AL_AXI_QSPI_EVENT_CMD_ERR;
    }

    Dev->State |= AL_AXI_QSPI_STATE_ERROR;
    QspiEvent.EventData = Dev->TransInfo.TotalTransSize;
    Dev->EventCallBack(QspiEvent, Dev->EventCallBackRef);
}

/**
 * This function initializes and configs the axi-qspi device.
 * 
 * @param Dev Pointer to the axi-qspi device structure.
 * @param HwConfig Pointer to the hardware configuration structure.
 * @param InitConfig Pointer to the initialization configuration structure. It can be passed by user.
 * @return AL_OK on success, error code otherwise.
 */
AL_U32 AlAxiQspi_Dev_Init(AlAxiQspi_DevStruct *Dev, AlAxiQspi_HwConfigStruct *HwConfig, AlAxiQspi_InitConfigStruct *InitConfig)
{
    AL_U32 Status = AL_OK;
    AL_U64 BaseAddr;

    if (Dev == AL_NULL || HwConfig == AL_NULL) {
        return AL_AXI_QSPI_ERR_ILLEGAL_PARAM;
    }

    Dev->HwConfig = *HwConfig;

    Dev->TransInfo.TargetSlave = 0;
    BaseAddr = Dev->HwConfig.BaseAddress;

    AlAxiQspi_ll_SwRst(BaseAddr);

    if (InitConfig == AL_NULL) {
        Dev->InitConfig.IsLoopback = 0;
        Dev->InitConfig.IsMaster = 1;
        Dev->InitConfig.CPHA = 0;
        Dev->InitConfig.CPOL = 0;
        Dev->InitConfig.ManualSlvSel = 1;
        Dev->InitConfig.LsbFirst = 0;
    } else {
        Dev->InitConfig = *InitConfig;
    }

    AlAxiQspi_ll_SetLoopback(BaseAddr, Dev->InitConfig.IsLoopback);
    AlAxiQspi_ll_SetMasterSlave(BaseAddr, Dev->InitConfig.IsMaster);
    AlAxiQspi_ll_SetCPHA(BaseAddr, Dev->InitConfig.CPHA);
    AlAxiQspi_ll_SetCPOL(BaseAddr, Dev->InitConfig.CPOL);
    AlAxiQspi_ll_SetManualSlaveSelect(BaseAddr, Dev->InitConfig.ManualSlvSel);
    AlAxiQspi_ll_SetLsbFirst(BaseAddr, Dev->InitConfig.LsbFirst);

    Dev->State = AL_AXI_QSPI_STATE_READY;

    return Status;
}

/**
 * This function sends data in interrupt mode.
 * 
 * @param Dev Pointer to the axi-qspi device structure.
 * @param SendBuf Pointer to the send buffer.
 * @param SendSize The size of the send buffer.
 * @return AL_OK on success, error code otherwise.
 */
AL_U32 AlAxiQspi_Dev_SendData(AlAxiQspi_DevStruct *Dev, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_U32 Status = AL_OK;
    AL_U32 SendData = 0;
    AL_U32 ByteSteps = 0;
    AL_U32 IsFifoExist = 0;
    AL_U64 BaseAddr = Dev->HwConfig.BaseAddress;

    if (Dev == AL_NULL || SendBuf == AL_NULL || SendSize == 0) {
        return AL_AXI_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Dev->State & AL_AXI_QSPI_STATE_READY) == 0) {
        return AL_AXI_QSPI_ERR_NOT_READY;
    }

    if (Dev->State & AL_AXI_QSPI_STATE_TX_BUSY) {
        return AL_AXI_QSPI_ERR_BUSY;
    }

    Dev->State |= AL_AXI_QSPI_STATE_TX_BUSY;

    Dev->TransInfo.SendBufPtr = SendBuf;
    Dev->TransInfo.SendSize = SendSize;
    Dev->TransInfo.SendCnt = 0;
    Dev->TransInfo.RecvBufPtr = AL_NULL;
    Dev->TransInfo.RecvSize = 0;
    Dev->TransInfo.RecvCnt = 0;
    Dev->TransInfo.TotalTransSize = SendSize;
    Dev->TransInfo.TotalSendCnt = 0;
    Dev->TransInfo.TotalRecvCnt = 0;

    ByteSteps = (Dev->HwConfig.TransBits >> 3);
    IsFifoExist = Dev->HwConfig.FifoDepth == AXI_QSPI_FIFO_DEPTH_0 ? 0 : 1;

    AlAxiQspi_ll_SetSpiEnable(BaseAddr, AL_FUNC_DISABLE);
    ALAxiQspi_ll_SetGlobalIntr(BaseAddr, AL_FUNC_ENABLE);
    AlAxiQspi_ll_SetMasterTransInhibit(BaseAddr, AL_TRUE);
    AlAxiQspi_ll_ResetTxFifo(BaseAddr);
    AlAxiQspi_ll_ResetRxFifo(BaseAddr);
    AlAxiQspi_ll_SetSlaveSelect(BaseAddr, Dev->TransInfo.TargetSlave, AL_FUNC_ENABLE);
    AlAxiQspi_ll_DisableAllIntr(BaseAddr);
    AlAxiQspi_ll_ClearAllIntrStatus(BaseAddr);
    AlAxiQspi_ll_SetSpiEnable(BaseAddr, AL_FUNC_ENABLE);

    if (Dev->HwConfig.SpiMode == AXI_QSPI_STANDARD) {
        while (1) {
            if (Dev->TransInfo.TotalSendCnt == Dev->TransInfo.TotalTransSize) {
                break;
            }
            
            if (Dev->TransInfo.TotalSendCnt < Dev->TransInfo.SendSize) {
                if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_8) {
                    SendData = (AL_U32)(*(AL_U8 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
                } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_16) {
                    SendData = (AL_U32)(*(AL_U16 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
                } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_32) {
                    SendData = (AL_U32)(*(AL_U32 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
                }

                Dev->TransInfo.SendCnt += ByteSteps;
            } else {
                SendData = 0x0U;
            }

            Dev->TransInfo.TotalSendCnt += ByteSteps;
            AlAxiQspi_ll_WriteTxBuf(BaseAddr, SendData);

            if (IsFifoExist) {
                if (AlAxiQspi_ll_GetStatus(BaseAddr, AXI_QSPI_SPI_STATUS_TX_FULL_SHIFT) != 0) {
                    break;
                }
            } else {
                break;
            }
        }
    } else {

    }

    AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_DTR_EMPTY_SHIFT, AL_FUNC_ENABLE);
    AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_TX_FIFO_HALF_SHIFT, AL_FUNC_ENABLE);
    AlAxiQspi_ll_SetMasterTransInhibit(Dev->HwConfig.BaseAddress, AL_FALSE);

    return Status;
}

/**
 * This function transfers data in polled mode. 
 * User can send data only or send data first and then receive data.
 * 
 * @param Dev Pointer to the axi-qspi device structure.
 * @param SendBuf Pointer to the send buffer.
 * @param SendSize The size of the send buffer.
 * @param RecvBuf Pointer to the receive buffer.
 * @param RecvSize The size of the receive buffer.
 * @return AL_OK on success, error code otherwise.
 */
AL_U32 AlAxiQspi_Dev_TransferDataPolled(AlAxiQspi_DevStruct *Dev, AL_U8 *SendBuf, AL_U32 SendSize, 
                                  AL_U8 *RecvBuf, AL_U32 RecvSize)
{
    AL_U32 Status = AL_OK;
    AL_U32 Index = 0;
    AL_U32 SendData = 0;
    AL_U32 RecvData = 0;
    AL_U32 ByteSteps = 0;
    AL_U32 IsFifoExist = 0;
    AL_U32 FifoSpace = 0;
    AL_U32 FifoFillCnt = 0;
    AL_U32 EnableTrans = 1;
    AL_U64 BaseAddr = Dev->HwConfig.BaseAddress;
    AlAxiQspi_EventStruct QspiEvent = { 0 };

    if (Dev == AL_NULL || (SendBuf == AL_NULL && SendSize != 0) || (RecvBuf == AL_NULL && RecvSize != 0)) {
        return AL_AXI_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Dev->State & AL_AXI_QSPI_STATE_READY) == 0) {
        return AL_AXI_QSPI_ERR_NOT_READY;
    }

    if ((Dev->State & AL_AXI_QSPI_STATE_TX_BUSY) || (Dev->State & AL_AXI_QSPI_STATE_RX_BUSY)) {
        return AL_AXI_QSPI_ERR_BUSY;
    }

    Dev->State |= AL_AXI_QSPI_STATE_TX_BUSY;
    Dev->State |= AL_AXI_QSPI_STATE_RX_BUSY;

    Dev->TransInfo.SendBufPtr = SendBuf;
    Dev->TransInfo.SendSize = SendSize;
    Dev->TransInfo.SendCnt = 0;
    Dev->TransInfo.RecvBufPtr = RecvBuf;
    Dev->TransInfo.RecvSize = RecvSize;
    Dev->TransInfo.RecvCnt = 0;
    Dev->TransInfo.TotalTransSize = SendSize + RecvSize;
    Dev->TransInfo.TotalSendCnt = 0;
    Dev->TransInfo.TotalRecvCnt = 0;

    ByteSteps = (Dev->HwConfig.TransBits >> 3);
    IsFifoExist = Dev->HwConfig.FifoDepth == AXI_QSPI_FIFO_DEPTH_0 ? 0 : 1;

    AlAxiQspi_ll_SetSpiEnable(BaseAddr, AL_FUNC_DISABLE);
    ALAxiQspi_ll_SetGlobalIntr(BaseAddr, AL_FUNC_DISABLE);
    AlAxiQspi_ll_SetMasterTransInhibit(BaseAddr, AL_TRUE);
    AlAxiQspi_ll_ResetTxFifo(BaseAddr);
    AlAxiQspi_ll_ResetRxFifo(BaseAddr);
    AlAxiQspi_ll_DisableAllIntr(BaseAddr);
    AlAxiQspi_ll_ClearAllIntrStatus(BaseAddr);
    AlAxiQspi_ll_SetSpiEnable(BaseAddr, AL_FUNC_ENABLE);

    while (1) {

        while (AlAxiQspi_ll_GetStatus(BaseAddr, AXI_QSPI_SPI_STATUS_RX_EMPTY_SHIFT) == 0) {
            RecvData = AlAxiQspi_ll_ReadRxBuf(BaseAddr);
            if (Dev->TransInfo.TotalRecvCnt < Dev->TransInfo.TotalTransSize - RecvSize) {
                ;
            } else {
                if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_8) {
                    *(AL_U8 *)(Dev->TransInfo.RecvBufPtr + Dev->TransInfo.RecvCnt) = (AL_U8)RecvData;
                } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_16) {
                    *(AL_U16 *)(Dev->TransInfo.RecvBufPtr + Dev->TransInfo.RecvCnt) = (AL_U16)RecvData;
                } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_32) {
                    *(AL_U32 *)(Dev->TransInfo.RecvBufPtr + Dev->TransInfo.RecvCnt) = (AL_U32)RecvData;
                }
                
                Dev->TransInfo.RecvCnt += ByteSteps;
            }

            Dev->TransInfo.TotalRecvCnt += ByteSteps;
        }

        while (AlAxiQspi_ll_GetStatus(BaseAddr, AXI_QSPI_SPI_STATUS_TX_FULL_SHIFT) != 0) {
            ;
        }

        if ((Dev->TransInfo.TotalRecvCnt == Dev->TransInfo.TotalTransSize) && 
            (Dev->TransInfo.TotalSendCnt == Dev->TransInfo.TotalTransSize)) {
            break;
        }

        if (IsFifoExist) {
            FifoSpace = Dev->HwConfig.FifoDepth - AlAxiQspi_ll_GetTxFifoLevel(BaseAddr) - 1;
        } else {
            FifoSpace = 1;
        }

        FifoFillCnt = 0;
        while (FifoFillCnt < FifoSpace) {

            /* If the clock of cpu is slow enough, the rx fifo may be overflow */
            if (AlAxiQspi_ll_GetStatus(BaseAddr, AXI_QSPI_SPI_STATUS_RX_FULL_SHIFT) != 0) {
                break;
            }

            if (Dev->TransInfo.TotalSendCnt < Dev->TransInfo.TotalTransSize) {
                if (Dev->TransInfo.TotalSendCnt < SendSize) {
                    if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_8) {
                        SendData = (AL_U32)(*(AL_U8 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
                    } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_16) {
                        SendData = (AL_U32)(*(AL_U16 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
                    } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_32) {
                        SendData = (AL_U32)(*(AL_U32 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
                    }

                    Dev->TransInfo.SendCnt += ByteSteps;
                } else {
                    SendData = 0x0U;
                }

                Dev->TransInfo.TotalSendCnt += ByteSteps;
                AlAxiQspi_ll_WriteTxBuf(BaseAddr, SendData);

                FifoFillCnt++;
            } else {
                break;
            }
        }

        if (EnableTrans) {
            AlAxiQspi_ll_SetSlaveSelect(BaseAddr, Dev->TransInfo.TargetSlave, AL_FUNC_ENABLE);
            AlAxiQspi_ll_SetMasterTransInhibit(Dev->HwConfig.BaseAddress, AL_FALSE);
            EnableTrans = 0;
        }
    }

    AlAxiQspi_ll_SetSlaveSelect(BaseAddr, Dev->TransInfo.TargetSlave, AL_FUNC_DISABLE);
    AlAxiQspi_ll_SetMasterTransInhibit(Dev->HwConfig.BaseAddress, AL_TRUE);

    Dev->State &= ~AL_AXI_QSPI_STATE_TX_BUSY;
    Dev->State &= ~AL_AXI_QSPI_STATE_RX_BUSY;

    if (Dev->EventCallBack != AL_NULL) {
        QspiEvent.Events = AL_AXI_QSPI_EVENT_TX_DONE;
        QspiEvent.EventData = Dev->TransInfo.TotalSendCnt;
        Dev->EventCallBack(QspiEvent, Dev->EventCallBackRef);
    }

    return Status;
}


/**
 * This function transfers data in interrupt mode.
 * User can send data only or send data first and then receive data.
 * 
 * @param Dev Pointer to the axi-qspi device structure.
 * @param SendBuf Pointer to the send buffer.
 * @param SendSize The size of the send buffer.
 * @param RecvBuf Pointer to the receive buffer.
 * @param RecvSize The size of the receive buffer.
 * @return AL_OK on success, error code otherwise.
 */
AL_U32 AlAxiQspi_Dev_TransferData(AlAxiQspi_DevStruct *Dev, AL_U8 *SendBuf, AL_U32 SendSize, 
                                  AL_U8 *RecvBuf, AL_U32 RecvSize)
{
    AL_U32 Status = AL_OK;
    AL_U32 SendData = 0;
    AL_U32 ByteSteps = 0;
    AL_U32 IsFifoExist = 0;
    AL_U64 BaseAddr = Dev->HwConfig.BaseAddress;

    if (Dev == AL_NULL || (SendBuf == AL_NULL && SendSize != 0) || (RecvBuf == AL_NULL && RecvSize != 0)) {
        return AL_AXI_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Dev->State & AL_AXI_QSPI_STATE_READY) == 0) {
        return AL_AXI_QSPI_ERR_NOT_READY;
    }

    if ((Dev->State & AL_AXI_QSPI_STATE_TX_BUSY) || (Dev->State & AL_AXI_QSPI_STATE_RX_BUSY)) {
        return AL_AXI_QSPI_ERR_BUSY;
    }

    Dev->State |= AL_AXI_QSPI_STATE_TX_BUSY;
    Dev->State |= AL_AXI_QSPI_STATE_RX_BUSY;

    Dev->TransInfo.SendBufPtr = SendBuf;
    Dev->TransInfo.SendSize = SendSize;
    Dev->TransInfo.SendCnt = 0;
    Dev->TransInfo.RecvBufPtr = RecvBuf;
    Dev->TransInfo.RecvSize = RecvSize;
    Dev->TransInfo.RecvCnt = 0;
    Dev->TransInfo.TotalTransSize = SendSize + RecvSize;
    Dev->TransInfo.TotalSendCnt = 0;
    Dev->TransInfo.TotalRecvCnt = 0;

    ByteSteps = (Dev->HwConfig.TransBits >> 3);
    IsFifoExist = Dev->HwConfig.FifoDepth == AXI_QSPI_FIFO_DEPTH_0 ? 0 : 1;

    AlAxiQspi_ll_SetSpiEnable(BaseAddr, AL_FUNC_DISABLE);
    ALAxiQspi_ll_SetGlobalIntr(BaseAddr, AL_FUNC_ENABLE);
    AlAxiQspi_ll_SetMasterTransInhibit(BaseAddr, AL_TRUE);
    AlAxiQspi_ll_ResetTxFifo(BaseAddr);
    AlAxiQspi_ll_ResetRxFifo(BaseAddr);
    AlAxiQspi_ll_SetSlaveSelect(BaseAddr, Dev->TransInfo.TargetSlave, AL_FUNC_ENABLE);
    AlAxiQspi_ll_DisableAllIntr(BaseAddr);
    AlAxiQspi_ll_ClearAllIntrStatus(BaseAddr);
    AlAxiQspi_ll_SetSpiEnable(BaseAddr, AL_FUNC_ENABLE);

    while (1) {
        if (Dev->TransInfo.TotalSendCnt == Dev->TransInfo.TotalTransSize) {
            break;
        }
        
        if (Dev->TransInfo.TotalSendCnt < Dev->TransInfo.SendSize) {
            if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_8) {
                SendData = (AL_U32)(*(AL_U8 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
            } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_16) {
                SendData = (AL_U32)(*(AL_U16 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
            } else if (Dev->HwConfig.TransBits == AXI_QSPI_TRANS_BITS_32) {
                SendData = (AL_U32)(*(AL_U32 *)(Dev->TransInfo.SendBufPtr + Dev->TransInfo.SendCnt));
            }

            Dev->TransInfo.SendCnt += ByteSteps;
        } else {
            SendData = 0x0U;
        }

        Dev->TransInfo.TotalSendCnt += ByteSteps;
        AlAxiQspi_ll_WriteTxBuf(BaseAddr, SendData);

        if (IsFifoExist) {
            if (AlAxiQspi_ll_GetStatus(BaseAddr, AXI_QSPI_SPI_STATUS_TX_FULL_SHIFT) != 0) {
                break;
            }
        } else {
            break;
        }
    }

    AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_DTR_EMPTY_SHIFT, AL_FUNC_ENABLE);
    AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_TX_FIFO_HALF_SHIFT, AL_FUNC_ENABLE);
    AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_DRR_OVER_SHIFT, AL_FUNC_ENABLE);
    if (Dev->InitConfig.IsMaster) {
        AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_MODF_SHIFT, AL_FUNC_ENABLE);
    } else {
        AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_SLAVE_MODF_SHIFT, AL_FUNC_ENABLE);
        AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_DTR_UNDER_SHIFT, AL_FUNC_ENABLE);
        AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_DRR_FULL_SHIFT, AL_FUNC_ENABLE);
        AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_SLV_SEL_SHIFT, AL_FUNC_ENABLE);
        AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_DRR_NOT_EMPTY_SHIFT, AL_FUNC_ENABLE);
    }
    if (Dev->HwConfig.SpiMode != AXI_QSPI_STANDARD) {
        AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_CPOL_CPHA_ERR_SHIFT, AL_FUNC_ENABLE);
        AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_SLAVE_MODE_ERR_SHIFT, AL_FUNC_ENABLE);
        AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_MSB_ERR_SHIFT, AL_FUNC_ENABLE);
        AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_LOOPBACK_ERR_SHIFT, AL_FUNC_ENABLE);
        AlAxiQspi_ll_SetIntrEnable(BaseAddr, AXI_QSPI_SPI_INTR_EN_CMD_ERR_SHIFT, AL_FUNC_ENABLE);
    }

    AlAxiQspi_ll_SetMasterTransInhibit(Dev->HwConfig.BaseAddress, AL_FALSE);

    return Status;
}

