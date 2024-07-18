/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_ADDR_MAP_H_
#define __AL_ADDR_MAP_H_

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------
  Define System base addr
 *----------------------------------------------------------------------------*/
#define SYSTIMER__BASE_ADDR     (0x68030000UL)
#define CRG__BASE_ADDR          (0xF8801000UL)

/*----------------------------------------------------------------------------
  Define Uart params
 *----------------------------------------------------------------------------*/
#define AL_UART_NUM_INSTANCE    (2)

#define UART0__BASE_ADDR        (0xF8400000UL)
#define UART1__BASE_ADDR        (0xF8401000UL)



/*----------------------------------------------------------------------------
  Define CAN params
 *----------------------------------------------------------------------------*/
#define AL_CAN_NUM_INSTANCE     (2)

#define CAN0__BASE_ADDR         (0xF8408000UL)
#define CAN1__BASE_ADDR         (0xF8409000UL)

#define CAN_TOP_CFG_CTRL_ADDR   (0xF8800164UL)

/*----------------------------------------------------------------------------
  Define CIPHER params
 *----------------------------------------------------------------------------*/

#define AL_CIPHER_NUM_INSTANCE     (1)

#define CIPHER__BASE_ADDR       (0xF8080000UL)

#define CIPHER_REQ_INTR_ADDR    (0xF841F410UL)

/*----------------------------------------------------------------------------
  Define DMACAHB params
 *----------------------------------------------------------------------------*/

#define AL_DMACAHB_NUM_INSTANCE (1)

#define DMACAHB__BASE_ADDR      (0xF804D000UL)

#define DMACAHB_CH_NUM          (8)

/* 0xF841F800 - 0xF841F80C */
#define DMACAHB_LOCK_ADDR       (0xF841F80CUL)
#define DMACAHB_CH_STATE_ADDR   (0xF88001B0UL)



/*---------------------------------------------------------------------------
  Define MMC params
 *----------------------------------------------------------------------------*/
#define AL_MMC_NUM_INSTANCE     (2)

#define MMC0__BASE_ADDR         (0xF8049000UL)
#define MMC1__BASE_ADDR         (0xF804A000UL)

#define MMC0_TOP_CFG_CTRL_ADDR  (0xF8800150UL)
#define MMC1_TOP_CFG_CTRL_ADDR  (0xF8800154UL)

#define MMC0_BUS_WIDTH          (8)
#define MMC1_BUS_WIDTH          (4)

/*----------------------------------------------------------------------------
  Define IIC params
 *----------------------------------------------------------------------------*/


#define AL_IIC_NUM_INSTANCE     (2)

#define I2C0__BASE_ADDR         (0xF8414000UL)
#define I2C1__BASE_ADDR         (0xF8415000UL)

/*----------------------------------------------------------------------------
  Define USB params
 *----------------------------------------------------------------------------*/
#define AL_USB_NUM_INSTANCE     (1)

#define USB0__BASE_ADDR         (0xF8180000UL)
#define USB1__BASE_ADDR         (0xF81C0000UL)

/*----------------------------------------------------------------------------
  Define QSPI params
 *----------------------------------------------------------------------------*/
#define AL_QSPI_NUM_INSTANCE   (1)
#define QSPI_BASE_ADDR         (0xF804E000UL)
#define QSPI_FIFO_LENGTH       (128)

#define QSPI_CS0_EN      (1 << 0)
#define QSPI_CS1_EN      (1 << 1)
#define QSPI_CS2_EN      (1 << 2)

/*----------------------------------------------------------------------------
  Define SPI params
 *----------------------------------------------------------------------------*/
#define AL_SPI_NUM_INSTANCE    (2)

#define SPI0_BASE_ADDR         (0xF8404000UL)
#define SPI1_BASE_ADDR         (0xF8405000UL)
#define SPI_FIFO_LENGTH        (32)

#define  SPI_CS0_EN        (1 << 0)
#define  SPI_CS1_EN        (1 << 1)
#define  SPI_CS2_EN        (1 << 2)

/*----------------------------------------------------------------------------
  Define GBE params
 *----------------------------------------------------------------------------*/

#define AL_GBE_NUM_INSTANCE     (2)

#define CFG_CTRL_GBE0_ADDR      (0xf8800140UL)
#define CFG_CTRL_GBE1_ADDR      (0xf8800144UL)

#define GBE0__BASE_ADDR         (0xF8100000UL)
#define GBE1__BASE_ADDR         (0xF8110000UL)


/*----------------------------------------------------------------------------
  Define Adc params
 *----------------------------------------------------------------------------*/

