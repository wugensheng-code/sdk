#include <string.h>
#include "dr1x90_ddrc_init.h"

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

int dr1x90_ddrc_init(double fck, ddr_type_t type, ddr_width_t width, ddr_ecc_t ecc, const ddr_timing_t* timpara, const ddr_addrmap_t* addrmap, const ddr_arbiter_t* arbiter_cfg)
{
    u32 regData = 0;
    u32 mtest_err = 0;

    regData = dr1x90_reg_read(0x27a0);

    AL_DDR_LOG("DDR Init V3\r\n");

    dr1x90_reg_write(0x11b0 ,0x00000050); // DFIMISC
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 01 : GPLL Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddr_gpll_cfg(fck);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 02 : DPLL Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddr_dpll_cfg();
    // release DDRC_SRST_N
    dr1x90_reg_write(DDRC_ADDR_GPLL + SRST_CTRL, 1);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 03 : IOB Configuraiton
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddr_iob_cfg(type);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 04 : IOL Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddr_iol_cfg();
    dr1x90_reg_write(DDRC_ADDR_DPLL + RW_TEST, 0x1); // used as reset
    dr1x90_reg_write(0x11b0 ,0x00000050); // DFIMISC

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 05 : Bus Matrix Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddr_busmatrix_cfg(type);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 06 : PPC Base Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddrppc_base_cfg(type, width, ecc);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 07 : MDL Calibration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddrppc_mdl_cal();
    // dr1x90_ddrppc_zq_cal(type);
    // dr1x90_ddrc_train_dcc();

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 08 : Fast Init
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // dr1x90_ddrppc_fast_init(fck);
    dr1x90_zq_overwrite_cfg(type);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 08 : DRAM INIT
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddrppc_set_timing();
    dr1x90_ddrppc_set_ddrmr(fck, type, timpara);
    dr1x90_ddrppc_dram_init();

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 09 : PPC MTEST
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_pub_training_cfg(type);
    // dr1x90_do_training(1, 1, 1);
    // dr1x90_ddrppc_show_delay();

    dr1x90_ddrc_train_wl();
    // dr1x90_ddrppc_show_delay();

    dr1x90_ddrc_train_gate();
    // dr1x90_ddrppc_show_delay();

    dr1x90_ddrc_train_wladj();
    dr1x90_ddrppc_show_delay();

    dr1x90_ddrc_train_eye();
    dr1x90_ddrppc_show_delay();

    // dr1x90_iomc_internal_loopback_cfg();

    mtest_err = dr1x90_ddrppc_mtest(0, 0, 0, 9);

    // while (1);

    if (ecc == DDR_ECC_SIDEBAND)
        dr1x90_reg_write(DDRC_ADDR_GPLL + FUNCTRL, 0x3);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 10 : MC Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    dr1x90_ddrmc_cfg(fck, type, width, ecc, timpara, addrmap, arbiter_cfg);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 11 : HandOff to DFI
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x0A, 0xffff);
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR1, PUBMODE_offset, PUBMODE_mask, 0);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // step 100 : MC Post Configuration
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Alc_GpioMaskWrite(GPIO_CH0, 0x333, 0xffff);
    dr1x90_release_ddr_bus();
    dr1x90_ddrmc_post_cfg();

    regData = dr1x90_field_read(DDRC_ADDR_PPC + PGCR1, PUBMODE_offset, PUBMODE_mask);
   //  AL_DDR_LOG("\n initial PUBMODE.   data =  0x%x\n", regData);


    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_hpr_q_depth_offset, dbg_hpr_q_depth_mask);
   //  AL_DDR_LOG("\n MC initial dbg_hpr_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_lpr_q_depth_offset, dbg_lpr_q_depth_mask);
   //  AL_DDR_LOG("\n MC initial dbg_lpr_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_w_q_depth_offset, dbg_w_q_depth_mask);
   //  AL_DDR_LOG("\n MC initial dbg_w_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_wr_q_empty_offset, dbg_wr_q_empty_mask);
   //  AL_DDR_LOG("\n MC initial dbg_wr_q_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_rd_q_empty_offset, dbg_rd_q_empty_mask);
   //  AL_DDR_LOG("\n MC initial dbg_rd_q_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, rd_data_pipeline_empty_offset, rd_data_pipeline_empty_mask);
   //  AL_DDR_LOG("\n MC initial rd_data_pipeline_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, wr_data_pipeline_empty_offset, wr_data_pipeline_empty_mask);
   //  AL_DDR_LOG("\n MC initial wr_data_pipeline_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_stall_wr_offset, dbg_stall_wr_mask);
   //  AL_DDR_LOG("\n MC initial dbg_stall_wr.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_stall_rd_offset, dbg_stall_rd_mask);
   //  AL_DDR_LOG("\n MC initial dbg_stall_rd.   data =  0x%x\n", regData);


    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + INIT3, mr_offset, mr_mask);
   //  AL_DDR_LOG("\n MR0.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + INIT3, emr_offset, emr_mask);
   //  AL_DDR_LOG("\n MR1.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + INIT4, emr2_offset, emr2_mask);
   //  AL_DDR_LOG("\n MR2.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + INIT4, emr3_offset, emr3_mask);
   //  AL_DDR_LOG("\n MR3.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DRAMTMG2, write_latency_offset, write_latency_mask);
   //  AL_DDR_LOG("\n write_latency.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DRAMTMG2, read_latency_offset, read_latency_mask);
   //  AL_DDR_LOG("\n read_latency.   data =  0x%x\n", regData);


    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFITMG0, dfi_tphy_wrlat_offset, dfi_tphy_wrlat_mask);
   //  AL_DDR_LOG("\n dfi_tphy_wrlat.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFITMG0, dfi_t_rddata_en_offset, dfi_t_rddata_en_mask);
   //  AL_DDR_LOG("\n dfi_t_rddata_en.   data =  0x%x\n", regData);

    // soft_reye_scanning_new();
    // soft_weye_scanning();
    // dr1x90_ddrppc_show_delay();

    if (ecc != DDR_ECC_NONE) {
        memset((void*)0x0, 0x0, 512 * 1024 * 1024);
    }

    set_rand_seed(100);

    // AL_DDR_LOG(" write DRAM address start test \r\n");
    for (u8 i = 0; i < 10; i++) {
        //dr1x90_dram_write(0x100000 + i * 4, 0x12345678);
        u32 zq_val = get_rand();
        dr1x90_dram_write(0x100000 + i * 8, zq_val);
        regData = dr1x90_dram_read(0x100000 + i * 8);
        if ((regData & 0xffffffff) == (zq_val & 0xffffffff)) {
            //AL_DDR_LOG(" pass No. 0x%08x\n", i);
        }
        else {
            ++mtest_err;
            AL_DDR_LOG(" No. 0x%08x, read Error : address 0x%08x =  0x%08x, expect = 0x%08x\r\n", i, (0x100000 + i * 8), regData, zq_val);
        }
    }
    // AL_DDR_LOG(" write DRAM address test done \r\n");

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_hpr_q_depth_offset, dbg_hpr_q_depth_mask);
   //  AL_DDR_LOG("\n MC test dbg_hpr_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_lpr_q_depth_offset, dbg_lpr_q_depth_mask);
   //  AL_DDR_LOG("\n MC test dbg_lpr_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_w_q_depth_offset, dbg_w_q_depth_mask);
   //  AL_DDR_LOG("\n MC test dbg_w_q_depth.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_wr_q_empty_offset, dbg_wr_q_empty_mask);
   //  AL_DDR_LOG("\n MC test dbg_wr_q_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_rd_q_empty_offset, dbg_rd_q_empty_mask);
   //  AL_DDR_LOG("\n MC test dbg_rd_q_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, rd_data_pipeline_empty_offset, rd_data_pipeline_empty_mask);
   //  AL_DDR_LOG("\n MC test rd_data_pipeline_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, wr_data_pipeline_empty_offset, wr_data_pipeline_empty_mask);
   //  AL_DDR_LOG("\n MC test wr_data_pipeline_empty.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_stall_wr_offset, dbg_stall_wr_mask);
   //  AL_DDR_LOG("\n MC test dbg_stall_wr.   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DBGCAM, dbg_stall_rd_offset, dbg_stall_rd_mask);
   //  AL_DDR_LOG("\n MC test dbg_stall_rd.   data =  0x%x\n", regData);

    regData = dr1x90_dram_read(0xF84400AC);
   //  AL_DDR_LOG(" XMON Debug Timeout   data =  0x%x\n", regData);

    regData = dr1x90_dram_read(0xF8800000);
   //  AL_DDR_LOG(" chipID   data =  0x%x\n", regData);

    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DRAMTMG2, write_latency_offset, write_latency_mask);
   //  AL_DDR_LOG("\n write_latency.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DRAMTMG2, read_latency_offset, read_latency_mask);
   //  AL_DDR_LOG("\n read_latency.   data =  0x%x\n", regData);


    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFITMG0, dfi_tphy_wrlat_offset, dfi_tphy_wrlat_mask);
   //  AL_DDR_LOG("\n dfi_tphy_wrlat.   data =  0x%x\n", regData);
    regData = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFITMG0, dfi_t_rddata_en_offset, dfi_t_rddata_en_mask);
   //  AL_DDR_LOG("\n dfi_t_rddata_en.   data =  0x%x\n", regData);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Test Done
    ///////////////////////////////////////////////////////////////////////////////////////////////

    return mtest_err;
}
