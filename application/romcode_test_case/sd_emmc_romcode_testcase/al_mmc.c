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

MtimerParams MmcMtimer, Mmc1Mtimer;
uint8_t __attribute__((aligned(4))) FlashSharedBuf[DEF_BLOCK_LEN];
uint32_t EfuseDelayParam = 0;
uint32_t CsdTab[4], CidTab[4], Resp[4], Rca = 0;
uint32_t IoBank1Ref = 0;
uint32_t CardType =  SDIO_HIGH_CAPACITY_SD_CARD;

#ifdef AL_DEBUG_PRINT
uint32_t BranchTestCount[BRANCH_MAX] = {0};
uint32_t ErrBranchCtrl = ~0;
#endif

__IO DWC_mshc_block_registers* SDIO = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO1__BASE_ADDR;
__IO DWC_mshc_block_registers* eMMC = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;


/**
 * @brief read value in RegAddress
 * 
 * @param RegAddress is read address
 * @return unsigned int value 
 */
uint32_t Mmc_RegRead32(unsigned long RegAddress)
{
    return *((volatile unsigned int *)RegAddress);
}

/**
 * @brief write RegWdata in RegAddress
 * 
 * @param RegAddress is write address
 * @param RegWdata is write data
 */
void Mmc_RegWrite32(unsigned long RegAddress, uint32_t RegWdata)
{
    *((volatile unsigned int *)RegAddress) = RegWdata;
}


/**
 * @brief print mshc block register
 * 
 * @param Ptr mshc structre
 */
void PrintfMshcBlock(volatile DWC_mshc_block_registers *Ptr)
{
#ifdef _USE_MSHC_PRINT
    uint32_t blocklen   = (sizeof(DWC_mshc_block_registers) >> 2);
    uint32_t *ptr       = (uint32_t *)Ptr;

    printf("Print Mshc Block!\r\n");
    for (int i = 0; i < blocklen; i++) {
        printf("%d 0x%x\t", i, ptr[i]);
    }
    printf("\r\n");
#endif
}

#ifdef _USE_ERR_PRINT
/**
 * @brief print error state
 * 
 * @param err_state offset of error code
 */

static void ErrStatPrint(uint32_t ErrState)
{
    MMC_PRINT("error stat print: ");
    switch (ErrState) {
        case 0:
            printf("MMC_CMD_TOUT_ERR\r\n");
            break;
        case 1:
            printf("MMC_CMD_CRC_ERR\r\n");
            break;
        case 2:
            printf("MMC_CMD_END_BIT_ERR\r\n");
            break;
        case 3:
            printf("MMC_CMD_IDX_ERR\r\n");
            break;
        case 4:
            printf("MMC_DATA_TOUT_ERR\r\n");
            break;
        case 5:
            printf("MMC_DATA_CRC_ERR\r\n");
            break;
        case 6:
            printf("MMC_DATA_END_BIT_ERR\r\n");
            break;
        case 7:
            printf("MMC_CUR_LMT_ERR\r\n");
            break;
        case 8:
            printf("MMC_AUTO_CMD_ERR\r\n");
            break;
        case 9:
            printf("MMC_ADMA_ERR\r\n");
            break;
        case 10:
            printf("MMC_TUNING_ERR\r\n");
            break;
        case 11:
            printf("MMC_RESP_ERR\r\n");
            break;
        case 12:
            printf("MMC_BOOT_ACK_ERR\r\n");
            break;
        default:
            printf("unknown error state\r\n");
            break;
    }
}
#endif

/**
 * @brief check error bit in error state register
 * 
 * @param r error state register
 * @return uint32_t mmc error code
 */
