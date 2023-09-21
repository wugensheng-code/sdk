/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "al_dma_hal.h"

#define I2S_BASE_ADDR 0xA0000000

#define I2S_RESET_REG       (0x0)
#define I2S_CTRL_REG        (0x4)
#define I2S_CLK_CTRL_REG    (0x8)
#define I2S_PERIOD_LEN_REG  (0x24)
#define I2S_DATA_RX_REG     (0x28)
#define I2S_DATA_TX_REG     (0x2c)

#define TOP_NS_PLS_PROT_REG         (0x80)
#define TOP_NS_AXI_DMA_CTRL_0_REG   (0x1A0)
#define TOP_NS_AXI_DMA_CTRL_1_REG   (0x1A4)

#define TOP_CRP_SRST_1_ADDR         (0x74)

#define TOP_S_AXI_DMA_CTRL_2_REG    (0xA0)

extern AL_S32 AlDma_Hal_MemcpyBlock(AL_DMA_HalStruct *Handle, AL_UINTPTR Dst, AL_UINTPTR Src, AL_U32 Len);

AL_S32 AlDma_Test_MemCpy(AL_VOID);
AL_S32 AlDma_Test_PerCpy(AL_VOID);

AL_S32 main(AL_VOID)
{
#if (defined AL_DMA_TEST_MEMCPY) || (defined AL_DMA_TEST_MEMCPY_BLOCK)
    AlDma_Test_MemCpy();
#endif

#if (defined AL_DMA_TEST_PERCPY) || (defined AL_DMA_TEST_PERCPY_CYCLE)
    AlDma_Test_PerCpy();
#endif

    while (1);
}

AL_S32 AlDma_Test_MemCpy(AL_VOID)
{
    AL_U32 Idx;
    AL_U32 *Src = memalign(64, 1024 * sizeof(AL_U32));
    AL_U32 *Dst = memalign(64, 1024 * sizeof(AL_U32));
    AL_S32 Ret;

    for (Idx = 0 ; Idx < 1024; Idx ++) {
        Src[Idx] = Idx;
    }

    AL_DMA_ChCfgStruct Config = {
        .Direction = DMA_MEM_TO_MEM,
        .SrcAddrWidth = DMA_CH_BUSWIDTH_32_BYTES,
        .DstAddrWidth = DMA_CH_BUSWIDTH_32_BYTES,
        .SrcMaxBurst  = 16,
        .DstMaxBurst  = 4,
    };

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
#ifdef AL_DMA_TEST_MEMCPY_BLOCK
    AL_DMA_HalStruct *Handle = AL_NULL;
    AlDma_Hal_Init(&Handle, 0, &Config);
    AL_LOG(AL_LOG_LEVEL_INFO, "start copy from %p to %p \r\n", Src, Dst);
    AlDma_Hal_MemCpyBlock(Handle, Dst, Src, 1024 * 4);
#endif
#ifdef AL_DMA_TEST_MEMCPY
    AlDma_Hal_MemCpy(Dst, Src, 1024 * 4);
#endif
    Ret = memcmp(Dst, Src, 1024 * 4);
    AL_LOG(AL_LOG_LEVEL_INFO, "Ret = %d \r\n", Ret);

    return Ret;
}

