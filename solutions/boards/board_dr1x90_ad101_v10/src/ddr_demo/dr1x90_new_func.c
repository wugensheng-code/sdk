#include "dr1x90_new_func.h"
#include "dr1x90_ddrc_init.h"
#include <stdio.h>

//gate_chain
u8 env_cfg_en_read_gate_train = 1;
u8 env_cfg_is_micron = 1;
u8 env_cfg_DM = 1;


//eye train
u8 env_cfg_en_bit_deskew_train = 1;
u8 env_cfg_en_eye_center_train = 1;
u8 ddr_vif_manual_ddl_update ;


void pinmux_config_dbgm_ddr()
{
    static volatile uint32_t* const MIO_FUNCSEL = 0xF8803000;
    static volatile uint32_t* const MIO_F15SEL = 0xF8803100;
    static volatile uint32_t* const CFG_DBGM_SEL = 0xF8800100;

    u32 reg_data1 = 0;
    u32 reg_data2 = 5;
    // DBGM[15 : 0]
    const int dbgm_pin[16] = { 17, 18, 19, 29, 30, 31, 32, 33, 40, 41, 42, 43, 44, 45, 52, 53 };
    // PINMUX
    for (int i = 0; i < 16; ++i) {
        // MIO_FUNCSEL[dbgm_pin[i]] = 15U;
        // MIO_F15SEL[dbgm_pin[i]] = 1U;
        uint32_t offset = dbgm_pin[i];
        ////  printf("\noffset = 0x%x\r\n", offset);

        dr1x90_ddr_reg_write(&ftcHandle, MIO_FUNCSEL + offset, 15U);
        //dr1x90_ddr_reg_read(&ftcHandle, MIO_FUNCSEL + offset,&reg_data1);

        //printf("\n******  addr = 0x%x,data = 0x%x *********\r\n", MIO_FUNCSEL + offset, reg_data1);


        dr1x90_ddr_reg_write(&ftcHandle, MIO_F15SEL + offset, 0x1);

        //dr1x90_ddr_reg_read(&ftcHandle, MIO_F15SEL + offset, &reg_data2);
        //printf("\n******  addr = 0x%x,data2 = 0x%x  *********\r\n", MIO_F15SEL + offset, reg_data2);
    }


    // Debug monitor sel
    uint32_t cfg = 0U | (1U << 7);
    // *CFG_DBGM_SEL = cfg;
    //dr1x90_ddr_reg_write(&ftcHandle, CFG_DBGM_SEL, cfg);
    dr1x90_ddr_reg_write(&ftcHandle, CFG_DBGM_SEL, cfg);

    //dr1x90_ddr_reg_read(&ftcHandle, CFG_DBGM_SEL, &reg_data);
    //printf("\n****** CFG_DBGM_SEL  addr = 0x%x,data = 0x%x  *********\r\n", CFG_DBGM_SEL, reg_data);

}

void internal_loopback_setup()
{
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr0_cfg3, U_byte0_se_mc1_lb_en_offset, U_byte0_se_mc1_lb_en_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr1to4_cfg11, U_byte0_quad1_mc1_lb_en_offset, U_byte0_quad1_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr5to8_cfg11, U_byte0_quad2_mc1_lb_en_offset, U_byte0_quad2_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr9to12_cfg11, U_byte0_quad3_mc1_lb_en_offset, U_byte0_quad3_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr13to16_cfg11, U_byte1_quad1_mc1_lb_en_offset, U_byte1_quad1_mc1_lb_en_mask, 0xf);


    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr17to20_cfg11, U_byte1_quad2_mc1_lb_en_offset, U_byte1_quad2_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr21to24_cfg11, U_byte1_quad3_mc1_lb_en_offset, U_byte1_quad3_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr25to28_cfg11, U_byte2_quad1_mc1_lb_en_offset, U_byte2_quad1_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr29to32_cfg11, U_byte2_quad2_mc1_lb_en_offset, U_byte2_quad2_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr33to36_cfg11, U_byte2_quad3_mc1_lb_en_offset, U_byte2_quad3_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr37to40_cfg11, U_byte3_quad1_mc1_lb_en_offset, U_byte3_quad1_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr41to44_cfg11, U_byte3_quad2_mc1_lb_en_offset, U_byte3_quad2_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr45to48_cfg11, U_byte3_quad3_mc1_lb_en_offset, U_byte3_quad3_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr49_cfg3, U_byte3_se_mc1_lb_en_offset, U_byte3_se_mc1_lb_en_mask, 1);

    //BANK0
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr0_cfg3, U_byte0_se_mc1_lb_en_offset, U_byte0_se_mc1_lb_en_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr1to4_cfg11, U_byte0_quad1_mc1_lb_en_offset, U_byte0_quad1_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr5to8_cfg11, U_byte0_quad2_mc1_lb_en_offset, U_byte0_quad2_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr9to12_cfg11, U_byte0_quad3_mc1_lb_en_offset, U_byte0_quad3_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr13to16_cfg11, U_byte1_quad1_mc1_lb_en_offset, U_byte1_quad1_mc1_lb_en_mask, 0xf);


    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr17to20_cfg11, U_byte1_quad2_mc1_lb_en_offset, U_byte1_quad2_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr21to24_cfg11, U_byte1_quad3_mc1_lb_en_offset, U_byte1_quad3_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr25to28_cfg11, U_byte2_quad1_mc1_lb_en_offset, U_byte2_quad1_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr29to32_cfg11, U_byte2_quad2_mc1_lb_en_offset, U_byte2_quad2_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr33to36_cfg11, U_byte2_quad3_mc1_lb_en_offset, U_byte2_quad3_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr37to40_cfg11, U_byte3_quad1_mc1_lb_en_offset, U_byte3_quad1_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr41to44_cfg11, U_byte3_quad2_mc1_lb_en_offset, U_byte3_quad2_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr45to48_cfg11, U_byte3_quad3_mc1_lb_en_offset, U_byte3_quad3_mc1_lb_en_mask, 0xf);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + pr49_cfg3, U_byte3_se_mc1_lb_en_offset, U_byte3_se_mc1_lb_en_mask, 1);

}

