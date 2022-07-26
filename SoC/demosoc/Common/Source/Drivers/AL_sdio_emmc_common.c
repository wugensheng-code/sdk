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
#include "AL_sdio_emmc_common.h"


#define AT_CTRL_R 	0x540
#define MBIU_CTRL_R 0x510
#define TOP_NS__CFG_CTRL_SDIO0_ADDR 0xF8800154ULL
#define SDIO_WRAP__SDIO0__BASE_ADDR 0xF8049000ULL
#define SDIO_WRAP__SDIO1__BASE_ADDR 0xF804A000ULL

SD_CardInfo SDCardInfo;
uint32_t CSD_Tab[4], CID_Tab[4], RCA = 1;
volatile MtimerParams Mtimer;
static uint32_t CardType =  SDIO_HIGH_CAPACITY_SD_CARD;

volatile DWC_mshc_block_registers* SDIO = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO1__BASE_ADDR;
volatile DWC_mshc_block_registers* eMMC = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;


unsigned int reg_read(unsigned long reg_address)
{
    return *((volatile unsigned int *)reg_address);
}

void reg_write(unsigned long reg_address, u32 reg_wdata)
{
    *((volatile unsigned int *)reg_address) = reg_wdata;
}

u32 wait_command_complete(volatile DWC_mshc_block_registers* ptr)
{
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r;
    MTIMER_OUT_CONDITION(100, &Mtimer, r.bit.cmd_complete != 1){
        r.d32 = REG_READ(&(ptr->error_int_stat_r__normal_int_stat));
    }
    if(Mtimer_IsTimerOut(&Mtimer)){
        return SD_EMMC_CMD_TIMEOUT;
    }else{
        return AL_SUCCESS;
    }
}

u32 wait_transfer_complete(volatile DWC_mshc_block_registers* ptr)
{
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r;
    MTIMER_OUT_CONDITION(100, &Mtimer, r.bit.xfer_complete != 1){
        r.d32 = REG_READ(&(ptr->error_int_stat_r__normal_int_stat));
    }
    if(Mtimer_IsTimerOut(&Mtimer)){
        return SD_EMMC_XFER_TIMEOUT;
    }else{
        return AL_SUCCESS;
    }
}

u32 wait_buffer_read_ready_complete(volatile DWC_mshc_block_registers* ptr)
{
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r;
    MTIMER_OUT_CONDITION(100, &Mtimer, r.bit.buf_rd_ready != 1){
        r.d32 = REG_READ(&(ptr->error_int_stat_r__normal_int_stat));
    }
    if(Mtimer_IsTimerOut(&Mtimer)){
        return SD_EMMC_BUF_RD_RDY_TIMEOUT;
    }else{
        return AL_SUCCESS;
    }
}

/***************************************************************************/
/**
 * @brief	init the SD/EMMC host controller
 *
 * @param	None
 * @return	AL_SUCCESS
 *
 ******************************************************************************/
u32 HostControllerSetup(volatile DWC_mshc_block_registers* ptr)
{
    u32 Status;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r2;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r3;

    r1.d32 = 0;
    r1.bit.dma_sel = 0x2;   //ADMA2
    r1.bit.sd_bus_pwr_vdd1 = 0x1;   //VDD1 PWR ON support SD and eMMC
    r1.bit.sd_bus_vol_vdd1 = 0x7;   //3V
    //r1.bit.sd_bus_pwr_vdd2 = 0x1;   //VDD2 PWR ON   only support for UHS-II
    //r1.bit.sd_bus_vol_vdd2 = 0x5;   //1.8V
    REG_WRITE(&(ptr->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);

    r2.d32 = 0;
    r2.bit.internal_clk_en = 0x1;       //Oscillate
    r2.bit.internal_clk_stable = 0x1;   // read only why read
    r2.bit.pll_enable = 0x1;            //PLL enabled
    r2.bit.clk_gen_select = 0x1;        //Programmable Clock Mode
    REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r2.d32);


    //r3.d32 = 0; //host_ctrl2 default 0, auto_cmd_stat is read only
    //REG_WRITE(&(ptr->host_ctrl2_r__auto_cmd_stat), r3.d32);

    Status = AL_SUCCESS;
    return Status;
}

/***************************************************************************/
/**
 * @brief	set the SD/EMMC host clock 
 *
 * @param	None
 * @return	AL_SUCCESS
 *
 ******************************************************************************/
