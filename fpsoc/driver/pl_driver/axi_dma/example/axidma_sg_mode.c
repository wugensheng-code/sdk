/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_axi_dma_hal.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/**************************** Macros Definitions *****************************/
#define DMA_DEV_ID                    0
#define GP0_Master                    0x80000000UL
#define STREAM_DATA_GEN_OFFSET        0x100000UL
#define STREAM_DATA_CHECK_OFFSET      0x200000UL

#define BUFFER_SIZE_1                 0x500
#define BUFFER_SIZE_2                 0x500
#define BUFFER_SIZE_3                 0x1000
#define BUFFER_SIZE_4                 0x1000
#define BUFFER_SIZE_5                 0x2000

#define TEST_ITERATIONS               100
#define TEST_START_VALUE              0x0
#define NUM_BUFFERS                   5

#define DEFAULT_THRESHLOD             1
#define TRANSFER_METHOD               1  // 1 = BLOCK, 0 = POLLING

/************************** Variable Definitions *****************************/
AL_AXI_DMA_S2MM_BUFFER_ALIGN AL_U8 TransferBuffer1[BUFFER_SIZE_1];
AL_AXI_DMA_S2MM_BUFFER_ALIGN AL_U8 TransferBuffer2[BUFFER_SIZE_2];
AL_AXI_DMA_S2MM_BUFFER_ALIGN AL_U8 TransferBuffer3[BUFFER_SIZE_3];
AL_AXI_DMA_S2MM_BUFFER_ALIGN AL_U8 TransferBuffer4[BUFFER_SIZE_4];
AL_AXI_DMA_S2MM_BUFFER_ALIGN AL_U8 TransferBuffer5[BUFFER_SIZE_5];

// Array of buffer pointers
AL_U8 *buffers[NUM_BUFFERS] = {TransferBuffer1, TransferBuffer2, TransferBuffer3, TransferBuffer4, TransferBuffer5};

// Lengths for each buffer
AL_U32 bufferLengths[] = {BUFFER_SIZE_1, BUFFER_SIZE_2, BUFFER_SIZE_3, BUFFER_SIZE_4, BUFFER_SIZE_5};

AL_AXI_DMA_DESC_NOCACHE_ALIGN_RAM_SECTION AlAxiDma_Descriptor descriptors[NUM_BUFFERS] = {0};

AlAxiDma_InitStruct InitConfig = {
    .S2mm_Threshold       = DEFAULT_THRESHLOD,
    .S2mm_HasCyclic       = AL_AXI_DMA_FUNC_DISABLE,
    .S2mm_HasKeyHole      = AL_AXI_DMA_FUNC_DISABLE,
    .Mm2s_Threshold       = DEFAULT_THRESHLOD,
    .Mm2s_HasCyclic       = AL_AXI_DMA_FUNC_DISABLE,
    .Mm2s_HasKeyHole      = AL_AXI_DMA_FUNC_DISABLE,
    .descriptors          = descriptors
};

/************************** Function Prototypes ******************************/
static AL_S32 AlAxiDma_SgMode_TransferExample(AL_U16 DeviceId);
static AL_S32 CheckData(AL_U32 *buffer, AL_U32 size, AL_U32 start_test);
static AL_S32 DmaTransferCheck(ALAXIDMA_TransferMsg *msg);
static AL_VOID ReleasePorts(AL_VOID);
static AL_VOID GenerateData(AL_VOID);
static AL_VOID PrepareDataCheck(AL_VOID);

/**
 * This function serves as the main entry point for the program. It logs the entrance and exit of the function,
 * releases any used ports, and runs multiple iterations of a scatter/gather mode DMA transfer test. If any iteration
 * fails, it logs an error message and returns the error code. If all iterations succeed, it logs a success message
 * and returns AL_OK.
 *
 * @return AL_OK on success, or an error code on failure.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "--- Entering main() ---");

    ReleasePorts();

    // Loop for multiple tests
    for (AL_U32 i = 0; i < TEST_ITERATIONS; i++) {
        AL_LOG(AL_LOG_LEVEL_INFO, "---- Scatter/Gather mode test Iteration: %d ----", i + 1);

        /* Run the example for simple transfer */
        Ret = AlAxiDma_SgMode_TransferExample(DMA_DEV_ID);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Failed on iteration %d with error code %d", i + 1, Ret);
            return Ret;
        }
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Completed all iterations.");
    AL_LOG(AL_LOG_LEVEL_INFO, "--- Exiting main() ---");

    return Ret;
}

