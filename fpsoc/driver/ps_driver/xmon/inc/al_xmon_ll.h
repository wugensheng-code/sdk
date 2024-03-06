/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_UART_LL_H_
#define __AL_UART_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_xmon_hw.h"

static inline AL_XMON_QosRecord ALXmon_ll_GetQosRecordValue(AL_REG BaseAddr)
{
    return (AL_XMON_QosRecord)AL_REG32_READ(BaseAddr + XMON__QOS_RECORD__OFFSET);
}

static inline AL_VOID AlXmon_ll_QosModeConfig(AL_REG BaseAddr, AL_XMON_QosMode Mode)
{
    AL_REG32_SET_BITS(BaseAddr + XMON__QOS_CONFIG__OFFSET, XMON__QOS_CONFIG__QOS_CONFIG_EN__SHIFT, 2, Mode);
}

static inline AL_VOID AlXmon_ll_QosSetConfigValue(AL_REG BaseAddr, AL_U8 Value)
{
    AL_REG32_SET_BITS(BaseAddr + XMON__QOS_CONFIG__OFFSET, XMON__QOS_CONFIG__QOS_CONFIG_VALUE__SHIFT, 4, Value);
}

static inline AL_VOID AlXmon_ll_QosSetConfigThreshold(AL_REG BaseAddr, AL_U8 Min, AL_U8 Max)
{
    AL_REG32_SET_BITS(BaseAddr + XMON__QOS_THRESHOLD__OFFSET, XMON__QOS_THRESHOLD__QOS_MIN__SHIFT, 4, Min);
    AL_REG32_SET_BITS(BaseAddr + XMON__QOS_THRESHOLD__OFFSET, XMON__QOS_THRESHOLD__QOS_MAX__SHIFT, 4, Max);
}

static inline AL_VOID AlXmon_ll_ResptimeEnable(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + XMON__RESPTIME_CFG__OFFSET, XMON__RESPTIME_CFG__RESPTIME_ENABLE__SHIFT, AL_FUNC_ENABLE);
}

static inline AL_VOID AlXmon_ll_ResptimeClean(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + XMON__RESPTIME_CFG__OFFSET, XMON__RESPTIME_CFG__RESPTIME_CLEAN__SHIFT, AL_FUNC_ENABLE);
}

static inline AL_U32 AlXmon_ll_GetAwResptimeMax(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__AWRESPTIME_MAX__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetAwResptimeAcch(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__AWRESPTIME_ACC_H__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetAwResptimeAccl(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__AWRESPTIME_ACC_L__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetAwResptimeCnt(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__AWRESPTIME_CNT__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetArResptimeMax(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__ARRESPTIME_MAX__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetArResptimeAcch(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__ARRESPTIME_ACC_H__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetArResptimeAccl(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__ARRESPTIME_ACC_L__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetArResptimeCnt(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__ARRESPTIME_CNT__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetBandWidthTmwd(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__BANDWIDTH_TMWD__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetBandWidthWr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__BANDWIDTH_WR__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetBandWidthRd(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__BANDWIDTH_RD__OFFSET);
}

static inline AL_VOID AlXmon_ll_SubparaCountEnable(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + XMON__SUBPARA_CONFIG__OFFSET, XMON__SUBPARA_CONFIG__SUBPARA_ENABLE__SHIFT, AL_FUNC_ENABLE);
}

static inline AL_VOID AlXmon_ll_SubparaSetAwid(AL_REG BaseAddr, AL_U16 Max, AL_U16 Min)
{
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_AWID__OFFSET, XMON__SUBPARA_AWID__SUBPARA_AWID_MAX__SHIFT, 16, Max);
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_AWID__OFFSET, XMON__SUBPARA_AWID__SUBPARA_AWID_MIN__SHIFT, 16, Min);
}

static inline AL_VOID AlXmon_ll_SubparaSetAwGrpid(AL_REG BaseAddr, AL_U16 Max, AL_U16 Min)
{
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_GRPID__OFFSET, XMON__SUBPARA_GRPID__SUBPARA_AWGID_MAX__SHIFT, 16, Max);
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_GRPID__OFFSET, XMON__SUBPARA_GRPID__SUBPARA_AWGID_MIN__SHIFT, 16, Min);
}

static inline AL_VOID AlXmon_ll_SubparaSetArGrpid(AL_REG BaseAddr, AL_U16 Max, AL_U16 Min)
{
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_GRPID__OFFSET, XMON__SUBPARA_GRPID__SUBPARA_ARGID_MAX__SHIFT, 16, Max);
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_GRPID__OFFSET, XMON__SUBPARA_GRPID__SUBPARA_ARGID_MIN__SHIFT, 16, Min);
}

static inline AL_VOID AlXmon_ll_SubparaSetArid(AL_REG BaseAddr, AL_U16 Max, AL_U16 Min)
{
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_ARID__OFFSET, XMON__SUBPARA_ARID__SUBPARA_ARID_MAX__SHIFT, 16, Max);
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_ARID__OFFSET, XMON__SUBPARA_ARID__SUBPARA_ARID_MIN__SHIFT, 16, Min);
}

