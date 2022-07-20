/******************************************************************************
 * @file     demosoc.h
 * @brief    NMSIS Core Peripheral Access Layer Header File for
 *           Nuclei Demo SoC which support Nuclei N/NX class cores
 * @version  V1.00
 * @date     22. Nov 2019
 ******************************************************************************/
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

#ifndef __DEMOSOC_H__
#define __DEMOSOC_H__

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


#define SIMU_TEST


/** @addtogroup Nuclei
  * @{
  */


/** @addtogroup demosoc
  * @{
  */


/** @addtogroup Configuration_of_NMSIS
  * @{
  */



/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

typedef enum IRQn {
    /* =======================================  Nuclei Core Specific Interrupt Numbers  ======================================== */

    Reserved0_IRQn            =   0,              /*!<  Internal reserved */
    Reserved1_IRQn            =   1,              /*!<  Internal reserved */
    Reserved2_IRQn            =   2,              /*!<  Internal reserved */
    SysTimerSW_IRQn           =   3,              /*!<  System Timer SW interrupt */
    Reserved3_IRQn            =   4,              /*!<  Internal reserved */
    Reserved4_IRQn            =   5,              /*!<  Internal reserved */
    Reserved5_IRQn            =   6,              /*!<  Internal reserved */
    SysTimer_IRQn             =   7,              /*!<  System Timer Interrupt */
    Reserved6_IRQn            =   8,              /*!<  Internal reserved */
    Reserved7_IRQn            =   9,              /*!<  Internal reserved */
    Reserved8_IRQn            =  10,              /*!<  Internal reserved */
    Reserved9_IRQn            =  11,              /*!<  Internal reserved */
    Reserved10_IRQn           =  12,              /*!<  Internal reserved */
    Reserved11_IRQn           =  13,              /*!<  Internal reserved */
    Reserved12_IRQn           =  14,              /*!<  Internal reserved */
    Reserved13_IRQn           =  15,              /*!<  Internal reserved */
    Reserved14_IRQn           =  16,              /*!<  Internal reserved */
    Reserved15_IRQn           =  17,              /*!<  Internal reserved */
    Reserved16_IRQn           =  18,              /*!<  Internal reserved */

    /* ===========================================  demosoc Specific Interrupt Numbers  ========================================= */
    /* ToDo: add here your device specific external interrupt numbers. 19~1023 is reserved number for user. Maxmum interrupt supported
             could get from clicinfo.NUM_INTERRUPT. According the interrupt handlers defined in startup_Device.s
             eg.: Interrupt for Timer#1       eclic_tim1_handler   ->   TIM1_IRQn */
    SOC_INT19_IRQn           = 19,                /*!< Device Interrupt */
    SOC_INT20_IRQn           = 20,                /*!< Device Interrupt */
    SOC_INT21_IRQn           = 21,                /*!< Device Interrupt */
    SOC_INT22_IRQn           = 22,                /*!< Device Interrupt */
    SOC_INT23_IRQn           = 23,                /*!< Device Interrupt */
    SOC_INT24_IRQn           = 24,                /*!< Device Interrupt */
    SOC_INT25_IRQn           = 25,                /*!< Device Interrupt */
    SOC_INT26_IRQn           = 26,                /*!< Device Interrupt */
    SOC_INT27_IRQn           = 27,                /*!< Device Interrupt */
    SOC_INT28_IRQn           = 28,                /*!< Device Interrupt */
    SOC_INT29_IRQn           = 29,                /*!< Device Interrupt */
    SOC_INT30_IRQn           = 30,                /*!< Device Interrupt */
    SOC_INT31_IRQn           = 31,                /*!< Device Interrupt */
    SOC_INT32_IRQn           = 32,                /*!< Device Interrupt */
    SOC_INT33_IRQn           = 33,                /*!< Device Interrupt */
    SOC_INT34_IRQn           = 34,                /*!< Device Interrupt */
    SOC_INT35_IRQn           = 35,                /*!< Device Interrupt */
    SOC_INT36_IRQn           = 36,                /*!< Device Interrupt */
    SOC_INT37_IRQn           = 37,                /*!< Device Interrupt */
    SOC_INT38_IRQn           = 38,                /*!< Device Interrupt */
    SOC_INT39_IRQn           = 39,                /*!< Device Interrupt */
    SOC_INT40_IRQn           = 40,                /*!< Device Interrupt */
    SOC_INT41_IRQn           = 41,                /*!< Device Interrupt */
    SOC_INT42_IRQn           = 42,                /*!< Device Interrupt */
	XADC_IRQn            	 = 43,                /*!< Device Interrupt */
    SOC_INT44_IRQn           = 44,                /*!< Device Interrupt */
	WDT_IRQn	     		 = 45,                /*!< Device Interrupt */
    SOC_INT46_IRQn           = 46,                /*!< Device Interrupt */
    SOC_INT47_IRQn           = 47,                /*!< Device Interrupt */
    SOC_INT48_IRQn           = 48,                /*!< Device Interrupt */
    SOC_INT49_IRQn           = 49,                /*!< Device Interrupt */
    SOC_INT50_IRQn           = 50,                /*!< Device Interrupt */
	SOC_INT51_IRQn           = 51,                /*!< Device Interrupt */
	SOC_INT52_IRQn           = 52,                /*!< Device Interrupt */
	SOC_INT53_IRQn           = 53,                /*!< Device Interrupt */
	SOC_INT54_IRQn           = 54,                /*!< Device Interrupt */
	SOC_INT55_IRQn           = 55,                /*!< Device Interrupt */
	SOC_INT56_IRQn           = 56,                /*!< Device Interrupt */
	SOC_INT57_IRQn           = 57,                /*!< Device Interrupt */
	/*XEC0_IRQn                = 58,     */           /*!gbe0*/
	/*XEC1_IRQn                = 59,      */          /*!gbe1*/
	/*SOC_INT36_IRQn           = 36,    */            /*!< Device Interrupt */
	SOC_INT58_IRQn           = 58,                /*!< Device Interrupt */
	SOC_INT59_IRQn           = 59,                /*!< Device Interrupt */
	SOC_INT60_IRQn           = 60,                /*!< Device Interrupt */
	SOC_INT61_IRQn           = 61,                /*!< Device Interrupt */
	SOC_INT62_IRQn           = 62,                /*!< Device Interrupt */
	SOC_INT63_IRQn           = 63,                /*!< Device Interrupt */
	SOC_INT64_IRQn           = 64,                /*!< Device Interrupt */
	SOC_INT65_IRQn           = 65,                /*!< Device Interrupt */
	SMC_IRQn           		 = 66,                /*!< Device Interrupt */
	SOC_INT67_IRQn           = 67,                /*!< Device Interrupt */
	SOC_INT68_IRQn           = 68,                /*!< Device Interrupt */
	SOC_INT69_IRQn           = 69,                /*!< Device Interrupt */
	SOC_INT70_IRQn           = 70,                /*!< Device Interrupt */
	GPIO0_IRQn		         = 71,                /*!< Device Interrupt */
	GPIO1_IRQn           	 = 72,                /*!< Device Interrupt */
	GPIO2_IRQn           	 = 73,                /*!< Device Interrupt */
	GPIO3_IRQn           	 = 74,                /*!< Device Interrupt */
	SOC_INT75_IRQn           = 75,                /*!< Device Interrupt */
	SOC_INT76_IRQn           = 76,                /*!< Device Interrupt */
	XEC0_IRQn                = 77,                /*!< Device Interrupt */
		XEC1_IRQn                = 78,                /*!< Device Interrupt */
	SOC_INT79_IRQn           = 79,                /*!< Device Interrupt */
	SOC_INT80_IRQn           = 80,                /*!< Device Interrupt */
	SOC_INT81_IRQn           = 81,                /*!< Device Interrupt */
	SOC_INT82_IRQn           = 82,                /*!< Device Interrupt */
	SOC_INT83_IRQn           = 83,                /*!< Device Interrupt */
	SOC_INT84_IRQn           = 84,                /*!< Device Interrupt */
	SOC_INT85_IRQn           = 85,                /*!< Device Interrupt */
	SOC_INT86_IRQn           = 86,                /*!< Device Interrupt */
	SOC_INT87_IRQn           = 87,                /*!< Device Interrupt */
	SOC_INT88_IRQn           = 88,                /*!< Device Interrupt */
	SOC_INT89_IRQn           = 89,                /*!< Device Interrupt */
	SOC_INT90_IRQn           = 90,                /*!< Device Interrupt */
	SOC_INT91_IRQn           = 91,                /*!< Device Interrupt */
	SOC_INT92_IRQn           = 92,                /*!< Device Interrupt */
	SOC_INT93_IRQn           = 93,                /*!< Device Interrupt */
    SOC_INT_MAX,
} IRQn_Type;

/* =========================================================================================================================== */
/* ================                                  Exception Code Definition                                ================ */
/* =========================================================================================================================== */

typedef enum EXCn {
    /* =======================================  Nuclei N/NX Specific Exception Code  ======================================== */
    InsUnalign_EXCn          =   0,              /*!<  Instruction address misaligned */
    InsAccFault_EXCn         =   1,              /*!<  Instruction access fault */
    IlleIns_EXCn             =   2,              /*!<  Illegal instruction */
    Break_EXCn               =   3,              /*!<  Beakpoint */
    LdAddrUnalign_EXCn       =   4,              /*!<  Load address misaligned */
    LdFault_EXCn             =   5,              /*!<  Load access fault */
    StAddrUnalign_EXCn       =   6,              /*!<  Store or AMO address misaligned */
    StAccessFault_EXCn       =   7,              /*!<  Store or AMO access fault */
    UmodeEcall_EXCn          =   8,              /*!<  Environment call from User mode */
    MmodeEcall_EXCn          =  11,              /*!<  Environment call from Machine mode */
    NMI_EXCn                 = 0xfff,            /*!<  NMI interrupt */
} EXCn_Type;

/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */

/* ToDo: set the defines according your Device */
/* ToDo: define the correct core revision */
#if __riscv_xlen == 32

#ifndef __NUCLEI_CORE_REV
#define __NUCLEI_N_REV            0x0104    /*!< Core Revision r1p4 */
#else
#define __NUCLEI_N_REV            __NUCLEI_CORE_REV
#endif

#elif __riscv_xlen == 64

#ifndef __NUCLEI_CORE_REV
#define __NUCLEI_NX_REV           0x0100    /*!< Core Revision r1p0 */
#else
#define __NUCLEI_NX_REV           __NUCLEI_CORE_REV
#endif

#endif /* __riscv_xlen == 64 */

/* ToDo: define the correct core features for the demosoc */
#define __ECLIC_PRESENT           1                     /*!< Set to 1 if ECLIC is present */

#if defined FPGA_STAGE_7
#define __ECLIC_BASEADDR          0x68020000UL            /*!< Set to ECLIC baseaddr of your device */
#define __SYSTIMER_BASEADDR       0x68030000UL            /*!< Set to SysTimer baseaddr of your device */
#define NAND_BASE                 0x64000000UL            /* Peripheral memory map */
#else
#define __ECLIC_BASEADDR          0x68000000UL            /*!< Set to ECLIC baseaddr of your device */
#define __SYSTIMER_BASEADDR       0x68020000UL            /*!< Set to SysTimer baseaddr of your device */
#define NAND_BASE                 0x69000000UL            /* Peripheral memory map */
#endif

//#define __ECLIC_INTCTLBITS        3                     /*!< Set to 1 - 8, the number of hardware bits are actually implemented in the clicintctl registers. */
#define __ECLIC_INTNUM            159//51                    /*!< Set to 1 - 1024, total interrupt number of ECLIC Unit */
#define __SYSTIMER_PRESENT        1                     /*!< Set to 1 if System Timer is present */

/*!< Set to 0, 1, or 2, 0 not present, 1 single floating point unit present, 2 double floating point unit present */
#if !defined(__riscv_flen)
#define __FPU_PRESENT             0
#elif __riscv_flen == 32
#define __FPU_PRESENT             1
#else
#define __FPU_PRESENT             2
#endif

#define __DSP_PRESENT             1                     /*!< Set to 1 if DSP is present */
#define __PMP_PRESENT             1                     /*!< Set to 1 if PMP is present */
#define __PMP_ENTRY_NUM           16                    /*!< Set to 8 or 16, the number of PMP entries */
#define __ICACHE_PRESENT          0                     /*!< Set to 1 if I-Cache is present */
#define __DCACHE_PRESENT          0                     /*!< Set to 1 if D-Cache is present */
#define __Vendor_SysTickConfig    0                     /*!< Set to 1 if different SysTick Config is used */
#define __Vendor_EXCEPTION        0                     /*!< Set to 1 if vendor exception hander is present */
void Enablepinmux1(void);
#define __Pinmux_PRESENT          1
/** @} */ /* End of group Configuration_of_CMSIS */


#include <nmsis_core.h>                         /*!< Nuclei N/NX class processor and core peripherals */
/* ToDo: include your system_demosoc.h file
         replace 'Device' with your device name */
#include "system_demosoc.h"                    /*!< demosoc System */


/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__GNUC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