int dr1x90_ddrppc_mtest_my(u8 bank, u16 row, u16 col, u8 byteNum)
{
    u8  patternType;
    u8  iterateCnt = 128;   // 240
    u32 regData;
    u8  bistDone;
    u16 bistDxErr;
    u8  rcvCnt;
    int mbist_fail_cnt = 0;

    dr1x90_field_write(DDRC_ADDR_PPC + PGCR1, LBMODE_offset, LBMODE_mask, 1);
    dr1x90_field_write(DDRC_ADDR_PPC + BISTWCR, BWCNT_offset, BWCNT_mask, iterateCnt);

    dr1x90_reg_write(DDRC_ADDR_PPC + BISTRR, 0x00000003);
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTLSR, 0x8DED3E2A);
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTUDPR, 0x1DF428D5);
    dr1x90_reg_write(DDRC_ADDR_PPC + DTDR0, 0xff00ff00);
    dr1x90_reg_write(DDRC_ADDR_PPC + DTDR1, 0x00ff00ff);
    dr1x90_reg_write(DDRC_ADDR_PPC + UDDR0, 0x55aa55aa);
    dr1x90_reg_write(DDRC_ADDR_PPC + UDDR1, 0x5aa55aa5);

    dr1x90_mtest_set_addr(bank, row, col);

    dr1x90_reg_write(DDRC_ADDR_PPC + BISTMSKR0, 0xFFFFFFFF);
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTMSKR1, 0xFFFFFFFF);
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTMSKR2, 0x00000000);

    for (patternType = 15; patternType <= 15; patternType++) {
        dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 0);
        dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 1);

        if (patternType == 0x18) continue;

        regData = dr1x90_reg_read(DDRC_ADDR_PPC + BISTRR);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BINST_offset, BINST_mask, 1, regData);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BMODE_offset, BMODE_mask, 0, regData);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BACEN_offset, BACEN_mask, 0, regData);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BDXEN_offset, BDXEN_mask, 1, regData);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BDPAT_offset, BDPAT_mask, patternType, regData);
        regData = dr1x90_field_set(DDRC_ADDR_PPC + BISTRR, BDXSEL_offset, BDXSEL_mask, byteNum, regData);
        dr1x90_reg_write(DDRC_ADDR_PPC + BISTRR, regData);

        dr1x90_field_wait(DDRC_ADDR_PPC + BISTGSR, BDONE_offset, BDONE_mask, 1, 0x1000); // wait for bistDone

        bistDone = dr1x90_field_read(DDRC_ADDR_PPC + BISTGSR, BDONE_offset, BDONE_mask);
        bistDxErr = dr1x90_field_read(DDRC_ADDR_PPC + BISTGSR, BDXERR_offset, BDXERR_mask);
        rcvCnt = dr1x90_field_read(DDRC_ADDR_PPC + BISTWCSR, DXWCNT_offset, DXWCNT_mask); // Equal to iterateCnt

       //  printf("\nbistDone = 0x%x bistDxErr = 0x%x rcvCnt = 0x%x \r\n", bistDone, bistDxErr, rcvCnt);

        dr1x90_field_write(DDRC_ADDR_PPC + BISTRR, BINST_offset, BINST_mask, 3);
        dr1x90_field_write(DDRC_ADDR_PPC + BISTRR, BINST_offset, BINST_mask, 2);
        dr1x90_field_write(DDRC_ADDR_PPC + BISTRR, BINST_offset, BINST_mask, 0);

        if (bistDxErr == 1) {
            mbist_fail_cnt++;
        }

    }

    return mbist_fail_cnt;
}

void external_loopback(void)
{
    //set  *_mc1_rdqsn_gate_dis_en 1
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte0_glue_cfg0, U_byte0_glue_mc1_rdqsn_gate_dis_en_offset, U_byte0_glue_mc1_rdqsn_gate_dis_en_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte1_glue_cfg0, U_byte1_glue_mc1_rdqsn_gate_dis_en_offset, U_byte1_glue_mc1_rdqsn_gate_dis_en_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte2_glue_cfg0, U_byte2_glue_mc1_rdqsn_gate_dis_en_offset, U_byte2_glue_mc1_rdqsn_gate_dis_en_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + byte3_glue_cfg0, U_byte3_glue_mc1_rdqsn_gate_dis_en_offset, U_byte3_glue_mc1_rdqsn_gate_dis_en_mask, 1);

    //set  *_mc1_rdqsn_gate_dis_en 1  BANK0
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + byte0_glue_cfg0, U_byte0_glue_mc1_rdqsn_gate_dis_en_offset, U_byte0_glue_mc1_rdqsn_gate_dis_en_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + byte1_glue_cfg0, U_byte1_glue_mc1_rdqsn_gate_dis_en_offset, U_byte1_glue_mc1_rdqsn_gate_dis_en_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + byte2_glue_cfg0, U_byte2_glue_mc1_rdqsn_gate_dis_en_offset, U_byte2_glue_mc1_rdqsn_gate_dis_en_mask, 1);
    dr1x90_field_write(DDRC_ADDR_BK0_IOMC1 + byte3_glue_cfg0, U_byte3_glue_mc1_rdqsn_gate_dis_en_offset, U_byte3_glue_mc1_rdqsn_gate_dis_en_mask, 1);

    //set oe_md = 2
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xc000 + 0x1c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xd000 + 0x1c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xe000 + 0x1c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xf000 + 0x1c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xc000 + 0x20, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xd000 + 0x20, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xe000 + 0x20, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xf000 + 0x20, 0x11111111);

    //set oe_md = 2  BANK0
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x8000 + 0x1c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x9000 + 0x1c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xa000 + 0x1c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xb000 + 0x1c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x800 + 0x20, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x9000 + 0x20, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xa000 + 0x20, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xb000 + 0x20, 0x11111111);

    //set te_md = 2 BANK0
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x8000 + 0x24, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x9000 + 0x24, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xa000 + 0x24, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xb000 + 0x24, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x8000 + 0x28, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x9000 + 0x28, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xa000 + 0x28, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xb000 + 0x28, 0x11111111);

    //set te_md = 2
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xc000 + 0x24, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xd000 + 0x24, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xe000 + 0x24, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xf000 + 0x24, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xc000 + 0x28, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xd000 + 0x28, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xe000 + 0x28, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xf000 + 0x28, 0x11111111);

    //set pdr_md = 2 BANK0
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x8000 + 0x2c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x9000 + 0x2c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xa000 + 0x2c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xb000 + 0x2c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x8000 + 0x30, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x9000 + 0x30, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xa000 + 0x30, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xb000 + 0x30, 0x11111111);

    //set pdr_md = 2
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xc000 + 0x2c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xd000 + 0x2c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xe000 + 0x2c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xf000 + 0x2c, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xc000 + 0x30, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xd000 + 0x30, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xe000 + 0x30, 0x11111111);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0xf000 + 0x30, 0x11111111);

    //set LCDLR3 = 0 ��LCDL4 = 0
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x2000 + 0x78c, 0x0);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x2000 + 0x790, 0x0);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x2000 + 0x88c, 0x0);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x2000 + 0x890, 0x0);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x2000 + 0x98c, 0x0);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x2000 + 0x990, 0x0);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x2000 + 0xa8c, 0x0);
    dr1x90_ddr_reg_write(&ftcHandle, 0xf8420000 + 0x2000 + 0xa90, 0x0);


}


