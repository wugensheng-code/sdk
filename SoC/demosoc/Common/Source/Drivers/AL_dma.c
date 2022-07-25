/*
 * aL_dma.c
 *
 *  Created on: 2021年11月3日
 *      Author: wei.pang
 */
#include "demosoc.h"
#include "stddef.h"
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#define DMAC_MAX_CHANNELS   8
#define DMAC_MAX_INTERRUPTS 5
#define DW_EBUSY            1
#define DW_ECHRNG	    1
#define DW_EINVAL	    1
#define DW_ENOSYS	    1
#define DW_EPERM	    1
#define Dmac_all_channels   8
#define Dmac_no_channel     0
#define num_channels        1
#define spi0_data_base_address   0xF8404060
#define spi1_data_base_adderss   0xF8405060
#define qspi_data_base_address  0xF804E060
#define i2c0_data_base_address  0xF8414010
#define i2c1_data_base_address  0xF8415010
volatile uint16_t DMA_BLOCK_LIST_reload_count = 0;
volatile uint32_t buffer_list_parm[100][5];
/**********************************************************************/
//pass

int AlDma_reset_all_channels(AL_DMAC_TypeDef *DMAC)
{
    int errorCode = 1;

    AlDma_Disable(DMAC);
    //AlDma_DisableChannel(DMAC);
    AlDma_ClearIrq(DMAC);

    return errorCode;
}
///
/**********************************************************************/
//pass
void AlDma_Enable(AL_DMAC_TypeDef *DMAC)
{
    DMAC->DmaCfgReg_L |=  DMAC_DmaCfgReg_DMA_EN;
}

/**********************************************************************/
//pas
int AlDma_Disable(AL_DMAC_TypeDef *DMAC)
{
    int errorCode=1;
    DMAC->DmaCfgReg_L &= ~DMAC_DmaCfgReg_DMA_EN;
    return errorCode;
}


/**********************************************************************/
//pass
int AlDma_EnableChannel(AL_DMAC_TypeDef *DMAC,enum  AL_dmac_channel_number ch_num)
{
    int errorCode=1;
    uint32_t reg;
     //Check for valid channel number and not busy
    //errorCode = AlDma_checkChannelBusy(DMAC, ch_num);
    if(errorCode == 1) {
         //The AlDma_channel_number enum is declared such that
         //the enumerated value maps exactly to the value that
         //needs to be written into the ChEnReg for enabling.
        reg = ch_num;
	switch(reg){
        case 0:
            DMAC ->ChEnReg_L |= DMAC_ChEnReg_L_CH1_EN;
	    break;
	case 1:
            DMAC ->ChEnReg_L |= DMAC_ChEnReg_L_CH2_EN;
	    break;
	case 2:
            DMAC ->ChEnReg_L |= DMAC_ChEnReg_L_CH3_EN;
	    break;
	case 3:
            DMAC ->ChEnReg_L |= DMAC_ChEnReg_L_CH4_EN;
	    break;
	case 4:
            DMAC ->ChEnReg_L |= DMAC_ChEnReg_L_CH5_EN;
	    break;
	case 5:
            DMAC ->ChEnReg_L |= DMAC_ChEnReg_L_CH6_EN;
	    break;
	case 6:
            DMAC ->ChEnReg_L |= DMAC_ChEnReg_L_CH7_EN;
	    break;
	case 7:
            DMAC ->ChEnReg_L |= DMAC_ChEnReg_L_CH8_EN;
	    break;
	default:
            DMAC ->ChEnReg_L |= DMAC_ChEnReg_L_CH1_EN;
	    break;

        }
    }
    return errorCode;
}

/**********************************************************************/
//pass
int AlDma_DisableChannel(AL_DMAC_TypeDef *DMAC,enum AL_dmac_channel_number ch_num)
{
	uint16_t reg = 0;
	reg = GET_BITS(DMAC -> ChEnReg_L,0,15);
       	DMAC ->ChEnReg_L &= ~((uint32_t)0x0000ffff);	
       return reg;
}
/**********************************************************************/

int AlDma_EnableChannelIrq(DMA_Channel_TypeDef *Channelx)
{
    int x;
    int errorCode = 1;
                if(Channelx ->CTL_L.INT_EN != 0x1) {
                	Channelx ->CTL_L.INT_EN = ENABLE;
                }
    return errorCode;
}
/**********************************************************************/

int AlDma_DisableChannelIrq(DMA_Channel_TypeDef *Channelx)
{
    int errorCode=1;
        if(Channelx ->CTL_L.INT_EN != 0x1) {
           Channelx ->CTL_L.INT_EN = DISABLE;
           }     
    return errorCode;
}
/**********************************************************************/

int AlDma_ClearIrq(AL_DMAC_TypeDef *DMAC)
{
        DMAC ->ClearTfer_L   |= DMAC_ClearTfer_CH1_CLEAR;
        DMAC ->ClearBlock_L  |= DMAC_ClearBlock_CH1_CLEAR;
        DMAC ->ClearSrcTran_L|= DMAC_ClearSrcTran_CH1_CLEAR;
        DMAC ->ClearDstTran_L|= DMAC_ClearDstTran_CH1_CLEAR;
        DMAC ->ClearErr_L    |= DMAC_ClearErr_CH1_CLEAR;
    return 0;
}

/**********************************************************************/

int AlDma_UnmaskIrq(
		AL_DMAC_TypeDef *DMAC,
		enum   AL_dmac_channel_number ch_num,
		enum   AL_dmac_irq ch_irq)
{
    int x;
    int errorCode=1;
    uint32_t reg;

    if(errorCode == 1) {

        // Loop through and mask the selected channel Irq
        // for the targeted channels.
	switch(ch_num){
	case0:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L   |= DMAC_MaskTfer_CH1_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L  |= DMAC_MaskBlock_CH1_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L|= DMAC_MaskSrcTran_CH1_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L|= DMAC_MaskDstTran_CH1_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L    |= DMAC_MaskErr_CH1_INT_MASK;
                             break;
                }
        }
	break;
	case1:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L   |= DMAC_MaskTfer_CH2_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L  |= DMAC_MaskBlock_CH2_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L|= DMAC_MaskSrcTran_CH2_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L|= DMAC_MaskDstTran_CH2_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L    |= DMAC_MaskErr_CH2_INT_MASK;
                             break;
                }
        }
	break;
	case2:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L   |= DMAC_MaskTfer_CH3_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L  |= DMAC_MaskBlock_CH3_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L|= DMAC_MaskSrcTran_CH3_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L|= DMAC_MaskDstTran_CH3_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L    |= DMAC_MaskErr_CH3_INT_MASK;
                             break;
                }
        }
	break;
	case3:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L   |= DMAC_MaskTfer_CH4_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L  |= DMAC_MaskBlock_CH4_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L|= DMAC_MaskSrcTran_CH4_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L|= DMAC_MaskDstTran_CH4_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L    |= DMAC_MaskErr_CH4_INT_MASK;
                             break;
                }
        }
	break;
	case4:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L   |= DMAC_MaskTfer_CH5_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L  |= DMAC_MaskBlock_CH5_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L|= DMAC_MaskSrcTran_CH5_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L|= DMAC_MaskDstTran_CH5_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L    |= DMAC_MaskErr_CH5_INT_MASK;
                             break;
                }
        }
	break;
	case5:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L   |= DMAC_MaskTfer_CH6_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L  |= DMAC_MaskBlock_CH6_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L|= DMAC_MaskSrcTran_CH6_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L|= DMAC_MaskDstTran_CH6_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L    |= DMAC_MaskErr_CH6_INT_MASK;
                             break;
                }
        }
	break;
	case6:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L   |= DMAC_MaskTfer_CH7_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L  |= DMAC_MaskBlock_CH7_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L|= DMAC_MaskSrcTran_CH7_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L|= DMAC_MaskDstTran_CH7_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L    |= DMAC_MaskErr_CH7_INT_MASK;
                             break;
                }
        }
	break;
	case7:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L   |= DMAC_MaskTfer_CH8_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L  |= DMAC_MaskBlock_CH8_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L|= DMAC_MaskSrcTran_CH8_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L|= DMAC_MaskDstTran_CH8_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L    |= DMAC_MaskErr_CH8_INT_MASK;
                             break;
                }
        }
	break;
	default:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L   |= DMAC_MaskTfer_CH1_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L  |= DMAC_MaskBlock_CH1_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L|= DMAC_MaskSrcTran_CH1_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L|= DMAC_MaskDstTran_CH1_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L    |= DMAC_MaskErr_CH1_INT_MASK;
                             break;
                }
        }
	break;

    }
}
    return errorCode;
}
/**********************************************************************/

int AlDma_MaskIrq(
		AL_DMAC_TypeDef *DMAC,
		enum   AL_dmac_channel_number ch_num,
		enum   AL_dmac_irq ch_irq)
{
    int x;
    int errorCode=1;
    uint32_t reg;

