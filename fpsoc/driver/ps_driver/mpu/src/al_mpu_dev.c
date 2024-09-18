/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_mpu_dev.h"
#include "al_errno.h"
#include "al_intr.h"

#include <string.h>


static AL_MPU_CommonMpuRegionStatusStruct CommonMpuRegionStatus[AL_MPU_NUM_COMMON_INSTANCE] = {
    {
        .DevId           = 0,
        {0}
    },

    {
        .DevId           = 1,
        {0}
    },

    {
        .DevId           = 2,
        {0}
    },

    {
        .DevId           = 3,
        {0}
    },

    {
        .DevId           = 4,
        {0}
    },

    {
        .DevId           = 5,
        {0}
    },
};

static AL_MPU_ApuRegionStatusStruct ApuMpuRegionStatus = {
    .DevId           = 6,
    {0},
};

extern AL_MPU_HwConfigStruct AlMpu_HwConfig[AL_MPU_NUM_INSTANCE];

/**
 * This function looks up the hardware configuration for a given device ID.
 *
 * @param DevId The device ID for which the hardware configuration is to be looked up.
 * @return A pointer to the hardware configuration structure if found, otherwise NULL.
 */
AL_MPU_HwConfigStruct *AlMpu_Dev_LookupConfigByDevId(AL_U8 DevId)
{
    AL_U32 Index;
    AL_MPU_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_MPU_NUM_INSTANCE; Index++) {
        if (AlMpu_HwConfig[Index].DeviceId == DevId) {
            ConfigPtr = &AlMpu_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

/**
 * This function gets the first available region number for a given device ID.
 *
 * @param DevId The device ID for which the available region number is requested.
 * @return The first available region number if found, otherwise AL_MPU_INVALID_REGION_NUMBER.
 */
static AL_U8 AlMpu_Dev_GetAvailableRegionByDevId(AL_U8 DevId)
{
    AL_U32 MpuNumIndex;
    AL_U32 MpuRegionIndex;

    if (DevId == ApuMpuRegionStatus.DevId) {
        for (MpuRegionIndex = 0; MpuRegionIndex <= AL_MPU_APU_MAX_REGION_NUMBER; MpuRegionIndex++) {
            if (ApuMpuRegionStatus.RegionEnableStatus[MpuRegionIndex] == MPU_REGION_DISABLE) {
                return MpuRegionIndex;
            }
        }
    } else {
        for (MpuNumIndex = 0; MpuNumIndex < AL_MPU_NUM_COMMON_INSTANCE; MpuNumIndex++) {
            if (CommonMpuRegionStatus[MpuNumIndex].DevId == DevId) {
                for (MpuRegionIndex = 0; MpuRegionIndex <= AL_MPU_COMMON_MAX_REGION_NUMBER; MpuRegionIndex++) {
                    if (CommonMpuRegionStatus[MpuNumIndex].RegionEnableStatus[MpuRegionIndex]
                                                                                        == MPU_REGION_DISABLE) {
                        return MpuRegionIndex;
                    }
                }
            }
        }
    }

    return AL_MPU_INVALID_REGION_NUMBER;
}

/**
 * This function updates the enable status of a region for a given device ID.
 *
 * @param DevId The device ID for which the region enable status is to be updated.
 * @param RegionNumber The region number whose enable status is to be updated.
 * @param EnableStatus The new enable status for the region.
 * @return AL_OK on success, otherwise an error code.
 */
static AL_S32 AlMpu_Dev_UpdateRegionEnableStatus(AL_U8 DevId, AL_U8 RegionNumber, AL_MPU_RegionEnEnum EnableStatus)
{
    AL_U32 MpuNumIndex;
    AL_U32 MpuRegionIndex = RegionNumber;

    if (DevId == ApuMpuRegionStatus.DevId) {
        ApuMpuRegionStatus.RegionEnableStatus[MpuRegionIndex] = EnableStatus;
    } else {
        for (MpuNumIndex = 0; MpuNumIndex < AL_MPU_NUM_COMMON_INSTANCE; MpuNumIndex++) {
            if (CommonMpuRegionStatus[MpuNumIndex].DevId == DevId) {
                CommonMpuRegionStatus[MpuNumIndex].RegionEnableStatus[MpuRegionIndex] = EnableStatus;
            }
        }
    }

    return AL_OK;
}

/**
 * This function sets the attributes for a memory protection unit (MPU) region.
 *
 * @param RegionBaseAddr The base address of the region.
 * @param Config Pointer to the region configuration structure.
 * @return None.
 */
static AL_VOID AlMpu_Dev_SetRegionAttr(AL_REG RegionBaseAddr, AL_MPU_RegionConfigStruct *Config)
{
    AlMpu_ll_SetRegionAttrSecure(RegionBaseAddr, Config->Secure);
    AlMpu_ll_SetRegionAttrPrivilege(RegionBaseAddr, Config->Privilege);
    AlMpu_ll_SetRegionAttrRw(RegionBaseAddr, Config->ReadWrite);
    AlMpu_ll_SetRegionAttrIntrEn(RegionBaseAddr, Config->InterruptEnable);
    AlMpu_ll_SetRegionAttrEnable(RegionBaseAddr, MPU_REGION_ENABLE);
}

/**
 * This function configures a region of the memory protection unit (MPU).
 *
 * @param RegionBaseAddr The base address of the region to configure.
 * @param Config Pointer to the configuration structure for the region.
 * @return AL_OK on success, otherwise an error code indicating the type of failure.
 */
static AL_S32 AlMpu_Dev_SetRegion(AL_REG RegionBaseAddr, AL_MPU_RegionConfigStruct *Config)
{
    AL_U32 StartAddr;
    AL_U32 EndAddr;

    /* Check the configure parameter */
    if (!(AL_MPU_IS_VALID_REGION_SECURE(Config->Secure))) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }
    if (!(AL_MPU_IS_VALID_REGION_PRIVILEGE(Config->Privilege))) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }
    if (!(AL_MPU_IS_VALID_READ_WRITE(Config->ReadWrite))) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }
    if (!(AL_MPU_IS_VALID_REGION_INTR_EN(Config->InterruptEnable))) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    if (((Config->StartAddr != 0) && ((Config->StartAddr) & AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE_MASK)) ||
        ((Config->Size) == 0) || ((Config->Size) & AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE_MASK)) {
            AL_LOG(AL_LOG_LEVEL_WARNING, "The protected address and length must be a multiple of 4K! "
                   "and the current configuration is ignored!\r\n");
            return AL_MPU_ERR_ILLEGAL_PARAM;
        }

    StartAddr = (Config->StartAddr) >> 12;
    EndAddr = ((Config->StartAddr + Config->Size) >> 12 ) - 1;

    AlMpu_ll_SetRegionAddr(RegionBaseAddr, StartAddr, EndAddr);
    AlMpu_Dev_SetRegionAttr(RegionBaseAddr, Config);

    AlMpu_ll_SetRegionGroupId(RegionBaseAddr, Config->GroupId);

    return AL_OK;
}

