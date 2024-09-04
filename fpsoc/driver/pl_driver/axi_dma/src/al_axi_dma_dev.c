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

    if(Dma->HwConfig.DmaMode == AL_AXIDMA_SG_MODE) {
        Dma->descriptors = InitConfig->descriptors;
    }

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

    return AL_OK;
}

AL_S32 AlAxiDma_Dev_RegisterEventCallBack(AlAxiDma_DevStruct *Dma, AlAxiDma_EventCallBack CallBack, AL_VOID *CallBackRef)
{
    Dma->EventCallBack = CallBack;
    Dma->EventCallBackRef = CallBackRef;
    return AL_OK;
}

AL_S32 AlAxiDma_Dev_DirectMode_Transfer(AlAxiDma_DevStruct *Dma, AL_U8 *Buffer, AL_U32 Length, AlAxiDma_TransDirEnum Direction, AlAxiDma_TransferMethodEnum Method)
{
    AL_S32 Ret = AL_OK;
    AL_U32 WordBits;
    AL_U32 MaxTransferLen;

    AL_ASSERT(Dma->HwConfig.DmaMode == AL_AXIDMA_DIRECT_MODE, AL_AxiDma_ERR_NOT_SUPPORT);

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

    // Validate the length of the transfer
    MaxTransferLen = (1 << Dma->HwConfig.BufferlenWidth) - 1;
    AL_ASSERT((Length > 0) && (Length <= MaxTransferLen), AL_AxiDma_ERR_ILLEGAL_PARAM);

    // Check for unaligned buffer address
    if ((AL_UINTPTR)Buffer & WordBits) {
        if ((Dma->HwConfig.EnableMicroMode) ||
            (Direction == AL_AXIDMA_DEVICE_TO_DMA && !Dma->HwConfig.HasS2mmDRE) ||
            (Direction == AL_AXIDMA_DMA_TO_DEVICE && !Dma->HwConfig.HasMm2sDRE)) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Unaligned transfer at address 0x%lx", (AL_UINTPTR)Buffer);
            return AL_AxiDma_ERR_ILLEGAL_PARAM;
        }
    }

    // Start transfer
    Ret = AlAxiDma_Dev_DirectMode_Start(Dma, Buffer, Length, Direction, Method);
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

AL_S32 AlAxiDma_Dev_DirectMode_Start(AlAxiDma_DevStruct *Dma, AL_U8 *Buffer, AL_U32 Length, AlAxiDma_TransDirEnum Direction, AlAxiDma_TransferMethodEnum Method)
{
    AL_S32 Ret = AL_OK;
    AL_U64 ChanBase = Dma->RegBase + (ALAXIDMA_S2MM_OFFSET * Direction);

    // Check if the DMA channel is halted or busy
    if (!AlAxiDma_ll_GetSrHalt(ChanBase) && AlAxiDma_Dev_Busy(Dma, Direction)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Engine is busy");
        return AL_AxiDma_ERR_BUSY;
    }

    // Configure interrupts based on transfer method
    AL_U8 interruptState = (Method == AL_AXIDMA_BLOCK) ? AL_AXI_DMA_FUNC_ENABLE : AL_AXI_DMA_FUNC_DISABLE;
    AlAxiDma_ll_SetIocIntr(ChanBase, interruptState);
    AlAxiDma_ll_SetErrIntr(ChanBase, interruptState);

    // Set the DMA transfer address
    if (Dma->HwConfig.AddrWidth > 32) {
        AlAxiDma_ll_SetAddrMsb(ChanBase, (AL_U32)(((AL_UINTPTR)Buffer >> 32) & 0xFFFFFFFF));
    }
    AlAxiDma_ll_SetAddrLsb(ChanBase, (AL_U32)((AL_UINTPTR)Buffer & 0xFFFFFFFF));

    // Start the DMA operation
    AlAxiDma_ll_SetRunStop(ChanBase, AL_AXI_DMA_FUNC_ENABLE);

    // set the transfer length will trigger transfer
    AlAxiDma_ll_SetLength(ChanBase, Length);

    // In POLLING mode, wait for the transfer to complete
    if (Method == AL_AXIDMA_POLLING) {
        while (AlAxiDma_Dev_Busy(Dma, Direction));
        AlAxiDma_Dev_Stop_Transfer(Dma, Direction);
    }

    return Ret; // Return the operation status
}

AL_VOID AlAxiDma_Dev_Stop_Transfer(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction)
{
    AL_U64 ChanBase;
    ChanBase = Dma->RegBase + (ALAXIDMA_S2MM_OFFSET * Direction);

    AlAxiDma_ll_SetIocIntr(ChanBase, AL_AXI_DMA_FUNC_DISABLE);
    AlAxiDma_ll_SetErrIntr(ChanBase, AL_AXI_DMA_FUNC_DISABLE);
    AlAxiDma_ll_SetRunStop(ChanBase, AL_AXI_DMA_FUNC_DISABLE);
}

