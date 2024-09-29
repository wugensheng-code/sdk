/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_axi_dma_hal.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
AlAxiDma_HalStruct AXI_DMA_Handle[AL_AXI_DMA_NUM_INSTANCE];

// Define an array of error information
ErrorInfo errorTable[] = {
        {DMA_INTERNAL_ERROR, "DMA Internal Error"},
        {DMA_SLAVE_ERROR, "DMA Slave Error"},
        {DMA_DECODE_ERROR, "DMA Decode Error"},
        {SG_INTERNAL_ERROR, "SG Internal Error"},
        {SG_SLAVE_ERROR, "SG Slave Error"},
        {SG_DECODE_ERROR, "SG Decode Error"}
};
/********************************************************/

/**
 * This function is called back when an AXI DMA event occurs and handles the event by sending it to the
 * appropriate message queue based on the type of event.
 *
 * @param AxiDmaEvent Pointer to the AXI DMA event structure containing details about the event.
 * @param CallbackRef A void pointer to the AXI DMA handle structure.
 * @return void
 */
static AL_VOID AlAxiDma_Hal_DefEventHandler(AlAxiDma_EventStruct *AxiDmaEvent, AL_VOID *CallbackRef)
{
    AlAxiDma_HalStruct *Handle = (AlAxiDma_HalStruct *)CallbackRef;
    AL_U64 ChanBase;

    switch (AxiDmaEvent->Event) {
        case EVENT_S2MM_DONE:
            AlOsal_Mb_Send(&Handle->S2mm_EventQueue, AxiDmaEvent);
            break;

        case EVENT_S2MM_CYCLIC:
            ChanBase = Handle->Dma.RegBase + AL_AXI_DMA_S2MM_OFFSET;
            AlAxiDma_ll_Reset(ChanBase, AL_FUNC_ENABLE);
            AlOsal_Mb_Send(&Handle->S2mm_EventQueue, AxiDmaEvent);
            break;

        case EVENT_S2MM_ERROR:
            for (int i = 0; i < sizeof(errorTable)/sizeof(ErrorInfo); ++i) {
                if (AxiDmaEvent->EventData & errorTable[i].errorBit) {
                    AL_LOG(AL_LOG_LEVEL_ERROR, "S2mm transfer Error: %s", errorTable[i].errorMsg);
                }
            }
            AlOsal_Mb_Send(&Handle->S2mm_EventQueue, AxiDmaEvent);
            break;

        case EVENT_MM2S_DONE:
            AlOsal_Mb_Send(&Handle->Mm2s_EventQueue, AxiDmaEvent);
            break;

        case EVENT_MM2S_CYCLIC:
            ChanBase = Handle->Dma.RegBase + AL_AXI_DMA_MM2S_OFFSET;
            AlAxiDma_ll_Reset(ChanBase, AL_FUNC_ENABLE);
            AlOsal_Mb_Send(&Handle->Mm2s_EventQueue, AxiDmaEvent);
            break;

        case EVENT_MM2S_ERROR:
            for (int i = 0; i < sizeof(errorTable)/sizeof(ErrorInfo); ++i) {
                if (AxiDmaEvent->EventData & errorTable[i].errorBit) {
                    AL_LOG(AL_LOG_LEVEL_ERROR, "Mm2s transfer Error: %s", errorTable[i].errorMsg);
                }
            }
            AlOsal_Mb_Send(&Handle->Mm2s_EventQueue, AxiDmaEvent);
            break;

        default:
            break;
    }
}