/**
 * This function configures a region of the MPU by region number.
 *
 * @param Mpu Pointer to the MPU device structure.
 * @param RegionNumber The number of the region to configure.
 * @param RegionConfig Pointer to the configuration structure for the region.
 * @return AL_OK on success, otherwise an error code.
 */
AL_S32 AlMpu_Dev_ConfigRegionByRegionNum(AL_MPU_DevStruct *Mpu, AL_U8 RegionNumber,
                                         AL_MPU_RegionConfigStruct *RegionConfig)
{
    AL_S32 RetValue;
    AL_U32 DevId;
    AL_REG MpuBaseAddr;
    AL_REG RegionBaseAddr;
    AL_U8 RegionEnableStatus;

    AL_ASSERT((Mpu != AL_NULL) && (RegionConfig != AL_NULL), AL_MPU_ERR_ILLEGAL_PARAM);

    DevId = Mpu->HwConfig.DeviceId;
    MpuBaseAddr = (AL_REG)(Mpu->HwConfig.BaseAddress);

    /* Apu mpu */
    if (DevId == ApuMpuRegionStatus.DevId) {
        if (RegionNumber > AL_MPU_APU_MAX_REGION_NUMBER) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }

        RegionEnableStatus = ApuMpuRegionStatus.RegionEnableStatus[RegionNumber];
    } else {
        if (RegionNumber > AL_MPU_COMMON_MAX_REGION_NUMBER) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }

        RegionEnableStatus = CommonMpuRegionStatus[DevId].RegionEnableStatus[RegionNumber];
    }
    if (RegionEnableStatus == MPU_REGION_ENABLE) {
        AL_LOG(AL_LOG_LEVEL_NOTICE, "Region already enabled\r\n");
        return AL_MPU_ERROR_REGION_ENABLED;
    }

    /* Get region base address */
    RegionBaseAddr = MPU_REGION_BASE_ADDR(MpuBaseAddr, RegionNumber);

    if ((RetValue = AlMpu_Dev_SetRegion(RegionBaseAddr, RegionConfig)) != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_WARNING, "AlMpu_Dev_SetRegion failed, "
               "and the current configuration is ignored!\r\n");
        return RetValue;
    }

    RegionConfig->ConfigRegionNumber = RegionNumber;

    /* Update region enable status */
    AlMpu_Dev_UpdateRegionEnableStatus(Mpu->HwConfig.DeviceId, RegionNumber, MPU_REGION_ENABLE);

    return AL_OK;
}

