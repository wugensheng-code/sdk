#include <common.h>
#include <al_spinor.h>

AL_U8 SendData[500] = { 0x0 };
AL_U8 RecvData[500] = { 0x0 };
AL_U8 FlashId[10] = { 0x0 };
AL_U8 InstAndAddr[10] = { 0x0 };


AL_U8 CACHE_LINE_ALIGN DmaSendData[500] = { 0x0 };
AL_U8 CACHE_LINE_ALIGN DmaRecvData[500] = { 0x0 };

/*******************************************************************************/
/**************************** al_qspi_x1_nor ***********************************/
/*******************************************************************************/

/************************** Variable Definitions *****************************/
AL_QSPI_HalStruct *Handle;


/* When SpiFrameFormat is Dual Quad,TransMode can only use QSPI_TX_ONLY,QSPI_RX_ONLY mode */
AL_QSPI_ConfigsStruct QspiInitConfigs =
{
    .SamplDelay         = 2,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SpiFrameFormat     = SPI_STANDARD_FORMAT,
    .ClockStretch       = QSPI_EnableClockStretch
};

AL_VOID AlNor_InfineonReset(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_INFINEON_SRST;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Reset error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_Reset(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_SRSTEN;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_RESET error:0x%x\r\n", Ret);
    }

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_SRST;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_RESET error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_Wren(AL_VOID)
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
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Wren error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_SetStatus(AL_U8 data)
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
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SETSTATUS error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_WaitWip(AL_VOID)
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
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WaitWip error:0x%x\r\n", Ret);
        }
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_ERROR, "WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}

AL_VOID AlNor_ReadStatus(AL_VOID)
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
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadStatus error:0x%x\r\n", Ret);
    }
// #ifdef QSPI_DEBUG
    AL_LOG(AL_LOG_LEVEL_ERROR, "Nor Status1 Reg:%x\r\n", RecvData[0]);
// #endif
}


AL_VOID AlNor_Erase(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    SendData[0] = NOR_OP_SE;
    SendData[1] = (addr >> 16) & 0xff;
    SendData[2] = (addr >> 8)&0xff;
    SendData[3] = addr&0xff;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 4, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Erase error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_EraseChip(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_CE;

    AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_EraseChip running\r\n");
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_EraseChip error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_ReadPage(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_READ;
    SendData[1] = (addr >> 16) & 0xff;
    SendData[2] = (addr >> 8)&0xff;
    SendData[3] = addr&0xff;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadPage error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_WritePage(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_PP;
    SendData[1] = (addr >> 16) & 0xff;
    SendData[2] = (addr >> 8)&0xff;
    SendData[3] = addr&0xff;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WritePage error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_ReadId(AL_VOID)
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
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadId error:0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}

AL_VOID qspi_test_x1_nor(AL_VOID)
{
    AL_U32 i;
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_ERROR, "Start FPSoc Qspi X1 Test\r\n");

    Ret = AlQspi_Hal_Init(&Handle, &QspiInitConfigs, AL_NULL, 0);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_Hal_Init error:0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlNor_ReadId();

    AlNor_Wren();
    AlNor_EraseChip();
    AlNor_WaitWip();

    AlNor_ReadPage(0);
    for (i = 0; i < 240; i++) {
        if(0xff != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            assert_int_equal(RecvData[i], 0xff);
            // while (1);
        }
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash success\r\n");

    /**/
    AlNor_Wren();
    AlNor_WritePage(0);
    AlNor_WaitWip();

    AlNor_ReadPage(0);
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi data write norflash test error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            // while (1);
            assert_int_equal(RecvData[i], i);
        }
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test write norflash success\r\n");
}

/*******************************************************************************/
/**************************** al_qspi_x4_nor ***********************************/
/*******************************************************************************/

AL_QSPI_ConfigsStruct QspiX4InitConfigs =
{
    .SamplDelay         = 2,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SpiFrameFormat     = SPI_QUAD_FORMAT,
    .ClockStretch       = QSPI_EnableClockStretch
};

AL_VOID AlNor_Reset_X4(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_INFINEON_SRST;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Reset error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_Wren_X4(AL_VOID)
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
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Wren error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_WaitWip_X4(AL_VOID)
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
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WaitWip error:0x%x\r\n", Ret);
        }
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_ERROR, "WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}

AL_VOID AlNor_ReadStatus_X4(AL_VOID)
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
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadStatus error:0x%x\r\n", Ret);
    }
#ifdef QSPI_DEBUG
    AL_LOG(AL_LOG_LEVEL_ERROR, "Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
}

AL_VOID AlNor_EraseChip_X4(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_CE;

    AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_EraseChip running\r\n");
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_EraseChip error:0x%x\r\n", Ret);
    }
}


