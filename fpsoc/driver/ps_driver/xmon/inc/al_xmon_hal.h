/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_xmon_Hal.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-03-29
 * @brief   XMON Halice driver
 */

#ifndef __AL_XMON_HAL_H_
#define __AL_XMON_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************* Exported Includes ************************************/
#include "al_xmon_ll.h"

#define AL_XMON_ERR_NULL_PTR                 (AL_DEF_ERR(AL_XMON, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_XMON_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_XMON, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))


/******************************* Exported Typedef ************************************/
/**
 * @brief  Configs Struct
 */


typedef struct
{
    AL_XMON_QosRecord       Record;
    AL_U32                  AwResptimeMax;
    AL_XMON_AwResptimeAcc   AwResptimeAcc;
    AL_U32                  AwResptimeCnt;
    AL_U32                  ArResptimeMax;
    AL_XMON_ArResptimeAcc   ArResptimeAcc;
    AL_U32                  ArResptimeCnt;
    AL_U32                  BandWidthWr;
    AL_U32                  BandWidthRd;

} AL_Xmon_Info;

typedef struct
{
    AL_REG                       BaseAddr;
    AL_Xmon_Info                 Info;
} AL_Xmon_HalStruct;

AL_S32 AlXmon_Hal_Init(AL_Xmon_HalStruct **Handle, AL_XMON_Instance Instance);
AL_S32 AlXmon_Hal_MonitorConfig(AL_Xmon_HalStruct *Handle, AL_U16 IdMax, AL_U16 IdMin,
    AL_U16 GidMax, AL_U16 GidMin, AL_U8 QosMax, AL_U8 QosMin, AL_U32 Thre);
AL_S32 AlXmon_Hal_MonitorGetValue(AL_Xmon_HalStruct *Handle);

#ifdef __cplusplus
}
#endif

#endif
