#ifndef _AL_MMC_H_
#define _AL_MMC_H_

#include <stdio.h>
#include <stdint.h>
//#include "../../../../demoapu/Common/Include/delay.h"
#include "mtimer.h"
#define AL_DEBUG_PRINT
#ifdef AL_DEBUG_PRINT
#include "al_debug.h"
#endif

typedef enum{
	//	COMMON STATUS
	MMC_SUCCESS = 0,
	MMC_FAILURE,
	//TIMER OUT ERROR
	MMC_CMD_TIMEOUT,				//send cmd timeout
	MMC_XFER_TIMEOUT,				//wait transfer complete timeout
	MMC_WAIT_CLK_STABLE_TIMEOUT,	//wait internal clock stable timeout
	MMC_BUF_RDWR_RDY_TIMEOUT,			//wait buffer read ready complete timeout
	//MMC_SD_CHECK_VOLT_TIMEOUT,		//SD check voltage timeout
	//MMC_EMMC_CHECK_VOLT_TIMEOUT,	//eMMC check voltage timeout
	MMC_CHECK_VOLT_TIMEOUT,
	MMC_CHECK_DEV_STATUS_TIMEOUT,
	MMC_WAIT_LINE_INHIBIT_TIMEOUT,
	//Error INT Status Regitster
	MMC_CMD_TOUT_ERR,				//no response is returned within 64 clock cycles from end bit of the cmd
	MMC_CMD_CRC_ERR,				//cmd resp CRC err
									//host controller detects a CMD line conflict
	MMC_CMD_END_BIT_ERR,			//detect the end bit of a cmd resp is 0
	MMC_CMD_IDX_ERR,				//cmd index err in the cmd resp
	MMC_DATA_TOUT_ERR,				//busy timeout for R1b,R5b type
									//busy timeout after write CRC status
									//write CRC status timeout
									//Read Data timeout
	MMC_DATA_CRC_ERR,				//CRC err when transfer rd data use DAT line
									//write status have a value other than 010
									//write CRC status timeout
	MMC_DATA_END_BIT_ERR,			//detect 0 at the end bit pos of read data use DAT line
									//detect 0 at the end bit pos of the CRC status
	MMC_WRONG_FREQ,
	MMC_MODE_ERROR,
	MMC_CMD_0_ERR,
	MMC_CMD_1_ERR,
	MMC_CMD_2_ERR,
	MMC_CMD_3_ERR,
	MMC_CMD_6_ERR,
	MMC_CMD_7_ERR,
	MMC_CMD_8_ERR,
	MMC_CMD_9_ERR,
	MMC_CMD_10_ERR,
	MMC_CMD_16_ERR,
	MMC_CMD_17_ERR,
	MMC_CMD_23_ERR,
	MMC_CMD_24_ERR,
	MMC_CMD_55_ERR,
	MMC_ACMD_41_ERR,
	MMC_CMD_8_XFER_ERR,
	MMC_CMD_17_XFER_ERR,
	MMC_CMD_24_XFER_ERR,
	MMC_CMD_XFER_ERR,
	MMC_ERR_MAX
}MMC_ERR_TYPE;

//error code offset
#define MMC_ERROR_CODE_OFFSET			MMC_CMD_TOUT_ERR
//error cmd offset
#define MMC_ERROR_CMD_OFFSET			MMC_CMD_0_ERR
#define MMC_ERROR_CMD_NUM				(MMC_CMD_8_XFER_ERR - MMC_CMD_0_ERR)
//error transfer offset
#define MMC_ERROR_TRANSFER_CMD_OFFSET	MMC_CMD_8_XFER_ERR
#define MMC_ERROR_TRANSFER_CMD_NUM		(MMC_CMD_XFER_ERR - MMC_CMD_8_XFER_ERR)
//Error INT status bits	length 13~15 reserved
#define MMC_ERR_INT_STAT_BITS_LEN	7

