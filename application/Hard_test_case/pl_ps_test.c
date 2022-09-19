/*
 * pl_ps_test.c
 *
 *  Created on: 2022年5月6日
 *      Author: Administrator
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"
#include "AL_dmac.h"

uint32_t plps_test(){
	uint32_t buffer[256];
	uint32_t checkdata[256];
	uint32_t reset = 0;
	volatile uint32_t i = 0;
	for(i = 0 ; i < 256 ; i ++){
		buffer[i] = i;
	}
	*(uint32_t *)AL_PLPS_RESET =0x00001133;
	*(uint32_t *)0xf8800080 =0x0;	//top register 
    printf("\r\n");
    printf("\r\n");
    printf("\r\n");
    printf("\r\n");
    printf("***************************PS-PL TEST*****************************\r\n");
	write_To_OCM((uint32_t*)buffer, 256, AL_FAHB_BASE);
	write_To_OCM((uint32_t*)buffer, 256, AL_GP0);
	write_To_OCM((uint32_t*)buffer, 256, AL_GP1);
	write_To_OCM((uint32_t*)buffer, 256, AL_GP1_1);
	//write_To_OCM((uint32_t*)buffer, 256, AL_GP0_1);
	write_To_OCM((uint32_t*)buffer, 256, AL_GP1_2);
	write_To_OCM((uint32_t*)buffer, 256, AL_GP1_3);

	//write_To_OCM(AL9000_FAHB_BASE, 256, (uint32_t*)checkdata);
	printf("PS-PL TEST\r\n");
	for(i = 0 ; i < 256 ; i ++){
		if(buffer[i] == (*(uint32_t*)(AL_FAHB_BASE + i*4))){
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_FAHB_BASE + i*4));
					printf("AL_FAHB_BASE is OK\r\n");
				}
				else{
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_FAHB_BASE + i*4));
					printf("AL_FAHB_BASE is fail\r\n");
				}/**/
		if(buffer[i] == (*(uint32_t*)(AL_GP0 + i*4))){
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP0 + i*4));
					printf("AL_GP0 is OK\r\n");
				}
				else{
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP0 + i*4));
					printf("AL_GP0 is fail\r\n");
				}
		if(buffer[i] == (*(uint32_t*)(AL_GP1 + i*4))){
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP1 + i*4));
					printf("AL_GP1 is OK\r\n");
				}
				else{
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP1 + i*4));
					printf("AL_GP1 is fail\r\n");
				}
		if(buffer[i] == (*(uint32_t*)(AL_GP1_1 + i*4))){
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP1_1 + i*4));
					printf("AL_GP1_1 is OK\r\n");
				}
				else{
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP1_1 + i*4));
					printf("AL_GP1_1 is fail\r\n");
				}
		/*temp = *algp0_1;
		if(buffer[i] == temp){
					//printf("buffer = %d\r\n",buffer[i]);
					//printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP0_1 + i*4));
					printf("AL_GP0_1 is OK\r\n");
				}
				else{
					//printf("buffer = %d\r\n",buffer[i]);
					//printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP0_1 + i*4));
					printf("AL_GP0_1 is fail\r\n");
				}*/
		if(buffer[i] == (*(uint32_t*)(AL_GP1_2 + i*4))){
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP1_2 + i*4));
					printf("AL_GP1_2 is OK\r\n");
				}
				else{
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP1_2 + i*4));
					printf("AL_GP1_2 is fail\r\n");
				}
		if(buffer[i] == (*(uint32_t*)(AL_GP1_3 + i*4))){
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP1_3 + i*4));
					printf("AL_GP1_3 is OK\r\n");
				}
				else{
					printf("buffer = %d\r\n",buffer[i]);
					printf("checkdata = %d\r\n",*(uint32_t*)(AL_GP1_3 + i*4));
					printf("AL_GP1_3 is fail\r\n");
				}
	}
	return 0;
}




