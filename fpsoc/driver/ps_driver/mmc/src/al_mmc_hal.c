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
 * @note
*/
static AL_VOID AlMmc_Hal_DefEventCallBack(AL_MMC_EventStruct *Event, AL_VOID *CallBackRef)
{

}

/**
 * This function init MMC module
 * @param   Handle is pointer to AL_MMC_HalStruct
 * @param   InitConfig is module config structure with AL_MMC_InitStruct
 * @param   CallBack is call back struct with AL_MMC_EventCallBack
 * @param   DevId is hardware module id
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlMmc_Hal_Init(AL_MMC_HalStruct **Handle, AL_U32 DevId, AL_MMC_InitStruct *InitConfig,
                      AL_MMC_EventCallBack *CallBack)
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

AL_S32 AlMmc_Hal_WriteBlocked(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt, AL_U32 Timeout)
{
    AL_UNUSED(Timeout);

    AL_S32 Ret = AL_OK;

    if ((Handle == AL_NULL) || (Buf == AL_NULL)) {
        return AL_MMC_ERR_NULL_PTR;
    }

    Ret = AlMmc_Dev_Write(&(Handle->Dev), Buf, BlkOffset, BlkCnt);
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}

AL_S32 AlMmc_Hal_ReadBlocked(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt, AL_U32 Timeout)
{
    AL_UNUSED(Timeout);

    AL_S32 Ret = AL_OK;

    if ((Handle == AL_NULL) || (Buf == AL_NULL)) {
        return AL_MMC_ERR_NULL_PTR;
    }

    Ret = AlMmc_Dev_Read(&(Handle->Dev), Buf, BlkOffset, BlkCnt);
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}