#define RTC_FREQ                    2500000
// The TIMER frequency is just the RTC frequency
#define SOC_TIMER_FREQ              RTC_FREQ
/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */


/** @addtogroup Device_Peripheral_peripherals
  * @{
  */
/****************************************************************************
 * TODO: Platform definitions
 *****************************************************************************/


/* enum definitions */
typedef enum {
    DISABLE = 0,
    ENABLE = !DISABLE
} EventStatus, ControlStatus ,FunctionalState;

typedef enum {
    FALSE = 0,
    TRUE = !FALSE
} BOOL;

typedef enum {
    RESET = 0,
    SET = 1,
    MAX = 0X7FFFFFFF
} FlagStatus;

typedef enum {
    ERROR = 0,
    SUCCESS = !ERROR
} ErrStatus;




/****************************************************************************
 * Platform definitions
 *****************************************************************************/
// IOF Mappings
#define IOF0_SPI1_MASK              _AC(0x000007FC,UL)
#define SPI11_NUM_SS                (4)
#define IOF_SPI1_SS0                (2u)
#define IOF_SPI1_SS1                (8u)
#define IOF_SPI1_SS2                (9u)
#define IOF_SPI1_SS3                (10u)
#define IOF_SPI1_MOSI               (3u)
#define IOF_SPI1_MISO               (4u)
#define IOF_SPI1_SCK                (5u)
#define IOF_SPI1_DQ0                (3u)
#define IOF_SPI1_DQ1                (4u)
#define IOF_SPI1_DQ2                (6u)
#define IOF_SPI1_DQ3                (7u)

#define IOF0_SPI2_MASK              _AC(0xFC000000,UL)
#define SPI2_NUM_SS                 (1)
#define IOF_SPI2_SS0                (26u)
#define IOF_SPI2_MOSI               (27u)
#define IOF_SPI2_MISO               (28u)
#define IOF_SPI2_SCK                (29u)
#define IOF_SPI2_DQ0                (27u)
#define IOF_SPI2_DQ1                (28u)
#define IOF_SPI2_DQ2                (30u)
#define IOF_SPI2_DQ3                (31u)

#define IOF0_UART0_MASK             _AC(0x00030000, UL)
#define IOF_UART0_RX                (16u)
#define IOF_UART0_TX                (17u)

#define IOF0_UART1_MASK             _AC(0x03000000, UL)
#define IOF_UART1_RX                (24u)
#define IOF_UART1_TX                (25u)

#define IOF0_I2C_MASK               _AC(0x00003000, UL)
#define IOF_I2C_SDA                 (12u)
#define IOF_I2C_SCL                 (13u)

#define IOF1_PWM0_MASK              _AC(0x0000000F, UL)
#define IOF1_PWM1_MASK              _AC(0x00780000, UL)
#define IOF1_PWM2_MASK              _AC(0x00003C00, UL)

// Interrupt Numbers
#define SOC_ECLIC_NUM_INTERRUPTS    32
#define SOC_ECLIC_INT_GPIO_BASE     19

// Interrupt Handler Definitions
#define SOC_MTIMER_HANDLER          eclic_mtip_handler
#define SOC_SOFTINT_HANDLER         eclic_msip_handler

#define GPIO_BIT_ALL_ZERO           (0x0)
#define GPIO_BIT_ALL_ONE            (0xFFFFFFFF)



typedef struct{
   __IOM uint32_t reverse[89];
   __IOM uint32_t can_fd_ctrl;
   __IOM uint32_t reverse1[18];
   __IOM uint32_t can_dma;

}TOPCFG_TypeDef;
/**
  * @brief GPIO-AL
  */
typedef struct {  /*!< GPIO Structure */
    __IOM uint32_t SWPORTA_DR;					/*offset 0x00,Port data register*/
    __IOM uint32_t SWPORTA_DDR;					/*offset 0x04,Port data direction register*/
    __IOM uint32_t SWPORTA_DR_CLR;				/*offset 0x08,Data clear register*/
    __IOM uint32_t SWPORTA_DDR_CLR;				/*offset 0x0c,Data Direction clear Register*/
    __IOM uint32_t INTEN_CLR;					/*offset 0x10,Interrupt enable clear register*/
    __IOM uint32_t INTMASK_CLR;					/*offset 0x14,Interrupt mask clear register*/
    __IOM uint32_t INTTYPE_LEVEL_CLR;			/*offset 0x18,Interrupt level clear register*/
    __IOM uint32_t INT_POLARITY_CLR;			/*offset 0x1c,Interrupt polarity clear register*/
    __IOM uint32_t DEBOUNCE_CLR;				/*offset 0x20,Debounce enable clear register*/
    __IOM uint32_t INT_BOTHEDGE_CLR;			/*offset 0x24,Interrupt Both Edge type clear register*/
    __IOM uint32_t RSVD_0;          			/*offset 0x28,reserved*/
    __IOM uint32_t RSVD_1;          			/*offset 0x2c,reserved*/
    __IOM uint32_t INTEN;						/*offset 0x30,Interrupt enable register*/
    __IOM uint32_t INTMASK;						/*offset 0x34,Interrupt mask register**/
    __IOM uint32_t INTTYPE_LEVEL;				/*offset 0x38,Interrupt level*/
    __IOM uint32_t INT_POLARITY;				/*offset 0x3c,Interrupt polarity*/
    __IOM uint32_t INTSTATUS;					/*offset 0x40,Interrupt status*/
    __IOM uint32_t RAW_INTSTATUS;				/*offset 0x44,Raw interrupt status*/
    __IOM uint32_t DEBOUNCE;					/*offset 0x48,Debounce enable*/
    __IOM uint32_t PORTA_EOI;					/*offset 0x4c,Clear interrupt register*/
    __IOM uint32_t EXT_PORTA;					/*offset 0x50,External port A register*/
    __IOM uint32_t RSVD_2;          			/*offset 0x54,reserved*/
    __IOM uint32_t RSVD_3;          			/*offset 0x58,reserved*/
    __IOM uint32_t RSVD_4;          			/*offset 0x5c,reserved*/
    __IOM uint32_t LS_SYNC;						/*offset 0x60,Synchronization level*/
    __IOM uint32_t ID_CODE;						/*offset 0x64,GPIO ID code*/
    __IOM uint32_t INT_BOTHEDGE;				/*offset 0x68,Interrupt Both Edge type*/
    __IOM uint32_t VER_ID_CODE;					/*offset 0x6c,GPIO Component Version*/
    __IOM uint32_t CONFIG_REG2;					/*offset 0x70,GPIO Configuration Register 2*/
    __IOM uint32_t CONFIG_REG1;					/*offset 0x74,GPIO Configuration Register 1*/
} GPIO_TypeDef;

/**
  * @brief UART
  */
typedef struct {
    __IOM uint32_t TXFIFO;
    __IOM uint32_t RXFIFO;
    __IOM uint32_t TXCTRL;
    __IOM uint32_t RXCTRL;
    __IOM uint32_t IE;
    __IOM uint32_t IP;
    __IOM uint32_t DIV;
} UART_TypeDef;

/**
  * @brief PWM
  */
typedef struct {
    __IOM uint32_t CFG;
    uint32_t RESERVED0;
    __IOM uint32_t COUNT;
    uint32_t RESERVED1;
    __IOM uint32_t S;
    uint32_t RESERVED2[3];
    __IOM uint32_t CMP0;
    __IOM uint32_t CMP1;
    __IOM uint32_t CMP2;
    __IOM uint32_t CMP3;
} PWM_TypeDef;




/**
  * @brief QSPI-AL
*/
typedef struct
{
    uint32_t DFS                :5;   /*[4:0]-Data Frame Size.*/
    uint32_t RSVD_CTRLR0_5      :1;   /*[5]-RSVD*/
    uint32_t FRF                :2;   /*[7:6]-Frame Format.0x0(SPI);0x1(SSP);0x2(MICROWIRE);default=0,SPI*/
    uint32_t SCPH               :1;   /*[8]-Serial Clock Phase.--fixed by HW*/
    uint32_t SCPOL              :1;   /*[9]-Serial Clock Polarity.--fixed by HW*/
    uint32_t TMOD               :2;   /*[11:10]-Transfer Mode. 0x0(TX_AND_RX);0x1(TX_ONLY);0x2(RX_ONLY);0x3(EEPROM_READ)*/
    uint32_t SLV_OE             :1;   /*[12]-Slave Output Enable.1-dis;0-en*/
    uint32_t SRL                :1;	  /*[13]-Shift Register Loop.*/
    uint32_t SSTE               :1;	  /*[14]-Slave Select Toggle Enable.1-en,0-dis*/
    uint32_t RSVD_CTRLR0_15     :1;   /*[15]-RSVD*/
    uint32_t CFS                :4;   /*[19:16]-Control Frame Size.--Microwire,,not use*/
    uint32_t RSVD_CTRLR0_20_21  :2;   /*[21:20]-RSVD */
    uint32_t SPI_FRF            :2;   /*[23:22]-SPI Frame Format.0(x1),1(x2),2(x4),3(x8) mode*/
    uint32_t SPI_HYPERBUS_EN    :1;   /*[24]-SPI Hyperbus Frame format enable. */
    uint32_t RSVD_CTRLR0_25_30  :6;   /*[30:25]-RSVD */
    uint32_t SSI_IS_MST         :1;   /*[31]-DWC_ssi =1,Master or =0,Slave mode; defualt=1;*/
}ctrlr0;

typedef struct
{
    uint32_t TRANS_TYPE         :2;   /*[1:0]-Address and instruction transfer format.*/
    uint32_t ADDR_L             :4;   /*[5:2]-This bit defines Length of Address to be transmitted.*/
    uint32_t RSVD_6             :1;   /*[6]-RSVD*/
    uint32_t XIP_MD_BIT_EN      :1;   /*[7]-Mode bits enable in XIP mode.*/
    uint32_t INST_L             :2;   /*[9:8]-Dual/Quad/Octal mode instruction length in bits.*/
    uint32_t RSVD_10            :1;   /*[10]-RSVD*/
    uint32_t WAIT_CYCLES        :5;   /*[15:11]-Wait cycles in Dual/Quad/Octal mode between control frames transmit and data reception.*/
    uint32_t SPI_DDR_EN         :1;   /*[16]-SPI DDR Enable bit.*/
    uint32_t INST_DDR_EN        :1;   /*[17]-Instruction DDR Enable bit.*/
    uint32_t SPI_RXDS_EN        :1;   /*[18]-Read data strobe enable bit.*/
    uint32_t XIP_DFS_HC         :1;   /*[19]-Fix DFS for XIP transfers.*/
    uint32_t XIP_INST_EN        :1;   /*[20]-XIP instruction enable bit.*/
    uint32_t SSIC_XIP_CONT_XFER_EN  :1;	/*[21]-Enable continuous transfer in XIP mode.*/
    uint32_t RSVD_23_22         :2;	  /*[23:22]-RSVD*/
    uint32_t SPI_DM_EN	      	:1;   /*[24]-SPI data mask enable bit.*/
    uint32_t SPI_RXDS_SIG_EN    :1;   /*[25]-Enable rxds signaling during address and command phase of Hypebus transfer.*/
    uint32_t XIP_MBL            :2;   /*[27:26]-XIP Mode bits length.*/
    uint32_t RSVD_28            :1;   /*[28]-RSVD*/
    uint32_t XIP_PREFETCH_EN    :1;   /*[29]-Enables XIP pre-fetch functionality in DWC_ssi. */
    uint32_t CLK_STRETCH_EN     :1;   /*[30]-Enables clock stretching capability in SPI transfers.*/
    uint32_t RSVD_31		    :1;   /*[31]-RSVD*/
}spi_ctrlr0;

typedef struct {
    __IOM ctrlr0 CTRLR0;           /*offset 0x0*/
//	__IOM uint32_t CTRLR0;              /*offset 0x0*/
    __IOM uint32_t CTRLR1;              /*offset 0x4*/
    __IOM uint32_t SSIENR;              /*offset 0x8*/
    __IOM uint32_t MWCR;                /*offset 0xC*/
    __IOM uint32_t SER;                 /*offset 0x10*/
    __IOM uint32_t BAUDR;               /*offset 0x14*/
    __IOM uint32_t TXFTLR;              /*offset 0x18*/
    __IOM uint32_t RXFTLR;              /*offset 0x1c*/
    __IOM uint32_t TXFLR;               /*offset 0x20*/
    __IOM uint32_t RXFLR;               /*offset 0x24*/
    __IOM uint32_t SR;                  /*offset 0x28*/
    __IOM uint32_t IMR;                 /*offset 0x2C*/
    __IOM uint32_t ISR;                 /*offset 0x30*/
    __IOM uint32_t RISR;                /*offset 0x34*/
    __IOM uint32_t TXOICR;              /*offset 0x38*/
    __IOM uint32_t RXOICR;              /*offset 0x3c*/
    __IOM uint32_t RXUICR;              /*offset 0x40*/
    __IOM uint32_t MSTICR;              /*offset 0x44*/
    __IOM uint32_t ICR;                 /*offset 0x48*/
    __IOM uint32_t DMACR;               /*offset 0x4c*/
    __IOM uint32_t DMATDLR;             /*offset 0x50*/
    __IOM uint32_t DMARDLR;             /*offset 0x54*/
    __IOM uint32_t IDR;                 /*offset 0x58*/
    __IOM uint32_t SSIC_VERSION_ID;     /*offset 0x5c*/
    __IOM uint32_t DRx[36];             /*offset 0x60+i*0x4, i=[0..35]*/
    __IOM uint32_t RX_SAMPLE_DELAY;     /*offset 0xf0*/
//    __IOM uint32_t SPI_CTRLR0;          /*offset 0xf4*/
    __IOM spi_ctrlr0 SPI_CTRLR0;          /*offset 0xf4*/
    __IOM uint32_t DDR_DRIVE_EDGE;      /*offset 0xf8*/
    __IOM uint32_t XIP_MODE_BITS;       /*offset 0xfc*/
} QSPI_TypeDef;

