#include <math.h>
#include "alc_ddr_env.h"
#include "dr1x90_ddrc_func.h"
#include "dr1x90_ddrc_init.h"

// task #
void dr1x90_ddrppc_dx_enable(u32 lane_mask)
{
    for (int i = 0; i < 9; i++ ) {
        /*
        if (i < width) {
            dr1x90_field_write(DDRC_ADDR_PPC + DX0GCR0 + 0x100 * i, DXEN_offset, DXEN_mask, 1);
        } else {
            dr1x90_field_write(DDRC_ADDR_PPC + DX0GCR0 + 0x100 * i, DXEN_offset, DXEN_mask, 0);
        }
        */
        u32 lane_en = lane_mask & (0x1 << i);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GCR0 + 0x100 * i, DXEN_offset, DXEN_mask, lane_en ? 1 : 0);
    }
    AL_DDR_LOG("[DDR PPC] Enable Lane Mask = 0x%X\r\n", lane_mask);
}

// task #
void dr1x90_ddrppc_base_cfg(ddr_type_t type, u32 lane_mask)
{
    u8 i ;
    u32 regData;

//  dr1x90_field_write(DDRC_ADDR_PPC + PIR, DCALPSE_offset, DCALPSE_mask, 1);
    dr1x90_field_write(DDRC_ADDR_PPC + PIR, ZCALBYP_offset, ZCALBYP_mask, 1);

    // set PUB Mode, take over the control of DFI interface
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGCR1);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR1, PUBMODE_offset, PUBMODE_mask, 1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR1, DUALCHN_offset, DUALCHN_mask, 0, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PGCR1, regData);

    // pub aligned fifo byp enable
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR3, PRFBYP_offset, PRFBYP_mask, env_prfbyp_en);

    // static mode configure
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR3, RDMODE_offset, RDMODE_mask, env_cfg_static_mode);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + DSGCR);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DSGCR, MODE81_EN_offset, MODE81_EN_mask, 0, regData); // 4:1 mode
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DSGCR, RRRMODE_offset,   RRRMODE_mask,   env_cfg_rd_rr_mode, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DSGCR, WRRMODE_offset,   WRRMODE_mask,   env_cfg_wr_rr_mode, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + DSGCR, regData);

    // configure DRAM system: DDR3/4 mode, no 2T, no simultaneous rank access
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + DCR);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DCR, DDRMD_offset, DDRMD_mask, type, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DCR, NOSRA_offset, NOSRA_mask, 1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DCR, DDR2T_offset, DDR2T_mask, 0, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + DCR, regData);

    // disable write post-amble extension
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR7, WRPSTEX_offset, WRPSTEX_mask, env_cfg_wrpstex);

    // enable configured DXn
    dr1x90_ddrppc_dx_enable(lane_mask);

    // added based on AL-1055 requirement
    for (i = 0; i <= 8; i++) {
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GCR9 + 0x100 * i, pdr_rtt_alat_offset, pdr_rtt_alat_mask, 2);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GCR9 + 0x100 * i, te_rtt_alat_offset,  te_rtt_alat_mask,  2);
    }

    // enable rank
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + DTCR1);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RANKEN_offset, RANKEN_mask, 1, regData);
    if (env_cfg_ddr_speed < 1000)
        regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVLGS2_offset, RDLVLGS2_mask, 7 , regData);    ////# 3'b111 -1000Mbps- 3'b110 -1300Mbps- 3'b100
    else if (env_cfg_ddr_speed < 1300)
        regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVLGS2_offset, RDLVLGS2_mask, 6 , regData);    ////# 3'b111 -1000Mbps- 3'b110 -1300Mbps- 3'b100
    else
        regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVLGS2_offset, RDLVLGS2_mask, 4 , regData);    ////# 3'b111 -1000Mbps- 3'b110 -1300Mbps- 3'b100
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVLGS2_EN_offset,          RDLVLGS2_EN_mask,          1 , regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVL_VALIDATE_BYP_offset,   RDLVL_VALIDATE_BYP_mask,   0 , regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVL_VALIDATE_VALUE_offset, RDLVL_VALIDATE_VALUE_mask, 15, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + DTCR1, regData);

    // delay line related configuration
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGCR1);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR1, DUALCHN_offset, DUALCHN_mask, 0, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR1, IODDRM_offset,  IODDRM_mask,  (type == DDR4_TYPE) ? 1 : 0, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR1, FDEPTH_offset,  FDEPTH_mask,  0, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PGCR1, regData);

    // cal clock chose, ddr x4 clock or ctl_clk 1x, default use ddr_clk
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGCR7);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR7, DXCALCLK_offset, DXCALCLK_mask, 0, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR7, ACCALCLK_offset, ACCALCLK_mask, 0, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PGCR7, regData);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGCR5);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR5, ac_dly_gten_offset, ac_dly_gten_mask, 0x1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR5, dx_dly_gten_offset, dx_dly_gten_mask, 0x1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR5, ac_wdqs_gten_offset, ac_wdqs_gten_mask, 0x0, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PGCR5, regData);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGCR4);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR4, acdlyld_gten_offset, acdlyld_gten_mask, 0x1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR4, dxdlyld_gten_offset, dxdlyld_gten_mask, 0x1, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PGCR4, regData);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGCR6);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR6, gt_pipe_sel_pub_offset, gt_pipe_sel_pub_mask, 0x4, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR6, gt_pipe_sel_iol_offset, gt_pipe_sel_iol_mask, 0x1, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PGCR6, regData);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGCR8);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR8, rankdly_pipe_sel_offset, rankdly_pipe_sel_mask, 0x3, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR8, gt_width_sel_pub_offset, gt_width_sel_pub_mask, 0x1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR8, gt_width_sel_iol_offset, gt_width_sel_iol_mask, 0x1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR8, ranksel_mode_offset    , ranksel_mode_mask    , 0x1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR8, gt_pipe_sel_dlyld_offset, gt_pipe_sel_dlyld_mask, 0x1, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PGCR8, regData);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PTR2);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR2, tCALON_offset, tCALON_mask, 0x1F, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR2, tCALS_offset,  tCALS_mask,  0x13, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR2, tCALH_offset,  tCALH_mask,  0x13, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PTR2, regData);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PLLCR0);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PLLCR0, gsdqs_ddl_cal_bypass_offset, gsdqs_ddl_cal_bypass_mask, 1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PLLCR0, tx_fifo_bypass_offset,       tx_fifo_bypass_mask,       env_cfg_tx_fifo_bypass, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PLLCR0, dx_gate_rd_io_sel_offset,    dx_gate_rd_io_sel_mask,    1, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PLLCR0, regData);

    // AL-1110 set read to write cmd gap during training
