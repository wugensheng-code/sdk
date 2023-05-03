/*
 * AL9000_ipc.c
 *
 *  Created on: 2022/10/28
 *      Author: yi.zhang
 */

#include "al9000_aximon.h"
#include "demosoc.h"
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"

uint8_t AlAxiMon_ResptimeRecordClean(AXIMON_TypeDef *AXIMON){
	AXIMON -> RESPTIME_CFG |= RESPTIME_CFG_RESPTIME_CLEAN;
	return 0;
}
uint8_t AlAxiMon_ResptimeRecordEnble(AXIMON_TypeDef *AXIMON){
	AXIMON -> RESPTIME_CFG |= RESPTIME_CFG_RESPTIME_ENABLE;
	return 0;
}
/*
 *parmater:QosFicvalue        value range:0-15
 */
uint8_t AlAxiMon_QosValue(AXIMON_TypeDef *AXIMON,uint8_t QosFixValue){
	AXIMON -> QOS_CONFIG |= QosFixValue;
	return 0;
}
/*
 * parmater:QosMaxThreHoldVal	value range:0-15
 * parmater:QosMaxThreHoldVal	value range:0-15
 */
uint8_t AlAxiMon_QosValueRange(AXIMON_TypeDef *AXIMON,uint8_t QosMaxThreHoldVal,uint8_t QosMinThreHoldVal){
	uint8_t value = 0;
	if(QosMaxThreHoldVal < QosMinThreHoldVal){
		value = QosMaxThreHoldVal ;
		QosMaxThreHoldVal = QosMinThreHoldVal;
		QosMinThreHoldVal = value;
		AXIMON -> QOS_THRESHOLD = ((AXIMON -> QOS_THRESHOLD)&(~QOS_MAX	)) | (QosMaxThreHoldVal <<4);
		AXIMON -> QOS_THRESHOLD |= QosMinThreHoldVal;
	}		
	else{
		AXIMON -> QOS_THRESHOLD = ((AXIMON -> QOS_THRESHOLD)&(~QOS_MAX	)) | (QosMaxThreHoldVal <<4);
		AXIMON -> QOS_THRESHOLD |= QosMinThreHoldVal;		
	}
	return 0;
}
uint8_t AlAxiMon_QosCfgEnable(AXIMON_TypeDef *AXIMON,enum QocCfgType type){
	switch(type){
		case 0:
			AXIMON -> QOS_CONFIG |= QOS_CONFIG_FIX_EN;
	
		break;
		case 1:
			AXIMON -> QOS_CONFIG |=  QOS_CONFIG_BYPASS_EN ;
		break;
		case 2:
			AXIMON -> QOS_CONFIG |=	QOS_CONFIG_INTERNAL_EN;
		break;
		default:
			AXIMON -> QOS_CONFIG |=	QOS_CONFIG_FIX_EN;
		break;
	}
	return 0;
}

