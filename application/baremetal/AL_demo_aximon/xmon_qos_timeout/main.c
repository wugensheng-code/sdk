/****************************************************************************/
/**
* @file non-secure_m2m test
*
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "nuclei_sdk_soc.h"
#include "xdmaps.h"
#include "al9000_ipc.h"
#include "al9000_aximon.h"

int main(void)
{
	uint32_t case_num=0;
	uint32_t qos_rd_max,qos_wr_max,qos_rd_min,qos_wr_min;
	qos_wr_max = 0;
	qos_rd_max = 0;
	qos_wr_min = 0xf;
	qos_rd_min = 0xf;
	config_monitor(AL_OCM_S2, XID_MAX, 0, XID_MAX, 0, XQOS_MAX , 0x1f);
	while(1)
	{
		if(case_num < 10)
		{
			if(AlAxiMon_GetMonValue(AL_OCM_S2,awqos_record) > qos_wr_max)
				qos_wr_max = AlAxiMon_GetMonValue(AL_OCM_S2,awqos_record);
			else
				qos_wr_max =qos_wr_max;
			if(AlAxiMon_GetMonValue(AL_OCM_S2,arqos_record) > qos_rd_max)
				qos_rd_max = AlAxiMon_GetMonValue(AL_OCM_S2,arqos_record);
			else
				qos_rd_max =qos_rd_max;
		}
			
		else
			break;
		case_num ++;
	}
	config_monitor(AL_OCM_S2, XID_MAX, 0, XID_MAX, 0, XQOS_MAX , 0x20);
	case_num = 0;
	while(1)
	{
		if(case_num < 10)
		{
			if(AlAxiMon_GetMonValue(AL_OCM_S2,awqos_record) > qos_wr_min)
			{
				qos_wr_min = qos_wr_min;
			}
			else
				qos_wr_min = AlAxiMon_GetMonValue(AL_OCM_S2,awqos_record);
			if(AlAxiMon_GetMonValue(AL_OCM_S2,arqos_record) > qos_rd_min)
			{
				qos_rd_min = qos_rd_min;
			}
			else
				qos_rd_min = AlAxiMon_GetMonValue(AL_OCM_S2,arqos_record);
		}
		else
			break;
		case_num ++;
	}
	if(qos_wr_max > 1 && qos_rd_max > 1 && qos_wr_min <= 1 && qos_rd_min <= 1)
	{
		printf("[AUTOTEST]:[XMON]:[XMON_QOS_TIMEOUT]:[PASS]\r\n");
		// printf("qos_wr_max = %x\r\n", qos_wr_max);
		// printf("qos_rd_max = %x\r\n", qos_rd_max);
		// printf("qos_wr_max = %x\r\n", qos_wr_min);
		// printf("qos_rd_max = %x\r\n", qos_rd_min);
		return XST_SUCCESS;
	}
	else{
		printf("[AUTOTEST]:[XMON]:[XMON_QOS_TIMEOUT]:[FAIL]\r\n");
		return XST_FAILURE;
	}
}

