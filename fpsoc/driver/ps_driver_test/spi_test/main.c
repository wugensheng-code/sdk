#include "al_spi_hal.h"
#include "al_spinor.h"

#include "al_dmacahb_hal.h"

AL_SPI_HalStruct Spi0Hal;

AL_SPI_ConfigsStruct SpiInitConfigs =
{
    .Mode               = SPI_MASTER_MODE,
    .TransMode          = SPI_EEPROM,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .DataFrameSize      = SPI_FRAME_8BITS,
    .ClkDiv             = 20,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
    .SlvSelEnum         = SPI_SER_SS0_EN,
    .IsUseDma           = AL_SPI_USE_INTR
};

AL_U8 __attribute__((aligned(4))) SendData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) RecvData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) FlashId[10] = { 0x0 };

AL_U8 __attribute__((aligned(4))) DmaSendData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) DmaRecvData[500] = { 0x0 };

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
    // printf("AL_NOR_WREN finish\r\n");
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
        ret = AlSpi_Hal_TranferDataBlock(&Spi0Hal, SendData, 1, RecvData, 1, 0);
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

    AlSpi_Hal_TranferDataBlock(&Spi0Hal, SendData, 1, RecvData, 1, 0);
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

   // AlSpi_Dev_DumpReg(Spi0Hal.Dev->BaseAddr);

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
    printf("AL_NOR_READPAGE\r\n");
    Spi0Hal.Dev->Configs.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlSpi_Hal_TranferDataBlock(&Spi0Hal, SendData, 4, RecvData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_READPAGE error!!!!!\r\n");
    }
    printf("AL_NOR_READPAGE end\r\n");
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
    ret = AlSpi_Hal_SendDataBlock(&Spi0Hal, SendData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error!!!!!\r\n");
    }
}


void AL_NOR_WRITEPAGE1(void)
{
    AL_S32 ret = AL_OK;

    Spi0Hal.Dev->Configs.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0x10;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
       SendData[i + 4] = i % 255;
    }
    ret = AlSpi_Hal_SendDataBlock(&Spi0Hal, SendData, 240, 0);
    if (ret != AL_OK) {
        printf("AL_NOR_WRITEPAGE error!!!!!\r\n");
    }
}

AL_VOID AL_NOR_READID(AL_VOID)
{
    AL_S32 ret = AL_OK;

    SendData[0] = NOR_OP_RDID;
    ret = AlSpi_Hal_TranferDataBlock(&Spi0Hal, SendData, 1, FlashId, 3, 1);
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


    *(uint32_t *)(0xf8803040u) =0x9;    //spi0  MIO16
    *(uint32_t *)(0xf8803044u) =0x9;    //spi0  MIO17
    *(uint32_t *)(0xf8803048u) =0x9;    //spi0  MIO18
    *(uint32_t *)(0xf880304cu) =0x4;    //GPIO MIO19
    *(uint32_t *)(0xf8803050u) =0x4;    //GPIO MIO20

    *(uint32_t *)(0xf8803054u) =0x9;    //spi0  MIO21
    *(uint32_t *)(0xf8803424u) =0x1;    //emio_sel9

#define AL_SPI_RUN_INTR
#ifdef AL_SPI_RUN_INTR
    // printf("Start FPSoc Spi Test\r\n");

    ret = AlSpi_Hal_Init(&Spi0Hal, &SpiInitConfigs, AL_NULL, AL_NULL, 1);
    if (ret != AL_OK) {
        printf("AlSpi_Hal_Init error!!!!!\r\n");
    }

    /**/
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

 printf("AL_NOR_READID\r\n");
    /**/
    AL_NOR_READID();

 printf("AL_NOR_WREN\r\n");
    /**/
    AL_NOR_WREN();
      printf("AL_NOR_READSTATUS\r\n");
    AL_NOR_READSTATUS();
     printf("AL_NOR_ERASE\r\n");
    AL_NOR_ERASE();
    // printf("AL_NOR_ERASE end\r\n");
    AL_NOR_WAITWIP();

// printf("AL_NOR_READPAGE\r\n");
    /**/
    AL_NOR_READPAGE();
    for (i = 0; i < 240; i++) {
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

    AL_NOR_WREN();
    AL_NOR_WRITEPAGE1();
    AL_NOR_WAITWIP();

    AL_NOR_READPAGE();
    for (i = 0; i < 230; i++) {
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
    for (i = 0; i < 240; i++) {
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
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            printf("AL Spi data write norflash test error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
    printf("AL Spi test write norflash success\r\n");
    // while (1);

#endif

#define AL_SPI_RUN_DMA
#ifdef AL_SPI_RUN_DMA
    printf("Start FPSoc Spi AL_SPI_RUN_DMA Test\r\n");

    SpiInitConfigs.IsUseDma = AL_SPI_USE_DMA;
    ret = AlSpi_Hal_Init(&Spi0Hal, &SpiInitConfigs, AL_NULL, AL_NULL, 1);
    if (ret != AL_OK) {
        printf("AlSpi_Hal_Init error!!!!!\r\n");
    }

    for (i = 0; i < 255; i++) {
        RecvData[i] != 0;
    }

    printf("AlSpi_Hal_DmaTranferData\r\n");
    Spi0Hal.Dev->Configs.TransMode  = SPI_EEPROM;
    DmaSendData[0] = NOR_OP_READ;
    DmaSendData[1] = 0;
    DmaSendData[2] = 0;
    DmaSendData[3] = 0;

    // ret = AlSpi_Hal_DmaTranferData(&Spi0Hal, DmaSendData, 4, DmaRecvData, 240, 0);
    // if (ret != AL_OK) {
    //     printf("AL_SPI_RUN_DMA AL_NOR_READPAGE error!!!!!\r\n");
    // }

    AlSpi_Hal_DmaStartBlockTranfer(&Spi0Hal, DmaSendData, 4, DmaRecvData, 240, 100000);

    for (i = 0; i < 230; i++) {
        if(i != DmaRecvData[i]) {
            printf("AL_SPI_RUN_DMA AlSpi data write norflash test error!!!!!\r\n");
            printf("AL_SPI_RUN_DMA Error DmaRecvData[%d]:%d\r\n", i, DmaRecvData[i]);
            while (1);
        }
    }
    printf("SPI DMA AL_NOR_READPAGE good!!!!!\r\n");
    while (1);

#endif

}

