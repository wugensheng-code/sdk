/****************************************************************************/
/**
* @file non-secure_m2m test
*
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "nuclei_sdk_soc.h"
#include "al9000_aximon.h"
#define XMON_CNT_PRINT
#define XMON_DEBUG_PRINT
uint32_t num=0;
#define REG_WRITE(reg_address, reg_wdata)  *(unsigned int*)(reg_address) = reg_wdata
#define REG_READ(reg_address)  *(unsigned int*)reg_address
void config_monitor(AXIMON_TypeDef *AXIMON, uint16_t id_max, uint16_t id_min, uint16_t gid_max, uint16_t gid_min, uint32_t thre)
{
	AlAxiMon_ResptimeRecordClean(AXIMON);
	AlAxiMon_ResptimeRecordEnble(AXIMON);
	AlAxiMon_QosValue(AXIMON,XQOS_MAX);
	AlAxiMon_QosValueRange(AXIMON,XQOS_MAX,0);
	AlAxiMon_QosCfgEnable(AXIMON,QosInternal);
	AlAxiMon_SbuparaCountClean(AXIMON);
	AlAxiMon_SubparaAwidRange(AXIMON,id_max,id_min);
	AlAxiMon_SubparaAridRange(AXIMON,id_max,id_min);
	AlAxiMon_SubparaAwgidRange(AXIMON,gid_max,gid_min);
	AlAxiMon_SubparaArgidRange(AXIMON,gid_max,gid_min);
	AlAxiMon_SubparaAwbtypeRange(AXIMON,XTYPE_MAX,0);//incr
	AlAxiMon_SubparaAwbsizeRange(AXIMON,XSIZE_MAX,0);//2^2
	AlAxiMon_SubparaAwblenRange(AXIMON,XLEN_MAX,0);
	AlAxiMon_SubparaArbtypeRange(AXIMON,XTYPE_MAX,0);
	AlAxiMon_SubparaArbsizeRange(AXIMON,XSIZE_MAX,0);
	AlAxiMon_SubparaArblenRange(AXIMON,XLEN_MAX,0);
	AlAxiMon_SbuparaCountEnable(AXIMON);
	AlAxiMon_SetTimeOutValue(AXIMON,thre);
}
void printf_monitor(AXIMON_TypeDef *AXIMON)
{
	printf("CASE NUM = %d\r\n",num);
#ifdef XMON_QOS_PRINT
  	printf("AXIMON awqos_record         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awqos_record      ));
  	printf("AXIMON arqos_record         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arqos_record      ));
#endif
#ifdef XMON_RESPTIME_PRINT
  	printf("AXIMON awresptime_max       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_max    ));
  	printf("AXIMON awresptime_acc_h     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_acc_h  ));
  	printf("AXIMON awresptime_acc_l     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_acc_l  ));
  	printf("AXIMON awresptime_cnt       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,awresptime_cnt    ));
  	printf("AXIMON arresptime_max       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_max    ));
  	printf("AXIMON arresptime_acc_h     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_acc_h  ));
  	printf("AXIMON arresptime_acc_l     = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_acc_l  ));
  	printf("AXIMON arresptime_cnt       = %x\r\n",AlAxiMon_GetMonValue(AXIMON,arresptime_cnt    ));
  	printf("AXIMON bandwidth_wr         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,bandwidth_wr      ));
  	printf("AXIMON bandwidth_rd         = %x\r\n",AlAxiMon_GetMonValue(AXIMON,bandwidth_rd      ));
#endif
#ifdef XMON_CNT_PRINT
 	printf("AXIMON cnt_awcmd  	        = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awcmd    ));
	printf("AXIMON cnt_awbtype	        = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awbtype  ));
	printf("AXIMON cnt_awblen	        = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awblen   ));
	printf("AXIMON cnt_awbsize          = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awbsize  ));
	printf("AXIMON cnt_awid             = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awid     ));
	printf("AXIMON cnt_awgid            = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_awgid    ));
	printf("AXIMON cnt_arcmd            = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arcmd    ));
	printf("AXIMON cnt_arbtype          = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arbtype  ));
	printf("AXIMON cnt_arblen           = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arblen   ));
	printf("AXIMON cnt_arbsize          = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arbsize  ));
	printf("AXIMON cnt_arid             = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_arid     ));
	printf("AXIMON cnt_argid            = %x\r\n",AlAxiMon_GetMonCountValue(AXIMON,cnt_argid    ));
#endif
#ifdef XMON_DEBUG_PRINT
	printf("AXIMON debug_access_type	= %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_access_type	));
	printf("AXIMON debug_burst_size	    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_burst_size		));
	printf("AXIMON debug_burst_length   = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_burst_length	));
	printf("AXIMON debug_burst_type	    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_burst_type		));
	printf("AXIMON debug_addr_h		    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_addr_h			));
	printf("AXIMON debug_addr_l		    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_addr_l			));
	printf("AXIMON debug_id			    = %x\r\n",AlAxiMon_GetMonDebugValue(AXIMON,debug_id				));
#endif
 	num ++;
}
