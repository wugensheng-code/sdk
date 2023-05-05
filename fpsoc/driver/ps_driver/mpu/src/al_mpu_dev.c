#include "al_mpu_dev.h"
#include "al_errno.h"

#include "nuclei_sdk_soc.h"
#include "gic_v3.h"

#include <string.h>

AL_MPU_DevStruct AL_MPU_DevInstance[AL_MPU_NUM_INSTANCE];

static AL_MPU_CommonMpuRegionStatusStruct CommonMpuRegionStatusStruct[AL_MPU_NUM_COMMON_INSTANCE] = {
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

static AL_MPU_ApuRegionStatusStruct ApuMpuRegionStatusStruct = {
    .DevId           = 6,
    {0},
};

extern AL_MPU_HwConfigStruct AlMpu_HwConfig[AL_MPU_NUM_INSTANCE];

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

static AL_MPU_HwConfigStruct *AlMpu_Dev_LookupConfigByIntrStateId(AL_U8 MpuIntrStateId)
{
    AL_U32 Index;
    AL_MPU_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_MPU_NUM_INSTANCE; Index++) {
        if (AlMpu_HwConfig[Index].IntrStateId == MpuIntrStateId) {
            ConfigPtr = &AlMpu_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

static AL_S32 AlMpu_Dev_UpdateRegionEnableStatus(AL_U8 DevId, AL_U8 RegionNumber, AL_MPU_RegionEnEnum EnableStatus)
{
    AL_U32 MpuNumIndex;
    AL_U32 MpuRegionIndex = RegionNumber - 1;

    if (DevId == ApuMpuRegionStatusStruct.DevId) {
        ApuMpuRegionStatusStruct.AL_RegionEnableStatus[MpuRegionIndex] = EnableStatus;
    } else {
        for (MpuNumIndex = 0; MpuNumIndex < AL_MPU_NUM_COMMON_INSTANCE; MpuNumIndex++) {
            if (CommonMpuRegionStatusStruct[MpuNumIndex].DevId == DevId) {
                CommonMpuRegionStatusStruct[MpuNumIndex].AL_RegionEnableStatus[MpuRegionIndex] = EnableStatus;
            }
        }
    }

    return AL_OK;
}

static AL_VOID AlMpu_Dev_SetRegionAttr(AL_REG RegionBaseAddr, AL_MPU_RegionConfigStruct *RegionConfig)
{
    AlMpu_ll_SetRegionAttrSecure(RegionBaseAddr, RegionConfig->Secure);
    AlMpu_ll_SetRegionAttrPrivilege(RegionBaseAddr, RegionConfig->Privilege);
    AlMpu_ll_SetRegionAttrRw(RegionBaseAddr, RegionConfig->ReadWrite);
    AlMpu_ll_SetRegionAttrIntrEn(RegionBaseAddr, RegionConfig->InterruptEnable);
    AlMpu_ll_SetRegionAttrEnable(RegionBaseAddr, MPU_REGION_ENABLE);
}

/**
 * This function configure the mpu region
 * @param RegionBaseAddr region base address
 * @param Config Configured parameter
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
static AL_S32 AlMpu_Dev_ConfigRegion(AL_REG RegionBaseAddr, AL_MPU_RegionConfigStruct *Config)
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

    if (((Config->StartAddr) % AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE) ||
        ((Config->Size) % AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE)      ||
        ((Config->Size) < AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE)) {
            AL_LOG(AL_ERR_LEVEL_WARNING, "[WARNING], The protected address and length must be a multiple of 4K! "
                   "and the current configuration is ignored!\r\n");
            return AL_MPU_ERR_ILLEGAL_PARAM;
        }

    StartAddr = (Config->StartAddr) >> 12;
    EndAddr = Config->StartAddr + Config->Size;
    EndAddr = (EndAddr >> 12) - 1;

    AlMpu_ll_SetRegionAddr(RegionBaseAddr, StartAddr, EndAddr);
    AlMpu_Dev_SetRegionAttr(RegionBaseAddr, Config);

    AlMpu_ll_SetRegionGroupId(RegionBaseAddr, Config->GroupId);

    return AL_OK;
}

static AL_U8 AlMpu_Dev_GetValidRegionByDevId(AL_U8 DevId)
{
    AL_U32 MpuNumIndex;
    AL_U32 MpuRegionIndex = 0;

    if (DevId == ApuMpuRegionStatusStruct.DevId) {
        for (MpuRegionIndex = 0; MpuRegionIndex < AL_MPU_APU_MAX_REGION; MpuRegionIndex++) {
            if (ApuMpuRegionStatusStruct.AL_RegionEnableStatus[MpuRegionIndex] == MPU_REGION_DISABLE) {
                return (MpuRegionIndex + 1);
            }
        }
    } else {
        for (MpuNumIndex = 0; MpuNumIndex < AL_MPU_NUM_COMMON_INSTANCE; MpuNumIndex++) {
            if (CommonMpuRegionStatusStruct[MpuNumIndex].DevId == DevId) {
                for (MpuRegionIndex = 0; MpuRegionIndex < AL_MPU_COMMON_MAX_REGION; MpuRegionIndex++) {
                    if (CommonMpuRegionStatusStruct[MpuNumIndex].AL_RegionEnableStatus[MpuRegionIndex] 
                                                                                        == MPU_REGION_DISABLE) {
                        return (MpuRegionIndex + 1);
                    }
                }
            }
        }
    }

    return 0;
}

AL_S32 AlMpu_Dev_Init(AL_MPU_DevStruct *Mpu, AL_MPU_HwConfigStruct *HwConfig,
                      AL_MPU_RegionConfigStruct *InitRegionConfig, AL_U8 ConfigNumber)
{
    AL_S32 RetValue;
    AL_U8 RegionNumber;
    AL_REG MpuInstance;
    AL_REG RegionBaseAddr;
    AL_U8 RegionCount;

    if (Mpu == AL_NULL) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    Mpu->RegionConfig  = InitRegionConfig;
    Mpu->ConfigNumber  = ConfigNumber;
    memcpy(&(Mpu->HwConfig), HwConfig, sizeof(AL_MPU_HwConfigStruct));

    MpuInstance = Mpu->HwConfig.BaseAddress;

    AlMpu_Dev_MpuDisable(Mpu);

    /* Config all the regions */
    for (RegionCount = 0; RegionCount < ConfigNumber; RegionCount++) {

        /* Get a valid region */
        if ((RegionNumber = AlMpu_Dev_GetValidRegionByDevId(Mpu->HwConfig.DeviceId)) == 0) {
            break;
        }

        /* Get region base address */
        RegionBaseAddr = MPU_REGION_BASE_ADDR(MpuInstance, RegionNumber);

        /* Config region register */
        if ((RetValue = AlMpu_Dev_ConfigRegion(RegionBaseAddr, &(Mpu->RegionConfig[RegionCount]))) != AL_OK) {
            AL_LOG(AL_ERR_LEVEL_WARNING, "[WARNING], AlMpu_Dev_ConfigRegion failed, "
                   "and the current configuration is ignored!\r\n");
            continue;
        }

        /* Update region enable status */
        AlMpu_Dev_UpdateRegionEnableStatus(Mpu->HwConfig.DeviceId, RegionNumber, MPU_REGION_ENABLE);
    }

    AlMpu_Dev_MpuEnable(Mpu);

    return AL_OK;
}

/**
 * This function enable the mpu region
 * @param DevId mpu device id
 * @param RegionNumber the region number
 * @param InitRegionConfig Configured parameter
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Dev_EnableRegion(AL_MPU_DevStruct *Mpu, AL_U8 RegionNumber, AL_MPU_RegionConfigStruct *InitRegionConfig)
{
    AL_REG Instance;
    AL_REG RegionBaseAddr;
    AL_U8 RegionEnableStatus = 0;
    AL_U32 DevId;

    DevId = Mpu->HwConfig.DeviceId;
    Instance = (AL_REG)Mpu->HwConfig.BaseAddress;

    /* Apu mpu */
    if (DevId == ApuMpuRegionStatusStruct.DevId) {
        if (RegionNumber == 0 || RegionNumber > AL_MPU_APU_MAX_REGION) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }

        RegionEnableStatus = ApuMpuRegionStatusStruct.AL_RegionEnableStatus[RegionNumber];
    } else {
        if (RegionNumber == 0 || RegionNumber > AL_MPU_COMMON_MAX_REGION) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }

        RegionEnableStatus = CommonMpuRegionStatusStruct[DevId].AL_RegionEnableStatus[RegionNumber];
    }
    if (RegionEnableStatus == MPU_REGION_ENABLE) {
        AL_LOG(AL_ERR_LEVEL_NOTICE, "Region already enabled\r\n");
        return AL_MPU_ERROR_REGION_ENABLED;
    }

    RegionBaseAddr = MPU_REGION_BASE_ADDR(Instance, RegionNumber);
    AlMpu_ll_SetRegionAttrEnable(RegionBaseAddr, MPU_REGION_ENABLE);

    /* Config region register */
    AlMpu_Dev_ConfigRegion(RegionBaseAddr, InitRegionConfig);

    AlMpu_Dev_UpdateRegionEnableStatus(Mpu->HwConfig.DeviceId, RegionNumber, MPU_REGION_ENABLE);

    return AL_OK;
}

