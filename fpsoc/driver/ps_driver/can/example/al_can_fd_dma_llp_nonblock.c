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
 * @brief   can fd dma llp non-block example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <malloc.h>
#include "al_can_hal.h"
#include "al_dmacahb_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_CAN_EX_CAN_DEVICE_ID         (0)
#define AL_CAN_EX_DMACAHB_DEVICE_ID     (0)
#define AL_CAN_EX_BLOCKED_TIMEOUT_IN_MS (100)
#define AL_CAN_EX_MEM_SIZE              (1)
#define AL_CAN_EX_ARRAY_SIZE            (256)
#define AL_CAN_EX_DMA_RECV_DATA         (80)
#define AL_CAN_EX_DMA_LLI_STRUCT_NUM    (5)

/************************** Variable Definitions *****************************/
static AL_CAN_InitStruct FdDma_CanConfig = {
    .OpsMode      = AL_CAN_MODE_NORMAL,
    .RunMode      = AL_CAN_RUN_INTR,
    .TransMode    = AL_CAN_TRANS_PTB,
    .RbAfwl       = AL_CAN_RB_LIMIT_8
};

static AL_DMACAHB_ChInitStruct FdDma_DmaChConfig = {
    .Id                     = AL_DMACAHB_CHANNEL_0,
    .TransType              = AL_DMACAHB_TRANS_TYPE_10,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_4,
    .DstBurstLength         = AL_DMACAHB_MSIZE_4,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .HandShaking.SrcHsSel   = AL_DMACAHB_HAND_SHAKING_SOFTWARE,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_7,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

/************************** Function Prototypes ******************************/
static AL_S32 AlCan_Test_FdDmaLlpNonBlock(AL_VOID);
/************************** Function Definitions ******************************/
/**
 *
 * This function logs the start of the test, calls the AlCan_Test_FdDmaLlpNonBlock function to perform the actual test, and logs the result based on the return value of the test function. If the test fails, it logs an error message and returns the error code. Otherwise, it logs a success message and returns AL_OK.
 *
 * @return AL_OK if the test runs successfully, error code otherwise.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Can fd dma llp non block test\r\n");

    Ret = AlCan_Test_FdDmaLlpNonBlock();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Can fd dma llp non block test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Can fd dma llp non block test success\r\n");

    return Ret;
}

/**
 * Performs the CAN FD DMA LLP non-block test.
 *
 * This function initializes the CAN and DMA hardware with specific configurations for non-blocking operations using linked list items (LLIs).
 * It allocates memory for receiving CAN frames and sets up DMA LLIs for transferring received data into this memory.
 * The function enters an infinite loop, continuously receiving CAN frames using DMA, validating the received data, and logging the results.
 * The loop includes mechanisms for handling errors and ensuring data coherence in memory through cache operations.
 *
 * @return AL_OK if the test completes successfully, error code otherwise.
 */
static AL_S32 AlCan_Test_FdDmaLlpNonBlock(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_CAN_FrameStruct Frame = {0};
    AL_CAN_EventStruct CanRecvEvent = {0};
    AL_DMACAHB_EventStruct DmaEvent = {0};
    AL_CAN_HalStruct *CanHandle = AL_NULL;
    AL_DMACAHB_HalStruct *DmaHandle = AL_NULL;
    AL_DMACAHB_LliStruct CACHE_LINE_ALIGN Lli[AL_CAN_EX_DMA_LLI_STRUCT_NUM] ;
    AL_U8 *RecvMem = memalign(CACHE_LINE_SIZE, AL_CAN_EX_DMA_RECV_DATA);

    /* Avoid evict, refer to dma_map_single in kernel */
    #ifdef ENABLE_MMU
    AlCache_FlushDcacheRange((AL_UINTPTR)RecvMem, (AL_UINTPTR)(RecvMem + AL_CAN_EX_DMA_RECV_DATA));
    #endif

    Ret = AlCan_Hal_Init(&CanHandle, AL_CAN_EX_CAN_DEVICE_ID, &FdDma_CanConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Can hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlDmacAhb_Hal_Init(&DmaHandle, AL_CAN_EX_DMACAHB_DEVICE_ID, &FdDma_DmaChConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dmacahb hal Init error:0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_U32 SrcTransWidthInByte = 1 << DmaHandle->Channel.Config.SrcTransWidth;
    AL_U32 SrcBurstLengthInByte = 1 << (DmaHandle->Channel.Config.SrcBurstLength == AL_DMACAHB_MSIZE_1 ?
                                        0 : (DmaHandle->Channel.Config.SrcBurstLength + 1));

    for (AL_U32 i = 0; i < AL_CAN_EX_DMA_LLI_STRUCT_NUM; i++) {
        Lli[i].SrcAddr = (AL_REG)(CanHandle->Dev.BaseAddr + (i * SrcTransWidthInByte * SrcBurstLengthInByte));
        Lli[i].DstAddr = (AL_REG)(RecvMem + (i * SrcTransWidthInByte * SrcBurstLengthInByte));

        AlDmacAhb_Dev_FillLliWithCtl(&(DmaHandle->Channel), &Lli[i]);
        Lli[i].CtlHigh.Bit.BlockTransSize = SrcBurstLengthInByte;

        if ((i + 1) == AL_CAN_EX_DMA_LLI_STRUCT_NUM) {
            Lli[i].LlpNext = (AL_REG)AL_NULL;
            Lli[i].CtlLow.Bit.LlpSrcEn = AL_FALSE;
            Lli[i].CtlLow.Bit.LlpDstEn = AL_FALSE;
        } else {
            Lli[i].LlpNext = (AL_REG)&Lli[i + 1];
            Lli[i].CtlLow.Bit.LlpSrcEn = AL_TRUE;
            Lli[i].CtlLow.Bit.LlpDstEn = AL_TRUE;
        }
    }

    DmaHandle->Channel.Trans.Lli = Lli;

    while (1) {
        Ret = AlOsal_Mb_Receive(&CanHandle->RxEventQueue, &CanRecvEvent, 0);
        if (Ret != AL_OK) {
            if (Ret != AL_ERR_UNAVAILABLE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Can Dma data recv error:0x%x\r\n", Ret);
                return Ret;
            }
        } else {
            if (AlCan_ll_GetRstat(CanHandle->Dev.BaseAddr) == 0) {
                AL_LOG(AL_LOG_LEVEL_DEBUG, "Can recv buffer empty\r\n");
            } else {
                AlDmacAhb_Hal_Start(DmaHandle);
            }
        }

        Ret = AlOsal_Mb_Receive(&DmaHandle->EventQueue, &DmaEvent, 0);
        if (Ret != AL_OK) {
            if (Ret != AL_ERR_UNAVAILABLE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Can Dma data recv error:0x%x\r\n", Ret);
                return Ret;
            }
        } else {
            AlCan_ll_SetRrel(CanHandle->Dev.BaseAddr, AL_CAN_RREL_RELEASE);
            #ifdef ENABLE_MMU
            AlCache_InvalidateDcacheRange((AL_UINTPTR)RecvMem, (AL_UINTPTR)(RecvMem + AL_CAN_EX_DMA_RECV_DATA));
            #endif
            AlCan_Dev_DecodeFrame((AL_U32 *)RecvMem, &Frame);
            AlCan_Dev_DisplayFrame(&Frame);
            memset(RecvMem, 0, AL_CAN_EX_DMA_RECV_DATA);
            #ifdef ENABLE_MMU
            AlCache_FlushDcacheRange((AL_UINTPTR)RecvMem, (AL_UINTPTR)(RecvMem + AL_CAN_EX_DMA_RECV_DATA));
            #endif
        }
    }

    return Ret;
}