//    u32 dt_r2w_gap = 14;
//    regData    = dr1x90_reg_read(PHY_DT_CMD0);
//    regData = dr1x90_field_set(PHY_DT_CMD0, dt_r2w_nop_rpt_max_offset, dt_r2w_nop_rpt_max_mask, dt_r2w_gap, regData);
//    dr1x90_reg_write(PHY_DT_CMD0, regData);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGCR3);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR3, PGCR3_RDBI_offset, PGCR3_RDBI_mask, env_cfg_RDBI, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR3, PGCR3_WDBI_offset, PGCR3_WDBI_mask, env_cfg_WDBI, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PGCR3, regData);

    dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, ACWLPON_offset, ACWLPON_mask, 1);
    dr1x90_field_write(DDRC_ADDR_PPC + DSGCR, LPIOPD_offset, LPIOPD_mask, 0);
}

void dr1x90_ddrppc_set_timing()
{
    u32 regData;

    // u32 tdinit_cke_high, tdinit_cke_low, tdinit_zq, tdinit_rst;
    // tdinit_cke_high = 0;
    // tdinit_cke_low  = 400000;
    // tdinit_zq       = 0;
    // tdinit_rst      = 160000;

    //  PTR0                             0x040
    regData = dr1x90_reg_read (DDRC_ADDR_PPC + PTR0);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR0, tPLLPD_offset,        tPLLPD_mask,       0x10, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR0, tPLLGS_offset,        tPLLGS_mask,       0x10, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR0, tPHYRST_offset,       tPHYRST_mask,      0x10, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PTR0, regData);
    //  PTR1                             0x044
    regData = dr1x90_reg_read (DDRC_ADDR_PPC + PTR1);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR1, tPLLLOCK_offset,      tPLLLOCK_mask,     0x01, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR1, tPLLRST_offset,       tPLLRST_mask,      0x10, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PTR1, regData);
    //  PTR2                             0x048
    regData = dr1x90_reg_read (DDRC_ADDR_PPC + PTR2);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR2, cfg_wlmd_sel_offset,  cfg_wlmd_sel_mask, 0, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR2, ac_cklcdl_byp_offset, ac_cklcdl_byp_mask,0, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR2, ctl_ck_dly_offset,    ctl_ck_dly_mask,   0, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR2, tWLDLYS_offset,       tWLDLYS_mask,      0x10, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR2, tCALH_offset,         tCALH_mask,        0x13, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR2, tCALS_offset,         tCALS_mask,        0x13, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR2, tCALON_offset,        tCALON_mask,       0x1f, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PTR2, regData);
    //  PTR3                             0x04c
    regData = dr1x90_reg_read (DDRC_ADDR_PPC + PTR3);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR3, tDINIT1_offset,       tDINIT1_mask,  0x180, regData); // tdinit_cke_high
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR3, tDINIT0_offset,       tDINIT0_mask,0x60000, regData); // tdinit_cke_low
    // regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR3, tDINIT0_offset,       tDINIT0_mask,  0x600, regData); // tdinit_cke_low
    dr1x90_reg_write(DDRC_ADDR_PPC + PTR3, regData);
    //  PTR4                             0x050
    regData = dr1x90_reg_read (DDRC_ADDR_PPC + PTR4);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR4, tDINIT3_offset,       tDINIT3_mask,  0x0C8, regData); // tdinit_zq
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR4, tDINIT2_offset,       tDINIT2_mask,0x30000, regData); // tdinit_rst
    // regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR4, tDINIT2_offset,       tDINIT2_mask,  0x300, regData); // tdinit_rst
    dr1x90_reg_write(DDRC_ADDR_PPC + PTR4, regData);
    //  PTR5                             0x054
    regData = dr1x90_reg_read (DDRC_ADDR_PPC + PTR5);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR5, tPLLFRQSEL_offset,    tPLLFRQSEL_mask, 0x08, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR5, tPLLFFCRGS_offset,    tPLLFFCRGS_mask,0x1AB, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR5, tPLLFFCGS_offset,     tPLLFFCGS_mask, 0x356, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PTR5, regData);
    //  PTR6                             0x058
    regData = dr1x90_reg_read (DDRC_ADDR_PPC + PTR6);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR6, tPLLRLCK1_offset,     tPLLRLCK1_mask, 0x14d8, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PTR6, regData);
}

