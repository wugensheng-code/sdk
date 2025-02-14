/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include <stdlib.h>
#include "al_type.h"
#include "al_dma_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
AL_DMA_HalStruct AL_DMA_HalInstance[AL_DMA_CHAN_COUNT];

AL_DMA_DevStruct AlDma_Dev[AL_DMA_NUM_INSTANCE];

extern AL_DMA_HwConfigStruct AlDma_HwConfig[AL_DMA_NUM_INSTANCE];

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 * This function is called when a DMA event occurs and handles specific events, such as transfer completion.
 *
 * @param Event Pointer to the DMA event structure containing event details.
 * @param CallBackRef User-defined reference passed to the callback function. In this case, it's a pointer to the DMA handle structure.
 * @return None
 */
static AL_VOID AlDma_Hal_DefEventCallBack(AL_DMA_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_DMA_HalStruct *Handle = (AL_DMA_HalStruct *)CallBackRef;

    switch (Event->EventId)
    {
    case AL_DMA_EVENT_TRANS_DONE:
        AlOsal_Mb_Send(&(Handle->EventQueue), Event);
        break;
    default:
        break;
    }
}

/**
 * This function initializes the DMA device, including hardware configuration and device locking.
 *
 * @param DevId The device ID of the DMA device to setup.
 * @return AL_S32 Returns AL_OK on success, AL_ERR_MAX if the device configuration is not found.
 */
AL_S32 AlDma_Hal_Setup(AL_U32 DevId)
{
    AL_S32 Ret = AL_OK;
    AL_DMA_DevStruct *Dev = AL_NULL;
    AL_DMA_HwConfigStruct *HwConfig = AL_NULL;

    HwConfig = AlDma_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        Dev = &AlDma_Dev[DevId];
    } else {
        return AL_ERR_MAX;
    }

    while (!AlDma_Dev_FetchLock(HwConfig->LockAddr));
    if (!Dev->Init) {
        Dev->Init = AL_TRUE;
        Dev->HwCfg = HwConfig;
        Dev->CtrlReg = (AL_DMA_DmaCtrlRegStruct *)(AL_UINTPTR)Dev->HwCfg->BaseAddress;
        AlDma_Dev_Reset();
        Ret = AlDma_Dev_Init(Dev);
        AL_ASSERT(Ret == AL_OK, Ret);
    }
    AlDma_Dev_ReleaseLock(HwConfig->LockAddr);

    return Ret;
}

/**
 * This function sets up the lock and event queue for the DMA handle.
 *
 * @param Handle Pointer to the DMA handle structure to initialize.
 * @return AL_S32 Returns AL_OK on success.
 */
static AL_S32 AlDma_Hal_HandleInit(AL_DMA_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    Ret = AlOsal_Lock_Init(&(Handle->Lock), "DMALOCK");
    AL_ASSERT(Ret == AL_OK, Ret);

    AlOsal_Mb_Init(&(Handle->EventQueue), "DMAQUEUE");
    AL_ASSERT(Ret == AL_OK, Ret);

    return AL_OK;
}

/**
 * This function sets up the DMA device, requests a channel, and configures it according to the specified initialization configuration.
 *
 * @param Handle Pointer to a pointer of the DMA handle structure. This will be initialized and returned to the caller.
 * @param DevId The device ID of the DMA device to initialize.
 * @param Initconfig Pointer to the DMA channel configuration structure.
 * @return AL_S32 Returns AL_OK on success, -1 if channel request fails.
 */
AL_S32 AlDma_Hal_Init(AL_DMA_HalStruct **Handle, AL_U32 DevId, AL_DMA_ChCfgStruct *Initconfig)
{
    AL_S32 Ret   = AL_OK;
    AL_DMA_DevStruct *Dev = AL_NULL;
    AL_DMA_ChStruct *Chan = AL_NULL;

    AL_ASSERT(Handle != AL_NULL, AL_ERR_NULL_PTR);

    AlDma_Hal_Setup(DevId);
    AL_ASSERT(Ret == AL_OK, AL_ERR_MAX);

    Dev = &AlDma_Dev[DevId];

    Chan = AlDma_Dev_RequestChannel(Dev);
    if (Chan == AL_NULL) {
        AL_LOG(AL_LOG_LEVEL_INFO, "AlDma_Dev_RequestChannel == NULL \r\n");
        return -1;
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "Request channel %d success\r\n", Chan->ChanId);
    }

    *Handle = &AL_DMA_HalInstance[Chan->ChanId];
    (*Handle)->Chan = Chan;
    (*Handle)->Dev = Dev;

    AlDma_Dev_RegisterEventCallBack(Chan, AlDma_Hal_DefEventCallBack, *Handle);

    AlIntr_RegHandler(Dev->HwCfg->IntrNum[Chan->ChanId], AL_NULL, AlDma_Dev_IrqHandler, Chan);
    AL_LOG(AL_LOG_LEVEL_INFO, "intr %d \r\n", Dev->HwCfg->IntrNum[Chan->ChanId]);

    Ret = AlDma_Dev_Config(Chan, Initconfig);
    AL_ASSERT(Ret == AL_OK, Ret);

    Ret = AlDma_Hal_HandleInit(*Handle);
    AL_ASSERT(Ret == AL_OK, Ret);

    return AL_OK;
}

