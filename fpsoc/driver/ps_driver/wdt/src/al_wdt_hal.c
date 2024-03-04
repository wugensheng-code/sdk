/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_wdt_hal.h"
#include "al_pmu_ll.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_WDT_HalStruct AlWdtHandle[AL_WDT_NUM_INSTANCE];

static AL_WDT_InitStruct WdtDefInitConfigs = {
    .ResetPuaseLength = WDT_RPL_PCLK_CYCLES_8,
    .ResponseMode = WDT_INTR_MODE,
    .TimeOutValue = WDT_TIMEOUT_PERIOD_32M_CLOCKS,
};

extern AL_WDT_HwConfigStruct AlWdt_HwConfig[AL_WDT_NUM_INSTANCE];

/************************** Function Prototypes ******************************/

/********************************************************/
/**
 * This function look up hardware config structure.
 * @param   HaliceId is hardware module id
 * @return
 *          - AL_WDT_HwConfigStruct for hardware config
 * @note
*/
AL_WDT_HwConfigStruct *AlWdt_Hal_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AL_WDT_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_WDT_NUM_INSTANCE; Index++) {
        if (AlWdt_HwConfig[Index].DeviceId == DevId) {
            ConfigPtr = &AlWdt_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

/**
 * This function action When the wdt interrupt occurs.
 * @param   CallbackRef Pointer to a AL_WDT_HalStruct structure
 * @return
 * @note
*/
AL_VOID AlWdt_Hal_DefEventHandler(AL_VOID *CallbackRef)
{
    AL_UNUSED(CallbackRef);

    AL_LOG(AL_LOG_LEVEL_DEBUG, "WDT interrupt occurred\r\n");
}

/**
 * This function is wdt's all interrupt entries handler.
 * @param   Instance is pointer to interrupts call back reference
 * @return
 * @note
*/
AL_VOID AlWdt_Hal_IntrHandler(AL_VOID *Instance)
{
    AL_WDT_HalStruct *Wdt = (AL_WDT_HalStruct *)Instance;
    AlWdt_ll_ClearIntr(Wdt->BaseAddr);
}

/**
 * This function register a User WDT Callback.
 * @param   Wdt Pointer to a AL_WDT_HalStruct structure that contains wdt Halice instance
 * @param   CallBack pointer to the Callback function
 * @param   CallbackRef pointer to the Callback function params
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlWdt_Hal_RegisterEventCallBack(AL_WDT_HalStruct *Wdt, AL_Wdt_EventCallBack Callback, void *CallbackRef)
{
    AL_ASSERT((Wdt != AL_NULL && Callback != AL_NULL), AL_WDT_ERR_ILLEGAL_PARAM);

    Wdt->EventCallBack        = Callback;
    Wdt->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

/**
 * This function initialize the WDT mode according to the specified
 *          parameters in the AL_WDT_InitStruct and initialize the associated handle.
 * @param   Handle Pointer to a AL_WDT_HalStruct structure that contains wdt Hal instance
 * @param   HalId is hardware module id
 * @param   InitConfig pointer to a AL_WDT_InitStruct structure
 *          that contains the configuration information for the specified WDT peripheral
 * @return
 *          - AL_OK for function success
 *          - Other for function failuregit
 * @note
*/
AL_S32 AlWdt_Hal_Init(AL_WDT_HalStruct **Handle, AL_U32 DevId, AL_WDT_InitStruct *InitConfig, AL_Wdt_EventCallBack Callback)
{
    AL_S32 Ret = AL_OK;
    AL_WDT_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT((Handle != AL_NULL && DevId < AL_WDT_NUM_INSTANCE), AL_WDT_ERR_ILLEGAL_PARAM);

    HwConfig = AlWdt_Hal_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlWdtHandle[DevId];
    } else {
        return AL_WDT_ERR_ILLEGAL_PARAM;
    }

    (*Handle)->BaseAddr      = HwConfig->BaseAddress;
    (*Handle)->DevId         = DevId;
    (*Handle)->Configs      = (InitConfig == AL_NULL) ? WdtDefInitConfigs : (*InitConfig);
    (*Handle)->IntrNum      = AlWdt_Hal_LookupConfig(DevId)->IntrNum;
    
    AlWdt_ll_Enable(HwConfig->BaseAddress, AL_FUNC_DISABLE);
    AlWdt_ll_Set_RestPulseLen(HwConfig->BaseAddress, InitConfig->ResetPuaseLength);
    AlWdt_ll_Set_ResopnseMode(HwConfig->BaseAddress, InitConfig->ResponseMode);
    AlWdt_ll_Set_TimeoutPeriod(HwConfig->BaseAddress, InitConfig->TimeOutValue);
    AlWdt_ll_Enable(HwConfig->BaseAddress, AL_FUNC_ENABLE);
    AlWdt_ll_CounterRestart(HwConfig->BaseAddress);

    AlPmu_SetHwEn((DevId == 0) ? AL_PMU_ERR_WDT0_OT: AL_PMU_ERR_WDT1_OT);
    AlPmu_SetIntEn((DevId == 0) ? AL_PMU_ERR_WDT0_OT: AL_PMU_ERR_WDT1_OT);
    AlPmu_Shack(AL_PMU_SHACK_HDRST_CNT_CLR);

    AlWdt_ll_Pause(HwConfig->BaseAddress, AL_FUNC_DISABLE);

    if(Callback == AL_NULL) {
        Ret = AlWdt_Hal_RegisterEventCallBack(*Handle, AlWdt_Hal_DefEventHandler, (AL_VOID *)*Handle);
    } else {
        Ret = AlWdt_Hal_RegisterEventCallBack(*Handle, Callback, (AL_VOID *)*Handle);
    }
    if (Ret != AL_OK) {
        return Ret;
    }

    (AL_VOID)AlIntr_RegHandler((*Handle)->IntrNum, AL_NULL, AlWdt_Hal_IntrHandler, *Handle);

    return AL_OK;
}

AL_VOID AlWdt_Hal_Stop(AL_WDT_HalStruct *Wdt)
{
    AlWdt_ll_Pause(Wdt->BaseAddr, AL_FUNC_ENABLE);

    return;
}

AL_VOID AlWdt_Hal_Start(AL_WDT_HalStruct *Wdt)
{
    AlWdt_ll_Enable(Wdt->BaseAddr, AL_FUNC_ENABLE);

    return;
}

AL_S32 AlWdt_Hal_Feed(AL_WDT_HalStruct *Wdt)
{
    AL_S32 IntrClearValue = 0x0;

    IntrClearValue = AlWdt_ll_ClearIntr(Wdt->BaseAddr);
    if (!IntrClearValue) {
        return AL_WDT_ERR_BUSY;
    }

    return AL_OK;
}

AL_VOID AlWdt_Hal_SetupTimeout(AL_WDT_HalStruct *Wdt, AL_WDT_TIMEOUT_PERIOD TimeOutPeriod)
{
    AlWdt_ll_Set_TimeoutPeriod(Wdt->BaseAddr, TimeOutPeriod);

    return;
}
