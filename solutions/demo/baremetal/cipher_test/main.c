/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_cipher_test_config.h"
#include "al_cipher_hal.h"
#include "al_errno.h"
#include "string.h"

#define PL_INIT_DONE_TIMEOUT    (90*1000*1000)

#define PERFORMANCE_IMAGE_NUM   7
#define PERFORMANCE_CASE_NUM    16

#define TEST_COUNT_MAX          (1)

#ifdef ENABLE_MMU
#define AL_CIPHER_FLUSH_MEM(Start, End)         AlCache_FlushDcacheRange(Start, End)
#define AL_CIPHER_INVALIDATE_MEM(Start, End)    AlCache_InvalidateDcacheRange(Start, End)
#else
#define AL_CIPHER_FLUSH_MEM(Start, End)
#define AL_CIPHER_INVALIDATE_MEM(Start, End)
#endif

uint64_t PerformanceTime[PERFORMANCE_IMAGE_NUM][PERFORMANCE_CASE_NUM] = {0};

AL_U64 AlCipher_Test_GetCurTime(void)
{
    return AlSys_GetTimerTickCount();
}

AL_U64 AlCipher_Test_SwitchTimeToUs(AL_U64 Time)
{
    return Time * 1000000 / AlSys_GetTimerFreq();
}

AL_U64 AlCipher_Test_CalcTimeToUs(AL_U64 StartTime, AL_U64 EndTime)
{
    if (EndTime == 0) {
        return AlCipher_Test_SwitchTimeToUs(StartTime);
    } else {
        return AlCipher_Test_SwitchTimeToUs(EndTime - StartTime);
    }
}

AL_U32 AlCipher_Test_CheckPlInitDone(void)
{
// 	AL_U32 Ret = AL_OK;
// 	AL_U32 InitDone = 0;

//     MTIMER_OUT_CONDITION(PL_INIT_DONE_TIMEOUT, &Mtimer, InitDone != AL_CIPHER_CRP_CFG_STATE_MSK_PL2PS_INITN) {
//         InitDone = REG32(AL_CIPHER_CRP_CFG_STATE) & AL_CIPHER_CRP_CFG_STATE_MSK_PL2PS_INITN;
//     }

//     if (Mtimer_IsTimerOut(&Mtimer)) {
//         Ret = AL_SECURE_PL_INIT_TIMEOUT;
//         goto END;
//     }

// END:
// 	return Ret;
    return AL_OK;
}

static AL_U32 AlCipher_Test_PlInit(void)
{
    AL_U32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Trigger pl reset\r\n");
	REG32(AL_CIPHER_SYSCTRL_S_GLOBAL_SRSTN) = REG32(AL_CIPHER_SYSCTRL_S_GLOBAL_SRSTN) & (~AL_CIPHER_SYSCTRL_S_GLOBAL_SRSTN_MSK_GLB_PL_SRST);

	AL_LOG(AL_LOG_LEVEL_DEBUG, "Release pl reset\r\n");
	REG32(AL_CIPHER_SYSCTRL_S_GLOBAL_SRSTN) = REG32(AL_CIPHER_SYSCTRL_S_GLOBAL_SRSTN) | (AL_CIPHER_SYSCTRL_S_GLOBAL_SRSTN_MSK_GLB_PL_SRST);

	AL_LOG(AL_LOG_LEVEL_DEBUG, "Set PCAP not enable\r\n");
	REG32(AL_CIPHER_CSU_PCAP_ENABLE) = 0;

	AL_LOG(AL_LOG_LEVEL_DEBUG, "Set PCAP enable\r\n");
	REG32(AL_CIPHER_CSU_PCAP_ENABLE) = 1;

	// check pl init done;
	Ret = AlCipher_Test_CheckPlInitDone();
	if(Ret != AL_OK) {
		goto END;
	}

END:
    return Ret;
}

static AL_U32 AlCipher_Test_DataCheck(AL_CIPHER_ConfigUnion *Config)
{
    AL_U32 Ret = AL_OK;
    AL_U32 OutDataLenght = INPUT_DATA_SIZE_1K;
    AL_U32 *OutDataAddr = (AL_U32 *)OutputDataBuff;

    //encrypt or decrypt data check
    if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_AES256 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_ENC \
        && Config->Crypt.EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
        Ret = strncmp((const char *)OutDataAddr, (const char *)EncAesBuff, OutDataLenght);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"AES256 encrypt ecb data check error\r\n");
            goto END;
        }
    } else if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_SM4 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_ENC \
                && Config->Crypt.EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
        Ret = strncmp((const char *)OutDataAddr, (const char *)EncSm4Buff, OutDataLenght);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"SM4 encrypt ecb data check error\r\n");
            goto END;
        }
    } else if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_AES256 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_ENC \
        && Config->Crypt.EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
        Ret = strncmp((const char *)OutDataAddr, (const char *)EncAesCbcBuff, OutDataLenght);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"AES256 encrypt cbc data check error\r\n");
            goto END;
        }
    } else if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_SM4 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_ENC \
                && Config->Crypt.EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
        Ret = strncmp((const char *)OutDataAddr, (const char *)EncSm4CbcBuff, OutDataLenght);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"SM4 encrypt cbc data check error\r\n");
            goto END;
        }
    } else if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_AES256 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_DEC) {
        Ret = strncmp((const char *)OutDataAddr, (const char *)SrcDataBuff, OutDataLenght);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"AES256 decrypt data check error\r\n");
            goto END;
        }
    } else if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_SM4 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_DEC) {
        Ret = strncmp((const char *)OutDataAddr, (const char *)SrcDataBuff, OutDataLenght);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"SM4 decrypt data check error\r\n");
            goto END;
        }
    } else if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_NONE) {
        Ret = strncmp((const char *)OutDataAddr, (const char *)InputDataBuff, OutDataLenght);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"Dma only data check error\r\n");
            goto END;
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR,"None encrypt or decrypt\r\n");
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG,"Crypt or decrypt data check done\r\n");

    //hash data check
    if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_AES256 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_ENC \
        && Config->Crypt.HashMode == AL_CIPHER_OPS_HASH_SHA256 && Config->Crypt.EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
        Ret = strncmp((const char *)Config->Crypt.HashOut, (const char *)EncAesDataBuff, AL_CIPHER_HASH_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"AES256 encrypt ecb hash data check error\r\n");
            goto END;
        }
    } else if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_SM4 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_ENC \
        && Config->Crypt.HashMode == AL_CIPHER_OPS_HASH_SM3 && Config->Crypt.EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
        Ret = strncmp((const char *)Config->Crypt.HashOut, (const char *)EncSm4DataBuff, AL_CIPHER_HASH_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"SM4 encrypt hash ecb data check error\r\n");
            goto END;
        }
    } else if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_AES256 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_ENC \
        && Config->Crypt.HashMode == AL_CIPHER_OPS_HASH_SHA256 && Config->Crypt.EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
        Ret = strncmp((const char *)Config->Crypt.HashOut, (const char *)EncCbcAesDataBuff, AL_CIPHER_HASH_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"AES256 encrypt cbc hash data check error\r\n");
            goto END;
        }
    } else if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_SM4 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_ENC \
        && Config->Crypt.HashMode == AL_CIPHER_OPS_HASH_SM3 && Config->Crypt.EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
        Ret = strncmp((const char *)Config->Crypt.HashOut, (const char *)EncCbcSm4DataBuff, AL_CIPHER_HASH_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"SM4 encrypt cbc hash data check error\r\n");
            goto END;
        }
    }else if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_AES256 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_DEC \
        && Config->Crypt.HashMode == AL_CIPHER_OPS_HASH_SHA256) {
        Ret = strncmp((const char *)Config->Crypt.HashOut, (const char *)SrcAesHashBuff, AL_CIPHER_HASH_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"AES256 hash data check error\r\n");
            goto END;
        }
    } else if (Config->Crypt.CryptAlgm == AL_CIPHER_OPS_CRYPT_SM4 && Config->Crypt.EncDec == AL_CIPHER_CRYPT_DEC \
        && Config->Crypt.HashMode == AL_CIPHER_OPS_HASH_SM3) {
        Ret = strncmp((const char *)Config->Crypt.HashOut, (const char *)SrcSm3HashBuff, AL_CIPHER_HASH_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR,"SM3 hash data check error\r\n");
            goto END;
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR,"None hash\r\n");
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG,"Hash data check done\r\n");

