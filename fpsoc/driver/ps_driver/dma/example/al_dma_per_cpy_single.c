/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_dma_per_cpy_single.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   axi dma peri cpy single example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <malloc.h>
#include "al_dma_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_DMA_EX_DATA_SIZE             (1024)
#define AL_DMA_EX_TRANS_TIMEOUT         (1000)

#define I2S_TX_ID   (0)
#define I2S_RX_ID   (3)

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

/************************** Variable Definitions *****************************/
AL_DMA_ChCfgStruct M2PConfig = {
    .Direction      = DMA_MEM_TO_DEV,
    .SrcAddrWidth   = DMA_CH_BUSWIDTH_4_BYTES,
    .DstAddrWidth   = DMA_CH_BUSWIDTH_4_BYTES,
    .SrcMaxBurst    = 1,
    .DstMaxBurst    = 1,
    .DstAddr        = I2S_BASE_ADDR + I2S_DATA_TX_REG,
};

AL_DMA_ChCfgStruct P2MConfig = {
    .Direction      = DMA_DEV_TO_MEM,
    .SrcAddrWidth   = DMA_CH_BUSWIDTH_4_BYTES,
    .DstAddrWidth   = DMA_CH_BUSWIDTH_4_BYTES,
    .SrcMaxBurst    = 1,
    .DstMaxBurst    = 1,
    .SrcAddr        = I2S_BASE_ADDR + I2S_DATA_RX_REG,
};

/************************** Function Prototypes ******************************/
static AL_S32 AlDma_Test_PerCpySingle(AL_VOID);

/************************** Function Definitions ******************************/
/**
 * This function initializes the test, executes it, and logs the result.
 * If the test fails at any point, it logs an error message and returns the error code.
 *
 * @return AL_OK if the test passes, otherwise returns an error code.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac per cpy single test\r\n");

    Ret = AlDma_Test_PerCpySingle();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dmac per cpy single test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmac per cpy single test success\r\n");

    return Ret;
}

/**
 * This function performs a single peripheral copy operation using DMA. It initializes DMA handles for memory-to-peripheral
 * and peripheral-to-memory transfers, sets up local interrupts, performs the data copying, checks the integrity of the copied data,
 * and cleans up resources. It also handles DMA transfer errors and data integrity errors by logging appropriate error messages
 * and returning error codes.
 *
 * @return AL_OK if the test and data verification pass, otherwise returns an error code.
 */
static AL_S32 AlDma_Test_PerCpySingle(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_DMA_HalStruct *M2PHandle = AL_NULL;
    AL_DMA_HalStruct *P2MHandle = AL_NULL;
    AL_MailBox M2PEvent, P2MEvent;
    AL_U32 *Src = memalign(CACHE_LINE_SIZE, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
    AL_U32 *Dst = memalign(CACHE_LINE_SIZE, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));

    AL_LOG(AL_LOG_LEVEL_INFO, "Copy from %p to %p \r\n", Src, Dst);

    Ret = AlDma_Hal_Init(&M2PHandle, 0, &M2PConfig);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "M2P hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlDma_Hal_Init(&P2MHandle, 0, &P2MConfig);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "P2M hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    /* I2S config */
    AL_REG32_WRITE(TOP_NS_BASE_ADDR + TOP_NS_PLS_PROT_REG, 0x00000001); /* Close gp port protect */
    AL_REG32_WRITE(I2S_BASE_ADDR + I2S_RESET_REG, 0x00000007);          /* Reset assert I2S */
    AL_REG32_WRITE(I2S_BASE_ADDR + I2S_RESET_REG, 0x00000000);          /* Reset release I2S */
    AL_REG32_WRITE(I2S_BASE_ADDR + I2S_CLK_CTRL_REG, 0x00000000);       /*  */
    AL_REG32_WRITE(I2S_BASE_ADDR + I2S_PERIOD_LEN_REG, 0x00000000);     /*  */

    while (1) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Loop: 0x%d\r\n", InitData++);

        for (AL_U32 i = 0; i < AL_DMA_EX_DATA_SIZE; i++) {
            Src[i] = (i + InitData) << 8;
        }

        Ret = AlDma_Hal_PeriCpySingle(M2PHandle, (AL_UINTPTR)Src, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32), I2S_TX_ID);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "M2P mem copy error:0x%x\r\n", Ret);
            return Ret;
        }

        Ret = AlDma_Hal_PeriCpySingle(P2MHandle, (AL_UINTPTR)Dst, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32), I2S_RX_ID);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "M2P mem copy error:0x%x\r\n", Ret);
            return Ret;
        }

        AL_REG32_WRITE(I2S_BASE_ADDR + I2S_CTRL_REG, 0x00000003);           /* Request DMA trans */

        AL_LOG(AL_LOG_LEVEL_INFO, "Request DMA trans\r\n");

        Ret = AlOsal_Mb_Receive(&M2PHandle->EventQueue, (AL_VOID *)&M2PEvent, AL_DMA_EX_TRANS_TIMEOUT);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "M2P Trans error: 0x%x\r\n", Ret);
        }

        Ret = AlOsal_Mb_Receive(&P2MHandle->EventQueue, (AL_VOID *)&P2MEvent, AL_DMA_EX_TRANS_TIMEOUT);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "P2M Trans error: 0x%x\r\n", Ret);
        }

        AL_LOG(AL_LOG_LEVEL_INFO, "DMA trans Done\r\n");

        for (AL_U32 i = 0 ; i < AL_DMA_EX_DATA_SIZE; i++) {
            /* drop low 8 bit time stamp */
            Dst[i] &= 0xFFFFFF00;
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

    Ret = AlDma_Hal_DeInit(M2PHandle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "M2P deinit error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlDma_Hal_DeInit(P2MHandle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "M2P deinit error:0x%x\r\n", Ret);
        return Ret;
    }

    free(Src);
    free(Dst);

    return Ret;
}
