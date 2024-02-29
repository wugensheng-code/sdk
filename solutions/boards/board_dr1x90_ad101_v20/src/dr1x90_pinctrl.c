/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include "dr1x90_pinctrl.h"

static volatile uint32_t* const MIO_FUNCSEL = (volatile uint32_t*) 0xF8803000;
static volatile uint32_t* const MIO_EMIOSEL = (volatile uint32_t*) 0xF8803400;
static volatile uint32_t* const MIO_PARA    = (volatile uint32_t*) 0xF8803800;

static void set_funcsel(int start, int end, uint32_t func)
{
    for (int i = start; i <= end; ++i)
        MIO_FUNCSEL[i] = func;
}

void pinmux_config_qspi()
{
    set_funcsel(0, 6, 1U);
}

void pinmux_config_nand(nand_width_t width)
{
    MIO_FUNCSEL[0] = 2U;
    set_funcsel(2, 14, 2U);
    if (width == NAND_16BIT)
        set_funcsel(16, 23, 2U);
}

void pinmux_config_eth0()
{
    set_funcsel(16, 27, 14U);
    // MIO_FUNCSEL[50] = 14U;     // GBE0_PHY
}

void pinmux_config_eth1()
{
    set_funcsel(28, 39, 14U);
    // MIO_FUNCSEL[51] = 14U;     // GBE1_PHY
}

void pinmux_config_usb0()
{
    set_funcsel(28, 39, 15U);
}

void pinmux_config_usb1()
{
    set_funcsel(40, 51, 15U);
}

void pinmux_config_mdio0()
{
    MIO_FUNCSEL[52] = 14U;
    MIO_FUNCSEL[53] = 14U;
}

void pinmux_config_mdio1()
{
    MIO_FUNCSEL[52] = 15U;
    MIO_FUNCSEL[53] = 15U;
}

void pinmux_config_cjtag()
{
    set_funcsel(46, 49, 14U);
}

void pinmux_config_psjtag()
{
    MIO_FUNCSEL[34] = 6U;
    MIO_FUNCSEL[35] = 11U;
    MIO_FUNCSEL[36] = 11U;
    MIO_FUNCSEL[37] = 6U;
}

void pinmux_config_gpio(int pin)
{
    MIO_FUNCSEL[pin] = 4U;
}

static uint32_t get_sdpw_func(int pin)
{
    if (pin <= 15)
        return 14U; // 0 - 15
    else if (pin <= 21)
        return 7U;  // 20 - 21
    else if (pin <= 24)
        return 2U;  // 24
    else if (pin <= 27)
        return 11U; // 25 - 27
    else if (pin <= 33)
        return 7U;  // 32 - 33
    else if (pin <= 45)
        return 14U; // 40 - 45
    else
        return 11U; // 50 - 53
}

void pinmux_config_sd0(const pin_sd_t* pin)
{
    MIO_FUNCSEL[pin->clk] = 10U;
    MIO_FUNCSEL[pin->cmd] = 10U;
    for (int i = 0; i < 4; ++i)
        MIO_FUNCSEL[pin->data[i]] = 10U;
    MIO_FUNCSEL[pin->pw] = get_sdpw_func(pin->pw);
    MIO_FUNCSEL[pin->wp] = (pin->wp <= 27) ? 15U : 8U;
    MIO_FUNCSEL[pin->cd] = 12U;
    MIO_EMIOSEL[11] = 1U;
}

void pinmux_config_sd1(const pin_sd_t* pin)
{
    MIO_FUNCSEL[pin->clk] = 10U;
    MIO_FUNCSEL[pin->cmd] = 10U;
    for (int i = 0; i < 4; ++i)
        MIO_FUNCSEL[pin->data[i]] = 10U;
    MIO_FUNCSEL[pin->pw] = get_sdpw_func(pin->pw);
    MIO_FUNCSEL[pin->wp] = (pin->wp <= 24) ? 11U : 2U;
    MIO_FUNCSEL[pin->cd] = 13U;
    MIO_EMIOSEL[12] = 1U;
}

void pinmux_config_sd0_8b(const pin_sd_t* pin)
{
    MIO_FUNCSEL[pin->clk] = 11U;
    MIO_FUNCSEL[pin->cmd] = 11U;
    for (int i = 0; i < 8; ++i)
        MIO_FUNCSEL[pin->data[i]] = 11U;
    MIO_FUNCSEL[pin->pw] = get_sdpw_func(pin->pw);
    MIO_FUNCSEL[pin->wp] = (pin->wp <= 27) ? 15U : 8U;
    MIO_FUNCSEL[pin->cd] = 12U;
    MIO_EMIOSEL[11] = 1U;
}

void pinmux_config_spi0(const pin_spi_t* pin)
{
    MIO_FUNCSEL[pin->mosi] = 9U;
    MIO_FUNCSEL[pin->miso] = 9U;
    MIO_FUNCSEL[pin->sclk] = 9U;
    for (int i = 0; i < 3; ++i)
        MIO_FUNCSEL[pin->ss[i]] = 9U;
    MIO_EMIOSEL[9] = 1U;
}

