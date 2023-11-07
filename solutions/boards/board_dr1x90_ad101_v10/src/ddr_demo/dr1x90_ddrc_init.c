#include <stdio.h>
#include "dr1x90_ddrc_init.h"

// task #
void dr1x90_ddrppc_dx_enable() {

    u8 i ;

    for (i = 0; i <= 8; i++ ) {
        if (i < env_cfg_dx_num) {
            dr1x90_field_write(DDRC_ADDR_PPC + DX0GCR0 + 0x100 * i, DXEN_offset, DXEN_mask, 1);
        } else {
            dr1x90_field_write(DDRC_ADDR_PPC + DX0GCR0 + 0x100 * i, DXEN_offset, DXEN_mask, 0);
        }
    }
}

// task #
void dr1x90_ddrppc_base_cfg()
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
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DCR, DDRMD_offset, DDRMD_mask, env_cfg_ddr_mode, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DCR, NOSRA_offset, NOSRA_mask, 1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DCR, DDR2T_offset, DDR2T_mask, 0, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + DCR, regData);

    // disable write post-amble extension
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR7, WRPSTEX_offset, WRPSTEX_mask, env_cfg_wrpstex);

    // enable configured DXn
    dr1x90_ddrppc_dx_enable();

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
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR1, IODDRM_offset,  IODDRM_mask,  (env_cfg_ddr_mode == DDR4) ? 1 : 0, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR1, FDEPTH_offset,  FDEPTH_mask,  0, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PGCR1, regData);

    // cal clock chose, ddr x4 clock or ctl_clk 1x, default use ddr_clk
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGCR7);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR7, DXCALCLK_offset, DXCALCLK_mask, 0, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PGCR7, ACCALCLK_offset, ACCALCLK_mask, 0, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PGCR7, regData);

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
//  regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR3, tDINIT0_offset,       tDINIT0_mask,0x60000, regData); // tdinit_cke_low
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR3, tDINIT0_offset,       tDINIT0_mask,  0x600, regData); // tdinit_cke_low
    dr1x90_reg_write(DDRC_ADDR_PPC + PTR3, regData);
    //  PTR4                             0x050
    regData = dr1x90_reg_read (DDRC_ADDR_PPC + PTR4);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR4, tDINIT3_offset,       tDINIT3_mask,  0x0C8, regData); // tdinit_zq
//  regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR4, tDINIT2_offset,       tDINIT2_mask,0x30000, regData); // tdinit_rst
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PTR4, tDINIT2_offset,       tDINIT2_mask,  0x300, regData); // tdinit_rst
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
void dr1x90_ddrppc_set_ddrmr()
{
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR0, MR0);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR1, MR1);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR2, MR2);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR3, MR3);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR4, MR4);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR5, MR5);
    dr1x90_reg_write(DDRC_ADDR_PPC + PPC_MR6, MR6);
}

void dr1x90_ddrppc_mdl_cal()
{
    u32 regData;

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PIR, DCAL_offset, DCAL_mask, 1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 1, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);

    dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, DCDONE_offset, DCDONE_mask, 1, 1000);
    dr1x90_field_wait(DDRC_ADDR_PPC + PIR,   INIT_offset,   INIT_mask,   0, 1000);
}

// task #
void dr1x90_ddrppc_dram_init()
{
    u32 regData;
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PIR, DRAMRST_offset,  DRAMRST_mask,  1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PIR, DRAMINIT_offset, DRAMINIT_mask, 1, regData);
    regData = dr1x90_field_set(DDRC_ADDR_PPC + PIR, INIT_offset,     INIT_mask,     1, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);

    dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, DIDONE_offset, DIDONE_mask, 1, 0x1000);
}


/* task #18 :b*/
void dr1x90_mtest_set_addr(u8 bank, u16 row, u16 col){

    u32 regData;

    regData = 0;
    regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTAR0, BBANK_offset, BBANK_mask, bank, regData); // bank
    regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTAR0, BCOL_offset,  BCOL_mask,  col,  regData); // col
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

    dr1x90_field_write(DDRC_ADDR_PPC + BISTAR3, BROW_offset,  BROW_mask, row);// row
    dr1x90_field_write(DDRC_ADDR_PPC + BISTAR4, BMROW_offset, BMROW_mask, 0x7fff);
}

