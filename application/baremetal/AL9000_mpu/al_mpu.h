#ifndef __MPU_DDR_H
#define __MPU_DDR_H
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"

#define APU_REGIONSIZE 32
#define REGIONSIZE 8
#define MAX_PRIVILEGE  2
#define MAX_SECUREMODE 2
#define MAX_GROUPIDNUMBER 6


#define PRIVILEGE_PROTECTED 0x1
#define PRIVILEGE_UNPROTECTED 0x0

#define SECURE 0x1
#define NON_SECURE 0x0

#define READONLY 0x1
#define WRITEONLY 0x2
#define READWRITE 0x0
#define NOREADWRITE 0x3
#define MAX_ACCESSMODE 4

#define CSU_GROUPID_NUM 0x1
#define RPU_GROUPID_NUM 0x2
#define APU_GROUPID_NUM 0x4
#define NPU_GROUPID_NUM 0x8
#define JPU_GROUPID_NUM 0x10
#define DMA_AXI_GROUPID_NUM 0x20
#define HP0_GROUPID_NUM 0x40
#define HP1_GROUPID_NUM 0x80
#define GP0_GROUPID_NUM 0x100
#define GP1_GROUPID_NUM 0x200
#define HBUS_GROUPID_NUM 0x400

#define REGION_ENABLED 0x1
#define REGION_DISABLED 0x0

#define INTERRUPT_ENABLED 0x1
#define INTERRUPT_DISABLED 0x0

#define REGIONEN      0
#define SECURE_BIT        6
#define PRIVILEGE_BIT    7
#define WRITE_BIT         8
#define READ_BIT          9
#define INTERRUPT_BIT     12
#define INTERRUPT_CLR_BIT 14

#define MPU_DDR0_START_ADDR 0x0

#define MPU_DDR_START_ADDR 0x44a00
#define MPU_DDR_END_ADDR 0x44a02

#define MPU_DMA_AXI_START_ADDR 0x61007
#define MPU_DMA_AXI_END_ADDR 0x61009

#define MPU_DDR_NPU_START_ADDR 0x3007e
#define MPU_DDR_NPU_END_ADDR   0x30080

#define MPU_CTRL_R           0x4UL
#define MPU_RASR_REGION_R    0x10
#define MPU_SAR_REGION_R     0x14
#define MPU_EAR_REGION_R     0x18
#define MPU_GROUPID_REGION_R 0x1c

/* MPU interrupt state register */
#define INTR_MPU_STATE      0xf8800214

typedef enum
{
    MPU_DDRS0 = 0x1,
    MPU_DDRS1 = 0x2,
    MPU_HPM0 = 0x4,
    MPU_HPM1 = 0x8,
    MPU_NPU = 0x10,
    MPU_OCMS2 = 0x20,
    MPU_APU = 0x40,
} MPU_INTERRUPT_ID;

uint32_t AlMpu_AllTest(void);
uint32_t AlMpu_CompareTest(AlMpu *InstancePtr,  uint32_t Reg_Sel, RegionAttr Attr);
unsigned int RegRead(uint32_t reg_address);
void RegWrite(uint32_t reg_address, uint32_t reg_wdata);
uint32_t AlMpu_SetRegionAttr(AlMpu* InstancePtr, uint32_t Region, RegionAttr Attr);
uint32_t SetAttrAndCompare(AlMpu *InstancePtr, RegionAttr Attr);

#endif