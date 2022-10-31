/*
 * AL_AXIMON.H
 *
 *  Created on: 2022/10/28
 *      Author: yi.zhang
 */
#ifndef _AL9000_AXIMON_H
#define _AL9000_AXIMON_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "demosoc.h"



/***************************bie define***********************/

#define RESPTIME_CFG_RESPTIME_ENABLE BIT(0)
#define RESPTIME_CFG_RESPTIME_CLEAN BIT(1)
#define SUBPARA_CONFIG_RESPTIME_ENABLE BIT(0)
#define SUBPARA_CONFIG_RESPTIME_CLEAN BIT(1)
#define QOS_CONFIG_FIX_EN   (uint32_t)(0x00000000)
#define QOS_CONFIG_BYPASS_EN BIT(4)
#define QOS_CONFIG_INTERNAL_EN BIT(5)
#define QOS_MAX			(uint32_t)(0x000000f0)

#define ID_MASK 0xFFFF
#define GID_MASK 0xFF
#define TYPE_MASK 0x3
#define SIZE_MASK 0x7
#define LEN_MASK 0xFF

#define AWID_MAX_MASK 	(ID_MASK << 16)
#define AWID_MIN_MASK 	(ID_MASK << 0)
#define ARID_MAX_MASK 	(ID_MASK << 16)
#define ARID_MIN_MASK 	(ID_MASK << 0)
#define AWGID_MAX_MASK	(GID_MASK << 24)
#define AWGID_MIN_MASK	(GID_MASK << 16)
#define ARGID_MAX_MASK	(GID_MASK << 8)
#define ARGID_MIN_MASK	(GID_MASK << 0)
#define AWTYPE_MAX_MASK (TYPE_MASK << 27)
#define AWSIZE_MAX_MASK (SIZE_MASK << 24)
#define AWLEN_MAX_MASK 	(LEN_MASK << 16)
#define AWTYPE_MIN_MASK (TYPE_MASK << 11)
#define AWSIZE_MIN_MASK (SIZE_MASK << 8)
#define AWLEN_MIN_MASK 	(LEN_MASK << 0)
#define ARTYPE_MAX_MASK (TYPE_MASK << 27)
#define ARSIZE_MAX_MASK (SIZE_MASK << 24)
#define ARLEN_MAX_MASK 	(LEN_MASK << 16)
#define ARTYPE_MIN_MASK (TYPE_MASK << 11)
#define ARSIZE_MIN_MASK (SIZE_MASK << 8)
#define ARLEN_MIN_MASK 	(LEN_MASK << 0)

#define INTR_INTR_ENABLE BIT(4)
#define INTR_INTR_MASK   BIT(3)
#define INTR_INTR_CLEAR   BIT(2)

#define XID_MAX 		0xFFFF
#define XSIZE_MAX 		0x7
#define XLEN_MAX 		0xFF
#define XTYPE_MAX 		0x3
#define XQOS_MAX 		0xF
#define GROUPID_CSU 	0
#define GROUPID_RPU 	1
#define GROUPID_APU 	2
#define GROUPID_NPU 	3
#define GROUPID_JPU 	4
#define GROUPID_DMAX 	5
#define GROUPID_HP0 	6
#define GROUPID_HP1 	7
#define GROUPID_GP0 	8
#define GROUPID_GP1 	9
#define GROUPID_HBUS 	10

