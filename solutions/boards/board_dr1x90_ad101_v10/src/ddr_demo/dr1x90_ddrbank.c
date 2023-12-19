#include <math.h>
#include "dr1x90_crg.h"
#include "dr1x90_ddrc_init.h"

void dr1x90_ddr_gpll_cfg(double fck)
{
    /*
    dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL9, ctrl9_fbk_div_offset,  ctrl9_fbk_div_mask,  63);     // 1066
    // dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL9, ctrl9_fbk_div_offset,  ctrl9_fbk_div_mask,  71);     // 1200
    // dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL9, ctrl9_fbk_div_offset,  ctrl9_fbk_div_mask,  70);     // 1183

    // clock c0
    dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL18, gpll_duty_offset, gpll_duty_mask, 1);
    dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL18, gpll_divc_offset, gpll_divc_mask, 1);
    // clock c1
    dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL19, gpll_duty_offset, gpll_duty_mask, 2);
    dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL19, gpll_divc_offset, gpll_divc_mask, 3);
    // clock c2
    dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL20, gpll_duty_offset, gpll_duty_mask, 1);
    dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL20, gpll_divc_offset, gpll_divc_mask, 1);

    // trigger GPLL reset and wait for GPLL Lock
    dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL1, ctrl1_pllreset_offset, ctrl1_pllreset_mask, 1);
    dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL1, ctrl1_pllreset_offset, ctrl1_pllreset_mask, 0);
    dr1x90_field_wait(DDRC_ADDR_GPLL + DDRGPLL_STATE0, gpll_lock_offset, gpll_lock_mask, 1, 1000);
    */
    double fbk = 2.0 * fck / (50.0 / 3.0);
    uint32_t fbk_div = lround(fbk);
    AL_DDR_LOG("[DDR GPLL] fck = %.3f MHz, fbk_div = %d\r\n", fck, fbk_div);

    #if CRYSTAL_OSC_HZ == 33333333
    pll_ddr_div_set(fbk_div, 1, 2, 4, 2);
    #elif CRYSTAL_OSC_HZ == 50000000
    pll_ddr_div_set(fbk_div, 1, 3, 6, 3);
    #else
    #error "[DDR GPLL] Unsupported CRYSTAL_OSC_HZ"
    #endif
    pll_ddr_waitLock();

}

void dr1x90_ddr_dpll_cfg()
{
    u32 regData = 0;
    // BK0_DPLL_CR1                     0xc
    regData = dr1x90_reg_read(DDRC_ADDR_DPLL + BK0_DPLL_CR1);
    regData = dr1x90_field_set(DDRC_ADDR_DPLL + BK0_DPLL_CR1, BK0_DPLL_VCOSEL_offset, BK0_DPLL_VCOSEL_mask,  1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_DPLL + BK0_DPLL_CR1, BK0_DPLL_POSTDIV_offset,BK0_DPLL_POSTDIV_mask, 1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_DPLL + BK0_DPLL_CR1, BK0_DPLL_FBDIV_offset,  BK0_DPLL_FBDIV_mask,   3, regData);
    regData = dr1x90_field_set(DDRC_ADDR_DPLL + BK0_DPLL_CR1, BK0_DPLL_REFDIV_offset, BK0_DPLL_REFDIV_mask,  3, regData);
    regData = dr1x90_field_set(DDRC_ADDR_DPLL + BK0_DPLL_CR1, BK0_DPLL_BYPASS_offset, BK0_DPLL_BYPASS_mask,  0, regData);
    dr1x90_reg_write(DDRC_ADDR_DPLL + BK0_DPLL_CR1, regData);

    regData = dr1x90_reg_read(DDRC_ADDR_DPLL + BK1_DPLL_CR1);
    regData = dr1x90_field_set(DDRC_ADDR_DPLL + BK1_DPLL_CR1, BK1_DPLL_VCOSEL_offset, BK1_DPLL_VCOSEL_mask,  1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_DPLL + BK1_DPLL_CR1, BK1_DPLL_POSTDIV_offset,BK1_DPLL_POSTDIV_mask, 1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_DPLL + BK1_DPLL_CR1, BK1_DPLL_FBDIV_offset,  BK1_DPLL_FBDIV_mask,   3, regData);
    regData = dr1x90_field_set(DDRC_ADDR_DPLL + BK1_DPLL_CR1, BK1_DPLL_REFDIV_offset, BK1_DPLL_REFDIV_mask,  3, regData);
    regData = dr1x90_field_set(DDRC_ADDR_DPLL + BK1_DPLL_CR1, BK1_DPLL_BYPASS_offset, BK1_DPLL_BYPASS_mask,  0, regData);
    dr1x90_reg_write(DDRC_ADDR_DPLL + BK1_DPLL_CR1, regData);

    dr1x90_field_write(DDRC_ADDR_DPLL + BK0_DPLL_CR1, BK0_DPLL_PD_offset, BK0_DPLL_PD_mask, 0);
    dr1x90_field_write(DDRC_ADDR_DPLL + BK1_DPLL_CR1, BK1_DPLL_PD_offset, BK1_DPLL_PD_mask, 0);

    // wait for DPLL Lock
    dr1x90_field_wait(DDRC_ADDR_DPLL + BK0_DPLL_SR0, BK0_DPLL_LOCK_offset, BK0_DPLL_LOCK_mask, 1, -1);
}

