/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_iic_hal.h"

#define AL_IIC_DEVICE_ID            0

#define IIC_MUX_ADDRESS             0x74

#define BOARD_DR1X90_AD101_V20

#ifdef BOARD_DR1X90_AD101_V20
#define EEPROM_ADDRESS              0x50
#define EEPROM_PAGE_SIZE            8
#else
#define EEPROM_ADDRESS              0x54
#define EEPROM_PAGE_SIZE            16
#endif

#define IIC_EEPROM_CHANNEL          0x04

typedef AL_U8                       AddrType;

#define EEPROM_TEST_START_ADDR      0x00

#define IIC_MASTER_TEST_TIMEOUT_MS  1000

/**
 *
 * This function sends a block of data to the EEPROM at the specified slave address.
 * It waits for the write operation to complete by introducing a delay after sending the data.
 *
 * @param Handle Pointer to the IIC HAL structure.
 * @param SlaveAddr The slave address of the EEPROM.
 * @param Buffer Pointer to the data buffer to be written.
 * @param Size The number of bytes to write.
 *
 * @return AL_S32 Returns AL_OK if the write operation is successful, otherwise returns an error code.

 */
AL_S32 static AlIic_EepromWriteData(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Buffer, AL_U32 Size)
{
    AL_S32 Ret;

    Ret = AlIic_Hal_MasterSendDataBlock(Handle, SlaveAddr, Buffer, Size, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        return Ret;
    }

    /* delay > 10ms for write complete */
    AlSys_MDelay(100);

    return AL_OK;
}

/**
 *
 * This function first sends the read address to the EEPROM and then reads a block of data from it.
 * It uses the AlIic_EepromWriteData function to send the read address.
 *
 * @param Handle Pointer to the IIC HAL structure.
 * @param SlaveAddr The slave address of the EEPROM.
 * @param ReadAddr The EEPROM address from where to start reading.
 * @param Buffer Pointer to the buffer where the read data will be stored.
 * @param Size The number of bytes to read.
 *
 * @return AL_S32 Returns AL_OK if the read operation is successful, otherwise returns an error code.
 *
 */
AL_S32 static AlIic_EepromReadData(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U16 ReadAddr,
                                   AL_U8 *Buffer, AL_U32 Size)
{
    AL_S32 Ret;

    /* Send read address */
    Ret = AlIic_EepromWriteData(Handle, SlaveAddr, (AL_U8 *)&ReadAddr, sizeof(AddrType));
    if (Ret != AL_OK) {
        return Ret;
    }

    /* Read data from eeprom */
    Ret = AlIic_Hal_MasterRecvDataBlock(Handle, SlaveAddr, Buffer, Size, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        return Ret;
    }

    return AL_OK;
}

/**
 *
 * This function sets the I2C MUX to the correct channel for EEPROM communication.
 *
 * @param Handle Pointer to the IIC HAL structure.
 *
 * @return AL_S32 Returns AL_OK if the MUX initialization is successful, otherwise returns an error code.
 *
 */
AL_S32 static AlIic_MuxInit(AL_IIC_HalStruct *Handle)
{
    AL_S32 Ret;
    AL_U8 Channel = IIC_EEPROM_CHANNEL;

    Ret = AlIic_EepromWriteData(Handle, IIC_MUX_ADDRESS, &Channel, 1);
    if (Ret != AL_OK) {
        return Ret;
    }

    return AL_OK;
}

/**
 *
 * This function initializes the I2C interface, writes a block of data to the EEPROM,
 * and then reads it back to verify the write operation. It logs the result of the operation.
 *
 * @param None.
 *
 * @return AL_S32 Returns AL_OK if the example completes successfully, otherwise returns an error code.
 *
 */
AL_S32 AlIic_E2promExample()
{
    AL_S32 Ret;
    AL_U16 Index;
    AL_U8 TestFail = 0;
    AL_IIC_HalStruct *Handle;
    AL_U16 SlaveAddr = EEPROM_ADDRESS;
    AL_U16 WriteLen;

    AL_IIC_InitStruct InitConfig =
    {
        .Mode           = AL_IIC_MODE_MASTER,
        .AddrMode       = AL_IIC_ADDR_7BIT,
    };

#ifdef BOARD_DR1X90_AD101_V20
    AL_U8 WriteBuffer[sizeof(AddrType) + EEPROM_PAGE_SIZE] =
    {
        0x00, //Read addr 0x00
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x29
    };
#else
    AL_U8 WriteBuffer[sizeof(AddrType) + EEPROM_PAGE_SIZE] =
    {
        0x00, //Read addr 0x00
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x29,
        0x31, 0x32, 0x33, 0x34, 0x65, 0x36, 0x37, 0x3a,
    };
#endif
    AL_U8 ReadBuffer[EEPROM_PAGE_SIZE] = {0};

    AL_LOG(AL_LOG_LEVEL_INFO, "AlIic_E2promExample\r\n");

    Ret = AlIic_Hal_Init(&Handle, AL_IIC_DEVICE_ID, &InitConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Hal_Init Failed\r\n");
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    /* IIC MUX, switch EEPROM */
    AlIic_MuxInit(Handle);

    if (sizeof(AddrType) == 1) {
        WriteBuffer[0] = (AL_U8) (EEPROM_TEST_START_ADDR);
    } else {
        WriteBuffer[0] = (AL_U8) (EEPROM_TEST_START_ADDR >> 8);
        WriteBuffer[1] = (AL_U8) (EEPROM_TEST_START_ADDR);
    }
    WriteLen = sizeof(WriteBuffer);

    /* Page write, Include address, WriteLen should not should not exceed the page size + 1 */
    Ret = AlIic_EepromWriteData(Handle, SlaveAddr, WriteBuffer, WriteLen);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_EepromWriteData Failed\r\n");
        return Ret;
    }

    /* Read data from e2prom */
    Ret = AlIic_EepromReadData(Handle, SlaveAddr, EEPROM_TEST_START_ADDR, ReadBuffer, EEPROM_PAGE_SIZE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_EepromReadData Failed\r\n");
    }

    for(Index = 0; Index < EEPROM_PAGE_SIZE; Index++) {
        if (WriteBuffer[Index+sizeof(AddrType)] != ReadBuffer[Index]) {
            TestFail = 1;
            break;
        }
    }
    if (TestFail) {
        AL_LOG(AL_LOG_LEVEL_INFO, "E2prom write read fail\r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "E2prom write read pass\r\n");
    }

    return AL_OK;
}

int main()
{
    AlIic_E2promExample();

    return 0;
}