#include <math.h>
#include "dr1x90_crg.h"
#include "dr1x90_ddrc_func.h"
#include "dr1x90_ddrc_init.h"

void dr1x90_ddr_gpll_cfg(double fck, double fsys)
{
    fsys = fsys / 2.0;
    uint32_t regData = 0;
    double fbk = 2.0 * fck / fsys;
    uint32_t fbk_div = lround(fbk);
    uint32_t out_div = 2;
    if (fbk_div <= 60) {
        // double fvco to improve clock stability
        fbk_div *= 2;
        out_div *= 2;
    }
    fck = fsys * fbk_div / out_div;
    AL_DDR_LOG("[DDR GPLL] fck = %.3f MHz, fbk_div = %d, out_div = %d\r\n", fck, fbk_div, out_div);
    
    pll_bw_parm_t parm;
    pll_bw_parm_fetch(fsys * 1e6, fbk_div, 1, &parm, 0x1);
    AL_DDR_LOG("[DDR GPLL] gmc = %d icp = %d lpf_cap = %d lpf_res = %d kvco = %d\r\n", parm.gmc, parm.icp, parm.lpf_cap, parm.lpf_res, parm.kvco);

    pll_ddr_bw_cfg(fsys * 1e6, fbk_div, 1, 0x1);
    pll_ddr_div_set(fbk_div, 1, out_div, out_div * 2, out_div, out_div * 2);

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
    dr1x90_field_wait(DDRC_ADDR_DPLL + BK1_DPLL_SR0, BK0_DPLL_LOCK_offset, BK0_DPLL_LOCK_mask, 1, -1);
}

void dr1x90_ddr_iob_vref_cfg(ddr_vref_t vref_sel, u32 vref)
{
    u32 regDate = 0;
    vref &= 0xFF;

    if (vref_sel == DDR_VREF_EXT)
    {
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0, U_bankref_hp_mc1_vref_sel_offset, U_bankref_hp_mc1_vref_sel_mask, 0x0);
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr0_cfg3, U_byte0_se_mc1_vref_padin_en_offset, U_byte0_se_mc1_vref_padin_en_mask, 1);
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg2, U_bankref_hp_mc1_vref_ext_en_offset, U_bankref_hp_mc1_vref_ext_en_mask, 1);
        dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + brefhp_cfg0, U_bankref_hp_mc1_vref_sel_offset, U_bankref_hp_mc1_vref_sel_mask, 0x0);
        dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr37to40_cfg8, U_byte3_quad1_mc1_vref_padin_en_offset, U_byte3_quad1_mc1_vref_padin_en_mask, 1);
        dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + brefhp_cfg2, U_bankref_hp_mc1_vref_ext_en_offset, U_bankref_hp_mc1_vref_ext_en_mask, 1);
        AL_DDR_LOG("[DDR Vref] External\r\n");
    }
    if (vref_sel == DDR_VREF_INT)
    {
        regDate = dr1x90_reg_read(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0);
        regDate = dr1x90_field_set(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0, U_bankref_hp_mc1_vref_sel_offset, U_bankref_hp_mc1_vref_sel_mask, 0x2, regDate);
        regDate = dr1x90_field_set(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0, U_bankref_hp_mc1_vref1_rank0_sel_offset, U_bankref_hp_mc1_vref1_rank0_sel_mask, 0x1, regDate);
        regDate = dr1x90_field_set(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0, U_bankref_hp_mc1_vref2_rank0_sel_offset, U_bankref_hp_mc1_vref2_rank0_sel_mask, 0x1, regDate);
        regDate = dr1x90_field_set(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0, U_bankref_hp_mc1_vref3_rank0_sel_offset, U_bankref_hp_mc1_vref3_rank0_sel_mask, 0x1, regDate);
        regDate = dr1x90_field_set(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0, U_bankref_hp_mc1_vref4_rank0_sel_offset, U_bankref_hp_mc1_vref4_rank0_sel_mask, 0x1, regDate);
        regDate = dr1x90_field_set(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0, U_bankref_hp_mc1_vref1_rank1_sel_offset, U_bankref_hp_mc1_vref1_rank1_sel_mask, 0x1, regDate);
        regDate = dr1x90_field_set(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0, U_bankref_hp_mc1_vref2_rank1_sel_offset, U_bankref_hp_mc1_vref2_rank1_sel_mask, 0x1, regDate);
        regDate = dr1x90_field_set(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0, U_bankref_hp_mc1_vref3_rank1_sel_offset, U_bankref_hp_mc1_vref3_rank1_sel_mask, 0x1, regDate);
        regDate = dr1x90_field_set(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0, U_bankref_hp_mc1_vref4_rank1_sel_offset, U_bankref_hp_mc1_vref4_rank1_sel_mask, 0x1, regDate);
        dr1x90_reg_write(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg0, regDate);
        dr1x90_reg_write(DDRC_ADDR_BK0_IOMC1 + brefhp_cfg0, regDate);

        regDate = (vref << 0) | (vref << 8) | (vref << 16) | (vref << 24);
        dr1x90_reg_write(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg1  , regDate);
        dr1x90_reg_write(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg4  , regDate);
        dr1x90_reg_write(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg1_1, regDate);
        dr1x90_reg_write(DDRC_ADDR_BK0_IOMC1 + brefhp_cfg1  , regDate);
        dr1x90_reg_write(DDRC_ADDR_BK0_IOMC1 + brefhp_cfg4  , regDate);
        dr1x90_reg_write(DDRC_ADDR_BK0_IOMC1 + brefhp_cfg1_1, regDate);

        AL_DDR_LOG("[DDR Vref] Internal 0x%x\r\n", vref);
    }
}

