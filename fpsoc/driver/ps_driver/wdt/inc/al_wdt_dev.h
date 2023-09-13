/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_wdt_dev.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-03-29
 * @brief   wdt device driver
 */

#ifndef AL_WDT_DEV_H
#define AL_WDT_DEV_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************* Exported Includes ************************************/
#include "al_errno.h"
#include "al_wdt_ll.h"
#include "al_core.h"

#define AL_WDT_ERR_NULL_PTR                 (AL_DEF_ERR(AL_WDT, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_WDT_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_WDT, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_WDT_ERR_NOT_READY                (AL_DEF_ERR(AL_WDT, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY))
#define AL_WDT_ERR_TIMEOUT                  (AL_DEF_ERR(AL_WDT, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT))
#define AL_WDT_ERR_BUSY                     (AL_DEF_ERR(AL_WDT, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY))


/******************************* Exported Typedef ************************************/
/**
 * @brief  Configs Struct
 */

typedef struct
{
    AL_U32                  ResetPuaseLength;
    AL_U32                  ResponseMode;
    AL_U32                  TimeOutValue;
} AL_WDT_InitStruct;


typedef enum
{
    AL_WDT_STATE_ENABLE       = (0x0),
    AL_WDT_STATE_DISABLE      = (0x01 << 0)
} AL_WDT_StateEnum;

typedef AL_VOID (*AL_Wdt_EventCallBack)(AL_VOID *CallbackRef);

typedef struct
{
    AL_REG                        BaseAddr;
    AL_WDT_InitStruct             Configs;
    AL_Wdt_EventCallBack          EventCallBack;
    AL_VOID                       *EventCallBackRef;
    AL_WDT_StateEnum              State;
    AL_U32                        IntrNum;
    AL_U32                        DevId;
} AL_WDT_DevStruct;

AL_S32 AlWdt_Dev_Init(AL_WDT_DevStruct *Handle, AL_U32 DevId, AL_WDT_InitStruct *InitConfig);
AL_S32 AlWdt_Dev_Stop(AL_WDT_DevStruct *Handle);
AL_S32 AlWdt_Dev_Start(AL_WDT_DevStruct *Handle);
AL_S32 AlWdt_Dev_Feed(AL_WDT_DevStruct *Handle);
AL_S32 AlWdt_Dev_SetupTimeout(AL_WDT_DevStruct *Handle, AL_WDT_TIMEOUT_PERIOD TimeOutPeriod);
AL_VOID AlWdt_Dev_DefEventHandler(AL_VOID *CallbackRef);
AL_VOID AlWdt_Dev_IntrHandler(AL_VOID *Instance);

#ifdef __cplusplus
}
#endif

#endif