static uint32_t ErrStatCheck(volatile ERROR_INT_STAT_R__NORMAL_INT_STAT_R Reg)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;

    if (Reg.bit.err_interrupt == 0)
        return status;

    uint32_t errorbits  = Reg.d32 >> 0x10;
    for (uint32_t i = 0; i < MMC_ERR_INT_STAT_BITS_LEN; i++) {
        if (((errorbits >> i) & 0x1) == 1) {
#ifdef _USE_ERR_PRINT
            ErrStatPrint(i);
#endif
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
    MMC_ERR_TYPE status                         = MMC_SUCCESS;
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r  = {.d32 = 0,};

    MTIMER_OUT_CONDITION(MMC_CMD_TIMEOUT_VAL, &MmcMtimer, r.bit.cmd_complete != 1) {
        MMC_BRANCHTEST_PRINT(BRANCH_CMD_COMPLETE_REG_READ);
        r.d32 = REG_READ(&(Ptr->error_int_stat_r__normal_int_stat.d32));

        status = ErrStatCheck(r);
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_CMD_COMPLETE_ERROR_NOTCMD8)) {
            status = MMC_FAILURE;
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_CMD_COMPLETE_ERROR_NOTCMD8);
        }
		if (status != MMC_SUCCESS) {
            MMC_BRANCHTEST_PRINT(BRANCH_CMD_COMPLETE_ERROR);
#ifdef _USE_MSHC_PRINT
            PrintfMshcBlock(Ptr);
#endif
            MMC_PRINT("[CMD COMPLETE]error cmd code %d\r\n", Err);
            if (Err == MMC_CMD_8_ERR) {
                MMC_BRANCHTEST_PRINT(BRANCH_CMD_COMPLETE_ERROR_CMD8);
                //sdsc v1.1 is error
                MMC_PRINT("[CMD COMPLETE]MMC CMD 8 ERR\r\n");
                __IO SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r1 = {.d32 = 0,};
                r1.d32 = REG_READ(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
                MMC_PRINT("[CMD COMPLETE]rst r is 0x%x\r\n", r1.d32);
                //reset cmd line
                r1.bit.sw_rst_cmd = 0x1;
                REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);

                r1.d32 = 0;
                r1.d32 = REG_READ(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
                MTIMER_OUT_CONDITION(MMC_CMDLINE_RESET_TIMEOUT_VAL, &Mmc1Mtimer, r1.bit.sw_rst_cmd != 0) {
                    MMC_BRANCHTEST_PRINT(BRANCH_CMDLINE_RST_COMPLETE_REG_READ);
                    r1.d32 = REG_READ(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
                    MMC_PRINT("[CMD COMPLETE]rst r is 0x%x\r\n", r1.d32);
                    if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_CMDLINE_RST_COMPLETE_TIMEOUT)) {
                        r1.bit.sw_rst_cmd = 1;
                    }
                }
                MMC_BRANCHTEST_PRINT(BRANCH_CMDLINE_RST_COMPLETE_DONE);
                
                if (Mtimer_IsTimerOut(&Mmc1Mtimer)) {
                    if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_CMDLINE_RST_COMPLETE_TIMEOUT)) {
                        ERROR_BRANCH_BIT_SET(BERROR_BRANCH_CMDLINE_RST_COMPLETE_TIMEOUT);
                    }
                    MMC_BRANCHTEST_PRINT(BRANCH_CMDLINE_RST_COMPLETE_TIMEOUT);
                    return MMC_CMD_TIMEOUT;
                }
                CardType = SDIO_STD_CAPACITY_SD_CARD_V1_1;
                MMC_BRANCHTEST_PRINT(BRANCH_CMDLINE_RST_COMPLETE_SUCCESS);
                MMC_PRINT("[CMD COMPLETE]MMC CMD line rst success!\r\n");
                return MMC_SUCCESS;
            } else {
                MMC_BRANCHTEST_PRINT(BRANCH_CMD_COMPLETE_ERROR_NOTCMD8);
                return Err;
            }
        }
        MMC_BRANCHTEST_PRINT(BRANCH_CMD_COMPLETE_NOTERROR);
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_CMD_COMPLETE_TIMEOUT)) {
            r.bit.cmd_complete  = 0x0;
        }
    }
    MMC_BRANCHTEST_PRINT(BRANCH_CMD_COMPLETE_DONE);

    if (Mtimer_IsTimerOut(&MmcMtimer)) {
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_CMD_COMPLETE_TIMEOUT)) {
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_CMD_COMPLETE_TIMEOUT);
        }
        MMC_BRANCHTEST_PRINT(BRANCH_CMD_COMPLETE_TIMEOUT);
        return MMC_CMD_TIMEOUT;
    } else {
        MMC_BRANCHTEST_PRINT(BRANCH_CMD_COMPLETE_SUCCESS);
        MMC_PRINT("[CMD COMPLETE]cur r is 0x%x\r\n", r.d32);
        r.d32               = 0;
        r.bit.cmd_complete  = 0x1;
        MMC_PRINT("[CMD COMPLETE]next r is 0x%x\r\n", r.d32);
        REG_WRITE(&(Ptr->error_int_stat_r__normal_int_stat.d32), r.d32);
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
    MMC_ERR_TYPE status                         = MMC_SUCCESS;
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r  = {.d32 = 0,};

    MTIMER_OUT_CONDITION(MMC_XFER_TIMEOUT_VAL, &MmcMtimer, r.bit.xfer_complete != 1) {
        MMC_BRANCHTEST_PRINT(BRANCH_XFER_COMPLETE_REG_READ);
        r.d32 = REG_READ(&(Ptr->error_int_stat_r__normal_int_stat.d32));

        status = ErrStatCheck(r);
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_XFER_COMPLETE_ERROR)) {
            status = MMC_FAILURE;
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_XFER_COMPLETE_ERROR);
        }
		if (status != MMC_SUCCESS) {
            MMC_BRANCHTEST_PRINT(BRANCH_XFER_COMPLETE_ERROR);
            PrintfMshcBlock(Ptr);
            MMC_PRINT("error cmd code %d\r\n", Err);
            return Err;
        }
        MMC_BRANCHTEST_PRINT(BRANCH_XFER_COMPLETE_NOTERROR);
        MMC_PRINT("[XFER COMPLETE]cur r is 0x%x\r\n", r.d32);
