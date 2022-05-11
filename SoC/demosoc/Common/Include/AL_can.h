/*
 * AL_can.h
 *
 *  Created on: 2021年12月9日
 *      Author: wei.pang
 */

/*
bian
*/
#ifndef AL_CAN_H
#define AL_CAN_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "demosoc.h"

enum can_tx_mode{
	 XMIT_FULL,
	 XMIT_SEP_FIFO,
	 XMIT_SEP_PRIO,
	 XMIT_PTB_MODE
 };

#define	CAN_FD_SET_RST_MASK 			0x80  	/*Set Reset Bit*/	
#define	CAN_FD_OFF_RST_MASK			((uint32_t)7f)	/*Reset Off Bit*/
	
#define	CAN_FD_SET_FULLCAN_MASK			(uint32_t)0x00100000	/*set TTTBM as 1->full TTCAN mode*/
#define	CAN_FD_OFF_FULLCAN_MASK			0xef	/*set TTTBM as 0->separate PTB and STB mode*/

#define	CAN_FD_SET_PRIORITY_MASK			BIT(21)	/*set TSMODE as 1->FIFO mode*/
#define	CAN_FD_OFF_FIFO_MASK			0xdf	/*set TSMODE as 0->Priority mode*/
#define CAN_FD_SET_FIFO_MASK			(uint32_t)0x00200000
#define	CAN_FD_SET_TSONE_MASK			0x04	
#define	CAN_FD_OFF_TSONE_MASK			0xfb
	
#define	CAN_FD_SET_TSALL_MASK			0x02
#define	CAN_FD_OFF_TSALL_MASK			0xfd
	
#define	CAN_FD_LBMEMOD_MASK				0x40	/*set loop back mode external*/
#define	CAN_FD_LBMIMOD_MASK				0x20
	
#define	CAN_FD_SET_BUSOFF_MASK			0x01
#define	CAN_FD_OFF_BUSOFF_MASK			0xfe
	
#define	CAN_FD_SET_TTSEN_MASK			0x80	/*set ttsen tts update enable*/
#define	CAN_FD_SET_BRS_MASK				0x10	/*can fd Bit Rate Switch mask*/
#define	CAN_FD_OFF_BRS_MASK				0xef	
#define	CAN_FD_SET_EDL_MASK				0x20	/*Extended Data Length*/
#define	CAN_FD_OFF_EDL_MASK				0xdf

#define	CAN_FD_SET_DLC_MASK				0x0f	
#define	CAN_FD_SET_TENEXT_MASK			(uint32_t)0x00400000
#define	CAN_FD_SET_IDE_MASK				0x80
#define	CAN_FD_OFF_IDE_MASK				0x7f
#define	CAN_FD_SET_RTR_MASK				0x40
#define	CAN_FD_OFF_RTR_MASK				0xbf
	
#define	CAN_FD_INTR_ALL_MASK			0xff 	/*all interrupts enable mask*/
#define	CAN_FD_SET_RIE_MASK				0x80
#define	CAN_FD_OFF_RIE_MASK				0x7f
#define	CAN_FD_SET_ROE_MASK				0x40
#define	CAN_FD_SET_RFIE_MASK			0x20
#define	CAN_FD_OFF_RFIE_MASK			0xdf
#define	CAN_FD_SET_RAFIE_MASK			0x10
#define	CAN_FD_OFF_RAFIE_MASK			0xef
#define	CAN_FD_SET_EIE_MASK				0x02
#define	CAN_FD_OFF_EIE_MASK				0xfd	
	
#define	CAN_FD_TASCTIVE_MASK			0x02
#define	CAN_FD_RASCTIVE_MASK			0x04
#define CAN_FD_SET_TDCEN_MASK           (uint32_t)(0x00008000)

#define	CAN_FD_OFF_TBSEL_MASK			0x7f	/*message writen in PTB*/	
	
