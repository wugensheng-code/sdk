/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_axi_dma_dev.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

extern AlAxiDma_HwConfigStruct AlAxiDma_HwConfig[AL_AXI_DMA_NUM_INSTANCE];

/**
 * This function looks up the hardware configuration for the AXI DMA device
 * based on the specified device ID.
 *
 * @param DevId The device ID of the AXI DMA to look up.
 * @return A pointer to the hardware configuration structure for the specified device,
 *         or AL_NULL if the device ID is not found.
 */
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

/**
 * This function initializes the AXI DMA device with specified configurations.
 *
 * @param Dma Pointer to the AXI DMA device structure to initialize.
 * @param HwConfig Pointer to the hardware configuration structure.
 * @param InitConfig Pointer to the initialization configuration structure.
 * @return AL_OK if successful, error code otherwise.
 */
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

    /* In MicroDMA mode, the maximum amount of data that can be transmitted
     * is '(Memory map Data Width / 8) * Burst Size / (Memory map Data Width / Stream data width)
     */
    if (Dma->HwConfig.EnableMicroMode) {
        Dma->HwConfig.Mm2sMicroMaxTransferLen =
            (Dma->HwConfig.Mm2sMemoryMapDataWidth / 8) * 
            Dma->HwConfig.Mm2sBurstSize / 
            (Dma->HwConfig.Mm2sMemoryMapDataWidth / Dma->HwConfig.Mm2sStreamDataWidth);

        Dma->HwConfig.S2mmMicroMaxTransferLen = 
            (Dma->HwConfig.S2mmMemoryMapDataWidth / 8) * 
            Dma->HwConfig.S2mmBurstSize / 
            (Dma->HwConfig.S2mmMemoryMapDataWidth / Dma->HwConfig.S2mmStreamDataWidth);

        if ((Dma->HwConfig.Mm2sMicroMaxTransferLen > ((1UL << Dma->HwConfig.BufferlenWidth) - 1)) ||
            (Dma->HwConfig.S2mmMicroMaxTransferLen > ((1UL << Dma->HwConfig.BufferlenWidth) - 1))) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "buffer length width is not enough to store transfer data length");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Mm2sMicroMaxTransferLen: %lu, S2mmMicroMaxTransferLen: %lu, BufferlenWidth: %lu",
                                        Dma->HwConfig.Mm2sMicroMaxTransferLen,
                                        Dma->HwConfig.S2mmMicroMaxTransferLen,
                                        Dma->HwConfig.BufferlenWidth);
            return AL_AxiDma_ERR_NOT_SUPPORT;
        }
    }

    /* Reset the engine */
    AlAxiDma_Dev_Reset(Dma);
    while (!AlAxiDma_Dev_ResetIsDone(Dma));

    return AL_OK;
}

/**
 * This function registers an event callback function for the AXI DMA device.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @param CallBack The event callback function to register.
 * @param CallBackRef A reference pointer to be passed to the callback function.
 * @return AL_OK if successful, error code otherwise.
 */
AL_S32 AlAxiDma_Dev_RegisterEventCallBack(AlAxiDma_DevStruct *Dma, AlAxiDma_EventCallBack CallBack, AL_VOID *CallBackRef)
{
    Dma->EventCallBack = CallBack;
    Dma->EventCallBackRef = CallBackRef;
    return AL_OK;
}

/**
 * This function initiates a transfer in Direct Mode for the AXI DMA device.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @param Buffer Pointer to the data buffer.
 * @param Length The length of the data to be transferred.
 * @param Direction The direction of the transfer (DMA to device or device to DMA).
 * @param Method The transfer method (interrupt or polling).
 * @return AL_OK if successful, error code otherwise.
 */