void AC_swap_en()
{
    dr1x90_field_write(DDRC_ADDR_GPLL + BK0_T01_MATRIX_CTRL, BK0_IS_AC_offset, BK0_IS_AC_mask, 0x0);
}

void dr1x90_zq_overwrite_cfg_new()
{


    u8 n;
    u8 i;
    //u8 BankInfo;
    u8 override_en = 1
        ;
    u8 pzq_cal_done;

    u32 bank_zq_ac_code;
    u32 bank_zq_dx_code;

    if (env_cfg_ddr_mode == DDR3) {
        bank_zq_ac_code = 0x1a1a3434;
        bank_zq_dx_code = 0x1a1a3434;
    }
    else {//DDR4
        bank_zq_ac_code = 0x19193235;
        bank_zq_dx_code = 0x19193235;
    }





    //u32 zqcal_done;
    //u32 zqcal_err;

    u32 regData, regDataNew;

    u32 env_cfg_dci_cascade = 0;
    // ddr4 = l ddr3 = 0
   // u32 env_cfg_ddr_mode = 3;
    u32 env_cfg_pzq_slave = 1;

    u8 env_cfg_host_vrefr;
    u8 env_cfg_host_vref;

    if (env_cfg_ddr_mode == DDR3) {
        env_cfg_host_vrefr = 0x0;
        env_cfg_host_vref = 0x2e;
    }
    else {//DDR4
        env_cfg_host_vrefr = 0x0;
        env_cfg_host_vref = 0x0;
    }


    pzq_cal_done = 0;

    if (override_en == 1)
    {
        dr1x90_field_write(DDRC_ADDR_PPC + PIR, ZCALBYP_offset, ZCALBYP_mask, 1);
        dr1x90_field_write(DDRC_ADDR_PPC + PIR, ZCAL_offset, ZCAL_mask, 0);
        //zQ override
        for (n = 0; n <= 1; n++)
        {
            //if ((n == 1) && (env_cfg_pzq_slave == 1)) continue;
            //BankInfo = Alc_DramFieldRead(SYS_BYTE_ MAP_INFO_0��8*(2-n)��0xff << 8*(2-n) );/
            //if (BankInfo == 0xff) continue ;
            dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0VREF + 0x2000 * n, ZQCALEN_offset, ZQCALEN_mask, 1);
            dr1x90_field_write(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, ddr_mode_offset, ddr_mode_mask, 1);



            for (i = 0; i <= 1; i++)
            {
                if (i == 0) {
                    regData = bank_zq_ac_code;
                }
                else {
                    regData = ((env_cfg_ddr_mode == DDR4) ? (bank_zq_dx_code & 0xff00ffff) : bank_zq_dx_code);
                }

                dr1x90_reg_write(DDRC_ADDR_BK0_VREF + ZQ0DR + 0x2000 * n + 0x04 * i, regData);

                regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n + 0x04 * i);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n + 0x04 * i, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 0, regData);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n + 0x04 * i, PGWAIT_offset, PGWAIT_mask, 1, regDataNew);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n + 0x04 * i, AVGEN_offset, AVGEN_mask, (env_cfg_ddr_mode == DDR4) ? 1 : 0, regDataNew); //ddr4=1 ddr3=0
                dr1x90_reg_update(DDRC_ADDR_BK0_VREF + ZQ0CR + 0x2000 * n + 0x04 * i, regData, regDataNew);

                regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n + 0x04 * i);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n + 0x04 * i, DRV_ZDEN_offset, DRV_ZDEN_mask, 1, regData);
                regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n + 0x04 * i, ODT_ZDEN_offset, ODT_ZDEN_mask, 1, regDataNew);
                dr1x90_reg_update(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n + 0x04 * i, regData, regDataNew);
            }
            dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ0PR + 0x2000 * n, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 1);
            dr1x90_field_write(DDRC_ADDR_BK0_VREF + ZQ1PR + 0x2000 * n, FORCE_ZCAL_VT_UPDATE_offset, FORCE_ZCAL_VT_UPDATE_mask, 1);
        }

    }
    //	setup a proper vref for basic functionality
    for (n = 0; n <= 1; n++)
    {
        //if ( (n == 1) && (env_cfg_pzq_slave == 1) ) continue ;
        // check if the bank is used, if not, skip the config
        //BankInfo = Alc_DramFieldRead(SYS_BYTE_MAP_INFO_0,8 * (2-n),0xff << 8* (2-n) );
        //if (BankInfo == 0xff)  continue ;
        regData = dr1x90_reg_read(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, Vref_from_sel_offset, Vref_from_sel_mask, 1, regData);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, Vref_rg_sel_offset, Vref_rg_sel_mask, env_cfg_host_vrefr, regDataNew);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, Vref_rk_sel_offset, Vref_rk_sel_mask, 0x0, regDataNew);
        regDataNew = dr1x90_field_set(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, ddr_mode_offset, ddr_mode_mask, (env_cfg_ddr_mode == DDR4) ? 1 : 0, regDataNew);
        dr1x90_reg_update(DDRC_ADDR_BK0_VREF + PZQVREF + 0x2000 * n, regData, regDataNew);

        dr1x90_field_write(DDRC_ADDR_BK0_VREF + VREF1 + 0x2000 * n, Vref1_ctrl_rank0_offset, Vref1_ctrl_rank0_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + VREF2 + 0x2000 * n, Vref2_ctrl_rank0_offset, Vref2_ctrl_rank0_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + VREF3 + 0x2000 * n, Vref3_ctrl_rank0_offset, Vref3_ctrl_rank0_mask, env_cfg_host_vref);
        dr1x90_field_write(DDRC_ADDR_BK0_VREF + VREF4 + 0x2000 * n, Vref4_ctrl_rank0_offset, Vref4_ctrl_rank0_mask, env_cfg_host_vref);
    }

    if (env_cfg_ddr_mode == DDR3)
    {
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + pr49_cfg3, U_byte3_se_mc1_vref_padin_en_offset, U_byte3_se_mc1_vref_padin_en_mask, 1);
        dr1x90_field_write(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg2, U_bankref_hp_mc1_vref_ext_en_offset, U_bankref_hp_mc1_vref_ext_en_mask, 1);

    }
}

