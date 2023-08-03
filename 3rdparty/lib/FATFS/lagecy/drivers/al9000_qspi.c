/*
 * al9000_qspi.c
 *
 *  Created on: 2021年9月29日
 *      Author: haitao.liu
 */

#include "demosoc.h"
#include "al9000_qspi.h"

/**************************AL9000******************************/
/*!
    \brief      Disable DWC_ssi
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/

int32_t dwc_ssi_disable(QSPI_TypeDef *qspi)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
    qspi->SSIENR = SSIC_DIS;
    return 0;
}
/*!
    \brief      Enable DWC_ssi
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/

int32_t dwc_ssi_enable(QSPI_TypeDef *qspi)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
    qspi->SSIENR = SSIC_EN;
    return 0;
}


/*!
    \brief      cfg SPI Frame Format - x1 mode (Standard)
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/

int32_t qspi_x1_mode(QSPI_TypeDef *qspi)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
//    qspi->CTRLR0 &= (~QSPI_CTRLR0_SPI_FRF);
	qspi->CTRLR0.SPI_FRF = SPI_FRF_X1_MODE;
    return 0;
}

/*!
    \brief      cfg SPI Frame Format - x2 mode (Dual)
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t qspi_x2_mode(QSPI_TypeDef *qspi)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
//	qspi->CTRLR0 &= (~QSPI_CTRLR0_SPI_FRF); // clr
//	qspi->CTRLR0 |=  BIT(22);
	qspi->CTRLR0.SPI_FRF = SPI_FRF_X2_MODE;
    return 0;
}
/*!
    \brief      cfg SPI Frame Format - x4 mode (Quad)
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/

int32_t qspi_x4_mode(QSPI_TypeDef *qspi)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
//	qspi->CTRLR0 &= (~QSPI_CTRLR0_SPI_FRF);
//	qspi->CTRLR0 |=  BIT(23);
	qspi->CTRLR0.SPI_FRF = SPI_FRF_X4_MODE;
    return 0;
}
/*!
    \brief      cfg QSPI Transfer Mode -Transmit & Receive;
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t qspi_tmod_tx_rx(QSPI_TypeDef *qspi)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
//	qspi->CTRLR0 &=  (~QSPI_CTRLR0_TMOD);
	qspi->CTRLR0.TMOD = TMOD_TX_RX;
    return 0;
}
/*!
    \brief      cfg QSPI0 Transfer Mode -Transmit only;
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t qspi_tmod_tx(QSPI_TypeDef *qspi)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
//	qspi->CTRLR0 &=  (~QSPI_CTRLR0_TMOD); // clr
//	qspi->CTRLR0 |= BIT(10);
	qspi->CTRLR0.TMOD = TMOD_TX_ONLY;
    return 0;
}
/*!
    \brief      cfg QSPI Transfer Mode -Receive only;
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t qspi_tmod_rx(QSPI_TypeDef *qspi)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
//	qspi->CTRLR0 &=  (~QSPI_CTRLR0_TMOD); // clr
//	qspi->CTRLR0 |= BIT(11);
	qspi->CTRLR0.TMOD = TMOD_RX_ONLY;
    return 0;
}
/*!
    \brief      cfg QSPI Transfer Mode -EEPROM Read mode;
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t qspi_tmod_e2prom(QSPI_TypeDef *qspi)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
//	qspi->CTRLR0 |= QSPI_CTRLR0_TMOD;
	qspi->CTRLR0.TMOD = TMOD_EEPROM;
	return 0;
}
/*!
    \brief      cfg QSPI Frame Format. --(SPI): Motorola SPI Frame Format
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t qspi_frf_spi(QSPI_TypeDef *qspi)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
//	qspi->CTRLR0 &= (~QSPI_CTRLR0_FRF);
	qspi->CTRLR0.FRF = FRF_SPI;
	return 0;
}
/*!
    \brief      Slave Select Toggle disable--ensure ncs keep valid before rx data after sent CMD.
    \param[in]  qspi: SPIx(x=0,1)
    \param[out] none
    \retval     none
*/
int32_t qspi_sste_dis(QSPI_TypeDef *qspi)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
//	qspi->CTRLR0 &= (~QSPI_CTRLR0_SSTE);
	qspi->CTRLR0.SSTE = CTRLR0_SSTE_DIS;
	return 0;
}