/**
 * This function initializes the MPU device with the given hardware configuration and initial region configurations.
 *
 * @param Mpu Pointer to the MPU device structure to be initialized.
 * @param HwConfig Pointer to the hardware configuration structure.
 * @param InitRegionConfig Pointer to an array of initial region configuration structures.
 * @param ConfigNumber The number of initial region configurations provided.
 * @return The number of regions successfully configured.
 */
AL_S32 AlMpu_Dev_Init(AL_MPU_DevStruct *Mpu, AL_MPU_HwConfigStruct *HwConfig,
                      AL_MPU_RegionConfigStruct *InitRegionConfig, AL_U8 ConfigNumber)
{
    AL_S32 RetValue;
    AL_U8 RegionNumber;
    AL_U8 RegionCount;
    AL_U8 ConfigCount = 0;

    AL_ASSERT((Mpu != AL_NULL) && (HwConfig != AL_NULL) && (InitRegionConfig != AL_NULL), ConfigCount);

    Mpu->HwConfig = *HwConfig;

    AlMpu_Dev_MpuDisable(Mpu);

    /* Config all the regions */
    for (RegionCount = 0; RegionCount < ConfigNumber; RegionCount++) {

        /* Get a available region */
        RegionNumber = AlMpu_Dev_GetAvailableRegionByDevId(Mpu->HwConfig.DeviceId);
        if (RegionNumber == AL_MPU_INVALID_REGION_NUMBER) {
            AL_LOG(AL_LOG_LEVEL_WARNING, "AlMpu_Dev_GetAvailableRegionByDevId failed\r\n");
            break;
        }

        RetValue = AlMpu_Dev_ConfigRegionByRegionNum(Mpu, RegionNumber, &(InitRegionConfig[RegionCount]));
        if (RetValue != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_WARNING, "AlMpu_Dev_ConfigRegionByRegionNum failed, "
                   "and the current configuration is ignored!\r\n");
            continue;
        }

        InitRegionConfig[RegionCount].ConfigRegionNumber = RegionNumber;
        ConfigCount++;
    }

    AlMpu_Dev_MpuEnable(Mpu);

    return ConfigCount;
}

/**
 * This function configures a region of the MPU.
 *
 * @param Mpu Pointer to the MPU device structure.
 * @param RegionConfig Pointer to the configuration structure for the region.
 * @return AL_OK on success, otherwise an error code.
 */