static inline AL_VOID AlXmon_ll_SubparaSetAwBurst(AL_REG BaseAddr, AL_U8 BtyeMax, AL_U8 BsizeMax, AL_U8 BlenMax, 
    AL_U8 BtyeMin, AL_U8 BsizeMin, AL_U8 BlenMin)
{
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_AWBURST__OFFSET, XMON__SUBPARA_AWBURST__SUBPARA_AWBTYPE_MAX__SHIFT, 2, BtyeMax);
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_AWBURST__OFFSET, XMON__SUBPARA_AWBURST__SUBPARA_AWBSIZE_MAX__SHIFT, 3, BsizeMax);
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_AWBURST__OFFSET, XMON__SUBPARA_AWBURST__SUBPARA_AWBLEN_MAX__SHIFT, 8, BlenMax);
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_AWBURST__OFFSET, XMON__SUBPARA_AWBURST__SUBPARA_AWBTYPE_MIN__SHIFT, 2, BtyeMin);
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_AWBURST__OFFSET, XMON__SUBPARA_AWBURST__SUBPARA_AWBSIZE_MIN__SHIFT, 2, BsizeMin);
    AL_REG32_SET_BITS(BaseAddr + XMON__SUBPARA_AWBURST__OFFSET, XMON__SUBPARA_AWBURST__SUBPARA_AWBLEN_MIN__SHIFT, 2, BlenMin);
}

static inline AL_U32 AlXmon_ll_GetCntAwCmd(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_AWCMD__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetCntAwBtype(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_AWBTYPE__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetCntAwBlen(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_AWBLEN__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetCntAwBsize(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_AWBSIZE__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetCntAwId(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_AWID__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetCntAwGid(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_AWGID__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetCntArCmd(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_ARCMD__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetCntArBtype(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_ARBTYPE__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetCntArBlen(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_ARBLEN__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetCntArBsize(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_ARBSIZE__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetCntArId(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_ARID__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetCntArGid(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__CNT_ARGID__OFFSET);
}

static inline AL_VOID AlXmon_ll_DebugClearRecord(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + XMON__DEBUG_FLAGINFO__OFFSET, XMON__DEBUG_FLAGINFO__DEBUG_CLEAR__SHIFT, AL_FUNC_ENABLE);
}

static inline AL_U32 AlXmon_ll_GetDebugAccessType(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + XMON__DEBUG_FLAGINFO__OFFSET, XMON__DEBUG_FLAGINFO__DEBUG_ACCESS_TYPE__SHIFT);
}

static inline AL_U32 AlXmon_ll_GetDebugBurstSize(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + XMON__DEBUG_FLAGINFO__OFFSET, XMON__DEBUG_FLAGINFO__DEBUG_BURST_SIZE__SHIFT, 3);
}

static inline AL_U32 AlXmon_ll_GetDebugBurstLength(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + XMON__DEBUG_FLAGINFO__OFFSET, XMON__DEBUG_FLAGINFO__DEBUG_BURST_LENGTH__SHIFT, 8);
}

static inline AL_U32 AlXmon_ll_GetDebugBurstType(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + XMON__DEBUG_FLAGINFO__OFFSET, XMON__DEBUG_FLAGINFO__DEBUG_ACCESS_TYPE__SHIFT, 2);
}

static inline AL_U32 AlXmon_ll_GetDebugAddrH(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + XMON__DEBUG_FLAGINFO__OFFSET, XMON__DEBUG_FLAGINFO__DEBUG_ADDR_H__SHIFT, 8);
}

static inline AL_U32 AlXmon_ll_GetDebugAddrL(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__DEBUG_ADDR_L__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetDebugId(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + XMON__DEBUG_ID__OFFSET);
}

static inline AL_U32 AlXmon_ll_GetDebugAwTimeOut(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + XMON__DEBUG_TIMEOUT_FLAG__OFFSET, XMON__DEBUG_TIMEOUT_FLAG__DEBUG_AWTIMEOUT__SHIFT);
}

static inline AL_U32 AlXmon_ll_GetDebugWTimeOut(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + XMON__DEBUG_TIMEOUT_FLAG__OFFSET, XMON__DEBUG_TIMEOUT_FLAG__DEBUG_WTIMEOUT__SHIFT);
}

static inline AL_U32 AlXmon_ll_GetDebugBTimeOut(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + XMON__DEBUG_TIMEOUT_FLAG__OFFSET, XMON__DEBUG_TIMEOUT_FLAG__DEBUG_BTIMEOUT__SHIFT);
}

static inline AL_U32 AlXmon_ll_GetDebugArTimeOut(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + XMON__DEBUG_TIMEOUT_FLAG__OFFSET, XMON__DEBUG_TIMEOUT_FLAG__DEBUG_ARTIMEOUT__SHIFT);
}