void gate_train()
{
    u8 DRAM_READ_RANDOM_INSTEAD_X = 0;
    u8 EN_GRAIN_REFRESH = 0;


    u32 regData, regDataNew;

    if (env_cfg_en_read_gate_train == 1)
    {
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + DTCR0);
        if (env_cfg_ddr_mode == DDR4) {
            regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTWBDDM_offset, DTWBDDM_mask, (env_cfg_DM | env_cfg_WDBI), regData);
        }
        else {
            regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTWBDDM_offset, DTWBDDM_mask, 1, regData);
        }
        if ((DRAM_READ_RANDOM_INSTEAD_X == 1) | (env_cfg_is_micron == 1)) {
            regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTRDBITR_offset, DTRDBITR_mask, 0, regDataNew);
        }
        else if ((env_cfg_ddr_mode == DDR4) && (env_cfg_RDBI == 1)) {
            regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTRDBITR_offset, DTRDBITR_mask, 1, regDataNew);
        }
        else {
            regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTRDBITR_offset, DTRDBITR_mask, 2, regDataNew);
        }

        regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTBDC_offset, DTBDC_mask, 2, regDataNew);  //data training bit deskewe centerning enable

        //temp disable refresh during training
        if (EN_GRAIN_REFRESH == 1) {
            regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, RFSHDT_offset, RFSHDT_mask, 8, regDataNew);//drefresh during training and bist, defalut is 8
        }
        else {
            regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, RFSHDT_offset, RFSHDT_mask, 0, regDataNew);
        }

        regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTMPR_offset, DTMPR_mask, 1, regDataNew); //data training using MPR
        dr1x90_reg_update(DDRC_ADDR_PPC + DTCR0, regData, regDataNew);

        regData = dr1x90_reg_read(DDRC_ADDR_PPC + DTCR1);
        regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDPRMBL_TRN_offset, RDPRMBL_TRN_mask, 1, regData); //read Preamble Training enable
        dr1x90_reg_update(DDRC_ADDR_PPC + DTCR1, regData, regDataNew);

        regData = dr1x90_reg_read(DDRC_ADDR_PPC + DXCCR);
        regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DXCCR, QSCNTEN_offset, QSCNTEN_mask, 1, regData);
        dr1x90_reg_update(DDRC_ADDR_PPC + DXCCR, regData, regDataNew);

    }

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + OFFCAL_PR);
    if (env_cfg_ddr_mode == DDR3){
        regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + OFFCAL_PR, dqs_pupd_en_offset, dqs_pupd_en_mask, 1, regData);
    }
    else {
        regDataNew = regData;
    }
    regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + OFFCAL_PR, eye_rdbi_rpt_en_offset, eye_rdbi_rpt_en_mask, 1, regDataNew);
    dr1x90_reg_update(DDRC_ADDR_PPC + OFFCAL_PR, regData, regDataNew);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + DTCR0);
    regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTRPTN_offset, DTRPTN_mask, 1, regData);
    dr1x90_reg_update(DDRC_ADDR_PPC + DTCR0, regData, regDataNew);
}

