#include "al_spi_hal.h"
#include "al_spinor.h"
#include "gic_v3.h"
#include "al_dmacahb_hal.h"

AL_SPI_HalStruct Spi0Hal;

AL_SPI_ConfigsStruct SpiInitConfigs =
{
    .Mode               = SPI_MASTER_MODE,
    .TransMode          = SPI_EEPROM,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .DataFrameSize      = SPI_FRAME_08BITS,
    .ClkDiv             = 2,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
    .SlvSelEnum         = SPI_SER_SS0_EN
};

AL_U8 __attribute__((aligned(4))) SendData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) RecvData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) FlashId[10] = { 0x0 };


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

	ret = AlSpi_Hal_TranferDataBlock(&Spi0Hal, SendData, 1, RecvData, 1, 0);
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

static AL_VOID AlSpi_Test_DmaCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_DMACAHB_HalStruct *Handle = (AL_DMACAHB_HalStruct *)CallBackRef;

    switch (Event->EventId)
    {
    case AL_DMACAHB_EVENT_BLOCK_TRANS_COMP:{
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Dmacahb trans count:%d!\r\n", Handle->Channel->Trans.ReloadCount);
        break;
    }
    case AL_DMACAHB_EVENT_ERR:
        AL_LOG(AL_ERR_LEVEL_ERROR, "Dmac ahb auto reload call back err!\r\n");
        break;
    default :
        break;
    }

}

AL_VOID AL_ENABLE_SPI1_TX_DMA(AL_U32 Length)
{
    AL_DMACAHB_ChInitStruct     Spi1TxDmacChConfig;
    AL_DMACAHB_HalStruct        Spi1TxDmacHandle;
    AL_DMACAHB_ChCallBackStruct Spi1TxDmacCallBack; 
    AL_DMACAHB_ChTransStruct    *Spi1TxDmacChTrans;
    AL_U32                      DmacDevId = 0;
    AL_U32                      Ret = AL_OK;

    Spi1TxDmacChConfig.Id = AL_DMACAHB_CHANNEL_6;
    Spi1TxDmacChConfig.TransType = AL_DMACAHB_TRANS_TYPE_4;
    Spi1TxDmacChConfig.Intr.IsIntrEn = AL_TRUE;
    Spi1TxDmacChConfig.Intr.IntrUnMask = AL_DMACAHB_CH_INTR_BLOCK | AL_DMACAHB_CH_INTR_ERR;
    Spi1TxDmacChConfig.SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_32;
    Spi1TxDmacChConfig.DstTransWidth = AL_DMACAHB_TRANS_WIDTH_32;
    Spi1TxDmacChConfig.SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
    Spi1TxDmacChConfig.DstAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
    Spi1TxDmacChConfig.SrcBurstLength = AL_DMACAHB_MSIZE_4;
    Spi1TxDmacChConfig.DstBurstLength = AL_DMACAHB_MSIZE_4;
    Spi1TxDmacChConfig.Direction = AL_DMACAHB_TT_FC_MEM2PER;
    Spi1TxDmacChConfig.SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
    Spi1TxDmacChConfig.DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
    Spi1TxDmacChConfig.HandShaking.SrcPer = AL_DMACAHB_PER_SPI1_TX;
    Spi1TxDmacChConfig.HandShaking.SrcHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
    Spi1TxDmacChConfig.HandShaking.SrcHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;
    Spi1TxDmacChConfig.ChPrior = AL_DMACAHB_CH_PRIOR_7;
    Spi1TxDmacChConfig.FifoMode = AL_DMACAHB_FIFO_MODE_0;
    Spi1TxDmacChConfig.ProtCtl = AL_DMACAHB_PROT_0;
    Spi1TxDmacChConfig.SgrDsr.IsSrcGatherEn = AL_FALSE;
    Spi1TxDmacChConfig.SgrDsr.IsDstScatterEn = AL_FALSE;

    Spi1TxDmacCallBack.Func = AlSpi_Test_DmaCallBack;
    Spi1TxDmacCallBack.Ref = &Spi1TxDmacHandle;

    Ret = AlDmacAhb_Hal_Init(&Spi1TxDmacHandle, &Spi1TxDmacChConfig, &Spi1TxDmacCallBack, DmacDevId);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Dmacahb hal Init error:0x%x\r\n", Ret);
    }

    Spi1TxDmacChTrans = &(Spi1TxDmacHandle.Channel->Trans);
    Spi1TxDmacChTrans->SrcAddr        = (AL_REG)SendData;
    Spi1TxDmacChTrans->DstAddr        = Spi0Hal.Dev->BaseAddr + SPI_DR0_MST_OFFSET;
    Spi1TxDmacChTrans->TransSize      = Length;
    // Spi1TxDmacChTrans->ReloadCountNum = AL_CAN_DMAC_RELOAD_COUNT_MAX;   /* max AL_U32 for trans forever */

    // AlDmacAhb_Dev_Start(Spi1TxDmacHandle.Channel);
    Ret = AlDmacAhb_Hal_StartBlock(&Spi1TxDmacHandle, 1000000);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

}

