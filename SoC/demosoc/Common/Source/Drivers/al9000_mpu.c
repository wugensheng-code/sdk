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

#if (defined _AARCH_64 || defined __aarch64__)
#define __DSB() asm volatile("dsb sy" :::"memory");
#define __ISB() asm volatile("isb sy" :::"memory");
#define __DMB() asm volatile("dmb sy" :::"memory");
#endif

inline void do_sync(void)
{
	__ISB();
	__DSB();
	__DMB();
}


uint32_t AlMpu_Enable(AlMpu *InstancePtr)
{
	do_sync();
	SET_BIT(InstancePtr->MPU_CTRL, 0);
	do_sync();

	return 0;
}

uint32_t AlMpu_Disable(AlMpu *InstancePtr)
{
	do_sync();

	CLR_BIT(InstancePtr->MPU_CTRL, 0);

	do_sync();

	return 0;
}

uint32_t AlMpu_GetMpuStatus(AlMpu *InstancePtr)
{
	uint32_t Status = 0;
	Status = GET_BIT(InstancePtr->MPU_CTRL, 0);

	return Status;
}







