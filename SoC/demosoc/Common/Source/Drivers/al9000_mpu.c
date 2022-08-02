/*
 * AL9000_mpu.c
 *
 *  Created on: 2022年6月20日
 *      Author: wei.pang
 */

#include "al9000_mpu.h"
#include "demosoc.h"
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"

uint8_t AlMpu_Enable(MPU_TypeDef *MPU_TYPE){
	MPU_TYPE -> MPU_CTRL |= MPU_CTRL_ENABLE;
	return 0;
}
uint8_t AlMpu_Disable(MPU_TypeDef *MPU_TYPE){
	MPU_TYPE -> MPU_CTRL &= ~MPU_CTRL_ENABLE;
	return 0;

}
uint8_t AlMpu_GetMpuStatus(MPU_TypeDef *MPU_TYPE){
	uint8_t status = 0;
	status = GET_BIT(MPU_TYPE -> MPU_CTRL,0 );
	return status;
}
uint8_t AlMpu_GetIntr(MPU_TypeDef *MPU_TYPE){
	uint8_t status = 0;
	status = GET_BIT(MPU_TYPE -> MPU_TYPER,0);
	return status;
}
uint8_t AlMpu_RegionSelect(MPU_TypeDef *MPU_TYPE,enum region_select Reg_Sel){
	switch(Reg_Sel){
	case 0:
	MPU_TYPE -> MPU_CTRL &= ~MPU_CTRL_ENABLE;
	}
	return 0;
}
uint8_t AlMpu_StartAddr(MPU_TypeDef *MPU_TYPE){
	
}
uint8_t AlMpu_EndAddr(MPU_TypeDef *MPU_TYPE){
	
}
uint8_t AlMpu_RegionUncfg(MPU_TypeDef *MPU_TYPE){
	MPU_TYPE -> MPU_RASR_REGION1 |= MPU_RASR_REGION_UNCONFIGURED;
	return 0;
}
uint8_t AlMpu_RegionReadProtect(MPU_TypeDef *MPU_TYPE){
	if(GET_BITS(MPU_TYPE ->MPU_RASR_REGION1,8,9)!=0){

		MPU_TYPE ->MPU_RASR_REGION1 &=~MPU_RASR_REGION_RW;
		MPU_TYPE ->MPU_RASR_REGION1 |= MPU_RASR_REGION_RO;
	}
	else{
		MPU_TYPE ->MPU_RASR_REGION1 |= MPU_RASR_REGION_RO;
	}
	return 0;
}
uint8_t AlMpu_RegionWriteProtect(MPU_TypeDef *MPU_TYPE){
		if(GET_BITS(MPU_TYPE ->MPU_RASR_REGION1,8,9)!=0){
		MPU_TYPE ->MPU_RASR_REGION1 &=~MPU_RASR_REGION_RW;
		MPU_TYPE ->MPU_RASR_REGION1 |= MPU_RASR_REGION_WO;
	}
	else{
		MPU_TYPE ->MPU_RASR_REGION1 |= MPU_RASR_REGION_WO;
	}
	return 0;
}
uint8_t AlMpu_RegionUnWR(MPU_TypeDef *MPU_TYPE){
	if(GET_BITS(MPU_TYPE ->MPU_RASR_REGION1,8,9)!=0){

		MPU_TYPE ->MPU_RASR_REGION1 &=~MPU_RASR_REGION_RW;
	}
	else{
	}
	return 0;
}
uint8_t AlMpu_RegionUnProtect(MPU_TypeDef *MPU_TYPE){

}
uint8_t AlMpu_RegionSecure(MPU_TypeDef *MPU_TYPE){
	
}
uint8_t AlMpu_RegionSize(MPU_TypeDef *MPU_TYPE,uint32_t Size){
	MPU_TYPE -> MPU_RASR_REGION1 |= Size << 1;
}
uint8_t AlMpu_RegionEnable(MPU_TypeDef *MPU_TYPE){
	MPU_TYPE -> MPU_RASR_REGION1 |= MPU_RASR_REGION_REGIONEN;
	return 0;
}
uint8_t AlMpu_RegionStartAddr(MPU_TypeDef *MPU_TYPE,uint32_t StartAddr){
	MPU_TYPE -> MPU_SAR_REGION1 = StartAddr;
	return 0;	
}
uint8_t AlMpu_RegionEndAddr(MPU_TypeDef *MPU_TYPE,uint32_t EndAddr){
	MPU_TYPE -> MPU_EAR_REGION1 = EndAddr;
	return 0;	
}
uint8_t AlMpu_RegionGroupId(MPU_TypeDef *MPU_TYPE,uint32_t Group_ID){
	MPU_TYPE -> MPU_GROUPID_REGION1 = Group_ID;
	return 0;
}






