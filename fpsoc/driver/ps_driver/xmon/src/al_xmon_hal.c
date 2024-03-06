/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_xmon_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/

extern AL_XMON_HwConfigStruct AlXmon_HwConfig[AL_XMON_NUM_INSTANCE];
static AL_Xmon_HalStruct AlXmonHandle[AL_XMON_NUM_INSTANCE];

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/

/********************************************************/

/**
 * This function look up hardware config structure.
 * @param   DeviceId is hardware module id
 * @return
 *          - AL_XMON_HwConfigStruct for hardware config
 * @note
*/
static AL_XMON_HwConfigStruct *AlXmon_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AL_XMON_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_XMON_NUM_INSTANCE; Index++) {
        if (AlXmon_HwConfig[Index].DeviceId == DevId) {
            ConfigPtr = &AlXmon_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

static AL_VOID AlXmon_IntrHandler(AL_VOID *Handle)
{
    AL_Xmon_HalStruct *Xmon = (AL_Xmon_HalStruct *)Handle;

    AlXmon_ll_ClearIntr(Xmon->BaseAddr);
    if (AlXmon_ll_GetTimeoutState((Xmon)->BaseAddr)) {
        AL_LOG(AL_LOG_LEVEL_NOTICE, "Response Timeout\r\n");
    }
}


/**
 * This function initialize the XMON mode according to the specified
 *          parameters in the AL_Xmon_HalStruct and initialize the associated handle.
 * @param   Handle Pointer to a AL_xmon_HalStruct structure that contains xmon Hal instance
 * @param   AL_XMON_Instance The xmon instance to be observed
 * @return
 *          - AL_OK for function success
 *          - Other for function failuregit
 * @note
*/
AL_S32 AlXmon_Hal_Init(AL_Xmon_HalStruct **Handle, AL_XMON_Instance Instance)
{
    AL_S32 Ret = AL_OK;
    AL_XMON_HwConfigStruct *HwConfig = NULL;

    AL_ASSERT((Handle != AL_NULL), AL_XMON_ERR_ILLEGAL_PARAM);

    HwConfig = AlXmon_Dev_LookupConfig((AL_U32) Instance);
    if (HwConfig != AL_NULL) {
        *Handle = &AlXmonHandle[(AL_U32) Instance];
        (*Handle)->BaseAddr = HwConfig->BaseAddress;
    } else {
        return AL_XMON_ERR_ILLEGAL_PARAM;
    }

    AlXmon_ll_SetTimeoutIntr((*Handle)->BaseAddr, AL_FUNC_ENABLE);
    AlXmon_ll_SetRespIntr((*Handle)->BaseAddr, AL_FUNC_ENABLE);
    AlXmon_ll_SetIntr((*Handle)->BaseAddr, AL_FUNC_ENABLE);

    (AL_VOID)AlIntr_RegHandler(SOC_XMON_IRQn, AL_NULL, AlXmon_IntrHandler, (*Handle));

    return AL_OK;
}

AL_S32 AlXmon_Hal_MonitorConfig(AL_Xmon_HalStruct *Handle,
    AL_U16 IdMax, AL_U16 IdMin,
    AL_U16 GidMax, AL_U16 GidMin, AL_U8 QosMax, AL_U8 QosMin, AL_U32 Thre)
{
    AL_ASSERT((Handle != AL_NULL), AL_XMON_ERR_ILLEGAL_PARAM);

    AlXmon_ll_ResptimeClean(Handle->BaseAddr);
    AlXmon_ll_ResptimeEnable(Handle->BaseAddr);
    AlXmon_ll_QosSetConfigValue(Handle->BaseAddr, QosMax);
    AlXmon_ll_QosSetConfigThreshold(Handle->BaseAddr, QosMax, QosMin);
    AlXmon_ll_QosModeConfig(Handle->BaseAddr, AL_QOS_Ip_Inter_Dynamic);
    AlXmon_ll_SubparaCountEnable(Handle->BaseAddr);
    AlXmon_ll_SubparaSetAwid(Handle->BaseAddr, IdMax, IdMin);
    AlXmon_ll_SubparaSetArid(Handle->BaseAddr, IdMax, IdMin);
    AlXmon_ll_SubparaSetAwGrpid(Handle->BaseAddr, GidMax, GidMin);
    AlXmon_ll_SubparaSetArGrpid(Handle->BaseAddr, GidMax, GidMin);
    AlXmon_ll_SubparaSetAwBurst(Handle->BaseAddr, AL_XTYPE_MAX, AL_XSIZE_MAX,
        AL_XLEN_MAX, 0, 0, 0);
    AlXmon_ll_SubparaCountEnable(Handle->BaseAddr);
    AlXmon_ll_GetDebugTimeOutThd(Handle->BaseAddr, Thre);

    return AL_OK;
}

AL_S32 AlXmon_Hal_MonitorGetValue(AL_Xmon_HalStruct *Handle)
{
    AL_ASSERT((Handle != AL_NULL), AL_XMON_ERR_ILLEGAL_PARAM);

    Handle->Info.Record = ALXmon_ll_GetQosRecordValue(Handle->BaseAddr);
    Handle->Info.AwResptimeMax = AlXmon_ll_GetAwResptimeMax(Handle->BaseAddr);
    Handle->Info.AwResptimeAcc.AwQos_RecordH = AlXmon_ll_GetAwResptimeAcch(Handle->BaseAddr);
    Handle->Info.AwResptimeAcc.AwQos_RecordL = AlXmon_ll_GetAwResptimeAccl(Handle->BaseAddr);
    Handle->Info.AwResptimeCnt = AlXmon_ll_GetArResptimeCnt(Handle->BaseAddr);
    Handle->Info.ArResptimeMax = AlXmon_ll_GetAwResptimeMax(Handle->BaseAddr);
    Handle->Info.ArResptimeAcc.ArQos_RecordH = AlXmon_ll_GetArResptimeAcch(Handle->BaseAddr);
    Handle->Info.ArResptimeAcc.ArQos_RecordL = AlXmon_ll_GetArResptimeAccl(Handle->BaseAddr);
    Handle->Info.ArResptimeCnt = AlXmon_ll_GetArResptimeCnt(Handle->BaseAddr);
    Handle->Info.BandWidthWr = AlXmon_ll_GetBandWidthWr(Handle->BaseAddr);
    Handle->Info.BandWidthRd = AlXmon_ll_GetBandWidthRd(Handle->BaseAddr);

    return AL_OK;
}
