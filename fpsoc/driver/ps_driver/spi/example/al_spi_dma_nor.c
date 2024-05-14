/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_spi_hal.h"
#include "al_spinor.h"
#include "al_dmacahb_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
AL_SPI_HalStruct *Handle;

AL_SPI_ConfigsStruct SpiInitConfigs =
{
    .Mode               = SPI_MASTER_MODE,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
};

AL_U8 CACHE_LINE_ALIGN FlashId[10] = { 0x0 };

AL_U8 CACHE_LINE_ALIGN DmaSendData[500] = { 0x0 };
AL_U8 CACHE_LINE_ALIGN DmaRecvData[500] = { 0x0 };

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
AL_VOID AlNorDma_Reset(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_INFINEON_SRST;

    ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 1, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_Reset error, ret:0x%x\r\n", ret);
    }
}


AL_VOID AlNorDma_Wren(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_WREN;

    ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 1, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_Wren error, ret:0x%x\r\n", ret);
    }
}

AL_VOID AlNorDma_WaitWip(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_RDSR;

    do {
        ret = AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, DmaRecvData, 1, 100000);
        if (AL_OK !=ret) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_WaitWip error, ret:0x%x\r\n", ret);
        }
#ifdef SPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_ERROR, "WAITWIP Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
    } while (DmaRecvData[0] & SR_WIP);
}

AL_VOID AlNorDma_ReadStatus(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_RDSR;

    AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, DmaRecvData, 1, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_ReadStatus error, ret:0x%x\r\n", ret);
    }
#ifdef SPI_DEBUG
    AL_LOG(AL_LOG_LEVEL_ERROR, "Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
}


AL_VOID AlNorDma_Erase(AL_VOID)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_SE;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0;
    DmaSendData[3] = 0;

    ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 4, 100000);
    if (AL_OK != ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_Erase error, ret:0x%x\r\n", ret);
    }
}


AL_VOID AlNorDma_ReadPage(AL_VOID)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_READ;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0;
    DmaSendData[3] = 0;

    ret = AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 4, DmaRecvData, 240, 100000);
    if (AL_OK !=ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_ReadPage error, ret:0x%x\r\n", ret);
    }
}


AL_VOID AlNorDma_WritePage(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_PP;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0;
    DmaSendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        DmaSendData[i + 4] = i % 255;
    }
    ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 240, 100000);
    if (AL_OK !=ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_WritePage error, ret:0x%x\r\n", ret);
    }
}

AL_VOID AlNorDma_ReadId(AL_VOID)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_RDID;;

    ret = AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, FlashId, 3, 100000);
    if (AL_OK !=ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNorDma_ReadId error, ret:0x%x\r\n", ret);
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA Read Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}

AL_VOID main(AL_VOID)
{
    AL_U32 i;
    AL_S32 ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_ERROR, "Start FPSoc Spi AL_SPI_RUN_DMA Test\r\n");

    ret = AlSpi_Hal_Init(&Handle, &SpiInitConfigs, AL_NULL, 0);
    if (ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi_Hal_Init error, ret:0x%x\r\n", ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    // AlNorDma_Reset();
    /**/
    AlNorDma_ReadId();

    /**/
    AlNorDma_Wren();
    AlNorDma_ReadStatus();
    AlNorDma_Erase();
    AlNorDma_WaitWip();

    /**/
    AlNorDma_ReadPage();
    for (i = 0; i < 240; i++) {
        if(0xff != DmaRecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlSpi test erase norflash error, ret:0x%x\r\n", ret);
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error DmaRecvData[%d]:%d\r\n", i, DmaRecvData[i]);
            while (1);
        }
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlSpi test erase norflash success\r\n");

    /**/
    AlNorDma_Wren();
    AlNorDma_WritePage();
    AlNorDma_WaitWip();

    AlNorDma_ReadPage();
    for (i = 0; i < 230; i++) {
        if(i != DmaRecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlSpi data write norflash test error, ret:0x%x\r\n", ret);
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error DmaRecvData[%d]:%d\r\n", i, DmaRecvData[i]);
            while (1);
        }
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlSpi test write norflash success\r\n");
}

