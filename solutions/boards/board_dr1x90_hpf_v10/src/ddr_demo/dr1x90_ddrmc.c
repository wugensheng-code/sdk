#include <math.h>
#include "dr1x90_ddrc_func.h"
#include "dr1x90_ddrc_init.h"

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

void dr1x90_ddrmc_cfg(const ddr_basic_t* basic_cfg, const ddr_timing_t* timpara, const ddr_addrmap_t* addrmap, const ddr_arbiter_t* arbiter_cfg)
{
    u32 regData;

    dr1x90_reg_write(0x1320, 0x00000000);  // SWCTL
    dr1x90_reg_write(0x1328 ,0x00000001); // SWCTLSTATIC
    // step 1.DDRMC configuration
    dr1x90_reg_write(0x11b0 ,0x00000051); // DFIMISC
    dr1x90_reg_write(0x1304 ,0x00000001); // DBG1
    dr1x90_reg_write(0x1030 ,0x00000001); // PWRCTL

    // regData = dr1x90_reg_read(DDRC_ADDR_UMCTL2 + MSTR);
    regData = 0x40040400;
    // regData = dr1x90_field_set(DDRC_ADDR_UMCTL2 + MSTR, device_config_offset, device_config_mask, 0x1, regData);
    if (basic_cfg->type == DDR4_TYPE)
        regData = dr1x90_field_set(DDRC_ADDR_UMCTL2 + MSTR, ddr4_offset, ddr4_mask, 0x1, regData);
    else
        regData = dr1x90_field_set(DDRC_ADDR_UMCTL2 + MSTR, ddr3_offset, ddr3_mask, 0x1, regData);
    
    if (basic_cfg->width == DDR_X8)
        regData = dr1x90_field_set(DDRC_ADDR_UMCTL2 + MSTR, data_bus_width_offset, data_bus_width_mask, 0x2, regData);
    else if (basic_cfg->width == DDR_X16)
        regData = dr1x90_field_set(DDRC_ADDR_UMCTL2 + MSTR, data_bus_width_offset, data_bus_width_mask, 0x1, regData);
    else    // DDR_X32
        regData = dr1x90_field_set(DDRC_ADDR_UMCTL2 + MSTR, data_bus_width_offset, data_bus_width_mask, 0x0, regData);
    
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + MSTR, regData); // MSTR
    /*
    if (ecc == DDR_ECC_SIDEBAND) {
        dr1x90_reg_write(0x1000 ,0x40041401); // MSTR
    } else {
        dr1x90_reg_write(0x1000 ,0x40040401); // MSTR
    }
    */

    dr1x90_reg_write(0x1010 ,0x40001010); // MRCTRL0
    dr1x90_reg_write(0x1014 ,0x00018954); // MRCTRL1
    dr1x90_reg_write(0x101c ,0xcb918919); // MRCTRL2

    // dr1x90_reg_write(0x1030 ,0x0000008a); // PWRCTL
    dr1x90_reg_write(0x1030 ,0x00000088);
    dr1x90_reg_write(0x1034 ,0x00080001); // PWRTMG
    dr1x90_reg_write(0x1038 ,0x00410002); // HWLPCTL
    dr1x90_reg_write(0x1050 ,0x00c17020); // RFSHCTL0
    dr1x90_reg_write(0x1060 ,0x00000000); // RFSHCTL3
    dr1x90_reg_write(0x1064 ,0x00610068); // RFSHTMG