// task #
int dr1x90_ddrppc_mtest(u8 bank, u16 row, u16 col, u8 byteNum) {
    u8  patternType;
    u8  iterateCnt = 128;   // 240
    u32 regData;
    u8  bistDone;
    u16 bistDxErr;
    u8  rcvCnt;
    int mbist_fail_cnt = 0;
    int loopback_mode_int_en = 0;
    int loopback_mode_ext_en = 0;

    dr1x90_field_write(DDRC_ADDR_PPC + PGCR1, LBMODE_offset, LBMODE_mask, loopback_mode_int_en);
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

    for (patternType = 0; patternType <= 27; patternType++){
        dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 0);
        dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 1);

        if ( patternType == 0x18  ) continue;

        regData = dr1x90_reg_read(DDRC_ADDR_PPC + BISTRR);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BINST_offset,  BINST_mask, 1, regData);

        //bist mode =1
        //regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BMODE_offset,  BMODE_mask, 1, regData);
        //loopback mode = 0
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BMODE_offset,  BMODE_mask, loopback_mode_ext_en+1 , regData);

        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BACEN_offset,  BACEN_mask, 0, regData);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BDXEN_offset,  BDXEN_mask, 1, regData);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BDPAT_offset,  BDPAT_mask, patternType, regData);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BDXSEL_offset, BDXSEL_mask, byteNum, regData);
        dr1x90_reg_write(DDRC_ADDR_PPC + BISTRR, regData);

        dr1x90_field_wait(DDRC_ADDR_PPC + BISTGSR, BDONE_offset, BDONE_mask, 1, 0x1500); // wait for bistDone

        bistDone  = dr1x90_field_read(DDRC_ADDR_PPC + BISTGSR,  BDONE_offset,  BDONE_mask);
        bistDxErr = dr1x90_field_read(DDRC_ADDR_PPC + BISTGSR,  BDXERR_offset, BDXERR_mask);
        rcvCnt    = dr1x90_field_read(DDRC_ADDR_PPC + BISTWCSR, DXWCNT_offset, DXWCNT_mask); // Equal to iterateCnt

       printf("\ndram_test : patternType =  0x%x, bistDone = 0x%x, bistDxErr = 0x%x, rcvCnt = 0x%x \r\n", patternType, bistDone, bistDxErr, rcvCnt);

        dr1x90_field_write(DDRC_ADDR_PPC + BISTRR, BINST_offset, BINST_mask, 3);
        dr1x90_field_write(DDRC_ADDR_PPC + BISTRR, BINST_offset, BINST_mask, 2);
        dr1x90_field_write(DDRC_ADDR_PPC + BISTRR, BINST_offset, BINST_mask, 0);

        if (bistDxErr == 1) {
            mbist_fail_cnt++;
        }

    }

    return mbist_fail_cnt ;
}

// task #
void dr1x90_ddrppc_fast_init()
{
    u32 regData;

    int n;

    regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0MDLR0, IPRD_offset, IPRD_mask);
   //  printf("\n DX0 mdl.   data =  0x%x\r\n", regData);

    u16 mdl_ui = regData/2;

    u16 dl_wld[4] = {120/5, 170/5, 255/5, 270/5};
//    u16 dl_gate[4] = {120/5 + mdl_ui + mdl_ui/2, 170/5 + mdl_ui + mdl_ui/2, 255/5 + mdl_ui + mdl_ui/2, 270/5 + mdl_ui + mdl_ui/2 }; // fail
//    u16 dl_gate[4] = {120/5 + mdl_ui  , 170/5 + mdl_ui,   255/5 + mdl_ui,   270/5 + mdl_ui   }; // pass
    u16 dl_gate[4] = {120/5 + mdl_ui/2, 170/5 + mdl_ui/2, 255/5 + mdl_ui/2, 270/5 + mdl_ui/2 }; // pass
