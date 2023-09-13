/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_qspi_hal.h"
#include "al_spinor.h"

AL_QSPI_HalStruct *Handle;

/* When SpiFrameFormat is Dual Quad,TransMode can only use QSPI_TX_ONLY,QSPI_RX_ONLY mode */
AL_QSPI_ConfigsStruct QspiInitConfigs =
{
    .ClkDiv             = 20,
    .SamplDelay         = 0,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SpiFrameFormat     = SPI_STANDARD_FORMAT,
    .ClockStretch       = QSPI_EnableClockStretch
};

AL_QSPI_ConfigsStruct QspiX4InitConfigs =
{
    .ClkDiv             = 20,
    .SamplDelay         = 2,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SpiFrameFormat     = SPI_QUAD_FORMAT,
    .ClockStretch       = QSPI_EnableClockStretch
};

AL_U8 __attribute__((aligned(4))) SendData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) RecvData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) FlashId[10] = { 0x0 };


void AL_NOR_RESET(void)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_INFINEON_SRST;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_RESET error:0x%x\r\n", Ret);
    }
}

void AL_NOR_RESET1(void)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_SRSTEN;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_RESET error:0x%x\r\n", Ret);
    }

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_SRST;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_RESET error:0x%x\r\n", Ret);
    }

}



void AL_NOR_WREN(void)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_WREN;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_WREN error:0x%x\r\n", Ret);
    }
}

void AL_NOR_SETSTATUS(AL_U8 data)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_WRSR;
    SendData[1] = data;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_SETSTATUS error:0x%x\r\n", Ret);
    }
}

void AL_NOR_WAITWIP(void)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    SendData[0] = NOR_OP_RDSR;

    do {
        Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
        if (Ret != AL_OK) {
            printf("AL_NOR_WAITWIP error:0x%x\r\n", Ret);
        }
#ifdef QSPI_DEBUG
        printf("WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}


void AL_NOR_READSTATUS(void)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_RDSR;

    AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_READSTATUS error:0x%x\r\n", Ret);
    }
// #ifdef QSPI_DEBUG
    printf("Nor Status1 Reg:%x\r\n", RecvData[0]);
// #endif
}


void AL_NOR_ERASE(void)
{
    AL_S32 Ret = AL_OK;

   // AlQspi_Dev_DumpReg(Handle->Dev.BaseAddr);

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 4, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_ERASE error:0x%x\r\n", Ret);
    }
}


void AL_NOR_ERASE1(void)
{
    AL_S32 Ret = AL_OK;

   // AlQspi_Dev_DumpReg(Handle->Dev.BaseAddr);

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;

    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 1;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 4, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_ERASE error:0x%x\r\n", Ret);
    }
}

void AL_NOR_ERASE2(void)
{
    AL_S32 Ret = AL_OK;

   // AlQspi_Dev_DumpReg(Handle->Dev.BaseAddr);

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;

    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 2;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 4, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_ERASE error:0x%x\r\n", Ret);
    }
}


void AL_NOR_ERASECHIP(void)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_CE;

    printf("AL_NOR_ERASECHIP running\r\n");
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_ERASECHIP error:0x%x\r\n", Ret);
    }
}


void AL_NOR_X4_1_1_4_READ_PAGE(AL_U8 addr)
{
    AL_S32 Ret = AL_OK;
    printf("AL_NOR_READPAGE\r\n");
    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
        Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;

    SendData[0] = NOR_OP_READ_1_1_4;
    SendData[1] = addr;
    SendData[2] = 0;
    SendData[3] = 0;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }
    printf("AL_NOR_READPAGE end\r\n");
}


void AL_NOR_WRITE_X4_1_1_4_PAGE(AL_U8 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_PP_1_1_4;
    SendData[1] = addr;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
        // printf
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }
}



void AL_NOR_X4_1_4_4_READ_PAGE(void)
{
    AL_S32 Ret = AL_OK;
    printf("AL_NOR_X4_1_4_4_READ_PAGE\r\n");
    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;

    SendData[0] = NOR_OP_READ_1_1_4;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }
    printf("AL_NOR_READPAGE end\r\n");
}

void AL_NOR_WRITE_X4_1_4_4_PAGE(void)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_PP_1_1_4;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }
}

void AL_NOR_X4_1_4_4_4B_READ_PAGE(void)
{
    AL_S32 Ret = AL_OK;
    printf("AL_NOR_READPAGE\r\n");
    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L32;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT1;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 6;

    SendData[0] = NOR_OP_READ_1_4_4_4B;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 2;
    SendData[4] = 0;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 5, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }
    printf("AL_NOR_READPAGE end\r\n");
}

void AL_NOR_WRITE_X4_1_1_4_4B_PAGE(void)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L32;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
        Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_PP_1_1_4_4B;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 2;
    SendData[4] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 5] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }
}


void AL_NOR_X4_1_1_4_4B_READ_PAGE(void)
{
    AL_S32 Ret = AL_OK;
    printf("AL_NOR_READPAGE\r\n");
    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L32;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;

    SendData[0] = NOR_OP_READ_1_1_4_4B;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 3;
    SendData[4] = 0;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 5, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }
    printf("AL_NOR_READPAGE end\r\n");
}

void AL_NOR_WRITE_X4_1_4_4_4B_PAGE(void)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L32;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_PP_1_1_4_4B;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 3;
    SendData[4] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 5] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }
}



