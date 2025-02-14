/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_QSPI_DRV_H_
#define __AL_QSPI_DRV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_qspi_ll.h"

/************************** Constant Definitions ****************************/
#define MIN3(x, y, z) MIN((typeof(x))MIN(x, y), z)

#define AL_QSPI_ERR_ILLEGAL_PARAM    AL_DEF_ERR(AL_QSPI, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM)
#define AL_QSPI_ERR_BUSY             AL_DEF_ERR(AL_QSPI, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY)
#define AL_QSPI_ERR_TIMEOUT          AL_DEF_ERR(AL_QSPI, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT)
#define AL_QSPI_ERR_NOT_SUPPORT      AL_DEF_ERR(AL_QSPI, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT)
#define AL_QSPI_ERR_NOT_READY        AL_DEF_ERR(AL_QSPI, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY)
#define AL_QSPI_EVENTS_TO_ERRS(Event) (AL_DEF_ERR(AL_QSPI, AL_LOG_LEVEL_ERROR, (Event << AL_ERR_MAX)))

/**
 * @brief  Qspi state enum
 */
typedef enum {
    AL_QSPI_STATE_NOT_INIT     = 0,
    AL_QSPI_STATE_READY        = (1 << 0),
    AL_QSPI_STATE_TX_BUSY      = (1 << 1),
    AL_QSPI_STATE_RX_BUSY      = (1 << 2),
    AL_QSPI_STATE_ERROR        = (1 << 3)
} AL_QSPI_StateEnum;

/**
 * @brief  Qspi Io ctl cmd enum
 */
typedef enum
{
    AL_QSPI_IOCTL_SET_SLAVE_SELECT,
    AL_QSPI_IOCTL_GET_SLAVE_SELECT,
    AL_QSPI_IOCTL_SET_IO_FREQ,
    AL_QSPI_IOCTL_GET_IO_FREQ,
    AL_QSPI_IOCTL_SET_SLV_TOGGLE,
    AL_QSPI_IOCTL_GET_SLV_TOGGLE,
    AL_QSPI_IOCTL_SET_SAMPLE_DELAY,
    AL_QSPI_IOCTL_GET_SAMPLE_DELAY,
} AL_Qspi_IoCtlCmdEnum;

/**************************** Type Definitions ******************************/
/**
 * @brief  Qspi Buffer Struct
 */
typedef struct {
    AL_U8                     *BufferPtr;
    AL_U32                    RequestedCnt;        /*total size requested*/
    AL_U32                    HandledCnt;          /*the size has handled*/
} AL_QSPI_BufferStruct;

typedef AL_VOID (*QSPI_EventCallBack)(AL_QSPI_EventStruct QspiEvent, AL_VOID *CallbackRef);

/**
 * @brief  Qspi Dev Struct
 */
typedef struct
{
    AL_QSPI_HwConfigStruct        HwConfig;
    AL_S32                        BitsPerWord;
    AL_QSPI_ConfigsStruct         Configs;
    AL_QSPI_BufferStruct          SendBuffer;
    AL_QSPI_BufferStruct          RecvBuffer;
    QSPI_EventCallBack            EventCallBack;
    AL_VOID                       *EventCallBackRef;
    AL_U32                        ErrorCode;
    AL_QSPI_StateEnum             State;
} AL_QSPI_DevStruct;

/***************** Macros (Inline Functions) Definitions ********************/

/******************************** Variable Prototypes ******************************/

/******************************** Function Prototypes ******************************/
AL_S32 AlQspi_Dev_Init(AL_QSPI_DevStruct *Qspi, AL_QSPI_HwConfigStruct *HwConfig, AL_QSPI_ConfigsStruct *Init);
AL_S32 AlQspi_Dev_SendData(AL_QSPI_DevStruct *Qspi, AL_U8 *Data, AL_U32 SendSize);
AL_S32 AlQspi_Dev_RecvData(AL_QSPI_DevStruct *Qspi, AL_U8 *Data, AL_U16 ReceiveSize);
AL_S32 AlQspi_Dev_TranferData(AL_QSPI_DevStruct *Qspi, AL_U8 *SendBuf, AL_U32 SendSize, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize);
AL_S32 AlQspi_Dev_DmaSendData(AL_QSPI_DevStruct *Qspi, AL_U32 SendDataSize);
AL_S32 AlQspi_Dev_DmaRecvData(AL_QSPI_DevStruct *Qspi, AL_U16 RecvSize);
AL_S32 AlQspi_Dev_DmaTranferData(AL_QSPI_DevStruct *Qspi, AL_U32 SendSize, AL_U16 RecvSize);
AL_S32 AlQspi_Dev_IoCtl(AL_QSPI_DevStruct *Qspi, AL_Qspi_IoCtlCmdEnum Cmd, AL_VOID *Data);
AL_S32 AlQspi_Dev_RegisterIntrCallBack(AL_QSPI_DevStruct *Qspi, QSPI_EventCallBack Callback, AL_VOID *CallbackRef);
AL_S32 AlQspi_Dev_UnRegisterIntrCallBack(AL_QSPI_DevStruct *Qspi);
AL_VOID AlQspi_Dev_IntrHandler(AL_VOID *instance);
AL_QSPI_HwConfigStruct *AlQspi_Dev_LookupConfig(AL_U32 DeviceId);
AL_S32 AlQspi_Dev_XipAddr32Init(AL_QSPI_DevStruct *Qspi);
AL_S32 AlQspi_Dev_XipAddr24Init(AL_QSPI_DevStruct *Qspi, AL_U8* FlashId);
AL_VOID AlQspi_Dev_DumpReg(AL_REG QspiBaseAddr);

#ifdef __cplusplus
}
#endif

#endif