/**
  * @brief SPI-AL
  */
// this ctrlr0 struct has problem(causeof big/little endian), to be modify.
//typedef struct
//{
//    uint32_t RSVD_CTRLR0_31_25  :7;   /*[31:25]-RSVD */
//    uint32_t SSTE               :1;   /*[24]-Slave Select Toggle Enable.1-en,0-dis*/
//    uint32_t RSVD_CTRLR0_23     :1;   /*[23]-RSVD*/
//    uint32_t SPI_FRF            :2;   /*[22:21]-SPI Frame Format.0(x1),1(x2),2(x4),3(x8) mode;Only support X1 mode.*/
//    uint32_t DFS_32             :5;   /*[20:16]-Data Frame Size in 32-bit transfer size mode.*/
//    uint32_t CFS                :4;   /*[15:12]-Control Frame Size.--Microwire,,not use*/
//    uint32_t SRL                :1;	/*[11]-Shift Register Loop.*/
//    uint32_t SLV_OE             :1;   /*[10]-Slave Output Enable.1-dis;0-en*/
//    uint32_t TMOD               :2;   /*[9:8]-Transfer Mode. 0x0(TX_AND_RX);0x1(TX_ONLY);0x2(RX_ONLY);0x3(EEPROM_READ)*/
//    uint32_t SCPOL              :1;   /*[7]-Serial Clock Polarity.--fixed by HW*/
//    uint32_t SCPH               :1;   /*[6]-Serial Clock Phase.--fixed by HW*/
//    uint32_t FRF                :2;   /*[5:4]-Frame Format.0x0(SPI);0x1(SSP);0x2(MICROWIRE);default=0,SPI*/
//    uint32_t DFS                :4;   /*[3:0]-Data Frame Size.*/
//}ctrlr0;
typedef struct {
	__IOM uint32_t CTRLR0;              /*offset 0x0*/
    __IOM uint32_t CTRLR1;              /*offset 0x4*/
    __IOM uint32_t SSIENR;              /*offset 0x8*/
    __IOM uint32_t MWCR;                /*offset 0xC*/
    __IOM uint32_t SER;                 /*offset 0x10*/
    __IOM uint32_t BAUDR;               /*offset 0x14*/
    __IOM uint32_t TXFTLR;              /*offset 0x18*/
    __IOM uint32_t RXFTLR;              /*offset 0x1c*/
    __IOM uint32_t TXFLR;               /*offset 0x20*/
    __IOM uint32_t RXFLR;               /*offset 0x24*/
    __IOM uint32_t SR;                  /*offset 0x28*/
    __IOM uint32_t IMR;                 /*offset 0x2C*/
    __IOM uint32_t ISR;                 /*offset 0x30*/
    __IOM uint32_t RISR;                /*offset 0x34*/
    __IOM uint32_t TXOICR;              /*offset 0x38*/
    __IOM uint32_t RXOICR;              /*offset 0x3c*/
    __IOM uint32_t RXUICR;              /*offset 0x40*/
    __IOM uint32_t MSTICR;              /*offset 0x44*/
    __IOM uint32_t ICR;                 /*offset 0x48*/
    __IOM uint32_t DMACR;               /*offset 0x4c*/
    __IOM uint32_t DMATDLR;             /*offset 0x50*/
    __IOM uint32_t DMARDLR;             /*offset 0x54*/
    __IOM uint32_t IDR;                 /*offset 0x58*/
    __IOM uint32_t SSIC_VERSION_ID;     /*offset 0x5c*/
    __IOM uint32_t DRx[36];             /*offset 0x60+i*0x4, i=[0..35]*/
    __IOM uint32_t RX_SAMPLE_DELAY;     /*offset 0xf0*/
    __IOM uint32_t RSVD_0;          	/*offset 0xf4*/
    __IOM uint32_t RSVD_1;      		/*offset 0xf8*/
    __IOM uint32_t RSVD_2;		        /*offset 0xfc*/
} SPI_TypeDef;
/******************************* I2C registers definition **************************************/
typedef struct
{
    uint32_t MASTER_MODE         		:1;		/*[0]-Master mode control. 1-mas en;0-mas dis*/
    uint32_t SPEED               		:2;   	/*[2:1]-Speed control. 0x1-standard(100k);0x2-fast(400k);0x3-high(3.4M)*/
    uint32_t IC_10BITADDR_SLAVE			:1;	  	/*[3]-slave addressing cfg. 0x1-10bit;0x0-7bit*/
    uint32_t IC_10BITADDR_MASTER		:1;	  	/*[4]-master addressing cfg. 0x1-10bit;0x0-7bit*/
    uint32_t IC_RESTART_EN				:1;	  	/*[5]-0x1 (ENABLED): Master restart enabled;0x0 (DISABLED): Master restart disabled*/
    uint32_t IC_SLAVE_DISABLE			:1;	  	/*[6]-0x1 (SLAVE_DISABLED): Slave mode is disabled;0x0 (SLAVE_ENABLED): Slave mode is enabled*/
    uint32_t STOP_DET_IFADDRESSED		:1;	  	/*[7]-0x1 (ENABLED): slave issues STOP_DET intr only if addressed;0x0 (DISABLED): slave issues STOP_DET intr always*/
    uint32_t TX_EMPTY_CTRL		 		:1;	  	/*[8]-0x1 (ENABLED): Controlled generation of TX_EMPTY interrupt;0x0 (DISABLED): Default behaviour of TX_EMPTY interrupt*/
    uint32_t RX_FIFO_FULL_HLD_CTRL		:1;	  	/*[9]-0x1 (ENABLED): Hold bus when RX_FIFO is full;0x0 (DISABLED): Overflow when RX_FIFO is full*/
    uint32_t STOP_DET_IF_MASTER_ACTIVE	:1;	  	/*[10]-0x1 (ENABLED): Master issues the STOP_DET interrupt only when master is active;0x0 (DISABLED): Master issues the STOP_DET interrupt irrespective of whether master is active or not*/
    uint32_t BUS_CLEAR_FEATURE_CTRL		:1;	  	/*[11]-0x1 (ENABLED): Bus Clear Feature is enabled;0x0 (DISABLED): Bus Clear Feature is disabled.*/
    uint32_t RSVD_IC_CON_1				:4;	  	/*[15:12]-Reserved*/
    uint32_t RSVD_SMBUS_SAR				:10;	/*[25:16]-Reserved*/
    uint32_t RSVD_IC_CON_2				:6;	  	/*[31:26]-Reserved*/
}ic_con;

typedef struct
{
	uint32_t IC_TAR						:10;  	/*[9:0]-target address*/
	uint32_t GC_OR_START				:1;	  	/*[10]-0x1 (START_BYTE): START byte transmission;0x0 (GENERAL_CALL): GENERAL_CALL byte transmission*/
	uint32_t SPECIAL					:1;	  	/*[11]-0x1 (ENABLED): Enables programming of GENERAL_CALL or START_BYTE transmission;0x0 (DISABLED): Disable...*/
	uint32_t RSVD_IC_TAR				:20;	/*[31:12]-Reserved*/
}ic_tar;

typedef struct
{
	uint32_t IC_SAR						:10;  	/*[9:0]-slave address*/
	uint32_t RSVD_IC_STAR				:22;	/*[31:10]-Reserved*/
}ic_sar;

typedef struct
{
	uint32_t DAT						:8;  	/*[7:0]-data to be transmitted or received*/
	uint32_t CMD						:1;		/*[8]-0x1 (READ): Master Read Command;0x0 (WRITE): Master Write Command*/
	uint32_t STOP						:1;		/*[9]-0x1 (ENABLE): Issue STOP after this command;0x0 (DISABLE): Do not Issue STOP after this command*/
	uint32_t RESTART					:1;		/*[10]-0x1 (ENABLE): Issue RESTART before this command;0x0 (DISABLE): Do not Issue RESTART before this command*/
	uint32_t RSVD						:21;	/*[31:11]-Reserved*/
}ic_data_cmd_bits;

typedef union
{
	uint32_t all;
	ic_data_cmd_bits BITS;
}ic_data_cmd;

typedef struct
{
	uint32_t R_RX_UNDER					:1;  	/*[0]-0x1 (ACTIVE): RX_UNDER interrupt is active;0x0 (INACTIVE): RX_UNDER interrupt is inactive*/
	uint32_t R_RX_OVER	 				:1;		/*[1]-0x1 (ACTIVE): RX_OVER interrupt is active;0x0 (INACTIVE): RX_OVER interrupt is inactive*/
	uint32_t R_RX_FULL	 				:1;		/*[2]-0x1 (ACTIVE): RX_FULL interrupt is active;0x0 (INACTIVE): RX_FULL interrupt is inactive*/
	uint32_t R_TX_OVER	 				:1;		/*[3]-0x1 (ACTIVE): TX_OVER interrupt is active;0x0 (INACTIVE): TX_OVER interrupt is inactive*/
	uint32_t R_TX_EMPTY 				:1;		/*[4]-0x1 (ACTIVE): TX_EMPTY interrupt is active;0x0 (INACTIVE): TX_EMPTY interrupt is inactive*/
	uint32_t R_RD_REQ	 				:1;		/*[5]-0x1 (ACTIVE): RD_REQ interrupt is active;0x0 (INACTIVE): RD_REQ interrupt is inactive*/
	uint32_t R_TX_ABRT	 				:1;		/*[6]-0x1 (ACTIVE): TX_ABORT interrupt is active;0x0 (INACTIVE): TX_ABORT interrupt is inactive*/
	uint32_t R_RX_DONE	 				:1;		/*[7]-0x1 (ACTIVE): RX_DONE interrupt is active;0x0 (INACTIVE): RX_DONE interrupt is inactive*/
	uint32_t R_ACTIVITY 				:1;		/*[8]-0x1 (ACTIVE): ACTIVITY interrupt is active;0x0 (INACTIVE): ACTIVITY interrupt is inactive*/
	uint32_t R_STOP_DET  				:1;		/*[9]-0x1 (ACTIVE): STOP_DET interrupt is active;0x0 (INACTIVE): STOP_DET interrupt is inactive*/
	uint32_t R_START_DET 				:1;		/*[10]-0x1 (ACTIVE): START_DET interrupt is active;0x0 (INACTIVE): START_DET interrupt is inactive*/
	uint32_t R_GEN_CALL 				:1;		/*[11]-0x1 (ACTIVE): GEN_CALL interrupt is active;0x0 (INACTIVE): GEN_CALL interrupt is inactive*/
	uint32_t R_RESTART_DET 				:1;		/*[12]-0x1 (ACTIVE): RESTART_DET interrupt is active;0x0 (INACTIVE): RESTART_DET interrupt is inactive*/
	uint32_t R_MASTER_ON_HOLD			:1;		/*[13]-0x1 (ACTIVE): MASTER_ON_HOLD interrupt is active;0x0 (INACTIVE): MASTER_ON_HOLD interrupt is inactive*/
	uint32_t R_SCL_STUCK_AT_LOW			:1;		/*[14]-0x1 (ACTIVE): SCL_STUCK_AT_LOW interrupt is active;0x0 (INACTIVE): SCL_STUCK_AT_LOW interrupt is inactive*/
	uint32_t RSVD_IC_STAR				:17;	/*[31:15]-Reserved*/
}ic_intr_stat_bits;

typedef union
{
	uint32_t all;
	ic_intr_stat_bits BITS;
}ic_intr_stat;