AL_S32 AlAxiDma_Dev_DirectMode_Transfer(AlAxiDma_DevStruct *Dma, AL_U8 *Buffer, AL_U32 Length, AlAxiDma_TransDirEnum Direction, AlAxiDma_TransferMethodEnum Method)
{
    AL_S32 Ret = AL_OK;
    AL_U32 WordBits;
    AL_U32 MaxTransferLen;

    AL_ASSERT(Dma->HwConfig.DmaMode == AL_AXIDMA_DIRECT_MODE, AL_AxiDma_ERR_NOT_SUPPORT);

    // Check if the direction is supported and set WordBits accordingly
    if (Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        if (!Dma->HwConfig.HasS2mm) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "S2MM is not enabled");
            return AL_AxiDma_ERR_NOT_SUPPORT;
        }
        WordBits = Dma->HwConfig.EnableMicroMode ? 0xFFF : (Dma->HwConfig.S2mmMemoryMapDataWidth / 8) - 1;
    } else if (Direction == AL_AXIDMA_DMA_TO_DEVICE) {
        if (!Dma->HwConfig.HasMm2s) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "MM2S is not enabled");
            return AL_AxiDma_ERR_NOT_SUPPORT;
        }
        WordBits = Dma->HwConfig.EnableMicroMode ? 0xFFF : (Dma->HwConfig.Mm2sMemoryMapDataWidth / 8) - 1;
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Transmission direction is not supported");
        return AL_AxiDma_ERR_ILLEGAL_PARAM;
    }

    // Determine MaxTransferLen based on whether Micro mode is enabled
    if (Dma->HwConfig.EnableMicroMode) {
        MaxTransferLen = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? Dma->HwConfig.S2mmMicroMaxTransferLen : Dma->HwConfig.Mm2sMicroMaxTransferLen;
    } else {
        MaxTransferLen = (1UL << Dma->HwConfig.BufferlenWidth) - 1;
    }

    // Validate transfer length
    if (Length == 0 || Length > MaxTransferLen) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Invalid transfer length: %lu. Maximum allowed length: %lu", Length, MaxTransferLen);
        return AL_AxiDma_ERR_ILLEGAL_PARAM;
    }

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

/**
 * This function resets the AXI DMA device.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @return void
 */
AL_VOID AlAxiDma_Dev_Reset(AlAxiDma_DevStruct *Dma)
{
    AL_U64 ChanBase;

    // Reset Mm2s channel if available
    if (Dma->HwConfig.HasMm2s) {
        ChanBase = Dma->RegBase + AL_AXI_DMA_MM2S_OFFSET;
        AlAxiDma_ll_Reset(ChanBase, AL_FUNC_ENABLE);
    }

    // Reset S2mm channel if available
    if (Dma->HwConfig.HasS2mm) {
        ChanBase = Dma->RegBase + AL_AXI_DMA_S2MM_OFFSET;
        AlAxiDma_ll_Reset(ChanBase, AL_FUNC_ENABLE);
    }
}

/**
 * This function checks if the reset operation on the AXI DMA device is complete.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @return AL_TRUE if reset is complete for all enabled channels, AL_FALSE otherwise.
 */