// task #
void dr1x90_ddrppc_set_ddrmr(double fck, ddr_type_t type, const ddr_timing_t* timpara)
{
    u32 MR[7] = {0};
    make_ddr_mr(fck, type, timpara, MR);
    // u32 MR[7] = {MR0, MR1, MR2, MR3, MR4, MR5, MR6};

    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR0, MR[0]);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR1, MR[1]);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR2, MR[2]);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR3, MR[3]);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR4, MR[4]);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR5, MR[5]);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR6, MR[6]);

    AL_DDR_LOG("[DDR MRS] DDR%d Mode\r\n", type);
    int n = (type == DDR3_TYPE) ? 4 : 7;
    for (int i = 0; i < n; ++i) {
        AL_DDR_LOG("[DDR MRS] MR%d = 0x%x\r\n", i, MR[i]);
    }
}

void dr1x90_ddrppc_mdl_cal()
{
    u32 regData = 0;

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PIR, DCAL_offset, DCAL_mask, 1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 1, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);

    dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, DCDONE_offset, DCDONE_mask, 1, -1);
    dr1x90_field_wait(DDRC_ADDR_PPC + PIR,   INIT_offset,   INIT_mask,   0, -1);
/*
    regData = dr1x90_reg_read(0x27a0);
    AL_DDR_LOG("[DDR MDL CAL] DX0 mdl = 0x%x\r\n", regData);
    regData = dr1x90_reg_read(0x28a0);
    AL_DDR_LOG("[DDR MDL CAL] DX1 mdl = 0x%x\r\n", regData);
    regData = dr1x90_reg_read(0x29a0);
    AL_DDR_LOG("[DDR MDL CAL] DX2 mdl = 0x%x\r\n", regData);
    regData = dr1x90_reg_read(0x2aa0);
    AL_DDR_LOG("[DDR MDL CAL] DX3 mdl = 0x%x\r\n", regData);
*/
}

// task #
void dr1x90_ddrppc_dram_init()
{
    u32 regData = 0;
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PIR, DRAMRST_offset,  DRAMRST_mask,  1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PIR, DRAMINIT_offset, DRAMINIT_mask, 1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PIR, INIT_offset,     INIT_mask,     1, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);

    dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, DIDONE_offset, DIDONE_mask, 1, -1);
}

void dr1x90_mtest_set_addr(u8 bank, u16 row, u16 col)
{
    u32 regData = 0;

    regData = 0;
    regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTAR0, BBANK_offset, BBANK_mask, bank, regData); // bank
    regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTAR0, BCOL_offset,  BCOL_mask,  col , regData); // col
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTAR0, regData);

    regData = 0;
    regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTAR1, BMRANK_offset, BMRANK_mask, 0, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTAR1, BAINC_offset,  BAINC_mask,  8, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTAR1, BRANK_offset,  BRANK_mask,  0, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTAR1, regData);

    regData = 0;
    regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTAR2, BMBANK_offset, BMBANK_mask, 7,     regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTAR2, BMCOL_offset,  BMCOL_mask,  0x3f8, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTAR2, regData);

    dr1x90_field_write(DDRC_ADDR_PPC + BISTAR3, BROW_offset,  BROW_mask, row);  // row
    dr1x90_field_write(DDRC_ADDR_PPC + BISTAR4, BMROW_offset, BMROW_mask, 0x7fff);
}