AL_VOID AL_ENABLE_SPI1_RX_DMA(AL_U32 Length)
{
    AL_DMACAHB_ChInitStruct     Spi1RxDmacChConfig;
    AL_DMACAHB_HalStruct        Spi1RxDmacHandle;
    AL_DMACAHB_ChCallBackStruct Spi1RxDmacCallBack; 
    AL_DMACAHB_ChTransStruct    *Spi1RxDmacChTrans;
    AL_U32                      DmacDevId = 0;
    AL_U32                      Ret = AL_OK;

    Spi1RxDmacChConfig.Id = AL_DMACAHB_CHANNEL_7;
    Spi1RxDmacChConfig.TransType = AL_DMACAHB_TRANS_TYPE_5;
    Spi1RxDmacChConfig.Intr.IsIntrEn = AL_TRUE;
    Spi1RxDmacChConfig.Intr.IntrUnMask = AL_DMACAHB_CH_INTR_BLOCK | AL_DMACAHB_CH_INTR_ERR;
    Spi1RxDmacChConfig.SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_32;
    Spi1RxDmacChConfig.DstTransWidth = AL_DMACAHB_TRANS_WIDTH_32;
    Spi1RxDmacChConfig.SrcAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
    Spi1RxDmacChConfig.DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
    Spi1RxDmacChConfig.SrcBurstLength = AL_DMACAHB_MSIZE_4;
    Spi1RxDmacChConfig.DstBurstLength = AL_DMACAHB_MSIZE_4;
    Spi1RxDmacChConfig.Direction = AL_DMACAHB_TT_FC_PER2MEM;
    Spi1RxDmacChConfig.SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
    Spi1RxDmacChConfig.DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
    Spi1RxDmacChConfig.HandShaking.SrcPer = AL_DMACAHB_PER_SPI1_RX;
    Spi1RxDmacChConfig.HandShaking.SrcHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
    Spi1RxDmacChConfig.HandShaking.SrcHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;
    Spi1RxDmacChConfig.ChPrior = AL_DMACAHB_CH_PRIOR_6;
    Spi1RxDmacChConfig.FifoMode = AL_DMACAHB_FIFO_MODE_0;
    Spi1RxDmacChConfig.ProtCtl = AL_DMACAHB_PROT_0;
    Spi1RxDmacChConfig.SgrDsr.IsSrcGatherEn = AL_FALSE;
    Spi1RxDmacChConfig.SgrDsr.IsDstScatterEn = AL_FALSE;

    Spi1RxDmacCallBack.Func = AlSpi_Test_DmaCallBack;
    Spi1RxDmacCallBack.Ref = &Spi1RxDmacHandle;

    Ret = AlDmacAhb_Hal_Init(&Spi1RxDmacHandle, &Spi1RxDmacChConfig, &Spi1RxDmacCallBack, DmacDevId);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Dmacahb hal Init error:0x%x\r\n", Ret);
    }

    Spi1RxDmacChTrans = &(Spi1RxDmacHandle.Channel->Trans);
    Spi1RxDmacChTrans->SrcAddr        = Spi0Hal.Dev->BaseAddr + SPI_DR0_MST_OFFSET;
    Spi1RxDmacChTrans->DstAddr        = (AL_REG)RecvData;
    Spi1RxDmacChTrans->TransSize      = Length;
    // Spi1RxDmacChTrans->ReloadCountNum = AL_CAN_DMAC_RELOAD_COUNT_MAX;   /* max AL_U32 for trans forever */

    // AlDmacAhb_Dev_Start(Spi1RxDmacHandle.Channel);
    // AlDmacAhb_Hal_Start(&Spi1RxDmacHandle);
    Ret = AlDmacAhb_Hal_StartBlock(&Spi1RxDmacHandle, 1000000);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }
    
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

    ret = AlSpi_Hal_Init(&Spi0Hal, &SpiInitConfigs, AL_NULL, AL_NULL, 0);
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
    while (1);

#endif


#ifdef AL_SPI_RUN_DMA
    printf("Start FPSoc Spi Test\r\n");

    ret = AlSpi_Hal_Init(&Spi0Hal, &SpiInitConfigs, AL_NULL, AL_NULL, 1);
    if (ret != AL_OK) {
        printf("AlSpi_Hal_Init error!!!!!\r\n");
    }

    /**/
    Al_Intr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    /**/
    AL_NOR_READID();

    /**/
    AL_NOR_WREN();
    AL_NOR_READSTATUS();
    AL_NOR_ERASE();
    AL_NOR_WAITWIP();
    
    
    while (1);

#endif

}