typedef struct
{
	uint32_t M_RX_UNDER					:1;  	/*[0]-0x1 (DISABLED): RX_UNDER interrupt is unmasked;0x0 (ENABLED): RX_UNDER interrupt is masked*/
	uint32_t M_RX_OVER	 				:1;		/*[1]-0x1 (DISABLED): RX_OVER interrupt is unmasked;0x0 (ENABLED): RX_OVER interrupt is masked*/
	uint32_t M_RX_FULL	 				:1;		/*[2]-0x1 (DISABLED): RX_FULL interrupt is unmasked;0x0 (ENABLED): RX_FULL interrupt is masked*/
	uint32_t M_TX_OVER	 				:1;		/*[3]-0x1 (DISABLED): TX_OVER interrupt is unmasked;0x0 (ENABLED): TX_OVER interrupt is masked*/
	uint32_t M_TX_EMPTY 				:1;		/*[4]-0x1 (DISABLED): TX_EMPTY interrupt is unmasked;0x0 (ENABLED): TX_EMPTY interrupt is masked*/
	uint32_t M_RD_REQ	 				:1;		/*[5]-0x1 (DISABLED): RD_REQ interrupt is unmasked;0x0 (ENABLED): RD_REQ interrupt is masked*/
	uint32_t M_TX_ABRT	 				:1;		/*[6]-0x1 (DISABLED): TX_ABORT interrupt is unmasked;0x0 (ENABLED): TX_ABORT interrupt is masked*/
	uint32_t M_RX_DONE	 				:1;		/*[7]-0x1 (DISABLED): RX_DONE interrupt is unmasked;0x0 (ENABLED): RX_DONE interrupt is masked*/
	uint32_t M_ACTIVITY 				:1;		/*[8]-0x1 (DISABLED): ACTIVITY interrupt is unmasked;0x0 (ENABLED): ACTIVITY interrupt is masked*/
	uint32_t M_STOP_DET  				:1;		/*[9]-0x1 (DISABLED): STOP_DET interrupt is unmasked;0x0 (ENABLED): STOP_DET interrupt is masked*/
	uint32_t M_START_DET 				:1;		/*[10]-0x1 (DISABLED): START_DET interrupt is unmasked;0x0 (ENABLED): START_DET interrupt is masked*/
	uint32_t M_GEN_CALL 				:1;		/*[11]-0x1 (DISABLED): GEN_CALL interrupt is unmasked;0x0 (ENABLED): GEN_CALL interrupt is masked*/
	uint32_t M_RESTART_DET 				:1;		/*[12]-0x1 (DISABLED): RESTART_DET interrupt is unmasked;0x0 (ENABLED): RESTART_DET interrupt is masked*/
	uint32_t M_MASTER_ON_HOLD_read_only	:1;		/*[13]-0x1 (DISABLED): MASTER_ON_HOLD interrupt is unmasked;0x0 (ENABLED): MASTER_ON_HOLD interrupt is masked*/
	uint32_t M_SCL_STUCK_AT_LOW			:1;		/*[14]-0x1 (DISABLED): SCL_STUCK_AT_LOW interrupt is unmasked;0x0 (ENABLED): SCL_STUCK_AT_LOW interrupt is masked*/
	uint32_t RSVD_IC_STAR				:17;	/*[31:15]-Reserved*/
}ic_intr_mask_bits;

typedef union
{
	uint32_t all;
	ic_intr_mask_bits BITS;
}ic_intr_mask;

typedef struct
{
	uint32_t CLR_INTR					:1;  	/*[0]-Read this register to clear the combined interrupt, all individual interrupts, and the IC_TX_ABRT_SOURCE register.*/
	uint32_t RSVD						:31;	/*[31:1]-Reserved*/
}ic_clr_intr;


typedef struct
{
	uint32_t ENABLE						:1;  	/*[0]-0x1 (ENABLED): I2C is enabled;0x0 (DISABLED): I2C is disabled*/
	uint32_t ABORT						:1;		/*[1]-0x1 (ENABLED): ABORT operation in progress;0x0 (DISABLE): ABORT operation not in progress*/
	uint32_t TX_CMD_BLOCK				:1;		/*[2]-0x1 (BLOCKED): Tx Command execution blocked;0x0 (NOT_BLOCKED): Tx Command execution not blocked*/
	uint32_t SDA_STUCK_RECOVERY_ENABLE	:1;		/*[3]-0x1 (ENABLED): Master initiates the SDA stuck at low recovery mechanism;0x0 (DISABLED): Master disabled the SDA stuck at low recovery mechanism.*/
	uint32_t RSVD						:28;	/*[31:4]-Reserved*/
}ic_enable;

typedef struct
{
	uint32_t ACTIVITY					:1;  	/*[0]-0x1 (ACTIVE): I2C is active;0x0 (INACTIVE): I2C is idle*/
	uint32_t TFNF						:1;		/*[1]-0x1 (NOT_FULL): Tx FIFO not full;0x0 (FULL): Tx FIFO is full*/
	uint32_t TFE  						:1;		/*[2]-0x1 (EMPTY): Tx FIFO is empty;0x0 (NON_EMPTY): Tx FIFO not empty*/
	uint32_t RFNE						:1;		/*[3]-0x1 (NOT_EMPTY): Rx FIFO not empty;0x0 (EMPTY): Rx FIFO is empty*/
	uint32_t RFF						:1;		/*[4]-0x1 (FULL): Rx FIFO is full;0x0 (NOT_FULL): Rx FIFO not full*/
	uint32_t MST_ACTIVITY				:1;		/*[5]-0x1 (ACTIVE): Master not idle;0x0 (IDLE): Master is idle*/
	uint32_t SLV_ACTIVITY				:1;		/*[6]-0x1 (ACTIVE): Slave not idle;0x0 (IDLE): Slave is idle*/
	uint32_t MST_HOLD_TX_FIFO_EMPTY		:1;		/*[7]-0x1 (ACTIVE): Master holds the bus due to Tx FIFO is empty;0x0 (INACTIVE): Master is not holding the bus or Bus hold is not due to Tx FIFO is empty*/
	uint32_t MST_HOLD_RX_FIFO_FULL		:1;		/*[8]-0x1 (ACTIVE): Master holds the bus due to Rx FIFO is full;0x0 (INACTIVE): Master is not holding the bus or Bus hold is not due to Rx FIFO is full*/
	uint32_t SLV_HOLD_TX_FIFO_EMPTY		:1;		/*[9]-0x1 (ACTIVE): Slave holds the bus due to Tx FIFO is empty;0x0 (INACTIVE): Slave is not holding the bus or Bus hold is not due to Tx FIFO is empty*/
	uint32_t SLV_HOLD_RX_FIFO_FULL		:1;		/*[10]-0x1 (ACTIVE): Slave holds the bus due to Rx FIFO is full;0x0 (INACTIVE): Slave is not holding the bus or Bus hold is not due to Rx FIFO is full*/
	uint32_t SDA_STUCK_NOT_RECOVERED	:1;		/*[11]-0x1 (ACTIVE): SDA Stuck at low is not recovered after recovery mechanism;0x0 (INACTIVE): SDA Stuck at low is recovered after recovery mechanism.*/
	uint32_t RSVD						:20;	/*[31:12]-Reserved*/
}ic_status_bits;

typedef union
{
	uint32_t all;
	ic_status_bits BITS;
}ic_status;

typedef struct
{
	uint32_t IC_SDA_TX_HOLD				:16;  	/*[15:0]-As transmitter,SDA hold time register*/
	uint32_t IC_SDA_RX_HOLD				:8;		/*[23:16]-As receiver,SDA hold time register*/
	uint32_t RSVD						:8;		/*[31:24]-Reserved*/
}ic_sda_hold;

typedef struct
{
	uint32_t RDMAE						:1;  	/*[0]-0x1 (ENABLED): Receive FIFO DMA channel enabled;0x0 (DISABLED): Receive FIFO DMA channel disabled*/
	uint32_t TDMAE						:1;		/*[1]-0x1 (ENABLED): Transmit FIFO DMA channel enabled;0x0 (DISABLED): transmit FIFO DMA channel disabled*/
	uint32_t RSVD						:30;	/*[31:2]-Reserved*/
}ic_dma_cr;

/**
  * @brief I2C-AL
  */
typedef struct {
	__IOM ic_con IC_CON;              					/*offset 0x00,I2C Control Register*/
    __IOM ic_tar IC_TAR;              					/*offset 0x04,I2C Target Address Register*/
    __IOM ic_sar IC_SAR;              					/*offset 0x08,I2C Slave Address Register*/
    __IOM uint32_t RSVD_0;              				/*offset 0x0c*/
    __IOM ic_data_cmd IC_DATA_CMD;         				/*offset 0x10*/
    __IOM uint32_t IC_SS_SCL_HCNT;      				/*offset 0x14,Standard Speed I2C Clock SCL High Count Register*/
    __IOM uint32_t IC_SS_SCL_LCNT;        	 			/*offset 0x18,Standard Speed I2C Clock SCL Low Count Register*/
    __IOM uint32_t IC_FS_SCL_HCNT;         				/*offset 0x1c,Fast Mode or Fast Mode Plus I2C Clock SCL High Count Register*/
    __IOM uint32_t IC_FS_SCL_LCNT;         				/*offset 0x20,Fast Mode or Fast Mode Plus I2C Clock SCL Low Count Register*/
    __IOM uint32_t IC_HS_SCL_HCNT;              		/*offset 0x24,High Speed I2C Clock SCL High Count Register*/
    __IOM uint32_t IC_HS_SCL_LCNT;              		/*offset 0x28,High Speed I2C Clock SCL Low Count Register*/
    __IOM ic_intr_stat IC_INTR_STAT;              		/*offset 0x2c,I2C Interrupt Status Register*/
    __IOM ic_intr_mask IC_INTR_MASK;         			/*offset 0x30,I2C Interrupt Mask Register*/
    __IOM uint32_t IC_RAW_INTR_STAT;              		/*offset 0x34,I2C Raw Interrupt Status Register*/
    __IOM uint32_t IC_RX_TL;         					/*offset 0x38,I2C Receive FIFO Threshold Register*/
    __IOM uint32_t IC_TX_TL;         					/*offset 0x3c,I2C Transmit FIFO Threshold Register*/
    __IOM ic_clr_intr IC_CLR_INTR;              		/*offset 0x40,Clear Combined and Individual Interrupt Register*/
    __IOM uint32_t RSVD_3[10];              			/*offset 0x44+i*4, i=[0..9]*/
    __IOM ic_enable IC_ENABLE;         					/*offset 0x6c*/
    __IOM ic_status IC_STATUS;              			/*offset 0x70,I2C STATUS Register*/
    __IOM uint32_t IC_TXFLR;              				/*offset 0x74,I2C Transmit FIFO Level Register[read only-ro]*/
    __IOM uint32_t IC_RXFLR;              				/*offset 0x78,I2C Receive FIFO Level Register[ro]*/
    __IOM ic_sda_hold IC_SDA_HOLD;         				/*offset 0x7c,I2C SDA Hold Time Length Register*/
    __IOM uint32_t IC_TX_ABRT_SOURCE;              		/*offset 0x80,I2C Transmit Abort Source Register*/
    __IOM uint32_t IC_SLV_DATA_NACK_ONLY;     			/*offset 0x84,Generate Slave Data NACK Register*/
    __IOM ic_dma_cr IC_DMA_CR;         					/*offset 0x88,DMA Control Register*/
    __IOM uint32_t IC_DMA_TDLR;         				/*offset 0x8c,DMA Transmit Data Level Register*/
    __IOM uint32_t IC_DMA_RDLR;         				/*offset 0x90,DMA Receive Data Level Register*/
    __IOM uint32_t IC_SDA_SETUP;         				/*offset 0x94,I2C SDA Setup Register,only used when operating as a slave transmitter.*/
    __IOM uint32_t IC_ACK_GENERAL_CALL;         		/*offset 0x98,I2C ACK General Call Register*/
    __IOM uint32_t IC_ENABLE_STATUS;       				/*offset 0x9c,I2C Enable Status Register*/
    __IOM uint32_t IC_FS_SPKLEN;         				/*offset 0xA0,I2C SS, FS or FM+ spike suppression limit.*/
    __IOM uint32_t RSVD_7[2];              				/*offset 0xA4+i*4, i=[0..1]*/
    __IOM uint32_t IC_SCL_STUCK_AT_LOW_TIMEOUT;         /*offset 0xAC,I2C SCL Stuck at Low Timeout register*/
    __IOM uint32_t IC_SDA_STUCK_AT_LOW_TIMEOUT;         /*offset 0xB0,I2C SDA Stuck at Low Timeout register*/
} I2C_TypeDef;


/*
 *
 * AL_uart
 *
 */
typedef struct{
	volatile uint32_t RBR_THR_DLL; 	//0x00
	volatile uint32_t DLH_IER;	//0x04
	volatile uint32_t IIR_FCR;	//0x08
	volatile uint32_t LCR;		//0x0c
	volatile uint32_t MCR;		//0x10
	volatile uint32_t LSR;		//0X14
	volatile uint32_t MSR;		//0X18
	volatile uint32_t SCR;		//0x1c
	volatile uint32_t reserved_reg1[20];  //0x20-0x70
	volatile uint32_t FAR;		//0x70
	volatile uint32_t TFR;		//0x74
	volatile uint32_t RFW;		//0x78
	volatile uint32_t USR;		//0x7c
	volatile uint32_t TFL;		//0x80
	volatile uint32_t RFL;		//0x84
	volatile uint32_t resv_reg2[7];
	volatile uint32_t HTX;
	volatile uint32_t DMASA;
	volatile uint32_t CPR;
}AL_UART_TypeDef;

/*
 *
 * al9000_wdt
 *
 */