u32 HostControllerClockSetup(volatile DWC_mshc_block_registers* ptr, int freq)
{
    SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r1;
    //  Host Controller Clock Setup
    if (freq == FREQ_10M)
    {
        r1.d32 = 0;
        r1.bit.internal_clk_en = 0x1;       //Oscillate
        r1.bit.internal_clk_stable = 0x1;   // ro why read
        r1.bit.pll_enable = 0x1;            //PLL enabled
        r1.bit.clk_gen_select = 0x1;        //Programmable Clock Mode
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r1.d32);

        //ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000002B;
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);     //配合bitfile
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);

        r1.d32 = 0;
        r1.bit.internal_clk_en = 0x1;       //Oscillate
        r1.bit.internal_clk_stable = 0x1;   // ro why read
        r1.bit.sd_clk_en = 0x1;             //Enable SDCLK/RCLK
        r1.bit.pll_enable = 0x1;            //PLL enabled
        r1.bit.clk_gen_select = 0x1;        //Programmable Clock Mode
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r1.d32);

        //ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000002F;
        ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000002F;
    }else{
        r1.d32 = 0;
        r1.bit.internal_clk_en = 0x1;       //Oscillate
        r1.bit.internal_clk_stable = 0x1;   // ro why read
        r1.bit.pll_enable = 0x1;            //PLL enabled
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r1.d32);
        //ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000000B;
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);
        r1.d32 = 0;
        r1.bit.internal_clk_en = 0x1;       //Oscillate
        r1.bit.internal_clk_stable = 0x1;   // ro why read
        r1.bit.sd_clk_en = 0x1;             //Enable SDCLK/RCLK
        r1.bit.pll_enable = 0x1;            //PLL enabled
        REG_WRITE(&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r1.d32);
        //ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000000F;
        ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000000F;  //配合bitfile
    }

    return AL_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	set interrupt controller
 *
 * @param	None
 * @return	AL_SUCCESS
 *
 ******************************************************************************/
u32 InitInterruptSetting(volatile DWC_mshc_block_registers* ptr)
{
    ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R r1;
    ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN_R r2;
    ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R r3;

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
    REG_WRITE(&(ptr->error_int_stat_en_r__normal_int_stat_en), r1.d32);
    //ptr->error_int_stat_en_r__normal_int_stat_en.d32 = 0x000002FF;
    r2.d32 = 0;
    r2.bit.cmd_complete_signal_en = 0x1;
    r2.bit.xfer_complete_signal_en = 0x1;
    r2.bit.bgap_event_signal_en = 0x1;
    r2.bit.dma_interrupt_signal_en = 0x1;
    r2.bit.buf_wr_ready_signal_en = 0x1;
    r2.bit.buf_rd_ready_signal_en = 0x1;
    r2.bit.card_insertion_signal_en = 0x1;
    r2.bit.card_removal_signal_en = 0x1;
    REG_WRITE(&(ptr->error_int_signal_en_r__normal_int_signal_en), r2.d32);
    //ptr->error_int_signal_en_r__normal_int_signal_en.d32 = 0x000000FF;
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
    r3.bit.cmd_idx_err_stat_en = 0x1;
    r3.bit.data_tout_err_stat_en = 0x1;
    r3.bit.data_crc_err_stat_en = 0x1;
    r3.bit.data_end_bit_err_stat_en = 0x1;
    r3.bit.cur_lmt_err_stat_en = 0x1;
    REG_WRITE(&(ptr->error_int_stat_en_r__normal_int_stat_en), r3.d32);
    //ptr->error_int_stat_en_r__normal_int_stat_en.d32 = 0x00FB02FF;
    ptr->host_ctrl2_r__auto_cmd_stat.d32 = 0x00000000;

    SDRegWrite(AT_CTRL_R, 0x0FFF0000);
    SDRegWrite(MBIU_CTRL_R, 0x01010004);

    return AL_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	Returns information about specific card.
 *
 * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD card
 *         information.
 * @return	AL_SUCCESS
 *
 ******************************************************************************/
u32 SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  u32 errorstatus = AL_SUCCESS;
  uint8_t tmp = 0;

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

  if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
  {
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
  }
  else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
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

  return(errorstatus);
}
/*********************************************END OF FILE**********************/