#define BRANCH_ERROR_CMD_START						0
#define BRANCH_ERROR_CMD_CHECKOUT_START				(BRANCH_ERROR_CMD_START+MMC_ERROR_CMD_NUM)
#define BRANCH_ERROR_CMD8_ERROR_START				(BRANCH_ERROR_CMD_CHECKOUT_START+MMC_ERROR_CMD_NUM)
#define BRANCH_ERROR_CMDLINE_RESET_START			(BRANCH_ERROR_CMD8_ERROR_START+MMC_ERROR_CMD_NUM)
#define BRANCH_ERROR_CMDLINE_RESET_TIMEOUT_START	(BRANCH_ERROR_CMDLINE_RESET_START+MMC_ERROR_CMD_NUM)
#define BRANCH_ERROR_CMD_TIMEOUT_START				(BRANCH_ERROR_CMDLINE_RESET_TIMEOUT_START+MMC_ERROR_CMD_NUM)
#define BRANCH_ERROR_TRANSFER_START					(BRANCH_ERROR_CMD_TIMEOUT_START+MMC_ERROR_CMD_NUM)
#define BRANCH_ERROR_TRANSFER_CHECKOUT_START		(BRANCH_ERROR_TRANSFER_START+MMC_ERROR_TRANSFER_CMD_NUM)
#define BRANCH_ERROR_TRANSFER_TIMEOUT_START			(BRANCH_ERROR_TRANSFER_CHECKOUT_START+MMC_ERROR_TRANSFER_CMD_NUM)
#define BRANCH_TRANSFER_RIGHT_CMD_START				(BRANCH_ERROR_TRANSFER_TIMEOUT_START+MMC_ERROR_TRANSFER_CMD_NUM)
#define BRANCH_ERROR_RDWR_RDY_START					(BRANCH_TRANSFER_RIGHT_CMD_START+MMC_ERROR_TRANSFER_CMD_NUM)
#define BRANCH_ERROR_RDWR_RDY_CHECKOUT_START		(BRANCH_ERROR_RDWR_RDY_START+MMC_ERROR_TRANSFER_CMD_NUM)
#define BRANCH_ERROR_RDWR_RDY_TIMEOUT_START			(BRANCH_ERROR_RDWR_RDY_CHECKOUT_START+MMC_ERROR_TRANSFER_CMD_NUM)
#define BRANCH_CLK_STABLE							(BRANCH_ERROR_RDWR_RDY_TIMEOUT_START+MMC_ERROR_TRANSFER_CMD_NUM)
#define BRANCH_CLK_STABLE_TIMEOUT					(BRANCH_CLK_STABLE+1)
#define BRANCH_CHECK_LINE_INHIBIT					(BRANCH_CLK_STABLE_TIMEOUT+1)
#define BRANCH_CHECK_LINE_INHIBIT_TIMEOUT			(BRANCH_CHECK_LINE_INHIBIT+1)
#define BRANCH_IO_BANK1_SUPPORT						(BRANCH_CHECK_LINE_INHIBIT_TIMEOUT+1)
#define BRANCH_EMMC_FREQ_SET						(BRANCH_IO_BANK1_SUPPORT+1)
#define BRANCH_SD_FREQ_SET							(BRANCH_EMMC_FREQ_SET+1)
#define BRANCH_EMMC_VOLT_CHECK						(BRANCH_SD_FREQ_SET+1)
#define BRANCH_EMMC_VOLT_CHECK_TIMEOUT				(BRANCH_EMMC_VOLT_CHECK+1)
#define BRANCH_SD_VOLT_CHECK						(BRANCH_EMMC_VOLT_CHECK_TIMEOUT+1)
#define BRANCH_SD_VOLT_CHECK_TIMEOUT				(BRANCH_SD_VOLT_CHECK+1)
#define BRANCH_SD_CARD_TYPE_CHECK					(BRANCH_SD_VOLT_CHECK_TIMEOUT+1)
#define BRANCH_EMMC_CARD_SIZE						(BRANCH_SD_CARD_TYPE_CHECK+1)
#define BRANCH_SD_CARD_SIZE							(BRANCH_EMMC_CARD_SIZE+1)
#define BRANCH_RAW_EMMC_INIT						(BRANCH_SD_CARD_SIZE+1)
#define BRANCH_RAW_EMMC_READ_FIRST					(BRANCH_RAW_EMMC_INIT+1)
#define BRANCH_RAW_EMMC_READ_MIDDLE					(BRANCH_RAW_EMMC_READ_FIRST+1)
#define BRANCH_RAW_EMMC_READ_LAST					(BRANCH_RAW_EMMC_READ_MIDDLE+1)
#define BRANCH_RAW_EMMC_SET_MODE_FREQ				(BRANCH_RAW_EMMC_READ_LAST+1)
#define BRANCH_RAW_SD_SET_MODE_FREQ					(BRANCH_RAW_EMMC_SET_MODE_FREQ+1)
#define BRANCH_EMMC_HOST_CONTROLLER_SETUP			(BRANCH_RAW_SD_SET_MODE_FREQ+1)
#define BRANCH_EMMC_INTR_INIT						(BRANCH_EMMC_HOST_CONTROLLER_SETUP+1)
#define BRANCH_EMMC_INIT_CMD						(BRANCH_EMMC_INTR_INIT+1)
#define BRANCH_EMMC_GET_CARD_INFO					(BRANCH_EMMC_INIT_CMD+1)
#define BRANCH_SD_HOST_CONTROLLER_SETUP				(BRANCH_EMMC_GET_CARD_INFO+1)
#define BRANCH_SD_INTR_INIT							(BRANCH_SD_HOST_CONTROLLER_SETUP+1)
#define BRANCH_SD_INIT_CMD							(BRANCH_SD_INTR_INIT+1)
#define BRANCH_SD_GET_CARD_INFO						(BRANCH_SD_INIT_CMD+1)
#define BRANCH_MAX									(BRANCH_SD_GET_CARD_INFO+1)



//#define _USE_SDMA
//#define _USE_4BIT
//#define _USE_8BIT
#ifdef AL_DEBUG_PRINT
#define BRANCH_SD_FLOW_PRINT
//#define BRANCH_EMMC_FLOW_PRINT
#define _USE_MSHC_PRINT
//#define _USE_ERR_PRINT
#define MMC_BRANCHTEST
#define MMC_INFO_PRINT
#define MMC_GENERAL_PRINT

#ifdef MMC_INFO_PRINT
#define MMC_PRINT(...)	rom_printf(DEBUG_INFO, __VA_ARGS__)
#else
#define MMC_PRINT(...)
#endif

#ifdef MMC_GENERAL_PRINT
#define MMC_GPRINT(...)	rom_printf(DEBUG_GENERAL, __VA_ARGS__)
#else
#define MMC_GPRINT(...)
#endif
#else
#define MMC_PRINT(...)
#define MMC_GPRINT(...)
#endif
//IO BANK1 REF
//bit 31-3:reserved
//bit 2:bank1_vccio_det3v3
//bit 1:bank1_vccio_det2v5
//bit 0:bank1_vccio_det1v8
#define IO_BANK1_REF                0xF8803C04ULL

#define MBIU_CTRL_R 0x510   //AHB BUS burst contrl register
//top cfg register
//bit 7:enable reg ctrl card write protection   0:io ctrl   1:reg ctrl
//bit 6:enable reg ctrl card detection          0:io ctrl   1:reg ctrl
//bit 5:reg ctrl write protection               0:disable   1:enable
//bit 4:reg ctrl card detect                    0:enable    1:disable
//bit 3:clk soft rst                            0:disable   1:enable
//bit [2:0]:clk phase select similar to tuning
#define TOP_NS__CFG_CTRL_SDIO0_ADDR 0xF8800150ULL   //sd0   emmc
#define TOP_NS__CFG_CTRL_SDIO1_ADDR 0xF8800150ULL	//0xF8800154ULL   //sd1   sd
#define SDIO_WRAP__SDIO0__BASE_ADDR 0xF8049000ULL
#define SDIO_WRAP__SDIO1__BASE_ADDR 0xF8049000ULL	//0xF804A000ULL

#define MMC_CMD_TIMEOUT_VAL					(10000*1000)	//10s
#define MMC_CMDLINE_RESET_TIMEOUT_VAL		(10000*1000)	//10s
#define MMC_XFER_TIMEOUT_VAL				(15000*1000)	//15s
#define MMC_BUF_RD_RDY_TIMEOUT_VAL			(1000*1000)		//1s
#define MMC_WAIT_CLK_STABLE_TIMEOUT_VAL		(5000*1000)		//5s
#define MMC_CHECK_LINE_INHIBIT_TIMEOUT_VAL	(1000*1000)		//1s
#define MMC_CHECK_DEV_STATUS_TIMEOUT_VAL	(1000*1000)		//1s
#define MMC_DELAY_SCALE						(2)

//capabilities1.base_clk_freq 0 another way 1~255 -> 1~255MHz
#define MMC_GET_INFO_ANOTHER_WAY	0x0
//capabilities2.clk_mul 0 not support 1~255 -> 2~256
#define MMC_CLK_MUL_NOT_SUPPORT	0x0

#define     __IO    volatile      

#define DEF_BLOCK_LEN   	0x1000	//4KB
#define BLOCK_1MB_NUM     	0x800	//2KB*512=1MB
#define BLOCK_128MB_NUM		0x40000	//256K*512=128MB

uint32_t Mmc_RegRead32(unsigned long reg_address);
void Mmc_RegWrite32(unsigned long reg_address, uint32_t reg_wdata);

#define REG_READ(reg_address) Mmc_RegRead32((unsigned long)reg_address)
#define REG_WRITE(reg_address, reg_wdata) Mmc_RegWrite32((unsigned long)reg_address, reg_wdata)


