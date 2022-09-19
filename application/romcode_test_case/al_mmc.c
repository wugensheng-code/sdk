/*****************************************************************************/
/**
*
* @file sd_test.c
* @addtogroup sdps_v3_13
* @{
*
* The implementation of the XSdPs component's static initialization
* functionality.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- ---    -------- -----------------------------------------------
* 1.00a hk/sg  10/17/13 Initial release
*       kvn    07/15/15 Modified the code according to MISRAC-2012.
* 3.7   aru    03/12/19 Modified the code according to MISRAC-2012.
*
* </pre>
*
******************************************************************************/

#include <string.h>
#include <stdio.h>
#include "al_mmc.h"
#include "mtimer.h"

__IO uint32_t CsdTab[4], CidTab[4], Resp[4], Rca = 1;
MtimerParams MmcMtimer;
uint8_t __attribute__((aligned(4))) FlashSharedBuf[DEF_BLOCK_LEN];

volatile DWC_mshc_block_registers* SDIO = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;
volatile DWC_mshc_block_registers* eMMC = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;

/**
 * @brief read value in RegAddress
 * 
 * @param RegAddress is read address
 * @return unsigned int value 
 */
unsigned int reg_read(unsigned long RegAddress)
{
    return *((volatile unsigned long *)RegAddress);
}

/**
 * @brief write RegWdata in RegAddress
 * 
 * @param RegAddress is write address
 * @param RegWdata is write data
 */
void reg_write(unsigned long RegAddress, uint32_t RegWdata)
{
    *((volatile unsigned long *)RegAddress) = RegWdata;
}
#if 1
/**
 * @brief print error state
 * 
 * @param err_state is offset of error code
 */

static void ErrStatPrint(uint32_t ErrState)
{
    MMC_PRINT("error stat print: ");
    switch(ErrState){
        case 0:
            MMC_PRINT("MMC_CMD_TOUT_ERR\r\n");
            break;
        case 1:
            MMC_PRINT("MMC_CMD_CRC_ERR\r\n");
            break;
        case 2:
            MMC_PRINT("MMC_CMD_END_BIT_ERR\r\n");
            break;
        case 3:
            MMC_PRINT("MMC_CMD_IDX_ERR\r\n");
            break;
        case 4:
            MMC_PRINT("MMC_DATA_TOUT_ERR\r\n");
            break;
        case 5:
            MMC_PRINT("MMC_DATA_CRC_ERR\r\n");
            break;
        case 6:
            MMC_PRINT("MMC_DATA_END_BIT_ERR\r\n");
            break;
        case 7:
            MMC_PRINT("MMC_CUR_LMT_ERR\r\n");
            break;
        case 8:
            MMC_PRINT("MMC_AUTO_CMD_ERR\r\n");
            break;
        case 9:
            MMC_PRINT("MMC_ADMA_ERR\r\n");
            break;
        case 10:
            MMC_PRINT("MMC_TUNING_ERR\r\n");
            break;
        case 11:
            MMC_PRINT("MMC_RESP_ERR\r\n");
            break;
        case 12:
            MMC_PRINT("MMC_BOOT_ACK_ERR\r\n");
            break;
        default:
            MMC_PRINT("unknown error state\r\n");
            break;
    }
}
#endif
/**
 * @brief check error bit in error state register
 * 
 * @param r is error state register
 */
static uint32_t ErrStatCheck(volatile ERROR_INT_STAT_R__NORMAL_INT_STAT_R Reg)
{
    uint32_t status = MMC_SUCCESS;
    uint32_t i = 0;
    uint32_t errorbits = Reg.d32>>0x10;
    for(;i < MMC_ERR_INT_STAT_BITS_LEN; i++){
        if(((errorbits>>i)&0x1) == 1){
            //ErrStatPrint(i);
            status = i + MMC_ERROR_CODE_OFFSET;
            break;
        }
    }
    return status;
}

/**
 * @brief wait transfer command complete
 * 
 * @param Ptr host controller parameter structure
 * @return uint32_t mmc error code
 */
