#ifndef AL_MPU_HAL_H
#define AL_MPU_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"
#include "al_mpu_ll.h"

/*
  Region number define
 */

/* Common region number define */
#define MPU_COMMON_REGION_NUMBER1       (0x01)
#define MPU_COMMON_REGION_NUMBER2       (0x02)
#define MPU_COMMON_REGION_NUMBER3       (0x03)
#define MPU_COMMON_REGION_NUMBER4       (0x04)
#define MPU_COMMON_REGION_NUMBER5       (0x05)
#define MPU_COMMON_REGION_NUMBER6       (0x06)
#define MPU_COMMON_REGION_NUMBER7       (0x07)
#define MPU_COMMON_REGION_NUMBER8       (0x08)

/* Just for apu mpu, apu mpu has 32 regions */
#define MPU_APU_REGION_NUMBER9          (0x09)
#define MPU_APU_REGION_NUMBER10         (0x0A)
#define MPU_APU_REGION_NUMBER11         (0x0B)
#define MPU_APU_REGION_NUMBER12         (0x0C)
#define MPU_APU_REGION_NUMBER13         (0x0D)
#define MPU_APU_REGION_NUMBER14         (0x0E)
#define MPU_APU_REGION_NUMBER15         (0x0F)
#define MPU_APU_REGION_NUMBER16         (0x10)
#define MPU_APU_REGION_NUMBER17         (0x11)
#define MPU_APU_REGION_NUMBER18         (0x12)
#define MPU_APU_REGION_NUMBER19         (0x13)
#define MPU_APU_REGION_NUMBER20         (0x14)
#define MPU_APU_REGION_NUMBER21         (0x15)
#define MPU_APU_REGION_NUMBER22         (0x16)
#define MPU_APU_REGION_NUMBER23         (0x17)
#define MPU_APU_REGION_NUMBER24         (0x18)
#define MPU_APU_REGION_NUMBER25         (0x19)
#define MPU_APU_REGION_NUMBER26         (0x1A)
#define MPU_APU_REGION_NUMBER27         (0x1B)
#define MPU_APU_REGION_NUMBER28         (0x1C)
#define MPU_APU_REGION_NUMBER29         (0x1D)
#define MPU_APU_REGION_NUMBER30         (0x1E)
#define MPU_APU_REGION_NUMBER31         (0x1F)
#define MPU_APU_REGION_NUMBER32         (0x20)

/* Check the config parameter */
#define MPU_IS_VALID_MPU(INSTANCE) (((INSTANCE) == MPU_DDRS1_BASE_ADDR)     || \
                                    ((INSTANCE) == MPU_DDRS0_BASE_ADDR)     || \
                                    ((INSTANCE) == MPU_HPM0_BASE_ADDR)      || \
                                    ((INSTANCE) == MPU_HPM1_BASE_ADDR)      || \
                                    ((INSTANCE) == MPU_HPM1_BASE_ADDR)      || \
                                    ((INSTANCE) == MPU_OCMS2_BASE_ADDR)     || \
                                    ((INSTANCE) == MPU_NPU_BASE_ADDR)       || \
                                    ((INSTANCE) == MPU_APU_BASE_ADDR))

#define MPU_IS_VALID_REGION_NUMBER(INSTANCE, NUMBER)                                        \
    ((((INSTANCE) == MPU_APU_BASE_ADDR) &&                                                  \
     (((NUMBER) >= MPU_COMMON_REGION_NUMBER1) && ((NUMBER) <= MPU_APU_REGION_NUMBER32))) || \
    ((((INSTANCE) != MPU_APU_BASE_ADDR) &&                                                  \
     (((NUMBER) >= MPU_COMMON_REGION_NUMBER1) && ((NUMBER) <= MPU_COMMON_REGION_NUMBER8)))))

#define MPU_IS_VALID_REGION_ENABLE(ENABLE) (((ENABLE) == MPU_REGION_ENABLE) || \
                                            ((ENABLE) == MPU_REGION_DISABLE))