/**
 * This function disable the mpu region
 * @param DevId mpu device id
 * @param RegionNumber the region number
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Dev_DisableRegion(AL_MPU_DevStruct *Mpu, AL_U8 RegionNumber)
{
    AL_REG Instance;
    AL_REG RegionBaseAddr;
    AL_U32 DevId;

    DevId = Mpu->HwConfig.DeviceId;
    Instance = (AL_REG)Mpu->HwConfig.BaseAddress;

    if (DevId == ApuMpuRegionStatusStruct.DevId) {
        if (RegionNumber == 0 || RegionNumber > AL_MPU_APU_MAX_REGION) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }
    } else {
        if (RegionNumber == 0 || RegionNumber > AL_MPU_COMMON_MAX_REGION) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }
    }

    RegionBaseAddr = MPU_REGION_BASE_ADDR(Instance, RegionNumber);
    AlMpu_ll_SetRegionAttrEnable(RegionBaseAddr, MPU_REGION_DISABLE);

    AlMpu_Dev_UpdateRegionEnableStatus(Mpu->HwConfig.DeviceId, RegionNumber, MPU_REGION_DISABLE);

    return AL_OK;
}

/**
 * This function enable the mpu
 * @param DevId mpu device id
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Dev_MpuEnable(AL_MPU_DevStruct *Mpu)
{
    AL_REG Instance;

    Instance = (AL_REG)Mpu->HwConfig.BaseAddress;

    AlMpu_ll_MpuEnable(Instance);

    return AL_OK;
}

/**
 * This function disable the mpu
 * @param DevId mpu device id
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Dev_MpuDisable(AL_MPU_DevStruct *Mpu)
{
    AL_REG Instance;

    Instance = (AL_REG)Mpu->HwConfig.BaseAddress;

    AlMpu_ll_MpuDisable(Instance);

    return AL_OK;
}

static AL_VOID AlMpu_Dev_EventHandler(AL_MPU_DevStruct *Mpu)
{
    if (Mpu->EventCallBack) {
        AL_MPU_EventStruct event = {
            .EventId    = AL_MPU_DENY_ACCESS,
            .EventData  = 0
        };

        (*Mpu->EventCallBack)(event, Mpu->EventCallBackRef);
    }

}

static AL_MPU_InterruptIdEnum AlMpu_Dev_GetMpuIntrId(AL_U32 *MpuIntrState)
{
    AL_U32 IntrState = *MpuIntrState;

    if (IntrState & MPU_INTR_DDRS0_ID) {
        (*MpuIntrState) = (*MpuIntrState) & (~MPU_INTR_DDRS0_ID);
        return MPU_INTR_DDRS0_ID;
    } else if (IntrState & MPU_INTR_DDRS1_ID) {
        (*MpuIntrState) = (*MpuIntrState) & (~MPU_INTR_DDRS1_ID);
        return MPU_INTR_DDRS1_ID;
    } else if (IntrState & MPU_INTR_HPM0_ID) {
        (*MpuIntrState) = (*MpuIntrState) & (~MPU_INTR_HPM0_ID);
        return MPU_INTR_HPM0_ID;
    } else if (IntrState & MPU_INTR_HPM1_ID) {
        (*MpuIntrState) = (*MpuIntrState) & (~MPU_INTR_HPM1_ID);
        return MPU_INTR_HPM1_ID;
    } else if (IntrState & MPU_INTR_NPU_ID) {
        (*MpuIntrState) = (*MpuIntrState) & (~MPU_INTR_NPU_ID);
        return MPU_INTR_NPU_ID;
    } else if (IntrState & MPU_INTR_OCMS2_ID) {
        (*MpuIntrState) = (*MpuIntrState) & (~MPU_INTR_OCMS2_ID);
        return MPU_INTR_OCMS2_ID;
    } else if (IntrState & MPU_INTR_APU_ID) {
        (*MpuIntrState) = (*MpuIntrState) & (~MPU_INTR_APU_ID);
        return MPU_INTR_APU_ID;
    } else {
        return 0;
    }
}

static AL_U8 AlMpu_Dev_GetIntrRegionNumber(AL_REG Instance)
{
    AL_U32 IntrRegion;
    AL_U8 RegionNumber = 0;

    IntrRegion = AlMpu_ll_GetIntrRegionNumber(Instance);

    while (IntrRegion) {

        RegionNumber++;

        if (IntrRegion & 0x1)
            break;

        IntrRegion = (IntrRegion >> 1);
    }

    return RegionNumber;
}

/**
 * This function is MPU interrupt handler
 * @param Ptr reserved, not use
 *
 * @return
 *
 * @note
 */
