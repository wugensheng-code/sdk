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

AL_S16 AlAdc_Hal_GetAdcData(AL_ADC_HalStruct *Handle, AL_ADC_ChanEnum ChanNum)
{
    return AlAdc_Dev_GetAdcData(&Handle->Dev, ChanNum);
}

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