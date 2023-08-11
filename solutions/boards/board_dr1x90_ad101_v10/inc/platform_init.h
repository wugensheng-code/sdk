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
#define MHZ                     1000000U


#define ALSOC_APU_FREQ          (100*MHZ)

#ifdef AL9000_OSC_25
#define SYSTEM_CLOCK            (25000000UL)
#elif AL9000_OSC_33
#define SYSTEM_CLOCK            (33333333UL)
#elif AL9000_OSC_50
#define SYSTEM_CLOCK            (50000000UL)
#else
#define SYSTEM_CLOCK            (50000000UL)
#endif



#define HAVE_UARTPS_DRIVER

#ifdef AL9000_OSC_25
#define UART_CLOCK              (25000000UL)
#elif AL9000_OSC_33
#define UART_CLOCK              (33333333UL)
#elif AL9000_OSC_50
#define UART_CLOCK              (50000000UL)
#else
#define UART_CLOCK              (40000000UL)
#endif


#define HAVE_IICPS_DRIVER
#define I2C_CLOCK               (40*MHZ)

#define HAVE_MMCPS_DRIVER
#define MMC_CLOCK               (100*MHZ)

#define HAVE_CANPS_DRIVER
#define CAN_CLOCK               (80*MHZ)

#define HAVE_QSPIPS_DRIVER
#define QSPI_CLOCK              (50*MHZ)

#define HAVE_SPIPS_DRIVER
#define SPI_CLOCK               (50*MHZ)


#define HAVE_SMCPS_DRIVER
#define SMC_CLOCK               (50*MHZ)


#define HAVE_XADC_DRIVER

#define HAVE_GBEPS_DRIVER
#define GBE_CLOCK               (25*MHZ)


#define HAVE_GPIOPS_DRIVER

#define HAVE_MPUPS_DRIVER

#define HAVE_CIPHERPS_DRIVER

#define HAVE_DMACAHBPS_DRIVER

#define HAVE_GPIOPS_DRIVER


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