typedef enum {
	FAILED = 0, 
	PASSED = !FAILED
} TestStatus;

typedef union
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t xfer_block_size:12;
		__IO uint32_t sdma_buf_bdary:3;
		__IO uint32_t rsvd_blocksize15:1;
		__IO uint32_t blockcount_r:16;
	}bit;
}BLOCKCOUNT_R__BLOCKSIZE_R;

typedef union
{
    __IO uint32_t d32;
    struct {
	    __IO uint32_t dma_en:1;
	    __IO uint32_t block_count_enable:1;
	    __IO uint32_t auto_cmd_enable:2;
	    __IO uint32_t data_xfer_dir:1;
	    __IO uint32_t multi_blk_sel:1;
	    __IO uint32_t resp_type:1;
	    __IO uint32_t resp_err_chk_enable:1;
	    __IO uint32_t resp_int_disable:1;
	    __IO uint32_t rsvd:7;
        __IO uint32_t resp_type_select:2;
	    __IO uint32_t sub_cmd_flag:1;
	    __IO uint32_t cmd_crc_chk_enable:1;
	    __IO uint32_t cmd_idx_chk_enable:1;
	    __IO uint32_t data_present_sel:1;
	    __IO uint32_t cmd_type:2;
	    __IO uint32_t cmd_index:6;
	    __IO uint32_t rsvd_30_31:2;
    }bit;
}CMD_R__XFER_MODE_R;

typedef union
{
	__IO uint32_t d32;
	struct {
	__IO uint32_t cmd_inhibit:1;
	__IO uint32_t cmd_inhibit_dat:1;
	__IO uint32_t dat_line_active:1;
	__IO uint32_t re_tune_req:1;
	__IO uint32_t dat_7_4_mask:4;
	__IO uint32_t wr_xfer_active:1;
	__IO uint32_t rd_xfer_active:1;
	__IO uint32_t buf_wr_enable:1;
	__IO uint32_t buf_rd_enable:1;
	__IO uint32_t rsvd_15_12:4;
	__IO uint32_t card_inserted:1;
	__IO uint32_t card_stable:1;
	__IO uint32_t card_detect_pin_level:1;
	__IO uint32_t wr_protect_sw_lvl:1;
	__IO uint32_t dat_3_0:4;
	__IO uint32_t cmd_line_lvl:1;
	__IO uint32_t host_reg_vol:1;
	__IO uint32_t rsvd_26:1;
	__IO uint32_t cmd_issue_err:1;
	__IO uint32_t sub_cmd_stat:1;
	__IO uint32_t in_dormant_st:1;
	__IO uint32_t lane_sync:1;
	__IO uint32_t uhs2_if_detect:1;
	}bit;
}PSTATE_REG_R;

typedef union
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t led_ctrl:1;
		__IO uint32_t dat_xfer_width:1;
		__IO uint32_t high_speed_en:1;
		__IO uint32_t dma_sel:2;
		__IO uint32_t extdat_xfer:1;
		__IO uint32_t card_detect_test_lvl:1;
		__IO uint32_t card_detect_sig_sel:1;
        __IO uint32_t sd_bus_pwr_vdd1:1;
	    __IO uint32_t sd_bus_vol_vdd1:3;
	    __IO uint32_t sd_bus_pwr_vdd2:1;
	    __IO uint32_t sd_bus_vol_vdd2:3;
        __IO uint32_t stop_bg_req:1;
	    __IO uint32_t continue_req:1;
	    __IO uint32_t rd_wait_ctrl:1;
	    __IO uint32_t int_at_bgap:1;
	    __IO uint32_t rsvd_23_20:4;
        __IO uint32_t card_int:1;
	    __IO uint32_t card_insert:1;
	    __IO uint32_t card_removal:1;
	    __IO uint32_t rsvd_31_27:5;
	}bit;
}WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R;

typedef union
{
	__IO uint32_t d32;
	struct {
	__IO uint32_t	internal_clk_en:1;
	__IO uint32_t	internal_clk_stable:1;
	__IO uint32_t	sd_clk_en:1;
	__IO uint32_t	pll_enable:1;
	__IO uint32_t   rsvd_4:1;
	__IO uint32_t	clk_gen_select:1;
	__IO uint32_t   upper_freq_sel:2;
	__IO uint32_t   freq_sel:8;
    __IO uint32_t   tout_cnt:4;
	__IO uint32_t   rsvd_23_20:4;
    __IO uint32_t	sw_rst_all:1;
	__IO uint32_t   sw_rst_cmd:1;
	__IO uint32_t	sw_rst_dat:1;
	__IO uint32_t   rsvd_31_27:5;
    }bit;
}SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R;


typedef union
{
    __IO uint32_t d32;
    struct {
	    __IO uint32_t	 cmd_complete:1;
	    __IO uint32_t	 xfer_complete:1;
	    __IO uint32_t	 bgap_event:1;
	    __IO uint32_t	 dma_interrupt:1;
	    __IO uint32_t    buf_wr_ready:1;
	    __IO uint32_t	 buf_rd_ready:1;
	    __IO uint32_t    card_insertion:1;
	    __IO uint32_t    card_removal:1;
	    __IO uint32_t    card_interrupt:1;
	    __IO uint32_t    int_a:1;
	    __IO uint32_t    int_b:1;
	    __IO uint32_t    int_c:1;
	    __IO uint32_t    re_tune_event:1;
	    __IO uint32_t    fx_event:1;
	    __IO uint32_t    cqe_event:1;
	    __IO uint32_t    err_interrupt:1;
        __IO uint32_t	 cmd_tout_err:1;
	    __IO uint32_t	 cmd_crc_err:1;
	    __IO uint32_t	 cmd_end_bit_err:1;
	    __IO uint32_t	 cmd_idx_err:1;
	    __IO uint32_t    data_tout_err:1;
	    __IO uint32_t	 data_crc_err:1;
	    __IO uint32_t    data_end_bit_err:1;
	    __IO uint32_t    cur_lmt_err:1;
	    __IO uint32_t    auto_cmd_err:1;
	    __IO uint32_t    adma_err:1;
	    __IO uint32_t    tuning_err:1;
	    __IO uint32_t    resp_err:1;
	    __IO uint32_t    boot_ack_err:1;
	    __IO uint32_t    vendor_err1:1;
	    __IO uint32_t    vendor_err2:1;
	    __IO uint32_t    vendor_err3:1;
    }bit;
}ERROR_INT_STAT_R__NORMAL_INT_STAT_R;

