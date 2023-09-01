#ifndef DR1X90_NEW_FUNC_H
#define DR1X90_NEW_FUNC_H

#endif

#include "alc_ddr_env.h"
#include "dr1x90_ddrc.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_gpll_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_dpll_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_ppc_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_bank_iomc1_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_bank_glue_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_umctl2_regs.h"
#include "./dr1x90_ddrc_regs/dr1x90_ddrc_bankref_regs.h"

void pinmux_config_dbgm_ddr();
void internal_loopback_setup();
void external_loopback();
void AC_swap_en();
void dr1x90_zq_overwrite_cfg_new();
void gate_train();
void do_training(u8 wl_en, u8 wl2_en, u8 gt_en);
void eye_training();
void adjust_rddbi_level_result(u8 pub_index);
int dr1x90_ddrppc_mtest_my(u8 bank, u16 row, u16 col, u8 byteNum);
