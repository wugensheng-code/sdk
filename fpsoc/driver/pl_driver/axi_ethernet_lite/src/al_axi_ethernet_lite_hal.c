/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_axi_ethernet_lite_hal.h"

AL_AXI_ETHLITE_HalStruct AlAxiEthLiteHandle[AL_AXI_ETH_LITE_NUM_INSTANCE];

static AL_VOID AlAxiEthLite_DefEventCallBack(AL_AXI_ETHLITE_EventStruct *Event, AL_VOID *CallbackRef)
{
    AL_AXI_ETHLITE_HalStruct *Handle = (AL_AXI_ETHLITE_HalStruct *)CallbackRef;

    switch (Event->Event) {
        case AL_AXI_ETHLITE_EVENT_RX_VALID:
        Handle->RxValidCallBack(CallbackRef);
        break;

        case AL_AXI_ETHLITE_EVENT_TX_DONE:
        Handle->TxDoneCallBack(CallbackRef);
        break;

        default:
        break;
    }
}

AL_S32 AlAxiEthLite_Hal_Init(AL_AXI_ETHLITE_HalStruct **Handle, AL_U32 DevId,
                         AL_AXI_ETHLITE_InitStruct *InitConfig, AL_AXI_ETHLITE_EventCallBack Callback)
{
    AL_S32 Ret = AL_OK;
    AlAxiEthLite_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT((Handle != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    HwConfig = AlAxiEthLite_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlAxiEthLiteHandle[DevId];
    } else {
        return AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM;
    }

    Ret = AlAxiEthLite_Dev_Init(&(*Handle)->Dev, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    if(AL_NULL == Callback) {
        Ret = AlAxiEthLite_Dev_RegisterEventCallBack(&(*Handle)->Dev, AlAxiEthLite_DefEventCallBack, (void *)*Handle);
    } else {
        Ret = AlAxiEthLite_Dev_RegisterEventCallBack(&(*Handle)->Dev, Callback, (void *)*Handle);
    }
    if (Ret != AL_OK) {
        return Ret;
    }

    (AL_VOID)AlIntr_RegHandler((*Handle)->Dev.HwConfig.IntrId, AL_NULL, AlAxiEthLite_Dev_IntrHandler, &(*Handle)->Dev);

    return AL_OK;
}

AL_S32 AlAxiEthLite_Hal_RegisterIntrHandlerCallBack(AL_AXI_ETHLITE_HalStruct *Handle,
                                                    AL_AXI_ETHLITE_IntrEventEnum IntrId, AL_VOID *CallBackHandler)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL) && (CallBackHandler != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    switch (IntrId) {
    case AL_AXI_ETHLITE_INTR_RX_VALID:
        Handle->RxValidCallBack = ((AL_AXI_ETHLITE_RxValidCallBack)CallBackHandler);
        break;

    case AL_AXI_ETHLITE_INTR_TX_DONE:
        Handle->TxDoneCallBack = ((AL_AXI_ETHLITE_TxDoneCallBack)CallBackHandler);
        break;

    default:
        Ret = AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM;
        break;
    }

    return Ret;
}

AL_S32 AlAxiEthLite_Hal_SendFrame(AL_AXI_ETHLITE_HalStruct *Handle, AL_U8 *SendBuffer, AL_U16 SendLen)
{
    AL_ASSERT((Handle != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_S32 Ret;

    Ret = AlAxiEthLite_Dev_SendFrame(&Handle->Dev, SendBuffer, SendLen);

    return Ret;
}

AL_S32 AlAxiEthLite_Hal_RecvFrame(AL_AXI_ETHLITE_HalStruct *Handle, AL_U8 *RecvBuffer, AL_U16 *RecvLen)
{
    AL_ASSERT((Handle != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_S32 Ret;

    Ret = AlAxiEthLite_Dev_RecvFrame(&Handle->Dev, RecvBuffer, RecvLen);

    return Ret;
}

AL_BOOL AlAxiEthLite_Hal_IsTxBufferAvailable(AL_AXI_ETHLITE_HalStruct *Handle)
{
    AL_ASSERT((Handle != AL_NULL), AL_FALSE);

    AL_BOOL Status;

    Status = AlAxiEthLite_Dev_IsTxBufferAvailable(&Handle->Dev);

    return Status;
}

AL_S32 AlAxiEthLite_Hal_SendFrameIntr(AL_AXI_ETHLITE_HalStruct *Handle, AL_U8 *SendBuffer, AL_U16 SendLen)
{
    AL_ASSERT((Handle != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_S32 Ret;

    Ret = AlAxiEthLite_Dev_SendFrameIntr(&Handle->Dev, SendBuffer, SendLen);

    return Ret;
}

AL_S32 AlAxiEthLite_Hal_RecvFrameIntr(AL_AXI_ETHLITE_HalStruct *Handle, AL_U8 *RecvBuffer, AL_U16 *RecvLen)
{
    AL_ASSERT((Handle != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_S32 Ret;

    Ret = AlAxiEthLite_Dev_RecvFrameIntr(&Handle->Dev, RecvBuffer, RecvLen);

    return Ret;
}

AL_S32 AlAxiEthLite_Hal_EnableIntr(AL_AXI_ETHLITE_HalStruct *Handle)
{
    AL_ASSERT((Handle != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_S32 Ret;

    Ret = AlAxiEthLite_Dev_EnableIntr(&Handle->Dev);

    return Ret;
}

AL_S32 AlAxiEthLite_Hal_ReadPhyRegister(AL_AXI_ETHLITE_HalStruct *Handle, AL_U32 PHYAddr,
                                        AL_U32 PHYReg, AL_U16 *RegValue)
{
    AL_ASSERT((Handle != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_S32 Ret;

    Ret = AlAxiEthLite_Dev_ReadPhyRegister(&Handle->Dev, PHYAddr, PHYReg, RegValue);

    return Ret;
}

AL_S32 AlAxiEthLite_Hal_WritePhyRegister(AL_AXI_ETHLITE_HalStruct *Handle, AL_U32 PHYAddr,
                                         AL_U32 PHYReg, AL_U16 RegValue)
{
    AL_ASSERT((Handle != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_S32 Ret;

    Ret = AlAxiEthLite_Dev_WritePhyRegister(&Handle->Dev, PHYAddr, PHYReg, RegValue);

    return Ret;
}

AL_S32 AlAxiEthLite_Hal_ConfigDuplexAndSpeed(AL_AXI_ETHLITE_HalStruct *Handle,
                                             AL_AXI_ETHLITE_SpeedEnum Speed, AL_U8 Duplex)
{
    AL_ASSERT((Handle != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_S32 Ret;

    Ret = AlAxiEthLite_Dev_ConfigDuplexAndSpeed(&Handle->Dev, Speed, Duplex);

    return Ret;
}