#include "al_iic_hal.h"

#define IIC_MUX_ADDRESS         0x74

#define EEPROM_ADDRESS          0x54
#define IIC_EEPROM_CHANNEL      0x04

#define IIC_MASTER_TEST_TIMEOUT_MS      1000

AL_S32 AlIic_MuxInit(AL_IIC_HalStruct *Handle)
{
    AL_S32 Ret;
    AL_U8 Channel = IIC_EEPROM_CHANNEL;

    Ret = AlIic_Hal_MasterSendDataBlock(Handle, IIC_MUX_ADDRESS, &Channel, 1, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        printf("AlIic_Hal_MasterSendDataBlock Failed\r\n");
    }

    return AL_OK;
}

AL_S32 AlIic_DemoBoardE2promTest()
{
    AL_S32 Ret;
    AL_U16 Index;
    AL_U8 TestFail = 0;
    AL_IIC_HalStruct Handle;
    AL_U32 DelayCnt = 0;
    AL_U16 ReadWriteAddr = 0x0000;
    AL_U16 SlaveAddr = EEPROM_ADDRESS;

    AL_IIC_InitStruct InitConfig =
    {
        .Mode           = AL_IIC_MODE_MASTER,
        .AddrMode       = AL_IIC_ADDR_7BIT,
        .SpeedMode      = AL_IIC_STANDARD_MODE,
    };
    AL_U8 WriteBuffer[130] =
    {
        0x00, //Read addr 0x00
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x29,
        0x31, 0x32, 0x33, 0x34, 0x65, 0x36, 0x37, 0x3a,
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
        0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
        0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
        0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
        0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
        0xa1, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0xb1, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0xc1, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0xd1, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
        0xf1, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x21, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x31, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    };
    AL_U8 ReadBuffer[128] = {0};

    printf("AlIic_DemoBoardE2promTest start \r\n");

    Ret = AlIic_Hal_Init(&Handle, 0, &InitConfig, AL_NULL);
    if (Ret != AL_OK) {
        printf("AlIic_Hal_Init Failed\r\n");
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    /* IIC MUX, switch EEPROM */
    AlIic_MuxInit(&Handle);

    /* Page write, Include address */
    Ret = AlIic_Hal_MasterSendDataBlock(&Handle, SlaveAddr, WriteBuffer, 17, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        printf("AlIic_Hal_MasterSendDataBlock Failed\r\n");
    }

    /* delay > 10ms for write complete */
    AlSys_MDelay(100);

    /* Send read address */
    Ret = AlIic_Hal_MasterSendDataBlock(&Handle, SlaveAddr, (AL_U8*)&ReadWriteAddr, 1, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        printf("AlIic_Hal_MasterSendDataBlock Failed\r\n");
    }

    /* Read data from e2prom */
    Ret = AlIic_Hal_MasterRecvDataBlock(&Handle, SlaveAddr, ReadBuffer, 16, IIC_MASTER_TEST_TIMEOUT_MS);
    if (Ret != AL_OK) {
        printf("AlIic_Hal_MasterRecvDataBlock Failed\r\n");
    }

    for(Index = 0; Index < 16; Index++) {
        if (WriteBuffer[Index+1] != ReadBuffer[Index]) {
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
