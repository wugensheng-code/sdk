#include "al_axi_dma_hal.h"

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

static AL_VOID AlAxiDma_Hal_DefEventHandler(AlAxiDma_EventStruct *AxiDmaEvent, AL_VOID *CallbackRef)
{
    AlAxiDma_HalStruct *Handle = (AlAxiDma_HalStruct *)CallbackRef;
    AL_U64 ChanBase;

    switch (AxiDmaEvent->Event) {
        case EVENT_S2MM_DONE:
            AlOsal_Mb_Send(&Handle->S2mm_EventQueue, AxiDmaEvent);
            break;

        case EVENT_S2MM_CYCLIC:
            ChanBase = Handle->Dma.RegBase + ALAXIDMA_S2MM_OFFSET;
            AlAxiDma_ll_Reset(ChanBase, AL_FUNC_ENABLE);
            AlOsal_Mb_Send(&Handle->S2mm_EventQueue, AxiDmaEvent);
            break;

        case EVENT_S2MM_ERROR:
            AL_LOG(AL_LOG_LEVEL_ERROR, "S2MM Error Occurred:");
            for (int i = 0; i < sizeof(errorTable)/sizeof(ErrorInfo); ++i) {
                if (AxiDmaEvent->EventData & errorTable[i].errorBit) {
                    AL_LOG(AL_LOG_LEVEL_ERROR, "%s;", errorTable[i].errorMsg);
                }
            }
            AlOsal_Mb_Send(&Handle->S2mm_EventQueue, AxiDmaEvent);
            break;

        case EVENT_MM2S_DONE:
            AlOsal_Mb_Send(&Handle->Mm2s_EventQueue, AxiDmaEvent);
            break;

        case EVENT_MM2S_CYCLIC:
            ChanBase = Handle->Dma.RegBase + ALAXIDMA_MM2S_OFFSET;
            AlAxiDma_ll_Reset(ChanBase, AL_FUNC_ENABLE);
            AlOsal_Mb_Send(&Handle->Mm2s_EventQueue, AxiDmaEvent);
            break;

        case EVENT_MM2S_ERROR:
            AL_LOG(AL_LOG_LEVEL_ERROR, "MM2S Error Occurred:");
            for (int i = 0; i < sizeof(errorTable)/sizeof(ErrorInfo); ++i) {
                if (AxiDmaEvent->EventData & errorTable[i].errorBit) {
                    AL_LOG(AL_LOG_LEVEL_ERROR, "%s;", errorTable[i].errorMsg);
                }
            }
            AlOsal_Mb_Send(&Handle->Mm2s_EventQueue, AxiDmaEvent);
            break;

        default:
            break;
    }
}

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

AL_S32 AlAxiDma_Hal_DirectMode_TransferBlock(AlAxiDma_HalStruct *Handle, AL_U8 *Buffer, AL_U32 Length, AL_U32 Direction, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AlAxiDma_EventStruct AxiDmaEvent = {0};
    AL_Lock_t Lock = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_Lock : &Handle->Mm2s_Lock;
    AL_MailBox_t EventQueue = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_EventQueue : &Handle->Mm2s_EventQueue;
    AlAxiDma_EventEnum ExpectedEvent = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? EVENT_S2MM_DONE : EVENT_MM2S_DONE;

    AL_ASSERT((Handle != AL_NULL), AL_AxiDma_ERR_NULL_PTR);

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

AL_S32 AlAxiDma_Hal_DirectMode_TransferPolling(AlAxiDma_HalStruct *Handle, AL_U8 *Buffer, AL_U32 Length, AL_U32 Direction)
{
    AL_S32 Ret = AL_OK;
    AL_Lock_t Lock = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_Lock : &Handle->Mm2s_Lock;

    AL_ASSERT((Handle != AL_NULL), AL_AxiDma_ERR_NULL_PTR);

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

AL_S32 AlAxiDma_Hal_SgMode_TransferBlock(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AlAxiDma_EventStruct AxiDmaEvent = {0};
    AL_Lock_t Lock = (Msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_Lock : &Handle->Mm2s_Lock;
    AL_MailBox_t EventQueue = (Msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_EventQueue : &Handle->Mm2s_EventQueue;
    AlAxiDma_EventEnum ExpectedEvent = (Msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) ?
        (Handle->Dma.InitConfig.S2mm_HasCyclic ? EVENT_S2MM_CYCLIC : EVENT_S2MM_DONE) :
        (Handle->Dma.InitConfig.Mm2s_HasCyclic ? EVENT_MM2S_CYCLIC : EVENT_MM2S_DONE);

    AL_ASSERT((Handle != AL_NULL), AL_AxiDma_ERR_NULL_PTR);

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

AL_S32 AlAxiDma_Hal_SgMode_TransferPolling(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg)
{
    AL_S32 Ret = AL_OK;
    AL_Lock_t Lock = (Msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Handle->S2mm_Lock : &Handle->Mm2s_Lock;

    AL_ASSERT((Handle != AL_NULL), AL_AxiDma_ERR_NULL_PTR);

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

AL_S32 AlAxiDma_Hal_SetupDescriptors(AlAxiDma_HalStruct *Handle, ALAXIDMA_TransferMsg *Msg)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT(Handle != NULL && Msg != NULL, AL_AxiDma_ERR_NULL_PTR);

    for (int i = 0; i < Msg->NumBuffers; i++) {
        Ret = AlAxiDma_Dev_SetupDescriptors(&Handle->Dma, i, Msg->Buffers[i], Msg->BufferLengths[i], Msg->NumBuffers, Msg->Direction);
        if (Ret != AL_OK) {
            return Ret;
        }
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