#ifdef _USE_SDMA
        if (r.bit.dma_interrupt == 1 && r.bit.xfer_complete != 1) {
            unsigned long Buffer_SingleBlock = REG_READ(&(Ptr->sdmasa_r));
            r.d32 = 0;
            r.bit.dma_interrupt = 0x1;
            REG_WRITE(&(Ptr->error_int_stat_r__normal_int_stat.d32), r.d32);
            REG_WRITE(&(Ptr->sdmasa_r), Buffer_SingleBlock);
        }
#endif
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_XFER_COMPLETE_TIMEOUT)) {
            r.bit.xfer_complete = 0;
        }
    }
    MMC_BRANCHTEST_PRINT(BRANCH_XFER_COMPLETE_DONE);

    if (Mtimer_IsTimerOut(&MmcMtimer)) {
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_XFER_COMPLETE_TIMEOUT)) {
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_XFER_COMPLETE_TIMEOUT);
        }
        MMC_BRANCHTEST_PRINT(BRANCH_XFER_COMPLETE_TIMEOUT);
        return MMC_XFER_TIMEOUT;
    } else {
        MMC_BRANCHTEST_PRINT(BRANCH_XFER_COMPLETE_SUCCESS);
        r.d32               = 0;
        r.bit.xfer_complete = 0x1;
        REG_WRITE(&(Ptr->error_int_stat_r__normal_int_stat.d32), r.d32);
        return MMC_SUCCESS;
    }
}

/**
 * @brief transfer block without dma
 * 
 * @param Ptr host controller parameter structure
 * @param Addr read/write addr
 * @param Err cur cmd error code
 * @return uint32_t status
 */