AL_S32 AlAxiDma_Dev_ResetIsDone(AlAxiDma_DevStruct *Dma)
{
    AL_S32 Mm2sResetDone = AL_TRUE;
    AL_S32 S2mmResetDone = AL_TRUE;

    /* Check transmit channel
     * Reset is done when the reset bit is low
     */
    if (Dma->HwConfig.HasMm2s) {
        Mm2sResetDone = AlAxiDma_ll_GetCrReset(Dma->RegBase) ? AL_FALSE : AL_TRUE;
    }

    if (Dma->HwConfig.HasS2mm) {
        S2mmResetDone = AlAxiDma_ll_GetCrReset(Dma->RegBase + AL_AXI_DMA_S2MM_OFFSET) ? AL_FALSE : AL_TRUE;
    }

    // Return true only if both enabled channels have completed reset
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

/**
 * This function checks if the AXI DMA device is busy.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @param Direction The direction of the transfer (DMA to device or device to DMA).
 * @return AL_TRUE if the DMA is busy, AL_FALSE otherwise.
 */
AL_BOOL AlAxiDma_Dev_Busy(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction)
{
    AL_U64 ChanBase;
    ChanBase = Dma->RegBase + (AL_AXI_DMA_S2MM_OFFSET * Direction);

    return (AlAxiDma_ll_GetSrIdle(ChanBase) ? AL_FALSE : AL_TRUE);
}

/**
 * This function starts a direct mode transfer for the AXI DMA device.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @param Buffer Pointer to the data buffer.
 * @param Length The length of the data to be transferred.
 * @param Direction The direction of the transfer (DMA to device or device to DMA).
 * @param Method The transfer method (block-based or polling).
 * @return AL_OK if successful, error code otherwise.
 */
AL_S32 AlAxiDma_Dev_DirectMode_Start(AlAxiDma_DevStruct *Dma, AL_U8 *Buffer, AL_U32 Length, AlAxiDma_TransDirEnum Direction, AlAxiDma_TransferMethodEnum Method)
{
    AL_S32 Ret = AL_OK;
    AL_U64 ChanBase = Dma->RegBase + (AL_AXI_DMA_S2MM_OFFSET * Direction);

    // Check if Method is valid
    AL_ASSERT((Method == AL_AXIDMA_BLOCK) || (Method == AL_AXIDMA_POLLING), AL_AxiDma_ERR_ILLEGAL_PARAM);

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

    // Set the transfer length will trigger transfer
    AlAxiDma_ll_SetLength(ChanBase, Length);

    // In polling mode, wait for the transfer to complete
    if (Method == AL_AXIDMA_POLLING) {
        while (AlAxiDma_Dev_Busy(Dma, Direction));
        AlAxiDma_Dev_Stop_Transfer(Dma, Direction);
    }

    return Ret;
}

/**
 * This function stops transfer for the AXI DMA device.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @param Direction The direction of the transfer (DMA to device or device to DMA).
 * @return void
 */
AL_VOID AlAxiDma_Dev_Stop_Transfer(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction)
{
    AL_U64 ChanBase;
    ChanBase = Dma->RegBase + (AL_AXI_DMA_S2MM_OFFSET * Direction);

    AlAxiDma_ll_SetIocIntr(ChanBase, AL_AXI_DMA_FUNC_DISABLE);
    AlAxiDma_ll_SetErrIntr(ChanBase, AL_AXI_DMA_FUNC_DISABLE);
    AlAxiDma_ll_SetRunStop(ChanBase, AL_AXI_DMA_FUNC_DISABLE);
}

/**
 * This function sets up the descriptors for AXI DMA transfers.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @param Index Index of the descriptor to be set up.
 * @param Buffer Pointer to the data buffer.
 * @param Length The length of the data to be transferred.
 * @param NumBuffers The number of buffers to be transferred.
 * @param Direction The direction of the transfer (DMA to device or device to DMA).
 * @return AL_OK if successful, error code otherwise.
 */
AL_S32 AlAxiDma_Dev_SetupDescriptors(AlAxiDma_DevStruct *Dma, AL_U32 Index, AL_U8 *Buffer, AL_U32 Length, AL_U32 NumBuffers, AlAxiDma_TransDirEnum Direction)
{
    AL_S32 Ret = AL_OK;
    AL_U32 WordBits = 0;
    AL_U32 ControlFlags = 0;
    AL_U32 SingleDescMaxTransferLen;
    AlAxiDma_Descriptor *desc = &Dma->descriptors[Index];

    AL_ASSERT(Dma->HwConfig.DmaMode == AL_AXIDMA_SG_MODE, AL_AxiDma_ERR_NOT_SUPPORT);

    // Determine SingleDescMaxTransferLen based on whether Micro mode is enabled
    if (Dma->HwConfig.EnableMicroMode) {
        SingleDescMaxTransferLen = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? Dma->HwConfig.S2mmMicroMaxTransferLen : Dma->HwConfig.Mm2sMicroMaxTransferLen;
    } else {
        SingleDescMaxTransferLen = (1UL << Dma->HwConfig.BufferlenWidth) - 1;
    }

    /* In SG Micro mode, the maximum amount of data that can be transmitted per descriptor is:
     * (Memory map Data Width / 8) * Burst Size / (Memory map Data Width / Stream data width).
     */
    if (Length == 0 || Length > SingleDescMaxTransferLen) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Invalid transfer length: %lu. single descriptor Maximum allowed length: %lu", Length, SingleDescMaxTransferLen);
        return AL_AxiDma_ERR_ILLEGAL_PARAM;
    }

    // Check if the direction is enabled and set WordBits accordingly
    if (Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        if (!Dma->HwConfig.HasS2mm) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "S2MM is not enabled");
            return AL_AxiDma_ERR_NOT_SUPPORT;
        }
        WordBits = Dma->HwConfig.EnableMicroMode ? 0xFFF : (Dma->HwConfig.S2mmMemoryMapDataWidth / 8) - 1;
    } else if (Direction == AL_AXIDMA_DMA_TO_DEVICE) {
        if (!Dma->HwConfig.HasMm2s) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "MM2S is not enabled");
            return AL_AxiDma_ERR_NOT_SUPPORT;
        }
        WordBits = Dma->HwConfig.EnableMicroMode ? 0xFFF : (Dma->HwConfig.Mm2sMemoryMapDataWidth / 8) - 1;
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Transmission direction is not supported");
        return AL_AxiDma_ERR_ILLEGAL_PARAM;
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

    // In Micro mode need set up s2mm descriptor's sof and eof manually
    if (Dma->HwConfig.EnableMicroMode || Direction == AL_AXIDMA_DMA_TO_DEVICE) {
        ControlFlags |= (Index == 0 ? (1UL << AL_AXI_DMA_DESC_CONTROL_SOF_SHIFT) : 0);
        ControlFlags |= (Index == NumBuffers - 1 ? (1UL << AL_AXI_DMA_DESC_CONTROL_EOF_SHIFT) : 0);
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
    desc->control = Length | ControlFlags;
    desc->status = 0;

    return Ret;
}

