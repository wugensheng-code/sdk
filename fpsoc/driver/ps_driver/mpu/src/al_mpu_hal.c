#include "al_mpu_hal.h"
#include "al_errno.h"

#include "nuclei_sdk_soc.h"
#include "gic_v3.h"

extern AL_MPU_DevStruct AL_MPU_DevInstance[AL_MPU_NUM_INSTANCE];

#define AL_MPU_HAL_LOCK(Handle)          do {} while (0)

#define AL_MPU_HAL_UNLOCK(Handle)        do {} while (0)

AL_VOID AlMpu_Hal_MpuRegisterIntr()
{
    interrupt_table AL_MPU_IntrTable = {
        .handler = AlMpu_Dev_MpuIntrHandler,
        .ref = NULL
    };

    ECLIC_Register_IRQ(SOC_INT130_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                       ECLIC_LEVEL_TRIGGER, 1, 1, &AL_MPU_IntrTable);
    __enable_irq();
}

static AL_VOID AlMpu_Hal_EventCallBack(AL_MPU_EventStruct MpuEvent, AL_VOID *CallbackRef)
{
    AL_MPU_HalStruct *Handle = (AL_MPU_HalStruct *)CallbackRef;

    AL_LOG(AL_ERR_LEVEL_INFO, "AlMpu_Hal_EventCallBack\r\n");

    switch (MpuEvent.EventId) {
        case AL_MPU_DENY_ACCESS:
            break;

        default:
            break;
    }
}

AL_S32 AlMpu_Hal_MpuEnable(AL_MPU_HalStruct *Handle)
{
    if (Handle == AL_NULL) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_MpuEnable illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    return AlMpu_Dev_MpuEnable(Handle->Dev);
}

AL_S32 AlMpu_Hal_MpuDisable(AL_MPU_HalStruct *Handle)
{
    if (Handle == AL_NULL) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_MpuDisable illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    return AlMpu_Dev_MpuDisable(Handle->Dev);
}

AL_S32 AlMpu_Hal_DisableRegion(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber)
{
    if (Handle == AL_NULL) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_DisableRegion illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    return AlMpu_Dev_DisableRegion(Handle->Dev, RegionNumber);
}

AL_S32 AlMpu_Hal_EnableRegion(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber, AL_MPU_RegionConfigStruct *InitRegionConfig)
{
    if (Handle == AL_NULL) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_EnableRegion illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    return AlMpu_Dev_EnableRegion(Handle->Dev, RegionNumber, InitRegionConfig);
}

AL_S32 AlMpu_Hal_Init(AL_U8 MpuDevId, AL_MPU_HalStruct *Handle, AL_Mpu_EventCallBack EventCallBack,
                      AL_MPU_RegionConfigStruct *InitRegionConfig, AL_U8 ConfigNumber)
{
    AL_S32 RetValue = AL_OK;
    AL_MPU_HwConfigStruct *HwConfig = AL_NULL;
    AL_Mpu_EventCallBack CallBack;

    if (Handle == AL_NULL || InitRegionConfig == AL_NULL || ConfigNumber == 0) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_Init illegal parameter\r\n");
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    AL_MPU_HAL_LOCK(Handle);

    HwConfig = AlMpu_Dev_LookupConfigByDevId(MpuDevId);
    if (HwConfig != AL_NULL) {
        Handle->Dev = &AL_MPU_DevInstance[MpuDevId];
    } else {
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_Init invalid device id\r\n");
        return AL_MPU_ERR_INVALID_DEVICE_ID;
    }

    CallBack = (EventCallBack == AL_NULL) ? AlMpu_Hal_EventCallBack : EventCallBack;
    RetValue = AlMpu_Dev_RegisterEventCallBack(Handle->Dev, CallBack, (void *)Handle);
    if (RetValue != AL_OK) {
        AL_MPU_HAL_UNLOCK(Handle);
        return RetValue;
    }

    AlMpu_Hal_MpuRegisterIntr();

    RetValue = AlMpu_Dev_Init(Handle->Dev, HwConfig, InitRegionConfig, ConfigNumber);
    if (RetValue != AL_OK) {
        AL_MPU_HAL_UNLOCK(Handle);
        AL_LOG(AL_ERR_LEVEL_ERROR, "AlMpu_Hal_Init initialize failed\r\n");
        return RetValue;
    }

    AL_MPU_HAL_UNLOCK(Handle);

    return AL_OK;
}