/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_TC_DEV_H_
#define __AL_TC_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_tc_ll.h"


/**************************** Exported Typedef *******************************/

#define AL_TC_ERR_NULL_PTR                 (AL_DEF_ERR(AL_TC, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_TC_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_TC, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_TC_ERR_NOT_READY                (AL_DEF_ERR(AL_TC, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY))
#define AL_TC_ERR_NOT_SUPPORT              (AL_DEF_ERR(AL_TC, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT))
#define AL_TC_ERR_TIMEOUT                  (AL_DEF_ERR(AL_TC, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT))
#define AL_TC_ERR_BUSY                     (AL_DEF_ERR(AL_TC, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY))

/**************************** Type Definitions *******************************/

typedef struct
{
    AL_TC_CountDec           CountDec;
    AL_TC_ClkSrcEnum         ClkSrc;
    AL_BOOL                   EnablePrescale;
    AL_U8                     PrescaleVal; /* max val is 0xf */
} AL_TC_TimerInitStruct;

typedef enum
{
    AL_TC_EVENT_Interval      = BIT(0),
    AL_TC_EVENT_Match1        = BIT(1),
    AL_TC_EVENT_Match2        = BIT(2),
    AL_TC_EVENT_Match3        = BIT(3),
    AL_TC_EVENT_Overflow      = BIT(4),
    AL_TC_EVENT_EventTimer    = BIT(5)
} AL_TC_EventIdEnum;

typedef enum
{
    AL_TC_IOCTL_SetIntervalMaxVal,
    AL_TC_IOCTL_GetEventTimerVal,
    AL_TC_IOCTL_GetCounterVal,
    AL_TC_IOCTL_EnableWaveOutput,
} AL_TC_IoCtlCmdEnum;

typedef union
{
    AL_U16    IntervalMaxVal;
    AL_U16    GetEventTimerVal;
    AL_U16    GetCounterVal;
    AL_BOOL   WaveOutputState;
}AL_TC_IoctlParamUnion;

typedef struct
{
    AL_TC_EventIdEnum      Events;
    AL_U32                  EventData;
} AL_TC_EventStruct;

typedef AL_VOID (*AL_TC_EventCallBack)(AL_TC_EventStruct TcEvent, AL_VOID *CallbackRef);

typedef struct
{
    AL_U32                        DevId;
    AL_REG                        BaseAddr;
    AL_U32                        IntrNum;
    AL_U32                        InputClockHz;
    AL_TC_EventCallBack          EventCallBack;
    AL_VOID                       *EventCallBackRef;
    AL_TC_TimerInitStruct        TimerInitConfigs;
} AL_TC_DevStruct;

/************************** Function Prototypes ******************************/
AL_TC_HwConfigStruct *AlTc_Dev_LookupConfig(AL_U32 DevId);
AL_S32 AlTc_Dev_Init(AL_TC_DevStruct *Tc, AL_U32 DevId, AL_TC_TimerInitStruct *InitConfig);
AL_VOID AlTc_Dev_IntrHandler(AL_VOID *Instance);
AL_S32 AlTc_Dev_IoCtl(AL_TC_DevStruct *Tc, AL_TC_IoCtlCmdEnum Cmd, AL_TC_IoctlParamUnion *IoctlParam);
AL_S32 AlTc_Dev_RegisterEventCallBack(AL_TC_DevStruct *Tc, AL_TC_EventCallBack Callback, void *CallbackRef);
AL_S32 AlTc_Dev_UnRegisterEventCallBack(AL_TC_DevStruct *Tc);

#ifdef __cplusplus
}
#endif

#endif
