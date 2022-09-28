
#include "qspi_drv.h"
#include "qspi_flash_drv.h"


/************************** Variable Definitions *****************************/

static u32 QspiFlashSize=0U;
static u32 QspiFlashMake=0U;
static u32 ReadCommand=0U;

static QspiFlash_Msg FlashMsg;
static u8 ReadBuffer[10] __attribute__ ((aligned(32)));
static u8 WriteBuffer[10] __attribute__ ((aligned(32)));

QSPI_CORE_HANDLE    QSPI_dev;
QSPI_CORE_HANDLE    *g_pdev=&QSPI_dev;

static u32 Mtimer_Delay(u32 us)
{
    u32 DelayCnt;
    u32 Start;
    u32 End;
    u32 Tconsumed;
    
    DelayCnt = us*10;
    Start = *(volatile u32*)COREN205_MTIMER_COUNTER64_LOW;
    
    do {
        End = *(volatile u32*)COREN205_MTIMER_COUNTER64_LOW;
        Tconsumed = End - Start;
    } while(Tconsumed < DelayCnt); 
        
    return 0;
}


static MtimerParams QspiTimer;
static MtimerParams QspiTimer1;

static u32 Mtimer_Enable()
{
    
    QspiTimer.enable = 1;
    QspiTimer.startPoint = 0;
    QspiTimer.curPoint = 0;
    QspiTimer.timer_out  = 0;
    
    return  0;
}
static u32 Mtimer_Disable()
{
    
    QspiTimer.enable = 0;
    QspiTimer.startPoint = 0;
    QspiTimer.curPoint = 0;
    QspiTimer.timer_out  = 0;
    
    return  0;
}

static u32 Mtimer_Start(u32 us)
{
    u32 StartPoint=0;
    
    if (QspiTimer.enable == 1) {
        QspiTimer.startPoint = *(volatile u32*)COREN205_MTIMER_COUNTER64_LOW;
        QspiTimer.curPoint =  QspiTimer.startPoint;
        QspiTimer.timer_out  = us*10;
    }
    else 
        return 1;  //error
    
    return  0;
}

static u32 Mtimer_TimerOut()
{
    u32 Tconsumed;
    u32 Ret=0;
    
    if (QspiTimer.enable == 0) {
        return 2;
    }
    
    QspiTimer.curPoint =  *(volatile u32*)COREN205_MTIMER_COUNTER64_LOW;
    
   
    Tconsumed = QspiTimer.curPoint - QspiTimer.startPoint;
    
    
    if (Tconsumed >= QspiTimer.timer_out) {
        Ret = 1; //timer out
    }
    else {
        Ret = 0;
    }
    
    return Ret;
}

static u32 Mtimer1_Enable()
{
    
    QspiTimer1.enable = 1;
    QspiTimer1.startPoint = 0;
    QspiTimer1.curPoint = 0;
    QspiTimer1.timer_out  = 0;
    
    return  0;
}
static u32 Mtimer1_Disable()
{
    
    QspiTimer1.enable = 0;
    QspiTimer1.startPoint = 0;
    QspiTimer1.curPoint = 0;
    QspiTimer1.timer_out  = 0;
    
    return  0;
}

static u32 Mtimer1_Start(u32 us)
{
    u32 StartPoint=0;
    
    if (QspiTimer1.enable == 1) {
        QspiTimer1.startPoint = *(volatile u32*)COREN205_MTIMER_COUNTER64_LOW;
        QspiTimer1.curPoint =  QspiTimer1.startPoint;
        QspiTimer1.timer_out  = us*10;
    }
    else 
        return 1;  //error
    
    return  0;
}

static u32 Mtimer1_TimerOut()
{
    u32 Tconsumed;
    u32 Ret=0;
    
    if (QspiTimer1.enable == 0) {
        return 2;
    }
    
    QspiTimer1.curPoint =  *(volatile u32*)COREN205_MTIMER_COUNTER64_LOW;
    
   
    Tconsumed = QspiTimer1.curPoint - QspiTimer1.startPoint;
    
    
    if (Tconsumed >= QspiTimer1.timer_out) {
        Ret = 1; //timer out
    }
    else {
        Ret = 0;
    }
    
    return Ret;
}