END:
    return Ret;
}

AL_VOID AlCipher_Test_Dma_WholeBlockDdrBothInc(AL_CIPHER_HalStruct *Handle)
{
    AL_U32 Ret = AL_OK;
    AL_U32 CaseNum = 33;//sizeof(TestCfgBothIncData)/sizeof(TestCfgBothInc_t);
    TestCfgBothInc_t *Cfg = NULL;
    AL_U32 DataLength = INPUT_DATA_SIZE_1K;
    AL_CIPHER_ConfigUnion Config;
    AL_U32 Timeout = 1000;
    AL_CIPHER_MsgAckEnum AckVal = 0;

    for (AL_U32 i = 0; i < CaseNum; i++) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "-------AlCipher_Test_Dma_WholeBlockDdrBothInc Case Num %d-------\r\n", i);
        //cpy config
        Cfg = (TestCfgBothInc_t *)&TestCfgBothIncData[i];

        //modify bhdr key to user key for temp test
        if (Cfg->KeyMode == AL_CIPHER_KEY_BHDR) {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "BHDR KEY\r\n");
            // Cfg->KeyMode = OP_USER_KEY;
        } else {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "USER_KEY\r\n");
        }

        //cpy enc key and iv
        if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256) {
            AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 32);
            AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
            memcpy(KeyDataBuff, UserAesKey, 32);
            memcpy(IvDataBuff, UserAesIV, 16);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4) {
            AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 16);
            AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
            memcpy(KeyDataBuff, UserSm4Key, 16);
            memcpy(IvDataBuff, UserSm4IV, 16);
        }

        //cpy input data
        AL_CIPHER_INVALIDATE_MEM(InputDataBuff, InputDataBuff + DataLength);
        if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256 &&
            Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
            Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
            AL_CIPHER_INVALIDATE_MEM(EncAesBuff, EncAesBuff + DataLength);
            memcpy(InputDataBuff, EncAesBuff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
            AL_CIPHER_INVALIDATE_MEM(EncSm4Buff, EncSm4Buff + DataLength);
            memcpy(InputDataBuff, EncSm4Buff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
            AL_CIPHER_INVALIDATE_MEM(EncAesCbcBuff, EncAesCbcBuff + DataLength);
            memcpy(InputDataBuff, EncAesCbcBuff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
            AL_CIPHER_INVALIDATE_MEM(EncSm4CbcBuff, EncSm4CbcBuff + DataLength);
            memcpy(InputDataBuff, EncSm4CbcBuff, DataLength);
        } else {
            AL_CIPHER_INVALIDATE_MEM(SrcDataBuff, SrcDataBuff + DataLength);
            memcpy(InputDataBuff, SrcDataBuff, DataLength);
        }

        Config.Crypt.CryptAlgm      = Cfg->CryptMethod;
        Config.Crypt.EcbCbc         = Cfg->EcbCbc;
        Config.Crypt.EncDec         = Cfg->EncDec;
        Config.Crypt.HashMode       = Cfg->HashMode;
        Config.Crypt.KeySel         = Cfg->KeyMode;
        Config.Crypt.Iv             = IvDataBuff;
        Config.Crypt.Key            = KeyDataBuff;
        Config.Crypt.DmaIncMode     = Cfg->DmaIncMode;
        Config.Crypt.InputData      = (AL_U32 *)(AL_UINTPTR)Cfg->DmaSrcAddr;
        Config.Crypt.OutputData     = (AL_U32 *)(AL_UINTPTR)Cfg->DmaDstAddr;
        Config.Crypt.HashOut        = (AL_U32 *)(AL_UINTPTR)Cfg->HashOutAddr;
        Config.Crypt.DataLength     = DataLength;
        Config.Crypt.HashBlkMode    = Cfg->HashBlockMode;

        Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_DMA, &Config, Timeout);
        if (AL_CIPHER_IS_ACK_STATE(Ret)) {
            if (Ret != AL_CIPHER_ACK_DMA_DONE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt ack err: 0x%x\r\n", Ret);
                continue;
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt start block err: 0x%x\r\n", Ret);
            continue;
        }

        Ret = AlCipher_Test_DataCheck(&Config);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt data check err: 0x%x\r\n", Ret);
            continue;
        }
    }
}

