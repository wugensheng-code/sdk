/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _ZX_XEC_H
#define _ZX_XEC_H

/*!
 * @file     zx_xec.h
 * @brief    This file contains all the functions prototypes for the XEC firmware
 */

#ifdef __cplusplus
 extern "C" {
#endif

#include "../../../demosoc/Common/Include/demosoc.h"

typedef struct  {

   __IO  uint32_t   DTYPE_SPEC_0;           /*!< Buffer2  */
   __IO  uint32_t   DTYPE_SPEC_1;           /*!< Buffer1 address pointer */
   __IO  uint32_t   DTYPE_SPEC_2;           /*!< Control and Buffer1, Buffer2 lengths */
   __IO  uint32_t   COMM;                   /*!< Status */
} ETH_DMADESCTypeDef;

typedef struct{
  uint32_t length;
  uint32_t buffer;
  __IO ETH_DMADESCTypeDef *descriptor;
}FrameTypeDef;

#define BigLittleSwap16(A)        ((((uint16_t)(A) & 0xff00) >> 8) |                                                        (((uint16_t)(A) & 0x00ff) << 8))

#define BigLittleSwap32(A)        ((((uint32_t)(A) & 0xff000000) >> 24) |                                                        (((uint32_t)(A) & 0x00ff0000) >> 8) |                                                        (((uint32_t)(A) & 0x0000ff00) << 8) |                                                        (((uint32_t)(A) & 0x000000ff) << 24))

#define ETH_RXBUFNB 8   //8
#define ETH_TXBUFNB 8   //8
#define ETH_MTU         4096  //1024//
#define ETH_RX_BUF_SIZE ETH_MTU
#define ETH_TX_BUF_SIZE ETH_MTU
#define TX_RING_LEN         ETH_TXBUFNB
#define TX_DESC_SKIP_LEN    0
#define RX_RING_LEN         ETH_RXBUFNB
#define RX_DESC_SKIP_LEN    0

#define RECE_NO_ERROR BITS(11,14)

/*  Common  Descriptor  */
#define DINFO                BITS(24,27)
#define DTYPE                BITS(28,31)
#define DTYPE_TxBuff         0x0<<28
#define DTYPE_TxCtxt0        0x1<<28
#define DTYPE_TxPkt0         0x2<<28

#define DTYPE_RxBuff         0x1<<28
#define DTYPE_RxCtxt0        0x2<<28

#define DINFO_HW             0x1<<24
#define DINFO_NXTD           0x2<<24
#define DINFO_INT           0x4<<24

/*    TxBuff Descriptor   */

#define BADDR0                 BITS(0,31)
#define BADDR1                 BITS(0,31)
#define HLEN                   BITS(0,15)
#define FLEN                 BITS(16,31)
#define PKTOP                BITS(0,9)
#define PKTOP_CSE               BIT(0)
#define PKTOP_TSE               BIT(1)
#define PKTOP_CRC_CTRL        BITS(2,3)
#define PKTOP_PAD_EN            BIT(4)

#define PKTID                BITS(10,19)
#define SUMM                 BITS(20,23)
#define SUMM_BUF0V            0x1<<20
#define SUMM_BUF1V            0x2<<20
#define SUMM_FD               0x4<<20
#define SUMM_LD               0x8<<20

/*     Rx DMA Descriptor   :   RxSts0 Descriptor */

#define RxSts0_BSIZE0                   BITS(0,15)
#define RxSts0_FLEN                     BITS(16,31)
#define RxSts0_FLENShift                16

#define RxSts0_RXSTS_FLENV              1<<0
#define RxSts0_RXSTS_FERR               2<<0
#define RxSts0_RXSTS_RUT_VLD            4<<0
#define RxSts0_RXSTS_LUT_RUT            0x38<<0
#define RxSts0_RXSTS_LUT_IDX            0x7c0<<0
#define RxSts0_RXSTS_Frame_Error        0xf<<11<<0
#define RxSts0_RXSTS                    BITS(0,31)

#define ETH_DMARxDesc_FrameLengthShift      16

/*       Rx Driver Descriptor   :  RxBuff Descriptor */

#define RxBuff_BADDR0           BITS(0,31)
#define RxBuff_BADDR0_HI        BITS(0,31)
#define RxBuff_BADDR1           BITS(0,31)
#define RxBuff_BADDR1_HI        BITS(0,19)
#define RxBuff_SUMM             BITS(20,23)
#define RxBuff_SUMM_OFS         20
#define RxBuff_SUMM_BUF0V       0x1<<RxBuff_SUMM_OFS
#define RxBuff_SUMM_BUF1V       0x2<<RxBuff_SUMM_OFS

/*            LUT             */
#define LUT_BUSY                BIT(31)
#define LUT_OT                  BIT(30)
#define LUT_OT_WIRTE            BIT(30)
#define LUT_OT_READ             ~BIT(30)

#define LUT_ADDR                BITS(0,29)

#define LUT_ACT                 BITS(1,3)
#define LUT_CE                  BIT(0)

#define LUT_ENTRY_MH             BITS(0,31)

#define LUT_ENTRY_ML_MASK_L      BITS(16,31)
#define LUT_ENTRY_ML_ENTRY_H     BITS(0,15)

#define LUT_ENTRY_L             BITS(0,31)
#endif

/*    PHY          */
#define PHY_BCR                          0          /*!< Transceiver Basic Control Register */
#define PHY_BSR                          1          /*!< Transceiver Basic Status Register */

#define ETH_AutoNegotiation_Disable DISABLE

/*  Interrupt  */
#define RX_DESC_BUS_INTR_EN  BIT(18)
#define RX_DATA_BUS_INTR_EN BIT(17)
#define RX_PKT_INTR_EN BIT(16)
#define TX_DESC_BUS_INTR_EN BIT(2)
#define TX_DATA_BUS_INTR_EN BIT(1)
#define TX_PKT_INTR_EN BIT(0)

#define MII_PORT_SEL            BITS(0,1)
#define MII_SPEED_SEL           BITS(2,3)
#define REF_CLK_SEL             BITS(4,5)
#define HDX_MODE                BIT(6)
#define LOOPBACK_MODE           BIT(7)


//#define MDIO_B2B_EN             BIT(17)

#define MAC_MDIO_CS_MASK        BITS(0,30)
#define MAC_MDIO_REGAD          BITS(16,20)
//#define MAC_MDIO_PHYAD_ClAUSE45         BITS(16,20)
#define MAC_MDIO_PHYAD          BITS(21,25)
//#define MAC_MDIO_DEVAD          BITS(21,26)

//#define MAC_MDIO_OP             BITS(26,27)
#define MAC_MDIO_OP_READ          3 <<2
//#define MAC_MDIO_OP_Post_Read   2 <<26
#define MAC_MDIO_OP_WRITE         1 <<2
//#define MAC_MDIO_OP             BITS(26,27)
#define MAC_MDIO_PA                BITS(21,25)  //PHY ADDR
#define MAC_MDIO_RDA               BITS(16,20)  //REG ADDR
#define MDC_DIV_RARIO              BITS(8,11)
#define MAC_MDIO_SKIP_ADDR         BIT(4)
#define MAC_MDIO_GMII_OPERATION         BITS(2,3)
#define MDIO_CL45_MODE          BIT(1)
#define MAC_MDIO_GB_BUSY         0x1
#define MAC_MDIO_PRE_BUSY        0x1
#define PHY_WRITE_TO             0xfff
#define PHY_READ_TO              0xfff
/*=============GBE registers========================================*/
///////////////////////////////////////////////////////
// Register: DMA_Mode
// Tx Packet Count Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned swr : 1;
        unsigned da : 1;
        unsigned reserved_4_2 : 3;
        unsigned reserved_7_5 : 3;
        unsigned reserved_8 : 1;
        unsigned arbc : 1;
        unsigned reserved_10 : 1;
        unsigned txpr : 1;
        unsigned pr : 3;
        unsigned reserved_15 : 1;
        unsigned intm : 2;
        unsigned reserved_31_18 : 14;
    };
    unsigned reg;
} GBE__DMA_Mode__ACC_T;

#define GBE__GBE0__DMA_MODE__ADDR (GBE__GBE0__BASE_ADDR + 0x1000)
#define GBE__GBE0__DMA_MODE__NUM  0x1

#define GBE__GBE0__DMA_MODE__SWR__SHIFT    0
#define GBE__GBE0__DMA_MODE__DA__SHIFT    1
#define GBE__GBE0__DMA_MODE__RESERVED_4_2__SHIFT    2
#define GBE__GBE0__DMA_MODE__RESERVED_7_5__SHIFT    5
#define GBE__GBE0__DMA_MODE__RESERVED_8__SHIFT    8
#define GBE__GBE0__DMA_MODE__ARBC__SHIFT    9
#define GBE__GBE0__DMA_MODE__RESERVED_10__SHIFT    10
#define GBE__GBE0__DMA_MODE__TXPR__SHIFT    11
#define GBE__GBE0__DMA_MODE__PR__SHIFT    12
#define GBE__GBE0__DMA_MODE__RESERVED_15__SHIFT    15
#define GBE__GBE0__DMA_MODE__INTM__SHIFT    16
#define GBE__GBE0__DMA_MODE__RESERVED_31_18__SHIFT    18

#define GBE__GBE0__DMA_MODE__SWR__MASK    0x00000001
#define GBE__GBE0__DMA_MODE__DA__MASK    0x00000002
#define GBE__GBE0__DMA_MODE__RESERVED_4_2__MASK    0x0000001c
#define GBE__GBE0__DMA_MODE__RESERVED_7_5__MASK    0x000000e0
#define GBE__GBE0__DMA_MODE__RESERVED_8__MASK    0x00000100
#define GBE__GBE0__DMA_MODE__ARBC__MASK    0x00000200
#define GBE__GBE0__DMA_MODE__RESERVED_10__MASK    0x00000400
#define GBE__GBE0__DMA_MODE__TXPR__MASK    0x00000800
#define GBE__GBE0__DMA_MODE__PR__MASK    0x00007000
#define GBE__GBE0__DMA_MODE__RESERVED_15__MASK    0x00008000
#define GBE__GBE0__DMA_MODE__INTM__MASK    0x00030000
#define GBE__GBE0__DMA_MODE__RESERVED_31_18__MASK    0xfffc0000

#define GBE__GBE0__DMA_MODE__SWR__POR_VALUE    0x0
#define GBE__GBE0__DMA_MODE__DA__POR_VALUE    0x0
#define GBE__GBE0__DMA_MODE__RESERVED_4_2__POR_VALUE    0x0
#define GBE__GBE0__DMA_MODE__RESERVED_7_5__POR_VALUE    0x0
#define GBE__GBE0__DMA_MODE__RESERVED_8__POR_VALUE    0x0
#define GBE__GBE0__DMA_MODE__ARBC__POR_VALUE    0x0
#define GBE__GBE0__DMA_MODE__RESERVED_10__POR_VALUE    0x0
#define GBE__GBE0__DMA_MODE__TXPR__POR_VALUE    0x0
#define GBE__GBE0__DMA_MODE__PR__POR_VALUE    0x0
#define GBE__GBE0__DMA_MODE__RESERVED_15__POR_VALUE    0x0
#define GBE__GBE0__DMA_MODE__INTM__POR_VALUE    0x0
#define GBE__GBE0__DMA_MODE__RESERVED_31_18__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_SysBus_Mode
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned fb : 1;
        unsigned reserved_11_1 : 11;
        unsigned aal : 1;
        unsigned reserved_13 : 1;
        unsigned mb : 1;
        unsigned rb : 1;
        unsigned reserved_31_16 : 16;
    };
    unsigned reg;
} GBE__DMA_SysBus_Mode__ACC_T;

#define GBE__GBE0__DMA_SYSBUS_MODE__ADDR (GBE__GBE0__BASE_ADDR + 0x1004)
#define GBE__GBE0__DMA_SYSBUS_MODE__NUM  0x1

#define GBE__GBE0__DMA_SYSBUS_MODE__FB__SHIFT    0
#define GBE__GBE0__DMA_SYSBUS_MODE__RESERVED_11_1__SHIFT    1
#define GBE__GBE0__DMA_SYSBUS_MODE__AAL__SHIFT    12
#define GBE__GBE0__DMA_SYSBUS_MODE__RESERVED_13__SHIFT    13
#define GBE__GBE0__DMA_SYSBUS_MODE__MB__SHIFT    14
#define GBE__GBE0__DMA_SYSBUS_MODE__RB__SHIFT    15
#define GBE__GBE0__DMA_SYSBUS_MODE__RESERVED_31_16__SHIFT    16

#define GBE__GBE0__DMA_SYSBUS_MODE__FB__MASK    0x00000001
#define GBE__GBE0__DMA_SYSBUS_MODE__RESERVED_11_1__MASK    0x00000ffe
#define GBE__GBE0__DMA_SYSBUS_MODE__AAL__MASK    0x00001000
#define GBE__GBE0__DMA_SYSBUS_MODE__RESERVED_13__MASK    0x00002000
#define GBE__GBE0__DMA_SYSBUS_MODE__MB__MASK    0x00004000
#define GBE__GBE0__DMA_SYSBUS_MODE__RB__MASK    0x00008000
#define GBE__GBE0__DMA_SYSBUS_MODE__RESERVED_31_16__MASK    0xffff0000

#define GBE__GBE0__DMA_SYSBUS_MODE__FB__POR_VALUE    0x0
#define GBE__GBE0__DMA_SYSBUS_MODE__RESERVED_11_1__POR_VALUE    0x0
#define GBE__GBE0__DMA_SYSBUS_MODE__AAL__POR_VALUE    0x0
#define GBE__GBE0__DMA_SYSBUS_MODE__RESERVED_13__POR_VALUE    0x0
#define GBE__GBE0__DMA_SYSBUS_MODE__MB__POR_VALUE    0x0
#define GBE__GBE0__DMA_SYSBUS_MODE__RB__POR_VALUE    0x0
#define GBE__GBE0__DMA_SYSBUS_MODE__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_Interrupt_Status
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned dc0is : 1;
        unsigned reserved_15_1 : 15;
        unsigned mtlis : 1;
        unsigned macis : 1;
        unsigned reserved_31_18 : 14;
    };
    unsigned reg;
} GBE__DMA_Interrupt_Status__ACC_T;

#define GBE__GBE0__DMA_INTERRUPT_STATUS__ADDR (GBE__GBE0__BASE_ADDR + 0x1008)
#define GBE__GBE0__DMA_INTERRUPT_STATUS__NUM  0x1

#define GBE__GBE0__DMA_INTERRUPT_STATUS__DC0IS__SHIFT    0
#define GBE__GBE0__DMA_INTERRUPT_STATUS__RESERVED_15_1__SHIFT    1
#define GBE__GBE0__DMA_INTERRUPT_STATUS__MTLIS__SHIFT    16
#define GBE__GBE0__DMA_INTERRUPT_STATUS__MACIS__SHIFT    17
#define GBE__GBE0__DMA_INTERRUPT_STATUS__RESERVED_31_18__SHIFT    18

#define GBE__GBE0__DMA_INTERRUPT_STATUS__DC0IS__MASK    0x00000001
#define GBE__GBE0__DMA_INTERRUPT_STATUS__RESERVED_15_1__MASK    0x0000fffe
#define GBE__GBE0__DMA_INTERRUPT_STATUS__MTLIS__MASK    0x00010000
#define GBE__GBE0__DMA_INTERRUPT_STATUS__MACIS__MASK    0x00020000
#define GBE__GBE0__DMA_INTERRUPT_STATUS__RESERVED_31_18__MASK    0xfffc0000

#define GBE__GBE0__DMA_INTERRUPT_STATUS__DC0IS__POR_VALUE    0x0
#define GBE__GBE0__DMA_INTERRUPT_STATUS__RESERVED_15_1__POR_VALUE    0x0
#define GBE__GBE0__DMA_INTERRUPT_STATUS__MTLIS__POR_VALUE    0x0
#define GBE__GBE0__DMA_INTERRUPT_STATUS__MACIS__POR_VALUE    0x0
#define GBE__GBE0__DMA_INTERRUPT_STATUS__RESERVED_31_18__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_Debug_Status0
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned axwhsts : 1;
        unsigned reserved_7_1 : 7;
        unsigned rps0 : 4;
        unsigned tps0 : 4;
        unsigned reserved_31_16 : 16;
    };
    unsigned reg;
} GBE__DMA_Debug_Status0__ACC_T;

#define GBE__GBE0__DMA_DEBUG_STATUS0__ADDR (GBE__GBE0__BASE_ADDR + 0x100c)
#define GBE__GBE0__DMA_DEBUG_STATUS0__NUM  0x1

#define GBE__GBE0__DMA_DEBUG_STATUS0__AXWHSTS__SHIFT    0
#define GBE__GBE0__DMA_DEBUG_STATUS0__RESERVED_7_1__SHIFT    1
#define GBE__GBE0__DMA_DEBUG_STATUS0__RPS0__SHIFT    8
#define GBE__GBE0__DMA_DEBUG_STATUS0__TPS0__SHIFT    12
#define GBE__GBE0__DMA_DEBUG_STATUS0__RESERVED_31_16__SHIFT    16

#define GBE__GBE0__DMA_DEBUG_STATUS0__AXWHSTS__MASK    0x00000001
#define GBE__GBE0__DMA_DEBUG_STATUS0__RESERVED_7_1__MASK    0x000000fe
#define GBE__GBE0__DMA_DEBUG_STATUS0__RPS0__MASK    0x00000f00
#define GBE__GBE0__DMA_DEBUG_STATUS0__TPS0__MASK    0x0000f000
#define GBE__GBE0__DMA_DEBUG_STATUS0__RESERVED_31_16__MASK    0xffff0000

#define GBE__GBE0__DMA_DEBUG_STATUS0__AXWHSTS__POR_VALUE    0x0
#define GBE__GBE0__DMA_DEBUG_STATUS0__RESERVED_7_1__POR_VALUE    0x0
#define GBE__GBE0__DMA_DEBUG_STATUS0__RPS0__POR_VALUE    0x0
#define GBE__GBE0__DMA_DEBUG_STATUS0__TPS0__POR_VALUE    0x0
#define GBE__GBE0__DMA_DEBUG_STATUS0__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_Control
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned mss : 14;
        unsigned reserved_15_14 : 2;
        unsigned pblx8 : 1;
        unsigned reserved_17 : 1;
        unsigned dsl : 3;
        unsigned reserved_31_21 : 11;
    };
    unsigned reg;
} GBE__DMA_CH0_Control__ACC_T;

#define GBE__GBE0__DMA_CH0_CONTROL__ADDR (GBE__GBE0__BASE_ADDR + 0x1100)
#define GBE__GBE0__DMA_CH0_CONTROL__NUM  0x1

#define GBE__GBE0__DMA_CH0_CONTROL__MSS__SHIFT    0
#define GBE__GBE0__DMA_CH0_CONTROL__RESERVED_15_14__SHIFT    14
#define GBE__GBE0__DMA_CH0_CONTROL__PBLX8__SHIFT    16
#define GBE__GBE0__DMA_CH0_CONTROL__RESERVED_17__SHIFT    17
#define GBE__GBE0__DMA_CH0_CONTROL__DSL__SHIFT    18
#define GBE__GBE0__DMA_CH0_CONTROL__RESERVED_31_21__SHIFT    21

#define GBE__GBE0__DMA_CH0_CONTROL__MSS__MASK    0x00003fff
#define GBE__GBE0__DMA_CH0_CONTROL__RESERVED_15_14__MASK    0x0000c000
#define GBE__GBE0__DMA_CH0_CONTROL__PBLX8__MASK    0x00010000
#define GBE__GBE0__DMA_CH0_CONTROL__RESERVED_17__MASK    0x00020000
#define GBE__GBE0__DMA_CH0_CONTROL__DSL__MASK    0x001c0000
#define GBE__GBE0__DMA_CH0_CONTROL__RESERVED_31_21__MASK    0xffe00000

#define GBE__GBE0__DMA_CH0_CONTROL__MSS__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_CONTROL__RESERVED_15_14__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_CONTROL__PBLX8__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_CONTROL__RESERVED_17__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_CONTROL__DSL__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_CONTROL__RESERVED_31_21__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_Tx_Control
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned st : 1;
        unsigned reserved_3_1 : 3;
        unsigned osf : 1;
        unsigned reserved_11_5 : 7;
        unsigned tse : 1;
        unsigned reserved_15_13 : 3;
        unsigned txpbl : 6;
        unsigned etic : 1;
        unsigned reserved_31_23 : 9;
    };
    unsigned reg;
} GBE__DMA_CH0_Tx_Control__ACC_T;

#define GBE__GBE0__DMA_CH0_TX_CONTROL__ADDR (GBE__GBE0__BASE_ADDR + 0x1104)
#define GBE__GBE0__DMA_CH0_TX_CONTROL__NUM  0x1

#define GBE__GBE0__DMA_CH0_TX_CONTROL__ST__SHIFT    0
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_3_1__SHIFT    1
#define GBE__GBE0__DMA_CH0_TX_CONTROL__OSF__SHIFT    4
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_11_5__SHIFT    5
#define GBE__GBE0__DMA_CH0_TX_CONTROL__TSE__SHIFT    12
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_15_13__SHIFT    13
#define GBE__GBE0__DMA_CH0_TX_CONTROL__TXPBL__SHIFT    16
#define GBE__GBE0__DMA_CH0_TX_CONTROL__ETIC__SHIFT    22
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_31_23__SHIFT    23

#define GBE__GBE0__DMA_CH0_TX_CONTROL__ST__MASK    0x00000001
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_3_1__MASK    0x0000000e
#define GBE__GBE0__DMA_CH0_TX_CONTROL__OSF__MASK    0x00000010
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_11_5__MASK    0x00000fe0
#define GBE__GBE0__DMA_CH0_TX_CONTROL__TSE__MASK    0x00001000
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_15_13__MASK    0x0000e000
#define GBE__GBE0__DMA_CH0_TX_CONTROL__TXPBL__MASK    0x003f0000
#define GBE__GBE0__DMA_CH0_TX_CONTROL__ETIC__MASK    0x00400000
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_31_23__MASK    0xff800000

#define GBE__GBE0__DMA_CH0_TX_CONTROL__ST__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_3_1__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_TX_CONTROL__OSF__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_11_5__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_TX_CONTROL__TSE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_15_13__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_TX_CONTROL__TXPBL__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_TX_CONTROL__ETIC__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_TX_CONTROL__RESERVED_31_23__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_Rx_Control
//
// Rx Packet Flush.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned sr : 1;
        unsigned rbsz_x_0 : 2;
        unsigned rbsz_13_y : 12;
        unsigned reserved_15 : 1;
        unsigned rxpbl : 6;
        unsigned eric : 1;
        unsigned reserved_30_23 : 8;
        unsigned rpf : 1;
    };
    unsigned reg;
} GBE__DMA_CH0_Rx_Control__ACC_T;

#define GBE__GBE0__DMA_CH0_RX_CONTROL__ADDR (GBE__GBE0__BASE_ADDR + 0x1108)
#define GBE__GBE0__DMA_CH0_RX_CONTROL__NUM  0x1

#define GBE__GBE0__DMA_CH0_RX_CONTROL__SR__SHIFT    0
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RBSZ_X_0__SHIFT    1
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RBSZ_13_Y__SHIFT    3
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RESERVED_15__SHIFT    15
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RXPBL__SHIFT    16
#define GBE__GBE0__DMA_CH0_RX_CONTROL__ERIC__SHIFT    22
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RESERVED_30_23__SHIFT    23
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RPF__SHIFT    31

#define GBE__GBE0__DMA_CH0_RX_CONTROL__SR__MASK    0x00000001
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RBSZ_X_0__MASK    0x00000006
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RBSZ_13_Y__MASK    0x00007ff8
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RESERVED_15__MASK    0x00008000
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RXPBL__MASK    0x003f0000
#define GBE__GBE0__DMA_CH0_RX_CONTROL__ERIC__MASK    0x00400000
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RESERVED_30_23__MASK    0x7f800000
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RPF__MASK    0x80000000

#define GBE__GBE0__DMA_CH0_RX_CONTROL__SR__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RBSZ_X_0__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RBSZ_13_Y__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RESERVED_15__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RXPBL__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RX_CONTROL__ERIC__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RESERVED_30_23__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RX_CONTROL__RPF__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_TxDesc_List_Address
// Start of Transmit List
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned reserved_lsb : 2;
        unsigned tdesla : 30;
    };
    unsigned reg;
} GBE__DMA_CH0_TxDesc_List_Address__ACC_T;

#define GBE__GBE0__DMA_CH0_TXDESC_LIST_ADDRESS__ADDR (GBE__GBE0__BASE_ADDR + 0x1114)
#define GBE__GBE0__DMA_CH0_TXDESC_LIST_ADDRESS__NUM  0x1

#define GBE__GBE0__DMA_CH0_TXDESC_LIST_ADDRESS__RESERVED_LSB__SHIFT    0
#define GBE__GBE0__DMA_CH0_TXDESC_LIST_ADDRESS__TDESLA__SHIFT    2

