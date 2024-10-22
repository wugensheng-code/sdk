/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_vdma_hal.h"

AlAxiVdma_Hal_Struct *Vdma;
AlAxiVdma_ChannelCfg *Mm2sChannelCfg;
AlAxiVdma_ChannelCfg *S2MmChannelCfg;

volatile AL_BOOL TransFlag = 0;
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
    case ALAXIVDMA_EVENT_MM2S_FRAMECNT:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al AxiVdma Mm2s interrupt");
        AlAxiVdma_Hal_Stop(Vdma, AL_VDMA_MM2S_CHANNEL);
        break;
    case ALAXIVDMA_EVENT_MM2S_ERRIRQ:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al AxiVdma Mm2s error interrupt");
        AlAxiVdma_Hal_Stop(Vdma, AL_VDMA_MM2S_CHANNEL);
        break;
    case ALAXIVDMA_EVENT_S2MM_FRAMECNT:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al AxiVdma S2Mm interrupt");
        TransFlag = 1;
        AlAxiVdma_Hal_Stop(Vdma, AL_VDMA_S2MM_CHANNEL);
        break;
    case ALAXIVDMA_EVENT_S2MM_ERRIRQ:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al AxiVdma S2Mm error interrupt");
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

    Mm2sChannelCfg = malloc(sizeof(AlAxiVdma_ChannelCfg));
    memset(Mm2sChannelCfg, 0, sizeof(AlAxiVdma_ChannelCfg));

    S2MmChannelCfg = malloc(sizeof(AlAxiVdma_ChannelCfg));
    memset(S2MmChannelCfg, 0, sizeof(AlAxiVdma_ChannelCfg));


    Mm2sChannelCfg->CircularPark = 1;
    Mm2sChannelCfg->FrameCount = 5;
    Mm2sChannelCfg->FrmCntIrqEn = 1;
    Mm2sChannelCfg->ErrIrqEn = 1;
    Mm2sChannelCfg->GenlockSrc = 1;
    Mm2sChannelCfg->GenlockEn = 1;
    Mm2sChannelCfg->Hsize = 64 ;
    Mm2sChannelCfg->Vsize = 64;
    Mm2sChannelCfg->Stride = 64;
    Mm2sChannelCfg->FrameStoreStartAddr[0]= &ReadArray[0];
    Mm2sChannelCfg->FrameStoreStartAddr[1]= &ReadArray[64*64];
    Mm2sChannelCfg->FrameStoreStartAddr[2]= &ReadArray[64*64*2];
    Mm2sChannelCfg->FrameStoreStartAddr[3]= &ReadArray[64*64*3];
    Mm2sChannelCfg->FrameStoreStartAddr[4]= &ReadArray[64*64*4];

    S2MmChannelCfg->CircularPark = 1;
    S2MmChannelCfg->FrameCount = 5;
    S2MmChannelCfg->FrmCntIrqEn = 1;
    S2MmChannelCfg->ErrIrqEn = 1;
    S2MmChannelCfg->GenlockSrc = 1;
    S2MmChannelCfg->GenlockEn = 1;
    S2MmChannelCfg->Hsize = 64;
    S2MmChannelCfg->Vsize = 64;
    S2MmChannelCfg->Stride = 64;
    S2MmChannelCfg->FrameStoreStartAddr[0]= &WriteArray[0];
    S2MmChannelCfg->FrameStoreStartAddr[1]= &WriteArray[64*64];
    S2MmChannelCfg->FrameStoreStartAddr[2]= &WriteArray[64*64*2];
    S2MmChannelCfg->FrameStoreStartAddr[3]= &WriteArray[64*64*3];
    S2MmChannelCfg->FrameStoreStartAddr[4]= &WriteArray[64*64*4];
    S2MmChannelCfg->EnableVerticalFlip = 1;

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlAxiVdma_Hal_Init(&Vdma, 0, AlAxiVdma_EventHandler);

    AlAxiVdma_Hal_Start(Vdma, AL_VDMA_MM2S_CHANNEL, Mm2sChannelCfg);
    AlAxiVdma_Hal_Start(Vdma, AL_VDMA_S2MM_CHANNEL, S2MmChannelCfg);
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

    AlCache_InvalidateDcacheRange(&WriteArray[0],&WriteArray[64*64*5]);

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