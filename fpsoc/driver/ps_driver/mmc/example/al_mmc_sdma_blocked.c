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
 * @brief   mmc sdma blocked example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <malloc.h>
#include "al_mmc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef DEVICE_ID
#define DEVICE_ID (0)
#endif

#define AL_MMC_EX_DEVICE_ID             (DEVICE_ID)
#define AL_MMC_EX_BLOCKED_TIMEOUT_IN_MS (100)
#define AL_MMC_EX_BLOCK_SIZE            (512)
#define AL_MMC_EX_BLOCK_COUNT           (10)
#define AL_MMC_EX_BLOCK_LEN             (AL_MMC_EX_BLOCK_SIZE * AL_MMC_EX_BLOCK_COUNT)

/************************** Variable Definitions *****************************/
static AL_MMC_InitStruct MmcSdmaInit = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_SDMA,
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_32K
};

/************************** Function Prototypes ******************************/
static AL_S32 AlMmc_Test_SdmaBlocked(AL_VOID);

/************************** Function Definitions ******************************/
/**
 *
 * This function initializes the MMC test for SDMA in blocked mode. It logs the start of the test,
 * calls the AlMmc_Test_SdmaBlocked function to perform the test, and logs the result. If the test
 * fails, it logs an error message and returns the error code. Otherwise, it logs a success message
 * indicating the test passed.
 *
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Mmc sdma blocked test\r\n");

    Ret = AlMmc_Test_SdmaBlocked();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Mmc sdma blocked test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Mmc sdma blocked test success\r\n");

    return Ret;
}

/**
 *
 * This function tests the MMC device using SDMA in blocked mode. It allocates memory for write and read
 * operations, initializes the MMC hardware, and performs write and read operations in a loop. Each loop
 * iteration writes to and reads from the MMC device, compares the written and read data, and logs the result.
 * If any operation fails, it logs an error message and returns an error code. The function also handles memory
 * cleanup and cache flushing if MMU is enabled. The loop continues indefinitely, logging success messages for
 * each successful data check.
 *
 * @return Returns AL_OK on success, or an error code on failure.
 */
static AL_S32 AlMmc_Test_SdmaBlocked(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_U32 BlockCnt = AL_MMC_EX_BLOCK_COUNT;
    AL_MMC_HalStruct *Handle = AL_NULL;
    AL_U8 *MemWrite = memalign(CACHE_LINE_SIZE, AL_MMC_EX_BLOCK_LEN);
    AL_U8 *MemRead = memalign(CACHE_LINE_SIZE, AL_MMC_EX_BLOCK_LEN);

    AL_LOG(AL_LOG_LEVEL_DEBUG, "Aligned Src:%p, Dst:%p\r\n", MemWrite, MemRead);

    Ret = AlMmc_Hal_Init(&Handle, AL_MMC_EX_DEVICE_ID, &MmcSdmaInit, AL_NULL);
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
