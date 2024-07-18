/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_iic_hal.h"

#define AL_IIC_DEVICE_ID            0

#define BUFFER_SIZE                 (128)
#define TEST_SLAVE_ADDR             0x77
#define IIC_MASTER_TEST_TIMEOUT_MS  100

#define IIC_MUX_ADDRESS             0x74

#define BOARD_DR1X90_AD101_V20

#ifdef BOARD_DR1X90_AD101_V20
#define FMC_IIC_CHANNEL             0x01
#else
#define FMC_IIC_CHANNEL             0x10
#endif

/**
 *
 * This function configures the I2C multiplexer to the correct channel to enable communication
 * with the desired device. It sends the channel number to the multiplexer's address and waits
 * for a short period to ensure the channel is set correctly before proceeding with further
 * operations.
 *
 * @param Handle Pointer to the IIC HAL structure, which contains the configuration and state
 *               information required by the HAL functions. This structure is used to initiate
 *               communication with the multiplexer.
 *
 * @return Returns AL_OK if the multiplexer was successfully initialized, otherwise returns
 *         an error code indicating the failure reason.
 *
 */
AL_S32 static AlIic_MuxInit(AL_IIC_HalStruct *Handle)
{
    AL_S32 Ret;
    AL_U8 Channel = FMC_IIC_CHANNEL;

    Ret = AlIic_Hal_MasterSendDataBlock(Handle, IIC_MUX_ADDRESS, &Channel, 1, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        return Ret;
    }

    AlSys_MDelay(100);

    return AL_OK;
}

/**
 *
 * This function showcases how to initialize the I2C interface in DMA mode, send a block of data,
 * and then receive a block of data from a slave device. It prepares the data for transmission,
 * ensuring alignment and correct formatting for DMA transfer, and then performs both sending and
 * receiving operations. After the data is received, it is printed to the console. This function
 * demonstrates the use of DMA for efficient data transfer over I2C.
 *
 * @return Returns AL_OK if both send and receive operations were successful, otherwise
 *         returns an error code. Specific error codes indicate whether the initialization,
 *         send, or receive operations failed.
 *
 */
AL_S32 AlIic_MasterDmaExample()
{
    AL_S32 Ret;
    AL_IIC_HalStruct *Handle;
    AL_IIC_InitStruct MasterInitConfig =
    {
        .Mode           = AL_IIC_MODE_MASTER,
        .AddrMode       = AL_IIC_ADDR_7BIT,
    };

    AL_U16 SlaveAddr = TEST_SLAVE_ADDR;

    /*
      The data to be sent must be 32-bit. During DMA transmission,
      32-bit data is directly written to the IC_DATA_CMD register,
      where the lower 8 bits are actual data and bit8 to bit10 are commands.
      For details, refer to the IC_DATA_CMD register.
    */
    AL_U32 SendData[BUFFER_SIZE] CACHE_LINE_ALIGN =
    {
        0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8,
        0x9,  0xa,  0xb,  0xc,  0xd,  0xe,  0xf,  0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
        0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
        0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
        0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60,
        0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
        0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
        0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
        0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80,
    };

    /*
      AL_IIC_STOP_ENABLE must be enabled for the last data,
      because no stop signal is sent after the dma transmission is complete.
    */
    SendData[BUFFER_SIZE-1] |= AL_IIC_STOP_ENABLE;

    AL_U8 RecvData[BUFFER_SIZE] CACHE_LINE_ALIGN = {0};

    AL_LOG(AL_LOG_LEVEL_INFO, "AlIic_MasterDmaExample\r\n");

    Ret = AlIic_Hal_Init(&Handle, AL_IIC_DEVICE_ID, &MasterInitConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_Init failed\r\n");
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlIic_MuxInit(Handle);

#ifdef ENABLE_MMU
    AlCache_FlushDcacheRange((AL_UINTPTR)SendData, (AL_UINTPTR)(SendData + BUFFER_SIZE));
#endif

    Ret = AlIic_Hal_MasterDmaSendDataBlock(Handle, SlaveAddr, SendData, BUFFER_SIZE, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_MasterDmaSendDataBlock failed\r\n");
        return Ret;
    }

    Ret = AlIic_Hal_MasterDmaRecvDataBlock(Handle, SlaveAddr, RecvData , BUFFER_SIZE, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_MasterDmaRecvDataBlock failed\r\n");
        return Ret;
    }

#ifdef ENABLE_MMU
    AlCache_InvalidateDcacheRange((AL_UINTPTR)RecvData, (AL_UINTPTR)(RecvData + BUFFER_SIZE));
#endif

    AL_LOG(AL_LOG_LEVEL_INFO, "AlIic_Hal_MasterDmaRecvDataBlock Data:\r\n");
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        printf("0x%x ", RecvData[i]);
        if ((i+1) % 16 == 0) {
            printf("\r\n");
        }
    }

    return AL_OK;
}

/**
 *
 * This function serves as the starting point of the program. It calls the AlIic_MasterDmaExample
 * function to demonstrate the I2C send and receive functionality in DMA mode. The function returns
 * 0 upon successful completion of the demonstration.
 *
 * @return Returns 0 upon successful completion.
 *
 */
int main()
{
    AlIic_MasterDmaExample();

    return 0;
}