#define GBE__GBE0__DMA_CH0_TXDESC_LIST_ADDRESS__RESERVED_LSB__MASK    0x00000003
#define GBE__GBE0__DMA_CH0_TXDESC_LIST_ADDRESS__TDESLA__MASK    0xfffffffc

#define GBE__GBE0__DMA_CH0_TXDESC_LIST_ADDRESS__RESERVED_LSB__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_TXDESC_LIST_ADDRESS__TDESLA__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_RxDesc_List_Address
// Start of Receive List
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned reserved_lsb : 2;
        unsigned rdesla : 30;
    };
    unsigned reg;
} GBE__DMA_CH0_RxDesc_List_Address__ACC_T;

#define GBE__GBE0__DMA_CH0_RXDESC_LIST_ADDRESS__ADDR (GBE__GBE0__BASE_ADDR + 0x111C)
#define GBE__GBE0__DMA_CH0_RXDESC_LIST_ADDRESS__NUM  0x1

#define GBE__GBE0__DMA_CH0_RXDESC_LIST_ADDRESS__RESERVED_LSB__SHIFT    0
#define GBE__GBE0__DMA_CH0_RXDESC_LIST_ADDRESS__RDESLA__SHIFT    2

#define GBE__GBE0__DMA_CH0_RXDESC_LIST_ADDRESS__RESERVED_LSB__MASK    0x00000003
#define GBE__GBE0__DMA_CH0_RXDESC_LIST_ADDRESS__RDESLA__MASK    0xfffffffc

#define GBE__GBE0__DMA_CH0_RXDESC_LIST_ADDRESS__RESERVED_LSB__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RXDESC_LIST_ADDRESS__RDESLA__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_TxDesc_Tail_Pointer
// Transmit Descriptor Tail Pointer
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned reserved_lsb : 2;
        unsigned tdtp : 30;
    };
    unsigned reg;
} GBE__DMA_CH0_TxDesc_Tail_Pointer__ACC_T;

#define GBE__GBE0__DMA_CH0_TXDESC_TAIL_POINTER__ADDR (GBE__GBE0__BASE_ADDR + 0x1120)
#define GBE__GBE0__DMA_CH0_TXDESC_TAIL_POINTER__NUM  0x1

#define GBE__GBE0__DMA_CH0_TXDESC_TAIL_POINTER__RESERVED_LSB__SHIFT    0
#define GBE__GBE0__DMA_CH0_TXDESC_TAIL_POINTER__TDTP__SHIFT    2

#define GBE__GBE0__DMA_CH0_TXDESC_TAIL_POINTER__RESERVED_LSB__MASK    0x00000003
#define GBE__GBE0__DMA_CH0_TXDESC_TAIL_POINTER__TDTP__MASK    0xfffffffc

#define GBE__GBE0__DMA_CH0_TXDESC_TAIL_POINTER__RESERVED_LSB__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_TXDESC_TAIL_POINTER__TDTP__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_RxDesc_Tail_Pointer
// Receive Descriptor Tail Pointer
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned reserved_lsb : 2;
        unsigned rdtp : 30;
    };
    unsigned reg;
} GBE__DMA_CH0_RxDesc_Tail_Pointer__ACC_T;

#define GBE__GBE0__DMA_CH0_RXDESC_TAIL_POINTER__ADDR (GBE__GBE0__BASE_ADDR + 0x1128)
#define GBE__GBE0__DMA_CH0_RXDESC_TAIL_POINTER__NUM  0x1

#define GBE__GBE0__DMA_CH0_RXDESC_TAIL_POINTER__RESERVED_LSB__SHIFT    0
#define GBE__GBE0__DMA_CH0_RXDESC_TAIL_POINTER__RDTP__SHIFT    2

#define GBE__GBE0__DMA_CH0_RXDESC_TAIL_POINTER__RESERVED_LSB__MASK    0x00000003
#define GBE__GBE0__DMA_CH0_RXDESC_TAIL_POINTER__RDTP__MASK    0xfffffffc

#define GBE__GBE0__DMA_CH0_RXDESC_TAIL_POINTER__RESERVED_LSB__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RXDESC_TAIL_POINTER__RDTP__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_TxDesc_Ring_Length
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tdrl : 10;
        unsigned reserved_31_10 : 22;
    };
    unsigned reg;
} GBE__DMA_CH0_TxDesc_Ring_Length__ACC_T;

#define GBE__GBE0__DMA_CH0_TXDESC_RING_LENGTH__ADDR (GBE__GBE0__BASE_ADDR + 0x112C)
#define GBE__GBE0__DMA_CH0_TXDESC_RING_LENGTH__NUM  0x1

#define GBE__GBE0__DMA_CH0_TXDESC_RING_LENGTH__TDRL__SHIFT    0
#define GBE__GBE0__DMA_CH0_TXDESC_RING_LENGTH__RESERVED_31_10__SHIFT    10

#define GBE__GBE0__DMA_CH0_TXDESC_RING_LENGTH__TDRL__MASK    0x000003ff
#define GBE__GBE0__DMA_CH0_TXDESC_RING_LENGTH__RESERVED_31_10__MASK    0xfffffc00

#define GBE__GBE0__DMA_CH0_TXDESC_RING_LENGTH__TDRL__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_TXDESC_RING_LENGTH__RESERVED_31_10__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_RxDesc_Ring_Length
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rdrl : 10;
        unsigned reserved_31_10 : 22;
    };
    unsigned reg;
} GBE__DMA_CH0_RxDesc_Ring_Length__ACC_T;

#define GBE__GBE0__DMA_CH0_RXDESC_RING_LENGTH__ADDR (GBE__GBE0__BASE_ADDR + 0x1130)
#define GBE__GBE0__DMA_CH0_RXDESC_RING_LENGTH__NUM  0x1

#define GBE__GBE0__DMA_CH0_RXDESC_RING_LENGTH__RDRL__SHIFT    0
#define GBE__GBE0__DMA_CH0_RXDESC_RING_LENGTH__RESERVED_31_10__SHIFT    10

#define GBE__GBE0__DMA_CH0_RXDESC_RING_LENGTH__RDRL__MASK    0x000003ff
#define GBE__GBE0__DMA_CH0_RXDESC_RING_LENGTH__RESERVED_31_10__MASK    0xfffffc00

#define GBE__GBE0__DMA_CH0_RXDESC_RING_LENGTH__RDRL__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RXDESC_RING_LENGTH__RESERVED_31_10__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_Interrupt_Enable
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tie : 1;
        unsigned txse : 1;
        unsigned tbue : 1;
        unsigned reserved_5_3 : 3;
        unsigned rie : 1;
        unsigned rbue : 1;
        unsigned rse : 1;
        unsigned rwte : 1;
        unsigned etie : 1;
        unsigned erie : 1;
        unsigned fbee : 1;
        unsigned cdee : 1;
        unsigned aie : 1;
        unsigned nie : 1;
        unsigned reserved_31_16 : 16;
    };
    unsigned reg;
} GBE__DMA_CH0_Interrupt_Enable__ACC_T;

#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__ADDR (GBE__GBE0__BASE_ADDR + 0x1134)
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__NUM  0x1

#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__TIE__SHIFT    0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__TXSE__SHIFT    1
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__TBUE__SHIFT    2
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RESERVED_5_3__SHIFT    3
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RIE__SHIFT    6
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RBUE__SHIFT    7
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RSE__SHIFT    8
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RWTE__SHIFT    9
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__ETIE__SHIFT    10
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__ERIE__SHIFT    11
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__FBEE__SHIFT    12
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__CDEE__SHIFT    13
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__AIE__SHIFT    14
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__NIE__SHIFT    15
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RESERVED_31_16__SHIFT    16

#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__TIE__MASK    0x00000001
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__TXSE__MASK    0x00000002
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__TBUE__MASK    0x00000004
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RESERVED_5_3__MASK    0x00000038
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RIE__MASK    0x00000040
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RBUE__MASK    0x00000080
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RSE__MASK    0x00000100
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RWTE__MASK    0x00000200
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__ETIE__MASK    0x00000400
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__ERIE__MASK    0x00000800
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__FBEE__MASK    0x00001000
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__CDEE__MASK    0x00002000
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__AIE__MASK    0x00004000
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__NIE__MASK    0x00008000
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RESERVED_31_16__MASK    0xffff0000

#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__TIE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__TXSE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__TBUE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RESERVED_5_3__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RIE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RBUE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RSE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RWTE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__ETIE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__ERIE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__FBEE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__CDEE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__AIE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__NIE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_INTERRUPT_ENABLE__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_Rx_Interrupt_Watchdog_Timer
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rwt : 8;
        unsigned reserved_15_8 : 8;
        unsigned rwtu : 2;
        unsigned reserved_31_18 : 14;
    };
    unsigned reg;
} GBE__DMA_CH0_Rx_Interrupt_Watchdog_Timer__ACC_T;

#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__ADDR (GBE__GBE0__BASE_ADDR + 0x1138)
#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__NUM  0x1

#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RWT__SHIFT    0
#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RESERVED_15_8__SHIFT    8
#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RWTU__SHIFT    16
#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RESERVED_31_18__SHIFT    18

#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RWT__MASK    0x000000ff
#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RESERVED_15_8__MASK    0x0000ff00
#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RWTU__MASK    0x00030000
#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RESERVED_31_18__MASK    0xfffc0000

#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RWT__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RESERVED_15_8__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RWTU__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RESERVED_31_18__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_Current_App_TxDesc
// Application Transmit Descriptor Address Pointer
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned curtdesaptr : 32;
    };
    unsigned reg;
} GBE__DMA_CH0_Current_App_TxDesc__ACC_T;

#define GBE__GBE0__DMA_CH0_CURRENT_APP_TXDESC__ADDR (GBE__GBE0__BASE_ADDR + 0x1144)
#define GBE__GBE0__DMA_CH0_CURRENT_APP_TXDESC__NUM  0x1

#define GBE__GBE0__DMA_CH0_CURRENT_APP_TXDESC__CURTDESAPTR__SHIFT    0

#define GBE__GBE0__DMA_CH0_CURRENT_APP_TXDESC__CURTDESAPTR__MASK    0xffffffff

#define GBE__GBE0__DMA_CH0_CURRENT_APP_TXDESC__CURTDESAPTR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_Current_App_RxDesc
// Application Receive Descriptor Address Pointer
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned currdesaptr : 32;
    };
    unsigned reg;
} GBE__DMA_CH0_Current_App_RxDesc__ACC_T;

#define GBE__GBE0__DMA_CH0_CURRENT_APP_RXDESC__ADDR (GBE__GBE0__BASE_ADDR + 0x114C)
#define GBE__GBE0__DMA_CH0_CURRENT_APP_RXDESC__NUM  0x1

#define GBE__GBE0__DMA_CH0_CURRENT_APP_RXDESC__CURRDESAPTR__SHIFT    0

#define GBE__GBE0__DMA_CH0_CURRENT_APP_RXDESC__CURRDESAPTR__MASK    0xffffffff

#define GBE__GBE0__DMA_CH0_CURRENT_APP_RXDESC__CURRDESAPTR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_Current_App_TxBuffer
// Application Transmit Buffer Address Pointer
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned curtbufaptr : 32;
    };
    unsigned reg;
} GBE__DMA_CH0_Current_App_TxBuffer__ACC_T;

#define GBE__GBE0__DMA_CH0_CURRENT_APP_TXBUFFER__ADDR (GBE__GBE0__BASE_ADDR + 0x1154)
#define GBE__GBE0__DMA_CH0_CURRENT_APP_TXBUFFER__NUM  0x1

#define GBE__GBE0__DMA_CH0_CURRENT_APP_TXBUFFER__CURTBUFAPTR__SHIFT    0

#define GBE__GBE0__DMA_CH0_CURRENT_APP_TXBUFFER__CURTBUFAPTR__MASK    0xffffffff

#define GBE__GBE0__DMA_CH0_CURRENT_APP_TXBUFFER__CURTBUFAPTR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_Current_App_RxBuffer
// Application Receive Buffer Address Pointer
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned currbufaptr : 32;
    };
    unsigned reg;
} GBE__DMA_CH0_Current_App_RxBuffer__ACC_T;

#define GBE__GBE0__DMA_CH0_CURRENT_APP_RXBUFFER__ADDR (GBE__GBE0__BASE_ADDR + 0x115C)
#define GBE__GBE0__DMA_CH0_CURRENT_APP_RXBUFFER__NUM  0x1

#define GBE__GBE0__DMA_CH0_CURRENT_APP_RXBUFFER__CURRBUFAPTR__SHIFT    0

#define GBE__GBE0__DMA_CH0_CURRENT_APP_RXBUFFER__CURRBUFAPTR__MASK    0xffffffff

#define GBE__GBE0__DMA_CH0_CURRENT_APP_RXBUFFER__CURRBUFAPTR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_Status
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned ti : 1;
        unsigned tps : 1;
        unsigned tbu : 1;
        unsigned reserved_5_3 : 3;
        unsigned ri : 1;
        unsigned rbu : 1;
        unsigned rps : 1;
        unsigned rwt : 1;
        unsigned eti : 1;
        unsigned eri : 1;
        unsigned fbe : 1;
        unsigned cde : 1;
        unsigned ais : 1;
        unsigned nis : 1;
        unsigned teb : 3;
        unsigned reb : 3;
        unsigned reserved_31_22 : 10;
    };
    unsigned reg;
} GBE__DMA_CH0_Status__ACC_T;

#define GBE__GBE0__DMA_CH0_STATUS__ADDR (GBE__GBE0__BASE_ADDR + 0x1160)
#define GBE__GBE0__DMA_CH0_STATUS__NUM  0x1

#define GBE__GBE0__DMA_CH0_STATUS__TI__SHIFT    0
#define GBE__GBE0__DMA_CH0_STATUS__TPS__SHIFT    1
#define GBE__GBE0__DMA_CH0_STATUS__TBU__SHIFT    2
#define GBE__GBE0__DMA_CH0_STATUS__RESERVED_5_3__SHIFT    3
#define GBE__GBE0__DMA_CH0_STATUS__RI__SHIFT    6
#define GBE__GBE0__DMA_CH0_STATUS__RBU__SHIFT    7
#define GBE__GBE0__DMA_CH0_STATUS__RPS__SHIFT    8
#define GBE__GBE0__DMA_CH0_STATUS__RWT__SHIFT    9
#define GBE__GBE0__DMA_CH0_STATUS__ETI__SHIFT    10
#define GBE__GBE0__DMA_CH0_STATUS__ERI__SHIFT    11
#define GBE__GBE0__DMA_CH0_STATUS__FBE__SHIFT    12
#define GBE__GBE0__DMA_CH0_STATUS__CDE__SHIFT    13
#define GBE__GBE0__DMA_CH0_STATUS__AIS__SHIFT    14
#define GBE__GBE0__DMA_CH0_STATUS__NIS__SHIFT    15
#define GBE__GBE0__DMA_CH0_STATUS__TEB__SHIFT    16
#define GBE__GBE0__DMA_CH0_STATUS__REB__SHIFT    19
#define GBE__GBE0__DMA_CH0_STATUS__RESERVED_31_22__SHIFT    22

#define GBE__GBE0__DMA_CH0_STATUS__TI__MASK    0x00000001
#define GBE__GBE0__DMA_CH0_STATUS__TPS__MASK    0x00000002
#define GBE__GBE0__DMA_CH0_STATUS__TBU__MASK    0x00000004
#define GBE__GBE0__DMA_CH0_STATUS__RESERVED_5_3__MASK    0x00000038
#define GBE__GBE0__DMA_CH0_STATUS__RI__MASK    0x00000040
#define GBE__GBE0__DMA_CH0_STATUS__RBU__MASK    0x00000080
#define GBE__GBE0__DMA_CH0_STATUS__RPS__MASK    0x00000100
#define GBE__GBE0__DMA_CH0_STATUS__RWT__MASK    0x00000200
#define GBE__GBE0__DMA_CH0_STATUS__ETI__MASK    0x00000400
#define GBE__GBE0__DMA_CH0_STATUS__ERI__MASK    0x00000800
#define GBE__GBE0__DMA_CH0_STATUS__FBE__MASK    0x00001000
#define GBE__GBE0__DMA_CH0_STATUS__CDE__MASK    0x00002000
#define GBE__GBE0__DMA_CH0_STATUS__AIS__MASK    0x00004000
#define GBE__GBE0__DMA_CH0_STATUS__NIS__MASK    0x00008000
#define GBE__GBE0__DMA_CH0_STATUS__TEB__MASK    0x00070000
#define GBE__GBE0__DMA_CH0_STATUS__REB__MASK    0x00380000
#define GBE__GBE0__DMA_CH0_STATUS__RESERVED_31_22__MASK    0xffc00000

#define GBE__GBE0__DMA_CH0_STATUS__TI__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__TPS__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__TBU__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__RESERVED_5_3__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__RI__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__RBU__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__RPS__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__RWT__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__ETI__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__ERI__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__FBE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__CDE__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__AIS__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__NIS__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__TEB__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__REB__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_STATUS__RESERVED_31_22__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_Miss_Frame_Cnt
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned mfc : 11;
        unsigned reserved_14_11 : 4;
        unsigned mfco : 1;
        unsigned reserved_31_16 : 16;
    };
    unsigned reg;
} GBE__DMA_CH0_Miss_Frame_Cnt__ACC_T;

#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__ADDR (GBE__GBE0__BASE_ADDR + 0x1164)
#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__NUM  0x1

#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__MFC__SHIFT    0
#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__RESERVED_14_11__SHIFT    11
#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__MFCO__SHIFT    15
#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__RESERVED_31_16__SHIFT    16

#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__MFC__MASK    0x000007ff
#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__RESERVED_14_11__MASK    0x00007800
#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__MFCO__MASK    0x00008000
#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__RESERVED_31_16__MASK    0xffff0000

#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__MFC__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__RESERVED_14_11__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__MFCO__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_MISS_FRAME_CNT__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMA_CH0_RX_ERI_Cnt
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned ecnt : 12;
        unsigned reserved_31_12 : 20;
    };
    unsigned reg;
} GBE__DMA_CH0_RX_ERI_Cnt__ACC_T;

#define GBE__GBE0__DMA_CH0_RX_ERI_CNT__ADDR (GBE__GBE0__BASE_ADDR + 0x1168)
#define GBE__GBE0__DMA_CH0_RX_ERI_CNT__NUM  0x1

#define GBE__GBE0__DMA_CH0_RX_ERI_CNT__ECNT__SHIFT    0
#define GBE__GBE0__DMA_CH0_RX_ERI_CNT__RESERVED_31_12__SHIFT    12

#define GBE__GBE0__DMA_CH0_RX_ERI_CNT__ECNT__MASK    0x00000fff
#define GBE__GBE0__DMA_CH0_RX_ERI_CNT__RESERVED_31_12__MASK    0xfffff000

#define GBE__GBE0__DMA_CH0_RX_ERI_CNT__ECNT__POR_VALUE    0x0
#define GBE__GBE0__DMA_CH0_RX_ERI_CNT__RESERVED_31_12__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Configuration
// ARP Offload Enable
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned re : 1;
        unsigned te : 1;
        unsigned prelen : 2;
        unsigned reserved_10_4 : 7;
        unsigned ecrsfd : 1;
        unsigned lm : 1;
        unsigned dm : 1;
        unsigned fes : 1;
        unsigned ps : 1;
        unsigned je : 1;
        unsigned jd : 1;
        unsigned reserved_18 : 1;
        unsigned wd : 1;
        unsigned acs : 1;
        unsigned cst : 1;
        unsigned s2kp : 1;
        unsigned gpslce : 1;
        unsigned ipg : 3;
        unsigned ipc : 1;
        unsigned reserved_30_28 : 3;
        unsigned arpen : 1;
    };
    unsigned reg;
} GBE__MAC_Configuration__ACC_T;

#define GBE__GBE0__MAC_CONFIGURATION__ADDR (GBE__GBE0__BASE_ADDR + 0x0)
#define GBE__GBE0__MAC_CONFIGURATION__NUM  0x1

#define GBE__GBE0__MAC_CONFIGURATION__RE__SHIFT    0
#define GBE__GBE0__MAC_CONFIGURATION__TE__SHIFT    1
#define GBE__GBE0__MAC_CONFIGURATION__PRELEN__SHIFT    2
#define GBE__GBE0__MAC_CONFIGURATION__RESERVED_10_4__SHIFT    4
#define GBE__GBE0__MAC_CONFIGURATION__ECRSFD__SHIFT    11
#define GBE__GBE0__MAC_CONFIGURATION__LM__SHIFT    12
#define GBE__GBE0__MAC_CONFIGURATION__DM__SHIFT    13
#define GBE__GBE0__MAC_CONFIGURATION__FES__SHIFT    14
#define GBE__GBE0__MAC_CONFIGURATION__PS__SHIFT    15
#define GBE__GBE0__MAC_CONFIGURATION__JE__SHIFT    16
#define GBE__GBE0__MAC_CONFIGURATION__JD__SHIFT    17
#define GBE__GBE0__MAC_CONFIGURATION__RESERVED_18__SHIFT    18
#define GBE__GBE0__MAC_CONFIGURATION__WD__SHIFT    19
#define GBE__GBE0__MAC_CONFIGURATION__ACS__SHIFT    20
#define GBE__GBE0__MAC_CONFIGURATION__CST__SHIFT    21
#define GBE__GBE0__MAC_CONFIGURATION__S2KP__SHIFT    22
#define GBE__GBE0__MAC_CONFIGURATION__GPSLCE__SHIFT    23
#define GBE__GBE0__MAC_CONFIGURATION__IPG__SHIFT    24
#define GBE__GBE0__MAC_CONFIGURATION__IPC__SHIFT    27
#define GBE__GBE0__MAC_CONFIGURATION__RESERVED_30_28__SHIFT    28
#define GBE__GBE0__MAC_CONFIGURATION__ARPEN__SHIFT    31

#define GBE__GBE0__MAC_CONFIGURATION__RE__MASK    0x00000001
#define GBE__GBE0__MAC_CONFIGURATION__TE__MASK    0x00000002
#define GBE__GBE0__MAC_CONFIGURATION__PRELEN__MASK    0x0000000c
#define GBE__GBE0__MAC_CONFIGURATION__RESERVED_10_4__MASK    0x000007f0
#define GBE__GBE0__MAC_CONFIGURATION__ECRSFD__MASK    0x00000800
#define GBE__GBE0__MAC_CONFIGURATION__LM__MASK    0x00001000
#define GBE__GBE0__MAC_CONFIGURATION__DM__MASK    0x00002000
#define GBE__GBE0__MAC_CONFIGURATION__FES__MASK    0x00004000
#define GBE__GBE0__MAC_CONFIGURATION__PS__MASK    0x00008000
#define GBE__GBE0__MAC_CONFIGURATION__JE__MASK    0x00010000
#define GBE__GBE0__MAC_CONFIGURATION__JD__MASK    0x00020000
#define GBE__GBE0__MAC_CONFIGURATION__RESERVED_18__MASK    0x00040000
#define GBE__GBE0__MAC_CONFIGURATION__WD__MASK    0x00080000
#define GBE__GBE0__MAC_CONFIGURATION__ACS__MASK    0x00100000
#define GBE__GBE0__MAC_CONFIGURATION__CST__MASK    0x00200000
#define GBE__GBE0__MAC_CONFIGURATION__S2KP__MASK    0x00400000
#define GBE__GBE0__MAC_CONFIGURATION__GPSLCE__MASK    0x00800000
#define GBE__GBE0__MAC_CONFIGURATION__IPG__MASK    0x07000000
#define GBE__GBE0__MAC_CONFIGURATION__IPC__MASK    0x08000000
#define GBE__GBE0__MAC_CONFIGURATION__RESERVED_30_28__MASK    0x70000000
#define GBE__GBE0__MAC_CONFIGURATION__ARPEN__MASK    0x80000000

#define GBE__GBE0__MAC_CONFIGURATION__RE__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__TE__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__PRELEN__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__RESERVED_10_4__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__ECRSFD__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__LM__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__DM__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__FES__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__PS__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__JE__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__JD__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__RESERVED_18__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__WD__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__ACS__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__CST__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__S2KP__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__GPSLCE__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__IPG__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__IPC__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__RESERVED_30_28__POR_VALUE    0x0
#define GBE__GBE0__MAC_CONFIGURATION__ARPEN__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Ext_Configuration
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned gpsl : 14;
        unsigned reserved_15_14 : 2;
        unsigned dcrcc : 1;
        unsigned spen : 1;
        unsigned usp : 1;
        unsigned reserved_23_19 : 5;
        unsigned eipgen : 1;
        unsigned eipg : 5;
        unsigned reserved_31_30 : 2;
    };
    unsigned reg;
} GBE__MAC_Ext_Configuration__ACC_T;

#define GBE__GBE0__MAC_EXT_CONFIGURATION__ADDR (GBE__GBE0__BASE_ADDR + 0x4)
#define GBE__GBE0__MAC_EXT_CONFIGURATION__NUM  0x1