void dr1x90_ddr_iob_cfg(ddr_type_t type)
{
    for (int i = 0; i < 12; ++i) {
        dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr1to4_cfg13 + 0x50 * i, U_byte0_quad1_mc1_slew_offset, U_byte0_quad1_mc1_slew_mask, 0x0);
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr1to4_cfg13 + 0x50 * i, U_byte0_quad1_mc1_slew_offset, U_byte0_quad1_mc1_slew_mask, 0x0);
        dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr1to4_cfg14 + 0x50 * i, U_byte0_quad1_mc1_pe_ctrl_offset, U_byte0_quad1_mc1_pe_ctrl_mask, 0xFF);
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr1to4_cfg14 + 0x50 * i, U_byte0_quad1_mc1_pe_ctrl_offset, U_byte0_quad1_mc1_pe_ctrl_mask, 0xFF);
    }

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

    // All DQ/DQS pins set to DDR i_mode
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr1to4_cfg10  , U_byte0_quad1_mc1_i_mode_offset, U_byte0_quad1_mc1_i_mode_mask, 0x55);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg10  , U_byte0_quad2_mc1_i_mode_offset, U_byte0_quad2_mc1_i_mode_mask, 0x55);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr9to12_cfg10 , U_byte0_quad3_mc1_i_mode_offset, U_byte0_quad3_mc1_i_mode_mask, 0x55);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr13to16_cfg10, U_byte1_quad1_mc1_i_mode_offset, U_byte1_quad1_mc1_i_mode_mask, 0x55);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg10, U_byte1_quad2_mc1_i_mode_offset, U_byte1_quad2_mc1_i_mode_mask, 0x55);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr21to24_cfg10, U_byte1_quad3_mc1_i_mode_offset, U_byte1_quad3_mc1_i_mode_mask, 0x55);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr25to28_cfg10, U_byte2_quad1_mc1_i_mode_offset, U_byte2_quad1_mc1_i_mode_mask, 0x55);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg10, U_byte2_quad2_mc1_i_mode_offset, U_byte2_quad2_mc1_i_mode_mask, 0x55);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr33to36_cfg10, U_byte2_quad3_mc1_i_mode_offset, U_byte2_quad3_mc1_i_mode_mask, 0x55);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr37to40_cfg10, U_byte3_quad1_mc1_i_mode_offset, U_byte3_quad1_mc1_i_mode_mask, 0x55);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg10, U_byte3_quad2_mc1_i_mode_offset, U_byte3_quad2_mc1_i_mode_mask, 0x55);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr45to48_cfg10, U_byte3_quad3_mc1_i_mode_offset, U_byte3_quad3_mc1_i_mode_mask, 0x55);

    // Set DQS pins to Diff mode
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg8_1  , U_byte0_quad2_mc1_hsinbuf_insel_offset, U_byte0_quad2_mc1_hsinbuf_insel_mask, 0xC);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg8_1, U_byte1_quad2_mc1_hsinbuf_insel_offset, U_byte1_quad2_mc1_hsinbuf_insel_mask, 0xC);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg8_1, U_byte2_quad2_mc1_hsinbuf_insel_offset, U_byte2_quad2_mc1_hsinbuf_insel_mask, 0xC);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg8_1, U_byte2_quad2_mc1_hsinbuf_insel_offset, U_byte2_quad2_mc1_hsinbuf_insel_mask, 0xC);
/*
    // All DQ/DQS pdr_ctrl set to 1
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr1to4_cfg7  , U_byte0_quad1_mc1_pdr_ctrl_offset, U_byte0_quad1_mc1_pdr_ctrl_mask, 0x3);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg7  , U_byte0_quad2_mc1_pdr_ctrl_offset, U_byte0_quad2_mc1_pdr_ctrl_mask, 0x3);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr9to12_cfg7 , U_byte0_quad3_mc1_pdr_ctrl_offset, U_byte0_quad3_mc1_pdr_ctrl_mask, 0x3);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr13to16_cfg7, U_byte1_quad1_mc1_pdr_ctrl_offset, U_byte1_quad1_mc1_pdr_ctrl_mask, 0x3);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg7, U_byte1_quad2_mc1_pdr_ctrl_offset, U_byte1_quad2_mc1_pdr_ctrl_mask, 0x3);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr21to24_cfg7, U_byte1_quad3_mc1_pdr_ctrl_offset, U_byte1_quad3_mc1_pdr_ctrl_mask, 0x3);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr25to28_cfg7, U_byte2_quad1_mc1_pdr_ctrl_offset, U_byte2_quad1_mc1_pdr_ctrl_mask, 0x3);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg7, U_byte2_quad2_mc1_pdr_ctrl_offset, U_byte2_quad2_mc1_pdr_ctrl_mask, 0x3);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr33to36_cfg7, U_byte2_quad3_mc1_pdr_ctrl_offset, U_byte2_quad3_mc1_pdr_ctrl_mask, 0x3);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr37to40_cfg7, U_byte3_quad1_mc1_pdr_ctrl_offset, U_byte3_quad1_mc1_pdr_ctrl_mask, 0x3);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg7, U_byte3_quad2_mc1_pdr_ctrl_offset, U_byte3_quad2_mc1_pdr_ctrl_mask, 0x3);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr45to48_cfg7, U_byte3_quad3_mc1_pdr_ctrl_offset, U_byte3_quad3_mc1_pdr_ctrl_mask, 0x3);
*/
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

    // DQS PUPD mode, for hardware gate training
    if (type == DDR3_TYPE)
    {
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg9  , U_byte0_quad2_mc1_dqs_pupd_offset, U_byte0_quad2_mc1_dqs_pupd_mask, 0x9100);
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg9, U_byte1_quad2_mc1_dqs_pupd_offset, U_byte1_quad2_mc1_dqs_pupd_mask, 0x9100);
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg9, U_byte2_quad2_mc1_dqs_pupd_offset, U_byte2_quad2_mc1_dqs_pupd_mask, 0x9100);
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg9, U_byte3_quad2_mc1_dqs_pupd_offset, U_byte3_quad2_mc1_dqs_pupd_mask, 0x9100);
    }

    for (int i = 0; i < 12; ++i) {
        dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr1to4_cfg9 + 0x50 * i, U_byte0_quad1_mc1_src_snk_sel_offset, U_byte0_quad1_mc1_src_snk_sel_mask, 0xFF);
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr1to4_cfg9 + 0x50 * i, U_byte0_quad1_mc1_src_snk_sel_offset, U_byte0_quad1_mc1_src_snk_sel_mask, 0xAA);
        dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr1to4_cfg9 + 0x50 * i, U_byte0_quad1_mc1_odt_sel_offset, U_byte0_quad1_mc1_odt_sel_mask, 0x55);
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr1to4_cfg9 + 0x50 * i, U_byte0_quad1_mc1_odt_sel_offset, U_byte0_quad1_mc1_odt_sel_mask, 0x55);
    }
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg9  , U_byte0_quad2_mc1_odt_sel_offset, U_byte0_quad2_mc1_odt_sel_mask, 0xEE);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg9, U_byte1_quad2_mc1_odt_sel_offset, U_byte1_quad2_mc1_odt_sel_mask, 0xEE);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg9, U_byte2_quad2_mc1_odt_sel_offset, U_byte2_quad2_mc1_odt_sel_mask, 0xEE);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg9, U_byte3_quad2_mc1_odt_sel_offset, U_byte3_quad2_mc1_odt_sel_mask, 0xEE);
}

