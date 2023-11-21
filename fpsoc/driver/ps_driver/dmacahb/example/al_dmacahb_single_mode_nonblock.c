/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_dmacahb_single_mode_nonblock.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   dmac ahb single mode non-block example
 */

/***************************** Include Files *********************************/
#include "string.h"
#include <stdlib.h>
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
static AL_S32 AlDmacAhb_Test_SingleModeNonBlock(AL_VOID);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac ahb single mode non-block test\r\n");

    Ret = AlDmacAhb_Test_SingleModeNonBlock();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dmac ahb single mode non-block test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac ahb single mode non-block test success\r\n");

    return Ret;
}

static AL_S32 AlDmacAhb_Test_SingleModeNonBlock(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_DMACAHB_HalStruct *Handle = AL_NULL;
    AL_DMACAHB_ChTransStruct ChTransCfg = {0};
    AL_U32 TransSize = AL_DMACAHB_EX_ARRAY_SIZE;
    AL_DMACAHB_EventStruct Event;
    AL_U8 *MemSrc = (AL_U8 *)(AL_UINTPTR)memalign(CACHE_LINE_SIZE, AL_DMACAHB_EX_ARRAY_SIZE);
    AL_U8 *MemDst = (AL_U8 *)(AL_UINTPTR)memalign(CACHE_LINE_SIZE, AL_DMACAHB_EX_ARRAY_SIZE);

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

    memset((AL_VOID *)(AL_UINTPTR)ChTransCfg.SrcAddr, 'A', AL_DMACAHB_EX_ARRAY_SIZE);

    Ret = AlDmacAhb_Hal_Start(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Start error:0x%x\r\n", Ret);
        return Ret;
    }

    while (1) {
        /* example for something else */
        {
            AL_LOG(AL_LOG_LEVEL_INFO, "Example for something else\r\n");
        }

        Ret = AlDmacAhb_Hal_WaitTransDoneOrTimeout(Handle, &Event, 0);
        if (Ret != AL_OK) {
            if (Ret == AL_ERR_UNAVAILABLE) {
                /* do something else */
                continue;
            } else {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Wait done error:0x%x\r\n", Ret);
                return Ret;
            }
        } else {
            if (Event.EventId != AL_DMACAHB_EVENT_TRANS_COMP) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "Event error:0x%x\r\n", Ret);
                return AL_DMACAHB_EVENTS_TO_ERRS(Event.EventId);
            } else {
                break;
            }
        }
    }

    Ret = memcmp((AL_VOID *)(AL_UINTPTR)Handle->Channel.Trans.SrcAddr,
                 (AL_VOID *)(AL_UINTPTR)Handle->Channel.Trans.DstAddr, AL_DMACAHB_EX_ARRAY_SIZE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlDmacAhb_Hal_DeInit(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
        return Ret;
    }

    return Ret;
}
