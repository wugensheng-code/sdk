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
 * This function searches through the available XMON hardware configurations to find the one that matches the specified device ID.
 *
 * @param DevId The device ID for which the hardware configuration is being looked up.
 * @return Pointer to the AL_XMON_HwConfigStruct structure if found, otherwise NULL.
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

/**
 * This function searches through the available XMON hardware configurations to find the one that matches the specified device ID.
 *
 * @param DevId The device ID for which the hardware configuration is being looked up.
 * @return Pointer to the AL_XMON_HwConfigStruct structure if found, otherwise NULL.
 */
static AL_VOID AlXmon_IntrHandler(AL_VOID *Handle)
{
    AL_Xmon_HalStruct *Xmon = (AL_Xmon_HalStruct *)Handle;

    AlXmon_ll_ClearIntr(Xmon->BaseAddr);
    if (AlXmon_ll_GetTimeoutState((Xmon)->BaseAddr)) {
        AL_LOG(AL_LOG_LEVEL_NOTICE, "Response Timeout\r\n");
    }

    if (AlXmon_ll_GetRespError((Xmon)->BaseAddr)) {
        AL_LOG(AL_LOG_LEVEL_NOTICE, "Response Error\r\n");
    }
}

/**
 * This function initializes an XMON instance with the specified hardware configuration. It enables interrupts and registers the interrupt handler.
 *
 * @param Handle Double pointer to the AL_Xmon_HalStruct structure where the initialized XMON instance will be stored.
 * @param Instance The instance of the XMON module to be initialized.
 * @return AL_OK on success, AL_XMON_ERR_ILLEGAL_PARAM if the parameters are invalid.
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

/**
 * This function sets up various parameters for monitoring, including ID ranges, QoS thresholds, and enables response time monitoring.
 *
 * @param Handle Pointer to the AL_Xmon_HalStruct structure that contains the XMON instance to be configured.
 * @param IdMax Maximum ID for monitoring.
 * @param IdMin Minimum ID for monitoring.
 * @param GidMax Maximum group ID for monitoring.
 * @param GidMin Minimum group ID for monitoring.
 * @param QosMax Maximum QoS value for monitoring.
 * @param QosMin Minimum QoS value for monitoring.
 * @param Thre Threshold for debug timeout.
 * @return AL_OK on success, AL_XMON_ERR_ILLEGAL_PARAM if the parameters are invalid.
 */
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

/**
 * This function reads various monitoring values from the XMON instance, such as QoS records, response times, and bandwidth usage.
 *
 * @param Handle Pointer to the AL_Xmon_HalStruct structure that contains the XMON instance from which values are to be retrieved.
 * @return AL_OK on success, AL_XMON_ERR_ILLEGAL_PARAM if the parameters are invalid.
 */
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