/**
 * This function sets up the hardware configuration for the AXI DMA, initializes locks and message queues for
 * MM2S and S2MM, and registers the event callback function.
 *
 * @param Handle A pointer to a pointer of the AXI DMA handle structure to be initialized.
 * @param DevId The device ID of the AXI DMA to be initialized.
 * @param InitConfig A pointer to the AXI DMA initialization structure.
 * @param EventCallBack The event callback function to be registered.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlAxiDma_Hal_Init(AlAxiDma_HalStruct **Handle, AL_U32 DevId, AlAxiDma_InitStruct *InitConfig, AlAxiDma_EventCallBack EventCallBack)
{
    AL_S32 Ret = AL_OK;
    AlAxiDma_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT((Handle != AL_NULL), AL_AxiDma_ERR_NULL_PTR);

    HwConfig = AlAxiDma_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AXI_DMA_Handle[DevId];
    } else {
        return AL_AxiDma_ERR_ILLEGAL_PARAM;
    }

    Ret = AlAxiDma_Dev_Init(&(*Handle)->Dma, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    if (EventCallBack != AL_NULL) {
        Ret = AlAxiDma_Dev_RegisterEventCallBack(&(*Handle)->Dma, EventCallBack, (AL_VOID *)*Handle);
    } else {
        Ret = AlAxiDma_Dev_RegisterEventCallBack(&(*Handle)->Dma, AlAxiDma_Hal_DefEventHandler, (AL_VOID *)*Handle);
    }
    if (Ret != AL_OK) {
        return Ret;
    }

    if((*Handle)->Dma.HwConfig.HasMm2s) {
        (AL_VOID)AlIntr_RegHandler((*Handle)->Dma.HwConfig.IntrNum_Mm2s, AL_NULL, AlAxiDma_Dev_Mm2s_IntrHandler, &(*Handle)->Dma);
    }

    if((*Handle)->Dma.HwConfig.HasS2mm) {
        (AL_VOID)AlIntr_RegHandler((*Handle)->Dma.HwConfig.IntrNum_S2mm, AL_NULL, AlAxiDma_Dev_S2mm_IntrHandler, &(*Handle)->Dma);
    }

    Ret = AlOsal_Lock_Init(&(*Handle)->Mm2s_Lock, "AxiDma_Mm2s_Lock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Lock_Init(&(*Handle)->S2mm_Lock, "AxiDma_S2mm_Lock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Mb_Init(&(*Handle)->Mm2s_EventQueue, "AxiDma_Mm2s_MailBox");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Mb_Init(&(*Handle)->S2mm_EventQueue, "AxiDma_S2mm_MailBox");
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}

/**
 * This function performs a direct mode transfer for the AXI DMA in block transfer mode. It sets up the transfer,
 * manages cache if MMU is enabled, waits for the transfer to complete, and handles any errors.
 *
 * @param Handle A pointer to the AXI DMA handle structure.
 * @param Buffer Pointer to the buffer to transfer.
 * @param Length The length of the buffer to transfer.
 * @param Direction The direction of the transfer (AL_AXIDMA_DEVICE_TO_DMA or AL_AXIDMA_DMA_TO_DEVICE).
 * @param Timeout The timeout value for waiting for the completion event.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlAxiDma_Hal_DirectMode_TransferBlock(AlAxiDma_HalStruct *Handle, AL_U8 *Buffer, AL_U32 Length, AL_U32 Direction, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_AxiDma_ERR_NULL_PTR);

    AlAxiDma_EventStruct AxiDmaEvent = {0};
    AL_Lock_t Lock = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_Lock : &Handle->Mm2s_Lock;
    AL_MailBox_t EventQueue = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_EventQueue : &Handle->Mm2s_EventQueue;
    AlAxiDma_EventEnum ExpectedEvent = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? EVENT_S2MM_DONE : EVENT_MM2S_DONE;

    // Take the lock
    Ret = AlOsal_Lock_Take(Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

#ifdef ENABLE_MMU
    // Manage cache based on Direction
    if (Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        AlCache_InvalidateDcacheRange((AL_UINTPTR)Buffer, (AL_UINTPTR)Buffer + Length);
    } else {
        AlCache_FlushDcacheRange((AL_UINTPTR)Buffer, (AL_UINTPTR)Buffer + Length);
    }
#endif

    // Perform the DMA transfer
    Ret = AlAxiDma_Dev_DirectMode_Transfer(&Handle->Dma, Buffer, Length, Direction, AL_AXIDMA_BLOCK);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(Lock);
        return Ret;
    }

    // Wait for the completion event
    Ret = AlOsal_Mb_Receive(EventQueue, &AxiDmaEvent, Timeout);
    if (Ret != AL_OK) {
        AlAxiDma_Dev_Stop_Transfer(&Handle->Dma, Direction);
        (AL_VOID)AlOsal_Mb_Receive(EventQueue, &AxiDmaEvent, AL_WAITING_NO);
    }

    // Release the lock
    (AL_VOID)AlOsal_Lock_Release(Lock);

    if (Ret == AL_OK && AxiDmaEvent.Event == ExpectedEvent) {
        return Ret;
    }

    // Return appropriate error code
    return (Ret != AL_OK) ? Ret : AL_AxiDma_EVENTS_TO_ERRS(AxiDmaEvent.Event);
}

/**
 * This function performs a direct mode transfer for the AXI DMA in polling mode. It sets up the transfer,
 * manages cache if MMU is enabled, and handles any errors.
 *
 * @param Handle A pointer to the AXI DMA handle structure.
 * @param Buffer Pointer to the buffer to transfer.
 * @param Length The length of the buffer to transfer.
 * @param Direction The direction of the transfer (AL_AXIDMA_DEVICE_TO_DMA or AL_AXIDMA_DMA_TO_DEVICE).
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlAxiDma_Hal_DirectMode_TransferPolling(AlAxiDma_HalStruct *Handle, AL_U8 *Buffer, AL_U32 Length, AL_U32 Direction)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_AxiDma_ERR_NULL_PTR);

    AL_Lock_t Lock = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_Lock : &Handle->Mm2s_Lock;

    // Take the lock
    Ret = AlOsal_Lock_Take(Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

#ifdef ENABLE_MMU
    // Manage cache based on Direction
    if (Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        AlCache_InvalidateDcacheRange((AL_UINTPTR)Buffer, (AL_UINTPTR)Buffer + Length);
    } else {
        AlCache_FlushDcacheRange((AL_UINTPTR)Buffer, (AL_UINTPTR)Buffer + Length);
    }
#endif

    // Perform the DMA transfer
    Ret = AlAxiDma_Dev_DirectMode_Transfer(&Handle->Dma, Buffer, Length, Direction, AL_AXIDMA_POLLING);

    // Release the lock
    (AL_VOID)AlOsal_Lock_Release(Lock);

    return Ret;
}

/**
 * This function performs a scatter-gather mode transfer for the AXI DMA in block transfer mode. It sets up the transfer,
 * manages cache if MMU is enabled, waits for the transfer to complete, and handles any errors.
 *
 * @param Handle A pointer to the AXI DMA handle structure.
 * @param Msg A pointer to the transfer message structure containing buffer details.
 * @param Timeout The timeout value for waiting for the completion event.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlAxiDma_Hal_SgMode_TransferBlock(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT(Handle != AL_NULL && Msg != AL_NULL, AL_AxiDma_ERR_NULL_PTR);

    AlAxiDma_EventStruct AxiDmaEvent = {0};
    AL_Lock_t Lock = (Msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_Lock : &Handle->Mm2s_Lock;
    AL_MailBox_t EventQueue = (Msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_EventQueue : &Handle->Mm2s_EventQueue;
    AlAxiDma_EventEnum ExpectedEvent = (Msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) ?
        (Handle->Dma.InitConfig.S2mm_HasCyclic ? EVENT_S2MM_CYCLIC : EVENT_S2MM_DONE) :
        (Handle->Dma.InitConfig.Mm2s_HasCyclic ? EVENT_MM2S_CYCLIC : EVENT_MM2S_DONE);

    // Take the lock
    Ret = AlOsal_Lock_Take(Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

#ifdef ENABLE_MMU
    // Manage cache based on Direction
    for (AL_U32 i = 0; i < Msg->NumBuffers; i++) {
        AL_UINTPTR BuffAddr = (AL_UINTPTR)Msg->Buffers[i];
        AL_U32 Length = Msg->BufferLengths[i];

        if (Msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) {
            AlCache_InvalidateDcacheRange(BuffAddr, BuffAddr + Length);
        } else {
            AlCache_FlushDcacheRange(BuffAddr, BuffAddr + Length);
        }
    }
#endif

    // Perform the DMA transfer
    Ret = AlAxiDma_Dev_SgMode_Transfer(&Handle->Dma, Msg->NumBuffers, Msg->Direction, AL_AXIDMA_BLOCK);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(Lock);
        return Ret;
    }

    // Wait for the completion event
    Ret = AlOsal_Mb_Receive(EventQueue, &AxiDmaEvent, Timeout);
    if (Ret != AL_OK) {
        AlAxiDma_Dev_Stop_Transfer(&Handle->Dma, Msg->Direction);
        (AL_VOID)AlOsal_Mb_Receive(EventQueue, &AxiDmaEvent, AL_WAITING_NO);
    }

    // Release the lock
    (AL_VOID)AlOsal_Lock_Release(Lock);

    if (Ret == AL_OK && AxiDmaEvent.Event == ExpectedEvent) {
        return Ret;
    }

    // Return appropriate error code
    return (Ret != AL_OK) ? Ret : AL_AxiDma_EVENTS_TO_ERRS(AxiDmaEvent.Event);
}

/**
 * This function performs a scatter-gather mode transfer for the AXI DMA in polling mode. It sets up the transfer,
 * manages cache if MMU is enabled, and handles any errors.
 *
 * @param Handle A pointer to the AXI DMA handle structure.
 * @param Msg A pointer to the transfer message structure containing buffer details.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlAxiDma_Hal_SgMode_TransferPolling(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT(Handle != AL_NULL && Msg != AL_NULL, AL_AxiDma_ERR_NULL_PTR);

    AL_Lock_t Lock = (Msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_Lock : &Handle->Mm2s_Lock;

    // Take the lock
    Ret = AlOsal_Lock_Take(Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

#ifdef ENABLE_MMU
    // Manage cache based on Direction
    for (AL_U32 i = 0; i < Msg->NumBuffers; i++) {
        AL_UINTPTR BuffAddr = (AL_UINTPTR)Msg->Buffers[i];
        AL_U32 Length = Msg->BufferLengths[i];

        if (Msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) {
            AlCache_InvalidateDcacheRange(BuffAddr, BuffAddr + Length);
        } else {
            AlCache_FlushDcacheRange(BuffAddr, BuffAddr + Length);
        }
    }
#endif

    // Perform the DMA transfer
    Ret = AlAxiDma_Dev_SgMode_Transfer(&Handle->Dma, Msg->NumBuffers, Msg->Direction, AL_AXIDMA_POLLING);

    // Release the lock
    (AL_VOID)AlOsal_Lock_Release(Lock);

    return Ret;
}

/**
 * This function sets up the descriptors for scatter-gather (SG) mode transfer in the AXI DMA. It configures each descriptor
 * based on the provided buffers and lengths, and ensures the total transfer length does not exceed the hardware limit.
 *
 * @param Handle A pointer to the AXI DMA handle structure.
 * @param Msg A pointer to the transfer message structure containing buffer details.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlAxiDma_Hal_SetupDescriptors(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg)
{
    AL_S32 Ret = AL_OK;
    AL_U32 TotalLength = 0;
    AL_U32 SgTotalMaxTransferLen;

    AL_ASSERT(Handle != AL_NULL && Msg != AL_NULL, AL_AxiDma_ERR_NULL_PTR);

    for (int i = 0; i < Msg->NumBuffers; i++) {
        TotalLength += Msg->BufferLengths[i];
        Ret = AlAxiDma_Dev_SetupDescriptors(&Handle->Dma, i, Msg->Buffers[i], Msg->BufferLengths[i], Msg->NumBuffers, Msg->Direction);
        if (Ret != AL_OK) {
            return Ret;
        }
    }

    /* The maximum amount of data can be transmitted in a frame,
     * i.e. the total number of descriptors transmitted,
     * do not exceed (1 << buffer length) - 1
     */
    SgTotalMaxTransferLen = (1UL << Handle->Dma.HwConfig.BufferlenWidth) - 1;
    if (TotalLength > SgTotalMaxTransferLen) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Invalid transfer length: %lu. Total maximum allowed length: %lu", TotalLength, SgTotalMaxTransferLen);
        return AL_AxiDma_ERR_ILLEGAL_PARAM;
    }

    // Set the next_desc and next_desc_msb of the last descriptor to point to the first descriptor
    AL_UINTPTR first_desc_addr = (AL_UINTPTR)&Handle->Dma.descriptors[0];
    Handle->Dma.descriptors[Msg->NumBuffers - 1].next_desc = (AL_U32)(first_desc_addr & 0xFFFFFFFF);
    Handle->Dma.descriptors[Msg->NumBuffers - 1].next_desc_msb = (AL_U32)(first_desc_addr >> 32);

    if (Msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        Handle->Dma.S2mmCyclicCount = Msg->S2mmCyclicCount;
    } else {
        Handle->Dma.Mm2sCyclicCount = Msg->Mm2sCyclicCount;
    }

    return Ret;
}

