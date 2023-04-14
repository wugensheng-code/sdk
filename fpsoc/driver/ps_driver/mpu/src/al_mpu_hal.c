#include "al_mpu_hal.h"
#include "al_errno.h"

static AL_VOID AlMpu_Hal_SetRegionAttr(AL_REG32 RegionBaseAddr, AL_MPU_RegionConfigStruct *RegionConfig)
{
    AlMpu_ll_SetRegionAttrSecure(RegionBaseAddr, RegionConfig->Secure);
    AlMpu_ll_SetRegionAttrPriviledge(RegionBaseAddr, RegionConfig->Priviledge);
    AlMpu_ll_SetRegionAttrRw(RegionBaseAddr, RegionConfig->ReadWrite);
    AlMpu_ll_SetRegionAttrIntrEn(RegionBaseAddr, RegionConfig->InterruptEnable);
    AlMpu_ll_SetRegionAttrEnable(RegionBaseAddr, RegionConfig->RegionEnable);
}

AL_S32 AlMpu_Hal_ConfigRegion(AL_REG32 Instance, AL_MPU_RegionConfigStruct *Config)
{
    AL_REG32 RegionBaseAddr;
    AL_MPU_RegionEnEnum EnableState;

    /* Check the configure parameter */
    if (!(MPU_IS_VALID_MPU(Instance))) {
         return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }
    if (!(MPU_IS_VALID_REGION_NUMBER(Instance, Config->RegionNumber))) {
        return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }
    if (!(MPU_IS_VALID_REGION_ENABLE(Config->RegionEnable))) {
        return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }
    if (!(MPU_IS_VALID_REGION_SECURE(Config->Secure))) {
        return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }
    if (!(MPU_IS_VALID_REGION_PRIVILEDGE(Config->Priviledge))) {
        return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }
    if (!(MPU_IS_VALID_READ_WRITE(Config->ReadWrite))) {
        return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }
    if (!(MPU_IS_VALID_REGION_INTR_EN(Config->InterruptEnable))) {
        return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    /* Check whether the region is configured */
    RegionBaseAddr = MPU_REGION_I_BASE_ADDR(Instance, Config->RegionNumber);
    EnableState = AlMpu_ll_GetRegionAttrEnable(RegionBaseAddr);
    if (EnableState == MPU_REGION_ENABLE) {
        return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_WARNING, AL_ERR_ILLEGAL_PARAM);
    }

    AlMpu_ll_SetRegionAddr(RegionBaseAddr, Config->StartAddr, Config->EndAddr);
    AlMpu_Hal_SetRegionAttr(RegionBaseAddr, Config);

    AlMpu_ll_SetRegionGroupId(RegionBaseAddr, Config->GroupId);

    return AL_ERR_OK;
}

AL_S32 AlMpu_Hal_DisableRegion(AL_REG32 Instance, AL_U8 RegionNumber)
{
    AL_REG32 RegionBaseAddr;

    /* Check the Instance */
    if (!(MPU_IS_VALID_MPU(Instance))) {
         return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    RegionBaseAddr = MPU_REGION_I_BASE_ADDR(Instance, RegionNumber);
    AlMpu_ll_SetRegionAttrEnable(RegionBaseAddr, MPU_REGION_DISABLE);

    return AL_ERR_OK;
}

AL_S32 AlMpu_Hal_MpuEnable(AL_REG32 Instance)
{
    /* Check the Instance */
    if (!(MPU_IS_VALID_MPU(Instance))) {
         return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    AlMpu_ll_MpuEnable(Instance);

    return AL_ERR_OK;
}

AL_S32 AlMpu_Hal_MpuDisable(AL_REG32 Instance)
{
    /* Check the Instance */
    if (!(MPU_IS_VALID_MPU(Instance))) {
         return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    AlMpu_ll_MpuDisable(Instance);

    return AL_ERR_OK;
}