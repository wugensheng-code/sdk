/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __AL_XADC_DEV_H_
#define __AL_XADC_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/

#include "al_xadc_ll.h"


/**************************** Exported Typedef *******************************/

#define AL_XADC_ERR_NULL_PTR             (AL_DEF_ERR(AL_XADC, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_XADC_ERR_ILLEGAL_PARAM        (AL_DEF_ERR(AL_XADC, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_XADC_ERR_NOT_READY            (AL_DEF_ERR(AL_XADC, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY))
#define AL_XADC_ERR_NOT_SUPPORT          (AL_DEF_ERR(AL_XADC, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT))

/**************************** Type Definitions *******************************/
#define AL_XADC_CHANNEL_NUM 8

typedef struct
{
    AL_XADC_ChannelEnum       ChannelNum;
    AL_XADC_IoMuxEnum         ChannelIomux;
    AL_U16                    LthValue;
    AL_U16                    GthValue;
} AL_XADC_ChannelCfg;

typedef struct
{
    AL_XADC_ChannelEnum       ChannelNum;
    AL_U16                    ChannelData;
} AL_XADC_Data;

typedef struct
{
    AL_XADC_InputSingalEnum          InputSingal;
    AL_XADC_ResolutionSelectlEnum    Resolution;
    AL_XADC_RefVoltagEnum            RefVoltag;
    AL_XADC_PhyExternalMuxEnum       ExternalMux;
    AL_XADC_ConvModeSelectEnum       ConvMode;
    AL_U8                            MaxConvChannelNum;
    AL_XADC_ClkSourceEnum            ClkSource;
} AL_XADC_InitStruct;

typedef enum
{
    AL_XADC_EVENT_GETDATA_DONE         = (0x00),
    AL_XADC_EVENT_GETDATA_GTH          = (0x01),
    AL_XADC_EVENT_GETDATA_LTH          = (0x02),
    AL_XADC_EVENT_ERROR                = (0x03),
} AL_XADC_EventIdEnum;

typedef struct
{
    AL_XADC_EventIdEnum     Events;
    AL_U32                  EventData;
} AL_XADC_EventStruct;

typedef enum
{
    AL_XADC_STATE_NOT_INIT     = (0x0),
    AL_XADC_STATE_READY        = (0x01 << 0),
    AL_XADC_STATE_GET_DONEDATA = (0x01 << 1),
    AL_XADC_STATE_GET_LTHDATA  = (0x01 << 2),
    AL_XADC_STATE_GET_GTHDATA  = (0x01 << 3)
} AL_XADC_StateEnum;

typedef enum
{
    AL_XADC_INTR_DONE_BIT     = BIT(AL_XADC_INTR_DONE),
    AL_XADC_INTR_GTH_BIT      = BIT(AL_XADC_INTR_GTH),
    AL_XADC_INTR_LTH_BIT      = BIT(AL_XADC_INTR_LTH),
    AL_XADC_INTR_ERROR_BIT    = BIT(AL_XADC_INTR_ERROR)
}AL_XADC_IntrtypeBitEnum;

typedef AL_VOID (*AL_XADC_EventCallBack)(AL_XADC_EventStruct XadcEvent, AL_VOID *CallbackRef);

typedef struct
{
    AL_U32                        DevId;
    AL_REG                        XadcBaseAddr;
    AL_REG                        GpBaseAddr;
    AL_U32                        IntrNum;
    AL_XADC_InitStruct            Configs;
    AL_XADC_ChannelCfg            ChannelCfg[AL_XADC_CHANNEL_NUM];
    AL_XADC_Data                  XadcData[AL_XADC_CHANNEL_NUM];
    AL_XADC_EventCallBack         EventCallBack;
    AL_VOID                       *EventCallBackRef;
    AL_XADC_StateEnum             State;
} AL_XADC_DevStruct;


typedef enum
{
    AL_XADC_IOCTL_SET_INPUTSINGAL,
    AL_XADC_IOCTL_SET_RESOLUTION,
    AL_XADC_IOCTL_SET_REFVOLTAG,
    AL_XADC_IOCTL_SET_CLKSOURCE,
    AL_XADC_IOCTL_SET_CONVMODE,
    AL_XADC_IOCTL_SET_MAXCONVCHANNELNUM
} AL_XADC_IoCtlCmdEnum;

typedef union {
    AL_U8    InputSingal;
    AL_U8    Resolution;
    AL_U8    RefVoltag;
    AL_U8    ClkSource;
    AL_U8    ConvMode;
    AL_U8    MaxConvChannelNum;

}AL_XADC_IoctlParamUnion;


/************************** Function Prototypes ******************************/

AL_XADC_HwConfigStruct *AlXadc_Dev_LookupConfig(AL_U32 DevId);
AL_S32 AlXadc_Dev_Init(AL_XADC_DevStruct *Xadc, AL_U32 DevId, AL_XADC_InitStruct *InitConfig);
AL_S32 AlXadc_Dev_SetIomuxForChannel(AL_XADC_DevStruct *Xadc, AL_XADC_ChannelCfg *ChannelCfg);
AL_S32 AlXadc_Dev_SetChannelThresHold(AL_XADC_DevStruct *Xadc, AL_XADC_ChannelCfg *ChannelCfg);
AL_S32 AlXadc_Dev_ClrIntr(AL_XADC_DevStruct *Xadc, AL_XADC_IntrtypeEnum IntrType);
AL_S32 AlXadc_Dev_EnableIntr(AL_XADC_DevStruct *Xadc, AL_XADC_IntrtypeEnum IntrType, AL_BOOL State);
AL_VOID AlXadc_Dev_EnableXadc(AL_XADC_DevStruct *Xadc);
AL_VOID AlXadc_Dev_DisableXadc(AL_XADC_DevStruct *Xadc);
AL_VOID AlXadc_Dev_StartConv(AL_XADC_DevStruct *Xadc);
AL_VOID AlXadc_Dev_StopConv(AL_XADC_DevStruct *Xadc);
AL_U16 AlXadc_Dev_GetAdcData(AL_XADC_DevStruct *Xadc, AL_XADC_ChannelEnum ChannelNum);
AL_VOID AlXadc_Dev_IntrHandler(AL_VOID *Instance);
AL_S32 AlXadc_Dev_RegisterEventCallBack(AL_XADC_DevStruct *Xadc, AL_XADC_EventCallBack Callback, AL_VOID *CallbackRef);
AL_S32 AlXadc_Dev_UnRegisterEventCallBack(AL_XADC_DevStruct *Xadc);
AL_S32 AlXadc_Dev_IoCtl(AL_XADC_DevStruct *Xadc, AL_XADC_IoCtlCmdEnum Cmd, AL_XADC_IoctlParamUnion *IoctlParam);



#ifdef __cplusplus
}
#endif

#endif
