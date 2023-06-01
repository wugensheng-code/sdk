#include "al_mmc_test_config.h"
#include "al_errno.h"

static AL_U8 *WriteBuff = AL_MMC_TEST_WR_BUFF_ADDR;
static AL_U8 *ReadBuff = AL_MMC_TEST_RD_BUFF_ADDR;

static AL_VOID AlMmc_Test_InitSrc(AL_REG Addr, AL_U32 ByteSize, AL_U32 DataOffset);
static AL_S32 AlMmc_Test_DataCheck(AL_MMC_HalStruct *Handle, AL_REG SrcAddr, AL_REG DstAddr, AL_U32 ByteSize);
static AL_VOID AlMmc_Test_Sd(AL_VOID);
static AL_VOID AlMmc_Test_CalcStart(AL_MMC_PerCalcStruct *PerCalc);
static AL_VOID AlMmc_Test_CalcEnd(AL_MMC_HalStruct *Handle, AL_MMC_PerCalcStruct *PerCalc, AL_UINTPTR BlkCnt);
static AL_VOID AlMmc_Test_CalcDisplay(AL_MMC_PerCalcStruct *PerCalc);


AL_U32 main(AL_VOID)
{
#if CONFIG_SD_TEST
    AlMmc_Test_Sd();
#endif

    while (1);

    return 0;
}

static AL_VOID AlMmc_Test_InitSrc(AL_REG Addr, AL_U32 ByteSize, AL_U32 DataOffset)
{
    AL_U8 *Char = (AL_U32 *)Addr;
    for (AL_U32 i = 0; i < ByteSize; i++) {
        *Char++ = i + DataOffset;
    }
}

static AL_S32 AlMmc_Test_DataCheck(AL_MMC_HalStruct *Handle, AL_REG SrcAddr, AL_REG DstAddr, AL_U32 BlockCnt)
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
            return -1;
        }
    }

    return AL_OK;
}

static AL_VOID AlMmc_Test_Sd(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U32 Timeout  = 1000;
    AL_U32 DevId    = 0;
    AL_MMC_HalStruct Handle;
    AL_MMC_InitStruct *InitConfig[AL_MMC_TEST_SD_CASE_NUM];
    AL_S8 *CaseName[AL_MMC_TEST_SD_CASE_NUM] = {"Non-DMA", "SDMA", "ADMA2"};
    AL_MMC_PerCalcStruct PerCalc;

    InitConfig[0] = &SdNoDmaInit;
    InitConfig[1] = &SdSdmaInit;
    InitConfig[2] = &SdAdma2Init;

    for (AL_U32 i = 0; i < AL_MMC_TEST_SD_CASE_NUM; i++) {

        AL_LOG(AL_ERR_LEVEL_INFO, "----------MMC test %s mode start----------\r\n", CaseName[i]);

        Ret = AlMmc_Hal_Init(&Handle, InitConfig[i], AL_NULL, DevId);
        if (Ret != AL_OK) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Hal init error 0x%x\r\n", Ret);
            return ;
        }

        Ret = AlMmc_Hal_WriteBlocked(&Handle, WriteBuff, 0, AL_MMC_TEST_SINGLE_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Hal write single block error 0x%x\r\n", Ret);
            return ;
        }

        Ret = AlMmc_Hal_ReadBlocked(&Handle, ReadBuff, 0, AL_MMC_TEST_SINGLE_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Hal read single block error 0x%x\r\n", Ret);
            return ;
        }

        Ret = AlMmc_Test_DataCheck(&Handle, WriteBuff, ReadBuff, AL_MMC_TEST_SINGLE_BLK_CNT);
        if (Ret != AL_OK) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Hal single block check error!\r\n");
            return ;
        }
#ifdef CONFIG_PERFORMANCE_CALC
        AlMmc_Test_CalcStart(&PerCalc);
#endif
        Ret = AlMmc_Hal_WriteBlocked(&Handle, WriteBuff, 1, AL_MMC_TEST_MULTI_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Hal write multi block error 0x%x\r\n", Ret);
            return ;
        }
#ifdef CONFIG_PERFORMANCE_CALC
        AlMmc_Test_CalcEnd(&Handle, &PerCalc, AL_MMC_TEST_MULTI_BLK_CNT);
#endif
#ifdef CONFIG_PERFORMANCE_CALC
        AlMmc_Test_CalcStart(&PerCalc);
#endif
        Ret = AlMmc_Hal_ReadBlocked(&Handle, ReadBuff, 1, AL_MMC_TEST_MULTI_BLK_CNT, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Hal read multi block error 0x%x\r\n", Ret);
            return ;
        }
#ifdef CONFIG_PERFORMANCE_CALC
        AlMmc_Test_CalcEnd(&Handle, &PerCalc, AL_MMC_TEST_MULTI_BLK_CNT);
#endif
        Ret = AlMmc_Test_DataCheck(&Handle, WriteBuff, ReadBuff, AL_MMC_TEST_MULTI_BLK_CNT);
        if (Ret != AL_OK) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Hal single block check error!\r\n");
            return ;
        }

        AL_LOG(AL_ERR_LEVEL_INFO, "----------MMC test %s mode done----------\r\n", CaseName[i]);
    }
}

static AL_VOID AlMmc_Test_CalcStart(AL_MMC_PerCalcStruct *PerCalc)
{
    PerCalc->Start = get_SystickTimer();
}

static AL_VOID AlMmc_Test_CalcEnd(AL_MMC_HalStruct *Handle, AL_MMC_PerCalcStruct *PerCalc, AL_UINTPTR BlkCnt)
{
    PerCalc->End = get_SystickTimer();
    PerCalc->TimeInUs = get_Us(PerCalc->Start, PerCalc->End);
    PerCalc->DatInByte = BlkCnt * Handle->Dev->CardInfo.BlkLen;
    PerCalc->BytePerSec = ((AL_DOUBLE)PerCalc->DatInByte / PerCalc->TimeInUs);

    AlMmc_Test_CalcDisplay(PerCalc);
}

static AL_VOID AlMmc_Test_CalcDisplay(AL_MMC_PerCalcStruct *PerCalc)
{
    AL_LOG(AL_ERR_LEVEL_DEBUG, "----------Performance----------\r\n");
    AL_LOG(AL_ERR_LEVEL_DEBUG, "|-Start: %llu, 0x%llx\r\n", PerCalc->Start,  PerCalc->Start);
    AL_LOG(AL_ERR_LEVEL_DEBUG, "|-End: %llu, 0x%llx\r\n", PerCalc->End,  PerCalc->End);
    AL_LOG(AL_ERR_LEVEL_DEBUG, "|-DatInByte: %llu, 0x%llx\r\n", PerCalc->DatInByte,  PerCalc->DatInByte);
    AL_LOG(AL_ERR_LEVEL_DEBUG, "|-TimeInUs: %llu, 0x%llx\r\n", PerCalc->TimeInUs,  PerCalc->TimeInUs);
    AL_LOG(AL_ERR_LEVEL_DEBUG, "|-BytePerSec: %f MB/s\r\n", PerCalc->BytePerSec);
}