//  dr1x90_reg_write(0x1070 ,0x40085894); // ECCCFG0
//  dr1x90_reg_write(0x1070 ,0x40085814); // ECCCFG0
    if(basic_cfg->ecc == DDR_ECC_SIDEBAND) {
        dr1x90_reg_write(0x1070 ,0x40085804); // ECCCFG0
        regData = 0x00000780;
        regData |= (basic_cfg->ecc_poison == ECC_POISON_NONE) ? 0x0 : 0x1;
        regData |= (basic_cfg->ecc_poison == ECC_POISON_1BIT) ? 0x2 : 0x0;
        dr1x90_reg_write(0x1074 ,regData);    // ECCCFG1
    } else {
        dr1x90_reg_write(0x1070 ,0x40085810); // ECCCFG0
        dr1x90_reg_write(0x1074 ,0x00000680); // ECCCFG1
    }

    dr1x90_reg_write(0x107c ,0x00000300); // ECCCTL
    dr1x90_reg_write(0x10b8 ,0x00000bd3); // ECCPOISONADDR0
    dr1x90_reg_write(0x10bc ,0x0003ffff); // ECCPOISONADDR1
    dr1x90_reg_write(0x10c0 ,0x00000000); // CRCPARCTL0

    dr1x90_ddrmc_init_cfg(basic_cfg->fck, basic_cfg->type, timpara);

    dr1x90_reg_write(0x10f0 ,0x00000010); // DIMMCTL

    dr1x90_reg_write(0x1180 ,0x01000040); // ZQCTL0
    dr1x90_reg_write(0x1184 ,0x00000070); // ZQCTL1

    dr1x90_ddrmc_timing_cfg(basic_cfg->fck, basic_cfg->type, timpara);

    dr1x90_ddrmc_addrmap_cfg(basic_cfg->type, basic_cfg->width, addrmap);

    dr1x90_reg_write(0x1240 ,0x06000608); // ODTCFG
    dr1x90_reg_write(0x1244 ,0x00000011); // ODTMAP
    // dr1x90_field_write(DDRC_ADDR_UMCTL2 + ODTMAP ,rank0_rd_odt_offset , rank0_rd_odt_mask ,0x0);

    dr1x90_ddrmc_arbiter_cfg(basic_cfg->type, arbiter_cfg);

    dr1x90_reg_write(0x1300 ,0x00000010); // DBG0
    dr1x90_reg_write(0x1304 ,0x00000000); // DBG1
    dr1x90_reg_write(0x130c ,0x00000000); // DBGCMD
    dr1x90_reg_write(0x1320 ,0x00000000); // SWCTL
    dr1x90_reg_write(0x1328 ,0x00000001); // SWCTLSTATIC
    dr1x90_reg_write(0x136c ,0x00110001); // POISONCFG
    dr1x90_reg_write(0x1374 ,0x0000010d); // ADVECCINDEX
    dr1x90_reg_write(0x137c ,0x00000000); // ECCPOISONPAT0
    dr1x90_reg_write(0x1384 ,0x00000000); // ECCPOISONPAT2
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
    dr1x90_field_wait(DDRC_ADDR_UMCTL2 + DFISTAT, dfi_init_complete_offset, dfi_init_complete_mask, 0x1, -1);

    //regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFISTAT, dfi_init_complete_offset, dfi_init_complete_mask);
   //  printf("\n dfi_init_complete.   data =  0x%x\r\n", regData);

    // step 8
    // cfg SWCTL.sw_done = 1'b1
    dr1x90_reg_write(0x1320, 0x00000001);  // SWCTL

    // step 9
    // wait SWSTAT.sw_done_ack
    dr1x90_field_wait(DDRC_ADDR_UMCTL2 + SWSTAT, sw_done_ack_offset, sw_done_ack_mask, 0x1, -1);

    // step 10
    // wait STAT.operating_mode = "normal" 2'b01
    dr1x90_field_wait(DDRC_ADDR_UMCTL2 + STAT, operating_mode_offset, operating_mode_mask, 0x1, -1);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + STAT, operating_mode_offset, operating_mode_mask);
    (void) regData;

    //printf("\n operating_mode.   data =  0x%x\r\n", regData);

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
    // dr1x90_reg_write(0x1030, 0x0000008a);  // PWRCTL
    dr1x90_reg_write(0x1490, 0x00000001);  // PCTRL_0
    dr1x90_reg_write(0x1540, 0x00000001);  // PCTRL_1
    dr1x90_reg_write(0x15f0, 0x00000001);  // PCTRL_2
    dr1x90_reg_write(0x16a0, 0x00000001);  // PCTRL_3
    dr1x90_reg_write(0x1304, 0x00000000);  // DBG1
}