void do_training(u8 wl_en,u8 wl2_en,u8 gt_en)
{
    u32 regData, regDataNew;
    u32 wlerr, wl2err,gterr;

   //  printf(" \r\n");
   //  printf( " +---------------------------------------+\r\n");
   //  printf( " |    do_training begin                  |\r\n");
   //  printf( " +---------------------------------------+\r\n");

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);

   //  printf( "\nGet PIR reg_data wlen = 0x%x wl2_en = 0x%x gt_en = 0x%x; before training regData = 0x%x\r\n", wl_en, wl2_en, gt_en, regData);

    if ( wl_en == 0x1)  {
        regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + PIR, WL_offset, WL_mask, 1, regData);
    }

    if (wl2_en == 0x1) {
        if (wl_en == 0x0)  {
            regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + PIR, WLADJ_offset, WLADJ_mask, 1, regData);
        }
        if (wl_en == 0x1 ) {
            regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + PIR, WLADJ_offset, WLADJ_mask, 1, regDataNew);
        }
    }

    if (gt_en == 0x1)  {
        regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + PIR, QSGATE_offset, QSGATE_mask, 1, regData);
    }


    if ((wl2_en || wl_en || gt_en) == 1 ) {
        regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 1, regDataNew);
        dr1x90_reg_update(DDRC_ADDR_PPC + PIR, regData, regDataNew);
    }

    if (wl_en == 0x1 ) {
        dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, WLDONE_offset, WLDONE_mask, 1, 20000000);
       //  printf("\nWLDONE training regData = 0x%x\r\n", wl_en, wl2_en, gt_en, regData);
    }

    if (wl2_en == 0x1 ) {
        dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, WLADONE_offset, WLADONE_mask, 1, 2000000);
    }

    if (gt_en == 0x1 ) {
        dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, QSGDONE_offset, QSGDONE_mask, 1, 2000000);
    }

    if ((wl_en || wl2_en || gt_en ) == 1 ){
        dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0, 200);
    }


    if (wl_en == 1 ) {
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        wlerr = (regData >> 21) & 0x1;
         if (wlerr == 1) {
            //  printf(" \n------- WL Failed ; PGSR0 is 0x%x\r\n",regData);
        }
         if (wlerr == 0) {
            //  printf(" \n------- WL PASSED ; PGSR0 is 0x%x\r\n", regData);
        }
    }

    if (wl2_en == 1 ) {
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        wl2err = ( regData >> 23 ) & 0x1;
        if (wl2err == 1) {
           //  printf(" \n------- WL2 Failed ; PGSR0 is 0x%x\r\n", regData);
        }
        if (wl2err == 0) {
           //  printf(" \n------- WL2 PASSED ; PGSR0 is 0x%x\r\n", regData);
        }
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
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x GTR0 :DGSL is 0x%x\r\n",n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, WLSL_offset, WLSL_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x GTR0 : WLSLis 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR0 + 0x100 * n, WLD_offset, WLD_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x LCDLR0 : WLD is 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * n, WDQD_offset, WDQD_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x LCDLR1 : WDQD is 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x LCDLR2 : DQSGD is 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x LCDLR3 :RDQSD is 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x LCDLR4 : RDQSND is 0x%x\r\n",n, regData);

    }



       //  printf(" \r\n");
       //  printf(" +---------------------------------------+\r\n");
       //  printf(" |    do_training end                  |\r\n");
       //  printf(" +---------------------------------------+\r\n");
       //  printf(" \r\n");



}

void eye_training()
{
    u32 regData, regDataNew;
    u32 rdErr, wrErr;

   //  printf(" \r\n");
   //  printf(" +---------------------------------------+\r\n");
   //  printf(" |    Eye Centering Training             |\r\n");
   //  printf(" +---------------------------------------+\r\n");
   //  printf(" \r\n");

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
    regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + PIR, WREYE_offset, WREYE_mask, 1, regData);
    regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + PIR, RDEYE_offset, RDEYE_mask, 1, regDataNew);
    regDataNew = dr1x90_field_set(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 1, regDataNew);
    dr1x90_reg_update(DDRC_ADDR_PPC + PIR, regData, regDataNew);

    dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, REDONE_offset, REDONE_mask, 1, 16);
    dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, WEDONE_offset, WEDONE_mask, 1, 16);
    dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0, 16);

    rdErr = dr1x90_field_read(DDRC_ADDR_PPC + PGSR0, PGSR0_REERR_offset, PGSR0_REERR_mask);
    wrErr = dr1x90_field_read(DDRC_ADDR_PPC + PGSR0, PGSR0_WEERR_offset, PGSR0_WEERR_mask);
    if ((rdErr || wrErr ) == 1) {
       //  printf(" ****   Error Occurs During Eye Centering Training     ****\r\n");
       //  printf("\nrdErr = %x , wrErr = %x\r\n", rdErr, wrErr);
    }

    if ( ((!(env_cfg_en_bit_deskew_train) && env_cfg_en_eye_center_train) || env_cfg_is_micron)  && ((env_cfg_ddr_mode == DDR4) && env_cfg_RDBI))
    {
        adjust_rddbi_level_result(0);
    }

    int n;
    for (n = 0; n <= 3; n++) {
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, DGSL_offset, DGSL_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x GTR0 :DGSL is 0x%x\r\n", n, regData);
        regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, WLSL_offset, WLSL_mask);
       //  printf(" \n***  DDRC_ADDR_PPC + DX %x GTR0 : WLSLis 0x%x\r\n", n, regData);
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

   //  printf(" \r\n");
   //  printf(" +---------------------------------------+\r\n");
   //  printf(" |    Eye Centering Training Finished    |\r\n");
   //  printf(" +---------------------------------------+\r\n");
   //  printf(" \r\n");



}

void adjust_rddbi_level_result(u8 pub_index)
{
    u32 regData_1, regData_2;
    ddr_vif_manual_ddl_update = 1;


   //  printf(" \r\n");
   //  printf(" +---------------------------------------+\r\n");
   //  printf(" |    adjust_rddbi_level_result          |\r\n");
   //  printf(" +---------------------------------------+\r\n");
   //  printf(" \r\n");

    //#define     DX0BDLR5            0x758
    //#define     DX0BDLR3            0x750
    //#define     DX1BDLR5            0x858
    u32 BDLR5[9] = { 0x758,0x858,0x958,0xa58,0xb58,0xc58,0xd58,0xe58,0xf58 };
    u32 BDLR3[9] = { 0x750,0x850,0x950,0xa50,0xb50,0xc50,0xd50,0xe50,0xf50 };
    int n;
    for (int j = 0; j <= 3; j++) {
        //if (env_cfg_dx_en[j] == 1) {
            regData_1  = dr1x90_field_read(DDRC_ADDR_PPC + BDLR5[j], DMRBD_offset, DMRBD_mask);
            regData_2 = dr1x90_field_read(DDRC_ADDR_PPC + BDLR3[j], DQ0RBD_offset, DQ0RBD_mask);
            for(n = 0;n<=1000;n++){}
            dr1x90_field_write(DDRC_ADDR_PPC + BDLR3[j], DMRBD_offset, DMRBD_mask, regData_2);
            for (n = 0; n <= 1000; n++) {}
        //}
    }

    ddr_vif_manual_ddl_update = 0;

   //  printf(" \r\n");
   //  printf(" +---------------------------------------+\r\n");
   //  printf(" |    adjust_rddbi_level_result Finished |\r\n");
   //  printf(" +---------------------------------------+\r\n");
   //  printf(" \r\n");
}