typedef struct{

    __IOM uint32_t CR;  		// 0x00
    __IOM uint32_t TORR;     		// 0x04
    __IM  uint32_t CCVR;      		// 0x08
    __IOM uint32_t CRR;         	// 0x0c
    __IM  uint32_t STAT;          	// 0x10
    __IM  uint32_t EOI;         	// 0X14
    __IOM uint32_t reserved_reg1;	// 0X18
    __IOM uint32_t PROT_LEVEL;		// 0X1c
    __IOM uint32_t reserved_reg2[49]; 	// 0x20-0xe4
    __IM  uint32_t COMP_PARAMS_5;	// 0xe4
    __IM  uint32_t COMP_PARAMS_4;       // 0xe8
    __IM  uint32_t COMP_PARAMS_3;       // 0xec
    __IM  uint32_t COMP_PARAMS_2;       // 0xf0
    __IM  uint32_t COMP_PARAMS_1;       // 0xf4
    __IM  uint32_t COMP_VERSION;        // 0xf8
    __IM  uint32_t COMP_TYPE;           // 0xfc


}WDT_TypeDef;

/*@}*/ /* end of group demosoc_Peripherals */


/**
  * @brief Ethernet MAC
  */
typedef struct {
	__IOM uint32_t MAC_Configuration;
	__IOM uint32_t MAC_Ext_Configuration;
	__IOM uint32_t MAC_Packet_Filter;
	__IOM uint32_t MAC_Watchdog_Timeout;  //0xc
	   // __IOM uint32_t reserved_re[19];     //offset 0x10+i*0x4, i=[0..35]*/
	__IOM uint32_t reserved_re1;          //0x10
	__IOM uint32_t reserved_re2;          //0x14
	__IOM uint32_t reserved_re3;          //0x18
	__IOM uint32_t reserved_re4;          //0x1c
	__IOM uint32_t reserved_re5;          //0x20
	__IOM uint32_t reserved_re6;          //0x24
	__IOM uint32_t reserved_re7;          //0x28
	__IOM uint32_t reserved_re8;          //0x2c
	__IOM uint32_t reserved_re9;          //0x30
	__IOM uint32_t reserved_re10;         //0x34
	__IOM uint32_t reserved_re11;         //0x38
	__IOM uint32_t reserved_re12;        //0x3c
	__IOM uint32_t reserved_re13;          //0x40
	__IOM uint32_t reserved_re14;         //0x44
	__IOM uint32_t reserved_re15;         //0x48
	__IOM uint32_t reserved_re16;        //0x4c
	__IOM uint32_t MAC_VLAN_Tag;       //0x50
	__IOM uint32_t MAC_VLAN_Tag_Data;  //0x54

} ETH_MACTypeDef;

typedef struct {
    __IOM uint32_t MAC_Interrupt_Status;
    __IOM uint32_t MAC_Interrupt_Enable;
    __IOM uint32_t MAC_Rx_Tx_Status;
} ETH_InterruptTypeDef;


typedef struct {
	__IOM uint32_t  MAC_PHYIF_Control_Status;
}ETH_PhyifctrlstatTypeDef;

typedef struct {
	__IOM uint32_t  MAC_Version;
	__IOM uint32_t  MAC_Debug;
	__IOM uint32_t  Reserved2;
	__IOM uint32_t  MAC_HW_Feature0;
	__IOM uint32_t  MAC_HW_Feature1;
	__IOM uint32_t  MAC_HW_Feature2;
	__IOM uint32_t  MAC_HW_Feature3;
}ETH_HwTypeDef;


typedef struct {
	__IOM uint32_t  MAC_MDIO_Address;
	__IOM uint32_t  MAC_MDIO_Data;
	__IOM uint32_t  Reserved2;
	__IOM uint32_t  MAC_ARP_Address;
}ETH_MdioTypeDef;


typedef struct {
	__IOM uint32_t  MAC_Address0_High;   //0x300
	__IOM uint32_t  MAC_Address0_Low;    //0x304
	__IOM uint32_t  MAC_Address1_High;   //0x308
	__IOM uint32_t  MAC_Address1_Low;    //0x30c
}ETH_MACAddrTypeDef;

typedef struct {
    __IOM uint32_t MMC_Control;
    __IOM uint32_t MMC_Rx_Interrupt;
    __IOM uint32_t MMC_Tx_Interrupt;
    __IOM uint32_t MMC_Rx_Interrupt_Mask;
    __IOM uint32_t MMC_Tx_Interrupt_Mask;
    __IOM uint32_t Tx_Octet_Count_Good_Bad;
    __IOM uint32_t Tx_Packet_Count_Good_Bad;
    __IOM uint32_t Tx_Broadcast_Packets_Good;
    __IOM uint32_t Tx_Multicast_Packets_Good;
    __IOM uint32_t Tx_64Octets_Packets_Good_Bad;
    __IOM uint32_t Tx_65To127Octets_Packets_Good_Bad;
    __IOM uint32_t Tx_128To255Octets_Packets_Good_Bad;
    __IOM uint32_t Tx_256To511Octets_Packets_Good_Bad;
    __IOM uint32_t Tx_512To1023Octets_Packets_Good_Bad;
    __IOM uint32_t Tx_1024ToMaxOctets_Packets_Good_Bad;
    __IOM uint32_t Tx_Unicast_Packets_Good_Bad;
    __IOM uint32_t Tx_Multicast_Packets_Good_Bad;
    __IOM uint32_t Tx_Broadcast_Packets_Good_Bad;
    __IOM uint32_t Tx_Underflow_Error_Packets;
    __IOM uint32_t  Reserd1;              //0X74C
    __IOM uint32_t  Reserd2;   //0X750
    __IOM uint32_t  Reserd3;  //0X754
    __IOM uint32_t  Reserd4;  //0X758
    __IOM uint32_t  Reserd5;  //0X75C
    __IOM uint32_t  Reserd6;  //0X760
    __IOM uint32_t  Tx_Octet_Count_Good;//0X764
} ETH_MMCTypeDef;


typedef struct {
	__IOM uint32_t MTL_Operation_Mode;

}ETH_MTLTypeDef;


typedef struct {
	__IOM uint32_t MTL_TxQ0_Operation_Mode;
	__IOM uint32_t MTL_TxQ0_Underflow;
	__IOM uint32_t MTL_TxQ0_Debug;
	__IOM uint32_t  Reserd7;  //0Xd0c
	__IOM uint32_t  Reserd8;  //0Xd10
	__IOM uint32_t  Reserd9;  //0Xd14
	__IOM uint32_t  Reserd10;  //0Xd18
	__IOM uint32_t  Reserd11;  //0Xd1c
	__IOM uint32_t  Reserd12;  //0Xd20
	__IOM uint32_t  Reserd13;  //0Xd24
	__IOM uint32_t  Reserd14;  //0Xd28
	__IOM uint32_t  MTL_Q0_Interrupt_Control_Status;  //0Xd2c
	__IOM uint32_t  MTL_RxQ0_Operation_Mode;  //0Xd30
	__IOM uint32_t  MTL_RxQ0_Missed_Packet_Overflow_Cnt;  //0Xd34
	__IOM uint32_t  MTL_RxQ0_Debug;
}ETH_MTLQ0TypeDef;



typedef struct {
    __IOM uint32_t DMA_Mode;
    __IOM uint32_t DMA_SysBus_Mode;
    __IOM uint32_t DMA_Interrupt_Status;
    __IOM uint32_t DMA_Debug_Status0;//0X100C
}ETH_DMATypeDef;

typedef struct {
	 __IOM uint32_t DMA_CH0_Control;             //0X1100
	 __IOM uint32_t DMA_CH0_Tx_Control;
	 __IOM uint32_t DMA_CH0_Rx_Control;
	 __IOM uint32_t Reserd15;                //0X110c
	 __IOM uint32_t Reserd16;                //0X1110
	 __IOM uint32_t DMA_CH0_TxDesc_List_Address; //0X1114
	 __IOM uint32_t Reserd17;                 //0X1118
	 __IOM uint32_t DMA_CH0_RxDesc_List_Address; //
	 __IOM uint32_t DMA_CH0_TxDesc_Tail_Pointer; //0x1120
	 __IOM uint32_t Reserd18;                //0X1124
	 __IOM uint32_t DMA_CH0_RxDesc_Tail_Pointer; //0x1128
	 __IOM uint32_t DMA_CH0_TxDesc_Ring_Length; //0x112c
	 __IOM uint32_t DMA_CH0_RxDesc_Ring_Length; //0x1130
	 __IOM uint32_t DMA_CH0_Interrupt_Enable;   //0x1134
	 __IOM uint32_t DMA_CH0_Rx_Interrupt_Watchdog_Timer;//0x1138
	 __IOM uint32_t Reserd19;
	 __IOM uint32_t Reserd20;
	 __IOM uint32_t DMA_CH0_Current_App_TxDesc;//0x1144
	 __IOM uint32_t Reserd21;
	 __IOM uint32_t DMA_CH0_Current_App_RxDesc;//0x114c
	 __IOM uint32_t Reserd22;
	 __IOM uint32_t DMA_CH0_Current_App_TxBuffer; //0x1154
	 __IOM uint32_t Reserd23;
	 __IOM uint32_t DMA_CH0_Current_App_RxBuffer;//0x115c
	 __IOM uint32_t DMA_CH0_Status;//0x1160
	 __IOM uint32_t DMA_CH0_Miss_Frame_Cnt;
	 __IOM uint32_t DMA_CH0_RX_ERI_Cnt;
}ETH_DMACHTypeDef;






typedef struct {
    __IOM uint32_t PTP_ID;
    __IOM uint32_t PTP_Version;
    __IOM uint32_t PTP_CLK_NS;
    __IOM uint32_t PTP_CLK_SEC_L;
    __IOM uint32_t PTP_CLK_SEC_H;
    __IOM uint32_t PTP_CLK_CTRL_0;
    __IOM uint32_t PTP_CLK_CTRL_1;
    __IOM uint32_t PTP_CLK_CTRL_2;
    __IOM uint32_t PTP_CLK_UPDT_NSES;
    __IOM uint32_t PTP_CLK_UPDT_SEC_L;
    __IOM uint32_t PTP_CLK_UPDT_SEC_H;
    __IOM uint32_t PPS_CTRL;
} ETH_PTPTypeDef;

typedef struct {
    __IOM uint32_t PPSX_WAVE_LOW_TIME;
    __IOM uint32_t PPSX_WAVE_HIGH_TIME;
    __IOM uint32_t PPSX_TRIG_TIME_NSEC;
    __IOM uint32_t PPSX_TRIG_TIME_NSEC_LO;
    __IOM uint32_t PPSX_TRIG_TIME_NSEC_HI;
} ETH_PPSXTypeDef;

typedef struct {
    __IOM uint32_t SWT_Version;
    __IOM uint32_t SWT_ID;
    __IOM uint32_t SWT_LUT_ADDR;
    __IOM uint32_t SWT_LUT_ENTRY_H;
    __IOM uint32_t SWT_LUT_ENTRY_MH;
    __IOM uint32_t SWT_LUT_ENTRY_ML;
    __IOM uint32_t SWT_LUT_ENTRY_L;
} ETH_SWT_LUTTypeDef;
/*@}*/ /* end of group demosoc_Peripherals */

typedef struct{
	volatile uint32_t INT_EN			:1; 	/*[0]-Interrupt Enable Bit */
	volatile uint32_t DST_TR_WIDTH		:3; 	/*[3:1]-Destination Transfer Width */
    volatile uint32_t SRC_TR_WIDTH		:3; 	/*[6:4]-Source Transfer Width */
	volatile uint32_t DINC			:2; 	/*[8:7]-Destination Address Increment */
	volatile uint32_t SINC			:2; 	/*[10:9]-Source Address Increment */
	volatile uint32_t DEST_MSIZE		:3; 	/*[13:11]-Destination Burst Transaction Length */
	volatile uint32_t SRC_MSIZE		:3; 	/*[16:14]-Source Burst Transaction Length */
	volatile uint32_t SRC_GATHER_EN		:1; 	/*[17]-Source gather enable */
	volatile uint32_t DST_SCATTER_EN 	:1; 	/*[18]-Destination scatter enable */
	volatile uint32_t Rsvd_CTL		:1;	/*[19]-Reserved field - read-only */
	volatile uint32_t TT_FC			:3; 	/*[22:20]-Transfer Type and Flow Control */
	volatile uint32_t DMS			:2; 	/*[24:23]-Destination Master Select */
	volatile uint32_t SMS			:2;	/*[26:25]-Source Master Select */
	volatile uint32_t LLP_DST_EN 		:1;	/*[27]-Block chaining is enabled on the destination side*/
	volatile uint32_t LLP_SRC_EN		:1;	/*[28]-Block chaining is enabled on the source side */
	volatile uint32_t Rsvd_1_CTL    		:3;	/*[31:29]-RSVD */
}parm_l;