AL_VOID AlMpu_Dev_MpuIntrHandler(void *Ptr)
{
    AL_U32 MpuIntrState = 0;
    AL_MPU_InterruptIdEnum MpuIntrStateId;
    AL_MPU_HwConfigStruct *HwConfig;
    AL_U32 Instance;
    AL_U8 IntrRegionNumber;
    AL_REG RegionBaseAddr;
    AL_MPU_DevStruct *Mpu;

    /* Get MPU interrupt id */
    MpuIntrState = AlMpu_ll_GetMpuIntrState();

    /* The loop handles all mpu interrupts */
    while (MpuIntrState) {
        MpuIntrStateId = AlMpu_Dev_GetMpuIntrId(&MpuIntrState);
        if (MpuIntrStateId == 0)
            continue;

        HwConfig = AlMpu_Dev_LookupConfigByIntrStateId(MpuIntrStateId);
        if (HwConfig == AL_NULL) {
            continue;
        }
        Instance = HwConfig->BaseAddress;
        Mpu = &(AL_MPU_DevInstance[HwConfig->DeviceId]);

        /* Get intrrupt region number and region base addrress */
        IntrRegionNumber = AlMpu_Dev_GetIntrRegionNumber(Instance);
        RegionBaseAddr = MPU_REGION_BASE_ADDR(Instance, IntrRegionNumber);

        /* Clear the interrupt */
        AlMpu_ll_ClrRegionIntr(RegionBaseAddr);

        AlMpu_Dev_EventHandler(Mpu);

        AL_LOG(AL_ERR_LEVEL_INFO, "[MPU][INTERRUPT]:MPU Instance is 0x%x, The region number "
               "that triggers the interrupt is region%d.\r\n", Instance, IntrRegionNumber);
    }
}

AL_S32 AlMpu_Dev_RegisterEventCallBack(AL_MPU_DevStruct *Mpu, AL_Mpu_EventCallBack Callback, void *CallbackRef)
{
    if (Mpu == AL_NULL || Callback == AL_NULL) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    if (Mpu->EventCallBack != AL_NULL) {

#ifdef MPU_DEBUG
        AL_LOG(AL_ERR_LEVEL_WARNING, "mpu=%p duplicate register callback: replace old:%p with New: %p\n", \
                Mpu, Mpu->EventCallBack, Callback);
#endif
    }

    Mpu->EventCallBack        = Callback;
    Mpu->EventCallBackRef     = CallbackRef;

    return AL_OK;
}