#define GBE__GBE0__MAC_EXT_CONFIGURATION__GPSL__SHIFT    0
#define GBE__GBE0__MAC_EXT_CONFIGURATION__RESERVED_15_14__SHIFT    14
#define GBE__GBE0__MAC_EXT_CONFIGURATION__DCRCC__SHIFT    16
#define GBE__GBE0__MAC_EXT_CONFIGURATION__SPEN__SHIFT    17
#define GBE__GBE0__MAC_EXT_CONFIGURATION__USP__SHIFT    18
#define GBE__GBE0__MAC_EXT_CONFIGURATION__RESERVED_23_19__SHIFT    19
#define GBE__GBE0__MAC_EXT_CONFIGURATION__EIPGEN__SHIFT    24
#define GBE__GBE0__MAC_EXT_CONFIGURATION__EIPG__SHIFT    25
#define GBE__GBE0__MAC_EXT_CONFIGURATION__RESERVED_31_30__SHIFT    30

#define GBE__GBE0__MAC_EXT_CONFIGURATION__GPSL__MASK    0x00003fff
#define GBE__GBE0__MAC_EXT_CONFIGURATION__RESERVED_15_14__MASK    0x0000c000
#define GBE__GBE0__MAC_EXT_CONFIGURATION__DCRCC__MASK    0x00010000
#define GBE__GBE0__MAC_EXT_CONFIGURATION__SPEN__MASK    0x00020000
#define GBE__GBE0__MAC_EXT_CONFIGURATION__USP__MASK    0x00040000
#define GBE__GBE0__MAC_EXT_CONFIGURATION__RESERVED_23_19__MASK    0x00f80000
#define GBE__GBE0__MAC_EXT_CONFIGURATION__EIPGEN__MASK    0x01000000
#define GBE__GBE0__MAC_EXT_CONFIGURATION__EIPG__MASK    0x3e000000
#define GBE__GBE0__MAC_EXT_CONFIGURATION__RESERVED_31_30__MASK    0xc0000000

#define GBE__GBE0__MAC_EXT_CONFIGURATION__GPSL__POR_VALUE    0x0
#define GBE__GBE0__MAC_EXT_CONFIGURATION__RESERVED_15_14__POR_VALUE    0x0
#define GBE__GBE0__MAC_EXT_CONFIGURATION__DCRCC__POR_VALUE    0x0
#define GBE__GBE0__MAC_EXT_CONFIGURATION__SPEN__POR_VALUE    0x0
#define GBE__GBE0__MAC_EXT_CONFIGURATION__USP__POR_VALUE    0x0
#define GBE__GBE0__MAC_EXT_CONFIGURATION__RESERVED_23_19__POR_VALUE    0x0
#define GBE__GBE0__MAC_EXT_CONFIGURATION__EIPGEN__POR_VALUE    0x0
#define GBE__GBE0__MAC_EXT_CONFIGURATION__EIPG__POR_VALUE    0x0
#define GBE__GBE0__MAC_EXT_CONFIGURATION__RESERVED_31_30__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Packet_Filter
// Receive All
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned pr : 1;
        unsigned reserved_2_1 : 2;
        unsigned daif : 1;
        unsigned pm : 1;
        unsigned dbf : 1;
        unsigned pcf : 2;
        unsigned reserved_15_8 : 8;
        unsigned vtfe : 1;
        unsigned reserved_30_17 : 14;
        unsigned ra : 1;
    };
    unsigned reg;
} GBE__MAC_Packet_Filter__ACC_T;

#define GBE__GBE0__MAC_PACKET_FILTER__ADDR (GBE__GBE0__BASE_ADDR + 0x8)
#define GBE__GBE0__MAC_PACKET_FILTER__NUM  0x1

#define GBE__GBE0__MAC_PACKET_FILTER__PR__SHIFT    0
#define GBE__GBE0__MAC_PACKET_FILTER__RESERVED_2_1__SHIFT    1
#define GBE__GBE0__MAC_PACKET_FILTER__DAIF__SHIFT    3
#define GBE__GBE0__MAC_PACKET_FILTER__PM__SHIFT    4
#define GBE__GBE0__MAC_PACKET_FILTER__DBF__SHIFT    5
#define GBE__GBE0__MAC_PACKET_FILTER__PCF__SHIFT    6
#define GBE__GBE0__MAC_PACKET_FILTER__RESERVED_15_8__SHIFT    8
#define GBE__GBE0__MAC_PACKET_FILTER__VTFE__SHIFT    16
#define GBE__GBE0__MAC_PACKET_FILTER__RESERVED_30_17__SHIFT    17
#define GBE__GBE0__MAC_PACKET_FILTER__RA__SHIFT    31

#define GBE__GBE0__MAC_PACKET_FILTER__PR__MASK    0x00000001
#define GBE__GBE0__MAC_PACKET_FILTER__RESERVED_2_1__MASK    0x00000006
#define GBE__GBE0__MAC_PACKET_FILTER__DAIF__MASK    0x00000008
#define GBE__GBE0__MAC_PACKET_FILTER__PM__MASK    0x00000010
#define GBE__GBE0__MAC_PACKET_FILTER__DBF__MASK    0x00000020
#define GBE__GBE0__MAC_PACKET_FILTER__PCF__MASK    0x000000c0
#define GBE__GBE0__MAC_PACKET_FILTER__RESERVED_15_8__MASK    0x0000ff00
#define GBE__GBE0__MAC_PACKET_FILTER__VTFE__MASK    0x00010000
#define GBE__GBE0__MAC_PACKET_FILTER__RESERVED_30_17__MASK    0x7ffe0000
#define GBE__GBE0__MAC_PACKET_FILTER__RA__MASK    0x80000000

#define GBE__GBE0__MAC_PACKET_FILTER__PR__POR_VALUE    0x0
#define GBE__GBE0__MAC_PACKET_FILTER__RESERVED_2_1__POR_VALUE    0x0
#define GBE__GBE0__MAC_PACKET_FILTER__DAIF__POR_VALUE    0x0
#define GBE__GBE0__MAC_PACKET_FILTER__PM__POR_VALUE    0x0
#define GBE__GBE0__MAC_PACKET_FILTER__DBF__POR_VALUE    0x0
#define GBE__GBE0__MAC_PACKET_FILTER__PCF__POR_VALUE    0x0
#define GBE__GBE0__MAC_PACKET_FILTER__RESERVED_15_8__POR_VALUE    0x0
#define GBE__GBE0__MAC_PACKET_FILTER__VTFE__POR_VALUE    0x0
#define GBE__GBE0__MAC_PACKET_FILTER__RESERVED_30_17__POR_VALUE    0x0
#define GBE__GBE0__MAC_PACKET_FILTER__RA__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Watchdog_Timeout
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned wto : 4;
        unsigned reserved_7_4 : 4;
        unsigned pwe : 1;
        unsigned reserved_31_9 : 23;
    };
    unsigned reg;
} GBE__MAC_Watchdog_Timeout__ACC_T;

#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__ADDR (GBE__GBE0__BASE_ADDR + 0xc)
#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__NUM  0x1

#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__WTO__SHIFT    0
#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__RESERVED_7_4__SHIFT    4
#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__PWE__SHIFT    8
#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__RESERVED_31_9__SHIFT    9

#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__WTO__MASK    0x0000000f
#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__RESERVED_7_4__MASK    0x000000f0
#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__PWE__MASK    0x00000100
#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__RESERVED_31_9__MASK    0xfffffe00

#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__WTO__POR_VALUE    0x0
#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__RESERVED_7_4__POR_VALUE    0x0
#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__PWE__POR_VALUE    0x0
#define GBE__GBE0__MAC_WATCHDOG_TIMEOUT__RESERVED_31_9__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_VLAN_Tag
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned vl : 16;
        unsigned etv : 1;
        unsigned vtim : 1;
        unsigned esvl : 1;
        unsigned ersvlm : 1;
        unsigned dovltc : 1;
        unsigned evls : 2;
        unsigned reserved_23 : 1;
        unsigned evlrxs : 1;
        unsigned reserved_31_25 : 7;
    };
    unsigned reg;
} GBE__MAC_VLAN_Tag__ACC_T;

#define GBE__GBE0__MAC_VLAN_TAG__ADDR (GBE__GBE0__BASE_ADDR + 0x50)
#define GBE__GBE0__MAC_VLAN_TAG__NUM  0x1

#define GBE__GBE0__MAC_VLAN_TAG__VL__SHIFT    0
#define GBE__GBE0__MAC_VLAN_TAG__ETV__SHIFT    16
#define GBE__GBE0__MAC_VLAN_TAG__VTIM__SHIFT    17
#define GBE__GBE0__MAC_VLAN_TAG__ESVL__SHIFT    18
#define GBE__GBE0__MAC_VLAN_TAG__ERSVLM__SHIFT    19
#define GBE__GBE0__MAC_VLAN_TAG__DOVLTC__SHIFT    20
#define GBE__GBE0__MAC_VLAN_TAG__EVLS__SHIFT    21
#define GBE__GBE0__MAC_VLAN_TAG__RESERVED_23__SHIFT    23
#define GBE__GBE0__MAC_VLAN_TAG__EVLRXS__SHIFT    24
#define GBE__GBE0__MAC_VLAN_TAG__RESERVED_31_25__SHIFT    25

#define GBE__GBE0__MAC_VLAN_TAG__VL__MASK    0x0000ffff
#define GBE__GBE0__MAC_VLAN_TAG__ETV__MASK    0x00010000
#define GBE__GBE0__MAC_VLAN_TAG__VTIM__MASK    0x00020000
#define GBE__GBE0__MAC_VLAN_TAG__ESVL__MASK    0x00040000
#define GBE__GBE0__MAC_VLAN_TAG__ERSVLM__MASK    0x00080000
#define GBE__GBE0__MAC_VLAN_TAG__DOVLTC__MASK    0x00100000
#define GBE__GBE0__MAC_VLAN_TAG__EVLS__MASK    0x00600000
#define GBE__GBE0__MAC_VLAN_TAG__RESERVED_23__MASK    0x00800000
#define GBE__GBE0__MAC_VLAN_TAG__EVLRXS__MASK    0x01000000
#define GBE__GBE0__MAC_VLAN_TAG__RESERVED_31_25__MASK    0xfe000000

#define GBE__GBE0__MAC_VLAN_TAG__VL__POR_VALUE    0x0
#define GBE__GBE0__MAC_VLAN_TAG__ETV__POR_VALUE    0x0
#define GBE__GBE0__MAC_VLAN_TAG__VTIM__POR_VALUE    0x0
#define GBE__GBE0__MAC_VLAN_TAG__ESVL__POR_VALUE    0x0
#define GBE__GBE0__MAC_VLAN_TAG__ERSVLM__POR_VALUE    0x0
#define GBE__GBE0__MAC_VLAN_TAG__DOVLTC__POR_VALUE    0x0
#define GBE__GBE0__MAC_VLAN_TAG__EVLS__POR_VALUE    0x0
#define GBE__GBE0__MAC_VLAN_TAG__RESERVED_23__POR_VALUE    0x0
#define GBE__GBE0__MAC_VLAN_TAG__EVLRXS__POR_VALUE    0x0
#define GBE__GBE0__MAC_VLAN_TAG__RESERVED_31_25__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Q0_Tx_Flow_Ctrl
// Pause Time
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned fcb_bpa : 1;
        unsigned tfe : 1;
        unsigned reserved_3_2 : 2;
        unsigned plt : 3;
        unsigned dzpq : 1;
        unsigned reserved_15_8 : 8;
        unsigned pt : 16;
    };
    unsigned reg;
} GBE__MAC_Q0_Tx_Flow_Ctrl__ACC_T;

#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__ADDR (GBE__GBE0__BASE_ADDR + 0x70)
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__NUM  0x1

#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__FCB_BPA__SHIFT    0
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__TFE__SHIFT    1
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__RESERVED_3_2__SHIFT    2
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__PLT__SHIFT    4
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__DZPQ__SHIFT    7
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__RESERVED_15_8__SHIFT    8
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__PT__SHIFT    16

#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__FCB_BPA__MASK    0x00000001
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__TFE__MASK    0x00000002
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__RESERVED_3_2__MASK    0x0000000c
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__PLT__MASK    0x00000070
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__DZPQ__MASK    0x00000080
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__RESERVED_15_8__MASK    0x0000ff00
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__PT__MASK    0xffff0000

#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__FCB_BPA__POR_VALUE    0x0
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__TFE__POR_VALUE    0x0
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__RESERVED_3_2__POR_VALUE    0x0
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__PLT__POR_VALUE    0x0
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__DZPQ__POR_VALUE    0x0
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__RESERVED_15_8__POR_VALUE    0x0
#define GBE__GBE0__MAC_Q0_TX_FLOW_CTRL__PT__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Rx_Flow_Ctrl
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rfe : 1;
        unsigned up : 1;
        unsigned reserved_31_2 : 30;
    };
    unsigned reg;
} GBE__MAC_Rx_Flow_Ctrl__ACC_T;

#define GBE__GBE0__MAC_RX_FLOW_CTRL__ADDR (GBE__GBE0__BASE_ADDR + 0x90)
#define GBE__GBE0__MAC_RX_FLOW_CTRL__NUM  0x1

#define GBE__GBE0__MAC_RX_FLOW_CTRL__RFE__SHIFT    0
#define GBE__GBE0__MAC_RX_FLOW_CTRL__UP__SHIFT    1
#define GBE__GBE0__MAC_RX_FLOW_CTRL__RESERVED_31_2__SHIFT    2

#define GBE__GBE0__MAC_RX_FLOW_CTRL__RFE__MASK    0x00000001
#define GBE__GBE0__MAC_RX_FLOW_CTRL__UP__MASK    0x00000002
#define GBE__GBE0__MAC_RX_FLOW_CTRL__RESERVED_31_2__MASK    0xfffffffc

#define GBE__GBE0__MAC_RX_FLOW_CTRL__RFE__POR_VALUE    0x0
#define GBE__GBE0__MAC_RX_FLOW_CTRL__UP__POR_VALUE    0x0
#define GBE__GBE0__MAC_RX_FLOW_CTRL__RESERVED_31_2__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Interrupt_Status
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rgsmiiis : 1;
        unsigned reserved_2_1 : 2;
        unsigned phyis : 1;
        unsigned reserved_7_4 : 4;
        unsigned mmcis : 1;
        unsigned mmcrxis : 1;
        unsigned mmctxis : 1;
        unsigned reserved_11 : 1;
        unsigned tsis : 1;
        unsigned txstsis : 1;
        unsigned rxstsis : 1;
        unsigned reserved_17_15 : 3;
        unsigned mdiois : 1;
        unsigned reserved_31_19 : 13;
    };
    unsigned reg;
} GBE__MAC_Interrupt_Status__ACC_T;

#define GBE__GBE0__MAC_INTERRUPT_STATUS__ADDR (GBE__GBE0__BASE_ADDR + 0xb0)
#define GBE__GBE0__MAC_INTERRUPT_STATUS__NUM  0x1

#define GBE__GBE0__MAC_INTERRUPT_STATUS__RGSMIIIS__SHIFT    0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_2_1__SHIFT    1
#define GBE__GBE0__MAC_INTERRUPT_STATUS__PHYIS__SHIFT    3
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_7_4__SHIFT    4
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MMCIS__SHIFT    8
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MMCRXIS__SHIFT    9
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MMCTXIS__SHIFT    10
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_11__SHIFT    11
#define GBE__GBE0__MAC_INTERRUPT_STATUS__TSIS__SHIFT    12
#define GBE__GBE0__MAC_INTERRUPT_STATUS__TXSTSIS__SHIFT    13
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RXSTSIS__SHIFT    14
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_17_15__SHIFT    15
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MDIOIS__SHIFT    18
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_31_19__SHIFT    19

#define GBE__GBE0__MAC_INTERRUPT_STATUS__RGSMIIIS__MASK    0x00000001
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_2_1__MASK    0x00000006
#define GBE__GBE0__MAC_INTERRUPT_STATUS__PHYIS__MASK    0x00000008
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_7_4__MASK    0x000000f0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MMCIS__MASK    0x00000100
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MMCRXIS__MASK    0x00000200
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MMCTXIS__MASK    0x00000400
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_11__MASK    0x00000800
#define GBE__GBE0__MAC_INTERRUPT_STATUS__TSIS__MASK    0x00001000
#define GBE__GBE0__MAC_INTERRUPT_STATUS__TXSTSIS__MASK    0x00002000
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RXSTSIS__MASK    0x00004000
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_17_15__MASK    0x00038000
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MDIOIS__MASK    0x00040000
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_31_19__MASK    0xfff80000

#define GBE__GBE0__MAC_INTERRUPT_STATUS__RGSMIIIS__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_2_1__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__PHYIS__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_7_4__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MMCIS__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MMCRXIS__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MMCTXIS__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_11__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__TSIS__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__TXSTSIS__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RXSTSIS__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_17_15__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__MDIOIS__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_STATUS__RESERVED_31_19__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Interrupt_Enable
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rgsmiiie : 1;
        unsigned reserved_2_1 : 2;
        unsigned phyie : 1;
        unsigned reserved_11_4 : 8;
        unsigned tsie : 1;
        unsigned txstsie : 1;
        unsigned rxstsie : 1;
        unsigned reserved_17_15 : 3;
        unsigned mdioie : 1;
        unsigned reserved_31_19 : 13;
    };
    unsigned reg;
} GBE__MAC_Interrupt_Enable__ACC_T;

#define GBE__GBE0__MAC_INTERRUPT_ENABLE__ADDR (GBE__GBE0__BASE_ADDR + 0xb4)
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__NUM  0x1

#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RGSMIIIE__SHIFT    0
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_2_1__SHIFT    1
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__PHYIE__SHIFT    3
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_11_4__SHIFT    4
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__TSIE__SHIFT    12
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__TXSTSIE__SHIFT    13
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RXSTSIE__SHIFT    14
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_17_15__SHIFT    15
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__MDIOIE__SHIFT    18
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_31_19__SHIFT    19

#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RGSMIIIE__MASK    0x00000001
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_2_1__MASK    0x00000006
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__PHYIE__MASK    0x00000008
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_11_4__MASK    0x00000ff0
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__TSIE__MASK    0x00001000
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__TXSTSIE__MASK    0x00002000
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RXSTSIE__MASK    0x00004000
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_17_15__MASK    0x00038000
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__MDIOIE__MASK    0x00040000
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_31_19__MASK    0xfff80000

#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RGSMIIIE__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_2_1__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__PHYIE__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_11_4__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__TSIE__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__TXSTSIE__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RXSTSIE__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_17_15__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__MDIOIE__POR_VALUE    0x0
#define GBE__GBE0__MAC_INTERRUPT_ENABLE__RESERVED_31_19__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Rx_Tx_Status
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tjt : 1;
        unsigned reserved_7_1 : 7;
        unsigned rwt : 1;
        unsigned reserved_31_9 : 23;
    };
    unsigned reg;
} GBE__MAC_Rx_Tx_Status__ACC_T;

#define GBE__GBE0__MAC_RX_TX_STATUS__ADDR (GBE__GBE0__BASE_ADDR + 0xb8)
#define GBE__GBE0__MAC_RX_TX_STATUS__NUM  0x1

#define GBE__GBE0__MAC_RX_TX_STATUS__TJT__SHIFT    0
#define GBE__GBE0__MAC_RX_TX_STATUS__RESERVED_7_1__SHIFT    1
#define GBE__GBE0__MAC_RX_TX_STATUS__RWT__SHIFT    8
#define GBE__GBE0__MAC_RX_TX_STATUS__RESERVED_31_9__SHIFT    9

#define GBE__GBE0__MAC_RX_TX_STATUS__TJT__MASK    0x00000001
#define GBE__GBE0__MAC_RX_TX_STATUS__RESERVED_7_1__MASK    0x000000fe
#define GBE__GBE0__MAC_RX_TX_STATUS__RWT__MASK    0x00000100
#define GBE__GBE0__MAC_RX_TX_STATUS__RESERVED_31_9__MASK    0xfffffe00

#define GBE__GBE0__MAC_RX_TX_STATUS__TJT__POR_VALUE    0x0
#define GBE__GBE0__MAC_RX_TX_STATUS__RESERVED_7_1__POR_VALUE    0x0
#define GBE__GBE0__MAC_RX_TX_STATUS__RWT__POR_VALUE    0x0
#define GBE__GBE0__MAC_RX_TX_STATUS__RESERVED_31_9__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_PHYIF_Control_Status
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tc : 1;
        unsigned lud : 1;
        unsigned reserved_15_2 : 14;
        unsigned lnkmod : 1;
        unsigned lnkspeed : 2;
        unsigned lnksts : 1;
        unsigned reserved_31_20 : 12;
    };
    unsigned reg;
} GBE__MAC_PHYIF_Control_Status__ACC_T;

#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__ADDR (GBE__GBE0__BASE_ADDR + 0xf8)
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__NUM  0x1

#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__TC__SHIFT    0
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LUD__SHIFT    1
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__RESERVED_15_2__SHIFT    2
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LNKMOD__SHIFT    16
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LNKSPEED__SHIFT    17
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LNKSTS__SHIFT    19
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__RESERVED_31_20__SHIFT    20

#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__TC__MASK    0x00000001
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LUD__MASK    0x00000002
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__RESERVED_15_2__MASK    0x0000fffc
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LNKMOD__MASK    0x00010000
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LNKSPEED__MASK    0x00060000
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LNKSTS__MASK    0x00080000
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__RESERVED_31_20__MASK    0xfff00000

#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__TC__POR_VALUE    0x0
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LUD__POR_VALUE    0x0
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__RESERVED_15_2__POR_VALUE    0x0
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LNKMOD__POR_VALUE    0x0
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LNKSPEED__POR_VALUE    0x0
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__LNKSTS__POR_VALUE    0x0
#define GBE__GBE0__MAC_PHYIF_CONTROL_STATUS__RESERVED_31_20__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Version
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned snpsver : 8;
        unsigned userver : 8;
        unsigned reserved_31_16 : 16;
    };
    unsigned reg;
} GBE__MAC_Version__ACC_T;

#define GBE__GBE0__MAC_VERSION__ADDR (GBE__GBE0__BASE_ADDR + 0x110)
#define GBE__GBE0__MAC_VERSION__NUM  0x1

#define GBE__GBE0__MAC_VERSION__SNPSVER__SHIFT    0
#define GBE__GBE0__MAC_VERSION__USERVER__SHIFT    8
#define GBE__GBE0__MAC_VERSION__RESERVED_31_16__SHIFT    16

#define GBE__GBE0__MAC_VERSION__SNPSVER__MASK    0x000000ff
#define GBE__GBE0__MAC_VERSION__USERVER__MASK    0x0000ff00
#define GBE__GBE0__MAC_VERSION__RESERVED_31_16__MASK    0xffff0000

#define GBE__GBE0__MAC_VERSION__SNPSVER__POR_VALUE    0x0
#define GBE__GBE0__MAC_VERSION__USERVER__POR_VALUE    0x0
#define GBE__GBE0__MAC_VERSION__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Debug
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rpests : 1;
        unsigned rfcfcsts : 2;
        unsigned reserved_15_3 : 13;
        unsigned tpests : 1;
        unsigned tfcsts : 2;
        unsigned reserved_31_19 : 13;
    };
    unsigned reg;
} GBE__MAC_Debug__ACC_T;

#define GBE__GBE0__MAC_DEBUG__ADDR (GBE__GBE0__BASE_ADDR + 0x114)
#define GBE__GBE0__MAC_DEBUG__NUM  0x1

#define GBE__GBE0__MAC_DEBUG__RPESTS__SHIFT    0
#define GBE__GBE0__MAC_DEBUG__RFCFCSTS__SHIFT    1
#define GBE__GBE0__MAC_DEBUG__RESERVED_15_3__SHIFT    3
#define GBE__GBE0__MAC_DEBUG__TPESTS__SHIFT    16
#define GBE__GBE0__MAC_DEBUG__TFCSTS__SHIFT    17
#define GBE__GBE0__MAC_DEBUG__RESERVED_31_19__SHIFT    19

#define GBE__GBE0__MAC_DEBUG__RPESTS__MASK    0x00000001
#define GBE__GBE0__MAC_DEBUG__RFCFCSTS__MASK    0x00000006
#define GBE__GBE0__MAC_DEBUG__RESERVED_15_3__MASK    0x0000fff8
#define GBE__GBE0__MAC_DEBUG__TPESTS__MASK    0x00010000
#define GBE__GBE0__MAC_DEBUG__TFCSTS__MASK    0x00060000
#define GBE__GBE0__MAC_DEBUG__RESERVED_31_19__MASK    0xfff80000

#define GBE__GBE0__MAC_DEBUG__RPESTS__POR_VALUE    0x0
#define GBE__GBE0__MAC_DEBUG__RFCFCSTS__POR_VALUE    0x0
#define GBE__GBE0__MAC_DEBUG__RESERVED_15_3__POR_VALUE    0x0
#define GBE__GBE0__MAC_DEBUG__TPESTS__POR_VALUE    0x0
#define GBE__GBE0__MAC_DEBUG__TFCSTS__POR_VALUE    0x0
#define GBE__GBE0__MAC_DEBUG__RESERVED_31_19__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_HW_Feature0
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned miisel : 1;
        unsigned gmiisel : 1;
        unsigned hdsel : 1;
        unsigned pcssel : 1;
        unsigned vlhash : 1;
        unsigned smasel : 1;
        unsigned rwksel : 1;
        unsigned mgksel : 1;
        unsigned mmcsel : 1;
        unsigned arpoffsel : 1;
        unsigned reserved_11_10 : 2;
        unsigned tssel : 1;
        unsigned eeesel : 1;
        unsigned txcoesel : 1;
        unsigned reserved_15 : 1;
        unsigned rxcoesel : 1;
        unsigned reserved_17 : 1;
        unsigned addmacadrsel : 5;
        unsigned macadr32sel : 1;
        unsigned macadr64sel : 1;
        unsigned tsstssel : 2;
        unsigned savlanins : 1;
        unsigned actphysel : 3;
        unsigned reserved_31 : 1;
    };
    unsigned reg;
} GBE__MAC_HW_Feature0__ACC_T;

