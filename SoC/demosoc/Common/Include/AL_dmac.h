/*
 * AL_dmac.h
 *
 *  Created on: 2021年11月5日
 *      Author: wei.pang
 */

/*
 * AL_dma.h
 *
 *  Created on: 2021年11月3日
 *      Author: wei.pang
 */

#ifndef _AL_DMAC_H
#define _AL_DMAC_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "demosoc.h"
#include "nuclei_sdk_soc.h"
#include "AL_can.h"
#include "al9000_spi.h"
#include "al9000_qspi.h"
#include "AL_uart.h"
#include "AL_i2c.h"
#define AL_DMA_MAX_NR_REQUESTS	8
#define LLP_BASE_ADDR      0x61032000
#define LLP_1_ADDR         0x61030200
#define LLP_2_ADDR         0x61030300
#define LLP_3_ADDR         0x61030400
#define LLP_4_ADDR         0x61030500
#define can_ctl_parm_l     	  0x18204825
#define can_ctl_parm_h     	  0x00000014
#define uart_ctl_parm_l_tx    0x18104D01
#define uart_ctl_parm_h_tx    0x00000020
#define uart_ctl_parm_l_rx    0x18204D01
#define uart_ctl_parm_h_rx    0x00000020
#define spi_ctl_parm_l_tx     0x18100125
#define spi_ctl_parm_h_tx     0x00000031
#define spi_ctl_parm_l_rx     0x18200125
#define spi_ctl_parm_h_rx     0x00000031
#define qspi_ctl_parm_l_tx    0x18100125
#define qspi_ctl_parm_h_tx    0x00000014
#define qspi_ctl_parm_l_rx    0x18200125
#define qspi_ctl_parm_h_rx    0x00000014
#define i2c_ctl_parm_l_tx     0x18100125
#define i2c_ctl_parm_h_tx     0x00000001
#define i2c_ctl_parm_l_rx     0x18200125
#define i2c_ctl_parm_h_rx     0x00000001


 struct AL_dmac_channel_config{
	 uint32_t ctl_int_en;
	 uint32_t ctl_dst_tr_width;
	 uint32_t ctl_src_tr_width;
	 uint32_t ctl_dinc;
	 uint32_t ctl_sinc;
	 uint32_t ctl_dst_msize;
	 uint32_t ctl_src_msize;
	 uint32_t ctl_src_gather_en;
	 uint32_t ctl_dst_scatter_en;
	 uint32_t ctl_tt_fc;
	 uint32_t ctl_dms;
	 uint32_t ctl_llp_dst_en;
	 uint32_t ctl_llp_src_en;
	 uint32_t ctl_block_ts;
	 uint32_t ctl_done;
	 uint32_t llp_lms;
	 uint32_t llp_loc;
	 uint32_t ctl_sms;
	 uint32_t cfg_src_per;
	 uint32_t cfg_dst_per;
	 uint32_t cfg_ch_prior;
	 uint32_t sar;
	 uint32_t dar;

 };
/*
 * Redefine this macro to handle differences between 32- and 64-bit
 * addressing, big vs. little endian, etc.
 */

/* Bitfields in DMAC_PARAMS */
#define DMAC_PARAMS_MBLK_EN				11			/* multi block transfer */
#define DMAC_PARAMS_HC_LLP				13			/* set LLP register to zero */
#define DMAC_PARAMS_MSIZE				16			/* max group transaction size */
/* Bitfields in LLP */
#define DMAC_LLP_L_LMS					BITS(0,1)	/* list master select */
#define DMAC_LLP_L_LOC		            	       /* next lli */




/* Bitfields in SGR */
#define DMAC_SGR_SGI(x)		((x) << 0)
#define DMAC_SGR_SGC(x)		((x) << 20)

/* Bitfields in DSR */
#define DMAC_DSR_DSI(x)		((x) << 0)
#define DMAC_DSR_DSC(x)		((x) << 20)





/* Bitfields in FIFO_PARTITION */
#define IDMA32C_FP_PSIZE_CH0(x)		((x) << 0)
#define IDMA32C_FP_PSIZE_CH1(x)		((x) << 13)
#define IDMA32C_FP_UPDATE		(1 << 26)

 /* Bitfields in RawTfer*/
#define DMAC_RawTfer_CH1_RAW
#define DMAC_RawTfer_CH2_RAW
#define DMAC_RawTfer_CH3_RAW
#define DMAC_RawTfer_CH4_RAW
#define DMAC_RawTfer_CH5_RAW
#define DMAC_RawTfer_CH6_RAW
#define DMAC_RawTfer_CH7_RAW
#define DMAC_RawTfer_CH8_RAW
 /* Bitfields in RawBlock*/
#define DMAC_RawBlock_CH1_RAW
#define DMAC_RawBlock_CH2_RAW
#define DMAC_RawBlock_CH3_RAW
#define DMAC_RawBlock_CH4_RAW
#define DMAC_RawBlock_CH5_RAW
#define DMAC_RawBlock_CH6_RAW
#define DMAC_RawBlock_CH7_RAW
#define DMAC_RawBlock_CH8_RAW
 /* Bitfields in RawSrcTran*/