//    u16 dl_gate[4] = {120/5  , 170/5,   255/5,   270/5   }; // pass

    dr1x90_field_write(DDRC_ADDR_PPC + RANKIDR, RANKWID_offset, RANKWID_mask, 0);
    dr1x90_field_write(DDRC_ADDR_PPC + RANKIDR, RANKRID_offset, RANKRID_mask, 0);

    for (n = 0; n <= 3; n++) {
       dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100*n, DGSL_offset,   DGSL_mask,   3 );
       dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100*n, WLSL_offset,   WLSL_mask,   2 );
       dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR0 + 0x100*n, WLD_offset,    WLD_mask,    dl_wld[n]);
       dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100*n, WDQD_offset,   WDQD_mask,   mdl_ui >> 1 );
       dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100*n, DQSGD_offset,  DQSGD_mask,  dl_gate[n]);
       //dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100*n, RDQSD_offset,  RDQSD_mask,  mdl_ui >> 2 );
       //dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100*n, RDQSND_offset, RDQSND_mask, mdl_ui >> 2 );

       dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask, mdl_ui >> 1);
       dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask, mdl_ui >> 1);
       dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR5 + 0x100*n, DQSGSD_offset, DQSGSD_mask, 0 );
    }

    dr1x90_field_write(DDRC_ADDR_PPC + PGCR4, DXDDLLD_offset, DXDDLLD_mask, 0);
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR4, DXDDLLD_offset, DXDDLLD_mask, 0x1f);
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR4, DXDDLLD_offset, DXDDLLD_mask, 0);

    dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 0);
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 1);

}