#define GBE__GBE0__MAC_HW_FEATURE0__ADDR (GBE__GBE0__BASE_ADDR + 0x11c)
#define GBE__GBE0__MAC_HW_FEATURE0__NUM  0x1

#define GBE__GBE0__MAC_HW_FEATURE0__MIISEL__SHIFT    0
#define GBE__GBE0__MAC_HW_FEATURE0__GMIISEL__SHIFT    1
#define GBE__GBE0__MAC_HW_FEATURE0__HDSEL__SHIFT    2
#define GBE__GBE0__MAC_HW_FEATURE0__PCSSEL__SHIFT    3
#define GBE__GBE0__MAC_HW_FEATURE0__VLHASH__SHIFT    4
#define GBE__GBE0__MAC_HW_FEATURE0__SMASEL__SHIFT    5
#define GBE__GBE0__MAC_HW_FEATURE0__RWKSEL__SHIFT    6
#define GBE__GBE0__MAC_HW_FEATURE0__MGKSEL__SHIFT    7
#define GBE__GBE0__MAC_HW_FEATURE0__MMCSEL__SHIFT    8
#define GBE__GBE0__MAC_HW_FEATURE0__ARPOFFSEL__SHIFT    9
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_11_10__SHIFT    10
#define GBE__GBE0__MAC_HW_FEATURE0__TSSEL__SHIFT    12
#define GBE__GBE0__MAC_HW_FEATURE0__EEESEL__SHIFT    13
#define GBE__GBE0__MAC_HW_FEATURE0__TXCOESEL__SHIFT    14
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_15__SHIFT    15
#define GBE__GBE0__MAC_HW_FEATURE0__RXCOESEL__SHIFT    16
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_17__SHIFT    17
#define GBE__GBE0__MAC_HW_FEATURE0__ADDMACADRSEL__SHIFT    18
#define GBE__GBE0__MAC_HW_FEATURE0__MACADR32SEL__SHIFT    23
#define GBE__GBE0__MAC_HW_FEATURE0__MACADR64SEL__SHIFT    24
#define GBE__GBE0__MAC_HW_FEATURE0__TSSTSSEL__SHIFT    25
#define GBE__GBE0__MAC_HW_FEATURE0__SAVLANINS__SHIFT    27
#define GBE__GBE0__MAC_HW_FEATURE0__ACTPHYSEL__SHIFT    28
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_31__SHIFT    31

#define GBE__GBE0__MAC_HW_FEATURE0__MIISEL__MASK    0x00000001
#define GBE__GBE0__MAC_HW_FEATURE0__GMIISEL__MASK    0x00000002
#define GBE__GBE0__MAC_HW_FEATURE0__HDSEL__MASK    0x00000004
#define GBE__GBE0__MAC_HW_FEATURE0__PCSSEL__MASK    0x00000008
#define GBE__GBE0__MAC_HW_FEATURE0__VLHASH__MASK    0x00000010
#define GBE__GBE0__MAC_HW_FEATURE0__SMASEL__MASK    0x00000020
#define GBE__GBE0__MAC_HW_FEATURE0__RWKSEL__MASK    0x00000040
#define GBE__GBE0__MAC_HW_FEATURE0__MGKSEL__MASK    0x00000080
#define GBE__GBE0__MAC_HW_FEATURE0__MMCSEL__MASK    0x00000100
#define GBE__GBE0__MAC_HW_FEATURE0__ARPOFFSEL__MASK    0x00000200
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_11_10__MASK    0x00000c00
#define GBE__GBE0__MAC_HW_FEATURE0__TSSEL__MASK    0x00001000
#define GBE__GBE0__MAC_HW_FEATURE0__EEESEL__MASK    0x00002000
#define GBE__GBE0__MAC_HW_FEATURE0__TXCOESEL__MASK    0x00004000
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_15__MASK    0x00008000
#define GBE__GBE0__MAC_HW_FEATURE0__RXCOESEL__MASK    0x00010000
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_17__MASK    0x00020000
#define GBE__GBE0__MAC_HW_FEATURE0__ADDMACADRSEL__MASK    0x007c0000
#define GBE__GBE0__MAC_HW_FEATURE0__MACADR32SEL__MASK    0x00800000
#define GBE__GBE0__MAC_HW_FEATURE0__MACADR64SEL__MASK    0x01000000
#define GBE__GBE0__MAC_HW_FEATURE0__TSSTSSEL__MASK    0x06000000
#define GBE__GBE0__MAC_HW_FEATURE0__SAVLANINS__MASK    0x08000000
#define GBE__GBE0__MAC_HW_FEATURE0__ACTPHYSEL__MASK    0x70000000
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_31__MASK    0x80000000

#define GBE__GBE0__MAC_HW_FEATURE0__MIISEL__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE0__GMIISEL__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE0__HDSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__PCSSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__VLHASH__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__SMASEL__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE0__RWKSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__MGKSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__MMCSEL__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE0__ARPOFFSEL__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_11_10__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__TSSEL__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE0__EEESEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__TXCOESEL__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_15__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__RXCOESEL__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_17__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__ADDMACADRSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__MACADR32SEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__MACADR64SEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__TSSTSSEL__POR_VALUE    0x3
#define GBE__GBE0__MAC_HW_FEATURE0__SAVLANINS__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__ACTPHYSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE0__RESERVED_31__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_HW_Feature1
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxfifosize : 5;
        unsigned spram : 1;
        unsigned txfifosize : 5;
        unsigned osten : 1;
        unsigned ptoen : 1;
        unsigned advthword : 1;
        unsigned addr64 : 2;
        unsigned dcben : 1;
        unsigned sphen : 1;
        unsigned tsoen : 1;
        unsigned dbgmema : 1;
        unsigned avsel : 1;
        unsigned ravsel : 1;
        unsigned reserved_22 : 1;
        unsigned pouost : 1;
        unsigned hashtblsz : 2;
        unsigned reserved_26 : 1;
        unsigned l3l4fnum : 4;
        unsigned reserved_31 : 1;
    };
    unsigned reg;
} GBE__MAC_HW_Feature1__ACC_T;

#define GBE__GBE0__MAC_HW_FEATURE1__ADDR (GBE__GBE0__BASE_ADDR + 0x120)
#define GBE__GBE0__MAC_HW_FEATURE1__NUM  0x1

#define GBE__GBE0__MAC_HW_FEATURE1__RXFIFOSIZE__SHIFT    0
#define GBE__GBE0__MAC_HW_FEATURE1__SPRAM__SHIFT    5
#define GBE__GBE0__MAC_HW_FEATURE1__TXFIFOSIZE__SHIFT    6
#define GBE__GBE0__MAC_HW_FEATURE1__OSTEN__SHIFT    11
#define GBE__GBE0__MAC_HW_FEATURE1__PTOEN__SHIFT    12
#define GBE__GBE0__MAC_HW_FEATURE1__ADVTHWORD__SHIFT    13
#define GBE__GBE0__MAC_HW_FEATURE1__ADDR64__SHIFT    14
#define GBE__GBE0__MAC_HW_FEATURE1__DCBEN__SHIFT    16
#define GBE__GBE0__MAC_HW_FEATURE1__SPHEN__SHIFT    17
#define GBE__GBE0__MAC_HW_FEATURE1__TSOEN__SHIFT    18
#define GBE__GBE0__MAC_HW_FEATURE1__DBGMEMA__SHIFT    19
#define GBE__GBE0__MAC_HW_FEATURE1__AVSEL__SHIFT    20
#define GBE__GBE0__MAC_HW_FEATURE1__RAVSEL__SHIFT    21
#define GBE__GBE0__MAC_HW_FEATURE1__RESERVED_22__SHIFT    22
#define GBE__GBE0__MAC_HW_FEATURE1__POUOST__SHIFT    23
#define GBE__GBE0__MAC_HW_FEATURE1__HASHTBLSZ__SHIFT    24
#define GBE__GBE0__MAC_HW_FEATURE1__RESERVED_26__SHIFT    26
#define GBE__GBE0__MAC_HW_FEATURE1__L3L4FNUM__SHIFT    27
#define GBE__GBE0__MAC_HW_FEATURE1__RESERVED_31__SHIFT    31

#define GBE__GBE0__MAC_HW_FEATURE1__RXFIFOSIZE__MASK    0x0000001f
#define GBE__GBE0__MAC_HW_FEATURE1__SPRAM__MASK    0x00000020
#define GBE__GBE0__MAC_HW_FEATURE1__TXFIFOSIZE__MASK    0x000007c0
#define GBE__GBE0__MAC_HW_FEATURE1__OSTEN__MASK    0x00000800
#define GBE__GBE0__MAC_HW_FEATURE1__PTOEN__MASK    0x00001000
#define GBE__GBE0__MAC_HW_FEATURE1__ADVTHWORD__MASK    0x00002000
#define GBE__GBE0__MAC_HW_FEATURE1__ADDR64__MASK    0x0000c000
#define GBE__GBE0__MAC_HW_FEATURE1__DCBEN__MASK    0x00010000
#define GBE__GBE0__MAC_HW_FEATURE1__SPHEN__MASK    0x00020000
#define GBE__GBE0__MAC_HW_FEATURE1__TSOEN__MASK    0x00040000
#define GBE__GBE0__MAC_HW_FEATURE1__DBGMEMA__MASK    0x00080000
#define GBE__GBE0__MAC_HW_FEATURE1__AVSEL__MASK    0x00100000
#define GBE__GBE0__MAC_HW_FEATURE1__RAVSEL__MASK    0x00200000
#define GBE__GBE0__MAC_HW_FEATURE1__RESERVED_22__MASK    0x00400000
#define GBE__GBE0__MAC_HW_FEATURE1__POUOST__MASK    0x00800000
#define GBE__GBE0__MAC_HW_FEATURE1__HASHTBLSZ__MASK    0x03000000
#define GBE__GBE0__MAC_HW_FEATURE1__RESERVED_26__MASK    0x04000000
#define GBE__GBE0__MAC_HW_FEATURE1__L3L4FNUM__MASK    0x78000000
#define GBE__GBE0__MAC_HW_FEATURE1__RESERVED_31__MASK    0x80000000

#define GBE__GBE0__MAC_HW_FEATURE1__RXFIFOSIZE__POR_VALUE    0x7
#define GBE__GBE0__MAC_HW_FEATURE1__SPRAM__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE1__TXFIFOSIZE__POR_VALUE    0x7
#define GBE__GBE0__MAC_HW_FEATURE1__OSTEN__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE1__PTOEN__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__ADVTHWORD__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__ADDR64__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__DCBEN__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__SPHEN__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__TSOEN__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE1__DBGMEMA__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__AVSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__RAVSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__RESERVED_22__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__POUOST__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE1__HASHTBLSZ__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__RESERVED_26__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__L3L4FNUM__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE1__RESERVED_31__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_HW_Feature2
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxqcnt : 4;
        unsigned reserved_5_4 : 2;
        unsigned txqcnt : 4;
        unsigned reserved_11_10 : 2;
        unsigned rxchcnt : 4;
        unsigned reserved_17_16 : 2;
        unsigned txchcnt : 4;
        unsigned reserved_23_22 : 2;
        unsigned ppsoutnum : 3;
        unsigned reserved_27 : 1;
        unsigned auxsnapnum : 3;
        unsigned reserved_31 : 1;
    };
    unsigned reg;
} GBE__MAC_HW_Feature2__ACC_T;

#define GBE__GBE0__MAC_HW_FEATURE2__ADDR (GBE__GBE0__BASE_ADDR + 0x124)
#define GBE__GBE0__MAC_HW_FEATURE2__NUM  0x1

#define GBE__GBE0__MAC_HW_FEATURE2__RXQCNT__SHIFT    0
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_5_4__SHIFT    4
#define GBE__GBE0__MAC_HW_FEATURE2__TXQCNT__SHIFT    6
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_11_10__SHIFT    10
#define GBE__GBE0__MAC_HW_FEATURE2__RXCHCNT__SHIFT    12
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_17_16__SHIFT    16
#define GBE__GBE0__MAC_HW_FEATURE2__TXCHCNT__SHIFT    18
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_23_22__SHIFT    22
#define GBE__GBE0__MAC_HW_FEATURE2__PPSOUTNUM__SHIFT    24
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_27__SHIFT    27
#define GBE__GBE0__MAC_HW_FEATURE2__AUXSNAPNUM__SHIFT    28
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_31__SHIFT    31

#define GBE__GBE0__MAC_HW_FEATURE2__RXQCNT__MASK    0x0000000f
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_5_4__MASK    0x00000030
#define GBE__GBE0__MAC_HW_FEATURE2__TXQCNT__MASK    0x000003c0
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_11_10__MASK    0x00000c00
#define GBE__GBE0__MAC_HW_FEATURE2__RXCHCNT__MASK    0x0000f000
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_17_16__MASK    0x00030000
#define GBE__GBE0__MAC_HW_FEATURE2__TXCHCNT__MASK    0x003c0000
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_23_22__MASK    0x00c00000
#define GBE__GBE0__MAC_HW_FEATURE2__PPSOUTNUM__MASK    0x07000000
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_27__MASK    0x08000000
#define GBE__GBE0__MAC_HW_FEATURE2__AUXSNAPNUM__MASK    0x70000000
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_31__MASK    0x80000000

#define GBE__GBE0__MAC_HW_FEATURE2__RXQCNT__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_5_4__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE2__TXQCNT__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_11_10__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE2__RXCHCNT__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_17_16__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE2__TXCHCNT__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_23_22__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE2__PPSOUTNUM__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_27__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE2__AUXSNAPNUM__POR_VALUE    0x1
#define GBE__GBE0__MAC_HW_FEATURE2__RESERVED_31__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_HW_Feature3
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned nrvf : 3;
        unsigned reserved_3 : 1;
        unsigned cbtisel : 1;
        unsigned dvlan : 1;
        unsigned reserved_8_6 : 3;
        unsigned pdupsel : 1;
        unsigned frpsel : 1;
        unsigned frpbs : 2;
        unsigned frpes : 2;
        unsigned reserved_15 : 1;
        unsigned estsel : 1;
        unsigned estdep : 3;
        unsigned estwid : 2;
        unsigned reserved_25_22 : 4;
        unsigned fpesel : 1;
        unsigned tbssel : 1;
        unsigned asp : 2;
        unsigned reserved_31_30 : 2;
    };
    unsigned reg;
} GBE__MAC_HW_Feature3__ACC_T;

#define GBE__GBE0__MAC_HW_FEATURE3__ADDR (GBE__GBE0__BASE_ADDR + 0x128)
#define GBE__GBE0__MAC_HW_FEATURE3__NUM  0x1

#define GBE__GBE0__MAC_HW_FEATURE3__NRVF__SHIFT    0
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_3__SHIFT    3
#define GBE__GBE0__MAC_HW_FEATURE3__CBTISEL__SHIFT    4
#define GBE__GBE0__MAC_HW_FEATURE3__DVLAN__SHIFT    5
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_8_6__SHIFT    6
#define GBE__GBE0__MAC_HW_FEATURE3__PDUPSEL__SHIFT    9
#define GBE__GBE0__MAC_HW_FEATURE3__FRPSEL__SHIFT    10
#define GBE__GBE0__MAC_HW_FEATURE3__FRPBS__SHIFT    11
#define GBE__GBE0__MAC_HW_FEATURE3__FRPES__SHIFT    13
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_15__SHIFT    15
#define GBE__GBE0__MAC_HW_FEATURE3__ESTSEL__SHIFT    16
#define GBE__GBE0__MAC_HW_FEATURE3__ESTDEP__SHIFT    17
#define GBE__GBE0__MAC_HW_FEATURE3__ESTWID__SHIFT    20
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_25_22__SHIFT    22
#define GBE__GBE0__MAC_HW_FEATURE3__FPESEL__SHIFT    26
#define GBE__GBE0__MAC_HW_FEATURE3__TBSSEL__SHIFT    27
#define GBE__GBE0__MAC_HW_FEATURE3__ASP__SHIFT    28
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_31_30__SHIFT    30

#define GBE__GBE0__MAC_HW_FEATURE3__NRVF__MASK    0x00000007
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_3__MASK    0x00000008
#define GBE__GBE0__MAC_HW_FEATURE3__CBTISEL__MASK    0x00000010
#define GBE__GBE0__MAC_HW_FEATURE3__DVLAN__MASK    0x00000020
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_8_6__MASK    0x000001c0
#define GBE__GBE0__MAC_HW_FEATURE3__PDUPSEL__MASK    0x00000200
#define GBE__GBE0__MAC_HW_FEATURE3__FRPSEL__MASK    0x00000400
#define GBE__GBE0__MAC_HW_FEATURE3__FRPBS__MASK    0x00001800
#define GBE__GBE0__MAC_HW_FEATURE3__FRPES__MASK    0x00006000
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_15__MASK    0x00008000
#define GBE__GBE0__MAC_HW_FEATURE3__ESTSEL__MASK    0x00010000
#define GBE__GBE0__MAC_HW_FEATURE3__ESTDEP__MASK    0x000e0000
#define GBE__GBE0__MAC_HW_FEATURE3__ESTWID__MASK    0x00300000
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_25_22__MASK    0x03c00000
#define GBE__GBE0__MAC_HW_FEATURE3__FPESEL__MASK    0x04000000
#define GBE__GBE0__MAC_HW_FEATURE3__TBSSEL__MASK    0x08000000
#define GBE__GBE0__MAC_HW_FEATURE3__ASP__MASK    0x30000000
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_31_30__MASK    0xc0000000

#define GBE__GBE0__MAC_HW_FEATURE3__NRVF__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_3__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__CBTISEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__DVLAN__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_8_6__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__PDUPSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__FRPSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__FRPBS__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__FRPES__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_15__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__ESTSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__ESTDEP__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__ESTWID__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_25_22__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__FPESEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__TBSSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__ASP__POR_VALUE    0x0
#define GBE__GBE0__MAC_HW_FEATURE3__RESERVED_31_30__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_MDIO_Address
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned gb : 1;
        unsigned c45e : 1;
        unsigned goc_0 : 1;
        unsigned goc_1 : 1;
        unsigned skap : 1;
        unsigned reserved_7_5 : 3;
        unsigned cr : 4;
        unsigned ntc : 3;
        unsigned reserved_15 : 1;
        unsigned rda : 5;
        unsigned pa : 5;
        unsigned btb : 1;
        unsigned pse : 1;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} GBE__MAC_MDIO_Address__ACC_T;

#define GBE__GBE0__MAC_MDIO_ADDRESS__ADDR (GBE__GBE0__BASE_ADDR + 0x200)
#define GBE__GBE0__MAC_MDIO_ADDRESS__NUM  0x1

#define GBE__GBE0__MAC_MDIO_ADDRESS__GB__SHIFT    0
#define GBE__GBE0__MAC_MDIO_ADDRESS__C45E__SHIFT    1
#define GBE__GBE0__MAC_MDIO_ADDRESS__GOC_0__SHIFT    2
#define GBE__GBE0__MAC_MDIO_ADDRESS__GOC_1__SHIFT    3
#define GBE__GBE0__MAC_MDIO_ADDRESS__SKAP__SHIFT    4
#define GBE__GBE0__MAC_MDIO_ADDRESS__RESERVED_7_5__SHIFT    5
#define GBE__GBE0__MAC_MDIO_ADDRESS__CR__SHIFT    8
#define GBE__GBE0__MAC_MDIO_ADDRESS__NTC__SHIFT    12
#define GBE__GBE0__MAC_MDIO_ADDRESS__RESERVED_15__SHIFT    15
#define GBE__GBE0__MAC_MDIO_ADDRESS__RDA__SHIFT    16
#define GBE__GBE0__MAC_MDIO_ADDRESS__PA__SHIFT    21
#define GBE__GBE0__MAC_MDIO_ADDRESS__BTB__SHIFT    26
#define GBE__GBE0__MAC_MDIO_ADDRESS__PSE__SHIFT    27
#define GBE__GBE0__MAC_MDIO_ADDRESS__RESERVED_31_28__SHIFT    28

#define GBE__GBE0__MAC_MDIO_ADDRESS__GB__MASK    0x00000001
#define GBE__GBE0__MAC_MDIO_ADDRESS__C45E__MASK    0x00000002
#define GBE__GBE0__MAC_MDIO_ADDRESS__GOC_0__MASK    0x00000004
#define GBE__GBE0__MAC_MDIO_ADDRESS__GOC_1__MASK    0x00000008
#define GBE__GBE0__MAC_MDIO_ADDRESS__SKAP__MASK    0x00000010
#define GBE__GBE0__MAC_MDIO_ADDRESS__RESERVED_7_5__MASK    0x000000e0
#define GBE__GBE0__MAC_MDIO_ADDRESS__CR__MASK    0x00000f00
#define GBE__GBE0__MAC_MDIO_ADDRESS__NTC__MASK    0x00007000
#define GBE__GBE0__MAC_MDIO_ADDRESS__RESERVED_15__MASK    0x00008000
#define GBE__GBE0__MAC_MDIO_ADDRESS__RDA__MASK    0x001f0000
#define GBE__GBE0__MAC_MDIO_ADDRESS__PA__MASK    0x03e00000
#define GBE__GBE0__MAC_MDIO_ADDRESS__BTB__MASK    0x04000000
#define GBE__GBE0__MAC_MDIO_ADDRESS__PSE__MASK    0x08000000
#define GBE__GBE0__MAC_MDIO_ADDRESS__RESERVED_31_28__MASK    0xf0000000

#define GBE__GBE0__MAC_MDIO_ADDRESS__GB__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__C45E__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__GOC_0__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__GOC_1__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__SKAP__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__RESERVED_7_5__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__CR__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__NTC__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__RESERVED_15__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__RDA__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__PA__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__BTB__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__PSE__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_ADDRESS__RESERVED_31_28__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_MDIO_Data
// Register Address
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned gd : 16;
        unsigned ra : 16;
    };
    unsigned reg;
} GBE__MAC_MDIO_Data__ACC_T;

#define GBE__GBE0__MAC_MDIO_DATA__ADDR (GBE__GBE0__BASE_ADDR + 0x204)
#define GBE__GBE0__MAC_MDIO_DATA__NUM  0x1

#define GBE__GBE0__MAC_MDIO_DATA__GD__SHIFT    0
#define GBE__GBE0__MAC_MDIO_DATA__RA__SHIFT    16

#define GBE__GBE0__MAC_MDIO_DATA__GD__MASK    0x0000ffff
#define GBE__GBE0__MAC_MDIO_DATA__RA__MASK    0xffff0000

#define GBE__GBE0__MAC_MDIO_DATA__GD__POR_VALUE    0x0
#define GBE__GBE0__MAC_MDIO_DATA__RA__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_ARP_Address
//
// ARP Protocol Address
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned arppa : 32;
    };
    unsigned reg;
} GBE__MAC_ARP_Address__ACC_T;

#define GBE__GBE0__MAC_ARP_ADDRESS__ADDR (GBE__GBE0__BASE_ADDR + 0x210)
#define GBE__GBE0__MAC_ARP_ADDRESS__NUM  0x1

#define GBE__GBE0__MAC_ARP_ADDRESS__ARPPA__SHIFT    0

#define GBE__GBE0__MAC_ARP_ADDRESS__ARPPA__MASK    0xffffffff

#define GBE__GBE0__MAC_ARP_ADDRESS__ARPPA__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_CSR_SW_Ctrl
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rcwe : 1;
        unsigned reserved_7_1 : 7;
        unsigned seen : 1;
        unsigned reserved_31_9 : 23;
    };
    unsigned reg;
} GBE__MAC_CSR_SW_Ctrl__ACC_T;

#define GBE__GBE0__MAC_CSR_SW_CTRL__ADDR (GBE__GBE0__BASE_ADDR + 0x230)
#define GBE__GBE0__MAC_CSR_SW_CTRL__NUM  0x1

#define GBE__GBE0__MAC_CSR_SW_CTRL__RCWE__SHIFT    0
#define GBE__GBE0__MAC_CSR_SW_CTRL__RESERVED_7_1__SHIFT    1
#define GBE__GBE0__MAC_CSR_SW_CTRL__SEEN__SHIFT    8
#define GBE__GBE0__MAC_CSR_SW_CTRL__RESERVED_31_9__SHIFT    9

#define GBE__GBE0__MAC_CSR_SW_CTRL__RCWE__MASK    0x00000001
#define GBE__GBE0__MAC_CSR_SW_CTRL__RESERVED_7_1__MASK    0x000000fe
#define GBE__GBE0__MAC_CSR_SW_CTRL__SEEN__MASK    0x00000100
#define GBE__GBE0__MAC_CSR_SW_CTRL__RESERVED_31_9__MASK    0xfffffe00

