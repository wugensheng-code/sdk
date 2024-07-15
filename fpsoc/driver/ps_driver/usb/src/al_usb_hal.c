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
 *
 * This function iterates through the available USB hardware configurations and returns the configuration
 * that matches the specified device ID. If no matching configuration is found, it returns NULL.
 *
 * @param DeviceId The device ID for which to find the hardware configuration.
 * @return A pointer to the matching AL_USB_HwConfigStruct if found, otherwise NULL.
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
 *
 * This function initializes the USB hardware based on the specified device ID. It sets up the hardware
 * configuration, including the base address and interrupt ID, and registers the interrupt handler. It
 * requires a valid handle to store the instance information and uses a callback function for interrupt handling.
 *
 * @param Handle A pointer to a pointer of AL_USB_HalStruct to store the USB instance information.
 * @param DevId The device ID of the USB to initialize.
 * @param CallBack A pointer to the callback function for interrupt handling.
 * @return AL_OK on success, AL_USB_ERR_ILLEGAL_PARAM if any parameter is invalid.
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
