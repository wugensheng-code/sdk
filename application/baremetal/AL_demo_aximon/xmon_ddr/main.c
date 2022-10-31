#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_aximon.h"
#include "al9000_mpu.h"
#include "AL_dmac.h"
#include "al9000_ipc.h"
#define GP0_S0_MEMBASE 0x80000000
#define GP0_S1_MEMBASE 0xA0000000
//#define DDR_0 0x01201000
//#define DDR_1 0x01202000

#define DDR_0 0x44a00000
#define DDR_1 0x44a00100
//AL_SX2X_M0
//#define DDR_0 0x80000000
//#define DDR_1 0x80000100
//AL_SX2X_M1
//#define DDR_0 0xA0000000
//#define DDR_1 0xA0000100
volatile uint16_t count = 0;
#define check_len 8
uint32_t num;
void config_monitor(AXIMON_TypeDef *AXIMON,uint16_t grp_id)
{
	AlAxiMon_ResptimeRecordClean(AXIMON);
	AlAxiMon_ResptimeRecordEnble(AXIMON);
	AlAxiMon_QosValue(AXIMON,15);
	AlAxiMon_QosValueRange(AXIMON,15,1);
	AlAxiMon_QosCfgEnable(AXIMON,QosInternal);
	AlAxiMon_SbuparaCountClean(AXIMON);
	AlAxiMon_SubparaAwidRange(AXIMON,grp_id,grp_id);
	AlAxiMon_SubparaAridRange(AXIMON,grp_id,grp_id);
	AlAxiMon_SubparaAwgidRange(AXIMON,grp_id,grp_id);
	AlAxiMon_SubparaArgidRange(AXIMON,grp_id,grp_id);
	AlAxiMon_SubparaAwbtypeRange(AXIMON,15,0);
	AlAxiMon_SubparaAwbsizeRange(AXIMON,15,0);
	AlAxiMon_SubparaAwblenRange(AXIMON,15,0);
	AlAxiMon_SubparaArbtypeRange(AXIMON,15,0);
	AlAxiMon_SubparaArbsizeRange(AXIMON,15,0);
	AlAxiMon_SubparaArblenRange(AXIMON,15,0);
	AlAxiMon_SbuparaCountEnable(AXIMON);

}
void Debug_Printf(AXIMON_TypeDef *AXIMON)
{
	printf("AXIMON debug_access_type	= %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_access_type	));
	printf("AXIMON debug_burst_size	    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_burst_size		));
	printf("AXIMON debug_burst_length   = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_burst_length	));
	printf("AXIMON debug_burst_type	    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_burst_type		));
	printf("AXIMON debug_addr_h		    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_addr_h			));
	printf("AXIMON debug_addr_l		    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_addr_l			));
	printf("AXIMON debug_id			    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_id				));
}
void Debug_Mon()
{
	Debug_Printf(AL_DDR_S0);
	Debug_Printf(AL_DDR_S1);
	Debug_Printf(AL_DDR_S2);
	Debug_Printf(AL_DDR_S3);
	Debug_Printf(AL_OCM_S2);
	AlAxiMon_ClearDebugRecord(AL_DDR_S0);
	AlAxiMon_ClearDebugRecord(AL_DDR_S1);
	AlAxiMon_ClearDebugRecord(AL_DDR_S2);
	AlAxiMon_ClearDebugRecord(AL_DDR_S3);
	AlAxiMon_ClearDebugRecord(AL_OCM_S2);
}
void Xmonitor_handler()
{
	Debug_Mon();
	AlAxiMon_IntrClear(AL_DDR_S0);
	AlAxiMon_IntrClear(AL_DDR_S1);
	AlAxiMon_IntrClear(AL_DDR_S2);
	AlAxiMon_IntrClear(AL_DDR_S3);
	AlAxiMon_IntrClear(AL_OCM_S2);
}
void SetupIntr()
{
	AlAxiMon_IntrMask  (AL_DDR_S0);
	AlAxiMon_IntrEnable(AL_DDR_S0);
	AlAxiMon_IntrMask  (AL_DDR_S1);
	AlAxiMon_IntrEnable(AL_DDR_S1);
	AlAxiMon_IntrMask  (AL_DDR_S2);
	AlAxiMon_IntrEnable(AL_DDR_S2);
	AlAxiMon_IntrMask  (AL_DDR_S3);
	AlAxiMon_IntrEnable(AL_DDR_S3);
	AlAxiMon_IntrMask  (AL_OCM_S2);
	AlAxiMon_IntrEnable(AL_OCM_S2);
	ECLIC_Register_IRQ(SOC_INT129_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,Xmonitor_handler);
	__enable_irq();
}
void clear_monitor(AXIMON_TypeDef *AXIMON)
{
	AlAxiMon_ResptimeRecordClean(AXIMON);
	AlAxiMon_SbuparaCountClean(AXIMON);
}
void data_check(uint32_t* pDnAddr)
{
	uint32_t i = 0;
	uint32_t check_buffer[check_len];
	uint32_t buffer[check_len];
	for(i = 0 ; i < check_len ; i++){
		buffer[i] = i;
	}
	write_To_OCM((uint32_t*)buffer,check_len,(uint32_t*)pDnAddr );
	for(i = 0 ; i < check_len ; i++){
		check_buffer[i] =REG32_READ((uint32_t)pDnAddr +i*4);
		if(buffer[i]!=check_buffer[i])
		{
			printf("buffer[%d]=%d,check_buffer[%d]=%d",i,buffer[i],i,check_buffer[i]);
			printf("Check fail %d\r\n",i);
		}
	}
}
void printf_monitor(AXIMON_TypeDef *AXIMON)
{
	printf("CASE NUM = %d\r\n",num);
//  	printf("AXIMON awqos_record         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awqos_record      ));
//  	printf("AXIMON arqos_record         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arqos_record      ));
//  	printf("AXIMON awresptime_max       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_max    ));
//  	printf("AXIMON awresptime_acc_h     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_acc_h  ));
//  	printf("AXIMON awresptime_acc_l     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_acc_l  ));
//  	printf("AXIMON awresptime_cnt       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_cnt    ));
//  	printf("AXIMON arresptime_max       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_max    ));
//  	printf("AXIMON arresptime_acc_h     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_acc_h  ));
//  	printf("AXIMON arresptime_acc_l     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_acc_l  ));
//  	printf("AXIMON arresptime_cnt       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_cnt    ));
//  	printf("AXIMON bandwidth_wr         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,bandwidth_wr      ));
//  	printf("AXIMON bandwidth_rd         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,bandwidth_rd      ));

 	printf("AXIMON cnt_awcmd  	        = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awcmd    ));
//	printf("AXIMON cnt_awbtype	        = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awbtype  ));
//	printf("AXIMON cnt_awblen	        = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awblen   ));
//	printf("AXIMON cnt_awbsize          = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awbsize  ));
	printf("AXIMON cnt_awid             = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awid     ));
	printf("AXIMON cnt_awgid            = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awgid    ));
	printf("AXIMON cnt_arcmd            = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arcmd    ));
//	printf("AXIMON cnt_arbtype          = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arbtype  ));
//	printf("AXIMON cnt_arblen           = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arblen   ));
//	printf("AXIMON cnt_arbsize          = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arbsize  ));
	printf("AXIMON cnt_arid             = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arid     ));

	printf("AXIMON subpara_awid         = %x\r\n",AlAxiMon_GetSubpara(AXIMON,subpara_awid       ));
	printf("AXIMON subpara_grpid        = %x\r\n",AlAxiMon_GetSubpara(AXIMON,subpara_grpid      ));
	printf("AXIMON subpara_arid         = %x\r\n",AlAxiMon_GetSubpara(AXIMON,subpara_arid       ));
 	num ++;
}
void config_case()
{

	config_monitor(AL_MAIN_M0, 8);//0 pass
	config_monitor(AL_MAIN_S1, 8);//1
	config_monitor(AL_DDR_S0 , 8);//2 pass
	config_monitor(AL_DDR_S1 , 8);//3 pass
	config_monitor(AL_DDR_S2 , 8);//4
	config_monitor(AL_DDR_S3 , 8);//5
	config_monitor(AL_HP_S0	 , 8);//6
	config_monitor(AL_HP_S1	 , 8);//7
	config_monitor(AL_SX2X_M0, 8);//8 pass
	config_monitor(AL_SX2X_M1, 8);//9 pass
	config_monitor(AL_SH_M2  , 8);//10
	config_monitor(AL_GP_S0	 , 8);//11
	config_monitor(AL_GP_S1	 , 8);//12
	config_monitor(AL_OCM_S2 , 8);//13

}
void print_case()
{

	printf_monitor(AL_MAIN_M0);
	printf_monitor(AL_MAIN_S1);
	printf_monitor(AL_DDR_S0 );
	printf_monitor(AL_DDR_S1 );
	printf_monitor(AL_DDR_S2 );
	printf_monitor(AL_DDR_S3 );
	printf_monitor(AL_HP_S0	 );
	printf_monitor(AL_HP_S1	 );
	printf_monitor(AL_SX2X_M0);
	printf_monitor(AL_SX2X_M1);
	printf_monitor(AL_SH_M2  );
	printf_monitor(AL_GP_S0	 );
	printf_monitor(AL_GP_S1	 );
	printf_monitor(AL_OCM_S2 );
}
#if 1
int main(){
	num =0;
	REG32_WRITE(0xf8801078,0x00001133);
	REG32_WRITE(0xf8801074,0x000073f0);
	REG32_WRITE(0xf8800080,0x0);
	REG32_WRITE(0xf840e004,0x0);
	REG32_WRITE(0xf840f004,0x0);
	SetupIntr();
	config_case();
//	data_check(DDR_BASE );
//	print_case();
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
//		else if((AlAxiMon_GetMonCountValue(AL_MAIN_M0,cnt_awcmd)&~MAIN_w0)|(AlAxiMon_GetMonCountValue(AL_MAIN_M0,cnt_arcmd)&~MAIN_r0))
//		{
//			printf("Monitor AL_MAIN_M0\r\n");
//			printf_monitor(AL_MAIN_M0 );
//			MAIN_w0=AlAxiMon_GetMonCountValue(AL_MAIN_M0,cnt_awcmd);
//			MAIN_r0=AlAxiMon_GetMonCountValue(AL_MAIN_M0,cnt_arcmd);
//		}
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
#endif
