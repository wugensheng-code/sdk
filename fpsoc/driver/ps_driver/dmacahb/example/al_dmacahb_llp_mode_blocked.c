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
 * @brief   dmac ahb llp mode blocked example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_dmacahb_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_DMACAHB_EX_DEVICE_ID             (0)
#define AL_DMACAHB_EX_BLOCKED_TIMEOUT_IN_MS (100)
#define AL_DMACAHB_EX_LINK_LIST_NUM         (3)
#define AL_DMACAHB_EX_ARRAY_SIZE            (256)

/************************** Variable Definitions *****************************/
static AL_DMACAHB_ChInitStruct ChInitCfg = {
    .Id                     = AL_DMACAHB_CHANNEL_0,
    .TransType              = AL_DMACAHB_TRANS_TYPE_10,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_8,
    .DstBurstLength         = AL_DMACAHB_MSIZE_8,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_5,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

/************************** Function Prototypes ******************************/
static AL_S32 AlDmacAhb_Test_LlpModeBlocked(AL_VOID);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac ahb llp mode blocked test\r\n");

    Ret = AlDmacAhb_Test_LlpModeBlocked();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dmac ahb llp mode blocked test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac ahb llp mode blocked test success\r\n");

    return Ret;
}

static AL_S32 AlDmacAhb_Test_LlpModeBlocked(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U8 InitData = 0;
    AL_DMACAHB_HalStruct *Handle = AL_NULL;
    AL_DMACAHB_ChTransStruct *Trans;
    AL_U32 TransSize = AL_DMACAHB_EX_ARRAY_SIZE;
    AL_U8 *MemSrc[AL_DMACAHB_EX_LINK_LIST_NUM];
    AL_U8 *MemDst[AL_DMACAHB_EX_LINK_LIST_NUM];
    AL_DMACAHB_LliStruct Lli[AL_DMACAHB_EX_LINK_LIST_NUM] CACHE_LINE_ALIGN;

    for (AL_U32 i = 0; i < AL_DMACAHB_EX_LINK_LIST_NUM; i++) {
        MemSrc[i] = (AL_U8 *)(AL_UINTPTR)memalign(CACHE_LINE_SIZE, AL_DMACAHB_EX_ARRAY_SIZE);
        MemDst[i] = (AL_U8 *)(AL_UINTPTR)memalign(CACHE_LINE_SIZE, AL_DMACAHB_EX_ARRAY_SIZE);
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Aligned %d, Src:%p, Dst:%p\r\n", i, MemSrc[i], MemDst[i]);
    }

    Ret = AlDmacAhb_Hal_Init(&Handle, AL_DMACAHB_EX_DEVICE_ID, &ChInitCfg, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    for (AL_U32 i = 0; i < AL_DMACAHB_EX_LINK_LIST_NUM; i++) {
        Lli[i].SrcAddr = (AL_REG)MemSrc[i];
        Lli[i].DstAddr = (AL_REG)MemDst[i];

        AlDmacAhb_Dev_FillLliWithCtl(&(Handle->Channel), &Lli[i]);
        Lli[i].CtlHigh.Bit.BlockTransSize = TransSize / (1 << ChInitCfg.SrcTransWidth);

        if ((i + 1) == AL_DMACAHB_EX_LINK_LIST_NUM) {
            Lli[i].LlpNext = (AL_REG)AL_NULL;
            Lli[i].CtlLow.Bit.LlpSrcEn = AL_FALSE;
            Lli[i].CtlLow.Bit.LlpDstEn = AL_FALSE;
        } else {
            Lli[i].LlpNext = (AL_REG)&Lli[i + 1];
            Lli[i].CtlLow.Bit.LlpSrcEn = AL_TRUE;
            Lli[i].CtlLow.Bit.LlpDstEn = AL_TRUE;
        }
    }

    Trans = &(Handle->Channel.Trans);
    Trans->Lli = Lli;

    while (1) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Loop: 0x%d\r\n", InitData/AL_DMACAHB_EX_LINK_LIST_NUM);

        for (AL_U32 i = 0; i < AL_DMACAHB_EX_LINK_LIST_NUM; i++) {
            memset((AL_VOID *)(AL_UINTPTR)Lli[i].SrcAddr, InitData++, AL_DMACAHB_EX_ARRAY_SIZE);
        }

        Ret = AlDmacAhb_Hal_StartBlock(Handle, AL_DMACAHB_EX_BLOCKED_TIMEOUT_IN_MS);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
            return Ret;
        }

        for (AL_U32 i = 0; i < AL_DMACAHB_EX_LINK_LIST_NUM; i++) {
            Ret = memcmp((AL_VOID *)(AL_UINTPTR)Lli[i].SrcAddr,
                         (AL_VOID *)(AL_UINTPTR)Lli[i].DstAddr, AL_DMACAHB_EX_ARRAY_SIZE);
            if (Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "List %d data check error:0x%x\r\n", i, Ret);
                return Ret;
            }

            memset((AL_VOID *)(AL_UINTPTR)Lli[i].DstAddr, 0, AL_DMACAHB_EX_ARRAY_SIZE);
            #ifdef ENABLE_MMU
            AlCache_FlushDcacheRange((AL_UINTPTR)Lli[i].DstAddr,
                                     (AL_UINTPTR)(Lli[i].DstAddr + AL_DMACAHB_EX_ARRAY_SIZE));
            #endif
        }
    }

    Ret = AlDmacAhb_Hal_DeInit(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
        return Ret;
    }

    for (AL_U32 i = 0; i < AL_DMACAHB_EX_LINK_LIST_NUM; i++) {
        free(MemSrc[i]);
        free(MemDst[i]);
    }

    return Ret;
}
