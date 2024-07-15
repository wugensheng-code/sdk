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
 * This function looks up the hardware configuration structure for a given device ID.
 *
 * @param DevId Hardware module ID to look up.
 * @return Pointer to the AL_WDT_HwConfigStruct structure for the specified hardware config. Returns NULL if the device ID is not found.
 */
static AL_WDT_HwConfigStruct *AlWdt_Hal_LookupConfig(AL_U32 DevId)
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
 * This function default event handler for WDT interrupt. This function is called when a WDT interrupt occurs.
 *
 * @param CallbackRef Pointer to user-defined data, typically a structure representing the WDT instance.
 */
AL_VOID AlWdt_Hal_DefEventHandler(AL_VOID *CallbackRef)
{
    AL_UNUSED(CallbackRef);

    AL_LOG(AL_LOG_LEVEL_DEBUG, "WDT interrupt occurred\r\n");
}

/**
 * This function handles all WDT interrupt entries. This function is called whenever a WDT interrupt is triggered.
 *
 * @param Instance Pointer to the interrupt callback reference, typically a structure representing the WDT instance.
 */
AL_VOID AlWdt_Hal_IntrHandler(AL_VOID *Instance)
{
    AL_WDT_HalStruct *Wdt = (AL_WDT_HalStruct *)Instance;

    Wdt->EventCallBack(Wdt->EventCallBackRef);

    AlWdt_ll_ClearIntr(Wdt->BaseAddr);
}

/**
 * This function begisters a user-defined callback function for WDT events.
 *
 * @param Wdt Pointer to the AL_WDT_HalStruct structure that contains the WDT instance.
 * @param Callback Pointer to the user-defined callback function to be registered.
 * @param CallbackRef Pointer to user-defined data to be passed to the callback function when it is called.
 * @return AL_OK if the function succeeds in registering the callback. Returns an error code if the function fails.
 */
AL_S32 AlWdt_Hal_RegisterEventCallBack(AL_WDT_HalStruct *Wdt, AL_Wdt_EventCallBack Callback, void *CallbackRef)
{
    AL_ASSERT((Wdt != AL_NULL && Callback != AL_NULL), AL_WDT_ERR_ILLEGAL_PARAM);

    Wdt->EventCallBack        = Callback;
    Wdt->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

/**
 * This function initializes the WDT module with specified parameters and associates it with a handle.
 *
 * @param Handle Double pointer to a AL_WDT_HalStruct structure that will contain the WDT instance after initialization.
 * @param DevId Hardware module ID for the WDT.
 * @param InitConfig Pointer to a AL_WDT_InitStruct structure that contains the configuration information for the WDT.
 * @param Callback Pointer to a user-defined callback function for WDT events.
 * @return AL_OK if the function succeeds in initializing the WDT. Returns an error code if the function fails.
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

    (*Handle)->BaseAddr     = HwConfig->BaseAddress;
    (*Handle)->DevId        = DevId;
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

    return AlIntr_RegHandler((*Handle)->IntrNum, AL_NULL, AlWdt_Hal_IntrHandler, *Handle);
}

/**
 * This function stops the WDT.
 *
 * @param Wdt Pointer to the AL_WDT_HalStruct structure that contains the WDT instance to be stopped.
 */
AL_VOID AlWdt_Hal_Stop(AL_WDT_HalStruct *Wdt)
{
    AlWdt_ll_Pause(Wdt->BaseAddr, AL_FUNC_ENABLE);

    return;
}

/**
 * This function starts the WDT.
 *
 * @param Wdt Pointer to the AL_WDT_HalStruct structure that contains the WDT instance to be started.
 */
AL_VOID AlWdt_Hal_Start(AL_WDT_HalStruct *Wdt)
{
    AlWdt_ll_Enable(Wdt->BaseAddr, AL_FUNC_ENABLE);

    return;
}

/**
 * This function feeds the WDT to prevent it from resetting the system.
 *
 * @param Wdt Pointer to the AL_WDT_HalStruct structure that contains the WDT instance to be fed.
 * @return AL_OK if the function succeeds in feeding the WDT.
 */
AL_S32 AlWdt_Hal_Feed(AL_WDT_HalStruct *Wdt)
{
    AlWdt_ll_CounterRestart(Wdt->BaseAddr);

    return AL_OK;
}

/**
 * This function sets up the timeout period for the WDT.
 *
 * @param Wdt Pointer to the AL_WDT_HalStruct structure that contains the WDT instance.
 * @param TimeOutPeriod The timeout period to be set for the WDT.
 */
AL_VOID AlWdt_Hal_SetupTimeout(AL_WDT_HalStruct *Wdt, AL_WDT_TIMEOUT_PERIOD TimeOutPeriod)
{
    AlWdt_ll_Set_TimeoutPeriod(Wdt->BaseAddr, TimeOutPeriod);

    return;
}