uint32_t TransferWithoutDMA(volatile DWC_mshc_block_registers* Ptr, uint32_t *Addr, MMC_ERR_TYPE Err)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    uint32_t *bufaddr   = Addr;
    uint32_t tranfernum = 0;
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r  = {.d32 = 0,};
    __IO BLOCKCOUNT_R__BLOCKSIZE_R r1           = {.d32 = 0,};

    if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_NOT_XFER_CMD)) {
        Err = MMC_CMD_XFER_ERR;
        ERROR_BRANCH_BIT_SET(BERROR_BRANCH_NOT_XFER_CMD);
    }
    if ((Err != MMC_CMD_17_XFER_ERR) && (Err != MMC_CMD_8_XFER_ERR) && (Err != MMC_CMD_24_XFER_ERR)) {
        MMC_BRANCHTEST_PRINT(BRANCH_NOT_XFER_CMD);
        return MMC_CMD_XFER_ERR;
    }
    MMC_BRANCHTEST_PRINT(BRANCH_IS_XFER_CMD);    

    MTIMER_OUT_CONDITION(MMC_BUF_RD_RDY_TIMEOUT_VAL, &MmcMtimer, 1) {
        MMC_BRANCHTEST_PRINT(BRANCH_BUF_RDY_REG_READ);
        r.d32 = REG_READ(&(Ptr->error_int_stat_r__normal_int_stat.d32));

        status = ErrStatCheck(r);
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_BUF_RDY_ERROR)) {
            status = MMC_FAILURE;
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_BUF_RDY_ERROR);
        }
        if (status != MMC_SUCCESS) {
            MMC_BRANCHTEST_PRINT(BRANCH_BUF_RDY_ERROR);
            return Err;
        }
        MMC_BRANCHTEST_PRINT(BRANCH_BUF_RDY_NOTERROR);
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_BUF_RDY_TIMEOUT)) {
            r.bit.buf_rd_ready = 0;
            r.bit.buf_wr_ready = 0;
        } else if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_RDCMD17BUF_RDY_NOT_READY) && \
                    Err == MMC_CMD_17_XFER_ERR) {
            r.bit.buf_rd_ready = 0;
        } else if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_RDCMD8BUF_RDY_NOT_READY) && \
                    Err == MMC_CMD_8_XFER_ERR) {
            r.bit.buf_rd_ready = 0;
        } else if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_WRCMD24BUF_RDY_NOT_READY) && \
                    Err == MMC_CMD_24_XFER_ERR) {
            r.bit.buf_wr_ready = 0;
        }
        if (Err == MMC_CMD_17_XFER_ERR) {
            if (r.bit.buf_rd_ready == 1) {
                MMC_BRANCHTEST_PRINT(BRANCH_RDCMD17_AND_RDBUF_READY);
                break;
            } else {
                if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_RDCMD17BUF_RDY_NOT_READY)) {
                    ERROR_BRANCH_BIT_SET(BERROR_BRANCH_RDCMD17BUF_RDY_NOT_READY);
                }
                MMC_BRANCHTEST_PRINT(BRANCH_RDCMD17BUF_RDY_NOT_READY);
                continue;
            }
        } else if (Err == MMC_CMD_8_XFER_ERR) {
            if (r.bit.buf_rd_ready == 1) {
                MMC_BRANCHTEST_PRINT(BRANCH_RDCMD8_AND_RDBUF_READY);
                break;
            } else {
                if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_RDCMD8BUF_RDY_NOT_READY)) {
                    ERROR_BRANCH_BIT_SET(BERROR_BRANCH_RDCMD8BUF_RDY_NOT_READY);
                }
                MMC_BRANCHTEST_PRINT(BRANCH_RDCMD8BUF_RDY_NOT_READY);
                continue;
            }
        } else {
            if (r.bit.buf_wr_ready == 1) {
                MMC_BRANCHTEST_PRINT(BRANCH_WRCMD24_AND_WRBUF_READY);
                break;
            } else {
                if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_WRCMD24BUF_RDY_NOT_READY)) {
                    ERROR_BRANCH_BIT_SET(BERROR_BRANCH_WRCMD24BUF_RDY_NOT_READY);
                }
                MMC_BRANCHTEST_PRINT(BRANCH_WRCMD24BUF_RDY_NOT_READY);
                continue;
            }
        }

    }
    MMC_BRANCHTEST_PRINT(BRANCH_BUF_RDY_DONE);

    if (Mtimer_IsTimerOut(&MmcMtimer)) {
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_BUF_RDY_TIMEOUT)) {
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_BUF_RDY_TIMEOUT);
        }
        MMC_BRANCHTEST_PRINT(BRANCH_BUF_RDY_TIMEOUT);
        return MMC_BUF_RDWR_RDY_TIMEOUT;
    } else {
        MMC_BRANCHTEST_PRINT(BRANCH_BUF_RDY_SUCCESS);
        r.d32 = 0;
        if (Err != MMC_CMD_24_XFER_ERR) {
            MMC_BRANCHTEST_PRINT(BRANCH_BUF_RDY_CLEAR_RDRDY);
            r.bit.buf_rd_ready = 0x1;
        } else {
            MMC_BRANCHTEST_PRINT(BRANCH_BUF_RDY_CLEAR_WRRDY);
            r.bit.buf_wr_ready = 0x1;
        }
        REG_WRITE(&(Ptr->error_int_stat_r__normal_int_stat.d32), r.d32);

        r1.d32 = REG_READ(&(Ptr->blockcount_r__blocksize.d32));
        tranfernum = (r1.bit.xfer_block_size >> 2);
        if (Err != MMC_CMD_24_XFER_ERR) {
            MMC_BRANCHTEST_PRINT(BRANCH_READ_DATA_BUF);
            for (int i = 0; i < tranfernum; i++) {
                *(uint32_t *)(bufaddr+i) = REG_READ(&(Ptr->buf_data));
            }
        } else {
            MMC_BRANCHTEST_PRINT(BRANCH_WRITE_DATA_BUF);
            for (int i = 0; i < tranfernum; i++) {
				REG_WRITE(&(Ptr->buf_data), *(uint32_t *)(bufaddr+i));
			}
        }
    }

	MMC_WAIT_TRANSFER_COMPLETE(Ptr, Err);

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
    uint32_t status                             = MMC_SUCCESS;
    __IO SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r    = {.d32 = 0,};

    MTIMER_OUT_CONDITION(MMC_WAIT_CLK_STABLE_TIMEOUT_VAL, &MmcMtimer, r.bit.internal_clk_stable != 1) {
        MMC_BRANCHTEST_PRINT(BRANCH_CLKSTABLE_REG_READ);
        r.d32 = REG_READ(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_CLKSTABLE_TIMEOUT)) {
            r.bit.internal_clk_stable = 0;
        }
    }
    MMC_BRANCHTEST_PRINT(BRANCH_CLKSTABLE_DONE);

    if (Mtimer_IsTimerOut(&MmcMtimer)) {
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_CLKSTABLE_TIMEOUT)) {
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_CLKSTABLE_TIMEOUT);
        }
        MMC_BRANCHTEST_PRINT(BRANCH_CLKSTABLE_TIMEOUT);
        status = MMC_WAIT_CLK_STABLE_TIMEOUT;
    } else {
        MMC_BRANCHTEST_PRINT(BRANCH_CLKSTABLE_SUCCESS);
        status = MMC_SUCCESS;
    }

    return status;
}

