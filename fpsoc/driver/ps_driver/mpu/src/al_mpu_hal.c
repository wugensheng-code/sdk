#include "al_mpu_hal.h"
#include "al_errno.h"

#include "nuclei_sdk_soc.h"
#include "al_intr.h"

AL_MPU_DevStruct AL_MPU_DevInstance[AL_MPU_NUM_INSTANCE];

#define AL_MPU_HAL_LOCK(Handle)          do {} while (0)

#define AL_MPU_HAL_UNLOCK(Handle)        do {} while (0)

static AL_VOID AlMpu_Hal_MpuRegisterIntr()
{
    AlIntr_RegHandler(SOC_INT130_IRQn, AL_NULL, AlMpu_Dev_MpuIntrHandler, AL_MPU_DevInstance);

    __enable_irq();
}

static AL_VOID AlMpu_Hal_EventCallBack(AL_MPU_EventStruct MpuEvent, AL_VOID *CallbackRef)
{
    AL_MPU_HalStruct *Handle = (AL_MPU_HalStruct *)CallbackRef;

    switch (MpuEvent.EventId) {
        case AL_MPU_DENY_ACCESS:
            AL_LOG(AL_ERR_LEVEL_INFO, "AlMpu_Hal_EventCallBack MPU Instance is 0x%x, The region number "
               "that triggers the event is region%d.\r\n", Handle->Dev->HwConfig.BaseAddress, MpuEvent.EventData);
            break;

        default:
            break;
    }
}

