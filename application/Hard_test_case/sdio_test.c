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

#include "sdio_test.h"
#include <string.h>
#include <stdio.h>
#include "FATFS/ff.h"
#include "mtimer.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#define BLOCK_SIZE            512 /* Block Size in Bytes */
#define NUMBER_OF_BLOCKS      10  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)

#define TOP_NS__CFG_CTRL_SDIO0_ADDR 0xF8800154
#define SDIO_WRAP__SDIO0__BASE_ADDR 0xF8049000ULL
#define SDIO_WRAP__SDIO1__BASE_ADDR 0xF804A000ULL

#define TUBE_ADDRESS ((volatile char *) 0xF8800010u)

static uint32_t CSD_Tab[4], CID_Tab[4], RCA = 1;
static uint32_t CardType =  SDIO_HIGH_CAPACITY_SD_CARD;
FATFS fs;
FRESULT res_sd;
uint8_t flag = 0;
static unsigned int rca = 0;
SD_CardInfo SDCardInfo;
static volatile DWC_mshc_block_registers* SDIO = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;
static volatile DWC_mshc_block_registers* eMMC = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;
uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint32_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];

static void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);
static void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
static TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength);

unsigned int reg_read(unsigned int reg_address)
{
    return *((volatile unsigned int *)reg_address);
}

void reg_write(unsigned int reg_address, unsigned int reg_wdata)
{
    *((volatile unsigned int *)reg_address) = reg_wdata;
}

static void sleep(int tick)
{
    for (int i = 0; i < tick; i++)
    {
        asm volatile("NOP");
    }
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++)
  {
    pBuffer[index] = index + Offset;
  }
}

static void wait_command_complete(volatile DWC_mshc_block_registers* ptr)
{
    //ERROR_INT_STAT_R__NORMAL_INT_STAT_R reg;
    MtimerParams* Mtimer;
    Mtimer_Init(Mtimer);
    MTIMER_OUT_CONDITION(100, Mtimer, \
                        ptr->error_int_stat_r__normal_int_stat.BIT.cmd_complete != 1);
    if(Mtimer_IsTimerOut(Mtimer)){
        return XST_FAILURE;
    }else{
        ptr->error_int_stat_r__normal_int_stat.BIT.cmd_complete = 1;
        return XST_SUCCESS;
    }
    /*for (;;)
    {
        reg = ptr->error_int_stat_r__normal_int_stat;
        if (reg.BIT.cmd_complete == 1)
        {
        	ptr->error_int_stat_r__normal_int_stat.BIT.cmd_complete = 1;
            break;
        }
    }*/
}

static void wait_transfer_complete(volatile DWC_mshc_block_registers* ptr)
{
    MtimerParams* Mtimer;
    Mtimer_Init(Mtimer);
    MTIMER_OUT_CONDITION(100, Mtimer, \
                        ptr->error_int_stat_r__normal_int_stat.BIT.xfer_complete != 1);
    if(Mtimer_IsTimerOut(Mtimer)){
        return XST_FAILURE;
    }else{
        ptr->error_int_stat_r__normal_int_stat.BIT.xfer_complete = 1;
        return XST_SUCCESS;
    }
    /*ERROR_INT_STAT_R__NORMAL_INT_STAT_R reg;
    for (;;)
    {
    	reg = ptr->error_int_stat_r__normal_int_stat;
        if (reg.BIT.xfer_complete == 1)
        {
        	ptr->error_int_stat_r__normal_int_stat.BIT.xfer_complete = 1;
            break;
        }
    }*/
}

static void wait_buffer_read_ready_complete(volatile DWC_mshc_block_registers* ptr)
{
    MtimerParams* Mtimer;
    Mtimer_Init(Mtimer);
    MTIMER_OUT_CONDITION(100, Mtimer, \
                    ptr->error_int_stat_r__normal_int_stat.BIT.buf_rd_ready != 1);
    if(Mtimer_IsTimerOut(Mtimer)){
        return XST_FAILURE;
    }else{
        ptr->error_int_stat_r__normal_int_stat.BIT.buf_rd_ready = 1;
        return XST_SUCCESS;
    }
    /*ERROR_INT_STAT_R__NORMAL_INT_STAT_R reg;
    for (;;)
    {
    	reg = ptr->error_int_stat_r__normal_int_stat;
        if (reg.BIT.buf_rd_ready == 1)
        {
        	ptr->error_int_stat_r__normal_int_stat.BIT.buf_rd_ready = 1;
            break;
        }
    }*/
}

