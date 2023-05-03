#ifndef AL_MPU_HAL_H
#define AL_MPU_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"
#include "al_mpu_ll.h"
#include "al_errno.h"

/*
  Region number define
 */

/* Common region number define */
#define AL_MPU_COMMON_REGION_1          (0x01)
#define AL_MPU_COMMON_REGION_2          (0x02)
#define AL_MPU_COMMON_REGION_3          (0x03)
#define AL_MPU_COMMON_REGION_4          (0x04)
#define AL_MPU_COMMON_REGION_5          (0x05)
#define AL_MPU_COMMON_REGION_6          (0x06)
#define AL_MPU_COMMON_REGION_7          (0x07)
#define AL_MPU_COMMON_REGION_8          (0x08)

/* Just for apu mpu, apu mpu has 32 regions */
#define AL_MPU_APU_REGION_9             (0x09)
#define AL_MPU_APU_REGION_10            (0x0A)
#define AL_MPU_APU_REGION_11            (0x0B)
#define AL_MPU_APU_REGION_12            (0x0C)
#define AL_MPU_APU_REGION_13            (0x0D)
#define AL_MPU_APU_REGION_14            (0x0E)
#define AL_MPU_APU_REGION_15            (0x0F)
#define AL_MPU_APU_REGION_16            (0x10)
#define AL_MPU_APU_REGION_17            (0x11)
#define AL_MPU_APU_REGION_18            (0x12)
#define AL_MPU_APU_REGION_19            (0x13)
#define AL_MPU_APU_REGION_20            (0x14)
#define AL_MPU_APU_REGION_21            (0x15)
#define AL_MPU_APU_REGION_22            (0x16)
#define AL_MPU_APU_REGION_23            (0x17)
#define AL_MPU_APU_REGION_24            (0x18)
#define AL_MPU_APU_REGION_25            (0x19)
#define AL_MPU_APU_REGION_26            (0x1A)
#define AL_MPU_APU_REGION_27            (0x1B)
#define AL_MPU_APU_REGION_28            (0x1C)
#define AL_MPU_APU_REGION_29            (0x1D)
#define AL_MPU_APU_REGION_30            (0x1E)
#define AL_MPU_APU_REGION_31            (0x1F)
#define AL_MPU_APU_REGION_32            (0x20)

/* Check the config parameter */
#define AL_MPU_IS_VALID_MPU(INSTANCE) (((INSTANCE) == MPU_DDRS0_BASE_ADDR)      || \
                                       ((INSTANCE) == MPU_DDRS1_BASE_ADDR)      || \
                                       ((INSTANCE) == MPU_HPM0_BASE_ADDR)       || \
                                       ((INSTANCE) == MPU_HPM1_BASE_ADDR)       || \
                                       ((INSTANCE) == MPU_NPU_BASE_ADDR)        || \
                                       ((INSTANCE) == MPU_OCMS2_BASE_ADDR)      || \
                                       ((INSTANCE) == MPU_APU_BASE_ADDR))

#define AL_MPU_IS_VALID_REGION_NUMBER(INSTANCE, REGION_NUMBER)                                      \
    ((((INSTANCE) == MPU_APU_BASE_ADDR) &&                                                          \
     (((REGION_NUMBER) >= AL_MPU_COMMON_REGION_1) && ((REGION_NUMBER) <= AL_MPU_APU_REGION_32))) || \
    ((((INSTANCE) != MPU_APU_BASE_ADDR) &&                                                          \
     (((REGION_NUMBER) >= AL_MPU_COMMON_REGION_1) && ((REGION_NUMBER) <= AL_MPU_COMMON_REGION_8)))))

#define AL_MPU_IS_VALID_REGION_ENABLE(ENABLE) (((ENABLE) == MPU_REGION_ENABLE) || \
                                               ((ENABLE) == MPU_REGION_DISABLE))

#define AL_MPU_IS_VALID_REGION_SECURE(SECURE) (((SECURE) == MPU_REGION_SECURE) || \
                                               ((SECURE) == MPU_REGION_NONSECURE))

#define AL_MPU_IS_VALID_REGION_PRIVILEGE(PRIVILEGE) (((PRIVILEGE) == MPU_REGION_UNPRIVILEGE) || \
                                                     ((PRIVILEGE) == MPU_REGION_PRIVILEGE))

#define AL_MPU_IS_VALID_READ_WRITE(READWRITE) (((READWRITE) == MPU_REGION_READWRITE)   || \
                                               ((READWRITE) == MPU_REGION_READONLY)    || \
                                               ((READWRITE) == MPU_REGION_WRITEONLY)   || \
                                               ((READWRITE) == MPU_REGION_NOREADWRITE))

#define AL_MPU_IS_VALID_REGION_INTR_EN(ENABLE) (((ENABLE) == MPU_REGION_INTERRUPT_DISABLE) || \
                                                ((ENABLE) == MPU_REGION_INTERRUPT_ENABLE))

/* MPU error code define */
typedef enum
{
    MPU_REGION_CONFIGURED = 0x100,
} AL_MPU_ErrorCodeEnum;

#define AL_MPU_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))

#define AL_MPU_ERROR_REGION_CONFIGURED      (AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, MPU_REGION_CONFIGURED))

 typedef struct
{
    /* Region number， The number starts with 1, which represents the first region */
    AL_U8 RegionNumber;

    AL_MPU_RegionEnEnum RegionEnable;

    /* Reversed, not use, and default region size is 4KB */
    AL_U8 RegionSize;

    AL_MPU_RegionSecureEnum Secure;
    AL_MPU_RegionPrivilegeEnum Privilege;
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
    MPU_INTR_DDRS0_ID   = 0x1,
    MPU_INTR_DDRS1_ID   = 0x2,
    MPU_INTR_HPM0_ID    = 0x4,
    MPU_INTR_HPM1_ID    = 0x8,
    MPU_INTR_NPU_ID     = 0x10,
    MPU_INTR_OCMS2_ID   = 0x20,
    MPU_INTR_APU_ID     = 0x40,
} AL_MPU_InterruptIdEnum;

AL_S32 AlMpu_Hal_ConfigRegion(AL_REG Instance, AL_MPU_RegionConfigStruct *RegionConfig);

AL_S32 AlMpu_Hal_EnableRegion(AL_REG Instance, AL_U8 RegionNumber);

AL_S32 AlMpu_Hal_DisableRegion(AL_REG Instance, AL_U8 RegionNumber);

AL_S32 AlMpu_Hal_MpuEnable(AL_REG Instance);

AL_S32 AlMpu_Hal_MpuDisable(AL_REG Instance);

AL_VOID AlMpu_Hal_MpuIntrHandler();

AL_VOID AlMpu_Hal_MpuRegisterIntr();

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_HAL_H */
