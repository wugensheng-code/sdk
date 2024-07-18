/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_tc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

static AL_TC_HalStruct AlTcHandle[AL_TC_NUM_INSTANCE];

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 * This function is executed when a callback is made in an interrupt function.
 * It switches through the possible events that can trigger the interrupt and logs the event.
 *
 * @param TcEvent Pointer to AL_TC_EventStruct contains event data.
 * @param CallbackRef Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @return None.
 */
static AL_VOID AlTc_Hal_DefEventHandler(AL_TC_EventStruct TcEvent, AL_VOID *CallbackRef)
{
    switch (TcEvent.Events)
    {
    case AL_TC_EVENT_Interval:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al tc generated a interval interrupt");
        break;
    case AL_TC_EVENT_Match1:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al tc generated a match1 interrupt");
        break;
    case AL_TC_EVENT_Match2:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al tc generated a match2 interrupt");
        break;
    case AL_TC_EVENT_Match3:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al tc generated a match3 interrupt");
        break;
    case AL_TC_EVENT_Overflow:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al tc generated a overflow interrupt");
        break;
    case AL_TC_EVENT_EventTimer:
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Al tc generated a event timer interrupt");
        break;
    default:
        break;
    }

}

/**
 * This function initializes the TC HAL layer with the specified device ID and initialization configuration.
 * If a callback is provided, it registers the callback for event handling; otherwise, it registers a default handler.
 * It also registers an interrupt handler for the device.
 *
 * @param Handle Double pointer to AL_TC_HalStruct that will be initialized.
 * @param DevId Device ID for the TC instance.
 * @param InitConfig Pointer to initialization configuration structure.
 * @param Callback Event callback function pointer.
 * @return AL_S32 Returns AL_OK on success, AL_TC_ERR_ILLEGAL_PARAM if parameters are invalid.
 */
AL_S32 AlTc_Hal_Init(AL_TC_HalStruct **Handle, AL_U32 DevId,
                      AL_TC_TimerInitStruct *InitConfig, AL_TC_EventCallBack Callback)
{
    AL_S32 Ret;
    AL_TC_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT((Handle != AL_NULL), AL_TC_ERR_ILLEGAL_PARAM);

    HwConfig = AlTc_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlTcHandle[DevId];
    } else {
        return AL_TC_ERR_ILLEGAL_PARAM;
    }

    Ret = AlTc_Dev_Init(&(*Handle)->Dev, DevId, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    if(Callback == AL_NULL) {
        Ret = AlTc_Dev_RegisterEventCallBack(&(*Handle)->Dev, AlTc_Hal_DefEventHandler, (AL_VOID *)Handle);
    } else {
        Ret = AlTc_Dev_RegisterEventCallBack(&(*Handle)->Dev, Callback, (AL_VOID *)Handle);
    }

    (AL_VOID)AlIntr_RegHandler((*Handle)->Dev.IntrNum, AL_NULL, AlTc_Dev_IntrHandler, &(*Handle)->Dev);

    return Ret;
}

/**
 * This function enables the overflow mode in the TC device.
 * This mode configures the TC to generate an interrupt when the counter overflows.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @return None.
 */
AL_VOID AlTc_Hal_EnableOverflowMode(AL_TC_HalStruct *Handle)
{
    AlTc_ll_EnbaleOverflowMode(Handle->Dev.BaseAddr);
}

/**
 * This function enables the interval mode in the TC device.
 * This mode configures the TC to generate an interrupt at regular intervals.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @return None.
 */
AL_VOID AlTc_Hal_EnableIntervalMode(AL_TC_HalStruct *Handle)
{
    AlTc_ll_EnbaleIntervalMode(Handle->Dev.BaseAddr);
}

/**
 * This function sets the maximum value for the interval mode in the TC device.
 * This value determines the interval at which interrupts will be generated.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param Value The maximum value of the interval mode.
 * @return None.
 */
AL_VOID AlTc_Hal_SetIntervalMaxVal(AL_TC_HalStruct *Handle, AL_U16 Value)
{
    AlTc_ll_SetIntervalMaxVal(Handle->Dev.BaseAddr, Value);
}

/**
 * This function retrieves the current counter value from the TC device.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @return AL_U16 The current count value.
 */
AL_U16 AlTc_Hal_GetCounterVal(AL_TC_HalStruct *Handle)
{
    return AlTc_ll_GetCounterVal(Handle->Dev.BaseAddr);
}

/**
 * This function enables or disables the match mode in the TC device.
 * In match mode, an interrupt is generated when the counter matches a predefined value.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param State Boolean value to enable or disable match mode.
 * @return None.
 */
AL_VOID AlTc_Hal_EnableMatchMode(AL_TC_HalStruct *Handle, AL_BOOL State)
{
    AlTc_ll_EnableMatchMode(Handle->Dev.BaseAddr, State);
}

/**
 * This function sets the match value for a specific match number in the TC device.
 * This value is used to generate interrupts when the counter matches this value.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param MatchNum The match number (e.g., Match1, Match2, Match3).
 * @param Value The match value.
 * @return None.
 */
