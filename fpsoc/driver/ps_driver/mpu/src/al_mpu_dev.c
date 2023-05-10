#include "al_mpu_dev.h"
#include "al_errno.h"

#include "nuclei_sdk_soc.h"
#include "al_intr.h"

#include <string.h>

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

/**
 * This function return AL_MPU_HwConfigStruct by MPU device id
 * @param MpuDevId unique MPU device id
 *
 * @return
 *        - A pointer on success
 *        - NULL on failure
 *
 * @note
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

static AL_U8 AlMpu_Dev_GetAvailableRegionByDevId(AL_U8 DevId)
{
    AL_U32 MpuNumIndex;
    AL_U32 MpuRegionIndex;

    if (DevId == ApuMpuRegionStatusStruct.DevId) {
        for (MpuRegionIndex = 0; MpuRegionIndex <= AL_MPU_APU_MAX_REGION_NUMBER; MpuRegionIndex++) {
            if (ApuMpuRegionStatusStruct.AL_RegionEnableStatus[MpuRegionIndex] == MPU_REGION_DISABLE) {
                return MpuRegionIndex;
            }
        }
    } else {
        for (MpuNumIndex = 0; MpuNumIndex < AL_MPU_NUM_COMMON_INSTANCE; MpuNumIndex++) {
            if (CommonMpuRegionStatusStruct[MpuNumIndex].DevId == DevId) {
                for (MpuRegionIndex = 0; MpuRegionIndex <= AL_MPU_COMMON_MAX_REGION_NUMBER; MpuRegionIndex++) {
                    if (CommonMpuRegionStatusStruct[MpuNumIndex].AL_RegionEnableStatus[MpuRegionIndex]
                                                                                        == MPU_REGION_DISABLE) {
                        return MpuRegionIndex;
                    }
                }
            }
        }
    }

    return AL_MPU_INVALID_REGION_NUMBER;
}

static AL_S32 AlMpu_Dev_UpdateRegionEnableStatus(AL_U8 DevId, AL_U8 RegionNumber, AL_MPU_RegionEnEnum EnableStatus)
{
    AL_U32 MpuNumIndex;
    AL_U32 MpuRegionIndex = RegionNumber;

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

static AL_VOID AlMpu_Dev_SetRegionAttr(AL_REG RegionBaseAddr, AL_MPU_RegionConfigStruct *Config)
{
    AlMpu_ll_SetRegionAttrSecure(RegionBaseAddr, Config->Secure);
    AlMpu_ll_SetRegionAttrPrivilege(RegionBaseAddr, Config->Privilege);
    AlMpu_ll_SetRegionAttrRw(RegionBaseAddr, Config->ReadWrite);
    AlMpu_ll_SetRegionAttrIntrEn(RegionBaseAddr, Config->InterruptEnable);
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

    if (((Config->StartAddr) % AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE) ||
        ((Config->Size) % AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE)      ||
        ((Config->Size) < AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE)) {
            AL_LOG(AL_ERR_LEVEL_WARNING, "The protected address and length must be a multiple of 4K! "
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

/**
 * This function config a region by region number
 * @param Mpu is pointer to AL_MPU_DevStruct
 * @param RegionNumber region number
 * @param InitRegionConfig pointer to AL_MPU_RegionConfigStruct
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Dev_ConfigRegionByRegionNum(AL_MPU_DevStruct *Mpu, AL_U8 RegionNumber,
                                         AL_MPU_RegionConfigStruct *InitRegionConfig)
{
    AL_S32 RetValue;
    AL_U32 DevId;
    AL_REG MpuBaseAddr;
    AL_REG RegionBaseAddr;
    AL_U8 RegionEnableStatus;

    DevId = Mpu->HwConfig.DeviceId;
    MpuBaseAddr = (AL_REG)(Mpu->HwConfig.BaseAddress);

    /* Apu mpu */
    if (DevId == ApuMpuRegionStatusStruct.DevId) {
        if (RegionNumber > AL_MPU_APU_MAX_REGION_NUMBER) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }

        RegionEnableStatus = ApuMpuRegionStatusStruct.AL_RegionEnableStatus[RegionNumber];
    } else {
        if (RegionNumber > AL_MPU_COMMON_MAX_REGION_NUMBER) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }

        RegionEnableStatus = CommonMpuRegionStatusStruct[DevId].AL_RegionEnableStatus[RegionNumber];
    }
    if (RegionEnableStatus == MPU_REGION_ENABLE) {
        AL_LOG(AL_ERR_LEVEL_NOTICE, "Region already enabled\r\n");
        return AL_MPU_ERROR_REGION_ENABLED;
    }

    /* Get region base address */
    RegionBaseAddr = MPU_REGION_BASE_ADDR(MpuBaseAddr, RegionNumber);

    if ((RetValue = AlMpu_Dev_SetRegion(RegionBaseAddr, InitRegionConfig)) != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_WARNING, "AlMpu_Dev_SetRegion failed, "
               "and the current configuration is ignored!\r\n");
        return RetValue;
    }

    /* Update region enable status */
    AlMpu_Dev_UpdateRegionEnableStatus(Mpu->HwConfig.DeviceId, RegionNumber, MPU_REGION_ENABLE);

    return AL_OK;
}