AL_S32 AlAxiDma_Dev_SetupDescriptors(AlAxiDma_DevStruct *Dma, AL_U32 Index, AL_U8 *Buffer, AL_U32 Length, AL_U32 NumBuffers, AlAxiDma_TransDirEnum Direction)
{
    AL_S32 Ret = AL_OK;
    AL_U32 WordBits;
    AL_U32 ControlFlags = 0;
    AL_U32 MaxTransferLen;
    AlAxiDma_Descriptor *desc = &Dma->descriptors[Index];

    AL_ASSERT(Dma->HwConfig.DmaMode == AL_AXIDMA_SG_MODE, AL_AxiDma_ERR_NOT_SUPPORT);

    // Determine MaxTransferLen based on whether Micro mode is enabled
    MaxTransferLen = (Dma->HwConfig.EnableMicroMode) 
                     ? (Direction == AL_AXIDMA_DEVICE_TO_DMA ? Dma->HwConfig.S2mmMicroMaxTransferLen : Dma->HwConfig.Mm2sMicroMaxTransferLen)
                     : (1 << AL_AXI_DMA0_DESC_BUFFER_LENGTH_WIDTH) - 1;

    // Validate Length
    AL_ASSERT((Length > 0) && (Length <= MaxTransferLen), AL_AxiDma_ERR_ILLEGAL_PARAM);

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

    // Check for unaligned buffer address
    if ((AL_UINTPTR)Buffer & WordBits) {
        if ((Dma->HwConfig.EnableMicroMode) ||
            (Direction == AL_AXIDMA_DEVICE_TO_DMA && !Dma->HwConfig.HasS2mmDRE)||
            (Direction == AL_AXIDMA_DMA_TO_DEVICE && !Dma->HwConfig.HasMm2sDRE)) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Unaligned transfer at address 0x%lx", (AL_UINTPTR)Buffer);
            return AL_AxiDma_ERR_ILLEGAL_PARAM;
        }
    }

    // Mm2s and S2mm in Micro mode need set up SOF and EOF manually
    if (Dma->HwConfig.EnableMicroMode || Direction == AL_AXIDMA_DMA_TO_DEVICE) {
        ControlFlags |= (Index == 0 ? (1 << ALAXIDMA_DESC_CONTROL_SOF_SHIFT) : 0);
        ControlFlags |= (Index == NumBuffers - 1 ? (1 << ALAXIDMA_DESC_CONTROL_EOF_SHIFT) : 0);
    }

    // Setup the current descriptor's contents
    desc->next_desc = (Index < NumBuffers - 1) 
                      ? (AL_U32)((AL_UINTPTR)&Dma->descriptors[Index + 1] & 0xFFFFFFFF)
                      : 0;
    desc->next_desc_msb = (Index < NumBuffers - 1) 
                          ? (AL_U32)(((AL_UINTPTR)&Dma->descriptors[Index + 1] >> 32) & 0xFFFFFFFF)
                          : 0;
    desc->buffer_addr = (AL_U32)((AL_UINTPTR)Buffer & 0xFFFFFFFF);
    desc->buffer_addr_msb = (AL_U32)(((AL_UINTPTR)Buffer >> 32) & 0xFFFFFFFF);
    desc->control = (Length & MaxTransferLen) | (ControlFlags & 0x0C000000);
    desc->status = 0;

    return Ret;
}