static void Config_QspiTx(QSPI_CORE_HANDLE *pdev, u32 start_level)
{
    Qspi_TxfifoStartLevelSet(pdev,start_level-1); //transfer will start on serial line
}


static void Config_QspiRx(QSPI_CORE_HANDLE *pdev, u32 rx_numfame)
{
    Qspi_Ctrl1Ndf(pdev,rx_numfame-1);
    
}

static void Wait_TxCompl(QSPI_CORE_HANDLE *pdev)
{
    Mtimer_Start(30);
    while( (!Qspi_TxfifoEmpty(pdev)) && (Mtimer_TimerOut()==0) ); // wait TFE returns to 1,txfifoempty
    //while(qspi_busy(pdev));  // check busy or idle,wait BUSY returns to 0
}


void Config_EhanceMode(QSPI_CORE_HANDLE *pdev)
{
    Qspi_TransType(pdev, SPI_TRANSFER_TYPE0); //Instruction and Address will be sent in Standard SPI Mode.  the next transfer data will be in enhance mode spi
 
    Qspi_InstructionLength(pdev,0x2);
    
    Qspi_WaitCycles(pdev,0x8); //different vendor and device , different wait cycle
    
    //qspi_clk_stretch_en(pdev,0x1);
    
}


void Qspi_ResetFlashMode0(QSPI_CORE_HANDLE *pdev)
{
    
     Qspi_Disable(pdev);
     Qspi_Mode(pdev, TMOD_TX_ONLY);
     Config_QspiTx(pdev,1);
     Config_QspiRx(pdev,0);
     Qspi_Enable(pdev);

     Qspi_DataTransmit(pdev,0x66);
     Wait_TxCompl(pdev);
      Mtimer_Start(30);
     while(Qspi_Busy(pdev) && (Mtimer_TimerOut() == 0));
     
     Qspi_Disable(pdev);
     Qspi_Mode(pdev, TMOD_TX_ONLY);
     Config_QspiTx(pdev,1);
     Config_QspiRx(pdev,0);
     Qspi_Enable(pdev);
     
     Qspi_DataTransmit(pdev,0x99);
     Wait_TxCompl(pdev);
      Mtimer_Start(30);
     while(Qspi_Busy(pdev) && (Mtimer_TimerOut() == 0));
     
     
     Mtimer_Delay(100000);
	 
}


void Qspi_ResetFlashMode1(QSPI_CORE_HANDLE *pdev)
{

     Qspi_Disable(pdev);
     Qspi_Mode(pdev, TMOD_TX_ONLY);
     Config_QspiTx(pdev,1);
     Config_QspiRx(pdev,0);
     Qspi_Enable(pdev);

     Qspi_DataTransmit(pdev,0xF0);
     Wait_TxCompl(pdev);
     Mtimer_Start(30);
     while( Qspi_Busy(pdev) && (Mtimer_TimerOut() == 0));

    
     Mtimer_Delay(100000);
      
}

//for mxic,gd exit enhance mode
void Qspi_ReturnCase0(QSPI_CORE_HANDLE *pdev)
{
	 Qspi_Disable(pdev);
     Qspi_Mode(pdev, TMOD_TX_ONLY);	
     Qspi_TransType(pdev,SPI_TRANSFER_TYPE0);
     Config_QspiTx(pdev,1);
     Config_QspiRx(pdev,0);
     Qspi_Enable(pdev);
	 Qspi_DataTransmit(pdev,0xFF);
     Wait_TxCompl(pdev);
     Mtimer_Start(30);
     while( Qspi_Busy(pdev) && (Mtimer_TimerOut() == 0));

	 Qspi_DataTransmit(pdev,0x00);
     Wait_TxCompl(pdev);
     Mtimer_Start(30);
     while( Qspi_Busy(pdev) && (Mtimer_TimerOut() == 0));

}

//for gd, disable QPI, 0xff cmd
void Qspi_ReturnCase1(QSPI_CORE_HANDLE *pdev)
{
	 Qspi_Disable(pdev);
     Qspi_Mode(pdev, TMOD_TX_ONLY);	
	 Qspi_TransType(pdev,SPI_TRANSFER_TYPE2);
	 Qspi_AddrMode(pdev,SPI_NO_ADDR);
     Config_QspiTx(pdev,1);
     Config_QspiRx(pdev,0);
     Qspi_Enable(pdev);
	 Qspi_DataTransmit(pdev,0xFF);
     Wait_TxCompl(pdev);
     Mtimer_Start(30);
     while( Qspi_Busy(pdev) && (Mtimer_TimerOut() == 0));
}