/***************************************************************************/
/**
 * @brief	check the card inserted or not inserted
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 CardDetection()
{
    u32 Status = XST_FAILURE;
    u32 CardStatus = 0;
    ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R r1;
    ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN_R r2;
    ERROR_INT_STAT_R__NORMAL_INT_STAT_R r3;
    PSTATE_REG_R r4;
    
    //  Card Detection
    r1.d32 = 0;
    r1.BIT.cmd_complete_stat_en = 0x1;
    r1.BIT.xfer_complete_stat_en = 0x1;
    r1.BIT.bgap_event_stat_en = 0x1;
    r1.BIT.dma_interrupt_stat_en = 0x1;
    r1.BIT.buf_wr_ready_stat_en = 0x1;
    r1.BIT.buf_rd_ready_stat_en = 0x1;
    r1.BIT.card_insertion_stat_en = 0x1;
    r1.BIT.card_removal_stat_en = 0x1;
    r1.BIT.int_a_stat_en = 0x1;
    REG_WRITE((u32)&(SDIO->error_int_stat_en_r__normal_int_stat_en), r1.d32);
    //SDIO->error_int_stat_en_r__normal_int_stat_en.d32 = 0x000002FF;
    r2.d32 = 0;
    r2.BIT.card_insertion_signal_en = 0x1;
    r2.BIT.card_removal_signal_en = 0x1;
    REG_WRITE((u32)&(SDIO->error_int_signal_en_r__normal_int_signal_en), r2.d32);
    //SDIO->error_int_signal_en_r__normal_int_signal_en.d32 = 0x000000C0;
    r3.d32 = 0;
    r3.BIT.card_insertion = 0x1;
    r3.BIT.card_removal = 0x1;
    REG_WRITE((u32)&(SDIO->error_int_stat_r__normal_int_stat), r3.d32);
    //SDIO->error_int_stat_r__normal_int_stat.d32 = 0x000000C0;

    sleep(200);
    
    while (!CardStatus)
    {
        r4.d32 = REG_READ((u32)&(SDIO->pstate_reg));
        //PSTATE_REG_R reg = SDIO->pstate_reg;
        CardStatus = (((r4.BIT.card_inserted) == 1) ? 1:0);
    	if (CardStatus == 1)
    	{
            Status = XST_SUCCESS;
    	    break;
    	}
    }
    	
    return Status;
}

/***************************************************************************/
/**
 * @brief	init the SD/EMMC host controller
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 HostControllerSetup(volatile DWC_mshc_block_registers* ptr)
{
    u32 Status;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r2;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r3;

    r1.d32 = 0;
    r1.BIT.dma_sel = 0x2;   //ADMA2
    r1.BIT.sd_bus_pwr_vdd1 = 0x1;   //PWR ON
    r1.BIT.sd_bus_vol_vdd1 = 0x7;   //3V
    r1.BIT.sd_bus_pwr_vdd2 = 0x1;   //PWR ON
    r1.BIT.sd_bus_vol_vdd2 = 0x5;   //1.8V
    REG_WRITE((u32)&(ptr->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    r2.d32 = 0;
    r2.BIT.internal_clk_en = 0x1;       //Oscillate
    r2.BIT.internal_clk_stable = 0x1;   // ro why read
    r2.BIT.pll_enable = 0x1;            //PLL enabled
    r2.BIT.clk_gen_select = 0x1;        //Programmable Clock Mode
    REG_WRITE((u32)&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r2.d32);
    r3.d32 = 0;
    REG_WRITE((u32)&(ptr->host_ctrl2_r__auto_cmd_stat), r3.d32);

    //  Host Controller Setup
    //ptr->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32= 0x0000BF10;
    //ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000002B;
    //ptr->host_ctrl2_r__auto_cmd_stat.d32= 0x00000000;

    Status = XST_SUCCESS;
    return Status;
}

/***************************************************************************/
/**
 * @brief	set the SD/EMMC host clock 
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 HostControllerClockSetup(volatile DWC_mshc_block_registers* ptr, int freq)
{
    SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r1;
    //  Host Controller Clock Setup
    if (freq == 0)
    {
        r1.d32 = 0;
        r1.BIT.internal_clk_en = 0x1;       //Oscillate
        r1.BIT.internal_clk_stable = 0x1;   // ro why read
        r1.BIT.pll_enable = 0x1;            //PLL enabled
        REG_WRITE((u32)&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r1.d32);
        //ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000000B;
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);
        r1.d32 = 0;
        r1.BIT.internal_clk_en = 0x1;       //Oscillate
        r1.BIT.internal_clk_stable = 0x1;   // ro why read
        r1.BIT.sd_clk_en = 0x1;             //Enable SDCLK/RCLK
        r1.BIT.pll_enable = 0x1;            //PLL enabled
        REG_WRITE((u32)&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r1.d32);
        //ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000000F;
        ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000000F;
    }
    else if (freq == 1)
    {
        r1.d32 = 0;
        r1.BIT.internal_clk_en = 0x1;       //Oscillate
        r1.BIT.internal_clk_stable = 0x1;   // ro why read
        r1.BIT.pll_enable = 0x1;            //PLL enabled
        r1.BIT.clk_gen_select = 0x1;        //Programmable Clock Mode
        REG_WRITE((u32)&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r1.d32);

        //ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000002B;
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);

        r1.d32 = 0;
        r1.BIT.internal_clk_en = 0x1;       //Oscillate
        r1.BIT.internal_clk_stable = 0x1;   // ro why read
        r1.BIT.sd_clk_en = 0x1;             //Enable SDCLK/RCLK
        r1.BIT.pll_enable = 0x1;            //PLL enabled
        r1.BIT.clk_gen_select = 0x1;        //Programmable Clock Mode
        REG_WRITE((u32)&(ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r1.d32);

        //ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000002F;
        ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32 = 0x0000002F;
    }
    else
    {
    }

    return XST_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	set interrupt controller
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 InitInterruptSetting(volatile DWC_mshc_block_registers* ptr)
{
    ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R r1;
    ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN_R r2;
    ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R r3;

    r1.d32 = 0;
    r1.BIT.cmd_complete_stat_en = 0x1;
    r1.BIT.xfer_complete_stat_en = 0x1;
    r1.BIT.bgap_event_stat_en = 0x1;
    r1.BIT.dma_interrupt_stat_en = 0x1;
    r1.BIT.buf_wr_ready_stat_en = 0x1;
    r1.BIT.buf_rd_ready_stat_en = 0x1;
    r1.BIT.card_insertion_stat_en = 0x1;
    r1.BIT.card_removal_stat_en = 0x1;
    r1.BIT.int_a_stat_en = 0x1;
    REG_WRITE((u32)&(ptr->error_int_stat_en_r__normal_int_stat_en), r1.d32);
    //ptr->error_int_stat_en_r__normal_int_stat_en.d32 = 0x000002FF;
    r2.d32 = 0;
    r2.BIT.cmd_complete_signal_en = 0x1;
    r2.BIT.xfer_complete_signal_en = 0x1;
    r2.BIT.bgap_event_signal_en = 0x1;
    r2.BIT.dma_interrupt_signal_en = 0x1;
    r2.BIT.buf_wr_ready_signal_en = 0x1;
    r2.BIT.buf_rd_ready_signal_en = 0x1;
    r2.BIT.card_insertion_signal_en = 0x1;
    r2.BIT.card_removal_signal_en = 0x1;
    REG_WRITE((u32)&(ptr->error_int_signal_en_r__normal_int_signal_en), r2.d32);
    //ptr->error_int_signal_en_r__normal_int_signal_en.d32 = 0x000000FF;
    r3.d32 = 0;
    r3.BIT.cmd_complete_stat_en = 0x1;
    r3.BIT.xfer_complete_stat_en = 0x1;
    r3.BIT.bgap_event_stat_en = 0x1;
    r3.BIT.dma_interrupt_stat_en = 0x1;
    r3.BIT.buf_wr_ready_stat_en = 0x1;
    r3.BIT.buf_rd_ready_stat_en = 0x1;
    r3.BIT.card_insertion_stat_en = 0x1;
    r3.BIT.card_removal_stat_en = 0x1;
    r3.BIT.int_a_stat_en = 0x1;
    r3.BIT.cmd_tout_err_stat_en = 0x1;
    r3.BIT.cmd_crc_err_stat_en = 0x1;
    r3.BIT.cmd_idx_err_stat_en = 0x1;
    r3.BIT.data_tout_err_stat_en = 0x1;
    r3.BIT.data_crc_err_stat_en = 0x1;
    r3.BIT.data_end_bit_err_stat_en = 0x1;
    r3.BIT.cur_lmt_err_stat_en = 0x1;
    REG_WRITE((u32)&(ptr->error_int_stat_en_r__normal_int_stat_en), r3.d32);
    //ptr->error_int_stat_en_r__normal_int_stat_en.d32 = 0x00FB02FF;
    ptr->host_ctrl2_r__auto_cmd_stat.d32 = 0x00000000;

    SDRegWrite(AT_CTRL_R, 0x0FFF0000);
    SDRegWrite(MBIU_CTRL_R, 0x01010004);

    return XST_SUCCESS;
}


/***************************************************************************/
/**
 * @brief	send the initial command to the sd/emmc device
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SendInitCmdSD()
{
    volatile unsigned int response01;
    volatile unsigned int validvoltage;
    volatile unsigned int errorstatus;
    int Status;
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;
    
    // send command 0
    arg_r = 0;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
    //SDIO->argument_r = 0;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.cmd_index = SD_CMD_GO_IDLE_STATE;
    reg.BIT.data_xfer_dir = DATA_READ;
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->cmd_r__xfer_mode = reg;
    wait_command_complete(SDIO);

    // send command 8
    arg_r = 0x1AA;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
    //SDIO->argument_r = 0x1AA;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.cmd_index = SD_CMD_HS_SEND_EXT_CSD;
    reg.BIT.data_xfer_dir = DATA_READ;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->cmd_r__xfer_mode = reg;
    wait_command_complete(SDIO);

    // send command 55
    arg_r = 0;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
    //SDIO->argument_r = 0;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.cmd_index = SD_CMD_APP_CMD;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    reg.BIT.block_count_enable = 0x1;
    reg.BIT.data_xfer_dir = DATA_READ;
    reg.BIT.multi_blk_sel = 0x1;
    reg.BIT.resp_err_chk_enable = 0x1;
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->cmd_r__xfer_mode = reg;
    wait_command_complete(SDIO);
    
    validvoltage = 0;
    while (!validvoltage)
    {
    	// CMD55
        arg_r = 0;
        REG_WRITE((u32)&(SDIO->argument_r), arg_r);
        //SDIO->argument_r = 0;
        //memset(&reg, 0, sizeof(reg));
        reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
        reg.BIT.cmd_index = SD_CMD_APP_CMD;
        reg.BIT.resp_type_select = SDIO_Response_Short;
        reg.BIT.block_count_enable = 0x1;
        reg.BIT.data_xfer_dir = DATA_READ;
        reg.BIT.multi_blk_sel = 0x1;
        reg.BIT.resp_err_chk_enable = 0x1;
        REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
        //SDIO->cmd_r__xfer_mode = reg;
        wait_command_complete(SDIO);

    	//CMD41
        arg_r = 0xC0100000;
        REG_WRITE((u32)&(SDIO->argument_r), arg_r);
        //SDIO->argument_r = 0xC0100000;
        //memset(&reg, 0, sizeof(reg));
        reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
        reg.BIT.cmd_index = SD_CMD_SD_APP_OP_COND;
        reg.BIT.resp_type_select = SDIO_Response_Short;
        reg.BIT.block_count_enable = 0x1;
        reg.BIT.data_xfer_dir = DATA_READ;
        reg.BIT.multi_blk_sel = 0x1;
        reg.BIT.resp_err_chk_enable = 0x1;
        REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
        //SDIO->cmd_r__xfer_mode = reg;
        wait_command_complete(SDIO);

        response01 = REG_READ((u32)&(SDIO->resp01));
        //response01 = SDIO->resp01;
    	validvoltage = (((response01 >> 31) == 1) ? 1:0);
    	if (validvoltage == 1)
    	{
    	    break;
    	}
    }

    // send command 2
    arg_r = 0;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
    //SDIO->argument_r = 0;
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.cmd_index = SD_CMD_ALL_SEND_CID;
    reg.BIT.resp_type_select = SDIO_Response_Long;
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->cmd_r__xfer_mode = reg;
    wait_command_complete(SDIO);
    CID_Tab[0] = REG_READ((u32)&(SDIO->resp01));
    CID_Tab[1] = REG_READ((u32)&(SDIO->resp23));
    CID_Tab[2] = REG_READ((u32)&(SDIO->resp45));
    CID_Tab[3] = REG_READ((u32)&(SDIO->resp67));
    //CID_Tab[0] = SDIO->resp01;
    //CID_Tab[1] = SDIO->resp23;
    //CID_Tab[2] = SDIO->resp45;
    //CID_Tab[3] = SDIO->resp67;
    
    // send command 3
    arg_r = 0x10000;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
    //SDIO->argument_r = 0x10000;
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.cmd_index = SD_CMD_SET_REL_ADDR;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->cmd_r__xfer_mode = reg;
    wait_command_complete(SDIO);
    rca = REG_READ((u32)&(SDIO->resp01)) & 0xFFFF0000;
    //rca = SDIO->resp01 & 0xFFFF0000;

    // send command 9
    arg_r = rca;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
    //SDIO->argument_r = rca;
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.cmd_index = SD_CMD_SEND_CSD;
    reg.BIT.resp_type_select = SDIO_Response_Long;
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->cmd_r__xfer_mode = reg;
    wait_command_complete(SDIO);
    CSD_Tab[3] = REG_READ((u32)&(SDIO->resp01));
    CSD_Tab[2] = REG_READ((u32)&(SDIO->resp23));
    CSD_Tab[1] = REG_READ((u32)&(SDIO->resp45));
    CSD_Tab[0] = REG_READ((u32)&(SDIO->resp67));
    //CSD_Tab[3] = SDIO->resp01;
    //CSD_Tab[2] = SDIO->resp23;
    //CSD_Tab[1] = SDIO->resp45;
    //CSD_Tab[0] = SDIO->resp67;    

    // Set buswidth to 1 bit clock to 48MHZ
    errorstatus = SD_GetCardInfo(&SDCardInfo);
    //Set Freq 10M
    Status = HostControllerClockSetup(SDIO, FREQ_10M);
    
    // send command 7
    arg_r = rca;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
    //SDIO->argument_r = rca;
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.cmd_index = SD_CMD_SEL_DESEL_CARD;
    reg.BIT.resp_type_select = SDIO_Response_Short_48B;
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->cmd_r__xfer_mode = reg;
    wait_command_complete(SDIO);

    return XST_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	send the initial command to the sd/emmc device
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SendInitCmdEmmc()
{
    volatile unsigned int response01;
    volatile unsigned int validvoltage;
    volatile unsigned int errorstatus;
    int Status;
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;
    
    // send command 0
    arg_r = 0;
    REG_WRITE((u32)&(eMMC->argument_r), arg_r);
    //eMMC->argument_r = 0;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(eMMC->cmd_r__xfer_mode));
    reg.BIT.cmd_index = SD_CMD_GO_IDLE_STATE;
    reg.BIT.data_xfer_dir = DATA_READ;
    REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
    //eMMC->cmd_r__xfer_mode = reg;
    wait_command_complete(eMMC);
    sleep(1000);
    
    validvoltage = 0;
    while (!validvoltage)
    {
    	//CMD1
        arg_r = 0x40000080;
        REG_WRITE((u32)&(eMMC->argument_r), arg_r);
        //eMMC->argument_r = 0x40000080;
        //memset(&reg, 0, sizeof(reg));
        reg.d32 = REG_READ((u32)&(eMMC->cmd_r__xfer_mode));
        reg.BIT.cmd_index = SD_CMD_SEND_OP_COND;
        reg.BIT.resp_type_select = SDIO_Response_Short;
        reg.BIT.data_xfer_dir = DATA_READ;
        REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
        //eMMC->cmd_r__xfer_mode = reg;
        wait_command_complete(eMMC);

        response01 = REG_READ((u32)&(eMMC->resp01));
        //response01 = eMMC->resp01;
    	validvoltage = (((response01 >> 31) == 1) ? 1:0);
    	if (validvoltage == 1)
    	{
    	    break;
    	}
    }

    // send command 2
    arg_r = 0;
    REG_WRITE((u32)&(eMMC->argument_r), arg_r);
    //eMMC->argument_r = 0;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(eMMC->cmd_r__xfer_mode));
    reg.BIT.cmd_index = SD_CMD_ALL_SEND_CID;
    reg.BIT.block_count_enable = 0x1;
    reg.BIT.data_xfer_dir = DATA_READ;
    reg.BIT.resp_err_chk_enable = 0x1;
    reg.BIT.multi_blk_sel = 0x1;
    reg.BIT.resp_type_select = SDIO_Response_Long;
    REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
    //eMMC->cmd_r__xfer_mode = reg;
    wait_command_complete(eMMC);

    // send command 3
    arg_r = 0x10000;
    REG_WRITE((u32)&(eMMC->argument_r), arg_r);
    //eMMC->argument_r = 0x10000;
    reg.BIT.cmd_index = SD_CMD_SET_REL_ADDR;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
    //eMMC->cmd_r__xfer_mode = reg;
    wait_command_complete(eMMC);
    rca = REG_READ((u32)&(eMMC->resp01)) & 0xFFFF0000;
    //rca = eMMC->resp01 & 0xFFFF0000;

    // send command 9
    arg_r = rca;
    REG_WRITE((u32)&(eMMC->argument_r), arg_r);
    //eMMC->argument_r = rca;
    reg.BIT.cmd_index = SD_CMD_SEND_CSD;
    reg.BIT.resp_type_select = SDIO_Response_Long;
    REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
    //eMMC->cmd_r__xfer_mode = reg;
    wait_command_complete(eMMC);
    CSD_Tab[3] = REG_READ((u32)&(eMMC->resp01));
    CSD_Tab[2] = REG_READ((u32)&(eMMC->resp23));
    CSD_Tab[1] = REG_READ((u32)&(eMMC->resp45));
    CSD_Tab[0] = REG_READ((u32)&(eMMC->resp67));
    //CSD_Tab[3] = eMMC->resp01;
    //CSD_Tab[2] = eMMC->resp23;
    //CSD_Tab[1] = eMMC->resp45;
    //CSD_Tab[0] = eMMC->resp67;    

    // Set buswidth to 1 bit clock to 48MHZ
    errorstatus = SD_GetCardInfo(&SDCardInfo);

    // send command 10
    arg_r = rca;
    REG_WRITE((u32)&(eMMC->argument_r), arg_r);
    //eMMC->argument_r = rca;
    reg.BIT.cmd_index = SD_CMD_SEND_CID;
    reg.BIT.resp_type_select = SDIO_Response_Long;
    REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
    //eMMC->cmd_r__xfer_mode = reg;
    wait_command_complete(eMMC);

    //Set Freq 10M
    Status = HostControllerClockSetup(eMMC, FREQ_10M);
    
    // send command 7
    arg_r = rca;
    REG_WRITE((u32)&(eMMC->argument_r), arg_r);
    //eMMC->argument_r = rca;
    reg.BIT.cmd_index = SD_CMD_SEL_DESEL_CARD;
    reg.BIT.resp_type_select = SDIO_Response_Short_48B;
    REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
    //eMMC->cmd_r__xfer_mode = reg;
    wait_command_complete(eMMC);

    return XST_SUCCESS;
}


/***************************************************************************/
/**
 * @brief	change the bit width
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SwitchDataWidthSD()
{
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;

    // send command 55  SET BUSWITHD TO 4 BIT
    arg_r = rca;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
   	//SDIO->argument_r = rca;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.cmd_index = SD_CMD_APP_CMD;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    reg.BIT.block_count_enable = 0x1;
    reg.BIT.data_xfer_dir = DATA_READ;
    reg.BIT.multi_blk_sel = 0x1;
    reg.BIT.resp_err_chk_enable = 0x1;
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->cmd_r__xfer_mode = reg;
    wait_command_complete(SDIO);

    // send command 6
    arg_r = 0x2;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
    //SDIO->argument_r = 0x2; //set sd model data width=4
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.cmd_index = SD_CMD_HS_SWITCH;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->cmd_r__xfer_mode = reg;
    wait_command_complete(SDIO);

    sleep(2000);

    return XST_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	change the bit width
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SwitchDataWidthEmmc()
{
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;

    // send command 6
    arg_r = 0x03b70200;
    REG_WRITE((u32)&(eMMC->argument_r), arg_r);
    //eMMC->argument_r = 0x03b70200; //set sd model data width=4
    reg.BIT.cmd_index = SD_CMD_HS_SWITCH;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
    //eMMC->cmd_r__xfer_mode = reg;
    wait_command_complete(eMMC);
    r1.d32 = REG_READ((u32)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1));
    r1.BIT.extdat_xfer = 0x1;
    REG_WRITE((u32)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    //eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.BIT.extdat_xfer = 0x1;
    sleep(2000);

    return XST_SUCCESS;
}


/***************************************************************************/
/**
 * @brief	SD init sequence
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SD_Init(void)
{
    int Status = XST_FAILURE;

    Status = CardDetection();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = HostControllerSetup(SDIO);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = HostControllerClockSetup(SDIO, FREQ_400K);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    sleep(200);
    Status = InitInterruptSetting(SDIO);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    sleep(200);
    Status = SendInitCmdSD();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = SwitchDataWidthSD();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    
    Status = XST_SUCCESS;
END:
	return Status;
}

/***************************************************************************/
/**
 * @brief	emmc init sequence
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 EMMC_Init(void)
{
    int Status = XST_FAILURE;

    Status = HostControllerSetup(eMMC);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = HostControllerClockSetup(eMMC, FREQ_400K);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    sleep(200);
    Status = InitInterruptSetting(eMMC);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    sleep(200);
    Status = SendInitCmdEmmc();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = SwitchDataWidthEmmc();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    
    Status = XST_SUCCESS;
END:
	return Status;
}


/***************************************************************************/
/**
 * @brief	read multi block size data 
 *
 * @param	readbuff reading data buffer 
 * @param	ReadAddr read start address
 * @param	BlockSize read data block size
 * @param	NumberOfBlocks data block number
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SD_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )readbuff;
    CMD_R__XFER_MODE_R reg;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;

    r1.d32 = 0;
    r1.BIT.dat_xfer_width = 0x1;   //4-bit mode
    r1.BIT.sd_bus_pwr_vdd1 = 0x1;   //PWR ON
    r1.BIT.sd_bus_vol_vdd1 = 0x7;   //3V
    r1.BIT.sd_bus_pwr_vdd2 = 0x1;   //PWR ON
    r1.BIT.sd_bus_vol_vdd2 = 0x5;   //1.8V
    REG_WRITE((u32)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    //eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32 = 0x0000BF02;
    REG_WRITE((u32)&(eMMC->sdmasa_r), Buffer_SingleBlock);
    //eMMC->sdmasa_r = Buffer_SingleBlock;
    REG_WRITE((u32)&(eMMC->adma_sa_low_r), Buffer_SingleBlock);
    //eMMC->adma_sa_low_r = Buffer_SingleBlock;
    r2.d32 = 0;
    REG_WRITE((u32)&(eMMC->host_ctrl2_r__auto_cmd_stat), r2.d32);
    //eMMC->host_ctrl2_r__auto_cmd_stat.d32 = 0x0;

	// send command 16
    arg_r = BlockSize;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
	//SDIO->argument_r = BlockSize;
    //memset(&block, 0, sizeof(block));
    block.d32 = REG_READ((u32)&(SDIO->blockcount_r__blocksize));
    block.BIT.xfer_block_size = BlockSize;
    block.BIT.blockcount_r = NumberOfBlocks;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.block_count_enable = 0x1;
    reg.BIT.resp_err_chk_enable = 0x1;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    reg.BIT.cmd_index = SD_CMD_SET_BLOCKLEN;
    REG_WRITE((u32)&(SDIO->blockcount_r__blocksize), block.d32);
    //SDIO->blockcount_r__blocksize = block;
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->cmd_r__xfer_mode = reg;
    wait_command_complete(SDIO);

	// send command 17 read single block
    arg_r = BlockSize;
    REG_WRITE((u32)&(SDIO->argument_r), ReadAddr);
	//SDIO->argument_r = ReadAddr;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.dma_en = 0x1;
    reg.BIT.data_xfer_dir = 0x1;
    reg.BIT.resp_err_chk_enable = 0x1;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    reg.BIT.data_present_sel = 0x1;
    reg.BIT.cmd_index = SD_CMD_READ_SINGLE_BLOCK;
    //memset(&block, 0, sizeof(block));
    block.d32 = REG_READ((u32)&(SDIO->blockcount_r__blocksize));
    block.BIT.xfer_block_size = BlockSize;
    block.BIT.blockcount_r = NumberOfBlocks;
    REG_WRITE((u32)&(SDIO->blockcount_r__blocksize), block.d32);
    //SDIO->blockcount_r__blocksize = block;
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->cmd_r__xfer_mode = reg;
	wait_command_complete(SDIO);
    wait_transfer_complete(SDIO);

	return XST_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	read multi block size data 
 *
 * @param	readbuff reading data buffer 
 * @param	ReadAddr read start address
 * @param	BlockSize read data block size
 * @param	NumberOfBlocks data block number
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 EMMC_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )readbuff;
    CMD_R__XFER_MODE_R reg;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;

    r1.d32 = 0;
    r1.BIT.dat_xfer_width = 0x1;   //4-bit mode
    r1.BIT.sd_bus_pwr_vdd1 = 0x1;   //PWR ON
    r1.BIT.sd_bus_vol_vdd1 = 0x7;   //3V
    r1.BIT.sd_bus_pwr_vdd2 = 0x1;   //PWR ON
    r1.BIT.sd_bus_vol_vdd2 = 0x5;   //1.8V
    REG_WRITE((u32)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    //eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32 = 0x0000BF02;
    REG_WRITE((u32)&(eMMC->sdmasa_r), Buffer_SingleBlock);
    //eMMC->sdmasa_r = Buffer_SingleBlock;
    REG_WRITE((u32)&(eMMC->adma_sa_low_r), Buffer_SingleBlock);
    //eMMC->adma_sa_low_r = Buffer_SingleBlock;
    r2.d32 = 0;
    REG_WRITE((u32)&(eMMC->host_ctrl2_r__auto_cmd_stat), r2.d32);
    //eMMC->host_ctrl2_r__auto_cmd_stat.d32 = 0x0;

	// send command 16
    arg_r = 0x200;
    REG_WRITE((u32)&(eMMC->argument_r), arg_r);
	//eMMC->argument_r = 0x200;
    //memset(&block, 0, sizeof(block));
    block.d32 = REG_READ((u32)&(eMMC->blockcount_r__blocksize));
    block.BIT.xfer_block_size = BlockSize;
    block.BIT.blockcount_r = NumberOfBlocks;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(eMMC->cmd_r__xfer_mode));
    reg.BIT.block_count_enable = 0x1;
    reg.BIT.resp_err_chk_enable = 0x1;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    reg.BIT.cmd_index = SD_CMD_SET_BLOCKLEN;
    REG_WRITE((u32)&(eMMC->blockcount_r__blocksize), block.d32);
    //eMMC->blockcount_r__blocksize = block;
    REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
    //eMMC->cmd_r__xfer_mode = reg;
    wait_command_complete(eMMC);

	// send command 17 read single block
	eMMC->argument_r = ReadAddr;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(eMMC->cmd_r__xfer_mode));
    reg.BIT.dma_en = 0x1;
    reg.BIT.data_xfer_dir = 0x1;
    reg.BIT.resp_err_chk_enable = 0x1;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    reg.BIT.data_present_sel = 0x1;
    reg.BIT.cmd_index = SD_CMD_READ_SINGLE_BLOCK;
    //memset(&block, 0, sizeof(block));
    block.d32 = REG_READ((u32)&(eMMC->blockcount_r__blocksize));
    block.BIT.xfer_block_size = BlockSize;
    block.BIT.blockcount_r = NumberOfBlocks;
    REG_WRITE((u32)&(eMMC->blockcount_r__blocksize), block.d32);
    //eMMC->blockcount_r__blocksize = block;
    REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
    //eMMC->cmd_r__xfer_mode = reg;
	wait_command_complete(eMMC);
    wait_transfer_complete(eMMC);

	return XST_SUCCESS;
}

     //wait_transfer_complete();    //sdma start

/***************************************************************************/
/**
 * @brief	write multi block size data 
 *
 * @param	writebuff reading data buffer 
 * @param	WriteAddr read start address
 * @param	BlockSize read data block size
 * @param	NumberOfBlocks data block number
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SD_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )writebuff;
    CMD_R__XFER_MODE_R reg;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;

    r1.d32 = 0;
    r1.BIT.dat_xfer_width = 0x1;   //4-bit mode
    r1.BIT.sd_bus_pwr_vdd1 = 0x1;   //PWR ON
    r1.BIT.sd_bus_vol_vdd1 = 0x7;   //3V
    r1.BIT.sd_bus_pwr_vdd2 = 0x1;   //PWR ON
    r1.BIT.sd_bus_vol_vdd2 = 0x5;   //1.8V
    REG_WRITE((u32)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    //eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32 = 0x0000BF02;
    REG_WRITE((u32)&(eMMC->sdmasa_r), Buffer_SingleBlock);
    //eMMC->sdmasa_r = Buffer_SingleBlock;
    REG_WRITE((u32)&(eMMC->adma_sa_low_r), Buffer_SingleBlock);
    //eMMC->adma_sa_low_r = Buffer_SingleBlock;
    r2.d32 = 0;
    REG_WRITE((u32)&(eMMC->host_ctrl2_r__auto_cmd_stat), r2.d32);
    //eMMC->host_ctrl2_r__auto_cmd_stat.d32 = 0x0;

	// send command 16
    arg_r = BlockSize;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
	//SDIO->argument_r = BlockSize;
    //memset(&block, 0, sizeof(block));
    block.d32 = REG_READ((u32)&(SDIO->blockcount_r__blocksize));
    block.BIT.xfer_block_size = BlockSize;
    block.BIT.blockcount_r = NumberOfBlocks;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.block_count_enable = 0x1;
    reg.BIT.resp_err_chk_enable = 0x1;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    reg.BIT.cmd_index = SD_CMD_SET_BLOCKLEN;
    REG_WRITE((u32)&(SDIO->blockcount_r__blocksize), block.d32);
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->blockcount_r__blocksize = block;
    //SDIO->cmd_r__xfer_mode = reg;
    wait_command_complete(SDIO);


	// send command 24
    arg_r = WriteAddr;
    REG_WRITE((u32)&(SDIO->argument_r), arg_r);
    //SDIO->argument_r = WriteAddr;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(SDIO->cmd_r__xfer_mode));
    reg.BIT.dma_en = 0x1;
    reg.BIT.block_count_enable = 0x1;
    reg.BIT.data_xfer_dir = DATA_WRITE;
    reg.BIT.resp_err_chk_enable = 0x1;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    reg.BIT.data_present_sel = 0x1;
    reg.BIT.cmd_index = SD_CMD_WRITE_SINGLE_BLOCK;
    //memset(&block, 0, sizeof(block));
    block.d32 = REG_READ((u32)&(SDIO->blockcount_r__blocksize));
    block.BIT.xfer_block_size = BlockSize;
    block.BIT.blockcount_r = NumberOfBlocks;
    REG_WRITE((u32)&(SDIO->blockcount_r__blocksize), block.d32);
    REG_WRITE((u32)&(SDIO->cmd_r__xfer_mode), reg.d32);
    //SDIO->blockcount_r__blocksize = block;
    //SDIO->cmd_r__xfer_mode = reg;
	wait_command_complete(SDIO);
    wait_transfer_complete(SDIO);

	return XST_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	write multi block size data 
 *
 * @param	writebuff reading data buffer 
 * @param	WriteAddr read start address
 * @param	BlockSize read data block size
 * @param	NumberOfBlocks data block number
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 EMMC_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )writebuff;
    CMD_R__XFER_MODE_R reg;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;

    r1.d32 = 0;
    r1.BIT.dat_xfer_width = 0x1;   //4-bit mode
    r1.BIT.sd_bus_pwr_vdd1 = 0x1;   //PWR ON
    r1.BIT.sd_bus_vol_vdd1 = 0x7;   //3V
    r1.BIT.sd_bus_pwr_vdd2 = 0x1;   //PWR ON
    r1.BIT.sd_bus_vol_vdd2 = 0x5;   //1.8V
    REG_WRITE((u32)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    //eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32 = 0x0000BF02;
    REG_WRITE((u32)&(eMMC->sdmasa_r), Buffer_SingleBlock);
    //eMMC->sdmasa_r = Buffer_SingleBlock;
    REG_WRITE((u32)&(eMMC->adma_sa_low_r), Buffer_SingleBlock);
    //eMMC->adma_sa_low_r = Buffer_SingleBlock;
    r2.d32 = 0;
    REG_WRITE((u32)&(eMMC->host_ctrl2_r__auto_cmd_stat), r2.d32);
    //eMMC->host_ctrl2_r__auto_cmd_stat.d32 = 0x0;

	// send command 16
    arg_r = BlockSize;
    REG_WRITE((u32)&(eMMC->argument_r), arg_r);
	//eMMC->argument_r = BlockSize;
    //memset(&block, 0, sizeof(block));
    block.d32 = REG_READ((u32)&(eMMC->blockcount_r__blocksize));
    block.BIT.xfer_block_size = BlockSize;
    block.BIT.blockcount_r = NumberOfBlocks;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(eMMC->cmd_r__xfer_mode));
    reg.BIT.block_count_enable = 0x1;
    reg.BIT.resp_err_chk_enable = 0x1;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    reg.BIT.cmd_index = SD_CMD_SET_BLOCKLEN;
    REG_WRITE((u32)&(eMMC->blockcount_r__blocksize), block.d32);
    //eMMC->blockcount_r__blocksize = block;
    REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
    //eMMC->cmd_r__xfer_mode = reg;
    wait_command_complete(eMMC);


	// send command 24
    arg_r = WriteAddr;
    REG_WRITE((u32)&(eMMC->argument_r), arg_r);
    //eMMC->argument_r = WriteAddr;
    //memset(&reg, 0, sizeof(reg));
    reg.d32 = REG_READ((u32)&(eMMC->cmd_r__xfer_mode));
    reg.BIT.dma_en = 0x1;
    reg.BIT.block_count_enable = 0x1;
    reg.BIT.data_xfer_dir = DATA_WRITE;
    reg.BIT.resp_err_chk_enable = 0x1;
    reg.BIT.resp_type_select = SDIO_Response_Short;
    reg.BIT.data_present_sel = 0x1;
    reg.BIT.cmd_index = SD_CMD_WRITE_SINGLE_BLOCK;
    //memset(&block, 0, sizeof(block));
    block.d32 = REG_READ((u32)&(eMMC->blockcount_r__blocksize));
    block.BIT.xfer_block_size = BlockSize;
    block.BIT.blockcount_r = NumberOfBlocks;
    REG_WRITE((u32)&(eMMC->blockcount_r__blocksize), block.d32);
    //eMMC->blockcount_r__blocksize = block;
    REG_WRITE((u32)&(eMMC->cmd_r__xfer_mode), reg.d32);
    //eMMC->cmd_r__xfer_mode = reg;
	wait_command_complete(eMMC);
    wait_transfer_complete(eMMC);

	return XST_SUCCESS;
}


u32 SD_WaitReadOperation(volatile DWC_mshc_block_registers* ptr)
{
    //ERROR_INT_STAT_R__NORMAL_INT_STAT_R reg;
    MtimerParams* Mtimer;
    Mtimer_Init(Mtimer);
    MTIMER_OUT_CONDITION(100, Mtimer, \
                        ptr->error_int_stat_r__normal_int_stat.BIT.xfer_complete != 1);
    if(Mtimer_IsTimerOut(Mtimer)){
        return XST_FAILURE;
    }else{
        ptr->error_int_stat_r__normal_int_stat.BIT.xfer_complete = 1;
        return XST_SUCCESS;
    }
    /*for (;;)
    {
        reg = ptr->error_int_stat_r__normal_int_stat;
        if (reg.BIT.xfer_complete == 1)
        {
            ptr->error_int_stat_r__normal_int_stat.BIT.xfer_complete = 1;
            break;
        }
    }*/
    return XST_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	Returns information about specific card.
 *
 * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD card
 *         information.
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  u32 errorstatus = XST_SUCCESS;
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