#define	CAN_FD_SET_STBY_MASK			0x20
	
#define	CAN_FD_SET_TPE_MASK		        ((uint32_t)0x00001000)	/*Transmit primary enable*/
	
#define	CAN_FD_SET_TPA_MASK			((uint32_t)0x00000800)
	
#define	CAN_FD_SET_SACK_MASK			0x80
#define	CAN_FD_SET_RREL_MASK			BIT(28)

#define	CAN_FD_RSTAT_NOT_EMPTY_MASK		0x03
#define CAN_FD_ACFCTRL_AELMASK			((uint32_t)0x00000020)
#define	CAN_FD_SET_RIF_MASK				0x80
#define	CAN_FD_OFF_RIF_MASK				0x7f
#define	CAN_FD_SET_RAFIF_MASK			((uint32_t)0x00001000)
#define	CAN_FD_SET_RFIF_MASK			0x20

#define	CAN_FD_SET_TPIF_MASK			BIT(11)	/*Transmission Primary Interrupt Flag*/
#define	CAN_FD_SET_TSIF_MASK			0x04
#define	CAN_FD_SET_EIF_MASK				0x02
#define	CAN_FD_SET_AIF_MASK				0x01

	
#define	CAN_FD_SET_EWARN_MASK			0x80
#define	CAN_FD_SET_EPASS_MASK			0x40
#define	CAN_FD_SET_EPIE_MASK			0x20
#define	CAN_FD_SET_EPIF_MASK			0x10
#define	CAN_FD_SET_ALIE_MASK			0x08
#define	CAN_FD_SET_ALIF_MASK			0x04
#define	CAN_FD_SET_BEIE_MASK			0x02
#define	CAN_FD_SET_BEIF_MASK			0x01
#define	CAN_FD_OFF_EPIE_MASK			0xdf
#define	CAN_FD_OFF_BEIE_MASK			0xfd
	
#define	CAN_FD_SET_AFWL_MASK			0x40
#define	CAN_FD_SET_EWL_MASK				0x0b
	
#define	CAN_FD_SET_KOER_MASK			0xe0
#define	CAN_FD_SET_BIT_ERROR_MASK		0x20
#define	CAN_FD_SET_FORM_ERROR_MASK		0x40
#define	CAN_FD_SET_STUFF_ERROR_MASK		0x60
#define	CAN_FD_SET_ACK_ERROR_MASK		0x80
#define	CAN_FD_SET_CRC_ERROR_MASK		0xa0
#define	CAN_FD_SET_OTH_ERROR_MASK		0xc0

#define BITS_OF_BITTIMING_REG 		8		/*seg1seg2,sjw,prescaler all have 8 bits*/
#define FBITS_IN_BITTIMING_STR		32		/*in can_bittiming strucure every field has 32 bits---->u32*/
#define SEG_1_SHIFT	 				0
#define SEG_2_SHIFT	 				8
#define SJW_SHIFT	 				16
#define PRESC_SHIFT	 				24
 
#define TTSEN_8_32_SHIFT			24		/*TTSEN bit used for 32 bit register read or write*/
#define RTR_32_8_SHIFT				24 
 
 
 
#define CAN_Mode_SingleShotTransmission             ((uint8_t)0x00)  /*!< SingleShotTransmission */
#define CAN_Mode_ListenOnlyMode          			((uint8_t)0x01)  /*!< ListenOnlyMode */
#define CAN_Mode_LoopBackMode_LBMI             		((uint8_t)0x02)  /*!< LoopBackMode_LBMI  */
#define CAN_Mode_LoopBackMode_LBME             		((uint8_t)0x03)  /*!< LoopBackMode_LBME  mode */
#define CAN_Mode_TransceiverStandbyMode    			((uint8_t)0x04)  /*!< TransceiverStandbyMode mode */
 
/*
define CAN mode Command Register RBUF CTL (0xa1) BITS   
 */