typedef struct{
	volatile uint32_t BLOCK_TS       	:12;	/*[11:0]-RSVD */
	volatile uint32_t DONE           	:1;	/*[12]-RSVD */
	volatile uint32_t Rsvd_3_CTL     	:19;	/*[31:13]-RSVD */
}parm_h;
typedef struct{
	volatile uint32_t Rsvd_CFG		:5;	/*[4:0]-RSVD */
	volatile uint32_t CH_PRIOR		:3;	/*[7:5]-RSVD */
	volatile uint32_t CH_SUSP		:1;	/*[8]-RSVD */
	volatile uint32_t FIFO_EMPTY		:1;	/*[9]-RSVD */
	volatile uint32_t HS_SEL_DST		:1;	/*[10]-RSVD */
	volatile uint32_t HS_SEL_SRC		:1;	/*[11]-RSVD */
	volatile uint32_t LOCK_CH_L		:2;	/*[13:12]-RSVD */
	volatile uint32_t LOCK_B_L		:2;	/*[15:14]-RSVD */
	volatile uint32_t LOCK_CH		:1;	/*[16]-RSVD */
	volatile uint32_t LOCK_B			:1;	/*[17]-RSVD */
	volatile uint32_t DST_HS_POL		:1;	/*[18]-RSVD */
	volatile uint32_t SRC_HS_POL		:1;	/*[19]-RSVD */
	volatile uint32_t MAX_ABRST		:10;	/*[29:20]-RSVD */
	volatile uint32_t RELOAD_SRC		:1;	/*[30]-RSVD */
	volatile uint32_t RELOAD_DST		:1;	/*[31]-RSVD */
}config_l;
typedef struct{
	volatile uint32_t FCMODE			:1;	/*[0]-RSVD */
	volatile uint32_t FIFO_MODE		:1;	/*[1]-RSVD */
	volatile uint32_t PROTCTL		:3;	/*[4:2]-RSVD */
	volatile uint32_t DS_UPD_EN		:1;	/*[5]-RSVD */
	volatile uint32_t SS_UPD_EN		:1;	/*[6]-RSVD */
	volatile uint32_t SRC_PER		:4;	/*[10:7]-RSVD */
	volatile uint32_t DEST_PER		:4;	/*[14:11]-RSVD */
	volatile uint32_t Rsvd_3_CFG		:17;	/*[31:15]-RSVD */

}config_h;
typedef struct{
	volatile uint32_t LMS   			:2;	/*[0:1]*/
	volatile uint32_t LOC			:30;	/*[2:31]*/
}LLP_PARM;
/*
 *
 * AL_dma_register_struct
 *
 */
typedef struct{
	/*
	 * DMAC/Channel_x_Registers Registers
	 */
	volatile uint32_t SAR_L;						/*offset 0x0*/
	volatile uint32_t SAR_H;						/*offset 0x4*/
	volatile uint32_t DAR_L;						/*offset 0x8*/
	volatile uint32_t DAR_H;						/*offset 0x0c*/
	volatile LLP_PARM LLP_L;						/*offset 0x10*/
	volatile uint32_t LLP_H;						/*offset 0x14*/
	volatile parm_l   CTL_L;						/*offset 0x18*/
	volatile parm_h   CTL_H;						/*offset 0x1c*/
	volatile uint32_t SSTAT_L;					/*offset 0x20*/
	volatile uint32_t SSTAT_H;					/*offset 0x24*/
	__IOM uint32_t DSTAT_L;					/*offset 0x28*/
	__IOM uint32_t DSTAT_H;					/*offset 0x2c*/
	__IOM uint32_t SSTATA_L;					/*offset 0x30*/
	__IOM uint32_t SSTATA_H;					/*offset 0x34*/
	__IOM uint32_t DSTATA_L;					/*offset 0x38*/
	__IOM uint32_t DSTATA_H;					/*offset 0x3c*/
	volatile config_l CFG_L;					        /*offset 0x40*/
	volatile config_h CFG_H;						/*offset 0x44*/
	__IOM uint32_t SGR_L;						/*offset 0x48*/
	__IOM uint32_t SGR_H;						/*offset 0x4c*/
	__IOM uint32_t DSR_L;						/*offset 0x50*/
	__IOM uint32_t DSR_H;						/*offset 0x54*/
}DMA_Channel_TypeDef;
typedef struct{
	/*
	 * DMAC/Interrupt_Registers Registers
	 */
	__IOM uint32_t RawTfer_L;			/*offset 0x2c0*/
	__IOM uint32_t RawTfer_H;					/*offset 0x2c4*/
	__IOM uint32_t RawBlock_L;			/*offset 0x2c8*/
	__IOM uint32_t RawBlock_H;				/*offset 0x2cc*/
	__IOM uint32_t RawSrcTran_L;			/*offset 0x2d0*/
	__IOM uint32_t RawSrcTran_H;				/*offset 0x2d4*/
	__IOM uint32_t RawDstTran_L;			/*offset 0x2d8*/
	__IOM uint32_t RawDstTran_H;				/*offset 0x2dc*/
	__IOM uint32_t RawErr_L;				/*offset 0x2e0*/
	__IOM uint32_t RawErr_H;					/*offset 0x2e4*/
	__IOM uint32_t StatusTfer_L;			/*offset 0x2e8*/
	__IOM uint32_t StatusTfer_H;				/*offset 0x2ec*/
	__IOM uint32_t StatusBlock_L;		/*offset 0x2f0*/
	__IOM uint32_t StatusBlock_H;				/*offset 0x2f4*/
	__IOM uint32_t StatusSrcTran_L;		/*offset 0x2f8*/
	__IOM uint32_t StatusSrcTran_H;			/*offset 0x2fc*/
	__IOM uint32_t StatusDstTran_L;		/*offset 0x300*/
	__IOM uint32_t StatusDstTran_H;			/*offset 0x304*/
	__IOM uint32_t StatusErr_L;			/*offset 0x308*/
	__IOM uint32_t StatusErr_H;				/*offset 0x30c*/
	__IOM uint32_t MaskTfer_L;			/*offset 0x310*/
	__IOM uint32_t MaskTfer_H;				/*offset 0x314*/
	__IOM uint32_t MaskBlock_L;			/*offset 0x318*/
	__IOM uint32_t MaskBlock_H;			    /*offset 0x31c*/
	__IOM uint32_t MaskSrcTran_L;		/*offset 0x320*/
	__IOM uint32_t MaskSrcTran_H;				/*offset 0x324*/
	__IOM uint32_t MaskDstTran_L;		/*offset 0x328*/
	__IOM uint32_t MaskDstTran_H;			    /*offset 0x32c*/
	__IOM uint32_t MaskErr_L;			/*offset 0x330*/
	__IOM uint32_t MaskErr_H;					/*offset 0x334*/
	__IOM uint32_t ClearTfer_L;			/*offset 0x338*/
	__IOM uint32_t ClearTfer_H;				/*offset 0x33c*/
	__IOM uint32_t ClearBlock_L;			/*offset 0x340*/
	__IOM uint32_t ClearBlock_H;				/*offset 0x344*/
	__IOM uint32_t ClearSrcTran_L;		/*offset 0x348*/
	__IOM uint32_t ClearSrcTran_H;			/*offset 0x34c*/
	__IOM uint32_t ClearDstTran_L;		/*offset 0x350*/
	__IOM uint32_t ClearDstTran_H;			/*offset 0x354*/
	__IOM uint32_t ClearErr_L;			/*offset 0x358*/
	__IOM uint32_t ClearErr_H;				/*offset 0x35c*/
	__IOM uint32_t StstusInt_L;			/*offset 0x360*/
	__IOM uint32_t StstusInt_H;				/*offset 0x364*/
	/*
	 * DMAC/Software_Handshake_Registers Registers
	 */
	__IOM uint32_t ReqSrcReg_L;			/*offset 0x368*/
	__IOM uint32_t ReqSrcReg_H;				/*offset 0x36c*/
	__IOM uint32_t ReqDstReg_L;			/*offset 0x370*/
	__IOM uint32_t ReqDstReg_H;				/*offset 0x374*/
	__IOM uint32_t SglRqSrcReg_L;		/*offset 0x378*/
	__IOM uint32_t SglRqSrcReg_H;				/*offset 0x37c*/
	__IOM uint32_t SglRqDstReg_L;		/*offset 0x380*/
	__IOM uint32_t SglRqDstReg_H;				/*offset 0x384*/
	__IOM uint32_t LstSrcReg_L;			/*offset 0x388*/
	__IOM uint32_t LstSrcReg_H;				/*offset 0x38c*/
	__IOM uint32_t LstDstReg_L;			/*offset 0x390*/
	__IOM uint32_t LstDstReg_H;				/*offset 0x394*/
	/*
	 * DMAC/Miscellaneous_Registers Registers
	 */
	__IOM uint32_t DmaCfgReg_L;			/*offset 0x398*/
	__IOM uint32_t DmaCfgReg_H;				/*offset 0x39c*/
	__IOM uint32_t ChEnReg_L;			/*offset 0x3a0*/
	__IOM uint32_t ChEnReg_H;					/*offset 0x3a4*/
	__IOM uint32_t DmaIdReg_L;			/*offset 0x3a8*/
	__IOM uint32_t DmaIdReg_H;				/*offset 0x3ac*/
	__IOM uint32_t DmaTestReg_L;			/*offset 0x3b0*/
	__IOM uint32_t DmaTestReg_H;				/*offset 0x3b4*/
	__IOM uint32_t DmaLpTimeoutReg_L;	/*offset 0x3b8*/
	__IOM uint32_t DmaLpTimeoutReg_H;			/*offset 0x3bc*/
	__IOM uint32_t DMA_rsvd_reg[2];			/*offset 0x3bc*/
	__IOM uint32_t Dma_COMP_PARAMS_6_L;	/*offset 0x3c8*/
	__IOM uint32_t Dma_COMP_PARAMS_6_H;		/*offset 0x3cc*/
	__IOM uint32_t Dma_COMP_PARAMS_5_L;	/*offset 0x3d0*/
	__IOM uint32_t Dma_COMP_PARAMS_5_H;		/*offset 0x3d4*/
	__IOM uint32_t Dma_COMP_PARAMS_4_L;	/*offset 0x3d8*/
	__IOM uint32_t Dma_COMP_PARAMS_4_H;		/*offset 0x3dc*/
	__IOM uint32_t Dma_COMP_PARAMS_3_L;	/*offset 0x3e0*/
	__IOM uint32_t Dma_COMP_PARAMS_3_H;		/*offset 0x3e4*/
	__IOM uint32_t Dma_COMP_PARAMS_2_L;	/*offset 0x3e8*/
	__IOM uint32_t Dma_COMP_PARAMS_2_H;		/*offset 0x3ec*/
	__IOM uint32_t Dma_COMP_PARAMS_1_L;	/*offset 0x3f0*/
	__IOM uint32_t Dma_COMP_PARAMS_1_H;		/*offset 0x3f4*/
	__IOM uint32_t DmaCompsID_L;			/*offset 0x3f8*/
	__IOM uint32_t DmaCompsID_H;				/*offset 0x3fc*/
}AL_DMAC_TypeDef;
typedef struct
{
   __IOM uint32_t RBUF_ID_PARM;							/*offset 0x00*/
   __IOM uint32_t RBUF_CTL;								/*offset 0x04*/
   __IOM uint32_t RBUF_DATA[16];						/*offset 0x08*/
   __IOM uint8_t RBUF_CiA_603[8];						/*offset 0x48 - 0x4f*/
   __IOM uint32_t TBUF_ID_PARM;							/*offset 0x50*/
   __IOM uint32_t TBUF_CTL;								/*offset 0x54*/
   //__IOM uint8_t revers[3];
   __IOM uint32_t TBUF_DATA[16];						/*offset 0x58 - 0x97*/
   __IOM uint8_t TTS[8];
   __IOM uint32_t CFG_STAT_TCMD_TCTRL_RCTRL;			/*offset 0xA0 */
   __IOM uint32_t RTIE_RTIF_ERRINT_LIMIT;				/*offset 0xA4 */
   __IOM uint32_t S_Seg_1_S_Seg_2_S_SJW_S_PRESC;		/*offset 0xA8 */
   __IOM uint32_t F_Seg_1_F_Seg_2_F_SJW_F_PRESC;		/*offset 0xAC */
   __IOM uint32_t EALCAP_TDC_RECNT_TECNT;				/*offset 0xB0 */
   __IOM uint32_t ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1;	/*offset 0xB4 */
   __IOM uint32_t ACF_0123 ;
   __IOM uint8_t VER_0;
   __IOM uint8_t VER_1;
   __IOM uint8_t TBSLOT;
   __IOM uint8_t TTCFG;
   __IOM uint8_t REF_MSG_0;
   __IOM uint8_t REF_MSG_1;
   __IOM uint8_t REF_MSG_2;
   __IOM uint8_t REF_MSG_3;
   __IOM uint8_t TRIG_CFG_0;
   __IOM uint8_t TRIG_CFG_1;
   __IOM uint8_t TT_TRIG_0;
   __IOM uint8_t TT_TRIG_1;
   __IOM uint8_t TT_WTRIG_0;
   __IOM uint8_t TT_WTRIG_1;
   __IOM uint8_t MEM_PROT;
   __IOM uint8_t MEM_STAT;
   __IOM uint8_t MEM_ES_0;
   __IOM uint8_t MEM_ES_1;
   __IOM uint8_t MEM_ES_2;
   __IOM uint8_t MEM_ES_3;
   __IOM uint8_t SCFG;
} AL_CAN_TypeDef;


