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
 * @brief   axi dma mem cpy example
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

/************************** Function Prototypes ******************************/
static AL_S32 AlDma_Test_MemCpy(AL_VOID);

/************************** Function Definitions ******************************/
/**
 * This function initializes the DMA memory copy test, executes it, and logs the result.
 * If the test fails, it logs an error message and returns the error code.
 *
 * @return AL_S32 Returns AL_OK if the test is successful, or an error code if the test fails.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac mem copy test\r\n");

    Ret = AlDma_Test_MemCpy();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dmac mem copy test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac mem copy test success\r\n");

    return Ret;
}

/**
 * This function allocates aligned memory for source and destination buffers, initializes local interrupts,
 * and enters a loop where it continuously updates the source buffer, performs a memory copy from source to destination,
 * verifies the copied data, and clears the destination buffer for the next iteration. It logs each step of the process
 * and checks for errors. If an error occurs during memory copy or data verification, it logs the error and exits.
 * The function also handles cache flushing if MMU is enabled.
 *
 * @return AL_S32 Returns AL_OK if the memory copy and verification are successful, or an error code if an error occurs.
 */
static AL_S32 AlDma_Test_MemCpy(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_DMA_HalStruct *Handle = AL_NULL;
    AL_U32 *Src = memalign(CACHE_LINE_SIZE, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
    AL_U32 *Dst = memalign(CACHE_LINE_SIZE, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));

    AL_LOG(AL_LOG_LEVEL_INFO, "Copy from %p to %p \r\n", Src, Dst);

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (1) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Loop: 0x%d\r\n", InitData++);

        for (AL_U32 i = 0; i < AL_DMA_EX_DATA_SIZE; i++) {
            Src[i] = i + InitData;
        }

        Ret = AlDma_Hal_MemCpy((AL_UINTPTR)Dst, (AL_UINTPTR)Src, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
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
        AlCache_FlushDcacheRange(Dst, Dst + AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
        #endif
    }

    free(Src);
    free(Dst);

    return Ret;
}
