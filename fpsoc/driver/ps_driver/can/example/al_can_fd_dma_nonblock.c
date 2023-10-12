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
    .TransType              = AL_DMACAHB_TRANS_TYPE_4,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_BLOCK | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_4,
    .DstBurstLength         = AL_DMACAHB_MSIZE_4,
    .Direction              = AL_DMACAHB_TT_FC_PER2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .HandShaking.SrcPer     = AL_DMACAHB_PER_CAN0_RX,
    .HandShaking.SrcHsSel   = AL_DMACAHB_HAND_SHAKING_HARDWARE,
    .HandShaking.SrcHsPol   = AL_DMACAHB_HS_POL_ACTIVE_HI,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_7,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

AL_MailBox ReloadEventQueue;

/************************** Function Prototypes ******************************/
static AL_S32 AlCan_Test_FdDmaNonBlock(AL_VOID);
static AL_S32 AlCan_Test_DmacahbChEventCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef);
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
    AL_DMACAHB_EventStruct DmaEvent = {0};
    AL_CAN_HalStruct *CanHandle = AL_NULL;
    AL_DMACAHB_HalStruct *DmaHandle = AL_NULL;
    AL_DMACAHB_ChTransStruct *DmacChTrans = AL_NULL;
    AL_U8 *RecvMem = (AL_U8 *)memalign(CACHE_LINE_SIZE, AL_CAN_EX_DMA_RECV_DATA);

    Ret = AlCan_Hal_Init(&CanHandle, AL_CAN_EX_CAN_DEVICE_ID, &FdDma_CanConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Can hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlDmacAhb_Hal_Init(&DmaHandle, AL_CAN_EX_DMACAHB_DEVICE_ID, &FdDma_DmaChConfig, AlCan_Test_DmacahbChEventCallBack);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dmacahb hal Init error:0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    DmacChTrans = &(DmaHandle->Channel.Trans);
    DmacChTrans->SrcAddr        = CanHandle->Dev.BaseAddr;
    DmacChTrans->DstAddr        = (AL_REG)RecvMem;
    DmacChTrans->TransSize      = AL_CAN_EX_DMA_RECV_DATA >> 2;
    DmacChTrans->ReloadCountNum = AL_DMACAHB_RELOAD_CNT_MAX;   /* max AL_U32 for trans forever */

    AlDmacAhb_Hal_Start(DmaHandle);

    while (1) {
        Ret = AlOsal_Mb_Receive(&ReloadEventQueue, &DmaEvent, 0);
        if (Ret != AL_OK) {
            if (Ret == AL_ERR_UNAVAILABLE) {
                continue;
            } else {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Can Dma data recv error:0x%x\r\n", Ret);
                return Ret;
            }
        } else {
            #ifdef ENABLE_MMU
            AlCache_InvalidateDcacheRange((AL_UINTPTR)RecvMem, (AL_UINTPTR)(RecvMem + AL_CAN_EX_DMA_RECV_DATA));
            #endif
            AlCan_Dev_DecodeFrame(RecvMem, &Frame);
            AlCan_Dev_DisplayFrame(&Frame);
        }
    }

    return Ret;
}

static AL_S32 AlCan_Test_DmacahbChEventCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_DMACAHB_HalStruct *Handle = (AL_DMACAHB_HalStruct *)CallBackRef;

    AL_ASSERT((Event != AL_NULL) && (CallBackRef != AL_NULL), AL_DMACAHB_ERR_NULL_PTR);

    switch (Event->EventId)
    {
    case AL_DMACAHB_EVENT_TRANS_COMP:
    case AL_DMACAHB_EVENT_BLOCK_TRANS_COMP:
        AlOsal_Mb_Send(&Handle->EventQueue, Event);
        break;
    case AL_DMACAHB_EVENT_ERR:
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dma trans error\r\n");
        break;
    case AL_DMACAHB_EVENT_RELOAD:
        AlOsal_Mb_Send(&ReloadEventQueue, Event);
        break;
    default:
        break;
    }
    return AL_OK;
}