AL_VOID AlCipher_Test_Dma_BlockDdrBothInc(AL_CIPHER_HalStruct *Handle)
{
    AL_U32 Ret = AL_OK;
    AL_U32 CaseNum = 16;//sizeof(TestCfgBothIncData)/sizeof(TestCfgBothInc_t);
    TestCfgBothInc_t *Cfg = NULL;
    AL_U32 DataLength = INPUT_DATA_SIZE_1K;
    AL_U32 FirstBlockLength = INPUT_DATA_SIZE_256B;
    AL_U32 LastBlockLength = INPUT_DATA_SIZE_256B;
    AL_U32 MidBlockLength = INPUT_DATA_SIZE_1K - FirstBlockLength - LastBlockLength;
    AL_CIPHER_ConfigUnion Config;
    AL_U32 Timeout = 1000;
    AL_CIPHER_MsgAckEnum AckVal = 0;

    for (AL_U32 i = 0; i < CaseNum; i++) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "-------AlCipher_Test_Dma_BlockDdrBothInc Case Num %d-------\r\n", i);
        //cpy config
        Cfg = (TestCfgBothInc_t *)&TestCfgBothIncData[i];

        //modify bhdr key to user key for temp test
        if (Cfg->KeyMode == AL_CIPHER_KEY_BHDR) {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "BHDR KEY\r\n");
            // Cfg->KeyMode = OP_USER_KEY;
        } else {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "USER_KEY\r\n");
        }

        //cpy enc key and iv
        if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256) {
            AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 32);
            AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
            memcpy(KeyDataBuff, UserAesKey, 32);
            memcpy(IvDataBuff, UserAesIV, 16);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4) {
            AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 16);
            AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
            memcpy(KeyDataBuff, UserSm4Key, 16);
            memcpy(IvDataBuff, UserSm4IV, 16);
        }

        //cpy input data
        AL_CIPHER_INVALIDATE_MEM(InputDataBuff, InputDataBuff + DataLength);
        if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256 &&
            Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
            Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
            AL_CIPHER_INVALIDATE_MEM(EncAesBuff, EncAesBuff + DataLength);
            memcpy(InputDataBuff, EncAesBuff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
            AL_CIPHER_INVALIDATE_MEM(EncSm4Buff, EncSm4Buff + DataLength);
            memcpy(InputDataBuff, EncSm4Buff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
            AL_CIPHER_INVALIDATE_MEM(EncAesCbcBuff, EncAesCbcBuff + DataLength);
            memcpy(InputDataBuff, EncAesCbcBuff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
            AL_CIPHER_INVALIDATE_MEM(EncSm4CbcBuff, EncSm4CbcBuff + DataLength);
            memcpy(InputDataBuff, EncSm4CbcBuff, DataLength);
        } else {
            AL_CIPHER_INVALIDATE_MEM(SrcDataBuff, SrcDataBuff + DataLength);
            memcpy(InputDataBuff, SrcDataBuff, DataLength);
        }

        Config.Crypt.CryptAlgm      = Cfg->CryptMethod;
        Config.Crypt.EcbCbc         = Cfg->EcbCbc;
        Config.Crypt.EncDec         = Cfg->EncDec;
        Config.Crypt.HashMode       = Cfg->HashMode;
        Config.Crypt.KeySel         = Cfg->KeyMode;
        Config.Crypt.Iv             = IvDataBuff;
        Config.Crypt.Key            = KeyDataBuff;
        Config.Crypt.DmaIncMode     = Cfg->DmaIncMode;
        Config.Crypt.InputData      = (AL_U32 *)(AL_UINTPTR)Cfg->DmaSrcAddr;
        Config.Crypt.OutputData     = (AL_U32 *)(AL_UINTPTR)Cfg->DmaDstAddr;
        Config.Crypt.HashOut        = (AL_U32 *)(AL_UINTPTR)Cfg->HashOutAddr;
        Config.Crypt.DataLength     = FirstBlockLength;
        Config.Crypt.HashBlkMode    = AL_CIPHER_BLK_FIRST;

        /* first block */
        Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_DMA, &Config, Timeout);
        if (AL_CIPHER_IS_ACK_STATE(Ret)) {
            if (Ret != AL_CIPHER_ACK_DMA_DONE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt first block ack err: 0x%x\r\n", Ret);
                continue;
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt start first block err: 0x%x\r\n", Ret);
            continue;
        }

        Config.Crypt.InputData      += (FirstBlockLength >> 2);
        Config.Crypt.OutputData     += (FirstBlockLength >> 2);
        Config.Crypt.DataLength     =  MidBlockLength;
        Config.Crypt.HashBlkMode    =  AL_CIPHER_BLK_MID;

        /*mid block*/
        Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_DMA, &Config, Timeout);
        if (AL_CIPHER_IS_ACK_STATE(Ret)) {
            if (Ret != AL_CIPHER_ACK_DMA_DONE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt mid block ack err: 0x%x\r\n", Ret);
                continue;
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt start mid block err: 0x%x\r\n", Ret);
            continue;
        }

        Config.Crypt.InputData      += (MidBlockLength >> 2);
        Config.Crypt.OutputData     += (MidBlockLength >> 2);
        Config.Crypt.DataLength     =  LastBlockLength;
        Config.Crypt.HashBlkMode    =  AL_CIPHER_BLK_LAST;

        /*mid block*/
        Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_DMA, &Config, Timeout);
        if (AL_CIPHER_IS_ACK_STATE(Ret)) {
            if (Ret != AL_CIPHER_ACK_DMA_DONE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt last block ack err: 0x%x\r\n", Ret);
                continue;
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt start last block err: 0x%x\r\n", Ret);
            continue;
        }

        Ret = AlCipher_Test_DataCheck(&Config);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt data check err: 0x%x\r\n", Ret);
            continue;
        }
    }
}


