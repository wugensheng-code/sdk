/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @note This example unable to run on the board,
 *  just shows how to use the IIC slave interface
 *
 */


#include "al_iic_hal.h"

#define AL_IIC_DEVICE_ID            0

#define BUFFER_SIZE                 128
#define TEST_SLAVE_ADDR             0x77
#define IIC_SLAVE_TEST_TIMEOUT_MS   100000
/**
 *
 * This function initializes the I2C interface with the specified device ID and configuration,
 * then sends a predefined block of data to a master device. After sending
 * the data, it attempts to receive a block of data from the same master device. Both the sending
 * and receiving operations are performed with a specified timeout. The received data is then
 * printed to the console. This function demonstrates the basic send and receive capabilities
 * of the I2C interface using the HAL layer.
 *
 * @return Returns AL_OK if both send and receive operations were successful, otherwise
 *         returns an error code. Specific error codes indicate whether the initialization,
 *         send, or receive operations failed.
 *
 */
AL_S32 AlIic_SlaveBlockExample()
{
    AL_S32 Ret;
    AL_IIC_HalStruct *Handle;
    AL_IIC_InitStruct SlaveInitConfig =
    {
        .Mode           = AL_IIC_MODE_SLAVE,
        .AddrMode       = AL_IIC_ADDR_7BIT,
        .SlaveAddr      = TEST_SLAVE_ADDR,
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

    AL_LOG(AL_LOG_LEVEL_INFO, "AlIic_SlaveBlockExample\r\n");

    Ret = AlIic_Hal_Init(&Handle, AL_IIC_DEVICE_ID, &SlaveInitConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_Init Failed\r\n");
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    Ret = AlIic_Hal_SlaveSendDataBlock(Handle, SendData, BUFFER_SIZE, IIC_SLAVE_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_SlaveSendDataBlock Failed\r\n");
        return Ret;
    }

    Ret = AlIic_Hal_SlaveRecvDataBlock(Handle, RecvData, BUFFER_SIZE, IIC_SLAVE_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_SlaveRecvDataBlock Failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "AlIic_Hal_SlaveRecvDataBlock Data:\r\n");
    for (AL_U32 Index = 0; Index < BUFFER_SIZE; Index++)
    {
        printf("0x%x ", RecvData[Index]);
        if ((Index+1) % 16 == 0) {
            printf("\r\n");
        }
    }

    return AL_OK;
}

/**
 *
 * This function is the starting point of the program. It calls the AlIic_SlaveBlockExample
 * function to demonstrate the I2C send and receive functionality. The function returns 0
 * upon successful completion of the demonstration.
 *
 * @return int Returns 0 upon successful completion.
 *
 */
int main()
{
    AlIic_SlaveBlockExample();

    return 0;
}