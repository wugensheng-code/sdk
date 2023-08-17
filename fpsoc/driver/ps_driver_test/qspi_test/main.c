#include "al_qspi_hal.h"
#include "al_spinor.h"
#include "al_dmacahb_hal.h"

AL_QSPI_HalStruct QspiHal;

/* When SpiFrameFormat is Dual Quad,TransMode can only use QSPI_TX_ONLY,QSPI_RX_ONLY mode */
AL_QSPI_ConfigsStruct QspiInitConfigs =
{
    .TransMode          = QSPI_EEPROM,
    .SpiFrameFormat     = SPI_STANDARD_FORMAT,
    .DataFrameSize      = QSPI_DFS_8BITS,
    .EnSpiCfg.AddrLength    = QSPI_ADDR_L0,
    .EnSpiCfg.InstLength    = QSPI_INST_L8,
    .EnSpiCfg.TransType     = QSPI_TT0,
    .EnSpiCfg.WaitCycles    = 0,
    .EnSpiCfg.ClockStretch  = QSPI_EnableClockStretch,
    //.EnSpiCfg.ClockStretch  = QSPI_DisableClockStretch,
    .ClkDiv             = 2,
    .SamplDelay         = 2,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SlvSelEnum         = QSPI_SER_SS0_EN,
    .IsUseDma           = AL_QSPI_USE_INTR
};

AL_U8 __attribute__((aligned(4))) SendData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) RecvData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) FlashId[10] = { 0x0 };

AL_U8 __attribute__((aligned(4))) DmaSendData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) DmaRecvData[500] = { 0x0 };

void AL_NOR_RESET(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_INFINEON_SRST;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_RESET error!!!!!\r\n");
    }
}

void AL_NOR_RESET1(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_SRSTEN;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_RESET error!!!!!\r\n");
    }

    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_SRST;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_RESET error!!!!!\r\n");
    }

}

void AL_NOR_DMA_RESET(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    DmaSendData[0] = NOR_OP_INFINEON_SRST;

    ret = AlQspi_Hal_DmaStartBlockSend(&QspiHal, DmaSendData, 1, 100000);
    if (ret != AL_OK) {
        printf("DMA AL_NOR_RESET error!!!!!\r\n");
    }
}


void AL_NOR_DMA_RESET1(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    SendData[0] = NOR_OP_SRSTEN;

    ret = AlQspi_Hal_DmaStartBlockSend(&QspiHal, SendData, 1, 100000);
    if (ret != AL_OK) {
        printf("AL_NOR_RESET error!!!!!\r\n");
    }

    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    SendData[0] = NOR_OP_SRST;

    ret = AlQspi_Hal_DmaStartBlockSend(&QspiHal, SendData, 1, 100000);
    if (ret != AL_OK) {
        printf("AL_NOR_RESET error!!!!!\r\n");
    }
}


void AL_NOR_WREN(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    SendData[0] = NOR_OP_WREN;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_WREN error!!!!!\r\n");
    }
}

void AL_NOR_DMA_WREN(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    DmaSendData[0] = NOR_OP_WREN;

    ret = AlQspi_Hal_DmaStartBlockSend(&QspiHal, DmaSendData, 1, 100000);
    if (ret != AL_OK) {
        printf("DMA AL_NOR_WREN error!!!!!\r\n");
    }
}

void AL_NOR_SETSTATUS(AL_U8 data)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_WRSR;
    SendData[1] = data;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 2, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_SETSTATUS error!!!!!\r\n");
    }
}

