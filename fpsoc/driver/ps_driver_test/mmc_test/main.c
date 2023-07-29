#include "al_mmc_test_config.h"
#include "al_errno.h"

static AL_U8 *WriteBuff = (AL_U8 *)AL_MMC_TEST_WR_BUFF_ADDR;
static AL_U8 *ReadBuff = (AL_U8 *)AL_MMC_TEST_RD_BUFF_ADDR;

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
    AL_U32 WordSize = BlockCnt * (Handle->Dev->CardInfo.BlkLen >> 2);
    while (WordSize--) {
        if (*SrcData == *DstData) {
            SrcData++;
            DstData++;
            continue;
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error word size is 0x%x\r\n", BlockCnt * (Handle->Dev->CardInfo.BlkLen >> 2) - WordSize);
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

    // AlMmc_Test_InitSrc(ReadBuff, AL_MMC_TEST_BLK_SIZE * BlkCnt, DataOffset + 1);
    memset(ReadBuff, 0, AL_MMC_TEST_BLK_SIZE * BlkCnt);

    for (AL_U32 i = 0; i < BlkCnt; i++) {
        AlMmc_Test_InitSrc(WriteBuff + AL_MMC_TEST_BLK_SIZE * i, AL_MMC_TEST_BLK_SIZE, i << 16);
    }
    // AlMmc_Test_InitSrc(WriteBuff, AL_MMC_TEST_BLK_SIZE * BlkCnt, DataOffset);

#ifdef CONFIG_PERFORMANCE_CALC
    if (BlkCnt > 1)
        AlMmc_Test_CalcStart(&PerCalc);
#endif
    Ret = AlMmc_Hal_WriteBlocked(Handle, WriteBuff, BlkOffset, BlkCnt, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal write %d block error 0x%x\r\n", BlkCnt, Ret);
        return Ret;
    }
#ifdef CONFIG_PERFORMANCE_CALC
    if (BlkCnt > 1)
        AlMmc_Test_CalcEnd(Handle, &PerCalc, BlkCnt);
#endif

#ifdef CONFIG_PERFORMANCE_CALC
    if (BlkCnt > 1)
        AlMmc_Test_CalcStart(&PerCalc);
#endif

    // AL_U32 _BlkCnt = BlkCnt;
    // AL_U8 *_ReadBuff = ReadBuff;
    // AL_U32 _BlkOffset = BlkOffset;
    // while (_BlkCnt > 8) {
    //     Ret = AlMmc_Hal_ReadBlocked(Handle, _ReadBuff, _BlkOffset, 8, Timeout);
    //     if (Ret != AL_OK) {
    //         AL_LOG(AL_LOG_LEVEL_ERROR, "Hal read %d block error 0x%x\r\n", _BlkCnt, Ret);
    //         return Ret;
    //     }
    //     _BlkCnt -= 8;
    //     _ReadBuff += 8 * AL_MMC_TEST_BLK_SIZE;
    //     _BlkOffset += 8;
    // }

    // Ret = AlMmc_Hal_ReadBlocked(Handle, _ReadBuff, _BlkOffset, _BlkCnt, Timeout);
    // if (Ret != AL_OK) {
    //     AL_LOG(AL_LOG_LEVEL_ERROR, "Hal read %d block error 0x%x\r\n", _BlkCnt, Ret);
    //     return Ret;
    // }

    Ret = AlMmc_Hal_ReadBlocked(Handle, ReadBuff, BlkOffset, BlkCnt, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal read %d block error 0x%x\r\n", BlkCnt, Ret);
        return Ret;
    }

#ifdef CONFIG_PERFORMANCE_CALC
    if (BlkCnt > 1)
        AlMmc_Test_CalcEnd(Handle, &PerCalc, BlkCnt);
#endif
    Ret = AlMmc_Test_DataCheck(Handle, WriteBuff, ReadBuff, BlkCnt);
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
    AL_MMC_HalStruct Handle;
    AL_MMC_InitStruct *InitConfig[AL_MMC_TEST_SD_CASE_NUM];
    AL_S8 *CaseName[AL_MMC_TEST_SD_CASE_NUM] = {"Non-DMA", "SDMA", "ADMA2"};

    InitConfig[0] = &SdNoDmaInit;
    InitConfig[1] = &SdSdmaInit;
    InitConfig[2] = &SdAdma2Init;

    for (AL_U32 i = 0; i < AL_MMC_TEST_SD_CASE_NUM; i++) {

        AL_LOG(AL_LOG_LEVEL_INFO, "----------MMC test %s mode start----------\r\n", CaseName[i]);

        Ret = AlMmc_Hal_Init(&Handle, InitConfig[i], AL_NULL, DevId);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal init error 0x%x\r\n", Ret);
            return ;
        }
        AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

        Ret = AlMmc_Test_BlockTest(&Handle, 'a', WriteBuff, ReadBuff, 0, AL_MMC_TEST_SINGLE_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Block test error\r\n");
        }

        Ret = AlMmc_Test_BlockTest(&Handle, 'b', WriteBuff, ReadBuff, 1, AL_MMC_TEST_SINGLE_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Block test error\r\n");
        }

        Ret = AlMmc_Test_BlockTest(&Handle, 'A', WriteBuff, ReadBuff, 2, AL_MMC_TEST_MULTI_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Block test error\r\n");
        }

        Ret = AlMmc_Test_BlockTest(&Handle, 'B', WriteBuff, ReadBuff, 3, AL_MMC_TEST_MULTI_BLK_CNT, Timeout);
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
    AL_MMC_HalStruct Handle;
    AL_MMC_InitStruct *InitConfig[AL_MMC_TEST_EMMC_CASE_NUM];
    AL_S8 *CaseName[AL_MMC_TEST_EMMC_CASE_NUM] = {"Non-DMA", "SDMA", "ADMA2"};
    AL_MMC_PerCalcStruct PerCalc;

    InitConfig[0] = &EmmcNoDmaInit;
    InitConfig[1] = &EmmcSdmaInit;
    InitConfig[2] = &EmmcAdma2Init;

    for (AL_U32 i = 0; i < AL_MMC_TEST_EMMC_CASE_NUM; i++) {

        AL_LOG(AL_LOG_LEVEL_INFO, "----------MMC test %s mode start----------\r\n", CaseName[i]);

        Ret = AlMmc_Hal_Init(&Handle, InitConfig[i], AL_NULL, DevId);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal init error 0x%x\r\n", Ret);
            return ;
        }
        AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

        Ret = AlMmc_Test_BlockTest(&Handle, 'a', WriteBuff, ReadBuff, 0, AL_MMC_TEST_SINGLE_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal write single block error 0x%x\r\n", Ret);
            return ;
        }

        Ret = AlMmc_Test_BlockTest(&Handle, 'b', WriteBuff, ReadBuff, 1, AL_MMC_TEST_SINGLE_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal read single block error 0x%x\r\n", Ret);
            return ;
        }

        Ret = AlMmc_Test_BlockTest(&Handle, 'c', WriteBuff, ReadBuff, 2, AL_MMC_TEST_MULTI_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Block test error\r\n");
        }

        Ret = AlMmc_Test_BlockTest(&Handle, 'd', WriteBuff, ReadBuff, 3, AL_MMC_TEST_MULTI_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Block test error\r\n");
        }

        AL_LOG(AL_LOG_LEVEL_INFO, "----------MMC test %s mode done----------\r\n", CaseName[i]);
    }
}

static AL_VOID AlMmc_Test_CalcStart(AL_MMC_PerCalcStruct *PerCalc)
{
    PerCalc->Start = AlSys_GetTimer();
}

static AL_VOID AlMmc_Test_CalcEnd(AL_MMC_HalStruct *Handle, AL_MMC_PerCalcStruct *PerCalc, AL_UINTPTR BlkCnt)
{
    PerCalc->End = AlSys_GetTimer();
    PerCalc->TimeInUs = (PerCalc->End - PerCalc->Start)/(AlSys_GetFreq()/1000000);
    PerCalc->DatInByte = BlkCnt * Handle->Dev->CardInfo.BlkLen;
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
    // AL_LOG(AL_LOG_LEVEL_DEBUG, "|-BytePerSec: %f MB/s\r\n", PerCalc->BytePerSec);
}
