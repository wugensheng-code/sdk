#ifndef DR1X90_DDRC_INIT_H
#define DR1X90_DDRC_INIT_H

#include "alc_ddr_env.h"
#include "dr1x90_ddrc.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_gpll_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_dpll_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_ppc_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_bank_iomc1_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_bank_glue_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_umctl2_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_bankref_regs.h"

// Configuration
void dr1x90_ddrppc_base_cfg();
void dr1x90_ddrppc_set_ddrmr();
void dr1x90_ddrppc_set_timing();
void dr1x90_mtest_set_addr(u8 bank, u16 row, u16 col);

void dr1x90_ddrppc_fast_init();
void dr1x90_ddrmc_init(u8 sd_ecc_en);
void dr1x90_ddrmc_post_cfg();

void dr1x90_iomc_internal_loopback_cfg();
void dr1x90_zq_overwrite_cfg();
void dr1x90_pub_training_cfg();
void dr1x90_do_training(u8 wl_en, u8 wl2_en, u8 gt_en);
// Execution
void dr1x90_ddrppc_mdl_cal ();
void dr1x90_ddrppc_dram_init();

// MTEST
int dr1x90_ddrppc_mtest(u8 bank, u16 row, u16 col, u8 byteNum);

#endif // end of DR1X90_DDRC_INIT_H

