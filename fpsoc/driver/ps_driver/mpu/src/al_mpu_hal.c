#include "al_mpu_hal.h"
#include "al_errno.h"

#include "nuclei_sdk_soc.h"
#include "gic_v3.h"

static interrupt_table AL_MPU_IntrTable = {
    .handler = AlMpu_Hal_MpuIntrHandler,
    .ref = NULL
};

static AL_VOID AlMpu_Hal_SetRegionAttr(AL_REG32 RegionBaseAddr, AL_MPU_RegionConfigStruct *RegionConfig)
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
    if (!(AL_MPU_IS_VALID_MPU(Instance))) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }
    if (!(AL_MPU_IS_VALID_REGION_NUMBER(Instance, Config->RegionNumber))) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }
    if (!(AL_MPU_IS_VALID_REGION_ENABLE(Config->RegionEnable))) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }
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

    /* Check whether the region is configured */
    RegionBaseAddr = MPU_REGION_BASE_ADDR(Instance, Config->RegionNumber);
    EnableState = AlMpu_ll_GetRegionAttrEnableStatus(RegionBaseAddr);
    if (EnableState == MPU_REGION_ENABLE) {
        return AL_MPU_ERROR_REGION_CONFIGURED;
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
    if (!(AL_MPU_IS_VALID_MPU(Instance))) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    RegionBaseAddr = MPU_REGION_BASE_ADDR(Instance, RegionNumber);
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
    if (!(AL_MPU_IS_VALID_MPU(Instance))) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    RegionBaseAddr = MPU_REGION_BASE_ADDR(Instance, RegionNumber);
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
    if (!(AL_MPU_IS_VALID_MPU(Instance))) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
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
    if (!(AL_MPU_IS_VALID_MPU(Instance))) {
        return AL_MPU_ERR_ILLEGAL_PARAM;
    }

    AlMpu_ll_MpuDisable(Instance);

    return AL_OK;
}

AL_MPU_InterruptIdEnum AlMpu_hal_GetMpuIntrId(AL_U32 *MpuIntrState)
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

static AL_U32 AlMpu_Hal_GetInstance(AL_MPU_InterruptIdEnum IntrId)
{
    AL_U32 Instance;

    switch (IntrId) {
        case MPU_INTR_DDRS0_ID:
            Instance = (AL_U32)MPU_DDRS0_BASE_ADDR;
            break;

        case MPU_INTR_DDRS1_ID:
            Instance = (AL_U32)MPU_DDRS1_BASE_ADDR;
            break;

        case MPU_INTR_HPM0_ID:
            Instance = (AL_U32)MPU_HPM0_BASE_ADDR;
            break;

        case MPU_INTR_HPM1_ID:
            Instance = (AL_U32)MPU_HPM1_BASE_ADDR;
            break;

        case MPU_INTR_NPU_ID:
            Instance = (AL_U32)MPU_NPU_BASE_ADDR;
            break;

        case MPU_INTR_APU_ID:
            Instance = (AL_U32)MPU_APU_BASE_ADDR;
            break;

        case MPU_INTR_OCMS2_ID:
            Instance = (AL_U32)MPU_OCMS2_BASE_ADDR;
            break;

        default:
            Instance = AL_NULL;
            break;
    }

    return Instance;
}

static AL_U8 AlMpu_Hal_GetIntrRegionNumber(AL_REG32 Instance)
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
 * @param
 *
 * @return
 *
 * @note
 */
AL_VOID AlMpu_Hal_MpuIntrHandler(void *Ptr)
{
    AL_U32 MpuIntrState = 0;
    AL_MPU_InterruptIdEnum MpuIntrId;
    AL_U32 Instance;
    AL_U8 IntrRegionNumber;
    AL_REG32 RegionBaseAddr;

    /* Get MPU interrupt id */
    MpuIntrState = AlMpu_ll_GetMpuIntrState();

    /* The loop handles all mpu interrupts */
    while (MpuIntrState) {
        MpuIntrId = AlMpu_hal_GetMpuIntrId(&MpuIntrState);
        if (MpuIntrId == 0)
            continue;

        /* Get MPU base address */
        Instance = AlMpu_Hal_GetInstance(MpuIntrId);
        if (Instance == AL_NULL) {
            continue;
        }

        /* Get intrrupt region number and region base addrress */
        IntrRegionNumber = AlMpu_Hal_GetIntrRegionNumber(Instance);
        RegionBaseAddr = MPU_REGION_BASE_ADDR(Instance, IntrRegionNumber);

        /* Clear the interrupt */
        AlMpu_ll_ClrRegionIntr(RegionBaseAddr);

        printf("[MPU][INTERRUPT]:MPU Instance is 0x%x, The region number "
               "that triggers the interrupt is region%d.\r\n", Instance, IntrRegionNumber);
    }
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