// task #
void dr1x90_ddrmc_init(u8 sd_ecc_en)
{
    // u32 regData;

    dr1x90_reg_write(0x1320, 0x00000000);  // SWCTL
    dr1x90_reg_write(0x1328 ,0x00000001); // SWCTLSTATIC
    // step 1.DDRMC configuration
    dr1x90_reg_write(0x11b0 ,0x00000050); // DFIMISC
    dr1x90_reg_write(0x1304 ,0x00000001); // DBG1
    dr1x90_reg_write(0x1030 ,0x00000001); // PWRCTL

    if (sd_ecc_en) {
        dr1x90_reg_write(0x1000 ,0x40041401); // MSTR
    } else {
        dr1x90_reg_write(0x1000 ,0x40040401); // MSTR
    }

    dr1x90_reg_write(0x1010 ,0x40001010); // MRCTRL0
    dr1x90_reg_write(0x1014 ,0x00018954); // MRCTRL1
    dr1x90_reg_write(0x101c ,0xcb918919); // MRCTRL2


    dr1x90_reg_write(0x1030 ,0x0000008a); // PWRCTL
    dr1x90_reg_write(0x1034 ,0x00080001); // PWRTMG
    dr1x90_reg_write(0x1038 ,0x00410002); // HWLPCTL
    dr1x90_reg_write(0x1050 ,0x00c17020); // RFSHCTL0
    dr1x90_reg_write(0x1060 ,0x00000000); // RFSHCTL3
    dr1x90_reg_write(0x1064 ,0x00610068); // RFSHTMG

//  dr1x90_reg_write(0x1070 ,0x40085894); // ECCCFG0
//  dr1x90_reg_write(0x1070 ,0x40085814); // ECCCFG0
    if(sd_ecc_en){
        dr1x90_reg_write(0x1070 ,0x40085804); // ECCCFG0
        dr1x90_reg_write(0x1074 ,0x00000780); // ECCCFG1
    } else {
        dr1x90_reg_write(0x1070 ,0x40085810); // ECCCFG0
        dr1x90_reg_write(0x1074 ,0x00000680); // ECCCFG1
    }

    dr1x90_reg_write(0x107c ,0x00000300); // ECCCTL
    dr1x90_reg_write(0x10b8 ,0x00000bd3); // ECCPOISONADDR0
    dr1x90_reg_write(0x10bc ,0x1600ac73); // ECCPOISONADDR1
    dr1x90_reg_write(0x10c0 ,0x00000000); // CRCPARCTL0

    dr1x90_reg_write(0x10d0 ,0x40020003); // INIT0
//  dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT0, skip_dram_init_offset, skip_dram_init_mask, 1);
    dr1x90_reg_write(0x10d4 ,0x00030009); // INIT1
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT0, pre_cke_x1024_offset, pre_cke_x1024_mask, 500);
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT0, post_cke_x1024_offset, post_cke_x1024_mask, 15);
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT0, skip_dram_init_offset, skip_dram_init_mask, 0x0);
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT1, dram_rstn_x1024_offset, dram_rstn_x1024_mask, 200);

    // set MR , CL = 9 , WL = 7
   // dr1x90_reg_write(0x10dc ,0x02500001); // INIT3
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT3 ,mr_offset , mr_mask , MR0); // DRAMTMG2
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT3 ,emr_offset , emr_mask , MR1); // DRAMTMG2
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT4 ,emr2_offset , emr2_mask , MR2); // DRAMTMG2
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT4 ,emr3_offset , emr3_mask , MR3); // DRAMTMG2
    //dr1x90_reg_write(0x10e0 ,0x00000000); // INIT4
    dr1x90_reg_write(0x10e4 ,0x00110000); // INIT5
    dr1x90_reg_write(0x10e8 ,0x02001400); // INIT6
    dr1x90_reg_write(0x10ec ,0x00000453); // INIT7
    dr1x90_reg_write(0x10f0 ,0x00000010); // DIMMCTL
    dr1x90_reg_write(0x1100 ,0x0d0a1a0e); // DRAMTMG0
    dr1x90_reg_write(0x1104 ,0x00030314); // DRAMTMG1

    // WL & RL, CL = 9 , WL = 7
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + DRAMTMG2 ,write_latency_offset , write_latency_mask ,0x7); // DRAMTMG2
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + DRAMTMG2 ,read_latency_offset , read_latency_mask ,0x9); // DRAMTMG2
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + DRAMTMG2 ,rd2wr_offset , rd2wr_mask ,0x10); // DRAMTMG2
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + DRAMTMG2 ,wr2rd_offset , wr2rd_mask ,0x10); // DRAMTMG2

    dr1x90_reg_write(0x110c ,0x0000400c); // DRAMTMG3
    dr1x90_reg_write(0x1110 ,0x06030307); // DRAMTMG4
    dr1x90_reg_write(0x1114 ,0x05e20302); // DRAMTMG5
    dr1x90_reg_write(0x1120 ,0x05050b0b); // DRAMTMG8
    dr1x90_reg_write(0x1124 ,0x00020208); // DRAMTMG9
    dr1x90_reg_write(0x1128 ,0x000f0e0d); // DRAMTMG10
    dr1x90_reg_write(0x112c ,0x0e060119); // DRAMTMG11
    dr1x90_reg_write(0x1130 ,0x0c000008); // DRAMTMG12
    dr1x90_reg_write(0x113c ,0x00000000); // DRAMTMG15
    dr1x90_reg_write(0x1180 ,0x01000040); // ZQCTL0
    dr1x90_reg_write(0x1184 ,0x00000070); // ZQCTL1

    // WL = 7 , set t_wrlat = WL -2, and RL = 9, trddata_en = RL -4
    dr1x90_reg_write(0x1190 ,0x02858203); // DFITMG0
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + DFITMG0 ,dfi_tphy_wrlat_offset , dfi_tphy_wrlat_mask ,0x5); // DRAMTMG2
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + DFITMG0 ,dfi_t_rddata_en_offset , dfi_t_rddata_en_mask ,0x5); // DRAMTMG2


    dr1x90_reg_write(0x1194 ,0x03070101); // DFITMG1
    dr1x90_reg_write(0x1198 ,0x07007031); // DFILPCFG0
    dr1x90_reg_write(0x119c ,0x000000f1); // DFILPCFG1
    dr1x90_reg_write(0x11a0 ,0x40400002); // DFIUPD0
    dr1x90_reg_write(0x11a4 ,0x003300e6); // DFIUPD1
    dr1x90_reg_write(0x11a8 ,0x80000000); // DFIUPD2