enum AXIMONParm{
	awqos_record,
	arqos_record,
	awresptime_max,
	awresptime_acc_h,
	awresptime_acc_l,
	awresptime_cnt,
	arresptime_max,
	arresptime_acc_h,
	arresptime_acc_l,
	arresptime_cnt,
	bandwidth_wr,
	bandwidth_rd,	
};
enum AXIMONSubpara{
	subpara_awid,
	subpara_grpid,
	subpara_awburst,
	subpara_arid,
	subpara_arburst,
};
enum CountParm{
	cnt_awcmd,
	cnt_awbtype,
	cnt_awblen,
	cnt_awbsize,
	cnt_awid,
	cnt_awgid,
	cnt_arcmd,
	cnt_arbtype,
	cnt_arblen,
	cnt_arbsize,
	cnt_arid,
	cnt_argid,	
};
enum DebugParm{
	debug_access_type,
	debug_burst_size,
	debug_burst_length,
	debug_burst_type,
	debug_addr_h,
	debug_addr_l,
	debug_id,	
};
enum QocCfgType{
	QosFixVal,
	QosBypass,
	QosInternal,
};
uint8_t AlAxiMon_ResptimeRecordClean(AXIMON_TypeDef *AXIMON);
uint8_t AlAxiMon_ResptimeRecordEnble(AXIMON_TypeDef *AXIMON);
uint8_t AlAxiMon_QosValue(AXIMON_TypeDef *AXIMON,uint8_t QosFixValue);
uint8_t AlAxiMon_QosValueRange(AXIMON_TypeDef *AXIMON,uint8_t QosMaxThreHoldVal,uint8_t QosMinThreHoldVal);
uint8_t AlAxiMon_QosCfgEnable(AXIMON_TypeDef *AXIMON,enum QocCfgType type);
uint8_t AlAxiMon_SbuparaCountClean(AXIMON_TypeDef *AXIMON);
uint8_t AlAxiMon_SbuparaCountEnable(AXIMON_TypeDef *AXIMON);
uint8_t AlAxiMon_SubparaAwidRange(AXIMON_TypeDef *AXIMON,uint16_t awid_max,uint16_t awid_min);
uint8_t AlAxiMon_SubparaAridRange(AXIMON_TypeDef *AXIMON,uint16_t arid_max,uint16_t arid_min);
uint8_t AlAxiMon_SubparaAwgidRange(AXIMON_TypeDef *AXIMON,uint8_t Awgid_max,uint8_t Awgid_min);
uint8_t AlAxiMon_SubparaArgidRange(AXIMON_TypeDef *AXIMON,uint8_t Argid_max,uint8_t Argid_min);
uint8_t AlAxiMon_SubparaAwbtypeRange(AXIMON_TypeDef *AXIMON,uint8_t Awbtype_max,uint8_t Awbtype_min);
uint8_t AlAxiMon_SubparaAwbsizeRange(AXIMON_TypeDef *AXIMON,uint8_t Awbsize_max,uint8_t Awbsize_min);
uint8_t AlAxiMon_SubparaAwblenRange(AXIMON_TypeDef *AXIMON,uint8_t Awlen_max,uint8_t Awlen_min);
uint8_t AlAxiMon_SubparaArbtypeRange(AXIMON_TypeDef *AXIMON,uint8_t Arbtype_max,uint8_t Arbtype_min);
uint8_t AlAxiMon_SubparaArbsizeRange(AXIMON_TypeDef *AXIMON,uint8_t Arbsize_max,uint8_t Arbsize_min);
uint8_t AlAxiMon_SubparaArblenRange(AXIMON_TypeDef *AXIMON,uint8_t Arlen_max,uint8_t Arlen_min);
uint8_t AlAxiMon_IntrEnable(AXIMON_TypeDef *AXIMON);
uint8_t AlAxiMon_IntrClear(AXIMON_TypeDef *AXIMON);
uint8_t AlAxiMon_IntrMask(AXIMON_TypeDef *AXIMON);
uint32_t AlAxiMon_GetMonValue(AXIMON_TypeDef *AXIMON,enum AXIMONParm type);
uint32_t AlAxiMon_GetMonCountValue(AXIMON_TypeDef *AXIMON,enum CountParm type);
uint32_t AlAxiMon_GetMonDebugValue(AXIMON_TypeDef *AXIMON,enum DebugParm type);
uint8_t AlAxiMon_SetBandWidthCnt(AXIMON_TypeDef *AXIMON,uint32_t cnt);
uint32_t AlAxiMon_GetBandWidthCnt(AXIMON_TypeDef *AXIMON);
uint32_t AlAxiMon_GetSubpara(AXIMON_TypeDef *AXIMON,enum AXIMONSubpara type);
uint32_t AlAxiMon_GetMonTimeOutValue(AXIMON_TypeDef *AXIMON);
void AlAxiMon_ClearDebugRecord(AXIMON_TypeDef *AXIMON);
#ifdef __cplusplus
}
#endif
#endif /* _HBIRD_AXIMON_H */
