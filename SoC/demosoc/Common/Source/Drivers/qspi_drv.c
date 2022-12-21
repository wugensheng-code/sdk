
#include "qspi_drv.h"



/**************************AL9000******************************/
/*!
    \brief      Disable DWC_ssi
    \param[in]  pdev: QSPI 
    \param[out] none
    \retval     none
*/

int32_t Qspi_Init(QSPI_CORE_HANDLE *pdev)
{
    pdev->cfg.is_enable = 1;
    pdev->cfg.width = 2;
    pdev->cfg.mode = 2;//TMOD_RX_ONLY
    pdev->cfg.enhanced = 1;
    pdev->cfg.addr_mode = SPI_FLASH_ADDR24;
    pdev->cfg.speed = 10;//10M
    pdev->regs = (QSPI_REGS *)(QSPI0_BASE);
    return 0;
}

int32_t Qspi_Disable(QSPI_CORE_HANDLE *pdev)
{
    SSIENR_TypeDef ssienr;
    
    ssienr.d32 = 0;
    ssienr.b.SSIC_EN = QSPI_DIS;

    
    REG32_WRITE((UINTPTR)&pdev->regs->SSIENR,ssienr.d32);
    pdev->cfg.is_enable = 0;
    return 0;
}

int32_t Qspi_Enable(QSPI_CORE_HANDLE *pdev)
{
    SSIENR_TypeDef ssienr;
    
    ssienr.d32 = 0;
    ssienr.b.SSIC_EN = QSPI_EN;
   // printf("pdev->regs->SSIENR 0x%x\n\r",(uint32_t)&pdev->regs->SSIENR);
    REG32_WRITE((UINTPTR)&pdev->regs->SSIENR,ssienr.d32);
    pdev->cfg.is_enable = 1;
    return 0;
}

int32_t Qspi_Width(QSPI_CORE_HANDLE *pdev, uint32_t spi_width)
{
    CTRLR0_TypeDef ctrlr0;
    
	ctrlr0.d32 = REG32_READ((UINTPTR)&pdev->regs->CTRLR0);
    
    if (spi_width == SPI_FRF_X1_MODE) 
        ctrlr0.b.SPI_FRF = SPI_FRF_X1_MODE;
    else if (spi_width == SPI_FRF_X2_MODE)
        ctrlr0.b.SPI_FRF = SPI_FRF_X2_MODE;
    else if (spi_width == SPI_FRF_X4_MODE)
        ctrlr0.b.SPI_FRF =  SPI_FRF_X4_MODE;
    
    REG32_WRITE((UINTPTR)&pdev->regs->CTRLR0,ctrlr0.d32);
    
    pdev->cfg.width = ctrlr0.b.SPI_FRF;
    
    if (pdev->cfg.width == SPI_FRF_X1_MODE)
         pdev->cfg.enhanced = 0;
    else if (pdev->cfg.width == SPI_FRF_X2_MODE || pdev->cfg.width == SPI_FRF_X4_MODE)
        pdev->cfg.enhanced = 1;
    
    return 0;
}

/**************************AL9000******************************/
/*!
    \brief      Disable DWC_ssi
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
    TMOD_TX_RX:Not Applicable in enhanced SPI operating mode or when SSIC_HAS_TX-_RX_EN is set to 0
    TMOD_TX_ONLY:Transmit only mode;Or Write in enhanced SPI operating mode
    TMOD_TX_ONLY:Receive only mode; Or Read in enhanced SPI operating mode
    TMOD_EEPROM:EEPROM Read mode; Not Applicable in enhanced SPI operating mode
    SPI X1 : Standard SPI
    SPI X2/X4: in enhanced SPI
    X1: all four transfer mode can be used;
    X2/X4: TMOD_TX_ONLY，TMOD_TX_ONLY valid
    
*/

int32_t Qspi_Mode(QSPI_CORE_HANDLE *pdev, uint32_t mode)
{
    CTRLR0_TypeDef ctrlr0;

	ctrlr0.d32 = REG32_READ((UINTPTR)&pdev->regs->CTRLR0);
    if (mode == TMOD_TX_RX ) 
        ctrlr0.b.TMOD = TMOD_TX_RX;
    else if (mode == TMOD_TX_ONLY)
        ctrlr0.b.TMOD = TMOD_TX_ONLY;
    else if ( mode == TMOD_RX_ONLY)
        ctrlr0.b.TMOD =  TMOD_RX_ONLY;
    else if (mode == TMOD_EEPROM )
        ctrlr0.b.TMOD =  TMOD_EEPROM;
    
    REG32_WRITE((UINTPTR)&pdev->regs->CTRLR0,ctrlr0.d32);
    
   // QSPI_MODIFY_REG32(&pdev->regs->CTRLR0,0,ctrlr0.d32);
    pdev->cfg.mode = ctrlr0.b.TMOD;
    return 0;
}