//  dr1x90_reg_write(0x11b0 ,0x00000051); // DFIMISC
//  dr1x90_reg_write(0x11b0 ,0x00000050); // DFIMISC

    dr1x90_reg_write(0x11b8 ,0x0000001e); // DFITMG3
    dr1x90_reg_write(0x11c0 ,0x00000005); // DBICTL
    dr1x90_reg_write(0x11c4 ,0x2a000000); // DFIPHYMSTR

    dr1x90_reg_write(0x1204, 0x00020202); // ADDRMAP1
    dr1x90_reg_write(0x1208 ,0x03030000); // ADDRMAP2
    dr1x90_reg_write(0x120c, 0x03030303); // ADDRMAP3
    dr1x90_reg_write(0x1210, 0x00001f1f); // ADDRMAP4
    dr1x90_reg_write(0x1214, 0x07070707); // ADDRMAP5
    dr1x90_reg_write(0x1218, 0x0f070707); // ADDRMAP6
    dr1x90_reg_write(0x121c, 0x00000f0f); // ADDRMAP7
    dr1x90_reg_write(0x1220, 0x00003f3f); // ADDRMAP8

    //dr1x90_reg_write(0x1204 ,0x003f0a0a); // ADDRMAP1
    //dr1x90_reg_write(0x120c ,0x00000000); // ADDRMAP3
    //dr1x90_reg_write(0x1210 ,0x00001f1f); // ADDRMAP4
    //dr1x90_reg_write(0x1214 ,0x08080808); // ADDRMAP5
    //dr1x90_reg_write(0x1218 ,0x0f080808); // ADDRMAP6
    //dr1x90_reg_write(0x121c ,0x00000f0f); // ADDRMAP7
    //dr1x90_reg_write(0x1220 ,0x00000808); // ADDRMAP8
//  dr1x90_reg_write(0x1224 ,0x01010503); // ADDRMAP9
//  dr1x90_reg_write(0x1228 ,0x0802020a); // ADDRMAP10
//  dr1x90_reg_write(0x122c ,0x0000000b); // ADDRMAP11
    dr1x90_reg_write(0x1240 ,0x061a0f48); // ODTCFG
    dr1x90_reg_write(0x1244 ,0x00000011); // ODTMAP
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + ODTMAP ,rank0_rd_odt_offset , rank0_rd_odt_mask ,0x0); // DRAMTMG2

    dr1x90_reg_write(0x1250 ,0x5c5b1780); // SCHED
    dr1x90_reg_write(0x1254 ,0x00000002); // SCHED1
    dr1x90_reg_write(0x125c ,0x2a00c05a); // PERFHPR1
    dr1x90_reg_write(0x1264 ,0x6100e7dc); // PERFLPR1
    dr1x90_reg_write(0x126c ,0x0000bbf3); // PERFWR1
    dr1x90_reg_write(0x1300 ,0x00000010); // DBG0
    dr1x90_reg_write(0x1304 ,0x00000000); // DBG1
    dr1x90_reg_write(0x130c ,0x00000000); // DBGCMD
    dr1x90_reg_write(0x1320 ,0x00000000); // SWCTL
    dr1x90_reg_write(0x1328 ,0x00000001); // SWCTLSTATIC
    dr1x90_reg_write(0x136c ,0x00110001); // POISONCFG
    dr1x90_reg_write(0x1374 ,0x0000010d); // ADVECCINDEX
    dr1x90_reg_write(0x137c ,0x00000000); // ECCPOISONPAT0
    dr1x90_reg_write(0x1384 ,0x00000000); // ECCPOISONPAT2
    dr1x90_reg_write(0x1400 ,0x00000100); // PCCFG
    dr1x90_reg_write(0x1404 ,0x0000333c); // PCFGR_0
    dr1x90_reg_write(0x1408 ,0x000042d0); // PCFGW_0
    dr1x90_reg_write(0x1490 ,0x00000001); // PCTRL_0
    dr1x90_reg_write(0x1494 ,0x0011000b); // PCFGQOS0_0
//  dr1x90_reg_write(0x1498 ,0x0749004f); // PCFGQOS1_0
//  dr1x90_reg_write(0x149c ,0x01100e0d); // PCFGWQOS0_0
//  dr1x90_reg_write(0x14a0 ,0x04b500dd); // PCFGWQOS1_0
    dr1x90_reg_write(0x14b4 ,0x0000333c); // PCFGR_1
    dr1x90_reg_write(0x14b8 ,0x00000243); // PCFGW_1
    dr1x90_reg_write(0x1540 ,0x00000001); // PCTRL_1
    dr1x90_reg_write(0x1544 ,0x0011000a); // PCFGQOS0_1
