/*
 * al9000_spi.c
 *
 *  Created on: Oct.27.2021
 *      Author: haitao.liu
 */

#include "demosoc.h"
#include "al9000_spi.h"

/**************************AL9000******************************/
/*!
    \brief      Disable DWC_ssi
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/

int32_t spi_dwc_ssi_disable(SPI_TypeDef *spi)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
    spi->SSIENR = SSIC_DIS;
    return 0;
}
/*!
    \brief      Enable DWC_ssi
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/

int32_t spi_dwc_ssi_enable(SPI_TypeDef *spi)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
    spi->SSIENR = SSIC_EN;
    return 0;
}


/*!
    \brief      cfg SPI Frame Format - x1 mode (Standard)
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/

int32_t spi_x1_mode(SPI_TypeDef *spi)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
    spi->CTRLR0 &= (~SPI_CTRLR0_SPI_FRF);
//	spi->CTRLR0.SPI_FRF = SPI_FRF_X1_MODE;
    return 0;
}

/*!
    \brief      cfg SPI Frame Format - x2 mode (Dual)
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/
int32_t spi_x2_mode(SPI_TypeDef *spi)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->CTRLR0 &= (SPI_CTRLR0_SPI_FRF); // clr
	spi->CTRLR0 |=  BIT(21);
//	spi->CTRLR0.SPI_FRF = SPI_FRF_X2_MODE;
    return 0;
}
/*!
    \brief      cfg SPI Frame Format - x4 mode (Quad)
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/

int32_t spi_x4_mode(SPI_TypeDef *spi)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->CTRLR0 &= (~SPI_CTRLR0_SPI_FRF);
	spi->CTRLR0 |=  BIT(22);
//	spi->CTRLR0.SPI_FRF = SPI_FRF_X4_MODE;
    return 0;
}
/*!
    \brief      cfg spi Transfer Mode -Transmit & Receive;
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/
int32_t spi_tmod_tx_rx(SPI_TypeDef *spi)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->CTRLR0 &=  (~SPI_CTRLR0_TMOD);
//	spi->CTRLR0.TMOD = TMOD_TX_RX;
    return 0;
}
/*!
    \brief      cfg SPI Transfer Mode -Transmit only;
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/
int32_t spi_tmod_tx(SPI_TypeDef *spi)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->CTRLR0 &=  (~SPI_CTRLR0_TMOD); // clr
	spi->CTRLR0 |= BIT(8);
//	spi->CTRLR0.TMOD = TMOD_TX_ONLY;
    return 0;
}
/*!
    \brief      cfg spi Transfer Mode -Receive only;
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/
int32_t spi_tmod_rx(SPI_TypeDef *spi)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->CTRLR0 &=  (~SPI_CTRLR0_TMOD); // clr
	spi->CTRLR0 |= BIT(9);
//	spi->CTRLR0.TMOD = TMOD_RX_ONLY;
    return 0;
}
/*!
    \brief      cfg spi Transfer Mode -EEPROM Read mode;
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/
int32_t spi_tmod_e2prom(SPI_TypeDef *spi)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->CTRLR0 |= SPI_CTRLR0_TMOD;
//	spi->CTRLR0.TMOD = TMOD_EEPROM;
	return 0;
}
/*!
    \brief      cfg spi Frame Format. --(SPI): Motorola SPI Frame Format
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/
int32_t spi_frf_spi(SPI_TypeDef *spi)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->CTRLR0 &= (~SPI_CTRLR0_FRF);
//	spi->CTRLR0.FRF = FRF_SPI;
	return 0;
}


/*!
    \brief      cfg spi sck polarity.
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/
int32_t spi_scpol_cfg(SPI_TypeDef *spi,uint32_t val)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->CTRLR0 &= (~SPI_CTRLR0_SCPOL); // clr
	if(val == 1)
	{
		spi->CTRLR0 |= (SPI_CTRLR0_SCPOL); // high
	}
	else
	{
		spi->CTRLR0 |= ~(SPI_CTRLR0_SCPOL); // low
	}
	return 0;
}

/*!
    \brief      cfg spi sck phase.
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/
int32_t spi_scph_cfg(SPI_TypeDef *spi,uint32_t val)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->CTRLR0 &= (~SPI_CTRLR0_SCPH); // clr
	if(val == 1)
	{
		spi->CTRLR0 |= (SPI_CTRLR0_SCPH); // sck toggles at start of first data bit.
	}
	else
	{
		spi->CTRLR0 |= ~(SPI_CTRLR0_SCPH); // sck toggles in middle of first data bit.
	}
	return 0;
}

/*!
    \brief      Slave Select Toggle disable--ensure ncs keep valid before rx data after sent CMD.
    \param[in]  spi: SPIx(x=0,1)
    \param[out] none
    \retval     none
*/
int32_t spi_sste_dis(SPI_TypeDef *spi)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->CTRLR0 &= (~SPI_CTRLR0_SSTE);
	return 0;
}