/*!
    \brief      cfg QSPI Data Frame Size.
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t qspi_dfs(QSPI_TypeDef *qspi, uint32_t len)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}

//	qspi->CTRLR0 |= len; // not ok
	qspi->CTRLR0.DFS = len;
	return 0;
}

/*!
    \brief      Slave Select Rigister cfg
    \param[in]  qspi: SPIx(x=0,1)
    \param[out] none
    \retval     none
*/
int32_t qspi_ser(QSPI_TypeDef *qspi, uint32_t slv)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
	qspi->SER = slv;
	return 0;
}


/*!
    \brief      cfg QSPI Clock Divider.
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t qspi_sckdiv_cfg(QSPI_TypeDef *qspi, uint32_t div)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
	qspi->BAUDR = (uint32_t) (div<<1);
    return 0;
}

/*!
    \brief      cfg QSPI Transmit FIFO Threshold Level.
    \param[in]  qspi: QSPI
    \param[out] none
    \retval     none
*/
int32_t qspi_txftl_tft(QSPI_TypeDef *qspi, uint32_t Threshold)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
//	qspi->TXFTLR |= (uint32_t) QSPI_TXFTLR_TFT;
	qspi->TXFTLR |= (uint32_t) Threshold;
    return 0;
}

/*!
    \brief      cfg QSPI Transfer start FIFO level.
    \param[in]  qspi: QSPI
    \param[out] none
    \retval     none
*/
int32_t qspi_txfifo_start_level_set(QSPI_TypeDef *qspi, uint32_t start_level)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
	qspi->TXFTLR &= ~(SPI_TXFTLR_TXFTHR(0xffffffff));  // clr txfthr bit area
	qspi->TXFTLR |= SPI_TXFTLR_TXFTHR(start_level);   // set txfthr bit area
    return 0;
}
/*!
    \brief      cfg QSPI Receive FIFO Threshold Level.
    \param[in]  qspi: QSPI0
    \param[out] none
    \retval     none
*/
int32_t qspi_rxftl_cfg(QSPI_TypeDef *qspi, uint32_t Threshold)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
//	qspi->RXFTLR |= (uint32_t) QSPI_RXFTLR_RFT;
	qspi->RXFTLR |= (uint32_t) Threshold;
	return 0;
}
/*!
    \brief      QSPI transmit data function
    \param[in]  qspi:
    \param[in]  data: data of transmission
    \param[out] none
    \retval     none
*/
int32_t qspi_data_transmit(QSPI_TypeDef *qspi, uint32_t data)
{
	if (UNLIKELY(qspi == NULL))
	{
		return -1;
	}
	qspi->DRx[0] = data;
    return 0;
}
/*!
    \brief      QSPI data read function
    \param[in]  qspi:
    \param[in]  data: data of transmission
    \param[out] none
    \retval     none
*/
uint32_t qspi_data_read(QSPI_TypeDef *qspi)
{
    return qspi->DRx[0];
}
/*!
    \brief      QSPI status TFE(Transmit FIFO Empty)function
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
uint32_t qspi_sr_tfe(QSPI_TypeDef *qspi)
{
    if(qspi->SR & SR_TFE_MASK)
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
uint32_t qspi_sr_busy(QSPI_TypeDef *qspi)
{
    if(qspi->SR & SR_BUSY_MASK)
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
uint32_t qspi_sr_rfne(QSPI_TypeDef *qspi)
{
    if(qspi->SR & SR_RFNE_MASK)
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
void qspi_ctrl1_ndf(QSPI_TypeDef *qspi, uint32_t num)
{
	qspi->CTRLR1 = num;
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-Address and instruction transfer format.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void qspi_spictrlr0_trans_type(QSPI_TypeDef *qspi, uint32_t val)
{
//	qspi->SPI_CTRLR0 |= val;
	qspi->SPI_CTRLR0.TRANS_TYPE = val;
}
/*!
    \brief      QSPI SPI_CTRLR0 cfg-defines Length of Address to be transmitted.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void qspi_spictrlr0_addr_l(QSPI_TypeDef *qspi, uint32_t val)
{
//	qspi->SPI_CTRLR0 |= (val<<2);
	qspi->SPI_CTRLR0.ADDR_L = val;
}
/*!
    \brief      QSPI SPI_CTRLR0 cfg-Mode bits enable in XIP mode.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void qspi_spictrlr0_xip_mod(QSPI_TypeDef *qspi, uint32_t val)
{
//	qspi->SPI_CTRLR0 |= (val<<7);
	qspi->SPI_CTRLR0.XIP_MD_BIT_EN = val;
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-defines Length of instruction to be transmitted.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void qspi_spictrlr0_inst_l(QSPI_TypeDef *qspi, uint32_t val)
{
//	qspi->SPI_CTRLR0 |= (val<<8);
	qspi->SPI_CTRLR0.INST_L = val;
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-dummy cycles.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void qspi_spictrlr0_wait_cycles(QSPI_TypeDef *qspi, uint32_t val)
{
//	qspi->SPI_CTRLR0 |= (val<<11);
	qspi->SPI_CTRLR0.WAIT_CYCLES = val;
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-Fix DFS for XIP transfers.
    \param[in]  qspi:
    \param[out] none
    \retval     none
*/
void qspi_spictrlr0_xip_dfs_hc(QSPI_TypeDef *qspi, uint32_t val)
{
//	qspi->SPI_CTRLR0 |= (val<<19);
	qspi->SPI_CTRLR0.XIP_DFS_HC = val;
}
/*!
    \brief      QSPI SPI_CTRLR0 cfg-XIP instruction enable bit.
    \param[in]  qspi:
    \param[out] none
    \retval     If this bit is set to 1 then XIP transfers will also have instruction phase.
*/
void qspi_spictrlr0_xip_inst_en(QSPI_TypeDef *qspi, uint32_t val)
{
//	qspi->SPI_CTRLR0 |= (val<<20);
	qspi->SPI_CTRLR0.XIP_INST_EN = val;
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-continuous transfer in XIP mode.
    \param[in]  qspi:
    \param[out] none
    \retval
*/
void qspi_spictrlr0_xip_cont_xfer_en(QSPI_TypeDef *qspi, uint32_t val)
{
//	qspi->SPI_CTRLR0 |= (val<<21);
	qspi->SPI_CTRLR0.SSIC_XIP_CONT_XFER_EN = val;
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-XIP Mode bits length.
    \param[in]  qspi:
    \param[out] none
    \retval
*/
void qspi_spictrlr0_xip_mbl(QSPI_TypeDef *qspi, uint32_t val)
{
//	qspi->SPI_CTRLR0 |= (val<<26);
	qspi->SPI_CTRLR0.XIP_MBL = val;
}

/*!
    \brief      QSPI SPI_CTRLR0 cfg-XIP pre-fetch functionality.
    \param[in]  qspi:
    \param[out] none
    \retval
*/
void qspi_spictrlr0_xip_prefetch_en(QSPI_TypeDef *qspi, uint32_t val)
{
//	qspi->SPI_CTRLR0 |= (val<<29);
	qspi->SPI_CTRLR0.XIP_PREFETCH_EN = val;
}
/*!
    \brief      QSPI SPI_CTRLR0 cfg-clock stretching capability in SPI transfers.
    \param[in]  qspi:
    \param[out] none
    \retval
*/
void qspi_spictrlr0_clk_stretch_en(QSPI_TypeDef *qspi, uint32_t val)
{
//	qspi->SPI_CTRLR0 |= (val<<30);
	qspi->SPI_CTRLR0.CLK_STRETCH_EN = val;
}
