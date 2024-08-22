#include "al_axi_dma_dev.h"

extern AlAxiDma_HwConfigStruct AlAxiDma_HwConfig[AL_AXI_DMA_NUM_INSTANCE];

AlAxiDma_HwConfigStruct *AlAxiDma_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AlAxiDma_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_AXI_DMA_NUM_INSTANCE; Index++) {
        if (AlAxiDma_HwConfig[Index].DeviceId == DevId) {
            ConfigPtr = &AlAxiDma_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

AL_S32 AlAxiDma_Dev_Init(AlAxiDma_DevStruct *Dma, AlAxiDma_HwConfigStruct *HwConfig, AlAxiDma_InitStruct *InitConfig)
{
    /* Setup the instance */
    memset(Dma, 0, sizeof(AlAxiDma_DevStruct));

    Dma->HwConfig = *HwConfig;
    Dma->InitConfig = *InitConfig;

    /* Get hardware setting information from the configuration structure */
    Dma->RegBase = HwConfig->BaseAddress;
    Dma->HasKeyHole = InitConfig->HasKeyHole;

    /* In MicroDMA mode, Maximum length that can be transferred
     * is '(Memory map Data Width / 8) * Burst Size
     */
    if (Dma->HwConfig.EnableMicroMode) {
        Dma->HwConfig.Mm2sMicroMaxTransferLen = (Dma->HwConfig.Mm2sMemoryMapDataWidth / 8) * Dma->HwConfig.Mm2sBurstSize;
        Dma->HwConfig.S2mmMicroMaxTransferLen = (Dma->HwConfig.S2mmMemoryMapDataWidth / 8) * Dma->HwConfig.S2mmBurstSize;
    }

    /* Reset the engine so the hardware starts from a known state */
    AlAxiDma_Dev_Reset(Dma);
    while (!AlAxiDma_Dev_ResetIsDone(Dma));

    /* Initialization is successful */
    Dma->Initialized = 1;

    return AL_OK;
}

AL_S32 AlAxiDma_Dev_RegisterEventCallBack(AlAxiDma_DevStruct *Dma, AlAxiDma_EventCallBack CallBack, AL_VOID *CallBackRef)
{
    Dma->EventCallBack = CallBack;
    Dma->EventCallBackRef = CallBackRef;
    return AL_OK;
}

AL_S32 AlAxiDma_Dev_Simple_Transfer(AlAxiDma_DevStruct *Dma, AL_U64 BuffAddr, AL_U32 Length, AlAxiDma_TransDirEnum Direction)
{
    AL_S32 Ret = AL_OK;
    AL_U32 WordBits;
    AL_U32 MaxTransferLen;

    // Check if the direction is supported and set WordBits accordingly
    switch (Direction) {
        case AL_AXIDMA_DEVICE_TO_DMA:
            AL_ASSERT(Dma->HwConfig.HasS2mm, AL_AxiDma_ERR_NOT_SUPPORT);
            WordBits = (Dma->HwConfig.S2mmMemoryMapDataWidth / 8) - 1;
            break;

        case AL_AXIDMA_DMA_TO_DEVICE:
            AL_ASSERT(Dma->HwConfig.HasMm2s, AL_AxiDma_ERR_NOT_SUPPORT);
            WordBits = (Dma->HwConfig.Mm2sMemoryMapDataWidth / 8) - 1;
            break;

        default:
            AL_LOG(AL_LOG_LEVEL_ERROR, "Transmission direction is not supported");
            return AL_AxiDma_ERR_NOT_SUPPORT;
    }

    // Determine MaxTransferLen based on whether Micro mode is enabled
    if (Dma->HwConfig.EnableMicroMode) {
        MaxTransferLen = (Direction == AL_AXIDMA_DEVICE_TO_DMA) 
                         ? Dma->HwConfig.S2mmMicroMaxTransferLen 
                         : Dma->HwConfig.Mm2sMicroMaxTransferLen;
    } else {
        MaxTransferLen = Dma->HwConfig.NormalMaxTransferLen;
    }
    // Validate the length of the transfer
    AL_ASSERT((Length > 0) && (Length <= MaxTransferLen), AL_AxiDma_ERR_ILLEGAL_PARAM);

    // Check for unaligned buffer address
    if (BuffAddr & WordBits) {
        if ((Direction == AL_AXIDMA_DEVICE_TO_DMA && !Dma->HwConfig.HasS2mmDRE) ||
            (Direction == AL_AXIDMA_DMA_TO_DEVICE && !Dma->HwConfig.HasMm2sDRE)) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Unaligned transfer at address 0x%lx", BuffAddr);
            return AL_AxiDma_ERR_ILLEGAL_PARAM;
        }
    }

    // Start transfer
    Ret = AlAxiDma_Dev_Start_Transfer(Dma, BuffAddr, Length, Direction);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Start transfer failed");
        return Ret;
    }

    return Ret;
}