typedef union
{
    __IO uint32_t d32;
    struct {
	    __IO uint32_t	cmd_complete_stat_en:1; /* intr when response received */
	    __IO uint32_t    xfer_complete_stat_en:1; /* intr when data read/write xfer completed */
	    __IO uint32_t    bgap_event_stat_en:1;
	    __IO uint32_t	dma_interrupt_stat_en:1;
	    __IO uint32_t	buf_wr_ready_stat_en:1;
	    __IO uint32_t    buf_rd_ready_stat_en:1;
	    __IO uint32_t	card_insertion_stat_en:1;
	    __IO uint32_t	card_removal_stat_en:1;
	    __IO uint32_t    card_interrupt_stat_en:1;
	    __IO uint32_t	int_a_stat_en:1;
	    __IO uint32_t	int_b_stat_en:1;
	    __IO uint32_t	int_c_stat_en:1;
	    __IO uint32_t	re_tune_event_stat_en:1;
	    __IO uint32_t	fx_event_stat_en:1;
	    __IO uint32_t    cqe_event_stat_en:1;
		__IO uint32_t	rsvd_15:1;
	    __IO uint32_t	cmd_tout_err_stat_en:1;
	    __IO uint32_t	cmd_crc_err_stat_en:1;
	    __IO uint32_t	cmd_end_bit_err_stat_en:1;
	    __IO uint32_t	cmd_idx_err_stat_en:1;
	    __IO uint32_t    data_tout_err_stat_en:1;
	    __IO uint32_t    data_crc_err_stat_en:1;
	    __IO uint32_t    data_end_bit_err_stat_en:1;
	    __IO uint32_t	cur_lmt_err_stat_en:1;
	    __IO uint32_t    auto_cmd_err_stat_en:1;
	    __IO uint32_t	adma_err_stat_en:1;
	    __IO uint32_t	tuning_err_stat_en:1;
	    __IO uint32_t	resp_err_stat_en:1;
	    __IO uint32_t    boot_ack_err_stat_en:1;
	    __IO uint32_t    vendor_err_stat_en1:1;
	    __IO uint32_t	vendor_err_stat_en2:1;
	    __IO uint32_t	vendor_err_stat_en3:1;
     }bit;
}ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R;

typedef union
{
    __IO uint32_t d32;
    struct {
	__IO uint32_t	cmd_complete_signal_en:1; /* intr when response received */
	__IO uint32_t    xfer_complete_signal_en:1; /* intr when data read/write xfer completed */
	__IO uint32_t    bgap_event_signal_en:1;
	__IO uint32_t	dma_interrupt_signal_en:1;
	__IO uint32_t	buf_wr_ready_signal_en:1;
	__IO uint32_t    buf_rd_ready_signal_en:1;
	__IO uint32_t	card_insertion_signal_en:1;
	__IO uint32_t	card_removal_signal_en:1;
	__IO uint32_t    card_interrupt_signal_en:1;
	__IO uint32_t	int_a_signal_en:1;
	__IO uint32_t	int_b_signal_en:1;
	__IO uint32_t	int_c_signal_en:1;
	__IO uint32_t	re_tune_event_signal_en:1;
	__IO uint32_t	fx_event_signal_en:1;
	__IO uint32_t    cqe_event_signal_en:1;
	__IO uint32_t	rsvd_15:1;
	__IO uint32_t	cmd_tout_err_signal_en:1;
	__IO uint32_t	cmd_crc_err_signal_en:1;
	__IO uint32_t	cmd_end_bit_err_signal_en:1;
	__IO uint32_t	cmd_idx_err_signal_en:1;
	__IO uint32_t    data_tout_err_signal_en:1;
	__IO uint32_t    data_crc_err_signal_en:1;
	__IO uint32_t    data_end_bit_err_signal_en	:1;
	__IO uint32_t	cur_lmt_err_signal_en:1;
	__IO uint32_t    auto_cmd_err_signal_en:1;
	__IO uint32_t	adma_err_signal_en:1;
	__IO uint32_t	tuning_err_signal_en:1;
	__IO uint32_t	resp_err_signal_en:1;
	__IO uint32_t    boot_ack_err_signal_en:1;
	__IO uint32_t    vendor_err_signal_en1:1;
	__IO uint32_t	vendor_err_signal_en2:1;
	__IO uint32_t	vendor_err_signal_en3:1;
    }bit;
}ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN_R;

typedef union
{
    __IO uint32_t d32;
    struct {
		__IO uint32_t	auto_cmd12_not_exec:1;
		__IO uint32_t	auto_cmd_tout_err:1;
		__IO uint32_t	auto_cmd_crc_err:1;
		__IO uint32_t	auto_cmd_ebit_err:1;
		__IO uint32_t    auto_cmd_idx_err:1;
		__IO uint32_t	auto_cmd_resp_err:1;
		__IO uint32_t    rsvd_6:1;
		__IO uint32_t    cmd_not_issued_auto_cmd12:1;
		__IO uint32_t    rsvd_15_8:8;
		__IO uint32_t	uhs_mode_sel:3;
		__IO uint32_t	signaling_en:1;
		__IO uint32_t	drv_strength_sel:2;
		__IO uint32_t	exec_tuning:1;
		__IO uint32_t    sample_clk_sel:1;
		__IO uint32_t	uhs2_if_enable:1;
		__IO uint32_t    rsvd_9:1;
		__IO uint32_t    adma2_len_mode:1;
		__IO uint32_t    cmd23_enable:1;
		__IO uint32_t    host_ver4_enable:1;
		__IO uint32_t    addressing:1;
		__IO uint32_t    async_int_enable:1;
		__IO uint32_t    preset_val_enable:1;
    }bit;
}HOST_CTRL2_R__AUTO_CMD_STAT_R;

typedef struct
{
	__IO uint32_t d32;
	struct {
	__IO uint32_t	sdr50_support:1;
	__IO uint32_t 	sdr104_support:1;
	__IO uint32_t 	ddr50_support:1;
	__IO uint32_t	uhs2_support:1;
	__IO uint32_t	drv_typea:1;
	__IO uint32_t 	drv_typec:1;
	__IO uint32_t	drv_typed:1;
	__IO uint32_t	rsvd_39:1;
	__IO uint32_t 	retune_cnt:4;
	__IO uint32_t	rsvd_44:1;
	__IO uint32_t	use_tuning_sdr50:1;
	__IO uint32_t	re_tuning_modes:2;
	__IO uint32_t	clk_mul:8;
	__IO uint32_t    rsvd_56_58:3;
	__IO uint32_t	adma3_support:1;
	__IO uint32_t	vdd2_18v_support:1;
	__IO uint32_t	rsvd_61:1;
	__IO uint32_t	rsvd_62_63:2;
	}bit;
}CAPABILITIES2_R;

