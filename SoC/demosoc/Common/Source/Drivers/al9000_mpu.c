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
	#ifdef __aarch64__
	asm volatile("dsb     sy" :::"memory");
	asm volatile("isb     sy" :::"memory");
	asm volatile("dmb     sy" :::"memory");
	#endif

	SET_BIT(InstancePtr->MPU_CTRL, 0);

	#ifdef __aarch64__
	asm volatile("dsb     sy" :::"memory");
	asm volatile("isb     sy" :::"memory");
	asm volatile("dmb     sy" :::"memory");
	#endif

	return 0;
}

uint32_t AlMpu_Disable(AlMpu *InstancePtr)
{
	#ifdef __aarch64__
	asm volatile("dsb     sy" :::"memory");
	asm volatile("isb     sy" :::"memory");
	asm volatile("dmb     sy" :::"memory");
	#endif

	CLR_BIT(InstancePtr->MPU_CTRL, 0);

	#ifdef __aarch64__
	asm volatile("dsb     sy" :::"memory");
	asm volatile("isb     sy" :::"memory");
	asm volatile("dmb     sy" :::"memory");
	#endif

	return 0;
}

uint32_t AlMpu_GetMpuStatus(AlMpu *InstancePtr)
{
	uint32_t Status = 0;
	Status = GET_BIT(InstancePtr->MPU_CTRL, 0);

	return Status;
}







