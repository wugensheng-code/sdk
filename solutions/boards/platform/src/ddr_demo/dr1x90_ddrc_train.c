#include "dr1x90_ddrc_func.h"
#include "dr1x90_ddrc_init.h"

#define TRY_CNT 32
#define DESKEW_EN 0

void dr1x90_ddrc_train_wl()
{
    u32 regData = 0;
    u32 wlprd = dr1x90_field_read(DDRC_ADDR_PPC + DX0GSR0, WLPRD_offset, WLPRD_mask);
    AL_DDR_LOG("[DDR WL] DX0GSR0.WLPRD = %d\r\n", wlprd);

    // 1'b0 = 8 DDL tap delays; 1'b1 = 0.52 * WLPRD DDL tap delays
    dr1x90_field_write(DDRC_ADDR_PPC + DTCR1, WLVLDPRD_offset, WLVLDPRD_mask, 0x0);
    // 1'b0 = 32 step sizes; 1'b1 = 1 step size
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR1, WLSTEP_offset, WLSTEP_mask, 0x0);
    // Start WL
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
    regData = regData | WL_mask | INIT_mask;
    dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
    // Wait WL Done
    dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, WLDONE_offset, WLDONE_mask, 0x1, -1);
    dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
    AL_DDR_LOG("[DDR WL] %s\r\n", (regData & PGSR0_WLERR_mask) ? "Error" : "Done");
}

void dr1x90_ddrc_train_gate()
{
    u32 regData = 0;
    u32 gdqsprd = dr1x90_field_read(DDRC_ADDR_PPC + DX0GSR0, GDQSPRD_offset, GDQSPRD_mask);
    AL_DDR_LOG("[DDR GATE] DX0GSR0.GDQSPRD = %d\r\n", gdqsprd);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + DTCR0);
    // Data Training Compare Data
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTCMPD_offset, DTCMPD_mask, 0x1, regData);
    // Data Training Using MPR
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTMPR_offset, DTMPR_mask, 0x1, regData);
    // Data Training Repeat Number
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTRPTN_offset, DTRPTN_mask, 15, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + DTCR0, regData);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + DTCR1);
    // Read Leveling Gate Sampling Difference, 0x2: GDQSPRD/8
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVLGDIFF_offset, RDLVLGDIFF_mask, 0x2, regData);
    // Read Leveling Gate Shift, 0x1: GDQSPRD/4 0x3: GDQSPRD/16
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVLGS_offset, RDLVLGS_mask, 0x1, regData);
    // Read Leveling Enable
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVLEN_offset, RDLVLEN_mask, 0x1, regData);
    // Basic Gate Training Enable
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, BSTEN_offset, BSTEN_mask, 0x1, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + DTCR1, regData);

    // Start QOS Gate Training
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
    regData = regData | QSGATE_mask | INIT_mask;
    dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
    // Wait Done
    dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, QSGDONE_offset, QSGDONE_mask, 0x1, -1);
    dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
    AL_DDR_LOG("[DDR GATE] %s\r\n", (regData & PGSR0_QSGERR_mask) ? "Error" : "Done");
}

void dr1x90_ddrc_train_wladj()
{
    u32 regData = 0;
    // Start WLADJ
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
    regData = regData | WLADJ_mask | INIT_mask;
    dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
    // Wait Done
    dr1x90_field_wait(DDRC_ADDR_PPC+ PGSR0, WLADONE_offset, WLADONE_mask, 0x1, -1);
    dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
    AL_DDR_LOG("[DDR WLADJ] %s\r\n", (regData & PGSR0_WLAERR_mask) ? "Error" : "Done");
}