    if(errorCode == 1) {

        // Loop through and mask the selected channel Irq
        // for the targeted channels.
	switch(ch_num){
	case0:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L    &= ~DMAC_MaskTfer_CH1_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L   &= ~DMAC_MaskBlock_CH1_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L &= ~DMAC_MaskSrcTran_CH1_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L &= ~DMAC_MaskDstTran_CH1_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L     &= ~DMAC_MaskErr_CH1_INT_MASK;
                             break;
                }
        }
	break;
	case1:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L    &= ~DMAC_MaskTfer_CH2_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L   &= ~DMAC_MaskBlock_CH2_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L &= ~DMAC_MaskSrcTran_CH2_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L &= ~DMAC_MaskDstTran_CH2_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L     &= ~DMAC_MaskErr_CH2_INT_MASK;
                             break;
                }
        }
	break;
	case2:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L    &= ~DMAC_MaskTfer_CH3_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L   &= ~DMAC_MaskBlock_CH3_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L &= ~DMAC_MaskSrcTran_CH3_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L &= ~DMAC_MaskDstTran_CH3_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L     &= ~DMAC_MaskErr_CH3_INT_MASK;
                             break;
                }
        }
	break;
	case3:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                   case 0 :
 		DMAC ->MaskTfer_L    &= ~DMAC_MaskTfer_CH4_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L   &= ~DMAC_MaskBlock_CH4_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L &= ~DMAC_MaskSrcTran_CH4_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L &= ~DMAC_MaskDstTran_CH4_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L     &= ~DMAC_MaskErr_CH4_INT_MASK;
                             break;
                }
        }
	break;
	case4:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                   case 0 :
 		DMAC ->MaskTfer_L    &= ~DMAC_MaskTfer_CH5_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L   &= ~DMAC_MaskBlock_CH5_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L &= ~DMAC_MaskSrcTran_CH5_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L &= ~DMAC_MaskDstTran_CH5_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L     &= ~DMAC_MaskErr_CH5_INT_MASK;
                             break;
                }
        }
	break;
	case5:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L    &= ~DMAC_MaskTfer_CH6_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L   &= ~DMAC_MaskBlock_CH6_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L &= ~DMAC_MaskSrcTran_CH6_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L &= ~DMAC_MaskDstTran_CH6_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L     &= ~DMAC_MaskErr_CH6_INT_MASK;
                             break;
                }
        }
	break;
	case6:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L    &= ~DMAC_MaskTfer_CH7_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L   &= ~DMAC_MaskBlock_CH7_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L &= ~DMAC_MaskSrcTran_CH7_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L &= ~DMAC_MaskDstTran_CH7_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L     &= ~DMAC_MaskErr_CH7_INT_MASK;
                             break;
                }
        }
	break;
	case7:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L    &= ~DMAC_MaskTfer_CH8_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L   &= ~DMAC_MaskBlock_CH8_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L &= ~DMAC_MaskSrcTran_CH8_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L &= ~DMAC_MaskDstTran_CH8_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L     &= ~DMAC_MaskErr_CH8_INT_MASK;
                             break;
                }
        }
	break;
	default:
        	for(x=0;x<DMAC_MAX_INTERRUPTS;x++) {
                switch(ch_irq) {
                    case 0 :
 		DMAC ->MaskTfer_L    &= ~DMAC_MaskTfer_CH1_INT_MASK;
                             break;
                    case 1 :
 		DMAC ->MaskBlock_L   &= ~DMAC_MaskBlock_CH1_INT_MASK;
                             break;
                    //case 2 :
		DMAC ->MaskSrcTran_L &= ~DMAC_MaskSrcTran_CH1_INT_MASK;
                            break;
                    case 3 :
 		DMAC ->MaskDstTran_L &= ~DMAC_MaskDstTran_CH1_INT_MASK;
                             break;
                    case 4 :
 		DMAC ->MaskErr_L     &= ~DMAC_MaskErr_CH1_INT_MASK;
                             break;
                }
        }
	break;

    }
}
    return errorCode;
}

/**********************************************************************/
//pass
int AlDma_SetAddress(DMA_Channel_TypeDef *Channelx,enum     AL_dmac_src_dst_select sd_sel,uint32_t address)
{
    int errorCode=1;
    int x;
                if(sd_sel == Dmac_src || sd_sel == Dmac_src_dst) {
                	Channelx -> SAR_L = address;
                }
                if(sd_sel == Dmac_dst || sd_sel == Dmac_src_dst) {
                	Channelx -> DAR_L = address;
                }
        
    return errorCode;
}
//pass
int AlDma_SetChannelConfig(DMA_Channel_TypeDef *Channelx)
{
         //Set the control register
        Channelx ->CTL_L.INT_EN     		= ENABLE ;
		Channelx ->CTL_L.DST_TR_WIDTH   	= WIDTH_8;
		Channelx ->CTL_L.SRC_TR_WIDTH   	= WIDTH_8;
		Channelx ->CTL_L.LLP_SRC_EN 		= DISABLE;
		Channelx ->CTL_L.LLP_DST_EN 		= DISABLE;
		Channelx ->CTL_L.DINC           	= Increments;
		Channelx ->CTL_L.SINC           	= Increments;
		Channelx ->CTL_L.DEST_MSIZE     	= AL_DMA_MSIZE_1 ;
		Channelx ->CTL_L.SRC_MSIZE     	 	= AL_DMA_MSIZE_1 ;
		Channelx ->CTL_L.SRC_GATHER_EN  	= DISABLE;
		Channelx ->CTL_L.DST_SCATTER_EN 	= DISABLE;
		Channelx ->CTL_L.TT_FC      		= MEM2MEM ;
		Channelx ->CTL_L.DMS				= DMS_master1;
		Channelx ->CTL_L.SMS				= SMS_master1;
		Channelx ->CTL_H.BLOCK_TS       	= 5;
		Channelx ->CTL_H.DONE           	= ENABLE;

         //Set the config register
		Channelx ->CFG_L.CH_PRIOR 			= Channel_priority_0;
		Channelx ->CFG_L.CH_SUSP  			= DISABLE;
		Channelx ->CFG_L.HS_SEL_DST     	= Hardware_handshaking;
		Channelx ->CFG_L.HS_SEL_SRC     	= Hardware_handshaking;
		Channelx ->CFG_L.DST_HS_POL     	= ACTIVE_HIGH;
		Channelx ->CFG_L.SRC_HS_POL     	= ACTIVE_HIGH;
		Channelx ->CFG_L.RELOAD_SRC			= DISABLE;
		Channelx ->CFG_L.RELOAD_DST			= DISABLE;

		Channelx ->CFG_H.FCMODE         	= fc_mode_0;
		Channelx ->CFG_H.FIFO_MODE      	= FIFO_MODE_0;
		Channelx ->CFG_H.DS_UPD_EN      	= DISABLE;
		Channelx ->CFG_H.SS_UPD_EN      	= DISABLE;
		Channelx ->CFG_H.SRC_PER        	= 0x04;
		Channelx ->CFG_H.DEST_PER       	= 0x04;

         //set the SAR/DAR registers
		Channelx ->SAR_L 			= MEM_BASE1_ADDR;
		Channelx ->DAR_L			= MEM_BASE2_ADDR;

    return 0;
}
/**********************************************************************/
//pass
int AlDma_SetTransferType(DMA_Channel_TypeDef *Channelx,enum AL_dmac_transfer_type  transfer)
{
    int errorCode=1;


     //Check for valid channel number and not busy
    //errorCode = AL_dmac_checkChannelBusy(DMAC, ch_num);
    if(errorCode == 1) {
        switch(transfer) {
            case Dmac_transfer_row1 :
                if(errorCode == 1) {
		   AlDma_SetSingle(Channelx,Dmac_src_dst);
                }
                break;
 	   case Dmac_transfer_row2 :
                if(errorCode == 1) {
		   AlDma_SetSingle(Channelx,Dmac_src);
		   AlDma_SetReload(Channelx,Dmac_dst);
                }
                break;
           case Dmac_transfer_row3 :
                if(errorCode == 1) {
		   AlDma_SetSingle(Channelx,Dmac_dst);
		   AlDma_SetReload(Channelx,Dmac_src);
                }
                break;
          case Dmac_transfer_row4 :
                if(errorCode == 1) {
		  AlDma_SetReload(Channelx,Dmac_src_dst);
                }
                break;
          case Dmac_transfer_row5 :
                if(errorCode == 1) {
		  AlDma_SetSingle(Channelx,Dmac_src_dst);
                }
                break;
          case Dmac_transfer_row6 :
                if(errorCode == 1) {
		   AlDma_SetContigous(Channelx,Dmac_src);
		   AlDma_SetLlpEnable(Channelx,Dmac_dst);
                }
                break;
          case Dmac_transfer_row7 :
                if(errorCode == 1) {
		   AlDma_SetReload(Channelx,Dmac_src);
		   AlDma_SetLlpEnable(Channelx,Dmac_dst);

}
                break;
          case Dmac_transfer_row8 :
                if(errorCode == 1) {
		   AlDma_SetContigous(Channelx,Dmac_dst);
		   AlDma_SetLlpEnable(Channelx,Dmac_src);

                }
                break;
 	  case Dmac_transfer_row9 :
                if(errorCode == 1) {
		   AlDma_SetReload(Channelx,Dmac_dst);
		   AlDma_SetLlpEnable(Channelx,Dmac_src);
 }
                break;
 	  case Dmac_transfer_row10 :
                if(errorCode == 1) {
		   AlDma_SetLlpEnable(Channelx,Dmac_src_dst);
                }
                break;

        }
    }
    return errorCode;
}
/**********************************************************************/
//pass
int AlDma_SetMemPeriphFlowCtl(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_transfer_flow  tt_fc)
{
    int x, errorCode=1;

     //Check for valid channel number and not busy
    //errorCode = AlDma_checkChannelBusy(DMAC, ch_num);

         //Check for hard-coded values
    	//仅支持dmac flow 控制
        	if(Channelx ->CTL_L.TT_FC != tt_fc)
        	{
            	switch(tt_fc)
            	{
            	case 0:
            		Channelx ->CTL_L.TT_FC = MEM2MEM;
		break;
            	case 1:
            		Channelx ->CTL_L.TT_FC = MEM2PER;
            	break;
            	case 2:
            		Channelx ->CTL_L.TT_FC = PER2MEM;
                break;
            	default:
            		Channelx ->CTL_L.TT_FC = PER2PER;
                break;
            	}
        	} 
    return errorCode;
}
/**********************************************************************/
//pass
int AlDma_SetTransWidth(
		DMA_Channel_TypeDef *Channelx,
        enum   AL_dmac_src_dst_select sd_sel,
        enum   AL_dmac_transfer_width xf_width)
{
    int errorCode=1;
    int x;
    BOOL write_en;
    uint32_t reg;