AL_S32 AlDma_Test_PerCpy(AL_VOID)
{
    AL_U32 Idx;
    AL_S32 Ret = AL_OK;
    AL_DMA_HalStruct *M2PHandle = AL_NULL;
    AL_DMA_HalStruct *P2MHandle = AL_NULL;
    AL_U32 * Src = memalign(64, 1024 * sizeof(AL_U32));
    AL_U32 * Dst = memalign(64, 1024 * sizeof(AL_U32));
    AL_MailBox M2PEvent, P2MEvent;

    for (Idx = 0 ; Idx < 1024; Idx ++) {
        /* high 24 bit write in I2S, low 8 bit for time stamp */
        Src[Idx] = (Idx << 8);
        Dst[Idx] = 0;
    }

    AL_DMA_ChCfgStruct M2PConfig = {
        .Direction = DMA_MEM_TO_DEV,
        .SrcAddrWidth = DMA_CH_BUSWIDTH_4_BYTES,
        .DstAddrWidth = DMA_CH_BUSWIDTH_4_BYTES,
        .SrcMaxBurst  = 1,
        .DstMaxBurst  = 1,
        .DstAddr      = I2S_BASE_ADDR + I2S_DATA_TX_REG,
    };

    AL_DMA_ChCfgStruct P2MConfig = {
        .Direction = DMA_DEV_TO_MEM,
        .SrcAddrWidth = DMA_CH_BUSWIDTH_4_BYTES,
        .DstAddrWidth = DMA_CH_BUSWIDTH_4_BYTES,
        .SrcMaxBurst  = 1,
        .DstMaxBurst  = 1,
        .SrcAddr      = I2S_BASE_ADDR + I2S_DATA_RX_REG,
    };

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlDma_Hal_Init(&M2PHandle, 0, &M2PConfig);
    AlDma_Hal_Init(&P2MHandle, 0, &P2MConfig);

    AL_LOG(AL_LOG_LEVEL_INFO, "start copy from %p to %x \r\n", Src, I2S_BASE_ADDR + I2S_DATA_TX_REG);

#ifdef AL_DMA_TEST_PERCPY
    Ret = AlDma_Hal_PeriCpySingle(M2PHandle, Src, 1024 * 4, 0);
#else
    Ret = AlDma_Hal_PeriCpyCycle(M2PHandle, Src, 1024 * 4, 0);
#endif
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "M2P Start error: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "start copy from %x to %p \r\n", I2S_BASE_ADDR + I2S_DATA_RX_REG, Dst);

#ifdef AL_DMA_TEST_PERCPY
    Ret = AlDma_Hal_PeriCpySingle(P2MHandle, Dst, 1024 * 4, 3);
#else
    Ret = AlDma_Hal_PeriCpyCycle(P2MHandle, Dst, 1024 * 4, 3);
#endif
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "P2M Start error: 0x%x\r\n", Ret);
    }

    /* I2S config */
    AL_REG32_WRITE(TOP_NS_BASE_ADDR + TOP_NS_PLS_PROT_REG, 0x00000001);  /* Close gp port protect */
    AL_REG32_WRITE(I2S_BASE_ADDR + I2S_RESET_REG, 0x00000007);  /* Reset assert I2S */
    AL_REG32_WRITE(I2S_BASE_ADDR + I2S_RESET_REG, 0x00000000);  /* Reset release I2S */
    AL_REG32_WRITE(I2S_BASE_ADDR + I2S_CLK_CTRL_REG, 0x00000000);  /*  */
    AL_REG32_WRITE(I2S_BASE_ADDR + I2S_PERIOD_LEN_REG, 0x00000000);  /*  */
    AL_REG32_WRITE(I2S_BASE_ADDR + I2S_CTRL_REG, 0x00000003);  /* Request DMA trans */

    AL_LOG(AL_LOG_LEVEL_INFO, "Request DMA trans\r\n");

    Ret = AlOsal_Mb_Receive(&M2PHandle->EventQueue, (AL_VOID *)&M2PEvent, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "M2P Trans error: 0x%x\r\n", Ret);
    }

    Ret = AlOsal_Mb_Receive(&P2MHandle->EventQueue, (AL_VOID *)&P2MEvent, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "P2M Trans error: 0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "DMA trans Done\r\n");

#if (defined AL_DMA_TEST_PERCPY_CYCLE) && (defined AL_DMA_TEST_PERCPY_CYCLE_STOP)
    AlDma_Hal_StopCpy(M2PHandle);
    AlDma_Hal_StopCpy(P2MHandle);
#endif

    for (Idx = 0 ; Idx < 1024; Idx ++) {
        /* drop low 8 bit time stamp */
        Dst[Idx] &= 0xFFFFFF00;
    }

    Ret = memcmp(Dst, Src, 1024 * 4);
    AL_LOG(AL_LOG_LEVEL_INFO, "Ret = %d \r\n", Ret);

    return Ret;
}