void dr1x90_ddrc_train_eye()
{
    u32 err = 1;
    u32 regData = 0;
    for (int i = 0; i < TRY_CNT && err != 0; ++i) {
        err = 0;
        #if DESKEW_EN
        // Read Data Bit Deskew
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
        regData = regData | RDDSKW_mask | INIT_mask;
        dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
        dr1x90_field_wait(DDRC_ADDR_PPC+ PGSR0, RDDONE_offset, RDDONE_mask, 0x1, -1);
        dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        // AL_DDR_LOG("[DDR RD DSKEW] #%d %s\r\n", i, (regData & PGSR0_RDERR_mask) ? "Error" : "Done");
        err |= regData & PGSR0_RDERR_mask;

        // Write Data Bit Deskew
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
        regData = regData | WRDSKW_mask | INIT_mask;
        dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
        dr1x90_field_wait(DDRC_ADDR_PPC+ PGSR0, WDDONE_offset, WDDONE_mask, 0x1, -1);
        dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        // AL_DDR_LOG("[DDR WR DSKEW] #%d %s\r\n", i, (regData & PGSR0_WDERR_mask) ? "Error" : "Done");
        err |= regData & PGSR0_WDERR_mask;
        #endif
        // Read Data Eye Training
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
        regData = regData | RDEYE_mask | INIT_mask;
        dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
        dr1x90_field_wait(DDRC_ADDR_PPC+ PGSR0, REDONE_offset, REDONE_mask, 0x1, -1);
        dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        // AL_DDR_LOG("[DDR RD CENTER] #%d %s\r\n", i, (regData & PGSR0_REERR_mask) ? "Error" : "Done");
        err |= regData & PGSR0_REERR_mask;

        // Write Data Eye Training
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
        regData = regData | WREYE_mask | INIT_mask;
        dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
        dr1x90_field_wait(DDRC_ADDR_PPC+ PGSR0, WEDONE_offset, WEDONE_mask, 0x1, -1);
        dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        // AL_DDR_LOG("[DDR WR CENTER] #%d %s\r\n", i, (regData & PGSR0_WEERR_mask) ? "Error" : "Done");
        err |= regData & PGSR0_WEERR_mask;

        #if DESKEW_EN
        AL_DDR_LOG("[DDR TRAIN #%d] RDDSKE:%c WRDSKW:%c RDEYE:%c WREYE:%c\r\n", i,
            (err & PGSR0_RDERR_mask) ? 'X' : 'O',
            (err & PGSR0_WDERR_mask) ? 'X' : 'O',
            (err & PGSR0_REERR_mask) ? 'X' : 'O',
            (err & PGSR0_WEERR_mask) ? 'X' : 'O'
        );
        #else
        AL_DDR_LOG("[DDR TRAIN #%d] RDEYE:%c WREYE:%c\r\n", i,
            (err & PGSR0_REERR_mask) ? 'X' : 'O',
            (err & PGSR0_WEERR_mask) ? 'X' : 'O'
        );
        #endif
    }
}

