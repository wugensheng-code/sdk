#ifndef AL_MPU_LL_H
#define AL_MPU_LL_H

#ifdef _cplusplus
extern "C" {
#endif

#include "al_mpu_hw.h"
#include "al_reg_io.h"

#if (defined _AARCH_64 || defined __aarch64__)
#define __DSB() asm volatile("dsb sy" :::"memory");
#define __ISB() asm volatile("isb sy" :::"memory");
#define __DMB() asm volatile("dmb sy" :::"memory");
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
static inline AL_VOID AlMpu_ll_MpuEnable(AL_REG32 MpuBaseAddr)
{
    SET_BIT((MpuBaseAddr + MPU__CTRL__OFFSET), MPU__CTRL__ENABLE__SHIFT, MPU_ENABLE);

    /* Ensure MPU setting take effects */
#if (defined __AARCH_64 || defined __aarch64__)
    __DSB();
    __ISB();
#else
    // Todo
#endif /* defined _AARCH_64 || defined  _aarch64_ */
}

/**
 * This function disable MPU
 * @param MpuBaseAddr MPU base address
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_MpuDisable(AL_REG32 MpuBaseAddr)
{
    /* Make sure outstanding transfers are done */
#if (defined _AARCH_64 || defined _aarch64_)
    __DSB();
#else
    // Todo
#endif /* defined _AARCH_64 || defined _aarch64_ */

    SET_BIT((MpuBaseAddr + MPU__CTRL__OFFSET), MPU__CTRL__ENABLE__SHIFT, MPU_DISABLE);
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
static inline AL_VOID AlMpu_ll_SetRegionAddr(AL_REG32 RegionBaseAddr,
                                             AL_U32 StartAddr, AL_U32 EndAddr)
{
    WRITE_REG((RegionBaseAddr + MPU__SAR_REGION__OFFSET), StartAddr);
    WRITE_REG((RegionBaseAddr + MPU__EAR_REGION__OFFSET), EndAddr);
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
static inline AL_VOID AlMpu_ll_SetRegionAttrSecure(AL_REG32 RegionBaseAddr,
                                                   AL_MPU_RegionSecureEnum Secure)
{
    SET_BIT((RegionBaseAddr + MPU__RASR_REGION__OFFSET), MPU__RASR_REGION__SECURE__SHIFT, Secure);
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
static inline AL_VOID AlMpu_ll_SetRegionAttrPrivilege(AL_REG32 RegionBaseAddr,
                                                      AL_MPU_RegionPrivilegeEnum Privilege)
{
    SET_BIT((RegionBaseAddr + MPU__RASR_REGION__OFFSET),
            MPU__RASR_REGION__PRIVILEGE__SHIFT, Privilege);
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
static inline AL_VOID AlMpu_ll_SetRegionAttrRw(AL_REG32 RegionBaseAddr,
                                               AL_MPU_RegionReadWriteEnum ReadWrite)
{
    SET_BITS((RegionBaseAddr + MPU__RASR_REGION__OFFSET),
             MPU__RASR_REGION__RW__SHIFT, MPU__RASR_REGION__RW__SIZE, ReadWrite);
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
static inline AL_VOID AlMpu_ll_SetRegionAttrIntrEn(AL_REG32 RegionBaseAddr,
                                                   AL_MPU_RegionInterruptEnEnum IntrEn)
{
    SET_BIT((RegionBaseAddr + MPU__RASR_REGION__OFFSET),
            MPU__RASR_REGION__INTR_EN__SHIFT, IntrEn);
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
static inline AL_VOID AlMpu_ll_SetRegionAttrEnable(AL_REG32 RegionBaseAddr,
                                                   AL_MPU_RegionEnEnum RegionEn)
{
    /* Make sure outstanding transfers are done */
#if (defined _AARCH_64 || defined _aarch64_)
    __DSB();
#else
    // Todo
#endif /* defined _AARCH_64 || defined _aarch64_ */

    SET_BIT((RegionBaseAddr + MPU__RASR_REGION__OFFSET),
            MPU__RASR_REGION__REGIONEN__SHIFT, RegionEn);

    /* Ensure MPU setting take effects */
#if (defined _AARCH_64 || defined _aarch64_)
    __DSB();
    __ISB();
#else
    // Todo
#endif /* defined _AARCH_64 || defined _aarch64_ */
}

/**
 * This function get region enable state
 * @param RegionBaseAddr region base address
 *
 * @return MPU_REGION_DISABLE or MPU_REGION_ENABLE
 *
 * @note
 */
static inline AL_MPU_RegionEnEnum AlMpu_ll_GetRegionAttrEnable(AL_REG32 RegionBaseAddr)
{
    return GET_BIT((RegionBaseAddr + MPU__RASR_REGION__OFFSET),
                   MPU__RASR_REGION__REGIONEN__SHIFT);
}

/**
 * This function set region RASR
 * @param RegionBaseAddr region base address
 * @param RegionAttr region attribute
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_SetRegionAttr(AL_REG32 RegionBaseAddr, AL_U32 RegionAttr)
{
    WRITE_REG((RegionBaseAddr + MPU__RASR_REGION__OFFSET), RegionAttr);
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
static inline AL_VOID AlMpu_ll_SetRegionGroupId(AL_REG32 RegionBaseAddr, AL_U32 RegionGroupId)
{
    WRITE_REG((RegionBaseAddr + MPU__GROUPID_REGION__OFFSET), RegionGroupId);
}

/**
 * This function clear MPU interrupt
 * @param RegionBaseAddr region base address
 *
 * @return
 *
 * @note
 */
static inline AL_VOID AlMpu_ll_ClrRegionIntr(AL_REG32 RegionBaseAddr)
{
    SET_BIT((RegionBaseAddr + MPU__RASR_REGION__OFFSET),
            MPU__RASR_REGION__INTR_CLR__SHIFT, AL_FUNC_ENABLE);
}

/**
 * This function get MPU interrupt region number from MPU_TYPER
 * @param MpuBaseAddr MPU base address
 *
 * @return region number
 *
 * @note
 */
static inline AL_U32 AlMpu_ll_GetIntrRegionNumber(AL_REG32 MpuBaseAddr)
{
    return READ_REG(MpuBaseAddr + MPU__TYPER__OFFSET);
}

/**
 * This function get MPU interrupt id from TOP INTR_MPU_STATE register
 * @param
 *
 * @return MPU interrupt id
 *
 * @note
 */
static inline AL_U32 AlMpu_ll_GetMpuIntrId()
{
    return GET_BITS(MPU_INTR_STATE_BASE_ADDR, MPU_INTR_STATE_SHIFT, MPU_INTR_STATE_SHIFT_SIZE);
}

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_LL_H */