     //Check for valid channel number and not busy
    //errorCode = AL_dmac_checkChannelBusy(DMAC, ch_num);

    if(errorCode == 1) {
                write_en = FALSE;
                if(sd_sel == Dmac_src || sd_sel == Dmac_src_dst) {
                    if(Channelx->CTL_L.SRC_TR_WIDTH != xf_width) {
                    	switch(xf_width)
                    	{
                    	    case 0:
                    	    	Channelx ->CTL_L.SRC_TR_WIDTH = WIDTH_8;//DMAC_CTL_L_SRC_TR_WIDTH;
                    	           write_en = TRUE;
                    	    break;
                    	    case 1:
                    	    	Channelx ->CTL_L.SRC_TR_WIDTH = WIDTH_16 ;//DMAC_CTL_L_SRC_TR_WIDTH;
                    	           write_en = TRUE;
                    	    break;
                    	    case 2:
                    	    	Channelx ->CTL_L.SRC_TR_WIDTH = WIDTH_32 ;//DMAC_CTL_L_SRC_TR_WIDTH;
                    	           write_en = TRUE;
                    	    break;
                    	    case 3:
                    	    	Channelx ->CTL_L.SRC_TR_WIDTH =WIDTH_64 ;//DMAC_CTL_L_SRC_TR_WIDTH;
                    	           write_en = TRUE;
                    	    break;
                    	    case 4:
                    	    	Channelx ->CTL_L.SRC_TR_WIDTH = WIDTH_128;//DMAC_CTL_L_SRC_TR_WIDTH;
                    	           write_en = TRUE;
                    	    break;
                    	    default:
                    	    	Channelx ->CTL_L.SRC_TR_WIDTH = WIDTH_256 ;//DMAC_CTL_L_SRC_TR_WIDTH;
                    	     	write_en = TRUE;
                    	    break;
                    	                    	}
                    }
                }
                if(sd_sel == Dmac_dst || sd_sel == Dmac_src_dst) {
                    if(Channelx->CTL_L.DST_TR_WIDTH !=xf_width) {
                    	switch(xf_width)
                    	{
                    	case 0:
                    		Channelx ->CTL_L.DST_TR_WIDTH = WIDTH_8 ;//DMAC_CTL_L_DST_TR_WIDTH;
                    			write_en = TRUE;
                        break;
                    	case 1:
                    		Channelx ->CTL_L.DST_TR_WIDTH = WIDTH_16 ;//DMAC_CTL_L_DST_TR_WIDTH;
                    			write_en = TRUE;
                        break;
                    	case 2:
                    		Channelx ->CTL_L.DST_TR_WIDTH = WIDTH_32 ;//DMAC_CTL_L_DST_TR_WIDTH;
                    			write_en = TRUE;
                        break;
                    	case 3:
                    		Channelx ->CTL_L.DST_TR_WIDTH = WIDTH_64 ;//DMAC_CTL_L_DST_TR_WIDTH;
                    			write_en = TRUE;
                        break;
                    	case 4:
                    		Channelx ->CTL_L.DST_TR_WIDTH = WIDTH_128 ;//DMAC_CTL_L_DST_TR_WIDTH;
                    			write_en = TRUE;
                        break;
                    	default:
                    		Channelx ->CTL_L.DST_TR_WIDTH = WIDTH_256 ;//DMAC_CTL_L_DST_TR_WIDTH;
                    			write_en = TRUE;
                        break;
                    	}
                    }
                }
    }
    return errorCode;
}

/**********************************************************************/
//pass
int AlDma_SetBlockTransSize(DMA_Channel_TypeDef *Channelx,uint16_t block_size)
{
    int errorCode = 1 ;
    int x;

     //Check for valid channel number and not busy
    //errorCode = AL_dmac_checkChannelBusy(DMAC, ch_num);

     if(Channelx -> CTL_H.BLOCK_TS != block_size) {

        Channelx -> CTL_H.BLOCK_TS = (uint32_t) block_size;
       }
        
    
    return errorCode;
}
/**********************************************************************/
//pass
int AlDma_SetBurstTransLength(
		DMA_Channel_TypeDef *Channelx,
        enum   AL_dmac_src_dst_select sd_sel,
        enum   AL_dmac_burst_trans_length xf_length)
{
    int errorCode=1;
    int x;
    BOOL write_en;

    //Check for valid channel number and not busy
    //errorCode = AL_dmac_checkChannelBusy(DMAC, ch_num);
    if(errorCode == 1) {
                write_en = FALSE;
                if(sd_sel == Dmac_src || sd_sel == Dmac_src_dst) {
                    if(Channelx ->CTL_L.SRC_MSIZE != xf_length) {
                    	switch(xf_length)
                    	{
                    	case 0:
                    		Channelx ->CTL_L.SRC_MSIZE = AL_DMA_MSIZE_1;
                    	break;
                    	case 1:
                    		Channelx ->CTL_L.SRC_MSIZE = AL_DMA_MSIZE_4;
                    	break;
                    	case 2:
                    		Channelx ->CTL_L.SRC_MSIZE = AL_DMA_MSIZE_8;
                    	break;
                    	case 3:
                    		Channelx ->CTL_L.SRC_MSIZE= AL_DMA_MSIZE_16;
                    	break;
                    	case 4:
                    		Channelx ->CTL_L.SRC_MSIZE = AL_DMA_MSIZE_32;
                    	break;
                    	case 5:
                    		Channelx ->CTL_L.SRC_MSIZE = AL_DMA_MSIZE_64;
                    	break;
                    	case 6:
                    		Channelx ->CTL_L.SRC_MSIZE = AL_DMA_MSIZE_128;
                    	break;
                    	default:
                    		Channelx ->CTL_L.SRC_MSIZE = AL_DMA_MSIZE_256;
                    	break;
                    	}
                        write_en = TRUE;
                    }
                }
                if(sd_sel == Dmac_dst || sd_sel == Dmac_src_dst) {
                    if(Channelx ->CTL_L.DEST_MSIZE  !=xf_length)
                    {
                    	switch(xf_length)
                    	      {
                    	       case 0:
                    	    	   Channelx ->CTL_L.DEST_MSIZE = AL_DMA_MSIZE_1;
                    	       break;
                    	       case 1:
                    	    	   Channelx ->CTL_L.DEST_MSIZE = AL_DMA_MSIZE_4;
                    	       break;
                    	       case 2:
                    	    	   Channelx ->CTL_L.DEST_MSIZE = AL_DMA_MSIZE_8;
                    	       break;
                    	       case 3:
                    	    	   Channelx ->CTL_L.DEST_MSIZE = AL_DMA_MSIZE_16;
                    	       break;
                    	       case 4:
                    	    	   Channelx ->CTL_L.DEST_MSIZE = AL_DMA_MSIZE_32;
                    	       break;
                    	       case 5:
                    	    	   Channelx ->CTL_L.DEST_MSIZE = AL_DMA_MSIZE_64;
                    	       break;
                    	       case 6:
                    	    	   Channelx ->CTL_L.DEST_MSIZE = AL_DMA_MSIZE_128;
                    	       break;
                    	       default:
                    	    	   Channelx ->CTL_L.DEST_MSIZE = AL_DMA_MSIZE_256;
                    	       break;
                    	       }
                        write_en = TRUE;
                    }
                }
    }
    return errorCode;
}
/**********************************************************************/
//pass
int AlDma_SetMstSelect(
		DMA_Channel_TypeDef *Channelx,
        	enum   AL_dmac_src_dst_select sd_sel,
		enum   AL_dmac_SRCmaster_number  SRCmst_num,
		enum   AL_dmac_DSTmaster_number  DSTmst_num)
{
    int errorCode=1;
    int x;
    BOOL write_en;