void dr1x90_ddrppc_zq_cal(ddr_type_t type)
{
    u32 zq_ac_code = 0;
    u32 zq_dx_code = 0;

    u8 env_cfg_host_odt = 5; // 40
    u8 env_cfg_host_drv = 11; // 480/40 -1

    u32 regData;
    u32 regDataNew;

    u8 env_cfg_host_vrefr = 0xf;
    u8 env_cfg_host_vref = 0x1f;

    // ZCAL = 1
    dr1x90_field_write(DDRC_ADDR_PPC + PIR, ZCALBYP_offset, ZCALBYP_mask, 0);
    for (int n = 0; n < 2; n++) {
        // AL_DDR_LOG("[DDR ZQCAL] training setting\r\n");
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0VREF + 0x2000 * n, ZQCALEN_offset, ZQCALEN_mask, 1);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, ddr_mode_offset, ddr_mode_mask, (type == DDR4_TYPE) ? 1 : 0);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n, PU_ODT_ONLY_offset, PU_ODT_ONLY_mask, 0);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n, ZPROG_PU_ODT_ONLY_offset, ZPROG_PU_ODT_ONLY_mask, env_cfg_host_odt);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n, ZPROG_ASYM_DRV_PD_offset, ZPROG_ASYM_DRV_PD_mask, env_cfg_host_drv);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n, ZPROG_ASYM_DRV_PU_offset, ZPROG_ASYM_DRV_PU_mask, env_cfg_host_drv);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 0);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ1CR + 0x2000 * n, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 0);
    }
    dr1x90_field_write(DDRC_ADDR_PPC + PIR, ZCAL_offset, ZCAL_mask, 1);
    dr1x90_field_write(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 1);
    regData = dr1x90_field_read(DDRC_ADDR_PPC + PGSR0, ZCDONE_offset, ZCDONE_mask);

    // AL_DDR_LOG("[DDR ZQCAL] training start\r\n");
    dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);
    dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, ZCDONE_offset, ZCDONE_mask, 0x1, -1);
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
    AL_DDR_LOG("[DDR ZQCAL] %s\r\n", (regData & ZCERR_mask) ? "Error" : "Done");
    
    dr1x90_field_write(DDRC_ADDR_PPC + PIR, ZCAL_offset, ZCAL_mask, 0);
    dr1x90_field_write(DDRC_ADDR_PPC + PIR, ZCALBYP_offset, ZCALBYP_mask, 1);

    for (int n = 0; n < 2; n++) {
        zq_ac_code = dr1x90_reg_read(DDRC_ADDR_BK0_VREF + ZQ0DR + 0x2000 * n);
        zq_dx_code = dr1x90_reg_read(DDRC_ADDR_BK0_VREF + ZQ1DR + 0x2000 * n);
        AL_DDR_LOG("[DDR ZQCAL] bank%d zq_ac_code = 0x%x, zq_dx_code = 0x%x\r\n", n, zq_ac_code, zq_dx_code);
    }

    int n = 0;
    // bank 1 overwtite
    dr1x90_reg_write(DDRC_ADDR_BK0_VREF + ZQ0DR + 0x2000 * n, zq_ac_code);
    dr1x90_reg_write(DDRC_ADDR_BK0_VREF + ZQ1DR + 0x2000 * n, zq_dx_code);
    
    regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n);
    regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n, DRV_ZDEN_offset, DRV_ZDEN_mask, 1, regData);
    regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n, ODT_ZDEN_offset, ODT_ZDEN_mask, 1, regDataNew);
    dr1x90_reg_update(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n, regData, regDataNew);

    regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF + ZQ1PR + 0x2000 * n );     
    regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ1PR + 0x2000 * n, DRV_ZDEN_offset, DRV_ZDEN_mask, 1, regData);
    regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ1PR + 0x2000 * n, ODT_ZDEN_offset, ODT_ZDEN_mask, 1, regDataNew);
    dr1x90_reg_update(DDRC_ADDR_BK0_VREF + ZQ1PR + 0x2000 * n, regData, regDataNew);

    dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ1CR + 0x2000 * n, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 1);
    // AL_DDR_LOG("[DDR ZQCAL] zq_ac_code = 0x%x, zq_dx_code = 0x%x\n", zq_ac_code, zq_dx_code);

    // setup a proper vref for basic functionality
    for (int n = 0; n < 2; n++) {
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
    }
}