// task #
int dr1x90_ddrppc_mtest(u8 bank, u16 row, u16 col, u8 byteNum)
{
    u8  patternType;
    u8  iterateCnt = 128;   // 240
    u32 regData;
    u8  bistDone;
    u16 bistDxErr;
    u8  rcvCnt;
    int mbist_cnt = 0;
    int mbist_fail_cnt = 0;
    int loopback_mode_int_en = 0;

    dr1x90_field_write(DDRC_ADDR_PPC + PGCR1, LBMODE_offset, LBMODE_mask, loopback_mode_int_en);
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR1, IOLB_offset, IOLB_mask, loopback_mode_int_en);
    dr1x90_field_write(DDRC_ADDR_PPC + BISTWCR, BWCNT_offset, BWCNT_mask, iterateCnt);

    dr1x90_reg_write(DDRC_ADDR_PPC + BISTRR   , 0x00000003);
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTLSR  , 0x8DED3E2A);
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTUDPR , 0x1DF428D5);
    dr1x90_reg_write(DDRC_ADDR_PPC + DTDR0    , 0xff00ff00);
    dr1x90_reg_write(DDRC_ADDR_PPC + DTDR1    , 0x00ff00ff);
    dr1x90_reg_write(DDRC_ADDR_PPC + UDDR0    , 0x55aa55aa);
    dr1x90_reg_write(DDRC_ADDR_PPC + UDDR1    , 0x5aa55aa5);

    dr1x90_mtest_set_addr(bank, row, col);

    dr1x90_reg_write(DDRC_ADDR_PPC + BISTMSKR0 , 0xFFFFFFFF);
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTMSKR1 , 0xFFFFFFFF);
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTMSKR2 , 0x00000000);

    for (patternType = 0x0; patternType <= 0x17; patternType++) {
        dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 0);
        dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 1);

        if ( patternType == 0x18  ) continue;

        regData = dr1x90_reg_read(DDRC_ADDR_PPC + BISTRR);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BINST_offset,  BINST_mask, 1, regData);

        //bist mode =1
        //regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BMODE_offset,  BMODE_mask, 1, regData);
        //loopback mode = 0
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BMODE_offset,  BMODE_mask, loopback_mode_int_en ? 0 : 1 , regData);

        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BACEN_offset,  BACEN_mask, 0, regData);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BDXEN_offset,  BDXEN_mask, 1, regData);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BDPAT_offset,  BDPAT_mask, patternType, regData);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BDXSEL_offset, BDXSEL_mask, byteNum, regData);
        dr1x90_reg_write(DDRC_ADDR_PPC + BISTRR, regData);

        dr1x90_field_wait(DDRC_ADDR_PPC + BISTGSR, BDONE_offset, BDONE_mask, 1, -1); // wait for bistDone

        bistDone  = dr1x90_field_read(DDRC_ADDR_PPC + BISTGSR,  BDONE_offset,  BDONE_mask);
        bistDxErr = dr1x90_field_read(DDRC_ADDR_PPC + BISTGSR,  BDXERR_offset, BDXERR_mask);
        rcvCnt    = dr1x90_field_read(DDRC_ADDR_PPC + BISTWCSR, DXWCNT_offset, DXWCNT_mask); // Equal to iterateCnt

        // AL_DDR_LOG("[DDR MTEST] #0x%x, bistDone = 0x%x, DxErr = 0x%x, rcvCnt = %d\r\n", patternType, bistDone, bistDxErr, rcvCnt);

        dr1x90_field_write(DDRC_ADDR_PPC + BISTRR, BINST_offset, BINST_mask, 3);
        dr1x90_field_write(DDRC_ADDR_PPC + BISTRR, BINST_offset, BINST_mask, 2);
        dr1x90_field_write(DDRC_ADDR_PPC + BISTRR, BINST_offset, BINST_mask, 0);

        if (bistDone != 1 || bistDxErr != 0 || rcvCnt != iterateCnt) {
            mbist_fail_cnt++;
            AL_DDR_LOG("[DDR MTEST] #0x%x, bistDone = 0x%x, DxErr = 0x%x, rcvCnt = %d\r\n", patternType, bistDone, bistDxErr, rcvCnt);
            // AL_DDR_LOG("[DDR MTEST] #0x%x, DX%d, bistDone = 0x%x, DxErr = 0x%x, rcvCnt = %d\r\n", patternType, byteNum, bistDone, bistDxErr, rcvCnt);
        }
        mbist_cnt++;
    }

    AL_DDR_LOG("[DDR MTEST] Done: %d in %d\r\n", mbist_cnt - mbist_fail_cnt, mbist_cnt);

    return mbist_fail_cnt ;
}