AL_VOID AlTc_Hal_SetMatchVal(AL_TC_HalStruct *Handle, AL_TC_MatchNumEnum MatchNum, AL_U16 Value)
{
    AlTc_ll_SetMatchVal(Handle->Dev.BaseAddr, Handle->Dev.DevId, MatchNum, Value);
}

/**
 * This function enables or disables the event timer mode in the TC device.
 * In this mode, the TC can count external events.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param State Boolean value to enable or disable event timer mode.
 * @return None.
 */

AL_VOID AlTc_Hal_EnableEventTimerMode(AL_TC_HalStruct *Handle, AL_BOOL State)
{
    AlTc_ll_EnableEventTimerMode(Handle->Dev.BaseAddr, State);
}

/**
 * This function sets the edge of the external input clock to count in the TC device.
 * This determines whether the TC counts on the rising edge, falling edge, or both.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param ClkEdge The counting edge configuration.
 * @return AL_S32 Returns AL_OK on success, AL_TC_ERR_ILLEGAL_PARAM if parameters are invalid, or AL_TC_ERR_NOT_SUPPORT if the feature is not supported.
 */
AL_S32 AlTc_Hal_SetExtClkEdge(AL_TC_HalStruct *Handle, AL_TC_ClkEdgeEnum ClkEdge)
{
    AL_ASSERT((Handle != AL_NULL), AL_TC_ERR_ILLEGAL_PARAM);

    if (Handle->Dev.TimerInitConfigs.ClkSrc == AL_TC_EXTCLK) {
        AlTc_ll_SetExtClkEdge(Handle->Dev.BaseAddr, ClkEdge);
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Set external clk edge need enable external clk");
        return AL_TC_ERR_NOT_SUPPORT;
    }

    return AL_OK;
}

/**
 * This function sets the event timer to count the pulse width of high or low levels in the TC device.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param Level The counting level (high or low).
 * @return None.
 */
AL_VOID AlTc_Hal_SetEventTimerLevel(AL_TC_HalStruct *Handle, AL_TC_LevelEnum Level)
{
    AlTc_ll_SetEventTimerLevel(Handle->Dev.BaseAddr, Level);
}

/**
 * This function sets whether to continue counting when the event timer overflows in the TC device.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param State Boolean value to enable or disable continuation of counting on overflow.
 * @return None.
 */
AL_VOID AlTc_Hal_SetEventTimerOvState(AL_TC_HalStruct *Handle, AL_BOOL State)
{
    AlTc_ll_SetEventTimerOvState(Handle->Dev.BaseAddr, State);
}

/**
 * This function retrieves the current count value of the event timer from the TC device.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @return AL_U16 The current count value of the event timer.
 */
AL_U16 AlTc_Hal_GetEventTimerVal(AL_TC_HalStruct *Handle)
{
    return AlTc_ll_GetEventTimerVal(Handle->Dev.BaseAddr);
}

/**
 * This function determines whether the waveform starts high and goes low, or starts low and goes high.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param ClkEdge The working clock edge configuration.
 * @return None.
 */
AL_VOID AlTc_Hal_SetWaveformPolarity(AL_TC_HalStruct *Handle, AL_TC_ClkEdgeEnum ClkEdge)
{
    AlTc_ll_SetWaveformPolarity(Handle->Dev.BaseAddr, ClkEdge);
}

/**
 * This function controls whether the TC generates a waveform output.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param State Boolean value to enable or disable waveform output.
 * @return None.
 */
AL_VOID AlTc_Hal_EnableWaveOutput(AL_TC_HalStruct *Handle, AL_BOOL State)
{
    AlTc_ll_EnableWaveOutput(Handle->Dev.BaseAddr, State);
}

/**
 * This function controls whether the TC starts or stops counting.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param State Boolean value to enable or disable the counter.
 * @return None.
 */
AL_VOID AlTc_Hal_EnableCounter(AL_TC_HalStruct *Handle, AL_BOOL State)
{
    AlTc_ll_EnbaleCounter(Handle->Dev.BaseAddr, State);
}

/**
 * This function allows control over which events will generate interrupts.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param IntrType The type of interrupt to enable or disable.
 * @param State Boolean value to enable or disable the interrupt.
 * @return None.
 */
AL_VOID AlTc_Hal_EnableIntr(AL_TC_HalStruct *Handle, AL_TC_IntrTypeEnum IntrType, AL_BOOL State)
{
    AlTc_ll_EnableIntr(Handle->Dev.BaseAddr, IntrType, State);
}

/**
 * This function provides a generic interface for various TC configurations and status checks.
 *
 * @param Handle Pointer to a AL_TC_HalStruct structure that contains TC device instance.
 * @param Cmd The IOCTL command to execute.
 * @param IoctlParam Pointer to command arguments.
 * @return AL_S32 Returns AL_OK on success or an error code on failure.
 */
AL_S32 AlTc_Hal_IoCtl(AL_TC_HalStruct *Handle, AL_TC_IoCtlCmdEnum Cmd, AL_TC_IoctlParamUnion *IoctlParam)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_TC_ERR_ILLEGAL_PARAM);

    Ret = AlTc_Dev_IoCtl(&Handle->Dev, Cmd, IoctlParam);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "TC Ioctl cmd error:%d\r\n", Ret);
    }

    return Ret;
}



