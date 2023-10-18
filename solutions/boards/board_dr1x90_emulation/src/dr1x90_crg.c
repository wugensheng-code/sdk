/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "dr1x90_crg.h"

#define TOP_CRG_BASE 0xF8801000UL
#define TOP_S_BASE   0xF8806000UL
static volatile uint32_t* TOP_CRG_CLK_SEL = (void*)(TOP_CRG_BASE + 0x40UL);

typedef volatile uint32_t* pll_t;
static pll_t CPU_PLL_CTRL = (void*)(TOP_CRG_BASE + 0x100UL);
static pll_t  IO_PLL_CTRL = (void*)(TOP_CRG_BASE + 0x200UL);

void pll_bypass()
{
    uint32_t val = *TOP_CRG_CLK_SEL;
    val |= (0x1 << 4);
    *TOP_CRG_CLK_SEL = val;
}

void pll_enable()
{
    uint32_t val = *TOP_CRG_CLK_SEL;
    val &= ~(0x1 << 4);
    *TOP_CRG_CLK_SEL = val;
}

static void pll_div_set(
    pll_t PLL_CTRL, uint32_t ref_div, uint32_t fbk_div,
    uint32_t out_div_0, uint32_t out_div_1, uint32_t out_div_2, uint32_t out_div_3
)
{
    uint32_t val = 0x0;
    uint32_t div = 0x0;

    // PLL_CTRL + 0x20
    val = PLL_CTRL[8];
    val &= ~0x7FU;
    div = (ref_div - 1U) & 0x7FU;
    val |= div;
    PLL_CTRL[8] = val;

    // PLL_CTRL + 0x24
    val = PLL_CTRL[9];
    val &= ~0x7FU;
    div = (fbk_div - 1U) & 0x7FU;
    val |= div;
    PLL_CTRL[9] = val;

    // PLL_CTRL + 0x48
    if (out_div_0 != 0U) {
        val = PLL_CTRL[18];
        val &= ~(0x7FU | 0x7FU << 24);
        div = (out_div_0 - 1U) & 0x7FU;
        val |= div << 24;
        div = (div >> 1) + 1U;
        val |= div;
        PLL_CTRL[18] = val;
    }
    // PLL_CTRL + 0x4C
    if (out_div_1 != 0U) {
        val = PLL_CTRL[19];
        val &= ~(0x7FU | 0x7FU << 24);
        div = (out_div_1 - 1U) & 0x7FU;
        val |= div << 24;
        div = (div >> 1) + 1U;
        val |= div;
        PLL_CTRL[19] = val;
    }
    // PLL_CTRL + 0x50
    if (out_div_2 != 0U) {
        val = PLL_CTRL[20];
        val &= ~(0x7FU | 0x7FU << 24);
        div = (out_div_2 - 1U) & 0x7FU;
        val |= div << 24;
        div = (div >> 1) + 1U;
        val |= div;
        PLL_CTRL[20] = val;
    }
    // PLL_CTRL + 0x54
    if (out_div_3 != 0U) {
        val = PLL_CTRL[21];
        val &= ~(0x7FU | 0x7FU << 24);
        div = (out_div_3 - 1U) & 0x7FU;
        val |= div << 24;
        div = (div >> 1) + 1U;
        val |= div;
        PLL_CTRL[21] = val;
    }
}

static void pll_reset(pll_t PLL_CTRL)
{
    // PLL_CTRL + 0x4
    uint32_t val = PLL_CTRL[1];
    val |= (0x1 << 9);
    PLL_CTRL[1] = val;
}

static void pll_release(pll_t PLL_CTRL)
{
    // PLL_CTRL + 0x4
    uint32_t val = PLL_CTRL[1];
    val &= ~(0x1 << 9);
    PLL_CTRL[1] = val;
}

static void pll_waitLock(pll_t PLL_CTRL)
{
    uint32_t val = 0U;
    while (val == 0U) {
        // PLL_CTRL + 0x80
        val = PLL_CTRL[32];
        val &= 1U;
    }
}

void pll_cpu_div_set(uint32_t fbk_div, uint32_t ref_div, uint32_t out_div_6x, uint32_t out_div_4x)
{
    pll_reset(CPU_PLL_CTRL);
    pll_div_set(CPU_PLL_CTRL, ref_div, fbk_div, out_div_6x, out_div_4x, 0U, 0U);
    pll_release(CPU_PLL_CTRL);
}

void pll_cpu_reset()
{
    pll_reset(CPU_PLL_CTRL);
}

void pll_cpu_release()
{
    pll_release(CPU_PLL_CTRL);
}

void pll_cpu_waitLock()
{
    pll_waitLock(CPU_PLL_CTRL);
}

void pll_io_div_set(
    uint32_t fbk_div, uint32_t ref_div,
    uint32_t out_div_1000, uint32_t out_div_400, uint32_t out_div_80, uint32_t out_div_25
)
{
    pll_reset(IO_PLL_CTRL);
    pll_div_set(IO_PLL_CTRL, ref_div, fbk_div, out_div_1000, out_div_400, out_div_25, out_div_80);
    pll_release(IO_PLL_CTRL);
}