/*!
    \brief      cfg spi Data Frame Size.
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/
int32_t spi_dfs(SPI_TypeDef *spi, uint32_t len)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->CTRLR0 |= len;
//	spi->CTRLR0.DFS = len;
	return 0;
}

/*!
    \brief      Slave Select Rigister cfg
    \param[in]  spi: SPIx(x=0,1)
    \param[out] none
    \retval     none
*/
int32_t spi_ser(SPI_TypeDef *spi, uint32_t slv)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->SER = slv;
	return 0;
}


/*!
    \brief      cfg spi Clock Divider.
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/
int32_t spi_sckdiv_cfg(SPI_TypeDef *spi, uint32_t div)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->BAUDR = (uint32_t) (div);
    return 0;
}

/*!
    \brief      cfg spi Transmit FIFO Threshold Level.
    \param[in]  spi: spi
    \param[out] none
    \retval     none
*/
int32_t spi_txftl_tft(SPI_TypeDef *spi, uint32_t Threshold)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->TXFTLR |= (uint32_t) Threshold;
    return 0;
}

/*!
    \brief      cfg spi Receive FIFO Threshold Level.
    \param[in]  spi: SPI
    \param[out] none
    \retval     none
*/
int32_t spi_rxftl_cfg(SPI_TypeDef *spi, uint32_t Threshold)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->RXFTLR |= (uint32_t) Threshold;
	return 0;
}
/*!
    \brief      spi transmit data function
    \param[in]  spi:
    \param[in]  data: data of transmission
    \param[out] none
    \retval     none
*/
int32_t spi_data_transmit(SPI_TypeDef *spi, uint32_t data)
{
	if (__RARELY(spi == NULL))
	{
		return -1;
	}
	spi->DRx[0] = data;
    return 0;
}
/*!
    \brief      spi data read function
    \param[in]  spi:
    \param[in]  data: data of transmission
    \param[out] none
    \retval     none
*/
uint32_t spi_data_read(SPI_TypeDef *spi)
{
    return spi->DRx[0];
}
/*!
    \brief      spi status TFE(Transmit FIFO Empty)function
    \param[in]  spi:
    \param[out] none
    \retval     none
*/
uint32_t spi_sr_tfe(SPI_TypeDef *spi)
{
    if(spi->SR & SR_TFE_MASK)
        return SPI_SR_TXFIFO_EMPTY;
    else
        return SPI_SR_TXFIFO_NOTEMPTY;
}
/*!
    \brief      spi status function
    \param[in]  spi:
    \param[out] none
    \retval     none
*/
uint32_t spi_sr_busy(SPI_TypeDef *spi)
{
    if(spi->SR & SR_BUSY_MASK)
        return SPI_SR_BUSY;
    else
        return SPI_SR_IDLE;
}

/*!
    \brief      check rx fifo empty or not
    \param[in]  spi:
    \param[out] none
    \retval     1-not empty;0-empty
*/
uint32_t spi_sr_rfne(SPI_TypeDef *spi)
{
    if(spi->SR & SR_RFNE_MASK)
        return SPI_SR_RXFIFO_NOTEMPTY;
    else
        return SPI_SR_RXFIFO_EMPTY;
}

/*!
    \brief      spi CTRLR1 cfg
    \param[in]  spi:
    \param[out] none
    \retval     none
*/
void spi_ctrl1_ndf(SPI_TypeDef *spi, uint32_t num)
{
	spi->CTRLR1 = num;
}


