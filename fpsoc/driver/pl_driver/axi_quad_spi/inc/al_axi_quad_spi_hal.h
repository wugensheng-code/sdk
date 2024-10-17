/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_axi_qspi_hal.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2024/08/27
 * @brief   axi-qspi device driver
 */

#ifndef __AL_AXI_QSPI_HAL__
#define __AL_AXI_QSPI_HAL__

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_axi_quad_spi_dev.h"
#include "al_osal.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/
typedef struct {
    AlAxiQspi_DevStruct Dev;

    AL_Lock AxiQspiLock;
    AL_MailBox TxEventQueue;
    AL_MailBox RxEventQueue;
} AlAxiQspi_HalStruct;

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
AL_U32 AlAxiQspi_Hal_Init(AlAxiQspi_HalStruct **Handle, AL_U32 DevId, AlAxiQspi_InitConfigStruct *InitConfig, AxiQspi_EventCallBack CallBack);
AL_U32 AlAxiQspi_Hal_SendDataBlock(AlAxiQspi_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);
AL_U32 AlAxiQspi_Hal_TransferDataPolledBlock(AlAxiQspi_HalStruct *Handle, AL_U8 *SendBuf, AL_U32 SendSize, 
                                             AL_U8 *RecvBuf, AL_U32 RecvSize, AL_U32 Timeout);
AL_U32 AlAxiQspi_Hal_TransferDataBlock(AlAxiQspi_HalStruct *Handle, AL_U8 *SendBuf, AL_U32 SendSize, 
                                       AL_U8 *RecvBuf, AL_U32 RecvSize, AL_U32 Timeout);

#ifdef __cplusplus
}
#endif

#endif /* __AL_AXI_QSPI_HAL__ */