void pinmux_config_spi1(const pin_spi_t* pin)
{
    MIO_FUNCSEL[pin->mosi] = 9U;
    MIO_FUNCSEL[pin->miso] = 9U;
    MIO_FUNCSEL[pin->sclk] = 9U;
    for (int i = 0; i < 3; ++i)
        MIO_FUNCSEL[pin->ss[i]] = 9U;
    MIO_EMIOSEL[10] = 1U;
}

void pinmux_config_uart0(const pin_uart_t* pin)
{
    MIO_FUNCSEL[pin->tx] = 3U;
    MIO_FUNCSEL[pin->rx] = 3U;
    MIO_EMIOSEL[4] = 1U;
}

void pinmux_config_uart1(const pin_uart_t* pin)
{
    MIO_FUNCSEL[pin->tx] = 3U;
    MIO_FUNCSEL[pin->rx] = 3U;
    MIO_EMIOSEL[5] = 1U;
}

void pinmux_config_i2c0(const pin_i2c_t* pin)
{
    MIO_FUNCSEL[pin->scl] = 0U;
    MIO_FUNCSEL[pin->sda] = 0U;
    MIO_EMIOSEL[0] = 1U;
}

void pinmux_config_i2c1(const pin_i2c_t* pin)
{
    MIO_FUNCSEL[pin->scl] = 0U;
    MIO_FUNCSEL[pin->sda] = 0U;
    MIO_EMIOSEL[1] = 1U;
}

void pinmux_config_can0(const pin_can_t* pin)
{
    MIO_FUNCSEL[pin->clk] = 5U;
    MIO_FUNCSEL[pin->tx] = 1U;
    MIO_FUNCSEL[pin->rx] = 1U;
    MIO_EMIOSEL[2] = 1U;
}

void pinmux_config_can1(const pin_can_t* pin)
{
    MIO_FUNCSEL[pin->clk] = 6U;
    MIO_FUNCSEL[pin->tx] = 1U;
    MIO_FUNCSEL[pin->rx] = 1U;
    MIO_EMIOSEL[3] = 1U;
}

void pinmux_config_ttc0(const pin_ttc_t* pin)
{
    MIO_FUNCSEL[pin->clk]  = 7U;
    MIO_FUNCSEL[pin->wave] = 7U;
    MIO_EMIOSEL[6] = 1U;
}

void pinmux_config_ttc1(const pin_ttc_t* pin)
{
    MIO_FUNCSEL[pin->clk]  = 7U;
    MIO_FUNCSEL[pin->wave] = 7U;
    MIO_EMIOSEL[7] = 1U;
}

void pinmux_config_swdt(const pin_swdt_t* pin)
{
    MIO_FUNCSEL[pin->clk] = 7U;
    MIO_FUNCSEL[pin->rst] = 7U;
    MIO_EMIOSEL[8] = 1U;
}

void pinmux_config_pjtag(const pin_jtag_t* pin)
{
    MIO_FUNCSEL[pin->tdi] = 7U;
    MIO_FUNCSEL[pin->tdo] = 7U;
    MIO_FUNCSEL[pin->tck] = 7U;
    MIO_FUNCSEL[pin->tms] = 7U;
}

uint32_t mio_pad_ctrl0(int pin, mio_speed_t speed, mio_pull_t pull_up, mio_pull_t pull_dw)
{
    uint32_t val = speed | (0x8U << 24) | (pull_dw << 28) | (pull_up << 29);
    MIO_PARA[2 * pin + 0] = val;
    return val;
}

uint32_t mio_pad_ctrl1(int pin, mio_recv_t mode, mio_vccio_t vcc, mio_driven_t up, mio_driven_t dw)
{
    // Doc @ AL IO MUX QRG.xlsx: PS OUTPUT PAD CTRL
    static const uint8_t ensnk[4][5] = {
    //   4mA   8mA  12mA  16mA  20mA
        {0x0A, 0x1E, 0x15, 0x17, 0x12},   // 3.3V
        {0x0A, 0x1E, 0x15, 0x17, 0x12},   // 2.5V
        {0x0A, 0x1E, 0x15, 0x17, 0x12},   // 1.8V
        {0x0A, 0x1E, 0x15, 0x17, 0x12}    // 1.5V
    };
    static const uint8_t ensrc[4][5] = {
    //   4mA   8mA  12mA  16mA  20mA
        {0x0B, 0x01, 0x17, 0x12, 0x13},   // 3.3V
        {0x09, 0x0B, 0x1B, 0x17, 0x11},   // 2.5V
        {0x09, 0x06, 0x18, 0x15, 0x17},   // 1.8V
        {0x09, 0x1D, 0x19, 0x17, 0x11}    // 1.5V
    };
    uint32_t src = (uint32_t)ensrc[vcc][up];
    uint32_t snk = (uint32_t)ensnk[vcc][dw];
    uint32_t val = mode | (src << 8) | (0U << 13) | (snk << 16);
    MIO_PARA[2 * pin + 1] = val;
    return val;
}
