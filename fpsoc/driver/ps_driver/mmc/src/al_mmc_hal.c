/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include "al_mmc_hal.h"

static AL_MMC_HalStruct AL_MMC_HalInstance[AL_MMC_NUM_INSTANCE];

/**
 * This function is intr handler call back function
 * @param   Event is pointer to AL_MMC_EventStruct
 * @param   CallBackRef
 * @return
*/
static AL_VOID AlMmc_Hal_DefEventCallBack(AL_MMC_EventStruct *Event, AL_VOID *CallBackRef)
{

}

/**
 * This function configures the MMC device with the specified initialization parameters,
 * registers a callback for MMC events, and sets up interrupt handling for the device.
 * It looks up the hardware configuration based on the device ID, initializes the MMC device,
 * and registers the event callback. If no callback is provided, a default callback is registered.
 * Finally, it registers the interrupt handler for the device.
 *
 * @param Handle Double pointer to the MMC HAL structure, which will be initialized by this function.
 * @param DevId Device ID for the MMC device. Used to look up hardware configuration.
 * @param InitConfig Pointer to the initialization configuration structure.
 * @param CallBack Event callback function to be registered.
 * @return Returns AL_OK on success, AL_MMC_ERR_NULL_PTR if the handle is NULL,
 *         AL_MMC_ERR_INVALID_DEVID if the device ID is invalid, or other error codes as defined.
 */
AL_S32 AlMmc_Hal_Init(AL_MMC_HalStruct **Handle, AL_U32 DevId, AL_MMC_InitStruct *InitConfig,
                      AL_MMC_EventCallBack CallBack)
{
    AL_S32 Ret = AL_OK;
    AL_MMC_DevStruct *Dev = AL_NULL;
    AL_MMC_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT(Handle != AL_NULL, AL_MMC_ERR_NULL_PTR);

    /* 1. look up hardware config */
    HwConfig = AlMmc_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AL_MMC_HalInstance[DevId];
        Dev = &((*Handle)->Dev);
    } else {
        return AL_MMC_ERR_INVALID_DEVID;
    }

    /* 2. Init IP, do not use intr */
    Ret = AlMmc_Dev_Init(Dev, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    /* 3. register callback */
    if (CallBack == AL_NULL) {
        AlMmc_Dev_RegisterEventCallBack(Dev, AlMmc_Hal_DefEventCallBack, *Handle);
    } else {
        AlMmc_Dev_RegisterEventCallBack(Dev, CallBack, *Handle);
    }
    AL_ASSERT(Ret == AL_OK, Ret);

    /* 4. register intr */
    AlIntr_RegHandler(HwConfig->IntrId, AL_NULL, AlMmc_Dev_IntrHandler, Dev);

    return Ret;
}

/**
 * This function writes a specified number of blocks to the MMC device starting at a specified block offset.
 * It waits until the write operation is complete or a timeout occurs.
 *
 * @param Handle Pointer to the MMC HAL structure.
 * @param Buf Pointer to the buffer containing the data to be written.
 * @param BlkOffset Block offset at which to start writing.
 * @param BlkCnt Number of blocks to write.
 * @param Timeout Timeout for the write operation in milliseconds. Currently unused.
 * @return Returns AL_OK on success or an error code on failure.
 */
AL_S32 AlMmc_Hal_WriteBlocked(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt, AL_U32 Timeout)
{
    AL_UNUSED(Timeout);
    AL_ASSERT(Handle != AL_NULL, AL_MMC_ERR_NULL_PTR);

    AL_S32 Ret = AL_OK;

    Ret = AlMmc_Dev_Write(&(Handle->Dev), Buf, BlkOffset, BlkCnt);
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}

/**
 * This function reads a specified number of blocks from the MMC device starting at a specified block offset.
 * It waits until the read operation is complete or a timeout occurs.
 *
 * @param Handle Pointer to the MMC HAL structure.
 * @param Buf Pointer to the buffer where the read data will be stored.
 * @param BlkOffset Block offset at which to start reading.
 * @param BlkCnt Number of blocks to read.
 * @param Timeout Timeout for the read operation in milliseconds. Currently unused.
 * @return Returns AL_OK on success or an error code on failure.
 */
AL_S32 AlMmc_Hal_ReadBlocked(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt, AL_U32 Timeout)
{
    AL_UNUSED(Timeout);
    AL_ASSERT(Handle != AL_NULL, AL_MMC_ERR_NULL_PTR);

    AL_S32 Ret = AL_OK;

    Ret = AlMmc_Dev_Read(&(Handle->Dev), Buf, BlkOffset, BlkCnt);
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}