//for mxic, five 0xff exit ehnace mode, 0xf5 exit QPI
void Qspi_ReturnCase2(QSPI_CORE_HANDLE *pdev)
{
	 Qspi_Disable(pdev);
     Qspi_Mode(pdev, TMOD_TX_ONLY);	
	 Qspi_TransType(pdev,SPI_TRANSFER_TYPE2);
	 Qspi_AddrMode(pdev,SPI_NO_ADDR);
     Config_QspiTx(pdev,5);
     Config_QspiRx(pdev,0);
     Qspi_Enable(pdev);
	 Qspi_DataTransmit(pdev,0xFF);
	 Qspi_DataTransmit(pdev,0xFF);
	 Qspi_DataTransmit(pdev,0xFF);
	 Qspi_DataTransmit(pdev,0xFF);
	 Qspi_DataTransmit(pdev,0xFF);
     Wait_TxCompl(pdev);
     Mtimer_Start(30);
     while( Qspi_Busy(pdev) && (Mtimer_TimerOut() == 0));

	 Qspi_Disable(pdev);
     Qspi_Mode(pdev, TMOD_TX_ONLY);	
	 Qspi_TransType(pdev,SPI_TRANSFER_TYPE2);
	 Qspi_AddrMode(pdev,SPI_NO_ADDR);
     Config_QspiTx(pdev,1);
     Config_QspiRx(pdev,0);
     Qspi_Enable(pdev);

	 Qspi_DataTransmit(pdev,0xF5);
     Wait_TxCompl(pdev);
     Mtimer_Start(30);
     while( Qspi_Busy(pdev) && (Mtimer_TimerOut() == 0));

}


/*****************************************************************************/
/**
*
* This function performs a transfer on the bus in polled mode. The messages
* passed are all transferred on the bus between one CS assert and de-assert.
*
* @param    pdev is a pointer to the QSPI instance.
* @param    Msg is a pointer to the structure containing transfer data.
*
* @return
*
* @note     None.
* becase spi width and dfs, address mode 24/32, three factor impact the spi format, so wo can only support 
* spi x1: 24/32, dfs = 8
* spi x2/x4: 24/31,dfs = 8
*
******************************************************************************/
static u32 Qspi_PolledTransfer(QSPI_CORE_HANDLE *pdev, QspiFlash_Msg *Msg)
{

    u32 Index;
    u32 Tdata=0;
    u32 Rdata=0;

    /*
     * Set the busy flag, which will be cleared when the transfer is
     * entirely done.
     */
     
    Index = 0;
    if (Msg->BusWidth == QSPI_WIDTH_X1) {
        Mtimer_Start(30);
        while ((Msg->txByteCount > 0) && (Mtimer_TimerOut() ==  0) ) {
            Qspi_DataTransmit(pdev,Msg->TxBfrPtr[Index]);
            Index++;
            Msg->txByteCount--;
        }
        
        Wait_TxCompl(pdev);
        Index = 0;
        Mtimer_Start(1000);
        while ((Msg->rxByteCount > 0) && (Mtimer_TimerOut() ==  0)) {
                if(Qspi_RxNotempty(pdev))  // RXFIFO not empty
                {
                    Msg->RxBfrPtr[Index]= Qspi_DataRead(pdev) & 0xFF;
                    Index++;
                    Msg->rxByteCount--;
                }
        }
    }
    else if (Msg->BusWidth == QSPI_WIDTH_X2 || Msg->BusWidth == QSPI_WIDTH_X4) {
         Mtimer_Start(30);
         while ( (Msg->txByteCount > 0) && (Mtimer_TimerOut() ==  0) ) {
            if (Index == 0) {
                Tdata = Msg->TxBfrPtr[0];
                Qspi_DataTransmit(pdev,Tdata);
                Index++;
                Msg->txByteCount--;
            }
            else if(Index == 1) {
                if (pdev->cfg.addr_mode == SPI_FLASH_ADDR32) {
                    Tdata = (Msg->TxBfrPtr[1] << 24) | (Msg->TxBfrPtr[2] << 16) | (Msg->TxBfrPtr[3] << 8) | (Msg->TxBfrPtr[4]);
                    Index += 4;
                    Msg->txByteCount -= 4;
                }
                else if (pdev->cfg.addr_mode == SPI_FLASH_ADDR24) {
                     Tdata = (Msg->TxBfrPtr[1] << 16) | (Msg->TxBfrPtr[2] << 8) | (Msg->TxBfrPtr[3]) ;
                     Index += 3;
                     Msg->txByteCount -= 3;
                }
                  Qspi_DataTransmit(pdev,Tdata);
            }
            else {      
              // never run here error
               Index += 4;
               Msg->txByteCount -= 4;
            }
         }
            Wait_TxCompl(pdev);
            Index = 0;
            Mtimer_Start(1000);
            while ( (Msg->rxByteCount > 0) && (Mtimer_TimerOut()==0)  ) {
                if(Qspi_RxNotempty(pdev))  // RXFIFO not empty, is must 4byte aligned
                {
                    Rdata = Qspi_DataRead(pdev);
                    Msg->RxBfrPtr[Index] = Rdata & 0xFF;
                    Index++;
                    Msg->rxByteCount --;
                }
        }

    }
    
    return 0;
}

