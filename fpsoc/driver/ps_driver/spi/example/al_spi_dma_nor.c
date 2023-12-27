/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_spi_hal.h"
#include "al_spinor.h"

#include "al_dmacahb_hal.h"

AL_SPI_HalStruct *Handle;

AL_SPI_ConfigsStruct SpiInitConfigs =
{
    .Mode               = SPI_MASTER_MODE,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
    .ClkDiv             = 40
};


AL_U8 CACHE_LINE_ALIGN FlashId[10] = { 0x0 };

AL_U8 CACHE_LINE_ALIGN DmaSendData[500] = { 0x0 };
AL_U8 CACHE_LINE_ALIGN DmaRecvData[500] = { 0x0 };


AL_VOID AL_NOR_DMA_RESET(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_INFINEON_SRST;

    ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 1, 100000);
    if (AL_OK != ret) {
        printf("DMA AL_NOR_RESET error, ret:0x%x\r\n", ret);
    }
}


AL_VOID AL_NOR_DMA_WREN(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_WREN;

    ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 1, 100000);
    if (AL_OK != ret) {
        printf("DMA AL_NOR_WREN error, ret:0x%x\r\n", ret);
    }
}

AL_VOID AL_NOR_DMA_WAITWIP(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_RDSR;

    do {
        ret = AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, DmaRecvData, 1, 100000);
        if (AL_OK !=ret) {
            printf("AL_NOR_WAITWIP error, ret:0x%x\r\n", ret);
        }
#ifdef SPI_DEBUG
        printf("WAITWIP Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
    } while (DmaRecvData[0] & SR_WIP);
}

AL_VOID AL_NOR_DMA_READSTATUS(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_RDSR;

    AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, DmaRecvData, 1, 100000);
    if (AL_OK != ret) {
        printf("DMA AL_NOR_READSTATUS error, ret:0x%x\r\n", ret);
    }
// #ifdef SPI_DEBUG
    printf("Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
// #endif
}


AL_VOID AL_NOR_DMA_ERASE(AL_VOID)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_SE;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0;
    DmaSendData[3] = 0;

    ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 4, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_ERASE error, ret:0x%x\r\n", ret);
    }
}


AL_VOID AL_NOR_DMA_READPAGE(AL_VOID)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_READ;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0;
    DmaSendData[3] = 0;

    ret = AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 4, DmaRecvData, 240, 100000);
    if (AL_OK !=ret) {
        printf("DMA AL_NOR_READPAGE error, ret:0x%x\r\n", ret);
    }
}


AL_VOID AL_NOR_DMA_WRITEPAGE(AL_VOID)
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
        printf("DMA AL_NOR_WRITEPAGE error, ret:0x%x\r\n", ret);
    }
}

AL_VOID AL_NOR_DMA_READID(AL_VOID)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_RDID;;

    ret = AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, FlashId, 3, 100000);
    if (AL_OK !=ret) {
        printf("AL_NOR_READID error, ret:0x%x\r\n", ret);
    }

    printf("DMA Read Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}

AL_VOID main(AL_VOID)
{
    AL_U32 i;
    AL_S32 ret = AL_OK;

    printf("Start FPSoc Spi AL_SPI_RUN_DMA Test\r\n");

    ret = AlSpi_Hal_Init(&Handle, &SpiInitConfigs, AL_NULL, 0);
    if (ret != AL_OK) {
        printf("AlSpi_Hal_Init error, ret:0x%x\r\n", ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    // AL_NOR_DMA_RESET();
    /**/
    AL_NOR_DMA_READID();

    /**/
    AL_NOR_DMA_WREN();
    AL_NOR_DMA_READSTATUS();
    AL_NOR_DMA_ERASE();
    AL_NOR_DMA_WAITWIP();

    /**/
    AL_NOR_DMA_READPAGE();
    for (i = 0; i < 240; i++) {
        if(0xff != DmaRecvData[i]) {
            printf("AlSpi test erase norflash error, ret:0x%x\r\n", ret);
            printf("Error DmaRecvData[%d]:%d\r\n", i, DmaRecvData[i]);
            while (1);
        }
    }

    printf("DMA AlSpi test erase norflash success\r\n");

    /**/
    AL_NOR_DMA_WREN();
    AL_NOR_DMA_WRITEPAGE();
    AL_NOR_DMA_WAITWIP();

    AL_NOR_DMA_READPAGE();
    for (i = 0; i < 230; i++) {
        if(i != DmaRecvData[i]) {
            printf("DMA AlSpi data write norflash test error, ret:0x%x\r\n", ret);
            printf("Error DmaRecvData[%d]:%d\r\n", i, DmaRecvData[i]);
            while (1);
        }
    }
    printf("DMA AlSpi test write norflash success\r\n");
}

