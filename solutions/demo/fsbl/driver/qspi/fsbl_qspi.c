#include "fsbl_qspi.h"

/*!
    \brief      Slave Select Toggle disable--ensure ncs keep valid before rx data after sent CMD.
    \param[in]  qspi: SPIx(x=0,1)
    \param[out] none
    \retval     none
*/
#ifdef   QSPI_EXTENSION_FUNCTION
int32_t Qspi_Sste(QSPI_CORE_HANDLE *pdev, uint32_t sste_mode)
{
    CTRLR0_TypeDef ctrlr0;
    
    
    ctrlr0.d32 = 0;
    
    if (sste_mode == CTRLR0_SSTE_EN)
        ctrlr0.b.SSTE = CTRLR0_SSTE_EN;
    else if (sste_mode == CTRLR0_SSTE_DIS)
        ctrlr0.b.SSTE = CTRLR0_SSTE_DIS;
    else
        return -1;
    
    QSPI_MODIFY_REG32(&pdev->regs->CTRLR0,0,ctrlr0.d32);
   
    return 0;
}

#endif







#ifdef QSPI_XIP_ENABLE

/*!
    \brief      QSPI SPI_CTRLR0 cfg-Mode bits enable in XIP mode.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void Qsp_XipMod(QSPI_CORE_HANDLE *pdev, uint32_t val)
{
    SPI_CTRLR0_TypeDef spi_ctrlr0;
    uint32_t rspi_ctrlr0;

    spi_ctrlr0.d32 = 0;
    if (val = 1)
        spi_ctrlr0.b.XIP_MD_BIT_EN = 1;
    else 
        spi_ctrlr0.b.XIP_MD_BIT_EN = 0;
    
   QSPI_MODIFY_REG32(&pdev->regs->SPI_CTRLR0,0,spi_ctrlr0.d32);
}
#endif




/*!
    \brief      QSPI SPI_CTRLR0 cfg-clock stretching capability in SPI transfers.
    \param[in]  qspi:
    \param[out] none
    \retval
*/
#ifdef   QSPI_EXTENSION_FUNCTION
void Qspi_ClkStretchEn(QSPI_CORE_HANDLE *pdev, uint32_t val)
{
    SPI_CTRLR0_TypeDef spi_ctrlr0;

    spi_ctrlr0.d32 = 0;
   
    spi_ctrlr0.b.CLK_STRETCH_EN = val;
    QSPI_MODIFY_REG32(&pdev->regs->SPI_CTRLR0,0,spi_ctrlr0.d32);
}
/*!
    \brief      QSPI SPI_CTRLR0 cfg-Fix DFS for XIP transfers.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void Qspi_XipDfsHc(QSPI_CORE_HANDLE *pdev, uint32_t val)
{
    SPI_CTRLR0_TypeDef spi_ctrlr0=0;
    
    spi_ctrlr0.b.XIP_DFS_HC = val;
    QSPI_MODIFY_REG32(&pdev->regs->SPI_CTRLR0,0,spi_ctrlr0.d32);
}
/*!
    \brief      QSPI SPI_CTRLR0 cfg-XIP instruction enable bit.
    \param[in]  qspi:
    \param[out] none
    \retval     If this bit is set to 1 then XIP transfers will also have instruction phase.
*/
void Qspi_XipInstEn(QSPI_CORE_HANDLE *pdev, uint32_t val)
{
    SPI_CTRLR0_TypeDef spi_ctrlr0=0;
    
    spi_ctrlr0.b.XIP_INST_EN = val;
    
    QSPI_MODIFY_REG32(&pdev->regs->SPI_CTRLR0,0,spi_ctrlr0.d32);
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-continuous transfer in XIP mode.
    \param[in]  qspi:
    \param[out] none
    \retval
*/
void Qspi_XipContXferEn(QSPI_CORE_HANDLE *pdev, uint32_t val)
{
    SPI_CTRLR0_TypeDef spi_ctrlr0=0;
    
    spi_ctrlr0.b.SSIC_XIP_CONT_XFER_EN = val;

    QSPI_MODIFY_REG32(&pdev->regs->SPI_CTRLR0,0,spi_ctrlr0.d32);
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-XIP Mode bits length.
    \param[in]  qspi:
    \param[out] none
    \retval
*/
void Qspi_XipMbl(QSPI_CORE_HANDLE *pdev, uint32_t val)
{
    SPI_CTRLR0_TypeDef spi_ctrlr0=0;
    uint32_t rspi_ctrlr0;
    
    spi_ctrlr0.b.XIP_MBL = val;
    
    rspi_ctrlr0 = QSPI_READ_REG32(&pdev->regs->SPI_CTRLR0);
    
    rspi_ctrlr0 &= ~(0x3<<26);
    
    rspi_ctrlr0 |= spi_ctrlr0.d32;
    
    QSPI_WRITE_REG32(&pdev->regs->SPI_CTRLR0,rspi_ctrlr0);
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-XIP pre-fetch functionality.
    \param[in]  qspi:
    \param[out] none
    \retval
*/
void Qspi_XipPrefetchEn(QSPI_CORE_HANDLE *pdev, uint32_t val)
{
    SPI_CTRLR0_TypeDef spi_ctrlr0=0;
    spi_ctrlr0.b.XIP_PREFETCH_EN = val;
    QSPI_MODIFY_REG32(&pdev->regs->SPI_CTRLR0,0,spi_ctrlr0.d32);
}


#endif

#ifdef QSPI_DEBUG
void Qspi_Info(QSPI_CORE_HANDLE *pdev)
{
    CTRLR0_TypeDef ctrlr0;
    CTRLR1_TypeDef ctrlr1;
    SSIENR_TypeDef ssiner;
    SER_TypeDef    ser;
    BAUDR_TypeDef  baud;
    
    SPI_CTRLR0_TypeDef spi_ctrlr0;
   
    ctrlr0.d32 =  QSPI_READ_REG32(&pdev->regs->CTRLR0);
    ctrlr1.d32 =  QSPI_READ_REG32(&pdev->regs->CTRLR1);
    ssiner.d32 =  QSPI_READ_REG32(&pdev->regs->SSIENR);
    ser.d32     =  QSPI_READ_REG32(&pdev->regs->SER);
    baud.d32    =  QSPI_READ_REG32(&pdev->regs->BAUDR);
    spi_ctrlr0.d32 = QSPI_READ_REG32(&pdev->regs->SPI_CTRLR0);
    
}

#endif