//  dr1x90_reg_write(0x1548 ,0x00440090); // PCFGQOS1_1
//  dr1x90_reg_write(0x154c ,0x01110503); // PCFGWQOS0_1
//  dr1x90_reg_write(0x1550 ,0x00cd00fa); // PCFGWQOS1_1
//  dr1x90_reg_write(0x1564 ,0x00006340); // PCFGR_2
    dr1x90_reg_write(0x1564 ,0x0000333c); // PCFGR_2
    dr1x90_reg_write(0x1568 ,0x0000126d); // PCFGW_2
    dr1x90_reg_write(0x15f0 ,0x00000001); // PCTRL_2
    dr1x90_reg_write(0x15f4 ,0x00110006); // PCFGQOS0_2
//  dr1x90_reg_write(0x15f8 ,0x051e04c9); // PCFGQOS1_2
//  dr1x90_reg_write(0x15fc ,0x01100a06); // PCFGWQOS0_2
//  dr1x90_reg_write(0x1600 ,0x073a0015); // PCFGWQOS1_2
//  dr1x90_reg_write(0x1614 ,0x000063f4); // PCFGR_3
    dr1x90_reg_write(0x1614 ,0x0000333c); // PCFGR_3
    dr1x90_reg_write(0x1618 ,0x0000705c); // PCFGW_3
    dr1x90_reg_write(0x16a0 ,0x00000001); // PCTRL_3
    dr1x90_reg_write(0x16a4 ,0x00100004); // PCFGQOS0_3
//  dr1x90_reg_write(0x16a8 ,0x006a0331); // PCFGQOS1_3
//  dr1x90_reg_write(0x16ac ,0x01100d02); // PCFGWQOS0_3
//  dr1x90_reg_write(0x16b0 ,0x05d2012f); // PCFGWQOS1_3

}

// task #
void dr1x90_ddrmc_post_cfg()
{
    u32 regData;

    // step 6
    // cfg SWCTL.sw_done = 1'b0
    dr1x90_reg_write(0x1320, 0x00000000);  // SWCTL

    for (u8 i = 0; i < 100; i++) {
       //  printf(" wait long time \r\n");
        regData = dr1x90_reg_read(0x1320);
    }

    // step 7
    // cfg DFIMISC.dfi_init_complete_en bit0 = 1'b1
    dr1x90_reg_write(0x11b0 , 0x00000051); // DFIMISC

    // wait phy_dfi_init_complete
    // poll DFISTAT.dfi_init_complete=1
    dr1x90_field_wait(DDRC_ADDR_UMCTL2 + DFISTAT, dfi_init_complete_offset, dfi_init_complete_mask, 0x1, 1000);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFISTAT, dfi_init_complete_offset, dfi_init_complete_mask);
   //  printf("\n dfi_init_complete.   data =  0x%x\r\n", regData);

    // step 8
    // cfg SWCTL.sw_done = 1'b1
    dr1x90_reg_write(0x1320, 0x00000001);  // SWCTL

    // step 9
    // wait SWSTAT.sw_done_ack
    dr1x90_reg_read(0x1324);

    // step 10
    // wait STAT.operating_mode = "normal" 2'b01
    dr1x90_field_wait(DDRC_ADDR_UMCTL2 + STAT, operating_mode_offset, operating_mode_mask, 0x1, 1000);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + STAT, operating_mode_offset, operating_mode_mask);
    printf("\n operating_mode.   data =  0x%x\r\n", regData);

    //step 11
    // cfg SWCTL.sw_done = 1'b0
    dr1x90_reg_write(0x1320, 0x00000000);// (

    // step 12
    // RFSHCTL3.dis_auto_refresh bit0 = 1'b1 disables auto_refresh
    dr1x90_reg_write(0x1060, 0x00000001);  // RFSHCTL3

    // PWRCTL.powerdown_en bit1 = 1'b0 en_dfi_dram_clk_disable bit3= 1'b0
    dr1x90_reg_write(0x1030, 0x00000080);  // PWRCTL

    // cfg DFIMISC.dfi_init_complete_en bit0 = 1'b0
    dr1x90_reg_write(0x11b0 , 0x00000053); // DFIMISC

    // step 13
    // cfg SWCTL.sw_done = 1'b1
    dr1x90_reg_write(0x1320, 0x00000001);// (

    // step 17
    // RFSHCTL3.dis_auto_refresh bit0 = 1'b1 disables auto_refresh
    dr1x90_reg_write(0x1060, 0x00000000);  // RFSHCTL3

    // final
    dr1x90_reg_write(0x1030, 0x00000088);  // PWRCTL
    dr1x90_reg_write(0x1030, 0x0000008a);  // PWRCTL
    dr1x90_reg_write(0x1490, 0x00000001);  // PCTRL_0
    dr1x90_reg_write(0x1540, 0x00000001);  // PCTRL_1
    dr1x90_reg_write(0x15f0, 0x00000001);  // PCTRL_2
    dr1x90_reg_write(0x16a0, 0x00000001);  // PCTRL_3
    dr1x90_reg_write(0x1304, 0x00000000);  // DBG1
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
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr49_cfg3 , U_byte3_se_mc1_lb_en_offset,   U_byte3_se_mc1_lb_en_mask,   0x1  );

   // dr1x90_reg_write(0x1320, 0x00000000);// (

}

