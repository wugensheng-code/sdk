#ifndef AL_XADC_HAL_H
#define AL_XADC_HAL_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************** Include Files ********************************/
#include "al_xadc_dev.h"


/**************************** Type Definitions *******************************/
typedef struct
{
    AL_XADC_DevStruct            *Dev;
} AL_XADC_HalStruct;


/************************** Function Prototypes ******************************/

AL_S32 AlXadc_Hal_Init(AL_XADC_HalStruct *Handle, AL_U32 DevId, AL_XADC_InitStruct *InitConfig,
                       AL_XADC_ChannelCfg *ChannelCfg, AL_XADC_EventCallBack Callback);
AL_VOID AlXadc_Hal_DefEventHandler(AL_XADC_EventStruct XadcEvent, AL_VOID *CallbackRef);
AL_S32 AlXadc_Hal_XadcStart(AL_XADC_HalStruct *Handle);
AL_S32 AlXadc_Hal_XadcStop(AL_XADC_HalStruct *Handle);
AL_S32 AlXadc_Hal_XadcStartIntr(AL_XADC_HalStruct *Handle, AL_U8 IntrData);
AL_S32 AlXadc_Hal_XadcStopIntr(AL_XADC_HalStruct *Handle, AL_U8 IntrData);
AL_U16 AlXadc_Hal_GetAdcData(AL_XADC_HalStruct *Handle, AL_XADC_ChannelEnum ChannelNum);
AL_S32 AlXadc_Hal_IoCtl(AL_XADC_HalStruct *Handle, AL_XADC_IoCtlCmdEnum Cmd, AL_XADC_IoctlParamUnion *IoctlParam);


#ifdef __cplusplus
}
#endif

#endif