#define DMAC_RawSrcTran_CH1_RAW
#define DMAC_RawSrcTran_CH2_RAW
#define DMAC_RawSrcTran_CH3_RAW
#define DMAC_RawSrcTran_CH4_RAW
#define DMAC_RawSrcTran_CH5_RAW
#define DMAC_RawSrcTran_CH6_RAW
#define DMAC_RawSrcTran_CH7_RAW
#define DMAC_RawSrcTran_CH8_RAW
 /* Bitfields in RawDstTran*/
#define DMAC_RawDstTran_CH1_RAW
#define DMAC_RawDstTran_CH2_RAW
#define DMAC_RawDstTran_CH3_RAW
#define DMAC_RawDstTran_CH4_RAW
#define DMAC_RawDstTran_CH5_RAW
#define DMAC_RawDstTran_CH6_RAW
#define DMAC_RawDstTran_CH7_RAW
#define DMAC_RawDstTran_CH8_RAW
 /* Bitfields in RawErr*/
#define DMAC_RawErr_CH1_RAW
#define DMAC_RawErr_CH2_RAW
#define DMAC_RawErr_CH3_RAW
#define DMAC_RawErr_CH4_RAW
#define DMAC_RawErr_CH5_RAW
#define DMAC_RawErr_CH6_RAW
#define DMAC_RawErr_CH7_RAW
#define DMAC_RawErr_CH8_RAW
 /* Bitfields in StatusTfer*/
#define DMAC_StatusTfer_CH1_Status
#define DMAC_StatusTfer_CH2_Status
#define DMAC_StatusTfer_CH3_Status
#define DMAC_StatusTfer_CH4_Status
#define DMAC_StatusTfer_CH5_Status
#define DMAC_StatusTfer_CH6_Status
#define DMAC_StatusTfer_CH7_Status
#define DMAC_StatusTfer_CH8_Status
 /* Bitfields in StatusBlock*/
#define DMAC_StatusBlock_CH1_Status
#define DMAC_StatusBlock_CH2_Status
#define DMAC_StatusBlock_CH3_Status
#define DMAC_StatusBlock_CH4_Status
#define DMAC_StatusBlock_CH5_Status
#define DMAC_StatusBlock_CH6_Status
#define DMAC_StatusBlock_CH7_Status
#define DMAC_StatusBlock_CH8_Status
 /* Bitfields in StatusSrcTran*/
#define DMAC_StatusSrcTran_CH1_Status
#define DMAC_StatusSrcTran_CH2_Status
#define DMAC_StatusSrcTran_CH3_Status
#define DMAC_StatusSrcTran_CH4_Status
#define DMAC_StatusSrcTran_CH5_Status
#define DMAC_StatusSrcTran_CH6_Status
#define DMAC_StatusSrcTran_CH7_Status
#define DMAC_StatusSrcTran_CH8_Status
 /* Bitfields in StatusDstTran*/
#define DMAC_StatusDstTran_CH1_Status
#define DMAC_StatusDstTran_CH2_Status
#define DMAC_StatusDstTran_CH3_Status
#define DMAC_StatusDstTran_CH4_Status
#define DMAC_StatusDstTran_CH5_Status
#define DMAC_StatusDstTran_CH6_Status
#define DMAC_StatusDstTran_CH7_Status
#define DMAC_StatusDstTran_CH8_Status
 /* Bitfields in StatusErr*/
#define DMAC_StatusErr_CH1_Status
#define DMAC_StatusErr_CH2_Status
#define DMAC_StatusErr_CH3_Status
#define DMAC_StatusErr_CH4_Status
#define DMAC_StatusErr_CH5_Status
#define DMAC_StatusErr_CH6_Status
#define DMAC_StatusErr_CH7_Status
#define DMAC_StatusErr_CH8_Status
 /* Bitfields in MaskTfer*/
#define DMAC_MaskTfer_CH1_INT_MASK							((uint32_t)0x00000101)
#define DMAC_MaskTfer_CH2_INT_MASK								BIT(1)
#define DMAC_MaskTfer_CH3_INT_MASK								BIT(2)
#define DMAC_MaskTfer_CH4_INT_MASK								BIT(3)
#define DMAC_MaskTfer_CH5_INT_MASK								BIT(4)
#define DMAC_MaskTfer_CH6_INT_MASK								BIT(5)
#define DMAC_MaskTfer_CH7_INT_MASK								BIT(6)
#define DMAC_MaskTfer_CH8_INT_MASK								BIT(7)
#define DMAC_MaskTfer_CH1_INT_MASK_WE							BIT(8)
#define DMAC_MaskTfer_CH2_INT_MASK_WE							BIT(9)
#define DMAC_MaskTfer_CH3_INT_MASK_WE							BIT(10)
#define DMAC_MaskTfer_CH4_INT_MASK_WE							BIT(11)
#define DMAC_MaskTfer_CH5_INT_MASK_WE							BIT(12)
#define DMAC_MaskTfer_CH6_INT_MASK_WE							BIT(13)
#define DMAC_MaskTfer_CH7_INT_MASK_WE							BIT(14)
#define DMAC_MaskTfer_CH8_INT_MASK_WE							BIT(15)
 /* Bitfields in MaskBlock*/
