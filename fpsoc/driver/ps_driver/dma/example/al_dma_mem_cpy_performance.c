/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_dmacahb_llp_mode_performance.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   axi dma mem cpy performance example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <malloc.h>
#include "al_dma_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/
typedef struct
{
    AL_U64      Start;
    AL_U64      End;
    AL_U64      Diff;
    AL_U64      TimeInUs;
    AL_U64      DatInByte;
    AL_FLOAT    BytePerSec;
} AL_DMA_CalStruct;

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_DMA_EX_DATA_SIZE_IN_WORD     (256)
#define AL_DMA_TEST_LOOP                (64)

/************************** Variable Definitions *****************************/
AL_DMA_ChCfgStruct Config = {
    .Direction      = DMA_MEM_TO_MEM,
    .SrcAddrWidth   = DMA_CH_BUSWIDTH_8_BYTES,
    .DstAddrWidth   = DMA_CH_BUSWIDTH_8_BYTES,
    .SrcMaxBurst    = 8,
    .DstMaxBurst    = 8,
};

AL_DMA_CalStruct Cal[AL_DMA_TEST_LOOP];
AL_U64 Start, End;
/************************** Function Prototypes ******************************/
static AL_S32 AlDma_Test_MemCpyBlocked(AL_VOID);

/************************** Function Definitions ******************************/
/**
 * This function initializes the logging system, runs the memory copy test in a blocked manner,
 * and logs the result of the test. If the test fails, it logs an error message and returns the error code.
 *
 * @return AL_S32 Returns AL_OK if the test is successful, or an error code if the test fails.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac mem copy blocked test\r\n");

    Ret = AlDma_Test_MemCpyBlocked();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dmac mem copy blocked test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac mem copy blocked test success\r\n");

    return Ret;
}

/**
 * This function initializes DMA hardware, performs memory copy operations in a loop, and measures
 * the performance of these operations. It allocates source and destination buffers, fills the source
 * buffer with data, copies the data from the source to the destination buffer using DMA, and verifies
 * the copied data. It also calculates and logs the time taken for each copy operation, the data rate,
 * and other performance metrics. The function handles initialization and deinitialization of the DMA
 * hardware, error checking, and cleanup.
 *
 * @return AL_S32 Returns AL_OK if all memory copy operations and checks are successful,
 *         or an error code if an operation fails.
 */
static AL_S32 AlDma_Test_MemCpyBlocked(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_DMA_HalStruct *Handle = AL_NULL;
    // AL_U32 *Src = memalign(CACHE_LINE_SIZE, AL_DMA_EX_DATA_SIZE_IN_WORD * sizeof(AL_U32) * AL_DMA_TEST_LOOP);
    // AL_U32 *Dst = memalign(CACHE_LINE_SIZE, AL_DMA_EX_DATA_SIZE_IN_WORD * sizeof(AL_U32) * AL_DMA_TEST_LOOP);
    AL_U32 *Dst = (AL_U32 *)(AL_UINTPTR)(0x40000000);
    AL_U32 *Src = (AL_U32 *)(AL_UINTPTR)(0x61020000);

    AL_LOG(AL_LOG_LEVEL_INFO, "Copy from %p to %p \r\n", Src, Dst);

    Ret = AlDma_Hal_Init(&Handle, 0, &Config);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Loop: 0x%d\r\n", ++InitData);

        for (AL_U32 i = 0; i < AL_DMA_EX_DATA_SIZE_IN_WORD * InitData; i++) {
            Src[i] = i + InitData;
        }

        Start = AlSys_GetTimerTickCount();

        Ret = AlDma_Hal_MemCpyBlock(Handle, (AL_UINTPTR)Dst, (AL_UINTPTR)Src, AL_DMA_EX_DATA_SIZE_IN_WORD * sizeof(AL_U32) * InitData);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Mem copy error:0x%x\r\n", Ret);
            return Ret;
        }

        End = AlSys_GetTimerTickCount();

        Cal[InitData-1].Start = Start;
        Cal[InitData-1].End = End;

        Ret = memcmp(Dst, Src, AL_DMA_EX_DATA_SIZE_IN_WORD * sizeof(AL_U32));
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
            return Ret;
        }

        memset(Dst, 0, AL_DMA_EX_DATA_SIZE_IN_WORD * sizeof(AL_U32) * InitData);
        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange(Dst, Dst + AL_DMA_EX_DATA_SIZE_IN_WORD * sizeof(AL_U32) * InitData);
        #endif

        if (InitData == AL_DMA_TEST_LOOP) {
            break;
        }
    }

    for (AL_U32 i = 0; i < AL_DMA_TEST_LOOP; i++) {
        if (Cal[i].End > Cal[i].Start) {
            Cal[i].Diff = Cal[i].End - Cal[i].Start;
        } else {
            Cal[i].Diff = (AL_U64)(~0) - Cal[i].Start + Cal[i].End;
        }
        Cal[i].TimeInUs = Cal[i].Diff/(AlSys_GetTimerFreq()/1000000);
        Cal[i].DatInByte = AL_DMA_EX_DATA_SIZE_IN_WORD * sizeof(AL_U32) * (i + 1);
        Cal[i].BytePerSec = ((AL_FLOAT)Cal[i].DatInByte * 1000000 / Cal[i].TimeInUs);
        AL_LOG(AL_LOG_LEVEL_INFO, "-------------------------------\r\n");
        AL_LOG(AL_LOG_LEVEL_INFO, "|-%d Start: %llu\r\n", i, Cal[i].Start);
        AL_LOG(AL_LOG_LEVEL_INFO, "|-%d End: %llu\r\n", i, Cal[i].End);
        AL_LOG(AL_LOG_LEVEL_INFO, "|-%d TimeInUs: %llu\r\n", i, Cal[i].TimeInUs);
        AL_LOG(AL_LOG_LEVEL_INFO, "|-%d DatInByte: %llu\r\n", i, Cal[i].DatInByte);
        // AL_LOG(AL_LOG_LEVEL_DEBUG, "|-%d BytePerSec: %f MB/s\r\n", i, Cal[i].BytePerSec);
    }

    Ret = AlDma_Hal_DeInit(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
        return Ret;
    }

    // free(Src);
    // free(Dst);

    return Ret;
}
