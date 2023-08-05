#ifndef __ALSOC_BOARD_INIT_H
#define __ALSOC_BOARD_INIT_H

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
#define MHZ                 1000000U

#define UART_FREQ           (40*MHZ )
#define ALSOC_APU_FREQ      (100*MHZ)

#if 0

#define UART_CLOCK
#define I2C_CLOCK
#define SPI_CLOCK
#define QSPI_CLOCK

#endif


/**
 * @desc  : init soc platform according to user configuration
 * @return {int} status code of platform init
 */
int soc_platform_init();
int get_device_version();

#ifdef __cplusplus
}
#endif

#endif /* __ALSOC_BOARD_INIT_H */
