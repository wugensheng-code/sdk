#include "al_mpu_hal.h"
#include "al_errno.h"

#include "nuclei_sdk_soc.h"
#include "gic_v3.h"

static interrupt_table AL_MPU_IntrTable = {
    .handler = AlMpu_Hal_MpuIntrHandler,
    .ref = NULL
};

static AL_VOID AlMpu_Hal_SetRegionAttr(AL_REG32 RegionBaseAddr,
                                       AL_MPU_RegionConfigStruct *RegionConfig)
{
    AlMpu_ll_SetRegionAttrSecure(RegionBaseAddr, RegionConfig->Secure);
    AlMpu_ll_SetRegionAttrPrivilege(RegionBaseAddr, RegionConfig->Privilege);
    AlMpu_ll_SetRegionAttrRw(RegionBaseAddr, RegionConfig->ReadWrite);
    AlMpu_ll_SetRegionAttrIntrEn(RegionBaseAddr, RegionConfig->InterruptEnable);
    AlMpu_ll_SetRegionAttrEnable(RegionBaseAddr, RegionConfig->RegionEnable);
}

/**
 * This function configure the mpu region
 * @param Instance mpu base address
 * @param RegionConfig Configured parameter
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
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
    if (!(MPU_IS_VALID_REGION_PRIVILEDGE(Config->Privilege))) {
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

    return AL_OK;
}

/**
 * This function enable the mpu region
 * @param Instance mpu base address
 * @param RegionNumber the region number
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Hal_EnableRegion(AL_REG32 Instance, AL_U8 RegionNumber)
{
    AL_REG32 RegionBaseAddr;

    /* Check the Instance */
    if (!(MPU_IS_VALID_MPU(Instance))) {
        return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    RegionBaseAddr = MPU_REGION_I_BASE_ADDR(Instance, RegionNumber);
    AlMpu_ll_SetRegionAttrEnable(RegionBaseAddr, MPU_REGION_ENABLE);

    return AL_OK;
}

/**
 * This function disable the mpu region
 * @param Instance mpu base address
 * @param RegionNumber the region number
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Hal_DisableRegion(AL_REG32 Instance, AL_U8 RegionNumber)
{
    AL_REG32 RegionBaseAddr;

    /* Check the Instance */
    if (!(MPU_IS_VALID_MPU(Instance))) {
        return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    RegionBaseAddr = MPU_REGION_I_BASE_ADDR(Instance, RegionNumber);
    AlMpu_ll_SetRegionAttrEnable(RegionBaseAddr, MPU_REGION_DISABLE);

    return AL_OK;
}

/**
 * This function enable the mpu
 * @param Instance mpu base address
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Hal_MpuEnable(AL_REG32 Instance)
{
    /* Check the Instance */
    if (!(MPU_IS_VALID_MPU(Instance))) {
        return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    AlMpu_ll_MpuEnable(Instance);

    return AL_OK;
}

/**
 * This function disable the mpu
 * @param Instance mpu base address
 *
 * @return
 *        - 0 on success
 *        - Numbers greater than zero on failure
 *
 * @note
 */
AL_S32 AlMpu_Hal_MpuDisable(AL_REG32 Instance)
{
    /* Check the Instance */
    if (!(MPU_IS_VALID_MPU(Instance))) {
        return AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    AlMpu_ll_MpuDisable(Instance);

    return AL_OK;
}

static AL_U32 AlMpu_Hal_GetInstance(AL_MPU_InterruptIdEnum IntrId)
{
    AL_U32 Instance;

    switch (IntrId) {
        case MPU_DDRS0:
            Instance = (AL_U32)MPU__MPU_DDRS0__BASE_ADDR;
            break;

        case MPU_DDRS1:
            Instance = (AL_U32)MPU__MPU_DDRS1__BASE_ADDR;
            break;

        case MPU_HPM0:
            Instance = (AL_U32)MPU__MPU_HPM0__BASE_ADDR;
            break;

        case MPU_HPM1:
            Instance = (AL_U32)MPU__MPU_HPM1__BASE_ADDR;
            break;

        case MPU_NPU:
            Instance = (AL_U32)MPU__MPU_NPU__BASE_ADDR;
            break;

        case MPU_APU:
            Instance = (AL_U32)MPU__MPU_APU__BASE_ADDR;
            break;

        case MPU_OCMS2:
            Instance = (AL_U32)MPU__MPU_OCMS2__BASE_ADDR;
            break;

        default:
            Instance = AL_NULL;
            break;
    }

    return Instance;
}

static AL_U32 AlMpu_Hal_GetIntrRegionNumber(AL_REG32 Instance)
{
    return AlMpu_ll_GetIntrRegionNumber(Instance);
}

/**
 * This function is MPU interrupt handler
 * @param
 *
 * @return
 *
 * @note
 */
AL_VOID AlMpu_Hal_MpuIntrHandler()
{
    AL_MPU_InterruptIdEnum MpuIntrId;
    AL_U32 Instance;
    AL_U32 IntrRegionNumber;
    AL_REG32 RegionBaseAddr;

    /* Get MPU interrupt id */
    MpuIntrId = AlMpu_ll_GetMpuIntrId();

    /* Get MPU base address */
    Instance = AlMpu_Hal_GetInstance(MpuIntrId);
    if (Instance == AL_NULL) {
        return;
    }

    /* Get intrrupt region number and region base addrress */
    IntrRegionNumber = AlMpu_Hal_GetIntrRegionNumber(Instance);
    RegionBaseAddr = MPU_REGION_I_BASE_ADDR(Instance, IntrRegionNumber);

    /* Clear the interrupt */
    AlMpu_ll_ClrRegionIntr(RegionBaseAddr);

    printf("[MPU][INTERRUPT]:MPU Instance is 0x%x, The region number "
            "that triggers the interrupt is number %d.\r\n", Instance, IntrRegionNumber);
}

/**
 * This function register MPU interrupt
 * @param
 *
 * @return
 *
 * @note
 */
AL_VOID AlMpu_Hal_MpuRegisterIntr()
{
    ECLIC_Register_IRQ(SOC_INT130_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                       ECLIC_LEVEL_TRIGGER, 1, 1, &AL_MPU_IntrTable);
    __enable_irq();
}