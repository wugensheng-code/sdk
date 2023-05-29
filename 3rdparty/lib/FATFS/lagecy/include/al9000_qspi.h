/*
 * al9000_qspi.h
 *
 *  Created on: 2021年9月29日
 *      Author: haitao.liu
 */

#ifndef _AL9000_QSPI_H_
#define _AL9000_QSPI_H_

#include "demosoc.h"

/**************************L9000******************************/
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
//#define SPI_CTRLR0           0xf4
//#define DDR_DRIVE_EDGE       0xf8
//#define XIP_MODE_BITS        0xfc


/* SPI registers definitions */
//#define		QSPI_CTRLR0(spix)          			    REG32((spix) + 0x000U)                   /*!< QSPI Control Register 0*/
//#define		QSPI_CTRLR1(spix)          			    REG32((spix) + 0x004U)                   /*!< QSPI Control Register 1*/
//#define		QSPI_SSIENR(spix)          			    REG32((spix) + 0x008U)                   /*!< QSPI Enable Register*/
//#define		QSPI_SER(spix)             			 	REG32((spix) + 0x010U)                   /*!< QSPI Slave Enable Register*/
//#define		QSPI_BAUDR(spix)           			   	REG32((spix) + 0x014U)                   /*!< QSPI Baud Rate Select*/
//#define		QSPI_TXFTLR(spix)          			    REG32((spix) + 0x018U)                   /*!< QSPI TX FIFO Threshold Level*/
//#define		QSPI_RXFTLR(spix)          			    REG32((spix) + 0x01CU)                   /*!< QSPI RX FIFO Threshold Level*/
//#define		QSPI_IMR(spix)             			 	REG32((spix) + 0x02CU)                   /*!< QSPI Interrupt Mask Register，‘0’表示Mask，‘1’表示Unmask*/
//#define		QSPI_DMACR(spix)           			   	REG32((spix) + 0x04CU)                   /*!< QSPI DMA Control Register*/
//#define		QSPI_DMATDLR(spix)         			    REG32((spix) + 0x050U)                   /*!< QSPI DMA TX FIFO水线*/
//#define		QSPI_DMARDLR(spix)         			    REG32((spix) + 0x054U)                   /*!< QSPI DMA RX FIFO水线*/
//#define		QSPI_DR0(spix)               			REG32((spix) + 0x060U)                   /*!< QSPI Data Register，连续36个地址，实际只用到DR0一个*/
//#define		QSPI_RX_SAMPLE_DLY(spix)               	REG32((spix) + 0x0F0U)                   /*!< QSPI RX Sample Delay Register*/
//#define		QSPI_SPI_CTRLR0(spix)               	REG32((spix) + 0x0F4U)                   /*!< QSPI Control Register*/
//#define		QSPI_XIP_MODE_BITS(spix)               	REG32((spix) + 0x0FCU)                   /*!< QSPI XIP Mode Bits Register*/
//#define		QSPI_XIP_INCR_INST(spix)               	REG32((spix) + 0x100U)                   /*!< QSPI XIP INCR Transfer OPcode*/
//#define		QSPI_XIP_WRAP_INST(spix)               	REG32((spix) + 0x104U)                   /*!< QSPI XIP WRAP Transfer OPcode*/
//#define		QSPI_XIP_CNT_TIME_OUT(spix)             REG32((spix) + 0x114U)                   /*!< QSPI XIP Continuous Transfer Timeout Register*/

#define DFS_BYTE 			(7)	// 7+1=8 bits=byte
#define DFS_HALFWORD		(15)// 15+1=16 bits=halfword
#define DFS_WORD 			(31)// 31+1=32 bits=word
/***************flash cmd*****************/
#define DUMMY				(0x00)

#define CMD_RDID			(0x9F)
#define CMD_WREN			(0x06)
#define CMD_WRR  			(0x01)  // Write Register(status-1,configuration-1)

//#define CMD_SECTOR_ERASE    (0xD8)  // 256KB(=64*4KB)
#define CMD_CHIP_ERASE      (0x60)
#define CMD_PP     			(0x02)
#define CMD_QPP  			(0x32)
#define CMD_SREAD  			(0x03)
#define CMD_DREAD  			(0x3B)
#define CMD_QREAD  			(0x6B)