#define GBE__GBE0__MAC_CSR_SW_CTRL__RCWE__POR_VALUE    0x0
#define GBE__GBE0__MAC_CSR_SW_CTRL__RESERVED_7_1__POR_VALUE    0x0
#define GBE__GBE0__MAC_CSR_SW_CTRL__SEEN__POR_VALUE    0x0
#define GBE__GBE0__MAC_CSR_SW_CTRL__RESERVED_31_9__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Address0_High
// Address Enable
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned addrhi : 16;
        unsigned reserved_30_16 : 15;
        unsigned ae : 1;
    };
    unsigned reg;
} GBE__MAC_Address0_High__ACC_T;

#define GBE__GBE0__MAC_ADDRESS0_HIGH__ADDR (GBE__GBE0__BASE_ADDR + 0x300)
#define GBE__GBE0__MAC_ADDRESS0_HIGH__NUM  0x1

#define GBE__GBE0__MAC_ADDRESS0_HIGH__ADDRHI__SHIFT    0
#define GBE__GBE0__MAC_ADDRESS0_HIGH__RESERVED_30_16__SHIFT    16
#define GBE__GBE0__MAC_ADDRESS0_HIGH__AE__SHIFT    31

#define GBE__GBE0__MAC_ADDRESS0_HIGH__ADDRHI__MASK    0x0000ffff
#define GBE__GBE0__MAC_ADDRESS0_HIGH__RESERVED_30_16__MASK    0x7fff0000
#define GBE__GBE0__MAC_ADDRESS0_HIGH__AE__MASK    0x80000000

#define GBE__GBE0__MAC_ADDRESS0_HIGH__ADDRHI__POR_VALUE    0x0
#define GBE__GBE0__MAC_ADDRESS0_HIGH__RESERVED_30_16__POR_VALUE    0x0
#define GBE__GBE0__MAC_ADDRESS0_HIGH__AE__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Address0_Low
// MAC Address0[31:0]
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned addrlo : 32;
    };
    unsigned reg;
} GBE__MAC_Address0_Low__ACC_T;

#define GBE__GBE0__MAC_ADDRESS0_LOW__ADDR (GBE__GBE0__BASE_ADDR + 0x304)
#define GBE__GBE0__MAC_ADDRESS0_LOW__NUM  0x1

#define GBE__GBE0__MAC_ADDRESS0_LOW__ADDRLO__SHIFT    0

#define GBE__GBE0__MAC_ADDRESS0_LOW__ADDRLO__MASK    0xffffffff

#define GBE__GBE0__MAC_ADDRESS0_LOW__ADDRLO__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MMC_Control
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned cntrst : 1;
        unsigned cntstopro : 1;
        unsigned rstonrd : 1;
        unsigned cntfreez : 1;
        unsigned cntprst : 1;
        unsigned cntprstlvl : 1;
        unsigned reserved_7_6 : 2;
        unsigned ucdbc : 1;
        unsigned reserved_31_9 : 23;
    };
    unsigned reg;
} GBE__MMC_Control__ACC_T;

#define GBE__GBE0__MMC_CONTROL__ADDR (GBE__GBE0__BASE_ADDR + 0x700)
#define GBE__GBE0__MMC_CONTROL__NUM  0x1

#define GBE__GBE0__MMC_CONTROL__CNTRST__SHIFT    0
#define GBE__GBE0__MMC_CONTROL__CNTSTOPRO__SHIFT    1
#define GBE__GBE0__MMC_CONTROL__RSTONRD__SHIFT    2
#define GBE__GBE0__MMC_CONTROL__CNTFREEZ__SHIFT    3
#define GBE__GBE0__MMC_CONTROL__CNTPRST__SHIFT    4
#define GBE__GBE0__MMC_CONTROL__CNTPRSTLVL__SHIFT    5
#define GBE__GBE0__MMC_CONTROL__RESERVED_7_6__SHIFT    6
#define GBE__GBE0__MMC_CONTROL__UCDBC__SHIFT    8
#define GBE__GBE0__MMC_CONTROL__RESERVED_31_9__SHIFT    9

#define GBE__GBE0__MMC_CONTROL__CNTRST__MASK    0x00000001
#define GBE__GBE0__MMC_CONTROL__CNTSTOPRO__MASK    0x00000002
#define GBE__GBE0__MMC_CONTROL__RSTONRD__MASK    0x00000004
#define GBE__GBE0__MMC_CONTROL__CNTFREEZ__MASK    0x00000008
#define GBE__GBE0__MMC_CONTROL__CNTPRST__MASK    0x00000010
#define GBE__GBE0__MMC_CONTROL__CNTPRSTLVL__MASK    0x00000020
#define GBE__GBE0__MMC_CONTROL__RESERVED_7_6__MASK    0x000000c0
#define GBE__GBE0__MMC_CONTROL__UCDBC__MASK    0x00000100
#define GBE__GBE0__MMC_CONTROL__RESERVED_31_9__MASK    0xfffffe00

#define GBE__GBE0__MMC_CONTROL__CNTRST__POR_VALUE    0x0
#define GBE__GBE0__MMC_CONTROL__CNTSTOPRO__POR_VALUE    0x0
#define GBE__GBE0__MMC_CONTROL__RSTONRD__POR_VALUE    0x0
#define GBE__GBE0__MMC_CONTROL__CNTFREEZ__POR_VALUE    0x0
#define GBE__GBE0__MMC_CONTROL__CNTPRST__POR_VALUE    0x0
#define GBE__GBE0__MMC_CONTROL__CNTPRSTLVL__POR_VALUE    0x0
#define GBE__GBE0__MMC_CONTROL__RESERVED_7_6__POR_VALUE    0x0
#define GBE__GBE0__MMC_CONTROL__UCDBC__POR_VALUE    0x0
#define GBE__GBE0__MMC_CONTROL__RESERVED_31_9__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MMC_Rx_Interrupt
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxgbpktis : 1;
        unsigned rxgboctis : 1;
        unsigned rxgoctis : 1;
        unsigned rxbcgpis : 1;
        unsigned rxmcgpis : 1;
        unsigned rxcrcerpis : 1;
        unsigned rxalgnerpis : 1;
        unsigned rxruntpis : 1;
        unsigned rxjaberpis : 1;
        unsigned rxusizegpis : 1;
        unsigned rxosizegpis : 1;
        unsigned rx64octgbpis : 1;
        unsigned rx65t127octgbpis : 1;
        unsigned rx128t255octgbpis : 1;
        unsigned rx256t511octgbpis : 1;
        unsigned rx512t1023octgbpis : 1;
        unsigned rx1024tmaxoctgbpis : 1;
        unsigned rxucgpis : 1;
        unsigned rxlenerpis : 1;
        unsigned rxorangepis : 1;
        unsigned rxpauspis : 1;
        unsigned rxfovpis : 1;
        unsigned rxvlangbpis : 1;
        unsigned rxwdogpis : 1;
        unsigned rxrcverrpis : 1;
        unsigned rxctrlpis : 1;
        unsigned reserved_31_26 : 6;
    };
    unsigned reg;
} GBE__MMC_Rx_Interrupt__ACC_T;

#define GBE__GBE0__MMC_RX_INTERRUPT__ADDR (GBE__GBE0__BASE_ADDR + 0x704)
#define GBE__GBE0__MMC_RX_INTERRUPT__NUM  0x1

#define GBE__GBE0__MMC_RX_INTERRUPT__RXGBPKTIS__SHIFT    0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXGBOCTIS__SHIFT    1
#define GBE__GBE0__MMC_RX_INTERRUPT__RXGOCTIS__SHIFT    2
#define GBE__GBE0__MMC_RX_INTERRUPT__RXBCGPIS__SHIFT    3
#define GBE__GBE0__MMC_RX_INTERRUPT__RXMCGPIS__SHIFT    4
#define GBE__GBE0__MMC_RX_INTERRUPT__RXCRCERPIS__SHIFT    5
#define GBE__GBE0__MMC_RX_INTERRUPT__RXALGNERPIS__SHIFT    6
#define GBE__GBE0__MMC_RX_INTERRUPT__RXRUNTPIS__SHIFT    7
#define GBE__GBE0__MMC_RX_INTERRUPT__RXJABERPIS__SHIFT    8
#define GBE__GBE0__MMC_RX_INTERRUPT__RXUSIZEGPIS__SHIFT    9
#define GBE__GBE0__MMC_RX_INTERRUPT__RXOSIZEGPIS__SHIFT    10
#define GBE__GBE0__MMC_RX_INTERRUPT__RX64OCTGBPIS__SHIFT    11
#define GBE__GBE0__MMC_RX_INTERRUPT__RX65T127OCTGBPIS__SHIFT    12
#define GBE__GBE0__MMC_RX_INTERRUPT__RX128T255OCTGBPIS__SHIFT    13
#define GBE__GBE0__MMC_RX_INTERRUPT__RX256T511OCTGBPIS__SHIFT    14
#define GBE__GBE0__MMC_RX_INTERRUPT__RX512T1023OCTGBPIS__SHIFT    15
#define GBE__GBE0__MMC_RX_INTERRUPT__RX1024TMAXOCTGBPIS__SHIFT    16
#define GBE__GBE0__MMC_RX_INTERRUPT__RXUCGPIS__SHIFT    17
#define GBE__GBE0__MMC_RX_INTERRUPT__RXLENERPIS__SHIFT    18
#define GBE__GBE0__MMC_RX_INTERRUPT__RXORANGEPIS__SHIFT    19
#define GBE__GBE0__MMC_RX_INTERRUPT__RXPAUSPIS__SHIFT    20
#define GBE__GBE0__MMC_RX_INTERRUPT__RXFOVPIS__SHIFT    21
#define GBE__GBE0__MMC_RX_INTERRUPT__RXVLANGBPIS__SHIFT    22
#define GBE__GBE0__MMC_RX_INTERRUPT__RXWDOGPIS__SHIFT    23
#define GBE__GBE0__MMC_RX_INTERRUPT__RXRCVERRPIS__SHIFT    24
#define GBE__GBE0__MMC_RX_INTERRUPT__RXCTRLPIS__SHIFT    25
#define GBE__GBE0__MMC_RX_INTERRUPT__RESERVED_31_26__SHIFT    26

#define GBE__GBE0__MMC_RX_INTERRUPT__RXGBPKTIS__MASK    0x00000001
#define GBE__GBE0__MMC_RX_INTERRUPT__RXGBOCTIS__MASK    0x00000002
#define GBE__GBE0__MMC_RX_INTERRUPT__RXGOCTIS__MASK    0x00000004
#define GBE__GBE0__MMC_RX_INTERRUPT__RXBCGPIS__MASK    0x00000008
#define GBE__GBE0__MMC_RX_INTERRUPT__RXMCGPIS__MASK    0x00000010
#define GBE__GBE0__MMC_RX_INTERRUPT__RXCRCERPIS__MASK    0x00000020
#define GBE__GBE0__MMC_RX_INTERRUPT__RXALGNERPIS__MASK    0x00000040
#define GBE__GBE0__MMC_RX_INTERRUPT__RXRUNTPIS__MASK    0x00000080
#define GBE__GBE0__MMC_RX_INTERRUPT__RXJABERPIS__MASK    0x00000100
#define GBE__GBE0__MMC_RX_INTERRUPT__RXUSIZEGPIS__MASK    0x00000200
#define GBE__GBE0__MMC_RX_INTERRUPT__RXOSIZEGPIS__MASK    0x00000400
#define GBE__GBE0__MMC_RX_INTERRUPT__RX64OCTGBPIS__MASK    0x00000800
#define GBE__GBE0__MMC_RX_INTERRUPT__RX65T127OCTGBPIS__MASK    0x00001000
#define GBE__GBE0__MMC_RX_INTERRUPT__RX128T255OCTGBPIS__MASK    0x00002000
#define GBE__GBE0__MMC_RX_INTERRUPT__RX256T511OCTGBPIS__MASK    0x00004000
#define GBE__GBE0__MMC_RX_INTERRUPT__RX512T1023OCTGBPIS__MASK    0x00008000
#define GBE__GBE0__MMC_RX_INTERRUPT__RX1024TMAXOCTGBPIS__MASK    0x00010000
#define GBE__GBE0__MMC_RX_INTERRUPT__RXUCGPIS__MASK    0x00020000
#define GBE__GBE0__MMC_RX_INTERRUPT__RXLENERPIS__MASK    0x00040000
#define GBE__GBE0__MMC_RX_INTERRUPT__RXORANGEPIS__MASK    0x00080000
#define GBE__GBE0__MMC_RX_INTERRUPT__RXPAUSPIS__MASK    0x00100000
#define GBE__GBE0__MMC_RX_INTERRUPT__RXFOVPIS__MASK    0x00200000
#define GBE__GBE0__MMC_RX_INTERRUPT__RXVLANGBPIS__MASK    0x00400000
#define GBE__GBE0__MMC_RX_INTERRUPT__RXWDOGPIS__MASK    0x00800000
#define GBE__GBE0__MMC_RX_INTERRUPT__RXRCVERRPIS__MASK    0x01000000
#define GBE__GBE0__MMC_RX_INTERRUPT__RXCTRLPIS__MASK    0x02000000
#define GBE__GBE0__MMC_RX_INTERRUPT__RESERVED_31_26__MASK    0xfc000000

#define GBE__GBE0__MMC_RX_INTERRUPT__RXGBPKTIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXGBOCTIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXGOCTIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXBCGPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXMCGPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXCRCERPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXALGNERPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXRUNTPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXJABERPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXUSIZEGPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXOSIZEGPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RX64OCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RX65T127OCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RX128T255OCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RX256T511OCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RX512T1023OCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RX1024TMAXOCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXUCGPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXLENERPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXORANGEPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXPAUSPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXFOVPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXVLANGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXWDOGPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXRCVERRPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RXCTRLPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT__RESERVED_31_26__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MMC_Tx_Interrupt
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txgboctis : 1;
        unsigned txgbpktis : 1;
        unsigned txbcgpis : 1;
        unsigned txmcgpis : 1;
        unsigned tx64octgbpis : 1;
        unsigned tx65t127octgbpis : 1;
        unsigned tx128t255octgbpis : 1;
        unsigned tx256t511octgbpis : 1;
        unsigned tx512t1023octgbpis : 1;
        unsigned tx1024tmaxoctgbpis : 1;
        unsigned txucgbpis : 1;
        unsigned txmcgbpis : 1;
        unsigned txbcgbpis : 1;
        unsigned txuflowerpis : 1;
        unsigned reserved_19_14 : 6;
        unsigned txgoctis : 1;
        unsigned txgpktis : 1;
        unsigned reserved_22 : 1;
        unsigned txpauspis : 1;
        unsigned txvlangpis : 1;
        unsigned txosizegpis : 1;
        unsigned reserved_31_26 : 6;
    };
    unsigned reg;
} GBE__MMC_Tx_Interrupt__ACC_T;

#define GBE__GBE0__MMC_TX_INTERRUPT__ADDR (GBE__GBE0__BASE_ADDR + 0x708)
#define GBE__GBE0__MMC_TX_INTERRUPT__NUM  0x1

#define GBE__GBE0__MMC_TX_INTERRUPT__TXGBOCTIS__SHIFT    0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXGBPKTIS__SHIFT    1
#define GBE__GBE0__MMC_TX_INTERRUPT__TXBCGPIS__SHIFT    2
#define GBE__GBE0__MMC_TX_INTERRUPT__TXMCGPIS__SHIFT    3
#define GBE__GBE0__MMC_TX_INTERRUPT__TX64OCTGBPIS__SHIFT    4
#define GBE__GBE0__MMC_TX_INTERRUPT__TX65T127OCTGBPIS__SHIFT    5
#define GBE__GBE0__MMC_TX_INTERRUPT__TX128T255OCTGBPIS__SHIFT    6
#define GBE__GBE0__MMC_TX_INTERRUPT__TX256T511OCTGBPIS__SHIFT    7
#define GBE__GBE0__MMC_TX_INTERRUPT__TX512T1023OCTGBPIS__SHIFT    8
#define GBE__GBE0__MMC_TX_INTERRUPT__TX1024TMAXOCTGBPIS__SHIFT    9
#define GBE__GBE0__MMC_TX_INTERRUPT__TXUCGBPIS__SHIFT    10
#define GBE__GBE0__MMC_TX_INTERRUPT__TXMCGBPIS__SHIFT    11
#define GBE__GBE0__MMC_TX_INTERRUPT__TXBCGBPIS__SHIFT    12
#define GBE__GBE0__MMC_TX_INTERRUPT__TXUFLOWERPIS__SHIFT    13
#define GBE__GBE0__MMC_TX_INTERRUPT__RESERVED_19_14__SHIFT    14
#define GBE__GBE0__MMC_TX_INTERRUPT__TXGOCTIS__SHIFT    20
#define GBE__GBE0__MMC_TX_INTERRUPT__TXGPKTIS__SHIFT    21
#define GBE__GBE0__MMC_TX_INTERRUPT__RESERVED_22__SHIFT    22
#define GBE__GBE0__MMC_TX_INTERRUPT__TXPAUSPIS__SHIFT    23
#define GBE__GBE0__MMC_TX_INTERRUPT__TXVLANGPIS__SHIFT    24
#define GBE__GBE0__MMC_TX_INTERRUPT__TXOSIZEGPIS__SHIFT    25
#define GBE__GBE0__MMC_TX_INTERRUPT__RESERVED_31_26__SHIFT    26

#define GBE__GBE0__MMC_TX_INTERRUPT__TXGBOCTIS__MASK    0x00000001
#define GBE__GBE0__MMC_TX_INTERRUPT__TXGBPKTIS__MASK    0x00000002
#define GBE__GBE0__MMC_TX_INTERRUPT__TXBCGPIS__MASK    0x00000004
#define GBE__GBE0__MMC_TX_INTERRUPT__TXMCGPIS__MASK    0x00000008
#define GBE__GBE0__MMC_TX_INTERRUPT__TX64OCTGBPIS__MASK    0x00000010
#define GBE__GBE0__MMC_TX_INTERRUPT__TX65T127OCTGBPIS__MASK    0x00000020
#define GBE__GBE0__MMC_TX_INTERRUPT__TX128T255OCTGBPIS__MASK    0x00000040
#define GBE__GBE0__MMC_TX_INTERRUPT__TX256T511OCTGBPIS__MASK    0x00000080
#define GBE__GBE0__MMC_TX_INTERRUPT__TX512T1023OCTGBPIS__MASK    0x00000100
#define GBE__GBE0__MMC_TX_INTERRUPT__TX1024TMAXOCTGBPIS__MASK    0x00000200
#define GBE__GBE0__MMC_TX_INTERRUPT__TXUCGBPIS__MASK    0x00000400
#define GBE__GBE0__MMC_TX_INTERRUPT__TXMCGBPIS__MASK    0x00000800
#define GBE__GBE0__MMC_TX_INTERRUPT__TXBCGBPIS__MASK    0x00001000
#define GBE__GBE0__MMC_TX_INTERRUPT__TXUFLOWERPIS__MASK    0x00002000
#define GBE__GBE0__MMC_TX_INTERRUPT__RESERVED_19_14__MASK    0x000fc000
#define GBE__GBE0__MMC_TX_INTERRUPT__TXGOCTIS__MASK    0x00100000
#define GBE__GBE0__MMC_TX_INTERRUPT__TXGPKTIS__MASK    0x00200000
#define GBE__GBE0__MMC_TX_INTERRUPT__RESERVED_22__MASK    0x00400000
#define GBE__GBE0__MMC_TX_INTERRUPT__TXPAUSPIS__MASK    0x00800000
#define GBE__GBE0__MMC_TX_INTERRUPT__TXVLANGPIS__MASK    0x01000000
#define GBE__GBE0__MMC_TX_INTERRUPT__TXOSIZEGPIS__MASK    0x02000000
#define GBE__GBE0__MMC_TX_INTERRUPT__RESERVED_31_26__MASK    0xfc000000

#define GBE__GBE0__MMC_TX_INTERRUPT__TXGBOCTIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXGBPKTIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXBCGPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXMCGPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TX64OCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TX65T127OCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TX128T255OCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TX256T511OCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TX512T1023OCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TX1024TMAXOCTGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXUCGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXMCGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXBCGBPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXUFLOWERPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__RESERVED_19_14__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXGOCTIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXGPKTIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__RESERVED_22__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXPAUSPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXVLANGPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__TXOSIZEGPIS__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT__RESERVED_31_26__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MMC_Rx_Interrupt_Mask
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxgbpktim : 1;
        unsigned rxgboctim : 1;
        unsigned rxgoctim : 1;
        unsigned rxbcgpim : 1;
        unsigned rxmcgpim : 1;
        unsigned rxcrcerpim : 1;
        unsigned rxalgnerpim : 1;
        unsigned rxruntpim : 1;
        unsigned rxjaberpim : 1;
        unsigned rxusizegpim : 1;
        unsigned rxosizegpim : 1;
        unsigned rx64octgbpim : 1;
        unsigned rx65t127octgbpim : 1;
        unsigned rx128t255octgbpim : 1;
        unsigned rx256t511octgbpim : 1;
        unsigned rx512t1023octgbpim : 1;
        unsigned rx1024tmaxoctgbpim : 1;
        unsigned rxucgpim : 1;
        unsigned rxlenerpim : 1;
        unsigned rxorangepim : 1;
        unsigned rxpauspim : 1;
        unsigned rxfovpim : 1;
        unsigned rxvlangbpim : 1;
        unsigned rxwdogpim : 1;
        unsigned rxrcverrpim : 1;
        unsigned rxctrlpim : 1;
        unsigned reserved_31_26 : 6;
    };
    unsigned reg;
} GBE__MMC_Rx_Interrupt_Mask__ACC_T;

#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__ADDR (GBE__GBE0__BASE_ADDR + 0x70c)
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__NUM  0x1

#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXGBPKTIM__SHIFT    0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXGBOCTIM__SHIFT    1
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXGOCTIM__SHIFT    2
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXBCGPIM__SHIFT    3
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXMCGPIM__SHIFT    4
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXCRCERPIM__SHIFT    5
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXALGNERPIM__SHIFT    6
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXRUNTPIM__SHIFT    7
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXJABERPIM__SHIFT    8
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXUSIZEGPIM__SHIFT    9
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXOSIZEGPIM__SHIFT    10
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX64OCTGBPIM__SHIFT    11
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX65T127OCTGBPIM__SHIFT    12
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX128T255OCTGBPIM__SHIFT    13
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX256T511OCTGBPIM__SHIFT    14
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX512T1023OCTGBPIM__SHIFT    15
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX1024TMAXOCTGBPIM__SHIFT    16
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXUCGPIM__SHIFT    17
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXLENERPIM__SHIFT    18
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXORANGEPIM__SHIFT    19
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXPAUSPIM__SHIFT    20
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXFOVPIM__SHIFT    21
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXVLANGBPIM__SHIFT    22
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXWDOGPIM__SHIFT    23
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXRCVERRPIM__SHIFT    24
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXCTRLPIM__SHIFT    25
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RESERVED_31_26__SHIFT    26

#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXGBPKTIM__MASK    0x00000001
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXGBOCTIM__MASK    0x00000002
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXGOCTIM__MASK    0x00000004
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXBCGPIM__MASK    0x00000008
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXMCGPIM__MASK    0x00000010
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXCRCERPIM__MASK    0x00000020
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXALGNERPIM__MASK    0x00000040
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXRUNTPIM__MASK    0x00000080
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXJABERPIM__MASK    0x00000100
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXUSIZEGPIM__MASK    0x00000200
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXOSIZEGPIM__MASK    0x00000400
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX64OCTGBPIM__MASK    0x00000800
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX65T127OCTGBPIM__MASK    0x00001000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX128T255OCTGBPIM__MASK    0x00002000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX256T511OCTGBPIM__MASK    0x00004000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX512T1023OCTGBPIM__MASK    0x00008000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX1024TMAXOCTGBPIM__MASK    0x00010000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXUCGPIM__MASK    0x00020000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXLENERPIM__MASK    0x00040000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXORANGEPIM__MASK    0x00080000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXPAUSPIM__MASK    0x00100000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXFOVPIM__MASK    0x00200000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXVLANGBPIM__MASK    0x00400000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXWDOGPIM__MASK    0x00800000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXRCVERRPIM__MASK    0x01000000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXCTRLPIM__MASK    0x02000000
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RESERVED_31_26__MASK    0xfc000000

