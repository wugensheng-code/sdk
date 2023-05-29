/*
 * al9000_spi.h
 *
 *  Created on: Oct.27.2021
 *      Author: haitao.liu
 */

#ifndef _AL9000_SPI_H_
#define _AL9000_SPI_H_

#include "demosoc.h"

/**************************AL9000******************************/
/* Register offsets */
//#define CTRLR0               0x00
//#define CTRLR1               0x04
//#define SSIENR               0x08
//#define MWCR                 0x0C
//#define SER                  0x10
//#define BAUDR                0x14
//#define TXFTLR               0x18
//#define RXFTLR               0x1c
//#define TXFLR                0x20
//#define RXFLR                0x24
//#define SR                   0x28
//#define IMR                  0x2C
//#define ISR                  0x30
//#define RISR                 0x34
//#define TXOICR               0x38
//#define RXOICR               0x3c
//#define RXUICR               0x40
//#define MSTICR               0x44
//#define ICR                  0x48
//#define DMACR                0x4c
//#define DMATDLR              0x50
//#define DMARDLR              0x54
//#define IDR                  0x58
//#define SSIC_VERSION_ID      0x5c
//#define DR0                  0x60
//#define RX_SAMPLE_DELAY      0xf0


#define SPI_DFS_BYTE 			(7)	// 7+1=8 bits=byte
#define SPI_DFS_WORD 			(31)// 31+1=32 bits=word
/***************flash cmd*****************/
#define DUMMY				(0x00)

#define CMD_RDID			(0x9F)
#define CMD_WREN			(0x06)
#define CMD_WRR  			(0x01)  // Write Register(status-1,configuration-1)
#define CMD_SECTOR_ERASE    (0x20)  // 256KB(=64*4KB)
#define CMD_CHIP_ERASE      (0x60)
#define CMD_PP     			(0x02)
#define CMD_SREAD  			(0x03)

//#define CMD_DREAD  			(0x3B)
//#define CMD_QREAD  			(0x6B)
#define CMD_RDSR  			(0x05)

#define SR1_WIP		        (1)


/* bits definitions */
#define SSIC_EN 	        1   /*Enables DWC_ssi*/
#define SSIC_DIS 	        0	/*Disables DWC_ssi*/

/* SPI_CTRLR0*/
 #define SPI_CTRLR0_SPI_FRF             BITS(21,22)     /*!< SPI Frame Format. \
 															0x0 (SPI_STANDARD): Standard SPI Format \
 															0x1 (SPI_DUAL): Dual SPI Format \
 															0x2 (SPI_QUAD): Quad SPI Format \
 															0x3 (SPI_OCTAL): Octal SPI Format*/
 // #define SPI_CTRLR0_CFS                  BITS(12,15)     /*!< Control Frame Size. --Microwire*/
 #define SPI_CTRLR0_TMOD                BITS(8,9)    	/*!< Transfer Mode. \
 															0x0: Transmit & Receive; Not Applicable in enhanced SPI operating mode(SSIC_HAS_TX_RX_EN=0)\
 															0x1: Transmit only mode; Or Write in enhanced SPI operating mode\
 															0x2: Receive only mode; Or Read in enhanced SPI operating mode\
 															0x3: EEPROM Read mode; Not Applicable in enhanced SPI operating mode*/
 #define SPI_CTRLR0_SCPOL              	BIT(7)        	/*!< Serial Clock Polarity. \
 															0x0: Inactive state of serial clock is low;\
 															0x1: Inactive state of serial clock is high*/
 #define SPI_CTRLR0_SCPH               	BIT(6)        	/*!< Serial Clock Phase. \
 															0x1 (START_BIT): Serial clock toggles at start of first bit \
 															0x0 (MIDDLE_BIT): Serial clock toggles in middle of first bit*/
 #define SPI_CTRLR0_FRF              	BITS(4,5)        /*!< Frame Format. \
 															0x0 (SPI): Motorola SPI Frame Format \
 															0x1 (SSP): Texas Instruments SSP Frame Format\
 															0x2 (MICROWIRE): National Semiconductors Microwire Frame Format\
 															0x3 (RESERVED): Reserved */
 #define SPI_CTRLR0_DFS		            BITS(0,3)        /*!< Data Frame Size. Selects the data frame length.
 															0x7 (DFS_08_BIT): 08-bit serial data transfer
 															0xf (DFS_16_BIT): 16-bit serial data transfer
 															0x1f (DFS_32_BIT): 32-bit serial data transfer.*/
 #define SPI_CTRLR0_SSTE              	BIT(24)


/* SPI_CTRLR1 */
#define SPI_CTRLR1_NDF		            BITS(0,15)        /*!< Number of Data Frames.
                                                                When TMOD = 10 or TMOD = 11, 
                                                                this register field sets the number of data frames to be continuously received by the DWC_ssi.*/
