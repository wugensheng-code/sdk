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
 * @brief   can fd dma non-block example
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

/************************** Variable Definitions *****************************/
static AL_CAN_InitStruct FdDma_CanConfig = {
    .OpsMode      = AL_CAN_MODE_NORMAL,
    .RunMode      = AL_CAN_RUN_INTR_DMA,
    .Type         = AL_CAN_TYPE_FD,
    .SlowBitRate  = AL_CAN_ARBITRATION_0_5M,
    .FastBitRate  = AL_CAN_1_M,
    .TransMode    = AL_CAN_TRANS_PTB,
    .RbAfwl       = AL_CAN_RB_LIMIT_8
};

static AL_DMACAHB_ChInitStruct FdDma_DmaChConfig = {
    .Id                     = AL_DMACAHB_CHANNEL_0,
    .TransType              = AL_DMACAHB_TRANS_TYPE_1,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_SRCT | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_8,
    .DstBurstLength         = AL_DMACAHB_MSIZE_4,
    .Direction              = AL_DMACAHB_TT_FC_PER2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .HandShaking.SrcPer     = AL_DMACAHB_PER_CAN0_RX,
    .HandShaking.SrcHsSel   = AL_DMACAHB_HAND_SHAKING_SOFTWARE,
    .HandShaking.SrcHsPol   = AL_DMACAHB_HS_POL_ACTIVE_HI,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_7,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

/************************** Function Prototypes ******************************/
static AL_S32 AlCan_Test_FdDmaNonBlock(AL_VOID);
/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Can fd dma non block test\r\n");

    Ret = AlCan_Test_FdDmaNonBlock();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Can fd dma non block test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Can fd dma non block test success\r\n");

    return Ret;
}

static AL_S32 AlCan_Test_FdDmaNonBlock(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_CAN_FrameStruct Frame = {0};
    AL_CAN_EventStruct CanRecvEvent = {0};
    AL_DMACAHB_EventStruct DmaEvent = {0};
    AL_CAN_HalStruct *CanHandle = AL_NULL;
    AL_DMACAHB_HalStruct *DmaHandle = AL_NULL;
    AL_DMACAHB_ChTransStruct *DmacChTrans = AL_NULL;
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

    DmacChTrans = &(DmaHandle->Channel.Trans);
    DmacChTrans->SrcAddr        = CanHandle->Dev.BaseAddr;
    DmacChTrans->DstAddr        = (AL_REG)RecvMem;
    DmacChTrans->TransSize      = AL_CAN_EX_DMA_RECV_DATA >> DmaHandle->Channel.Config.SrcTransWidth;

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
                AL_LOG(AL_LOG_LEVEL_INFO, "Request a soft request\r\n");
                AlDmacAhb_Hal_Start(DmaHandle);
                AlDmacAhb_Dev_SoftRequest(&(DmaHandle->Channel));
            }
        }

        Ret = AlOsal_Mb_Receive(&DmaHandle->SrcEventQueue, &DmaEvent, 0);
        if (Ret != AL_OK) {
            if (Ret != AL_ERR_UNAVAILABLE) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Can Dma wait src trans event error:0x%x\r\n", Ret);
                return Ret;
            }
        } else {
            Ret = AlOsal_Mb_Receive(&DmaHandle->EventQueue, &DmaEvent, 0);
            if (Ret != AL_OK) {
                if (Ret != AL_ERR_UNAVAILABLE) {
                    AL_LOG(AL_LOG_LEVEL_ERROR, "Can Dma data recv error:0x%x\r\n", Ret);
                    return Ret;
                } else {
                    AlDmacAhb_Dev_SoftRequest(&(DmaHandle->Channel));
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
    }

    return Ret;
}