#define DMAC_MaskBlock_CH1_INT_MASK							((uint32_t)0x00000101)
#define DMAC_MaskBlock_CH2_INT_MASK							BIT(1)
#define DMAC_MaskBlock_CH3_INT_MASK							BIT(2)
#define DMAC_MaskBlock_CH4_INT_MASK							BIT(3)
#define DMAC_MaskBlock_CH5_INT_MASK							BIT(4)
#define DMAC_MaskBlock_CH6_INT_MASK							BIT(5)
#define DMAC_MaskBlock_CH7_INT_MASK							BIT(6)
#define DMAC_MaskBlock_CH8_INT_MASK							BIT(7)
#define DMAC_MaskBlock_CH1_INT_MASK_WE							BIT(8)
#define DMAC_MaskBlock_CH2_INT_MASK_WE							BIT(9)
#define DMAC_MaskBlock_CH3_INT_MASK_WE							BIT(10)
#define DMAC_MaskBlock_CH4_INT_MASK_WE							BIT(11)
#define DMAC_MaskBlock_CH5_INT_MASK_WE							BIT(12)
#define DMAC_MaskBlock_CH6_INT_MASK_WE							BIT(13)
#define DMAC_MaskBlock_CH7_INT_MASK_WE							BIT(14)
#define DMAC_MaskBlock_CH8_INT_MASK_WE							BIT(15)
 /* Bitfields in MaskSrcTran*/
#define DMAC_MaskSrcTran_CH1_INT_MASK						((uint32_t)0x00000101)
#define DMAC_MaskSrcTran_CH2_INT_MASK 							BIT(1)
#define DMAC_MaskSrcTran_CH3_INT_MASK 							BIT(2)
#define DMAC_MaskSrcTran_CH4_INT_MASK 							BIT(3)
#define DMAC_MaskSrcTran_CH5_INT_MASK 							BIT(4)
#define DMAC_MaskSrcTran_CH6_INT_MASK 							BIT(5)
#define DMAC_MaskSrcTran_CH7_INT_MASK 							BIT(6)
#define DMAC_MaskSrcTran_CH8_INT_MASK  							BIT(7)
#define DMAC_MaskSrcTran_CH1_INT_MASK_WE						BIT(8)
#define DMAC_MaskSrcTran_CH2_INT_MASK_WE						BIT(9)
#define DMAC_MaskSrcTran_CH3_INT_MASK_WE						BIT(10)
#define DMAC_MaskSrcTran_CH4_INT_MASK_WE						BIT(11)
#define DMAC_MaskSrcTran_CH5_INT_MASK_WE						BIT(12)
#define DMAC_MaskSrcTran_CH6_INT_MASK_WE						BIT(13)
#define DMAC_MaskSrcTran_CH7_INT_MASK_WE						BIT(14)
#define DMAC_MaskSrcTran_CH8_INT_MASK_WE						BIT(15)
 /* Bitfields in MaskDstTran*/
#define DMAC_MaskDstTran_CH1_INT_MASK							((uint32_t)0x00000101)
#define DMAC_MaskDstTran_CH2_INT_MASK 							((uint32_t)0x00000202)
#define DMAC_MaskDstTran_CH3_INT_MASK 							((uint32_t)0x00000404)
#define DMAC_MaskDstTran_CH4_INT_MASK 							((uint32_t)0x00000808)
#define DMAC_MaskDstTran_CH5_INT_MASK 							((uint32_t)0x00001010)
#define DMAC_MaskDstTran_CH6_INT_MASK 							((uint32_t)0x00002020)
#define DMAC_MaskDstTran_CH7_INT_MASK 							((uint32_t)0x00004040)
#define DMAC_MaskDstTran_CH8_INT_MASK   						((uint32_t)0x00008080)
#define DMAC_MaskDstTran_CH1_INT_MASK_WE						BIT(8)
#define DMAC_MaskDstTran_CH2_INT_MASK_WE						BIT(9)
#define DMAC_MaskDstTran_CH3_INT_MASK_WE						BIT(10)
#define DMAC_MaskDstTran_CH4_INT_MASK_WE						BIT(11)
#define DMAC_MaskDstTran_CH5_INT_MASK_WE						BIT(12)
#define DMAC_MaskDstTran_CH6_INT_MASK_WE						BIT(13)
#define DMAC_MaskDstTran_CH7_INT_MASK_WE						BIT(14)
#define DMAC_MaskDstTran_CH8_INT_MASK_WE						BIT(15)
 /* Bitfields in MaskErr*/