AL_VOID AlCipher_Test_Dma_WholeBlockPcap(AL_CIPHER_HalStruct *Handle)
{
    AL_U32 Ret = AL_OK;
    AL_U32 CaseNum = 17;//sizeof(TestCfgBothIncData)/sizeof(TestCfgBothInc_t);
    TestCfgBothInc_t *Cfg = NULL;
    AL_U32 DataLength = INPUT_DATA_SIZE_1K;
    AL_CIPHER_ConfigUnion Config;
    AL_U32 Timeout = 1000;

    for (AL_U32 i = 0; i < CaseNum; i++) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "-------AlCipher_Test_Dma_WholeBlockPcap Case Num %d-------\r\n", i);
        //cpy config
        Cfg = (TestCfgBothInc_t *)&TestCfgSrcIncData[i];

        //pl init
        Ret = AlCipher_Test_PlInit();
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "PL Init Error\r\n");
        }

        //cpy enc key and iv
        if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256) {
            AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 32);
            AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
            memcpy(KeyDataBuff, UserAesKey, 32);
            memcpy(IvDataBuff, UserAesIV, 16);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4) {
            AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 16);
            AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
            memcpy(KeyDataBuff, UserSm4Key, 16);
            memcpy(IvDataBuff, UserSm4IV, 16);
        }

        //cpy input data
        AL_CIPHER_INVALIDATE_MEM(InputDataBuff, InputDataBuff + DataLength);
        if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256 &&
            Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
            Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
            AL_CIPHER_INVALIDATE_MEM(EncAesBuff, EncAesBuff + DataLength);
            memcpy(InputDataBuff, EncAesBuff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
            AL_CIPHER_INVALIDATE_MEM(EncSm4Buff, EncSm4Buff + DataLength);
            memcpy(InputDataBuff, EncSm4Buff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
            AL_CIPHER_INVALIDATE_MEM(EncAesCbcBuff, EncAesCbcBuff + DataLength);
            memcpy(InputDataBuff, EncAesCbcBuff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
            AL_CIPHER_INVALIDATE_MEM(EncSm4CbcBuff, EncSm4CbcBuff + DataLength);
            memcpy(InputDataBuff, EncSm4CbcBuff, DataLength);
        } else {
            AL_CIPHER_INVALIDATE_MEM(SrcDataBuff, SrcDataBuff + DataLength);
            memcpy(InputDataBuff, SrcDataBuff, DataLength);
        }

        Config.Crypt.CryptAlgm      = Cfg->CryptMethod;
        Config.Crypt.EcbCbc         = Cfg->EcbCbc;
        Config.Crypt.EncDec         = Cfg->EncDec;
        Config.Crypt.HashMode       = Cfg->HashMode;
        Config.Crypt.KeySel         = Cfg->KeyMode;
        Config.Crypt.Iv             = IvDataBuff;
        Config.Crypt.Key            = KeyDataBuff;
        Config.Crypt.DmaIncMode     = Cfg->DmaIncMode;
        Config.Crypt.InputData      = (AL_U32 *)(AL_UINTPTR)Cfg->DmaSrcAddr;
        Config.Crypt.OutputData     = (AL_U32 *)(AL_UINTPTR)Cfg->DmaDstAddr;
        Config.Crypt.HashOut        = (AL_U32 *)(AL_UINTPTR)Cfg->HashOutAddr;
        Config.Crypt.DataLength     = DataLength;
        Config.Crypt.HashBlkMode    = Cfg->HashBlockMode;

        Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_DMA, &Config, Timeout);
        if (AL_CIPHER_IS_ACK_STATE(Ret)) {
            if (Ret != AL_CIPHER_ACK_DMA_DONE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt last block ack err: 0x%x\r\n", Ret);
                continue;
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt start last block err: 0x%x\r\n", Ret);
            continue;
        }
    }
}

AL_VOID AlCipher_Test_Dma_BlockPcap(AL_CIPHER_HalStruct *Handle)
{
    AL_U32 Ret = AL_OK;
    AL_U32 CaseNum = 8;//sizeof(TestCfgBothIncData)/sizeof(TestCfgBothInc_t);
    TestCfgBothInc_t *Cfg = NULL;
    AL_U32 DataLength = INPUT_DATA_SIZE_1K;
    AL_U32 FirstBlockLength = INPUT_DATA_SIZE_128B;
    AL_U32 LastBlockLength = INPUT_DATA_SIZE_128B;
    AL_U32 MidBlockLength = INPUT_DATA_SIZE_1K - FirstBlockLength - LastBlockLength;
    AL_CIPHER_ConfigUnion Config;
    AL_U32 Timeout = 1000;

    for (AL_U32 i = 0; i < CaseNum; i++) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "-------AlCipher_Test_Dma_BlockPcap Case Num %d-------\r\n", i);
        //cpy config
        Cfg = (TestCfgBothInc_t *)&TestCfgSrcIncData[i];

        //pl init
        Ret = AlCipher_Test_PlInit();
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "PL Init Error\r\n");
        }

        //modify bhdr key to user key for temp test
        if (Cfg->KeyMode == AL_CIPHER_KEY_BHDR) {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "BHDR KEY\r\n");
            // Cfg->KeyMode = OP_USER_KEY;
        } else {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "USER_KEY\r\n");
        }

        //cpy enc key and iv
        if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256) {
            AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 32);
            AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
            memcpy(KeyDataBuff, UserAesKey, 32);
            memcpy(IvDataBuff, UserAesIV, 16);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4) {
            AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 16);
            AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
            memcpy(KeyDataBuff, UserSm4Key, 16);
            memcpy(IvDataBuff, UserSm4IV, 16);
        }

        //cpy input data
        AL_CIPHER_INVALIDATE_MEM(InputDataBuff, InputDataBuff + DataLength);
        if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256 &&
            Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
            Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
            AL_CIPHER_INVALIDATE_MEM(EncAesBuff, EncAesBuff + DataLength);
            memcpy(InputDataBuff, EncAesBuff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
            AL_CIPHER_INVALIDATE_MEM(EncSm4Buff, EncSm4Buff + DataLength);
            memcpy(InputDataBuff, EncSm4Buff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
            AL_CIPHER_INVALIDATE_MEM(EncAesCbcBuff, EncAesCbcBuff + DataLength);
            memcpy(InputDataBuff, EncAesCbcBuff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4 &&
                   Cfg->EncDec == AL_CIPHER_CRYPT_DEC &&
                   Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
            AL_CIPHER_INVALIDATE_MEM(EncSm4CbcBuff, EncSm4CbcBuff + DataLength);
            memcpy(InputDataBuff, EncSm4CbcBuff, DataLength);
        } else {
            AL_CIPHER_INVALIDATE_MEM(SrcDataBuff, SrcDataBuff + DataLength);
            memcpy(InputDataBuff, SrcDataBuff, DataLength);
        }

        Config.Crypt.CryptAlgm      = Cfg->CryptMethod;
        Config.Crypt.EcbCbc         = Cfg->EcbCbc;
        Config.Crypt.EncDec         = Cfg->EncDec;
        Config.Crypt.HashMode       = Cfg->HashMode;
        Config.Crypt.KeySel         = Cfg->KeyMode;
        Config.Crypt.Iv             = IvDataBuff;
        Config.Crypt.Key            = KeyDataBuff;
        Config.Crypt.DmaIncMode     = Cfg->DmaIncMode;
        Config.Crypt.InputData      = (AL_U32 *)(AL_UINTPTR)Cfg->DmaSrcAddr;
        Config.Crypt.OutputData     = (AL_U32 *)(AL_UINTPTR)Cfg->DmaDstAddr;
        Config.Crypt.HashOut        = (AL_U32 *)(AL_UINTPTR)Cfg->HashOutAddr;
        Config.Crypt.DataLength     = FirstBlockLength;
        Config.Crypt.HashBlkMode    = AL_CIPHER_BLK_FIRST;

        /* first block */
        Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_DMA, &Config, Timeout);
        if (AL_CIPHER_IS_ACK_STATE(Ret)) {
            if (Ret != AL_CIPHER_ACK_DMA_DONE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt first block ack err: 0x%x\r\n", Ret);
                continue;
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt start first block err: 0x%x\r\n", Ret);
            continue;
        }

        Config.Crypt.InputData      += (FirstBlockLength >> 2);
        Config.Crypt.DataLength     =  MidBlockLength;
        Config.Crypt.HashBlkMode    =  AL_CIPHER_BLK_MID;

        /*mid block*/
        Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_DMA, &Config, Timeout);
        if (AL_CIPHER_IS_ACK_STATE(Ret)) {
            if (Ret != AL_CIPHER_ACK_DMA_DONE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt mid block ack err: 0x%x\r\n", Ret);
                continue;
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt start mid block err: 0x%x\r\n", Ret);
            continue;
        }

        Config.Crypt.InputData      += (MidBlockLength >> 2);
        Config.Crypt.DataLength     =  LastBlockLength;
        Config.Crypt.HashBlkMode    =  AL_CIPHER_BLK_LAST;

        /*last block*/
        Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_DMA, &Config, Timeout);
        if (AL_CIPHER_IS_ACK_STATE(Ret)) {
            if (Ret != AL_CIPHER_ACK_DMA_DONE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt last block ack err: 0x%x\r\n", Ret);
                continue;
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt start last block err: 0x%x\r\n", Ret);
            continue;
        }
    }
}

