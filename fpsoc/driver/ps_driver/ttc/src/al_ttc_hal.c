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
/**
 * This function is executed when a callback is made in an interrupt function.
 * It switches through the possible events that can trigger the interrupt and logs the event.
 *
 * @param TtcEvent Pointer to AL_TTC_EventStruct contains event data.
 * @param CallbackRef Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @return None.
 */
static AL_VOID AlTtc_Hal_DefEventHandler(AL_TTC_EventStruct TtcEvent, AL_VOID *CallbackRef)
{
    switch (TtcEvent.Events)
    {
    case AL_TTC_EVENT_Interval:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al ttc generated a interval interrupt");
        break;
    case AL_TTC_EVENT_Match1:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al ttc generated a match1 interrupt");
        break;
    case AL_TTC_EVENT_Match2:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al ttc generated a match2 interrupt");
        break;
    case AL_TTC_EVENT_Match3:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al ttc generated a match3 interrupt");
        break;
    case AL_TTC_EVENT_Overflow:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al ttc generated a overflow interrupt");
        break;
    case AL_TTC_EVENT_EventTimer:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al ttc generated a event timer interrupt");
        break;
    default:
        break;
    }

}

/**
 * This function initializes the TTC HAL layer with the specified device ID and initialization configuration.
 * If a callback is provided, it registers the callback for event handling; otherwise, it registers a default handler.
 * It also registers an interrupt handler for the device.
 *
 * @param Handle Double pointer to AL_TTC_HalStruct that will be initialized.
 * @param DevId Device ID for the TTC instance.
 * @param InitConfig Pointer to initialization configuration structure.
 * @param Callback Event callback function pointer.
 * @return AL_S32 Returns AL_OK on success, AL_TTC_ERR_ILLEGAL_PARAM if parameters are invalid.
 */
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

/**
 * This function enables the overflow mode in the TTC device.
 * This mode configures the TTC to generate an interrupt when the counter overflows.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @return None.
 */
AL_VOID AlTtc_Hal_EnableOverflowMode(AL_TTC_HalStruct *Handle)
{
    AlTtc_ll_EnbaleOverflowMode(Handle->Dev.BaseAddr);
}

/**
 * This function enables the interval mode in the TTC device.
 * This mode configures the TTC to generate an interrupt at regular intervals.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @return None.
 */
AL_VOID AlTtc_Hal_EnableIntervalMode(AL_TTC_HalStruct *Handle)
{
    AlTtc_ll_EnbaleIntervalMode(Handle->Dev.BaseAddr);
}

/**
 * This function sets the maximum value for the interval mode in the TTC device.
 * This value determines the interval at which interrupts will be generated.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param Value The maximum value of the interval mode.
 * @return None.
 */
AL_VOID AlTtc_Hal_SetIntervalMaxVal(AL_TTC_HalStruct *Handle, AL_U16 Value)
{
    AlTtc_ll_SetIntervalMaxVal(Handle->Dev.BaseAddr, Value);
}

/**
 * This function retrieves the current counter value from the TTC device.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @return AL_U16 The current count value.
 */
AL_U16 AlTtc_Hal_GetCounterVal(AL_TTC_HalStruct *Handle)
{
    return AltTtc_ll_GetCounterVal(Handle->Dev.BaseAddr);
}

/**
 * This function enables or disables the match mode in the TTC device.
 * In match mode, an interrupt is generated when the counter matches a predefined value.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param State Boolean value to enable or disable match mode.
 * @return None.
 */
AL_VOID AlTtc_Hal_EnableMatchMode(AL_TTC_HalStruct *Handle, AL_BOOL State)
{
    AlTtc_ll_EnableMatchMode(Handle->Dev.BaseAddr, State);
}

/**
 * This function sets the match value for a specific match number in the TTC device.
 * This value is used to generate interrupts when the counter matches this value.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param MatchNum The match number (e.g., Match1, Match2, Match3).
 * @param Value The match value.
 * @return None.
 */
AL_VOID AlTtc_Hal_SetMatchVal(AL_TTC_HalStruct *Handle, AL_TTC_MatchNumEnum MatchNum, AL_U16 Value)
{
    AlTtc_ll_SetMatchVal(Handle->Dev.BaseAddr, Handle->Dev.DevId, MatchNum, Value);
}

/**
 * This function enables or disables the event timer mode in the TTC device.
 * In this mode, the TTC can count external events.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param State Boolean value to enable or disable event timer mode.
 * @return None.
 */