#define CMD_RDSR  			(0x05)
#define CMD_RESET			(0xf0)
#define SR1_WIP		        (1)
//#define GD25Q40B
/* bits definitions */
#define SSIC_EN 	1   /*Enables DWC_ssi*/
#define SSIC_DIS 	0	/*Disables DWC_ssi*/
/* QSPI_CTRLR0 */
#define QSPI_CTRLR0_MST                 BIT(31)        /*!< This field selects if DWC_ssi is working in Master or Slave mode. \
                                                               1-master;0-slave */
#define QSPI_CTRLR0_SPI_FRF             BITS(22,23)     /*!< SPI Frame Format. \
															0x0 (SPI_STANDARD): Standard SPI Format \
															0x1 (SPI_DUAL): Dual SPI Format \
															0x2 (SPI_QUAD): Quad SPI Format \
															0x3 (SPI_OCTAL): Octal SPI Format*/
// #define QSPI_CTRLR0_CFS                  BITS(19,16)     /*!< Control Frame Size. --Microwire*/
#define QSPI_CTRLR0_TMOD                BITS(10,11)    	/*!< Transfer Mode. \
															0x0: Transmit & Receive; Not Applicable in enhanced SPI operating mode(SSIC_HAS_TX_RX_EN=0)\
															0x1: Transmit only mode; Or Write in enhanced SPI operating mode\
															0x2: Receive only mode; Or Read in enhanced SPI operating mode\
															0x3: EEPROM Read mode; Not Applicable in enhanced SPI operating mode*/
#define QSPI_CTRLR0_SCPOL              	BIT(9)        	/*!< Serial Clock Polarity. \
															0x0: Inactive state of serial clock is low;\
															0x1: Inactive state of serial clock is high*/
#define QSPI_CTRLR0_SCPH               	BIT(8)        	/*!< Serial Clock Phase. \
															0x1 (START_BIT): Serial clock toggles at start of first bit \
															0x0 (MIDDLE_BIT): Serial clock toggles in middle of first bit*/
#define QSPI_CTRLR0_FRF              	BITS(6,7)        /*!< Frame Format. \
															0x0 (SPI): Motorola SPI Frame Format \
															0x1 (SSP): Texas Instruments SSP Frame Format\
															0x2 (MICROWIRE): National Semiconductors Microwire Frame Format\
															0x3 (RESERVED): Reserved */
#define QSPI_CTRLR0_DFS		            BITS(0,4)        /*!< Data Frame Size. Selects the data frame length.
															0x7 (DFS_08_BIT): 08-bit serial data transfer
															0xf (DFS_16_BIT): 16-bit serial data transfer
															0x1f (DFS_32_BIT): 32-bit serial data transfer.*/
#define QSPI_CTRLR0_SSTE              	BIT(14)


/* QSPI_CTRLR1 */
#define QSPI_CTRLR1_NDF		            BITS(0,15)        /*!< Number of Data Frames.
                                                                When TMOD = 10 or TMOD = 11, 
                                                                this register field sets the number of data frames to be continuously received by the DWC_ssi.*/
/* QSPI_SSIENR */
#define QSPI_SSIENR_SSI_EN              BIT(0)         /*!< SSI Enable. 0x1 (ENABLED): Enables DWC_ssi；0x0 (DISABLE): Disables DWC_ssi*/

/* QSPI_BAUDR */
#define QSPI_BAUDR_SCKDV                BITS(1,15)      /*!< SSI Clock Divider.Fsclk_out = Fssi_clk/SCKDV*/

/* QSPI_TXFTLR */
#define QSPI_TXFTLR_TFT                 BITS(1,5)      /*!< Transmit FIFO Threshold. Controls the level of entries (or below)
                                                            at which the transmit FIFO controller triggers an interrupt.*/

/* QSPI_TXFTLR */
#define QSPI_RXFTLR_RFT                 BITS(1,7)      /*!< Receive FIFO Threshold. Controls the level of entries (or below)
                                                            at which the Receive FIFO controller triggers an interrupt.*/
/* QSPI_IMR */



/*SER*/
#define SER_SS0_EN               			(1<<0)
#define SER_SS1_EN               			(1<<1)

/*SR*/
#define SR_BUSY_MASK						(1<<0)
#define QSPI_SR_BUSY						1
#define QSPI_SR_IDLE						0

#define SR_TFE_MASK							(1<<2)
#define QSPI_SR_TXFIFO_EMPTY				1
#define QSPI_SR_TXFIFO_NOTEMPTY				0

#define SR_RFNE_MASK						(1<<3)
#define QSPI_SR_RXFIFO_NOTEMPTY				1
#define QSPI_SR_RXFIFO_EMPTY				0

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