#define AL_ADC_NUM_INSTANCE    (1)

#define ADC__BASE_ADDR        (0xF840B000UL)

/*----------------------------------------------------------------------------
  Define TC params
 *----------------------------------------------------------------------------*/

#define AL_TC_NUM_INSTANCE    (6)

#define TC0__TC0_BASE_ADDR    0xF840C000ULL
#define TC0__TC1_BASE_ADDR    0xF840C004ULL
#define TC0__TC2_BASE_ADDR    0xF840C008ULL
#define TC1__TC0_BASE_ADDR    0xF840D000ULL
#define TC1__TC1_BASE_ADDR    0xF840D004ULL
#define TC1__TC2_BASE_ADDR    0xF840D008ULL


/*----------------------------------------------------------------------------
  Define WDT params
 *----------------------------------------------------------------------------*/

#define AL_WDT_NUM_INSTANCE     (2)

#define WDT0__BASE_ADDR         (0xF8410000UL)
#define WDT1__BASE_ADDR         (0xF8410200UL)

#define WDT_TOP_CFG_CTRL_ADDR   (0XF8800168UL)

/*----------------------------------------------------------------------------
  Define DMA params
 *----------------------------------------------------------------------------*/
#define AL_DMA_NUM_INSTANCE     (1)
#define AL_DMA_S_BASE_ADDR      (0xF8418000)
#define AL_DMA_NS_BASE_ADDR     (0xF8419000)
#define AL_DMA_CHAN_COUNT       (8)

/* 0xF841F800 - 0xF841F80C */
#define AL_DMA_LOCK_ADDR        (0xF841F808UL)

/*----------------------------------------------------------------------------
  Define IPC params
 *----------------------------------------------------------------------------*/
#define AL_IPC_NUM_INSTANCE     (1)
#define AL_IPC_INTR_NUM_INSTANCE    (4)
#define AL_IPC_BASE_ADDR        (0xF841F000)

/*----------------------------------------------------------------------------
  Define XMON params
 *----------------------------------------------------------------------------*/
#define AL_XMON_NUM_INSTANCE       (21)
#define AL_XMON_DDR_S0_BASE_ADDR  (0xF8440000UL)
#define AL_XMON_DDR_S1_BASE_ADDR  (0xF8440400UL)
#define AL_XMON_DDR_S2_BASE_ADDR  (0xF8440800UL)
#define AL_XMON_DDR_S3_BASE_ADDR  (0xF8440C00UL)
#define AL_XMON_SX2X_M0_BASE_ADDR (0xF8441000UL)
#define AL_XMON_SX2X_M1_BASE_ADDR (0xF8441400UL)
#define AL_XMON_OCM_S2_BASE_ADDR  (0xF8441800UL)
#define AL_XMON_SH_M2_BASE_ADDR   (0xF8441C00UL)
#define AL_XMON_SMC_BASE_ADDR     (0xF8442000UL)
#define AL_XMON_DMACX_BASE_ADDR   (0xF8442400UL)
#define AL_XMON_GP_S0_BASE_ADDR   (0xF8444000UL)
#define AL_XMON_GP_S1_BASE_ADDR   (0xF8444400UL)
#define AL_XMON_HP_S0_BASE_ADDR   (0xF8444800UL)
#define AL_XMON_HP_S1_BASE_ADDR   (0xF8444C00UL)
#define AL_XMON_APU_BASE_ADDR     (0xF8445000UL)
#define AL_XMON_JPU_BASE_ADDR     (0xF8445400UL)
#define AL_XMON_MAIN_S6_BASE_ADDR (0xF8445800UL)
#define AL_XMON_MAIN_M6_BASE_ADDR (0xF8445C00UL)
#define AL_XMON_MAIN_S1_BASE_ADDR (0xF8446000UL)
#define AL_XMON_MAIN_M0_BASE_ADDR (0xF8446400UL)
#define AL_XMON_ACP_BASE_ADDR     (0xF8446800UL)

#define XMON_TOP_INTR_STATE_ADDR  (0xF8800210UL)

/*----------------------------------------------------------------------------
  Define TOP
 *----------------------------------------------------------------------------*/
#define TOP_NS_BASE_ADDR        (0xF8800000UL)
#define TOP_CRP_BASE_ADDR       (0xF8801000UL)
#define TOP_SYSCNT_S_BASE_ADDR  (0xF8804000UL)
#define TOP_S_BASE_ADDR         (0xF8806000UL)

#define TSG_CTRL_CNTCR          TOP_SYSCNT_S_BASE_ADDR


#ifdef __cplusplus
}
#endif

#endif  /* __AL_PARAMS__ */