/******************************************************************************
*
* This function reads serial FLASH ID connected to the SPI interface.
* It then deduces the make and size of the flash 
* The flash driver will function based on this and
* it presently supports Micron and Spansion - 128, 256 and 512Mbit and
* Winbond 128Mbit
*
* @param    pdev
*
* @return   QSPI_SUCCESS OR UNSUPPORTED_QSPI
*
* @note     None.
*
******************************************************************************/
static u32 Flash_ReadID(QSPI_CORE_HANDLE *pdev)
{
    
    u32 index;
    uint32_t  flashID=0;
    s32 UStatus = QSPI_SUCCESS;
    
    Qspi_Disable(pdev);
    Config_QspiRx(pdev,3);
    Config_QspiTx(pdev,1); //when rdid cmd send,transfer will start on serial line
    Qspi_Enable(pdev);

    
    //bellow read flash ID

    Qspi_DataTransmit(pdev,READ_ID_CMD);

    Wait_TxCompl(pdev);

    index=0;
    Mtimer_Start(30);
    while ( (index < 3) && (Mtimer_TimerOut() == 0)) {

        if (Qspi_RxNotempty(pdev)) {
            ReadBuffer[index] = Qspi_DataRead(pdev) & 0xff;
            index++;
        }
    }
    flashID = ReadBuffer[0] | (ReadBuffer[1]<<8) | (ReadBuffer[2]<<16);
    pdev->cfg.flashID = flashID;
    /*
     * Deduce flash make
     */
    if (ReadBuffer[0] == MICRON_ID) {
        QspiFlashMake = MICRON_ID;
    } else if(ReadBuffer[0] == SPANSION_ID) {
        QspiFlashMake = SPANSION_ID;
    } else if(ReadBuffer[0] == WINBOND_ID) {
        QspiFlashMake = WINBOND_ID;
    } else if(ReadBuffer[0] == MACRONIX_ID) {
        QspiFlashMake = MACRONIX_ID;
    } else if(ReadBuffer[0] == ISSI_ID) {
        QspiFlashMake = ISSI_ID;
    } else if(ReadBuffer[0] == PUYA_ID) {
        QspiFlashMake = PUYA_ID;
    } else if(ReadBuffer[0] == GIGA_ID) {
        QspiFlashMake = GIGA_ID;
    } else {
        UStatus = UNSUPPORTED_QSPI;
    }
    
    /*
     * Deduce flash Size
     */
     
     QspiFlashSize = FLASH_SIZE_128M;
     
    if (ReadBuffer[2] == FLASH_SIZE_ID_2M) {
        QspiFlashSize = FLASH_SIZE_2M;
    } 
    else if (ReadBuffer[2] == FLASH_SIZE_ID_4M) {
        QspiFlashSize = FLASH_SIZE_4M;
    } 
    else if (ReadBuffer[2] == FLASH_SIZE_ID_8M) {
        QspiFlashSize = FLASH_SIZE_8M;
    } else if (ReadBuffer[2] == FLASH_SIZE_ID_16M) {
        QspiFlashSize = FLASH_SIZE_16M;
    } else if (ReadBuffer[2] == FLASH_SIZE_ID_32M) {
        QspiFlashSize = FLASH_SIZE_32M;
    } else if (ReadBuffer[2] == FLASH_SIZE_ID_64M) {
        QspiFlashSize = FLASH_SIZE_64M;
    } else if (ReadBuffer[2] == FLASH_SIZE_ID_128M) {
        QspiFlashSize = FLASH_SIZE_128M;
    } else if ((ReadBuffer[2] == FLASH_SIZE_ID_256M)
            || (ReadBuffer[2] == MACRONIX_FLASH_1_8_V_MX25_ID_256)) {
        QspiFlashSize = FLASH_SIZE_256M;
    } else if ((ReadBuffer[2] == FLASH_SIZE_ID_512M)
            || (ReadBuffer[2] == MACRONIX_FLASH_SIZE_ID_512M)
            || (ReadBuffer[2] == MACRONIX_FLASH_1_8_V_MX66_ID_512)) {
        QspiFlashSize = FLASH_SIZE_512M;
    } else if ((ReadBuffer[2] == FLASH_SIZE_ID_1G)
            || (ReadBuffer[2] == MACRONIX_FLASH_SIZE_ID_1G)
            || (ReadBuffer[2] == MACRONIX_FLASH_1_8_V_SIZE_ID_1G)) {
        QspiFlashSize = FLASH_SIZE_1G;
    } else if ((ReadBuffer[2] == FLASH_SIZE_ID_2G)
            || (ReadBuffer[2] == MACRONIX_FLASH_SIZE_ID_2G)
            || (ReadBuffer[2] == MACRONIX_FLASH_1_8_V_SIZE_ID_2G)) {
                QspiFlashSize = FLASH_SIZE_2G;
    }else {
        UStatus = UNSUPPORTED_QSPI;
    }
    
    if (QspiFlashSize > FLASH_SIZE_128M) {
         ReadCommand = BASE_READ_CMD_32BIT;
    }
    else {
         ReadCommand = BASE_READ_CMD_24BIT;
    }
    
    pdev->cfg.flashSize = QspiFlashSize;
    
    return UStatus;
    
}

