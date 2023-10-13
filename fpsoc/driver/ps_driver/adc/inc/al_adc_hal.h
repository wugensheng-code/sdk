/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __AL_ADC_HAL_H_
#define __AL_ADC_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif


/***************************** Include Files ********************************/
#include "al_adc_dev.h"


/**************************** Type Definitions *******************************/
typedef struct
{
    AL_ADC_DevStruct     Dev;
    AL_Lock              Lock;
} AL_ADC_HalStruct;


/************************** Function Prototypes ******************************/

AL_S32 AlAdc_Hal_Init(AL_ADC_HalStruct **Handle, AL_U32 DevId, AL_ADC_InitStruct *InitConfig,
                       AL_ADC_ChannelCfg *ChannelCfg, AL_ADC_EventCallBack Callback);
AL_VOID AlAdc_Hal_DefEventHandler(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef);
AL_S32 AlAdc_Hal_AdcStart(AL_ADC_HalStruct *Handle);
AL_S32 AlAdc_Hal_AdcStop(AL_ADC_HalStruct *Handle);
AL_S32 AlAdc_Hal_AdcStartIntr(AL_ADC_HalStruct *Handle, AL_U8 IntrData);
AL_S32 AlAdc_Hal_AdcStopIntr(AL_ADC_HalStruct *Handle, AL_U8 IntrData);
AL_U16 AlAdc_Hal_GetAdcData(AL_ADC_HalStruct *Handle, AL_ADC_ChanEnum ChanNum);
AL_S32 AlAdc_Hal_IoCtl(AL_ADC_HalStruct *Handle, AL_ADC_IoCtlCmdEnum Cmd, AL_ADC_IoctlParamUnion *IoctlParam);


#ifdef __cplusplus
}
#endif

#endif