// task #
void dr1x90_ddrppc_fast_init(double fck, u32 lane_mask, const ddr_train_t* cfg)
{
    u32 mdl[4];
    for (int i = 0; i < 4; ++i) {
        mdl[i] = dr1x90_field_read(DDRC_ADDR_PPC + DX0MDLR0 + 0x100 * i, TPRD_offset, TPRD_mask);
    }
    AL_DDR_LOG("[FAST INIT] MDL = %d %d %d %d\r\n", mdl[0], mdl[1], mdl[2], mdl[3]);

    dr1x90_field_write(DDRC_ADDR_PPC + RANKIDR, RANKWID_offset, RANKWID_mask, 0);
    dr1x90_field_write(DDRC_ADDR_PPC + RANKIDR, RANKRID_offset, RANKRID_mask, 0);

    for (int i = 0; i < 4; ++i) {
        double tLCDL = 1e6 / fck / (double)mdl[i];
        // Set by Write Leveling (WL) & WL Adjustment (WLADJ)
        u32 wlsl = 2;
        if (cfg->wlsl[i] != (u16)-1)
            wlsl = (u32)cfg->wlsl[i];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * i, WLSL_offset, WLSL_mask, wlsl);

        u32 wld = (u32)lround((cfg->ac_dly[i] - cfg->dx_dly[i]) / tLCDL);
        if (cfg->wld[i] != (u16)-1)
            wld = (u32)cfg->wld[i];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR0 + 0x100 * i, WLD_offset, WLD_mask, wld + 1);

        // Set by Gate Training
        u32 dgsl = 3;
        if (cfg->dgsl[i] != (u16)-1)
            dgsl = (u32)cfg->dgsl[i];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * i, DGSL_offset, DGSL_mask, dgsl);

        u32 dqsgd = wld + mdl[i] / 4;
        if (cfg->dqsgd[i] != (u16)-1)
            dqsgd = (u32)cfg->dqsgd[i];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * i, DQSGD_offset, DQSGD_mask, dqsgd + 1);

        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR5 + 0x100 * i, DQSGSD_offset, DQSGSD_mask, mdl[i] / 16); // UI / 8

        // Set by Write Eye Training
        u32 wdqd = mdl[i] / 4;
        if (cfg->wdqd[i] != (u16)-1)
            wdqd = (u32)cfg->wdqd[i];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * i, WDQD_offset, WDQD_mask, wdqd + 1);

        // Set by Read Eye Training
        u32 rdqsd = mdl[i] / 4 + 10;
        if (cfg->rdqsd[i] != (u16)-1)
            rdqsd = (u32)cfg->rdqsd[i];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * i, RDQSD_offset, RDQSD_mask, rdqsd + 1);

        u32 rdqsnd = mdl[i] / 4;
        if (cfg->rdqsnd[i] != (u16)-1)
            rdqsnd = (u32)cfg->rdqsnd[i];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * i, RDQSND_offset, RDQSND_mask, rdqsnd + 1);
    }

    dr1x90_field_write(DDRC_ADDR_PPC + PGCR4, DXDDLLD_offset, DXDDLLD_mask, 0);
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR4, DXDDLLD_offset, DXDDLLD_mask, 0x1f);
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR4, DXDDLLD_offset, DXDDLLD_mask, 0);

    dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 0);
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 1);
}

