/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_mmc_non_dma_blocked.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   mmc adma2 blocked example
 */

#include <common.h>
#include <string.h>
#include <malloc.h>

#ifndef DEVICE_ID
#define DEVICE_ID (0)
#endif

#define AL_MMC_EX_DEVICE_ID             (DEVICE_ID)
#define MMC_BLOCK_OFFSET            (2*1024*100)
#define MMC_BLOCK_SIZE              (512)
#define MMC_BLOCK_COUNT             (100)
#define MMC_BLOCK_LEN               (MMC_BLOCK_SIZE * MMC_BLOCK_COUNT)

#define AL_MMC_EX_BLOCKED_TIMEOUT_IN_MS (100)
#define AL_MMC_EX_BLOCK_SIZE            (512)
#define AL_MMC_EX_BLOCK_COUNT           (10)
#define AL_MMC_EX_BLOCK_LEN             (AL_MMC_EX_BLOCK_SIZE * AL_MMC_EX_BLOCK_COUNT)



/*******************************************************************************/
/**************************** MMC sdma block test ******************************/
/*******************************************************************************/

static AL_MMC_InitStruct MmcSdmaInit = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_SDMA,
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_32K
};

/* A test case that does nothing and succeeds. */
static void AlMmc_Test_SdmaBlocked_Tc(AL_VOID) {
     AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_U32 BlockCnt = AL_MMC_EX_BLOCK_COUNT;
    AL_MMC_HalStruct *Handle = AL_NULL;
    AL_U8 *MemWrite = (AL_U8 *)pvPortMalloc(AL_MMC_EX_BLOCK_LEN);
    AL_U8 *MemRead = (AL_U8 *)pvPortMalloc(AL_MMC_EX_BLOCK_LEN);

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Aligned Src:%p, Dst:%p\r\n", MemWrite, MemRead);
    
    Ret = AlMmc_Hal_Init(&Handle, AL_MMC_EX_DEVICE_ID, &MmcSdmaInit, AL_NULL);
    
    assert_int_equal(Ret, AL_OK);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal init error 0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    
    TickType_t Start = xTaskGetTickCount();


    while ((xTaskGetTickCount() - Start) < (pdMS_TO_TICKS(25000)))
    {
        memset(MemWrite, InitData++, AL_MMC_EX_BLOCK_LEN);

        Ret = AlMmc_Hal_WriteBlocked(Handle, MemWrite, InitData, BlockCnt, pdMS_TO_TICKS(5000));
        assert_int_equal(Ret, AL_OK);

        Ret = AlMmc_Hal_ReadBlocked(Handle, MemRead, InitData, BlockCnt, pdMS_TO_TICKS(5000));
        assert_int_equal(Ret, AL_OK);

        Ret = memcmp(MemWrite, MemRead, AL_MMC_EX_BLOCK_LEN);
        assert_int_equal(Ret, AL_OK);

        assert_memory_equal(MemWrite, MemRead, AL_MMC_EX_BLOCK_LEN);

        memset(MemRead, 0, AL_MMC_EX_BLOCK_LEN);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange((AL_UINTPTR)MemRead, MemRead + AL_MMC_EX_BLOCK_LEN);
        #endif

    }
    vPortFree(MemWrite);
    vPortFree(MemRead);
}

/*******************************************************************************/
/**************************** MMC Non dma blocked test *************************/
/*******************************************************************************/

static AL_MMC_InitStruct MmcNonDmaInit = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_NONE,
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_32K
};