/*!
    \brief      cfg QSPI Data Frame Size.
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t Qspi_Dfs(QSPI_CORE_HANDLE *pdev, uint32_t len)
{
    
    CTRLR0_TypeDef ctrlr0;
    
    ctrlr0.d32 = REG32_READ((UINTPTR)&pdev->regs->CTRLR0);
    ctrlr0.b.DFS = len;
    REG32_WRITE((UINTPTR)&pdev->regs->CTRLR0,ctrlr0.d32);

    pdev->cfg.dfs = len+1;
    return 0;
}

/*!
    \brief      Slave Select Rigister cfg
    \param[in]  qspi: SPIx(x=0,1)
    \param[out] none
    \retval     none
*/
int32_t Qspi_Ser(QSPI_CORE_HANDLE *pdev, uint32_t slv)
{
    SER_TypeDef ser;
    
    ser.d32 = 0;
    
    if (slv == SLAVE_CS0_SEL)
        ser.b.SER = SLAVE_CS0_SEL;
    else if (slv == SLAVE_CS1_SEL)
        ser.b.SER = SLAVE_CS1_SEL;
    else if (slv == SLAVE_ALL_SEL)
        ser.b.SER = SLAVE_ALL_SEL;
    
    REG32_WRITE((UINTPTR)&pdev->regs->SER,ser.d32);
    return 0;
}

/*!
    \brief      cfg QSPI Clock Divider.
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t Qspi_SckdivCfg(QSPI_CORE_HANDLE *pdev, uint32_t div)
{
    BAUDR_TypeDef baud;
    
    baud.d32 = 0;
    
    baud.b.SCKDV = div;
    
    REG32_WRITE((UINTPTR)&pdev->regs->BAUDR,baud.d32);
   
    return 0;
}

/*!
    \brief      cfg QSPI Transmit FIFO Threshold Level.
    \param[in]  qspi: QSPI
    \param[out] none
    \retval     none
*/
int32_t Qspi_TxftlTft(QSPI_CORE_HANDLE *pdev, uint32_t Threshold)
{
    TXFTLR_TypeDef txftlr;
    
    txftlr.d32 = REG32_READ((UINTPTR)&pdev->regs->TXFTLR);
    
    txftlr.b.TFT = Threshold;
    
    REG32_WRITE((UINTPTR)&pdev->regs->TXFTLR,txftlr.d32);

    return 0;
}

/*!
    \brief      cfg QSPI Transfer start FIFO level.
    \param[in]  qspi: QSPI
    \param[out] none
    \retval     none
*/
int32_t Qspi_TxfifoStartLevelSet(QSPI_CORE_HANDLE *pdev, uint32_t start_level)
{
    TXFTLR_TypeDef txftlr;

    txftlr.d32 = REG32_READ((UINTPTR)&pdev->regs->TXFTLR);
    

    txftlr.b.TXFTHR = start_level;
    
    REG32_WRITE((UINTPTR)&pdev->regs->TXFTLR,txftlr.d32);
     
     
    //QSPI_MODIFY_REG32(&pdev->regs->TXFTLR,0,txftlr.d32);
   
    return 0;
}

/*!
    \brief      cfg QSPI Receive FIFO Threshold Level.
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t Qspi_RxftlCfg(QSPI_CORE_HANDLE *pdev, uint32_t Threshold)
{
    RXFTLR_TypeDef rxftlr;
    
     rxftlr.d32 = 0;
     
     rxftlr.b.RFT = Threshold;
     
    
    REG32_WRITE((UINTPTR)&pdev->regs->RXFTLR,rxftlr.d32);
    return 0;
}

/*!
    \brief      QSPI transmit data function
    \param[in]  qspi:
    \param[in]  data: data of transmission
    \param[out] none
    \retval     none
*/
int32_t Qspi_DataTransmit(QSPI_CORE_HANDLE *pdev, uint32_t data)
{
    DRX_TypeDef tdata;
    
    tdata.d32 = data;
    
   // printf("DRx:0x%x\n\r",(uint32_t)&pdev->regs->DRx);
    REG32_WRITE((UINTPTR)&pdev->regs->DRx,tdata.d32);
    
    return 0;
}

/*!
    \brief      QSPI data read function
    \param[in]  qspi:
    \param[in]  data: data of transmission
    \param[out] none
    \retval     none
*/
uint32_t Qspi_DataRead(QSPI_CORE_HANDLE *pdev)
{
    uint32_t data=0;
    
    data = REG32_READ((UINTPTR)&pdev->regs->DRx);
    return data;
}

/*!
    \brief      QSPI status TFE(Transmit FIFO Empty)function
    \param[in]  qspi:
    \param[out] none
    \retval     1-empty, 0-notempty
*/
uint32_t Qspi_TxfifoEmpty(QSPI_CORE_HANDLE *pdev)
{
    SR_TypeDef sr;

    sr.d32 = REG32_READ((UINTPTR)&pdev->regs->SR);
    if(sr.b.TFE)
        return QSPI_SR_TXFIFO_EMPTY;
    else
        return QSPI_SR_TXFIFO_NOTEMPTY;
}