AL_VOID AlAxiDma_Dev_Reset(AlAxiDma_DevStruct *Dma)
{
    AL_U64 ChanBase;

    // Reset MM2S if available
    if (Dma->HwConfig.HasMm2s) {
        ChanBase = Dma->RegBase + ALAXIDMA_MM2S_OFFSET;
        AlAxiDma_ll_Reset(ChanBase, AL_FUNC_ENABLE);
    }

    // Reset S2MM if available
    if (Dma->HwConfig.HasS2mm) {
        ChanBase = Dma->RegBase + ALAXIDMA_S2MM_OFFSET;
        AlAxiDma_ll_Reset(ChanBase, AL_FUNC_ENABLE);
    }
}

AL_S32 AlAxiDma_Dev_ResetIsDone(AlAxiDma_DevStruct *Dma)
{
    AL_S32 Mm2sResetDone = AL_TRUE;
    AL_S32 S2mmResetDone = AL_TRUE;

    /* Check transmit channel */
    if (Dma->HwConfig.HasMm2s) {
        /* Reset is done when the reset bit is low */
        Mm2sResetDone = AlAxiDma_ll_GetCrReset(Dma->RegBase) ? AL_FALSE : AL_TRUE;
    }

    /* Check receive channel */
    if (Dma->HwConfig.HasS2mm) {
        /* Reset is done when the reset bit is low */
        S2mmResetDone = AlAxiDma_ll_GetCrReset(Dma->RegBase + ALAXIDMA_S2MM_OFFSET) ? AL_FALSE : AL_TRUE;
    }

    /* Return true only if both channels have completed reset, 
       but only check the relevant channel */
    if (Dma->HwConfig.HasMm2s && Dma->HwConfig.HasS2mm) {
        return (Mm2sResetDone && S2mmResetDone);
    } else if (Dma->HwConfig.HasMm2s) {
        return Mm2sResetDone;
    } else if (Dma->HwConfig.HasS2mm) {
        return S2mmResetDone;
    } else {
        return AL_TRUE;
    }
}

AL_BOOL AlAxiDma_Dev_Busy(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction)
{
    AL_U64 ChanBase;
    ChanBase = Dma->RegBase + (ALAXIDMA_S2MM_OFFSET * Direction);

    return (AlAxiDma_ll_GetSrIdle(ChanBase) ? AL_FALSE : AL_TRUE);
}

AL_S32 AlAxiDma_Dev_Start_Transfer(AlAxiDma_DevStruct *Dma, AL_U64 BuffAddr, AL_U32 Length, AlAxiDma_TransDirEnum Direction)
{
    AL_S32 Ret = AL_OK;
    AL_U64 ChanBase;
    ChanBase = Dma->RegBase + (ALAXIDMA_S2MM_OFFSET * Direction);

    // Check if the DMA channel is halted, and if it's busy
    if (!AlAxiDma_ll_GetSrHalt(ChanBase) && AlAxiDma_Dev_Busy(Dma, Direction)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Engine is busy");
        return AL_AxiDma_ERR_BUSY;
    }

    // Set keyhole if applicable and perform assert checks
    if (Dma->HasKeyHole) {
        if (Direction == AL_AXIDMA_DEVICE_TO_DMA) {
            // For S2MM direction
            AL_ASSERT(!(Dma->HwConfig.HasS2mmDRE) && (Dma->HwConfig.S2mmBurstSize <= 16), AL_AxiDma_ERR_NOT_SUPPORT);
        } else if (Direction == AL_AXIDMA_DMA_TO_DEVICE) {
            // For MM2S direction
            AL_ASSERT(!(Dma->HwConfig.HasMm2sDRE) && (Dma->HwConfig.Mm2sBurstSize <= 16), AL_AxiDma_ERR_NOT_SUPPORT);
        }
        AlAxiDma_ll_KeyHole(ChanBase, AL_AXI_DMA_FUNC_ENABLE);
    }

    // Set intr
    AlAxiDma_ll_SetIocIntr(ChanBase, AL_AXI_DMA_FUNC_ENABLE);
    AlAxiDma_ll_SetErrIntr(ChanBase, AL_AXI_DMA_FUNC_ENABLE);

    // Set address
    AlAxiDma_ll_SetLSBAddr(ChanBase, BuffAddr);
    if (Dma->HwConfig.AddrWidth > 32) {
        AlAxiDma_ll_SetMSBAddr(ChanBase, BuffAddr >> 32);
    }

    // Start transfer, and set the length
    AlAxiDma_ll_SetRunStop(ChanBase, AL_AXI_DMA_FUNC_ENABLE);
    AlAxiDma_ll_SetLength(ChanBase, Length);

    return Ret;
}

