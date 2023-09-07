/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_mmc_test_config.h"
#include "al_errno.h"

#ifdef USE_DDR
static AL_U8 *WriteBuff = (AL_U8 *)AL_MMC_TEST_WR_BUFF_ADDR;
static AL_U8 *ReadBuff = (AL_U8 *)AL_MMC_TEST_RD_BUFF_ADDR;
#else
static AL_U8 *WriteBuff[AL_MMC_TEST_BLK_NUM][AL_MMC_TEST_BLK_SIZE] CACHE_LINE_ALIGN;
static AL_U8 *ReadBuff[AL_MMC_TEST_BLK_NUM][AL_MMC_TEST_BLK_SIZE] CACHE_LINE_ALIGN;
#endif

static AL_VOID AlMmc_Test_InitSrc(AL_U8 *Addr, AL_U32 ByteSize, AL_U32 DataOffset);
static AL_S32 AlMmc_Test_DataCheck(AL_MMC_HalStruct *Handle, AL_U8 *SrcAddr, AL_U8 *DstAddr, AL_U32 BlockCnt);
static AL_VOID AlMmc_Test_Sd(AL_VOID);
static AL_VOID AlMmc_Test_Emmc(AL_VOID);
static AL_VOID AlMmc_Test_CalcStart(AL_MMC_PerCalcStruct *PerCalc);
static AL_VOID AlMmc_Test_CalcEnd(AL_MMC_HalStruct *Handle, AL_MMC_PerCalcStruct *PerCalc, AL_UINTPTR BlkCnt);
static AL_VOID AlMmc_Test_CalcDisplay(AL_MMC_PerCalcStruct *PerCalc);


AL_U32 main(AL_VOID)
{
#if CONFIG_SD_TEST
    AlMmc_Test_Sd();
#endif

#if CONFIG_EMMC_TEST
    AlMmc_Test_Emmc();
#endif

    while (1);

    return 0;
}

static AL_VOID AlMmc_Test_InitSrc(AL_U8 *Addr, AL_U32 ByteSize, AL_U32 DataOffset)
{
    AL_U32 *Word = (AL_U32 *)Addr;
    AL_U32 WordSize = ByteSize >> 2;
    for (AL_U32 i = 0; i < WordSize; i++) {
        *Word++ = (i + DataOffset);
    }
}

static AL_S32 AlMmc_Test_DataCheck(AL_MMC_HalStruct *Handle, AL_U8 *SrcAddr, AL_U8 *DstAddr, AL_U32 BlockCnt)
{
    AL_U32 *SrcData = (AL_U32 *)SrcAddr;
    AL_U32 *DstData = (AL_U32 *)DstAddr;
    AL_U32 WordSize = BlockCnt * (Handle->Dev.CardInfo.BlkLen >> 2);
    while (WordSize--) {
        if (*SrcData == *DstData) {
            SrcData++;
            DstData++;
            continue;
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error word size is 0x%x\r\n", BlockCnt * (Handle->Dev.CardInfo.BlkLen >> 2) - WordSize);
            return -1;
        }
    }

    return AL_OK;
}

static AL_S32 AlMmc_Test_BlockTest(AL_MMC_HalStruct *Handle, AL_U32 DataOffset, AL_U8 *WriteBuff, AL_U8 *ReadBuff, AL_U32 BlkOffset,
                                   AL_U32 BlkCnt, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_MMC_PerCalcStruct PerCalc;
    AL_U32 WriteBlkCnt = BlkCnt;
    AL_U32 ReadBlkCnt = BlkCnt;

    #ifndef USE_DDR
    if (WriteBlkCnt > AL_MMC_TEST_BLK_NUM) {
        WriteBlkCnt = AL_MMC_TEST_BLK_NUM;
    }
    if (ReadBlkCnt > AL_MMC_TEST_BLK_NUM) {
        ReadBlkCnt = AL_MMC_TEST_BLK_NUM;
    }
    #endif

    memset(ReadBuff, 0, AL_MMC_TEST_BLK_SIZE * ReadBlkCnt);

    for (AL_U32 i = 0; i < WriteBlkCnt; i++) {
        AlMmc_Test_InitSrc(WriteBuff + AL_MMC_TEST_BLK_SIZE * i, AL_MMC_TEST_BLK_SIZE, i << 16);
    }

#ifdef CONFIG_PERFORMANCE_CALC
    AlMmc_Test_CalcStart(&PerCalc);
#endif

    Ret = AlMmc_Hal_WriteBlocked(Handle, WriteBuff, BlkOffset, WriteBlkCnt, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal write %d block error 0x%x\r\n", WriteBlkCnt, Ret);
        return Ret;
    }

#ifdef CONFIG_PERFORMANCE_CALC
    AlMmc_Test_CalcEnd(Handle, &PerCalc, WriteBlkCnt);
#endif

#ifdef CONFIG_PERFORMANCE_CALC
    AlMmc_Test_CalcStart(&PerCalc);
#endif

    Ret = AlMmc_Hal_ReadBlocked(Handle, ReadBuff, BlkOffset, ReadBlkCnt, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal read %d block error 0x%x\r\n", ReadBlkCnt, Ret);
        return Ret;
    }

#ifdef CONFIG_PERFORMANCE_CALC
    AlMmc_Test_CalcEnd(Handle, &PerCalc, ReadBlkCnt);
#endif

    Ret = AlMmc_Test_DataCheck(Handle, WriteBuff, ReadBuff, ReadBlkCnt);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal block check error!\r\n");
        return Ret;
    }

    return Ret;
}

