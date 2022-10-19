/*
 * AL9000_mpu.c
 *
 *  Created on: 2022.06.20
 *      Author: wei.pang
 */

#include "al9000_mpu.h"
#include "demosoc.h"
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"

uint32_t AlMpu_Enable(AlMpu *InstancePtr) 
{
	SET_BIT(InstancePtr->MPU_CTRL, 0);
	return 0;
}

uint32_t AlMpu_Disable(AlMpu *InstancePtr) 
{
	CLR_BIT(InstancePtr->MPU_CTRL, 0);
	return 0;
}

uint32_t AlMpu_GetMpuStatus(AlMpu *InstancePtr) 
{
	uint32_t Status = 0;
	Status = GET_BIT(InstancePtr->MPU_CTRL, 0);
	return Status;
}







