/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_SOC_BOARD_INIT_H_
#define __AL_SOC_BOARD_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _UL
#define _UL unsigned int
#endif

/* config opcode */
#define CONFIG_EXIT         0U
#define CONFIG_CLEAR        1U
#define CONFIG_WRITE        2U
#define CONFIG_MASKWRITE    3U
#define CONFIG_MASKPOLL     4U
#define CONFIG_MASKDELAY    5U
#define NEW_ALSOC_ERR_CODE  6U

#define CONFIG_POLL_TIME    100000

/* Encode number of arguments */
#define CONFIG_REG_EXIT()                    ( (CONFIG_EXIT      << 4 ) | 0 )
#define CONFIG_REG_CLEAR(addr)               ( (CONFIG_CLEAR     << 4 ) | 1 ) , addr
#define CONFIG_REG(addr,val)                 ( (CONFIG_WRITE     << 4 ) | 2 ) , addr, val
#define CONFIG_REG_MASK(addr,mask,val)       ( (CONFIG_MASKWRITE << 4 ) | 3 ) , addr, mask, val

/* Returns codes  of ALSOC_Init */
#define ALSOC_INIT_SUCCESS         (0)    // success
#define ALSOC_INIT_OPCODE_ERROR    (1)    // config opcode error
#define ALSOC_INIT_VERSION_ERROR   (2)    // data error
#define ALSOC_INIT_TIMEOUT         (3)    // timeout

/* Silicon Versions */
#define SOC_VERSION_1 0


/* This flag to be used by FSBL to check whether apu_post_config() proc exixts */
#define ALSOC_POST_CONFIG

/* Freq of all peripherals */
#define MHZ                     1000000U


#define ALSOC_APU_FREQ          (100*MHZ)

#if defined __aarch64__
#ifdef AL_OSC_25
#define SYSTEM_CLOCK            (25*MHZ)
#elif defined AL_OSC_33
#define SYSTEM_CLOCK            (33333333UL)
#elif defined AL_OSC_50
#define SYSTEM_CLOCK            (50*MHZ)
#else
#define SYSTEM_CLOCK            (50*MHZ)
#endif
#else
#ifdef AL_CLK_CONFIG
#ifdef AL_OSC_25
#define SYSTEM_CLOCK            (200*MHZ)
#elif defined AL_OSC_33
#if (defined AL_CLK_800M || defined AL_CLK_400M || defined AL_CLK_1200M)
#define SYSTEM_CLOCK            (400*MHZ)
#elif defined AL_CLK_1000M
#define SYSTEM_CLOCK            (333333333UL)
#else
#error "undefined RPU CLK"
#endif
#elif defined AL_OSC_50
#define SYSTEM_CLOCK            (400*MHZ)
#else
#error "undefined OSC"
#endif
#else
#ifdef AL_OSC_25
#define SYSTEM_CLOCK            (200*MHZ)
#elif defined AL_OSC_33
#define SYSTEM_CLOCK            (266666666UL)
#elif defined AL_OSC_50
#define SYSTEM_CLOCK            (400*MHZ)
#else
#error "undefined OSC"
#endif
#endif
#endif

#define HAVE_UARTPS_DRIVER
#ifdef AL_CLK_CONFIG
#define UART_CLOCK              (50*MHZ)
#else
#ifdef AL_OSC_25
#define UART_CLOCK              (25*MHZ)
#elif defined AL_OSC_33
#define UART_CLOCK              (33333333UL)
#elif defined AL_OSC_50
#define UART_CLOCK              (50*MHZ)
#else
#define UART_CLOCK              (40*MHZ)
#endif
#endif

#define HAVE_IICPS_DRIVER
#define I2C_CLOCK               (40*MHZ)

#define HAVE_MMCPS_DRIVER
#ifdef AL_CLK_CONFIG
#define MMC_CLOCK               (100*MHZ)
#else
#ifdef AL_OSC_25
#define MMC_CLOCK               (50*MHZ)
#elif defined AL_OSC_33
#define MMC_CLOCK               (66*MHZ)
#else
#define MMC_CLOCK               (100*MHZ)
#endif
#endif

#define HAVE_CANPS_DRIVER
#define CAN_CLOCK               (80*MHZ)

#define HAVE_QSPIPS_DRIVER
#define QSPI_CLOCK              (50*MHZ)

#define HAVE_SPIPS_DRIVER
#define SPI_CLOCK               (50*MHZ)


#define HAVE_SMCPS_DRIVER
#define SMC_CLOCK               (50*MHZ)


#define HAVE_ADC_DRIVER
#define ADC_CLOCK               (50*MHZ)

#define HAVE_TTCPS_DRIVER
#define TTC_CLOCK              (200*MHZ)


#define HAVE_GBEPS_DRIVER
#define GBE_CLOCK               (25*MHZ)

#define GBE_DEVICE_ID           (0)

#define HAVE_GPIOPS_DRIVER

#define HAVE_MPUPS_DRIVER

#define HAVE_CIPHERPS_DRIVER

#define HAVE_DMACAHBPS_DRIVER

#define HAVE_PMU_DRIVER

#define HAVE_WDTPS_DRIVER
#define WDT_CLOCK              (200*MHZ)

#define HAVE_IPCPS_DRIVER

#define HAVE_DMAPS_DRIVER

#define HAVE_USBPS_DRIVER

#define DDR_AVAILABLE

#define UART0_BAUDRATE 115200
#define UART1_BAUDRATE 115200

/**
 * @desc  : init soc platform according to user configuration
 * @return {int} status code of platform init
 */
int Soc_PlatInit();
int Soc_PsPlInit();
int Soc_PlatGetDeviceVersion();

#ifdef __cplusplus
}
#endif

#endif /* AL_SOC_BOARD_INIT_H */