AL_S32 AlMpu_Dev_ConfigRegion(AL_MPU_DevStruct *Mpu, AL_MPU_RegionConfigStruct *RegionConfig)
{
    AL_S32 RetValue;
    AL_U8 RegionNumber;

    AL_ASSERT((Mpu != AL_NULL) && (RegionConfig != AL_NULL), AL_MPU_ERR_ILLEGAL_PARAM);

    /* Get a available region */
    if ((RegionNumber = AlMpu_Dev_GetAvailableRegionByDevId(Mpu->HwConfig.DeviceId)) == AL_MPU_INVALID_REGION_NUMBER) {
        return AL_MPU_ERROR_NO_AVAILABLE_REGION;
    }

    if ((RetValue = AlMpu_Dev_ConfigRegionByRegionNum(Mpu, RegionNumber, RegionConfig)) != AL_OK) {
        return RetValue;
    }

    RegionConfig->ConfigRegionNumber = RegionNumber;

    return AL_OK;
}

/**
 * This function sets the enable status of a specific region in the MPU.
 *
 * @param Mpu Pointer to the MPU device structure.
 * @param RegionNumber The number of the region whose enable status is to be set.
 * @param EnableStatus The new enable status for the region.
 * @return AL_OK on success, otherwise an error code.
 */
AL_S32 AlMpu_Dev_SetRegionEnableStatus(AL_MPU_DevStruct *Mpu, AL_U8 RegionNumber, AL_MPU_RegionEnEnum EnableStatus)
{
    AL_U32 DevId;
    AL_REG MpuBaseAddr;
    AL_REG RegionBaseAddr;

    AL_ASSERT((Mpu != AL_NULL), AL_MPU_ERR_ILLEGAL_PARAM);

    DevId = Mpu->HwConfig.DeviceId;
    MpuBaseAddr = (AL_REG)(Mpu->HwConfig.BaseAddress);

    if (DevId == ApuMpuRegionStatus.DevId) {
        if (RegionNumber > AL_MPU_APU_MAX_REGION_NUMBER) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }
    } else {
        if (RegionNumber > AL_MPU_COMMON_MAX_REGION_NUMBER) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }
    }

    RegionBaseAddr = MPU_REGION_BASE_ADDR(MpuBaseAddr, RegionNumber);
    AlMpu_ll_SetRegionAttrEnable(RegionBaseAddr, EnableStatus);

    AlMpu_Dev_UpdateRegionEnableStatus(Mpu->HwConfig.DeviceId, RegionNumber, EnableStatus);

    return AL_OK;
}

/**
 * This function enables the MPU.
 *
 * @param Mpu Pointer to the MPU device structure.
 * @return AL_OK on success, otherwise an error code.
 */
AL_S32 AlMpu_Dev_MpuEnable(AL_MPU_DevStruct *Mpu)
{
    AL_REG MpuBaseAddr;

    AL_ASSERT(Mpu != AL_NULL, AL_MPU_ERR_ILLEGAL_PARAM);

    MpuBaseAddr = (AL_REG)Mpu->HwConfig.BaseAddress;

    AlMpu_ll_MpuEnable(MpuBaseAddr);

    return AL_OK;
}

/**
 *
 * This function disables the MPU by calling a low-level disable function with the base address of the MPU.
 * It asserts that the MPU pointer is not NULL before proceeding.
 *
 * @param Mpu Pointer to the MPU device structure.
 * @return AL_OK on success, otherwise AL_MPU_ERR_ILLEGAL_PARAM if Mpu is NULL.
 */
AL_S32 AlMpu_Dev_MpuDisable(AL_MPU_DevStruct *Mpu)
{
    AL_REG MpuBaseAddr;

    AL_ASSERT(Mpu != AL_NULL, AL_MPU_ERR_ILLEGAL_PARAM);

    MpuBaseAddr = (AL_REG)Mpu->HwConfig.BaseAddress;

    AlMpu_ll_MpuDisable(MpuBaseAddr);

    return AL_OK;
}

/**
 *
 * This function is called when an MPU event occurs. It checks if an event callback is registered.
 * If so, it constructs an event structure with the event ID and data (region number) and calls the callback.
 *
 * @param Mpu Pointer to the MPU device structure.
 * @param RegionNumber The region number associated with the event.
 * @return None.
 */