void dr1x90_ddr_iob_cfg()
{
    // ...
}

void dr1x90_ddr_iol_cfg()
{
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg1,   U_byte0_quad2_mc1_oclk_sclk_offset, U_byte0_quad2_mc1_oclk_sclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg1,   U_byte0_quad2_mc1_oclk_pclk_offset, U_byte0_quad2_mc1_oclk_pclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg1, U_byte1_quad2_mc1_oclk_sclk_offset, U_byte1_quad2_mc1_oclk_sclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg1, U_byte1_quad2_mc1_oclk_pclk_offset, U_byte1_quad2_mc1_oclk_pclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg1, U_byte2_quad2_mc1_oclk_sclk_offset, U_byte2_quad2_mc1_oclk_sclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg1, U_byte2_quad2_mc1_oclk_pclk_offset, U_byte2_quad2_mc1_oclk_pclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg1, U_byte3_quad2_mc1_oclk_sclk_offset, U_byte3_quad2_mc1_oclk_sclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg1, U_byte3_quad2_mc1_oclk_pclk_offset, U_byte3_quad2_mc1_oclk_pclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));

    // tri-state enable for DQS_t/c
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg4,   U_byte0_quad2_mc1_ts_sel_offset, U_byte0_quad2_mc1_ts_sel_mask, (0x6 << 9) | (0x2 << 6) | (0x0 << 3) | 0x0);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg4, U_byte0_quad2_mc1_ts_sel_offset, U_byte0_quad2_mc1_ts_sel_mask, (0x6 << 9) | (0x2 << 6) | (0x0 << 3) | 0x0);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg4, U_byte0_quad2_mc1_ts_sel_offset, U_byte0_quad2_mc1_ts_sel_mask, (0x6 << 9) | (0x2 << 6) | (0x0 << 3) | 0x0);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg4, U_byte0_quad2_mc1_ts_sel_offset, U_byte0_quad2_mc1_ts_sel_mask, (0x6 << 9) | (0x2 << 6) | (0x0 << 3) | 0x0);

    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr1to4_cfg10  , U_byte0_quad1_mc1_i_mode_offset, U_byte0_quad1_mc1_i_mode_mask, 0xff);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg10  , U_byte0_quad2_mc1_i_mode_offset, U_byte0_quad2_mc1_i_mode_mask, 0xff);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr9to12_cfg10 , U_byte0_quad3_mc1_i_mode_offset, U_byte0_quad3_mc1_i_mode_mask, 0xff);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr13to16_cfg10, U_byte1_quad1_mc1_i_mode_offset, U_byte1_quad1_mc1_i_mode_mask, 0xff);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg10, U_byte1_quad2_mc1_i_mode_offset, U_byte1_quad2_mc1_i_mode_mask, 0xff);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr21to24_cfg10, U_byte1_quad3_mc1_i_mode_offset, U_byte1_quad3_mc1_i_mode_mask, 0xff);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr25to28_cfg10, U_byte2_quad1_mc1_i_mode_offset, U_byte2_quad1_mc1_i_mode_mask, 0xff);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg10, U_byte2_quad2_mc1_i_mode_offset, U_byte2_quad2_mc1_i_mode_mask, 0xff);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr33to36_cfg10, U_byte2_quad3_mc1_i_mode_offset, U_byte2_quad3_mc1_i_mode_mask, 0xff);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr37to40_cfg10, U_byte3_quad1_mc1_i_mode_offset, U_byte3_quad1_mc1_i_mode_mask, 0xff);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg10, U_byte3_quad2_mc1_i_mode_offset, U_byte3_quad2_mc1_i_mode_mask, 0xff);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr45to48_cfg10, U_byte3_quad3_mc1_i_mode_offset, U_byte3_quad3_mc1_i_mode_mask, 0xff);

    // select gate source
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte0_glue_cfg0, U_byte0_glue_mc1_qs_gate_sel_offset, U_byte0_glue_mc1_qs_gate_sel_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte1_glue_cfg0, U_byte1_glue_mc1_qs_gate_sel_offset, U_byte1_glue_mc1_qs_gate_sel_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte2_glue_cfg0, U_byte2_glue_mc1_qs_gate_sel_offset, U_byte2_glue_mc1_qs_gate_sel_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte3_glue_cfg0, U_byte3_glue_mc1_qs_gate_sel_offset, U_byte3_glue_mc1_qs_gate_sel_mask, 1);

    // DQS Byte mode
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte0_glue_cfg0, U_byte0_glue_mc1_dqs_byte_md_offset, U_byte0_glue_mc1_dqs_byte_md_mask, 0);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte1_glue_cfg0, U_byte1_glue_mc1_dqs_byte_md_offset, U_byte1_glue_mc1_dqs_byte_md_mask, 0);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte2_glue_cfg0, U_byte2_glue_mc1_dqs_byte_md_offset, U_byte2_glue_mc1_dqs_byte_md_mask, 0);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte3_glue_cfg0, U_byte3_glue_mc1_dqs_byte_md_offset, U_byte3_glue_mc1_dqs_byte_md_mask, 0);
}