/*
define CAN mode Command Register TBUF CTL (0xa1) BITS   
 */
 
/*
define CAN mode Command Register TCMD (0xa1) BITS 
*/
#define CAN_CFG_STAT_BUSOFF						BIT(0)
#define CAN_CFG_STAT_TACTIVE					BIT(1)
#define CAN_CFG_STAT_RACTIVE					BIT(2)
#define CAN_CFG_STAT_TSSS						BIT(3)
#define CAN_CFG_STAT_TPSS						BIT(4)
#define CAN_CFG_STAT_LBMI						BIT(6)
#define CAN_CFG_STAT_LBME						BIT(6)
#define CAN_CFG_STAT_RESET						BIT(7)
/*
define CAN mode Command Register TCMD (0xa1) BITS 
*/
#define CAN_TCMD_TBSEL 							BIT(7) 	///* Transmit Buffer Select */
#define CAN_FD_SET_TBSEL_MASK					BIT(15)		/*message writen in STB*/
#define CAN_FD_OFF_STBY_MASK					(uint32_t)0xffffdfff
#define CAN_TCMD_LOM 							BIT(6)	///*Listen Only Mode*/
#define CAN_TCMD_STBY 							BIT(5)	///*Transceiver Standby Mode*/
#define CAN_TCMD_TPE 							BIT(4)	///*Transmit Primary Enable*/
#define CAN_TCMD_TPA 							BIT(3)	///*Transmit Primary Abort*/
#define CAN_TCMD_TSONE 							BIT(2)	///*Transmit Secondary ONE frame*/
#define CAN_TCMD_TSALL 							BIT(1)	///*Transmit Secondary ALL frames*/
#define CAN_TCMD_TSA							BIT(0)	///*Transmit Secondary Abort*/
/*
define CAN mode Command Register TCTRL (0xa2) BITS 
*/
#define CAN_TCTRL_FD_ISO						BIT(7)
#define CAN_TCTRL_TSNEXT						BIT(6)
#define CAN_TCTRL_TSMODE						BIT(5)
#define CAN_TCTRL_TTTBM							BIT(4)
/*
define CAN mode Command Register RCTRL (0xa3) BITS 
*/
#define CAN_RCTRL_SACK							BIT(7)
#define CAN_RCTRL_ROM							BIT(6)
#define CAN_RCTRL_RREL							BIT(4)
#define CAN_RCTRL_RBALL							BIT(3)
#define CAN_RCTAL_RSTAT							BITS(0,1)
/*
define CAN mode Command Register INTE (0xa4) BITS 
*/
#define CAN_INTE_SWIE							BIT(18)
#define CAN_INTE_MDWIE							BIT(14)
#define CAN_INTE_WTIE							BIT(13)
#define CAN_INTE_TTIE							BIT(11)
#define CAN_INTE_EPIE							BIT(10)
#define CAN_INTE_ALIE							BIT(9)
#define CAN_INTE_BEIE							BIT(8)
#define CAN_FD_OFF_RIE_MASK			           		0x7f
#define CAN_FD_INTR_ALL_MASK						0xff
#define CAN_INTE_ROIE							BIT(6)
#define CAN_INTE_RFIE							BIT(5)
#define CAN_INTE_RAFIE							BIT(4)
#define CAN_INTE_TPIE							BIT(3)
#define CAN_INTE_TSIE							BIT(2)
#define CAN_INTE_EIE							BIT(1)
/*
define CAN mode Command Register RTIF (0xa5) BITS 
*/
#define CAN_INTF_SWIF 							BIT(18) 	///* Transmit Buffer Select */
#define CAN_INTF_SEIF 							BIT(17) 	///* Transmit Buffer Select */
#define CAN_INTF_MAEIF 							BIT(16) 	///* Transmit Buffer Select */
#define CAN_INTF_MDEIF 							BIT(15) 	///* Transmit Buffer Select */
#define CAN_INTF_MDWIF 							BIT(14) 	///* Transmit Buffer Select */
#define CAN_INTF_WTIF 							BIT(13) 	///* Transmit Buffer Select */
#define CAN_INTF_TEIF 							BIT(12) 	///* Transmit Buffer Select */
#define CAN_INTF_TTIF 							BIT(11) 	///* Transmit Buffer Select */
#define CAN_INTF_EPIF 							BIT(10) 	///* Transmit Buffer Select */
#define CAN_INTF_ALIF 							BIT(9) 	///* Transmit Buffer Select */
#define CAN_INTF_BEIF 							BIT(8) 	///* Transmit Buffer Select */ 
#define CAN_INTF_RIF 							BIT(15) 	///* Transmit Buffer Select */
#define CAN_INTF_ROIF 							BIT(6)	///*Listen Only Mode*/
#define CAN_INTF_RFIF 							BIT(5)	///*Transceiver Standby Mode*/
#define CAN_INTF_RAFIF 							BIT(4)	///*Transmit Primary Enable*/
#define CAN_INTF_TPIF 							BIT(11)	///*Transmit Primary Abort*/
#define CAN_INTF_TSIF 							BIT(2)	///*Transmit Secondary ONE frame*/
#define CAN_INTF_EIF 							BIT(1)	///*Transmit Secondary ALL frames*/
#define CAN_INTF_AIF							BIT(0)	///*Transmit Secondary Abort*/
/*
define CAN mode Command Register ERRINT (0xa6) BITS 
*/
#define CAN_RBUF_FORMAT_RMF						BIT(20)
/*
CAN_remote_transmission_request 
*/
#define CAN_RTR_Data                ((uint32_t)0x00000000)  /*!< Data frame */
#define CAN_RTR_Remote              ((uint32_t)0x00000002)  /*!< Remote frame */
#define IS_CAN_RTR(RTR) (((RTR) == CAN_RTR_Data) || ((RTR) == CAN_RTR_Remote))