/**
 * This function initiates a Scatter-Gather (SG) mode transfer for the AXI DMA device.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @param NumBuffers Number of buffers to be transferred.
 * @param Direction The direction of the transfer (DMA to device or device to DMA).
 * @param Method The transfer method (block-based or polling).
 * @return AL_OK if successful, error code otherwise.
 */
AL_S32 AlAxiDma_Dev_SgMode_Transfer(AlAxiDma_DevStruct *Dma, AL_U32 NumBuffers, AlAxiDma_TransDirEnum Direction, AlAxiDma_TransferMethodEnum Method)
{
    AL_S32 Ret = AL_OK;
    AL_U64 ChanBase = Dma->RegBase + (AL_AXI_DMA_S2MM_OFFSET * Direction);

    // Assert to check if direction is valid
    AL_ASSERT(Direction == AL_AXIDMA_DMA_TO_DEVICE || Direction == AL_AXIDMA_DEVICE_TO_DMA, AL_AxiDma_ERR_ILLEGAL_PARAM);

    // Check if Method is valid
    AL_ASSERT((Method == AL_AXIDMA_BLOCK) || (Method == AL_AXIDMA_POLLING), AL_AxiDma_ERR_ILLEGAL_PARAM);

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
        // Ensure DRE is disabled and burstSize is less than or equal to 16
        if (HasDRE || (BurstSize > 16)) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "KeyHole is not supported when DRE is enabled or BurstSize exceeds 16");
            return AL_AxiDma_ERR_NOT_SUPPORT;
        }
        AlAxiDma_ll_KeyHole(ChanBase, AL_AXI_DMA_FUNC_ENABLE);
    }

    // Configure interrupts based on the transfer method
    AL_U8 interruptState = (Method == AL_AXIDMA_BLOCK) ? AL_AXI_DMA_FUNC_ENABLE : AL_AXI_DMA_FUNC_DISABLE;
    AlAxiDma_ll_SetIocIntr(ChanBase, interruptState);
    AlAxiDma_ll_SetErrIntr(ChanBase, interruptState);

    AlAxiDma_ll_SetThreshold(ChanBase, Threshlod);

    // Set the current descriptor register
    AL_UINTPTR HeadDescAddr = (AL_UINTPTR)&Dma->descriptors[0];
    if (Dma->HwConfig.AddrWidth > 32) {
        AlAxiDma_ll_SetCurDescMsb(ChanBase, (AL_U32)((HeadDescAddr >> 32) & 0xFFFFFFFF));
    }
    AlAxiDma_ll_SetCurDescLsb(ChanBase, (AL_U32)(HeadDescAddr & 0xFFFFFFFF));

    // Start the DMA transfer
    AlAxiDma_ll_SetRunStop(ChanBase, AL_AXI_DMA_FUNC_ENABLE);

    // Set the tail descriptor
    AL_UINTPTR TailDescAddr = (AL_UINTPTR)&Dma->descriptors[NumBuffers - 1];
    if (HasCycli) {
          // In cyclic mode, set tail descriptor register with some value which is not a part of the BD chain
        AlAxiDma_ll_SetTailDescLsb(ChanBase, 0x50);
    } else {
        if (Dma->HwConfig.AddrWidth > 32) {
            AlAxiDma_ll_SetTailDescMsb(ChanBase, (AL_U32)((TailDescAddr >> 32) & 0xFFFFFFFF));
        }
        AlAxiDma_ll_SetTailDescLsb(ChanBase, (AL_U32)(TailDescAddr & 0xFFFFFFFF));
    }

    // In polling mode, wait for the transfer to complete
    if (Method == AL_AXIDMA_POLLING) {
        while (AlAxiDma_Dev_Busy(Dma, Direction));
        AlAxiDma_Dev_Stop_Transfer(Dma, Direction);
    }

    return Ret;
}

