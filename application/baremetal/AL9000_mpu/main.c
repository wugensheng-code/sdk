//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include <time.h>

extern void modeswitch();
/*
 *baudrate:115200
 *datebits:8
 *stopbits:2
 *fifomode:enable
 *function:uart_wrire
 *test_mode:printf read_irq	  :
*/
int main(){
	uint8_t MPU_Status = 0;
	uint32_t MPU_Counter = 0;
	MPU_Status = AlMpu_GetMpuStatus(AL_MPU_HPM0);
	if(MPU_Status == 1){
	   AlMpu_Disable(AL_MPU_HPM0);
	}
	AlMpu_RegionSize(AL_MPU_HPM0,8);
	AlMpu_RegionStartAddr(AL_MPU_HPM0,0x61030);
	AlMpu_RegionEndAddr(AL_MPU_HPM0,0x6103f);
	AlMpu_RegionGroupId(AL_MPU_HPM0,0x120);
	AlMpu_RegionWriteProtect(AL_MPU_HPM0);
	AlMpu_RegionEnable(AL_MPU_HPM0);
	AlMpu_Enable(AL_MPU_HPM0);
	*(uint32_t *)(0xf88001bcul) = 0xffffffff;//ocm_cfg=1;
	*(uint32_t *)(0xf8801078ul) = 0x1101;	//reset_n =1;
	modeswitch();
	//do {
		//MPU_Counter ++;
	//}while(MPU_Counter < 666666);
	return 0;
}
