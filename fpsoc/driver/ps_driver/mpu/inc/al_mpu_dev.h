#ifndef AL_MPU_DEV_H
#define AL_MPU_DEV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_mpu_ll.h"
#include "al_errno.h"

/*
  Region number define
 */

/* Common region number define, Note that regions are numbered from 0 */
#define AL_MPU_COMMON_REGION_0          (0x00)
#define AL_MPU_COMMON_REGION_1          (0x01)
#define AL_MPU_COMMON_REGION_2          (0x02)
#define AL_MPU_COMMON_REGION_3          (0x03)
#define AL_MPU_COMMON_REGION_4          (0x04)
#define AL_MPU_COMMON_REGION_5          (0x05)
#define AL_MPU_COMMON_REGION_6          (0x06)
#define AL_MPU_COMMON_REGION_7          (0x07)
#define AL_MPU_COMMON_MAX_REGION_NUMBER (AL_MPU_COMMON_REGION_7)

/* Just for apu mpu, apu mpu has 32 regions */
#define AL_MPU_APU_REGION_8             (0x08)
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
#define AL_MPU_APU_MAX_REGION_NUMBER    (AL_MPU_APU_REGION_31)

#define AL_MPU_INVALID_REGION_NUMBER    (AL_MPU_APU_REGION_31 + 1)

#define AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE   (4096)

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
    MPU_INVALID_DEVICE_ID       = 0x100,
    MPU_REGION_ENABLED          = 0x101,
    MPU_INVALID_REGION_NUMBER   = 0x102,
    MPU_NO_AVAILABLE_REGION     = 0x103,
    MPU_INITIALIZE_FAILED       = 0x104,
} AL_MPU_ErrorCodeEnum;

#define AL_MPU_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_MPU_ERR_INVALID_DEVICE_ID        (AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, MPU_INVALID_DEVICE_ID))
#define AL_MPU_ERROR_REGION_ENABLED         (AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, MPU_REGION_ENABLED))
#define AL_MPU_ERROR_REGION_NUMBER          (AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, MPU_INVALID_REGION_NUMBER))
#define AL_MPU_ERROR_NO_AVAILABLE_REGION    (AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, MPU_NO_AVAILABLE_REGION))
#define AL_MPU_ERROR_INITIALIZE_FAILED      (AL_DEF_ERR(AL_MPU, AL_ERR_LEVEL_ERROR, MPU_INITIALIZE_FAILED))

 typedef struct
{
    AL_MPU_RegionSecureEnum Secure;
    AL_MPU_RegionPrivilegeEnum Privilege;
    AL_MPU_RegionReadWriteEnum ReadWrite;
    AL_MPU_RegionInterruptEnEnum InterruptEnable;

    AL_U32 StartAddr;
    AL_U32 Size;

    /* Master group id, This can be a single group id
       or multiple group id can be grouped together
     */
    AL_U32 GroupId;
} AL_MPU_RegionConfigStruct;

typedef enum
{
    AL_MPU_DENY_ACCESS          = (0x01),
} AL_MPU_EventIdEnum;

typedef struct
{
    AL_MPU_EventIdEnum      EventId;
    AL_U32                  EventData;
} AL_MPU_EventStruct;

typedef AL_VOID (*AL_Mpu_EventCallBack)(AL_MPU_EventStruct MpuEvent, AL_VOID *CallbackRef);

typedef struct
{
    AL_MPU_HwConfigStruct       HwConfig;
    AL_MPU_RegionConfigStruct   *RegionConfig;
    AL_U8                       ConfigNumber;

    AL_Mpu_EventCallBack        EventCallBack;
    AL_VOID                     *EventCallBackRef;
} AL_MPU_DevStruct;

typedef struct
{
    AL_U8   DevId;
    AL_U8   AL_RegionEnableStatus[AL_MPU_COMMON_MAX_REGION_NUMBER + 1];
} AL_MPU_CommonMpuRegionStatusStruct;

typedef struct
{
    AL_U8   DevId;
    AL_U8   AL_RegionEnableStatus[AL_MPU_APU_MAX_REGION_NUMBER + 1];
} AL_MPU_ApuRegionStatusStruct;

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

AL_S32 AlMpu_Dev_MpuEnable(AL_MPU_DevStruct *Mpu);

AL_S32 AlMpu_Dev_MpuDisable(AL_MPU_DevStruct *Mpu);

AL_S32 AlMpu_Dev_ConfigRegion(AL_MPU_DevStruct *Mpu, AL_MPU_RegionConfigStruct *InitRegionConfig);

AL_S32 AlMpu_Dev_ConfigRegionByRegionNum(AL_MPU_DevStruct *Mpu, AL_U8 RegionNumber,
                                         AL_MPU_RegionConfigStruct *InitRegionConfig);

AL_S32 AlMpu_Dev_SetRegionEnableStatus(AL_MPU_DevStruct *Mpu, AL_U8 RegionNumber, AL_MPU_RegionEnEnum EnableStatus);

AL_S32 AlMpu_Dev_Init(AL_MPU_DevStruct *Mpu, AL_MPU_HwConfigStruct *HwConfig,
                      AL_MPU_RegionConfigStruct *InitRegionConfig, AL_U8 ConfigNumber);

AL_MPU_HwConfigStruct *AlMpu_Dev_LookupConfigByDevId(AL_U8 DevId);

AL_S32 AlMpu_Dev_RegisterEventCallBack(AL_MPU_DevStruct *Mpu, AL_Mpu_EventCallBack Callback, void *CallbackRef);

AL_VOID AlMpu_Dev_MpuIntrHandler(void *Instance);

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_DEV_H */