void dr1x90_ddr_iol_cfg(ddr_type_t type)
{
    // Always on : 0x11111111; Always off : 0x22222222
    // dr1x90_reg_write(DDRC_ADDR_BK1_T0 + DQSGRP_TE_MD0 , 0x11111111);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T1 + DQSGRP_TE_MD0 , 0x11111111);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T2 + DQSGRP_TE_MD0 , 0x11111111);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T3 + DQSGRP_TE_MD0 , 0x11111111);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T0 + DQSGRP_TE_MD1 , 0x11111111);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T1 + DQSGRP_TE_MD1 , 0x11111111);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T2 + DQSGRP_TE_MD1 , 0x11111111);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T3 + DQSGRP_TE_MD1 , 0x11111111);

    // Always on : 0x11111111; Always off : 0x22222222
    // dr1x90_reg_write(DDRC_ADDR_BK1_T0 + DQSGRP_PDR_MD0, 0x22222222);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T1 + DQSGRP_PDR_MD0, 0x22222222);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T2 + DQSGRP_PDR_MD0, 0x22222222);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T3 + DQSGRP_PDR_MD0, 0x22222222);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T0 + DQSGRP_PDR_MD1, 0x22222222);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T1 + DQSGRP_PDR_MD1, 0x22222222);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T2 + DQSGRP_PDR_MD1, 0x22222222);
    // dr1x90_reg_write(DDRC_ADDR_BK1_T3 + DQSGRP_PDR_MD1, 0x22222222);
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
    //set  *_mc1_rdqsn_gate_dis_en 1
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte0_glue_cfg0, U_byte0_glue_mc1_rdqsn_gate_dis_en_offset, U_byte0_glue_mc1_rdqsn_gate_dis_en_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte1_glue_cfg0, U_byte1_glue_mc1_rdqsn_gate_dis_en_offset, U_byte1_glue_mc1_rdqsn_gate_dis_en_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte2_glue_cfg0, U_byte2_glue_mc1_rdqsn_gate_dis_en_offset, U_byte2_glue_mc1_rdqsn_gate_dis_en_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte3_glue_cfg0, U_byte3_glue_mc1_rdqsn_gate_dis_en_offset, U_byte3_glue_mc1_rdqsn_gate_dis_en_mask, 1);

    //set pdr_md = 2
    dr1x90_reg_write(DDRC_ADDR_BK1_T0 + DQSGRP_PDR_MD0, 0x22222222);
    dr1x90_reg_write(DDRC_ADDR_BK1_T1 + DQSGRP_PDR_MD0, 0x22222222);
    dr1x90_reg_write(DDRC_ADDR_BK1_T2 + DQSGRP_PDR_MD0, 0x22222222);
    dr1x90_reg_write(DDRC_ADDR_BK1_T3 + DQSGRP_PDR_MD0, 0x22222222);
    dr1x90_reg_write(DDRC_ADDR_BK1_T0 + DQSGRP_PDR_MD1, 0x22222222);
    dr1x90_reg_write(DDRC_ADDR_BK1_T1 + DQSGRP_PDR_MD1, 0x22222222);
    dr1x90_reg_write(DDRC_ADDR_BK1_T2 + DQSGRP_PDR_MD1, 0x22222222);
    dr1x90_reg_write(DDRC_ADDR_BK1_T3 + DQSGRP_PDR_MD1, 0x22222222);

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
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr49_cfg3 , U_byte3_se_mc1_lb_en_offset,   U_byte3_se_mc1_lb_en_mask,   0x1  );
}