void dr1x90_zq_overwrite_cfg() {

    u8 n ;
    u8 i ;
    // u8 BankInfo ;
    u8 override_en = 1;

    // u8 pzq_cal_done ;

    //u32  bank_zq_ac_code[3] = {0x19193235,0x19193235,0x19193235} ;
    //u32  bank_zq_dx_code[3] = {0x19193235,0x19193235,0x19193235} ;
    u32  bank_zq_ac_code[3] = {0x11112323,0x11112323,0x11112323} ;
    u32  bank_zq_dx_code[3] = {0x11112323,0x11112323,0x11112323} ;

    // u32  zqcal_done;
    // u32  zqcal_err ;

    u32 regData, regDataNew ;

    // u32 env_cfg_dci_cascade = 0;
    // ddr4=1 ddr3=0
    // u32 env_cfg_ddr_mode = 3;
    // u32 env_cfg_pzq_slave = 1;
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
                    regData =  ((env_cfg_ddr_mode == DDR4) ? (bank_zq_dx_code[n] & 0xff00ffff) : bank_zq_dx_code[n] ) ;
                }

                dr1x90_reg_write(DDRC_ADDR_BK0_VREF + ZQ0DR + 0x2000 * n + 0x04 * i, regData);

                regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF+ZQ0CR + 0x2000 * n + 0x04 * i);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+ZQ0CR + 0x2000 * n + 0x04 * i, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 0,  regData);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+ZQ0CR + 0x2000 * n + 0x04 * i, PGWAIT_offset, PGWAIT_mask, 1,  regDataNew);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+ZQ0CR + 0x2000 * n + 0x04 * i, AVGEN_offset, AVGEN_mask, (env_cfg_ddr_mode == DDR4) ? 1 : 0, regDataNew); // ddr4=1 ddr3=0
                dr1x90_reg_update(DDRC_ADDR_BK0_VREF+ZQ0CR + 0x800 * n + 0x04 * i, regData, regDataNew);

                regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF+ZQ0PR + 0x2000 * n + 0x04 * i);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+ZQ0PR + 0x2000 * n + 0x04 * i, DRV_ZDEN_offset, DRV_ZDEN_mask, 1,  regData);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+ZQ0PR + 0x2000 * n + 0x04 * i, ODT_ZDEN_offset, ODT_ZDEN_mask, 1,  regDataNew);
                dr1x90_reg_update(DDRC_ADDR_BK0_VREF+ZQ0PR + 0x2000 * n + 0x04 * i, regData, regDataNew);
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
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF+PZQVREF + 0x2000 * n, ddr_mode_offset, ddr_mode_mask, (env_cfg_ddr_mode == DDR4) ? 1 : 0, regDataNew);
        dr1x90_reg_update(DDRC_ADDR_BK0_VREF+PZQVREF + 0x2000 * n, regData, regDataNew);

        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF1 + 0x2000 * n, Vref1_ctrl_rank0_offset, Vref1_ctrl_rank0_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF2 + 0x2000 * n, Vref2_ctrl_rank0_offset, Vref2_ctrl_rank0_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF3 + 0x2000 * n, Vref3_ctrl_rank0_offset, Vref3_ctrl_rank0_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF+VREF4 + 0x2000 * n, Vref4_ctrl_rank0_offset, Vref4_ctrl_rank0_mask, env_cfg_host_vref);
    }

    // assert PZQ_CALDONE_FLAG after PZQ Cal and Vref Setting
    //Alc_GpioMaskWrite(PZQ_CALDONE_FLAG_mask, PZQ_CALDONE_FLAG_mask);

}

