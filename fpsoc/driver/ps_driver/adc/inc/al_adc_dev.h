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
    AL_BOOL   IntrDoneMask;
    AL_BOOL   IntrGthMask;
    AL_BOOL   IntrLthMask;
    AL_BOOL   IntrErrorMask;
} AL_ADC_PlIntrCfg;

typedef struct
{
    AL_ADC_InputSingalEnum          InputSingal;
    AL_ADC_ResolutionSelEnum        Resolution;
    AL_ADC_RefVoltagEnum            RefVoltag;
    AL_ADC_ConvModeSelectEnum       ConvMode;
    AL_U8                           ConvChanNum;
    AL_ADC_ClkSourceEnum            ClkSource;
    AL_U8                           AdcClkDiv;
    AL_ADC_PlIntrCfg                PlIntrCfg;
} AL_ADC_InitStruct;
/* when AdcClkDiv = 0, AdcClk = AdcClk. otherwise AdcClk = AdcClk / AdcClkDiv / 2 */

typedef struct
{
    AL_ADC_ChanEnum       ChanNum;
    AL_ADC_MuxEnum        MuxForChan;
    AL_U16                LthVal;
    AL_U16                GthVal;
} AL_ADC_ChanCfg;

typedef struct
{
    AL_ADC_ChanEnum       ChanNum;
    AL_U16                ChanData;
} AL_ADC_Data;

typedef enum
{
    AL_ADC_DATA_DONE         = BIT(AL_ADC_PL_INTR_DONE),
    AL_ADC_DATA_GTH          = BIT(AL_ADC_PL_INTR_GTH),
    AL_ADC_DATA_LTH          = BIT(AL_ADC_PL_INTR_LTH),
    AL_ADC_DATA_ERROR        = BIT(AL_ADC_PL_INTR_ERROR),
} AL_ADC_PlAdcFuncEnum;
typedef enum
{
    AL_ADC_EVENT_DATA_DONE         = (0x00),
    AL_ADC_EVENT_DATA_GTH          = (0x01),
    AL_ADC_EVENT_DATA_LTH          = (0x02),
    AL_ADC_EVENT_DATA_ERROR        = (0x03),
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

typedef AL_VOID (*AL_ADC_EventCallBack)(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef);

typedef struct
{
    AL_U32                       DevId;
    AL_REG                       AdcBaseAddr;
    AL_REG                       GpBaseAddr;
    AL_U32                       IntrNum;
    AL_ADC_InitStruct            Configs;
    AL_ADC_ChanCfg               ChanCfg[AL_ADC_CHAN_NUM];
    AL_ADC_Data                  AdcData[AL_ADC_CHAN_NUM];
    AL_ADC_PlIntrCfg             PlIntrCfg;
    AL_ADC_EventCallBack         EventCallBack;
    AL_VOID                      *EventCallBackRef;
    AL_ADC_StateEnum             State;
} AL_ADC_DevStruct;


typedef enum
{
    AL_ADC_IOCTL_SET_REFVOLTAG,
    AL_ADC_IOCTL_SET_RESOLUTION,
    AL_ADC_IOCTL_SET_INPUTSINGAL,
    AL_ADC_IOCTL_SET_CLK_GATE,
    AL_ADC_IOCTL_POWER_DOWN,
    AL_ADC_IOCTL_ENABLE_PLADC_INTR,
    AL_ADC_IOCTL_DISABLE_PLADC_INTR,
    AL_ADC_IOCTL_CLR_PLADC_INTR,
} AL_ADC_IoCtlCmdEnum;

typedef union {
    AL_U8    InputSingal;
    AL_U8    Resolution;
    AL_U8    RefVoltag;
    AL_U8    ClkGateState;
    AL_U8    PowerDownState;
    AL_U8    EnablePlAdcIntrType;
    AL_U8    DisablePlAdcIntrType;
    AL_U8    ClrPlAdcIntrType;
} AL_ADC_IoctlParamUnion;


/************************** Function Prototypes ******************************/

AL_ADC_HwConfigStruct *AlAdc_Dev_LookupConfig(AL_U32 DevId);
AL_S32 AlAdc_Dev_Init(AL_ADC_DevStruct *Adc, AL_U32 DevId, AL_ADC_InitStruct *InitConfig, AL_ADC_ChanCfg *ChanCfg);
AL_S32 AlAdc_Dev_EnablePsAdcIntr(AL_ADC_DevStruct *Adc, AL_ADC_PsIntrTypeEnum IntrType, AL_BOOL State);
AL_S32 AlAdc_Dev_SetMuxForChan(AL_ADC_DevStruct *Adc, AL_ADC_ChanCfg *ChanCfg);
AL_S32 AlAdc_Dev_SetThreForChan(AL_ADC_DevStruct *Adc, AL_ADC_ChanCfg *ChanCfg);
AL_S32 AlAdc_Dev_MaskPlAdcIntr(AL_ADC_DevStruct *Adc, AL_ADC_PlIntrTypeEnum IntrType, AL_BOOL State);
AL_S32 AlAdc_Dev_ClrPlAdcIntr(AL_ADC_DevStruct *Adc, AL_ADC_PlIntrTypeEnum IntrType);
AL_S16 AlAdc_Dev_GetAdcData(AL_ADC_DevStruct *Adc, AL_ADC_ChanEnum ChanNum);
AL_VOID AlAdc_Dev_IntrHandler(AL_VOID *Instance);
AL_S32 AlAdc_Dev_IoCtl(AL_ADC_DevStruct *Adc, AL_ADC_IoCtlCmdEnum Cmd, AL_ADC_IoctlParamUnion *IoctlParam);
AL_S32 AlAdc_Dev_RegisterEventCallBack(AL_ADC_DevStruct *Adc, AL_ADC_EventCallBack Callback, AL_VOID *CallbackRef);
AL_S32 AlAdc_Dev_UnRegisterEventCallBack(AL_ADC_DevStruct *Adc);



#ifdef __cplusplus
}
#endif

#endif
