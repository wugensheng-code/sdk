/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_AXI_ETHLITE_HAL_H_
#define __AL_AXI_ETHLITE_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_axi_ethlite_dev.h"

typedef AL_VOID (*AL_AXI_ETHLITE_RxValidCallBack)(AL_VOID *CallbackRef);
typedef AL_VOID (*AL_AXI_ETHLITE_TxDoneCallBack)(AL_VOID *CallbackRef);

typedef struct
{
    AL_AXI_ETHLITE_DevStruct            Dev;

    AL_AXI_ETHLITE_RxValidCallBack      RxValidCallBack;
    AL_AXI_ETHLITE_TxDoneCallBack       TxDoneCallBack;

} AL_AXI_ETHLITE_HalStruct;

AL_S32 AlAxiEthLite_Hal_Init(AL_AXI_ETHLITE_HalStruct **Handle, AL_U32 DevId,
                             AL_AXI_ETHLITE_InitStruct *InitConfig, AL_AXI_ETHLITE_EventCallBack Callback);

AL_S32 AlAxiEthLite_Hal_SendFrame(AL_AXI_ETHLITE_HalStruct *Handle, AL_U8 *SendBuffer, AL_U16 SendLen);

AL_S32 AlAxiEthLite_Hal_RecvFrame(AL_AXI_ETHLITE_HalStruct *Handle, AL_U8 *RecvBuffer, AL_U16 *RecvLen);

AL_BOOL AlAxiEthLite_Hal_IsTxBufferAvailable(AL_AXI_ETHLITE_HalStruct *Handle);

AL_S32 AlAxiEthLite_Hal_SendFrameIntr(AL_AXI_ETHLITE_HalStruct *Handle, AL_U8 *SendBuffer, AL_U16 SendLen);

AL_S32 AlAxiEthLite_Hal_RecvFrameIntr(AL_AXI_ETHLITE_HalStruct *Handle, AL_U8 *RecvBuffer, AL_U16 *RecvLen);

AL_S32 AlAxiEthLite_Hal_EnableIntr(AL_AXI_ETHLITE_HalStruct *Handle);

AL_S32 AlAxiEthLite_Hal_ReadPhyRegister(AL_AXI_ETHLITE_HalStruct *Handle, AL_U32 PHYAddr,
                                        AL_U32 PHYReg, AL_U16 *RegValue);

AL_S32 AlAxiEthLite_Hal_WritePhyRegister(AL_AXI_ETHLITE_HalStruct *Handle, AL_U32 PHYAddr,
                                         AL_U32 PHYReg, AL_U16 RegValue);

AL_S32 AlAxiEthLite_Hal_ConfigDuplexAndSpeed(AL_AXI_ETHLITE_HalStruct *Handle,
                                             AL_AXI_ETHLITE_SpeedEnum Speed, AL_U8 Duplex);

#ifdef __cplusplus
}
#endif

#endif /* __AL_AXI_ETHLITE_HAL_H_ */
