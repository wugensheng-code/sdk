/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_dmacahb_test_config.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-05-06
 * @brief   dmac ahb test channel config file
 */

#ifndef AL_DMACAHB_TEST_CONFIG_H
#define AL_DMACAHB_TEST_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_dmacahb_hal.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_DMACAHB_TEST_MEM_SIZE            5
#define AL_DMACAHB_TEST_ARRAY_SIZE          256
#define AL_DMACAHB_RELOAD_COUNT_MAX         3
#define AL_DMACAHB_LLI_STRUCT_NUM           5

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/
/* same as AL_DMACAHB_TRANS_TYPE_5 */
static AL_DMACAHB_ChInitStruct ChSingleModeEg1 = {
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

static AL_DMACAHB_ChInitStruct ChAutoReloadModeEg1 = {
    .Id                     = AL_DMACAHB_CHANNEL_1,
    .TransType              = AL_DMACAHB_TRANS_TYPE_4,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_BLOCK | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_16,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_16,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_4,
    .DstBurstLength         = AL_DMACAHB_MSIZE_4,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_2,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_2,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_6,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

static AL_DMACAHB_ChInitStruct ChLlpModeEg1 = {
    .Id                     = AL_DMACAHB_CHANNEL_2,
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

/* Auto-reload multi-block transfer with contiguous SAR */
static AL_DMACAHB_ChInitStruct ChOtherTransTypeEg1 = {
    .Id                     = AL_DMACAHB_CHANNEL_0,
    .TransType              = AL_DMACAHB_TRANS_TYPE_2,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_BLOCK | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_16,
    .DstBurstLength         = AL_DMACAHB_MSIZE_16,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_0,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

/* Auto-reload multi-block transfer with contiguous DAR */
static AL_DMACAHB_ChInitStruct ChOtherTransTypeEg2 = {
    .Id                     = AL_DMACAHB_CHANNEL_1,
    .TransType              = AL_DMACAHB_TRANS_TYPE_3,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_BLOCK | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_32,
    .DstBurstLength         = AL_DMACAHB_MSIZE_32,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_1,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

/* Linked list multi-block transfer with contiguous SAR */
static AL_DMACAHB_ChInitStruct ChOtherTransTypeEg3 = {
    .Id                     = AL_DMACAHB_CHANNEL_2,
    .TransType              = AL_DMACAHB_TRANS_TYPE_6,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_32,
    .DstBurstLength         = AL_DMACAHB_MSIZE_32,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_2,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

/* Linked list multi-block transfer with auto-reload SAR */
static AL_DMACAHB_ChInitStruct ChOtherTransTypeEg4 = {
    .Id                     = AL_DMACAHB_CHANNEL_3,
    .TransType              = AL_DMACAHB_TRANS_TYPE_7,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_BLOCK | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_128,
    .DstBurstLength         = AL_DMACAHB_MSIZE_128,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_3,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

/* Linked list multi-block transfer with contiguous DAR */
static AL_DMACAHB_ChInitStruct ChOtherTransTypeEg5 = {
    .Id                     = AL_DMACAHB_CHANNEL_4,
    .TransType              = AL_DMACAHB_TRANS_TYPE_8,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_256,
    .DstBurstLength         = AL_DMACAHB_MSIZE_256,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_4,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

/* Linked list multi-block transfer with auto-reload DAR */
static AL_DMACAHB_ChInitStruct ChOtherTransTypeEg6 = {
    .Id                     = AL_DMACAHB_CHANNEL_5,
    .TransType              = AL_DMACAHB_TRANS_TYPE_9,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_BLOCK | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_256,
    .DstBurstLength         = AL_DMACAHB_MSIZE_256,
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

#ifdef __cplusplus
}
#endif

#endif
