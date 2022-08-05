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
#include "AL_mmc.h"
#include "mtimer.h"

#define MBIU_CTRL_R 0x510   //AHB BUS burst contrl register
//top register
//bit 7:enable reg ctrl card write protection   0:io ctrl   1:reg ctrl
//bit 6:enable reg ctrl card detection          0:io ctrl   1:reg ctrl
//bit 5:reg ctrl write protection               0:disable   1:enable
//bit 4:reg ctrl card detect                    0:enable    1:disable
//bit 3:clk soft rst                            0:disable   1:enable
//bit [2:0]:clk phase select similar to tuning
#define TOP_NS__CFG_CTRL_SDIO0_ADDR 0xF8800154ULL 
#define TOP_NS__CFG_CTRL_ADDR       0xF8800150  
#define SDIO_WRAP__SDIO0__BASE_ADDR 0xF8049000ULL
#define SDIO_WRAP__SDIO1__BASE_ADDR 0xF804A000ULL

SD_CardInfo SDCardInfo;
uint32_t CSD_Tab[4], CID_Tab[4], RCA = 1;
volatile MtimerParams mtimer;
static uint32_t CardType =  SDIO_HIGH_CAPACITY_SD_CARD;

volatile DWC_mshc_block_registers* SDIO = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;
volatile DWC_mshc_block_registers* eMMC = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;


inline unsigned int reg_read(unsigned long reg_address)
{
    return *((volatile unsigned int *)reg_address);
}

inline void reg_write(unsigned long reg_address, u32 reg_wdata)
{
    *((volatile unsigned int *)reg_address) = reg_wdata;
}

