/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_iic_hal.h"

#define AL_IIC_DEVICE_ID            0

#define BUFFER_SIZE                 128
#define TEST_COUNT                  16

#define TEST_SLAVE_ADDR             0x77
#define IIC_MASTER_TEST_TIMEOUT_MS  100

#define IIC_MUX_ADDRESS             0x74
#define FMC_IIC_CHANNEL             0x10

/**
 *
 * This function configures the I2C multiplexer to connect to the specified channel,
 * allowing communication with devices connected to that channel. It sends a command
 * to the multiplexer to set the channel, using the I2C address of the multiplexer.
 *
 * @param Handle Pointer to the IIC HAL structure. This structure contains the configuration
 *               and state information required by the HAL functions to manage the I2C operations.
 *
 * @return AL_S32 Returns AL_OK if the operation was successful, otherwise returns an error code.
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
 * This function showcases the process of initializing an I2C interface in master mode with a 10-bit
 * addressing scheme. It sends a predefined block of data to a slave device and then receives a block
 * of data from the same device. The function demonstrates setting up the I2C, including switching
 * the address mode to 10-bit, setting the I2C rate, and handling data transmission and reception.
 *
 * @return Returns AL_OK if both send and receive operations were successful. If any
 *         operation fails, it returns an error code indicating the failure reason.
 *
 */
AL_S32 AlIic_MasterFast10BitExample()
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

    AL_LOG(AL_LOG_LEVEL_INFO, "AlIic_Master10BitBlockExample\r\n");

    Ret = AlIic_Hal_Init(&Handle, AL_IIC_DEVICE_ID, &MasterInitConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_Init Failed\r\n");
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlIic_MuxInit(Handle);

    /*
      The IIC mux is in 7-bit address mode.
      The slave address mode is 10bit,
      need to switch to a 10bit address here,
      Use AlIic_Hal_IoCtl to set 10bit address mode.
     */
    AL_IIC_AddrModeEnum AddrMode = AL_IIC_ADDR_10BIT;
    Ret = AlIic_Hal_IoCtl(Handle, AL_IIC_IOCTL_SET_ADDR_MODE, &AddrMode);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_IoCtl Failed\r\n");
        return Ret;
    }

    AL_U32 IicRate = AL_IIC_RATE_400K;
    Ret = AlIic_Hal_IoCtl(Handle, AL_IIC_IOCTL_SET_RATE, &IicRate);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_IoCtl Failed\r\n");
        return Ret;
    }

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
 * This function serves as the starting point of the program. It calls the AlIic_MasterFast10BitExample
 * function to demonstrate I2C communication using a 10-bit addressing mode. The function returns 0
 * upon successful completion of the demonstration.
 *
 * @return int Returns 0 upon successful completion.
 *
 */
int main()
{
    AlIic_MasterFast10BitExample();

    return 0;
}