void dr1x90_ddrmc_init_cfg(double fck, ddr_type_t type, const ddr_timing_t* timpara)
{
    u32 MR[7] = {0};
    make_ddr_mr(fck, type, timpara, MR);
    // u32 MR[7] = {MR0, MR1, MR2, MR3, MR4, MR5, MR6};

    dr1x90_reg_write(0x10d0 ,0x40020003); // INIT0
    dr1x90_reg_write(0x10d4 ,0x00030009); // INIT1
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT0, pre_cke_x1024_offset, pre_cke_x1024_mask, 500);
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT0, post_cke_x1024_offset, post_cke_x1024_mask, 15);
    // dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT0, skip_dram_init_offset, skip_dram_init_mask, 0);
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT0, skip_dram_init_offset, skip_dram_init_mask, 1);
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT1, dram_rstn_x1024_offset, dram_rstn_x1024_mask, 200);

    // set MR , CL = 9 , WL = 7
    // dr1x90_reg_write(0x10dc ,0x02500001); // INIT3
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT3, mr_offset,   mr_mask,   MR[0]);
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT3, emr_offset,  emr_mask,  MR[1]);
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT4, emr2_offset, emr2_mask, MR[2]);
    dr1x90_field_write(DDRC_ADDR_UMCTL2 + INIT4, emr3_offset, emr3_mask, MR[3]);
    //dr1x90_reg_write(0x10e0 ,0x00000000); // INIT4
    dr1x90_reg_write(0x10e4 ,0x00110000); // INIT5
    dr1x90_reg_write(0x10e8 ,0x02001400); // INIT6
    dr1x90_reg_write(0x10ec ,0x00000453); // INIT7
}

static u32 idiv2_ceil(u32 x)
{
    return (x + 1) / 2;
}

static u32 idiv2_floor(u32 x)
{
    return x / 2;
}

static u32 fdiv2_ceil(double x)
{
    return (u32)ceil(x / 2.0);
}

static u32 fdiv2_floor(double x)
{
    return (u32)floor(x / 2.0);
}

static u32 make_field(u32 val, u32 offset, u32 mask)
{
    return (val << offset) & mask;
}