// zq_code[4]   = { bank0_ac, bank0_dx, bank1_ac, bank1_dx }
// code[31 : 0] = { odt_pu[7:0], odt_pd[7:0], drv_pu[7:0], drv_pd[7:0] }
void dr1x90_zq_overwrite_v2(ddr_type_t type, const u32* zq_code, u32 vref)
{
    u32 regData = 0;
    u32 regDataNew = 0;

    u32 vref_rg_sel = (vref & 0x80) ? 0xF : 0x0;
    u32 vref_ctrl   = (vref & 0x7F);

    dr1x90_field_write(DDRC_ADDR_PPC + PIR, ZCALBYP_offset, ZCALBYP_mask, 1);
    dr1x90_field_write(DDRC_ADDR_PPC + PIR, ZCAL_offset,    ZCAL_mask,    0);

    // ZQ override
    for (int n = 0; n < 2; ++n) {
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0VREF + 0x2000 * n, ZQCALEN_offset,  ZQCALEN_mask,  1);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, ddr_mode_offset, ddr_mode_mask, 1);

        for (int i = 0; i < 2; ++i) {
            regData = zq_code[2 * n + i];
            if (type == DDR4_TYPE && i == 1)
                regData &= 0xFF00FFFFU;

            dr1x90_reg_write(DDRC_ADDR_BK0_VREF + ZQ0DR + 0x2000 * n + 0x10 * i, regData);

            regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n + 0x10 * i);
            regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n + 0x10 * i, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 0, regData);
            regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n + 0x10 * i, PGWAIT_offset, PGWAIT_mask, 1, regDataNew);
            regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n + 0x10 * i, AVGEN_offset, AVGEN_mask, (type == DDR4_TYPE) ? 1 : 0, regDataNew); // ddr4=1 ddr3=0
            dr1x90_reg_update(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n + 0x10 * i, regData, regDataNew);

            regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n + 0x10 * i);
            regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n + 0x10 * i, DRV_ZDEN_offset, DRV_ZDEN_mask, 1, regData);
            regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n + 0x10 * i, ODT_ZDEN_offset, ODT_ZDEN_mask, 1, regDataNew);
            dr1x90_reg_update(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n + 0x10 * i, regData, regDataNew);
        }
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 1);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ1CR + 0x2000 * n, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 1);
    }

    // setup a proper vref for basic functionality
    for (int n = 0; n < 2; ++n) {
        regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, Vref_from_sel_offset,Vref_from_sel_mask, 1, regData);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, Vref_rg_sel_offset, Vref_rg_sel_mask, vref_rg_sel, regDataNew);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, Vref_rk_sel_offset, Vref_rk_sel_mask, 0x0, regDataNew);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, ddr_mode_offset, ddr_mode_mask, (type == DDR4_TYPE) ? 1 : 0, regDataNew);
        dr1x90_reg_update(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, regData, regDataNew);

        regData = (vref_ctrl << Vref1_ctrl_rank0_offset) | (vref_ctrl << Vref1_ctrl_rank1_offset);
        dr1x90_reg_write(DDRC_ADDR_BK0_VREF + VREF1 + 0x2000 * n, regData);
        dr1x90_reg_write(DDRC_ADDR_BK0_VREF + VREF2 + 0x2000 * n, regData);
        dr1x90_reg_write(DDRC_ADDR_BK0_VREF + VREF3 + 0x2000 * n, regData);
        dr1x90_reg_write(DDRC_ADDR_BK0_VREF + VREF4 + 0x2000 * n, regData);
    }
}