AL_VOID AlTtc_Hal_EnableEventTimerMode(AL_TTC_HalStruct *Handle, AL_BOOL State)
{
    AlTtc_ll_EnableEventTimerMode(Handle->Dev.BaseAddr, State);
}

/**
 * This function sets the edge of the external input clock to count in the TTC device.
 * This determines whether the TTC counts on the rising edge, falling edge, or both.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param ClkEdge The counting edge configuration.
 * @return AL_S32 Returns AL_OK on success, AL_TTC_ERR_ILLEGAL_PARAM if parameters are invalid, or AL_TTC_ERR_NOT_SUPPORT if the feature is not supported.
 */
AL_S32 AlTtc_Hal_SetExtClkEdge(AL_TTC_HalStruct *Handle, AL_TTC_ClkEdgeEnum ClkEdge)
{
    AL_ASSERT((Handle != AL_NULL), AL_TTC_ERR_ILLEGAL_PARAM);

    if (Handle->Dev.TimerInitConfigs.ClkSrc == AL_TTC_EXTCLK) {
        AlTtc_ll_SetExtClkEdge(Handle->Dev.BaseAddr, ClkEdge);
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Set external clk edge need enable external clk");
        return AL_TTC_ERR_NOT_SUPPORT;
    }

    return AL_OK;
}

/**
 * This function sets the event timer to count the pulse width of high or low levels in the TTC device.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param Level The counting level (high or low).
 * @return None.
 */
AL_VOID AlTtc_Hal_SetEventTimerLevel(AL_TTC_HalStruct *Handle, AL_TTC_LevelEnum Level)
{
    AlTtc_ll_SetEventTimerLevel(Handle->Dev.BaseAddr, Level);
}

/**
 * This function sets whether to continue counting when the event timer overflows in the TTC device.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param State Boolean value to enable or disable continuation of counting on overflow.
 * @return None.
 */
AL_VOID AlTtc_Hal_SetEventTimerOvState(AL_TTC_HalStruct *Handle, AL_BOOL State)
{
    AlTtc_ll_SetEventTimerOvState(Handle->Dev.BaseAddr, State);
}

/**
 * This function retrieves the current count value of the event timer from the TTC device.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @return AL_U16 The current count value of the event timer.
 */
AL_U16 AltTtc_Hal_GetEventTimerVal(AL_TTC_HalStruct *Handle)
{
    return AltTtc_ll_GetEventTimerVal(Handle->Dev.BaseAddr);
}

/**
 * This function determines whether the waveform starts high and goes low, or starts low and goes high.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param ClkEdge The working clock edge configuration.
 * @return None.
 */
AL_VOID AlTtc_Hal_SetWaveformPolarity(AL_TTC_HalStruct *Handle, AL_TTC_ClkEdgeEnum ClkEdge)
{
    AlTtc_ll_SetWaveformPolarity(Handle->Dev.BaseAddr, ClkEdge);
}

/**
 * This function controls whether the TTC generates a waveform output.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param State Boolean value to enable or disable waveform output.
 * @return None.
 */
AL_VOID AlTtc_Hal_EnableWaveOutput(AL_TTC_HalStruct *Handle, AL_BOOL State)
{
    AlTtc_ll_EnableWaveOutput(Handle->Dev.BaseAddr, State);
}

/**
 * This function controls whether the TTC starts or stops counting.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param State Boolean value to enable or disable the counter.
 * @return None.
 */
AL_VOID AlTtc_Hal_EnableCounter(AL_TTC_HalStruct *Handle, AL_BOOL State)
{
    AlTtc_ll_EnbaleCounter(Handle->Dev.BaseAddr, State);
}

/**
 * This function allows control over which events will generate interrupts.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param IntrType The type of interrupt to enable or disable.
 * @param State Boolean value to enable or disable the interrupt.
 * @return None.
 */
AL_VOID AlTtc_Hal_EnableIntr(AL_TTC_HalStruct *Handle, AL_TTC_IntrTypeEnum IntrType, AL_BOOL State)
{
    AlTtc_ll_EnableIntr(Handle->Dev.BaseAddr, IntrType, State);
}

/**
 * This function provides a generic interface for various TTC configurations and status checks.
 *
 * @param Handle Pointer to a AL_TTC_HalStruct structure that contains TTC device instance.
 * @param Cmd The IOCTL command to execute.
 * @param IoctlParam Pointer to command arguments.
 * @return AL_S32 Returns AL_OK on success or an error code on failure.
 */
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