/**
 * This function releases the DMA channel and performs device-specific de-initialization if necessary.
 *
 * @param Handle Pointer to the DMA handle structure.
 * @return AL_S32 Returns AL_OK on success.
 */
AL_S32 AlDma_Hal_DeInit(AL_DMA_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    while (!AlDma_Dev_FetchLock(Handle->Dev->HwCfg->LockAddr));

    AlDma_Dev_ReleaseChannel(Handle->Chan);

    for (AL_U32 i = 0; i < AL_DMA_CHAN_COUNT; i++) {
        if (Handle->Dev->Chans[i].Used) {
            goto END;
        }
    }

    AlDma_Dev_DeInit(Handle->Dev);

END:
    AlDma_Dev_ReleaseLock(Handle->Dev->HwCfg->LockAddr);
    return Ret;
}

/**
 * This function prepares a DMA memory copy operation, starts it, and waits for completion.
 *
 * @param Handle Pointer to the DMA handle structure.
 * @param Dst Destination address for the memory copy.
 * @param Src Source address for the memory copy.
 * @param Len Length of the memory to copy in bytes.
 * @return AL_S32 Returns AL_OK on success, error code on failure.
 */
AL_S32 AlDma_Hal_MemCpyBlock(AL_DMA_HalStruct *Handle, AL_UINTPTR Dst, AL_UINTPTR Src, AL_U32 Len)
{
    AL_MailBox Event;
    AL_S32 Ret;

    AlOsal_Lock_Take(&(Handle->Lock), AL_WAITFOREVER);

    /*0. AlDma_Dev_PrepDmaMemcpy */
    Ret = AlDma_Dev_PrepDmaMemcpy(Handle->Chan, Dst, Src, Len);
    AL_ASSERT(Ret == AL_OK, Ret);

    /*1: AlDma_Dev_Start */
    Ret = AlDma_Dev_Start(Handle->Chan);
    AL_ASSERT(Ret == AL_OK, Ret);

    /*2: Wait done of memcpy */
    Ret = AlOsal_Mb_Receive(&Handle->EventQueue, &Event, AL_WAITFOREVER);

    AlOsal_Lock_Release(&(Handle->Lock));

    return Ret;
}

/**
 * This function prepares a DMA peripheral copy operation for a single block, starts it, and waits for completion.
 *
 * @param Handle Pointer to the DMA handle structure.
 * @param MemAddr Memory address for the operation.
 * @param Len Length of the data to copy in bytes.
 * @param PerId Peripheral ID for the operation.
 * @return AL_S32 Returns AL_OK on success, error code on failure.
 */
AL_S32 AlDma_Hal_PeriCpySingleBlock(AL_DMA_HalStruct *Handle, AL_UINTPTR MemAddr, AL_U32 Len, AL_U32 PerId)
{
    AL_S32 Ret;
    AL_MailBox Event;

    Handle->Chan->PeriId = PerId;

    AlOsal_Lock_Take(&(Handle->Lock), AL_WAITFOREVER);

    Ret = AlDma_Dev_PrepDmaSingle(Handle->Chan, MemAddr, Len, Handle->Chan->Dir);
    AL_ASSERT(Ret == AL_OK, Ret);

    /*1: AlDma_Dev_Start */
    Ret = AlDma_Dev_Start(Handle->Chan);
    AL_ASSERT(Ret == AL_OK, Ret);

    /*2: Wait done of Peri cpy */
    Ret = AlOsal_Mb_Receive(&Handle->EventQueue, &Event, AL_WAITFOREVER);

    AlOsal_Lock_Release(&(Handle->Lock));

    return Ret;
}

/**
 * This function prepares a DMA peripheral copy operation for cyclic transfers, starts it, and waits for completion.
 *
 * @param Handle Pointer to the DMA handle structure.
 * @param MemAddr Memory address for the operation.
 * @param Len Length of the data to copy in bytes.
 * @param PerId Peripheral ID for the operation.
 * @return AL_S32 Returns AL_OK on success, error code on failure.
 */
AL_S32 AlDma_Hal_PeriCpyCycleBlock(AL_DMA_HalStruct *Handle, AL_UINTPTR MemAddr, AL_U32 Len, AL_U32 PerId)
{
    AL_S32 Ret;
    AL_MailBox Event;

    Handle->Chan->PeriId = PerId;

    AlOsal_Lock_Take(&(Handle->Lock), AL_WAITFOREVER);

    Ret = AlDma_Dev_PrepDmaCyclic(Handle->Chan, MemAddr, Len, Len, Handle->Chan->Dir);
    AL_ASSERT(Ret == AL_OK, Ret);

    /*1: AlDma_Dev_Start */
    Ret = AlDma_Dev_Start(Handle->Chan);
    AL_ASSERT(Ret == AL_OK, Ret);

    /*2: Wait done of Peri cpy */
    Ret = AlOsal_Mb_Receive(&Handle->EventQueue, &Event, AL_WAITFOREVER);

    AlOsal_Lock_Release(&(Handle->Lock));

    return Ret;
}