uint8_t AlAxiMon_SbuparaCountClean(AXIMON_TypeDef *AXIMON){
	AXIMON -> SUBPARA_CONFIG |= SUBPARA_CONFIG_RESPTIME_CLEAN;
	return 0;
}
uint8_t AlAxiMon_SbuparaCountEnable(AXIMON_TypeDef *AXIMON){
	AXIMON -> SUBPARA_CONFIG |= SUBPARA_CONFIG_RESPTIME_ENABLE;
	return 0;
}
uint8_t AlAxiMon_SubparaAwidRange(AXIMON_TypeDef *AXIMON,uint16_t awid_max,uint16_t awid_min){
	uint16_t value = 0;
	if(awid_max < awid_min){
		value = awid_max ;
		awid_max = awid_min;
		awid_min = value;
		AXIMON -> SUBPARA_AWID  = ((AXIMON -> SUBPARA_AWID) & (~AWID_MAX_MASK)) | awid_max <<16;
		AXIMON -> SUBPARA_AWID  = ((AXIMON -> SUBPARA_AWID) & (~AWID_MIN_MASK)) | awid_min;
	}		
	else{
		AXIMON -> SUBPARA_AWID  = ((AXIMON -> SUBPARA_AWID) & (~AWID_MAX_MASK)) | awid_max <<16;
		AXIMON -> SUBPARA_AWID  = ((AXIMON -> SUBPARA_AWID) & (~AWID_MIN_MASK)) | awid_min;
	}
	return 0;
}
uint8_t AlAxiMon_SubparaAridRange(AXIMON_TypeDef *AXIMON,uint16_t arid_max,uint16_t arid_min){
	uint16_t value = 0;
	if(arid_max < arid_min){
		value = arid_max ;
		arid_max = arid_min;
		arid_min = value;
		AXIMON -> SUBPARA_ARID  = ((AXIMON -> SUBPARA_ARID) & (~ARID_MAX_MASK)) | arid_max <<16;
		AXIMON -> SUBPARA_ARID  = ((AXIMON -> SUBPARA_ARID) & (~ARID_MIN_MASK)) | arid_min;
	}
	else{
		AXIMON -> SUBPARA_ARID  = ((AXIMON -> SUBPARA_ARID) & (~ARID_MAX_MASK)) | arid_max <<16;
		AXIMON -> SUBPARA_ARID  = ((AXIMON -> SUBPARA_ARID) & (~ARID_MIN_MASK)) | arid_min;
	}
	return 0;
}

uint8_t AlAxiMon_SubparaAwgidRange(AXIMON_TypeDef *AXIMON,uint8_t Awgid_max,uint8_t Awgid_min){
	uint8_t value = 0;
	if(Awgid_max < Awgid_min){
		value = Awgid_max ;
		Awgid_max = Awgid_min;
		Awgid_min = value;
		AXIMON -> SUBPARA_GRPID  = ((AXIMON -> SUBPARA_GRPID) & (~AWGID_MAX_MASK)) | Awgid_max <<24;
		AXIMON -> SUBPARA_GRPID  = ((AXIMON -> SUBPARA_GRPID) & (~AWGID_MIN_MASK)) | Awgid_min <<8;
	}		
	else{
		AXIMON -> SUBPARA_GRPID  = ((AXIMON -> SUBPARA_GRPID) & (~AWGID_MAX_MASK)) | Awgid_max <<24;
		AXIMON -> SUBPARA_GRPID  = ((AXIMON -> SUBPARA_GRPID) & (~AWGID_MIN_MASK)) | Awgid_min <<8;
	}
	return 0;
}
uint8_t AlAxiMon_SubparaArgidRange(AXIMON_TypeDef *AXIMON,uint8_t Argid_max,uint8_t Argid_min){
	uint8_t value = 0;
	if(Argid_max< Argid_min){
		value = Argid_max ;
		Argid_max = Argid_min;
		Argid_min = value;
		AXIMON -> SUBPARA_GRPID  = ((AXIMON -> SUBPARA_GRPID) & (~ARGID_MAX_MASK)) | Argid_max <<16;
		AXIMON -> SUBPARA_GRPID  = ((AXIMON -> SUBPARA_GRPID) & (~ARGID_MIN_MASK)) | Argid_min ;
	}		
	else{
		AXIMON -> SUBPARA_GRPID  = ((AXIMON -> SUBPARA_GRPID) & (~ARGID_MAX_MASK)) | Argid_max <<16;
		AXIMON -> SUBPARA_GRPID  = ((AXIMON -> SUBPARA_GRPID) & (~ARGID_MIN_MASK)) | Argid_min ;
	}
	return 0;
}
uint8_t AlAxiMon_SubparaAwbtypeRange(AXIMON_TypeDef *AXIMON,uint8_t Awbtype_max,uint8_t Awbtype_min){
	uint8_t value = 0;
	if(Awbtype_max< Awbtype_min){
		value = Awbtype_max ;
		Awbtype_max = Awbtype_min;
		Awbtype_min = value;
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWTYPE_MAX_MASK)) | Awbtype_max <<27;
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWTYPE_MIN_MASK)) | Awbtype_min <<11;
	}	
	else{
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWTYPE_MAX_MASK)) | Awbtype_max <<27;
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWTYPE_MIN_MASK)) | Awbtype_min <<11;
	}
	return 0;
}
uint8_t AlAxiMon_SubparaAwbsizeRange(AXIMON_TypeDef *AXIMON,uint8_t Awbsize_max,uint8_t Awbsize_min){
	uint8_t value = 0;
	if(Awbsize_max< Awbsize_min){
		value = Awbsize_max ;
		Awbsize_max = Awbsize_min;
		Awbsize_min = value;
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWSIZE_MAX_MASK)) | Awbsize_max <<24;
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWSIZE_MIN_MASK)) | Awbsize_min <<8;
	}	
	else{
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWSIZE_MAX_MASK)) | Awbsize_max <<24;
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWSIZE_MIN_MASK)) | Awbsize_min <<8;
	}
	return 0;
}
uint8_t AlAxiMon_SubparaAwblenRange(AXIMON_TypeDef *AXIMON,uint8_t Awlen_max,uint8_t Awlen_min){
	uint8_t value = 0;
	if(Awlen_max< Awlen_min){
		value = Awlen_max ;
		Awlen_max = Awlen_min;
		Awlen_min = value;
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWLEN_MAX_MASK)) | Awlen_max <<16;
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWLEN_MIN_MASK)) | Awlen_min <<0;
	}		
	else{
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWLEN_MAX_MASK)) | Awlen_max <<16;
		AXIMON -> SUBPARA_AWBURST  = ((AXIMON -> SUBPARA_AWBURST)&(~AWLEN_MIN_MASK)) | Awlen_min <<0;
	}
	return 0;
}