uint32_t WaitCmdComplete(volatile DWC_mshc_block_registers* Ptr, MMC_ERR_TYPE Err)
{
    uint32_t status = MMC_SUCCESS;
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r = {.d32 = 0,};
    MTIMER_OUT_CONDITION(MMC_CMD_TIMEOUT_VAL, &MmcMtimer, r.bit.cmd_complete != 1){
        r.d32 = REG_READ((unsigned long)&(Ptr->error_int_stat_r__normal_int_stat.d32));
        status = ErrStatCheck(r);
		if(status != MMC_SUCCESS)
			return status;
    }
    if(Mtimer_IsTimerOut(&MmcMtimer)){
        return Err;
    }else{
        MMC_PRINT("[CMD COMPLETE]cur r is 0x%x\r\n", r.d32);
        r.d32 = 0;
        r.bit.cmd_complete = 0x1;
        MMC_PRINT("[CMD COMPLETE]next r is 0x%x\r\n", r.d32);
        REG_WRITE((unsigned long)&(Ptr->error_int_stat_r__normal_int_stat.d32), r.d32);
        return MMC_SUCCESS;
    }
}

/**
 * @brief wait transfer data complete
 * 
 * @param Ptr host controller parameter structure
 * @param Err cur cmd error code
 * @return uint32_t status
 */
uint32_t WaitTransferComplete(volatile DWC_mshc_block_registers* Ptr, MMC_ERR_TYPE Err)
{
    uint32_t status = MMC_SUCCESS;
    ERROR_INT_STAT_R__NORMAL_INT_STAT_R r = {.d32 = 0,};
    MTIMER_OUT_CONDITION(MMC_XFER_TIMEOUT_VAL, &MmcMtimer, r.bit.xfer_complete != 1){
        r.d32 = REG_READ((unsigned long)&(Ptr->error_int_stat_r__normal_int_stat.d32));
        status = ErrStatCheck(r);
		if(status != MMC_SUCCESS)
			return status;
        MMC_PRINT("[XFER COMPLETE]cur r is 0x%x\r\n", r.d32);
        if(r.bit.dma_interrupt == 1 && r.bit.xfer_complete != 1){
            unsigned long Buffer_SingleBlock = REG_READ((unsigned long)&(Ptr->sdmasa_r));
            r.d32 = 0;
            r.bit.dma_interrupt = 0x1;
            REG_WRITE((unsigned long)&(Ptr->error_int_stat_r__normal_int_stat.d32), r.d32);
            REG_WRITE((unsigned long)&(Ptr->sdmasa_r), Buffer_SingleBlock);
        }
    }
    if(Mtimer_IsTimerOut(&MmcMtimer)){
        return Err;
    }else{
        r.d32 = 0;
        r.bit.xfer_complete = 0x1;
        REG_WRITE((unsigned long)&(Ptr->error_int_stat_r__normal_int_stat.d32), r.d32);
        return MMC_SUCCESS;
    }
}

/**
 * @brief 
 * 
 * @param Ptr host controller parameter structure
 * @param Addr read/write addr
 * @param Err cur cmd error code
 * @return uint32_t status
 */
uint32_t TransferWithoutDMA(volatile DWC_mshc_block_registers* Ptr, uint32_t *Addr, MMC_ERR_TYPE Err)
{
    uint32_t status = MMC_SUCCESS;
    uint32_t *bufaddr = Addr;
    ERROR_INT_STAT_R__NORMAL_INT_STAT_R r = {.d32 = 0,};

	if(Err == MMC_CMD_17_XFER_ERR){
		MTIMER_OUT_CONDITION(MMC_BUF_RD_RDY_TIMEOUT_VAL, &MmcMtimer, r.bit.buf_rd_ready != 1){
			r.d32 = REG_READ((unsigned long)&(Ptr->error_int_stat_r__normal_int_stat.d32));
			status = ErrStatCheck(r);
			if(status != MMC_SUCCESS)
				return status;
		}
		if(Mtimer_IsTimerOut(&MmcMtimer)){
			return Err;
		}else{
			r.d32 = 0;
			r.bit.buf_rd_ready = 0x1;
			REG_WRITE((unsigned long)&(Ptr->error_int_stat_r__normal_int_stat.d32), r.d32);
			r.d32 = 0;
			for(int i = 0; i < 512; i++){
				*(uint32_t *)(bufaddr+i) = REG_READ((unsigned long)&(Ptr->buf_data));
			}
		}
	}else{
		MTIMER_OUT_CONDITION(MMC_BUF_RD_RDY_TIMEOUT_VAL, &MmcMtimer, r.bit.buf_wr_ready != 1){
			r.d32 = REG_READ((unsigned long)&(Ptr->error_int_stat_r__normal_int_stat.d32));
			status = ErrStatCheck(r);
			if(status != MMC_SUCCESS)
				return status;
		}
		if(Mtimer_IsTimerOut(&MmcMtimer)){
			return Err;
		}else{
			r.d32 = 0;
			r.bit.buf_wr_ready = 0x1;
			REG_WRITE((unsigned long)&(Ptr->error_int_stat_r__normal_int_stat.d32), r.d32);
			r.d32 = 0;
			for(int i = 0; i < 512; i++){
				REG_WRITE((unsigned long)&(Ptr->buf_data), *(uint32_t *)(bufaddr+i));
			}
		}
	}
	WaitTransferComplete(Ptr, Err);
    return status;
}

