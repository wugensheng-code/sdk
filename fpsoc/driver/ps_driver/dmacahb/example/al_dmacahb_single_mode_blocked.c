/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_dmacahb_single_mode_blocked.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   dmac ahb single mode blocked example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <malloc.h>
#include "al_dmacahb_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_DMACAHB_EX_DEVICE_ID             (0)
#define AL_DMACAHB_EX_BLOCKED_TIMEOUT_IN_MS (100)
#define AL_DMACAHB_EX_MEM_SIZE              (1)
#define AL_DMACAHB_EX_ARRAY_SIZE            (256)

/************************** Variable Definitions *****************************/
static AL_DMACAHB_ChInitStruct ChInitCfg = {
    .Id                     = AL_DMACAHB_CHANNEL_0,
    .TransType              = AL_DMACAHB_TRANS_TYPE_1,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_1,
    .DstBurstLength         = AL_DMACAHB_MSIZE_1,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_7,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

/************************** Function Prototypes ******************************/
static AL_S32 AlDmacAhb_Test_SingleModeBlocked(AL_VOID);

/************************** Function Definitions ******************************/
/**
 *
 * This function initializes the test environment and then calls the AlDmacAhb_Test_SingleModeBlocked function
 * to perform the test. It logs the start and end of the test, and reports success or failure based on the return
 * value of the test function.
 *
 * @return AL_OK if the test passes, otherwise an error code indicating the type of failure.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac ahb single mode blocked test\r\n");

    Ret = AlDmacAhb_Test_SingleModeBlocked();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dmac ahb single mode blocked test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac ahb single mode blocked test success\r\n");

    return Ret;
}

/**
 *
 * This function sets up a DMA transfer in single mode with blocking. It allocates memory for source and destination
 * buffers, initializes the DMA channel, and performs a series of data transfers, each time verifying the integrity
 * of the transferred data. It cleans up allocated resources upon completion or error. The function demonstrates
 * handling of DMA transfers including setup, execution, and validation of transfer results.
 *
 * @return AL_OK if all transfers are successful and data integrity is maintained, otherwise an error code.
 */
static AL_S32 AlDmacAhb_Test_SingleModeBlocked(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U8 InitData = 0;
    AL_DMACAHB_HalStruct *Handle = AL_NULL;
    AL_DMACAHB_ChTransStruct ChTransCfg = {0};
    AL_U32 TransSize = AL_DMACAHB_EX_ARRAY_SIZE;
    AL_U8 *MemSrc = memalign(CACHE_LINE_SIZE, AL_DMACAHB_EX_ARRAY_SIZE);
    AL_U8 *MemDst = memalign(CACHE_LINE_SIZE, AL_DMACAHB_EX_ARRAY_SIZE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Aligned Src:0x%x, Dst:0x%x\r\n", MemSrc, MemDst);

    Ret = AlDmacAhb_Hal_Init(&Handle, AL_DMACAHB_EX_DEVICE_ID, &ChInitCfg, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    ChTransCfg.SrcAddr = (AL_REG)MemSrc;
    ChTransCfg.DstAddr = (AL_REG)MemDst;
    ChTransCfg.TransSize = TransSize / (1 << ChInitCfg.SrcTransWidth);
    Handle->Channel.Trans = ChTransCfg;

    while (1) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Loop: 0x%d\r\n", InitData);

        memset((AL_VOID *)(AL_UINTPTR)ChTransCfg.SrcAddr, InitData++, AL_DMACAHB_EX_ARRAY_SIZE);

        Ret = AlDmacAhb_Hal_StartBlock(Handle, AL_DMACAHB_EX_BLOCKED_TIMEOUT_IN_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
            return Ret;
        }

        Ret = memcmp((AL_VOID *)(AL_UINTPTR)Handle->Channel.Trans.SrcAddr,
                     (AL_VOID *)(AL_UINTPTR)Handle->Channel.Trans.DstAddr, AL_DMACAHB_EX_ARRAY_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
            return Ret;
        }

        memset((AL_VOID *)(AL_UINTPTR)Handle->Channel.Trans.DstAddr, 0, AL_DMACAHB_EX_ARRAY_SIZE);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange(Handle->Channel.Trans.DstAddr, Handle->Channel.Trans.DstAddr + AL_DMACAHB_EX_ARRAY_SIZE);
        #endif
    }

    Ret = AlDmacAhb_Hal_DeInit(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
        return Ret;
    }

    free(MemSrc);
    free(MemDst);

    return Ret;
}
