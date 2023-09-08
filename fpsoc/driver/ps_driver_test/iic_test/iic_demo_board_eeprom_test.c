/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_iic_hal.h"

#define IIC_MUX_ADDRESS             0x74

#define EEPROM_ADDRESS              0x54
#define IIC_EEPROM_CHANNEL          0x04

typedef AL_U8                       AddrType;
#define EEPROM_PAGE_SIZE            16

#define EEPROM_TEST_START_ADDR      0x00

#define IIC_MASTER_TEST_TIMEOUT_MS  1000

AL_S32 static AlIic_MuxInit(AL_IIC_HalStruct *Handle)
{
    AL_S32 Ret;
    AL_U8 Channel = IIC_EEPROM_CHANNEL;

    Ret = AlIic_Hal_MasterSendDataBlock(Handle, IIC_MUX_ADDRESS, &Channel, 1, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        return Ret;
    }

    return AL_OK;
}

AL_S32 AlIic_EepromWriteData(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Buffer, AL_U32 Size)
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

AL_S32 AlIic_EepromReadData(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U16 ReadAddr, AL_U8 *Buffer, AL_U32 Size)
{
    AL_S32 Ret;

    /* Send read address */
    Ret = AlIic_Hal_MasterSendDataBlock(Handle, SlaveAddr, (AL_U8 *)&ReadAddr,
                                        sizeof(AddrType), IIC_MASTER_TEST_TIMEOUT_MS);
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

AL_S32 AlIic_DemoBoardE2promTest()
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
        .SpeedMode      = AL_IIC_STANDARD_MODE,
    };
    AL_U8 WriteBuffer[sizeof(AddrType) + EEPROM_PAGE_SIZE] =
    {
        0x00, //Read addr 0x00
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x29,
        0x31, 0x32, 0x33, 0x34, 0x65, 0x36, 0x37, 0x3a,
    };
    AL_U8 ReadBuffer[EEPROM_PAGE_SIZE] = {0};

    printf("AlIic_DemoBoardE2promTest start \r\n");

    Ret = AlIic_Hal_Init(&Handle, 0, &InitConfig, AL_NULL);
    if (Ret != AL_OK) {
        printf("AlIic_Hal_Init Failed\r\n");
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
        printf("AlIic_EepromWriteData Failed\r\n");
        return Ret;
    }

    /* Read data from e2prom */
    Ret = AlIic_EepromReadData(Handle, SlaveAddr, EEPROM_TEST_START_ADDR, ReadBuffer, EEPROM_PAGE_SIZE);
    if (Ret != AL_OK) {
        printf("AlIic_EepromReadData Failed\r\n");
    }

    for(Index = 0; Index < 16; Index++) {
        if (WriteBuffer[Index+sizeof(AddrType)] != ReadBuffer[Index]) {
            TestFail = 1;
            break;
        }
    }
    if (TestFail) {
        printf("E2prom write read test fail\r\n");
    } else {
        printf("E2prom write read test pass\r\n");
    }

    return AL_OK;
}