/*
Error Counter Registers RECNT (0xb2) and TECNT (0xb3)
*/
#define CAN_RECNT_RECNT 0xb2
#define CAN_RECNT_TECNT 0xb3


#define CAN_STATE_ERROR_ACTIVE		BIT(31)
enum data_length{
	data_length_0 = 0,
	data_length_1 = 1,
	data_length_2 = 2,
	data_length_3 = 3,
	data_length_4 = 4,
	data_length_5 = 5,
	data_length_6 = 6,
	data_length_7 = 7,
	data_length_8 = 8,
	data_length_12 = 12,
	data_length_16 = 16,
	data_length_20 = 20,
	data_length_24 = 24,
	data_length_32 = 32,
	data_length_48 = 48,
	data_length_64 = 64
};
enum ACFADR{
	AE0 = 1,
	AE1 = 2,
	AE2 = 4,
	AE3 = 8,
	AE4 = 16,
	AE5 = 32,
	AE6 = 64,
	AE7 = 128,
	AE8 = 256,
	AE9 = 512,
	AE10 = 1042,
	AE11 = 2048,
	AE12 = 4096,
};
enum can_type{
	can,
	can_fd
};
enum ID_type{
	CAN_Id_Standard,
	CAN_Id_Extended
};
enum Intr_type{
	RIE,
	ROIE,
	RFIE,
	RAFIE,
	TPIE,
	TSIE,
	EIE,
	EPIE,
	ALIE,
	BEIE
};
enum baud_rate{
	Sarbrate_0_25Mbit,
	Sarbrate_0_5Mbit,
	rate_0_5Mbit,
	rate_0_833Mbit,
	rate_1Mbit,
	rate_1_538Mbit,
	rate_2Mbit,
	rate_3_077Mbit,
	rate_4Mbit,
	rate_5Mbit,
	rate_6_667Mbit,
	rate_8Mbit,
	rate_10Mbit,
	can_rate_1Mbit
};
enum arb_baud_rate {
	arbrate_0_25Mbit,
	arbrate_0_5Mbit
};
enum tx_mode_type{
	IN_LOOPBACK,
	EX_LOOPBACK,
	NORMAL
};
enum active_mode{
	CAN_TX,
	CAN_RX
	};