static u32 Qspi_Copy(u32 SrcAddress, u8* DestAddress, u32 Length)
{
    s32 Status=0;
    u32 DiscardByteCnt;
    
    WriteBuffer[COMMAND_OFFSET]   = (u8)ReadCommand;
    
    if (ReadCommand == BASE_READ_CMD_24BIT || ReadCommand == DUAL_READ_CMD_24BIT || ReadCommand == QUAD_READ_CMD_24BIT) {
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)((SrcAddress & 0xFF0000U) >> 16);
        WriteBuffer[ADDRESS_2_OFFSET] = (u8)((SrcAddress & 0xFF00U) >> 8);
        WriteBuffer[ADDRESS_3_OFFSET] = (u8)(SrcAddress & 0xFFU);
        DiscardByteCnt = 4;
    }
    else if (ReadCommand == BASE_READ_CMD_32BIT ||ReadCommand == DUAL_READ_CMD_32BIT || ReadCommand == QUAD_READ_CMD_32BIT) {
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)((SrcAddress & 0xFF000000U) >> 24);
        WriteBuffer[ADDRESS_2_OFFSET] = (u8)((SrcAddress & 0xFF0000U) >> 16);
        WriteBuffer[ADDRESS_3_OFFSET] = (u8)((SrcAddress & 0xFF00U) >> 8);
        WriteBuffer[ADDRESS_4_OFFSET] = (u8)(SrcAddress & 0xFFU);
        DiscardByteCnt = 5;
    }
    
    FlashMsg.TxBfrPtr = WriteBuffer;
    FlashMsg.RxBfrPtr = (u8 *)DestAddress;
    FlashMsg.txByteCount = DiscardByteCnt;
    FlashMsg.rxByteCount = Length;
    FlashMsg.BusWidth = g_pdev->cfg.width;
    FlashMsg.Flags = XQSPIPSU_MSG_FLAG_TX | XQSPIPSU_MSG_FLAG_RX;

    if (g_pdev->cfg.width == SPI_FRF_X1_MODE) {
        Qspi_Disable(g_pdev);
        Config_QspiTx(g_pdev,DiscardByteCnt);
        Config_QspiRx(g_pdev,Length);
        Qspi_Enable(g_pdev);
    }
    else if (g_pdev->cfg.width == SPI_FRF_X2_MODE || g_pdev->cfg.width == SPI_FRF_X4_MODE) {
        Qspi_Disable(g_pdev);
        Config_QspiTx(g_pdev,DiscardByteCnt/4+1);
        Config_QspiRx(g_pdev,Length);
        Qspi_Enable(g_pdev);
    }
    /**
    * Send the read command to the Flash to read the specified number
    * of bytes from the Flash, send the read command and address and
    * receive the specified number of bytes of data in the data buffer
    */
    Status = Qspi_PolledTransfer(g_pdev, &FlashMsg);
   
    if (Status != 0) {
         Status= 2;
    }
    