uint8_t AlAxiMon_SubparaArbtypeRange(AXIMON_TypeDef *AXIMON,uint8_t Arbtype_max,uint8_t Arbtype_min){
	uint8_t value = 0;
	if(Arbtype_max< Arbtype_min){
		value = Arbtype_max ;
		Arbtype_max = Arbtype_min;
		Arbtype_min = value;
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARTYPE_MAX_MASK)) | Arbtype_max <<27;
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARTYPE_MIN_MASK)) | Arbtype_min <<11;
	}	
	else{
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARTYPE_MAX_MASK)) | Arbtype_max <<27;
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARTYPE_MIN_MASK)) | Arbtype_min <<11;
	}
	return 0;
}
uint8_t AlAxiMon_SubparaArbsizeRange(AXIMON_TypeDef *AXIMON,uint8_t Arbsize_max,uint8_t Arbsize_min){
	uint8_t value = 0;
	if(Arbsize_max< Arbsize_min){
		value = Arbsize_max ;
		Arbsize_max = Arbsize_min;
		Arbsize_min = value;
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARSIZE_MAX_MASK)) | Arbsize_max <<24;
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARSIZE_MIN_MASK)) | Arbsize_min <<8;
	}	
	else{
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARSIZE_MAX_MASK)) | Arbsize_max <<24;
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARSIZE_MIN_MASK)) | Arbsize_min <<8;
	}
	return 0;
}
uint8_t AlAxiMon_SubparaArblenRange(AXIMON_TypeDef *AXIMON,uint8_t Arlen_max,uint8_t Arlen_min){
	uint8_t value = 0;
	if(Arlen_max< Arlen_min){
		value = Arlen_max ;
		Arlen_max = Arlen_min;
		Arlen_min = value;
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARLEN_MAX_MASK)) | Arlen_max <<16;
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARLEN_MIN_MASK)) | Arlen_min <<0;
	}		
	else{
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARLEN_MAX_MASK)) | Arlen_max <<16;
		AXIMON -> SUBPARA_ARBURST  = ((AXIMON -> SUBPARA_ARBURST)&(~ARLEN_MIN_MASK)) | Arlen_min <<0;
	}
	return 0;
}


uint8_t AlAxiMon_IntrEnable(AXIMON_TypeDef *AXIMON){
	AXIMON -> INTR |= INTR_INTR_ENABLE;
	return 0;
}
uint8_t AlAxiMon_IntrDisable(AXIMON_TypeDef *AXIMON){
	AXIMON -> INTR = AXIMON -> INTR &~ INTR_INTR_ENABLE;
	return 0;
}
uint8_t AlAxiMon_IntrClear(AXIMON_TypeDef *AXIMON){
	AXIMON -> INTR |= INTR_INTR_CLEAR;
	return 0;
}