     //Check for valid channel number and not busy
    //errorCode = AL_dmac_checkChannelBusy(DMAC, ch_num);

    if(errorCode == 1) {
                write_en = FALSE;
                if(sd_sel == Dmac_src || sd_sel == Dmac_src_dst) {
                    if(Channelx -> CTL_L.SMS != SRCmst_num) {
                    	if(1 == SRCmst_num)
                    	{
                    		Channelx -> CTL_L.SMS = SMS_master2;//都是假值需要替换成具体配置
                    	}
                    	else
                    	{
                    		Channelx -> CTL_L.SMS = SMS_master1;//都是假值需要替换成具体配置
                    	}
                        write_en = TRUE;
                    }
                }
                if(sd_sel == Dmac_dst || sd_sel == Dmac_src_dst) {
                    if(Channelx -> CTL_L.DMS != DSTmst_num) {
                    	switch(DSTmst_num)
                    	{
                    		case 0:
                    			Channelx -> CTL_L.DMS = DMS_master1;
                    			break;
                    		case 1:
                    			Channelx -> CTL_L.DMS = DMS_master2;
                    		    break;
                    		case 2:
                    			Channelx -> CTL_L.DMS = DMS_master3;
                    		    break;
                    		default:
                    			Channelx -> CTL_L.DMS = DMS_master4;
                    		    break;
                    	}
                        write_en = TRUE;
                    }
                }
    }
    return errorCode;
}

/**********************************************************************/
//pass
int AlDma_SetAddressInc(
		DMA_Channel_TypeDef *Channelx,
        enum   AL_dmac_src_dst_select sd_sel,
        enum   AL_dmac_address_increment addr_inc)
{
    int errorCode=1;
    int x;
    BOOL write_en;
    uint32_t reg;
    struct AL_dmac_param *param;

     //Check for valid channel number and not busy
    //errorCode = AL_dmac_checkChannelBusy(DMAC, ch_num);
    if(errorCode == 1) {
                write_en = FALSE;
                if(sd_sel == Dmac_src || sd_sel == Dmac_src_dst) {
                    if(Channelx ->CTL_L.SINC != addr_inc) {
                    	switch(addr_inc)
                    	{
                    	case 0:
                    		Channelx ->CTL_L.SINC = Increments;
                    			write_en = TRUE;
                        break;
                    	case 1:
                    		Channelx ->CTL_L.SINC = Decrements;
                    			write_en = TRUE;
                        break;
                    	default :
                    		Channelx ->CTL_L.SINC = FIX;
                    			write_en = TRUE;
                        break;
                    	}
                    }
                }
                if(sd_sel == Dmac_dst || sd_sel == Dmac_src_dst) {
                    if(Channelx ->CTL_L.DINC != addr_inc) {
                    	switch(addr_inc)
                    	{
                    	case 0:
                    		Channelx ->CTL_L.DINC = Increments;
                    			write_en = TRUE;
                    	break;
                    	case 1:
                    		Channelx ->CTL_L.DINC = Decrements;
                    			write_en = TRUE;
                    	break;
                    	default:
                    		Channelx ->CTL_L.DINC = FIX;
                    			write_en = TRUE;
                    	break;
                    	}
                    }
                }
    }
    return errorCode;
}

/**********************************************************************/
//pass
int AlDma_SetHsPolarity(DMA_Channel_TypeDef *Channelx,
        enum   AL_dmac_src_dst_select sd_sel,
        enum   AL_dmac_polarity_level pol_level)
{
    int errorCode=1;
    int x;
    BOOL write_en;
    uint32_t reg;

    if(errorCode == 1) {
                write_en = FALSE;
                if(sd_sel == Dmac_src || sd_sel == Dmac_src_dst) {
                    if(Channelx ->CFG_L.SRC_HS_POL!=pol_level)
                    {
                    	Channelx ->CFG_L.SRC_HS_POL = ACTIVE_HIGH ;// (reg, DMAC_CFG_L_SRC_HS_POL,pol_level);
                        write_en = TRUE;
                    }
                }
                if(sd_sel == Dmac_dst || sd_sel == Dmac_src_dst) {
                    if(Channelx ->CFG_L.DST_HS_POL !=pol_level)
                    {
                    	Channelx ->CFG_L.DST_HS_POL = ACTIVE_HIGH;
                        write_en = TRUE;
                    }
                }  
    }
    return errorCode;
}
/**********************************************************************/
//pass
int AlDma_SetFifoMode(
		DMA_Channel_TypeDef *Channelx,
        enum   AL_dmac_fifo_mode fifo_mode)
{
    int errorCode = 1;
    int x;

    if(errorCode == 1) {
    
                if(Channelx ->CFG_H.FIFO_MODE != fifo_mode) {
                	if(fifo_mode == 1)
                	{
                		Channelx ->CFG_H.FIFO_MODE = FIFO_MODE_0 ;// DMAC_CFG_H_FIFO_MODE, fifo_mode
                	}
                	else
                	{
                		Channelx ->CFG_H.FIFO_MODE = FIFO_MODE_1;
                	}
                }
        
    }
    return errorCode;
}
/**********************************************************************/
//pass
int AlDma_SetFlowCtlMode(
		DMA_Channel_TypeDef *Channelx,
        enum   AL_dmac_flow_ctl_mode  fc_mode)
{
    int errorCode=1;
    int x;

    if(errorCode == 1) {
                if(Channelx -> CFG_H.FCMODE != fc_mode_0) {
                	Channelx -> CFG_H.FCMODE = fc_mode_1; //(reg, DMAC_CFG_H_FCMODE, fc_mode);
                }
        
    }
    return errorCode;
}
/**********************************************************************/
//pass
int AlDma_SetHandshakingMode(
		DMA_Channel_TypeDef *Channelx,
        enum   AL_dmac_src_dst_select  sd_sel,
        enum   AL_dmac_sw_hw_hs_select hs_hwsw_sel)
{
    int errorCode=1;
    int x;
    BOOL write_en;

    if(errorCode == 1) {
                write_en = FALSE;
                if(sd_sel == Dmac_src || sd_sel == Dmac_src_dst) {
                    if(Channelx -> CFG_L.HS_SEL_SRC!= hs_hwsw_sel) {
                    	Channelx -> CFG_L.HS_SEL_SRC = Hardware_handshaking;// (reg, DMAC_CFG_L_HS_SEL_SRC,hs_hwsw_sel);
                        write_en = TRUE;
                    }
                }
                if(sd_sel == Dmac_dst || sd_sel == Dmac_src_dst) {
                    if(Channelx -> CFG_L.HS_SEL_DST !=hs_hwsw_sel) {
                       Channelx -> CFG_L.HS_SEL_DST = Hardware_handshaking;
                        write_en = TRUE;
                    }
                }
        
    }
    return errorCode;
}
int AlDma_SetContigous(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_src_dst_select sd_sel){
	 if(sd_sel == Dmac_src) {
                   	Channelx ->CTL_L.LLP_SRC_EN = DISABLE;
                  
                }
         if(sd_sel == Dmac_dst) {
			Channelx ->CTL_L.LLP_SRC_EN = DISABLE;
                        
                }
}
/**********************************************************************/
int AlDma_SetReload(
		DMA_Channel_TypeDef *Channelx,
        enum   AL_dmac_src_dst_select sd_sel)
{
    int x, errorCode=1;
    BOOL write_en;
    uint32_t reg;
    //struct dw_dmac_param *param;
    if(errorCode == 1) {
                write_en = FALSE;
                if(sd_sel == Dmac_src || sd_sel == Dmac_src_dst) {
                   	Channelx ->CTL_L.LLP_SRC_EN = DISABLE;
                    	Channelx ->CFG_L.RELOAD_SRC = ENABLE;
                        write_en = TRUE;
                }
                if(sd_sel == Dmac_dst || sd_sel == Dmac_src_dst) {
			Channelx ->CTL_L.LLP_SRC_EN = DISABLE;
                    	Channelx ->CFG_L.RELOAD_DST = ENABLE;
                        write_en = TRUE;
                }

        
    }
    return errorCode;
}

