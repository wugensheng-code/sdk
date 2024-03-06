/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_uart_nonblocked.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   uart nonblocked example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_xmon_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_XID_MIN 0
#define AL_QOS_MAX 0xF
#define AL_QOS_MIN 0
#define AL_TIMEOUT_THRESHOLD 0x20
/************************** Variable Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/


/************************** Function Prototypes ******************************/


/************************** Function Definitions ******************************/

void printf_monitor(AL_Xmon_HalStruct *Xmon)
{
  	AL_LOG(AL_LOG_LEVEL_ERROR, "AXIMON awqos_record         = %x\r\n", Xmon->Info.Record.AwQos_Record);
  	AL_LOG(AL_LOG_LEVEL_ERROR, "AXIMON arqos_record         = %x\r\n", Xmon->Info.Record.ArQos_Record);
  	AL_LOG(AL_LOG_LEVEL_ERROR, "AXIMON awresptime_max       = %x\r\n", Xmon->Info.AwResptimeMax);
  	AL_LOG(AL_LOG_LEVEL_ERROR, "AXIMON awresptime_acc       = %x\r\n", Xmon->Info.AwResptimeAcc.AwRespTimeAcc);
  	AL_LOG(AL_LOG_LEVEL_ERROR, "AXIMON awresptime_cnt       = %x\r\n", Xmon->Info.AwResptimeCnt);
  	AL_LOG(AL_LOG_LEVEL_ERROR, "AXIMON arresptime_max       = %x\r\n", Xmon->Info.ArResptimeMax);
  	AL_LOG(AL_LOG_LEVEL_ERROR, "AXIMON awresptime_acc       = %x\r\n", Xmon->Info.ArResptimeAcc.ArRespTimeAcc);
  	AL_LOG(AL_LOG_LEVEL_ERROR, "AXIMON arresptime_cnt       = %x\r\n", Xmon->Info.ArResptimeCnt);
  	AL_LOG(AL_LOG_LEVEL_ERROR, "AXIMON bandwidth_wr         = %x\r\n", Xmon->Info.BandWidthWr);
  	AL_LOG(AL_LOG_LEVEL_ERROR, "AXIMON bandwidth_rd         = %x\r\n", Xmon->Info.BandWidthRd);
}

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_Xmon_HalStruct *DdrS0Handle;
    AL_Xmon_HalStruct *DdrS1Handle;
    AL_Xmon_HalStruct *DdrS2Handle;
    AL_Xmon_HalStruct *DdrS3Handle;

    AL_LOG(AL_LOG_LEVEL_INFO, "Xmon test\r\n");


    Ret = AlXmon_Hal_Init(&DdrS0Handle, XMON_DDR_S0);
    Ret = AlXmon_Hal_Init(&DdrS1Handle, XMON_DDR_S1);
    Ret = AlXmon_Hal_Init(&DdrS2Handle, XMON_DDR_S2);
    Ret = AlXmon_Hal_Init(&DdrS3Handle, XMON_DDR_S3);
    if (Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Xmon init failed");
    }

    AlXmon_Hal_MonitorConfig(DdrS0Handle, AL_XID_MAX, AL_XID_MIN, AL_XID_MAX, 
        AL_XID_MIN, AL_QOS_MAX, AL_QOS_MIN, AL_TIMEOUT_THRESHOLD);
    
    AlXmon_Hal_MonitorConfig(DdrS1Handle, AL_XID_MAX, AL_XID_MIN, AL_XID_MAX, 
        AL_XID_MIN, AL_QOS_MAX, AL_QOS_MIN, AL_TIMEOUT_THRESHOLD);

    AlXmon_Hal_MonitorConfig(DdrS2Handle, AL_XID_MAX, AL_XID_MIN, AL_XID_MAX, 
        AL_XID_MIN, AL_QOS_MAX, AL_QOS_MIN, AL_TIMEOUT_THRESHOLD);

    AlXmon_Hal_MonitorConfig(DdrS3Handle, AL_XID_MAX, AL_XID_MIN, AL_XID_MAX, 
        AL_XID_MIN, AL_QOS_MAX, AL_QOS_MIN, AL_TIMEOUT_THRESHOLD);

    while (1) {
        AlXmon_Hal_MonitorGetValue(DdrS0Handle);
        AL_LOG(AL_LOG_LEVEL_INFO, "Xmon Information \r\n");
        printf_monitor(DdrS0Handle);
        AlXmon_Hal_MonitorGetValue(DdrS1Handle);
        printf_monitor(DdrS1Handle);
        AlXmon_Hal_MonitorGetValue(DdrS2Handle);
        printf_monitor(DdrS2Handle);
        AlXmon_Hal_MonitorGetValue(DdrS3Handle);
        printf_monitor(DdrS3Handle);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "\r\n");
    AL_LOG(AL_LOG_LEVEL_INFO, "Xmon test success\r\n");
    return Ret;
}
