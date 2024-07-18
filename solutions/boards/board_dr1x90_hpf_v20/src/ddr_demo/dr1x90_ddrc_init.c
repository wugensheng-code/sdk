#include <string.h>
#include "dr1x90_ddrc_init.h"
#include "dr1x90_ddrc_func.h"
#include "dr1x90_ddrppc_dcu.h"

static uint64_t rand_val;

static void set_rand_seed(uint64_t v)
{
    rand_val = v;
}

static uint64_t get_rand()
{
    rand_val = rand_val * 25214903917ULL + 11ULL;
    return rand_val;
}

int dr1x90_ddrc_is_init()
{
    u32 dfi_done = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFISTAT, dfi_init_complete_offset, dfi_init_complete_mask);
    u32 opr_mode = dr1x90_field_read(DDRC_ADDR_UMCTL2 + STAT, operating_mode_offset, operating_mode_mask);
    // AL_DDR_LOG("DFI_Done == 0x%x, OP_Mode == 0x%x\r\n", dfi_done, opr_mode);
    return dfi_done == 0x1 && opr_mode != 0x0; // Normal or Power Down or Power Saving Mode
}

int dr1x90_ddrc_init(
    const uint64_t para_version , const ddr_basic_t* basic_cfg,
    const ddr_train_t* train_cfg, const ddr_timing_t* timpara, 
    const ddr_addrmap_t* addrmap, const ddr_arbiter_t* arbiter_cfg
)
{
    AL_DDR_LOG("[DDR FW] Version %d.%d\r\n", FW_VERSION_MAJOR, FW_VERSION_MINOR);
    AL_DDR_LOG("[DDR FW] Compile Time: %s %s\r\n", __DATE__, __TIME__);
    if (para_version != FW_VERSION) {
        uint64_t major = para_version >> 32;
        uint64_t minor = para_version & 0xFFFFFFFFUL;
        AL_DDR_LOG("[DDR FW] Parameter Version %d.%d\r\n", major, minor);
        if (major != FW_VERSION_MAJOR) {
            AL_DDR_LOG("[DDR FW] Major Version Mismatch, Exit ...\r\n");
            return -1;
        }
        else {
            AL_DDR_LOG("[DDR FW] Minor Version Mismatch\r\n");
        }
    }

    const u32 ddr3_vref_ctrl = 0x10;    // 50%
    const u32 ddr4_vref_ctrl = 0x50;    // 72%
    const u32 ddr3_zqcode[4] = {0x17173232, 0x17173232, 0x17173232, 0x17173232};
    const u32 ddr4_zqcode[4] = {0x2E003232, 0x2E003232, 0x2E003232, 0x2E003232};

    const u32 vref_ctrl = (basic_cfg->type == DDR4_TYPE) ? ddr4_vref_ctrl : ddr3_vref_ctrl;
    const u32* zqcode   = (basic_cfg->type == DDR4_TYPE) ? ddr4_zqcode : ddr3_zqcode;

    u32 regData = 0;
    u32 mtest_err = 0;
    u32 MR[7] = {0};
    u32 lane_mask = (0x1 << basic_cfg->width) - 1;
    if (basic_cfg->ecc == DDR_ECC_SIDEBAND)
        lane_mask |= 0b1000;
    make_ddr_mr(basic_cfg->fck, basic_cfg->type, timpara, MR);

    regData = dr1x90_reg_read(0x27a0);

    dr1x90_reg_write(0x11b0 ,0x00000050); // DFIMISC
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 01 : GPLL Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddr_gpll_cfg(basic_cfg->fck, basic_cfg->fsys);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 02 : DPLL Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddr_dpll_cfg();
    // release DDRC_SRST_N
    dr1x90_reg_write(DDRC_ADDR_GPLL + SRST_CTRL, 1);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 03 : IOB Configuraiton
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddr_iob_cfg(basic_cfg->type);
    dr1x90_ddr_iob_vref_cfg(basic_cfg->vref, vref_ctrl);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 04 : IOL Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddr_iol_cfg(basic_cfg->type);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 10 : MC Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_reg_write(DDRC_ADDR_GPLL + FUNCTRL, (basic_cfg->ecc == DDR_ECC_SIDEBAND) ? 0x2 : 0x0);
    
    dr1x90_ddrmc_cfg(basic_cfg, timpara, addrmap, arbiter_cfg);
    dr1x90_reg_write(DDRC_ADDR_DPLL + RW_TEST, 0x1); // Release MC reset
    // DFIMISC.dfi_init_complete_en = 1'b0
    dr1x90_reg_write(0x11b0 ,0x00000050); // DFIMISC

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 05 : Bus Matrix Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddr_busmatrix_cfg(basic_cfg->type);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 06 : PPC Base Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddrppc_base_cfg(basic_cfg->type, lane_mask);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 07 : MDL Calibration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddrppc_mdl_cal();
    dr1x90_ddrppc_show_mdl(basic_cfg->fck, lane_mask);
    if (train_cfg->pzq) {
        dr1x90_zq_overwrite_v2(basic_cfg->type, zqcode, vref_ctrl);
        dr1x90_ddrppc_zq_cal(basic_cfg->type);
    }
    else {
        // dr1x90_zq_overwrite_cfg(basic_cfg->type);
        dr1x90_zq_overwrite_v2(basic_cfg->type, zqcode, vref_ctrl);
    }
    
    if (train_cfg->dcc)
        dr1x90_ddrc_train_dcc();

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 08 : Fast Init
    ///////////////////////////////////////////////////////////////////////////////////////////////
    if (train_cfg->fast)
        dr1x90_ddrppc_fast_init(basic_cfg->fck, lane_mask, train_cfg);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 08 : DRAM INIT
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddrppc_set_timing();
    dr1x90_ddrppc_set_ddrmr(basic_cfg->fck, basic_cfg->type, timpara);
    dr1x90_ddrppc_dram_init();

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 09 : Training
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_pub_training_cfg(basic_cfg->type);
    // dr1x90_do_training(1, 1, 1);
    // dr1x90_ddrppc_show_delay();

    // dr1x90_iomc_internal_loopback_cfg();
    // for (int i = 0; ; ++i) {
    //     // dcu_mtest_log(0xF, 0, 0, 0, 1);
    //     mtest_err = dr1x90_ddrppc_mtest(0, 0, 0, 9);
    //     delay_ms(1000);
    // }

    pub_lsfr_set(0x1234ABCD);
    dr1x90_reg_write(DDRC_ADDR_PPC + UDDR0, 0x55aa55aa);
    dr1x90_reg_write(DDRC_ADDR_PPC + UDDR1, 0x5aa55aa5);

    if (train_cfg->wl) {
        dr1x90_ddrc_train_wl();
        // dr1x90_ddrppc_show_delay();
    }
    if (train_cfg->gate & 0x1) {
        dr1x90_ddrc_train_gate();
    }
    if (train_cfg->gate & 0x2) {
        dcu_gate_train(lane_mask, MR, basic_cfg->type);
        // dr1x90_ddrppc_show_delay();
    }
    if (train_cfg->reye & 0x4) {
        dcu_reye_mpr_train(lane_mask, MR[3]);
        dr1x90_ddrppc_show_delay();
    }
    if (train_cfg->wladj) {
        dr1x90_ddrc_train_wladj();
        // dcu_wladj();
    }
    if ((train_cfg->weye & 0x1) && (train_cfg->reye & 0x1)) {
        dr1x90_ddrc_train_eye();
    }
    if (train_cfg->weye & 0x2) {
        dcu_weye_train(lane_mask);
    }
    if (train_cfg->reye & 0x2) {
        dcu_reye_train(lane_mask);
    }
    dr1x90_ddrppc_show_delay();
    // dr1x90_ddrppc_show_bdl();

    // mtest_err = dr1x90_ddrppc_mtest(0, 0, 0, 9);
    // for (int i = 0; i < 10; ++i) {
    //     dcu_mtest_log(0xF, 0, 0, 0, 1);
    //     // dr1x90_ddrppc_mtest(0, 0, 0, 9);
    //     delay_ms(200);
    // }

    // dcu_vref_reye_scan(0xF, MR[3]);
    // dcu_vref_rweye_scan(0xF);
    // while (1);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 11 : HandOff to DFI
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR1, PUBMODE_offset, PUBMODE_mask, 0);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 100 : MC Post Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_release_ddr_bus();
    dr1x90_ddrmc_post_cfg();

    if (basic_cfg->ecc != DDR_ECC_NONE) {
        // memset((void*)0x0, 0x0, basic_cfg->size);
        // dc_zeros((void*)0x0, basic_cfg->size);
        dr1x90_ddrmc_scrub_write(basic_cfg);
    }

    return mtest_err;
}