/**********************************************************************/
int AlDma_SetSingle(
	DMA_Channel_TypeDef *Channelx,
        enum   AL_dmac_src_dst_select sd_sel
		)
{
    int x, errorCode=1;
    BOOL write_en;
    uint32_t reg;
    //struct dw_dmac_param *param;
    if(errorCode == 1) {
        for(x=0;x<num_channels;x++) {
                write_en = FALSE;
                if(sd_sel == Dmac_src || sd_sel == Dmac_src_dst) {

                    	Channelx ->CFG_L.RELOAD_SRC = DISABLE;
                        write_en = TRUE;

                }
                if(sd_sel == Dmac_dst || sd_sel == Dmac_src_dst) {
                    	Channelx ->CFG_L.RELOAD_DST = DISABLE;
                        write_en = TRUE;
                    }

        }
    }
    return errorCode;
}
/**********************************************************************/
int AlDma_SetLlpEnable(
	DMA_Channel_TypeDef *Channelx,
        enum   AL_dmac_src_dst_select sd_sel)
{
    int x, errorCode=1;
    BOOL write_en;
    uint32_t reg;
    //struct dw_dmac_param *param;
    if(errorCode == 1) {
        for(x=0;x<num_channels;x++) {
                write_en = FALSE;
                if(sd_sel == Dmac_src || sd_sel == Dmac_src_dst) {
			Channelx ->CFG_L.RELOAD_SRC = DISABLE;
                    	Channelx ->CTL_L.LLP_SRC_EN = ENABLE;
                        write_en = TRUE;
                }
                if(sd_sel == Dmac_dst || sd_sel == Dmac_src_dst) {
			Channelx ->CFG_L.RELOAD_SRC = DISABLE;
                    	Channelx ->CTL_L.LLP_DST_EN  =ENABLE;
                        write_en = TRUE;
                }

        }
    }
    return errorCode;
}
int AlDma_SetListPointerAddress(
		DMA_Channel_TypeDef *Channelx,
		enum   AL_dmac_LMSmaster_number LMS_master,
		uint32_t address)
{
	int errorcode = 1;
	int x         = 0;
	if(errorcode == 1){
		if(Channelx ->LLP_L.LMS != LMS_master1)
			switch(LMS_master){
				case 0:
   					Channelx ->LLP_L.LMS != LMS_master1;
				break;
				case 1:
					Channelx ->LLP_L.LMS != LMS_master2;
				break;
				case 2:
   					Channelx ->LLP_L.LMS != LMS_master3;
				break;
				defulat:
   					Channelx ->LLP_L.LMS != LMS_master4;
				break;

    }
}
	if(errorcode == 1){
		
			if(Channelx -> LLP_L.LOC != address){
				Channelx -> LLP_L.LOC =((uint32_t) address >> 2);
	    }
		else{
			Channelx -> LLP_L.LOC =((uint32_t) address >> 2);
	    }
        
    }
	return errorcode;
}
/**********************************************************************/
#if 0
void dw_dmac_addLliItem(
			list_t     **list_head,
			uint32_t ctl_l_reg_par,
			uint32_t ctl_h_reg_par,
			uint32_t SARaddress,
			uint32_t DSTaddress,
			uint32_t address){
    list_t *temp        = (list_t *)malloc(sizeof(list_t));
    temp -> next = NULL;
    temp -> list0.sar   = SARaddress;
    temp -> list0.dst   = DSTaddress;
    temp -> list0.llp   = address;
    temp -> list0.ctl_l = ctl_l_reg_par;
    temp -> list0.ctl_h = ctl_h_reg_par;
    if(*list_head == NULL){
			(*list_head) = temp;
	}else{
		list_t *t = (*list_head);
		if(temp ->list0.sar < (*list_head)-> sar){
		    temp ->next = (*list_head);
		    (*list_head)= temp;
		    return;
	}
	while(t != NULL){
             if(t ->next == NULL){
		t ->next = temp;
		return;
           }else if(t ->next->sar > temp ->list0.sar){
	        temp ->next = t ->next;
		t -> next   = temp;
		return;
	   }
	   t =  t ->next;
	}
    }
#if 0
    dmac_node -> ctl_h = ctl_h_reg;
    dmac_node -> ctl_l = ctl_l_reg;
    dmac_node -> llp   = address;
    dmac_node -> sar   = SARaddress;
    dmac_node -> dar   = DSTaddress;
    dmac_node -> next  =NULL;
			if(head == NULL){
				head = dmac_node;
			}
			else{
				q -> next = dmac_node;
			}
			q         = dmac_node;
#endif
}
#endif
#if 0
/**********************************************************************/
void dw_dmac_addLliItem(
			uint32_t ctl_l_reg_par,
			uint32_t ctl_h_reg_par,
			uint32_t SARaddress,
			uint32_t DSTaddress,
			uint32_t address){
    temp -> sar  = SARaddress;
    temp -> dst  = DSTaddress;
    temp -> llp  = address;
    temp -> ctl_l= ctl_l_reg_par;
    temp -> ctl_h= ctl_h_reg_par;
#if 0
    dmac_node -> ctl_h = ctl_h_reg;
    dmac_node -> ctl_l = ctl_l_reg;
    dmac_node -> llp   = address;
    dmac_node -> sar   = SARaddress;
    dmac_node -> dar   = DSTaddress;
    dmac_node -> next  =NULL;
			if(head == NULL){
				head = dmac_node;
			}
			else{
				q -> next = dmac_node;
			}
			q         = dmac_node;
#endif
}
#endif
#if 0
/**********************************************************************/
void dw_dmac_addLliItem(DMA_Channel_TypeDef *Channelx,uint32_t ctl_l_parameter,uint32_t ctl_h_parameter,uint32_t list_length){


     struct dw_dmac_lli_item *head , *dmac_node,*q;

     volatile uint32_t ctl_l_reg,ctl_h_reg;
		ctl_l_reg = ((uint32_t)0x00001008);
		ctl_h_reg = ((uint32_t)0x18004801);
		head = NULL;
		dmac_node = (struct dw_dmac_lli_item *)malloc(sizeof(struct dw_dmac_lli_item));
		dmac_node -> ctl_h = ctl_h_reg;
		dmac_node -> ctl_l = ctl_l_reg;
		dmac_node -> sar   = Channelx -> LLP_L.LOC + 0X4;
		dmac_node -> dar   = Channelx -> LLP_L.LOC + 0X8;
		dmac_node -> next  =NULL;
			if(head == NULL)
				head = dmac_node;
			else
				q -> next = dmac_node;
				q         = dmac_node;
}
#endif
/**********************************************************************/
int AlDma_CheckChannelBusy(AL_DMAC_TypeDef *DMAC)
{
    uint8_t reg = 0;

    reg = DMAC -> ChEnReg_L;
    if(reg == 0){
	return 0;
	}
    else{
    	return 1;
	}
}
uint8_t dam_handshake_interfance(DMA_Channel_TypeDef *Channelx,enum HS_interfance HS_TYPE){
	Channelx ->CFG_H.SRC_PER        	= HS_TYPE;
	Channelx ->CFG_H.DEST_PER       	= HS_TYPE;

}
void write_To_OCM(uint32_t* pSnAddr, uint32_t Count, uint32_t* pDnAddr)
{
    volatile uint32_t i;
    if (Count < 0 || Count == 0) return;
    for (i = 0; i < Count; i++)
    {
        *pDnAddr++ = *pSnAddr++;
    }
}
void write_iic_data(enum i2c_cmd_type type,uint32_t transimte_data_length){
	volatile uint32_t i = 0;
	volatile uint32_t j = 0;
	volatile uint32_t data[transimte_data_length];
	if(type == normal){
		for(i = 0 ; i < transimte_data_length ; i++){
			data[i] = i ;
			if(i == (transimte_data_length -1)){
				data[i] = (uint32_t)data[i]|(uint32_t)0x00000200;
			}
		}
		write_To_OCM((uint32_t*)data,transimte_data_length,(uint32_t*)MEM_BASE1_ADDR);
	}
	if(type == restart){
		for(i = 0 ; i < transimte_data_length ; i++){
			data[i] = i ;
		}
		data[i] = (uint32_t)data[i]|(uint32_t)0x00000200;
		write_To_OCM((uint32_t*)data,transimte_data_length,(uint32_t*)MEM_BASE1_ADDR);
	}
	else
	{}
}
#if 0
int can_dma_mode(AL_CAN_TypeDef* CANX,enum data_length len) {
    volatile uint32_t RBUFFER[2000];
    volatile uint8_t count,val ;
    //val = GET_BITS(CANX -> RBUF_CTL,0,3);
    switch (val) {
    case 0:
        write_To_OCM((uint32_t*)MEM_BASE1_ADDR, 80, (uint32_t*)RBUFFER);
        /*for (count = 0; count < 2000; count++)
        {
           // printf("can_dma_data_length_1", RBUFFER[count]);
        }*/
        break;
    default:
    	write_To_OCM((uint32_t*)MEM_BASE1_ADDR, 80, (uint32_t*)RBUFFER);
    	       /* for (count = 0; count < 2000; count++)
    	        {
    	           // printf("can_dma_data_length_1", RBUFFER[count]);
    	        }*/
        break;
    }
    return 0;
}
#endif
/*
uint8_t dma_Periphselect(uint32_t dma_mode){
    switch(tt_fc){
      case PER2MEM:
          return PER2MEM;
      break;
      case MEM2PER:
        return MEM2PER2;
      break;
      case MEM2MEM:
        return MEM2MEM;
      break;
}
   
}*/
int dma_blocklist_function(uint32_t srcaddress,uint32_t dstaddress,uint32_t llp_address,uint32_t block_number) {
    volatile uint8_t  block_count = 0;
    volatile uint8_t val = 0;
    volatile uint32_t buffer[5];
    //val = dma_block_chain_select(uint32_t srcaddress,uint32_t dma_mode, enum   AL_dmac_transfer_flow  tt_fc)
    if(srcaddress == AL_CAN0_BASE ||dstaddress == AL_CAN0_BASE || srcaddress == AL_CAN1_BASE || dstaddress == AL_CAN1_BASE ){
        for (volatile uint8_t i = 0; i < 10; i++) {
            for (volatile uint8_t j = 0; j < 5; j++) {
                switch (j){
                case 0:
                    buffer_list_parm[i][j] = srcaddress;
                    break;
                case 1:
                    buffer_list_parm[i][j] = dstaddress +i*80;
                    break;
                case 2:
		    if(i ==100){
			buffer_list_parm[i][j] = llp_address ;
                    }
		    else
                    buffer_list_parm[i][j] = llp_address + (i+1)*20;
                    break;
                case 3:
                    buffer_list_parm[i][j] = can_ctl_parm_l;
                    break;
                case 4:
                    buffer_list_parm[i][j] = can_ctl_parm_h;
                    break;
                }
            }
        }
      for (volatile uint32_t counti = 0 ; counti< block_number ;counti++)
	{
		for (volatile uint32_t countj = 0 ; countj < 5 ; countj++)
		{
 			buffer[countj] = (uint32_t)buffer_list_parm[counti][countj];
 		}
	      
              write_To_OCM((uint32_t*)buffer,5,(uint32_t*)((uint32_t)llp_address + counti*20));
	}
     }
    if(srcaddress == AL_UART0_BASE ||dstaddress == AL_UART0_BASE || srcaddress == AL_UART1_BASE ||dstaddress == AL_UART1_BASE ){
	if(srcaddress == AL_UART0_BASE || srcaddress == AL_UART1_BASE){
        for (volatile uint8_t i = 0; i < 10; i++) {
            for (volatile uint8_t j = 0; j < 5; j++) {
                switch (j){
                case 0:
                    buffer_list_parm[i][j] = srcaddress;
	            break;
                case 1:
                    buffer_list_parm[i][j] = dstaddress +i;
                    break;
                case 2:
                    buffer_list_parm[i][j] = llp_address + i*20;
                    break;
                case 3:
                    buffer_list_parm[i][j] = can_ctl_parm_l;
                    break;
                case 4:
                    buffer_list_parm[i][j] = can_ctl_parm_h;
                    break;
                }
            }
        }
      for (volatile uint32_t counti = 0 ; counti< block_number ;counti++)
	{
		for (volatile uint32_t countj = 0 ; countj < 5 ; countj++)
		{
 			buffer[countj] = (uint32_t)buffer_list_parm[counti][countj];
 		}
	      
              write_To_OCM((uint32_t*)buffer,5,(uint32_t*)((uint32_t)llp_address + counti*20));
	}
     }
	if(dstaddress == AL_UART0_BASE || dstaddress == AL_UART1_BASE){
        for (volatile uint8_t i = 0; i < 10; i++) {
            for (volatile uint8_t j = 0; j < 5; j++) {
                switch (j){
                case 0:
                    buffer_list_parm[i][j] = srcaddress + i;
	            break;
                case 1:
                    buffer_list_parm[i][j] = dstaddress;
                    break;
                case 2:
                    buffer_list_parm[i][j] = llp_address + (i+1)*20;
                    break;
                case 3:
                    buffer_list_parm[i][j] = can_ctl_parm_l;
                    break;
                case 4:
                    buffer_list_parm[i][j] = can_ctl_parm_h;
                    break;
                }
            }
        }
      for (volatile uint32_t counti = 0 ; counti< block_number ;counti++)
	{
		for (volatile uint32_t countj = 0 ; countj < 5 ; countj++)
		{
 			buffer[countj] = (uint32_t)buffer_list_parm[counti][countj];
 		}
	      
              write_To_OCM((uint32_t*)buffer,5,(uint32_t*)((uint32_t)llp_address + counti*20));
	}
     }

     }
 if(srcaddress == spi0_data_base_address ||dstaddress == spi0_data_base_address || srcaddress == spi1_data_base_adderss||dstaddress == spi1_data_base_adderss ){
	if(srcaddress == spi0_data_base_address || srcaddress == spi1_data_base_adderss){
        for (volatile uint8_t i = 0; i < 10; i++) {
            for (volatile uint8_t j = 0; j < 5; j++) {
                switch (j){
                case 0:
                    buffer_list_parm[i][j] = srcaddress;
	            break;
                case 1:
                    buffer_list_parm[i][j] = dstaddress +i;
                    break;
                case 2:
                    buffer_list_parm[i][j] = llp_address + i*20;
                    break;
                case 3:
                    buffer_list_parm[i][j] = can_ctl_parm_l;
                    break;
                case 4:
                    buffer_list_parm[i][j] = spi_ctl_parm_h;
                    break;
                }
            }
        }
      for (volatile uint32_t counti = 0 ; counti< block_number ;counti++)
	{
		for (volatile uint32_t countj = 0 ; countj < 5 ; countj++)
		{
 			buffer[countj] = (uint32_t)buffer_list_parm[counti][countj];
 		}
	      
              write_To_OCM((uint32_t*)buffer,5,(uint32_t*)((uint32_t)llp_address + counti*20));
	}
     }
	if(dstaddress == spi0_data_base_address || dstaddress == spi1_data_base_adderss){
        for (volatile uint8_t i = 0; i < 10; i++) {
            for (volatile uint8_t j = 0; j < 5; j++) {
                switch (j){
                case 0:
                    buffer_list_parm[i][j] = srcaddress + i;
	            break;
                case 1:
                    buffer_list_parm[i][j] = dstaddress;
                    break;
                case 2:
                    buffer_list_parm[i][j] = llp_address + (i+1)*20;
                    break;
                case 3:
                    buffer_list_parm[i][j] = can_ctl_parm_l;
                    break;
                case 4:
                    buffer_list_parm[i][j] = spi_ctl_parm_h;
                    break;
                }
            }
        }
      for (volatile uint32_t counti = 0 ; counti< block_number ;counti++)
	{
		for (volatile uint32_t countj = 0 ; countj < 5 ; countj++)
		{
 			buffer[countj] = (uint32_t)buffer_list_parm[counti][countj];
 		}
	      
              write_To_OCM((uint32_t*)buffer,5,(uint32_t*)((uint32_t)llp_address + counti*20));
	}
     }

     }
 if(srcaddress == qspi_data_base_address ||dstaddress == qspi_data_base_address){
	if(srcaddress == qspi_data_base_address ){
        for (volatile uint8_t i = 0; i < 10; i++) {
            for (volatile uint8_t j = 0; j < 5; j++) {
                switch (j){
                case 0:
                    buffer_list_parm[i][j] = srcaddress;
	            break;
                case 1:
                    buffer_list_parm[i][j] = dstaddress +i;
                    break;
                case 2:
                    buffer_list_parm[i][j] = llp_address + i*20;
                    break;
                case 3:
                    buffer_list_parm[i][j] = can_ctl_parm_l;
                    break;
                case 4:
                    buffer_list_parm[i][j] = can_ctl_parm_h;
                    break;
                }
            }
        }
      for (volatile uint32_t counti = 0 ; counti< block_number ;counti++)
	{
		for (volatile uint32_t countj = 0 ; countj < 5 ; countj++)
		{
 			buffer[countj] = (uint32_t)buffer_list_parm[counti][countj];
 		}
	      
              write_To_OCM((uint32_t*)buffer,5,(uint32_t*)((uint32_t)llp_address + counti*20));
	}
     }
	if(dstaddress == qspi_data_base_address ){
        for (volatile uint8_t i = 0; i < 10; i++) {
            for (volatile uint8_t j = 0; j < 5; j++) {
                switch (j){
                case 0:
                    buffer_list_parm[i][j] = srcaddress + i;
	            break;
                case 1:
                    buffer_list_parm[i][j] = dstaddress;
                    break;
                case 2:
                    buffer_list_parm[i][j] = llp_address + (i+1)*20;
                    break;
                case 3:
                    buffer_list_parm[i][j] = can_ctl_parm_l;
                    break;
                case 4:
                    buffer_list_parm[i][j] = can_ctl_parm_h;
                    break;
                }
            }
        }
      for (volatile uint32_t counti = 0 ; counti< block_number ;counti++)
	{
		for (volatile uint32_t countj = 0 ; countj < 5 ; countj++)
		{
 			buffer[countj] = (uint32_t)buffer_list_parm[counti][countj];
 		}
	      
              write_To_OCM((uint32_t*)buffer,5,(uint32_t*)((uint32_t)llp_address + counti*20));
	}
     }

     }

    return 0;
}
uint8_t cpu_dma_read_block_num(uint32_t llp_address){
   volatile uint32_t count = 0;
   volatile const uint32_t DMAC_DONE_BIT_Status = 1;
       for(volatile uint32_t i = 0 ; i <10 ; i++ ){
           if(DMAC_DONE_BIT_Status ==((*(uint32_t *)llp_address + i*20 + 0x10) >> 12));
	        {
                  count ++;
                 } 
            }
    return count;
}
void updata_ctl_parm_h(uint32_t llp_address){
   volatile uint32_t count = 0; 
   volatile uint32_t buffer[100];
   count =  cpu_dma_read_block_num(llp_address);
   for (volatile uint8_t i = 0; i < count; i++) {
         buffer_list_parm[i][4] = can_ctl_parm_h;     			
 	 buffer[i] = (uint32_t)buffer_list_parm[i][4];
         write_To_OCM((uint32_t*)buffer,1,(uint32_t*)buffer_list_parm[i][4]);
     }
}
void dma_interrupt_readfunction(uint32_t llp_address){
 cpu_dma_read_block_num(llp_address);
 updata_ctl_parm_h(llp_address);
}
void can_handshank_function(TOPCFG_TypeDef* TOP){
	volatile uint8_t i;
	GET_BITS(TOP ->can_dma,0,4);
	TOP ->can_dma |= (uint32_t)0x00000008;
}
void can_dma_enable(TOPCFG_TypeDef* TOP,AL_CAN_TypeDef* CANX){
    if(CANX == AL_CAN0){
         TOP ->can_fd_ctrl |= (uint32_t)0x00000002;
     }
    else{
         TOP ->can_fd_ctrl |= (uint32_t)0x00000200;
    }
}
void spi_dma_data_level(SPI_TypeDef *spi,uint8_t tx_data_level,uint8_t rx_data_level){
    spi -> DMATDLR = tx_data_level;
    spi -> DMARDLR = rx_data_level;
}
void spi_dma_tx_enable(SPI_TypeDef *spi){
    spi -> DMACR |= SPI_DMACR_TDMAE;	
}
void spi_dma_rx_enable(SPI_TypeDef *spi){
    spi -> DMACR |= SPI_DMACR_RDMAE;	 
}
void qspi_dma_data_level(QSPI_TypeDef *qspi,uint8_t tx_data_level,uint8_t rx_data_level){
    qspi -> DMATDLR = tx_data_level;
    qspi -> DMARDLR = rx_data_level;
}
void qspi_dma_tx_enable(QSPI_TypeDef *qspi){
    qspi -> DMACR |= SPI_DMACR_TDMAE;	
}
void qspi_dma_rx_enable(QSPI_TypeDef *qspi){
    qspi -> DMACR |= SPI_DMACR_RDMAE;	 
}
void i2c_dma_data_level(I2C_TypeDef *i2c,uint8_t tx_data_level,uint8_t rx_data_level){
    i2c ->IC_DMA_TDLR = tx_data_level;
    i2c ->IC_DMA_RDLR = rx_data_level;

}
void i2c_dma_tx_enable(I2C_TypeDef *i2c){
    i2c ->IC_DMA_CR.TDMAE = ENABLE;
}
void i2c_dma_rx_enable(I2C_TypeDef *i2c){
	i2c ->IC_DMA_CR.RDMAE = ENABLE;
}
void AlSpi_DmaInit(DMA_Channel_TypeDef *Channelx,enum PER_type PER,enum dma_type type,uint8_t tx_data_level,uint8_t rx_data_level,enum AL_dmac_transfer_type  transfer,uint16_t block_size){
	 AlDma_SetChannelConfig(Channelx);
        if(type == tx)
	{
	     if(PER == spi0)
		{
    	    AlDma_SetAddress(Channelx,Dmac_dst,spi0_data_base_address);
		    dam_handshake_interfance(Channelx,spi0_tx_hs_interfance);
		    spi_dma_tx_enable(SPI0);
		    spi_dma_data_level(SPI0,tx_data_level,rx_data_level);
		}
    	  else if(PER == spi1){
	        AlDma_SetAddress(Channelx,Dmac_dst,spi1_data_base_adderss);
		    dam_handshake_interfance(Channelx,spi1_tx_hs_interfance);
		    spi_dma_tx_enable(SPI1);
	        }
    	    AlDma_SetMemPeriphFlowCtl(Channelx,MEM2PER);
    	    AlDma_SetAddressInc(Channelx,Dmac_dst,FIX);
    	    spi_dma_data_level(SPI1,tx_data_level,rx_data_level);
	}
    else{
	     if(PER == spi0)
		{
    	    AlDma_SetAddress(Channelx,Dmac_src,spi0_data_base_address);
		    dam_handshake_interfance(Channelx,spi0_rx_hs_interfance);
		    spi_dma_rx_enable(SPI0);
		    spi_dma_data_level(SPI0,tx_data_level,rx_data_level);
		}
     else if(PER == spi1){
	        AlDma_SetAddress(Channelx,Dmac_src,spi1_data_base_adderss);
		    dam_handshake_interfance(Channelx,spi1_rx_hs_interfance);
		    spi_dma_rx_enable(SPI1);
	        }
	     AlDma_SetMemPeriphFlowCtl(Channelx,PER2MEM);
	     AlDma_SetAddressInc(Channelx,Dmac_src,FIX);
	     spi_dma_data_level(SPI1,tx_data_level,rx_data_level);
	}
     AlDma_SetMemPeriphFlowCtl(Channelx,transfer);
	 AlDma_SetTransWidth(Channelx,Dmac_src_dst,WIDTH_32);
	 AlDma_SetBlockTransSize(Channelx,block_size);
	 AlDma_SetBurstTransLength(Channelx,Dmac_src_dst,AL_DMA_MSIZE_1);
}
void AlQspi_DmaInit(DMA_Channel_TypeDef *Channelx,enum dma_type type,uint8_t tx_data_level,uint8_t rx_data_level,enum AL_dmac_transfer_type  transfer,uint16_t block_size){
	 AlDma_SetChannelConfig(Channelx);
        if(type == tx)
	{
    	    AlDma_SetAddress(Channelx,Dmac_dst,qspi_data_base_address);
	    dam_handshake_interfance(Channelx,qspi_tx_hs_interfance);
            qspi_dma_tx_enable(QSPI0);
    	    AlDma_SetMemPeriphFlowCtl(Channelx,MEM2PER);
	    AlDma_SetAddressInc(Channelx,Dmac_dst,FIX);
	}
        else{
	     AlDma_SetAddress(Channelx,Dmac_src,qspi_data_base_address);
	     dam_handshake_interfance(Channelx,qspi_rx_hs_interfance);
	     qspi_dma_rx_enable(QSPI0);
	     AlDma_SetMemPeriphFlowCtl(Channelx,PER2MEM);
	     AlDma_SetAddressInc(Channelx,Dmac_src,FIX);
	}
         AlDma_SetTransferType(Channelx,transfer);
	 AlDma_SetTransWidth(Channelx,Dmac_src_dst,WIDTH_32);
	 AlDma_SetBlockTransSize(Channelx,block_size);
	 AlDma_SetBurstTransLength(Channelx,Dmac_src_dst,AL_DMA_MSIZE_1);
	 qspi_dma_data_level(QSPI0,tx_data_level,rx_data_level);
}
void AlCan_DmaInit(DMA_Channel_TypeDef *Channelx,enum PER_type PER,enum AL_dmac_transfer_type  transfer) {
    AlDma_SetChannelConfig(Channelx);
	if(PER == can0){
    	    	AlDma_SetAddress(Channelx,Dmac_src,AL_CAN0_BASE);
		    dam_handshake_interfance(Channelx,can0_rx_hs_interfance);
		    can_dma_enable(AL_TOP0,AL_CAN0);
	}
    	else if(PER == can1){
	        AlDma_SetAddress(Channelx,Dmac_src,AL_CAN1_BASE);
		dam_handshake_interfance(Channelx,can1_rx_hs_interfance);
		can_dma_enable(AL_TOP0,AL_CAN1);
	}
    AlDma_SetTransferType(Channelx,transfer);
    AlDma_SetMemPeriphFlowCtl(Channelx,PER2MEM);
    AlDma_SetTransWidth(Channelx,Dmac_src_dst,WIDTH_32);
    AlDma_SetBlockTransSize(Channelx,20);
    AlDma_SetBurstTransLength(Channelx,Dmac_src_dst,AL_DMA_MSIZE_4);
    //AlDma_SetAddressInc(Channelx,Dmac_src,FIX);
}
void AlUart_DmaInit(DMA_Channel_TypeDef *Channelx,enum PER_type PER,enum dma_type type,enum AL_dmac_transfer_type  transfer,uint16_t block_size) {
    AlDma_SetChannelConfig(Channelx);
    if(PER == uart0){
 	if(type == tx){
	    AlDma_SetAddress(Channelx,Dmac_dst,AL_UART0_BASE);
	    dam_handshake_interfance(Channelx,uart0_tx_hs_interfance);
    	    AlDma_SetMemPeriphFlowCtl(Channelx,MEM2PER);
	    AlDma_SetAddressInc(Channelx,Dmac_dst,FIX);
	}
    	else{
 	     AlDma_SetAddress(Channelx,Dmac_src,AL_UART0_BASE);
	     dam_handshake_interfance(Channelx,uart0_rx_hs_interfance);
	     AlDma_SetMemPeriphFlowCtl(Channelx,PER2MEM);
	     AlDma_SetAddressInc(Channelx,Dmac_src,FIX);
	}
   }
    else if(PER == uart1){
	   if(type == tx){
	    AlDma_SetAddress(Channelx,Dmac_dst,AL_UART1_BASE);
	    dam_handshake_interfance(Channelx,uart1_tx_hs_interfance);
    	    AlDma_SetMemPeriphFlowCtl(Channelx,MEM2PER);
	    AlDma_SetAddressInc(Channelx,Dmac_dst,FIX);
	}
    	else{
 	     AlDma_SetAddress(Channelx,Dmac_src,AL_UART1_BASE);
	     dam_handshake_interfance(Channelx,uart1_rx_hs_interfance);
	     AlDma_SetMemPeriphFlowCtl(Channelx,PER2MEM);
	     AlDma_SetAddressInc(Channelx,Dmac_src,FIX);
	}
    }
    AlDma_SetTransWidth(Channelx,Dmac_src_dst,WIDTH_32);
    AlDma_SetBlockTransSize(Channelx,block_size);
    AlDma_SetTransferType(Channelx,transfer);
    AlDma_SetBurstTransLength(Channelx,Dmac_src_dst,AL_DMA_MSIZE_4);
}
void AlI2c_DmaInit(DMA_Channel_TypeDef *Channelx,enum PER_type PER,enum dma_type type,uint8_t tx_data_level,uint8_t rx_data_level,enum AL_dmac_transfer_type  transfer,uint16_t block_size) {
    AlDma_SetChannelConfig(Channelx);
    if(PER == i2c0){
     	if(type == tx){
    	    AlDma_SetAddress(Channelx,Dmac_dst,i2c0_data_base_address);
    	    dam_handshake_interfance(Channelx,i2c0_tx_hs_interfance);
        	AlDma_SetMemPeriphFlowCtl(Channelx,MEM2PER);
    	    AlDma_SetAddressInc(Channelx,Dmac_dst,FIX);
    	    i2c_dma_tx_enable(I2C0);
    	    i2c_dma_data_level(I2C0,tx_data_level,rx_data_level);
    	}
        	else{
     	     AlDma_SetAddress(Channelx,Dmac_src,i2c0_data_base_address);
    	     dam_handshake_interfance(Channelx,i2c0_rx_hs_interfance);
    	     AlDma_SetMemPeriphFlowCtl(Channelx,PER2MEM);
    	     AlDma_SetAddressInc(Channelx,Dmac_src,FIX);
    	     i2c_dma_rx_enable(I2C0);
    	     i2c_dma_data_level(I2C0,tx_data_level,rx_data_level);
    	}
       }
        else if(PER == i2c1){
    	   if(type == tx){
    	    AlDma_SetAddress(Channelx,Dmac_dst,i2c1_data_base_address);
    	    dam_handshake_interfance(Channelx,i2c1_tx_hs_interfance);
        	AlDma_SetMemPeriphFlowCtl(Channelx,MEM2PER);
    	    AlDma_SetAddressInc(Channelx,Dmac_dst,FIX);
    	    i2c_dma_tx_enable(I2C1);
    	    i2c_dma_data_level(I2C1,tx_data_level,rx_data_level);
    	}
        	else{
     	     AlDma_SetAddress(Channelx,Dmac_src,i2c1_data_base_address);
    	     dam_handshake_interfance(Channelx,i2c1_rx_hs_interfance);
    	     AlDma_SetMemPeriphFlowCtl(Channelx,PER2MEM);
    	     AlDma_SetAddressInc(Channelx,Dmac_src,FIX);
    	     i2c_dma_rx_enable(I2C1);
    	     i2c_dma_data_level(I2C1,tx_data_level,rx_data_level);
    	}
        }
        AlDma_SetTransWidth(Channelx,Dmac_src_dst,WIDTH_32);
        AlDma_SetBlockTransSize(Channelx,block_size);
        AlDma_SetTransferType(Channelx,transfer);
        AlDma_SetBurstTransLength(Channelx,Dmac_src_dst,AL_DMA_MSIZE_1);
}