#define CTRLR0_SSTE_EN						1
#define CTRLR0_SSTE_DIS						0

#define SPI_TXFTLR_TXFTHR(x)    			(((x) & 0xff) << 16)
/*SPI_CTRLR0*/
#define TRANS_TYPE_TT0						0  // inst & addr both in x1 spi mode
#define TRANS_TYPE_TT1						1  // inst-x1 spi; addr-CTRLR0.SPI_FRF;
#define TRANS_TYPE_TT2						2  // inst & addr both in-CTRLR0.SPI_FRF;
#define XIP_MD_EN							1
#define XIP_MD_DIS							0
#define XIP_DFS_HC_EN						1
#define XIP_DFS_HC_DIS						0
#define XIP_INST_ENABLE						1
#define XIP_INST_DISABLE   					0
#define XIP_CONT_XFER_EN     				1
#define XIP_CONT_XFER_DIS     				0
#define XIP_MBL_2							0
#define XIP_MBL_4							1
#define XIP_MBL_8							2
#define XIP_MBL_16							3
#define XIP_PREFETCH_ENABLE					1
#define XIP_PREFETCH_DISABLE   				0
#define CLK_STRETCH_ENABLE					1
#define CLK_STRETCH_DISABLE					0

/*Func Declaration*/
int32_t dwc_ssi_disable(QSPI_TypeDef *qspi);
int32_t dwc_ssi_enable(QSPI_TypeDef *qspi);
int32_t qspi_x1_mode(QSPI_TypeDef *qspi);
int32_t qspi_x2_mode(QSPI_TypeDef *qspi);
int32_t qspi_x4_mode(QSPI_TypeDef *qspi);
int32_t qspi_tmod_tx_rx(QSPI_TypeDef *qspi);
int32_t qspi_tmod_tx(QSPI_TypeDef *qspi);
int32_t qspi_tmod_rx(QSPI_TypeDef *qspi);
int32_t qspi_tmod_e2prom(QSPI_TypeDef *qspi);
int32_t qspi_frf_spi(QSPI_TypeDef *qspi);
int32_t qspi_sste_dis(QSPI_TypeDef *qspi);
int32_t qspi_dfs(QSPI_TypeDef *qspi, uint32_t len);
int32_t qspi_ser(QSPI_TypeDef *qspi, uint32_t slv);
int32_t qspi_sckdiv_cfg(QSPI_TypeDef *qspi, uint32_t div);
int32_t qspi_txftl_tft(QSPI_TypeDef *qspi, uint32_t Threshold);
int32_t qspi_txfifo_start_level_set(QSPI_TypeDef *qspi, uint32_t start_level);
int32_t qspi_rxftl_cfg(QSPI_TypeDef *qspi, uint32_t Threshold);
int32_t qspi_data_transmit(QSPI_TypeDef *qspi, uint32_t data);
uint32_t qspi_data_read(QSPI_TypeDef *qspi);
uint32_t qspi_sr_tfe(QSPI_TypeDef *qspi);
uint32_t qspi_sr_busy(QSPI_TypeDef *qspi);
uint32_t qspi_sr_rfne(QSPI_TypeDef *qspi);
void qspi_ctrl1_ndf(QSPI_TypeDef *qspi, uint32_t num);
void qspi_spictrlr0_trans_type(QSPI_TypeDef *qspi, uint32_t val);
void qspi_spictrlr0_addr_l(QSPI_TypeDef *qspi, uint32_t val);
void qspi_spictrlr0_xip_mod(QSPI_TypeDef *qspi, uint32_t val);
void qspi_spictrlr0_inst_l(QSPI_TypeDef *qspi, uint32_t val);
void qspi_spictrlr0_wait_cycles(QSPI_TypeDef *qspi, uint32_t val);
void qspi_spictrlr0_xip_dfs_hc(QSPI_TypeDef *qspi, uint32_t val);
void qspi_spictrlr0_xip_inst_en(QSPI_TypeDef *qspi, uint32_t val);
void qspi_spictrlr0_xip_cont_xfer_en(QSPI_TypeDef *qspi, uint32_t val);
void qspi_spictrlr0_xip_mbl(QSPI_TypeDef *qspi, uint32_t val);
void qspi_spictrlr0_xip_prefetch_en(QSPI_TypeDef *qspi, uint32_t val);
void qspi_spictrlr0_clk_stretch_en(QSPI_TypeDef *qspi, uint32_t val);



#endif /* _AL9000_QSPI_H_ */