/**
 * @brief check CMD_INHIBIT and CMD_INHIBIT_DAT bit for line ready
 * 
 * @param Ptr host controller parameter structure
 * @return uint32_t 
 */
uint32_t CheckLineInhibit(volatile DWC_mshc_block_registers* Ptr)
{
    uint32_t status     = MMC_SUCCESS;
    __IO PSTATE_REG_R r = {.d32 = 0,};

    r.d32 = REG_READ(&(Ptr->pstate_reg.d32));
    if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_LINEINHIBIT_REG_READ)) {
        r.bit.cmd_inhibit = 1;
        ERROR_BRANCH_BIT_SET(BERROR_BRANCH_LINEINHIBIT_REG_READ);
    }
    MTIMER_OUT_CONDITION(MMC_CHECK_LINE_INHIBIT_TIMEOUT_VAL, &MmcMtimer, \
                        (r.bit.cmd_inhibit == 1 || r.bit.cmd_inhibit_dat == 1) ) {
        MMC_BRANCHTEST_PRINT(BRANCH_LINEINHIBIT_REG_READ);
        r.d32 = REG_READ(&(Ptr->pstate_reg.d32));
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_LINEINHIBIT_CMD_NOTREADY)) {
            r.bit.cmd_inhibit = 1;
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_LINEINHIBIT_CMD_NOTREADY);
        } else if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_LINEINHIBIT_DATA_NOTREADY)) {
            r.bit.cmd_inhibit_dat = 1;
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_LINEINHIBIT_DATA_NOTREADY);
        } else if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_LINEINHIBIT_CND_DATA_NOTREADY)) {
            r.bit.cmd_inhibit = 1;
            r.bit.cmd_inhibit_dat = 1;
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_LINEINHIBIT_CND_DATA_NOTREADY);
        } else if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_LINEINHIBIT_TIMEOUT)) {
            r.bit.cmd_inhibit = 1;
            r.bit.cmd_inhibit_dat = 1;
        }
        if ((r.bit.cmd_inhibit == 1) && (r.bit.cmd_inhibit_dat == 0)) {
            MMC_BRANCHTEST_PRINT(BRANCH_LINEINHIBIT_CMD_NOTREADY);
        } else if((r.bit.cmd_inhibit == 0) && (r.bit.cmd_inhibit_dat == 1)) {
            MMC_BRANCHTEST_PRINT(BRANCH_LINEINHIBIT_DATA_NOTREADY);
        } else if((r.bit.cmd_inhibit == 1) && (r.bit.cmd_inhibit_dat == 1)) {
            MMC_BRANCHTEST_PRINT(BRANCH_LINEINHIBIT_CMD_DATA_NOTREADY);
        }
    }
    MMC_BRANCHTEST_PRINT(BRANCH_LINEINHIBIT_DONE);

    if (Mtimer_IsTimerOut(&MmcMtimer)) {
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_LINEINHIBIT_TIMEOUT)) {
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_LINEINHIBIT_TIMEOUT);
        }
        MMC_BRANCHTEST_PRINT(BRANCH_LINEINHIBIT_TIMEOUT);
        status = MMC_WAIT_LINE_INHIBIT_TIMEOUT;
    } else {
        MMC_BRANCHTEST_PRINT(BRANCH_LINEINHIBIT_SUCCESS);
        status = MMC_SUCCESS;
    }

    return status;
}