void AL_NOR_WAITWIP(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    SendData[0] = NOR_OP_RDSR;

    do {
        ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 1, RecvData, 1, 100000);
        if (ret != AL_OK) {
            printf("AL_NOR_WAITWIP error!!!!!\r\n");
        }
#ifdef QSPI_DEBUG
        printf("WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}

void AL_NOR_DMA_WAITWIP(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    DmaSendData[0] = NOR_OP_RDSR;

    do {
        ret = AlQspi_Hal_DmaStartBlockTranfer(&QspiHal, DmaSendData, 1, DmaRecvData, 1, 100000);
        if (ret != AL_OK) {
            printf("AL_NOR_WAITWIP error!!!!!\r\n");
        }
#ifdef QSPI_DEBUG
        printf("WAITWIP Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
    } while (DmaRecvData[0] & SR_WIP);
}

void AL_NOR_READSTATUS(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendData[0] = NOR_OP_RDSR;

    AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 1, RecvData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_READSTATUS error!!!!!\r\n");
    }
// #ifdef QSPI_DEBUG
    printf("Nor Status1 Reg:%x\r\n", RecvData[0]);
// #endif
}


void AL_NOR_DMA_READSTATUS(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    DmaSendData[0] = NOR_OP_RDSR;

    AlQspi_Hal_DmaStartBlockTranfer(&QspiHal, DmaSendData, 1, DmaRecvData, 1, 100000);
    if (ret != AL_OK) {
        printf("DMA AL_NOR_READSTATUS error!!!!!\r\n");
    }
#ifdef QSPI_DEBUG
    printf("Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
}

void AL_NOR_ERASE(void)
{
    AL_S32 ret = AL_OK;

   // AlQspi_Dev_DumpReg(QspiHal.Dev->BaseAddr);

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;

    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 4, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_ERASE error!!!!!\r\n");
    }
}


void AL_NOR_ERASE1(void)
{
    AL_S32 ret = AL_OK;

   // AlQspi_Dev_DumpReg(QspiHal.Dev->BaseAddr);

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;

    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 1;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 4, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_ERASE error!!!!!\r\n");
    }
}

void AL_NOR_ERASE2(void)
{
    AL_S32 ret = AL_OK;

   // AlQspi_Dev_DumpReg(QspiHal.Dev->BaseAddr);

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;

    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 2;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 4, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_ERASE error!!!!!\r\n");
    }
}

void AL_NOR_DMA_ERASE(void)
{
    AL_S32 ret = AL_OK;

   // AlQspi_Dev_DumpReg(QspiHal.Dev->BaseAddr);
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlQspi_Hal_DmaStartBlockSend(&QspiHal, SendData, 4, 100000);
    if (ret != AL_OK) {
        printf("AL_NOR_ERASE error!!!!!\r\n");
    }
}


void AL_NOR_ERASECHIP(void)
{
    AL_S32 ret = AL_OK;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_CE;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_ERASECHIP error!!!!!\r\n");
    }
}


void AL_NOR_READPAGE(void)
{
    AL_S32 ret = AL_OK;
    // printf("AL_NOR_READPAGE\r\n");
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 4, RecvData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_READPAGE error!!!!!\r\n");
    }
    // printf("AL_NOR_READPAGE end\r\n");
}

void AL_NOR_X4_1_1_4_READ_PAGE(AL_U8 addr)
{
    AL_S32 ret = AL_OK;
    printf("AL_NOR_READPAGE\r\n");
    QspiHal.Dev->Configs.TransMode  = QSPI_RX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
        QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 8;

    SendData[0] = NOR_OP_READ_1_1_4;
    SendData[1] = addr;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 4, RecvData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_READPAGE error!!!!!\r\n");
    }
    printf("AL_NOR_READPAGE end\r\n");
}


void AL_NOR_WRITE_X4_1_1_4_PAGE(AL_U8 addr)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_PP_1_1_4;
    SendData[1] = addr;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
        // printf
    }
    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error!!!!!\r\n");
    }
}



void AL_NOR_X4_1_4_4_READ_PAGE(void)
{
    AL_S32 ret = AL_OK;
    printf("AL_NOR_X4_1_4_4_READ_PAGE\r\n");
    QspiHal.Dev->Configs.TransMode  = QSPI_RX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 8;

    SendData[0] = NOR_OP_READ_1_1_4;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 4, RecvData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_READPAGE error!!!!!\r\n");
    }
    printf("AL_NOR_READPAGE end\r\n");
}

void AL_NOR_WRITE_X4_1_4_4_PAGE(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_PP_1_1_4;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error!!!!!\r\n");
    }
}

void AL_NOR_X4_1_4_4_4B_READ_PAGE(void)
{
    AL_S32 ret = AL_OK;
    printf("AL_NOR_READPAGE\r\n");
    QspiHal.Dev->Configs.TransMode  = QSPI_RX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L32;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT1;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 6;

    SendData[0] = NOR_OP_READ_1_4_4_4B;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 2;
    SendData[4] = 0;

    ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 5, RecvData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_READPAGE error!!!!!\r\n");
    }
    printf("AL_NOR_READPAGE end\r\n");
}

void AL_NOR_WRITE_X4_1_1_4_4B_PAGE(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L32;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
        QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_PP_1_1_4_4B;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 2;
    SendData[4] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 5] = i % 255;
    }
    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error!!!!!\r\n");
    }
}


