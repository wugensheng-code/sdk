/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/***************************** Include Files *********************************/
#include "al_adc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

static AL_ADC_HalStruct AlAdcHandle[AL_ADC_NUM_INSTANCE];


/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/


/**
 * This function is called when a callback is triggered in an interrupt function.
 * @param   AdcEvent Pointer to AL_ADC_EventStruct containing event data
 * @param   CallbackRef Pointer to a AL_ADC_HalStruct structure that contains the ADC device instance
 * @return  None
 * @note    This function handles different ADC events such as data completion, threshold checks, and errors.
*/
AL_VOID AlAdc_Hal_DefEventHandler(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef)
{
    switch (AdcEvent.Events)
    {
    case AL_ADC_EVENT_DATA_DONE:
        break;
    case AL_ADC_EVENT_DATA_GTH:
        break;
    case AL_ADC_EVENT_DATA_LTH:
        break;
    case AL_ADC_EVENT_DATA_ERROR:
        break;
    default:
        break;
    }
}

/**
 * Initializes the ADC Hardware Abstraction Layer (HAL).
 *
 * Sets up the hardware configuration, initializes the device, registers interrupt handlers, and sets up event callbacks.
 *
 * @param Handle Pointer to the ADC HAL structure.
 * @param DevId Device ID of the ADC.
 * @param InitConfig Pointer to the ADC initialization configuration.
 * @param ChanConfig Pointer to the ADC channel configuration.
 * @param Callback Event callback function for the ADC.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlAdc_Hal_Init(AL_ADC_HalStruct **Handle, AL_U32 DevId, AL_ADC_InitStruct *InitConfig,
                       AL_ADC_ChanCfg *ChanConfig, AL_ADC_EventCallBack Callback)
{
    AL_S32 Ret;
    AL_ADC_HwConfigStruct *HwConfig;

    AL_ASSERT((Handle != AL_NULL && DevId < AL_ADC_NUM_INSTANCE), AL_ADC_ERR_ILLEGAL_PARAM);

    HwConfig = AlAdc_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlAdcHandle[DevId];
    } else {
        return AL_ADC_ERR_ILLEGAL_PARAM;
    }

    Ret = AlAdc_Dev_Init(&(*Handle)->Dev, DevId, InitConfig, ChanConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    (AL_VOID)AlIntr_RegHandler((*Handle)->Dev.IntrNum, AL_NULL, AlAdc_Dev_IntrHandler, &(*Handle)->Dev);

    if (Callback == AL_NULL) {
        Ret = AlAdc_Dev_RegisterEventCallBack(&(*Handle)->Dev, AlAdc_Hal_DefEventHandler, (AL_VOID *)(*Handle));
    } else {
        Ret = AlAdc_Dev_RegisterEventCallBack(&(*Handle)->Dev, Callback, (AL_VOID *)(*Handle));
    }

    Ret = AlOsal_Lock_Init(&(*Handle)->Lock, "Adc-Lock");
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}

/**
 * Starts collecting conversion data from the ADC.
 *
 * Disables ps-adc interrupt and uses pl-adc interrupts to determine if data conversion is complete.
 *
 * @param   Handle Pointer to a AL_ADC_HalStruct structure that contains the ADC device instance
 * @return  AL_OK for function success, other values indicate failure
 * @note    This function prepares the ADC for data collection.
*/
AL_S32 AlAdc_Hal_AdcStart(AL_ADC_HalStruct *Handle)
{
    AL_S32 Ret;

    AL_ASSERT((Handle != AL_NULL), AL_ADC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    AlAdc_ll_EnablePlAdc(Handle->Dev.AdcBaseAddr, AL_TRUE);
    AlAdc_ll_StartConv(Handle->Dev.AdcBaseAddr);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return AL_OK;
}

/**
 * Stops collecting conversion data from the ADC and clears pl-adc interrupts.
 *
 * @param   Handle Pointer to a AL_ADC_HalStruct structure that contains the ADC device instance
 * @return  AL_OK for function success, other values indicate failure
 * @note    This function stops the ADC from collecting data and clears any pending interrupts.
*/
AL_S32 AlAdc_Hal_AdcStop(AL_ADC_HalStruct *Handle)
{
    AL_S32 Ret;
    AL_U16 PlAdcIntrStatus;

    AL_ASSERT((Handle != AL_NULL), AL_ADC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    AlAdc_ll_StopConv(Handle->Dev.AdcBaseAddr);

    PlAdcIntrStatus = AlAdc_ll_GetPlAdcIntrType(Handle->Dev.AdcBaseAddr);
    if ((Handle->Dev.Configs.PlIntrCfg.IntrDoneMask == AL_FALSE) && (PlAdcIntrStatus & BIT(AL_ADC_PL_INTR_DONE))) {
        AlAdc_Dev_ClrPlAdcIntr(&Handle->Dev, AL_ADC_PL_INTR_DONE);
    }
    if ((Handle->Dev.Configs.PlIntrCfg.IntrDoneMask == AL_FALSE) && (PlAdcIntrStatus & BIT(AL_ADC_PL_INTR_GTH))) {
        AlAdc_Dev_ClrPlAdcIntr(&Handle->Dev, AL_ADC_PL_INTR_GTH);
    }
    if ((Handle->Dev.Configs.PlIntrCfg.IntrDoneMask == AL_FALSE) && (PlAdcIntrStatus & BIT(AL_ADC_PL_INTR_LTH))) {
        AlAdc_Dev_ClrPlAdcIntr(&Handle->Dev, AL_ADC_PL_INTR_LTH);
    }
    if ((Handle->Dev.Configs.PlIntrCfg.IntrDoneMask == AL_FALSE) && (PlAdcIntrStatus & BIT(AL_ADC_PL_INTR_ERROR))) {
        AlAdc_Dev_ClrPlAdcIntr(&Handle->Dev, AL_ADC_PL_INTR_ERROR);
    }

    AlAdc_ll_EnablePlAdc(Handle->Dev.AdcBaseAddr, AL_FALSE);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return AL_OK;
}

/**
 * Starts collecting conversion data from the ADC with interrupts.
 *
 * Enables ps-adc interrupt and uses pl-adc interrupts to determine if data conversion is complete.
 *
 * @param   Handle Pointer to a AL_ADC_HalStruct structure that contains the ADC device instance
 * @return  AL_OK for function success, other values indicate failure
 * @note    This function prepares the ADC for data collection with interrupt support.
*/
AL_S32 AlAdc_Hal_AdcStartIntr(AL_ADC_HalStruct *Handle)
{
    AL_S32 Ret;

    AL_ASSERT((Handle != AL_NULL), AL_ADC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    AlAdc_Dev_EnablePsAdcIntr(&Handle->Dev, AL_ADC_PLADC_INTR, AL_TRUE);
    AlAdc_ll_EnablePlAdc(Handle->Dev.AdcBaseAddr, AL_TRUE);
    AlAdc_ll_StartConv(Handle->Dev.AdcBaseAddr);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return AL_OK;
}

/**
 * Stops collecting conversion data from the ADC, clears pl-adc interrupts, and disables interrupt support.
 *
 * @param   Handle Pointer to a AL_ADC_HalStruct structure that contains the ADC device instance
 * @return  AL_OK for function success, other values indicate failure
 * @note    This function stops the ADC from collecting data, clears any pending interrupts, and disables interrupt support.
*/
AL_S32 AlAdc_Hal_AdcStopIntr(AL_ADC_HalStruct *Handle)
{
    AL_S32 Ret;
    AL_U16 PlAdcIntrStatus;

    AL_ASSERT((Handle != AL_NULL), AL_ADC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    AlAdc_ll_StopConv(Handle->Dev.AdcBaseAddr);
    AlAdc_Dev_EnablePsAdcIntr(&Handle->Dev, AL_ADC_PLADC_INTR, AL_FALSE);

    PlAdcIntrStatus = AlAdc_ll_GetPlAdcIntrType(Handle->Dev.AdcBaseAddr);
    if ((Handle->Dev.Configs.PlIntrCfg.IntrDoneMask == AL_FALSE) && (PlAdcIntrStatus & BIT(AL_ADC_PL_INTR_DONE))) {
        AlAdc_Dev_ClrPlAdcIntr(&Handle->Dev, AL_ADC_PL_INTR_DONE);
    }
    if ((Handle->Dev.Configs.PlIntrCfg.IntrDoneMask == AL_FALSE) && (PlAdcIntrStatus & BIT(AL_ADC_PL_INTR_GTH))) {
        AlAdc_Dev_ClrPlAdcIntr(&Handle->Dev, AL_ADC_PL_INTR_GTH);
    }
    if ((Handle->Dev.Configs.PlIntrCfg.IntrDoneMask == AL_FALSE) && (PlAdcIntrStatus & BIT(AL_ADC_PL_INTR_LTH))) {
        AlAdc_Dev_ClrPlAdcIntr(&Handle->Dev, AL_ADC_PL_INTR_LTH);
    }
    if ((Handle->Dev.Configs.PlIntrCfg.IntrDoneMask == AL_FALSE) && (PlAdcIntrStatus & BIT(AL_ADC_PL_INTR_ERROR))) {
        AlAdc_Dev_ClrPlAdcIntr(&Handle->Dev, AL_ADC_PL_INTR_ERROR);
    }

    AlAdc_ll_EnablePlAdc(Handle->Dev.AdcBaseAddr, AL_FALSE);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return AL_OK;
}

/**
 * Retrieves the ADC data for a specified channel.
 *
 * This function fetches the ADC conversion data for the channel specified by ChanNum. It delegates the actual data retrieval to the device-specific implementation by calling AlAdc_Dev_GetAdcData.
 *
 * @param Handle Pointer to the ADC HAL structure, representing the ADC device instance.
 * @param ChanNum The channel number from which to retrieve the ADC data.
 *
 * @return The ADC data for the specified channel as a 16-bit signed integer.
 */
AL_S16 AlAdc_Hal_GetAdcData(AL_ADC_HalStruct *Handle, AL_ADC_ChanEnum ChanNum)
{
    return AlAdc_Dev_GetAdcData(&Handle->Dev, ChanNum);
}

/**
 * @brief This function executes operations to set or check ADC status.
 *
 * @param Handle Pointer to a AL_ADC_HalStruct structure that contains ADC device instance.
 * @param Cmd Command to execute, specified as an AL_ADC_IoCtlCmdEnum value.
 * @param IoctlParam Pointer to command arguments, specified as AL_ADC_IoctlParamUnion.
 * @return AL_S32 Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlAdc_Hal_IoCtl(AL_ADC_HalStruct *Handle, AL_ADC_IoCtlCmdEnum Cmd, AL_ADC_IoctlParamUnion *IoctlParam)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_ADC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlAdc_Dev_IoCtl(&Handle->Dev, Cmd, IoctlParam);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc ioctl cmd error:%d\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return Ret;
}