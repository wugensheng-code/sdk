/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_ADDR_MAP_H
#define __AL_ADDR_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------
  Define System base addr
 *----------------------------------------------------------------------------*/
#define SYSTIMER__BASE_ADDR     (0x68030000UL)
#define CRP__BASE_ADDR          (0xF8801000UL)

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
#define HAVE_IICPS_DRIVER

#define AL_IIC_NUM_INSTANCE     (2)

#define I2C0__BASE_ADDR         (0xF8414000UL)
#define I2C1__BASE_ADDR         (0xF8415000UL)


/*----------------------------------------------------------------------------
  Define QSPI params
 *----------------------------------------------------------------------------*/
#define AL_QSPI_NUM_INSTANCE   (1)
#define QSPI_BASE_ADDR         (0xF804E000UL)
#define QSPI_FIFO_LENGTH       (128)



/*----------------------------------------------------------------------------
  Define SPI params
 *----------------------------------------------------------------------------*/
#define AL_SPI_NUM_INSTANCE    (2)

#define SPI0_BASE_ADDR         (0xF8404000UL)
#define SPI1_BASE_ADDR         (0xF8405000UL)
#define SPI_FIFO_LENGTH        (32)


/*----------------------------------------------------------------------------
  Define GBE params
 *----------------------------------------------------------------------------*/

#define AL_GBE_NUM_INSTANCE     (2)

#define CFG_CTRL_GBE0_ADDR      (0xf8800140UL)
#define CFG_CTRL_GBE1_ADDR      (0xf8800144UL)

#define GBE0__BASE_ADDR         (0xF8100000UL)
#define GBE1__BASE_ADDR         (0xF8110000UL)

/*----------------------------------------------------------------------------
  Define XAdc params
 *----------------------------------------------------------------------------*/

#define AL_XADC_NUM_INSTANCE    (1)

#define XADC__BASE_ADDR        (0xF840B000UL)


/*----------------------------------------------------------------------------
  Define SYSCNT_S
 *----------------------------------------------------------------------------*/
#define TOP_NS_BASE_ADDR        (0xF8800000UL)

#define SYSCNT_S_BASE           (0xF8804000UL)

#define TSG_CTRL_CNTCR          SYSCNT_S_BASE




#ifdef __cplusplus
}
#endif

#endif  /* __AL_PARAMS__ */