void dr1x90_ddrmc_timing_cfg(double fck, ddr_type_t type, const ddr_timing_t* timpara)
{

    const double tck = 1e3 / fck;
    double fval = 0.0;
    u32 ival = 0;
    u32 regData = 0;

    // wr2pre
    ival = timpara->nCWL + 4 + get_nwr(fck, type);   // WL + BL/2 + nWR
    ival = idiv2_floor(ival);                        // divide 2, no rounding up
    regData |= make_field(ival, wr2pre_offset, wr2pre_mask);
    // faw
    fval = timpara->tFAW;
    ival = fdiv2_ceil(fval / tck);
    regData |= make_field(ival, t_faw_offset, t_faw_mask);
    // t_raw_max
    fval = timpara->tREFI * 9;
    fval = floor(fval / tck / 1024);
    ival = fdiv2_floor(fval - 1.0);
    regData |= make_field(ival, t_ras_max_offset, t_ras_max_mask);
    // t_raw_min
    fval = timpara->tRAS;
    fval = ceil(fval / tck);
    ival = fdiv2_floor(fval);
    regData |= make_field(ival, t_ras_min_offset, t_ras_min_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG0, regData);

    regData = 0;
    // t_xp
    regData |= make_field((u32)0x3, t_xp_offset, t_xp_mask);
    // rd2pre
    ival = max(4, (u32)ceil(timpara->tRTP / tck)); // tRTP = max(4ck, 7.5ns)
    if (type == DDR4_TYPE)
        ival = max(ival, timpara->nCL + 4 - timpara->nRP);
    ival = idiv2_floor(ival);
    regData |= make_field(ival, rd2pre_offset, rd2pre_mask);
    // t_rc
    fval = timpara->tRC;
    ival = fdiv2_ceil(fval / tck);
    regData |= make_field(ival, t_rc_offset, t_rc_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG1, regData);

    regData = 0;
    // write_latency
    ival = idiv2_ceil(timpara->nCWL);
    regData |= make_field(ival, write_latency_offset, write_latency_mask);
    // read_latency
    ival = idiv2_ceil(timpara->nCL);
    regData |= make_field(ival, read_latency_offset, read_latency_mask);
    // rd2wr
    ival = timpara->nCL + 4 + 2 - timpara->nCWL; // RL + BL/2 + 2 - WL
    ival = idiv2_ceil(ival);
    regData |= make_field(ival, rd2wr_offset, rd2wr_mask);
    // wr2rd
    ival = timpara->nCWL + 4 + max(4, (u32)ceil(timpara->tWTR_L / tck)); // tWTR = max(4ck, 7.5ns)
    ival = idiv2_ceil(ival);
    regData |= make_field(ival, wr2rd_offset, wr2rd_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG2, regData);

    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG3, 0x0000400c);

    regData = 0;
    // t_rcd
    ival = idiv2_ceil(timpara->nRCD);
    regData |= make_field(ival, t_rcd_offset, t_rcd_mask);
    // t_ccd
    ival = (u32)ceil(timpara->tCCD_L / tck);
    ival = max(ival, 4);
    ival = idiv2_ceil(ival);
    regData |= make_field(ival, t_ccd_offset, t_ccd_mask);
    // t_rrd
    ival = (u32)ceil(timpara->tRRD_L / tck);
    ival = max(ival, 4);
    ival = idiv2_ceil(ival);
    regData |= make_field(ival, t_rrd_offset, t_rrd_mask);
    // t_rp
    ival = idiv2_floor(timpara->nRP) + 1;
    regData |= make_field(ival, t_rp_offset, t_rp_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG4, regData);

    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG5, 0x05e20302); // DRAMTMG5
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG8, 0x05050b0b); // DRAMTMG8

    if (type == DDR4_TYPE) {
        regData = 0;
        // ddr4_wr_preamble
        regData |= make_field(0, ddr4_wr_preamble_offset, ddr4_wr_preamble_mask);   // 1 tCK preamble
        // t_ccd_s
        ival = idiv2_ceil(4);
        regData |= make_field(ival, t_ccd_s_offset, t_ccd_s_mask);
        // t_rrd_s
        ival = (u32)ceil(timpara->tRRD_S / tck);
        ival = max(ival, 4);
        ival = idiv2_ceil(ival);
        regData |= make_field(ival, t_rrd_s_offset, t_rrd_s_mask);
        // wr2rd_s
        ival = timpara->nCWL + 4 + max(2, (u32)ceil(timpara->tWTR_S / tck)); // tWTR_S = max(2ck, 2.5ns)
        ival = idiv2_ceil(ival);
        regData |= make_field(ival, wr2rd_s_offset, wr2rd_s_mask);
        dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG9, regData);
    }

    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG10, 0x000f0e0d); // DRAMTMG10
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG11, 0x0e060119); // DRAMTMG11
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG12, 0x0c000008); // DRAMTMG12
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DRAMTMG15, 0x00000000); // DRAMTMG15

    regData = 0;
    // t_rfc_nom_x1_x32
    fval = floor(timpara->tREFI / tck);
    ival = fdiv2_floor(fval);
    ival = ival / 32;
    regData |= make_field(ival, t_rfc_nom_x1_x32_offset, t_rfc_nom_x1_x32_mask);
    // t_rfc_min
    fval = ceil(timpara->tRFC / tck);
    ival = fdiv2_ceil(fval);
    regData |= make_field(ival, t_rfc_min_offset, t_rfc_min_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + RFSHTMG, regData);

    regData = 0;
    // dfi_t_ctrl_delay
    regData |= make_field(2, dfi_t_ctrl_delay_offset, dfi_t_ctrl_delay_mask);
    // dfi_rddata_use_dfi_phy_clk
    regData |= make_field(0x1, dfi_rddata_use_dfi_phy_clk_offset, dfi_rddata_use_dfi_phy_clk_mask);
    // dfi_t_rddata_en
    ival = timpara->nCL - 4;
    regData |= make_field(ival, dfi_t_rddata_en_offset, dfi_t_rddata_en_mask);
    // dfi_wrdata_use_dfi_phy_clk
    regData |= make_field(0x1, dfi_wrdata_use_dfi_phy_clk_offset, dfi_wrdata_use_dfi_phy_clk_mask);
    // dfi_tphy_wrdata
    ival = 2;
    regData |= make_field(ival, dfi_tphy_wrdata_offset, dfi_tphy_wrdata_mask);
    // dfi_tphy_wrlat
    ival = timpara->nCWL - 2;
    regData |= make_field(ival, dfi_tphy_wrlat_offset, dfi_tphy_wrlat_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DFITMG0, regData);

    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DFITMG1   , 0x03070101); // DFITMG1
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DFILPCFG0 , 0x07007031); // DFILPCFG0
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DFILPCFG1 , 0x000000f1); // DFILPCFG1
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DFIUPD0   , 0x40400002); // DFIUPD0
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DFIUPD1   , 0x003300e6); // DFIUPD1
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DFIUPD2   , 0x80000000); // DFIUPD2
//  dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DFIMISC   , 0x00000051); // DFIMISC
//  dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DFIMISC   , 0x00000050); // DFIMISC
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DFITMG3   , 0x0000001e); // DFITMG3
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DBICTL    , 0x00000005); // DBICTL
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + DFIPHYMSTR, 0x2a000000); // DFIPHYMSTR
}