/**
 * This function performs a scatter/gather mode DMA transfer test. It initializes the DMA handle,
 * sets up the transfer messages and descriptors, and performs both S2MM (Stream to Memory-Mapped) 
 * and MM2S (Memory-Mapped to Stream) transfers. It checks the data integrity at each step and logs 
 * the process. If any step fails, it logs an error message and returns the error code. Otherwise, 
 * it returns AL_OK.
 *
 * @param DeviceId The ID of the DMA device.
 * @return AL_OK on success, or an error code on failure.
 */
static AL_S32 AlAxiDma_SgMode_TransferExample(AL_U16 DeviceId)
{
    AlAxiDma_HalStruct *Handle;
    AL_S32 Ret = AL_OK;

    for (AL_U32 i = 0; i < NUM_BUFFERS; i++) {
        memset(buffers[i], 0, bufferLengths[i]);
    }

    // Configure DMA transfer message
    ALAXIDMA_TransferMsg msg;
    msg.Buffers           = buffers;
    msg.BufferLengths     = bufferLengths;
    msg.NumBuffers        = NUM_BUFFERS;
    msg.Direction         = AL_AXIDMA_DEVICE_TO_DMA;
    msg.S2mmCyclicCount   = 0;
    msg.Mm2sCyclicCount   = 0;

    Ret = AlAxiDma_Hal_Init(&Handle, DeviceId, &InitConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "AlAxiDma_Hal_Init failed %d", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

/************************************ S2MM ******************************************/

    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM/DDR)<--AXIStream(PL) S2MM Start");

    memset(descriptors, 0, NUM_BUFFERS * sizeof(AlAxiDma_Descriptor));

    // Generate data for S2MM transfer (DEVICE_TO_DMA)
    GenerateData();

    // Set up descriptors for S2MM transfer
    Ret = AlAxiDma_Hal_SetupDescriptors(Handle, &msg);
    if (Ret != AL_OK) {
        return Ret;
    }

    // Start S2MM transfer
#if TRANSFER_METHOD == 1
    Ret = AlAxiDma_Hal_SgMode_TransferBlock(Handle, &msg, AL_WAITFOREVER);
#else
    Ret = AlAxiDma_Hal_SgMode_TransferPolling(Handle, &msg);
#endif
    if (Ret != AL_OK) {
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM/DDR)<--AXIStream(PL) S2MM Done");

    // DATA Check
    Ret = DmaTransferCheck(&msg);
    if (Ret != AL_OK) {
        return Ret;
    }

/************************************ MM2S ******************************************/

    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM/DDR)-->AXIStream(PL) MM2S Start");

    memset(descriptors, 0, NUM_BUFFERS * sizeof(AlAxiDma_Descriptor));

    // Prepare for MM2S transfer check
    PrepareDataCheck();

    // Set up descriptors for MM2S transfer
    msg.Direction = AL_AXIDMA_DMA_TO_DEVICE;
    Ret = AlAxiDma_Hal_SetupDescriptors(Handle, &msg);
    if (Ret != AL_OK) {
        return Ret;
    }

#if TRANSFER_METHOD == 1
    Ret = AlAxiDma_Hal_SgMode_TransferBlock(Handle, &msg, AL_WAITFOREVER);
#else
    Ret = AlAxiDma_Hal_SgMode_TransferPolling(Handle, &msg);
#endif
    if (Ret != AL_OK) {
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM/DDR)-->AXIStream(PL) MM2S Done");

    // DATA Check
    Ret = DmaTransferCheck(&msg);
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}

/**
 * This function releases various ports by setting specific register bits. It logs the release process of
 * each port, including the GP master port, HP0 slave port, and GP0 slave port. Additionally, it logs the data
 * width of the HP0 and HP1 slave ports, indicating whether it is 32-bit or 64-bit.
 *
 * @return void
 */