static AL_VOID AlMmc_Test_Sd(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U32 Timeout  = 1000;
    AL_U32 DevId    = 0;
    AL_MMC_HalStruct *Handle;
    AL_MMC_InitStruct *InitConfig[AL_MMC_TEST_SD_CASE_NUM];
    AL_S8 *CaseName[AL_MMC_TEST_SD_CASE_NUM] = {"Non-DMA", "SDMA", "ADMA2"};

    InitConfig[0] = &SdNoDmaInit;
    InitConfig[1] = &SdSdmaInit;
    InitConfig[2] = &SdAdma2Init;

    for (AL_U32 i = 0; i < AL_MMC_TEST_SD_CASE_NUM; i++) {

        AL_LOG(AL_LOG_LEVEL_INFO, "----------MMC test %s mode start----------\r\n", CaseName[i]);

        Ret = AlMmc_Hal_Init(&Handle, DevId, InitConfig[i], AL_NULL);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal init error 0x%x\r\n", Ret);
            return ;
        }
        AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

        Ret = AlMmc_Test_BlockTest(Handle, 'a', WriteBuff, ReadBuff, 0, AL_MMC_TEST_SINGLE_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Block test error\r\n");
        }

        Ret = AlMmc_Test_BlockTest(Handle, 'b', WriteBuff, ReadBuff, 0, AL_MMC_TEST_SINGLE_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Block test error\r\n");
        }

        Ret = AlMmc_Test_BlockTest(Handle, 'A', WriteBuff, ReadBuff, 0, AL_MMC_TEST_MULTI_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Block test error\r\n");
        }

        Ret = AlMmc_Test_BlockTest(Handle, 'B', WriteBuff, ReadBuff, 0, AL_MMC_TEST_MULTI_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Block test error\r\n");
        }

        AL_LOG(AL_LOG_LEVEL_INFO, "----------MMC test %s mode done----------\r\n", CaseName[i]);
    }
}

static AL_VOID AlMmc_Test_Emmc(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U32 Timeout  = 1000;
    AL_U32 DevId    = 0;
    AL_MMC_HalStruct *Handle;
    AL_MMC_InitStruct *InitConfig[AL_MMC_TEST_EMMC_CASE_NUM];
    AL_S8 *CaseName[AL_MMC_TEST_EMMC_CASE_NUM] = {"Non-DMA", "SDMA", "ADMA2"};
    AL_MMC_PerCalcStruct PerCalc;

    InitConfig[0] = &EmmcNoDmaInit;
    InitConfig[1] = &EmmcSdmaInit;
    InitConfig[2] = &EmmcAdma2Init;

    for (AL_U32 i = 0; i < AL_MMC_TEST_EMMC_CASE_NUM; i++) {

        AL_LOG(AL_LOG_LEVEL_INFO, "----------MMC test %s mode start----------\r\n", CaseName[i]);

        Ret = AlMmc_Hal_Init(&Handle, DevId, InitConfig[i], AL_NULL);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal init error 0x%x\r\n", Ret);
            return ;
        }
        AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

        Ret = AlMmc_Test_BlockTest(Handle, 'a', WriteBuff, ReadBuff, 0, AL_MMC_TEST_SINGLE_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal write single block error 0x%x\r\n", Ret);
            return ;
        }

        Ret = AlMmc_Test_BlockTest(Handle, 'b', WriteBuff, ReadBuff, 1, AL_MMC_TEST_SINGLE_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal read single block error 0x%x\r\n", Ret);
            return ;
        }

        Ret = AlMmc_Test_BlockTest(Handle, 'c', WriteBuff, ReadBuff, 2, AL_MMC_TEST_MULTI_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Block test error\r\n");
        }

        Ret = AlMmc_Test_BlockTest(Handle, 'd', WriteBuff, ReadBuff, 3, AL_MMC_TEST_MULTI_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Block test error\r\n");
        }

        AL_LOG(AL_LOG_LEVEL_INFO, "----------MMC test %s mode done----------\r\n", CaseName[i]);
    }
}

static AL_VOID AlMmc_Test_CalcStart(AL_MMC_PerCalcStruct *PerCalc)
{
    PerCalc->Start = AlSys_GetTimerTickCount();
}

static AL_VOID AlMmc_Test_CalcEnd(AL_MMC_HalStruct *Handle, AL_MMC_PerCalcStruct *PerCalc, AL_UINTPTR BlkCnt)
{
    PerCalc->End = AlSys_GetTimerTickCount();
    PerCalc->TimeInUs = (PerCalc->End - PerCalc->Start)/(AlSys_GetTimerFreq()/1000000);
    PerCalc->DatInByte = BlkCnt * Handle->Dev.CardInfo.BlkLen;
    PerCalc->BytePerSec = ((AL_DOUBLE)PerCalc->DatInByte / PerCalc->TimeInUs);

    AlMmc_Test_CalcDisplay(PerCalc);
}

static AL_VOID AlMmc_Test_CalcDisplay(AL_MMC_PerCalcStruct *PerCalc)
{
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Performance----------\r\n");
    AL_LOG(AL_LOG_LEVEL_DEBUG, "|-Start: %llu, 0x%llx\r\n", PerCalc->Start,  PerCalc->Start);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "|-End: %llu, 0x%llx\r\n", PerCalc->End,  PerCalc->End);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "|-DatInByte: %llu, 0x%llx\r\n", PerCalc->DatInByte,  PerCalc->DatInByte);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "|-TimeInUs: %llu, 0x%llx\r\n", PerCalc->TimeInUs,  PerCalc->TimeInUs);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "|-BytePerSec: %f MB/s\r\n", PerCalc->BytePerSec);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "-------------------------------\r\n");
}
