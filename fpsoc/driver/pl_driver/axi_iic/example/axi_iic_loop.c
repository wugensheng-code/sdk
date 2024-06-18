/**
 * AXI IIC Loop Test.
*/

#include "al_axi_iic_hal.h"

#define IIC_LOOP_SLAVE_ADDR 0x52

extern AlAxiIic_HalStruct AXI_IIC_Handle[AL_AXI_IIC_NUM_INSTANCE];

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

AL_VOID SlaveCallback(AlAxiIic_EventStruct *Event, AL_VOID *CallbackRef)
{
    AlAxiIic_HalStruct *Handle = CallbackRef;
    AL_U8 *RecvBuff = Handle->Iic.RecvBuffer.BuffPtr;

    printf("RecvDataCnt = %d\n", Event->EventData);
    for (int i = 0; i < Event->EventData; i++)
    {
        printf("%d: %x\n", i, RecvBuff[i]);
    }
}

AL_S32 AlAxiIic_SlaveTest(AlAxiIic_HalStruct *Handle)
{
    AL_U32 DevId = 1;
    AL_U32 Ret;
    AlAxiIic_InitStruct InitConfig = 
    {
        .IIC_Mode           = AL_AXI_IIC_SLAVE      ,
        .IIC_AddressMode    = AL_AXI_IIC_7BIT       ,
        .SlaveAddr          = IIC_LOOP_SLAVE_ADDR   ,
    };
    
    Ret = AlAxiIic_Hal_Init(&Handle, DevId, &InitConfig, SlaveCallback);
    if (Ret != AL_OK)
    {
        return Ret;
    }

    AL_U8 RecvBuf[] = { 0x0 };
    Ret = AlAxiIic_Hal_SlaveRecvData(&Handle, RecvBuf);
    if (Ret != AL_OK)
    {
        return Ret;
    }
}

AL_VOID MasterCallback(AlAxiIic_EventStruct *Event, AL_VOID *CallbackRef)
{
    // AlAxiIic_HalStruct *Handle = CallbackRef;

    printf("Event->Event = %d\n", Event->Event);
}

AL_S32 AlAxiIic_MasterTest()
{
    AlAxiIic_HalStruct *Handle;
    AL_U32 DevId = 0;
    AL_U32 Ret;
    
    AlAxiIic_InitStruct InitConfig = 
    {
        .IIC_Mode           =   AL_AXI_IIC_MASTER           ,
        .IIC_AddressMode    =   AL_AXI_IIC_7BIT             ,
        .SpeedMode          =   AL_AXI_IIC_FAST_MODE        ,
        .Taclk              =   20              
    };

    // Ret = AlAxiIic_Hal_Init(&Handle, DevId, &InitConfig, MasterCallback);
    Ret = AlAxiIic_Hal_Init(&Handle, DevId, &InitConfig, AL_NULL);
    // AlAxiIic_SetInt(Handle->Iic.HwConfig.BaseAddress, INT_CMPL, AL_AXI_IIC_FUNC_ENABLE);
    if (Ret != AL_OK)
    {
        return Ret;
    }

    AL_U32 SendSize = 1;
    AL_U8 SendBuf[] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9 };

    Ret = AlAxiIic_Hal_MasterSendData(&Handle, IIC_LOOP_SLAVE_ADDR, SendBuf, SendSize);
    if (Ret != AL_OK)
    {
        return Ret;
    }

    return AL_OK;
}

AL_S32 main()
{

    GP_Port_Enable();
    printf("=== AXI IIC Test ===\n");

    AlAxiIic_HalStruct *SlaveHandle;

    AlAxiIic_SlaveTest(SlaveHandle);
    AlAxiIic_MasterTest();

    while (1)
    {

    }
    return 0;
}