typedef struct
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t tout_clk_freq : 6;
		__IO uint32_t rsvd_6 : 1;
		__IO uint32_t tout_clk_unit : 1;
		__IO uint32_t base_clk_freq : 8;
		__IO uint32_t max_blk_len : 2;
		__IO uint32_t embedded_8_bit : 1;
		__IO uint32_t adma2_support : 1;
		__IO uint32_t rsvd_20 : 1;
		__IO uint32_t high_speed_support : 1;
		__IO uint32_t sdma_support : 1;
		__IO uint32_t sus_res_support : 1;
		__IO uint32_t volt_33 : 1;
		__IO uint32_t volt_30 : 1;
		__IO uint32_t volt_18 : 1;
		__IO uint32_t sys_addr_64_v4 : 1;
		__IO uint32_t sys_addr_64_v3 : 1;
		__IO uint32_t async_int_support : 1;
		__IO uint32_t slot_type_r : 1;
	}bit;
}CAPABILITIES1_R;

typedef struct
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t max_cur_33v : 8;
		__IO uint32_t max_cur_30v : 8;
		__IO uint32_t max_cur_18v : 8;
		__IO uint32_t rsvd_31_24 : 8;
	}bit;
}CURR_CAPABILITIES1_R;

typedef struct
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t max_cur_vdd2_18v : 8;
		__IO uint32_t rsvd_63_40 : 24;
	}bit;
}CURR_CAPABILITIES2_R;

typedef struct
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t force_auto_cmd12_not_exec : 1;
		__IO uint32_t force_auto_cmd_tout_err : 1;
		__IO uint32_t force_auto_cmd_crc_err : 1;
		__IO uint32_t force_auto_cmd_ebit_err : 1;
		__IO uint32_t force_auto_cmd_idx_err : 1;
		__IO uint32_t force_auto_cmd_resp_err : 1;
		__IO uint32_t rsvd_6 : 1;
		__IO uint32_t force_cmd_not_issued_auto_cmd12 : 1;
		__IO uint32_t rsvd_15_8 : 8;
		__IO uint32_t force_cmd_tout_err : 1;
		__IO uint32_t force_cmd_crc_err : 1;
		__IO uint32_t force_cmd_end_bit_err : 1;
		__IO uint32_t force_cmd_idx_err : 1;
		__IO uint32_t force_data_tout_err : 1;
		__IO uint32_t force_data_crc_err : 1;
		__IO uint32_t force_data_end_bit_err : 1;
		__IO uint32_t force_cur_lmt_err : 1;
		__IO uint32_t force_auto_cmd_err : 1;
		__IO uint32_t force_adma_err : 1;
		__IO uint32_t force_tuning_err : 1;
		__IO uint32_t force_resp_err : 1;
		__IO uint32_t force_boot_ack_err : 1;
		__IO uint32_t force_vendor_err1 : 1;
		__IO uint32_t force_vendor_err2 : 1;
		__IO uint32_t force_vendor_err3 : 1;
	}bit;
}FORCE_ERROR_INT_STAT_R__FORCE_AUTO_CMD_STAT_R;


typedef struct
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t adma_err_states : 2;
		__IO uint32_t adma_len_err : 1;
		__IO uint32_t rsvd_31_3 : 29;
	}bit;
}ADMA_ERR_STAT_R;

typedef struct
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t freq_sel_val_init : 10;
		__IO uint32_t clk_gen_sel_val_init : 1;
		__IO uint32_t rsvd_13_11 : 3;
		__IO uint32_t drv_sel_val_init : 2;
		__IO uint32_t freq_sel_val_ds : 10;
		__IO uint32_t clk_gen_sel_val_ds : 1;
		__IO uint32_t rsvd_29_27 : 3;
		__IO uint32_t drv_sel_val_ds : 2;
	}bit;
}PRESET_DS_R__PRESET_INIT_R;

typedef struct
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t freq_sel_val_0 : 10;
		__IO uint32_t clk_gen_sel_val_0 : 1;
		__IO uint32_t rsvd_13_11 : 3;
		__IO uint32_t drv_sel_val_0 : 2;
		__IO uint32_t freq_sel_val_1 : 10;
		__IO uint32_t clk_gen_sel_val_1 : 1;
		__IO uint32_t rsvd_29_27 : 3;
		__IO uint32_t drv_sel_val_1 : 2;
	}bit;
}PRESET_HS_R;

typedef struct
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t freq_sel_val_0 : 10;
		__IO uint32_t clk_gen_sel_val_0 : 1;
		__IO uint32_t rsvd_13_11 : 3;
		__IO uint32_t drv_sel_val_0 : 2;
		__IO uint32_t freq_sel_val_1 : 10;
		__IO uint32_t clk_gen_sel_val_1 : 1;
		__IO uint32_t rsvd_29_27 : 3;
		__IO uint32_t drv_sel_val_1 : 2;
	}bit;
}PRESET_SDR50_R__PRESET_SDR25_R;

typedef struct
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t freq_sel_val_0 : 10;
		__IO uint32_t clk_gen_sel_val_0 : 1;
		__IO uint32_t rsvd_13_11 : 3;
		__IO uint32_t drv_sel_val_0 : 2;
		__IO uint32_t freq_sel_val_1 : 10;
		__IO uint32_t clk_gen_sel_val_1 : 1;
		__IO uint32_t rsvd_29_27 : 3;
		__IO uint32_t drv_sel_val_1 : 2;
	}bit;
}PRESET_DDR50_R__PRESET_SDR104_R;

typedef struct
{
	__IO uint32_t d32;
	struct {
		__IO uint32_t freq_sel_val : 10;
		__IO uint32_t clk_gen_sel_val : 1;
		__IO uint32_t rsvd_13_11 : 3;
		__IO uint32_t drv_sel_val : 2;
		__IO uint32_t rsvd_31_24:16;
	}bit;
}PRESET_UHS2;

/* dwc_mshc_map/DWC_mshc_block registers */
typedef struct
{
    __IO uint32_t sdmasa_r;
    __IO BLOCKCOUNT_R__BLOCKSIZE_R blockcount_r__blocksize;
    __IO uint32_t argument_r;
    __IO CMD_R__XFER_MODE_R cmd_r__xfer_mode;
    __IO uint32_t resp01;
	__IO uint32_t resp23;
	__IO uint32_t resp45;
	__IO uint32_t resp67;
	__IO uint32_t buf_data;
	__IO PSTATE_REG_R pstate_reg;
	__IO WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1;
    __IO SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R sw_rst_r__tout_ctrl_r__clk_ctrl;
	__IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R error_int_stat_r__normal_int_stat;
    __IO ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R error_int_stat_en_r__normal_int_stat_en;
    __IO ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN_R error_int_signal_en_r__normal_int_signal_en;
    __IO HOST_CTRL2_R__AUTO_CMD_STAT_R host_ctrl2_r__auto_cmd_stat;
	__IO CAPABILITIES1_R capabilities1;
	__IO CAPABILITIES2_R capabilities2;
	__IO CURR_CAPABILITIES1_R curr_capabilities1;
	__IO CURR_CAPABILITIES2_R curr_capabilities2;
	__IO FORCE_ERROR_INT_STAT_R__FORCE_AUTO_CMD_STAT_R force_auto_cmd_stat_r;
    __IO ADMA_ERR_STAT_R adma_err_stat_r;
	__IO uint32_t adma_sa_low_r;
	__IO uint32_t adma_sa_high_r;
	__IO PRESET_DS_R__PRESET_INIT_R preset_ds_r__preset_init;
	__IO PRESET_HS_R preset_ds;
	__IO PRESET_SDR50_R__PRESET_SDR25_R preset_sdr50_r__preset_sdr25;
	__IO PRESET_DDR50_R__PRESET_SDR104_R preset_ddr50_r__preset_sdr104;
}DWC_mshc_block_registers;
/**
  * @brief  Card Specific Data: CSD Register
  */
