#include "al_axi_dma_hal.h"

AlAxiDma_HalStruct AXI_DMA_Handle[AL_AXI_DMA_NUM_INSTANCE];

static AL_VOID AlAxiDma_Hal_DefEventHandler(AlAxiDma_EventStruct *AxiDmaEvent, AL_VOID *CallbackRef)
{
    AlAxiDma_HalStruct *Handle = (AlAxiDma_HalStruct *)CallbackRef;

    switch (AxiDmaEvent->Event) {
        case EVENT_S2MM_DONE:
        case EVENT_S2MM_ERROR:
        AlOsal_Mb_Send(&Handle->S2mm_EventQueue, AxiDmaEvent);
        break;

        case EVENT_MM2S_DONE:
        case EVENT_MM2S_ERROR:
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

AL_U32 AlAxiDma_Hal_Simple_Transfer(AlAxiDma_HalStruct *Handle, AL_U64 BuffAddr, AL_U32 Length, AL_U32 Direction, AL_U32 Timeout)
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

    // Manage cache based on direction
    if (Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        AlCache_InvalidateDcacheRange(BuffAddr, BuffAddr + Length);
    } else {
        AlCache_FlushDcacheRange(BuffAddr, BuffAddr + Length);
    }

    // Perform the DMA transfer
    Ret = AlAxiDma_Dev_Simple_Transfer(&Handle->Dma, BuffAddr, Length, Direction);
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
        return AL_OK;
    }

    // Return appropriate error code
    return (Ret != AL_OK) ? Ret : AL_AxiDma_EVENTS_TO_ERRS(AxiDmaEvent.Event);
}