AL_S32 AlAxiDma_Dev_SgMode_Transfer(AlAxiDma_DevStruct *Dma, AL_U32 NumBuffers, AlAxiDma_TransDirEnum Direction, AlAxiDma_TransferMethodEnum Method)
{
    AL_S32 Ret = AL_OK;
    AL_U64 ChanBase = Dma->RegBase + (ALAXIDMA_S2MM_OFFSET * Direction);

    // Assert to check if the DMA is in SG mode
    AL_ASSERT(Dma->HwConfig.DmaMode == AL_AXIDMA_SG_MODE, AL_AxiDma_ERR_NOT_SUPPORT);

    // Check if the DMA is idle
    if (!AlAxiDma_ll_GetSrHalt(ChanBase) && AlAxiDma_Dev_Busy(Dma, Direction)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Engine is busy");
        return AL_AxiDma_ERR_BUSY;
    }

    // Configure KeyHole and Cyclic mode
    AL_U32 HasKeyHole = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? Dma->InitConfig.S2mm_HasKeyHole : Dma->InitConfig.Mm2s_HasKeyHole;
    AL_U32 HasCycli = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? Dma->InitConfig.S2mm_HasCyclic : Dma->InitConfig.Mm2s_HasCyclic;
    AL_U32 Threshlod = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? Dma->InitConfig.S2mm_Threshold : Dma->InitConfig.Mm2s_Threshold;
    AL_U32 BurstSize = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? Dma->HwConfig.S2mmBurstSize : Dma->HwConfig.Mm2sBurstSize;
    AL_U32 HasDRE = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? Dma->HwConfig.HasS2mmDRE : Dma->HwConfig.HasMm2sDRE;

    // Check if polling method is used with cyclic mode before starting the transfer
    if (Method == AL_AXIDMA_POLLING && HasCycli) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Polling transfer does not support cyclic mode, use block transfer");
        return AL_AxiDma_ERR_NOT_SUPPORT;
    }

    if (HasCycli) {
        AlAxiDma_ll_Cyclic(ChanBase, AL_AXI_DMA_FUNC_ENABLE);
    }

    if (HasKeyHole) {
        // Assert to ensure DRE is disabled and BurstSize is less than or equal to 16
        AL_ASSERT(!HasDRE && (BurstSize <= 16), AL_AxiDma_ERR_NOT_SUPPORT);
        AlAxiDma_ll_KeyHole(ChanBase, AL_AXI_DMA_FUNC_ENABLE);
    }

    // Configure interrupts based on the transfer method
    AL_U8 interruptState = (Method == AL_AXIDMA_BLOCK) ? AL_AXI_DMA_FUNC_ENABLE : AL_AXI_DMA_FUNC_DISABLE;
    AlAxiDma_ll_SetIocIntr(ChanBase, interruptState);
    AlAxiDma_ll_SetErrIntr(ChanBase, interruptState);

    AlAxiDma_ll_SetThreshold(ChanBase, Threshlod);

    // Set the starting descriptor address
    AL_UINTPTR descAddr = (AL_UINTPTR)&Dma->descriptors[0];
    if (Dma->HwConfig.AddrWidth > 32) {
        AlAxiDma_ll_SetCurDescMsb(ChanBase, (AL_U32)((descAddr >> 32) & 0xFFFFFFFF));
    }
    AlAxiDma_ll_SetCurDescLsb(ChanBase, (AL_U32)(descAddr & 0xFFFFFFFF));

    // Start the DMA transfer
    AlAxiDma_ll_SetRunStop(ChanBase, AL_AXI_DMA_FUNC_ENABLE);

    // Set the tail descriptor
    AL_UINTPTR tailDescAddr = (AL_UINTPTR)&Dma->descriptors[NumBuffers - 1];
    if (HasCycli) {
        AlAxiDma_ll_SetTailDescLsb(ChanBase, 0x80);  // In cyclic mode, set the tail descriptor to a special value
    } else {
        if (Dma->HwConfig.AddrWidth > 32) {
            AlAxiDma_ll_SetTailDescMsb(ChanBase, (AL_U32)((tailDescAddr >> 32) & 0xFFFFFFFF));
        }
        AlAxiDma_ll_SetTailDescLsb(ChanBase, (AL_U32)(tailDescAddr & 0xFFFFFFFF));
    }

    // In polling mode, wait for the transfer to complete
    if (Method == AL_AXIDMA_POLLING) {
        while (AlAxiDma_Dev_Busy(Dma, Direction));
        AlAxiDma_Dev_Stop_Transfer(Dma, Direction);
    }

    return Ret;
}

#define AL_AXIDMA_INTR_IOC(Status)           (Status == AL_AXIDMA_INTR_IOC_DONE)
#define AL_AXIDMA_INTR_DLY(Status)           (Status == AL_AXIDMA_INTR_DLY_DONE)
#define AL_AXIDMA_INTR_ERR(Status)           (Status == AL_AXIDMA_INTR_ERR_OCCUR)

AL_VOID ClearCompleteStatus(AlAxiDma_DevStruct *Dma)
{
    AlAxiDma_Descriptor *curDesc = Dma->descriptors;  // Get the starting address of the descriptor list
    AlAxiDma_Descriptor *startDesc = curDesc;  // Record the starting address of the list to check for loop completion

    do {
        // Clear the complete status bit
        curDesc->status &= ~(1 << 31);

        // Find the next descriptor based on next_desc and next_desc_msb
        AL_UINTPTR nextDescAddr = ((AL_UINTPTR)(curDesc->next_desc_msb) << 32) | curDesc->next_desc;
        curDesc = (AlAxiDma_Descriptor *)nextDescAddr;  // Cast to the descriptor type to get the address of the next descriptor

    } while (curDesc != startDesc);  // Check if we have looped back to the starting descriptor
}