typedef struct
{
  __IO uint8_t  CSDStruct;            /*!< CSD structure */
  __IO uint8_t  SysSpecVersion;       /*!< System specification version */
  __IO uint8_t  Reserved1;            /*!< Reserved */
  __IO uint8_t  TAAC;                 /*!< Data read access-time 1 */
  __IO uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  __IO uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  __IO uint16_t CardComdClasses;      /*!< Card command classes */
  __IO uint8_t  RdBlockLen;           /*!< Max. read data block length */
  __IO uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
  __IO uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
  __IO uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
  __IO uint8_t  DSRImpl;              /*!< DSR implemented */
  __IO uint8_t  Reserved2;            /*!< Reserved */
  __IO uint32_t DeviceSize;           /*!< Device Size */
  __IO uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  __IO uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  __IO uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  __IO uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  __IO uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
  __IO uint8_t  EraseGrSize;          /*!< Erase group size */
  __IO uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
  __IO uint8_t  WrProtectGrSize;      /*!< Write protect group size */
  __IO uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
  __IO uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
  __IO uint8_t  WrSpeedFact;          /*!< Write speed factor */
  __IO uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
  __IO uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  __IO uint8_t  Reserved3;            /*!< Reserded */
  __IO uint8_t  ContentProtectAppli;  /*!< Content protection application */
  __IO uint8_t  FileFormatGrouop;     /*!< File format group */
  __IO uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
  __IO uint8_t  PermWrProtect;        /*!< Permanent write protection */
  __IO uint8_t  TempWrProtect;        /*!< Temporary write protection */
  __IO uint8_t  FileFormat;           /*!< File Format */
  __IO uint8_t  ECC;                  /*!< ECC code */
  __IO uint8_t  CSD_CRC;              /*!< CSD CRC */
  __IO uint8_t  Reserved4;            /*!< always 1*/
} SD_CSD;

/**
  * @brief  Card Identification Data: CID Register
  */
typedef struct
{
  __IO uint8_t  ManufacturerID;       /*!< ManufacturerID */
  __IO uint16_t OEM_AppliID;          /*!< OEM/Application ID */
  __IO uint32_t ProdName1;            /*!< Product Name part1 */
  __IO uint8_t  ProdName2;            /*!< Product Name part2*/
  __IO uint8_t  ProdRev;              /*!< Product Revision */
  __IO uint32_t ProdSN;               /*!< Product Serial Number */
  __IO uint8_t  Reserved1;            /*!< Reserved1 */
  __IO uint16_t ManufactDate;         /*!< Manufacturing Date */
  __IO uint8_t  CID_CRC;              /*!< CID CRC */
  __IO uint8_t  Reserved2;            /*!< always 1 */
} SD_CID;

/**
  * @brief SD Card information
  */
typedef struct
{
  SD_CSD SD_csd;
  SD_CID SD_cid;
  uint64_t CardCapacity;  /*!< Card Capacity */
  uint32_t CardBlockSize; /*!< Card Block Size */
  uint16_t RCA;
  uint8_t CardType;
} SD_CardInfo;

typedef union{
    __IO uint32_t d32;
    struct {
        __IO uint32_t block_num : 16;
        __IO uint32_t reserved23_16 : 8;
        __IO uint32_t forced_program : 1;
        __IO uint32_t comtext_id : 4;
        __IO uint32_t tag_request : 1;
        __IO uint32_t request : 1;
        __IO uint32_t reliable_write_request : 1;
    }bit;
}MMC_CMD23_PARAM;

typedef union{
    __IO uint32_t d32;
    struct {
        __IO uint32_t reserved_mtm : 2;
        __IO uint32_t reserved_asc : 2;
        __IO uint32_t reserved : 1;
        __IO uint32_t app_cmd : 1;
        __IO uint32_t execption_event : 1;
        __IO uint32_t switch_error : 1;
        __IO uint32_t ready_for_data : 1;
		__IO uint32_t current_state : 4;
		__IO uint32_t erase_reset : 1;
		__IO uint32_t reserved_set0 : 1;
		__IO uint32_t wp_erase_skip : 1;
		__IO uint32_t cid_csd_overwrite : 1;
		__IO uint32_t obsolete : 2;
		__IO uint32_t error : 1;
		__IO uint32_t cc_error : 1;
		__IO uint32_t device_ecc_failed : 1;
		__IO uint32_t illegal_command : 1;
		__IO uint32_t com_crc_error : 1;
		__IO uint32_t lock_unlock_failed : 1;
		__IO uint32_t device_is_locked : 1;
		__IO uint32_t wp_volation : 1;
		__IO uint32_t erase_param : 1;
		__IO uint32_t erase_seq_error : 1;
		__IO uint32_t block_len_error : 1;
		__IO uint32_t address_misalign : 1;
		__IO uint32_t address_out_of_range : 1;
    }bit;
}MMC_DEV_STAT;

typedef enum{
    MMC_MODE_FREQ,
    MMC_MODE_MAX
}MMC_MODE;

/**
  * @brief Supported SD Memory Cards
  */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((uint32_t)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007)

/** 
  * @brief SDIO Commands  Index 
  */