void AL_NOR_X4_1_1_4_4B_READ_PAGE(void)
{
    AL_S32 ret = AL_OK;
    printf("AL_NOR_READPAGE\r\n");
    QspiHal.Dev->Configs.TransMode  = QSPI_RX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L32;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
        QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 8;

    SendData[0] = NOR_OP_READ_1_1_4_4B;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 3;
    SendData[4] = 0;

    ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 5, RecvData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_READPAGE error!!!!!\r\n");
    }
    printf("AL_NOR_READPAGE end\r\n");
}

void AL_NOR_WRITE_X4_1_4_4_4B_PAGE(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L32;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_PP_1_1_4_4B;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 3;
    SendData[4] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 5] = i % 255;
    }
    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error!!!!!\r\n");
    }
}

void AL_NOR_DMA_READPAGE(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.TransMode  = QSPI_RX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 8;

    DmaSendData[0] = NOR_OP_READ_1_1_4;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0x20;
    DmaSendData[3] = 0;

    ret = AlQspi_Hal_DmaStartBlockTranfer(&QspiHal, DmaSendData, 4, DmaRecvData, 240, 100000);
    if (ret != AL_OK) {
        printf("DMA AL_NOR_READPAGE error!!!!!\r\n");
    }
    printf("DMA AL_NOR_READPAGE end\r\n");
}

void AL_NOR_READPAGE1(void)
{
    AL_S32 ret = AL_OK;
    printf("AL_NOR_READPAGE\r\n");
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0x10;
    SendData[3] = 0;

    ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 4, RecvData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_READPAGE error!!!!!\r\n");
    }
    printf("AL_NOR_READPAGE end\r\n");
}


void AL_NOR_WRITEPAGE(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error!!!!!\r\n");
    }
}


void AL_NOR_DMA_WRITEPAGE(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;

    DmaSendData[0] = NOR_OP_PP_1_1_4;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0x20;
    DmaSendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        DmaSendData[i + 4] = i % 255;
    }
    ret = AlQspi_Hal_DmaStartBlockSend(&QspiHal, DmaSendData, 240, 100000);
    if (ret != AL_OK) {
        printf("DMA AL_NOR_WRITEPAGE error!!!!!\r\n");
    }
}

void AL_NOR_WRITEPAGE1(void)
{
    AL_S32 ret = AL_OK;

    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0x10;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
       SendData[i + 4] = i % 255;
    }
    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error!!!!!\r\n");
    }
}

AL_VOID AL_NOR_READID(AL_VOID)
{
    AL_S32 ret = AL_OK;
    SendData[0] = NOR_OP_RDID;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 1, FlashId, 3, 1);
    if (ret != AL_OK) {
        printf("AL_NOR_READID error!!!!!\r\n");
    }
    printf("Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}


AL_VOID AL_NOR_REMOVE_PTOTECT(AL_VOID)
{
    AL_S32 ret = AL_OK;
    AL_U8 Temp;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendData[0] = NOR_OP_RDCR;
printf("AL_NOR_REMOVE_PTOTECT 0!!!!!\r\n");
    ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 1, RecvData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_REMOVE_PTOTECT1 error!!!!!\r\n");
    }
    Temp = RecvData[0];
printf("AL_NOR_REMOVE_PTOTECT 1!!!!!\r\n");
    AL_NOR_WREN();

    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    RecvData[2] = Temp | 0x2;
    RecvData[1] = 0;
    RecvData[0] = NOR_OP_WRSR;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, RecvData, 3, 0x10000);
    if (ret != AL_OK) {
        printf("AL_NOR_REMOVE_PTOTECT2 error!!!!!\r\n");
    }
    printf("AL_NOR_REMOVE_PTOTECT 2!!!!!\r\n");
    AL_NOR_WAITWIP();

}




AL_VOID AL_NOR_MXIC_SET_QE(AL_VOID)
{
    AL_S32 ret = AL_OK;
    AL_U8 Temp;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendData[0] = NOR_OP_RDSR;

    ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 1, RecvData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_MXIC_SET_QE error!!!!!\r\n");
    }
    Temp = RecvData[0];

    printf("AL_NOR_MXIC_SET_QE Status:%d\r\n", Temp);
    AL_NOR_WREN();

    QspiHal.Dev->Configs.TransMode  = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    RecvData[1] = Temp | (1 << 6);
    RecvData[0] = NOR_OP_WRSR;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, RecvData, 2, 0x10000);
    if (ret != AL_OK) {
        printf("AL_NOR_MXIC_SET_QE error!!!!!\r\n");
    }

    AL_NOR_WAITWIP();

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendData[0] = NOR_OP_RDSR;

    ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 1, RecvData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_MXIC_SET_QE error!!!!!\r\n");
    }

    printf("AL_NOR_MXIC_SET_QE Status:%d\r\n", RecvData[0]);

}