static void AlMmc_Test_NonDmaBlocked(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_U32 BlockCnt = AL_MMC_EX_BLOCK_COUNT;
    AL_MMC_HalStruct *Handle = AL_NULL;
    AL_U8 *MemWrite_NonDma = (AL_U8 *)pvPortMalloc(AL_MMC_EX_BLOCK_LEN);
    AL_U8 *MemRead_NonDma = (AL_U8 *)pvPortMalloc(AL_MMC_EX_BLOCK_LEN);
    TickType_t Start = xTaskGetTickCount();

    AL_LOG(AL_LOG_LEVEL_ERROR, "Aligned Src:%p, Dst:%p\r\n", MemWrite_NonDma, MemRead_NonDma);

    Ret = AlMmc_Hal_Init(&Handle, AL_MMC_EX_DEVICE_ID, &MmcNonDmaInit, AL_NULL);
    assert_int_equal(Ret, AL_OK);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal init error 0x%x\r\n", Ret);
        return Ret;
    }
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal init error 0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while ((xTaskGetTickCount() - Start) < (pdMS_TO_TICKS(50000))) {
        memset(MemWrite_NonDma, InitData++, AL_MMC_EX_BLOCK_LEN);

        Ret = AlMmc_Hal_WriteBlocked(Handle, MemWrite_NonDma, InitData, BlockCnt, 100);
        assert_int_equal(Ret, AL_OK);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal write %d block error 0x%x\r\n", BlockCnt, Ret);
            return Ret;
        }

        Ret = AlMmc_Hal_ReadBlocked(Handle, MemRead_NonDma, InitData, BlockCnt, 100);
        assert_int_equal(Ret, AL_OK);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal read %d block error 0x%x\r\n", BlockCnt, Ret);
            return Ret;
        }

        Ret = memcmp(MemWrite_NonDma, MemRead_NonDma, AL_MMC_EX_BLOCK_LEN);
        assert_int_equal(Ret, AL_OK);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
            return Ret;
        } else {
            AL_LOG(AL_LOG_LEVEL_INFO, "Loop: %d, Data check success\r\n", InitData);
        }
        
        memset(MemRead_NonDma, 0, AL_MMC_EX_BLOCK_LEN);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange((AL_UINTPTR)MemRead_NonDma, (AL_UINTPTR)(MemRead_NonDma + AL_MMC_EX_BLOCK_LEN));
        #endif
    }
    vPortFree(MemRead_NonDma);
    vPortFree(MemWrite_NonDma);

}

/*******************************************************************************/
/**************************** MMC Non adma2 block test *************************/
/*******************************************************************************/
static AL_MMC_InitStruct MmcAdma2Init = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_ADMA2,
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_ENABLE,
    .DmaBdary           = AL_MMC_BDARY_32K
};

static void AlMmc_Test_Adma2Blocked(AL_VOID)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Mmc adma2 blocked test\r\n");
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_U32 BlockCnt = AL_MMC_EX_BLOCK_COUNT;
    AL_MMC_HalStruct *Handle = AL_NULL;
    AL_U8 *MemWrite_adma2 = (AL_U8 *)pvPortMalloc(AL_MMC_EX_BLOCK_LEN);
    AL_U8 *MemRead_adma2 = (AL_U8 *)pvPortMalloc(AL_MMC_EX_BLOCK_LEN);

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Aligned Src:%p, Dst:%p\r\n", MemWrite_adma2, MemRead_adma2);

    Ret = AlMmc_Hal_Init(&Handle, AL_MMC_EX_DEVICE_ID, &MmcAdma2Init, AL_NULL);
    assert_int_equal(Ret, AL_OK);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal init error 0x%x\r\n", Ret);
        return Ret;
    }
    
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    TickType_t Start = xTaskGetTickCount();

    while ((xTaskGetTickCount() - Start) < pdMS_TO_TICKS(25000)) {
        memset(MemWrite_adma2, InitData++, AL_MMC_EX_BLOCK_LEN);

        Ret = AlMmc_Hal_WriteBlocked(Handle, MemWrite_adma2, InitData, BlockCnt, pdMS_TO_TICKS(5000));
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal write %d block error 0x%x\r\n", BlockCnt, Ret);
            return Ret;
        }
        assert_int_equal(Ret, AL_OK);

        Ret = AlMmc_Hal_ReadBlocked(Handle, MemRead_adma2, InitData, BlockCnt, pdMS_TO_TICKS(5000));
        assert_int_equal(Ret, AL_OK);

        Ret = memcmp(MemWrite_adma2, MemRead_adma2, AL_MMC_EX_BLOCK_LEN);
        assert_int_equal(Ret, AL_OK);

        memset(MemRead_adma2, 0, AL_MMC_EX_BLOCK_LEN);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange((AL_UINTPTR)MemRead_adma2, (AL_UINTPTR)(MemRead_adma2 + AL_MMC_EX_BLOCK_LEN));
        #endif
    }
    vPortFree(MemWrite_adma2);
    vPortFree(MemRead_adma2);

}


CMOCKA_TC_DEFINE(AlMmc_Test_SdmaBlocked_Tc, NULL, NULL, NULL);
CMOCKA_TC_DEFINE(AlMmc_Test_NonDmaBlocked, NULL, NULL, NULL);
CMOCKA_TC_DEFINE(AlMmc_Test_Adma2Blocked, NULL, NULL, NULL);
