/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_mpu_hal.h"
#include "al_errno.h"
#include "al_intr.h"

AL_MPU_DevStruct AL_MPU_DevInstance[AL_MPU_NUM_INSTANCE];
AL_MPU_HalStruct  AlMpuHandle[AL_MPU_NUM_INSTANCE];

/**
 *
 * This function registers a specific interrupt handler for the MPU (Memory Protection Unit) interrupts. It uses the SOC_MPU_IRQn
 * as the interrupt number and specifies the AlMpu_Dev_MpuIntrHandler as the callback function to handle MPU interrupts.
 * The function does not return any value and does not take any parameters.
 *
 * @return None
 */
static AL_VOID AlMpu_Hal_MpuRegisterIntr()
{
    AlIntr_RegHandler(SOC_MPU_IRQn, AL_NULL, AlMpu_Dev_MpuIntrHandler, AL_MPU_DevInstance);

}

/**
 *
 * This function is called when an MPU event occurs. It processes the event based on its ID. For example, if the event is
 * a deny access event, it logs the instance and region number that triggered the event. The function does not return any value.
 *
 * @param MpuEvent Pointer to the MPU event structure containing details about the event.
 * @param CallbackRef User-defined reference passed to the callback function.
 * @return None
 */
static AL_VOID AlMpu_Hal_EventCallBack(AL_MPU_EventStruct *MpuEvent, AL_VOID *CallbackRef)
{
    AL_MPU_HalStruct *Handle = (AL_MPU_HalStruct *)CallbackRef;

    switch (MpuEvent->EventId) {
        case AL_MPU_DENY_ACCESS:
            AL_LOG(AL_LOG_LEVEL_INFO, "AlMpu_Hal_EventCallBack MPU Instance is 0x%x, The region number "
               "that triggers the event is region%d.\r\n", Handle->Dev->HwConfig.BaseAddress, MpuEvent->EventData);
            break;

        default:
            break;
    }
}

/**
 *
 * This function enables the MPU by taking a lock, enabling the MPU device, and then releasing the lock. It returns an error code if any step fails.
 *
 * @param Handle Pointer to the MPU HAL structure.
 * @return AL_OK on success, an error code otherwise.
 */
AL_S32 AlMpu_Hal_MpuEnable(AL_MPU_HalStruct *Handle)
{
    AL_S32 RetValue;

    AL_ASSERT((Handle != AL_NULL), AL_MPU_ERR_ILLEGAL_PARAM);

    RetValue = AlOsal_Lock_Take(&Handle->Lock, 0);
    if (RetValue != AL_OK) {
        return RetValue;
    }

    RetValue = AlMpu_Dev_MpuEnable(Handle->Dev);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return RetValue;
}

/**
 *
 * Similar to AlMpu_Hal_MpuEnable, this function disables the MPU by taking a lock, disabling the MPU device,
 * and then releasing the lock. It returns an error code if any step fails.
 *
 * @param Handle Pointer to the MPU HAL structure.
 * @return AL_OK on success, an error code otherwise.
 */
AL_S32 AlMpu_Hal_MpuDisable(AL_MPU_HalStruct *Handle)
{
    AL_S32 RetValue;

    AL_ASSERT((Handle != AL_NULL), AL_MPU_ERR_ILLEGAL_PARAM);

    RetValue = AlOsal_Lock_Take(&Handle->Lock, 0);
    if (RetValue != AL_OK) {
        return RetValue;
    }

    RetValue = AlMpu_Dev_MpuDisable(Handle->Dev);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return RetValue;
}

/**
 *
 * This function enables a specified region of the MPU. It takes a lock, sets the region enable status to enabled,
 * and then releases the lock. It returns an error code if any step fails.
 *
 * @param Handle Pointer to the MPU HAL structure.
 * @param RegionNumber The number of the region to enable.
 * @return AL_OK on success, an error code otherwise.
 */
AL_S32 AlMpu_Hal_EnableRegion(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber)
{
    AL_S32 RetValue;

    AL_ASSERT((Handle != AL_NULL), AL_MPU_ERR_ILLEGAL_PARAM);

    RetValue = AlOsal_Lock_Take(&Handle->Lock, 0);
    if (RetValue != AL_OK) {
        return RetValue;
    }

    RetValue = AlMpu_Dev_SetRegionEnableStatus(Handle->Dev, RegionNumber, MPU_REGION_ENABLE);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return RetValue;
}

/**
 *
 * This function disables a specified region of the MPU. It takes a lock, sets the region enable status to disabled,
 * and then releases the lock. It returns an error code if any step fails.
 *
 * @param Handle Pointer to the MPU HAL structure.
 * @param RegionNumber The number of the region to disable.
 * @return AL_OK on success, an error code otherwise.
 */
AL_S32 AlMpu_Hal_DisableRegion(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber)
{
    AL_S32 RetValue;

    AL_ASSERT((Handle != AL_NULL), AL_MPU_ERR_ILLEGAL_PARAM);

    RetValue = AlOsal_Lock_Take(&Handle->Lock, 0);
    if (RetValue != AL_OK) {
        return RetValue;
    }

    RetValue = AlMpu_Dev_SetRegionEnableStatus(Handle->Dev, RegionNumber, MPU_REGION_DISABLE);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return RetValue;
}