AL_VOID AL_NOR_DMA_READID(AL_VOID)
{
    AL_S32 ret = AL_OK;
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    SendData[0] = NOR_OP_PP_1_4_4;

    DmaSendData[0] = NOR_OP_RDID;;

    ret = AlQspi_Hal_DmaStartBlockTranfer(&QspiHal, DmaSendData, 1, FlashId, 3, 100000);
    if (ret != AL_OK) {
        printf("AL_NOR_READID error!!!!!\r\n");
    }

    printf("DMA Read Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}


AL_S32 AL_NOR_SetQuad(AL_U8 SetQuadCmd, AL_U8 ReadQuadCmd, AL_U8 QuadPos)
{
    AL_S32 Data = 0, ret = AL_OK;

    SendData[0] = ReadQuadCmd;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 1, &Data, 1, 10000);
    if (ret != AL_OK) {
        printf("AL_NOR_SetQuad ReadQuadCmd error!!!!!\r\n");
    }

    Data = Data | (1 << QuadPos);

    AL_NOR_WREN();

    SendData[0] = SetQuadCmd;
    SendData[1] = Data;

    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;

    ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 2, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_SetQuad SetQuadCmd error!!!!!\r\n");
    }

    AL_NOR_WAITWIP();

    return ret;
}


void main(void)
{
    AL_U32 i;
    AL_S32 ret = AL_OK;

    *(uint32_t *)(0xf8803000u) =0x1;	  //QSPI  MIO0
    *(uint32_t *)(0xf8803004u) =0x1;
    *(uint32_t *)(0xf8803008u) =0x1;
    *(uint32_t *)(0xf880300cu) =0x1;
    *(uint32_t *)(0xf8803010u) =0x1;
    *(uint32_t *)(0xf8803014u) =0x1;
    *(uint32_t *)(0xf8803018u) =0x1;

// #define AL_QSPI_USE_INTR
#ifdef AL_QSPI_USE_INTR
    printf("Start FPSoc Qspi Test\r\n");

    ret = AlQspi_Hal_Init(&QspiHal, &QspiInitConfigs, AL_NULL, AL_NULL, 0);
    if (ret != AL_OK) {
        printf("AlQspi_Hal_Init error!!!!!\r\n");
    }

    /**/
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    // AL_NOR_RESET();

    //  AL_NOR_WREN();
    //  AL_NOR_SETSTATUS(0);
AL_NOR_RESET1();

printf("AL_NOR_RESET Running!!!!!\r\n");
printf("AL_NOR_RESET Running!!!!!\r\n");
printf("AL_NOR_RESET Running!!!!!\r\n");
printf("AL_NOR_RESET Running!!!!!\r\n");
printf("AL_NOR_RESET Running!!!!!\r\n");

// AL_NOR_REMOVE_PTOTECT();
    AL_NOR_READSTATUS();

    printf("AL_NOR_READSTATUS\r\n");
    AL_NOR_READSTATUS();

// while (1);

// AL_NOR_RESET();

// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
     /**/

      printf("AL_NOR_READID1\r\n");
    AL_NOR_READSTATUS();
     AL_NOR_READID();

      printf("AL_NOR_READID2\r\n");
    AL_NOR_READSTATUS();
     AL_NOR_READID();

    if((FlashId[0] != 0x01) && (FlashId[0] != 0x20) && (FlashId[0] != 0x0) && (FlashId[0] != 0xff))
    {
        if( (FlashId[0] != 0x9d) && (FlashId[0] != 0xc2) ){
            ret = AL_NOR_SetQuad(0x31, 0x35, 1);
        }else{
            ret = AL_NOR_SetQuad(0x01, 0x05, 6);
        }

        if(ret != AL_OK) {
            return ret;
        }
    }

printf("AL_NOR_READID3\r\n");
    AL_NOR_READSTATUS();

    /**/
    printf("AL_NOR_WREN\r\n");
    AL_NOR_WREN();
    printf("AL_NOR_READSTATUS\r\n");
    AL_NOR_READSTATUS();
    printf("AL_NOR_ERASE\r\n");
    AL_NOR_ERASE();
      AL_NOR_READSTATUS();
    printf("AL_NOR_WAITWIP\r\n");
    AL_NOR_WAITWIP();

    /**/
    AL_NOR_READPAGE();
    for (i = 0; i < 240; i++) {
        if(0xff != RecvData[i]) {
            printf("AlQspi test erase norflash error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    printf("AlQspi test erase norflash success\r\n");

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
            printf("AlQspi data write norflash test error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    AL_NOR_READPAGE1();
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            printf("AlQspi data write1 norflash test error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    printf("AlQspi test write norflash success\r\n");

#endif

// #define AL_QSPI_USE_QUAD
#ifdef AL_QSPI_USE_QUAD


printf("Start FPSoc Quad Qspi Test\r\n");

    ret = AlQspi_Hal_Init(&QspiHal, &QspiInitConfigs, AL_NULL, AL_NULL, 0);
    if (ret != AL_OK) {
        printf("AlQspi_Hal_Init error!!!!!\r\n");
    }

    /**/
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

//     AL_NOR_RESET();

printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");

// AL_NOR_REMOVE_PTOTECT();
//     AL_NOR_READSTATUS();

//     printf("AL_NOR_READSTATUS\r\n");
//     AL_NOR_READSTATUS();

// // while (1);

// AL_NOR_RESET();

// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
     /**/


AL_NOR_RESET1();

// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
// printf("AL_NOR_RESET Running!!!!!\r\n");
     AL_NOR_READID();

// AL_NOR_MXIC_SET_QE();
    /**/
    AL_NOR_WREN();
    // AL_NOR_ERASE();
    AL_NOR_ERASECHIP();
    AL_NOR_WAITWIP();

    // AL_NOR_WREN();
    // AL_NOR_ERASE1();
    // AL_NOR_WAITWIP();

    // AL_NOR_WREN();
    // AL_NOR_ERASE2();
    // AL_NOR_WAITWIP();

    /**/
    // AL_NOR_READPAGE();
    AL_NOR_X4_1_1_4_READ_PAGE(0);
    for (i = 0; i < 240; i++) {
        if(0xff != RecvData[i]) {
            printf("AlQspi test erase norflash error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
    // printf("AlQspi test erase norflash success\r\n");
// while(1)
//     ;
// AlQspi_Dev_DumpReg(QspiHal.Dev->BaseAddr);
AL_NOR_WREN();
// printf("AL_NOR_WRITE_X4_1_1_4_PAGE\r\n");
// AlQspi_Dev_DumpReg(QspiHal.Dev->BaseAddr);
AL_NOR_WRITE_X4_1_1_4_PAGE(0);
printf("AL_NOR_WAITWIP\r\n");
// AlQspi_Dev_DumpReg(QspiHal.Dev->BaseAddr);
AL_NOR_WAITWIP();

AL_NOR_X4_1_1_4_READ_PAGE(0);
// AL_NOR_READPAGE();
    for (i = 0; i < 230; i++)
    {
        if (i != RecvData[i])
        {
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
        }
    }

    if (i != RecvData[i])
    {
        while (1);
    }

    printf("AL_NOR_WRITE_X4_1_1_4_PAGE test Success\r\n");
    for (i = 0; i < 230; i++) {
        RecvData[i] = 0;
    }

    while (1);

    AL_NOR_WREN();
    printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    // AlQspi_Dev_DumpReg(QspiHal.Dev->BaseAddr);
    AL_NOR_WRITE_X4_1_1_4_PAGE(0);
    printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    // AlQspi_Dev_DumpReg(QspiHal.Dev->BaseAddr);
    AL_NOR_WAITWIP();

    AL_NOR_X4_1_1_4_READ_PAGE(0);
    // AL_NOR_READPAGE();
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
        }
    }
    if(i == RecvData[i])
    printf("AL_NOR_WRITE_X4_1_1_4_PAGE test Success\r\n");
    else
    printf("AL_NOR_WRITE_X4_1_1_4_PAGE test Error!!!!\r\n");

    for (i = 0; i < 230; i++) {
        RecvData[i] = 0;
    }

    // AL_NOR_WREN();
    // AL_NOR_WRITE_X4_1_4_4_PAGE();
    // // AlQspi_Dev_DumpReg(QspiHal.Dev->BaseAddr);
    // AL_NOR_WAITWIP();
    // //  AL_NOR_READPAGE();
    // AL_NOR_X4_1_4_4_READ_PAGE();
    // for (i = 0; i < 230; i++) {
    //     if(i != RecvData[i]) {
    //         // printf("AlQspi AL_NOR_WRITE_X4_1_4_4_PAGE test error!!!!!\r\n");
    //         printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
    //         // i++;
    //         // printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
    //         // i++;
    //         // printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
    //         // i++;
    //         // printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
    //         // i++;
    //         // printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
    //         // while (1);
    //     }
    // }
    // printf("AL_NOR_WRITE_X4_1_4_4_PAGE test Success\r\n");
    // for (i = 0; i < 230; i++) {
    //     RecvData[i] = 0;
    // }

AL_NOR_WREN();
AL_NOR_WRITE_X4_1_1_4_4B_PAGE();
    AL_NOR_WAITWIP();
    AL_NOR_X4_1_4_4_4B_READ_PAGE();
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            printf("AlQspi AL_NOR_WRITE_X4_1_1_4_4B_PAGE test error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
    printf("AL_NOR_WRITE_X4_1_1_4_4B_PAGE test Success\r\n");
    for (i = 0; i < 230; i++) {
        RecvData[i] = 0;
    }

AL_NOR_WREN();
   AL_NOR_WRITE_X4_1_4_4_4B_PAGE();
       AL_NOR_WAITWIP();
    AL_NOR_X4_1_1_4_4B_READ_PAGE();
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            printf("AlQspi AL_NOR_WRITE_X4_1_4_4_4B_PAGE test error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
    printf("AL_NOR_WRITE_X4_1_4_4_4B_PAGE test Success\r\n");
    for (i = 0; i < 230; i++) {
        RecvData[i] = 0;
    }

#endif

#define AL_QSPI_RUN_DMA
#ifdef AL_QSPI_RUN_DMA
    printf("Start FPSoc Qspi AL_QSPI_RUN_DMA Test\r\n");

    QspiInitConfigs.IsUseDma = AL_QSPI_USE_DMA;
    ret = AlQspi_Hal_Init(&QspiHal, &QspiInitConfigs, AL_NULL, AL_NULL, 0);
    if (ret != AL_OK) {
        printf("AlQspi_Hal_Init error!!!!!\r\n");
    }

    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    // printf("AL_NOR_DMA_RESET!!!!!\r\n");
    // // AL_NOR_DMA_RESET();
    // AL_NOR_DMA_RESET1();

    // /**/
    // printf("AL_NOR_DMA_READID!!!!!\r\n");
    // AL_NOR_DMA_READID();

    // /**/
    // printf("AL_NOR_DMA_WREN!!!!!\r\n");
    // AL_NOR_DMA_WREN();
    // printf("AL_NOR_DMA_READSTATUS!!!!!\r\n");
    // AL_NOR_DMA_READSTATUS();
    // printf("AL_NOR_DMA_ERASE!!!!!\r\n");
    // AL_NOR_DMA_ERASE();
    // printf("AL_NOR_DMA_WAITWIP!!!!!\r\n");
    // AL_NOR_DMA_WAITWIP();
    printf("AL_NOR_DMA_READPAGE!!!!!\r\n");
    /**/
    AL_NOR_DMA_READPAGE();
    for (i = 0; i < 240; i++) {
        if(0xff != DmaRecvData[i]) {
            printf("AlQspi test erase norflash error!!!!!\r\n");
            printf("Error DmaRecvData[%d]:%d\r\n", i, DmaRecvData[i]);
            while (1);
        }
    }

    printf("DMA AlQspi test erase norflash success\r\n");

    /**/
    AL_NOR_DMA_WREN();
    printf("AL_NOR_DMA_WREN!!!!!\r\n");
    AL_NOR_DMA_WRITEPAGE();
    printf("AL_NOR_DMA_WRITEPAGE!!!!!\r\n");
    AL_NOR_DMA_WAITWIP();
    printf("AL_NOR_DMA_WAITWIP!!!!!\r\n");

    AL_NOR_DMA_READPAGE();
    for (i = 0; i < 230; i++) {
        if(i != DmaRecvData[i]) {
            printf("DMA AlQspi data write norflash test error!!!!!\r\n");
            printf("Error DmaRecvData[%d]:%d\r\n", i, DmaRecvData[i]);
            while (1);
        }
    }
    printf("DMA AlQspi test write norflash success\r\n");

#endif


}