#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXGBPKTIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXGBOCTIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXGOCTIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXBCGPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXMCGPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXCRCERPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXALGNERPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXRUNTPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXJABERPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXUSIZEGPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXOSIZEGPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX64OCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX65T127OCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX128T255OCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX256T511OCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX512T1023OCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RX1024TMAXOCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXUCGPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXLENERPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXORANGEPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXPAUSPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXFOVPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXVLANGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXWDOGPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXRCVERRPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RXCTRLPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_RX_INTERRUPT_MASK__RESERVED_31_26__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MMC_Tx_Interrupt_Mask
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txgboctim : 1;
        unsigned txgbpktim : 1;
        unsigned txbcgpim : 1;
        unsigned txmcgpim : 1;
        unsigned tx64octgbpim : 1;
        unsigned tx65t127octgbpim : 1;
        unsigned tx128t255octgbpim : 1;
        unsigned tx256t511octgbpim : 1;
        unsigned tx512t1023octgbpim : 1;
        unsigned tx1024tmaxoctgbpim : 1;
        unsigned txucgbpim : 1;
        unsigned txmcgbpim : 1;
        unsigned txbcgbpim : 1;
        unsigned txuflowerpim : 1;
        unsigned reserved_19_14 : 6;
        unsigned txgoctim : 1;
        unsigned txgpktim : 1;
        unsigned reserved_22 : 1;
        unsigned txpauspim : 1;
        unsigned txvlangpim : 1;
        unsigned txosizegpim : 1;
        unsigned reserved_31_26 : 6;
    };
    unsigned reg;
} GBE__MMC_Tx_Interrupt_Mask__ACC_T;

#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__ADDR (GBE__GBE0__BASE_ADDR + 0x710)
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__NUM  0x1

#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGBOCTIM__SHIFT    0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGBPKTIM__SHIFT    1
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXBCGPIM__SHIFT    2
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXMCGPIM__SHIFT    3
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX64OCTGBPIM__SHIFT    4
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX65T127OCTGBPIM__SHIFT    5
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX128T255OCTGBPIM__SHIFT    6
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX256T511OCTGBPIM__SHIFT    7
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX512T1023OCTGBPIM__SHIFT    8
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX1024TMAXOCTGBPIM__SHIFT    9
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXUCGBPIM__SHIFT    10
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXMCGBPIM__SHIFT    11
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXBCGBPIM__SHIFT    12
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXUFLOWERPIM__SHIFT    13
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__RESERVED_19_14__SHIFT    14
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGOCTIM__SHIFT    20
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGPKTIM__SHIFT    21
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__RESERVED_22__SHIFT    22
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXPAUSPIM__SHIFT    23
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXVLANGPIM__SHIFT    24
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXOSIZEGPIM__SHIFT    25
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__RESERVED_31_26__SHIFT    26

#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGBOCTIM__MASK    0x00000001
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGBPKTIM__MASK    0x00000002
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXBCGPIM__MASK    0x00000004
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXMCGPIM__MASK    0x00000008
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX64OCTGBPIM__MASK    0x00000010
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX65T127OCTGBPIM__MASK    0x00000020
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX128T255OCTGBPIM__MASK    0x00000040
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX256T511OCTGBPIM__MASK    0x00000080
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX512T1023OCTGBPIM__MASK    0x00000100
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX1024TMAXOCTGBPIM__MASK    0x00000200
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXUCGBPIM__MASK    0x00000400
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXMCGBPIM__MASK    0x00000800
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXBCGBPIM__MASK    0x00001000
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXUFLOWERPIM__MASK    0x00002000
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__RESERVED_19_14__MASK    0x000fc000
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGOCTIM__MASK    0x00100000
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGPKTIM__MASK    0x00200000
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__RESERVED_22__MASK    0x00400000
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXPAUSPIM__MASK    0x00800000
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXVLANGPIM__MASK    0x01000000
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXOSIZEGPIM__MASK    0x02000000
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__RESERVED_31_26__MASK    0xfc000000

#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGBOCTIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGBPKTIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXBCGPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXMCGPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX64OCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX65T127OCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX128T255OCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX256T511OCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX512T1023OCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TX1024TMAXOCTGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXUCGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXMCGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXBCGBPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXUFLOWERPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__RESERVED_19_14__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGOCTIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXGPKTIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__RESERVED_22__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXPAUSPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXVLANGPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__TXOSIZEGPIM__POR_VALUE    0x0
#define GBE__GBE0__MMC_TX_INTERRUPT_MASK__RESERVED_31_26__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_Octet_Count_Good_Bad
// Tx Octet Count Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txoctgb : 32;
    };
    unsigned reg;
} GBE__Tx_Octet_Count_Good_Bad__ACC_T;

#define GBE__GBE0__TX_OCTET_COUNT_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x714)
#define GBE__GBE0__TX_OCTET_COUNT_GOOD_BAD__NUM  0x1

#define GBE__GBE0__TX_OCTET_COUNT_GOOD_BAD__TXOCTGB__SHIFT    0

#define GBE__GBE0__TX_OCTET_COUNT_GOOD_BAD__TXOCTGB__MASK    0xffffffff

#define GBE__GBE0__TX_OCTET_COUNT_GOOD_BAD__TXOCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_Packet_Count_Good_Bad
// Tx Packet Count Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txpktgb : 32;
    };
    unsigned reg;
} GBE__Tx_Packet_Count_Good_Bad__ACC_T;

#define GBE__GBE0__TX_PACKET_COUNT_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x718)
#define GBE__GBE0__TX_PACKET_COUNT_GOOD_BAD__NUM  0x1

#define GBE__GBE0__TX_PACKET_COUNT_GOOD_BAD__TXPKTGB__SHIFT    0

#define GBE__GBE0__TX_PACKET_COUNT_GOOD_BAD__TXPKTGB__MASK    0xffffffff

#define GBE__GBE0__TX_PACKET_COUNT_GOOD_BAD__TXPKTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_Broadcast_Packets_Good
// Tx Broadcast Packets Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txbcastg : 32;
    };
    unsigned reg;
} GBE__Tx_Broadcast_Packets_Good__ACC_T;

#define GBE__GBE0__TX_BROADCAST_PACKETS_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x71c)
#define GBE__GBE0__TX_BROADCAST_PACKETS_GOOD__NUM  0x1

#define GBE__GBE0__TX_BROADCAST_PACKETS_GOOD__TXBCASTG__SHIFT    0

#define GBE__GBE0__TX_BROADCAST_PACKETS_GOOD__TXBCASTG__MASK    0xffffffff

#define GBE__GBE0__TX_BROADCAST_PACKETS_GOOD__TXBCASTG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_Multicast_Packets_Good
// Tx Multicast Packets Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txmcastg : 32;
    };
    unsigned reg;
} GBE__Tx_Multicast_Packets_Good__ACC_T;

#define GBE__GBE0__TX_MULTICAST_PACKETS_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x720)
#define GBE__GBE0__TX_MULTICAST_PACKETS_GOOD__NUM  0x1

#define GBE__GBE0__TX_MULTICAST_PACKETS_GOOD__TXMCASTG__SHIFT    0

#define GBE__GBE0__TX_MULTICAST_PACKETS_GOOD__TXMCASTG__MASK    0xffffffff

#define GBE__GBE0__TX_MULTICAST_PACKETS_GOOD__TXMCASTG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_64Octets_Packets_Good_Bad
// Tx 64Octets Packets Good_Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tx64octgb : 32;
    };
    unsigned reg;
} GBE__Tx_64Octets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__TX_64OCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x724)
#define GBE__GBE0__TX_64OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__TX_64OCTETS_PACKETS_GOOD_BAD__TX64OCTGB__SHIFT    0

#define GBE__GBE0__TX_64OCTETS_PACKETS_GOOD_BAD__TX64OCTGB__MASK    0xffffffff

#define GBE__GBE0__TX_64OCTETS_PACKETS_GOOD_BAD__TX64OCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_65To127Octets_Packets_Good_Bad
// Tx 65To127Octets Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tx65_127octgb : 32;
    };
    unsigned reg;
} GBE__Tx_65To127Octets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__TX_65TO127OCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x728)
#define GBE__GBE0__TX_65TO127OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__TX_65TO127OCTETS_PACKETS_GOOD_BAD__TX65_127OCTGB__SHIFT    0

#define GBE__GBE0__TX_65TO127OCTETS_PACKETS_GOOD_BAD__TX65_127OCTGB__MASK    0xffffffff

#define GBE__GBE0__TX_65TO127OCTETS_PACKETS_GOOD_BAD__TX65_127OCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_128To255Octets_Packets_Good_Bad
// Tx 128To255Octets Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tx128_255octgb : 32;
    };
    unsigned reg;
} GBE__Tx_128To255Octets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__TX_128TO255OCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x72c)
#define GBE__GBE0__TX_128TO255OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__TX_128TO255OCTETS_PACKETS_GOOD_BAD__TX128_255OCTGB__SHIFT    0

#define GBE__GBE0__TX_128TO255OCTETS_PACKETS_GOOD_BAD__TX128_255OCTGB__MASK    0xffffffff

#define GBE__GBE0__TX_128TO255OCTETS_PACKETS_GOOD_BAD__TX128_255OCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_256To511Octets_Packets_Good_Bad
// Tx 256To511Octets Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tx256_511octgb : 32;
    };
    unsigned reg;
} GBE__Tx_256To511Octets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__TX_256TO511OCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x730)
#define GBE__GBE0__TX_256TO511OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__TX_256TO511OCTETS_PACKETS_GOOD_BAD__TX256_511OCTGB__SHIFT    0

#define GBE__GBE0__TX_256TO511OCTETS_PACKETS_GOOD_BAD__TX256_511OCTGB__MASK    0xffffffff

#define GBE__GBE0__TX_256TO511OCTETS_PACKETS_GOOD_BAD__TX256_511OCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_512To1023Octets_Packets_Good_Bad
//
// Tx 512To1023Octets Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tx512_1023octgb : 32;
    };
    unsigned reg;
} GBE__Tx_512To1023Octets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__TX_512TO1023OCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x734)
#define GBE__GBE0__TX_512TO1023OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__TX_512TO1023OCTETS_PACKETS_GOOD_BAD__TX512_1023OCTGB__SHIFT    0

#define GBE__GBE0__TX_512TO1023OCTETS_PACKETS_GOOD_BAD__TX512_1023OCTGB__MASK    0xffffffff

#define GBE__GBE0__TX_512TO1023OCTETS_PACKETS_GOOD_BAD__TX512_1023OCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_1024ToMaxOctets_Packets_Good_Bad
// Tx 1024ToMaxOctets Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tx1024_maxoctgb : 32;
    };
    unsigned reg;
} GBE__Tx_1024ToMaxOctets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__TX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x738)
#define GBE__GBE0__TX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__TX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__TX1024_MAXOCTGB__SHIFT    0

#define GBE__GBE0__TX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__TX1024_MAXOCTGB__MASK    0xffffffff

#define GBE__GBE0__TX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__TX1024_MAXOCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_Unicast_Packets_Good_Bad
//
// Tx Unicast Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txucastgb : 32;
    };
    unsigned reg;
} GBE__Tx_Unicast_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__TX_UNICAST_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x73c)
#define GBE__GBE0__TX_UNICAST_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__TX_UNICAST_PACKETS_GOOD_BAD__TXUCASTGB__SHIFT    0

#define GBE__GBE0__TX_UNICAST_PACKETS_GOOD_BAD__TXUCASTGB__MASK    0xffffffff

#define GBE__GBE0__TX_UNICAST_PACKETS_GOOD_BAD__TXUCASTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_Multicast_Packets_Good_Bad
//
// Tx Multicast Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txmcastgb : 32;
    };
    unsigned reg;
} GBE__Tx_Multicast_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__TX_MULTICAST_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x740)
#define GBE__GBE0__TX_MULTICAST_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__TX_MULTICAST_PACKETS_GOOD_BAD__TXMCASTGB__SHIFT    0

#define GBE__GBE0__TX_MULTICAST_PACKETS_GOOD_BAD__TXMCASTGB__MASK    0xffffffff

#define GBE__GBE0__TX_MULTICAST_PACKETS_GOOD_BAD__TXMCASTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_Broadcast_Packets_Good_Bad
// Tx Broadcast Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txbcastgb : 32;
    };
    unsigned reg;
} GBE__Tx_Broadcast_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__TX_BROADCAST_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x744)
#define GBE__GBE0__TX_BROADCAST_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__TX_BROADCAST_PACKETS_GOOD_BAD__TXBCASTGB__SHIFT    0

#define GBE__GBE0__TX_BROADCAST_PACKETS_GOOD_BAD__TXBCASTGB__MASK    0xffffffff

#define GBE__GBE0__TX_BROADCAST_PACKETS_GOOD_BAD__TXBCASTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_Underflow_Error_Packets
// Tx Underflow Error Packets
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txundrflw : 32;
    };
    unsigned reg;
} GBE__Tx_Underflow_Error_Packets__ACC_T;

#define GBE__GBE0__TX_UNDERFLOW_ERROR_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x748)
#define GBE__GBE0__TX_UNDERFLOW_ERROR_PACKETS__NUM  0x1

#define GBE__GBE0__TX_UNDERFLOW_ERROR_PACKETS__TXUNDRFLW__SHIFT    0

#define GBE__GBE0__TX_UNDERFLOW_ERROR_PACKETS__TXUNDRFLW__MASK    0xffffffff

#define GBE__GBE0__TX_UNDERFLOW_ERROR_PACKETS__TXUNDRFLW__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_Octet_Count_Good
// Tx Octet Count Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txoctg : 32;
    };
    unsigned reg;
} GBE__Tx_Octet_Count_Good__ACC_T;

#define GBE__GBE0__TX_OCTET_COUNT_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x764)
#define GBE__GBE0__TX_OCTET_COUNT_GOOD__NUM  0x1

#define GBE__GBE0__TX_OCTET_COUNT_GOOD__TXOCTG__SHIFT    0

#define GBE__GBE0__TX_OCTET_COUNT_GOOD__TXOCTG__MASK    0xffffffff

#define GBE__GBE0__TX_OCTET_COUNT_GOOD__TXOCTG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_Packet_Count_Good
//
// Tx Packet Count Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txpktg : 32;
    };
    unsigned reg;
} GBE__Tx_Packet_Count_Good__ACC_T;

#define GBE__GBE0__TX_PACKET_COUNT_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x768)
#define GBE__GBE0__TX_PACKET_COUNT_GOOD__NUM  0x1

#define GBE__GBE0__TX_PACKET_COUNT_GOOD__TXPKTG__SHIFT    0

#define GBE__GBE0__TX_PACKET_COUNT_GOOD__TXPKTG__MASK    0xffffffff

#define GBE__GBE0__TX_PACKET_COUNT_GOOD__TXPKTG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_Pause_Packets
// Tx Pause Packets
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txpause : 32;
    };
    unsigned reg;
} GBE__Tx_Pause_Packets__ACC_T;

#define GBE__GBE0__TX_PAUSE_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x770)
#define GBE__GBE0__TX_PAUSE_PACKETS__NUM  0x1

#define GBE__GBE0__TX_PAUSE_PACKETS__TXPAUSE__SHIFT    0

#define GBE__GBE0__TX_PAUSE_PACKETS__TXPAUSE__MASK    0xffffffff

#define GBE__GBE0__TX_PAUSE_PACKETS__TXPAUSE__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_VLAN_Packets_Good
// Tx VLAN Packets Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txvlang : 32;
    };
    unsigned reg;
} GBE__Tx_VLAN_Packets_Good__ACC_T;

#define GBE__GBE0__TX_VLAN_PACKETS_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x774)
#define GBE__GBE0__TX_VLAN_PACKETS_GOOD__NUM  0x1

#define GBE__GBE0__TX_VLAN_PACKETS_GOOD__TXVLANG__SHIFT    0

#define GBE__GBE0__TX_VLAN_PACKETS_GOOD__TXVLANG__MASK    0xffffffff

#define GBE__GBE0__TX_VLAN_PACKETS_GOOD__TXVLANG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Tx_OSize_Packets_Good
// Tx OSize Packets Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txosizg : 32;
    };
    unsigned reg;
} GBE__Tx_OSize_Packets_Good__ACC_T;

#define GBE__GBE0__TX_OSIZE_PACKETS_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x778)
#define GBE__GBE0__TX_OSIZE_PACKETS_GOOD__NUM  0x1

#define GBE__GBE0__TX_OSIZE_PACKETS_GOOD__TXOSIZG__SHIFT    0

#define GBE__GBE0__TX_OSIZE_PACKETS_GOOD__TXOSIZG__MASK    0xffffffff

#define GBE__GBE0__TX_OSIZE_PACKETS_GOOD__TXOSIZG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Packets_Count_Good_Bad
// Rx Packets Count Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxpktgb : 32;
    };
    unsigned reg;
} GBE__Rx_Packets_Count_Good_Bad__ACC_T;

#define GBE__GBE0__RX_PACKETS_COUNT_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x780)
#define GBE__GBE0__RX_PACKETS_COUNT_GOOD_BAD__NUM  0x1

#define GBE__GBE0__RX_PACKETS_COUNT_GOOD_BAD__RXPKTGB__SHIFT    0

#define GBE__GBE0__RX_PACKETS_COUNT_GOOD_BAD__RXPKTGB__MASK    0xffffffff

#define GBE__GBE0__RX_PACKETS_COUNT_GOOD_BAD__RXPKTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Octet_Count_Good_Bad
//
// Rx Octet Count Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxoctgb : 32;
    };
    unsigned reg;
} GBE__Rx_Octet_Count_Good_Bad__ACC_T;

#define GBE__GBE0__RX_OCTET_COUNT_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x784)
#define GBE__GBE0__RX_OCTET_COUNT_GOOD_BAD__NUM  0x1

#define GBE__GBE0__RX_OCTET_COUNT_GOOD_BAD__RXOCTGB__SHIFT    0

#define GBE__GBE0__RX_OCTET_COUNT_GOOD_BAD__RXOCTGB__MASK    0xffffffff

#define GBE__GBE0__RX_OCTET_COUNT_GOOD_BAD__RXOCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Octet_Count_Good
// Rx Octet Count Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxoctg : 32;
    };
    unsigned reg;
} GBE__Rx_Octet_Count_Good__ACC_T;

#define GBE__GBE0__RX_OCTET_COUNT_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x788)
#define GBE__GBE0__RX_OCTET_COUNT_GOOD__NUM  0x1

#define GBE__GBE0__RX_OCTET_COUNT_GOOD__RXOCTG__SHIFT    0

#define GBE__GBE0__RX_OCTET_COUNT_GOOD__RXOCTG__MASK    0xffffffff

#define GBE__GBE0__RX_OCTET_COUNT_GOOD__RXOCTG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Broadcast_Packets_Good
// Rx Broadcast Packets Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxbcastg : 32;
    };
    unsigned reg;
} GBE__Rx_Broadcast_Packets_Good__ACC_T;

#define GBE__GBE0__RX_BROADCAST_PACKETS_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x78c)
#define GBE__GBE0__RX_BROADCAST_PACKETS_GOOD__NUM  0x1

#define GBE__GBE0__RX_BROADCAST_PACKETS_GOOD__RXBCASTG__SHIFT    0

#define GBE__GBE0__RX_BROADCAST_PACKETS_GOOD__RXBCASTG__MASK    0xffffffff

#define GBE__GBE0__RX_BROADCAST_PACKETS_GOOD__RXBCASTG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Multicast_Packets_Good
//
// Rx Multicast Packets Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxmcastg : 32;
    };
    unsigned reg;
} GBE__Rx_Multicast_Packets_Good__ACC_T;

#define GBE__GBE0__RX_MULTICAST_PACKETS_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x790)
#define GBE__GBE0__RX_MULTICAST_PACKETS_GOOD__NUM  0x1

#define GBE__GBE0__RX_MULTICAST_PACKETS_GOOD__RXMCASTG__SHIFT    0

#define GBE__GBE0__RX_MULTICAST_PACKETS_GOOD__RXMCASTG__MASK    0xffffffff

#define GBE__GBE0__RX_MULTICAST_PACKETS_GOOD__RXMCASTG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_CRC_Error_Packets
// Rx CRC Error Packets
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxcrcerr : 32;
    };
    unsigned reg;
} GBE__Rx_CRC_Error_Packets__ACC_T;

#define GBE__GBE0__RX_CRC_ERROR_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x794)
#define GBE__GBE0__RX_CRC_ERROR_PACKETS__NUM  0x1

#define GBE__GBE0__RX_CRC_ERROR_PACKETS__RXCRCERR__SHIFT    0

#define GBE__GBE0__RX_CRC_ERROR_PACKETS__RXCRCERR__MASK    0xffffffff

#define GBE__GBE0__RX_CRC_ERROR_PACKETS__RXCRCERR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Alignment_Error_Packets
// Rx Alignment Error Packets
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxalgnerr : 32;
    };
    unsigned reg;
} GBE__Rx_Alignment_Error_Packets__ACC_T;

#define GBE__GBE0__RX_ALIGNMENT_ERROR_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x798)
#define GBE__GBE0__RX_ALIGNMENT_ERROR_PACKETS__NUM  0x1

#define GBE__GBE0__RX_ALIGNMENT_ERROR_PACKETS__RXALGNERR__SHIFT    0

#define GBE__GBE0__RX_ALIGNMENT_ERROR_PACKETS__RXALGNERR__MASK    0xffffffff

#define GBE__GBE0__RX_ALIGNMENT_ERROR_PACKETS__RXALGNERR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Runt_Error_Packets
// Rx Runt Error Packets
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxrunterr : 32;
    };
    unsigned reg;
} GBE__Rx_Runt_Error_Packets__ACC_T;

#define GBE__GBE0__RX_RUNT_ERROR_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x79c)
#define GBE__GBE0__RX_RUNT_ERROR_PACKETS__NUM  0x1

#define GBE__GBE0__RX_RUNT_ERROR_PACKETS__RXRUNTERR__SHIFT    0

#define GBE__GBE0__RX_RUNT_ERROR_PACKETS__RXRUNTERR__MASK    0xffffffff

#define GBE__GBE0__RX_RUNT_ERROR_PACKETS__RXRUNTERR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Jabber_Error_Packets
// Rx Jabber Error Packets
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxjaberr : 32;
    };
    unsigned reg;
} GBE__Rx_Jabber_Error_Packets__ACC_T;

#define GBE__GBE0__RX_JABBER_ERROR_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x7a0)
#define GBE__GBE0__RX_JABBER_ERROR_PACKETS__NUM  0x1

#define GBE__GBE0__RX_JABBER_ERROR_PACKETS__RXJABERR__SHIFT    0

#define GBE__GBE0__RX_JABBER_ERROR_PACKETS__RXJABERR__MASK    0xffffffff

#define GBE__GBE0__RX_JABBER_ERROR_PACKETS__RXJABERR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Undersize_Packets_Good
// Rx Undersize Packets Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxunderszg : 32;
    };
    unsigned reg;
} GBE__Rx_Undersize_Packets_Good__ACC_T;

#define GBE__GBE0__RX_UNDERSIZE_PACKETS_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x7a4)
#define GBE__GBE0__RX_UNDERSIZE_PACKETS_GOOD__NUM  0x1

#define GBE__GBE0__RX_UNDERSIZE_PACKETS_GOOD__RXUNDERSZG__SHIFT    0

#define GBE__GBE0__RX_UNDERSIZE_PACKETS_GOOD__RXUNDERSZG__MASK    0xffffffff

#define GBE__GBE0__RX_UNDERSIZE_PACKETS_GOOD__RXUNDERSZG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Oversize_Packets_Good
// Rx Oversize Packets Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxoverszg : 32;
    };
    unsigned reg;
} GBE__Rx_Oversize_Packets_Good__ACC_T;

#define GBE__GBE0__RX_OVERSIZE_PACKETS_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x7a8)
#define GBE__GBE0__RX_OVERSIZE_PACKETS_GOOD__NUM  0x1

#define GBE__GBE0__RX_OVERSIZE_PACKETS_GOOD__RXOVERSZG__SHIFT    0

#define GBE__GBE0__RX_OVERSIZE_PACKETS_GOOD__RXOVERSZG__MASK    0xffffffff

#define GBE__GBE0__RX_OVERSIZE_PACKETS_GOOD__RXOVERSZG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_64Octets_Packets_Good_Bad
// Rx 64 Octets Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rx64octgb : 32;
    };
    unsigned reg;
} GBE__Rx_64Octets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__RX_64OCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x7ac)
#define GBE__GBE0__RX_64OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__RX_64OCTETS_PACKETS_GOOD_BAD__RX64OCTGB__SHIFT    0

#define GBE__GBE0__RX_64OCTETS_PACKETS_GOOD_BAD__RX64OCTGB__MASK    0xffffffff

#define GBE__GBE0__RX_64OCTETS_PACKETS_GOOD_BAD__RX64OCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_65To127Octets_Packets_Good_Bad
//
// Rx 65-127 Octets Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rx65_127octgb : 32;
    };
    unsigned reg;
} GBE__Rx_65To127Octets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__RX_65TO127OCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x7b0)
#define GBE__GBE0__RX_65TO127OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__RX_65TO127OCTETS_PACKETS_GOOD_BAD__RX65_127OCTGB__SHIFT    0

#define GBE__GBE0__RX_65TO127OCTETS_PACKETS_GOOD_BAD__RX65_127OCTGB__MASK    0xffffffff

#define GBE__GBE0__RX_65TO127OCTETS_PACKETS_GOOD_BAD__RX65_127OCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_128To255Octets_Packets_Good_Bad
//
// Rx 128-255 Octets Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rx128_255octgb : 32;
    };
    unsigned reg;
} GBE__Rx_128To255Octets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__RX_128TO255OCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x7b4)
#define GBE__GBE0__RX_128TO255OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__RX_128TO255OCTETS_PACKETS_GOOD_BAD__RX128_255OCTGB__SHIFT    0

