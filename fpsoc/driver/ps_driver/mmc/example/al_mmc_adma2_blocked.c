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

/***************************** Include Files *********************************/
#include <string.h>
#include <malloc.h>
#include "al_mmc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_MMC_EX_DEVICE_ID             (0)
#define AL_MMC_EX_BLOCKED_TIMEOUT_IN_MS (100)
#define AL_MMC_EX_BLOCK_SIZE            (512)
#define AL_MMC_EX_BLOCK_COUNT           (10)
#define AL_MMC_EX_BLOCK_LEN             (AL_MMC_EX_BLOCK_SIZE * AL_MMC_EX_BLOCK_COUNT)

/************************** Variable Definitions *****************************/
static AL_MMC_InitStruct MmcAdma2Init = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_ADMA2,
    .FreqKhz            = AL_MMC_FREQ_KHZ_50000,
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_ENABLE,
    .DmaBdary           = AL_MMC_BDARY_32K
};

/************************** Function Prototypes ******************************/
static AL_S32 AlMmc_Test_Adma2Blocked(AL_VOID);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Mmc adma2 blocked test\r\n");

    Ret = AlMmc_Test_Adma2Blocked();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Mmc adma2 blocked test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Mmc adma2 blocked test success\r\n");

    return Ret;
}

static AL_S32 AlMmc_Test_Adma2Blocked(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_U32 BlockCnt = AL_MMC_EX_BLOCK_COUNT;
    AL_MMC_HalStruct *Handle = AL_NULL;
    AL_U8 *MemWrite = memalign(CACHE_LINE_SIZE, AL_MMC_EX_BLOCK_LEN);
    AL_U8 *MemRead = memalign(CACHE_LINE_SIZE, AL_MMC_EX_BLOCK_LEN);

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Aligned Src:%p, Dst:%p\r\n", MemWrite, MemRead);

    Ret = AlMmc_Hal_Init(&Handle, AL_MMC_EX_DEVICE_ID, &MmcAdma2Init, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal init error 0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        memset(MemWrite, InitData++, AL_MMC_EX_BLOCK_LEN);

        Ret = AlMmc_Hal_WriteBlocked(Handle, MemWrite, InitData, BlockCnt, AL_MMC_EX_BLOCKED_TIMEOUT_IN_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal write %d block error 0x%x\r\n", BlockCnt, Ret);
            return Ret;
        }

        Ret = AlMmc_Hal_ReadBlocked(Handle, MemRead, InitData, BlockCnt, AL_MMC_EX_BLOCKED_TIMEOUT_IN_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Hal read %d block error 0x%x\r\n", BlockCnt, Ret);
            return Ret;
        }

        Ret = memcmp(MemWrite, MemRead, AL_MMC_EX_BLOCK_LEN);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
            return Ret;
        } else {
            AL_LOG(AL_LOG_LEVEL_INFO, "Loop: %d, Data check success\r\n", InitData);
        }

        memset(MemRead, 0, AL_MMC_EX_BLOCK_LEN);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange((AL_UINTPTR)MemRead, (AL_UINTPTR)(MemRead + AL_MMC_EX_BLOCK_LEN));
        #endif
    }

    free(MemWrite);
    free(MemRead);

    return Ret;
}