void dr1x90_zq_overwrite_cfg(ddr_type_t type) {

    u8 n ;
    u8 i ;
    // u8 BankInfo ;
    u8 override_en = 1;

    // zq_code: { odt_pu[7:0], odt_pd[7:0], drv_pu[7:0], drv_pd[7:0] }
    // u8 pzq_cal_done ;

    // u32  bank_zq_ac_code[3] = {0x19193235,0x19193235,0x19193235} ;
    // u32  bank_zq_dx_code[3] = {0x19193235,0x19193235,0x19193235} ;
    // u32  bank_zq_ac_code[3] = {0x11112323,0x11112323,0x11112323} ;
    // u32  bank_zq_dx_code[3] = {0x11112323,0x11112323,0x11112323} ;
    u32  bank_zq_ac_code[3] = {0x17173232,0x17173232,0x17173232} ;
    u32  bank_zq_dx_code[3] = {0x17173232,0x17173232,0x17173232} ;

    // DDR4
    // u32  bank_zq_ac_code[3] = {0x2E003232,0x2E003232,0x2E003232} ;
    // u32  bank_zq_dx_code[3] = {0x2E003232,0x2E003232,0x2E003232} ;

    // u32  zqcal_done;
    // u32  zqcal_err ;

    u32 regData, regDataNew ;

    // u32 env_cfg_dci_cascade = 0;
    u8 env_cfg_host_vrefr = 0xf;
    u8 env_cfg_host_vref = 0x1f;

    // pzq_cal_done = 0 ;

     if ( override_en == 1 ) {
        dr1x90_field_write(DDRC_ADDR_PPC + PIR, ZCALBYP_offset, ZCALBYP_mask, 1);
        dr1x90_field_write(DDRC_ADDR_PPC + PIR, ZCAL_offset,    ZCAL_mask,    0);
              // ZQ override
        for (n = 0; n <= 1; n++) {
            //if ( (n == 1) && (env_cfg_pzq_slave == 1) ) continue ;
            //BankInfo = Alc_DramFieldRead(SYS_BYTE_MAP_INFO_0, 8 * (2-n), 0xff << 8*(2-n) );
            //if (BankInfo == 0xff) continue ;

            dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0VREF + 0x2000 * n, ZQCALEN_offset,  ZQCALEN_mask,  1);
            dr1x90_field_write(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, ddr_mode_offset, ddr_mode_mask, 1);

            for (i = 0; i <= 1; i++) {
                if ( i == 0 ) {
                    regData = bank_zq_ac_code[n];
                } else{
                    regData =  ((type == DDR4_TYPE) ? (bank_zq_dx_code[n] & 0xff00ffff) : bank_zq_dx_code[n] ) ;
                }

                dr1x90_reg_write(DDRC_ADDR_BK0_VREF + ZQ0DR + 0x2000 * n + 0x10 * i, regData);

                regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF+ZQ0CR + 0x2000 * n + 0x10 * i);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+ZQ0CR + 0x2000 * n + 0x10 * i, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 0,  regData);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+ZQ0CR + 0x2000 * n + 0x10 * i, PGWAIT_offset, PGWAIT_mask, 1,  regDataNew);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+ZQ0CR + 0x2000 * n + 0x10 * i, AVGEN_offset, AVGEN_mask, (type == DDR4_TYPE) ? 1 : 0, regDataNew); // ddr4=1 ddr3=0
                dr1x90_reg_update(DDRC_ADDR_BK0_VREF+ZQ0CR + 0x800 * n + 0x10 * i, regData, regDataNew);

                regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF+ZQ0PR + 0x2000 * n + 0x10 * i);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+ZQ0PR + 0x2000 * n + 0x10 * i, DRV_ZDEN_offset, DRV_ZDEN_mask, 1,  regData);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+ZQ0PR + 0x2000 * n + 0x10 * i, ODT_ZDEN_offset, ODT_ZDEN_mask, 1,  regDataNew);
                dr1x90_reg_update(DDRC_ADDR_BK0_VREF+ZQ0PR + 0x2000 * n + 0x10 * i, regData, regDataNew);
            }
            dr1x90_field_write(DDRC_ADDR_BK0_VREF+ZQ0CR+ 0x2000 * n , FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 1);
            dr1x90_field_write(DDRC_ADDR_BK0_VREF+ZQ1CR+ 0x2000 * n , FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 1);
        }
    }

    // setup a proper vref for basic functionality
    for (n = 0; n <= 1; n++) {
        //if ( (n == 1) && (env_cfg_pzq_slave == 1) ) continue ;
        // check if the bank is used, if not, skip the config
        //BankInfo = Alc_DramFieldRead(SYS_BYTE_MAP_INFO_0, 8 * (2-n), 0xff << 8*(2-n) );
        //if (BankInfo == 0xff) continue ;

        regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF+PZQVREF + 0x2000 * n);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+PZQVREF + 0x2000 * n, Vref_from_sel_offset,Vref_from_sel_mask, 1, regData);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+PZQVREF + 0x2000 * n, Vref_rg_sel_offset, Vref_rg_sel_mask,  env_cfg_host_vrefr, regDataNew);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+PZQVREF + 0x2000 * n, Vref_rk_sel_offset, Vref_rk_sel_mask,  0x0, regDataNew);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+PZQVREF + 0x2000 * n, ddr_mode_offset, ddr_mode_mask, (type == DDR4_TYPE) ? 1 : 0, regDataNew);
        dr1x90_reg_update(DDRC_ADDR_BK0_VREF+PZQVREF + 0x2000 * n, regData, regDataNew);

        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF1 + 0x2000 * n, Vref1_ctrl_rank0_offset, Vref1_ctrl_rank0_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF2 + 0x2000 * n, Vref2_ctrl_rank0_offset, Vref2_ctrl_rank0_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF3 + 0x2000 * n, Vref3_ctrl_rank0_offset, Vref3_ctrl_rank0_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF4 + 0x2000 * n, Vref4_ctrl_rank0_offset, Vref4_ctrl_rank0_mask, env_cfg_host_vref);

        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF1 + 0x2000 * n, Vref1_ctrl_rank1_offset, Vref1_ctrl_rank1_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF2 + 0x2000 * n, Vref2_ctrl_rank1_offset, Vref2_ctrl_rank1_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF3 + 0x2000 * n, Vref3_ctrl_rank1_offset, Vref3_ctrl_rank1_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF4 + 0x2000 * n, Vref4_ctrl_rank1_offset, Vref4_ctrl_rank1_mask, env_cfg_host_vref);
    }
}

