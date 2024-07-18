/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_iic_hal.h"

#define AL_IIC_DEVICE_ID            0

#define BUFFER_SIZE                 128
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
 * This function sets the I2C multiplexer to the correct channel for EEPROM communication.
 * It sends a single byte, which is the channel number, to the I2C multiplexer's address.
 * After sending the channel number, it waits for 100 milliseconds to ensure the multiplexer
 * has switched to the correct channel before proceeding with further I2C operations.
 *
 * @param Handle Pointer to the IIC HAL structure, which contains the configuration and state
 *               information required by the HAL functions.
 *
 * @return AL_S32 Returns AL_OK if the multiplexer was successfully initialized, otherwise returns
 *                an error code indicating the failure reason.
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
 * This function initializes the I2C interface with the specified device ID and configuration,
 * then sends a predefined block of data to a slave device with a specific address. After sending
 * the data, it attempts to receive a block of data from the same slave device. Both the sending
 * and receiving operations are performed with a specified timeout. The received data is then
 * printed to the console. This function demonstrates the basic send and receive capabilities
 * of the I2C interface using the HAL layer.
 *
 * @return Returns AL_OK if both send and receive operations were successful, otherwise
 *         returns an error code. Specific error codes indicate whether the initialization,
 *         send, or receive operations failed.
 *
 */
AL_S32 AlIic_MasterBlockExample()
{
    AL_S32 Ret;
    AL_IIC_HalStruct *Handle;
    AL_IIC_InitStruct MasterInitConfig =
    {
        .Mode           = AL_IIC_MODE_MASTER,
        .AddrMode       = AL_IIC_ADDR_7BIT,
    };

    AL_U8 SendData[BUFFER_SIZE] =
    {
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    };

    AL_U8 RecvData[BUFFER_SIZE] = {0};

    AL_U16 SlaveAddr = TEST_SLAVE_ADDR;

    AL_LOG(AL_LOG_LEVEL_INFO, "AlIic_MasterBlockExample\r\n");

    Ret = AlIic_Hal_Init(&Handle, AL_IIC_DEVICE_ID, &MasterInitConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_Init Failed\r\n");
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlIic_MuxInit(Handle);

    Ret = AlIic_Hal_MasterSendDataBlock(Handle, SlaveAddr, SendData, BUFFER_SIZE, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_MasterSendDataBlock Failed\r\n");
        return Ret;
    }

    Ret = AlIic_Hal_MasterRecvDataBlock(Handle, SlaveAddr, RecvData , BUFFER_SIZE, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_MasterRecvDataBlock Failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "AlIic_Hal_MasterRecvDataBlock Data:\r\n");
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
 * This function is the starting point of the program. It calls the AlIic_MasterBlockExample
 * function to demonstrate the I2C send and receive functionality. The function returns 0
 * upon successful completion of the demonstration.
 *
 * @return int Returns 0 upon successful completion.
 *
 */
int main()
{
    AlIic_MasterBlockExample();

    return 0;
}