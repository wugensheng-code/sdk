#include "alc_types.h"
#include "dr1x90_ddrc_init.h"
#include "dr1x90_misc.h"

#include "dr1x90_crg.h"
#include "dr1x90_mpu.h"

/*************************************************************/
int demo_ddr_init()
{
    u32 regData = 0;
    u32 mtest_err = 0;

    printf("DDR Init Proc\r\n");

    regData = dr1x90_reg_read(0x27a0);
    printf(" 0x27a0.initial  data =  0x%x\r\n", regData);

    dr1x90_reg_write(0x11b0 ,0x00000050); // DFIMISC
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 01 : GPLL Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_field_write(DDRC_ADDR_GPLL + DDRGPLL_CTRL9, ctrl9_fbk_div_offset,  ctrl9_fbk_div_mask,  63);

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
    dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR4, AC_BANK_MTX_IO20_SEL_offset, AC_BANK_MTX_IO20_SEL_mask, 21); // ddr3_a14
    dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR4, AC_BANK_MTX_IO21_SEL_offset, AC_BANK_MTX_IO21_SEL_mask, 25); // ddr3_we_n
    dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR4, AC_BANK_MTX_IO22_SEL_offset, AC_BANK_MTX_IO22_SEL_mask, 36); // ddr3_rst_n
    dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR5, AC_BANK_MTX_IO25_SEL_offset, AC_BANK_MTX_IO25_SEL_mask, 33); // ddr3_ras_n
    dr1x90_field_write(DDRC_ADDR_GPLL + AC_BANK_MTX_CR7, AC_BANK_MTX_IO36_SEL_offset, AC_BANK_MTX_IO36_SEL_mask, 26); // ddr3_cas_n

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 06 : PPC Base Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x06, 0xffff);
    dr1x90_ddrppc_base_cfg();
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 07 : MDL Calibration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x07, 0xffff);
    dr1x90_ddrppc_mdl_cal();

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
    dr1x90_ddrmc_init(0);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 08 : PPC MTEST
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x0B, 0xffff);
    dr1x90_pub_training_cfg();
    dr1x90_do_training(1,1,1);
    //dr1x90_iomc_internal_loopback_cfg();
    //Alc_GpioMaskWrite(GPIO_CH0, 0x111, 0xffff);
    mtest_err = dr1x90_ddrppc_mtest(0, 0, 0, 9);
    printf("mtest errcnt: %d\r\n", mtest_err);

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



    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Test Done
    ///////////////////////////////////////////////////////////////////////////////////////////////
    const uint64_t gold = 0x1145141919810UL;

    crg_srst_release(SRST_DDRBUS);
    mpu_disable(MPU_DDR0_INST);
    mpu_disable(MPU_DDR1_INST);

    *(volatile uint64_t*)(0x400000) = gold;
    uint64_t val = *(volatile uint64_t*)(0x400000);
    printf("DDR Read Back: 0x%lx\r\n", val);
    printf("DDR Init: %s\r\n", val == gold ? "PASS" : "FAIL");

    return mtest_err == 0 && val == gold;
}