/**
 * This function sets the application-specific word in the descriptor's APP array for the last buffer. If the hardware
 * build has Rxlength in the status stream set to 1, the last APP word must have a non-zero value when ANDed with 0x7FFFFF
 * to avoid hardware stalling.
 *
 * @param Handle A pointer to the AXI DMA handle structure.
 * @param Msg A pointer to the transfer message structure containing buffer details.
 * @param Offset The offset within the APP array where the word should be set (must be between 0 and 4).
 * @param Word The application-specific word to set.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlAxiDma_Hal_SetDescAppWord(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg, AL_U32 Offset, AL_U32 Word)
{
    if (!Handle->Dma.HwConfig.EnableStatusControlStream) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "stream in hardware not build");
        return AL_AxiDma_ERR_NOT_SUPPORT;
    }

    if ((Offset < 0) || (Offset > 4)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "invalid offset %d", Offset);
        return AL_AxiDma_ERR_ILLEGAL_PARAM;
    }

    Handle->Dma.descriptors[Msg->NumBuffers - 1].app[Offset] = Word;

    return AL_OK;
}

/**
 * This function retrieves the application-specific word from the descriptor's APP array for the last buffer. The function
 * checks the hardware configuration for status control stream support and validates the provided offset.
 *
 * @param Handle A pointer to the AXI DMA handle structure.
 * @param Msg A pointer to the transfer message structure containing buffer details.
 * @param Offset The offset within the APP array from which the word should be retrieved (must be between 0 and 4).
 * @return Returns the value of the application-specific word, or an error code on failure.
 */
AL_S32 AlAxiDma_Hal_GetDescAppWord(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg, AL_U32 Offset)
{
    if (!Handle->Dma.HwConfig.EnableStatusControlStream) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "stream in hardware not build");
        return AL_AxiDma_ERR_NOT_SUPPORT;
    }

    if ((Offset < 0) || (Offset > 4)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "invalid offset %d", Offset);
        return AL_AxiDma_ERR_ILLEGAL_PARAM;
    }

    return (AL_S32)Handle->Dma.descriptors[Msg->NumBuffers - 1].app[Offset];
}