AL_VOID AlAxiDma_Dev_Transfer_ErrorHandler(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction)
{
    AL_U64 ChanBase;
    AL_U32 Status;
    AL_U32 EventType;
    // Define a bitmask that includes all the error bits we care about
    AL_U32 ErrorMask = DMA_INTERNAL_ERROR | DMA_SLAVE_ERROR | DMA_DECODE_ERROR |
                       SG_INTERNAL_ERROR | SG_SLAVE_ERROR | SG_DECODE_ERROR;

    // Set the base address and event type based on the direction of the DMA transfer
    if (Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        ChanBase = Dma->RegBase + ALAXIDMA_S2MM_OFFSET;
        EventType = EVENT_S2MM_ERROR;
    } else {
        ChanBase = Dma->RegBase + ALAXIDMA_MM2S_OFFSET;
        EventType = EVENT_MM2S_ERROR;
    }

    // Read the error status register
    Status = AlAxiDma_ll_GetStatus(ChanBase);

    // Apply the error mask to filter out only the bits we are interested in
    AL_U32 ErrSource = Status & ErrorMask;

    // If a callback function is set, pass the event and filtered error data
    if (Dma->EventCallBack) {
        AlAxiDma_EventStruct AlAxiDmaEvent = {
            .Event     = EventType,
            .EventData = ErrSource, // Set the filtered error data to EventData
        };
        (*Dma->EventCallBack)(&AlAxiDmaEvent, Dma->EventCallBackRef);
    }

    AlAxiDma_Dev_Reset(Dma);
    while (!AlAxiDma_Dev_ResetIsDone(Dma));
}

AL_VOID AlAxiDma_Dev_Transfer_DoneHandler(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction)
{
    AL_U32 EventType;
    AL_U32 IsCyclic = 0;

    if (Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        IsCyclic = Dma->InitConfig.S2mm_HasCyclic;
        EventType = IsCyclic ? EVENT_S2MM_CYCLIC : EVENT_S2MM_DONE;
    } else {
        IsCyclic = Dma->InitConfig.Mm2s_HasCyclic;
        EventType = IsCyclic ? EVENT_MM2S_CYCLIC : EVENT_MM2S_DONE;
    }

    AlAxiDma_EventStruct AlAxiDmaEvent = {
        .Event     = EventType,
        .EventData = AL_NULL,
    };

    if (IsCyclic) {
        AL_U32 *CyclicCompleted = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? &Dma->S2mmCyclicCompleted : &Dma->Mm2sCyclicCompleted;
        AL_U32 CyclicCount = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? Dma->S2mmCyclicCount : Dma->Mm2sCyclicCount;

        (*CyclicCompleted)++;
        if (*CyclicCompleted >= CyclicCount) {
            *CyclicCompleted = 0;
            if (Dma->EventCallBack) {
                (*Dma->EventCallBack)(&AlAxiDmaEvent, Dma->EventCallBackRef);
            }
        } else {
            // Clear status only when cyclic count has not been reached
            ClearCompleteStatus(Dma);
        }
    } else {
        if (Dma->EventCallBack) {
            (*Dma->EventCallBack)(&AlAxiDmaEvent, Dma->EventCallBackRef);
        }
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
        AlAxiDma_ll_ClrErr(Dma->RegBase + ALAXIDMA_MM2S_OFFSET);
        AlAxiDma_Dev_Transfer_ErrorHandler(Dma, AL_AXIDMA_DMA_TO_DEVICE);
    } else if (AL_AXIDMA_INTR_IOC(IntrStatus)) {
        AlAxiDma_ll_ClrIoc(Dma->RegBase + ALAXIDMA_MM2S_OFFSET);
        AlAxiDma_Dev_Transfer_DoneHandler(Dma, AL_AXIDMA_DMA_TO_DEVICE);
    }
}

AL_VOID AlAxiDma_Dev_S2mm_IntrHandler(AL_VOID *Instance)
{
    AlAxiDma_DevStruct *Dma = (AlAxiDma_DevStruct *)Instance;
    AlAxiDma_IntrEnum IntrStatus = AlAxiDma_ll_GetIntrStatus(Dma->RegBase + ALAXIDMA_S2MM_OFFSET);

    if (AL_AXIDMA_INTR_ERR(IntrStatus)) {
        AlAxiDma_ll_ClrErr(Dma->RegBase + ALAXIDMA_S2MM_OFFSET);
        AlAxiDma_Dev_Transfer_ErrorHandler(Dma, AL_AXIDMA_DEVICE_TO_DMA);
    } else if (AL_AXIDMA_INTR_IOC(IntrStatus)) {
        AlAxiDma_ll_ClrIoc(Dma->RegBase + ALAXIDMA_S2MM_OFFSET);
        AlAxiDma_Dev_Transfer_DoneHandler(Dma, AL_AXIDMA_DEVICE_TO_DMA);
    }
}
