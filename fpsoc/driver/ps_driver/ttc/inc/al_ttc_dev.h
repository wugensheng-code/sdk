/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_TTC_DEV_H_
#define __AL_TTC_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_ttc_ll.h"


/**************************** Exported Typedef *******************************/

#define AL_TTC_ERR_NULL_PTR                 (AL_DEF_ERR(AL_TTC, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_TTC_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_TTC, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_TTC_ERR_NOT_READY                (AL_DEF_ERR(AL_TTC, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY))
#define AL_TTC_ERR_NOT_SUPPORT              (AL_DEF_ERR(AL_TTC, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT))
#define AL_TTC_ERR_TIMEOUT                  (AL_DEF_ERR(AL_TTC, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT))
#define AL_TTC_ERR_BUSY                     (AL_DEF_ERR(AL_TTC, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY))

/**************************** Type Definitions *******************************/

typedef struct
{
    AL_TTC_CountDec           CountDec;
    AL_TTC_ClkSrcEnum         ClkSrc;
    AL_BOOL                   EnablePrescale;
    AL_U8                     PrescaleVal; /* max val is 0xf */
} AL_TTC_TimerInitStruct;

typedef enum
{
    AL_TTC_EVENT_Interval      = BIT(0),
    AL_TTC_EVENT_Match1        = BIT(1),
    AL_TTC_EVENT_Match2        = BIT(2),
    AL_TTC_EVENT_Match3        = BIT(3),
    AL_TTC_EVENT_Overflow      = BIT(4),
    AL_TTC_EVENT_EventTimer    = BIT(5)
} AL_TTC_EventIdEnum;

typedef enum
{
    AL_TTC_IOCTL_SetIntervalMaxVal,
    AL_TTC_IOCTL_GetEventTimerVal,
    AL_TTC_IOCTL_GetCounterVal,
    AL_TTC_IOCTL_EnableWaveOutput,
} AL_TTC_IoCtlCmdEnum;

typedef union
{
    AL_U16    IntervalMaxVal;
    AL_U16    GetEventTimerVal;
    AL_U16    GetCounterVal;
    AL_BOOL   WaveOutputState;
}AL_TTC_IoctlParamUnion;

typedef struct
{
    AL_TTC_EventIdEnum      Events;
    AL_U32                  EventData;
} AL_TTC_EventStruct;

typedef AL_VOID (*AL_TTC_EventCallBack)(AL_TTC_EventStruct TtcEvent, AL_VOID *CallbackRef);

typedef struct
{
    AL_U32                        DevId;
    AL_REG                        BaseAddr;
    AL_U32                        IntrNum;
    AL_U32                        InputClockHz;
    AL_TTC_EventCallBack          EventCallBack;
    AL_VOID                       *EventCallBackRef;
    AL_TTC_TimerInitStruct        TimerInitConfigs;
} AL_TTC_DevStruct;

/************************** Function Prototypes ******************************/
AL_TTC_HwConfigStruct *AlTtc_Dev_LookupConfig(AL_U32 DevId);
AL_S32 AlTtc_Dev_Init(AL_TTC_DevStruct *Ttc, AL_U32 DevId, AL_TTC_TimerInitStruct *InitConfig);
AL_VOID AlTtc_Dev_IntrHandler(AL_VOID *Instance);
AL_S32 AlTtc_Dev_IoCtl(AL_TTC_DevStruct *Ttc, AL_TTC_IoCtlCmdEnum Cmd, AL_TTC_IoctlParamUnion *IoctlParam);
AL_S32 AlTtc_Dev_RegisterEventCallBack(AL_TTC_DevStruct *Ttc, AL_TTC_EventCallBack Callback, void *CallbackRef);
AL_S32 AlTtc_Dev_UnRegisterEventCallBack(AL_TTC_DevStruct *Ttc);

#ifdef __cplusplus
}
#endif

#endif