#define SD_CMD_GO_IDLE_STATE                       ((uint8_t)0)
#define SD_CMD_SEND_OP_COND                        ((uint8_t)1)
#define SD_CMD_ALL_SEND_CID                        ((uint8_t)2)
#define SD_CMD_SET_REL_ADDR                        ((uint8_t)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((uint8_t)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((uint8_t)5)
#define SD_CMD_HS_SWITCH                           ((uint8_t)6)
#define SD_CMD_SEL_DESEL_CARD                      ((uint8_t)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((uint8_t)8)
#define SD_CMD_SEND_CSD                            ((uint8_t)9)
#define SD_CMD_SEND_CID                            ((uint8_t)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((uint8_t)11) /*!< SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                   ((uint8_t)12)
#define SD_CMD_SEND_STATUS                         ((uint8_t)13)
#define SD_CMD_HS_BUSTEST_READ                     ((uint8_t)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((uint8_t)15)
#define SD_CMD_SET_BLOCKLEN                        ((uint8_t)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((uint8_t)17)
#define SD_CMD_READ_MULT_BLOCK                     ((uint8_t)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((uint8_t)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((uint8_t)20) /*!< SD Card doesn't support it */
#define SD_CMD_SET_BLOCK_COUNT                     ((uint8_t)23) /*!< SD Card doesn't support it */
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((uint8_t)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((uint8_t)25)
#define SD_CMD_PROG_CID                            ((uint8_t)26) /*!< reserved for manufacturers */
#define SD_CMD_PROG_CSD                            ((uint8_t)27)
#define SD_CMD_SET_WRITE_PROT                      ((uint8_t)28)
#define SD_CMD_CLR_WRITE_PROT                      ((uint8_t)29)
#define SD_CMD_SEND_WRITE_PROT                     ((uint8_t)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((uint8_t)32) /*!< To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((uint8_t)33) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((uint8_t)35) /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SD_CMD_ERASE_GRP_END                       ((uint8_t)36) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SD_CMD_ERASE                               ((uint8_t)38)
#define SD_CMD_FAST_IO                             ((uint8_t)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((uint8_t)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((uint8_t)42)
#define SD_CMD_APP_CMD                             ((uint8_t)55)
#define SD_CMD_GEN_CMD                             ((uint8_t)56)
#define SD_CMD_NO_CMD                              ((uint8_t)64)

/** 
  * @brief Following commands are SD Card Specific commands.
  *        SDIO_APP_CMD should be sent before sending these commands. 
  */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((uint8_t)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((uint8_t)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((uint8_t)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((uint8_t)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((uint8_t)53) /*!< For SD I/O Card only */

/** 
  * @brief Following commands are SD Card Specific security commands.
  *        SDIO_APP_CMD should be sent before sending these commands. 
  */
#define SD_CMD_SD_APP_GET_MKB                      ((uint8_t)43) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_MID                      ((uint8_t)44) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((uint8_t)45) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((uint8_t)46) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((uint8_t)47) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((uint8_t)48) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((uint8_t)38) /*!< For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48) /*!< For SD Card only */

//host_ctrl1 register param
#define MMC_HC1_LED_CTRL_OFF			0x0
#define MMC_HC1_LED_CTRL_ON				0x1
#define MMC_HC1_DAT_XFER_WIDTH_1BIT		0x0
#define MMC_HC1_DAT_XFER_WIDTH_4BIT		0x1
#define MMC_HC1_NORMAL_SPEED			0x0
#define MMC_HC1_HIGH_SPEED				0x1
#define MMC_HC1_DMA_SEL_SDMA			0x0
#define MMC_HC1_DMA_SEL_RSV				0x1
#define MMC_HC1_DMA_SEL_ADMA2			0x2
#define MMC_HC1_DMA_SEL_ADMA3			0x3
#define MMC_HC1_EXT_DAT_XFER_DEF		0x0		
#define MMC_HC1_EXT_DAT_XFER_8BIT		0x1

//pwr_ctrl register param
#define MMC_PC_SBP_VDD1_OFF				0x0	//SD BUS PWR
#define MMC_PC_SBP_VDD1_ON				0x1
#define MMC_PC_SBV_VDD1_RSV				0x4	//SD BUS VOLTAGE 0x0~0x4 reserved
#define SD_PC_SBV_VDD1_1V8				0x5
#define SD_PC_SBV_VDD1_3V0				0x6
#define EMMC_PC_SBV_VDD1_1V2			0x5
#define EMMC_PC_SBV_VDD1_1V8			0x6
#define MMC_PC_SBV_VDD1_3V3				0x7

//xfer_mode register param
#define MMC_XM_DMA_DISABLE				0x0
#define MMC_XM_DMA_ENABLE				0x1
#define MMC_XM_BLOCK_COUNT_DISABLE		0x0
#define MMC_XM_BLOCK_COUNT_ENABLE		0x1
#define MMC_XM_AUTO_CMD_DISABLE			0x0	//in SDIO, this field must be 0
#define MMC_XM_AUTO_CMD12_ENABLE		0x1
#define MMC_XM_AUTO_CMD23_ENABLE		0x2
#define MMC_XM_AUTO_CMD_AUTO_ENABLE		0x3
#define MMC_XM_DATA_XFER_DIR_WRITE		0x0
#define MMC_XM_DATA_XFER_DIR_READ		0x1
#define MMC_XM_SEL_SINGLE_BLOCK			0x0
#define MMC_XM_SEL_MULTI_BLOCK			0x1
#define MMC_XM_RESP_R1					0x0	//Memory
#define MMC_XM_RESP_R5					0x1	//SDIO
#define MMC_XM_RESP_ERR_CHK_DISABLE		0x0
#define MMC_XM_RESP_ERR_CHK_ENABLE		0x1
#define MMC_XM_RESP_INT_ENABLE			0x0
#define MMC_XM_RESP_INT_DISABLE			0x1

//cmd register param
#define MMC_C_NO_RESP					0x0
#define MMC_C_RESP_LEN_136				0x1
#define MMC_C_RESP_LEN_48				0x2
#define MMC_C_RESP_LEN_48B				0x3	//check busy after response
#define MMC_C_CMD_FLAG_MAIN				0x0
#define MMC_C_CMD_FLAG_SUB				0x1
#define MMC_C_CMD_CRC_CHECK_DISABLE		0x0
#define MMC_C_CMD_CRC_CHECK_ENABLE		0x1
#define MMC_C_CMD_IDX_CHECK_DISABLE		0x0
#define MMC_C_CMD_IDX_CHECK_ENABLE		0x1
#define MMC_C_NO_DATA_PRESENT			0x0
#define MMC_C_DATA_PRESENT				0x1
#define MMC_C_NORMAL_CMD				0x0
#define MMC_C_SUSPEND_CMD				0x1
#define MMC_C_RESUME_CMD				0x2
#define MMC_C_ABORT_CMD					0x3

//clk_ctrl register param
#define MMC_CC_INTER_CLK_DISABLE		0x0
#define MMC_CC_INTER_CLK_ENABLE			0x1
#define MMC_CC_INTER_CLK_NOT_STABLE		0x0
#define MMC_CC_INTER_CLK_STABLE			0x1
#define MMC_CC_SD_CLK_DISABLE			0x0
#define MMC_CC_SD_CLK_ENABLE			0x1
#define MMC_CC_PLL_LOW_PWR_MODE			0x0
#define MMC_CC_PLL_ENABLE				0x1
#define MMC_CC_CLK_GEN_SEL_DIVIDED		0x0
#define MMC_CC_CLK_GEN_SEL_PROGRAM		0x1
#define MMC_CC_SW_RST_ALL_DISABLE		0x0
#define MMC_CC_SW_RST_ALL_ENABLE		0x1

//tout_ctrl register param
#define MMC_TC_TOUT_CNT_2_13			0x0
#define MMC_TC_TOUT_CNT_2_14			0x1
#define MMC_TC_TOUT_CNT_2_15			0x2
#define MMC_TC_TOUT_CNT_2_16			0x3
#define MMC_TC_TOUT_CNT_2_17			0x4
#define MMC_TC_TOUT_CNT_2_18			0x5
#define MMC_TC_TOUT_CNT_2_19			0x6
#define MMC_TC_TOUT_CNT_2_20			0x7
#define MMC_TC_TOUT_CNT_2_21			0x8
#define MMC_TC_TOUT_CNT_2_22			0x9
#define MMC_TC_TOUT_CNT_2_23			0xA
#define MMC_TC_TOUT_CNT_2_24			0xB
#define MMC_TC_TOUT_CNT_2_25			0xC
#define MMC_TC_TOUT_CNT_2_26			0xD
#define MMC_TC_TOUT_CNT_2_27			0xE

//normal_int_stat_en register param
#define MMC_NORMAL_INT_STAT_MASKED		0x0
#define MMC_NORMAL_INT_STAT_EN			0x1

//error_int_stat_en register param
#define MMC_ERR_INT_STAT_MASKED			0x0
#define MMC_ERR_INT_STAT_EN				0x1

//normal_int_signal_en register param
#define MMC_NORMAL_INT_SIGN_MASKED		0x0
#define MMC_NORMAL_INT_SIGN_EN			0x1

//error_int_signal_en register param
#define MMC_ERR_INT_SIGN_MASKED			0x0
#define MMC_ERR_INT_SIGN_EN				0x1

//io bank1 vcc ref
#define MMC_IO_BANK1_SUPPORT_1V8(reg)	(reg & 0x1)
#define MMC_IO_BANK1_SUPPORT_2V5(reg)	((reg & 0x2) >> 1)
#define MMC_IO_BANK1_SUPPORT_3V3(reg)	((reg & 0x4) >> 2)


#define DATA_READ 0x1
#define DATA_WRITE 0x0

#define MMC_Response_No                    (0x0)
#define MMC_Response_Long                  (0x1)
#define MMC_Response_Short                 (0x2)
#define MMC_Response_Short_48B             (0x3)

#define MMC_FREQ_400K					(0)    
#define MMC_FREQ_10M					(1)
#define MMC_FREQ_MAX					(2)

//delay func
#define MMC_DELAY_US(us)				Mtimer_Delay(us)
#define MMC_DELAY_MS(ms)				Mtimer_Delay(ms*1000)

uint32_t WaitCmdComplete(volatile DWC_mshc_block_registers* ptr, MMC_ERR_TYPE Err);
uint32_t WaitTransferComplete(volatile DWC_mshc_block_registers* ptr, MMC_ERR_TYPE Err);
uint32_t TransferWithoutDMA(volatile DWC_mshc_block_registers* Ptr, uint32_t *Addr, MMC_ERR_TYPE Err);
uint32_t WaitClockStable(volatile DWC_mshc_block_registers* Ptr);
uint32_t CheckLineInhibit(volatile DWC_mshc_block_registers* Ptr);
void ClearErrandIntStatus(volatile DWC_mshc_block_registers* Ptr);
#define MMC_WAIT_CLK_STABLE(ptr)					status = WaitClockStable(ptr);\
													if(status != MMC_SUCCESS){\
														return status;\
													}
#define MMC_WAIT_CMD_COMPLETE(ptr, err)				status = WaitCmdComplete(ptr, err);\
													if(status != MMC_SUCCESS){\
														return status;\
													}
#define MMC_WAIT_TRANSFER_COMPLETE(ptr, err)		status = WaitTransferComplete(ptr, err);\
													if(status != MMC_SUCCESS){\
														return status;\
													}
#define MMC_TRANSFER_WITHOUT_DMA(ptr, addr, err)	status = TransferWithoutDMA(ptr, addr, err);\
													if(status != MMC_SUCCESS){\
														return status;\
													}
#define MMC_CHECK_LINE_INHIBIT(ptr)					status = CheckLineInhibit(ptr);\
													if(status != MMC_SUCCESS){\
														return status;\
													}
#define MMC_CLEAR_STATUS(ptr)						ClearErrandIntStatus(ptr)

#define MMC_CHECK_LINE_AND_CLEAR_STATUS(ptr)		status = CheckLineInhibit(ptr);\
													if(status != MMC_SUCCESS){\
														return status;\
													}\
													ClearErrandIntStatus(ptr)

#ifdef AL_DEBUG_PRINT
#ifdef MMC_BRANCHTEST
#define BRANCH_NUM		BRANCH_MAX
#define BRANCH_BRANCH	2
extern uint32_t BranchTestCount[BRANCH_NUM][BRANCH_BRANCH];
#define MMC_BRANCHTEST_PRINT(branch, num)		do{\
													rom_printf(DEBUG_BRANCHTEST, "[G]:[%s][%s][%d]\t\t[%d]\t[%d]\r\n",\
													__FILE__, __func__, __LINE__, branch, num);\
													BranchTestCount[branch][num]++;\
												}while(0)
#define MMC_FLOWNUM_PRINT(...)					do{\
													rom_printf(DEBUG_BRANCHTEST, __VA_ARGS__);\
												}while(0)
#else
#define MMC_BRANCHTEST_PRINT(branch, num)
#define MMC_FLOWNUM_PRINT(...)
#endif

typedef enum{
	BRANCH_FLOW_MODULE_INIT,
	BRANCH_FLOW_MODULE_BYTEREAD,
	BRANCH_FLOW_MODULE_BLOCKREAD,
	BRANCH_FLOW_MODULE_MODESET,
	BRANCH_FLOW_MODULE_MAX
}BRANCH_FLOW_MODULE;
#else
#define MMC_BRANCHTEST_PRINT(branch, num)
#define MMC_FLOWNUM_PRINT(...)
#endif

extern __IO DWC_mshc_block_registers* SDIO;
extern __IO DWC_mshc_block_registers* eMMC;
extern MtimerParams MmcMtimer;
extern uint32_t CsdTab[4];
extern uint32_t CidTab[4];
extern uint32_t Resp[4];
extern uint32_t Rca;
extern uint8_t FlashSharedBuf[DEF_BLOCK_LEN];
extern uint8_t EfuseDelayParam;

uint32_t HostControllerSetup(volatile DWC_mshc_block_registers* ptr);
uint32_t InitInterruptSetting(volatile DWC_mshc_block_registers* ptr);
void Mmc_BranchFlowPrint(uint32_t Module, uint32_t FlowNumS, uint32_t FlowNumE);
void PrintfMshcBlock(volatile DWC_mshc_block_registers *Ptr);

/*****************************END OF FILE**************************/

#endif