static void error_stat_print(uint32_t err_state)
{
    printf("error stat print: ");
    switch(err_state){
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

void error_stat_check(volatile ERROR_INT_STAT_R__NORMAL_INT_STAT_R r)
{
    uint32_t i = 0;
    uint32_t errorbits = r.d32>>0x10;
    for(;i < MMC_ERR_INT_STAT_BITS_LEN; i++){
        if(((errorbits>>i)&0x1) == 1)
            error_stat_print(i);
    }
}

u32 wait_command_complete(volatile DWC_mshc_block_registers* ptr)
{
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r;
    MTIMER_OUT_CONDITION(MMC_CMD_TIMEOUT_VAL, &mtimer, r.bit.cmd_complete != 1){
        r.d32 = REG_READ(&(ptr->error_int_stat_r__normal_int_stat.d32));
    }
    if(Mtimer_IsTimerOut(&mtimer)){
        error_stat_check(r);
        return MMC_CMD_TIMEOUT;
    }else{
        r.d32 = REG_READ(&(ptr->error_int_stat_r__normal_int_stat.d32));
        r.bit.cmd_complete = 0x1;
        REG_WRITE(&(ptr->error_int_stat_r__normal_int_stat.d32), r.d32);
        return MMC_SUCCESS;
    }
}

u32 wait_transfer_complete(volatile DWC_mshc_block_registers* ptr)
{
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r;
    MTIMER_OUT_CONDITION(MMC_XFER_TIMEOUT_VAL, &mtimer, r.bit.xfer_complete != 1){
        r.d32 = REG_READ(&(ptr->error_int_stat_r__normal_int_stat.d32));
    }
    if(Mtimer_IsTimerOut(&mtimer)){
        error_stat_check(r);
        return MMC_XFER_TIMEOUT;
    }else{
        r.d32 = REG_READ(&(ptr->error_int_stat_r__normal_int_stat.d32));
        r.bit.xfer_complete = 0x1;
        REG_WRITE(&(ptr->error_int_stat_r__normal_int_stat.d32), r.d32);
        return MMC_SUCCESS;
    }
}

u32 wait_buffer_read_ready_complete(volatile DWC_mshc_block_registers* ptr)
{
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r;
    MTIMER_OUT_CONDITION(MMC_BUF_RD_RDY_TIMEOUT_VAL, &mtimer, r.bit.buf_rd_ready != 1){
        r.d32 = REG_READ(&(ptr->error_int_stat_r__normal_int_stat.d32));
    }
    if(Mtimer_IsTimerOut(&mtimer)){
        error_stat_check(r);
        return MMC_BUF_RD_RDY_TIMEOUT;
    }else{
        r.d32 = REG_READ(&(ptr->error_int_stat_r__normal_int_stat.d32));
        r.bit.buf_rd_ready = 0x1;
        REG_WRITE(&(ptr->error_int_stat_r__normal_int_stat.d32), r.d32);
        return MMC_SUCCESS;
    }
}

u32 wait_clock_stable(volatile DWC_mshc_block_registers* ptr)
{
    __IO SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r;
    MTIMER_OUT_CONDITION(MMC_WAIT_CLK_STABLE_TIMEOUT_VAL, &mtimer, r.bit.internal_clk_stable != 1){
        r.d32 = REG_READ(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
    }
    if(Mtimer_IsTimerOut(&mtimer)){
        return MMC_WAIT_CLK_STABLE_TIMEOUT;
    }else{
        return MMC_SUCCESS;
    }
}

u32 set_clock_frequency(volatile DWC_mshc_block_registers* ptr)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    CAPABILITIES1_R r1;
    CAPABILITIES2_R r2;
    MMC_PRINT("set_clock_frequency\r\n");   //sequence path print

    r1.d32 = REG_READ(&(ptr->capabilities1.d32));

    if(r1.bit.base_clk_freq == MMC_GET_INFO_ANOTHER_WAY){
        printf("Get base clk freq information through another method\r\n");
    }else{
        printf("base clk freq is %d\r\n", r1.bit.base_clk_freq);
    }

    r2.d32 = REG_READ(&(ptr->capabilities2));
    if(r2.bit.clk_mul == MMC_CLK_MUL_NOT_SUPPORT){
        printf("Clock Multiplier is not Supported");
    }else{
        printf("clock multiplier is %d", r2.bit.clk_mul+1);
    }
    
    //Set CLK_CTRL_R.FREQ_SEL and CLK_CTRL_R.CLK_GEN_SELECT as per results in (1)
    return status;
}

/***************************************************************************/
/**
 * @brief	init the SD/EMMC host controller
 *
 * @param	None
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 HostControllerSetup(volatile DWC_mshc_block_registers* ptr)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r2;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r3;
    MMC_PRINT("HostControllerSetup\r\n");   //sequence path print

    r1.d32 = 0;
    r1.bit.dma_sel = MMC_HC1_DMA_SEL_ADMA2;   //ADMA2
    r1.bit.sd_bus_pwr_vdd1 = MMC_PC_SBP_VDD1_ON;   //VDD1 PWR ON support SD and eMMC
    r1.bit.sd_bus_vol_vdd1 = MMC_PC_SBV_VDD1_3V3;   //3.3V
    REG_WRITE(&(ptr->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32), r1.d32);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

    /*status = set_clock_frequency(ptr);
    if(status != MMC_SUCCESS){
        return status;
    }*/
    r2.d32 = 0;
    r2.bit.internal_clk_en = 0x1;       //Oscillate
    r2.bit.clk_gen_select = 0x1;        //Programmable Clock Mode
    REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r2.d32);
    MMC_WAIT_CLK_STABLE(ptr);
    r2.d32 = REG_READ(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl));
    r2.bit.pll_enable = 0x1;            //PLL enabled
    REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r2.d32);
    MMC_WAIT_CLK_STABLE(ptr);
    MMC_PRINT("r2.d32 is %x\r\n", r2.d32);

    r3.d32 = 0; //Do not set Version 4 Parameters
    REG_WRITE(&(ptr->host_ctrl2_r__auto_cmd_stat.d32), r3.d32);
    MMC_PRINT("r3.d32 is %x\r\n", r3.d32);
    return status;
}