#define GBE__GBE0__RX_128TO255OCTETS_PACKETS_GOOD_BAD__RX128_255OCTGB__MASK    0xffffffff

#define GBE__GBE0__RX_128TO255OCTETS_PACKETS_GOOD_BAD__RX128_255OCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_256To511Octets_Packets_Good_Bad
// Rx 256-511 Octets Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rx256_511octgb : 32;
    };
    unsigned reg;
} GBE__Rx_256To511Octets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__RX_256TO511OCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x7b8)
#define GBE__GBE0__RX_256TO511OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__RX_256TO511OCTETS_PACKETS_GOOD_BAD__RX256_511OCTGB__SHIFT    0

#define GBE__GBE0__RX_256TO511OCTETS_PACKETS_GOOD_BAD__RX256_511OCTGB__MASK    0xffffffff

#define GBE__GBE0__RX_256TO511OCTETS_PACKETS_GOOD_BAD__RX256_511OCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_512To1023Octets_Packets_Good_Bad
// RX 512-1023 Octets Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rx512_1023octgb : 32;
    };
    unsigned reg;
} GBE__Rx_512To1023Octets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__RX_512TO1023OCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x7bc)
#define GBE__GBE0__RX_512TO1023OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__RX_512TO1023OCTETS_PACKETS_GOOD_BAD__RX512_1023OCTGB__SHIFT    0

#define GBE__GBE0__RX_512TO1023OCTETS_PACKETS_GOOD_BAD__RX512_1023OCTGB__MASK    0xffffffff

#define GBE__GBE0__RX_512TO1023OCTETS_PACKETS_GOOD_BAD__RX512_1023OCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_1024ToMaxOctets_Packets_Good_Bad
// Rx 1024-Max Octets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rx1024_maxoctgb : 32;
    };
    unsigned reg;
} GBE__Rx_1024ToMaxOctets_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__RX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x7c0)
#define GBE__GBE0__RX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__RX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__RX1024_MAXOCTGB__SHIFT    0

#define GBE__GBE0__RX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__RX1024_MAXOCTGB__MASK    0xffffffff

#define GBE__GBE0__RX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__RX1024_MAXOCTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Unicast_Packets_Good
// Rx Unicast Packets Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxucastg : 32;
    };
    unsigned reg;
} GBE__Rx_Unicast_Packets_Good__ACC_T;

#define GBE__GBE0__RX_UNICAST_PACKETS_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x7c4)
#define GBE__GBE0__RX_UNICAST_PACKETS_GOOD__NUM  0x1

#define GBE__GBE0__RX_UNICAST_PACKETS_GOOD__RXUCASTG__SHIFT    0

#define GBE__GBE0__RX_UNICAST_PACKETS_GOOD__RXUCASTG__MASK    0xffffffff

#define GBE__GBE0__RX_UNICAST_PACKETS_GOOD__RXUCASTG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Length_Error_Packets
// Rx Length Error Packets
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxlenerr : 32;
    };
    unsigned reg;
} GBE__Rx_Length_Error_Packets__ACC_T;

#define GBE__GBE0__RX_LENGTH_ERROR_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x7c8)
#define GBE__GBE0__RX_LENGTH_ERROR_PACKETS__NUM  0x1

#define GBE__GBE0__RX_LENGTH_ERROR_PACKETS__RXLENERR__SHIFT    0

#define GBE__GBE0__RX_LENGTH_ERROR_PACKETS__RXLENERR__MASK    0xffffffff

#define GBE__GBE0__RX_LENGTH_ERROR_PACKETS__RXLENERR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Out_Of_Range_Type_Packets
// Rx Out of Range Type Packet
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxoutofrng : 32;
    };
    unsigned reg;
} GBE__Rx_Out_Of_Range_Type_Packets__ACC_T;

#define GBE__GBE0__RX_OUT_OF_RANGE_TYPE_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x7cc)
#define GBE__GBE0__RX_OUT_OF_RANGE_TYPE_PACKETS__NUM  0x1

#define GBE__GBE0__RX_OUT_OF_RANGE_TYPE_PACKETS__RXOUTOFRNG__SHIFT    0

#define GBE__GBE0__RX_OUT_OF_RANGE_TYPE_PACKETS__RXOUTOFRNG__MASK    0xffffffff

#define GBE__GBE0__RX_OUT_OF_RANGE_TYPE_PACKETS__RXOUTOFRNG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Pause_Packets
// Rx Pause Packets
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxpausepkt : 32;
    };
    unsigned reg;
} GBE__Rx_Pause_Packets__ACC_T;

#define GBE__GBE0__RX_PAUSE_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x7d0)
#define GBE__GBE0__RX_PAUSE_PACKETS__NUM  0x1

#define GBE__GBE0__RX_PAUSE_PACKETS__RXPAUSEPKT__SHIFT    0

#define GBE__GBE0__RX_PAUSE_PACKETS__RXPAUSEPKT__MASK    0xffffffff

#define GBE__GBE0__RX_PAUSE_PACKETS__RXPAUSEPKT__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_FIFO_Overflow_Packets
// Rx FIFO Overflow Packets
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxfifoovfl : 32;
    };
    unsigned reg;
} GBE__Rx_FIFO_Overflow_Packets__ACC_T;

#define GBE__GBE0__RX_FIFO_OVERFLOW_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x7d4)
#define GBE__GBE0__RX_FIFO_OVERFLOW_PACKETS__NUM  0x1

#define GBE__GBE0__RX_FIFO_OVERFLOW_PACKETS__RXFIFOOVFL__SHIFT    0

#define GBE__GBE0__RX_FIFO_OVERFLOW_PACKETS__RXFIFOOVFL__MASK    0xffffffff

#define GBE__GBE0__RX_FIFO_OVERFLOW_PACKETS__RXFIFOOVFL__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_VLAN_Packets_Good_Bad
//
// Rx VLAN Packets Good Bad
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxvlanpktgb : 32;
    };
    unsigned reg;
} GBE__Rx_VLAN_Packets_Good_Bad__ACC_T;

#define GBE__GBE0__RX_VLAN_PACKETS_GOOD_BAD__ADDR (GBE__GBE0__BASE_ADDR + 0x7d8)
#define GBE__GBE0__RX_VLAN_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__GBE0__RX_VLAN_PACKETS_GOOD_BAD__RXVLANPKTGB__SHIFT    0

#define GBE__GBE0__RX_VLAN_PACKETS_GOOD_BAD__RXVLANPKTGB__MASK    0xffffffff

#define GBE__GBE0__RX_VLAN_PACKETS_GOOD_BAD__RXVLANPKTGB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Watchdog_Error_Packets
// Rx Watchdog Error Packets
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxwdgerr : 32;
    };
    unsigned reg;
} GBE__Rx_Watchdog_Error_Packets__ACC_T;

#define GBE__GBE0__RX_WATCHDOG_ERROR_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x7dc)
#define GBE__GBE0__RX_WATCHDOG_ERROR_PACKETS__NUM  0x1

#define GBE__GBE0__RX_WATCHDOG_ERROR_PACKETS__RXWDGERR__SHIFT    0

#define GBE__GBE0__RX_WATCHDOG_ERROR_PACKETS__RXWDGERR__MASK    0xffffffff

#define GBE__GBE0__RX_WATCHDOG_ERROR_PACKETS__RXWDGERR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Receive_Error_Packets
// Rx Receive Error Packets
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxrcverr : 32;
    };
    unsigned reg;
} GBE__Rx_Receive_Error_Packets__ACC_T;

#define GBE__GBE0__RX_RECEIVE_ERROR_PACKETS__ADDR (GBE__GBE0__BASE_ADDR + 0x7e0)
#define GBE__GBE0__RX_RECEIVE_ERROR_PACKETS__NUM  0x1

#define GBE__GBE0__RX_RECEIVE_ERROR_PACKETS__RXRCVERR__SHIFT    0

#define GBE__GBE0__RX_RECEIVE_ERROR_PACKETS__RXRCVERR__MASK    0xffffffff

#define GBE__GBE0__RX_RECEIVE_ERROR_PACKETS__RXRCVERR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: Rx_Control_Packets_Good
// Rx Control Packets Good
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rxctrlg : 32;
    };
    unsigned reg;
} GBE__Rx_Control_Packets_Good__ACC_T;

#define GBE__GBE0__RX_CONTROL_PACKETS_GOOD__ADDR (GBE__GBE0__BASE_ADDR + 0x7e4)
#define GBE__GBE0__RX_CONTROL_PACKETS_GOOD__NUM  0x1

#define GBE__GBE0__RX_CONTROL_PACKETS_GOOD__RXCTRLG__SHIFT    0

#define GBE__GBE0__RX_CONTROL_PACKETS_GOOD__RXCTRLG__MASK    0xffffffff

#define GBE__GBE0__RX_CONTROL_PACKETS_GOOD__RXCTRLG__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Control
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tsena : 1;
        unsigned tscfupdt : 1;
        unsigned tsinit : 1;
        unsigned tsupdt : 1;
        unsigned tstrig : 1;
        unsigned tsaddreg : 1;
        unsigned reserved_7_6 : 2;
        unsigned tsenall : 1;
        unsigned tsctrlssr : 1;
        unsigned tsver2ena : 1;
        unsigned tsipena : 1;
        unsigned tsipv6ena : 1;
        unsigned tsipv4ena : 1;
        unsigned tsevntena : 1;
        unsigned tsmstrena : 1;
        unsigned snaptypsel : 2;
        unsigned tsenmacaddr : 1;
        unsigned csc : 1;
        unsigned esti : 1;
        unsigned reserved_23_21 : 3;
        unsigned txtsstsm : 1;
        unsigned reserved_27_25 : 3;
        unsigned av8021asmen : 1;
        unsigned reserved_31_29 : 3;
    };
    unsigned reg;
} GBE__MAC_Timestamp_Control__ACC_T;

#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__ADDR (GBE__GBE0__BASE_ADDR + 0xb00)
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__NUM  0x1

#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSENA__SHIFT    0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSCFUPDT__SHIFT    1
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSINIT__SHIFT    2
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSUPDT__SHIFT    3
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSTRIG__SHIFT    4
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSADDREG__SHIFT    5
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_7_6__SHIFT    6
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSENALL__SHIFT    8
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSCTRLSSR__SHIFT    9
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSVER2ENA__SHIFT    10
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSIPENA__SHIFT    11
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSIPV6ENA__SHIFT    12
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSIPV4ENA__SHIFT    13
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSEVNTENA__SHIFT    14
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSMSTRENA__SHIFT    15
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__SNAPTYPSEL__SHIFT    16
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSENMACADDR__SHIFT    18
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__CSC__SHIFT    19
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__ESTI__SHIFT    20
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_23_21__SHIFT    21
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TXTSSTSM__SHIFT    24
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_27_25__SHIFT    25
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__AV8021ASMEN__SHIFT    28
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_31_29__SHIFT    29

#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSENA__MASK    0x00000001
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSCFUPDT__MASK    0x00000002
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSINIT__MASK    0x00000004
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSUPDT__MASK    0x00000008
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSTRIG__MASK    0x00000010
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSADDREG__MASK    0x00000020
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_7_6__MASK    0x000000c0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSENALL__MASK    0x00000100
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSCTRLSSR__MASK    0x00000200
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSVER2ENA__MASK    0x00000400
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSIPENA__MASK    0x00000800
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSIPV6ENA__MASK    0x00001000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSIPV4ENA__MASK    0x00002000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSEVNTENA__MASK    0x00004000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSMSTRENA__MASK    0x00008000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__SNAPTYPSEL__MASK    0x00030000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSENMACADDR__MASK    0x00040000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__CSC__MASK    0x00080000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__ESTI__MASK    0x00100000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_23_21__MASK    0x00e00000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TXTSSTSM__MASK    0x01000000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_27_25__MASK    0x0e000000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__AV8021ASMEN__MASK    0x10000000
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_31_29__MASK    0xe0000000

#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSENA__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSCFUPDT__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSINIT__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSUPDT__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSTRIG__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSADDREG__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_7_6__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSENALL__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSCTRLSSR__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSVER2ENA__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSIPENA__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSIPV6ENA__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSIPV4ENA__POR_VALUE    0x1
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSEVNTENA__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSMSTRENA__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__SNAPTYPSEL__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TSENMACADDR__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__CSC__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__ESTI__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_23_21__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__TXTSSTSM__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_27_25__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__AV8021ASMEN__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_CONTROL__RESERVED_31_29__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Sub_Second_Increment
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned reserved_7_0 : 8;
        unsigned snsinc : 8;
        unsigned ssinc : 8;
        unsigned reserved_31_24 : 8;
    };
    unsigned reg;
} GBE__MAC_Sub_Second_Increment__ACC_T;

#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__ADDR (GBE__GBE0__BASE_ADDR + 0xb04)
#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__NUM  0x1

#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__RESERVED_7_0__SHIFT    0
#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__SNSINC__SHIFT    8
#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__SSINC__SHIFT    16
#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__RESERVED_31_24__SHIFT    24

#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__RESERVED_7_0__MASK    0x000000ff
#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__SNSINC__MASK    0x0000ff00
#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__SSINC__MASK    0x00ff0000
#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__RESERVED_31_24__MASK    0xff000000

#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__RESERVED_7_0__POR_VALUE    0x0
#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__SNSINC__POR_VALUE    0x0
#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__SSINC__POR_VALUE    0x0
#define GBE__GBE0__MAC_SUB_SECOND_INCREMENT__RESERVED_31_24__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_System_Time_Seconds
//
// Timestamp Second
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tss : 32;
    };
    unsigned reg;
} GBE__MAC_System_Time_Seconds__ACC_T;

#define GBE__GBE0__MAC_SYSTEM_TIME_SECONDS__ADDR (GBE__GBE0__BASE_ADDR + 0xb08)
#define GBE__GBE0__MAC_SYSTEM_TIME_SECONDS__NUM  0x1

#define GBE__GBE0__MAC_SYSTEM_TIME_SECONDS__TSS__SHIFT    0

#define GBE__GBE0__MAC_SYSTEM_TIME_SECONDS__TSS__MASK    0xffffffff

#define GBE__GBE0__MAC_SYSTEM_TIME_SECONDS__TSS__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_System_Time_Nanoseconds
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tsss : 31;
        unsigned reserved_31 : 1;
    };
    unsigned reg;
} GBE__MAC_System_Time_Nanoseconds__ACC_T;

#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS__ADDR (GBE__GBE0__BASE_ADDR + 0xb0c)
#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS__NUM  0x1

#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS__TSSS__SHIFT    0
#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS__RESERVED_31__SHIFT    31

#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS__TSSS__MASK    0x7fffffff
#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS__RESERVED_31__MASK    0x80000000

#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS__TSSS__POR_VALUE    0x0
#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS__RESERVED_31__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_System_Time_Seconds_Update
//
// Timestamp Seconds
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tss : 32;
    };
    unsigned reg;
} GBE__MAC_System_Time_Seconds_Update__ACC_T;

#define GBE__GBE0__MAC_SYSTEM_TIME_SECONDS_UPDATE__ADDR (GBE__GBE0__BASE_ADDR + 0xb10)
#define GBE__GBE0__MAC_SYSTEM_TIME_SECONDS_UPDATE__NUM  0x1

#define GBE__GBE0__MAC_SYSTEM_TIME_SECONDS_UPDATE__TSS__SHIFT    0

#define GBE__GBE0__MAC_SYSTEM_TIME_SECONDS_UPDATE__TSS__MASK    0xffffffff

#define GBE__GBE0__MAC_SYSTEM_TIME_SECONDS_UPDATE__TSS__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_System_Time_Nanoseconds_Update
// Add or Subtract Time
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tsss : 31;
        unsigned addsub : 1;
    };
    unsigned reg;
} GBE__MAC_System_Time_Nanoseconds_Update__ACC_T;

#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__ADDR (GBE__GBE0__BASE_ADDR + 0xb14)
#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__NUM  0x1

#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__TSSS__SHIFT    0
#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__ADDSUB__SHIFT    31

#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__TSSS__MASK    0x7fffffff
#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__ADDSUB__MASK    0x80000000

#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__TSSS__POR_VALUE    0x0
#define GBE__GBE0__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__ADDSUB__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Addend
// Timestamp Addend Register
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tsar : 32;
    };
    unsigned reg;
} GBE__MAC_Timestamp_Addend__ACC_T;

#define GBE__GBE0__MAC_TIMESTAMP_ADDEND__ADDR (GBE__GBE0__BASE_ADDR + 0xb18)
#define GBE__GBE0__MAC_TIMESTAMP_ADDEND__NUM  0x1

#define GBE__GBE0__MAC_TIMESTAMP_ADDEND__TSAR__SHIFT    0

#define GBE__GBE0__MAC_TIMESTAMP_ADDEND__TSAR__MASK    0xffffffff

#define GBE__GBE0__MAC_TIMESTAMP_ADDEND__TSAR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Status
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tssovf : 1;
        unsigned tstargt0 : 1;
        unsigned auxtstrig : 1;
        unsigned tstrgterr0 : 1;
        unsigned reserved_14_4 : 11;
        unsigned txtssis : 1;
        unsigned reserved_23_16 : 8;
        unsigned atsstm : 1;
        unsigned atsns : 5;
        unsigned reserved_31_30 : 2;
    };
    unsigned reg;
} GBE__MAC_Timestamp_Status__ACC_T;

#define GBE__GBE0__MAC_TIMESTAMP_STATUS__ADDR (GBE__GBE0__BASE_ADDR + 0xb20)
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__NUM  0x1

#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TSSOVF__SHIFT    0
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TSTARGT0__SHIFT    1
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__AUXTSTRIG__SHIFT    2
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TSTRGTERR0__SHIFT    3
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__RESERVED_14_4__SHIFT    4
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TXTSSIS__SHIFT    15
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__RESERVED_23_16__SHIFT    16
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__ATSSTM__SHIFT    24
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__ATSNS__SHIFT    25
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__RESERVED_31_30__SHIFT    30

#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TSSOVF__MASK    0x00000001
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TSTARGT0__MASK    0x00000002
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__AUXTSTRIG__MASK    0x00000004
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TSTRGTERR0__MASK    0x00000008
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__RESERVED_14_4__MASK    0x00007ff0
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TXTSSIS__MASK    0x00008000
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__RESERVED_23_16__MASK    0x00ff0000
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__ATSSTM__MASK    0x01000000
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__ATSNS__MASK    0x3e000000
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__RESERVED_31_30__MASK    0xc0000000

#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TSSOVF__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TSTARGT0__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__AUXTSTRIG__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TSTRGTERR0__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__RESERVED_14_4__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__TXTSSIS__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__RESERVED_23_16__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__ATSSTM__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__ATSNS__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_STATUS__RESERVED_31_30__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Tx_Timestamp_Status_Nanoseconds
// Transmit Timestamp Status Missed
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txtsslo : 31;
        unsigned txtssmis : 1;
    };
    unsigned reg;
} GBE__MAC_Tx_Timestamp_Status_Nanoseconds__ACC_T;

#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__ADDR (GBE__GBE0__BASE_ADDR + 0xb30)
#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__NUM  0x1

#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__TXTSSLO__SHIFT    0
#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__TXTSSMIS__SHIFT    31

#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__TXTSSLO__MASK    0x7fffffff
#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__TXTSSMIS__MASK    0x80000000

#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__TXTSSLO__POR_VALUE    0x0
#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__TXTSSMIS__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Tx_Timestamp_Status_Seconds
// Transmit Timestamp Status High
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txtsshi : 32;
    };
    unsigned reg;
} GBE__MAC_Tx_Timestamp_Status_Seconds__ACC_T;

#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_SECONDS__ADDR (GBE__GBE0__BASE_ADDR + 0xb34)
#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_SECONDS__NUM  0x1

#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_SECONDS__TXTSSHI__SHIFT    0

#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_SECONDS__TXTSSHI__MASK    0xffffffff

#define GBE__GBE0__MAC_TX_TIMESTAMP_STATUS_SECONDS__TXTSSHI__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Auxiliary_Control
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned atsfc : 1;
        unsigned reserved_3_1 : 3;
        unsigned atsen0 : 1;
        unsigned reserved_31_5 : 27;
    };
    unsigned reg;
} GBE__MAC_Auxiliary_Control__ACC_T;

#define GBE__GBE0__MAC_AUXILIARY_CONTROL__ADDR (GBE__GBE0__BASE_ADDR + 0xb40)
#define GBE__GBE0__MAC_AUXILIARY_CONTROL__NUM  0x1

#define GBE__GBE0__MAC_AUXILIARY_CONTROL__ATSFC__SHIFT    0
#define GBE__GBE0__MAC_AUXILIARY_CONTROL__RESERVED_3_1__SHIFT    1
#define GBE__GBE0__MAC_AUXILIARY_CONTROL__ATSEN0__SHIFT    4
#define GBE__GBE0__MAC_AUXILIARY_CONTROL__RESERVED_31_5__SHIFT    5

#define GBE__GBE0__MAC_AUXILIARY_CONTROL__ATSFC__MASK    0x00000001
#define GBE__GBE0__MAC_AUXILIARY_CONTROL__RESERVED_3_1__MASK    0x0000000e
#define GBE__GBE0__MAC_AUXILIARY_CONTROL__ATSEN0__MASK    0x00000010
#define GBE__GBE0__MAC_AUXILIARY_CONTROL__RESERVED_31_5__MASK    0xffffffe0

#define GBE__GBE0__MAC_AUXILIARY_CONTROL__ATSFC__POR_VALUE    0x0
#define GBE__GBE0__MAC_AUXILIARY_CONTROL__RESERVED_3_1__POR_VALUE    0x0
#define GBE__GBE0__MAC_AUXILIARY_CONTROL__ATSEN0__POR_VALUE    0x0
#define GBE__GBE0__MAC_AUXILIARY_CONTROL__RESERVED_31_5__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Auxiliary_Timestamp_Nanoseconds
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned auxtslo : 31;
        unsigned reserved_31 : 1;
    };
    unsigned reg;
} GBE__MAC_Auxiliary_Timestamp_Nanoseconds__ACC_T;

#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__ADDR (GBE__GBE0__BASE_ADDR + 0xb48)
#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__NUM  0x1

#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__AUXTSLO__SHIFT    0
#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__RESERVED_31__SHIFT    31

#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__AUXTSLO__MASK    0x7fffffff
#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__RESERVED_31__MASK    0x80000000

#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__AUXTSLO__POR_VALUE    0x0
#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__RESERVED_31__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Auxiliary_Timestamp_Seconds
// Auxiliary Timestamp
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned auxtshi : 32;
    };
    unsigned reg;
} GBE__MAC_Auxiliary_Timestamp_Seconds__ACC_T;

#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_SECONDS__ADDR (GBE__GBE0__BASE_ADDR + 0xb4c)
#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_SECONDS__NUM  0x1

#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_SECONDS__AUXTSHI__SHIFT    0

#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_SECONDS__AUXTSHI__MASK    0xffffffff

#define GBE__GBE0__MAC_AUXILIARY_TIMESTAMP_SECONDS__AUXTSHI__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Ingress_Asym_Corr
//
// One-Step Timestamp Ingress Asymmetry Correction
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned ostiac : 32;
    };
    unsigned reg;
} GBE__MAC_Timestamp_Ingress_Asym_Corr__ACC_T;

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_ASYM_CORR__ADDR (GBE__GBE0__BASE_ADDR + 0xb50)
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_ASYM_CORR__NUM  0x1

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_ASYM_CORR__OSTIAC__SHIFT    0

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_ASYM_CORR__OSTIAC__MASK    0xffffffff

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_ASYM_CORR__OSTIAC__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Egress_Asym_Corr
//
// One-Step Timestamp Ingress Asymmetry Correction
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned osteac : 32;
    };
    unsigned reg;
} GBE__MAC_Timestamp_Egress_Asym_Corr__ACC_T;

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_ASYM_CORR__ADDR (GBE__GBE0__BASE_ADDR + 0xb54)
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_ASYM_CORR__NUM  0x1

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_ASYM_CORR__OSTEAC__SHIFT    0

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_ASYM_CORR__OSTEAC__MASK    0xffffffff

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_ASYM_CORR__OSTEAC__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Ingress_Corr_Nanosecond
// Timestamp Egress Correction
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tsic : 32;
    };
    unsigned reg;
} GBE__MAC_Timestamp_Ingress_Corr_Nanosecond__ACC_T;

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_NANOSECOND__ADDR (GBE__GBE0__BASE_ADDR + 0xb58)
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_NANOSECOND__NUM  0x1

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_NANOSECOND__TSIC__SHIFT    0

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_NANOSECOND__TSIC__MASK    0xffffffff

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_NANOSECOND__TSIC__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Ingress_Corr_Subnanosec
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned reserved_7_0 : 8;
        unsigned tsicsns : 8;
        unsigned reserved_31_16 : 16;
    };
    unsigned reg;
} GBE__MAC_Timestamp_Ingress_Corr_Subnanosec__ACC_T;

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__ADDR (GBE__GBE0__BASE_ADDR + 0xb60)
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__NUM  0x1

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__RESERVED_7_0__SHIFT    0
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__TSICSNS__SHIFT    8
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__RESERVED_31_16__SHIFT    16

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__RESERVED_7_0__MASK    0x000000ff
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__TSICSNS__MASK    0x0000ff00
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__RESERVED_31_16__MASK    0xffff0000

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__RESERVED_7_0__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__TSICSNS__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Egress_Corr_Subnanosec
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned reserved_7_0 : 8;
        unsigned tsecsns : 8;
        unsigned reserved_31_16 : 16;
    };
    unsigned reg;
} GBE__MAC_Timestamp_Egress_Corr_Subnanosec__ACC_T;

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__ADDR (GBE__GBE0__BASE_ADDR + 0xb64)
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__NUM  0x1

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__RESERVED_7_0__SHIFT    0
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__TSECSNS__SHIFT    8
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__RESERVED_31_16__SHIFT    16

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__RESERVED_7_0__MASK    0x000000ff
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__TSECSNS__MASK    0x0000ff00
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__RESERVED_31_16__MASK    0xffff0000

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__RESERVED_7_0__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__TSECSNS__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Ingress_Latency
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned reserved_7_0 : 8;
        unsigned itlsns : 8;
        unsigned itlns : 12;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} GBE__MAC_Timestamp_Ingress_Latency__ACC_T;

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__ADDR (GBE__GBE0__BASE_ADDR + 0xb68)
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__NUM  0x1

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__RESERVED_7_0__SHIFT    0
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__ITLSNS__SHIFT    8
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__ITLNS__SHIFT    16
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__RESERVED_31_28__SHIFT    28

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__RESERVED_7_0__MASK    0x000000ff
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__ITLSNS__MASK    0x0000ff00
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__ITLNS__MASK    0x0fff0000
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__RESERVED_31_28__MASK    0xf0000000

