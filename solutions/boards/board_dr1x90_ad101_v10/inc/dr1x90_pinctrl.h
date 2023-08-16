#ifndef _DR1X90_PINCTRL_H_
#define _DR1X90_PINCTRL_H_

#include <stdint.h>

typedef enum nand_width_t
{
    NAND_8BIT  = 0x0, 
    NAND_16BIT = 0x1
} nand_width_t;

// Pin 0 - 6
void pinmux_config_qspi();
// Pin 0, 2 - 14 (8 & 16 Bit), 16 - 23 (16 Bit)
void pinmux_config_nand(nand_width_t width);
// Pin 16 - 27
void pinmux_config_eth0();
// Pin 28 - 39
void pinmux_config_eth1();
// Pin 28 - 39
void pinmux_config_usb0();
// Pin 40 - 51
void pinmux_config_usb1();
// Pin 52, 53
void pinmux_config_mdio0();
// Pin 52, 53
void pinmux_config_mdio1();
// Pin 46 - 49
void pinmux_config_cjtag();
// Pin 34 - 37
void pinmux_config_psjtag();

void pinmux_config_gpio(int pin);

typedef struct pin_sd_t
{
    int clk;
    int cmd;
    int data[8];
    int pw;     // powr_ctrl
    int wp;
    int cd;
} pin_sd_t;
void pinmux_config_sd0(const pin_sd_t* pin);
void pinmux_config_sd1(const pin_sd_t* pin);
void pinmux_config_sd0_8b(const pin_sd_t* pin);

typedef struct pin_spi_t
{
    int mosi;
    int miso;
    int sclk;
    int ss[3];
} pin_spi_t;
void pinmux_config_spi0(const pin_spi_t* pin);
void pinmux_config_spi1(const pin_spi_t* pin);

typedef struct pin_uart_t
{
    int tx;
    int rx;
} pin_uart_t;
void pinmux_config_uart0(const pin_uart_t* pin);
void pinmux_config_uart1(const pin_uart_t* pin);

typedef struct pin_i2c_t
{
    int scl;
    int sda;
} pin_i2c_t;
void pinmux_config_i2c0(const pin_i2c_t* pin);
void pinmux_config_i2c1(const pin_i2c_t* pin);

typedef struct pin_can_t
{
    int clk;
    int tx;
    int rx;
} pin_can_t;
void pinmux_config_can0(const pin_can_t* pin);
void pinmux_config_can1(const pin_can_t* pin);

typedef struct pin_ttc_t
{
    int clk;
    int wave;
} pin_ttc_t;
void pinmux_config_ttc0(const pin_ttc_t* pin);
void pinmux_config_ttc1(const pin_ttc_t* pin);

typedef struct pin_swdt_t
{
    int clk;
    int rst;
} pin_swdt_t;
void pinmux_config_swdt(const pin_swdt_t* pin);

typedef struct pin_jtag_t
{
    int tdi;
    int tdo;
    int tck;
    int tms;
} pin_jtag_t;
void pinmux_config_pjtag(const pin_jtag_t* pin);

typedef enum mio_speed_t
{
    MIO_SPEED_SLOW = 0x0, 
    MIO_SPEED_MEDI = 0x2,
    MIO_SPEED_FAST = 0x7
} mio_speed_t;

typedef enum mio_hyst_t
{
    MIO_HYST_0_0     = 0x0,
    MIO_HYST_23_4    = 0x1,
    MIO_HYST_59_19   = 0x2,
    MIO_HYST_95_31   = 0x3,
    MIO_HYST_131_43  = 0x4,
    MIO_HYST_171_59  = 0x5,
    MIO_HYST_211_71  = 0x6,
    MIO_HYST_251_83  = 0x7,
    MIO_HYST_207_151 = 0x8,
    MIO_HYST_251_163 = 0x9,
    MIO_HYST_295_179 = 0xA,
    MIO_HYST_343_191 = 0xB,
    MIO_HYST_384_203 = 0xC,
    MIO_HYST_439_219 = 0xD,
    MIO_HYST_491_231 = 0xE,
    MIO_HYST_543_243 = 0xF,
    MIO_HYST_DEFT = 0x8
} mio_hyst_t;

typedef enum mio_thrd_t
{
    MIO_THRD_HIGH = 0x0,
    MIO_THRD_LOW  = 0x1,
    MIO_THRD_DEFT = 0x0
} mio_thrd_t;

typedef enum mio_pull_t
{
    MIO_PULL_DIS = 0x0,
    MIO_PULL_25K_EN = 0x4,
    MIO_PULL_10K_EN = 0x5
} mio_pull_t;

typedef enum mio_recv_t
{
    MIO_RECV_DIS     = 0x0,
    MIO_RECV_DIFF    = 0x1,
    MIO_RECV_1V8_1V5 = 0x2,   // 1.8V or 1.5V
    MIO_RECV_3V3_2V5 = 0x3    // 3.3V or 2.5V
} mio_recv_t;

typedef enum mio_vccio_t
{
    MIO_VCCIO_3V3 = 0x0,
    MIO_VCCIO_2V5 = 0x1,
    MIO_VCCIO_1V8 = 0x2,
    MIO_VCCIO_1V5 = 0x3
} mio_vccio_t;

typedef enum mio_driven_t
{
    MIO_DRIVEN_4MA  = 0x0,
    MIO_DRIVEN_8MA  = 0x1,
    MIO_DRIVEN_12MA = 0x2,
    MIO_DRIVEN_16MA = 0x3,
    MIO_DRIVEN_20MA = 0x4
} mio_driven_t;

uint32_t mio_pad_ctrl0(int pin, mio_speed_t speed, mio_pull_t pull_up, mio_pull_t pull_dw);
uint32_t mio_pad_ctrl1(int pin, mio_recv_t mode, mio_vccio_t vcc, mio_driven_t up, mio_driven_t dw);

// Default Config
// mio_pad_ctrl0(0, MIO_SPEED_SLOW, MIO_PULL_25K_EN, MIO_PULL_DIS);     // == 0x88000000
// mio_pad_ctrl1(0, MIO_RECV_3V3_2V5, MIO_VCCIO_3V3, MIO_DRIVEN_8MA, MIO_DRIVEN_8MA);   // == 0x1E0103

// #include "al9000_pinmux_auto.h"

#endif