END:
    return Status;
}

/*!
    \brief       Csu_QspiInit：QSPI Initlization, Read flash ID and parse the size of flash.
    \param[in]  : params is the point of the flash information
    \param[out] : flash information  connected with the spi
    \retval     : 0
*/
u32 Csu_QspiInit(QspiParams *params)
{
    s32 Status = 0;

    //set qspi pin mux
    
    Mtimer_Enable();
    Mtimer1_Enable();

    Qspi_Init(g_pdev);

    Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_24);
    
	Status = Flash_ReadID(g_pdev);
  
    if (g_pdev->cfg.flashID ==  0xffffff || g_pdev->cfg.flashID == 0x0) {
        
        Qspi_ResetFlashMode0(g_pdev);
	    Csu_QspiSetMode(QSPI_WIDTH_X1,QSPI_ADDR_24);
        Status = Flash_ReadID(g_pdev);
    }

    if (g_pdev->cfg.flashID ==  0xffffff || g_pdev->cfg.flashID == 0x0) {
        Qspi_ResetFlashMode1(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1,QSPI_ADDR_24);
        Status = Flash_ReadID(g_pdev);
    }
    
	if (g_pdev->cfg.flashID ==  0xffffff || g_pdev->cfg.flashID == 0x0) {
        Qspi_ReturnCase0(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1,QSPI_ADDR_24);
	   	Qspi_ResetFlashMode0(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1,QSPI_ADDR_24);
        Status = Flash_ReadID(g_pdev);
    }

	if (g_pdev->cfg.flashID ==  0xffffff || g_pdev->cfg.flashID == 0x0) {
		Csu_QspiSetMode(QSPI_WIDTH_X4,QSPI_ADDR_24);
        Qspi_ReturnCase1(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1,QSPI_ADDR_24);
		Qspi_ResetFlashMode0(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1,QSPI_ADDR_24);
        Status = Flash_ReadID(g_pdev);
    }

	if (g_pdev->cfg.flashID ==  0xffffff || g_pdev->cfg.flashID == 0x0) {
		Csu_QspiSetMode(QSPI_WIDTH_X4,QSPI_ADDR_24);
        Qspi_ReturnCase2(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1,QSPI_ADDR_24);
		Qspi_ResetFlashMode0(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1,QSPI_ADDR_24);
        Status = Flash_ReadID(g_pdev);
    }
	if (g_pdev->cfg.flashID ==  0xffffff || g_pdev->cfg.flashID == 0x0) {
		Status = 2; //flashID read error!!!
	}

    
   
    params->flashID     =  g_pdev->cfg.flashID;
    params->flashSize   =  g_pdev->cfg.flashSize; //return size in byte
 
    return Status;
    
}

