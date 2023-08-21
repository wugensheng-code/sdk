#ifndef AL9000_CRG_H_
#define AL9000_CRG_H_

#include <stdint.h>

// FC Env: 50 MHz
// #define CRYSTAL_OSC_HZ 50000000
// #define CRYSTAL_OSC_HZ 25000000

// Demo Board: 33.33 MHz
#ifdef AL9000_OSC_25
#define CRYSTAL_OSC_HZ 25000000
#elif AL9000_OSC_33
#define CRYSTAL_OSC_HZ 33333333
#else
#define CRYSTAL_OSC_HZ 50000000
#endif

void pll_bypass();
void pll_enable();

// F_cpu6x = CRYSTAL_OSC_HZ * fbk_div / ref_div / out_div_6x
// F_cpu4x = CRYSTAL_OSC_HZ * fbk_div / ref_div / out_div_4x
void pll_cpu_div_set(uint32_t fbk_div, uint32_t ref_div, uint32_t out_div_6x, uint32_t out_div_4x);
void pll_cpu_reset();
void pll_cpu_release();
void pll_cpu_waitLock();

// F_io1000 = CRYSTAL_OSC_HZ * fbk_div / ref_div / out_div_1000
// F_io400  = CRYSTAL_OSC_HZ * fbk_div / ref_div / out_div_400
// F_io80   = CRYSTAL_OSC_HZ * fbk_div / ref_div / out_div_80
// F_io25   = CRYSTAL_OSC_HZ * fbk_div / ref_div / out_div_25
void pll_io_div_set(uint32_t fbk_div, uint32_t ref_div, uint32_t out_div_1000, uint32_t out_div_400, uint32_t out_div_80, uint32_t out_div_25);
void pll_io_reset();
void pll_io_release();
void pll_io_waitLock();

enum icg_para_t {
    CPU6X_DIV1_PARA = 0x0,
    CPU6X_DIV3_PARA = 0x4,
    CPU6X_DIV6_PARA = 0x8,
    CPU4X_DIV1_PARA = 0x10,
    CPU4X_DIV2_PARA = 0x14,
    CPU4X_DIV4_PARA = 0x18,
    IO400_DIV2_PARA = 0x20,
    IO400_DIV4_PARA = 0x24,
    IO400_DIV8_PARA = 0x28,
    IO400_DIV16_PARA = 0x2C,
    CPU4X_DIVN_PARA   = 0x50,
    CPU4X_DIVN_PARA_R = 0x54
};
// div = [1, 2, 3, 4, 6, 8, 12, 24]
void icg_div_set(enum icg_para_t inst, uint32_t div);

enum clk_sel_t {
    // APU: CPU6x, RPU: CPU2x
    SEL_CPU6X_2X = 0x1,
    // APU: CPU4x, RPU: CPU4x
    SEL_CPU4X_4X = 0x0,
    // APU: CPU4x, RPU: CPU2x
    SEL_CPU4X_2X = 0x0 | (0x1 << 5)
};
void clk_cpu_sel(enum clk_sel_t parm);

void clk_cpu_ratio_2221();
void clk_cpu_ratio_4221();
void clk_cpu_ratio_4421();
void clk_cpu_ratio_6221();

// CPU PLL = 800 MHz & 800 MHz
// CPU Clk 2221 = 400 MHz & 400 MHZ & 400MHz & 200 MHz
//  IO PLL = 1000 MHz & 400 MHz & 80 MHz & 25 MHz
void clk_simple_config();

enum cnt_div_para_t {
    IO1000_DIV_SDIO = 0x30 | (24 << 8) | (6 << 16),
    IO1000_DIV_SMC  = 0x30 | (16 << 8) | (6 << 16),
    IO1000_DIV_SPI  = 0x30 | ( 8 << 8) | (6 << 16),
    IO1000_DIV_QSPI = 0x30 | ( 0 << 8) | (6 << 16),
    IO25_DIV_TRNG   = 0x34 | (24 << 8) | (7 << 16),
    IO25_DIV_RVS    = 0x34 | (16 << 8) | (6 << 16),
    IO25_DIV_GPIO   = 0x34 | ( 0 << 8) | (6 << 16),
     OSC_DIV_PWM    = 0x38 | ( 8 << 8) | (12 << 16),
     OSC_DIV_OSC    = 0x38 | ( 0 << 8) | (6 << 16),
    IO400_DIV_FCLK3 = 0x3C | (24 << 8) | (6 << 16),
    IO400_DIV_FCLK2 = 0x3C | (16 << 8) | (6 << 16),
    IO400_DIV_FCLK1 = 0x3C | ( 8 << 8) | (6 << 16),
    IO400_DIV_FCLK0 = 0x3C | ( 0 << 8) | (6 << 16)
};
#define CNT_DIV_GATING 1U
void cnt_div_set(enum cnt_div_para_t parm, uint32_t div);

enum crg_srst_para_t {
    SRST_GIC  = 0x70 | ( 9 << 16),
    SRST_ACP  = 0x70 | ( 8 << 16),
    SRST_CORESIGHT = 0x70 | ( 5 << 16),
    SRST_JPU  = 0x74 | (13 << 16),
    SRST_NPU  = 0x74 | (12 << 16),
    SRST_OCM  = 0x74 | ( 4 << 16),
    SRST_IPC  = 0x78 | (12 << 16),
    SRST_GPIO = 0x78 | ( 8 << 16),
    SRST_GPM1 = 0x78 | ( 5 << 16),
    SRST_GPM0 = 0x78 | ( 4 << 16),
    SRST_HP1  = 0x78 | ( 1 << 16),
    SRST_HP0  = 0x78 | ( 0 << 16),
    SRST_UART0 = 0x7C | (13 << 16),
    SRST_UART1 = 0x7C | (12 << 16)
};
void crg_srst_assert(enum crg_srst_para_t parm);
void crg_srst_release(enum crg_srst_para_t parm);

#endif
