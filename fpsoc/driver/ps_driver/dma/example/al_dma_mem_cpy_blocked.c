/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_dmacahb_llp_mode_blocked.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   axi dma mem cpy blocked example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <malloc.h>
#include "al_dma_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_DMA_EX_DATA_SIZE             (1024)

/************************** Variable Definitions *****************************/
AL_DMA_ChCfgStruct Config = {
    .Direction      = DMA_MEM_TO_MEM,
    .SrcAddrWidth   = DMA_CH_BUSWIDTH_32_BYTES,
    .DstAddrWidth   = DMA_CH_BUSWIDTH_32_BYTES,
    .SrcMaxBurst    = 16,
    .DstMaxBurst    = 4,
};

/************************** Function Prototypes ******************************/
static AL_S32 AlDma_Test_MemCpyBlocked(AL_VOID);

/************************** Function Definitions ******************************/
/**
 * This function initializes the DMA hardware, performs a memory copy operation in a blocked manner,
 * and then deinitializes the DMA hardware. It logs information about the operation and checks for errors
 * at each step. If an error occurs, it logs the error and exits with the error code.
 *
 * @return AL_S32 Returns AL_OK on success or an error code on failure.
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
 * This function allocates source and destination memory buffers, initializes the DMA hardware for a memory
 * to memory transfer, and performs a series of memory copy operations in a loop. Each loop iteration copies
 * data from the source buffer to the destination buffer, verifies the copied data, and then clears the destination
 * buffer for the next iteration. It also handles cache flushing if MMU is enabled. If any step fails, it logs
 * the error, releases resources, and returns the error code.
 *
 * @return AL_S32 Returns AL_OK on success or an error code on failure.
 */
static AL_S32 AlDma_Test_MemCpyBlocked(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_DMA_HalStruct *Handle = AL_NULL;
    AL_U32 *Src = memalign(CACHE_LINE_SIZE, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
    AL_U32 *Dst = memalign(CACHE_LINE_SIZE, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));

    AL_LOG(AL_LOG_LEVEL_INFO, "Copy from %p to %p \r\n", Src, Dst);

    Ret = AlDma_Hal_Init(&Handle, 0, &Config);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Loop: 0x%d\r\n", InitData++);

        for (AL_U32 i = 0; i < AL_DMA_EX_DATA_SIZE; i++) {
            Src[i] = i + InitData;
        }

        Ret = AlDma_Hal_MemCpyBlock(Handle, (AL_UINTPTR)Dst, (AL_UINTPTR)Src, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Mem copy error:0x%x\r\n", Ret);
            return Ret;
        }

        Ret = memcmp(Dst, Src, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
            return Ret;
        }

        memset(Dst, 0, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange((AL_UINTPTR)Dst, (AL_UINTPTR)(Dst + AL_DMA_EX_DATA_SIZE * sizeof(AL_U32)));
        #endif
    }

    Ret = AlDma_Hal_DeInit(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
        return Ret;
    }

    free(Src);
    free(Dst);

    return Ret;
}