#define XEC0_MAC_BASE                 (XEC0_BASE)
#define XEC0_Interrupt_BASE           (XEC0_BASE+0xb0UL)
#define XEC0_Phyifctrlstat_BASE       (XEC0_BASE+0xf8UL)
#define XEC0_Hw_BASE                  (XEC0_BASE+0x110UL)
#define XEC0_Mdio_BASE                (XEC0_BASE+0x200UL)
#define XEC0_MACAddr_BASE             (XEC0_BASE+0x300UL)
#define XEC0_MMC_BASE                 (XEC0_BASE+0x700UL)

#define XEC0_EQOS_MTL_BASE            (XEC0_BASE+0xc00UL)
#define XEC0_EQOS_MTL_Q0_BASE         (XEC0_BASE+0xd00UL)

#define XEC0_DMA_BASE                 (XEC0_BASE+0x01000UL)
#define XEC0_DMA_CH_BASE              (XEC0_BASE+0x01100UL)


//#define XEC0_IOC_BASE                 (XEC0_BASE+0x0800UL)
//#define XEC0_IOC_CHX_BASE             (XEC0_BASE+0x0820UL)
//#define XEC0_MAC_BASE                 (XEC0_BASE+0x0C00UL)
//#define XEC0_PTP_BASE                 (XEC0_BASE+0x1400UL)
//#define XEC0_PPSX_BASE                (XEC0_BASE+0x1420UL)
//#define XEC0_SWT_LUT_BASE             (XEC0_BASE+0x1000UL)

#define XEC0_MAC                      ((ETH_MACTypeDef *)XEC0_MAC_BASE)                  //mac
#define XEC0_Interrupt                ((ETH_InterruptTypeDef *)XEC0_Interrupt_BASE)
#define XEC0_Phyifctrlstat            ((ETH_PhyifctrlstatTypeDef *)XEC0_Phyifctrlstat_BASE)
#define XEC0_Hw                       ((XEC0_HwTypeDef*)XEC0_BASE)
#define XEC0_Mdio                     ((ETH_MdioTypeDef *)XEC0_Mdio_BASE)
#define XEC0_MACAddr                  ((ETH_MACAddrTypeDef *)XEC0_MACAddr_BASE)
#define XEC0_MMC                      ((ETH_MMCTypeDef *)XEC0_MMC_BASE)

#define XEC0_EQOS_MTL                 ((ETH_MTLTypeDef *)XEC0_EQOS_MTL_BASE)
#define XEC0_EQOS_MTL_Q0              ((ETH_MTLQ0TypeDef *)XEC0_EQOS_MTL_Q0_BASE)

#define XEC0_DMA                      ((ETH_DMATypeDef *)XEC0_DMA_BASE)
#define XEC0_DMA_CH                   ((ETH_DMACHTypeDef *)XEC0_DMA_CH_BASE)

//----------------------XEC1------------------------------------------------------

/**
  * @brief Ethernet MAC
  */
typedef struct {
    __IOM uint32_t MAC_Configuration;
    __IOM uint32_t MAC_Ext_Configuration;
    __IOM uint32_t MAC_Packet_Filter;
    __IOM uint32_t MAC_Watchdog_Timeout;  //0xc
   // __IOM uint32_t reserved_re[19];     //offset 0x10+i*0x4, i=[0..35]*/
    __IOM uint32_t reserved_re1;          //0x10
    __IOM uint32_t reserved_re2;          //0x14
    __IOM uint32_t reserved_re3;          //0x18
    __IOM uint32_t reserved_re4;          //0x1c
    __IOM uint32_t reserved_re5;          //0x20
    __IOM uint32_t reserved_re6;          //0x24
    __IOM uint32_t reserved_re7;          //0x28
    __IOM uint32_t reserved_re8;          //0x2c
    __IOM uint32_t reserved_re9;          //0x30
    __IOM uint32_t reserved_re10;         //0x34
    __IOM uint32_t reserved_re11;         //0x38
     __IOM uint32_t reserved_re12;        //0x3c
     __IOM uint32_t reserved_re13;          //0x40
     __IOM uint32_t reserved_re14;         //0x44
     __IOM uint32_t reserved_re15;         //0x48
      __IOM uint32_t reserved_re16;        //0x4c
    __IOM uint32_t MAC_VLAN_Tag;       //0x50
    __IOM uint32_t MAC_VLAN_Tag_Data;  //0x54

} ETH1_MACTypeDef;

typedef struct {
    __IOM uint32_t MAC_Interrupt_Status;
    __IOM uint32_t MAC_Interrupt_Enable;
    __IOM uint32_t MAC_Rx_Tx_Status;
} ETH1_InterruptTypeDef;


typedef struct {
	__IOM uint32_t  MAC_PHYIF_Control_Status;
}ETH1_PhyifctrlstatTypeDef;

typedef struct {
	__IOM uint32_t  MAC_Version;
	__IOM uint32_t  MAC_Debug;
	__IOM uint32_t  Reserved2;
	__IOM uint32_t  MAC_HW_Feature0;
	__IOM uint32_t  MAC_HW_Feature1;
	__IOM uint32_t  MAC_HW_Feature2;
	__IOM uint32_t  MAC_HW_Feature3;
}ETH1_HwTypeDef;


typedef struct {
	__IOM uint32_t  MAC_MDIO_Address;
	__IOM uint32_t  MAC_MDIO_Data;
	__IOM uint32_t  Reserved2;
	__IOM uint32_t  MAC_ARP_Address;
}ETH1_MdioTypeDef;


typedef struct {
	__IOM uint32_t  MAC_Address0_High;   //0x300
	__IOM uint32_t  MAC_Address0_Low;    //0x304
	__IOM uint32_t  MAC_Address1_High;   //0x308
	__IOM uint32_t  MAC_Address1_Low;    //0x30c

}ETH1_MACAddrTypeDef;

typedef struct {
    __IOM uint32_t MMC_Control;
    __IOM uint32_t MMC_Rx_Interrupt;
    __IOM uint32_t MMC_Tx_Interrupt;
    __IOM uint32_t MMC_Rx_Interrupt_Mask;
    __IOM uint32_t MMC_Tx_Interrupt_Mask;
    __IOM uint32_t Tx_Octet_Count_Good_Bad;
    __IOM uint32_t Tx_Packet_Count_Good_Bad;
    __IOM uint32_t Tx_Broadcast_Packets_Good;
    __IOM uint32_t Tx_Multicast_Packets_Good;
    __IOM uint32_t Tx_64Octets_Packets_Good_Bad;
    __IOM uint32_t Tx_65To127Octets_Packets_Good_Bad;
    __IOM uint32_t Tx_128To255Octets_Packets_Good_Bad;
    __IOM uint32_t Tx_256To511Octets_Packets_Good_Bad;
    __IOM uint32_t Tx_512To1023Octets_Packets_Good_Bad;
    __IOM uint32_t Tx_1024ToMaxOctets_Packets_Good_Bad;
    __IOM uint32_t Tx_Unicast_Packets_Good_Bad;
    __IOM uint32_t Tx_Multicast_Packets_Good_Bad;
    __IOM uint32_t Tx_Broadcast_Packets_Good_Bad;
    __IOM uint32_t Tx_Underflow_Error_Packets;
    __IOM uint32_t  Reserd1;              //0X74C
    __IOM uint32_t  Reserd2;   //0X750
    __IOM uint32_t  Reserd3;  //0X754
    __IOM uint32_t  Reserd4;  //0X758
    __IOM uint32_t  Reserd5;  //0X75C
    __IOM uint32_t  Reserd6;  //0X760
    __IOM uint32_t  Tx_Octet_Count_Good;//0X764
} ETH1_MMCTypeDef;


typedef struct {
	__IOM uint32_t MTL_Operation_Mode;

}ETH1_MTLTypeDef;


typedef struct {
	__IOM uint32_t MTL_TxQ0_Operation_Mode;
	__IOM uint32_t MTL_TxQ0_Underflow;
	__IOM uint32_t MTL_TxQ0_Debug;
	__IOM uint32_t  Reserd7;  //0Xd0c
	__IOM uint32_t  Reserd8;  //0Xd10
	__IOM uint32_t  Reserd9;  //0Xd14
	__IOM uint32_t  Reserd10;  //0Xd18
	__IOM uint32_t  Reserd11;  //0Xd1c
	__IOM uint32_t  Reserd12;  //0Xd20
	__IOM uint32_t  Reserd13;  //0Xd24
	__IOM uint32_t  Reserd14;  //0Xd28
	__IOM uint32_t  MTL_Q0_Interrupt_Control_Status;  //0Xd2c
	__IOM uint32_t  MTL_RxQ0_Operation_Mode;  //0Xd30
	__IOM uint32_t  MTL_RxQ0_Missed_Packet_Overflow_Cnt;  //0Xd34
	__IOM uint32_t  MTL_RxQ0_Debug;
}ETH1_MTLQ0TypeDef;



typedef struct {
    __IOM uint32_t DMA_Mode;
    __IOM uint32_t DMA_SysBus_Mode;
    __IOM uint32_t DMA_Interrupt_Status;
    __IOM uint32_t DMA_Debug_Status0;//0X100C
}ETH1_DMATypeDef;

typedef struct {
	 __IOM uint32_t DMA_CH0_Control;             //0X1100
	 __IOM uint32_t DMA_CH0_Tx_Control;
	 __IOM uint32_t DMA_CH0_Rx_Control;
	 __IOM uint32_t Reserd15;                //0X110c
	 __IOM uint32_t Reserd16;                //0X1110
	 __IOM uint32_t DMA_CH0_TxDesc_List_Address; //0X1114
	 __IOM uint32_t Reserd17;                 //0X1118
	 __IOM uint32_t DMA_CH0_RxDesc_List_Address; //
	 __IOM uint32_t DMA_CH0_TxDesc_Tail_Pointer; //0x1120
	 __IOM uint32_t Reserd18;                //0X1124
	 __IOM uint32_t DMA_CH0_RxDesc_Tail_Pointer; //0x1128
	 __IOM uint32_t DMA_CH0_TxDesc_Ring_Length; //0x112c
	 __IOM uint32_t DMA_CH0_RxDesc_Ring_Length; //0x1130
	 __IOM uint32_t DMA_CH0_Interrupt_Enable;   //0x1134
	 __IOM uint32_t DMA_CH0_Rx_Interrupt_Watchdog_Timer;//0x1138
	 __IOM uint32_t Reserd19;
	 __IOM uint32_t Reserd20;
	 __IOM uint32_t DMA_CH0_Current_App_TxDesc;//0x1144
	 __IOM uint32_t Reserd21;
	 __IOM uint32_t DMA_CH0_Current_App_RxDesc;//0x114c
	 __IOM uint32_t Reserd22;
	 __IOM uint32_t DMA_CH0_Current_App_TxBuffer; //0x1154
	 __IOM uint32_t Reserd23;
	 __IOM uint32_t DMA_CH0_Current_App_RxBuffer;//0x115c
	 __IOM uint32_t DMA_CH0_Status;//0x1160
	 __IOM uint32_t DMA_CH0_Miss_Frame_Cnt;
	 __IOM uint32_t DMA_CH0_RX_ERI_Cnt;
}ETH1_DMACHTypeDef;


#define XEC1_MAC_BASE                 (XEC1_BASE)
#define XEC1_Interrupt_BASE           (XEC1_BASE+0xb0UL)
#define XEC1_Phyifctrlstat_BASE       (XEC1_BASE+0xf8UL)
#define XEC1_Hw_BASE                  (XEC1_BASE+0x110UL)
#define XEC1_Mdio_BASE                (XEC1_BASE+0x200UL)
#define XEC1_MACAddr_BASE             (XEC1_BASE+0x300UL)
#define XEC1_MMC_BASE                 (XEC1_BASE+0x700UL)

#define XEC1_EQOS_MTL_BASE            (XEC1_BASE+0xc00UL)
#define XEC1_EQOS_MTL_Q0_BASE         (XEC1_BASE+0xd00UL)

#define XEC1_DMA_BASE                 (XEC1_BASE+0x01000UL)
#define XEC1_DMA_CH_BASE              (XEC1_BASE+0x01100UL)

#define XEC1_MAC                      ((ETH1_MACTypeDef *)XEC1_MAC_BASE)                  //mac
#define XEC1_Interrupt                ((ETH1_InterruptTypeDef *)XEC1_Interrupt_BASE)
#define XEC1_Phyifctrlstat            ((ETH1_PhyifctrlstatTypeDef *)XEC1_Phyifctrlstat_BASE)
#define XEC1_Hw                       ((XEC1_HwTypeDef*)XEC1_BASE)
#define XEC1_Mdio                     ((ETH1_MdioTypeDef *)XEC1_Mdio_BASE)
#define XEC1_MACAddr                  ((ETH1_MACAddrTypeDef *)XEC1_MACAddr_BASE)
#define XEC1_MMC                      ((ETH1_MMCTypeDef *)XEC1_MMC_BASE)

#define XEC1_EQOS_MTL                 ((ETH1_MTLTypeDef *)XEC1_EQOS_MTL_BASE)
#define XEC1_EQOS_MTL_Q0              ((ETH1_MTLQ0TypeDef *)XEC1_EQOS_MTL_Q0_BASE)