void dr1x90_ac_swap_cfg()
{
   dr1x90_field_write( DDRC_ADDR_GPLL+DTCR0, BK0_IS_AC_offset, BK0_IS_AC_mask, 0x0 );
}


void dr1x90_pub_training_cfg(ddr_type_t type)
{
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR0, DTWBDDM_offset, DTWBDDM_mask, 0x1 );	 // DM or DBI
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR0, DTRDBITR_offset,  DTRDBITR_mask,  0);
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR0, DTBDC_offset,  DTBDC_mask,  1);
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR0, RFSHDT_offset,  RFSHDT_mask,  8);
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR0, DTMPR_offset,  DTMPR_mask,  1);
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR1, RDPRMBL_TRN_offset,  RDPRMBL_TRN_mask,  1);
   dr1x90_field_write( DDRC_ADDR_PPC + DXCCR, QSCNTEN_offset,  QSCNTEN_mask,  1);
   if (type == DDR3_TYPE) {
      dr1x90_field_write( DDRC_ADDR_PPC + OFFCAL_PR, dqs_pupd_en_offset,  dqs_pupd_en_mask,  1);
   }
   dr1x90_field_write( DDRC_ADDR_PPC + OFFCAL_PR, eye_rdbi_rpt_en_offset,  eye_rdbi_rpt_en_mask,  1);
   dr1x90_field_write( DDRC_ADDR_PPC + PHY_EYE_CR3, eye_rdbi_train_type1_offset,  eye_rdbi_train_type1_mask,  1);
}

void dr1x90_do_training(u8 wl_en, u8 wl2_en, u8 gt_en )
{
    u32 regData;
    u8 gterr;

    if ( wl_en == 0x1 ) {
        dr1x90_field_write( DDRC_ADDR_PPC + PIR, WL_offset, WL_mask, 0x1 );	 // WL
    }
    if ( wl2_en == 0x1 ) {
		if ( wl_en == 0x0 ) {
            dr1x90_field_write( DDRC_ADDR_PPC + PIR, WLADJ_offset, WLADJ_mask, 0x1 );	 // WLADJ
		}
		if ( wl_en == 0x1 ) {
            dr1x90_field_write( DDRC_ADDR_PPC + PIR, WLADJ_offset, WLADJ_mask, 0x1 );	 // WLADJ
		}
	}

	if ( gt_en == 0x1 ) {
        dr1x90_field_write( DDRC_ADDR_PPC + PIR, QSGATE_offset, QSGATE_mask, 0x1 );	 // QSGATE
	}

    if ( (wl2_en || wl_en || gt_en )  == 1 ) {
        dr1x90_field_write( DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x1 );	 // QSGATE
	}

    if ( wl_en == 0x1 ) {
		dr1x90_field_wait (DDRC_ADDR_PPC+ PGSR0, WLDONE_offset,  WLDONE_mask,  1,  -1 );
	}

	if ( wl2_en == 0x1 ) {
		dr1x90_field_wait (DDRC_ADDR_PPC+ PGSR0, WLADONE_offset,  WLADONE_mask,  1,  -1 );
	}

	if ( gt_en == 0x1 ) {
		dr1x90_field_wait (DDRC_ADDR_PPC+ PGSR0, QSGDONE_offset,  QSGDONE_mask,  1,  -1 );
	}

    if (gt_en == 1) {
        regData = dr1x90_field_read(DDRC_ADDR_PPC + PGSR0, QSGDONE_offset, QSGDONE_mask);
        AL_DDR_LOG("[DDR WL & GATE] ------- GateTraining done = 0x%x\r\n", regData);
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        gterr = (regData >> 22) & 0x1;
        if (gterr == 1) {
            AL_DDR_LOG("[DDR WL & GATE] ------- GateTraining Failed ; PGSR0 is 0x%x\r\n", regData);
        }
        if (gterr == 0) {
            AL_DDR_LOG("[DDR WL & GATE] ------- GateTraining PASSED ; PGSR0 is 0x%x\r\n", regData);
        }
    }
    // dr1x90_ddrppc_show_delay();
}