static inline AL_U32 AlXmon_ll_GetDebugRTimeOut(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + XMON__DEBUG_TIMEOUT_FLAG__OFFSET, XMON__DEBUG_TIMEOUT_FLAG__DEBUG_RTIMEOUT__SHIFT);
}

static inline AL_U32 AlXmon_ll_GetDebugTimeOutThd(AL_REG BaseAddr, AL_U32 Threshold)
{
    AL_REG32_WRITE(BaseAddr + XMON__DEBUG_TIMEOUT_FLAG__OFFSET, Threshold);
}

static inline AL_VOID AlXmon_ll_GetIntrState()
{
    return AL_REG32_READ(XMON_TOP_INTR_STATE_ADDR);
}

static inline AL_VOID AlXmon_ll_SetIntr(AL_REG BaseAddr, AL_FUNCTION State)
{
    AL_REG32_SET_BIT(BaseAddr + XMON__INTR__OFFSET, XMON__INTR__INTR_ENABLE__SHIFT, State);
}

static inline AL_VOID AlXmon_ll_ClearIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + XMON__INTR__OFFSET, XMON__INTR__INTR_CLEAR__SHIFT, AL_FUNC_DISABLE);
    AL_REG32_SET_BIT(BaseAddr + XMON__INTR__OFFSET, XMON__INTR__INTR_CLEAR__SHIFT, AL_FUNC_ENABLE);
}

static inline AL_VOID AlXmon_ll_SetTimeoutIntr(AL_REG BaseAddr, AL_FUNCTION State)
{
    AL_REG32_SET_BIT(BaseAddr + XMON__INTR__OFFSET, XMON__INTR__DEBUG_TIMEOUT__SHIFT, State);
}

static inline AL_VOID AlXmon_ll_SetRespIntr(AL_REG BaseAddr, AL_FUNCTION State)
{
    AL_REG32_SET_BIT(BaseAddr + XMON__INTR__OFFSET, XMON__INTR__DEBUG_RESP__SHIFT, State);
}

static inline AL_U32 AlXmon_ll_GetTimeoutState(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + XMON__INTR__OFFSET, XMON__INTR__DEBUG_TIMEOUT__SHIFT);
}

static inline AL_VOID AlXmon_ll_GetRespError(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + XMON__INTR__OFFSET, XMON__INTR__DEBUG_RESP__SHIFT);
}

typedef enum
{
    XMON_DDR_S0,
    XMON_DDR_S1,
    XMON_DDR_S2,
    XMON_DDR_S3,
    XMON_SX2X_M0,
    XMON_SX2X_M1,
    XMON_OCM_S2,
    XMON_SH_M2,
    XMON_SMC,
    XMON_DMACX,
    XMON_GP_S0,
    XMON_GP_S1,
    XMON_HP_S0,
    XMON_HP_S1,
    XMON_APU,
    XMON_JPU,
    XMON_MAIN_S6,
    XMON_MAIN_M6,
    XMON_MAIN_S1,
    XMON_MAIN_M0,
    XMON_ACP,
} AL_XMON_Instance;

#ifdef __cplusplus
}
#endif

#endif