#define DMAC_MaskErr_CH1_INT_MASK								BIT(0)
#define DMAC_MaskErr_CH2_INT_MASK 								BIT(1)
#define DMAC_MaskErr_CH3_INT_MASK								BIT(2)
#define DMAC_MaskErr_CH4_INT_MASK 								BIT(3)
#define DMAC_MaskErr_CH5_INT_MASK 								BIT(4)
#define DMAC_MaskErr_CH6_INT_MASK 								BIT(5)
#define DMAC_MaskErr_CH7_INT_MASK 								BIT(6)
#define DMAC_MaskErr_CH8_INT_MASK  								BIT(7)
#define DMAC_MaskErr_CH1_INT_MASK_WE							BIT(8)
#define DMAC_MaskErr_CH2_INT_MASK_WE							BIT(9)
#define DMAC_MaskErr_CH3_INT_MASK_WE							BIT(10)
#define DMAC_MaskErr_CH4_INT_MASK_WE							BIT(11)
#define DMAC_MaskErr_CH5_INT_MASK_WE							BIT(12)
#define DMAC_MaskErr_CH6_INT_MASK_WE							BIT(13)
#define DMAC_MaskErr_CH7_INT_MASK_WE							BIT(14)
#define DMAC_MaskErr_CH8_INT_MASK_WE							BIT(15)
 /* Bitfields in ClearTfer*/
#define DMAC_ClearTfer_CH1_CLEAR								BIT(0)
#define DMAC_ClearTfer_CH2_CLEAR								BIT(1)
#define DMAC_ClearTfer_CH3_CLEAR								BIT(2)
#define DMAC_ClearTfer_CH4_CLEAR								BIT(3)
#define DMAC_ClearTfer_CH5_CLEAR								BIT(4)
#define DMAC_ClearTfer_CH6_CLEAR								BIT(5)
#define DMAC_ClearTfer_CH7_CLEAR								BIT(6)
#define DMAC_ClearTfer_CH8_CLEAR								BIT(7)
 /* Bitfields in ClearBlock*/
#define DMAC_ClearBlock_CH1_CLEAR								BIT(0)
#define DMAC_ClearBlock_CH2_CLEAR								BIT(1)
#define DMAC_ClearBlock_CH3_CLEAR								BIT(2)
#define DMAC_ClearBlock_CH4_CLEAR								BIT(3)
#define DMAC_ClearBlock_CH5_CLEAR								BIT(4)
#define DMAC_ClearBlock_CH6_CLEAR								BIT(5)
#define DMAC_ClearBlock_CH7_CLEAR								BIT(6)
#define DMAC_ClearBlock_CH8_CLEAR								BIT(7)
 /* Bitfields in ClearSrcTran*/
#define DMAC_ClearSrcTran_CH1_CLEAR								BIT(0)
#define DMAC_ClearSrcTran_CH2_CLEAR								BIT(1)
#define DMAC_ClearSrcTran_CH3_CLEAR								BIT(2)
#define DMAC_ClearSrcTran_CH4_CLEAR								BIT(3)
#define DMAC_ClearSrcTran_CH5_CLEAR								BIT(4)
#define DMAC_ClearSrcTran_CH6_CLEAR								BIT(5)
#define DMAC_ClearSrcTran_CH7_CLEAR								BIT(6)
#define DMAC_ClearSrcTran_CH8_CLEAR								BIT(7)
 /* Bitfields in ClearDstTran*/
#define DMAC_ClearDstTran_CH1_CLEAR								BIT(0)
#define DMAC_ClearDstTran_CH2_CLEAR								BIT(1)
#define DMAC_ClearDstTran_CH3_CLEAR								BIT(2)
#define DMAC_ClearDstTran_CH4_CLEAR								BIT(3)
#define DMAC_ClearDstTran_CH5_CLEAR								BIT(4)
#define DMAC_ClearDstTran_CH6_CLEAR								BIT(5)
#define DMAC_ClearDstTran_CH7_CLEAR								BIT(6)
#define DMAC_ClearDstTran_CH8_CLEAR								BIT(7)
 /* Bitfields in ClearErr*/
#define DMAC_ClearErr_CH1_CLEAR									BIT(0)
#define DMAC_ClearErr_CH2_CLEAR									BIT(1)
#define DMAC_ClearErr_CH3_CLEAR									BIT(2)
#define DMAC_ClearErr_CH4_CLEAR									BIT(3)
#define DMAC_ClearErr_CH5_CLEAR									BIT(4)
#define DMAC_ClearErr_CH6_CLEAR									BIT(5)
#define DMAC_ClearErr_CH7_CLEAR									BIT(6)
#define DMAC_ClearErr_CH8_CLEAR									BIT(7)
 /* Bitfields in StstusInt*/
#define DMAC_StstusInt_ERR
#define DMAC_StstusInt_SRCT
#define DMAC_StstusInt_BLOCK
#define DMAC_StstusInt_TFR
 /* Bitfields in ReqSrcReg*/
#define DMAC_ReqSrcReg_CH1_SRC_REQ      BIT(0)
#define DMAC_ReqSrcReg_CH1_SRC_REQ_WE
#define DMAC_ReqSrcReg_CH2_SRC_REQ
#define DMAC_ReqSrcReg_CH2_SRC_REQ_WE
#define DMAC_ReqSrcReg_CH3_SRC_REQ
#define DMAC_ReqSrcReg_CH3_SRC_REQ_WE
#define DMAC_ReqSrcReg_CH4_SRC_REQ
#define DMAC_ReqSrcReg_CH4_SRC_REQ_WE
#define DMAC_ReqSrcReg_CH5_SRC_REQ
#define DMAC_ReqSrcReg_CH5_SRC_REQ_WE
#define DMAC_ReqSrcReg_CH6_SRC_REQ
#define DMAC_ReqSrcReg_CH6_SRC_REQ_WE
#define DMAC_ReqSrcReg_CH7_SRC_REQ
#define DMAC_ReqSrcReg_CH7_SRC_REQ_WE
#define DMAC_ReqSrcReg_CH8_SRC_REQ
#define DMAC_ReqSrcReg_CH8_SRC_REQ_WE
 /* Bitfields in ReqDstReg*/