#define AL_AXIDMA_INTR_IOC(Status)           (Status & AL_AXIDMA_INTR_IOC_DONE)
#define AL_AXIDMA_INTR_DLY(Status)           (Status & AL_AXIDMA_INTR_DLY_DONE)
#define AL_AXIDMA_INTR_ERR(Status)           (Status & AL_AXIDMA_INTR_ERR_OCCUR)

/**
 * This function clears the complete status bit for all descriptors in the chain.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @return void
 */
AL_VOID ClearCompleteStatus(AlAxiDma_DevStruct *Dma)
{
    AlAxiDma_Descriptor *CurDesc = Dma->descriptors;
    AlAxiDma_Descriptor *StartDesc = CurDesc;

    do {
        // Clear the complete status bit
        CurDesc->status &= ~(1UL << 31);

        // Find the next descriptor
        AL_UINTPTR NextDescAddr = ((AL_UINTPTR)(CurDesc->next_desc_msb) << 32) | CurDesc->next_desc;
        CurDesc = (AlAxiDma_Descriptor *)NextDescAddr;

    } while (CurDesc != StartDesc);
}

/**
 * This function handles transfer errors for the AXI DMA device.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @param Direction The direction of the transfer (DMA to device or device to DMA).
 * @return void
 */
AL_VOID AlAxiDma_Dev_Transfer_ErrorHandler(AlAxiDma_DevStruct *Dma, AlAxiDma_TransDirEnum Direction)
{
    AL_U64 ChanBase;
    AL_U32 Status;
    AL_U32 EventType;
    // Define a bitmask that includes all the error bits
    AL_U32 ErrorMask = DMA_INTERNAL_ERROR | DMA_SLAVE_ERROR | DMA_DECODE_ERROR |
                       SG_INTERNAL_ERROR | SG_SLAVE_ERROR | SG_DECODE_ERROR;

    // Set the base address and event type based on the direction
    if (Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        ChanBase = Dma->RegBase + AL_AXI_DMA_S2MM_OFFSET;
        EventType = EVENT_S2MM_ERROR;
    } else {
        ChanBase = Dma->RegBase + AL_AXI_DMA_MM2S_OFFSET;
        EventType = EVENT_MM2S_ERROR;
    }

    // Read the error status register
    Status = AlAxiDma_ll_GetStatus(ChanBase);

    // Apply the error mask to filter out specific error
    AL_U32 ErrSource = Status & ErrorMask;

    if (Dma->EventCallBack) {
        AlAxiDma_EventStruct AlAxiDmaEvent = {
            .Event     = EventType,
            .EventData = ErrSource,
        };
        (*Dma->EventCallBack)(&AlAxiDmaEvent, Dma->EventCallBackRef);
    }

    AlAxiDma_Dev_Reset(Dma);
    while (!AlAxiDma_Dev_ResetIsDone(Dma));
}