void set_mpu () {

    u32 regData;
    regData = dr1x90_dram_read(0xf840e004);
   //  printf(" \n*** initial MPU0 is 0x%x\r\n", regData);
    dr1x90_dram_field_write(0xf840e004, 0, 1, 0);
    regData = dr1x90_dram_read(0xf840e004);
   //  printf(" \n***  MPU0 is 0x%x\r\n", regData);

    regData = dr1x90_dram_read(0xf840f004);
   //  printf(" \n*** initial MPU1 is 0x%x\r\n", regData);
    dr1x90_dram_field_write(0xf840f004, 0, 1, 0);
    regData = dr1x90_dram_read(0xf840f004);
   //  printf(" \n***  MPU1 is 0x%x\r\n", regData);

    regData = dr1x90_dram_read(0xF8801074);
   //  printf(" DDRBUS reset initial data =  0x%x\r\n", regData);
    //dr1x90_dram_field_write(0xF8801074, 14, 1, 1);
    dr1x90_dram_write(0xF8801074, 0xffffffff);
    regData = dr1x90_dram_read(0xF8801074);
   //  printf(" DDRBUS reset data =  0x%x\r\n", regData);

    regData = dr1x90_dram_read(0xF8800000);
   //  printf(" chipID initial data =  0x%x\r\n", regData);
}

void soft_gate_training( ) {

    //global logfile
    u32 loop_cnt;
    u32 regData;
    u8 n;

       //  printf(" \r\n");
       //  printf( "+---------------------------------------+\r\n");
       //  printf( "|    soft_gate_training begin           |\r\n");
       //  printf( "+---------------------------------------+\r\n");


        //#set regData[scan[regRead "PGCR3"] % x]
        //#set regNewData[fieldSet "PGCR3.PRFBYP" 1 $regData]
        //#regUpdate "PGCR3" $regData $regNewData

        dr1x90_field_write(DDRC_ADDR_PPC + DTCR0, DTDEN_offset, DTDEN_mask, 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DTCR1, BSTEN_offset, BSTEN_mask, 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DTCR1, RDLVLEN_offset, RDLVLEN_mask, 1);

        dr1x90_field_write(DDRC_ADDR_PPC + PIR, QSGATE_offset, QSGATE_mask, 1);
        dr1x90_field_write(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 1);

        dr1x90_field_write(DDRC_ADDR_PPC + PHY_EYE_CR3, grp0_sel_offset, grp0_sel_mask, 22);

        loop_cnt = 0;
        regData = dr1x90_field_read(DDRC_ADDR_PPC + PGSR0, QSGDONE_offset, QSGDONE_mask);
        while (regData != 1 ) {
            for (n = 0; n <= 3; n++) {

                regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, DGSL_offset, DGSL_mask);
               //  printf("Soft Gate Training, DX 0x%x GTR0.DGSL is 0x%x  \r\n", n, regData);
                regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR5 + 0x100 * n, DQSGSD_offset, DQSGSD_mask);
               //  printf("Soft Gate Training, DX 0x%x LCDLR5.DQSGSD is  0x%x \r\n", n, regData);
                regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask);
               //  printf("Soft Gate Training, DX 0x%x LCDLR2.DQSGD is  0x%x \r\n", n, regData);
                regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0RSR0 + 0x100 * n, DXnRSR0_QSGERR_offset, DXnRSR0_QSGERR_mask);
               //  printf("Soft Gate Training, DX 0x%x RSR0.QSGERR is  0x%x \r\n", n, regData);
                regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0RSR1 + 0x100 * n, RDLVLERR_offset, RDLVLERR_mask);
               //  printf("Soft Gate Training, DX 0x%x RSR1.RDLVLERR is  0x%x \r\n", n, regData);

            }

            dr1x90_reg_write(DDRC_ADDR_PPC + GPR0, 0);
            regData = dr1x90_field_read(DDRC_ADDR_PPC + GPR0, GPR0_offset, GPR0_mask);

           //  printf("Soft Gate Training, qs_status is 0x%x \r\n", regData);

            regData = dr1x90_field_read(DDRC_ADDR_PPC + GPR0, GPR0_offset, GPR0_mask);

            for (n = 0; n <= 3; n++) {
                regData = dr1x90_field_read(DDRC_ADDR_BK1_T0 + 0x1000 * n + DQSGRP_QS_GATE_SR, QS_GATE_SR_STATUS_offset, QS_GATE_SR_STATUS_mask);
               //  printf("Soft Gate Training, DX 0x%x DQSGRP_QS_GATE_SR is  0x%x \r\n", n, regData);
            }

            dr1x90_field_write(DDRC_ADDR_PPC + DTCR0, DTDSTP_offset, DTDSTP_mask, 1);

            loop_cnt = loop_cnt + 1;

            regData = dr1x90_field_read(DDRC_ADDR_PPC + PGSR0, QSGDONE_offset, QSGDONE_mask);

           //  printf("Polling 0x%x,  GateTraining done = 0x%x \r\n", loop_cnt, regData);

        }

        regData = dr1x90_field_read(DDRC_ADDR_PPC + PGSR0, PGSR0_QSGERR_offset, PGSR0_QSGERR_mask);

        if (regData == 1) {
       //  printf("GateTraining Failed ; PGSR0 is 0x%x , loop_cnt = 0x%x \r\n", regData, loop_cnt);
        } else {
       //  printf("GateTraining PASSED ; PGSR0 is 0x%x , loop_cnt = 0x%x \r\n", regData, loop_cnt);
        }

        for (n = 0; n <= 3; n++) {

            regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, DGSL_offset, DGSL_mask);
           //  printf("Soft Gate Training done, DX 0x%x GTR0.DGSL is 0x%x  \r\n", n, regData);
            regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR5 + 0x100 * n, DQSGSD_offset, DQSGSD_mask);
           //  printf("Soft Gate Training done, DX 0x%x LCDLR5.DQSGSD is  0x%x \r\n", n, regData);
            regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask);
           //  printf("Soft Gate Training done, DX 0x%x LCDLR2.DQSGD is  0x%x \r\n", n, regData);
            regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0RSR0 + 0x100 * n, DXnRSR0_QSGERR_offset, DXnRSR0_QSGERR_mask);
           //  printf("Soft Gate Training done, DX 0x%x RSR0.QSGERR is  0x%x \r\n", n, regData);
            regData = dr1x90_field_read(DDRC_ADDR_PPC + DX0RSR1 + 0x100 * n, RDLVLERR_offset, RDLVLERR_mask);
           //  printf("Soft Gate Training done, DX 0x%x RSR1.RDLVLERR is  0x%x \r\n", n, regData);

        }


       //  printf("\r\n");
       //  printf("+---------------------------------------+\r\n");
       //  printf("|    soft_gate_training end             |\r\n");
       //  printf("+---------------------------------------+\r\n");
       //  printf("\r\n");

}