AL_VOID AlNor_ReadPage_1_1_4(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;

    SendData[0] = NOR_OP_READ_1_1_4;
    SendData[1] = (addr >> 16) & 0xff;
    SendData[2] = (addr >> 8)&0xff;
    SendData[3] = addr&0xff;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_WritePage_1_1_4(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;
    AL_U32 i = 0;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_PP_1_1_4;
    SendData[1] = (addr >> 16) & 0xff;
    SendData[2] = (addr >> 8)&0xff;
    SendData[3] = addr&0xff;

    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }
}

AL_VOID AL_NOR_WRITEPAGE(AL_VOID)
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
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_ReadId_X4(AL_VOID)
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
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadId error:0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}

AL_S32 AlNor_SetQuad1ByteMode(AL_U8 SetQuadCmd, AL_U8 ReadQuadCmd, AL_U8 QuadPos)
{
    AL_S32  Ret = AL_OK;
    AL_U8 SendData[4] = {0x0}, Data = 0;

    SendData[0] = ReadQuadCmd;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, &Data, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad1ByteMode ReadQuadCmd error\r\n");
    }

    Data = Data | (1 << QuadPos);

    AlNor_Wren();

    SendData[0] = SetQuadCmd;
    SendData[1] = Data;

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad1ByteMode SetQuadCmd error\r\n");
    }

    AlNor_WaitWip();

    return Ret;
}

AL_S32 AlNor_SetQuad2ByteMode(AL_U8 SetQuadCmd, AL_U8 ReadQuadCmd1, AL_U8 ReadQuadCmd2, AL_U8 QuadPos)
{
    AL_S32  Ret = AL_OK;
    AL_U8 SendData[4] = {0x0}, RecvData = 0;

    SendData[0] = ReadQuadCmd1;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType  = QSPI_TT0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_EEPROM;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, &RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad2ByteMode ReadQuadCmd error\r\n");
    }
    SendData[1] = RecvData;

    SendData[0] = ReadQuadCmd2;
    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, &RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad2ByteMode ReadQuadCmd error\r\n");
    }
    SendData[2] = RecvData;

    if (QuadPos < 8)
        SendData[1] |= (1 << QuadPos);
    else
        SendData[2] |= (1 << QuadPos - 8);

    AlNor_Wren();

    SendData[0] = SetQuadCmd;

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 3, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad2ByteMode SetQuadCmd error\r\n");
    }

    AlNor_WaitWip();

    return Ret;
}