u32 RawReadWriteTestSD()
{
    int Status;
    BYTE WriteBuffer[] = "tessssssssssssttttttt\r\n";
    BYTE ReadBuffer[1024]={0}; 
    int result;
    
    SD_Init();
    SD_WriteMultiBlocks(WriteBuffer,5,SDCardInfo.CardBlockSize,1);
    SD_ReadMultiBlocks(ReadBuffer, 5, SDCardInfo.CardBlockSize,1);

    result = strcmp(WriteBuffer, ReadBuffer);
    if (result == 0)
    {
        Status = XST_SUCCESS;
    }
    else
    {
        Status = XST_FAILURE;
    }

    return Status;
}

u32 RawReadWriteTestEmmc()
{
    int Status;
    int result;
    BYTE WriteBuffer[] = "welcomewelcome\r\n";
    BYTE ReadBuffer[1024]={0};   
    EMMC_Init();
    EMMC_WriteMultiBlocks(WriteBuffer, 80,SDCardInfo.CardBlockSize,1);
    EMMC_ReadMultiBlocks(ReadBuffer, 80, SDCardInfo.CardBlockSize,1);

    result = strcmp(WriteBuffer, ReadBuffer);
    if (result == 0)
    {
        Status = XST_SUCCESS;
    }
    else
    {
        Status = XST_FAILURE;
    }

    return Status;
}



