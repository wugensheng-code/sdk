/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_vdma_hal.h"

AlAxiVdma_Hal_Struct *Vdma;
AlAxiVdma_InitConfig *VdmaInit;

AL_BOOL TransFlag = 0;
AL_U8 ReadArray[64*64*5];
AL_U8 WriteArray[64*64*5];

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


static AL_VOID AlAxiVdma_EventHandler(AlAxiVdma_EventStruct VdmaEvent, AL_VOID *CallbackRef)
{
    switch (VdmaEvent.Events)
    {
    case AlAxiVdma_Event_Mm2S:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al AxiVdma Mm2S interrupt");
        AlAxiVdma_Hal_Stop(Vdma, AL_VDMA_MM2S_CHANNEL);
        break;
    case AlAxiVdma_Event_S2Mm:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al AxiVdma S2Mm interrupt");
        TransFlag = 1;
        AlAxiVdma_Hal_Stop(Vdma, AL_VDMA_S2MM_CHANNEL);
        break;
    default:
        break;
    }
}

AL_VOID AxiVdmaLoopbackTest()
{
    AL_S32 Status;

    for (AL_S32 i = 0; i < 64 * 64 * 5; i++) {
        ReadArray[i] = (AL_U8)i;
    }

    for (AL_S32 i = 0; i < 64 * 64 * 5; i++) {
        WriteArray[i] = (AL_U8)i + 5;
    }

    AlCache_FlushDcacheAll();

    AL_U32 FrameStartAddr = 0x2000000;
    AL_U32 Index;
    AL_U32 MaxFrameStoreNum = 1;

    VdmaInit = malloc(sizeof(AlAxiVdma_InitConfig));
    memset(VdmaInit, 0, sizeof(AlAxiVdma_InitConfig));

    VdmaInit->ReadChannelCfg.CircularPark = 1;
    VdmaInit->ReadChannelCfg.FrameCount = 5;
    VdmaInit->ReadChannelCfg.FrmCntIrqEn = 1;
    VdmaInit->ReadChannelCfg.ErrIrqEn = 1;
    VdmaInit->ReadChannelCfg.GenlockSrc = 1;
    VdmaInit->ReadChannelCfg.GenlockEn = 1;
    VdmaInit->ReadChannelCfg.Mm2sHsize = 64 ;
    VdmaInit->ReadChannelCfg.Mm2sVsize = 64;
    VdmaInit->ReadChannelCfg.Stride = 64;
    VdmaInit->ReadChannelCfg.FrameStoreStartAddr[0]= &ReadArray[0];
    VdmaInit->ReadChannelCfg.FrameStoreStartAddr[1]= &ReadArray[64*64];
    VdmaInit->ReadChannelCfg.FrameStoreStartAddr[2]= &ReadArray[64*64*2];
    VdmaInit->ReadChannelCfg.FrameStoreStartAddr[3]= &ReadArray[64*64*3];
    VdmaInit->ReadChannelCfg.FrameStoreStartAddr[4]= &ReadArray[64*64*4];

    VdmaInit->WriteChannelCfg.CircularPark = 1;
    VdmaInit->WriteChannelCfg.FrameCount = 5;
    VdmaInit->WriteChannelCfg.FrmCntIrqEn = 1;
    VdmaInit->WriteChannelCfg.ErrIrqEn = 1;
    VdmaInit->WriteChannelCfg.GenlockSrc = 1;
    VdmaInit->WriteChannelCfg.GenlockEn = 1;
    VdmaInit->WriteChannelCfg.S2MmHsize = 64;
    VdmaInit->WriteChannelCfg.S2MmVsize = 64;
    VdmaInit->WriteChannelCfg.Stride = 64;
    VdmaInit->WriteChannelCfg.FrameStoreStartAddr[0]= &WriteArray[0];
    VdmaInit->WriteChannelCfg.FrameStoreStartAddr[1]= &WriteArray[64*64];
    VdmaInit->WriteChannelCfg.FrameStoreStartAddr[2]= &WriteArray[64*64*2];
    VdmaInit->WriteChannelCfg.FrameStoreStartAddr[3]= &WriteArray[64*64*3];
    VdmaInit->WriteChannelCfg.FrameStoreStartAddr[4]= &WriteArray[64*64*4];

    VdmaInit->EnableVerticalFlip = 1;

    AlAxiVdma_Hal_Init(&Vdma, 0, VdmaInit, AlAxiVdma_EventHandler);

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlAxiVdma_Hal_Start(Vdma, AL_VDMA_MM2S_CHANNEL);
    AlAxiVdma_Hal_Start(Vdma, AL_VDMA_S2MM_CHANNEL);
}

AL_S32 main()
{
    AL_U32 Val = 0;
    AL_S32 ret = AL_OK;
    AL_BOOL IsEqual = AL_TRUE;

    AL_LOG(AL_LOG_LEVEL_INFO, "Axi vdma loopback test!");

    GP_Port_Enable();

    AxiVdmaLoopbackTest();

    while (1) {
        if (TransFlag == 1)
            break;
    }

    for (AL_S32 i = 0; i < 64 * 64 * 5; i++) {
        if (WriteArray [i] != ReadArray[i]) {
                IsEqual = AL_FALSE;
                AL_LOG(AL_LOG_LEVEL_INFO, "Test Failed at index %d: ReadArray[%d] = %u, WriteArray[%d] = %u\n", i, i, ReadArray[i], i, WriteArray[i]);
            break;
        }
    }

    if (IsEqual) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Test Passed: All elements are equal.\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO,"Test Failed: Not all elements are equal.\n");
    }

    return 0;
}