AL_VOID AlCipher_Test_Dma_Profermance(AL_CIPHER_HalStruct *Handle)
{
    AL_U32 Ret = AL_OK;
    AL_U32 CaseNum = PERFORMANCE_CASE_NUM;//sizeof(TestCfgBothIncData)/sizeof(TestCfgBothInc_t);
    AL_U32 ImageNum = PERFORMANCE_IMAGE_NUM;
    TestCfgBothInc_t *Cfg = NULL;
    AL_U32 DataLength = PerformaceImageSize[0];
    AL_U32 InputDataAddr = PerformaceImageAddr[0];
    AL_CIPHER_ConfigUnion Config;
    AL_U32 Timeout = 1000;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "-------AlCipher_Test_Dma_Profermance-------\r\n");

    for (AL_U32 index = 0; index < ImageNum; index++) {
        DataLength = PerformaceImageSize[index];
        InputDataAddr = PerformaceImageAddr[index];
        AL_LOG(AL_LOG_LEVEL_DEBUG, "-----Image Num %d, Data Length 0x%x, Input Data Addr 0x%x-----\r\n", index, DataLength, InputDataAddr);

        for (AL_U32 i = 0; i < CaseNum; i++) {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "Image Num %d, Case Num %d\r\n", index, i);
            //cpy config
            Cfg = (TestCfgBothInc_t *)&TestPerformanceData[i];

            //cpy enc key and iv
            if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256) {
                AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 32);
                AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
                memcpy(KeyDataBuff, UserAesKey, 32);
                memcpy(IvDataBuff, UserAesIV, 16);
            } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4) {
                AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 16);
                AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
                memcpy(KeyDataBuff, UserSm4Key, 16);
                memcpy(IvDataBuff, UserSm4IV, 16);
            }

            //cpy input data
            memcpy(InputDataBuff, (AL_U32 *)(AL_UINTPTR)InputDataAddr, DataLength);

            Config.Crypt.CryptAlgm      = Cfg->CryptMethod;
            Config.Crypt.EcbCbc         = Cfg->EcbCbc;
            Config.Crypt.EncDec         = Cfg->EncDec;
            Config.Crypt.HashMode       = Cfg->HashMode;
            Config.Crypt.KeySel         = Cfg->KeyMode;
            Config.Crypt.Iv             = IvDataBuff;
            Config.Crypt.Key            = KeyDataBuff;
            Config.Crypt.DmaIncMode     = Cfg->DmaIncMode;
            Config.Crypt.InputData      = (AL_U32 *)(AL_UINTPTR)Cfg->DmaSrcAddr;
            Config.Crypt.OutputData     = (AL_U32 *)(AL_UINTPTR)Cfg->DmaDstAddr;
            Config.Crypt.HashOut        = (AL_U32 *)(AL_UINTPTR)Cfg->HashOutAddr;
            Config.Crypt.DataLength     = DataLength;
            Config.Crypt.HashBlkMode    = Cfg->HashBlockMode;

            PerformanceTime[index][i] = AlCipher_Test_GetCurTime();

            Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_DMA, &Config, Timeout);
            if (AL_CIPHER_IS_ACK_STATE(Ret)) {
                if (Ret != AL_CIPHER_ACK_DMA_DONE) {
                    AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt last block ack err: 0x%x\r\n", Ret);
                    continue;
                }
            } else {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt start last block err: 0x%x\r\n", Ret);
                continue;
            }

            PerformanceTime[index][i] = AlCipher_Test_GetCurTime() - PerformanceTime[index][i];
            PerformanceTime[index][i] = AlCipher_Test_CalcTimeToUs(PerformanceTime[index][i], 0);
            AL_LOG(AL_LOG_LEVEL_DEBUG, "PerformanceTime %llu\r\n", PerformanceTime[index][i]);
        }
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "-----PerformanceTime-----\r\n");
    AL_LOG(AL_LOG_LEVEL_DEBUG, "    1k    4k    8k    16k    32k    64k    128k\r\n");
    for (AL_U32 i = 0; i < PERFORMANCE_CASE_NUM; i++) {
        for (AL_U32 j = 0; j < PERFORMANCE_IMAGE_NUM; j++) {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "%llu\t", PerformanceTime[j][i]);
        }
        AL_LOG(AL_LOG_LEVEL_DEBUG, "\r\n");
    }
}