/*!
    \brief       Csu_QspiSetMode：QSPI width set.
    \param[in]  : Mode is spi width:
                  QSPI_WIDTH_X1: QSPI X1 
                  QSPI_WIDTH_X2: QSPI X2 
                  QSPI_WIDTH_X4: QSPI X4 
                : AddrMode
                 QSPI_ADDR_24: force 24bit address width
                 QSPI_ADDR_32: force 32bit address width
                  
    \param[out] : no
    \retval     : 0
*/
u32 Csu_QspiSetMode(u32 Mode, u32 AddrMode)
{
    Qspi_Disable(g_pdev);
     
    if (Mode == QSPI_WIDTH_X1 || Mode == QSPI_WIDTH_X2 || Mode == QSPI_WIDTH_X4) {
        Qspi_Width(g_pdev,Mode);

        
        if (Mode == QSPI_WIDTH_X1) {
            Qspi_Mode(g_pdev,TMOD_EEPROM);
            Qspi_Dfs(g_pdev,DFS_BYTE);
            Qspi_Ser(g_pdev,SLAVE_CS0_SEL);
			Qspi_TransType(g_pdev,SPI_TRANSFER_TYPE0);
	        Qspi_AddrMode(g_pdev,SPI_FLASH_ADDR24);
            Qspi_RxftlCfg(g_pdev,0x3F); //rx fifo = 128 frame, set half of depth generate rx fifo full interrupt
            Qspi_SckdivCfg(g_pdev,0xA); // ahb:200M, spi default:200/20=10M; must modify on real chip
        }
        else if (Mode == QSPI_WIDTH_X2 || Mode == QSPI_WIDTH_X4)  {
            Qspi_Mode(g_pdev,TMOD_RX_ONLY);
            Qspi_Dfs(g_pdev,DFS_BYTE);
            Qspi_Ser(g_pdev,SLAVE_CS0_SEL);
            Qspi_RxftlCfg(g_pdev,0x3F); //rx fifo = 128 frame, set half of depth generate rx fifo full interrupt
            Qspi_SckdivCfg(g_pdev,0xA); // ahb:200M, spi default:200/20=10M;ust modify on real chip
			Qspi_TransType(g_pdev,SPI_TRANSFER_TYPE0);
			if (AddrMode == QSPI_ADDR_32) {
				Qspi_AddrMode(g_pdev,SPI_FLASH_ADDR32);
			}
			else if (AddrMode == QSPI_ADDR_24){
				Qspi_AddrMode(g_pdev,SPI_FLASH_ADDR24);
		    }

            Config_EhanceMode(g_pdev);
        }
    }
    
    if (Mode == QSPI_WIDTH_X1) {
        if (AddrMode == QSPI_ADDR_24) {  //force address to 24BIT
            ReadCommand = BASE_READ_CMD_24BIT;
        }
        else if (AddrMode == QSPI_ADDR_32) { //force address to 32BIT
            ReadCommand = BASE_READ_CMD_32BIT;
        }
         
    }
    else if (Mode == QSPI_WIDTH_X2) {
        if (AddrMode == QSPI_ADDR_24) {  //force address to 24BIT
            ReadCommand = DUAL_READ_CMD_24BIT;
        }
        else if (AddrMode == QSPI_ADDR_32) { //force address to 32BIT
            ReadCommand = DUAL_READ_CMD_32BIT;
        }
    }
    else if (Mode == QSPI_WIDTH_X4) {
        if (AddrMode == QSPI_ADDR_24) {  //force address to 24BIT
            ReadCommand = QUAD_READ_CMD_24BIT;
        }
        else if (AddrMode == QSPI_ADDR_32) { //force address to 32BIT
            ReadCommand = QUAD_READ_CMD_32BIT;
        }
    }
    
     Qspi_Enable(g_pdev);
     
     return 0;
}

/*!
    \brief       Csu_QspiRead：QSPI flash data read.
    \param[in]  : offset-address of flash space
                  dest-the user buffer point
                  length-read data length
    \param[out] : no
    \retval     : 0 or -2,-3
*/

u32 Csu_QspiRead(u32 offset, u8* dest, u32 length)
{
	u32 RemainingBytes=0;
	u32 TransferBytes=0;
    
	/**
	* Check the read length with Qspi flash size
	*/

//	printf("offset: %08x\n", offset);
//	printf("length: %08x\n", length);
//	printf("size  : %08x\n", QspiFlashSize);

	if ((offset + length) > QspiFlashSize)
	{
		return  -1;
	}
    
	RemainingBytes = length;
	Mtimer1_Start(1000000); //1s
	while(RemainingBytes > 0U && (Mtimer1_TimerOut() == 0) ) {

		if (RemainingBytes > QSPI_FIFO_MAX_SIZE)
			 TransferBytes = QSPI_FIFO_MAX_SIZE;
		 else
			 TransferBytes = RemainingBytes;

		if(Qspi_Copy(offset, dest, TransferBytes) == 0) {

			/**
			* Update the variables
			*/
			RemainingBytes -= TransferBytes;
			dest    += TransferBytes;
			offset  += TransferBytes;
		}
		else {
			return 1;
		}
	}

	return 0;
}