static uint64_t rand_val;

void set_rand_seed(uint64_t v)
{
    rand_val = v;
}

uint64_t get_rand()
{
    rand_val = rand_val * 25214903917ULL + 11ULL;
    return rand_val;
}


void soft_reye_scanning() {

    //global logfile
    u32 loop_cnt;
    u32 regData;
    u8 n;
    u32 left_step;
    u32 right_step;
    u32 mid_step;

    u32 dx_MDL;
    u32 dx_DGSL;
    u32 dx_DQSGD;
    u32 dx_RDQSD;
    u32 dx_RDQSND;
    u32 dx_DQSGSD;
    u32 err_cnt = 0;


   //  printf(" \r\n");
   //  printf("+---------------------------------------+\r\n");
   //  printf("|    soft_reye_scanning begin           |\r\n");
   //  printf("+---------------------------------------+\r\n");

    //dr1x90_dram_write(0x100000 , 0x12345678);
   //  printf(" Start reye scan \r\n");
    left_step = 0;
    right_step = 0;
    mid_step = 0;
    err_cnt = 0;
    for (n = 0; n <= 3; n++) {
        dx_MDL = dr1x90_field_read(DDRC_ADDR_PPC + DX0MDLR0, IPRD_offset, IPRD_mask);
       //  printf("\neye scan : DX 0x%x  mdl =  0x%x\r\n", dx_MDL);
        dx_DGSL = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, DGSL_offset, DGSL_mask);
       //  printf("\neye scan : DX 0x%x,  DGSL is 0x%x  \r\n", n, dx_DGSL);
        dx_DQSGD = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask);
       //  printf("\neye scan : DX 0x%x,  DQSGD =  0x%x \r\n", n, dx_DQSGD);
        dx_RDQSD = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask);
       //  printf("\neye scan : DX 0x%x,  RDQSD =  0x%x \r\n", n, dx_RDQSD);
        dx_RDQSND = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask);
       //  printf("\neye scan : DX 0x%x,  RDQSND =  0x%x \r\n", n, dx_RDQSND);
        dx_DQSGSD = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR5 + 0x100 * n, DQSGSD_offset, DQSGSD_mask);
       //  printf("\neye scan : DX 0x%x,  DQSGSD =  0x%x \r\n", n, dx_DQSGSD);

        //dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, DGSL_offset, DGSL_mask,  );
        left_step = 0;
        err_cnt = 0;
        while (left_step <= dx_MDL) {
            //for (left_step = 0; left_step <= dx_MDL; left_step++)
            if (left_step > dx_DQSGD) {
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask, dx_DQSGD + dx_MDL / 2 - left_step);
                dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, DGSL_offset, DGSL_mask, dx_DGSL - 1);
            }
            else {
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask, dx_DQSGD - left_step);
            }
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask, dx_RDQSD - left_step);
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask, dx_RDQSND - left_step);
           //  printf(" left_step = 0x%x, write DRAM address \r\n", left_step);

            for (u8 i = 0; i < 10; i++) {
                u32 zq_val = get_rand();
                dr1x90_dram_write(0x100000 + i * 8, zq_val);
                regData = dr1x90_dram_read(0x100000 + i * 8);
                if ((regData & 0xffffffff) == (zq_val & 0xffffffff)) {
                    //printf(" pass No. 0x%08x\r\n", i);
                }
                else {
                   //  printf(" No. 0x%08x, read Error : address 0x%08x =  0x%08x, expect = 0x%08x\r\n", i, (0x100000 + i * 8), regData, zq_val);
                    err_cnt = err_cnt + 1;
                    break;
                }
            }
            if (err_cnt > 0) {
                break;
            }
            else {
                left_step++;
            }
        }

        //dx_DQSGD = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask);
       //  printf("\neye scan : DX 0x%x, old dx_DQSGD =  0x%x, left DQSGD =  0x%x \r\n", n, dx_DQSGD, dx_DQSGD - left_step);
        //dx_RDQSD = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask);
       //  printf("\neye scan : DX 0x%x, old dx_RDQSD =  0x%x, left RDQSD =  0x%x \r\n", n, dx_RDQSD, dx_RDQSD - left_step);
        //dx_RDQSND = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask);
       //  printf("\neye scan : DX 0x%x, old dx_RDQSND =  0x%x, left RDQSND =  0x%x \r\n", n, dx_RDQSND, dx_RDQSND - left_step);
        err_cnt = 0;
        right_step = 0;
        while (right_step <= dx_MDL) {
            //for (right_step = 0; right_step <= dx_MDL; right_step++)
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask, dx_DQSGD + right_step);
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask, dx_RDQSD + right_step);
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask, dx_RDQSND + right_step);
           //  printf(" right_step = 0x%x, write DRAM address \r\n", right_step);


            for (u8 i = 0; i < 10; i++) {
                u32 zq_val = get_rand();
                dr1x90_dram_write(0x100000 + i * 8, zq_val);
                regData = dr1x90_dram_read(0x100000 + i * 8);
                if ((regData & 0xffffffff) == (zq_val & 0xffffffff)) {
                    //printf(" pass No. 0x%08x\r\n", i);
                }
                else {
                   //  printf(" No. 0x%08x, read Error : address 0x%08x =  0x%08x, expect = 0x%08x\r\n", i, (0x100000 + i * 8), regData, zq_val);
                    err_cnt = err_cnt + 1;
                    break;
                }
            }
            if (err_cnt > 0) {
                break;
            }
            else {
                right_step++;
            }
        }

       //  printf("\neye scan : DX 0x%x, old dx_DQSGD =  0x%x, right DQSGD =  0x%x \r\n", n, dx_DQSGD, dx_DQSGD + right_step);
        //dx_RDQSD = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask);
       //  printf("\neye scan : DX 0x%x, old dx_RDQSD =  0x%x, right RDQSD =  0x%x \r\n", n, dx_RDQSD, dx_RDQSD + right_step);
        //dx_RDQSND = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask);
       //  printf("\neye scan : DX 0x%x, old dx_RDQSND =  0x%x, right RDQSND =  0x%x \r\n", n, dx_RDQSND, dx_RDQSND + right_step);

        if (right_step >= left_step) {
           mid_step = (right_step - left_step) / 2;
           dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask, dx_DQSGD + mid_step);
           dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask, dx_RDQSD + mid_step);
           dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask, dx_RDQSND + mid_step);
        } else {
           mid_step = (left_step - right_step) / 2;
           dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask, dx_DQSGD - mid_step);
           dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * n, RDQSD_offset, RDQSD_mask, dx_RDQSD - mid_step);
           dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * n, RDQSND_offset, RDQSND_mask, dx_RDQSND - mid_step);
        }

    }

   //  printf( "Reye scan Done\r\n");

}