AL_VOID AlCipher_Test_Dma_Error(AL_CIPHER_HalStruct *Handle)
{
    AL_U32 Ret = AL_OK;
    AL_U32 CaseNum = 12;//sizeof(TestCfgBothIncData)/sizeof(TestCfgBothInc_t);
    TestCfgBothInc_t *Cfg = NULL;
    AL_U32 DataLength = INPUT_DATA_SIZE_1K;
    AL_CIPHER_ConfigUnion Config;
    AL_U32 Timeout = 1000;

    for (AL_U32 i = 0; i < CaseNum; i++) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "-------AlCipher_Test_Dma_Error Case Num %d-------\r\n", i);
        //cpy config
        Cfg = (TestCfgBothInc_t *)&TestCfgDmaErrorBranch[i];

        //modify bhdr key to user key for temp test
        if (Cfg->KeyMode == AL_CIPHER_KEY_BHDR) {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "BHDR KEY\r\n");
            // Cfg->KeyMode = OP_USER_KEY;
        } else {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "USER_KEY\r\n");
        }

        //cpy enc key and iv
        if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256) {
            AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 32);
            AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
            memcpy(KeyDataBuff, UserAesKey, 32);
            memcpy(IvDataBuff, UserAesIV, 16);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4) {
            AL_CIPHER_INVALIDATE_MEM(KeyDataBuff, KeyDataBuff + 16);
            AL_CIPHER_INVALIDATE_MEM(IvDataBuff, IvDataBuff + 16);
            memcpy(KeyDataBuff, UserSm4Key, 16);
            memcpy(IvDataBuff, UserSm4IV, 16);
        }

        //cpy input data
        if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256 && Cfg->EncDec == AL_CIPHER_CRYPT_DEC && Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
            memcpy(InputDataBuff, EncAesBuff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4 && Cfg->EncDec == AL_CIPHER_CRYPT_DEC && Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_ECB) {
            memcpy(InputDataBuff, EncSm4Buff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_AES256 && Cfg->EncDec == AL_CIPHER_CRYPT_DEC && Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
            memcpy(InputDataBuff, EncAesCbcBuff, DataLength);
        } else if (Cfg->CryptMethod == AL_CIPHER_OPS_CRYPT_SM4 && Cfg->EncDec == AL_CIPHER_CRYPT_DEC && Cfg->EcbCbc == AL_CIPHER_CRYPT_MODE_CBC) {
            memcpy(InputDataBuff, EncSm4CbcBuff, DataLength);
        } else {
            memcpy(InputDataBuff, SrcDataBuff, DataLength);
        }

        Config.Crypt.CryptAlgm      = Cfg->CryptMethod;
        Config.Crypt.EcbCbc         = Cfg->EcbCbc;
        Config.Crypt.EncDec         = Cfg->EncDec;
        Config.Crypt.HashMode       = Cfg->HashMode;
        Config.Crypt.KeySel         = Cfg->KeyMode;
        Config.Crypt.Iv             = IvDataBuff;
        Config.Crypt.Key            = KeyDataBuff;
        Config.Crypt.DmaIncMode     = Cfg->DmaIncMode;
        Config.Crypt.InputData      = (AL_U32 *)(AL_UINTPTR)Cfg->DmaSrcAddr;
        Config.Crypt.OutputData     = (AL_U32 *)(AL_UINTPTR)Cfg->DmaDstAddr;
        Config.Crypt.HashOut        = (AL_U32 *)(AL_UINTPTR)Cfg->HashOutAddr;
        Config.Crypt.DataLength     = DataLength;
        Config.Crypt.HashBlkMode    = Cfg->HashBlockMode;

        Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_DMA, &Config, Timeout);
        if (AL_CIPHER_IS_ACK_STATE(Ret)) {
            if (Ret != AL_CIPHER_ACK_DMA_DONE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt last block ack err: 0x%x\r\n", Ret);
                continue;
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Crypt start last block err: 0x%x\r\n", Ret);
            continue;
        }

        AL_LOG(AL_LOG_LEVEL_DEBUG, "-------TestDma_Error Case Num %d Done-------\r\n", i);
    }
}