#define DMAC_ReqDstReg_CH1_DST_REQ									BIT(0)
#define DMAC_ReqDstReg_CH1_DST_REQ_WE
#define DMAC_ReqDstReg_CH2_DST_REQ
#define DMAC_ReqDstReg_CH2_DST_REQ_WE
#define DMAC_ReqDstReg_CH3_DST_REQ
#define DMAC_ReqDstReg_CH3_DST_REQ_WE
#define DMAC_ReqDstReg_CH4_DST_REQ
#define DMAC_ReqDstReg_CH4_DST_REQ_WE
#define DMAC_ReqDstReg_CH5_DST_REQ
#define DMAC_ReqDstReg_CH5_DST_REQ_WE
#define DMAC_ReqDstReg_CH6_DST_REQ
#define DMAC_ReqDstReg_CH6_DST_REQ_WE
#define DMAC_ReqDstReg_CH7_DST_REQ
#define DMAC_ReqDstReg_CH7_DST_REQ_WE
#define DMAC_ReqDstReg_CH8_DST_REQ
#define DMAC_ReqDstReg_CH8_DST_REQ_WE
 /* Bitfields in SglRqSrcReg*/
#define DMAC_SglRqSrcReg_CH1_SRC_SGLREQ							BIT(0)
#define DMAC_SglRqSrcReg_CH1_SRC_SGLREQ_WE
#define DMAC_SglRqSrcReg_CH2_SRC_SGLREQ
#define DMAC_SglRqSrcReg_CH2_SRC_SGLREQ_WE
#define DMAC_SglRqSrcReg_CH3_SRC_SGLREQ
#define DMAC_SglRqSrcReg_CH3_SRC_SGLREQ_WE
#define DMAC_SglRqSrcReg_CH4_SRC_SGLREQ
#define DMAC_SglRqSrcReg_CH4_SRC_SGLREQ_WE
#define DMAC_SglRqSrcReg_CH5_SRC_SGLREQ
#define DMAC_SglRqSrcReg_CH5_SRC_SGLREQ_WE
#define DMAC_SglRqSrcReg_CH6_SRC_SGLREQ
#define DMAC_SglRqSrcReg_CH6_SRC_SGLREQ_WE
#define DMAC_SglRqSrcReg_CH7_SRC_SGLREQ
#define DMAC_SglRqSrcReg_CH7_SRC_SGLREQ_WE
#define DMAC_SglRqSrcReg_CH8_SRC_SGLREQ
#define DMAC_SglRqSrcReg_CH8_SRC_SGLREQ_WE
 /* Bitfields in SglRqDstReg*/
#define DMAC_SglRqDstReg_CH1_DST_SGLREQ									BIT(0)
#define DMAC_SglRqDstReg_CH1_DST_SGLREQ_WE
#define DMAC_SglRqDstReg_CH2_DST_SGLREQ
#define DMAC_SglRqDstReg_CH2_DST_SGLREQ_WE
#define DMAC_SglRqDstReg_CH3_DST_SGLREQ
#define DMAC_SglRqDstReg_CH3_DST_SGLREQ_WE
#define DMAC_SglRqDstReg_CH4_DST_SGLREQ
#define DMAC_SglRqDstReg_CH4_DST_SGLREQ_WE
#define DMAC_SglRqDstReg_CH5_DST_SGLREQ
#define DMAC_SglRqDstReg_CH5_DST_SGLREQ_WE
#define DMAC_SglRqDstReg_CH6_DST_SGLREQ
#define DMAC_SglRqDstReg_CH6_DST_SGLREQ_WE
#define DMAC_SglRqDstReg_CH7_DST_SGLREQ
#define DMAC_SglRqDstReg_CH7_DST_SGLREQ_WE
#define DMAC_SglRqDstReg_CH8_DST_SGLREQ
#define DMAC_SglRqDstReg_CH8_DST_SGLREQ_WE
 /* Bitfields in LstSrcReg*/
#define DMAC_LstSrcReg_CH1_LSTSRC						BIT(0)
#define DMAC_LstSrcReg_CH1_LSTSRC_WE
#define DMAC_LstSrcReg_CH2_LSTSRC
#define DMAC_LstSrcReg_CH2_LSTSRC_WE
#define DMAC_LstSrcReg_CH3_LSTSRC
#define DMAC_LstSrcReg_CH3_LSTSRC_WE
#define DMAC_LstSrcReg_CH4_LSTSRC
#define DMAC_LstSrcReg_CH4_LSTSRC_WE
#define DMAC_LstSrcReg_CH5_LSTSRC
#define DMAC_LstSrcReg_CH5_LSTSRC_WE
#define DMAC_LstSrcReg_CH6_LSTSRC
#define DMAC_LstSrcReg_CH6_LSTSRC_WE
#define DMAC_LstSrcReg_CH7_LSTSRC
#define DMAC_LstSrcReg_CH7_LSTSRC_WE
#define DMAC_LstSrcReg_CH8_LSTSRC
#define DMAC_LstSrcReg_CH8_LSTSRC_WE
 /* Bitfields in LstDstReg*/