void pll_io_reset()
{
    pll_reset(IO_PLL_CTRL);
}

void pll_io_release()
{
    pll_release(IO_PLL_CTRL);
}

void pll_io_waitLock()
{
    pll_waitLock(IO_PLL_CTRL);
}

void icg_div_set(enum icg_para_t inst, uint32_t div)
{
    /* div  val (24 bit)
        1   0xffffff
        2   0x555555
        3   0x249249
        4   0x111111
        6   0x041041
        8   0x010101
        12  0x001001
        24  0x000001    */
    // Method 1 (prefered)
    uint32_t val = (uint32_t)(-1) / ((0x1U << div) - 1U);

    // Method 2
    // uint32_t val = 0U;
    // for (uint32_t bit = 1U; bit != 0U; bit <<= div)
    //     val |= bit;

    volatile uint32_t* ptr = (void*)(TOP_CRG_BASE + (uint64_t)inst);
    *ptr = val;
}

void clk_cpu_sel(enum clk_sel_t parm)
{
    uint32_t val = *TOP_CRG_CLK_SEL;
    val &= ~(0x1 | 0x1 << 5);
    val |= (uint32_t)parm;
    *TOP_CRG_CLK_SEL = val;
}

void clk_cpu_ratio_2221()
{
    icg_div_set(CPU4X_DIV1_PARA, 2);
    icg_div_set(CPU4X_DIV2_PARA, 1);
    icg_div_set(CPU4X_DIV4_PARA, 2);
    clk_cpu_sel(SEL_CPU4X_4X);
}

void clk_cpu_ratio_4221()
{
    icg_div_set(CPU4X_DIV1_PARA, 1);
    icg_div_set(CPU4X_DIV2_PARA, 2);
    icg_div_set(CPU4X_DIV4_PARA, 4);
    clk_cpu_sel(SEL_CPU4X_2X);
}

void clk_cpu_ratio_4421()
{
    icg_div_set(CPU4X_DIV1_PARA, 1);
    icg_div_set(CPU4X_DIV2_PARA, 2);
    icg_div_set(CPU4X_DIV4_PARA, 4);
    clk_cpu_sel(SEL_CPU4X_4X);
}

void clk_cpu_ratio_6221()
{
    icg_div_set(CPU6X_DIV1_PARA, 1);
    icg_div_set(CPU6X_DIV3_PARA, 3);
    icg_div_set(CPU6X_DIV6_PARA, 6);
    clk_cpu_sel(SEL_CPU6X_2X);
}

void clk_simple_config()
{
    pll_bypass();
    clk_cpu_ratio_2221();

    #if   CRYSTAL_OSC_HZ == 50000000
    pll_cpu_div_set(64, 2, 2, 2);
    pll_io_div_set( 80, 2, 2, 5, 25, 80);
    #elif CRYSTAL_OSC_HZ == 33333333
    pll_cpu_div_set(96, 2, 2, 2);
    pll_io_div_set(120, 2, 2, 5, 25, 80);
    #elif CRYSTAL_OSC_HZ == 25000000
    pll_cpu_div_set(64, 1, 2, 2);
    pll_io_div_set( 80, 1, 2, 5, 25, 80);
    #else
    #error "Unknow CRYSTAL_OSC_HZ Value"
    #endif

    pll_cpu_waitLock();
    pll_io_waitLock();
    pll_enable();
}

void cnt_div_set(enum cnt_div_para_t parm, uint32_t div)
{
    uint32_t lsh = (parm >> 8) & 0xFFU;
    uint32_t msk = (0x1U << (parm >> 16)) - 1U;
    volatile uint32_t* ptr = (void*)(TOP_CRG_BASE + (uint64_t)(parm & 0xFFU));

    msk = msk << lsh;
    div = (div - 1U) << lsh;

    uint32_t val = *ptr;
    val &= (~msk);
    val |= (div & msk);
    *ptr = val;
}

void crg_srst_assert(enum crg_srst_para_t parm)
{
    uint32_t bit = 0x1U << (parm >> 16);
    volatile uint32_t* ptr = (void*)(TOP_CRG_BASE + (uint64_t)(parm & 0xFFFFU));

    uint32_t val = *ptr;
    *ptr = val & ~bit;
}

void crg_srst_release(enum crg_srst_para_t parm)
{
    uint32_t bit = 0x1U << (parm >> 16);
    volatile uint32_t* ptr = (void*)(TOP_CRG_BASE + (uint64_t)(parm & 0xFFFFU));

    uint32_t val = *ptr;
    *ptr = val | bit;
}

void top_srst_assert(enum top_srst_para_t parm)
{
    uint32_t bit = 0x1U << (parm >> 16);
    volatile uint32_t* ptr = (void*)(TOP_S_BASE + (uint64_t)(parm & 0xFFFFU));
    uint32_t val = *ptr;
    *ptr = val & ~bit;
}

void top_srst_release(enum top_srst_para_t parm)
{
    uint32_t bit = 0x1U << (parm >> 16);
    volatile uint32_t* ptr = (void*)(TOP_S_BASE + (uint64_t)(parm & 0xFFFFU));
    uint32_t val = *ptr;
    *ptr = val | bit;
}