AL_VOID AlAxiDma_Dev_Stop_Transfer(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction)
{
    AL_U64 ChanBase;
    ChanBase = Dma->RegBase + (ALAXIDMA_S2MM_OFFSET * Direction);

    AlAxiDma_ll_SetIocIntr(ChanBase, AL_AXI_DMA_FUNC_DISABLE);
    AlAxiDma_ll_SetErrIntr(ChanBase, AL_AXI_DMA_FUNC_DISABLE);
    AlAxiDma_ll_SetRunStop(ChanBase, AL_AXI_DMA_FUNC_DISABLE);
}

#define AL_AXIDMA_INTR_IOC(Status)           (Status == AL_AXIDMA_INTR_IOC_DONE)
#define AL_AXIDMA_INTR_ERR(Status)           (Status == AL_AXIDMA_INTR_ERROR)

AL_VOID AlAxiDma_Dev_Transfer_ErrorHandler(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction)
{
    AL_U64 ChanBase;
    AL_U32 EventType;

    if (Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        ChanBase = Dma->RegBase + ALAXIDMA_S2MM_OFFSET;
        EventType = EVENT_S2MM_ERROR;
    } else {
        ChanBase = Dma->RegBase + ALAXIDMA_MM2S_OFFSET;
        EventType = EVENT_MM2S_ERROR;
    }

    if (Dma->EventCallBack) {
        AlAxiDma_EventStruct AlAxiDmaEvent = {
            .Event         = EventType,
            .EventData     = AL_NULL,
        };
        (*Dma->EventCallBack)(&AlAxiDmaEvent, Dma->EventCallBackRef);
    }

    AlAxiDma_Dev_Stop_Transfer(Dma, Direction);
}

AL_VOID AlAxiDma_Dev_Transfer_DoneHandler(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction)
{
    AL_U64 ChanBase;
    AL_U32 EventType;

    if (Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        ChanBase = Dma->RegBase + ALAXIDMA_S2MM_OFFSET;
        EventType = EVENT_S2MM_DONE;
    } else {
        ChanBase = Dma->RegBase + ALAXIDMA_MM2S_OFFSET;
        EventType = EVENT_MM2S_DONE;
    }

    if (Dma->EventCallBack) {
        AlAxiDma_EventStruct AlAxiDmaEvent = {
            .Event         = EventType,
            .EventData     = AL_NULL,
        };
        (*Dma->EventCallBack)(&AlAxiDmaEvent, Dma->EventCallBackRef);
    }

    /*
     * When CR.RS = 1, SR.idle indicates whether the transfer is complete; 
     * when complete, SR.idle = 1, and when not complete, SR.idle = 0. 
     * When CR.RS = 0, SR.idle = 0.
     * 
     * When CR.RS = 1, SR.halt = 0; 
     * when CR.RS = 0, SR.halt = 1.
     */
    if (!AlAxiDma_Dev_Busy(Dma, Direction)) {
        AlAxiDma_Dev_Stop_Transfer(Dma, Direction);
    }
}

AL_VOID AlAxiDma_Dev_Mm2s_IntrHandler(AL_VOID *Instance)
{
    AlAxiDma_DevStruct *Dma = (AlAxiDma_DevStruct *)Instance;
    AlAxiDma_IntrEnum IntrStatus = AlAxiDma_ll_GetIntrStatus(Dma->RegBase + ALAXIDMA_MM2S_OFFSET);

    if (AL_AXIDMA_INTR_ERR(IntrStatus)) {
        AlAxiDma_Dev_Transfer_ErrorHandler(Dma, AL_AXIDMA_DMA_TO_DEVICE);
        AlAxiDma_ll_ClrErr(Dma->RegBase + ALAXIDMA_MM2S_OFFSET);
    } else if (AL_AXIDMA_INTR_IOC(IntrStatus)) {
        AlAxiDma_Dev_Transfer_DoneHandler(Dma, AL_AXIDMA_DMA_TO_DEVICE);
        AlAxiDma_ll_ClrIoc(Dma->RegBase + ALAXIDMA_MM2S_OFFSET);
    }
}

AL_VOID AlAxiDma_Dev_S2mm_IntrHandler(AL_VOID *Instance)
{
    AlAxiDma_DevStruct *Dma = (AlAxiDma_DevStruct *)Instance;
    AlAxiDma_IntrEnum IntrStatus = AlAxiDma_ll_GetIntrStatus(Dma->RegBase + ALAXIDMA_S2MM_OFFSET);

    if (AL_AXIDMA_INTR_ERR(IntrStatus)) {
        AlAxiDma_Dev_Transfer_ErrorHandler(Dma, AL_AXIDMA_DEVICE_TO_DMA);
        AlAxiDma_ll_ClrErr(Dma->RegBase + ALAXIDMA_S2MM_OFFSET);
    } else if (AL_AXIDMA_INTR_IOC(IntrStatus)) {
        AlAxiDma_Dev_Transfer_DoneHandler(Dma, AL_AXIDMA_DEVICE_TO_DMA);
        AlAxiDma_ll_ClrIoc(Dma->RegBase + ALAXIDMA_S2MM_OFFSET);
    }
}