#if 0
void uart_dma_Init(){
	 AL_dmac_setChannelConfig(AL_DMAC_channel_0);
	 AL_dmac_setAddress(AL_DMAC_channel_0,Dmac_dst,AL_UART0_BASE);
	 //AL_dmac_setAddress(AL_DMAC_channel_0,AL_dmac_channel_num_1,Dmac_dst,AL_UART0_BASE);
	 AL_dmac_setTransferType(AL_DMAC_channel_0,Dmac_transfer_row4);
	 AL_dmac_setMemPeriphFlowCtl(AL_DMAC_channel_0,MEM2PER);
	 AL_dmac_setTransWidth(AL_DMAC_channel_0,Dmac_src_dst,WIDTH_32);
	 AL_dmac_setBlockTransSize(AL_DMAC_channel_0,50);
	 AL_dmac_setBurstTransLength(AL_DMAC_channel_0,Dmac_src_dst,AL_DMA_MSIZE_32);
	 AL_dmac_setAddressInc(AL_DMAC_channel_0,Dmac_dst,FIX);
         //AL_dmac_setLlpEnable(AL_DMAC_channel_0,AL_dmac_channel_num_1,Dmac_src_dst);
         //dma_blocklist_function(AL_CAN0_BASE,MEM_BASE1_ADDR,LLP_BASE_ADDR);

}
void can_dma_Init(TOPCFG_TypeDef* TOP, enum can_type type, AL_CAN_TypeDef* CANX) {
    AL_dmac_setChannelConfig(AL_DMAC_channel_0);
    AL_dmac_setAddress(AL_DMAC_channel_0,Dmac_src,AL_CAN0_BASE);
    AL_dmac_setAddress(AL_DMAC_channel_0,Dmac_dst,MEM_BASE1_ADDR);
    //AL_dmac_setTransferType(AL_DMAC_channel_0,AL_dmac_channel_num_1,Dmac_transfer_row1,Dmac_src_dst);
    AL_dmac_setListPointerAddress(AL_DMAC_channel_0,LMS_master1,LLP_BASE_ADDR);
    //AL_dmac_setTransWidth(AL_DMAC_channel_0,AL_dmac_channel_num_1,Dmac_src_dst,WIDTH_32);
    //AL_dmac_setTransWidth(AL_DMAC_channel_0,AL_dmac_channel_num_1,Dmac_src_dst,WIDTH_32);
    //AL_dmac_setBurstTransLength(AL_DMAC_channel_0,AL_dmac_channel_num_1,Dmac_src_dst,AL_DMA_MSIZE_4);
    //AL_dmac_setBlockTransSize(AL_DMAC_channel_0,AL_dmac_channel_num_1,80);
    dw_dmac_setLlpEnable(AL_DMAC_channel_0,Dmac_src_dst);
    dw_dmac_setMemPeriphFlowCtl(AL_DMAC_channel_0,PER2MEM);
    //dw_dmac_setAddressInc(AL_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src,FIX);
    can_handshank_function(TOP);
    dma_blocklist_function(AL_CAN0_BASE,MEM_BASE1_ADDR,LLP_BASE_ADDR,10);
    //can_dma_mode(AL_CAN0,data_length_20);
    can_handshank_function(TOP);
    can_dma_enable(TOP,AL_CAN0);
}

#endif