void soft_weye_scanning() {

    //global logfile
    u32 loop_cnt;
    u32 regData;
    u8 n;
    u32 left_step;
    u32 right_step;
    u32 mid_step;

    u32 dx_MDL;
    u32 dx_WLSL;
    u32 dx_WLD;
    u32 dx_WDQD;
    u32 err_cnt = 0;


   //  printf(" \r\n");
   //  printf("+---------------------------------------+\r\n");
   //  printf("|    soft_weye_scanning begin           |\r\n");
   //  printf("+---------------------------------------+\r\n");

    //dr1x90_dram_write(0x100000 , 0x12345678);
   //  printf(" Start weye scan \r\n");
    left_step = 0;
    right_step = 0;
    mid_step = 0;
    err_cnt = 0;
    for (n = 0; n <= 3; n++) {
        dx_MDL = dr1x90_field_read(DDRC_ADDR_PPC + DX0MDLR0, IPRD_offset, IPRD_mask);
       //  printf("\neye scan : DX 0x%x  mdl =  0x%x\r\n", dx_MDL);

        dx_WLSL = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, WLSL_offset, WLSL_mask);
       //  printf("\nweye scan : DX 0x%x,  WLSL is 0x%x  \r\n", n, dx_WLSL);

        dx_WLD = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR0 + 0x100 * n, WLD_offset, WLD_mask);
       //  printf("\nweye scan : DX 0x%x,  WLD is 0x%x  \r\n", n, dx_WLD);

        dx_WDQD = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * n, WDQD_offset, WDQD_mask);
       //  printf("\nweye scan : DX 0x%x,  WDQD is 0x%x  \r\n", n, dx_WDQD);

        //dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * n, DGSL_offset, DGSL_mask,  );
        left_step = 0;
        err_cnt = 0;
        while (left_step <= dx_MDL) {
            //for (left_step = 0; left_step <= dx_MDL; left_step++)

            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * n, WDQD_offset, WDQD_mask, dx_WDQD - left_step);

           //  printf(" weye scan : left_step = 0x%x, write DRAM address \r\n", left_step);

            for (u8 i = 0; i < 10; i++) {

                //dr1x90_dram_write(0x100000 + i * 4, 0x12345678);
                u32 zq_val = get_rand();
                dr1x90_dram_write(0x100000 + i * 8, zq_val);
                regData = dr1x90_dram_read(0x100000 + i * 8);
                if ((regData & 0xffffffff) == (zq_val & 0xffffffff)) {
                    //printf(" pass No. 0x%08x\r\n", i);
                }
                else {
                   //  printf(" weye scan : No. 0x%08x, read Error : address 0x%08x =  0x%08x, expect = 0x%08x\r\n", i, (0x100000 + i * 8), regData, zq_val);
                    err_cnt = err_cnt + 1;
                    break;
                }
            }
            if (err_cnt > 0) {
                break;
            }
            else {
                left_step++;
            }
        }

        //dx_DQSGD = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * n, DQSGD_offset, DQSGD_mask);
       //  printf("\nweye scan : DX 0x%x, old dx_WDQD =  0x%x, left DQSGD =  0x%x \r\n", n, dx_WDQD, dx_WDQD - left_step);

        err_cnt = 0;
        right_step = 0;
        while (right_step <= dx_MDL) {
            //for (right_step = 0; right_step <= dx_MDL; right_step++)

            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * n, WDQD_offset, WDQD_mask, dx_WDQD + right_step);

           //  printf(" weye scan : left_step = 0x%x, write DRAM address \r\n", left_step);
           //  printf(" right_step = 0x%x, write DRAM address \r\n", right_step);


            for (u8 i = 0; i < 10; i++) {

                //dr1x90_dram_write(0x100000 + i * 4, 0x12345678);
                u32 zq_val = get_rand();
                dr1x90_dram_write(0x100000 + i * 8, zq_val);
                regData = dr1x90_dram_read(0x100000 + i * 8);
                if ((regData & 0xffffffff) == (zq_val & 0xffffffff)) {
                    //printf(" pass No. 0x%08x\r\n", i);
                }
                else {
                   //  printf(" No. 0x%08x, read Error : address 0x%08x =  0x%08x, expect = 0x%08x\r\n", i, (0x100000 + i * 8), regData, zq_val);
                    err_cnt = err_cnt + 1;
                    break;
                }
            }
            if (err_cnt > 0) {
                break;
            }
            else {
                right_step++;
            }
        }


       //  printf("\nweye scan : DX 0x%x, old dx_WDQD =  0x%x, right WDQD =  0x%x \r\n", n, dx_WDQD, dx_WDQD + right_step);

        if (right_step >= left_step) {
            mid_step = (right_step - left_step) / 2;
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * n, WDQD_offset, WDQD_mask, dx_WDQD + mid_step);
        }
        else {
            mid_step = (left_step - right_step) / 2;
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * n, WDQD_offset, WDQD_mask, dx_WDQD - mid_step);
        }

    }

   //  printf( "Reye scan Done\r\n");

}


