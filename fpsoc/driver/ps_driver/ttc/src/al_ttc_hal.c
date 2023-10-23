/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_ttc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

static AL_TTC_HalStruct AlTtcHandle[AL_TTC_NUM_INSTANCE];

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/

static AL_VOID AlTtc_Hal_DefEventHandler(AL_TTC_EventStruct TtcEvent, AL_VOID *CallbackRef)
{
    switch (TtcEvent.Events)
    {
    case AL_TTC_EVENT_Interval:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al ttc generated a interval interrupt");
        break;
    case AL_TTC_EVENT_Match1:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al ttc generated a match1 interrupt");
        break;
    case AL_TTC_EVENT_Match2:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al ttc generated a match2 interrupt");
        break;
    case AL_TTC_EVENT_Match3:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al ttc generated a match3 interrupt");
        break;
    case AL_TTC_EVENT_Overflow:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al ttc generated a overflow interrupt");
        break;
    case AL_TTC_EVENT_EventTimer:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al ttc generated a event timer interrupt");
        break;
    default:
        break;
    }

}

AL_S32 AlTtc_Hal_Init(AL_TTC_HalStruct **Handle, AL_U32 DevId,
                      AL_TTC_TimerInitStruct *InitConfig, AL_TTC_EventCallBack Callback)
{
    AL_S32 Ret;
    AL_TTC_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    HwConfig = AlTtc_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlTtcHandle[DevId];
    } else {
        return AL_TTC_ERR_ILLEGAL_PARAM;
    }

    Ret = AlTtc_Dev_Init(&(*Handle)->Dev, DevId, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    if(Callback == AL_NULL) {
        Ret = AlTtc_Dev_RegisterEventCallBack(&(*Handle)->Dev, AlTtc_Hal_DefEventHandler, (AL_VOID *)Handle);
    } else {
        Ret = AlTtc_Dev_RegisterEventCallBack(&(*Handle)->Dev, Callback, (AL_VOID *)Handle);
    }

    (AL_VOID)AlIntr_RegHandler((*Handle)->Dev.IntrNum, AL_NULL, AlTtc_Dev_IntrHandler, &(*Handle)->Dev);

    return Ret;
}

AL_VOID AlTtc_Hal_EnableOverflowMode(AL_TTC_HalStruct *Handle)
{
    AlTtc_Dev_EnableOverflowMode(&Handle->Dev);
}

AL_VOID AlTtc_Hal_EnableIntervalMode(AL_TTC_HalStruct *Handle)
{
    AlTtc_Dev_EnableIntervalMode(&Handle->Dev);
}

AL_S32 AlTtc_Hal_SetIntervalMaxVal(AL_TTC_HalStruct *Handle, AL_U16 Value)
{
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    (AL_VOID)AlTtc_Dev_SetIntervalMaxVal(&Handle->Dev, Value);

    return AL_OK;
}

AL_U16 AlTtc_Hal_GetCounterVal(AL_TTC_HalStruct *Handle)
{
    return AlTtc_Dev_GetCounterVal(&Handle->Dev);
}

AL_S32 AlTtc_Hal_EnableMatchMode(AL_TTC_HalStruct *Handle, AL_BOOL State)
{
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    (AL_VOID)AlTtc_Dev_EnableMatchMode(&Handle->Dev, State);

    return AL_OK;
}

AL_S32 AlTtc_Hal_SetMatchVal(AL_TTC_HalStruct *Handle, AL_TTC_MatchNumEnum MatchNum, AL_U16 Value)
{
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);
    AL_ASSERT(MatchNum != AL_TTC_Match1 || MatchNum != AL_TTC_Match2 ||
              MatchNum != AL_TTC_Match3, AL_TTC_ERR_ILLEGAL_PARAM);

    (AL_VOID)AlTtc_Dev_SetMatchVal(&Handle->Dev, MatchNum, Value);

    return AL_OK;
}

AL_S32 AlTtc_Hal_EnableEventTimerMode(AL_TTC_HalStruct *Handle, AL_BOOL State)
{
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    (AL_VOID)AlTtc_Dev_EnableEventTimerMode(&Handle->Dev, State);

    return AL_OK;
}

AL_S32 AlTtc_Hal_SelExtClkEdge(AL_TTC_HalStruct *Handle, AL_TTC_ClkEdgeEnum ClkEdge)
{
    AL_S32 Ret;
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    Ret = AlTtc_Dev_SelExtClkEdge(&Handle->Dev, ClkEdge);
    if (Ret != AL_OK) {
        return Ret;
    }

    return AL_OK;
}

/*Set the event timer to count the pulse width of high or low levels */
AL_S32 AlTtc_Hal_SetEventTimerLevel(AL_TTC_HalStruct *Handle, AL_TTC_LevelEnum Level)
{
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);
    AL_ASSERT((Level != AL_TTC_HighLevel || Level != AL_TTC_HighLevel), AL_TTC_ERR_ILLEGAL_PARAM);

    (AL_VOID)AlTtc_Dev_SetEventTimerLevel(&Handle->Dev, Level);

    return AL_OK;
}

/*wheh event timer is overflow,whether continue counting*/
AL_S32 AlTtc_Hal_EventTimerOv(AL_TTC_HalStruct *Handle, AL_BOOL State)
{
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    (AL_VOID)AlTtc_Dev_EventTimerOv(&Handle->Dev, State);

    return AL_OK;
}

AL_U16 AltTtc_Hal_GetEventTimerVal(AL_TTC_HalStruct *Handle)
{
    return AltTtc_Dev_GetEventTimerVal(&Handle->Dev);
}

AL_S32 AlTtc_Hal_SetWaveformPolarity(AL_TTC_HalStruct *Handle, AL_TTC_ClkEdgeEnum ClkEdge)
{
    AL_S32 Ret = AL_OK;
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    Ret = AlTtc_Dev_SetWaveformPolarity(&Handle->Dev, ClkEdge);
    if (Ret != AL_OK) {
        return Ret;
    }

    return AL_OK;
}

AL_S32 AlTtc_Hal_EnableWaveOutput(AL_TTC_HalStruct *Handle, AL_BOOL State)
{
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    (AL_VOID)AlTtc_Dev_EnableWaveOutput(&Handle->Dev, State);

    return AL_OK;
}

AL_S32 AlTtc_Hal_EnableCounter(AL_TTC_HalStruct *Handle, AL_BOOL State)
{
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    (AL_VOID)AlTtc_Dev_EnableCounter(&Handle->Dev, State);

    return AL_OK;
}

AL_S32 AlTtc_Hal_EnableIntr(AL_TTC_HalStruct *Handle, AL_TTC_IntrTypeEnum IntrType)
{
    AL_S32 Ret = AL_OK;
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    Ret = AlTtc_Dev_EnableIntr(&Handle->Dev, IntrType);
    if (Ret != AL_OK) {
        return Ret;
    }

    return AL_OK;
}

AL_S32 AlTtc_Hal_IoCtl(AL_TTC_HalStruct *Handle, AL_TTC_IoCtlCmdEnum Cmd, AL_TTC_IoctlParamUnion *IoctlParam)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    Ret = AlTtc_Dev_IoCtl(&Handle->Dev, Cmd, IoctlParam);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "TTC Ioctl cmd error:%d\r\n", Ret);
    }

    return Ret;
}



