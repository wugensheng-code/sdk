/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __AL_ADC_DEV_H_
#define __AL_ADC_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/

#include "al_adc_ll.h"


/**************************** Exported Typedef *******************************/

#define AL_ADC_ERR_NULL_PTR             (AL_DEF_ERR(AL_ADC, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_ADC_ERR_ILLEGAL_PARAM        (AL_DEF_ERR(AL_ADC, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_ADC_ERR_NOT_READY            (AL_DEF_ERR(AL_ADC, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY))
#define AL_ADC_ERR_NOT_SUPPORT          (AL_DEF_ERR(AL_ADC, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT))

/**************************** Type Definitions *******************************/
#define AL_ADC_CHAN_NUM 8

typedef struct
{
    AL_ADC_ChanEnum       ChanNum;
    AL_ADC_IoMuxEnum      ChanIomux;
    AL_U16                LthVal;
    AL_U16                GthVal;
} AL_ADC_ChannelCfg;

typedef struct
{
    AL_ADC_ChanEnum       ChanNum;
    AL_U16                ChanData;
} AL_ADC_Data;

typedef struct
{
    AL_ADC_InputSingalEnum          InputSingal;
    AL_ADC_ResolutionSelEnum        Resolution;
    AL_ADC_RefVoltagEnum            RefVoltag;
    AL_ADC_PhyExternalMuxEnum       ExternalMux;
    AL_ADC_ConvModeSelectEnum       ConvMode;
    AL_U8                           MaxConvChanNum;
    AL_ADC_ClkSourceEnum            ClkSource;
} AL_ADC_InitStruct;

typedef enum
{
    AL_ADC_EVENT_GETDATA_DONE         = (0x00),
    AL_ADC_EVENT_GETDATA_GTH          = (0x01),
    AL_ADC_EVENT_GETDATA_LTH          = (0x02),
    AL_ADC_EVENT_ERROR                = (0x03),
} AL_ADC_EventIdEnum;

typedef struct
{
    AL_ADC_EventIdEnum      Events;
    AL_U32                  EventData;
} AL_ADC_EventStruct;

typedef enum
{
    AL_ADC_STATE_NOT_INIT     = (0x0),
    AL_ADC_STATE_READY        = (0x01 << 0),
    AL_ADC_STATE_GET_DONEDATA = (0x01 << 1),
    AL_ADC_STATE_GET_LTHDATA  = (0x01 << 2),
    AL_ADC_STATE_GET_GTHDATA  = (0x01 << 3)
} AL_ADC_StateEnum;

typedef enum
{
    AL_ADC_INTR_DONE_BIT     = BIT(AL_ADC_INTR_DONE),
    AL_ADC_INTR_GTH_BIT      = BIT(AL_ADC_INTR_GTH),
    AL_ADC_INTR_LTH_BIT      = BIT(AL_ADC_INTR_LTH),
    AL_ADC_INTR_ERROR_BIT    = BIT(AL_ADC_INTR_ERROR)
}AL_ADC_IntrtypeBitEnum;

typedef AL_VOID (*AL_ADC_EventCallBack)(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef);

typedef struct
{
    AL_U32                       DevId;
    AL_REG                       AdcBaseAddr;
    AL_REG                       GpBaseAddr;
    AL_U32                       IntrNum;
    AL_ADC_InitStruct            Configs;
    AL_ADC_ChannelCfg            ChannelCfg[AL_ADC_CHAN_NUM];
    AL_ADC_Data                  AdcData[AL_ADC_CHAN_NUM];
    AL_ADC_EventCallBack         EventCallBack;
    AL_VOID                      *EventCallBackRef;
    AL_ADC_StateEnum             State;
} AL_ADC_DevStruct;


typedef enum
{
    AL_ADC_IOCTL_SET_REFVOLTAG,
    AL_ADC_IOCTL_SET_RESOLUTION,
    AL_ADC_IOCTL_SET_INPUTSINGAL,
} AL_ADC_IoCtlCmdEnum;

typedef union {
    AL_U8    InputSingal;
    AL_U8    Resolution;
    AL_U8    RefVoltag;
    AL_U8    ClkSource;
    AL_U8    ConvMode;
    AL_U8    MaxConvChanNum;
}AL_ADC_IoctlParamUnion;


/************************** Function Prototypes ******************************/

AL_ADC_HwConfigStruct *AlAdc_Dev_LookupConfig(AL_U32 DevId);
AL_S32 AlAdc_Dev_Init(AL_ADC_DevStruct *Adc, AL_U32 DevId, AL_ADC_InitStruct *InitConfig);
AL_S32 AlAdc_Dev_SetIomuxForChan(AL_ADC_DevStruct *Adc, AL_ADC_ChannelCfg *ChannelCfg);
AL_S32 AlAdc_Dev_SetChanThre(AL_ADC_DevStruct *Adc, AL_ADC_ChannelCfg *ChannelCfg);
AL_S32 AlAdc_Dev_ClrAdcChanIntr(AL_ADC_DevStruct *Adc, AL_ADC_IntrtypeEnum IntrType);
AL_S32 AlAdc_Dev_EnableChanIntr(AL_ADC_DevStruct *Adc, AL_ADC_IntrtypeEnum IntrType, AL_BOOL State);
AL_VOID AlAdc_Dev_EnableAdc(AL_ADC_DevStruct *Adc);
AL_VOID AlAdc_Dev_DisableAdc(AL_ADC_DevStruct *Adc);
AL_VOID AlAdc_Dev_StartConv(AL_ADC_DevStruct *Adc);
AL_VOID AlAdc_Dev_StopConv(AL_ADC_DevStruct *Adc);
AL_U16 AlAdc_Dev_GetAdcData(AL_ADC_DevStruct *Adc, AL_ADC_ChanEnum ChanNum);
AL_VOID AlAdc_Dev_IntrHandler(AL_VOID *Instance);
AL_S32 AlAdc_Dev_RegisterEventCallBack(AL_ADC_DevStruct *Adc, AL_ADC_EventCallBack Callback, AL_VOID *CallbackRef);
AL_S32 AlAdc_Dev_UnRegisterEventCallBack(AL_ADC_DevStruct *Adc);



#ifdef __cplusplus
}
#endif

#endif