#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__RESERVED_7_0__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__ITLSNS__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__ITLNS__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_INGRESS_LATENCY__RESERVED_31_28__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Egress_Latency
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned reserved_7_0 : 8;
        unsigned etlsns : 8;
        unsigned etlns : 12;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} GBE__MAC_Timestamp_Egress_Latency__ACC_T;

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__ADDR (GBE__GBE0__BASE_ADDR + 0xb6c)
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__NUM  0x1

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__RESERVED_7_0__SHIFT    0
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__ETLSNS__SHIFT    8
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__ETLNS__SHIFT    16
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__RESERVED_31_28__SHIFT    28

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__RESERVED_7_0__MASK    0x000000ff
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__ETLSNS__MASK    0x0000ff00
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__ETLNS__MASK    0x0fff0000
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__RESERVED_31_28__MASK    0xf0000000

#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__RESERVED_7_0__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__ETLSNS__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__ETLNS__POR_VALUE    0x0
#define GBE__GBE0__MAC_TIMESTAMP_EGRESS_LATENCY__RESERVED_31_28__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_PPS_Control
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned ppsctrl_ppscmd : 4;
        unsigned reserved_31_4 : 28;
    };
    unsigned reg;
} GBE__MAC_PPS_Control__ACC_T;

#define GBE__GBE0__MAC_PPS_CONTROL__ADDR (GBE__GBE0__BASE_ADDR + 0xb70)
#define GBE__GBE0__MAC_PPS_CONTROL__NUM  0x1

#define GBE__GBE0__MAC_PPS_CONTROL__PPSCTRL_PPSCMD__SHIFT    0
#define GBE__GBE0__MAC_PPS_CONTROL__RESERVED_31_4__SHIFT    4

#define GBE__GBE0__MAC_PPS_CONTROL__PPSCTRL_PPSCMD__MASK    0x0000000f
#define GBE__GBE0__MAC_PPS_CONTROL__RESERVED_31_4__MASK    0xfffffff0

#define GBE__GBE0__MAC_PPS_CONTROL__PPSCTRL_PPSCMD__POR_VALUE    0x0
#define GBE__GBE0__MAC_PPS_CONTROL__RESERVED_31_4__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_PPS0_Target_Time_Seconds
// PPS Target Time Seconds Register
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tstrh0 : 32;
    };
    unsigned reg;
} GBE__MAC_PPS0_Target_Time_Seconds__ACC_T;

#define GBE__GBE0__MAC_PPS0_TARGET_TIME_SECONDS__ADDR (GBE__GBE0__BASE_ADDR + 0x0b80)
#define GBE__GBE0__MAC_PPS0_TARGET_TIME_SECONDS__NUM  0x1

#define GBE__GBE0__MAC_PPS0_TARGET_TIME_SECONDS__TSTRH0__SHIFT    0

#define GBE__GBE0__MAC_PPS0_TARGET_TIME_SECONDS__TSTRH0__MASK    0xffffffff

#define GBE__GBE0__MAC_PPS0_TARGET_TIME_SECONDS__TSTRH0__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MAC_PPS0_Target_Time_Nanoseconds
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned ttsl0 : 31;
        unsigned reserved_31 : 1;
    };
    unsigned reg;
} GBE__MAC_PPS0_Target_Time_Nanoseconds__ACC_T;

#define GBE__GBE0__MAC_PPS0_TARGET_TIME_NANOSECONDS__ADDR (GBE__GBE0__BASE_ADDR + 0x0B84)
#define GBE__GBE0__MAC_PPS0_TARGET_TIME_NANOSECONDS__NUM  0x1

#define GBE__GBE0__MAC_PPS0_TARGET_TIME_NANOSECONDS__TTSL0__SHIFT    0
#define GBE__GBE0__MAC_PPS0_TARGET_TIME_NANOSECONDS__RESERVED_31__SHIFT    31

#define GBE__GBE0__MAC_PPS0_TARGET_TIME_NANOSECONDS__TTSL0__MASK    0x7fffffff
#define GBE__GBE0__MAC_PPS0_TARGET_TIME_NANOSECONDS__RESERVED_31__MASK    0x80000000

#define GBE__GBE0__MAC_PPS0_TARGET_TIME_NANOSECONDS__TTSL0__POR_VALUE    0x0
#define GBE__GBE0__MAC_PPS0_TARGET_TIME_NANOSECONDS__RESERVED_31__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MTL_Operation_Mode
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned reserved_0 : 1;
        unsigned dtxsts : 1;
        unsigned reserved_7_2 : 6;
        unsigned cntprst : 1;
        unsigned cntclr : 1;
        unsigned reserved_31_10 : 22;
    };
    unsigned reg;
} GBE__MTL_Operation_Mode__ACC_T;

#define GBE__GBE0__MTL_OPERATION_MODE__ADDR (GBE__GBE0__BASE_ADDR + 0xc00)
#define GBE__GBE0__MTL_OPERATION_MODE__NUM  0x1

#define GBE__GBE0__MTL_OPERATION_MODE__RESERVED_0__SHIFT    0
#define GBE__GBE0__MTL_OPERATION_MODE__DTXSTS__SHIFT    1
#define GBE__GBE0__MTL_OPERATION_MODE__RESERVED_7_2__SHIFT    2
#define GBE__GBE0__MTL_OPERATION_MODE__CNTPRST__SHIFT    8
#define GBE__GBE0__MTL_OPERATION_MODE__CNTCLR__SHIFT    9
#define GBE__GBE0__MTL_OPERATION_MODE__RESERVED_31_10__SHIFT    10

#define GBE__GBE0__MTL_OPERATION_MODE__RESERVED_0__MASK    0x00000001
#define GBE__GBE0__MTL_OPERATION_MODE__DTXSTS__MASK    0x00000002
#define GBE__GBE0__MTL_OPERATION_MODE__RESERVED_7_2__MASK    0x000000fc
#define GBE__GBE0__MTL_OPERATION_MODE__CNTPRST__MASK    0x00000100
#define GBE__GBE0__MTL_OPERATION_MODE__CNTCLR__MASK    0x00000200
#define GBE__GBE0__MTL_OPERATION_MODE__RESERVED_31_10__MASK    0xfffffc00

#define GBE__GBE0__MTL_OPERATION_MODE__RESERVED_0__POR_VALUE    0x0
#define GBE__GBE0__MTL_OPERATION_MODE__DTXSTS__POR_VALUE    0x0
#define GBE__GBE0__MTL_OPERATION_MODE__RESERVED_7_2__POR_VALUE    0x0
#define GBE__GBE0__MTL_OPERATION_MODE__CNTPRST__POR_VALUE    0x0
#define GBE__GBE0__MTL_OPERATION_MODE__CNTCLR__POR_VALUE    0x0
#define GBE__GBE0__MTL_OPERATION_MODE__RESERVED_31_10__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MTL_Interrupt_Status
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned q0is : 1;
        unsigned reserved_31_1 : 31;
    };
    unsigned reg;
} GBE__MTL_Interrupt_Status__ACC_T;

#define GBE__GBE0__MTL_INTERRUPT_STATUS__ADDR (GBE__GBE0__BASE_ADDR + 0xc20)
#define GBE__GBE0__MTL_INTERRUPT_STATUS__NUM  0x1

#define GBE__GBE0__MTL_INTERRUPT_STATUS__Q0IS__SHIFT    0
#define GBE__GBE0__MTL_INTERRUPT_STATUS__RESERVED_31_1__SHIFT    1

#define GBE__GBE0__MTL_INTERRUPT_STATUS__Q0IS__MASK    0x00000001
#define GBE__GBE0__MTL_INTERRUPT_STATUS__RESERVED_31_1__MASK    0xfffffffe

#define GBE__GBE0__MTL_INTERRUPT_STATUS__Q0IS__POR_VALUE    0x0
#define GBE__GBE0__MTL_INTERRUPT_STATUS__RESERVED_31_1__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MTL_TxQ0_Operation_Mode
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned ftq : 1;
        unsigned tsf : 1;
        unsigned reserved_3_2 : 2;
        unsigned ttc : 3;
        unsigned reserved_31_7 : 25;
    };
    unsigned reg;
} GBE__MTL_TxQ0_Operation_Mode__ACC_T;

#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__ADDR (GBE__GBE0__BASE_ADDR + 0xd00)
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__NUM  0x1

#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__FTQ__SHIFT    0
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__TSF__SHIFT    1
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__RESERVED_3_2__SHIFT    2
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__TTC__SHIFT    4
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__RESERVED_31_7__SHIFT    7

#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__FTQ__MASK    0x00000001
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__TSF__MASK    0x00000002
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__RESERVED_3_2__MASK    0x0000000c
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__TTC__MASK    0x00000070
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__RESERVED_31_7__MASK    0xffffff80

#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__FTQ__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__TSF__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__RESERVED_3_2__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__TTC__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_OPERATION_MODE__RESERVED_31_7__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MTL_TxQ0_Underflow
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned uffrmcnt : 11;
        unsigned ufcntovf : 1;
        unsigned reserved_31_12 : 20;
    };
    unsigned reg;
} GBE__MTL_TxQ0_Underflow__ACC_T;

#define GBE__GBE0__MTL_TXQ0_UNDERFLOW__ADDR (GBE__GBE0__BASE_ADDR + 0xd04)
#define GBE__GBE0__MTL_TXQ0_UNDERFLOW__NUM  0x1

#define GBE__GBE0__MTL_TXQ0_UNDERFLOW__UFFRMCNT__SHIFT    0
#define GBE__GBE0__MTL_TXQ0_UNDERFLOW__UFCNTOVF__SHIFT    11
#define GBE__GBE0__MTL_TXQ0_UNDERFLOW__RESERVED_31_12__SHIFT    12

#define GBE__GBE0__MTL_TXQ0_UNDERFLOW__UFFRMCNT__MASK    0x000007ff
#define GBE__GBE0__MTL_TXQ0_UNDERFLOW__UFCNTOVF__MASK    0x00000800
#define GBE__GBE0__MTL_TXQ0_UNDERFLOW__RESERVED_31_12__MASK    0xfffff000

#define GBE__GBE0__MTL_TXQ0_UNDERFLOW__UFFRMCNT__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_UNDERFLOW__UFCNTOVF__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_UNDERFLOW__RESERVED_31_12__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MTL_TxQ0_Debug
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txqpaused : 1;
        unsigned trcsts : 2;
        unsigned twcsts : 1;
        unsigned txqsts : 1;
        unsigned txstsfsts : 1;
        unsigned reserved_15_6 : 10;
        unsigned ptxq : 3;
        unsigned reserved_19 : 1;
        unsigned stxstsf : 3;
        unsigned reserved_31_23 : 9;
    };
    unsigned reg;
} GBE__MTL_TxQ0_Debug__ACC_T;

#define GBE__GBE0__MTL_TXQ0_DEBUG__ADDR (GBE__GBE0__BASE_ADDR + 0xd08)
#define GBE__GBE0__MTL_TXQ0_DEBUG__NUM  0x1

#define GBE__GBE0__MTL_TXQ0_DEBUG__TXQPAUSED__SHIFT    0
#define GBE__GBE0__MTL_TXQ0_DEBUG__TRCSTS__SHIFT    1
#define GBE__GBE0__MTL_TXQ0_DEBUG__TWCSTS__SHIFT    3
#define GBE__GBE0__MTL_TXQ0_DEBUG__TXQSTS__SHIFT    4
#define GBE__GBE0__MTL_TXQ0_DEBUG__TXSTSFSTS__SHIFT    5
#define GBE__GBE0__MTL_TXQ0_DEBUG__RESERVED_15_6__SHIFT    6
#define GBE__GBE0__MTL_TXQ0_DEBUG__PTXQ__SHIFT    16
#define GBE__GBE0__MTL_TXQ0_DEBUG__RESERVED_19__SHIFT    19
#define GBE__GBE0__MTL_TXQ0_DEBUG__STXSTSF__SHIFT    20
#define GBE__GBE0__MTL_TXQ0_DEBUG__RESERVED_31_23__SHIFT    23

#define GBE__GBE0__MTL_TXQ0_DEBUG__TXQPAUSED__MASK    0x00000001
#define GBE__GBE0__MTL_TXQ0_DEBUG__TRCSTS__MASK    0x00000006
#define GBE__GBE0__MTL_TXQ0_DEBUG__TWCSTS__MASK    0x00000008
#define GBE__GBE0__MTL_TXQ0_DEBUG__TXQSTS__MASK    0x00000010
#define GBE__GBE0__MTL_TXQ0_DEBUG__TXSTSFSTS__MASK    0x00000020
#define GBE__GBE0__MTL_TXQ0_DEBUG__RESERVED_15_6__MASK    0x0000ffc0
#define GBE__GBE0__MTL_TXQ0_DEBUG__PTXQ__MASK    0x00070000
#define GBE__GBE0__MTL_TXQ0_DEBUG__RESERVED_19__MASK    0x00080000
#define GBE__GBE0__MTL_TXQ0_DEBUG__STXSTSF__MASK    0x00700000
#define GBE__GBE0__MTL_TXQ0_DEBUG__RESERVED_31_23__MASK    0xff800000

#define GBE__GBE0__MTL_TXQ0_DEBUG__TXQPAUSED__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_DEBUG__TRCSTS__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_DEBUG__TWCSTS__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_DEBUG__TXQSTS__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_DEBUG__TXSTSFSTS__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_DEBUG__RESERVED_15_6__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_DEBUG__PTXQ__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_DEBUG__RESERVED_19__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_DEBUG__STXSTSF__POR_VALUE    0x0
#define GBE__GBE0__MTL_TXQ0_DEBUG__RESERVED_31_23__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MTL_Q0_Interrupt_Control_Status
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned txunfis : 1;
        unsigned reserved_7_1 : 7;
        unsigned txuie : 1;
        unsigned reserved_15_9 : 7;
        unsigned rxovfis : 1;
        unsigned reserved_23_17 : 7;
        unsigned rxoie : 1;
        unsigned reserved_31_25 : 7;
    };
    unsigned reg;
} GBE__MTL_Q0_Interrupt_Control_Status__ACC_T;

#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__ADDR (GBE__GBE0__BASE_ADDR + 0xd2c)
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__NUM  0x1

#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__TXUNFIS__SHIFT    0
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_7_1__SHIFT    1
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__TXUIE__SHIFT    8
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_15_9__SHIFT    9
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RXOVFIS__SHIFT    16
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_23_17__SHIFT    17
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RXOIE__SHIFT    24
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_31_25__SHIFT    25

#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__TXUNFIS__MASK    0x00000001
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_7_1__MASK    0x000000fe
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__TXUIE__MASK    0x00000100
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_15_9__MASK    0x0000fe00
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RXOVFIS__MASK    0x00010000
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_23_17__MASK    0x00fe0000
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RXOIE__MASK    0x01000000
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_31_25__MASK    0xfe000000

#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__TXUNFIS__POR_VALUE    0x0
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_7_1__POR_VALUE    0x0
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__TXUIE__POR_VALUE    0x0
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_15_9__POR_VALUE    0x0
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RXOVFIS__POR_VALUE    0x0
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_23_17__POR_VALUE    0x0
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RXOIE__POR_VALUE    0x0
#define GBE__GBE0__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_31_25__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MTL_RxQ0_Operation_Mode
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rtc : 2;
        unsigned reserved_2 : 1;
        unsigned fup : 1;
        unsigned fep : 1;
        unsigned rsf : 1;
        unsigned dis_tcp_ef : 1;
        unsigned ehfc : 1;
        unsigned rfa : 5;
        unsigned reserved_13 : 1;
        unsigned rfd : 5;
        unsigned reserved_31_19 : 13;
    };
    unsigned reg;
} GBE__MTL_RxQ0_Operation_Mode__ACC_T;

#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__ADDR (GBE__GBE0__BASE_ADDR + 0xd30)
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__NUM  0x1

#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RTC__SHIFT    0
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RESERVED_2__SHIFT    2
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__FUP__SHIFT    3
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__FEP__SHIFT    4
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RSF__SHIFT    5
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__DIS_TCP_EF__SHIFT    6
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__EHFC__SHIFT    7
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RFA__SHIFT    8
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RESERVED_13__SHIFT    13
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RFD__SHIFT    14
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RESERVED_31_19__SHIFT    19

#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RTC__MASK    0x00000003
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RESERVED_2__MASK    0x00000004
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__FUP__MASK    0x00000008
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__FEP__MASK    0x00000010
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RSF__MASK    0x00000020
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__DIS_TCP_EF__MASK    0x00000040
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__EHFC__MASK    0x00000080
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RFA__MASK    0x00001f00
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RESERVED_13__MASK    0x00002000
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RFD__MASK    0x0007c000
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RESERVED_31_19__MASK    0xfff80000

#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RTC__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RESERVED_2__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__FUP__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__FEP__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RSF__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__DIS_TCP_EF__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__EHFC__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RFA__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RESERVED_13__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RFD__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_OPERATION_MODE__RESERVED_31_19__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MTL_RxQ0_Missed_Packet_Overflow_Cnt
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned ovfpktcnt : 11;
        unsigned ovfcntovf : 1;
        unsigned reserved_15_12 : 4;
        unsigned mispktcnt : 11;
        unsigned miscntovf : 1;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} GBE__MTL_RxQ0_Missed_Packet_Overflow_Cnt__ACC_T;

#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__ADDR (GBE__GBE0__BASE_ADDR + 0xd34)
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__NUM  0x1

#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__OVFPKTCNT__SHIFT    0
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__OVFCNTOVF__SHIFT    11
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__RESERVED_15_12__SHIFT    12
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__MISPKTCNT__SHIFT    16
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__MISCNTOVF__SHIFT    27
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__RESERVED_31_28__SHIFT    28

#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__OVFPKTCNT__MASK    0x000007ff
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__OVFCNTOVF__MASK    0x00000800
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__RESERVED_15_12__MASK    0x0000f000
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__MISPKTCNT__MASK    0x07ff0000
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__MISCNTOVF__MASK    0x08000000
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__RESERVED_31_28__MASK    0xf0000000

#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__OVFPKTCNT__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__OVFCNTOVF__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__RESERVED_15_12__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__MISPKTCNT__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__MISCNTOVF__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__RESERVED_31_28__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MTL_RxQ0_Debug
// Reserved.
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned rwcsts : 1;
        unsigned rrcsts : 2;
        unsigned reserved_3 : 1;
        unsigned rxqsts : 2;
        unsigned reserved_15_6 : 10;
        unsigned prxq : 14;
        unsigned reserved_31_30 : 2;
    };
    unsigned reg;
} GBE__MTL_RxQ0_Debug__ACC_T;

#define GBE__GBE0__MTL_RXQ0_DEBUG__ADDR (GBE__GBE0__BASE_ADDR + 0xd38)
#define GBE__GBE0__MTL_RXQ0_DEBUG__NUM  0x1

#define GBE__GBE0__MTL_RXQ0_DEBUG__RWCSTS__SHIFT    0
#define GBE__GBE0__MTL_RXQ0_DEBUG__RRCSTS__SHIFT    1
#define GBE__GBE0__MTL_RXQ0_DEBUG__RESERVED_3__SHIFT    3
#define GBE__GBE0__MTL_RXQ0_DEBUG__RXQSTS__SHIFT    4
#define GBE__GBE0__MTL_RXQ0_DEBUG__RESERVED_15_6__SHIFT    6
#define GBE__GBE0__MTL_RXQ0_DEBUG__PRXQ__SHIFT    16
#define GBE__GBE0__MTL_RXQ0_DEBUG__RESERVED_31_30__SHIFT    30

#define GBE__GBE0__MTL_RXQ0_DEBUG__RWCSTS__MASK    0x00000001
#define GBE__GBE0__MTL_RXQ0_DEBUG__RRCSTS__MASK    0x00000006
#define GBE__GBE0__MTL_RXQ0_DEBUG__RESERVED_3__MASK    0x00000008
#define GBE__GBE0__MTL_RXQ0_DEBUG__RXQSTS__MASK    0x00000030
#define GBE__GBE0__MTL_RXQ0_DEBUG__RESERVED_15_6__MASK    0x0000ffc0
#define GBE__GBE0__MTL_RXQ0_DEBUG__PRXQ__MASK    0x3fff0000
#define GBE__GBE0__MTL_RXQ0_DEBUG__RESERVED_31_30__MASK    0xc0000000

#define GBE__GBE0__MTL_RXQ0_DEBUG__RWCSTS__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_DEBUG__RRCSTS__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_DEBUG__RESERVED_3__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_DEBUG__RXQSTS__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_DEBUG__RESERVED_15_6__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_DEBUG__PRXQ__POR_VALUE    0x0
#define GBE__GBE0__MTL_RXQ0_DEBUG__RESERVED_31_30__POR_VALUE    0x0


/*=============GBE registers========================================*/


/** @defgroup PHY_basic_Control_register
  * @{
  */
#define PHY_Reset                       ((uint16_t)0x8000)      /*!< PHY Reset */
#define PHY_Loopback                    ((uint16_t)0x4000)      /*!< Select loop-back mode */
#define PHY_FULLDUPLEX_100M             ((uint16_t)0x2100)      /*!< Set the full-duplex mode at 100 Mb/s */
#define PHY_HALFDUPLEX_100M             ((uint16_t)0x2000)      /*!< Set the half-duplex mode at 100 Mb/s */
#define PHY_FULLDUPLEX_10M              ((uint16_t)0x0100)      /*!< Set the full-duplex mode at 10 Mb/s */
#define PHY_HALFDUPLEX_10M              ((uint16_t)0x0000)      /*!< Set the half-duplex mode at 10 Mb/s */
#define PHY_AutoNegotiation             ((uint16_t)0x1000)      /*!< Enable auto-negotiation function */
#define PHY_Restart_AutoNegotiation     ((uint16_t)0x0200)      /*!< Restart auto-negotiation function */
#define PHY_Powerdown                   ((uint16_t)0x0800)      /*!< Select the power down mode */
#define PHY_Isolate                     ((uint16_t)0x0400)      /*!< Isolate PHY from MII */

/** @defgroup PHY_basic_status_register
  * @{
  */
#define PHY_AutoNego_Complete           ((uint16_t)0x0020)      /*!< Auto-Negotiation process completed */
#define PHY_Linked_Status               ((uint16_t)0x0004)      /*!< Valid link established */
#define PHY_Jabber_detection            ((uint16_t)0x0002)      /*!< Jabber condition detected */

uint16_t ETH_ReadPHYRegister(uint16_t PHYAddress, uint16_t PHYReg);
uint32_t ETH_WritePHYRegister(uint16_t PHYAddress, uint16_t PHYReg, uint16_t PHYValue);

void ETH_Init(void);
void ETH1_Init(void);
FrameTypeDef ETH_Rx_Packet(void);
FrameTypeDef ETH1_Rx_Packet(void);
uint8_t ETH_Tx_Packet(uint32_t FrameLength);
uint8_t ETH1_Tx_Packet(uint32_t FrameLength);
void ETH_DMARxDescChainInit();
void ETH_DMATxDescChainInit();
void ETH1_DMARxDescChainInit();
void ETH1_DMATxDescChainInit();



void ETH_IOCTxCmd(FunctionalState NewState);
void ETH_IOCRxCmd(FunctionalState NewState);
//void IOC_CHx_Interrupt_Enable( ETH_IOC_CHXTypeDef * IOC_CHx ,uint32_t IT);

uint32_t ETH_Get_ITStatus(uint32_t status);

uint32_t ETH_Clear_ITStatus(uint32_t status);

#ifdef __cplusplus
}
#endif

/* _ZX_XEC_H */
