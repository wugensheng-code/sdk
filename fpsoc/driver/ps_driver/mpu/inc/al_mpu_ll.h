#ifndef AL_MPU_LL_H
#define AL_MPU_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_mpu_hw.h"
#include "al_reg_io.h"

#if (defined _AARCH_64 || defined __aarch64__)
#define dsb() asm volatile("dsb sy" :::"memory");
#define isb() asm volatile("isb sy" :::"memory");
#define dmb() asm volatile("dmb sy" :::"memory");
#endif /* defined _AARCH_64 || defined __aarch64__ */

typedef enum
{
    MPU_DISABLE = 0,
    MPU_ENABLE  = 1,
} AL_MPU_EnEnum;

typedef enum
{
    MPU_REGION_DISABLE = 0,
    MPU_REGION_ENABLE  = 1,
} AL_MPU_RegionEnEnum;

typedef enum
{
    MPU_REGION_NONSECURE = 0,
    MPU_REGION_SECURE    = 1,
} AL_MPU_RegionSecureEnum;

typedef enum
{
    MPU_REGION_UNPRIVILEGE = 0,
    MPU_REGION_PRIVILEGE   = 1,
} AL_MPU_RegionPrivilegeEnum;

typedef enum
{
    MPU_REGION_READWRITE   = 0,
    MPU_REGION_READONLY    = 1,
    MPU_REGION_WRITEONLY   = 2,
    MPU_REGION_NOREADWRITE = 3,
} AL_MPU_RegionReadWriteEnum;

typedef enum
{
    MPU_REGION_INTERRUPT_DISABLE = 0,
    MPU_REGION_INTERRUPT_ENABLE  = 1,
} AL_MPU_RegionInterruptEnEnum;

/**
 * This function enable MPU
 * @param MpuBaseAddr MPU base address
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_MpuEnable(AL_REG MpuBaseAddr)
{
    AL_REG32_SET_BIT((MpuBaseAddr + MPU_CTRL_OFFSET), MPU_CTRL_ENABLE_SHIFT, MPU_ENABLE);

    /* Ensure MPU setting take effects */
#if (defined _AARCH_64 || defined __aarch64__)
    dsb();
    isb();
#else
    // Todo
#endif /* defined _AARCH_64 || defined  __aarch64__ */
}

/**
 * This function disable MPU
 * @param MpuBaseAddr MPU base address
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_MpuDisable(AL_REG MpuBaseAddr)
{
    /* Make sure outstanding transfers are done */
#if (defined _AARCH_64 || defined __aarch64__)
    dmb();
#else
    // Todo
#endif /* defined _AARCH_64 || defined __aarch64__ */

    AL_REG32_SET_BIT((MpuBaseAddr + MPU_CTRL_OFFSET), MPU_CTRL_ENABLE_SHIFT, MPU_DISABLE);
}