/* SPI_SSIENR */
#define SPI_SSIENR_SSI_EN              BIT(0)         /*!< SSI Enable. 0x1 (ENABLED): Enables DWC_ssi；0x0 (DISABLE): Disables DWC_ssi*/

/* SPI_BAUDR */
#define SPI_BAUDR_SCKDV                BITS(0,15)      /*!< SSI Clock Divider.Fsclk_out = Fssi_clk/SCKDV,SCKDV is any even value between 2 and 65534*/

/* SPI_TXFTLR */
#define SPI_TXFTLR_TFT                 BITS(0,7)      /*!< Transmit FIFO Threshold. Controls the level of entries (or below)
                                                            at which the transmit FIFO controller triggers an interrupt.*/

/* SPI_TXFTLR */
#define SPI_RXFTLR_RFT                 BITS(0,7)      /*!< Receive FIFO Threshold. Controls the level of entries (or below)
                                                            at which the Receive FIFO controller triggers an interrupt.*/
/* SPI_IMR */

/* SPI DMA Control Register*/
#define SPI_DMACR_TDMAE	BIT(1)
#define SPI_DMACR_RDMAE	BIT(0)
/*SER*/
#define SER_SS0_EN               			(1<<0)
#define SER_SS1_EN               			(1<<1)
#define SER_SS2_EN               			(1<<2)

/*SR*/
#define SR_BUSY_MASK						(1<<0)
#define SPI_SR_BUSY							1
#define SPI_SR_IDLE							0

#define SR_TFE_MASK							(1<<2)
#define SPI_SR_TXFIFO_EMPTY					1
#define SPI_SR_TXFIFO_NOTEMPTY				0

#define SR_TFNF_MASK						(1<<1)
#define SPI_SR_TXFIFO_NOTFULL				1
#define SPI_SR_TXFIFO_FULL					0

#define SR_RFNE_MASK						(1<<3)
#define SPI_SR_RXFIFO_NOTEMPTY				1
#define SPI_SR_RXFIFO_EMPTY					0

#define SR_RFF_MASK							(1<<4)
#define SPI_SR_RXFIFO_FULL					1
#define SPI_SR_RXFIFO_NOT_FULL				0

#define FRF_SPI								0
#define FRF_SSP								1
#define FRF_MICR							2

#define SPI_MAS								1
#define SPI_SLV								0

#define TMOD_TX_RX				     		0
#define TMOD_TX_ONLY						1
#define TMOD_RX_ONLY						2
#define TMOD_EEPROM						    3

#define SPI_FRF_X1_MODE						0
#define SPI_FRF_X2_MODE						1
#define SPI_FRF_X4_MODE						2
#define SPI_FRF_X8_MODE						3


/*Func Declaration*/
 int32_t spi_dwc_ssi_disable(SPI_TypeDef *spi);
 int32_t spi_dwc_ssi_enable(SPI_TypeDef *spi);
 int32_t spi_x1_mode(SPI_TypeDef *spi);
 int32_t spi_x2_mode(SPI_TypeDef *spi);
 int32_t spi_x4_mode(SPI_TypeDef *spi);
 int32_t spi_tmod_tx_rx(SPI_TypeDef *spi);
 int32_t spi_tmod_tx(SPI_TypeDef *spi);
 int32_t spi_tmod_rx(SPI_TypeDef *spi);
 int32_t spi_tmod_e2prom(SPI_TypeDef *spi);
 int32_t spi_frf_spi(SPI_TypeDef *spi);
 int32_t spi_scpol_cfg(SPI_TypeDef *spi,uint32_t val);
 int32_t spi_scph_cfg(SPI_TypeDef *spi,uint32_t val);
 int32_t spi_sste_dis(SPI_TypeDef *spi);
 int32_t spi_dfs(SPI_TypeDef *spi, uint32_t len);
 int32_t spi_ser(SPI_TypeDef *spi, uint32_t slv);
 int32_t spi_sckdiv_cfg(SPI_TypeDef *spi, uint32_t div);
 int32_t spi_txftl_tft(SPI_TypeDef *spi, uint32_t Threshold);
 int32_t spi_rxftl_cfg(SPI_TypeDef *spi, uint32_t Threshold);
 int32_t spi_data_transmit(SPI_TypeDef *spi, uint32_t data);
 uint32_t spi_data_read(SPI_TypeDef *spi);
 uint32_t spi_sr_tfe(SPI_TypeDef *spi);
 uint32_t spi_sr_tfnf(SPI_TypeDef *spi);
 uint32_t spi_sr_busy(SPI_TypeDef *spi);
 uint32_t spi_sr_rfne(SPI_TypeDef *spi);
 uint32_t spi_sr_rff(SPI_TypeDef *spi);
 void spi_ctrl1_ndf(SPI_TypeDef *spi, uint32_t num);

#endif /* _AL9000_SPI_H_ */