uint8_t AlAxiMon_IntrTimeOutMask(AXIMON_TypeDef *AXIMON){
	AXIMON -> INTR |=INTR_INTR_MASK_TIMEOUT;
	return 0;
}
uint8_t AlAxiMon_IntrTimeOutUnMask(AXIMON_TypeDef *AXIMON){
	AXIMON -> INTR =AXIMON -> INTR &~ INTR_INTR_MASK_TIMEOUT;
	return 0;
}
uint8_t AlAxiMon_IntrRespMask(AXIMON_TypeDef *AXIMON){
	AXIMON -> INTR |=INTR_INTR_MASK_RESP;
	return 0;
}
uint8_t AlAxiMon_IntrRespUnMask(AXIMON_TypeDef *AXIMON){
	AXIMON -> INTR =AXIMON -> INTR &~ INTR_INTR_MASK_RESP;
	return 0;
}
uint8_t AlAxiMon_SetBandWidthCnt(AXIMON_TypeDef *AXIMON,uint32_t cnt){
	AXIMON -> BANDWIDTH_TMWD =cnt;
	return 0;
}
uint32_t AlAxiMon_GetBandWidthCnt(AXIMON_TypeDef *AXIMON){
	return AXIMON -> BANDWIDTH_TMWD;
}
uint32_t AlAxiMon_GetSubpara(AXIMON_TypeDef *AXIMON,enum AXIMONSubpara type){
	uint32_t value = 0;
	switch(type){
		case 0:
			value = GET_BITS(AXIMON -> SUBPARA_AWID,0,31);
			return value;
		break;
		case 1:
			value = GET_BITS(AXIMON -> SUBPARA_GRPID,0,31);
			return value;
		break;
		case 2:
			value = GET_BITS(AXIMON -> SUBPARA_AWBURST,0,31);
			return value;
		break;
		case 3:
			value = GET_BITS(AXIMON -> SUBPARA_ARID,0,31);
			return value;
		break;
		case 4:
			value = GET_BITS(AXIMON -> SUBPARA_ARBURST,0,31);
			return value;
		break;
		default:
			value = GET_BITS(AXIMON -> SUBPARA_AWID,0,31);
			return value;
		break;
	}
	return value;
}
uint32_t AlAxiMon_GetMonValue(AXIMON_TypeDef *AXIMON,enum AXIMONParm type){
	uint32_t value = 0;
	switch(type){
		case 0:
			value = GET_BITS(AXIMON -> QOS_RECORD,4,7);
			return value;
		break;
		case 1:
			value = GET_BITS(AXIMON -> QOS_RECORD,0,3);
			return value;			
		break;
		case 2:
			value = GET_BITS(AXIMON -> AWRESPTIME_MAX,0,31);
			return value;		
		break;
		case 3:
			value = GET_BITS(AXIMON -> AWRESPTIME_ACC_H,0,31);
			return value;		
		break;
		case 4:
			value = GET_BITS(AXIMON -> AWRESPTIME_ACC_L,0,31);
			return value;		
		break;
		case 5:
			value = GET_BITS(AXIMON -> AWRESPTIME_CNT,0,31);
			return value;		
		break;
		case 6:
			value = GET_BITS(AXIMON -> ARRESPTIME_MAX,0,31);
			return value;		
		break;
		case 7:
			value = GET_BITS(AXIMON -> ARRESPTIME_ACC_H,0,31);
			return value;		
		break;
		case 8:
			value = GET_BITS(AXIMON -> ARRESPTIME_ACC_L,0,31);
			return value;		
		break;
		case 9:
			value = GET_BITS(AXIMON -> ARRESPTIME_CNT,0,31);
			return value;		
		break;
		case 10:
			value = GET_BITS(AXIMON -> BANDWIDTH_WR,0,31);
			return value;		
		break;
		case 11:
			value = GET_BITS(AXIMON -> BANDWIDTH_RD,0,3);
			return value;		
		break;
		default:
			value = GET_BITS(AXIMON -> QOS_RECORD,0,3);
			return value;	
		break;
	}
	return value;	
}
uint32_t AlAxiMon_GetMonCountValue(AXIMON_TypeDef *AXIMON,enum CountParm type){
	uint32_t value = 0;
	switch(type){
		case 0:
			value = GET_BITS(AXIMON -> CNT_AWCMD,0,31);
			return value;
		break;
		case 1:
			value = GET_BITS(AXIMON -> CNT_AWBTYPE,0,31);
			return value;			
		break;
		case 2:
			value = GET_BITS(AXIMON -> CNT_AWBLEN,0,31);
			return value;		
		break;
		case 3:
			value = GET_BITS(AXIMON -> CNT_AWBSIZE,0,31);
			return value;		
		break;
		case 4:
			value = GET_BITS(AXIMON -> CNT_AWID,0,31);
			return value;		
		break;
		case 5:
			value = GET_BITS(AXIMON -> CNT_AWGID,0,31);
			return value;		
		break;
		case 6:
			value = GET_BITS(AXIMON -> CNT_ARCMD,0,31);
			return value;		
		break;
		case 7:
			value = GET_BITS(AXIMON -> CNT_ARBTYPE,0,31);
			return value;		
		break;
		case 8:
			value = GET_BITS(AXIMON -> CNT_ARBLEN,0,31);
			return value;		
		break;
		case 9:
			value = GET_BITS(AXIMON -> CNT_ARBSIZE,0,31);
			return value;		
		break;
		case 10:
			value = GET_BITS(AXIMON -> CNT_ARID,0,31);
			return value;		
		break;
		case 11:
			value = GET_BITS(AXIMON -> CNT_ARGID,0,31);
			return value;		
		break;
		default:
			value = GET_BITS(AXIMON -> CNT_AWCMD,0,31);
			return value;	
		break;
	}
	return value;	

	
}
uint32_t AlAxiMon_GetMonDebugValue(AXIMON_TypeDef *AXIMON,enum DebugParm type){
	uint32_t value = 0;
	switch(type){
		case 0:
			value = GET_BIT(AXIMON -> DEBUG_FLAGINFO,21);
			return value;
		break;
		case 1:
			value = GET_BITS(AXIMON -> DEBUG_FLAGINFO,18,20);
			return value;			
		break;
		case 2:
			value = GET_BITS(AXIMON -> DEBUG_FLAGINFO,10,17);
			return value;		
		break;
		case 3:
			value = GET_BITS(AXIMON -> DEBUG_FLAGINFO,8,9);
			return value;		
		break;
		case 4:
			value = GET_BITS(AXIMON -> DEBUG_FLAGINFO,0,7);
			return value;		
		break;
		case 5:
			value = GET_BITS(AXIMON -> DEBUG_ADDR_L,0,31);
			return value;		
		break;
		case 6:
			value = GET_BITS(AXIMON -> DEBUG_ID,0,31);
			return value;		
		break;
		default:
			value = GET_BITS(AXIMON -> DEBUG_ID,0,31);
			return value;	
		break;
	}
	return value;	
}
void AlAxiMon_SetTimeOutValue(AXIMON_TypeDef *AXIMON,uint32_t TimeoutThre){
	AXIMON->DEBUG_TIMEOUT_THD = TimeoutThre;
}
uint32_t AlAxiMon_GetMonTimeOutValue(AXIMON_TypeDef *AXIMON){
	return GET_BITS(AXIMON -> DEBUG_TIMEOUT_FLAG,0,31);
}
void AlAxiMon_ClearDebugRecord(AXIMON_TypeDef *AXIMON)
{
	AXIMON->DEBUG_FLAGINFO = AXIMON->DEBUG_FLAGINFO &~ (0x1 << 22);
}
void AlAxiMon_GlobalEnable()
{
	*(unsigned int*)(0xF88014B8) = 0x1;
}
void AlAxiMon_GlobalDisable()
{
	*(unsigned int*)(0xF88014B8) = 0x0;
}