void dr1x90_ac_swap_cfg() {
   dr1x90_field_write( DDRC_ADDR_GPLL+DTCR0, BK0_IS_AC_offset, BK0_IS_AC_mask, 0x0 );
}


void dr1x90_pub_training_cfg() {
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR0, DTWBDDM_offset, DTWBDDM_mask, 0x1 );	 // DM or DBI
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR0, DTRDBITR_offset,  DTRDBITR_mask,  0);
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR0, DTBDC_offset,  DTBDC_mask,  1);
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR0, RFSHDT_offset,  RFSHDT_mask,  8);
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR0, DTMPR_offset,  DTMPR_mask,  1);
   dr1x90_field_write( DDRC_ADDR_PPC + DTCR1, RDPRMBL_TRN_offset,  RDPRMBL_TRN_mask,  1);
   dr1x90_field_write( DDRC_ADDR_PPC + DXCCR, QSCNTEN_offset,  QSCNTEN_mask,  1);
   if ( env_cfg_ddr_mode == DDR3 ) {
      dr1x90_field_write( DDRC_ADDR_PPC + OFFCAL_PR, dqs_pupd_en_offset,  dqs_pupd_en_mask,  1);
   }

   dr1x90_field_write( DDRC_ADDR_PPC + OFFCAL_PR, eye_rdbi_rpt_en_offset,  eye_rdbi_rpt_en_mask,  1);
   dr1x90_field_write( DDRC_ADDR_PPC + PHY_EYE_CR3, eye_rdbi_train_type1_offset,  eye_rdbi_train_type1_mask,  1);

}


void dr1x90_do_training (u8 wl_en, u8 wl2_en, u8 gt_en ) {

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
		dr1x90_field_wait (DDRC_ADDR_PPC+ PGSR0, WLDONE_offset,  WLDONE_mask,  1,  200 );
	}

	if ( wl2_en == 0x1 ) {
		dr1x90_field_wait (DDRC_ADDR_PPC+ PGSR0, WLADONE_offset,  WLADONE_mask,  1,  200 );
	}

	if ( gt_en == 0x1 ) {
		dr1x90_field_wait (DDRC_ADDR_PPC+ PGSR0, QSGDONE_offset,  QSGDONE_mask,  1,  20000000 );
	}

    if (gt_en == 1) {
        regData = dr1x90_field_read(DDRC_ADDR_PPC + PGSR0, QSGDONE_offset, QSGDONE_mask);
       //  printf(" \n------- GateTraining done = 0x%x\r\n", regData);
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        gterr = (regData >> 22) & 0x1;
        if (gterr == 1) {
           //  printf(" \n------- GateTraining Failed ; PGSR0 is 0x%x\r\n", regData);
        }
        if (gterr == 0) {
           //  printf(" \n------- GateTraining PASSED ; PGSR0 is 0x%x\r\n", regData);
        }
    }

    int n;
    for (n = 0; n <= 3; n++) {
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, DGSL_offset, DGSL_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x GTR0 :DGSL is 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, WLSL_offset, WLSL_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x GTR0 : WLSL is 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR0 + 0x100 * n, WLD_offset, WLD_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x LCDLR0 : WLD is 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * n, WDQD_offset, WDQD_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x LCDLR1 : WDQD is 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x LCDLR2 : DQSGD is 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x LCDLR3 :RDQSD is 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x LCDLR4 : RDQSND is 0x%x\r\n", n, regData);

    }

}