/**
 * This function initialize the mpu region configs
 * @param Mpu is pointer to AL_MPU_DevStruct
 * @param HwConfig is pointer to AL_MPU_HwConfigStruct
 * @param InitRegionConfig pointer to AL_MPU_RegionConfigStruct
 * @param ConfigNumber config number of AL_MPU_RegionConfigStruct
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Dev_Init(AL_MPU_DevStruct *Mpu, AL_MPU_HwConfigStruct *HwConfig,
                      AL_MPU_RegionConfigStruct *InitRegionConfig, AL_U8 ConfigNumber)
{
    AL_S32 RetValue;
    AL_U8 RegionNumber;
    AL_REG MpuBaseAddr;
    AL_U8 RegionCount;
    AL_U8 ConfigCount = 0;

    if (Mpu == AL_NULL) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    Mpu->RegionConfig  = InitRegionConfig;
    Mpu->ConfigNumber  = ConfigNumber;
    Mpu->HwConfig      = *HwConfig;

    MpuBaseAddr = Mpu->HwConfig.BaseAddress;

    AlMpu_Dev_MpuDisable(Mpu);

    /* Config all the regions */
    for (RegionCount = 0; RegionCount < ConfigNumber; RegionCount++) {

        /* Get a available region */
        if ((RegionNumber = AlMpu_Dev_GetAvailableRegionByDevId(Mpu->HwConfig.DeviceId))
                                                                == AL_MPU_INVALID_REGION_NUMBER) {
            RetValue = AL_MPU_ERROR_NO_AVAILABLE_REGION;
            break;
        }

        RetValue = AlMpu_Dev_ConfigRegionByRegionNum(Mpu, RegionNumber, &(Mpu->RegionConfig[RegionCount]));
        if (RetValue != AL_OK) {
            AL_LOG(AL_ERR_LEVEL_WARNING, "AlMpu_Dev_ConfigRegion failed, "
                   "and the current configuration is ignored!\r\n");
            continue;
        }

        ConfigCount++;
    }

    AlMpu_Dev_MpuEnable(Mpu);

    return ConfigCount;
}

/**
 * This function config a region and enable the region
 * @param Mpu is pointer to AL_MPU_DevStruct
 * @param InitRegionConfig is pointer to AL_MPU_RegionConfigStruct
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Dev_ConfigRegion(AL_MPU_DevStruct *Mpu, AL_MPU_RegionConfigStruct *InitRegionConfig)
{
    AL_S32 RetValue;
    AL_U32 DevId;
    AL_REG MpuBaseAddr;
    AL_U8 RegionNumber;

    DevId = Mpu->HwConfig.DeviceId;
    MpuBaseAddr = (AL_REG)(Mpu->HwConfig.BaseAddress);

    /* Get a available region */
    if ((RegionNumber = AlMpu_Dev_GetAvailableRegionByDevId(Mpu->HwConfig.DeviceId)) == AL_MPU_INVALID_REGION_NUMBER) {
        return AL_MPU_ERROR_NO_AVAILABLE_REGION;
    }

     if ((RetValue = AlMpu_Dev_ConfigRegionByRegionNum(Mpu, RegionNumber, InitRegionConfig)) != AL_OK) {
        return RetValue;
     }

    return AL_OK;
}

/**
 * This function disable or enable a region
 * @param Mpu is pointer to AL_MPU_DevStruct
 * @param RegionNumber the region number
 * @param EnableStatus region MPU_REGION_DISABLE or MPU_REGION_ENABLE
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Dev_SetRegionEnableStatus(AL_MPU_DevStruct *Mpu, AL_U8 RegionNumber, AL_MPU_RegionEnEnum EnableStatus)
{
    AL_U32 DevId;
    AL_REG MpuBaseAddr;
    AL_REG RegionBaseAddr;

    DevId = Mpu->HwConfig.DeviceId;
    MpuBaseAddr = (AL_REG)(Mpu->HwConfig.BaseAddress);

    if (DevId == ApuMpuRegionStatusStruct.DevId) {
        if (RegionNumber > AL_MPU_APU_MAX_REGION_NUMBER) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }
    } else {
        if (RegionNumber > AL_MPU_COMMON_MAX_REGION_NUMBER) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Region number unvalid\r\n");
            return AL_MPU_ERROR_REGION_NUMBER;
        }
    }

    RegionBaseAddr = MPU_REGION_BASE_ADDR(MpuBaseAddr, RegionNumber);
    AlMpu_ll_SetRegionAttrEnable(RegionBaseAddr, EnableStatus);

    AlMpu_Dev_UpdateRegionEnableStatus(Mpu->HwConfig.DeviceId, RegionNumber, EnableStatus);

    return AL_OK;
}

/**
 * This function enable the mpu
 * @param Mpu is pointer to AL_MPU_DevStruct
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Dev_MpuEnable(AL_MPU_DevStruct *Mpu)
{
    AL_REG MpuBaseAddr;

    MpuBaseAddr = (AL_REG)Mpu->HwConfig.BaseAddress;

    AlMpu_ll_MpuEnable(MpuBaseAddr);

    return AL_OK;
}

/**
 * This function disable the mpu
 * @param Mpu is pointer to AL_MPU_DevStruct
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Dev_MpuDisable(AL_MPU_DevStruct *Mpu)
{
    AL_REG MpuBaseAddr;

    MpuBaseAddr = (AL_REG)Mpu->HwConfig.BaseAddress;

    AlMpu_ll_MpuDisable(MpuBaseAddr);

    return AL_OK;
}

static AL_VOID AlMpu_Dev_EventHandler(AL_MPU_DevStruct *Mpu, AL_U8 RegionNumber)
{
    if (Mpu->EventCallBack) {
        AL_MPU_EventStruct event = {
            .EventId    = AL_MPU_DENY_ACCESS,
            .EventData  = RegionNumber
        };

        (*Mpu->EventCallBack)(event, Mpu->EventCallBackRef);
    }

}

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
 * This function is MPU interrupt handler
 * @param Instance is pointer to AL_MPU_DevInstance
 *
 * @return
 *
 * @note
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
 * This function register event callback
 * @param Mpu is pointer to AL_MPU_DevStruct
 * @param Callback the callback function
 * @param CallbackRef pointer a handle
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
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