/**
 * This function handles the completion of a transfer for the AXI DMA device.
 *
 * @param Dma Pointer to the AXI DMA device structure.
 * @param Direction The direction of the transfer (DMA to device or device to DMA).
 * @return void
 */
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
            // Clear status when cyclic count has not been reached
            ClearCompleteStatus(Dma);
        }
    } else {
        if (Dma->EventCallBack) {
            (*Dma->EventCallBack)(&AlAxiDmaEvent, Dma->EventCallBackRef);
        }
    }

    /* When CR.RS = 1, SR.idle indicates whether the transfer is complete;
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

/**
 * This function handles the interrupt for the MM2S (memory-mapped to stream) transfer of the AXI DMA device.
 *
 * @param Instance Pointer to the instance of the AXI DMA device structure.
 * @return void
 */
AL_VOID AlAxiDma_Dev_Mm2s_IntrHandler(AL_VOID *Instance)
{
    AlAxiDma_DevStruct *Dma = (AlAxiDma_DevStruct *)Instance;
    AlAxiDma_IntrEnum IntrStatus = AlAxiDma_ll_GetIntrStatus(Dma->RegBase + AL_AXI_DMA_MM2S_OFFSET);

    if (AL_AXIDMA_INTR_ERR(IntrStatus)) {
        AlAxiDma_ll_ClrErr(Dma->RegBase + AL_AXI_DMA_MM2S_OFFSET);
        AlAxiDma_Dev_Transfer_ErrorHandler(Dma, AL_AXIDMA_DMA_TO_DEVICE);
    } else if (AL_AXIDMA_INTR_IOC(IntrStatus)) {
        AlAxiDma_ll_ClrIoc(Dma->RegBase + AL_AXI_DMA_MM2S_OFFSET);
        AlAxiDma_Dev_Transfer_DoneHandler(Dma, AL_AXIDMA_DMA_TO_DEVICE);
    }
}

/**
 * This function handles the interrupt for the S2MM (stream to memory-mapped) transfer of the AXI DMA device.
 *
 * @param Instance Pointer to the instance of the AXI DMA device structure.
 * @return void
 */
AL_VOID AlAxiDma_Dev_S2mm_IntrHandler(AL_VOID *Instance)
{
    AlAxiDma_DevStruct *Dma = (AlAxiDma_DevStruct *)Instance;
    AlAxiDma_IntrEnum IntrStatus = AlAxiDma_ll_GetIntrStatus(Dma->RegBase + AL_AXI_DMA_S2MM_OFFSET);

    if (AL_AXIDMA_INTR_ERR(IntrStatus)) {
        AlAxiDma_ll_ClrErr(Dma->RegBase + AL_AXI_DMA_S2MM_OFFSET);
        AlAxiDma_Dev_Transfer_ErrorHandler(Dma, AL_AXIDMA_DEVICE_TO_DMA);
    } else if (AL_AXIDMA_INTR_IOC(IntrStatus)) {
        AlAxiDma_ll_ClrIoc(Dma->RegBase + AL_AXI_DMA_S2MM_OFFSET);
        AlAxiDma_Dev_Transfer_DoneHandler(Dma, AL_AXIDMA_DEVICE_TO_DMA);
    }
}
