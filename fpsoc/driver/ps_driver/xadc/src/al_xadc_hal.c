/***************************** Include Files *********************************/
#include "al_xadc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

static AL_XADC_DevStruct AL_XADC_DevInstance[AL_XADC_NUM_INSTANCE];


/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/

AL_VOID AlXadc_Hal_DefEventHandler(AL_XADC_EventStruct XadcEvent, AL_VOID *CallbackRef)
{
    AL_XADC_HalStruct *Handle = (AL_XADC_HalStruct *)CallbackRef;
    AL_S32 Ret = AL_OK;

    switch (XadcEvent.Events)
    {
    case AL_XADC_EVENT_GETDATA_DONE:

        break;
    case AL_XADC_EVENT_GETDATA_GTH:

        break;
    case AL_XADC_EVENT_GETDATA_LTH:

        break;
    case AL_XADC_EVENT_ERROR:
        break;
    default:
        break;
    }

}

AL_S32 AlXadc_Hal_Init(AL_XADC_HalStruct *Handle, AL_U32 DevId, AL_XADC_InitStruct *InitConfig,
                       AL_XADC_ChannelCfg *ChannelCfg, AL_XADC_EventCallBack Callback)
{
    AL_S32 Ret;
    AL_U8 Index;
    AL_XADC_HwConfigStruct *HwConfig;

    AL_ASSERT((Handle != AL_NULL && DevId < AL_XADC_NUM_INSTANCE && ChannelCfg != AL_NULL), AL_XADC_ERR_ILLEGAL_PARAM);

    HwConfig = AlXadc_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        Handle->Dev = &AL_XADC_DevInstance[DevId];
    } else {
        return AL_XADC_ERR_ILLEGAL_PARAM;
    }

    Ret = AlXadc_Dev_Init(Handle->Dev, DevId, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    for (Index = 0; Index <= Handle->Dev->Configs.MaxConvChannelNum; Index++) {
        Handle->Dev->ChannelCfg[Index] = ChannelCfg[Index];
        AlXadc_Dev_SetIomuxForChannel(Handle->Dev, &ChannelCfg[Index]);
        AlXadc_Dev_SetChannelThresHold(Handle->Dev, &ChannelCfg[Index]);
    }

    AlIntr_RegHandler(Handle->Dev->IntrNum, AL_NULL, AlXadc_Dev_IntrHandler, Handle->Dev);

    if(Callback == AL_NULL) {
        Ret = AlXadc_Dev_RegisterEventCallBack(Handle->Dev, AlXadc_Hal_DefEventHandler, (void *)Handle);
    } else {
        Ret = AlXadc_Dev_RegisterEventCallBack(Handle->Dev, Callback, (void *)Handle);
    }

    return Ret;
}

AL_S32 AlXadc_Hal_XadcStart(AL_XADC_HalStruct *Handle)
{
    AL_ASSERT((Handle != AL_NULL), AL_XADC_ERR_ILLEGAL_PARAM);

    AlXadc_Dev_EnableXadc(Handle->Dev);
    AlXadc_Dev_StartConv(Handle->Dev);

    return AL_OK;
}

AL_S32 AlXadc_Hal_XadcStop(AL_XADC_HalStruct *Handle)
{
    AL_ASSERT((Handle != AL_NULL), AL_XADC_ERR_ILLEGAL_PARAM);

    AlXadc_Dev_DisableXadc(Handle->Dev);
    AlXadc_Dev_StopConv(Handle->Dev);

    return AL_OK;
}

AL_S32 AlXadc_Hal_XadcStartIntr(AL_XADC_HalStruct *Handle, AL_U8 IntrData)
{
    AL_ASSERT((Handle != AL_NULL), AL_XADC_ERR_ILLEGAL_PARAM);

    if (IntrData & (1 << AL_XADC_INTR_DONE)) {
        AlXadc_Dev_EnableIntr(Handle->Dev, AL_XADC_INTR_DONE , AL_TRUE);
    }
    if (IntrData & (1 << AL_XADC_INTR_GTH)) {
        AlXadc_Dev_EnableIntr(Handle->Dev, AL_XADC_INTR_GTH , AL_TRUE);
    }
    if (IntrData & (1 << AL_XADC_INTR_LTH)) {
        AlXadc_Dev_EnableIntr(Handle->Dev, AL_XADC_INTR_LTH , AL_TRUE);
    }
    if (IntrData & (1 << AL_XADC_INTR_ERROR)) {
        AlXadc_Dev_EnableIntr(Handle->Dev, AL_XADC_INTR_ERROR , AL_TRUE);
    }

    AlXadc_Dev_EnableXadc(Handle->Dev);
    AlXadc_Dev_StartConv(Handle->Dev);

    return AL_OK;
}

AL_S32 AlXadc_Hal_XadcStopIntr(AL_XADC_HalStruct *Handle, AL_U8 IntrData)
{
    AL_ASSERT((Handle != AL_NULL), AL_XADC_ERR_ILLEGAL_PARAM);

    if (IntrData & (1 << AL_XADC_INTR_DONE)) {
        AlXadc_Dev_EnableIntr(Handle->Dev, AL_XADC_INTR_DONE , AL_FALSE);
    }
    if (IntrData & (1 << AL_XADC_INTR_GTH)) {
        AlXadc_Dev_EnableIntr(Handle->Dev, AL_XADC_INTR_GTH , AL_FALSE);
    }
    if (IntrData & (1 << AL_XADC_INTR_LTH)) {
        AlXadc_Dev_EnableIntr(Handle->Dev, AL_XADC_INTR_LTH , AL_FALSE);
    }
    if (IntrData & (1 << AL_XADC_INTR_ERROR)) {
        AlXadc_Dev_EnableIntr(Handle->Dev, AL_XADC_INTR_ERROR , AL_FALSE);
    }

    AlXadc_Dev_DisableXadc(Handle->Dev);
    AlXadc_Dev_StopConv(Handle->Dev);

    return AL_OK;
}

AL_U16 AlXadc_Hal_GetAdcData(AL_XADC_HalStruct *Handle, AL_XADC_ChannelEnum ChannelNum)
{
    return AlXadc_Dev_GetAdcData(Handle->Dev, ChannelNum);
}


AL_S32 AlXadc_Hal_IoCtl(AL_XADC_HalStruct *Handle, AL_XADC_IoCtlCmdEnum Cmd, AL_XADC_IoctlParamUnion *IoctlParam)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_XADC_ERR_ILLEGAL_PARAM);

    Ret = AlXadc_Dev_IoCtl(Handle->Dev, Cmd, IoctlParam);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Xadc io ctl cmd error:%d\r\n", Ret);
    }

    return Ret;
}