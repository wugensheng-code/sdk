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
#define PRIVILEDGE    7
#define WRITE         8
#define READ          9            

uint32_t AlMpu_AllTest(void);
uint32_t AlMpu_CompareTest(AlMpu *InstancePtr,  uint32_t Reg_Sel, RegionAttr Attr);
#endif