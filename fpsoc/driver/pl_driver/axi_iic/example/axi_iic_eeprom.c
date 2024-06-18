/**
 * Axi Iic Eeprom Test.
*/

#include "al_axi_iic_hal.h"

#define AXI_LOOP_MASTER             0x80000000
#define IIC_MUX_ADDRESS             0x74
#define EEPROM_ADDRESS              0x54
#define IIC_EEPROM_CHANNEL          0x04

AL_VOID GP_Port_Enable()
{
    AL_U32 val = AL_REG32_READ(0xF8800080);
    val &= ~0x2;
    AL_REG32_WRITE(0xF8800080, val);
    val = 0;
    val = AL_REG32_READ(0xF8801078);
    AL_REG32_WRITE(0xF8801078, val & (~0x33));
    AlSys_MDelay(1);
    
    AL_REG32_WRITE(0xF8801078, val | 0x33);
}

AL_S32 static AxiIic_MuxInit(AlAxiIic_HalStruct *Handle)
{
    AL_U8 Channel = IIC_EEPROM_CHANNEL;

    AlAxiIic_Dev_CreateTrans(&Handle->Iic, AL_AXI_IIC_STOP);
    AlAxiIic_Hal_MasterSendData(&Handle, IIC_MUX_ADDRESS, &Channel, 1);
	AlSys_MDelay(10);
}

AL_S32 Eeprom_WriteData(AlAxiIic_HalStruct *Handle, AL_U8 *SendBuff, AL_U32 SendSize)
{
	AlAxiIic_Dev_CreateTrans(&Handle->Iic, AL_AXI_IIC_STOP);
    AlAxiIic_Hal_MasterSendData(&Handle, EEPROM_ADDRESS, SendBuff, SendSize);
	AlSys_MDelay(10);	/* Wait Writing Finish */
}

AL_S32 Eeprom_ReadData(AlAxiIic_HalStruct *Handle, AL_U8 StartReadAddr, AL_U8 *RecvBuff, AL_U32 RecvSize)
{
	AlAxiIic_Dev_CreateTrans(&Handle->Iic, AL_AXI_IIC_STOP);
	AlAxiIic_Hal_MasterSendData(&Handle, EEPROM_ADDRESS, &StartReadAddr, 1);
	AlSys_MDelay(1);
	AlAxiIic_Dev_CreateTrans(&Handle->Iic, AL_AXI_IIC_STOP);
	AlAxiIic_Hal_MasterRecvData(&Handle, EEPROM_ADDRESS, RecvBuff, RecvSize);
}

AL_S32 AxiIic_EepromExample()
{
    AlAxiIic_HalStruct *Handle;
    AL_U32 DevId = 0;
    AL_U32 Ret;

    AlAxiIic_InitStruct InitConfig = 
    {
        .IIC_Mode           =   AL_AXI_IIC_MASTER           ,
        .IIC_AddressMode    =   AL_AXI_IIC_7BIT             ,
        .SpeedMode          =   AL_AXI_IIC_STANDARD_MODE  ,
        .Taclk              =   20              
    };
    Ret = AlAxiIic_Hal_Init(&Handle, DevId, &InitConfig, AL_NULL);
    if (Ret != AL_OK)
    {
        return Ret;
    }

    AxiIic_MuxInit(Handle);
    
    AL_U8 WriteBuf[] = 
    { 
        0x0,    /* Start Write Address */
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19
    };
    AL_U8 ReadBuf[] = { 0x0 };

    AL_U32 WriteSize = sizeof(WriteBuf);
    AL_U32 ReadSize = WriteSize - 1;
	AL_U8 StartReadAddr = 0x0;

	Eeprom_WriteData(Handle, WriteBuf, WriteSize);
	Eeprom_ReadData(Handle, StartReadAddr, ReadBuf, ReadSize);
    for (int i = 0; i < ReadSize; i++)
    {
        printf("data%d = %d\n", i, ReadBuf[i]);
    }   
}

int main()
{
    GP_Port_Enable();
    printf("=== Axi Iic Eeprom Test ===\n");
    
    AxiIic_EepromExample();
    
    return 0;
}