typedef struct
{
  uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that 
                        will be transmitted. This parameter can be a value 
                        of @ref CAN_identifier_type */
  uint32_t TTSEN;
  uint8_t RTR;     /*!< Specifies the type of frame for the message that will 
                        be transmitted. This parameter can be a value of 
                        @ref CAN_remote_transmission_request */
  uint8_t FDF;	   
  
  uint8_t BRS;

  uint8_t DLC;     /*!< Specifies the length of the frame that will be 
                        transmitted. This parameter can be a value between 
                        0 to 8 */

  uint8_t Data[64]; /*!< Contains the data to be transmitted. It ranges from 0 
                        to 0xFF. */
} CanTxMsg;

/** 
  * @brief  CAN Rx message structure definition  
  */

typedef struct
{
  uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that 
                        will be received. This parameter can be a value of 
                        @ref CAN_identifier_type */

  uint8_t RTR;     /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  uint8_t DLC;     /*!< Specifies the length of the frame that will be received.
                       This parameter can be a value between 0 to 8 */
  uint8_t Status;
  uint8_t Data[64]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */
  uint8_t FDF;	
  uint8_t BRS;     /*!< Specifies the index of the filter the message stored in 
                        the mailbox passes through. This parameter can be a 
                        value between 0 to 0xFF */
} CanRxMsg;
uint8_t 	AlCan_ModeSelect(TOPCFG_TypeDef *TOP,enum can_type type);
uint8_t 	AlCan_TxMode(AL_CAN_TypeDef *CANX,enum tx_mode_type mode);
int 		AlCan_DeviceDriverBittimeConfiguration(AL_CAN_TypeDef *CANX,enum baud_rate rate,TOPCFG_TypeDef *TOP,enum can_type type);
uint32_t 	AlCan_Rx(AL_CAN_TypeDef *CANX,CanRxMsg* RxMessage,enum data_length len);
int 		AlCan_TmitMode(AL_CAN_TypeDef *CANX,enum can_tx_mode tx_mode);
int 		AlCan_DriverStartTmit(AL_CAN_TypeDef *CANX,CanTxMsg* TxMessage,enum can_tx_mode tx_mode,enum data_length len);
uint8_t 	AlCan_SetRxFillter(AL_CAN_TypeDef *CANX,enum ACFADR ACFADDR,uint32_t ID);
int 		AlCan_SetResetMode(AL_CAN_TypeDef *CANX);
uint32_t 	AlCan_InterruptEnable(AL_CAN_TypeDef *CANX,enum Intr_type type);
uint8_t 	AlCan_GetFlag(AL_CAN_TypeDef *CANX,enum Intr_type type);
uint8_t 	AlCan_ErrorKinds(AL_CAN_TypeDef *CANX);
uint8_t 	AlCan_MessagePending(AL_CAN_TypeDef *CANX);
uint8_t 	AlCan_TxLength(enum data_length len);
uint8_t 	AlCan_SendMsg(AL_CAN_TypeDef *CANX,uint8_t* msg,enum can_tx_mode tx_mode,enum data_length len,uint32_t ID);
uint8_t 	AlCan_ReceiveMsg(AL_CAN_TypeDef *CANX,uint8_t *buf,enum data_length len);
void  		AlCan_TxComplete(AL_CAN_TypeDef *CANX);
void 		AlCan_TestError(AL_CAN_TypeDef *CANX);
#ifdef __cplusplus
}
#endif



#endif /* SOC_DEMOSOC_COMMON_INCLUDE_AlCan_H_ */