/**
 * This function enable MPU
 * @param Handle is pointer to AL_MPU_HalStruct
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Hal_MpuEnable(AL_MPU_HalStruct *Handle)
{
    AL_S32 RetValue;

    if (Handle == AL_NULL) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_MpuEnable illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    AL_MPU_HAL_LOCK(Handle);

    RetValue = AlMpu_Dev_MpuEnable(Handle->Dev);

    AL_MPU_HAL_UNLOCK(Handle);

    return RetValue;
}

/**
 * This function disable MPU
 * @param Handle is pointer to AL_MPU_HalStruct
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Hal_MpuDisable(AL_MPU_HalStruct *Handle)
{
    AL_S32 RetValue;

    if (Handle == AL_NULL) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_MpuDisable illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    AL_MPU_HAL_LOCK(Handle);

    RetValue = AlMpu_Dev_MpuDisable(Handle->Dev);

    AL_MPU_HAL_UNLOCK(Handle);

    return RetValue;
}

/**
 * This function enable a region
 * @param Handle is pointer to AL_MPU_HalStruct
 * @param RegionNumber region number
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Hal_EnableRegion(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber)
{
    AL_S32 RetValue;

    if (Handle == AL_NULL) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_DisableRegion illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    AL_MPU_HAL_LOCK(Handle);

    RetValue = AlMpu_Dev_SetRegionEnableStatus(Handle->Dev, RegionNumber, MPU_REGION_ENABLE);

    AL_MPU_HAL_UNLOCK(Handle);

    return RetValue;
}

/**
 * This function disable a region
 * @param Handle is pointer to AL_MPU_HalStruct
 * @param RegionNumber region number
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Hal_DisableRegion(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber)
{
    AL_S32 RetValue;

    if (Handle == AL_NULL) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_DisableRegion illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    AL_MPU_HAL_LOCK(Handle);

    RetValue = AlMpu_Dev_SetRegionEnableStatus(Handle->Dev, RegionNumber, MPU_REGION_DISABLE);

    AL_MPU_HAL_UNLOCK(Handle);

    return RetValue;
}

/**
 * This function config a region by region number
 * @param Handle is pointer to AL_MPU_HalStruct
 * @param RegionNumber region number
 * @param InitRegionConfig pointer to AL_MPU_RegionConfigStruct
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Hal_ConfigRegionByRegionNum(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber,
                                         AL_MPU_RegionConfigStruct *InitRegionConfig)
{
    AL_S32 RetValue;

    if (Handle == AL_NULL) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_EnableRegion illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    AL_MPU_HAL_LOCK(Handle);

    RetValue = AlMpu_Dev_ConfigRegionByRegionNum(Handle->Dev, RegionNumber, InitRegionConfig);

    AL_MPU_HAL_UNLOCK(Handle);

    return RetValue;
}

/**
 * This function config a region
 * @param Handle is pointer to AL_MPU_HalStruct
 * @param InitRegionConfig pointer to AL_MPU_RegionConfigStruct
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Hal_ConfigRegion(AL_MPU_HalStruct *Handle, AL_MPU_RegionConfigStruct *InitRegionConfig)
{
    AL_S32 RetValue;

    if (Handle == AL_NULL) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_EnableRegion illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    AL_MPU_HAL_LOCK(Handle);

    RetValue = AlMpu_Dev_ConfigRegion(Handle->Dev, InitRegionConfig);

    AL_MPU_HAL_UNLOCK(Handle);

    return RetValue;
}

/**
 * This function initialize the mpu region configs
 * @param MpuDevId unique MPU device id
 * @param Handle is pointer to AL_MPU_HalStruct
 * @param EventCallBack Event CallBack
 * @param InitRegionConfig pointer to AL_MPU_RegionConfigStruct
 * @param ConfigNumber config number of AL_MPU_RegionConfigStruct
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Hal_ConfigInit(AL_U8 MpuDevId, AL_MPU_HalStruct *Handle, AL_Mpu_EventCallBack EventCallBack,
                            AL_MPU_RegionConfigStruct *InitRegionConfig, AL_U8 ConfigNumber)
{
    AL_S32 RetValue = AL_OK;
    AL_MPU_HwConfigStruct *HwConfig = AL_NULL;
    AL_Mpu_EventCallBack CallBack;
    AL_S32 ConfigCount;

    if (Handle == AL_NULL || InitRegionConfig == AL_NULL || ConfigNumber == 0) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_ConfigInit illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    AL_MPU_HAL_LOCK(Handle);

    HwConfig = AlMpu_Dev_LookupConfigByDevId(MpuDevId);
    if (HwConfig != AL_NULL) {
        Handle->Dev = &AL_MPU_DevInstance[MpuDevId];
    } else {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_ConfigInit invalid device id\r\n");
        return AL_MPU_ERR_INVALID_DEVICE_ID;
    }

    CallBack = (EventCallBack == AL_NULL) ? AlMpu_Hal_EventCallBack : EventCallBack;
    RetValue = AlMpu_Dev_RegisterEventCallBack(Handle->Dev, CallBack, (void *)Handle);
    if (RetValue != AL_OK) {
        AL_MPU_HAL_UNLOCK(Handle);
        return RetValue;
    }

    ConfigCount = AlMpu_Dev_Init(Handle->Dev, HwConfig, InitRegionConfig, ConfigNumber);
    if (ConfigCount == 0) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_ConfigInit initialize failed\r\n");
        RetValue = AL_MPU_ERROR_INITIALIZE_FAILED;
    } else if (ConfigCount < ConfigNumber) {
        AL_LOG(AL_ERR_LEVEL_WARNING, "Some configurations failed, and only %d configurations "
                                     "were correctly configured\r\n", ConfigCount);
        RetValue = AL_OK;
    } else {
        AL_LOG(AL_ERR_LEVEL_INFO, "AlMpu_Hal_ConfigInit initialize success\r\n");
        RetValue = AL_OK;
    }

    AL_MPU_HAL_UNLOCK(Handle);

    return RetValue;
}

/**
 * This function initialize the mpu related interrupt
 * @param
 *
 * @return
 *
 * @note
 */
AL_VOID AlMpu_Hal_Init()
{
    AlMpu_Hal_MpuRegisterIntr();
}