AL_VOID qspi_test_x4_nor(AL_VOID)
{
    AL_U32 i;
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_ERROR, "Start FPSoc Qspi X4 Test\r\n");

    Ret = AlQspi_Hal_Init(&Handle, &QspiX4InitConfigs, AL_NULL, 0);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_Hal_Init error:0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlNor_ReadId_X4();

    AlNor_ReadStatus_X4();

    if((FlashId[0] != 0x01) && (FlashId[0] != 0x20) && (FlashId[0] != 0x0) && (FlashId[0] != 0xff)) {
        if((FlashId[0] == 0xc8) && (FlashId[1] == 0x60)) {
            Ret = AlNor_SetQuad2ByteMode(0x1, 0x5, 0x35, 9);
        } else  if((FlashId[0] != 0x9d) && (FlashId[0] != 0xc2)) {
            Ret = AlNor_SetQuad1ByteMode(0x31, 0x35, 1);
        } else {
            Ret = AlNor_SetQuad1ByteMode(0x01, 0x05, 6);
        }
    }

    AlNor_Wren_X4();
    AlNor_EraseChip_X4();
    AlNor_WaitWip_X4();

    AlNor_ReadPage_1_1_4(0);
    for (i = 0; i < 240; i++) {
        if(0xff != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            assert_int_equal(RecvData[i], 0xff);
        }
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash success\r\n");

    AlNor_Wren_X4();
    AlNor_WritePage_1_1_4(0);
    AlNor_WaitWip_X4();

    AlNor_ReadPage_1_1_4(0);
    for (i = 0; i < 230; i++) {
        if (i != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            assert_int_equal(RecvData[i], i);
        }
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WritePage_1_1_4 test Success\r\n");
}

/*******************************************************************************/
/**************************** al_qspi_dma_nor ***********************************/
/*******************************************************************************/
AL_VOID AlNorDma_InfineonReset(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;

    InstAndAddr[0] = NOR_OP_INFINEON_SRST;

    Ret = AlQspi_Hal_DmaStartSendBlock(Handle, DmaSendData, InstAndAddr, 0, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AL_NOR_RESET error:0x%x\r\n", Ret);
    }
}


AL_VOID AlNorDma_Reset(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    InstAndAddr[0] = NOR_OP_SRSTEN;

    Ret = AlQspi_Hal_DmaStartSendBlock(Handle, DmaSendData, InstAndAddr, 0, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_RESET error:0x%x\r\n", Ret);
    }

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    InstAndAddr[0] = NOR_OP_SRST;

    Ret = AlQspi_Hal_DmaStartSendBlock(Handle, DmaSendData, InstAndAddr, 0, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_RESET error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNorDma_Wren(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    InstAndAddr[0] = NOR_OP_WREN;

    Ret = AlQspi_Hal_DmaStartSendBlock(Handle, DmaSendData, InstAndAddr, 0, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AL_NOR_WREN error:0x%x\r\n", Ret);
    }
}


AL_VOID AlNorDma_WaitWip(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    DmaSendData[0] = NOR_OP_RDSR;

    do {
        Ret = AlQspi_Hal_DmaStartTranferBlock(Handle, DmaSendData, 1, DmaRecvData, 1, 100000);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_WAITWIP error:0x%x\r\n", Ret);
        }
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_ERROR, "WAITWIP Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
    } while (DmaRecvData[0] & SR_WIP);
}

AL_VOID AlNorDma_ReadStatus(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;

    DmaSendData[0] = NOR_OP_RDSR;

    AlQspi_Hal_DmaStartTranferBlock(Handle, DmaSendData, 1, DmaRecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AL_NOR_READSTATUS error:0x%x\r\n", Ret);
    }
#ifdef QSPI_DEBUG
    AL_LOG(AL_LOG_LEVEL_ERROR, "Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
}

AL_VOID AlNorDma_Erase(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;

    InstAndAddr[0] = NOR_OP_SE;
    InstAndAddr[1] = (addr >> 16) & 0xff;
    InstAndAddr[2] = (addr >> 8)&0xff;
    InstAndAddr[3] = addr&0xff;

    Ret = AlQspi_Hal_DmaStartSendBlock(Handle, DmaSendData, InstAndAddr, 0, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_ERASE error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNorDma_ReadPage(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;

    DmaSendData[0] = NOR_OP_READ_1_1_4;
    DmaSendData[1] = (addr >> 16) & 0xff;
    DmaSendData[2] = (addr >> 8)&0xff;
    DmaSendData[3] = addr&0xff;

    Ret = AlQspi_Hal_DmaStartTranferBlock(Handle, DmaSendData, 4, DmaRecvData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNorDma_WritePage(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    InstAndAddr[0] = NOR_OP_PP_1_1_4;
    InstAndAddr[1] = (addr >> 16) & 0xff;
    InstAndAddr[2] = (addr >> 8)&0xff;
    InstAndAddr[3] = addr&0xff;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        DmaSendData[i] = i % 255;
    }
    Ret = AlQspi_Hal_DmaStartSendBlock(Handle, DmaSendData, InstAndAddr, 240, 1000000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNorDma_ReadId(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;

    DmaSendData[0] = NOR_OP_RDID;;

    Ret = AlQspi_Hal_DmaStartTranferBlock(Handle, DmaSendData, 1, FlashId, 3, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READID error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA Read Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}

AL_VOID qspi_dma_nor(AL_VOID)
{
    AL_U32 i;
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_ERROR, "Start FPSoc Qspi DMA Test\r\n");

    Ret = AlQspi_Hal_Init(&Handle, &QspiX4InitConfigs, AL_NULL, 0);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_Hal_Init error:0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlNorDma_ReadId();


    AlNorDma_Wren();
    AlNorDma_Erase(0);
    AlNorDma_WaitWip();

    /**/
    AlNorDma_ReadPage(0);
    for (i = 0; i < 240; i++) {
        if(0xff != DmaRecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error DmaRecvData[%d]:%d\r\n", i, DmaRecvData[i]);
            // while (1);
            assert_int_equal(DmaRecvData[i], 0xff);
        }
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlQspi test erase norflash success\r\n");

    /**/
    AlNorDma_Wren();
    AlNorDma_WritePage(0);
    AlNorDma_WaitWip();


    AlNorDma_ReadPage(0);
    for (i = 0; i < 230; i++) {
        if(i != DmaRecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlQspi data write norflash test error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error DmaRecvData[%d]:%d\r\n", i, DmaRecvData[i]);
            // while (1);
            assert_int_equal(DmaRecvData[i], i);
        }
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlQspi test write norflash success\r\n");
}




CMOCKA_TC_DEFINE(qspi_test_x1_nor, NULL, NULL, NULL);
CMOCKA_TC_DEFINE(qspi_test_x4_nor, NULL, NULL, NULL);
CMOCKA_TC_DEFINE(qspi_dma_nor, NULL, NULL, NULL);