/***************************************************************************/
/**
 * @brief	test SD/EMMC read/write
 *
 * @param  None
 * @return XST_SUCCESS
 *
 ******************************************************************************/
u32 SD_Test(void)
{
	UINT fnum;            			  
	BYTE ReadBuffer[1024]={0};
	BYTE WriteBuffer[] = "welcome777777777777777\r\n";
	FIL fnew;
	u32 Status;

    RawReadWriteTestEmmc();
    RawReadWriteTestSD();

    //RawReadWriteTestEmmc();
    //RawReadWriteTestSD();
    for(;;);
#if 0
	res_sd = f_mount(&fs,"0:",1);  //SD test
#if 0
	    if(res_sd == FR_NO_FILESYSTEM)
	    {
	    	res_sd=f_mkfs("0:",0,0);
	    	if(res_sd == FR_OK)
	    	{
	    		res_sd = f_mount(NULL,"0:",1);
	    		res_sd = f_mount(&fs,"0:",1);
	    	}
	    }
#endif
    res_sd = f_open(&fnew, "0:FatFs1.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
        res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
        if(res_sd==FR_OK)
        {
            //printf("》文件写入成功，写入字节数据：%d\n",fnum);
            //printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
        }
        else
        {
            //printf("！！文件写入失败：(%d)\n",res_sd);
        }
        f_close(&fnew);
    }
    else
    {
    }
	/*------------------- 文件系统测试：读测试 ------------------------------------*/
    //printf("****** 即将进行文件读取测试... ******\r\n");
    res_sd = f_open(&fnew, "0:FatFs1.txt", FA_OPEN_EXISTING | FA_READ);
    if(res_sd == FR_OK)
    {
        //printf("》打开文件成功。\r\n");
        res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if(res_sd==FR_OK)
        {
            //printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
            //printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);
        }
        else
        {
            //printf("！！文件读取失败：(%d)\n",res_sd);
        }
    }
    else
    {
        //LED_RED;
        //printf("！！打开文件失败。\r\n");
    }
    f_close(&fnew);
    f_mount(NULL,"0:",1);
#endif

    res_sd = f_mount(&fs,"1:",1);  //EMMC test
#if 1
            if(res_sd == FR_NO_FILESYSTEM)
            {
                res_sd=f_mkfs("1:",0,0);
                if(res_sd == FR_OK)
                {
                    res_sd = f_mount(NULL,"1:",1);
                    res_sd = f_mount(&fs,"1:",1);
                }
            }
#endif
        res_sd = f_open(&fnew, "1:FatFs1.txt",FA_CREATE_ALWAYS | FA_WRITE );
        if ( res_sd == FR_OK )
        {
            res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
            if(res_sd==FR_OK)
            {
                //printf("》文件写入成功，写入字节数据：%d\n",fnum);
                //printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
            }
            else
            {
                //printf("！！文件写入失败：(%d)\n",res_sd);
            }
            f_close(&fnew);
        }
        else
        {
        }
        /*------------------- 文件系统测试：读测试 ------------------------------------*/
        //printf("****** 即将进行文件读取测试... ******\r\n");
        res_sd = f_open(&fnew, "1:FatFs1.txt", FA_OPEN_EXISTING | FA_READ);
        if(res_sd == FR_OK)
        {
            //printf("》打开文件成功。\r\n");
            res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
            if(res_sd==FR_OK)
            {
                //printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
                //printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);
            }
            else
            {
                //printf("！！文件读取失败：(%d)\n",res_sd);
            }
        }
        else
        {
            //LED_RED;
            //printf("！！打开文件失败。\r\n");
        }
        f_close(&fnew);
        f_mount(NULL,"1:",1);

    

    return Status;
}
/*********************************************END OF FILE**********************/