#define MPU_IS_VALID_REGION_SECURE(SECURE) (((SECURE) == MPU_REGION_SECURE)   || \
                                            ((SECURE) == MPU_REGION_NONSECURE))

#define MPU_IS_VALID_REGION_PRIVILEDGE(PRIVILEDGE) (((PRIVILEDGE) == MPU_REGION_UNPRIVILEDGE) || \
                                                    ((PRIVILEDGE) == MPU_REGION_PRIVILEDGE))

#define MPU_IS_VALID_READ_WRITE(READWRITE) (((READWRITE) == MPU_REGION_READWRITE)   || \
                                            ((READWRITE) == MPU_REGION_READONLY)    || \
                                            ((READWRITE) == MPU_REGION_WRITEONLY)   || \
                                            ((READWRITE) == MPU_REGION_NOREADWRITE))

#define MPU_IS_VALID_REGION_INTR_EN(ENABLE) (((ENABLE) == MPU_REGION_INTERRUPT_DISABLE) || \
                                             ((ENABLE) == MPU_REGION_INTERRUPT_ENABLE))

 typedef struct
{
    /* Region number， The number starts with 1, which represents the first region */
    AL_U8 RegionNumber;

    AL_MPU_RegionEnEnum RegionEnable;

    /* Reversed, not use, and default region size is 4KB */
    AL_U8 RegionSize;

    AL_MPU_RegionSecureEnum Secure;
    AL_MPU_RegionPriviledgeEnum Priviledge;
    AL_MPU_RegionReadWriteEnum ReadWrite;
    AL_MPU_RegionInterruptEnEnum InterruptEnable;

    /* The start and end addresses of region protection,
       the lower 12 bits addresses need to be omitted
     */
    AL_U32 StartAddr;
    AL_U32 EndAddr;

    /* Master group id, This can be a single group id
       or multiple group id can be grouped together
     */
    AL_U32 GroupId;
} AL_MPU_RegionConfigStruct;

/* Master group id enum */
typedef enum
{
    MPU_GROUP_ID_CSU     = 0x1,
    MPU_GROUP_ID_RPU     = 0x2,
    MPU_GROUP_ID_APU     = 0x4,
    MPU_GROUP_ID_NPU     = 0x8,
    MPU_GROUP_ID_JPU     = 0x10,
    MPU_GROUP_ID_DMA_AXI = 0x20,
    MPU_GROUP_ID_HP0     = 0x40,
    MPU_GROUP_ID_HP1     = 0x80,
    MPU_GROUP_ID_GP0     = 0x100,
    MPU_GROUP_ID_GP1     = 0x200,
    MPU_GROUP_ID_HBUS    = 0x400,
} AL_MPU_GroupIdEnum;

/* MPU interrupt id enum */
typedef enum
{
    MPU_DDRS0 = 0x1,
    MPU_DDRS1 = 0x2,
    MPU_HPM0  = 0x4,
    MPU_HPM1  = 0x8,
    MPU_NPU   = 0x10,
    MPU_OCMS2 = 0x20,
    MPU_APU   = 0x40,
} AL_MPU_InterruptIdEnum;

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
AL_S32 AlMpu_Hal_ConfigRegion(AL_REG32 Instance, AL_MPU_RegionConfigStruct *RegionConfig);

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
AL_S32 AlMpu_Hal_DisableRegion(AL_REG32 Instance, AL_U8 RegionNumber);

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
AL_S32 AlMpu_Hal_MpuEnable(AL_REG32 Instance);

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
AL_S32 AlMpu_Hal_MpuDisable(AL_REG32 Instance);

/**
 * This function is MPU interrupt handler
 * @param
 *
 * @return
 *
 * @note
 */
AL_VOID AlMpu_Hal_MpuIntrHandler();

/**
 * This function register MPU interrupt
 * @param
 *
 * @return
 *
 * @note
 */
AL_VOID AlMpu_Hal_MpuRegisterIntr();

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_HAL_H */