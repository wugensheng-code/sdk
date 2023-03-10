/*
 * AL_UART.H
 *
 *  Created on: 2022年6月20日
 *      Author: wei.pang
 */

/**
 ****************************************************************************************
 * @defgroup UART
 * @ingroup DRIVERS
 * @brief UART Drivers.
 *        Functions specified for UART.
 *
 * @{
 ****************************************************************************************
 */

#ifndef _AL9000_MPU_H
#define _AL9000_MPU_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "demosoc.h"





/**************************************bit define*********************/

#define MPU_CTRL_ENABLE		BIT(0)	//Set 1 to enable MPU


#define MPU_RASR_REGION_UNCONFIGURED 	BIT(10)//unconfigured region protect
#define MPU_RASR_REGION_RO		(uint32_t)0x00000200		//read and write permitted
#define MPU_RASR_REGION_WO		(uint32_t)0x00000100
#define MPU_RASR_REGION_RW		(uint32_t)0x00000300	
#define MPU_RASR_REGION_PRIVILEDGE 	BIT(7)//priviledge access permitted
#define MPU_RASR_REGION_SECURE		BIT(6)//secure access permitted
#define MPU_RASR_REGION_REGIONSIZE	//The region capacity. Capacity value: 1<<(REGIONSIZE+1) The minimum capacity is 32 bytes
#define MPU_RASR_REGION_REGIONEN	BIT(0)//1 = Enable the region. 0= Disable the region


#define MPU_SUCCESS  0
#define MPU_FAILURE  1


uint32_t AlMpu_Enable(AlMpu *MPU_TYPE);
uint32_t AlMpu_Disable(AlMpu *MPU_TYPE);
uint32_t AlMpu_GetMpuStatus(AlMpu *MPU_TYPE);

typedef struct {
    uint32_t StartAddr;
    uint32_t EndAddr;
    uint32_t Privilege;
    uint32_t Secure;
    uint32_t ReadWrite;
    uint32_t RegionEnable;
    uint32_t GroupId;
}RegionAttr;

#ifdef __cplusplus
}
#endif
#endif /* _HBIRD_MPU_H */