#define DMAC_LstDstReg_CH1_LSTDST										BIT(0)
#define DMAC_LstDstReg_CH1_LSTDST_WE
#define DMAC_LstDstReg_CH2_LSTDST
#define DMAC_LstDstReg_CH2_LSTDST_WE
#define DMAC_LstDstReg_CH3_LSTDST
#define DMAC_LstDstReg_CH3_LSTDST_WE
#define DMAC_LstDstReg_CH4_LSTDST
#define DMAC_LstDstReg_CH4_LSTDST_WE
#define DMAC_LstDstReg_CH5_LSTDST
#define DMAC_LstDstReg_CH5_LSTDST_WE
#define DMAC_LstDstReg_CH6_LSTDST
#define DMAC_LstDstReg_CH6_LSTDST_WE
#define DMAC_LstDstReg_CH7_LSTDST
#define DMAC_LstDstReg_CH7_LSTDST_WE
#define DMAC_LstDstReg_CH8_LSTDST
#define DMAC_LstDstReg_CH8_LSTDST_WE
 /* Bitfields in DmaCfgReg*/
#define DMAC_DmaCfgReg_DMA_EN								BIT(0)
 /* Bitfields in ChEnReg_L*/
#define DMAC_ChEnReg_L_CH1_EN							((uint32_t)0x00000101)
#define DMAC_ChEnReg_L_CH2_EN							((uint32_t)0x00000202)
#define DMAC_ChEnReg_L_CH3_EN							((uint32_t)0x00000404)
#define DMAC_ChEnReg_L_CH4_EN							((uint32_t)0x00000808)
#define DMAC_ChEnReg_L_CH5_EN							((uint32_t)0x00001010)
#define DMAC_ChEnReg_L_CH6_EN							((uint32_t)0x00002020)
#define DMAC_ChEnReg_L_CH7_EN							((uint32_t)0x00004040)
#define DMAC_ChEnReg_L_CH8_EN							((uint32_t)0x00008080)
 /* Bitfields in DmaIdReg*/
#define DMAC_DmaIdReg_DMA_ID
 /* Bitfields in DmaTest*/
#define DMAC_DmaTestReg_TEST_SLV_IF
#define DMAC_CH_MASK								((uint32_t) 0x00000000)
#define DMAC_MAX_CH_MASK                            ((uint32_t) 0x00000080)
#define ch_fc(x)       (((x) == AL_DMAC_channel_0 ) || ((x) == AL_DMAC_channel_1 ) ||\
						((x) == AL_DMAC_channel_2 ) || ((x) == AL_DMAC_channel_3 ) ||\
						((x) == AL_DMAC_channel_4 ) || ((x) == AL_DMAC_channel_5 ) ||\
						((x) == AL_DMAC_channel_6 ) || ((x) == AL_DMAC_channel_7))
/*
 * TT_FC
 * Mode Define
 */
 /*
  *  /* Bitfields in DmaLpTimeoutReg
  * Bitfields in Dma_COMP_PARAMS_6
  * Bitfields in Dma_COMP_PARAMS_5
  * Bitfields in Dma_COMP_PARAMS_4
  * Bitfields in Dma_COMP_PARAMS_3
  * Bitfields in Dma_COMP_PARAMS_2
  * Bitfields in Dma_COMP_PARAMS_1
  * Bitfields in DmaCompsID
  *
  * above regsiter only can be read
  * #define BIT_GET
  * fetch bit of the register
  *
  */
enum   AL_dmac_transfer_width{
	WIDTH_8,
	WIDTH_16,
	WIDTH_32,
	WIDTH_64,
	WIDTH_128,
	WIDTH_256,
};
/* bursts size */
enum AL_dmac_burst_trans_length {
	AL_DMA_MSIZE_1,
	AL_DMA_MSIZE_4,
	AL_DMA_MSIZE_8,
	AL_DMA_MSIZE_16,
	AL_DMA_MSIZE_32,
	AL_DMA_MSIZE_64,
	AL_DMA_MSIZE_128,
	AL_DMA_MSIZE_256,
};
enum   AL_dmac_SRCmaster_number{
	SMS_master1,
	SMS_master2,
};
enum   AL_dmac_DSTmaster_number{
	DMS_master1,
	DMS_master2,
	DMS_master3,
	DMS_master4,
};
enum   AL_dmac_LMSmaster_number{
	LMS_master1,
	LMS_master2,
	LMS_master3,
	LMS_master4,
};
enum AL_dmac_flags {
	AL_DMA_IS_CYCLIC = 0,
	AL_DMA_IS_SOFT_LLP = 1,
	AL_DMA_IS_PAUSED = 2,
	AL_DMA_IS_INITIALIZED = 3,
};
/* transfer type and flow controller */
enum AL_dmac_transfer_flow {
	MEM2MEM,
	MEM2PER,
	PER2MEM,
	PER2PER,
};
enum AL_dmac_transfer_type{
	Dmac_transfer_row1,
	Dmac_transfer_row2,
	Dmac_transfer_row3,
	Dmac_transfer_row4,
	Dmac_transfer_row5,
	Dmac_transfer_row6,
	Dmac_transfer_row7,
	Dmac_transfer_row8,
	Dmac_transfer_row9,
	Dmac_transfer_row10,
};
enum  AL_dmac_channel_number{
	AL_dmac_channel_num_0,
	AL_dmac_channel_num_1,
	AL_dmac_channel_num_2,
	AL_dmac_channel_num_3,
	AL_dmac_channel_num_4,
	AL_dmac_channel_num_5,
	AL_dmac_channel_num_6,
	AL_dmac_channel_num_7,
	AL_dmac_channel_num_8,
};

