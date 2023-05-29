#include "al_spi_hal.h"
#include "al_spinor.h"
#include "al_intr.h"

AL_SPI_HalStruct Spi0Hal;

AL_SPI_ConfigsStruct SpiInitConfigs =
{
    .Mode               = SPI_MASTER_MODE,
    .TransMode          = SPI_EEPROM,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .DataFrameSize      = SPI_FRAME_08BITS,
    .ClkDiv             = 70,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
    .SlvSelEnum         = SPI_SER_SS0_EN
};

AL_U8 SendData[500] = { 0x0 };
AL_U8 RecvData[500] = { 0x0 };
AL_U8 FlashId[10] = { 0x0 };


void AL_NOR_RESET(void)
{
    AL_S32 ret = AL_OK;

	Spi0Hal.Dev->Configs.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_INFINEON_SRST;

	ret = AlSpi_Hal_SendDataBlock(&Spi0Hal, SendData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_RESET error!!!!!\r\n");
    }
}

void AL_NOR_WREN(void)
{
    AL_S32 ret = AL_OK;

	Spi0Hal.Dev->Configs.TransMode = SPI_TX_ONLY;
	SendData[0] = NOR_OP_WREN;

    ret = AlSpi_Hal_SendDataBlock(&Spi0Hal, SendData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_WREN error!!!!!\r\n");
    }
}


void AL_NOR_SETSTATUS(AL_U8 data)
{
    AL_S32 ret = AL_OK;

	Spi0Hal.Dev->Configs.TransMode = SPI_TX_ONLY;
	SendData[0] = NOR_OP_WRSR;
    SendData[1] = data;

    ret = AlSpi_Hal_SendDataBlock(&Spi0Hal, SendData, 2, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_SETSTATUS error!!!!!\r\n");
    }
}

void AL_NOR_WAITWIP(void)
{
    AL_S32 ret = AL_OK;

    Spi0Hal.Dev->Configs.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDSR;

    do {
	    ret = AlSpi_Hal_SendRecvDataBlock(&Spi0Hal, SendData, 1, RecvData, 1, 0);
        if (ret != AL_OK) {
            printf("AL_NOR_WAITWIP error!!!!!\r\n");
        }
#ifdef SPI_DEBUG
        printf("Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}

void AL_NOR_READSTATUS(void)
{
    AL_S32 ret = AL_OK;

    Spi0Hal.Dev->Configs.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDSR;

	ret = AlSpi_Hal_SendRecvDataBlock(&Spi0Hal, SendData, 1, RecvData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_READSTATUS error!!!!!\r\n");
    }
#ifdef SPI_DEBUG
    printf("Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
}

void AL_NOR_ERASE(void)
{
    AL_S32 ret = AL_OK;

    Spi0Hal.Dev->Configs.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlSpi_Hal_SendDataBlock(&Spi0Hal, SendData, 4, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_ERASE error!!!!!\r\n");
    }
}


void AL_NOR_ERASECHIP(void)
{
    AL_S32 ret = AL_OK;

    Spi0Hal.Dev->Configs.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_CE;

    ret = AlSpi_Hal_SendDataBlock(&Spi0Hal, SendData, 1, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_ERASECHIP error!!!!!\r\n");
    }
}


void AL_NOR_READPAGE(void)
{
    AL_S32 ret = AL_OK;

    Spi0Hal.Dev->Configs.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlSpi_Hal_SendRecvDataBlock(&Spi0Hal, SendData, 4, RecvData, 255, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_READPAGE error!!!!!\r\n");
    }
}


void AL_NOR_WRITEPAGE(void)
{
    AL_S32 ret = AL_OK;

    Spi0Hal.Dev->Configs.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
	    SendData[i + 4] = i % 255;
    }
    ret = AlSpi_Hal_SendDataBlock(&Spi0Hal, SendData, 255, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error!!!!!\r\n");
    }
}

AL_VOID AL_NOR_READID(AL_VOID)
{
    AL_S32 ret = AL_OK;

    SendData[0] = NOR_OP_RDID;
    ret = AlSpi_Hal_SendRecvDataBlock(&Spi0Hal, SendData, 1, FlashId, 3, 1);
    if (ret != AL_OK) {
        printf("AL_NOR_READID error!!!!!\r\n");
    }
    printf("Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
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

    printf("Start FPSoc Spi Test\r\n");

    ret = AlSpi_Hal_Init(&Spi0Hal, &SpiInitConfigs, AL_NULL, AL_NULL, 1);
    if (ret != AL_OK) {
        printf("AlSpi_Hal_Init error!!!!!\r\n");
    }

    /**/
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    /**/
    AL_NOR_READID();

    /**/
    AL_NOR_WREN();
    AL_NOR_READSTATUS();
    AL_NOR_ERASE();
    AL_NOR_WAITWIP();

    /**/
    AL_NOR_READPAGE();
    for (i = 0; i < 255; i++) {
        if(0xff != RecvData[i]) {
            printf("AlSpi test erase norflash error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    printf("AlSpi test erase norflash success\r\n");

    /**/
    AL_NOR_WREN();
    AL_NOR_WRITEPAGE();
    AL_NOR_WAITWIP();

    AL_NOR_READPAGE();
    for (i = 0; i < 240; i++) {
        if(i != RecvData[i]) {
            printf("AlSpi data write norflash test error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    printf("AlSpi test write norflash success\r\n");

        /**/
    AL_NOR_WREN();
    AL_NOR_READSTATUS();
    AL_NOR_ERASE();
    AL_NOR_WAITWIP();
    AL_NOR_READPAGE();
    for (i = 0; i < 255; i++) {
        if(0xff != RecvData[i]) {
            printf("AL Spi test erase norflash error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
    printf("AL Spi test erase norflash success\r\n");
    AL_NOR_WREN();
    AL_NOR_WRITEPAGE();
    AL_NOR_WAITWIP();
    AL_NOR_READPAGE();
    for (i = 0; i < 240; i++) {
        if(i != RecvData[i]) {
            printf("AL Spi data write norflash test error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
    printf("AL Spi test write norflash success\r\n");
    while (1);
}

