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

//Select the next region to be configured. Generally, there are only eight regions, so the 2:0 bit is valid

enum region_select{
 MPU_CTRL_REGION_0 ,   				 
 MPU_CTRL_REGION_1,    		 
 MPU_CTRL_REGION_2,    	
 MPU_CTRL_REGION_3,    	
 MPU_CTRL_REGION_4,    	
 MPU_CTRL_REGION_5,    	 
 MPU_CTRL_REGION_6,    	 
 MPU_CTRL_REGION_7   	
};

uint8_t AlMpu_Enable(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_Disable(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_GetMpuStatus(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_GetIntr(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_RegionSelect(MPU_TypeDef *MPU_TYPE,enum region_select Reg_Sel);
uint8_t AlMpu_StartAddr(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_EndAddr(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_RegionUncfg(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_RegionReadProtect(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_RegionWriteProtect(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_RegionUnWR(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_RegionUnProtect(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_RegionSecure(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_RegionSize(MPU_TypeDef *MPU_TYPE,uint32_t Size);
uint8_t AlMpu_RegionEnable(MPU_TypeDef *MPU_TYPE);
uint8_t AlMpu_RegionStartAddr(MPU_TypeDef *MPU_TYPE,uint32_t StartAddr);
uint8_t AlMpu_RegionEndAddr(MPU_TypeDef *MPU_TYPE,uint32_t EndAddr);
uint8_t AlMpu_RegionGroupId(MPU_TypeDef *MPU_TYPE,uint32_t Group_ID);

#ifdef __cplusplus
}
#endif
#endif /* _HBIRD_MPU_H */
