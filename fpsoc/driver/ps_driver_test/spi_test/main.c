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
};

AL_U8 __attribute__((aligned(4))) SendData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) RecvData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) FlashId[10] = { 0x0 };

AL_U8 __attribute__((aligned(4))) DmaSendData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) DmaRecvData[500] = { 0x0 };

void AL_NOR_RESET(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_INFINEON_SRST;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_RESET error, ret:0x%x\r\n", ret);
    }
}

void AL_NOR_DMA_RESET(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_INFINEON_SRST;

    ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 1, 100000);
    if (AL_OK != ret) {
        printf("DMA AL_NOR_RESET error, ret:0x%x\r\n", ret);
    }
}

void AL_NOR_WREN(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_WREN;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_WREN error, ret:0x%x\r\n", ret);
    };
}

void AL_NOR_DMA_WREN(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_WREN;

    ret = AlSpi_Hal_DmaStartBlockSend(Handle, DmaSendData, 1, 100000);
    if (AL_OK != ret) {
        printf("DMA AL_NOR_WREN error, ret:0x%x\r\n", ret);
    }
}

void AL_NOR_SETSTATUS(AL_U8 data)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_WRSR;
    SendData[1] = data;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_SETSTATUS error, ret:0x%x\r\n", ret);
    }
}

void AL_NOR_WAITWIP(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDSR;

    do {
        ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
        if (AL_OK != ret) {
            printf("AL_NOR_WAITWIP error, ret:0x%x\r\n");
        }
#ifdef SPI_DEBUG
        printf("WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}

void AL_NOR_DMA_WAITWIP(void)
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

void AL_NOR_READSTATUS(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDSR;

    AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_READSTATUS error, ret:0x%x\r\n", ret);
    }
#ifdef SPI_DEBUG
    printf("Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
}


void AL_NOR_DMA_READSTATUS(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_RDSR;

    AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, DmaRecvData, 1, 100000);
    if (AL_OK != ret) {
        printf("DMA AL_NOR_READSTATUS error, ret:0x%x\r\n", ret);
    }
#ifdef SPI_DEBUG
    printf("Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
}

void AL_NOR_ERASE(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 4, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_ERASE error, ret:0x%x\r\n", ret);
    }
}


void AL_NOR_DMA_ERASE(void)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlSpi_Hal_DmaStartBlockSend(Handle, SendData, 4, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_ERASE error, ret:0x%x\r\n", ret);
    }
}


void AL_NOR_ERASECHIP(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_CE;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_ERASECHIP error, ret:0x%x\r\n", ret);
    }
}


void AL_NOR_READPAGE(void)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_READPAGE error, ret:0x%x\r\n", ret);
    }
}


void AL_NOR_DMA_READPAGE(void)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_READ;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0x20;
    DmaSendData[3] = 0;

    ret = AlSpi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 4, DmaRecvData, 240, 100000);
    if (AL_OK !=ret) {
        printf("DMA AL_NOR_READPAGE error, ret:0x%x\r\n", ret);
    }
}

void AL_NOR_READPAGE1(void)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;

    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0x10;
    SendData[3] = 0;

    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (AL_OK !=ret) {
        printf("AL_NOR_READPAGE error, ret:0x%x\r\n", ret);
    }
}


void AL_NOR_WRITEPAGE(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (AL_OK !=ret) {
        printf("AL_NOR_WRITEPAGE error ret:0x%x\r\n", ret);
    }
}


void AL_NOR_DMA_WRITEPAGE(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    DmaSendData[0] = NOR_OP_PP;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0x20;
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

void AL_NOR_WRITEPAGE1(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0x10;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
       SendData[i + 4] = i % 255;
    }
    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (AL_OK !=ret) {
        printf("AL_NOR_WRITEPAGE error, ret:0x%x\r\n", ret);
    }
}

AL_VOID AL_NOR_READID(AL_VOID)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDID;
    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, FlashId, 3, 100000);
    if (AL_OK !=ret) {
        printf("AL_NOR_READID error, ret:0x%x\r\n", ret);
    }
    printf("Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
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

void main(void)
{
    AL_U32 i;
    AL_S32 ret = AL_OK;

    *(uint32_t *)(0xf88030b8u) =0x9;      //spi1 mio46-49
    *(uint32_t *)(0xf88030bcu) =0x9;
    *(uint32_t *)(0xf88030c0u) =0x9;
    *(uint32_t *)(0xf88030c4u) =0x9;
    *(uint32_t *)(0xf8803428u) =0x1;      //emio_sel9


    *(uint32_t *)(0xf8803040u) =0x9;    //spi0  MIO16
    *(uint32_t *)(0xf8803044u) =0x9;    //spi0  MIO17
    *(uint32_t *)(0xf8803048u) =0x9;    //spi0  MIO18
    *(uint32_t *)(0xf880304cu) =0x4;    //GPIO MIO19
    *(uint32_t *)(0xf8803050u) =0x4;    //GPIO MIO20

    *(uint32_t *)(0xf8803054u) =0x9;    //spi0  MIO21
    *(uint32_t *)(0xf8803424u) =0x1;    //emio_sel9

#define AL_SPI_RUN_INTR
#ifdef AL_SPI_RUN_INTR
    printf("Start FPSoc Spi Test\r\n");

    ret = AlSpi_Hal_Init(&Handle, &SpiInitConfigs, AL_NULL, 1);
    if (AL_OK != ret) {
        printf("AlSpi_Hal_Init error, ret:0x%x\r\n", ret);
    }

    /**/
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    /**/
    AL_NOR_READID();

    /**/
    AL_NOR_WREN();
    AL_NOR_READSTATUS();
    AL_NOR_ERASE();
    AL_NOR_WAITWIP();

    /**/
    AL_NOR_READPAGE();
    for (i = 0; i < 240; i++) {
        if(0xff != RecvData[i]) {
            printf("AlSpi test erase norflash error\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    printf("AlSpi test erase norflash success\r\n");

    /**/
    AL_NOR_WREN();
    AL_NOR_WRITEPAGE();
    AL_NOR_WAITWIP();

    AL_NOR_WREN();
    AL_NOR_WRITEPAGE1();
    AL_NOR_WAITWIP();

    AL_NOR_READPAGE();
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            printf("AlSpi data write norflash test error, ret:0x%x\r\n", ret);
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    AL_NOR_READPAGE1();
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            printf("AlSpi data write1 norflash test error, ret:0x%x\r\n", ret);
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    printf("AlSpi test write norflash success\r\n");

#endif

// #define AL_SPI_RUN_DMA
#ifdef AL_SPI_RUN_DMA
    printf("Start FPSoc Spi AL_SPI_RUN_DMA Test\r\n");

    ret = AlSpi_Hal_Init(&Handle, &SpiInitConfigs, AL_NULL, 1);
    if (ret != AL_OK) {
        printf("AlSpi_Hal_Init error, ret:0x%x\r\n", ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_NOR_DMA_RESET();
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

#endif

// #define AL_SPI_SLAVE_TEST
#ifdef AL_SPI_SLAVE_TEST


#endif

// #define AL_SPI_DMA_SLAVE_TEST
#ifdef AL_SPI_DMA_SLAVE_TEST


#endif

}