/**
 *
 * This function configures a specific MPU region based on the provided configuration structure. It takes a lock,
 * applies the configuration to the specified region, and then releases the lock. It returns an error code if any step fails.
 *
 * @param Handle Pointer to the MPU HAL structure.
 * @param RegionNumber The number of the region to configure.
 * @param InitRegionConfig Pointer to the configuration structure for the region.
 * @return AL_OK on success, an error code otherwise.
 */
AL_S32 AlMpu_Hal_ConfigRegionByRegionNum(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber,
                                         AL_MPU_RegionConfigStruct *InitRegionConfig)
{
    AL_S32 RetValue;

    AL_ASSERT((Handle != AL_NULL), AL_MPU_ERR_ILLEGAL_PARAM);

    RetValue = AlOsal_Lock_Take(&Handle->Lock, 0);
    if (RetValue != AL_OK) {
        return RetValue;
    }

    RetValue = AlMpu_Dev_ConfigRegionByRegionNum(Handle->Dev, RegionNumber, InitRegionConfig);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return RetValue;
}

/**

 *
 * This function applies a configuration to an MPU region. It takes a lock, applies the provided configuration,
 * and then releases the lock. It returns an error code if any step fails.
 *
 * @param Handle Pointer to the MPU HAL structure.
 * @param InitRegionConfig Pointer to the configuration structure for the region.
 * @return AL_OK on success, an error code otherwise.
 */
AL_S32 AlMpu_Hal_ConfigRegion(AL_MPU_HalStruct *Handle, AL_MPU_RegionConfigStruct *InitRegionConfig)
{
    AL_S32 RetValue;

    AL_ASSERT((Handle != AL_NULL), AL_MPU_ERR_ILLEGAL_PARAM);

    RetValue = AlOsal_Lock_Take(&Handle->Lock, 0);
    if (RetValue != AL_OK) {
        return RetValue;
    }

    RetValue = AlMpu_Dev_ConfigRegion(Handle->Dev, InitRegionConfig);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return RetValue;
}

/**
 *
 * This function initializes the MPU device with a specified configuration. It looks up the hardware configuration based on the device ID,
 * registers an event callback, and initializes the device with the provided configuration. It returns an error code if any step fails.
 *
 * @param MpuDevId The device ID of the MPU.
 * @param Handle Pointer to a pointer to the MPU HAL structure to be initialized.
 * @param EventCallBack The event callback function to be registered.
 * @param InitRegionConfig Pointer to the initial region configuration structure.
 * @param ConfigNumber The number of configurations to apply.
 * @return AL_OK on success, an error code otherwise.
 */
AL_S32 AlMpu_Hal_ConfigInit(AL_U8 MpuDevId, AL_MPU_HalStruct **Handle, AL_Mpu_EventCallBack EventCallBack,
                            AL_MPU_RegionConfigStruct *InitRegionConfig, AL_U8 ConfigNumber)
{
    AL_S32 RetValue = AL_OK;
    AL_MPU_HwConfigStruct *HwConfig = AL_NULL;
    AL_Mpu_EventCallBack CallBack;
    AL_S32 ConfigCount;

    AL_ASSERT((Handle != AL_NULL), AL_MPU_ERR_ILLEGAL_PARAM);

    HwConfig = AlMpu_Dev_LookupConfigByDevId(MpuDevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlMpuHandle[MpuDevId];
        (*Handle)->Dev = &AL_MPU_DevInstance[MpuDevId];
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlMpu_Dev_LookupConfigByDevId invalid device id\r\n");
        return AL_MPU_ERR_INVALID_DEVICE_ID;
    }

    CallBack = (EventCallBack == AL_NULL) ? AlMpu_Hal_EventCallBack : EventCallBack;
    RetValue = AlMpu_Dev_RegisterEventCallBack((*Handle)->Dev, CallBack, (void *)*Handle);
    if (RetValue != AL_OK) {
        return RetValue;
    }

    ConfigCount = AlMpu_Dev_Init((*Handle)->Dev, HwConfig, InitRegionConfig, ConfigNumber);
    if (ConfigCount == 0) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlMpu_Dev_Init initialize failed\r\n");
        RetValue = AL_MPU_ERROR_INITIALIZE_FAILED;
    } else if (ConfigCount < ConfigNumber) {
        AL_LOG(AL_LOG_LEVEL_WARNING, "Some configurations failed, and only %d configurations "
                                     "were correctly configured\r\n", ConfigCount);
        RetValue = AL_OK;
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "AlMpu_Dev_Init initialize success\r\n");
        RetValue = AL_OK;
    }

    RetValue = AlOsal_Lock_Init(&(*Handle)->Lock, "Mpu-Lock");
    if (RetValue != AL_OK) {
        return RetValue;
    }

    return RetValue;
}

/**
 *
 * This function initializes the MPU HAL layer by registering the MPU interrupt handler. It does not return any value and does not take any parameters.
 *
 * @return None
 */
AL_VOID AlMpu_Hal_Init()
{
    AlMpu_Hal_MpuRegisterIntr();
}