/*!
    \brief      QSPI status function
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
uint32_t Qspi_Busy(QSPI_CORE_HANDLE *pdev)
{
    SR_TypeDef sr;
    
    sr.d32 = REG32_READ((UINTPTR)&pdev->regs->SR);
    
    if(sr.b.BUSY)
        return QSPI_SR_BUSY;
    else
        return QSPI_SR_IDLE;
}

/*!
    \brief      check rx fifo empty or not
    \param[in]  qspi:
    \param[out] none
    \retval     1-not empty;0-empty
*/
uint32_t Qspi_RxNotempty(QSPI_CORE_HANDLE *pdev)
{
    SR_TypeDef sr;
    
    sr.d32 = REG32_READ((UINTPTR)&pdev->regs->SR);
    //printf("sr.b.RFNE:0x%x\n\r",sr.b.RFNE);
    if(sr.b.RFNE)
        return QSPI_SR_RXFIFO_NOTEMPTY;
    else
        return QSPI_SR_RXFIFO_EMPTY;
}

/*!
    \brief      QSPI CTRLR1 cfg
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void Qspi_Ctrl1Ndf(QSPI_CORE_HANDLE *pdev, uint32_t num)
{
    CTRLR1_TypeDef ctrlr1;
    
    ctrlr1.d32 = num;
    
    REG32_WRITE((UINTPTR)&pdev->regs->CTRLR1,ctrlr1.d32);
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-Address and instruction transfer format.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/


int32_t Qspi_TransType(QSPI_CORE_HANDLE *pdev, uint32_t type)
{
    SPI_CTRLR0_TypeDef spi_ctrlr0;

    
    spi_ctrlr0.d32 = REG32_READ((UINTPTR)&pdev->regs->SPI_CTRLR0);
    if (type == SPI_TRANSFER_TYPE0)
        spi_ctrlr0.b.TRANS_TYPE = SPI_TRANSFER_TYPE0;
    else if (type == SPI_TRANSFER_TYPE1)
        spi_ctrlr0.b.TRANS_TYPE = SPI_TRANSFER_TYPE1; 
     else if (type == SPI_TRANSFER_TYPE2)
        spi_ctrlr0.b.TRANS_TYPE = SPI_TRANSFER_TYPE2;
    
    REG32_WRITE((UINTPTR)&pdev->regs->SPI_CTRLR0,spi_ctrlr0.d32);

    
    //QSPI_MODIFY_REG32(&pdev->regs->SPI_CTRLR0,0,spi_ctrlr0.d32);
    
    return 0;
}


/*!
    \brief      QSPI SPI_CTRLR0 cfg-defines Length of Address to be transmitted.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void Qspi_AddrMode(QSPI_CORE_HANDLE *pdev, uint32_t val)
{
    SPI_CTRLR0_TypeDef spi_ctrlr0;

    spi_ctrlr0.d32 = REG32_READ((UINTPTR)&pdev->regs->SPI_CTRLR0);
    if (val == SPI_FLASH_ADDR24) {
        spi_ctrlr0.b.ADDR_L = val;
        pdev->cfg.addr_mode = SPI_FLASH_ADDR24;
    }
    else if (val == SPI_FLASH_ADDR32) {
        spi_ctrlr0.b.ADDR_L = val;
        pdev->cfg.addr_mode = SPI_FLASH_ADDR32;
    }
	else {
		 spi_ctrlr0.b.ADDR_L = val;
		 pdev->cfg.addr_mode = val;
	}
    
    
    REG32_WRITE((UINTPTR)&pdev->regs->SPI_CTRLR0,spi_ctrlr0.d32);
    
    //QSPI_MODIFY_REG32(&pdev->regs->SPI_CTRLR0,0,spi_ctrlr0.d32);
    
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-defines Length of instruction to be transmitted.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void Qspi_InstructionLength(QSPI_CORE_HANDLE *pdev, uint32_t val)
{
    SPI_CTRLR0_TypeDef spi_ctrlr0;

    
    spi_ctrlr0.d32 = REG32_READ((UINTPTR)&pdev->regs->SPI_CTRLR0);
    spi_ctrlr0.b.INST_L = val;


    REG32_WRITE((UINTPTR)&pdev->regs->SPI_CTRLR0,spi_ctrlr0.d32);
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-dummy cycles.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void Qspi_WaitCycles(QSPI_CORE_HANDLE *pdev, uint32_t val)
{
    SPI_CTRLR0_TypeDef spi_ctrlr0;

    spi_ctrlr0.d32 = REG32_READ((UINTPTR)&pdev->regs->SPI_CTRLR0);
    spi_ctrlr0.b.WAIT_CYCLES = val;
    

    
    REG32_WRITE((UINTPTR)&pdev->regs->SPI_CTRLR0,spi_ctrlr0.d32);
   
}