enum AL_dmac_Channel_Priority{
	Channel_priority_0,
	Channel_priority_1,
	Channel_priority_2,
	Channel_priority_3,
	Channel_priority_4,
	Channel_priority_5,
	Channel_priority_6,
	Channel_priority_7,
};
enum AL_dmac_Handshaking{
	 Hardware_handshaking,
	 Software_handshaking,
};
enum   AL_dmac_irq{
	tfr_l,
	block_1,
	srcTran,
	DstTran,
	Err,
};

enum   AL_dmac_src_dst_select{
	Dmac_src,
	Dmac_dst,
	Dmac_src_dst,
};

enum   AL_dmac_software_req{
	Dmac_request,
	Dmac_single_request,
	Dmac_last_request,

};
enum   AL_state{
	AL_set,
	Dmac_idle,
	AL_clear,
};
enum   AL_dmac_address_increment{
	Increments,
	Decrements,
	FIX,
};
enum   AL_dmac_hs_interface{
	hs_inter,
};
enum   AL_dmac_prot_level{
	prot_lvl,
};
enum   AL_dmac_fifo_mode{
	FIFO_MODE_0,
	FIFO_MODE_1,
};
enum   AL_dmac_flow_ctl_mode{
	fc_mode_0,
	fc_mode_1,
};
enum   AL_dmac_polarity_level{
	ACTIVE_HIGH,
	ACTIVE_LOW,
};
enum   AL_dmac_sw_hw_hs_select {hardhandshack,softhandshack};
enum HS_interfance{
	i2c0_tx_hs_interfance,
	i2c0_rx_hs_interfance,
	i2c1_tx_hs_interfance,
	i2c1_rx_hs_interfance,
	spi0_tx_hs_interfance,
	spi0_rx_hs_interfance,
	spi1_tx_hs_interfance,
	spi1_rx_hs_interfance,
	uart0_tx_hs_interfance,
	uart0_rx_hs_interfance,
	uart1_tx_hs_interfance,
	uart1_rx_hs_interfance,
	qspi_tx_hs_interfance,
	qspi_rx_hs_interfance,
	can0_rx_hs_interfance,
	can1_rx_hs_interfance
};

enum PER_type{
	i2c0,
	i2c1,
	spi0,
	spi1,
	uart0,
	uart1,
	qspi,
	can0,
	can1
};
enum PER_TransmissionType{
	ALI2C,
	ALSPI,
	ALQSPI,
	ALUART,
	ALCAN
};
enum dma_type{
	tx,
	rx,
};


int 	AlDma_Init(AL_DMAC_TypeDef *DMAC);
void 	AlDma_Enable(AL_DMAC_TypeDef *DMAC);
int 	AlDma_Disable(AL_DMAC_TypeDef *DMAC);
int 	AlDma_EnableChannel(AL_DMAC_TypeDef *DMAC,enum  AL_dmac_channel_number ch_num);
int 	AlDma_DisableChannel(AL_DMAC_TypeDef *DMAC,enum AL_dmac_channel_number ch_num);
int 	AlDma_EnableChannelIrq(DMA_Channel_TypeDef *Channelx);
int 	AlDma_DisableChannelIrq(DMA_Channel_TypeDef *Channelx);
int 	AlDma_ClearIrq(AL_DMAC_TypeDef *DMAC);
int 	AlDma_UnmaskIrq(AL_DMAC_TypeDef *DMAC,enum   AL_dmac_channel_number ch_num,enum   AL_dmac_irq ch_irq);
int 	AlDma_MaskIrq(AL_DMAC_TypeDef *DMAC,enum   AL_dmac_channel_number ch_num,enum   AL_dmac_irq ch_irq);
int 	AlDma_SetAddress(DMA_Channel_TypeDef *Channelx,enum    AL_dmac_src_dst_select sd_sel,uint32_t address);
int 	AlDma_SetChannelConfig(DMA_Channel_TypeDef *Channelx);
int 	AlDma_SetTransferType(DMA_Channel_TypeDef *Channelx,enum AL_dmac_transfer_type  transfer);
int 	AlDma_SetMemPeriphFlowCtl(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_transfer_flow  tt_fc);
int 	AlDma_SetTransWidth(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_src_dst_select sd_sel,enum   AL_dmac_transfer_width xf_width);
int 	AlDma_SetBlockTransSize(DMA_Channel_TypeDef *Channelx,uint16_t block_size);
int 	AlDma_SetBurstTransLength(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_src_dst_select sd_sel,enum   AL_dmac_burst_trans_length xf_length);
int 	AlDma_SetMstSelect(DMA_Channel_TypeDef *Channelx,
        	enum   AL_dmac_src_dst_select sd_sel,
		enum   AL_dmac_SRCmaster_number  SRCmst_num,
		enum   AL_dmac_DSTmaster_number  DSTmst_num);
