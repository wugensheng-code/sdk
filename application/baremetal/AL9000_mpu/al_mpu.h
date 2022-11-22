#ifndef __MPU_DDR_H
#define __MPU_DDR_H
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"

#define MAX_APU_REGIONSIZE 32
#define MAX_REGIONSIZE 8
#define MAX_PRIVILEGE  2
#define MAX_SECUREMODE 2
#define MAX_GROUPIDNUMBER 6
#define MAX_ACCESSMODE 4

#define READWRITE      0
#define READONLY       1
#define WRITEONLY      2
#define NOREADWRITE    3

#define REGIONEN      0
#define SECURE        6 
#define PRIVILEGE    7
#define WRITE         8
#define READ          9  

#define MPU_CTRL_R           0x4UL
#define MPU_RASR_REGION_R    0x10
#define MPU_SAR_REGION_R     0x14
#define MPU_EAR_REGION_R     0x18
#define MPU_GROUPID_REGION_R 0x1c

uint32_t AlMpu_AllTest(void);
uint32_t AlMpu_CompareTest(AlMpu *InstancePtr,  uint32_t Reg_Sel, RegionAttr Attr);
uint32_t NpuMpuTest(void);
unsigned int RegRead(uint32_t reg_address);
void RegWrite(uint32_t reg_address, uint32_t reg_wdata);

#endif