static AL_VOID AlMpu_Dev_EventHandler(AL_MPU_DevStruct *Mpu, AL_U8 RegionNumber)
{
    if (Mpu->EventCallBack) {
        AL_MPU_EventStruct event = {
            .EventId    = AL_MPU_DENY_ACCESS,
            .EventData  = RegionNumber
        };

        (*Mpu->EventCallBack)(&event, Mpu->EventCallBackRef);
    }

}

/**
 *
 * This function reads the interrupt status and determines the region number that caused the interrupt.
 * It iterates through the bits of the interrupt status until it finds the set bit corresponding to the region number.
 *
 * @param MpuBaseAddr The base address of the MPU.
 * @return The region number that caused the interrupt.
 */
static AL_U8 AlMpu_Dev_GetIntrRegionNumber(AL_REG MpuBaseAddr)
{
    AL_U32 IntrRegion;
    AL_U8 RegionNumber = 0;

    IntrRegion = AlMpu_ll_GetIntrRegionNumber(MpuBaseAddr);

    while (!(IntrRegion & (BIT(RegionNumber)))) {
        RegionNumber++;
    }

    return RegionNumber;
}

/**
 *
 * This function is the interrupt handler for MPU interrupts. It iterates through all possible MPU instances,
 * checks if an interrupt has occurred for each, and then handles the interrupt by clearing it and calling the event handler.
 *
 * @param Instance Pointer to the array of MPU device structures.
 * @return None.
 */
AL_VOID AlMpu_Dev_MpuIntrHandler(void *Instance)
{
    AL_U32 MpuIntrState;
    AL_U32 MpuBaseAddr;
    AL_U8 IntrRegionNumber;
    AL_REG RegionBaseAddr;
    AL_MPU_DevStruct *Mpu;

    AL_U32 Index = 0;

    /* Get MPU interrupt state id */
    MpuIntrState = AlMpu_ll_GetMpuIntrState();

    /* The loop handles all mpu interrupts */
    while ((Index < AL_MPU_NUM_INSTANCE)) {

        if (!(MpuIntrState & BIT(Index))) {
            Index++;
            continue;
        }

        Mpu = &((AL_MPU_DevStruct *)Instance)[Index];
        MpuBaseAddr = Mpu->HwConfig.BaseAddress;

        /* Get intrrupt region number and region base addrress */
        IntrRegionNumber = AlMpu_Dev_GetIntrRegionNumber(MpuBaseAddr);
        RegionBaseAddr = MPU_REGION_BASE_ADDR(MpuBaseAddr, IntrRegionNumber);

        /* Clear the interrupt */
        AlMpu_ll_ClrRegionIntr(RegionBaseAddr);

        AlMpu_Dev_EventHandler(Mpu, IntrRegionNumber);

        Index++;
    }
}

/**
 *
 * This function allows registering a callback function that will be called when an MPU event occurs.
 * It asserts that both the MPU device structure and the callback function are not NULL.
 * If a callback is already registered, it will be replaced with the new one.
 *
 * @param Mpu Pointer to the MPU device structure.
 * @param Callback The callback function to register.
 * @param CallbackRef A user-defined reference that will be passed to the callback function.
 * @return AL_OK on success, otherwise AL_MPU_ERR_ILLEGAL_PARAM if either Mpu or Callback is NULL.
 */
AL_S32 AlMpu_Dev_RegisterEventCallBack(AL_MPU_DevStruct *Mpu, AL_Mpu_EventCallBack Callback, void *CallbackRef)
{
    AL_ASSERT((Mpu != AL_NULL) && (Callback != AL_NULL), AL_MPU_ERR_ILLEGAL_PARAM);

    if (Mpu->EventCallBack != AL_NULL) {

#ifdef MPU_DEBUG
        AL_LOG(AL_LOG_LEVEL_WARNING, "mpu=%p duplicate register callback: replace old:%p with New: %p\n", \
                Mpu, Mpu->EventCallBack, Callback);
#endif
    }

    Mpu->EventCallBack        = Callback;
    Mpu->EventCallBackRef     = CallbackRef;

    return AL_OK;
}