#define XEC1_DMA                      ((ETH1_DMATypeDef *)XEC1_DMA_BASE)
#define XEC1_DMA_CH                   ((ETH1_DMACHTypeDef *)XEC1_DMA_CH_BASE)

//
//#define XEC0_IOC_CHX                  ((ETH_IOC_CHXTypeDef *)XEC0_IOC_CHX_BASE)
//#define XEC0_IOC                      ((ETH_IOCTypeDef *)XEC0_IOC_BASE)
//#define XEC0_PTP                      ((ETH_PTPTypeDef *)XEC0_PTP_BASE)
//#define XEC0_PPSX                     ((ETH_PPSXTypeDef *)XEC0_PPSX_BASE)

//#define XEC0_PhyPort                  ((ETH_PhyPort_TypeDef *)XEC0_PRT_BASE)






/* =========================================  End of section using anonymous unions  ========================================= */
#if defined (__GNUC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif


/* =========================================================================================================================== */
/* ================                          Device Specific Peripheral Address Map                           ================ */
/* =========================================================================================================================== */


/* ToDo: add here your device peripherals base addresses
         following is an example for timer */
/** @addtogroup Device_Peripheral_peripheralAddr
  * @{
  */
/* Peripheral and SRAM base address */
#define QSPI_FLASH_BASE         (0x70000000UL)      /*!< (FLASH     ) Base Address */
#define ONCHIP_ROM_BASE         (0x61000000UL)      /*!< (ROM       ) Base Address */
#define ONCHIP_ILM_BASE         (0x60000000UL)//(0x80000000UL)      /*!< (ILM       ) Base Address */
#define ONCHIP_DLM_BASE         (0x60040000UL)//(0x90000000UL)      /*!< (DLM       ) Base Address */
//#define DEMOSOC_PERIPH_BASE     (0x10000000UL)//(0x10000000UL)      /*!< (Peripheral) Base Address */
#define PERIPH_BASE     		(0xF8000000UL)					    /*!< (Peripheral) Base Address */
#define MEM_BASE1_ADDR           (0x61030000)
#define MEM_BASE2_ADDR           (0x61030100)

#define WDT_BASE                  	(0xF8410000)
#define SMC_BASE                  	(0xF841A000UL)

#define XADC_BASE                  	(0xF840B000UL)/* Peripheral memory map */

#define GPIO_BLOCK0_BASE        (0xF8411000UL)          			/*!< (GPIO BLOCK0) Base Address */
#define GPIO_BLOCK1_BASE        (0xF8411100UL)          			/*!< (GPIO BLOCK1) Base Address */
#define GPIO_BLOCK2_BASE        (0xF8411200UL)          			/*!< (GPIO BLOCK2) Base Address */
#define GPIO_BLOCK3_BASE        (0xF8411300UL)          			/*!< (GPIO BLOCK3) Base Address */
/* PPI Interfaced IP */
//#define UART0_BASE              (DEMOSOC_PERIPH_BASE + 0x13000)          /*!< (UART0) Base Address */
//#define PWM0_BASE               (DEMOSOC_PERIPH_BASE + 0x15000)          /*!< (PWM0) Base Address */
//#define UART1_BASE              (DEMOSOC_PERIPH_BASE + 0x23000)          /*!< (UART1) Base Address */
//#define PWM1_BASE               (DEMOSOC_PERIPH_BASE + 0x25000)          /*!< (PWM1) Base Address */
//#define PWM2_BASE               (DEMOSOC_PERIPH_BASE + 0x35000)          /*!< (PWM2) Base Address */
//#define I2C_BASE                (DEMOSOC_PERIPH_BASE + 0x42000)          /*!< (I2C Master) Base Address */
#define AL_TOP_CFG_BASE     		 (0xF8800000)
#define AL_PLPS_RESET                (0xf8801078)
#define AL_GP0                       (0x80000000)
#define AL_GP1                       (0xA0000000)
#define AL_FAHB_BASE				 (0xFC000000)
#define AL_CAN0_BASE                 (0xF8408000)
#define AL_CAN1_BASE                 (0xF8409000)
#define AL_DMAC_channel_1BASE        (0xF804D000)				 			 /*!< (AL_DMAC_channel_1BASE) Base Address */
#define AL_DMAC_channel_2BASE        (0xF804D058)				 			 /*!< (AL_DMAC_channel_2BASE) Base Address */
#define AL_DMAC_channel_3BASE        (0xF804D0B0)				 			 /*!< (AL_DMAC_channel_3BASE) Base Address */
#define AL_DMAC_channel_4BASE        (0xF804D108)				 			 /*!< (AL_DMAC_channel_4BASE) Base Address */
#define AL_DMAC_channel_5BASE        (0xF804D160)				 			 /*!< (AL_DMAC_channel_5BASE) Base Address */
#define AL_DMAC_channel_6BASE        (0xF804D1B8)				 			 /*!< (AL_DMAC_channel_6BASE) Base Address */
#define AL_DMAC_channel_7BASE        (0xF804D210)				 			 /*!< (AL_DMAC_channel_7BASE) Base Address */
#define AL_DMAC_channel_8BASE        (0xF804D268)				 			 /*!< (AL_DMAC_channel_8BASE) Base Address */
#define AL_DMAC_BASE                 (0xF804D2C0)				 			 /*!< (AL_UART0_BASE) Base Address */
#define AL_UART0_BASE       (0xF8400000)				 			         /*!< (AL_UART0) Base Address */
#define AL_UART1_BASE       (0xF8401000)				 			         /*!< (AL_UART1) Base Address */
#define QSPI0_BASE              (PERIPH_BASE + 0x0004E000)	 			 /*!< (QSPI0) Base Address */
#define SPI0_BASE               (PERIPH_BASE + 0x00404000)	 			 /*!< (SPI0) Base Address */
#define SPI1_BASE               (PERIPH_BASE + 0x00405000)	 			 /*!< (SPI1) Base Address */
#define I2C0_BASE               (PERIPH_BASE + 0x00414000)          	 /*!< (I2C0) Base Address */
#define I2C1_BASE               (PERIPH_BASE + 0x00415000)          	 /*!< (I2C1) Base Address */
//QSPI-XIP0 128M
#define QSPI_XIP0_START_ADDR	(0x70000000UL)							/*qspi-xip0 start address*/
#define QSPI_XIP0_END_ADDR		(0x77FFFFFFUL)							/*qspi-xip0 end address*/
//QSPI-XIP1 128M
#define QSPI_XIP1_START_ADDR	(0x78000000UL)							/*qspi-xip1 start address*/
#define QSPI_XIP1_END_ADDR		(0x7FFFFFFFUL)							/*qspi-xip1 end address*/
#define XEC0_BASE               (0xf8100000UL)
#define XEC1_BASE               (0xf8110000UL)
#define DDR_BASE				(0x00000000UL)
/** @} */ /* End of group Device_Peripheral_peripheralAddr */


/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */


/* ToDo: add here your device peripherals pointer definitions
         following is an example for timer */
/** @addtogroup Device_Peripheral_declaration
  * @{
  */
#define GPIO0             		((GPIO_TypeDef *) GPIO_BLOCK0_BASE)
#define GPIO1                   ((GPIO_TypeDef *) GPIO_BLOCK1_BASE)
#define GPIO2                   ((GPIO_TypeDef *) GPIO_BLOCK2_BASE)
#define GPIO3                   ((GPIO_TypeDef *) GPIO_BLOCK3_BASE)

#define UART0                   ((UART_TypeDef *) UART0_BASE)
#define WDT                   	((WDT_TypeDef *)  WDT_BASE)
#define XADC                   	((XADC_TypeDef *)  XADC_BASE)

#define PWM0                    ((PWM_TypeDef *) PWM0_BASE)
#define UART1                   ((UART_TypeDef *) UART1_BASE)
#define PWM1                    ((PWM_TypeDef *) PWM1_BASE)
#define PWM2                    ((PWM_TypeDef *) PWM2_BASE)
#define I2C                     ((I2C_TypeDef *) I2C_BASE)
#define AL_UART0		        ((AL_UART_TypeDef *) AL_UART0_BASE)
#define AL_UART1		        ((AL_UART_TypeDef *) AL_UART1_BASE)
#define AL_CAN0					((AL_CAN_TypeDef *) AL_CAN0_BASE)
#define AL_CAN1					((AL_CAN_TypeDef *) AL_CAN1_BASE)
#define AL_TOP0             	((TOPCFG_TypeDef *) AL_TOP_CFG_BASE  )
#define AL_DMAC             		  ((AL_DMAC_TypeDef *) AL_DMAC_BASE)
#define AL_DMAC_channel_0             ((DMA_Channel_TypeDef *) AL_DMAC_channel_1BASE)
#define AL_DMAC_channel_1             ((DMA_Channel_TypeDef *) AL_DMAC_channel_2BASE)
#define AL_DMAC_channel_2             ((DMA_Channel_TypeDef *) AL_DMAC_channel_3BASE)
#define AL_DMAC_channel_3             ((DMA_Channel_TypeDef *) AL_DMAC_channel_4BASE)
#define AL_DMAC_channel_4             ((DMA_Channel_TypeDef *) AL_DMAC_channel_5BASE)
#define AL_DMAC_channel_5             ((DMA_Channel_TypeDef *) AL_DMAC_channel_6BASE)
#define AL_DMAC_channel_6             ((DMA_Channel_TypeDef *) AL_DMAC_channel_7BASE)
#define AL_DMAC_channel_7             ((DMA_Channel_TypeDef *) AL_DMAC_channel_8BASE)
#define QSPI0			              ((QSPI_TypeDef *) QSPI0_BASE)
#define SPI0			              ((SPI_TypeDef *) SPI0_BASE)
#define SPI1			              ((SPI_TypeDef *) SPI1_BASE)
#define I2C0                     	  ((I2C_TypeDef *) I2C0_BASE)
#define I2C1                     	  ((I2C_TypeDef *) I2C1_BASE)
// Helper functions
#define _REG8(p, i)             (*(volatile uint8_t *) ((p) + (i)))
#define _REG32(p, i)            (*(volatile uint32_t *) ((p) + (i)))
#define _REG32P(p, i)           ((volatile uint32_t *) ((p) + (i)))

#define GPIO_REG(offset)        _REG32(GPIO_BASE, offset)
#define PWM0_REG(offset)        _REG32(PWM0_BASE, offset)
#define PWM1_REG(offset)        _REG32(PWM1_BASE, offset)
#define PWM2_REG(offset)        _REG32(PWM2_BASE, offset)
#define SPI0_REG(offset)        _REG32(QSPI0_BASE, offset)
#define SPI1_REG(offset)        _REG32(QSPI1_BASE, offset)
#define SPI2_REG(offset)        _REG32(QSPI2_BASE, offset)
#define UART0_REG(offset)       _REG32(UART0_BASE, offset)
#define UART1_REG(offset)       _REG32(UART1_BASE, offset)
#define I2C_REG(offset)         _REG8(I2C_BASE, offset)
#define AL_UART0_REG(offset)_REG32(AL_UART0_BASE,offset)

#define XEC0                     ((XEC_TypeDef *) XEC0_BASE)
#define XEC0_NCFG                ((XEC_TypeDef *) XEC0_NCFG_BASE)
#define XEC1                     ((XEC_TypeDef *) XEC1_BASE)
#define XEC1_NCFG               ((XEC_TypeDef *) XEC1_NCFG_BASE)



// Misc

#define NUM_GPIO 32

uint32_t get_cpu_freq();
void delay_1ms(uint32_t count);
void pass_fail_simulation(uint32_t state);

void fsbl_pass_fail_simulation(uint32_t state);
void print_value(uint64_t val);
void print_char(uint8_t c);
void print_string(uint8_t *str);
/** @} */ /* End of group demosoc */


/* ToDo: add here your device peripherals pointer definitions
         following is an example for timer */
/** @addtogroup Device_Peripheral_declaration
  * @{
  */
/* bit operations */
#define REG32(addr)                  (*(volatile uint32_t *)(uint32_t)(addr))
#define REG16(addr)                  (*(volatile uint16_t *)(uint32_t)(addr))
#define REG8(addr)                   (*(volatile uint8_t *)(uint32_t)(addr))
#define BIT(x)                       ((uint32_t)((uint32_t)0x01U<<(x)))
#define BITS(start, end)             ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31U - (uint32_t)(end))))
#define GET_BITS(regval, start, end) (((regval) & BITS((start),(end))) >> (start))
#define GET_BIT(regval, x) 	     ((((regval) & BIT(x)) >> (x)))

#define XEC0_REG(offset)                 _REG32(XEC0_BASE, offset)
#define XEC0_NCFG_REG(offset)            _REG32(XEC0_NCFG_BASE, offset)
//#define XEC1_REG(offset)                 _REG32(XEC1_BASE, offset)
//#define XEC1_NCFG_REG(offset)            _REG32(XEC1_NCFG_BASE, offset)

/** @} */ /* End of group Nuclei */

#ifdef __cplusplus
}
#endif

#endif  /* __DEMOSOC_H__ */