/**
 * @brief wait clock stable after enable
 * 
 * @param Ptr host controller parameter structure
 * @return uint32_t mmc error code
 */
uint32_t WaitClockStable(volatile DWC_mshc_block_registers* Ptr)
{
    SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r = {.d32 = 0,};
    MTIMER_OUT_CONDITION(MMC_WAIT_CLK_STABLE_TIMEOUT_VAL, &MmcMtimer, r.bit.internal_clk_stable != 1){
        r.d32 = REG_READ((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
    }
    if(Mtimer_IsTimerOut(&MmcMtimer)){
        return MMC_WAIT_CLK_STABLE_TIMEOUT;
    }else{
        return MMC_SUCCESS;
    }
}

/**
 * @brief init the SD/EMMC host controller
 * 
 * @param Ptr host controller parameter structure
 * @return uint32_t mmc error code
 */
uint32_t HostControllerSetup(volatile DWC_mshc_block_registers* Ptr)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    uint32_t r = 0;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1 = {.d32 = 0,};
    SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r2 = {.d32 = 0,};
    MMC_PRINT("HostControllerSetup\r\n");   //sequence path print

    r1.d32 = 0;
#ifdef _USE_SDMA
    r1.bit.dma_sel = MMC_HC1_DMA_SEL_SDMA;          //SDMA
#endif
    r1.bit.sd_bus_pwr_vdd1 = MMC_PC_SBP_VDD1_ON;    //VDD1 PWR ON support SD and eMMC
    r = REG_READ((unsigned long)IO_BANK1_REF);
    MMC_PRINT("r is %x\r\n", r);
    if(MMC_IO_BANK1_SUPPORT_1V8(r) == 1){
        r1.bit.sd_bus_vol_vdd1 = EMMC_PC_SBV_VDD1_1V8;   //1.8V
    }else{
        r1.bit.sd_bus_vol_vdd1 = MMC_PC_SBV_VDD1_3V3;   //3.3V
    }
    REG_WRITE((unsigned long)&(Ptr->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32), r1.d32);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

    MMC_DELAY_MS(40);   //1msec+0.1~35msec+1msec+(74 clock)

    r2.d32 = REG_READ((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
    r2.bit.tout_cnt = MMC_TC_TOUT_CNT_2_27;
    REG_WRITE((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r2.d32);
    MMC_PRINT("r2.d32 is %x\r\n", r2.d32);

    return status;
}

/***************************************************************************/
/**
 * @brief	set interrupt controller
 *
 * @param	None
 * @return	status
 *
 ******************************************************************************/
uint32_t InitInterruptSetting(volatile DWC_mshc_block_registers* Ptr){
    uint32_t status = MMC_SUCCESS;
    ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R r1 = {.d32 = 0,};

    MMC_PRINT("InitInterruptSetting\r\n");
    r1.d32 = 0;
    r1.bit.cmd_complete_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.xfer_complete_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.bgap_event_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.dma_interrupt_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.buf_wr_ready_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.buf_rd_ready_stat_en = MMC_NORMAL_INT_STAT_EN;
     r1.bit.card_insertion_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.card_removal_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.int_a_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.cmd_tout_err_stat_en = MMC_ERR_INT_STAT_EN;
    r1.bit.cmd_crc_err_stat_en = MMC_ERR_INT_STAT_EN;
    r1.bit.cmd_end_bit_err_stat_en = MMC_ERR_INT_STAT_EN;
    r1.bit.cmd_idx_err_stat_en = MMC_ERR_INT_STAT_EN;
    r1.bit.data_tout_err_stat_en = MMC_ERR_INT_STAT_EN;
    r1.bit.data_crc_err_stat_en = MMC_ERR_INT_STAT_EN;
    r1.bit.data_end_bit_err_stat_en = MMC_ERR_INT_STAT_EN;
    r1.bit.cur_lmt_err_stat_en = MMC_ERR_INT_STAT_EN;
    REG_WRITE((unsigned long)&(Ptr->error_int_stat_en_r__normal_int_stat_en.d32), r1.d32);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

    return status;
}


/*********************************************END OF FILE**********************/