static u32 make_addrmap_field(u8 axaddr_bit, u32 base, u32 offset, u32 mask)
{
    u32 val = (u32)-1;
    if (axaddr_bit != (u8)-1)
        val = (u32)axaddr_bit - base - 2;
    return (val << offset) & mask;
}

void dr1x90_ddrmc_addrmap_cfg(ddr_type_t type, ddr_width_t width, const ddr_addrmap_t* addrmap)
{
    u32 col_shift = 0;  // DDR_X32
    if (width == DDR_X16)
        col_shift = 1;
    if (width == DDR_X8)
        col_shift = 2;
    
    u32 regData = 0;
    regData |= make_addrmap_field(addrmap->ba[0], 2, addrmap_bank_b0_offset, addrmap_bank_b0_mask);
    regData |= make_addrmap_field(addrmap->ba[1], 3, addrmap_bank_b1_offset, addrmap_bank_b1_mask);
    regData |= make_addrmap_field(addrmap->ba[2], 4, addrmap_bank_b2_offset, addrmap_bank_b2_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + ADDRMAP1, regData);

    regData = 0;
    regData |= make_addrmap_field(addrmap->col[2 + col_shift], 2, addrmap_col_b2_offset, addrmap_col_b2_mask);
    regData |= make_addrmap_field(addrmap->col[3 + col_shift], 3, addrmap_col_b3_offset, addrmap_col_b3_mask);
    regData |= make_addrmap_field(addrmap->col[4 + col_shift], 4, addrmap_col_b4_offset, addrmap_col_b4_mask);
    regData |= make_addrmap_field(addrmap->col[5 + col_shift], 5, addrmap_col_b5_offset, addrmap_col_b5_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + ADDRMAP2, regData);

    regData = 0;
    regData |= make_addrmap_field(addrmap->col[6 + col_shift], 6, addrmap_col_b6_offset, addrmap_col_b6_mask);
    regData |= make_addrmap_field(addrmap->col[7 + col_shift], 7, addrmap_col_b7_offset, addrmap_col_b7_mask);
    regData |= make_addrmap_field(col_shift > 1 ? (u8)-1 : addrmap->col[8 + col_shift], 8, addrmap_col_b8_offset, addrmap_col_b8_mask);
    regData |= make_addrmap_field(col_shift > 0 ? (u8)-1 : addrmap->col[9 + col_shift], 9, addrmap_col_b9_offset, addrmap_col_b9_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + ADDRMAP3, regData);

    regData = 0;
    regData |= make_addrmap_field((u8)-1, 10, addrmap_col_b10_offset, addrmap_col_b10_mask);
    regData |= make_addrmap_field((u8)-1, 11, addrmap_col_b11_offset, addrmap_col_b11_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + ADDRMAP4, regData);

    regData = 0;
    regData |= make_addrmap_field(addrmap->row[0] , 6 , addrmap_row_b0_offset, addrmap_row_b0_mask);
    regData |= make_addrmap_field(addrmap->row[1] , 7 , addrmap_row_b1_offset, addrmap_row_b1_mask);
    regData |= make_addrmap_field(addrmap->row[2] , 8 , addrmap_row_b2_10_offset, addrmap_row_b2_10_mask);  // Assume row[2 - 10] increase with step 1
    regData |= make_addrmap_field(addrmap->row[11], 17, addrmap_row_b11_offset, addrmap_row_b11_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + ADDRMAP5, regData);

    regData = 0;
    regData |= make_addrmap_field(addrmap->row[12], 18, addrmap_row_b12_offset, addrmap_row_b12_mask);
    regData |= make_addrmap_field(addrmap->row[13], 19, addrmap_row_b13_offset, addrmap_row_b13_mask);
    regData |= make_addrmap_field(addrmap->row[14], 20, addrmap_row_b14_offset, addrmap_row_b14_mask);
    regData |= make_addrmap_field(addrmap->row[15], 21, addrmap_row_b15_offset, addrmap_row_b15_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + ADDRMAP6, regData);

    regData = 0;
    regData |= make_addrmap_field(addrmap->row[16], 22, addrmap_row_b16_offset, addrmap_row_b16_mask);
    regData |= make_addrmap_field(addrmap->row[17], 23, addrmap_row_b17_offset, addrmap_row_b17_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + ADDRMAP7, regData);

    regData = 0;
    regData |= make_addrmap_field(addrmap->bg[0], 2, addrmap_bg_b0_offset, addrmap_bg_b0_mask);
    regData |= make_addrmap_field(addrmap->bg[1], 3, addrmap_bg_b1_offset, addrmap_bg_b1_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + ADDRMAP8, regData);
}