int 	AlDma_SetAddressInc(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_src_dst_select sd_sel,enum   AL_dmac_address_increment addr_inc);
int 	AlDma_SetHsPolarity(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_src_dst_select sd_sel,enum   AL_dmac_polarity_level pol_level);
int 	AlDma_SetFifoMode(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_fifo_mode fifo_mode);
int 	AlDma_SetFlowCtlMode(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_flow_ctl_mode  fc_mode);
int 	AlDma_SetHandshakingMode(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_src_dst_select  sd_sel,enum   AL_dmac_sw_hw_hs_select hs_hwsw_sel);
int 	AlDma_SetReload(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_src_dst_select sd_sel);
int 	AlDma_SetSingle(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_src_dst_select sd_sel);
int 	AlDma_SetLlpEnable(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_src_dst_select sd_sel);
int 	AlDma_SetContigous(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_src_dst_select sd_sel);
int 	AlDma_SetListPointerAddress(DMA_Channel_TypeDef *Channelx,enum   AL_dmac_LMSmaster_number LMS_master,uint32_t address);
int		AlDma_CheckChannelBusy(AL_DMAC_TypeDef *DMAC);
void 	AlDma_SetChannelPriorityOrder(AL_DMAC_TypeDef *DMAC);
uint8_t AlDma_GetChannelIndex(uint8_t channelx);
void 	write_To_OCM(uint32_t* pSnAddr, uint32_t Count, uint32_t* pDnAddr);
void 	write_iic_data(enum i2c_cmd_type type,uint32_t transimte_data_length);
//void 	uart_dma_Init();
//int 	can_dma_mode(AL_CAN_TypeDef* CANX,enum data_length len);
void 	can_dma_enable(TOPCFG_TypeDef* TOP,AL_CAN_TypeDef* CANX);
int 	dma_blocklist_function(uint32_t srcaddress,uint32_t dstaddress,uint32_t llp_address,uint32_t block_number) ;
uint8_t cpu_dma_read_block_num(uint32_t llp_address,uint32_t block_number);
void 	updata_blocklist(uint32_t srcaddress,uint32_t dstaddress,uint32_t llp_address,uint32_t block_number,uint32_t count);
void 	dma_interrupt_readfunction(uint32_t llp_address);
uint8_t dam_handshake_interfance(DMA_Channel_TypeDef *Channelx,enum HS_interfance HS_TYPE);
void 	spi_dma_data_level(SPI_TypeDef *spi,uint8_t tx_data_level,uint8_t rx_data_level);
void 	spi_dma_tx_enable(SPI_TypeDef *spi);
void 	spi_dma_rx_enable(SPI_TypeDef *spi);
void 	i2c_dma_data_level(I2C_TypeDef *i2c,uint8_t tx_data_level,uint8_t rx_data_level);
void 	i2c_dma_tx_enable(I2C_TypeDef *i2c);
void 	i2c_dma_rx_enable(I2C_TypeDef *i2c);
void 	test_blocklist(uint32_t srcaddress,uint32_t dstaddress,enum dma_type tran_type,uint32_t block_number,uint32_t llp_address);
void 	AlDma_blocklist(uint32_t srcaddress,uint32_t dstaddress,enum dma_type tran_type,uint32_t block_number,uint32_t llp_address,enum PER_TransmissionType per_type);
void 	uart_blocklist(uint32_t srcaddress,uint32_t dstaddress,enum dma_type tran_type,uint32_t block_number,uint32_t llp_address);
void 	AlSpi_DmaInit(DMA_Channel_TypeDef *Channelx,enum PER_type PER,enum dma_type type,uint8_t tx_data_level,uint8_t rx_data_level,enum AL_dmac_transfer_type  transfer,uint16_t block_size);
void 	AlUart_DmaInit(DMA_Channel_TypeDef *Channelx,enum  PER_type PER,enum dma_type type,enum AL_dmac_transfer_type  transfer,uint16_t block_size);
void 	AlQspi_DmaInit(DMA_Channel_TypeDef *Channelx,enum dma_type type,uint8_t tx_data_level,uint8_t rx_data_level,enum AL_dmac_transfer_type  transfer,uint16_t block_size);
void 	AlCan_DmaInit(DMA_Channel_TypeDef *Channelx,enum PER_type PER,enum AL_dmac_transfer_type  transfer);
void 	AlI2c_DmaInit(DMA_Channel_TypeDef *Channelx,enum PER_type PER,enum dma_type type,uint8_t tx_data_level,uint8_t rx_data_level,enum AL_dmac_transfer_type  transfer,uint16_t block_size);
void 	can_handshank_function(TOPCFG_TypeDef* TOP);
#ifdef __cplusplus
}
#endif



#endif /* SOC_DEMOSOC_COMMON_INCLUDE_AL_DMA_H_ */