static AL_VOID ReleasePorts(AL_VOID)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Release gp master port:");
    AL_REG32_SET_BIT(0xF8800080, 1, 0);

    AL_LOG(AL_LOG_LEVEL_INFO, "Release hp0 slave port:");
    AL_REG32_SET_BIT(0xF8801078, 0, 1);

    AL_LOG(AL_LOG_LEVEL_INFO, "Release gp0 slave port:");
    AL_REG32_SET_BIT(0xF8801078, 4, 1);

    AL_LOG(AL_LOG_LEVEL_INFO, "HP0 HP1 slave data width %s!", ((*(unsigned int *)0xF8800084) != 0) ? "32bit" : "64bit");
}

/**
 * This function generates data for a DMA transfer by writing specific values to designated registers.
 * It sets the transfer length and the start value for the data generation.
 *
 * @return void
 */
static AL_VOID GenerateData(AL_VOID)
{
    AL_U32 totalSize = 0;
    for (int i = 0; i < NUM_BUFFERS; i++) {
        totalSize += bufferLengths[i];
    }
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_GEN_OFFSET + 0x04, totalSize / (AXI_DMA0_S2MM_STREAM_DATA_WIDTH / 8));
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_GEN_OFFSET + 0x00, TEST_START_VALUE);
}

/**
 * This function prepares for data checking in an MM2S (Memory-Mapped to Stream) DMA transfer
 * by writing specific values to designated registers. It sets the transfer length and the start value
 * for the data verification process.
 *
 * @return void
 */
static AL_VOID PrepareDataCheck(AL_VOID)
{
    AL_U32 totalSize = 0;
    for (int i = 0; i < NUM_BUFFERS; i++) {
        totalSize += bufferLengths[i];
    }
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x04, totalSize / (AXI_DMA0_MM2S_STREAM_DATA_WIDTH / 8));
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x00, TEST_START_VALUE);
}

/**
 * This function checks the integrity of the data in the provided buffer by comparing it against an expected 
 * sequence of values starting from `start_test`. If the data does not match the expected values, it logs an 
 * error message and returns AL_ERROR. Otherwise, it returns AL_OK.
 *
 * @param buffer A pointer to the buffer containing the data to be checked.
 * @param size The size of the buffer in bytes.
 * @param start_test The starting value of the expected data sequence.
 * @return AL_OK on success, or AL_ERROR on data mismatch.
 */
static AL_S32 CheckData(AL_U32 *buffer, AL_U32 size, AL_U32 start_test)
{
    AL_U32 *RxPacket = (AL_U32 *)buffer;
    AL_U32 expectedValue = start_test;

    for (AL_U32 Index = 0; Index < size / 4; Index++) {
        if (RxPacket[Index] != expectedValue) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Data error %d: 0x%x/0x%x",
                   Index, (unsigned int)RxPacket[Index], (unsigned int)expectedValue);
            return AL_ERROR;
        }
        expectedValue++;
    }

    return AL_OK;
}

/**
 * This function checks the integrity of the DMA transfer data based on the transfer direction specified in the 
 * transfer message. For S2MM (Stream to Memory-Mapped) direction, it manually checks the data in each buffer. 
 * For MM2S (Memory-Mapped to Stream) direction, it uses register values to verify the transfer success. If any 
 * check fails, it logs an error message and returns AL_ERROR. Otherwise, it logs a success message and returns AL_OK.
 *
 * @param msg A pointer to the transfer message structure containing transfer details.
 * @return AL_OK on success, or AL_ERROR on data mismatch or transfer failure.
 */
static AL_S32 DmaTransferCheck(ALAXIDMA_TransferMsg *msg)
{
    if (msg->Direction == AL_AXIDMA_DEVICE_TO_DMA) {
        AL_U32 offset = 0;
        AL_S32 ret;
        for (int i = 0; i < NUM_BUFFERS; i++) {
            ret = CheckData((AL_U32 *)buffers[i], bufferLengths[i], TEST_START_VALUE + offset / (AXI_DMA0_MM2S_STREAM_DATA_WIDTH / 8));
            if (ret != AL_OK) return ret;
            offset += bufferLengths[i];
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "S2MM AXI Stream manual data check success!");
    } else {
        if (AL_REG32_READ(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x08)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "MM2S AXI Stream auto data check fail!");
            return AL_ERROR;
        } else if (AL_REG32_READ(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x0C) &&
                   AL_REG32_READ(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x10)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "MM2S AXI Stream auto data check success!");
        }
    }

    return AL_OK;
}