void dr1x90_ddrppc_show_delay()
{
    AL_DDR_LOG("========================================\r\n");
    AL_DDR_LOG("DX DGSL WLSL WLD WDQD DQSGD RDQSD RDQSND\r\n");
    AL_DDR_LOG("----------------------------------------\r\n");
    for (int n = 0; n < 4; n++) {
        u32 dgsl   = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, DGSL_offset, DGSL_mask);
        u32 wlsl   = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, WLSL_offset, WLSL_mask);
        u32 wld    = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR0 + 0x100 * n, WLD_offset, WLD_mask);
        u32 wdqd   = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * n, WDQD_offset, WDQD_mask);
        u32 dqsgd  = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask);
        u32 rdqsd  = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask);
        u32 rdqsnd = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask);
        AL_DDR_LOG("%2d %4d %4d %3d %4d %5d %5d %6d\r\n", n, dgsl, wlsl, wld, wdqd, dqsgd, rdqsd, rdqsnd);
    }
    AL_DDR_LOG("========================================\r\n");
}

void dr1x90_ddrppc_show_mdl(double fck, u32 lane_mask)
{
    u32 regData = 0;
    for (int i = 0; i < 4; ++i) {
        u32 lane_en = lane_mask & (0x1 << i);
        if (lane_en == 0)
            continue;
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + DX0MDLR0 + 0x100 * i);
        double t = 1e6 / fck / (double)(regData & MDLD_mask);
        AL_DDR_LOG("[DDR MDL] DX%d = 0x%x\t%.3f ps\r\n", i, regData, t);
    }
}

void dr1x90_ddrppc_show_bdl()
{
    AL_DDR_LOG("==================================================\r\n");
    AL_DDR_LOG("DX DQ0 DQ1 DQ2 DQ3 DQ4 DQ5 DQ6 DQ7  DM DQS OE/DQSN\r\n");
    AL_DDR_LOG("--------------------------------------------------\r\n");
    for (int i = 0; i < 4; ++i) {
        u32 bdlr0 = dr1x90_reg_read(DDRC_ADDR_PPC + DX0BDLR0 + 0x100 * i);
        u32 bdlr1 = dr1x90_reg_read(DDRC_ADDR_PPC + DX0BDLR1 + 0x100 * i);
        u32 bdlr2 = dr1x90_reg_read(DDRC_ADDR_PPC + DX0BDLR2 + 0x100 * i);
        AL_DDR_LOG("W%d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\r\n", i, 
            (bdlr0 >> DQ0WBD_offset) & 0x3F, (bdlr0 >> DQ1WBD_offset) & 0x3F, 
            (bdlr0 >> DQ2WBD_offset) & 0x3F, (bdlr0 >> DQ3WBD_offset) & 0x3F, 
            (bdlr1 >> DQ4WBD_offset) & 0x3F, (bdlr1 >> DQ5WBD_offset) & 0x3F, 
            (bdlr1 >> DQ6WBD_offset) & 0x3F, (bdlr1 >> DQ7WBD_offset) & 0x3F, 
            (bdlr2 >>  DMWBD_offset) & 0x3F, (bdlr2 >>  DSWBD_offset) & 0x3F, (bdlr2 >> DSOEBD_offset) & 0x3F
        );
    }
    for (int i = 0; i < 4; ++i) {
        u32 bdlr3 = dr1x90_reg_read(DDRC_ADDR_PPC + DX0BDLR3 + 0x100 * i);
        u32 bdlr4 = dr1x90_reg_read(DDRC_ADDR_PPC + DX0BDLR4 + 0x100 * i);
        u32 bdlr5 = dr1x90_reg_read(DDRC_ADDR_PPC + DX0BDLR5 + 0x100 * i);
        AL_DDR_LOG("R%d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\r\n", i, 
            (bdlr3 >> DQ0RBD_offset) & 0x3F, (bdlr3 >> DQ1RBD_offset) & 0x3F, 
            (bdlr3 >> DQ2RBD_offset) & 0x3F, (bdlr3 >> DQ3RBD_offset) & 0x3F, 
            (bdlr4 >> DQ4RBD_offset) & 0x3F, (bdlr4 >> DQ5RBD_offset) & 0x3F, 
            (bdlr4 >> DQ6RBD_offset) & 0x3F, (bdlr4 >> DQ7RBD_offset) & 0x3F, 
            (bdlr5 >>  DMRBD_offset) & 0x3F, (bdlr5 >>  DSRBD_offset) & 0x3F, (bdlr5 >> DSNRBD_offset) & 0x3F
        );
    }
    AL_DDR_LOG("==================================================\r\n");
}