/**
 * This function set SAR and EAR
 * @param RegionBaseAddr region base address
 * @param StartAddr start address
 * @param StartAddr end address
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_SetRegionAddr(AL_REG RegionBaseAddr, AL_U32 StartAddr, AL_U32 EndAddr)
{
    AL_REG32_WRITE((RegionBaseAddr + MPU_SAR_REGION_OFFSET), StartAddr);
    AL_REG32_WRITE((RegionBaseAddr + MPU_EAR_REGION_OFFSET), EndAddr);
}

/**
 * This function set RASR secure
 * @param RegionBaseAddr region base address
 * @param Secure secure attribute
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_SetRegionAttrSecure(AL_REG RegionBaseAddr, AL_MPU_RegionSecureEnum Secure)
{
    AL_REG32_SET_BIT((RegionBaseAddr + MPU_RASR_REGION_OFFSET), MPU_RASR_REGION_SECURE_SHIFT, Secure);
}

/**
 * This function set RASR priviledge
 * @param RegionBaseAddr region base address
 * @param Privilege privilege attribute
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_SetRegionAttrPrivilege(AL_REG RegionBaseAddr, AL_MPU_RegionPrivilegeEnum Privilege)
{
    AL_REG32_SET_BIT((RegionBaseAddr + MPU_RASR_REGION_OFFSET), MPU_RASR_REGION_PRIVILEGE_SHIFT, Privilege);
}

/**
 * This function set RASR readwrite
 * @param RegionBaseAddr region base address
 * @param ReadWrite read write attribute
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_SetRegionAttrRw(AL_REG RegionBaseAddr, AL_MPU_RegionReadWriteEnum ReadWrite)
{
    AL_REG32_SET_BITS((RegionBaseAddr + MPU_RASR_REGION_OFFSET),
             MPU_RASR_REGION_RW_SHIFT, MPU_RASR_REGION_RW_SIZE, ReadWrite);
}

/**
 * This function enable region interrupt
 * @param RegionBaseAddr region base address
 * @param IntrEn interrupt enable
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_SetRegionAttrIntrEn(AL_REG RegionBaseAddr, AL_MPU_RegionInterruptEnEnum IntrEn)
{
    AL_REG32_SET_BIT((RegionBaseAddr + MPU_RASR_REGION_OFFSET), MPU_RASR_REGION_INTR_EN_SHIFT, IntrEn);
}

/**
 * This function enable region
 * @param RegionBaseAddr region base address
 * @param RegionEn region enable
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_SetRegionAttrEnable(AL_REG RegionBaseAddr, AL_MPU_RegionEnEnum RegionEn)
{
    /* Make sure outstanding transfers are done */
#if (defined _AARCH_64 || defined __aarch64__)
    dsb();
#else
    // Todo
#endif /* defined _AARCH_64 || defined __aarch64__ */

    AL_REG32_SET_BIT((RegionBaseAddr + MPU_RASR_REGION_OFFSET), MPU_RASR_REGION_REGIONEN_SHIFT, RegionEn);

    /* Ensure MPU setting take effects */
#if (defined _AARCH_64 || defined __aarch64__)
    dsb();
    isb();
#else
    // Todo
#endif /* defined _AARCH_64 || defined __aarch64__ */
}

/**
 * This function get region enable state
 * @param RegionBaseAddr region base address
 *
 * @return MPU_REGION_DISABLE or MPU_REGION_ENABLE
 *
 * @note
 */
static inline AL_MPU_RegionEnEnum AlMpu_ll_GetRegionAttrEnableStatus(AL_REG RegionBaseAddr)
{
    return AL_REG32_GET_BIT((RegionBaseAddr + MPU_RASR_REGION_OFFSET), MPU_RASR_REGION_REGIONEN_SHIFT);
}

/**
 * This function set region group id
 * @param RegionBaseAddr region base address
 * @param RegionGroupId group id
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_SetRegionGroupId(AL_REG RegionBaseAddr, AL_U32 RegionGroupId)
{
    AL_REG32_WRITE((RegionBaseAddr + MPU_GROUPID_REGION_OFFSET), RegionGroupId);
}

/**
 * This function clear MPU interrupt
 * @param RegionBaseAddr region base address
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_ClrRegionIntr(AL_REG RegionBaseAddr)
{
    AL_REG32_SET_BIT((RegionBaseAddr + MPU_RASR_REGION_OFFSET), MPU_RASR_REGION_INTR_CLR_SHIFT, AL_FUNC_ENABLE);
}

/**
 * This function get MPU interrupt region number from MPU_TYPER
 * @param MpuBaseAddr MPU base address
 *
 * @return region number
 *
 * @note
 */
static inline AL_U32 AlMpu_ll_GetIntrRegionNumber(AL_REG MpuBaseAddr)
{
    return AL_REG32_READ(MpuBaseAddr + MPU_TYPER_OFFSET);
}

/**
 * This function get MPU interrupt id from TOP INTR_MPU_STATE register
 * @param
 *
 * @return MPU interrupt id
 *
 * @note
 */
static inline AL_U32 AlMpu_ll_GetMpuIntrState()
{
    return AL_REG32_GET_BITS(MPU_INTR_STATE_BASE_ADDR, MPU_INTR_STATE_SHIFT, MPU_INTR_STATE_SHIFT_SIZE);
}

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_LL_H */