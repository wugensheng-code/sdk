#include "alc_types.h"
#include "dr1x90_ddrc_init.h"
#include "dr1x90_misc.h"

#include "dr1x90_mpu.h"
#include "al_barrier.h"
#include "al_systimer.h"

/*************************************************************/

int demo_ddr_init()
{
    u32 regData = 0;
    u32 mtest_err = 0;

    regData = dr1x90_reg_read(0x27a0);

    ////printf("DDRGPLL_CTRL18\n");
   //  printf(" 0x27a0.initial  data =  0x%x\n", regData);
   printf("DDR Init V2 \r\n");

    dr1x90_reg_write(0x11b0 ,0x00000050); // DFIMISC
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 01 : GPLL Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////

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
    /*
    #if CRYSTAL_OSC_HZ == 33333333
    // OSC 33 MHz -> DDR 1066 MHz
    pll_ddr_div_set(64, 1, 2, 4, 2);    // 1066 MHz
    // pll_ddr_div_set(72, 1, 2, 4, 2);    // 1200 MHz
    #elif CRYSTAL_OSC_HZ == 50000000
    // OSC 50 MHz -> DDR 1066 MHz
    pll_ddr_div_set(64, 1, 3, 6, 3);    // 1066 MHz
    // pll_ddr_div_set(48, 1, 3, 6, 3);    // 800 MHz
    #endif
    pll_ddr_waitLock();
    */
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 02 : DPLL Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
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
    dr1x90_field_wait(DDRC_ADDR_DPLL + BK0_DPLL_SR0, BK0_DPLL_LOCK_offset, BK0_DPLL_LOCK_mask, 1, 1000);

    // release DDRC_SRST_N
    dr1x90_reg_write(DDRC_ADDR_GPLL + SRST_CTRL, 1);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 03 : IOB Configuraiton
    ///////////////////////////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 04 : IOL Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg1,   U_byte0_quad2_mc1_oclk_sclk_offset, U_byte0_quad2_mc1_oclk_sclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg1,   U_byte0_quad2_mc1_oclk_pclk_offset, U_byte0_quad2_mc1_oclk_pclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg1, U_byte1_quad2_mc1_oclk_sclk_offset, U_byte1_quad2_mc1_oclk_sclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg1, U_byte1_quad2_mc1_oclk_pclk_offset, U_byte1_quad2_mc1_oclk_pclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg1, U_byte2_quad2_mc1_oclk_sclk_offset, U_byte2_quad2_mc1_oclk_sclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg1, U_byte2_quad2_mc1_oclk_pclk_offset, U_byte2_quad2_mc1_oclk_pclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg1, U_byte3_quad2_mc1_oclk_sclk_offset, U_byte3_quad2_mc1_oclk_sclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg1, U_byte3_quad2_mc1_oclk_pclk_offset, U_byte3_quad2_mc1_oclk_pclk_mask, 9 | (9 << 4) | (8 << 8) | (8 << 12));

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

    //Alc_GpioMaskWrite(GPIO_CH0, 0x04, 0xffff);
    dr1x90_reg_write(DDRC_ADDR_DPLL + RW_TEST, 0x1); // used as reset
    dr1x90_reg_write(0x11b0 ,0x00000050); // DFIMISC

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 05 : Bus Matrix Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x05, 0xffff);

    if (env_cfg_ddr_mode == DDR3)
    {
        dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR4, AC_BANK_MTX_IO20_SEL_offset, AC_BANK_MTX_IO20_SEL_mask, 21); // ddr3_a14
        dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR4, AC_BANK_MTX_IO21_SEL_offset, AC_BANK_MTX_IO21_SEL_mask, 25); // ddr3_we_n
        dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR4, AC_BANK_MTX_IO22_SEL_offset, AC_BANK_MTX_IO22_SEL_mask, 36); // ddr3_rst_n
        dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR5, AC_BANK_MTX_IO25_SEL_offset, AC_BANK_MTX_IO25_SEL_mask, 33); // ddr3_ras_n
        dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR7, AC_BANK_MTX_IO36_SEL_offset, AC_BANK_MTX_IO36_SEL_mask, 26); // ddr3_cas_n
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 06 : PPC Base Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x06, 0xffff);
    dr1x90_ddrppc_base_cfg();
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 07 : MDL Calibration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x07, 0xffff);
    regData = dr1x90_reg_read(0x27a0);

    ////printf("DDRGPLL_CTRL18\n");
   //  printf(" 0x27a0.before  data =  0x%x\n", regData);

    dr1x90_ddrppc_mdl_cal();

    regData = dr1x90_reg_read(0x27a0);
   //  printf("\n DX0 mdl.   data =  0x%x\n", regData);

    regData = dr1x90_reg_read(0x28a0);
   //  printf("\n DX1 mdl.   data =  0x%x\n", regData);

    regData = dr1x90_reg_read(0x29a0);
   //  printf("\n DX2 mdl.   data =  0x%x\n", regData);

    regData = dr1x90_reg_read(0x2aa0);
   //  printf("\n DX3 mdl.   data =  0x%x\n", regData);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 08 : Fast Init
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x08, 0xffff);
    dr1x90_ddrppc_fast_init();
    //Alc_GpioMaskWrite(GPIO_CH0, 0x0222, 0xffff);
    dr1x90_zq_overwrite_cfg();

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 08 : DRAM INIT
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x09, 0xffff);
    dr1x90_ddrppc_set_timing();

    //Alc_GpioMaskWrite(GPIO_CH0, 0x0A, 0xffff);
    dr1x90_ddrppc_set_ddrmr();
    dr1x90_ddrppc_dram_init();

    //Alc_GpioMaskWrite(GPIO_CH0, 0x16, 0xffff);

    set_mpu();
    dr1x90_ddrmc_init(0);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 08 : PPC MTEST
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x0B, 0xffff);
    dr1x90_pub_training_cfg();
    //dr1x90_do_training(1,1,1);

    regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0MDLR0, IPRD_offset, IPRD_mask);
   //  printf("\n DX0 mdl.   data =  0x%x\n", regData);

    for (u8 n = 0; n <= 0; n++) {

        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, DGSL_offset, DGSL_mask);
       //  printf("\nDX 0x%x,  DGSL =  0x%x \n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, WLSL_offset, WLSL_mask);
       //  printf("\nDX 0x%x,  WLSL =  0x%x \n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR0 + 0x100 * n, WLD_offset, WLD_mask);
       //  printf("\nDX 0x%x,  WLD =  0x%x \n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * n, WDQD_offset, WDQD_mask);
       //  printf("\nDX 0x%x,  WDQD =  0x%x \n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask);
       //  printf("\nDX 0x%x,  DQSGD =  0x%x \n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask);
       //  printf("\nDX 0x%x,  RDQSD =  0x%x \n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask);
       //  printf("\nDX 0x%x,  RDQSND =  0x%x \n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR5 + 0x100 * n, DQSGSD_offset, DQSGSD_mask);
       //  printf("\nDX 0x%x,  DQSGSD =  0x%x \n", n, regData);
    }

   /* dr1x90_do_training(1, 0, 0);
    dr1x90_do_training(0, 1, 0);*/
    //soft_gate_training();
    dr1x90_do_training(0, 0, 1);

    for (u8 n = 0; n <= 3; n++) {
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, DGSL_offset, DGSL_mask);
       //  printf("\nDX 0x%x,  DGSL =  0x%x \n", n, regData);
        //regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, WLSL_offset, WLSL_mask);
        //printf("\nDX 0x%x,  WLSL =  0x%x \n", n, regData);
        //regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR0 + 0x100 * n, WLD_offset, WLD_mask);
        //printf("\nDX 0x%x,  WLD =  0x%x \n", n, regData);
        //regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * n, WDQD_offset, WDQD_mask);
        //printf("\nDX 0x%x,  WDQD =  0x%x \n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask);
       //  printf("\nDX 0x%x,  DQSGD =  0x%x \n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask);
       //  printf("\nDX 0x%x,  RDQSD =  0x%x \n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask);
       //  printf("\nDX 0x%x,  RDQSND =  0x%x \n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR5 + 0x100 * n, DQSGSD_offset, DQSGSD_mask);
       //  printf("\nDX 0x%x,  DQSGSD =  0x%x \n", n, regData);
    }


    //dr1x90_iomc_internal_loopback_cfg();

    // mtest_err = dr1x90_ddrppc_mtest(0, 0, 0, 9);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 08 : HandOff to DFI
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x0A, 0xffff);
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR1, PUBMODE_offset, PUBMODE_mask, 0);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 100 : MC Post Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x333, 0xffff);
    dr1x90_ddrmc_post_cfg();

    regData = dr1x90_field_read(DDRC_ADDR_PPC + PGCR1, PUBMODE_offset, PUBMODE_mask);
   //  printf("\n initial PUBMODE.   data =  0x%x\n", regData);


    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_hpr_q_depth_offset, dbg_hpr_q_depth_mask);
   //  printf("\n MC initial dbg_hpr_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_lpr_q_depth_offset, dbg_lpr_q_depth_mask);
   //  printf("\n MC initial dbg_lpr_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_w_q_depth_offset, dbg_w_q_depth_mask);
   //  printf("\n MC initial dbg_w_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_wr_q_empty_offset, dbg_wr_q_empty_mask);
   //  printf("\n MC initial dbg_wr_q_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_rd_q_empty_offset, dbg_rd_q_empty_mask);
   //  printf("\n MC initial dbg_rd_q_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, rd_data_pipeline_empty_offset, rd_data_pipeline_empty_mask);
   //  printf("\n MC initial rd_data_pipeline_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, wr_data_pipeline_empty_offset, wr_data_pipeline_empty_mask);
   //  printf("\n MC initial wr_data_pipeline_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_stall_wr_offset, dbg_stall_wr_mask);
   //  printf("\n MC initial dbg_stall_wr.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_stall_rd_offset, dbg_stall_rd_mask);
   //  printf("\n MC initial dbg_stall_rd.   data =  0x%x\n", regData);


    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + INIT3, mr_offset, mr_mask);
   //  printf("\n MR0.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + INIT3, emr_offset, emr_mask);
   //  printf("\n MR1.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + INIT4, emr2_offset, emr2_mask);
   //  printf("\n MR2.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + INIT4, emr3_offset, emr3_mask);
   //  printf("\n MR3.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DRAMTMG2, write_latency_offset, write_latency_mask);
   //  printf("\n write_latency.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DRAMTMG2, read_latency_offset, read_latency_mask);
   //  printf("\n read_latency.   data =  0x%x\n", regData);


    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFITMG0, dfi_tphy_wrlat_offset, dfi_tphy_wrlat_mask);
   //  printf("\n dfi_tphy_wrlat.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFITMG0, dfi_t_rddata_en_offset, dfi_t_rddata_en_mask);
   //  printf("\n dfi_t_rddata_en.   data =  0x%x\n", regData);

    set_rand_seed(100);

    soft_reye_scanning();

    soft_weye_scanning();

    printf(" write DRAM address start test \r\n");
    for (u8 i = 0; i < 10; i++) {

        //dr1x90_dram_write(0x100000 + i * 4, 0x12345678);
        u32 zq_val = get_rand();
        dr1x90_dram_write(0x100000 + i * 8, zq_val);
        regData = dr1x90_dram_read(0x100000 + i * 8);
        if ((regData & 0xffffffff) == (zq_val & 0xffffffff)) {
            //printf(" pass No. 0x%08x\n", i);
        }
        else {
            ++mtest_err;
            printf(" No. 0x%08x, read Error : address 0x%08x =  0x%08x, expect = 0x%08x\r\n", i, (0x100000 + i * 8), regData, zq_val);
        }
    }
    printf(" write DRAM address test done \r\n");
 /*   regData = dr1x90_dram_read(0x100000);
   //  printf(" read address 0x100000.   data =  0x%x\n", regData);

    regData = dr1x90_dram_read(0x100004);
   //  printf(" read address 0x100004.   data =  0x%x\n", regData);*/


    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_hpr_q_depth_offset, dbg_hpr_q_depth_mask);
   //  printf("\n MC test dbg_hpr_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_lpr_q_depth_offset, dbg_lpr_q_depth_mask);
   //  printf("\n MC test dbg_lpr_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_w_q_depth_offset, dbg_w_q_depth_mask);
   //  printf("\n MC test dbg_w_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_wr_q_empty_offset, dbg_wr_q_empty_mask);
   //  printf("\n MC test dbg_wr_q_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_rd_q_empty_offset, dbg_rd_q_empty_mask);
   //  printf("\n MC test dbg_rd_q_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, rd_data_pipeline_empty_offset, rd_data_pipeline_empty_mask);
   //  printf("\n MC test rd_data_pipeline_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, wr_data_pipeline_empty_offset, wr_data_pipeline_empty_mask);
   //  printf("\n MC test wr_data_pipeline_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_stall_wr_offset, dbg_stall_wr_mask);
   //  printf("\n MC test dbg_stall_wr.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_stall_rd_offset, dbg_stall_rd_mask);
   //  printf("\n MC test dbg_stall_rd.   data =  0x%x\n", regData);

    regData = dr1x90_dram_read(0xF84400AC);
   //  printf(" XMON Debug Timeout   data =  0x%x\n", regData);

    regData = dr1x90_dram_read(0xF8800000);
   //  printf(" chipID   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DRAMTMG2, write_latency_offset, write_latency_mask);
   //  printf("\n write_latency.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DRAMTMG2, read_latency_offset, read_latency_mask);
   //  printf("\n read_latency.   data =  0x%x\n", regData);


    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFITMG0, dfi_tphy_wrlat_offset, dfi_tphy_wrlat_mask);
   //  printf("\n dfi_tphy_wrlat.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFITMG0, dfi_t_rddata_en_offset, dfi_t_rddata_en_mask);
   //  printf("\n dfi_t_rddata_en.   data =  0x%x\n", regData);


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Test Done
    ///////////////////////////////////////////////////////////////////////////////////////////////

    return mtest_err;
}