/***************************************************************************/
/**
 * @brief	set the SD/EMMC host clock 
 *
 * @param	None
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 HostControllerClockSetup(volatile DWC_mshc_block_registers* ptr, uint32_t freq)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r1;
    MMC_PRINT("HostControllerClockSetup\r\n");

    //  Host Controller Clock Setup
    if (freq == MMC_FREQ_10M)
    {
        MMC_PRINT("MMC_FREQ_10M\r\n");
        r1.d32 = 0;
        r1.bit.internal_clk_en = 0x1;       //Oscillate
        r1.bit.clk_gen_select = 0x1;        //Programmable Clock Mode
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r1.d32);
        MMC_WAIT_CLK_STABLE(ptr);
        r1.d32 = REG_READ(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl));
        r1.bit.pll_enable = 0x1;            //PLL enabled
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(ptr);
        MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);     //clk soft reset
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);

        r1.d32 = 0;
        r1.bit.internal_clk_en = 0x1;       //Oscillate
        r1.bit.clk_gen_select = 0x1;        //Programmable Clock Mode
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(ptr);
        r1.d32 = REG_READ(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
        r1.bit.pll_enable = 0x1;            //PLL enabled
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(ptr);
        r1.d32 = REG_READ(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
        r1.bit.sd_clk_en = 0x1;             //Enable SDCLK/RCLK
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_PRINT("r1.d32 is %x\r\n", r1.d32);
    }else{
        MMC_PRINT("MMC_FREQ_400K\r\n");
        r1.d32 = 0;
        r1.bit.internal_clk_en = 0x1;       //Oscillate
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(ptr);
        r1.d32 = REG_READ(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
        r1.bit.pll_enable = 0x1;            //PLL enabled
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(ptr);
        MMC_PRINT("r1.d32 is %x\r\n", r1.d32);


        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);
        REG_WRITE(TOP_NS__CFG_CTRL_ADDR, 0x000000b0);
        
        r1.d32 = 0;
        r1.bit.internal_clk_en = 0x1;       //Oscillate
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(ptr);
        r1.d32 = REG_READ(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
        r1.bit.pll_enable = 0x1;            //PLL enabled
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(ptr);
        r1.d32 = REG_READ(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
        r1.bit.sd_clk_en = 0x1;             //Enable SDCLK/RCLK
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_PRINT("r1.d32 is %x\r\n", r1.d32);
    }
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
u32 InitInterruptSetting(volatile DWC_mshc_block_registers* ptr)
{
    uint32_t status = MMC_SUCCESS;
    ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R r1;
    ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN_R r2;
    ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R r3;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r4;

    MMC_PRINT("InitInterruptSetting\r\n");
    r1.d32 = 0;
    r1.bit.cmd_complete_stat_en = 0x1;
    r1.bit.xfer_complete_stat_en = 0x1;
    r1.bit.bgap_event_stat_en = 0x1;
    r1.bit.dma_interrupt_stat_en = 0x1;
    r1.bit.buf_wr_ready_stat_en = 0x1;
    r1.bit.buf_rd_ready_stat_en = 0x1;
    r1.bit.card_insertion_stat_en = 0x1;
    r1.bit.card_removal_stat_en = 0x1;
    r1.bit.int_a_stat_en = 0x1;
    REG_WRITE(&(ptr->error_int_stat_en_r__normal_int_stat_en.d32), r1.d32);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

    r2.d32 = 0;
    r2.bit.cmd_complete_signal_en = 0x1;
    r2.bit.xfer_complete_signal_en = 0x1;
    r2.bit.bgap_event_signal_en = 0x1;
    r2.bit.dma_interrupt_signal_en = 0x1;
    r2.bit.buf_wr_ready_signal_en = 0x1;
    r2.bit.buf_rd_ready_signal_en = 0x1;
    r2.bit.card_insertion_signal_en = 0x1;
    r2.bit.card_removal_signal_en = 0x1;
    REG_WRITE(&(ptr->error_int_signal_en_r__normal_int_signal_en.d32), r2.d32);
    MMC_PRINT("r2.d32 is %x\r\n", r2.d32);

    r3.d32 = 0;
    r3.bit.cmd_complete_stat_en = 0x1;
    r3.bit.xfer_complete_stat_en = 0x1;
    r3.bit.bgap_event_stat_en = 0x1;
    r3.bit.dma_interrupt_stat_en = 0x1;
    r3.bit.buf_wr_ready_stat_en = 0x1;
    r3.bit.buf_rd_ready_stat_en = 0x1;
    r3.bit.card_insertion_stat_en = 0x1;
    r3.bit.card_removal_stat_en = 0x1;
    r3.bit.int_a_stat_en = 0x1;
    r3.bit.cmd_tout_err_stat_en = 0x1;
    r3.bit.cmd_crc_err_stat_en = 0x1;
    r3.bit.cmd_end_bit_err_stat_en = 0x1;
    r3.bit.cmd_idx_err_stat_en = 0x1;
    r3.bit.data_tout_err_stat_en = 0x1;
    r3.bit.data_crc_err_stat_en = 0x1;
    r3.bit.data_end_bit_err_stat_en = 0x1;
    r3.bit.cur_lmt_err_stat_en = 0x1;
    REG_WRITE(&(ptr->error_int_stat_en_r__normal_int_stat_en.d32), r3.d32);
    MMC_PRINT("r3.d32 is %x\r\n", r3.d32);

    r4.d32 = 0; //Do not set Version 4 Parameters
    REG_WRITE(&(ptr->host_ctrl2_r__auto_cmd_stat.d32), r4.d32);
    MMC_PRINT("r4.d32 is %x\r\n", r4.d32);

    SDRegWrite(MBIU_CTRL_R, 0x01010004);    //AHB Bus Burst Mode:Undefined INCR Burst
    return status;
}

/***************************************************************************/
/**
 * @brief	Returns information about specific card.
 *
 * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD card
 *         information.
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 SD_GetCardInfo(SD_CardInfo *cardinfo)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    uint8_t tmp = 0;

    MMC_PRINT("SD_GetCardInfo\r\n");
    cardinfo->CardType = (uint8_t)CardType;
    cardinfo->RCA = (uint16_t)RCA;

    /*adjust postion*/
    CSD_Tab[0] = CSD_Tab[0] << 8;
    tmp = (CSD_Tab[1] & 0xFF000000) >> 24;
    memcpy(((uint8_t *)&CSD_Tab[0]), &tmp, 1);
    CSD_Tab[1] = CSD_Tab[1] << 8;
    tmp = (CSD_Tab[2] & 0xFF000000) >> 24;
    memcpy(((uint8_t *)&CSD_Tab[1]), &tmp, 1);
    CSD_Tab[2] = CSD_Tab[2] << 8;
    tmp = (CSD_Tab[3] & 0xFF000000) >> 24;
    memcpy(((uint8_t *)&CSD_Tab[2]), &tmp, 1);
    CSD_Tab[3] = CSD_Tab[3] << 8;

    /*!< Byte 0 */
    tmp = (uint8_t)((CSD_Tab[0] & 0xFF000000) >> 24);
    cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
    cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
    cardinfo->SD_csd.Reserved1 = tmp & 0x03;

    /*!< Byte 1 */
    tmp = (uint8_t)((CSD_Tab[0] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.TAAC = tmp;

    /*!< Byte 2 */
    tmp = (uint8_t)((CSD_Tab[0] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.NSAC = tmp;

    /*!< Byte 3 */
    tmp = (uint8_t)(CSD_Tab[0] & 0x000000FF);
    cardinfo->SD_csd.MaxBusClkFrec = tmp;

    /*!< Byte 4 */
    tmp = (uint8_t)((CSD_Tab[1] & 0xFF000000) >> 24);
    cardinfo->SD_csd.CardComdClasses = tmp << 4;

    /*!< Byte 5 */
    tmp = (uint8_t)((CSD_Tab[1] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
    cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

    /*!< Byte 6 */
    tmp = (uint8_t)((CSD_Tab[1] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
    cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
    cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
    cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
    cardinfo->SD_csd.Reserved2 = 0; /*!< Reserved */

    if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0)){
        MMC_PRINT("SD card STD catacity V1.1 or V2.0\r\n");
        cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

        /*!< Byte 7 */
        tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
        cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

        /*!< Byte 8 */
        tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
        cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

        cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
        cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

        /*!< Byte 9 */
        tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
        cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
        cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
        cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
        /*!< Byte 10 */
        tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
        cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;

        cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
        cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
        cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
        cardinfo->CardCapacity *= cardinfo->CardBlockSize;
    }else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD){
        MMC_PRINT("SD card high capacity\r\n");
        /*!< Byte 7 */
        tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
        cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

        /*!< Byte 8 */
        tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);

        cardinfo->SD_csd.DeviceSize |= (tmp << 8);

        /*!< Byte 9 */
        tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);

        cardinfo->SD_csd.DeviceSize |= (tmp);

        /*!< Byte 10 */
        tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);

        cardinfo->CardCapacity = ((uint64_t)cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
        cardinfo->CardBlockSize = 512;
    }

    cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
    cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

    /*!< Byte 11 */
    tmp = (uint8_t)(CSD_Tab[2] & 0x000000FF);
    cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
    cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

    /*!< Byte 12 */
    tmp = (uint8_t)((CSD_Tab[3] & 0xFF000000) >> 24);
    cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
    cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
    cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
    cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

    /*!< Byte 13 */
    tmp = (uint8_t)((CSD_Tab[3] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
    cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
    cardinfo->SD_csd.Reserved3 = 0;
    cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

    /*!< Byte 14 */
    tmp = (uint8_t)((CSD_Tab[3] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
    cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
    cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
    cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
    cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
    cardinfo->SD_csd.ECC = (tmp & 0x03);

    /*!< Byte 15 */
    tmp = (uint8_t)(CSD_Tab[3] & 0x000000FF);
    cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
    cardinfo->SD_csd.Reserved4 = 1;

    /*!< Byte 0 */
    tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ManufacturerID = tmp;

    /*!< Byte 1 */
    tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.OEM_AppliID = tmp << 8;

    /*!< Byte 2 */
    tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
    cardinfo->SD_cid.OEM_AppliID |= tmp;

    /*!< Byte 3 */
    tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
    cardinfo->SD_cid.ProdName1 = tmp << 24;

    /*!< Byte 4 */
    tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdName1 |= tmp << 16;

    /*!< Byte 5 */
    tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.ProdName1 |= tmp << 8;

    /*!< Byte 6 */
    tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ProdName1 |= tmp;

    /*!< Byte 7 */
    tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
    cardinfo->SD_cid.ProdName2 = tmp;

    /*!< Byte 8 */
    tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdRev = tmp;

    /*!< Byte 9 */
    tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.ProdSN = tmp << 24;

    /*!< Byte 10 */
    tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ProdSN |= tmp << 16;

    /*!< Byte 11 */
    tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
    cardinfo->SD_cid.ProdSN |= tmp << 8;

    /*!< Byte 12 */
    tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdSN |= tmp;

    /*!< Byte 13 */
    tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
    cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

    /*!< Byte 14 */
    tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ManufactDate |= tmp;

    /*!< Byte 15 */
    tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
    cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
    cardinfo->SD_cid.Reserved2 = 1;

    return status;
}
/*********************************************END OF FILE**********************/