AL_VOID AlCipher_Test_Auth(AL_CIPHER_HalStruct *Handle)
{
    AL_U32 Ret = AL_OK;
    AL_CIPHER_ConfigUnion Config;
    AL_U32 Timeout = 1000;
    AL_CIPHER_MsgAckEnum AckVal = 0;

    /********************************************************auth ECC256*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "Auth Verify ECC256\r\n");

    Config.Auth.AuthMode    = AL_CIPHER_OPS_AUTH_ECC256;
    Config.Auth.PubKey      = EccPpk;
    Config.Auth.Digest      = EccSpkHash;
    Config.Auth.Signature   = EccSpkSig;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_AUTH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_AUTH_PASS) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Auth ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Auth start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Auth Verify ECC256 Done\r\n");

    /********************************************************auth SM2*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "Auth Verify SM2\r\n");

    Config.Auth.AuthMode    = AL_CIPHER_OPS_AUTH_SM2;
    Config.Auth.PubKey      = Sm2Ppk;
    Config.Auth.Digest      = Sm2SpkHash;
    Config.Auth.Signature   = Sm2SpkSig;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_AUTH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_AUTH_PASS) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Auth ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Auth start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Auth Verify SM2 Done\r\n");

    /********************************************************auth none : error branch*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "Auth None\r\n");

    Config.Auth.AuthMode    = AL_CIPHER_OPS_CRYPT_NONE;
    Config.Auth.PubKey      = Sm2Ppk;
    Config.Auth.Digest      = Sm2SpkHash;
    Config.Auth.Signature   = Sm2SpkSig;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_AUTH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret == AL_CIPHER_ACK_CMD_ERR) {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "Auth None Done\r\n");
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Auth None Error 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Auth start block err: 0x%x\r\n", Ret);
    }

    /********************************************************auth SM2*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "Auth Verify SM2 Fail Test\r\n");

    AL_U32 tempSm2SpkHash[32] = {0};
    memcpy(&tempSm2SpkHash[4], &Sm2SpkHash[4], 28);

    Config.Auth.AuthMode    = AL_CIPHER_OPS_AUTH_SM2;
    Config.Auth.PubKey      = Sm2Ppk;
    Config.Auth.Digest      = tempSm2SpkHash;
    Config.Auth.Signature   = Sm2SpkSig;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_AUTH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_AUTH_FAIL) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Auth ack err: 0x%x\r\n", Ret);
        } else {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "Auth ack fail done\r\n");
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Auth start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Auth Verify SM2 Fail Test Done\r\n");
}

AL_VOID AlCipher_Test_Hash(AL_CIPHER_HalStruct *Handle)
{
    AL_U32 Ret = AL_OK;
    AL_U32 DataLength = INPUT_DATA_SIZE_1K;
    AL_U32 FirstBlockLength = INPUT_DATA_SIZE_128B;
    AL_U32 LastBlockLength = INPUT_DATA_SIZE_128B;
    AL_U32 MidBlockLength = DataLength - FirstBlockLength - LastBlockLength;
    AL_U32 BlockInputAddr = (AL_UINTPTR)InputDataBuff;
    AL_CIPHER_ConfigUnion Config;
    AL_U32 Timeout = 1000;
    AL_CIPHER_MsgAckEnum AckVal = 0;

    AL_CIPHER_INVALIDATE_MEM(InputDataBuff, InputDataBuff + DataLength);
    AL_CIPHER_INVALIDATE_MEM(SrcDataBuff, SrcDataBuff + DataLength);
    memcpy(InputDataBuff, SrcDataBuff, DataLength);
    /**************************************************************************sha256 whole block*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "-----Hash Sha256 Whole block!-----\r\n");

    Config.Hash.HashMode    = AL_CIPHER_OPS_HASH_SHA256;
    Config.Hash.HashBlkMode = AL_CIPHER_BLK_WHOLE;
    Config.Hash.InputData   = InputDataBuff;
    Config.Hash.HashOut     = HashDataBuff;
    Config.Hash.DataLength  = DataLength;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_HASH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_HASH_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hash ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash start block err: 0x%x\r\n", Ret);
    }

    Ret = strncmp((const char *)HashDataBuff, (const char *)SrcAesHashBuff, AL_CIPHER_HASH_SIZE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash Data Check error 0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG, "-----Hash Sha256 Whole block Done!-----\r\n");

    /**************************************************************************sha256 block*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "-----Hash Sha256 block!-----\r\n");

    Config.Hash.HashMode    = AL_CIPHER_OPS_HASH_SHA256;
    Config.Hash.HashBlkMode = AL_CIPHER_BLK_FIRST;
    Config.Hash.InputData   = InputDataBuff;
    Config.Hash.HashOut     = HashDataBuff;
    Config.Hash.DataLength  = FirstBlockLength;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_HASH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_HASH_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hash first block ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash start first block err: 0x%x\r\n", Ret);
    }

    Config.Hash.HashBlkMode = AL_CIPHER_BLK_MID;
    Config.Hash.InputData   += (FirstBlockLength >> 2);
    Config.Hash.DataLength  = MidBlockLength;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_HASH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_HASH_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hash mid block ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash start mid block err: 0x%x\r\n", Ret);
    }

    Config.Hash.HashBlkMode = AL_CIPHER_BLK_LAST;
    Config.Hash.InputData   += (MidBlockLength >> 2);
    Config.Hash.DataLength  = LastBlockLength;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_HASH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_HASH_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hash last block ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash start last block err: 0x%x\r\n", Ret);
    }

    Ret = strncmp((const char *)HashDataBuff, (const char *)SrcAesHashBuff, AL_CIPHER_HASH_SIZE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash Data Check error 0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG, "-----Hash Sha256 block Done!-----\r\n");

    /**************************************************************************sm3 whole block*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "-----Hash Sm3 Whole block!-----\r\n");

    Config.Hash.HashMode    = AL_CIPHER_OPS_HASH_SM3;
    Config.Hash.HashBlkMode = AL_CIPHER_BLK_WHOLE;
    Config.Hash.InputData   = InputDataBuff;
    Config.Hash.HashOut     = HashDataBuff;
    Config.Hash.DataLength  = DataLength;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_HASH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_HASH_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hash block ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash start block err: 0x%x\r\n", Ret);
    }

    Ret = strncmp((const char *)HashDataBuff, (const char *)SrcSm3HashBuff, AL_CIPHER_HASH_SIZE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash Data Check error 0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG, "-----Hash Sm3 Whole block Done!-----\r\n");
    /**************************************************************************sm3 block*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "-----Hash Sm3 block!-----\r\n");

    Config.Hash.HashMode    = AL_CIPHER_OPS_HASH_SM3;
    Config.Hash.HashBlkMode = AL_CIPHER_BLK_FIRST;
    Config.Hash.InputData   = InputDataBuff;
    Config.Hash.HashOut     = HashDataBuff;
    Config.Hash.DataLength  = FirstBlockLength;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_HASH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_HASH_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hash first block ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash start first block err: 0x%x\r\n", Ret);
    }

    Config.Hash.HashBlkMode = AL_CIPHER_BLK_MID;
    Config.Hash.InputData   += (FirstBlockLength >> 2);
    Config.Hash.DataLength  = MidBlockLength;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_HASH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_HASH_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hash mid block ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash start mid block err: 0x%x\r\n", Ret);
    }

    Config.Hash.HashBlkMode = AL_CIPHER_BLK_LAST;
    Config.Hash.InputData   += (MidBlockLength >> 2);
    Config.Hash.DataLength  = LastBlockLength;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_HASH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_HASH_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hash last block ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash start last block err: 0x%x\r\n", Ret);
    }

    Ret = strncmp((const char *)HashDataBuff, (const char *)SrcSm3HashBuff, AL_CIPHER_HASH_SIZE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash Data Check error 0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG, "-----Hash Sm3 block Done!-----\r\n");
    /**************************************************************************hash error branch*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "-----Hash error branch!-----\r\n");

    Config.Hash.HashMode = AL_CIPHER_OPS_HASH_NONE;
    Config.Hash.HashBlkMode = AL_CIPHER_BLK_WHOLE;
    Config.Hash.InputData   = InputDataBuff;
    Config.Hash.HashOut     = HashDataBuff;
    Config.Hash.DataLength  = DataLength;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_HASH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_CMD_ERR) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hash block ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hash start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "-----Hash error branch Done!-----\r\n");
}

AL_VOID AlCipher_Test_Sign(AL_CIPHER_HalStruct *Handle)
{
    AL_U32 Ret = AL_OK;
    AL_CIPHER_ConfigUnion Config;
    AL_U32 Timeout = 1000;
    AL_CIPHER_MsgAckEnum AckVal = 0;

    /********************************************************Sign ECC256*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Sign ECC256!----------\r\n");

    Config.Sign.SignMode    = AL_CIPHER_OPS_AUTH_ECC256;
    Config.Sign.PriKey      = EccPpk;
    Config.Sign.Digest      = EccSpkHash;
    Config.Sign.Signature   = SignEccSign;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_SIGN, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_SIGN_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Sign ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Sign start block err: 0x%x\r\n", Ret);
    }

    Ret = strncmp((const char *)SignEccSign, (const char *)EccSpkSig, PPK_SPK_SIGN_LENGTH);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Sign check error: 0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Sign ECC256 Done!----------\r\n");
    /********************************************************Sign SM2*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Sign SM2!----------\r\n");

    Config.Sign.SignMode    = AL_CIPHER_OPS_AUTH_SM2;
    Config.Sign.PriKey      = Sm2Ppk;
    Config.Sign.Digest      = Sm2SpkHash;
    Config.Sign.Signature   = SignSm2Sign;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_SIGN, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_SIGN_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Sign ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Sign start block err: 0x%x\r\n", Ret);
    }

    Ret = strncmp((const char *)SignSm2Sign, (const char *)Sm2SpkSig, PPK_SPK_SIGN_LENGTH);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Sign check error: 0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Sign SM2 Done!----------\r\n");
    /********************************************************Sign none : error branch*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Sign Default!----------\r\n");

    Config.Sign.SignMode    = AL_CIPHER_OPS_CRYPT_NONE;
    Config.Sign.PriKey      = Sm2Ppk;
    Config.Sign.Digest      = Sm2SpkHash;
    Config.Sign.Signature   = SignSm2Sign;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_SIGN, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_CMD_ERR) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Sign ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Sign start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Sign Default Done!----------\r\n");
}

AL_VOID AlCipher_Test_GenKey(AL_CIPHER_HalStruct *Handle)
{
    AL_U32 Ret = AL_OK;
    AL_CIPHER_ConfigUnion Config;
    AL_U32 Timeout = 1000;
    AL_CIPHER_MsgAckEnum AckVal = 0;

    /********************************************************Gen Key ECC256*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Gen Key ECC256!----------\r\n");

    Config.GenKey.KeyMode   = AL_CIPHER_OPS_AUTH_ECC256;
    Config.GenKey.PriKey    = GenKeyEccPri;
    Config.GenKey.PubKey    = GenKeyEccPub;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_GENKEY, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_GENKEY_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Gen key ack Error: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Gen key start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Gen Key ECC256 Done!----------\r\n");
    /********************************************************Gen Key SM2*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Gen Key SM2!----------\r\n");

    Config.GenKey.KeyMode   = AL_CIPHER_OPS_AUTH_SM2;
    Config.GenKey.PriKey    = GenKeySm2Pri;
    Config.GenKey.PubKey    = GenKeySm2Pub;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_GENKEY, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_GENKEY_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Gen key ack Error: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Gen key start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Gen Key SM2 Done!----------\r\n");
    /********************************************************Gen Key none : error branch*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Gen Key Default!----------\r\n");

    Config.GenKey.KeyMode   = AL_CIPHER_OPS_CRYPT_AES256;
    Config.GenKey.PriKey    = GenKeySm2Pri;
    Config.GenKey.PubKey    = GenKeySm2Pub;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_GENKEY, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_CMD_ERR) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Gen key ack Error: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Gen key start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Gen Key Default Done!----------\r\n");
}

AL_VOID AlCipher_Test_SignatureFlow(AL_CIPHER_HalStruct *Handle)
{
    AL_U32 Ret = AL_OK;
    AL_CIPHER_ConfigUnion Config;
    AL_U32 Timeout = 1000;
    AL_CIPHER_MsgAckEnum AckVal = 0;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------ECC256 Signature flow!----------\r\n");
    /********************************************************Gen Key ECC256*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Gen Key ECC256!----------\r\n");

    Config.GenKey.KeyMode   = AL_CIPHER_OPS_AUTH_ECC256;
    Config.GenKey.PriKey    = GenKeyEccPri;
    Config.GenKey.PubKey    = GenKeyEccPub;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_GENKEY, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_GENKEY_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Gen key ack Error: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Gen key start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Gen Key ECC256 Done!----------\r\n");
    /********************************************************Sign ECC256*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Sign ECC256!----------\r\n");

    Config.Sign.SignMode    = AL_CIPHER_OPS_AUTH_ECC256;
    Config.Sign.PriKey      = GenKeyEccPri;
    Config.Sign.Digest      = EccSpkHash;
    Config.Sign.Signature   = SignEccSign;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_SIGN, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_SIGN_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Sign ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Sign start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Sign ECC256 Done!----------\r\n");
    /********************************************************auth ECC256*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Auth Verify ECC256----------\r\n");

    Config.Auth.AuthMode    = AL_CIPHER_OPS_AUTH_ECC256;
    Config.Auth.PubKey      = GenKeyEccPub;
    Config.Auth.Digest      = EccSpkHash;
    Config.Auth.Signature   = SignEccSign;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_AUTH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_AUTH_PASS) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Auth ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Auth start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Auth Verify ECC256 Done----------\r\n");

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------SM2 Signature flow!----------\r\n");
    /********************************************************Gen Key SM2*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Gen Key SM2!----------\r\n");

    Config.GenKey.KeyMode   = AL_CIPHER_OPS_AUTH_SM2;
    Config.GenKey.PriKey    = GenKeySm2Pri;
    Config.GenKey.PubKey    = GenKeySm2Pub;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_GENKEY, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_GENKEY_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Gen key ack Error: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Gen key start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Gen Key SM2 Done!----------\r\n");
    /********************************************************Sign SM2*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Sign SM2!----------\r\n");

    Config.Sign.SignMode    = AL_CIPHER_OPS_AUTH_SM2;
    Config.Sign.PriKey      = GenKeySm2Pri;
    Config.Sign.Digest      = Sm2SpkHash;
    Config.Sign.Signature   = SignSm2Sign;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_SIGN, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_SIGN_DONE) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Sign ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Sign start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Sign SM2 Done!----------\r\n");
    /********************************************************auth SM2*/
    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Auth Verify SM2----------\r\n");

    Config.Auth.AuthMode    = AL_CIPHER_OPS_AUTH_SM2;
    Config.Auth.PubKey      = GenKeySm2Pub;
    Config.Auth.Digest      = Sm2SpkHash;
    Config.Auth.Signature   = SignSm2Sign;

    Ret = AlCipher_Hal_StartBlock(Handle, AL_CIPHER_CMD_AUTH, &Config, Timeout);
    if (AL_CIPHER_IS_ACK_STATE(Ret)) {
        if (Ret != AL_CIPHER_ACK_AUTH_PASS) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Auth ack err: 0x%x\r\n", Ret);
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Auth start block err: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_DEBUG, "----------Auth Verify SM2 Done----------\r\n");
}