void dr1x90_ddr_busmatrix_cfg(ddr_type_t type)
{
    if (type == DDR3_TYPE)
    {
        dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR4, AC_BANK_MTX_IO20_SEL_offset, AC_BANK_MTX_IO20_SEL_mask, 21); // ddr3_a14
        dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR4, AC_BANK_MTX_IO21_SEL_offset, AC_BANK_MTX_IO21_SEL_mask, 25); // ddr3_we_n
        dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR4, AC_BANK_MTX_IO22_SEL_offset, AC_BANK_MTX_IO22_SEL_mask, 36); // ddr3_rst_n
        dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR5, AC_BANK_MTX_IO25_SEL_offset, AC_BANK_MTX_IO25_SEL_mask, 33); // ddr3_ras_n
        dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR7, AC_BANK_MTX_IO36_SEL_offset, AC_BANK_MTX_IO36_SEL_mask, 26); // ddr3_cas_n
    }
}

void dr1x90_iomc_internal_loopback_cfg()
{
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr0_cfg3       , U_byte0_se_mc1_lb_en_offset,      U_byte0_se_mc1_lb_en_mask,   0x1 );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr1to4_cfg11   , U_byte0_quad1_mc1_lb_en_offset,   U_byte0_quad1_mc1_lb_en_mask,   0xf );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg11   , U_byte0_quad2_mc1_lb_en_offset,   U_byte0_quad2_mc1_lb_en_mask,   0xf );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr9to12_cfg11  , U_byte0_quad3_mc1_lb_en_offset,   U_byte0_quad3_mc1_lb_en_mask,   0xf );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr13to16_cfg11 , U_byte1_quad1_mc1_lb_en_offset,   U_byte1_quad1_mc1_lb_en_mask,   0xf  );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg11 , U_byte1_quad2_mc1_lb_en_offset,   U_byte1_quad2_mc1_lb_en_mask,   0xf  );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr21to24_cfg11 , U_byte1_quad3_mc1_lb_en_offset,   U_byte1_quad3_mc1_lb_en_mask,   0xf  );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr25to28_cfg11 , U_byte2_quad1_mc1_lb_en_offset,   U_byte2_quad1_mc1_lb_en_mask,   0xf  );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg11 , U_byte2_quad2_mc1_lb_en_offset,   U_byte2_quad2_mc1_lb_en_mask,   0xf  );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr33to36_cfg11 , U_byte2_quad3_mc1_lb_en_offset,   U_byte2_quad3_mc1_lb_en_mask,   0xf  );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr37to40_cfg11 , U_byte3_quad1_mc1_lb_en_offset,   U_byte3_quad1_mc1_lb_en_mask,   0xf  );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg11 , U_byte3_quad2_mc1_lb_en_offset,   U_byte3_quad2_mc1_lb_en_mask,   0xf  );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr45to48_cfg11 , U_byte3_quad3_mc1_lb_en_offset,   U_byte3_quad3_mc1_lb_en_mask,   0xf  );
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr49_cfg3      , U_byte3_se_mc1_lb_en_offset,      U_byte3_se_mc1_lb_en_mask,      0x1  );
}