void dr1x90_ddrc_train_dcc()
{
    u32 dcc_clk_div_num = 0;
    u32 env_clkrst_agt_ddr_clk_half_cycle = 625;
    u32 env_clkrst_agt_cfg_dfi_ratio = 4;
    u32 regData = 0;

    // ZQCAL disable
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + byte0_glue_cfg1, U_byte0_glue_mc1_dcc_byp_offset, U_byte0_glue_mc1_dcc_byp_mask, 0x0);	 // DM or DBI
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + byte1_glue_cfg1, U_byte1_glue_mc1_dcc_byp_offset, U_byte1_glue_mc1_dcc_byp_mask, 0x0);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + byte2_glue_cfg1, U_byte2_glue_mc1_dcc_byp_offset, U_byte2_glue_mc1_dcc_byp_mask, 0x0);	 // DM or DBI
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + byte3_glue_cfg1, U_byte3_glue_mc1_dcc_byp_offset, U_byte3_glue_mc1_dcc_byp_mask, 0x0);

    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte0_glue_cfg1, U_byte0_glue_mc1_dcc_byp_offset, U_byte0_glue_mc1_dcc_byp_mask, 0x0);	 // DM or DBI
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte1_glue_cfg1, U_byte1_glue_mc1_dcc_byp_offset, U_byte1_glue_mc1_dcc_byp_mask, 0x0);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte2_glue_cfg1, U_byte2_glue_mc1_dcc_byp_offset, U_byte2_glue_mc1_dcc_byp_mask, 0x0);	 // DM or DBI
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte3_glue_cfg1, U_byte3_glue_mc1_dcc_byp_offset, U_byte3_glue_mc1_dcc_byp_mask, 0x0);

    for (int i = 0; i < 8; i++) {
        regData = dr1x90_field_read(DDRC_ADDR_BK0_T0 + 0x1000 * i + DQSGRP_DCC_SR, dutyx_offset, dutyx_mask);
        AL_DDR_LOG("[DDR DCC] #%d initial dutyx = 0x%x\r\n", i, regData);
    }

    // DCC 需要划分ctl_clk时钟
    dcc_clk_div_num = 1000000.0 / ((env_clkrst_agt_ddr_clk_half_cycle * env_clkrst_agt_cfg_dfi_ratio) + 1);
    if (dcc_clk_div_num > 1023) {
        dcc_clk_div_num = 1023;
    }

    for (int i = 0; i < 8; i++) {
        regData = dr1x90_reg_read(DDRC_ADDR_BK0_T0 + 0x1000 * i + DQSGRP_DCC_CR);
        // AL_DDR_LOG("[DDR DCC] #%d DQSGRP_DCC_CR = 0x%x\n", i, regData);
        dr1x90_field_write(DDRC_ADDR_BK0_T0 + 0x1000 * i + DQSGRP_DCC_CR, cfg_dcc_disable_offset, cfg_dcc_disable_mask, 0x1);
        dr1x90_field_write(DDRC_ADDR_BK0_T0 + 0x1000 * i + DQSGRP_DCC_CR, cfg_dcc_clk_div_num_offset, cfg_dcc_clk_div_num_mask, dcc_clk_div_num);
        dr1x90_field_write(DDRC_ADDR_BK0_T0 + 0x1000 * i + DQSGRP_DCC_CR, cfg_dcc_disable_offset, cfg_dcc_disable_mask, 0x0);
    }
    // Clear DCC
    dr1x90_field_write(DDRC_ADDR_PPC + PIR, cfg_dcc_clrsr_offset, cfg_dcc_clrsr_mask, 0x1);	
    dr1x90_field_write(DDRC_ADDR_PPC + PIR, cfg_dcc_clrsr_offset, cfg_dcc_clrsr_mask, 0x0);
    dr1x90_field_write(DDRC_ADDR_PPC + PIR, cfg_dcc_offset, cfg_dcc_mask, 0x1);
    dr1x90_field_write(DDRC_ADDR_PPC + PIR, cfg_dcc_offset, cfg_dcc_mask, 0x0);

    for (int i = 0; i < 8; i++) {
        dr1x90_field_wait(DDRC_ADDR_BK0_T0 + 0x1000 * i + DQSGRP_DCC_SR, dcc_done_offset, dcc_done_mask, 0x1, -1);
        regData = dr1x90_reg_read(DDRC_ADDR_BK0_T0 + 0x1000 * i + DQSGRP_DCC_SR);
        AL_DDR_LOG("[DDR DCC] #%d pol = 0x%c, dutyx = 0x%x, overflow = 0x%c, undeflow = %c\r\n", i,
            (regData & pol_mask) ? '1' : '0',
            (regData & dutyx_mask) >> dutyx_offset,
            (regData & overflow_mask) ? '1' : '0',
            (regData & underflow_mask) ? '1' : '0'
        );
    }
}