/**
 * This function initializes the DMA, performs a memory copy, and then de-initializes the DMA.
 *
 * @param Dst Destination address for the memory copy.
 * @param Src Source address for the memory copy.
 * @param Len Length of the memory to copy in bytes.
 * @return AL_S32 Returns AL_OK on success, error code on failure.
 */
AL_S32 AlDma_Hal_MemCpy(AL_UINTPTR Dst, AL_UINTPTR Src, AL_U32 Len)
{
    AL_S32 Ret = AL_OK;
    AL_MailBox Event;
    AL_DMA_HalStruct *Handle = AL_NULL;
    AL_DMA_ChCfgStruct InitConfig = {
        .Direction = DMA_MEM_TO_MEM,
        .SrcAddrWidth = DMA_CH_BUSWIDTH_32_BYTES,
        .DstAddrWidth = DMA_CH_BUSWIDTH_32_BYTES,
        .SrcMaxBurst  = 16,
        .DstMaxBurst  = 4,
    };
    AlDma_Hal_Init(&Handle, 0, &InitConfig);

    AlOsal_Lock_Take(&(Handle->Lock), AL_WAITFOREVER);

    /*0. AlDma_Dev_PrepDmaMemcpy */
    Ret = AlDma_Dev_PrepDmaMemcpy(Handle->Chan, Dst, Src, Len);
    AL_ASSERT(Ret == AL_OK, Ret);

    /*1: AlDma_Dev_Start */
    Ret = AlDma_Dev_Start(Handle->Chan);
    AL_ASSERT(Ret == AL_OK, Ret);

    /*2: Wait done of memcpy */
    Ret = AlOsal_Mb_Receive(&(Handle->EventQueue), &Event, AL_WAITFOREVER);

    AlOsal_Lock_Release(&(Handle->Lock));

    AlDma_Hal_DeInit(Handle);

    return Ret;
}

/**
 * This function prepares a DMA peripheral copy operation for a single block and starts it.
 *
 * @param Handle Pointer to the DMA handle structure.
 * @param MemAddr Memory address for the operation.
 * @param Len Length of the data to copy in bytes.
 * @param PerId Peripheral ID for the operation.
 * @return AL_S32 Returns AL_OK on success, error code on failure.
 */
AL_S32 AlDma_Hal_PeriCpySingle(AL_DMA_HalStruct *Handle, AL_UINTPTR MemAddr, AL_U32 Len, AL_U32 PerId)
{
    AL_S32 Ret;

    AlOsal_Lock_Take(&(Handle->Lock), AL_WAITFOREVER);

    Handle->Chan->PeriId = PerId;

    Ret = AlDma_Dev_PrepDmaSingle(Handle->Chan, MemAddr, Len, Handle->Chan->Dir);
    AL_ASSERT(Ret == AL_OK, Ret);

    /*1: AlDma_Dev_Start */
    Ret = AlDma_Dev_Start(Handle->Chan);
    AL_ASSERT(Ret == AL_OK, Ret);

    AlOsal_Lock_Release(&(Handle->Lock));

    return Ret;
}

/**
 * This function prepares a DMA peripheral copy operation for cyclic transfers and starts it.
 *
 * @param Handle Pointer to the DMA handle structure.
 * @param MemAddr Memory address for the operation.
 * @param Len Length of the data to copy in bytes.
 * @param PerId Peripheral ID for the operation.
 * @return AL_S32 Returns AL_OK on success, error code on failure.
 */
AL_S32 AlDma_Hal_PeriCpyCycle(AL_DMA_HalStruct *Handle, AL_UINTPTR MemAddr, AL_U32 Len, AL_U32 PerId)
{
    AL_S32 Ret;

    AlOsal_Lock_Take(&(Handle->Lock), AL_WAITFOREVER);

    Handle->Chan->PeriId = PerId;

    Ret = AlDma_Dev_PrepDmaCyclic(Handle->Chan, MemAddr, Len, Len, Handle->Chan->Dir);
    AL_ASSERT(Ret == AL_OK, Ret);

    /*1: AlDma_Dev_Start */
    Ret = AlDma_Dev_Start(Handle->Chan);
    AL_ASSERT(Ret == AL_OK, Ret);

    AlOsal_Lock_Release(&(Handle->Lock));

    return Ret;
}

/**
 * This function stops the DMA channel associated with the given handle.
 *
 * @param Handle Pointer to the DMA handle structure.
 * @return AL_S32 Returns AL_OK on success, error code on failure.
 */
AL_S32 AlDma_Hal_StopCpy(AL_DMA_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    AlOsal_Lock_Take(&(Handle->Lock), AL_WAITFOREVER);

    Ret = AlDma_Dev_Stop(Handle->Chan);

    AlOsal_Lock_Release(&(Handle->Lock));

    return Ret;
}