/**
 * @brief clear ERROR_INT_STAT_R and NORMAL_INT_STAT_R register
 * 
 * @param Ptr host controller parameter structure
 */
void ClearErrandIntStatus(volatile DWC_mshc_block_registers* Ptr)
{
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r = {.d32 = 0,};

    r.d32 = REG_READ(&(Ptr->error_int_stat_r__normal_int_stat.d32));
    MMC_PRINT("err and int stat is %x\r\n", r.d32);
    REG_WRITE(&(Ptr->error_int_stat_r__normal_int_stat.d32), ~0);
}

/**
 * @brief read efuse delay value for increase delay somewhere
 * 
 */
void EfuseDelayVlueCheck(void)
{
    uint32_t reg = REG_READ(EFUSE_NVM_DELAY_REG);
    EfuseDelayParam = (((reg >> 16) | (reg)) & 0xFFFF);
    MMC_PRINT("EFUSE_NVM_DELAY_REG is 0x%x\r\n", reg);
    MMC_PRINT("EfuseDelayParam is %d\r\n", EfuseDelayParam);
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
    __IO WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1   = {.d32 = 0,};
    __IO SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r2                   = {.d32 = 0,};

    MMC_PRINT("HostControllerSetup\r\n");   //sequence path print

    r1.d32                  = 0;
#ifdef _USE_SDMA
    r1.bit.dma_sel          = MMC_HC1_DMA_SEL_SDMA;  //SDMA
#endif
    r1.bit.sd_bus_pwr_vdd1  = MMC_PC_SBP_VDD1_ON;    //VDD1 PWR ON support SD and eMMC
    IoBank1Ref = REG_READ(IO_BANK1_REF);
    if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_IOBANK1_1V8)) {
        IoBank1Ref |= 0x1;
        ERROR_BRANCH_BIT_SET(BERROR_BRANCH_IOBANK1_1V8);
    }
    MMC_PRINT("IoBank1Ref is %x\r\n", IoBank1Ref);
    if (MMC_IO_BANK1_SUPPORT_1V8(IoBank1Ref) == 1) {
        MMC_BRANCHTEST_PRINT(BRANCH_IOBANK1_1V8);
        r1.bit.sd_bus_vol_vdd1 = EMMC_PC_SBV_VDD1_1V8;   //1.8V
    } else {
        MMC_BRANCHTEST_PRINT(BRANCH_IOBANK1_3V3);
        r1.bit.sd_bus_vol_vdd1 = MMC_PC_SBV_VDD1_3V3;   //3.3V
    }
    REG_WRITE(&(Ptr->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32), r1.d32);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

    r2.d32 = REG_READ(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
    r2.bit.tout_cnt = MMC_TC_TOUT_CNT_2_27;
    REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r2.d32);
    MMC_PRINT("r2.d32 is %x\r\n", r2.d32);

    MMC_DELAY_MS(40);   //1msec+0.1~35msec+1msec+(74 clock)

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
uint32_t InitInterruptSetting(volatile DWC_mshc_block_registers* Ptr) {
    MMC_ERR_TYPE status                                 = MMC_SUCCESS;
    __IO ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R r1   = {.d32 = 0,};

    MMC_PRINT("InitInterruptSetting\r\n");
    r1.d32                          = 0;
    r1.bit.cmd_complete_stat_en     = MMC_NORMAL_INT_STAT_EN;
    r1.bit.xfer_complete_stat_en    = MMC_NORMAL_INT_STAT_EN;
    r1.bit.bgap_event_stat_en       = MMC_NORMAL_INT_STAT_EN;
#ifdef _USE_SDMA
    r1.bit.dma_interrupt_stat_en    = MMC_NORMAL_INT_STAT_EN;
#endif
    r1.bit.buf_wr_ready_stat_en     = MMC_NORMAL_INT_STAT_EN;
    r1.bit.buf_rd_ready_stat_en     = MMC_NORMAL_INT_STAT_EN;
    r1.bit.int_a_stat_en            = MMC_NORMAL_INT_STAT_EN;
    r1.bit.cmd_tout_err_stat_en     = MMC_ERR_INT_STAT_EN;
    r1.bit.cmd_crc_err_stat_en      = MMC_ERR_INT_STAT_EN;
    r1.bit.cmd_end_bit_err_stat_en  = MMC_ERR_INT_STAT_EN;
    r1.bit.cmd_idx_err_stat_en      = MMC_ERR_INT_STAT_EN;
    r1.bit.data_tout_err_stat_en    = MMC_ERR_INT_STAT_EN;
    r1.bit.data_crc_err_stat_en     = MMC_ERR_INT_STAT_EN;
    r1.bit.data_end_bit_err_stat_en = MMC_ERR_INT_STAT_EN;
    REG_WRITE(&(Ptr->error_int_stat_en_r__normal_int_stat_en.d32), r1.d32);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

    return status;
}

uint32_t ResetHostComtroller(volatile DWC_mshc_block_registers* Ptr)
{
     __IO SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r1 = {.d32 = 0,};

    r1.d32 = REG_READ(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
    MMC_PRINT("[CMD COMPLETE]rst r is 0x%x\r\n", r1.d32);
    //reset all
    r1.bit.sw_rst_all = 0x1;
    REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
    do {
        r1.d32 = REG_READ(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
    } while(r1.bit.sw_rst_all == 1);

    return 0;
}

/*********************************************END OF FILE**********************/