AL_U32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U32 TestCount = 0;
    AL_U32 DevId = 0;
    AL_CIPHER_HalStruct *Handle = AL_NULL;

    Ret = AlCipher_Hal_Init(&Handle, AL_NULL, DevId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Cipher init error: 0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (TestCount++ < TEST_COUNT_MAX) {
        AL_LOG(AL_LOG_LEVEL_DEBUG,"[[[[[[[[[[[Test count is %d]]]]]]]]]]]\r\n", TestCount);
        //csu dma -> whole block ddr -> correct
        AlCipher_Test_Dma_WholeBlockDdrBothInc(Handle);
        //csu dma -> block ddr -> correct
        AlCipher_Test_Dma_BlockDdrBothInc(Handle);
        //csu dma -> whole block pcap -> correct
        AlCipher_Test_Dma_WholeBlockPcap(Handle);
        //csu dma -> block pcap -> correct
        AlCipher_Test_Dma_BlockPcap(Handle);
        //csu dma -> performance -> correct
        AlCipher_Test_Dma_Profermance(Handle);
        //csu dma -> Error branch -> correct
        AlCipher_Test_Dma_Error(Handle);
        //csu auth -> correct
        AlCipher_Test_Auth(Handle);
        //csu hash -> correct
        AlCipher_Test_Hash(Handle);
        // //csu sign -> check in TestSignatureFlow correct
        // AlCipher_Test_Sign(Handle);
        // //csu gen key -> check in TestSignatureFlow correct
        // AlCipher_Test_GenKey(Handle);
        //csu sign flow -> correct
        AlCipher_Test_SignatureFlow(Handle);
    }

    while (1);
}
