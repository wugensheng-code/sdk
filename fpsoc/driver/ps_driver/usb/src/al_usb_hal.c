/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_usb_hal.h"

/************************** Variable Definitions *****************************/
static AL_USB_HalStruct AL_USB_HalInstance[AL_USB_NUM_INSTANCE];
extern AL_USB_HwConfigStruct AlUsb_HwCfg[AL_USB_NUM_INSTANCE];

/**
 * @brief  This function looks for the device configuration based on the unique device ID.
 * @param  DeviceId is the unique device ID of the device being looked up.
 * @return A pointer to the configuration table entry corresponding to the given device ID, or NULL if no match is found.
 */
static AL_USB_HwConfigStruct *AlUsb_Hal_LookupConfig(AL_U32 DeviceId)
{
    AL_U32 Index = 0;
    AL_USB_HwConfigStruct *CfgPtr = AL_NULL;

    for (Index = 0; Index < AL_USB_NUM_INSTANCE; Index++) {
        if (AlUsb_HwCfg[Index].DeviceId == DeviceId) {
            CfgPtr = &AlUsb_HwCfg[Index];
            break;
        }
    }

    return CfgPtr;
}

/**
 * @brief  This function inits USB module.
 * @param  Handle is pointer to AL_USB_HalStruct
 * @param  HwConfig
 * @return AL_S32
 */
AL_S32 AlUsb_Hal_Init(AL_USB_HalStruct **Handle, AL_U32 DevId, AL_VOID *CallBack)
{
    AL_S32 Ret = AL_OK;
    AL_USB_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT(Handle != AL_NULL, AL_USB_ERR_ILLEGAL_PARAM);

    HwConfig = AlUsb_Hal_LookupConfig(DevId);
    if(HwConfig != AL_NULL){
        (*Handle) = &AL_USB_HalInstance[DevId];
    } else {
        return AL_USB_ERR_ILLEGAL_PARAM;
    }

    (*Handle)->HwConfig.BaseAddress = HwConfig->BaseAddress;
    (*Handle)->HwConfig.IntrId = HwConfig->IntrId;

    (AL_VOID)AlIntr_RegHandler((*Handle)->HwConfig.IntrId, AL_NULL, CallBack, *Handle);

    return AL_OK;
}