void dr1x90_ddrmc_arbiter_cfg(ddr_type_t type, const ddr_arbiter_t* arbiter_cfg)
{
    u32 regData = 0x1;
    // rdwr_idle_gap = 0, autopre_rmw = 0, pageclose = 0, prefer_write = 0
    regData |= make_field(arbiter_cfg->lpr_num, lpr_num_entries_offset, lpr_num_entries_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + SCHED, regData);

    // pageclose_timer = 0
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + SCHED1, 0x0);

    regData = 0;
    regData |= make_field(arbiter_cfg->hpr_xact_run, hpr_xact_run_length_offset, hpr_xact_run_length_mask);
    regData |= make_field(arbiter_cfg->hpr_max_starve, hpr_max_starve_offset, hpr_max_starve_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + PERFHPR1, regData);
    regData = 0;
    regData |= make_field(arbiter_cfg->lpr_xact_run, lpr_xact_run_length_offset, lpr_xact_run_length_mask);
    regData |= make_field(arbiter_cfg->lpr_max_starve, lpr_max_starve_offset, lpr_max_starve_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + PERFLPR1, regData);
    regData = 0;
    regData |= make_field(arbiter_cfg->w_xact_run, w_xact_run_length_offset, w_xact_run_length_mask);
    regData |= make_field(arbiter_cfg->w_max_starve, w_max_starve_offset, w_max_starve_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + PERFWR1, regData);

    regData = 0;
    regData |= make_field(0x1, bl_exp_mode_offset, bl_exp_mode_mask);
    regData |= make_field(0x0, pagematch_limit_offset, pagematch_limit_mask);
    regData |= make_field(0x0, go2critical_en_offset, go2critical_en_mask);
    dr1x90_reg_write(DDRC_ADDR_UMCTL2 + PCCFG, regData);

    for (int i = 0; i < 4; ++i) {
        regData = 0;
        regData |= make_field(type == DDR4_TYPE ? 0x0 : 0x1, rd_port_pagematch_en_offset, rd_port_pagematch_en_mask);
        regData |= make_field(0x1, rd_port_urgent_en_offset, rd_port_urgent_en_mask);
        if (arbiter_cfg->port[i].rd_aging != (u16)-1) {
            regData |= make_field(0x1, rd_port_aging_en_offset, rd_port_aging_en_mask);
            regData |= make_field(arbiter_cfg->port[i].rd_aging, rd_port_priority_offset, rd_port_priority_mask);
        }
        dr1x90_reg_write(DDRC_ADDR_UMCTL2 + PCFGR_0 + 0xb0 * i, regData);

        regData = 0;
        regData |= make_field(type == DDR4_TYPE ? 0x0 : 0x1, wr_port_pagematch_en_offset, rd_port_pagematch_en_mask);
        regData |= make_field(0x1, wr_port_urgent_en_offset, wr_port_urgent_en_mask);
        if (arbiter_cfg->port[i].wr_aging != (u16)-1) {
            regData |= make_field(0x1, wr_port_aging_en_offset, wr_port_aging_en_mask);
            regData |= make_field(arbiter_cfg->port[i].wr_aging, wr_port_priority_offset, wr_port_priority_mask);
        }
        dr1x90_reg_write(DDRC_ADDR_UMCTL2 + PCFGW_0 + 0xb0 * i, regData);

        // port_en = 0x1
        dr1x90_reg_write(DDRC_ADDR_UMCTL2 + PCTRL_0 + 0xb0 * i, 0x1);

        regData = 0;
        regData |= make_field(arbiter_cfg->port[i].rqos_level1, rqos_map_level1_offset, rqos_map_level1_mask);
        regData |= make_field(arbiter_cfg->port[i].rqos_region0, rqos_map_region0_offset, rqos_map_region0_mask);
        regData |= make_field(arbiter_cfg->port[i].rqos_region1, rqos_map_region1_offset, rqos_map_region1_mask);
        dr1x90_reg_write(DDRC_ADDR_UMCTL2 + PCFGQOS0_0 + 0xb0 * i, regData);

        regData = 0;
        regData |= make_field(arbiter_cfg->port[i].vpr_timeout, rqos_map_timeoutb_offset, rqos_map_timeoutb_mask);
        regData |= make_field(arbiter_cfg->port[i].vpr_timeout, rqos_map_timeoutr_offset, rqos_map_timeoutr_mask);
        dr1x90_reg_write(DDRC_ADDR_UMCTL2 + PCFGQOS1_0 + 0xb0 * i, regData);

        regData = 0;
        regData |= make_field(arbiter_cfg->port[i].wqos_level1, wqos_map_level1_offset, wqos_map_level1_mask);
        regData |= make_field(arbiter_cfg->port[i].wqos_level2, wqos_map_level2_offset, wqos_map_level2_mask);
        regData |= make_field(arbiter_cfg->port[i].wqos_region0, wqos_map_region0_offset, wqos_map_region0_mask);
        regData |= make_field(arbiter_cfg->port[i].wqos_region1, wqos_map_region1_offset, wqos_map_region1_mask);
        regData |= make_field(arbiter_cfg->port[i].wqos_region2, wqos_map_region2_offset, wqos_map_region2_mask);
        dr1x90_reg_write(DDRC_ADDR_UMCTL2 + PCFGWQOS0_0 + 0xb0 * i, regData);

        regData = 0;
        regData |= make_field(arbiter_cfg->port[i].vpw_timeout1, wqos_map_timeout1_offset, wqos_map_timeout1_mask);
        regData |= make_field(arbiter_cfg->port[i].vpw_timeout2, wqos_map_timeout2_offset, wqos_map_timeout2_mask);
        dr1x90_reg_write(DDRC_ADDR_UMCTL2 + PCFGWQOS1_0 + 0xb0 * i, regData);
    }
}