void AL_NOR_READPAGE(void)
{
    AL_S32 Ret = AL_OK;
    // printf("AL_NOR_READPAGE\r\n");
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }
    // printf("AL_NOR_READPAGE end\r\n");
}

void AL_NOR_READPAGE1(void)
{
    AL_S32 Ret = AL_OK;
    printf("AL_NOR_READPAGE\r\n");
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0x10;
    SendData[3] = 0;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }
    printf("AL_NOR_READPAGE end\r\n");
}


void AL_NOR_WRITEPAGE(void)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }

    // AlQspi_Dev_DumpReg(Handle->Dev.HwConfig.BaseAddress);
}
void AL_NOR_WRITEPAGE1(void)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0x10;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
       SendData[i + 4] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }

    // AlQspi_Dev_DumpReg(Handle->Dev.HwConfig.BaseAddress);
}

AL_VOID AL_NOR_READID(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    SendData[0] = NOR_OP_RDID;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, FlashId, 3, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_READID error:0x%x\r\n", Ret);
    }
    printf("Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}


AL_VOID AL_NOR_REMOVE_PTOTECT(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 Temp;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendData[0] = NOR_OP_RDCR;
printf("AL_NOR_REMOVE_PTOTECT 0\r\n");
    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_REMOVE_PTOTECT1 error:0x%x\r\n", Ret);
    }
    Temp = RecvData[0];
printf("AL_NOR_REMOVE_PTOTECT 1\r\n");
    AL_NOR_WREN();

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    RecvData[2] = Temp | 0x2;
    RecvData[1] = 0;
    RecvData[0] = NOR_OP_WRSR;

    Ret = AlQspi_Hal_SendDataBlock(Handle, RecvData, 3, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_REMOVE_PTOTECT2 error:0x%x\r\n", Ret);
    }
    printf("AL_NOR_REMOVE_PTOTECT 2\r\n");
    AL_NOR_WAITWIP();

}




AL_VOID AL_NOR_MXIC_SET_QE(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 Temp;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendData[0] = NOR_OP_RDSR;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_MXIC_SET_QE error:0x%x\r\n", Ret);
    }
    Temp = RecvData[0];

    printf("AL_NOR_MXIC_SET_QE Status:%d\r\n", Temp);
    AL_NOR_WREN();

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    RecvData[1] = Temp | (1 << 6);
    RecvData[0] = NOR_OP_WRSR;

    Ret = AlQspi_Hal_SendDataBlock(Handle, RecvData, 2, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_MXIC_SET_QE error:0x%x\r\n", Ret);
    }

    AL_NOR_WAITWIP();

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendData[0] = NOR_OP_RDSR;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_MXIC_SET_QE error:0x%x\r\n", Ret);
    }

    printf("AL_NOR_MXIC_SET_QE Status:%d\r\n", RecvData[0]);

}



AL_S32 AL_NOR_SetQuad(AL_U8 SetQuadCmd, AL_U8 ReadQuadCmd, AL_U8 QuadPos)
{
    AL_S32 Data = 0, Ret = AL_OK;

    SendData[0] = ReadQuadCmd;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, &Data, 1, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_SetQuad ReadQuadCmd error:0x%x\r\n", Ret);
    }

    Data = Data | (1 << QuadPos);

    AL_NOR_WREN();

    SendData[0] = SetQuadCmd;
    SendData[1] = Data;

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (Ret != AL_OK) {
        printf("AL_NOR_SetQuad SetQuadCmd error:0x%x\r\n", Ret);
    }

    AL_NOR_WAITWIP();

    return Ret;
}


void main(void)
{
    AL_U32 i;
    AL_S32 Ret = AL_OK;

    printf("Start FPSoc Quad Qspi Test\r\n");

    Ret = AlQspi_Hal_Init(&Handle, &QspiX4InitConfigs, AL_NULL, 0);
    if (Ret != AL_OK) {
        printf("AlQspi_Hal_Init error:0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_NOR_READID();

    AL_NOR_READSTATUS();
    if((FlashId[0] != 0x01) && (FlashId[0] != 0x20) && (FlashId[0] != 0x0) && (FlashId[0] != 0xff)) {
        if( (FlashId[0] != 0x9d) && (FlashId[0] != 0xc2) ){
            Ret = AL_NOR_SetQuad(0x31, 0x35, 1);
        }else{
            Ret = AL_NOR_SetQuad(0x01, 0x05, 6);
        }

        if(Ret != AL_OK) {
            return Ret;
        }
    }

    AL_NOR_WREN();
    AL_NOR_ERASECHIP();
    AL_NOR_WAITWIP();

    AL_NOR_X4_1_1_4_READ_PAGE(0);
    for (i = 0; i < 240; i++) {
        if(0xff != RecvData[i]) {
            printf("AlQspi test erase norflash error\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
        }
    }
    printf("AlQspi test erase norflash success\r\n");

    AL_NOR_WREN();
    AL_NOR_WRITE_X4_1_1_4_PAGE(0);
    AL_NOR_WAITWIP();

    AL_NOR_X4_1_1_4_READ_PAGE(0);
    for (i = 0; i < 230; i++) {
        if (i != RecvData[i]) {
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
        }
    }
    printf("AL_NOR_WRITE_X4_1_1_4_PAGE test Success\r\n");
}

