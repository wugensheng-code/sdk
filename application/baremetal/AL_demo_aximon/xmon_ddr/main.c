#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_aximon.h"
#define REG_WRITE(reg_address, reg_wdata)  *(unsigned int*)(reg_address) = reg_wdata
#define REG_READ(reg_address)  *(unsigned int*)reg_address
int main(){
	REG_WRITE(0xf8801078,0x00001133);
	REG_WRITE(0xf8801074,0x000073f0);
	REG_WRITE(0xf8800080,0x0);
	REG_WRITE(0xf840e004,0x0);
	REG_WRITE(0xf840f004,0x0);
	config_monitor(AL_GP_S0,XID_MAX,0,XID_MAX,0,0x20);
	config_monitor(AL_GP_S1,XID_MAX,0,XID_MAX,0,0x20);
	config_monitor(AL_HP_S0,XID_MAX,0,XID_MAX,0,0x20);
	config_monitor(AL_HP_S1,XID_MAX,0,XID_MAX,0,0x20);
	config_monitor(AL_DDR_S0,XID_MAX,0,XID_MAX,0,0x20);
	config_monitor(AL_DDR_S1,XID_MAX,0,XID_MAX,0,0x20);
	config_monitor(AL_DDR_S2,XID_MAX,0,XID_MAX,0,0x20);
	config_monitor(AL_DDR_S3,XID_MAX,0,XID_MAX,0,0x20);

	uint32_t DDR_w0 = 0;
	uint32_t DDR_w1 = 0;
	uint32_t DDR_w2 = 0;
	uint32_t DDR_w3 = 0;
	uint32_t DDR_r0 = 0;
	uint32_t DDR_r1 = 0;
	uint32_t DDR_r2 = 0;
	uint32_t DDR_r3 = 0;
	uint32_t MAIN_w0 = 0;
	uint32_t MAIN_w1 = 0;
	uint32_t MAIN_r0 = 0;
	uint32_t MAIN_r1 = 0;
	uint32_t GP_w0 = 0;
	uint32_t GP_w1 = 0;
	uint32_t GP_r0 = 0;
	uint32_t GP_r1 = 0;
	uint32_t HP_w0 = 0;
	uint32_t HP_w1 = 0;
	uint32_t HP_r0 = 0;
	uint32_t HP_r1 = 0;

	while(1)
	{
		if((AlAxiMon_GetMonCountValue(AL_DDR_S0,cnt_awcmd)&~DDR_w0)|(AlAxiMon_GetMonCountValue(AL_DDR_S0,cnt_arcmd)&~DDR_r0))
		{
			printf("Monitor DDR S0\r\n");
			printf_monitor(AL_DDR_S0 );
			DDR_w0=AlAxiMon_GetMonCountValue(AL_DDR_S0,cnt_awcmd);
			DDR_r0=AlAxiMon_GetMonCountValue(AL_DDR_S0,cnt_arcmd);
		}
		else if((AlAxiMon_GetMonCountValue(AL_DDR_S1,cnt_awcmd)&~DDR_w1)|(AlAxiMon_GetMonCountValue(AL_DDR_S1,cnt_arcmd)&~DDR_r1))
		{
			printf("Monitor DDR S1\r\n");
			printf_monitor(AL_DDR_S1 );
			DDR_w1=AlAxiMon_GetMonCountValue(AL_DDR_S1,cnt_awcmd);
			DDR_r1=AlAxiMon_GetMonCountValue(AL_DDR_S1,cnt_arcmd);
		}
		else if((AlAxiMon_GetMonCountValue(AL_DDR_S2,cnt_awcmd)&~DDR_w2)|(AlAxiMon_GetMonCountValue(AL_DDR_S2,cnt_arcmd)&~DDR_r2))
		{
			printf("Monitor DDR S2\r\n");
			printf_monitor(AL_DDR_S2 );
			DDR_w2=AlAxiMon_GetMonCountValue(AL_DDR_S2,cnt_awcmd);
			DDR_r2=AlAxiMon_GetMonCountValue(AL_DDR_S2,cnt_arcmd);
		}
		else if((AlAxiMon_GetMonCountValue(AL_DDR_S3,cnt_awcmd)&~DDR_w3)|(AlAxiMon_GetMonCountValue(AL_DDR_S3,cnt_arcmd)&~DDR_r3))
		{
			printf("Monitor DDR S3\r\n");
			printf_monitor(AL_DDR_S3 );
			DDR_w3=AlAxiMon_GetMonCountValue(AL_DDR_S3,cnt_awcmd);
			DDR_r3=AlAxiMon_GetMonCountValue(AL_DDR_S3,cnt_arcmd);
		}
		else if((AlAxiMon_GetMonCountValue(AL_GP_S0,cnt_awcmd)&~GP_w0)|(AlAxiMon_GetMonCountValue(AL_GP_S0,cnt_arcmd)&~GP_r0))
		{
			printf("Monitor AL_GP_S0\r\n");
			printf_monitor(AL_GP_S0 );
			GP_w0=AlAxiMon_GetMonCountValue(AL_GP_S0,cnt_awcmd);
			GP_r0=AlAxiMon_GetMonCountValue(AL_GP_S0,cnt_arcmd);
		}
		else if((AlAxiMon_GetMonCountValue(AL_GP_S1,cnt_awcmd)&~GP_w1)|(AlAxiMon_GetMonCountValue(AL_GP_S1,cnt_arcmd)&~GP_r1))
		{
			printf("Monitor AL_GP_S1\r\n");
			printf_monitor(AL_GP_S1 );
			GP_w1=AlAxiMon_GetMonCountValue(AL_GP_S1,cnt_awcmd);
			GP_r1=AlAxiMon_GetMonCountValue(AL_GP_S1,cnt_arcmd);
		}
		else if((AlAxiMon_GetMonCountValue(AL_HP_S0,cnt_awcmd)&~(HP_w0))|(AlAxiMon_GetMonCountValue(AL_HP_S0,cnt_arcmd)&~(HP_r0)))
		{
			printf("Monitor AL_HP_S0\r\n");
			printf_monitor(AL_HP_S0 );
			HP_w0=AlAxiMon_GetMonCountValue(AL_HP_S0,cnt_awcmd);
			HP_r1=AlAxiMon_GetMonCountValue(AL_HP_S0,cnt_arcmd);
		}
		else if((AlAxiMon_GetMonCountValue(AL_HP_S1,cnt_awcmd)&~HP_w1)|(AlAxiMon_GetMonCountValue(AL_HP_S1,cnt_arcmd)&~HP_r1))
		{
			printf("Monitor AL_HP_S1\r\n");
			printf_monitor(AL_HP_S1 );
			HP_w1=AlAxiMon_GetMonCountValue(AL_HP_S1,cnt_awcmd);
			HP_r1=AlAxiMon_GetMonCountValue(AL_HP_S1,cnt_arcmd);
		}
		else if((AlAxiMon_GetMonCountValue(AL_MAIN_M0,cnt_awcmd)&~MAIN_w0)|(AlAxiMon_GetMonCountValue(AL_MAIN_M0,cnt_arcmd)&~MAIN_r0))
		{
			printf("Monitor AL_MAIN_M0\r\n");
			printf_monitor(AL_MAIN_M0 );
			MAIN_w0=AlAxiMon_GetMonCountValue(AL_MAIN_M0,cnt_awcmd);
			MAIN_r0=AlAxiMon_GetMonCountValue(AL_MAIN_M0,cnt_arcmd);
		}
//		else if((AlAxiMon_GetMonCountValue(AL_MAIN_S1,cnt_awcmd)&~MAIN_w1)|(AlAxiMon_GetMonCountValue(AL_MAIN_S1,cnt_arcmd)&~MAIN_r1))
//		{
//			printf("Monitor AL_MAIN_S1\r\n");
//			printf_monitor(AL_MAIN_S1 );
//			MAIN_w1=AlAxiMon_GetMonCountValue(AL_MAIN_S1,cnt_awcmd);
//			MAIN_r1=AlAxiMon_GetMonCountValue(AL_MAIN_S1,cnt_arcmd);
//		}
	}
	return 0;
}
