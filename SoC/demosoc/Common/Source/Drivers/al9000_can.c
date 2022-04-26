/*
 * al9000_can.c
 *
 *  Created on: 2021年12月9日
 *      Author: wei.pang
 */

#include "al9000_can.h"
#include "demosoc.h"
#include "al9000_uart.h"
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#define CANFD_MASK	0x01
#define CAN_MASK	0x00
#define TX_MAX		1
#define EINVAL		1
#define CANFD_BRS	0x01
#define CAN_RTR_FLAG 0x40
#define CAN_EFF_FLAG 0x20
#define BITS_OF_BITTIMING_REG 		8		/*seg1,seg2,sjw,prescaler all have 8 bits*/
#define FBITS_IN_BITTIMING_STR		32		/*in can_bittiming strucure every field has 32 bits---->uint32_t*/
#define SEG_1_SHIFT	 				0
#define SEG_2_SHIFT	 				8
#define SJW_SHIFT	 				16
#define PRESC_SHIFT	 				24
#define TTSEN_8_32_SHIFT			24		/*TTSEN bit used for 32 bit register read or write*/
#define RTR_32_8_SHIFT				24
/*transmit mode*/
#define XMIT_FULL					0
#define XMIT_SEP_FIFO				1
#define XMIT_SEP_PRIO				2
#define XMIT_PTB_MODE				3

typedef enum{
	PASSIVE =0,
	ACTIVE  =!PASSIVE
}STATE;
struct ipms_platform_data{
	uint8_t can_or_canfd;
};

/* CAN Bittiming constants */
/*Bittiming setting from IPMS DS. */
/*CANFD*/
#define	tseg1_min  	2
#define	tseg1_max  	17
#define	tseg2_min  	1
#define	tseg2_max  	8
#define	sjw_max 	8
#define	brp_min  	1
#define	brp_max  	512
#define	brp_inc  	1


/* CAN Data Bittiming constants */
/*CAN2.0*/
//TODO: t_seg2 for CAN FD nominal is differently defined

#define Ftseg1_min 	 	2
#define Ftseg1_max 		65
#define Ftseg2_min  		1
#define Fsjw_max  		16
#define Fbrp_min  		1
#define Fbrp_max  		512
#define Fbrp_inc  		1

#define CAN_TxStatus_Failed         ((uint8_t)0x00)/*!< CAN transmission failed */
#define CAN_TxStatus_Ok             ((uint8_t)0x01) /*!< CAN transmission succeeded */
#define CAN_TxStatus_Pending        ((uint8_t)0x02) /*!< CAN transmission pending */

#define CAN_RxStatus_Failed         ((uint8_t)0x00)/*!< CAN transmission failed */
#define CAN_RxStatus_Ok             ((uint8_t)0x01) /*!< CAN transmission succeeded */


uint8_t can_mode_select(TOPCFG_TypeDef *TOP,enum can_type type)
{
	uint8_t mode = 0;
	if (0 == type){
		TOP -> can_fd_ctrl = 0x00000000;
		mode = 0 ;
	}
	else{
		TOP -> can_fd_ctrl = 0x00000101;
		mode =  1;
	}
	return mode;
}
uint8_t can_tx_mode(CAN_AL9000_TypeDef *CANX,enum tx_mode_type mode){
	volatile uint8_t val = 2;
	switch(mode)
	{
	case 0:
		val = 0;
		CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |= CAN_FD_LBMIMOD_MASK;

	break;
	case 1:
		CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |= CAN_FD_LBMEMOD_MASK;
		val = 1;

	break;
	defalut:
		CANX ->CFG_STAT_TCMD_TCTRL_RCTRL &= ~CAN_FD_LBMIMOD_MASK;
		CANX ->CFG_STAT_TCMD_TCTRL_RCTRL &= ~CAN_FD_LBMEMOD_MASK;
		val = 2;

	break;
	}
	return val;
}
int canfd_device_driver_bittime_configuration(CAN_AL9000_TypeDef *CANX,enum baud_rate rate,TOPCFG_TypeDef *TOP,enum can_type type){

	volatile uint32_t delaytime=0;
	volatile uint32_t S_presc,F_presc,s_seg1,s_seg2,f_seg1,f_seg2,s_sjw,f_sjw;
	volatile uint8_t reset_test,tseg1,tseg2;
	volatile uint8_t val;
	//printf("can_mode \r\n",val);
	val =  can_mode_select(TOP,type);
	printf("can_mode %d\r\n",val);
	switch(rate){
	case Sarbrate_0_25Mbit:
		s_seg1 = 62;s_seg2 = 15;s_sjw = 15;S_presc = 3;
	break;
	case Sarbrate_0_5Mbit:
		s_seg1 = 62;s_seg2 =15;s_sjw = 15;S_presc = 1;
	break;
	case rate_0_5Mbit:
		s_seg1 = 30;s_seg2 = 7;s_sjw = 7;S_presc = 3;
	break;
	case rate_0_833Mbit:
		s_seg1 = 17;s_seg2 = 4;s_sjw = 4;S_presc = 3;
	break;
	case rate_1Mbit:
		s_seg1 = 62;s_seg2 =15;s_sjw = 15;S_presc = 1;
		f_seg1 = 30;f_seg2 = 7;f_sjw = 7;F_presc = 0;
	break;
	case rate_1_538Mbit:
		s_seg1 = 62;s_seg2 =15;s_sjw = 15;S_presc = 1;
		f_seg1 = 18;f_seg2 = 5;f_sjw = 5;F_presc = 0;
	break;
	case rate_2Mbit:
		s_seg1 = 62;s_seg2 =15;s_sjw = 15;S_presc = 1;
		f_seg1 = 14;f_seg2 = 3;f_sjw = 3;F_presc = 1;
	break;
	case rate_3_077Mbit:
		s_seg1 = 62;s_seg2 =15;s_sjw = 15;S_presc = 1;
		f_seg1 = 10;f_seg2 = 2;f_sjw = 2;F_presc = 1;
	break;
	case rate_4Mbit:
		s_seg1 = 62;s_seg2 =15;s_sjw = 15;S_presc = 1;
		f_seg1 = 14;f_seg2 = 3;f_sjw = 3;F_presc = 0;
	break;
	case rate_5Mbit:
		s_seg1 = 62;s_seg2 = 15;s_sjw = 15;S_presc = 1;
		f_seg1 = 10;f_seg2 = 3;f_sjw = 3;F_presc = 0;
		printf( "baud rate is 5Mbit/s \r\n");
	break;
	case rate_6_667Mbit:
		s_seg1 = 62;s_seg2 =15;s_sjw = 15;S_presc = 1;
		f_seg1 = 8;f_seg2 = 1;f_sjw = 1;F_presc = 0;
	break;
	case rate_8Mbit:
		s_seg1 = 62;s_seg2 =15;s_sjw = 15;S_presc = 1;
		f_seg1 = 6;f_seg2 = 1;f_sjw = 1;F_presc = 0;
	break;
	case rate_10Mbit:
		s_seg1 = 62;s_seg2 =15;s_sjw = 15;S_presc = 1;
		f_seg1 = 4;f_seg2 = 1;f_sjw = 1;F_presc = 0;
	break;
	case can_rate_1Mbit:
		s_seg1 = 30;s_seg2 = 7;s_sjw = 7;S_presc = 1;
	break;
	default:
		s_seg1 = 30;s_seg2 = 7;s_sjw = 7;S_presc = 1;
	break;
	}
	reset_test=GET_BIT(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,7);

	if(reset_test ==0)
	{
		CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |=CAN_FD_SET_RST_MASK ;
	}
	/*The input bittime setting is incorrect, should be correct*/
	if(val==0){
		CANX ->S_Seg_1_S_Seg_2_S_SJW_S_PRESC =	(uint32_t)(s_seg1 | s_seg2 << 8 | s_sjw  << 16 | S_presc << 24);
		tseg1 = s_seg1 + 2;
		tseg2 = s_seg2 + 1;
	}
	else{
		CANX ->S_Seg_1_S_Seg_2_S_SJW_S_PRESC =	(uint32_t)(s_seg1 | s_seg2 << 8 | s_sjw  << 16 | S_presc << 24);

		CANX ->F_Seg_1_F_Seg_2_F_SJW_F_PRESC =  (uint32_t)(f_seg1 | f_seg2 << 8 | f_sjw  << 16 | F_presc << 24);
		CANX ->EALCAP_TDC_RECNT_TECNT |= CAN_FD_SET_TDCEN_MASK ;
		CANX ->EALCAP_TDC_RECNT_TECNT |= (uint32_t)(f_seg1 << 8);
	}
	/*reset off*/
	CANX ->CFG_STAT_TCMD_TCTRL_RCTRL &= ~CAN_FD_SET_RST_MASK;
	for(delaytime = 0 ; delaytime < 150; delaytime ++){
		delaytime++;
	}
	return 0;
}

int canfd_get_freebuffer(CAN_AL9000_TypeDef *CANX){
	/*get next transmit buffer. If all buffers are full, return -1*/
	CANX ->CFG_STAT_TCMD_TCTRL_RCTRL|=CAN_FD_SET_TENEXT_MASK;
	if(GET_BITS(CANX -> CFG_STAT_TCMD_TCTRL_RCTRL,0,7)&CAN_FD_SET_TENEXT_MASK){


	}
	return 0;

}

void canfd_tx_interrupt(CAN_AL9000_TypeDef *CANX){

	uint8_t isr=0;
	////printf( "canfd tx interrupt\n");
	isr = GET_BITS(CANX -> RTIE_RTIF_ERRINT_LIMIT,0,7);
	/*wait till transmission of the PTB or STB finished*/
	while(isr & (CAN_FD_SET_TPIF_MASK|CAN_FD_SET_TSIF_MASK)){
		if(isr & CAN_FD_SET_TPIF_MASK)
		 CANX ->RTIE_RTIF_ERRINT_LIMIT|=CAN_FD_SET_TPIF_MASK;
		if(isr & CAN_FD_SET_TSIF_MASK)
		CANX ->RTIE_RTIF_ERRINT_LIMIT |=CAN_FD_SET_TSIF_MASK;
		//TODO: time out error
		isr = GET_BITS(CANX -> RTIE_RTIF_ERRINT_LIMIT,0,7);
	}
}

/*
 * Return: 1 on success and 0 on failure.
 */

uint32_t can_rx(CAN_AL9000_TypeDef *CANX,CanRxMsg* RxMessage,enum data_length len){
	uint8_t	control,rx_status;
	volatile int i = 0;

	//printf("start can_rx interrrupt\n");
	control   = can_tx_mode(CANX,NORMAL);
	if(control == 0 || control == 1)
	{
		RxMessage ->IDE   = GET_BIT(CANX ->RBUF_CTL,7);
		RxMessage ->StdId = GET_BITS(CANX ->RBUF_ID_PARM,0,31);
		RxMessage ->DLC = GET_BITS(CANX ->RBUF_CTL,0,3);
		RxMessage ->RTR = GET_BIT(CANX ->RBUF_CTL,6);
		RxMessage ->FDF = GET_BIT(CANX ->RBUF_CTL,5);
		RxMessage ->BRS = GET_BIT(CANX ->RBUF_CTL,4);
		RxMessage -> Status= GET_BITS(CANX ->  RBUF_CTL,8,15);
		if(CANX ->RBUF_CTL & CAN_FD_SET_IDE_MASK){
			/*extended format*/
		RxMessage ->ExtId = GET_BITS(CANX ->RBUF_ID_PARM,0,31);
			printf("ExtID  = %08x\n",RxMessage ->ExtId);
		/*bit 29,error message not defined here*/
		//TODO:error message
		}
		else{
			RxMessage ->StdId =GET_BITS(CANX ->RBUF_ID_PARM,0,31);
			//CANX ->RBUF_ID_PARM &=(~CAN_EFF_FLAG) ;
			printf("without ID Extension\n");
			printf("StdID = %08x\n",RxMessage ->ExtId);
		}

		//if((CANX ->RBUF_CTL)& CAN_FD_SET_RTR_MASK)
			//CANX ->RBUF_CTL |=CAN_RTR_FLAG ;

		/* Data*/
		//if(GET_BIT(CANX ->RBUF_CTL,6) == 0){
			for(i = 0;i< len; i++)
			{
			    //RxMessage ->Data[i] = CANX -> RBUF_DATA[i];
				switch(i%4)
					{
					case 0:
					RxMessage ->Data[i] = (uint8_t)(CANX ->RBUF_DATA[i/4]);
					break;
					case 1:
					RxMessage ->Data[i] = (uint8_t)(CANX ->RBUF_DATA[i/4] >> 8   & 0xff );
					break;
					case 2:
					RxMessage ->Data[i] = (uint8_t)(CANX ->RBUF_DATA[i/4] >> 16  & 0xff);
					break;
					default:
					RxMessage ->Data[i] = (uint8_t)(CANX ->RBUF_DATA[i/4] >> 24  & 0xff);
					break;
					}

			 //printf("RBUF_DATA = %d",RxMessage->Data[i]);
			 //printf("RBUF_DATA = %d",RxMessage->Data[i]);
			}
		//}
		CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |=CAN_FD_SET_RREL_MASK;
		return 1;
	}
	else{
	//rx_status = GET_BITS(CANX -> CFG_STAT_TCMD_TCTRL_RCTRL,24,25);
	//	if(rx_status & CAN_FD_RSTAT_NOT_EMPTY_MASK){
		RxMessage ->IDE = GET_BIT(CANX ->RBUF_CTL,7);
		RxMessage ->StdId = GET_BITS(CANX ->RBUF_ID_PARM,0,31);
		RxMessage ->DLC = GET_BITS(CANX ->RBUF_CTL,0,3);
		RxMessage ->RTR = GET_BIT(CANX ->RBUF_CTL,6);
		RxMessage ->FDF = GET_BIT(CANX ->RBUF_CTL,5);
		RxMessage ->BRS = GET_BIT(CANX ->RBUF_CTL,4);
		RxMessage -> Status= GET_BITS(CANX ->  RBUF_CTL,8,15);
		//printf("DLC in can_rx is %02x",DLC);
		/*change the CANFD id into socketcan id format*/
		if(CANX ->RBUF_CTL & CAN_FD_SET_IDE_MASK){
			/*extended format*/
			RxMessage ->ExtId = GET_BITS(CANX ->RBUF_ID_PARM,0,31);
			printf("ID Extension\n");
			printf("ExtID  = %08x\n",RxMessage ->ExtId);
		/*bit 29,error message not defined here*/
		//TODO:error message
		}
		else{
			RxMessage ->StdId =GET_BITS(CANX ->RBUF_ID_PARM,0,31);
			//CANX ->RBUF_ID_PARM &=(~CAN_EFF_FLAG) ;
			printf("without ID Extension\n");
			printf("StdID  = %08x\n",RxMessage ->StdId);
		}

		//if((CANX ->RBUF_CTL)& CAN_FD_SET_RTR_MASK)
			//CANX ->RBUF_CTL |=CAN_RTR_FLAG ;

		/* Data*/
		//if(!(CANX ->RBUF_CTL)&(CAN_FD_SET_RTR_MASK)){
			for(i = 0;i< len; i++)
			{
			    //RxMessage ->Data[i] = CANX -> RBUF_DATA[i];
				switch(i%4)
					{
					case 0:
					RxMessage ->Data[i] = (uint8_t)CANX ->RBUF_DATA[i/4];
					break;
					case 1:
					RxMessage ->Data[i] = (uint8_t)(CANX ->RBUF_DATA[i/4] >> 8  & 0xff );
					break;
					case 2:
					RxMessage ->Data[i] = (uint8_t)(CANX ->RBUF_DATA[i/4] >> 16  & 0xff);
					break;
					default:
					RxMessage ->Data[i] = (uint8_t)(CANX ->RBUF_DATA[i/4]  >> 24 & 0xff);
					break;
					}

			 //printf("RBUF_DATA = %d",RxMessage->Data[i]);
			}
		//}

		/*point to next packet and will save to buffer*/
		/*reset RREL to release RB slot, next RBUF will be updated. Then reset receive interrupt flag(we can not reset interrupt flag here)*/
		CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |=CAN_FD_SET_RREL_MASK;
		return 1;
	//}
    }
	return 0;
}
#if 0
/*
 * Return: 1 on success and 0 on failure.
 */
int canfd_rx(CAN_AL9000_TypeDef *CANX,uint8_t DLC){
	struct canfd_frame *cf;
	struct sk_buff *skb;

	uint32_t	ctl,can_id,data=0,data_can[2]={0,0};
	uint8_t	dlc,control,rx_status;
	int i;

	/*since here we start canfd_rx interrrupt*/
	//printf("start canfd_rx interrrupt\n");

	/*rx control register has 32 bits--->uint32_t ctl will be used later*/
	rx_status=GET_BITS(CANX ->RCTRL,0,7);
	if(rx_status & CAN_FD_RSTAT_NOT_EMPTY_MASK){
		can_id = GET_BITS(CANX ->RBUF_ID_PARM.RBUF_ID,0,10);
		//dlc=GET_BITS(CANX ->RBUF_CTL,0,7)&CAN_FD_SET_DLC_MASK;


	/*change the CANFD id into socketcan id format*/
		if(control&CAN_FD_SET_EDL_MASK){
			/*extended format*/
			CANX ->TBUF_ID_PARM.TBUF_ID=can_id;
			if(CANX ->RBUF_CTL.IDE & CAN_FD_SET_IDE_MASK)
				CANX ->TBUF_ID_PARM.TBUF_ID |= CAN_EFF_FLAG;
			else
				CANX ->TBUF_ID_PARM.TBUF_ID &= (~CAN_EFF_FLAG);
		/*bit 29,error message not defined here*/

		}
		else{
			CANX ->RBUF_ID_PARM.RBUF_ID=can_id;
			if(CANX ->RBUF_CTL.IDE & CAN_FD_SET_IDE_MASK)
				CANX ->RBUF_ID_PARM.RBUF_ID |= CAN_EFF_FLAG;
			else
				CANX ->RBUF_ID_PARM.RBUF_ID &= (~CAN_EFF_FLAG);
	/*deal with RTR in can2.0*/
			if(CANX ->RBUF_CTL.RTR&CAN_FD_SET_RTR_MASK)
				CANX ->RBUF_ID_PARM.RBUF_ID |=CAN_RTR_FLAG ;

		}

		/* Data*/
		/*CANFD frames handed over to SKB*/
		if(control&CAN_FD_SET_EDL_MASK){
			for (i = 0; i < DLC; i += 4) {
				printf( "CAN FD original DATA 0x%08x\n",CANX ->RBUF_DATA[i]);
			}
		}
		/*point to next packet and will save to buffer*/
		/*reset RREL to release RB slot, next RBUF will be updated. Then reset receive interrupt flag(we can not reset interrupt flag here)*/

		CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |=CAN_FD_SET_RREL_MASK;
		//canfd_reigister_set_bit(priv,RTIF,CAN_FD_SET_RIF_MASK);

		return 1;
	}
	return 0;
}
#endif
int canfd_rx_poll(CAN_AL9000_TypeDef *CANX, int quota){
	int work_done = 0;
	uint8_t rx_status = 0,control = 0;

	control   = GET_BITS(CANX -> RBUF_CTL,0,7);
	rx_status = GET_BITS(CANX -> CFG_STAT_TCMD_TCTRL_RCTRL,24,31);
	/*clear receive interrupt and deal with all the received frames*/
	//printf("CANFD RX POLL");
	//canfd_reigister_set_bit(priv,RTIF,CAN_FD_SET_RIF_MASK);
	while((rx_status&CAN_FD_RSTAT_NOT_EMPTY_MASK)&&(work_done < quota)){
		rx_status = GET_BITS(CANX -> CFG_STAT_TCMD_TCTRL_RCTRL,0,7);
		printf( "rx-status is 0x%02x\n",rx_status);
		printf( "rx-status is 0x%08x\n",GET_BITS(CANX -> CFG_STAT_TCMD_TCTRL_RCTRL,0,7));

	}

	//if (work_done < 10) {
		CANX ->RTIE_RTIF_ERRINT_LIMIT |=CAN_FD_SET_RIE_MASK;
		//canfd_reigister_set_bit(priv,RTIE,	CAN_FD_SET_RFIE_MASK);
		//canfd_reigister_set_bit(priv,RTIE,	CAN_FD_SET_RAFIE_MASK);
	//}
	return work_done;
}

void canfd_rxfull_interrupt(CAN_AL9000_TypeDef *CANX,uint32_t isr){

	if(isr & CAN_FD_SET_RAFIF_MASK)
	CANX ->RTIE_RTIF_ERRINT_LIMIT|=CAN_FD_SET_RAFIF_MASK;

}

/*Mode could be chosen, by reading corresponding registers.*/
/*This part could also be removed, when only one mode is used*/
 int set_canfd_xmit_mode(CAN_AL9000_TypeDef *CANX,enum can_tx_mode tx_mode){
	switch(tx_mode){
		case XMIT_FULL:
			CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |=CAN_FD_SET_FULLCAN_MASK;
			printf( "Full can mode\n");
			break;

		case XMIT_SEP_FIFO:
			CANX ->CFG_STAT_TCMD_TCTRL_RCTRL &=~CAN_FD_SET_FULLCAN_MASK;
			CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |= CAN_FD_SET_FIFO_MASK;
			CANX ->CFG_STAT_TCMD_TCTRL_RCTRL &= ~CAN_FD_SET_TBSEL_MASK;
			printf( "FIFO mode\n");

			break;

		case XMIT_SEP_PRIO://有问题
			CANX ->CFG_STAT_TCMD_TCTRL_RCTRL &=~CAN_FD_SET_FULLCAN_MASK;
			CANX ->CFG_STAT_TCMD_TCTRL_RCTRL &=~CAN_FD_SET_FIFO_MASK ;
			CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |=CAN_FD_SET_TBSEL_MASK ;
			CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |= CAN_FD_SET_TENEXT_MASK;
			printf( "Priority mode\n");

			break;
		case XMIT_PTB_MODE:
			//CANX ->CFG_STAT_TCMD_TCTRL_RCTRL &= ~CAN_FD_SET_FULLCAN_MASK;
			//CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |= CAN_FD_SET_FIFO_MASK;
			//CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |= CAN_CFG_STAT_TPSS;
			//CANX ->CFG_STAT_TCMD_TCTRL_RCTRL   |= CAN_FD_SET_TENEXT_MASK;
			printf( "PTB mode\n");

			break;
		default:
			break;
	}
	return 0;
}

/*
 * netdev_tx_t (*ndo_start_xmit)(struct sk_buff *skb,
 *                               struct net_device *dev);
 *	Called when a packet needs to be transmitted.
 *	Must return NETDEV_TX_OK , NETDEV_TX_BUSY.
 *        (can also return NETDEV_TX_LOCKED iff NETIF_F_LLTX)
 *	Required can not be NULL.
*/

int canfd_driver_start_xmit(CAN_AL9000_TypeDef *CANX,CanTxMsg* TxMessage,enum can_tx_mode tx_mode,enum data_length len){
	volatile uint32_t ttsen,id,temp,addr_off;
	volatile uint8_t i;

	//printf( "CANFD Driver starts to send canfd/can 2.0 frame since here\n");
	tx_mode=XMIT_PTB_MODE;//test
						CANX ->TBUF_CTL = ((uint32_t)TxMessage ->DLC)|\
								   ((uint32_t)TxMessage ->IDE) << 7|\
								   ((uint32_t)TxMessage ->FDF) << 5|\
								   ((uint32_t)TxMessage ->RTR) << 6|\
								   ((uint32_t)TxMessage ->BRS) << 4;

	switch(tx_mode){
		case XMIT_FULL:
			return -1;
			break;
		case XMIT_PTB_MODE:
			set_canfd_xmit_mode(CANX,tx_mode);
			CANX ->CFG_STAT_TCMD_TCTRL_RCTRL &=CAN_FD_OFF_STBY_MASK;/* standy mode off*/
			//printf("CANX ->TBUF_ID_PARM.TBUF_ID=%x10",CANX ->RBUF_ID_PARM.RBUF_ID);
			/*transmit can fd frame*/
			if(GET_BIT(CANX ->TBUF_CTL,5)==CANFD_MASK){
				if (GET_BIT(CANX ->TBUF_CTL,5)==CANFD_MASK) {
					if(CAN_Id_Standard == GET_BIT(CANX ->TBUF_CTL,7)){
						CANX ->TBUF_CTL = ((uint32_t)TxMessage ->DLC)|\
								   ((uint32_t)TxMessage ->IDE) << 7|\
								   ((uint32_t)TxMessage ->FDF) << 5|\
								   ((uint32_t)TxMessage ->RTR) << 6|\
								   ((uint32_t)TxMessage ->BRS) << 4;
						CANX ->TBUF_ID_PARM = TxMessage ->StdId;// | TxMessage ->TTSEN;
						//printf("tx_StdID = %08x\n",TxMessage ->StdId);
					}
					else{
						//CANX ->TBUF_ID_PARM = (uint32_t)0x00000000;
						CANX ->TBUF_CTL = ((uint32_t)TxMessage ->DLC)|\
								   ((uint32_t)TxMessage ->IDE) << 7|\
								   ((uint32_t)TxMessage ->FDF) << 5|\
								   ((uint32_t)TxMessage ->RTR) << 6|\
								   ((uint32_t)TxMessage ->BRS) << 4;

						CANX ->TBUF_ID_PARM = TxMessage->ExtId ;//| TxMessage ->TTSEN;/*IDE=0*/
					}
					if (GET_BIT(CANX ->TBUF_CTL,4)== 1){		/*CANFD_BRS=0x01*/
						CANX ->TBUF_CTL = ((uint32_t)TxMessage ->DLC)|\
								   ((uint32_t)TxMessage ->IDE) << 7|\
								   ((uint32_t)TxMessage ->FDF) << 5|\
								   ((uint32_t)TxMessage ->RTR) << 6|\
								   ((uint32_t)TxMessage ->BRS) << 4;
						}
						for(i=0;i< len;i++){
					switch(i%4)
					{
					case 0:
					CANX ->TBUF_DATA[i/4] =TxMessage ->Data[i];
					break;
					case 1:
					CANX ->TBUF_DATA[i/4] =TxMessage ->Data[i-1] |TxMessage ->Data[i] << 8 ;
					break;
					case 2:
					CANX ->TBUF_DATA[i/4] =TxMessage ->Data[i-2] |TxMessage ->Data[i-1] << 8 | TxMessage ->Data[i] << 16;
					break;
					default:
					CANX ->TBUF_DATA[i/4] =TxMessage ->Data[i-3] |TxMessage ->Data[i-2] << 8 | TxMessage ->Data[i-1] << 16|TxMessage ->Data[i] << 24;
					break;
					}
					//printf("TBUF_data %d",i,CANX ->TBUF_DATA[i]);
				    }
				}

				/* Transmit can 2.0 frame under canfd mode*/
				else{
					TxMessage ->BRS =0;
					if(CAN_Id_Standard == GET_BIT(CANX ->TBUF_CTL,7)){
						//CANX ->TBUF_ID_PARM = (uint32_t)0x00000000;
						//CANX ->TBUF_CTL.IDE = TxMessage ->IDE;
						CANX ->TBUF_CTL = ((uint32_t)TxMessage ->DLC)|\
								   ((uint32_t)TxMessage ->IDE) << 7|\
								   ((uint32_t)TxMessage ->FDF) << 5|\
								   ((uint32_t)TxMessage ->RTR) << 6|\
								   ((uint32_t)TxMessage ->BRS) << 4;
						CANX ->TBUF_ID_PARM = TxMessage ->StdId;// | TxMessage ->TTSEN;
						//printf("StdID = %08x\n",TxMessage ->StdId);
					}
					else{
						//CANX ->TBUF_ID_PARM = (uint32_t)0x00000000;
						CANX ->TBUF_CTL = ((uint32_t)TxMessage ->DLC)|\
								   ((uint32_t)TxMessage ->IDE) << 7|\
								   ((uint32_t)TxMessage ->FDF) << 5|\
								   ((uint32_t)TxMessage ->RTR) << 6|\
								   ((uint32_t)TxMessage ->BRS) << 4;
						CANX ->TBUF_ID_PARM = TxMessage->ExtId;// | TxMessage ->TTSEN;/*IDE=0*/
					}

				}

			}
			/*deal with can 2.0 frame, when the driver can only be compatible with can 2.0 tranceiver	*/
			else{
				/*transmit can2.0 frame*/
				//printf("Sending can2.0 frame\n");
				CANX ->TBUF_CTL |= TxMessage -> BRS << 4 ;
				if(CAN_Id_Standard == GET_BIT(CANX ->TBUF_CTL,7)){
					//CANX ->TBUF_ID_PARM = (uint32_t)0x00000000;
					//CANX ->TBUF_CTL.IDE = TxMessage ->IDE;
						CANX ->TBUF_CTL = ((uint32_t)TxMessage ->DLC)|\
								   ((uint32_t)TxMessage ->IDE) << 7|\
								   ((uint32_t)TxMessage ->FDF) << 5|\
								   ((uint32_t)TxMessage ->RTR) << 6|\
								   ((uint32_t)TxMessage ->BRS) << 4;
					CANX ->TBUF_ID_PARM = TxMessage ->StdId;// | TxMessage ->TTSEN;
					//printf("StdID = %08x\n",TxMessage ->StdId);
				}
				else{
					//CANX ->TBUF_ID_PARM = (uint32_t)0x00000000;
						CANX ->TBUF_CTL = ((uint32_t)TxMessage ->DLC)|\
								   ((uint32_t)TxMessage ->IDE) << 7|\
								   ((uint32_t)TxMessage ->FDF) << 5|\
								   ((uint32_t)TxMessage ->RTR) << 6|\
								   ((uint32_t)TxMessage ->BRS) << 4;

					CANX ->TBUF_ID_PARM = TxMessage->ExtId;// | TxMessage ->TTSEN;/*IDE=0*/
				}

					for(i=0;i<len;i++){
					switch(i%4)
					{
					case 0:
					CANX ->TBUF_DATA[i/4] =TxMessage ->Data[i];
					break;
					case 1:
					CANX ->TBUF_DATA[i/4] =TxMessage ->Data[i-1] |TxMessage ->Data[i] << 8 ;
					break;
					case 2:
					CANX ->TBUF_DATA[i/4] =TxMessage ->Data[i-2] |TxMessage ->Data[i-1] << 8 | TxMessage ->Data[i] << 16;
					break;
					default:
					CANX ->TBUF_DATA[i/4] =TxMessage ->Data[i-3] |TxMessage ->Data[i-2] << 8 | TxMessage ->Data[i-1] << 16|TxMessage ->Data[i] << 24;
					break;
					}
				    }

				}

			/*set TPE to transmit data. update statistic*/
			CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |=CAN_FD_SET_TPE_MASK;
					break;
		default:
			break;

	return 0;
	}
}

uint8_t set_rx_fillter(CAN_AL9000_TypeDef *CANX,enum ACFADR ACFADDR,uint32_t ID){
	set_reset_mode(CANX);
	volatile uint32_t ACE_val = 0;
	switch(ACFADDR){
		case AE1:
				ACE_val = 1;
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 |= CAN_FD_ACFCTRL_AELMASK;
				CANX ->ACF_0123							 &= (uint32_t)(0xe0000000);
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 |= (uint32_t)(((uint32_t)AE1) << 16);
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 &= (uint32_t)(0x00000000);
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 |= ACE_val;
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 |= (uint32_t)(AE1 << 16);
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 &=~CAN_FD_ACFCTRL_AELMASK;
				CANX ->ACF_0123							 |= ID;
				printf("AE1 %08x\r\n",CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1);
				printf("AE1_ID %08x\r\n",CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1);

		break;
		case AE2:
				ACE_val = 2;
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 &= (uint32_t)(0x00000000);
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 |= ACE_val;
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 |= (uint32_t)(AE2 << 16);
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 |= CAN_FD_ACFCTRL_AELMASK;
				CANX ->ACF_0123							 &= (uint32_t)(0xefffff00);
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 &=~CAN_FD_ACFCTRL_AELMASK;
				CANX ->ACF_0123							 |= (uint32_t)(0x00000011);
				printf("AE2\r\n");
		break;
		default:
				ACE_val = 0;
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 &= (uint32_t)(0x00000000);
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 |= ACE_val;
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 |= (uint32_t)(AE0 << 16);
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 |= CAN_FD_ACFCTRL_AELMASK;
				CANX ->ACF_0123							 |= (uint32_t)(0x1fffffff);
				CANX ->ACFCTRL_TIMECFG_ACF_EN_0_ACF_EN_1 &=~CAN_FD_ACFCTRL_AELMASK;
				CANX ->ACF_0123							 &= (uint32_t)(0xe0000000);
				printf("AE0\r\n");
		break;

	}

}
int set_reset_mode(CAN_AL9000_TypeDef *CANX){

	/*Situation for time out still not defined*/
	volatile uint8_t ret;
	//unsigned long timeout;
	if(GET_BIT(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,7) == 0)
	{
		CANX ->CFG_STAT_TCMD_TCTRL_RCTRL |= CAN_FD_SET_RST_MASK;
	}
	else
	{
		printf("reset_ok\r\n");
	}
		return 0;

}

uint32_t get_of_chip_status(CAN_AL9000_TypeDef *CANX){
	volatile uint8_t  can_stat,eir;
	can_stat=GET_BITS(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,0,7);
	eir = GET_BITS(CANX -> RTIE_RTIF_ERRINT_LIMIT,0,7);

	if(can_stat & CAN_FD_SET_BUSOFF_MASK)
		return GET_BIT(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,7);
	if((eir & CAN_FD_SET_EPASS_MASK) && ~(can_stat & CAN_FD_SET_BUSOFF_MASK))
		return 1;
	 if((eir & CAN_FD_SET_EWARN_MASK) && ~(eir & CAN_FD_SET_EPASS_MASK))
		return 0;
	 if(~(eir & CAN_FD_SET_EPASS_MASK))
		return 1;
}

/*Error Interrupt Routine*/
void canfd_error_interrupt(CAN_AL9000_TypeDef *CANX){
	uint8_t  koer,recnt=0,tecnt=0,can_stat=0,stats=0,count=0;
	uint32_t	 data[8];
	uint8_t eir = 0;
	eir = GET_BITS(CANX ->RTIE_RTIF_ERRINT_LIMIT,0,7);
	//printf("in Error interrupt");

	koer = GET_BITS(CANX ->EALCAP_TDC_RECNT_TECNT,0,7)&CAN_FD_SET_KOER_MASK;
	recnt= GET_BITS(CANX ->EALCAP_TDC_RECNT_TECNT,0,7);
	tecnt= GET_BITS(CANX ->EALCAP_TDC_RECNT_TECNT,0,7);

	//test
	//if(recnt>40 || tecnt>40) canfd_reigister_set_bit(priv,CANX ->CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,CAN_FD_SET_BUSOFF_MASK);

	/*read CAN Status*/
	can_stat=GET_BITS(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,0,7);

	/*BUS-OFF->active error mode*/

	/*bus off*/
	/*if status is BUSOFF*/
	if(can_stat & CAN_FD_SET_BUSOFF_MASK){
		//printf("CANX ->CANX ->CFG_STAT_TCMD_TCTRL_RCTRL(BUSOFF bit) in EIF branch: 0x%02x\n",GET_BITS(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,0,7));
		//printf("BUSOFF exists\n");
		/*abortion of not transmitted frame*/
		//canfd_reigister_set_bit(priv,TCMD,CAN_FD_SET_TPA_MASK);
		/*statistics*/
		stats=get_of_chip_status(CANX);
		count++;
		/*reset canfd ip core*/
		//test with reset busoff status
		CANX ->CFG_STAT_TCMD_TCTRL_RCTRL &=CAN_FD_SET_BUSOFF_MASK;
		//canfd_reigister_set_bit(priv,CANX ->CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,CAN_FD_SET_RST_MASK);

		/*tell netif layer,no more packets*/
		//TODO:configure the can to bus-off status?
		//printf("CAN_ERR_BUSOFF");
	}else if((eir & CAN_FD_SET_EPASS_MASK) && ~(can_stat & CAN_FD_SET_BUSOFF_MASK)){
		/*Error Passive mode*/
		/*EPASS is either 0 or 1. Hypothetically, when transceiver is in BUSOFF mode, bit EPASS stays unchanged, i.e. "1" */
		//printf("Error Interrupt in Error Passive mode\n");
		stats= get_of_chip_status(CANX);
		count++;
		//printf("CAN_ERR_CRTL");
	}else if((eir & CAN_FD_SET_EWARN_MASK) && ~(eir & CAN_FD_SET_EPASS_MASK)){
		/*Counter over Error limitation*/

		//printf("Error Warning\n");
		stats =get_of_chip_status(CANX);
		count++;
	}


	//printf("CANX ->CANX ->CFG_STAT_TCMD_TCTRL_RCTRL(BUSOFF bit) before protocol errors: 0x%02x\n",GET_BITS(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,0,7));
	/* Check for in protocol defined error interrupt */
	if(eir & CAN_FD_SET_BEIF_MASK){
		//printf("protocol defined error exists\n");
		//printf("KOER is :0x%02x in protocol error \n",GET_BITS(CANX ->EALCAP,0,7)&CAN_FD_SET_KOER_MASK);
		/*BIT error interrupt*/
		if(koer == CAN_FD_SET_BIT_ERROR_MASK){
			//printf("BIT error exists\n");
			count++;
		}

		/*FORM error interrupt*/
		if(koer == CAN_FD_SET_FORM_ERROR_MASK){
			//printf("FORM error exists\n");
			count++;
		}

		/*STUFF error interrupt*/
		if(koer == CAN_FD_SET_STUFF_ERROR_MASK){
			//printf("STUFF error exists\n");
			count++;
		}

		/*ACK error interrupt*/
		if(koer == CAN_FD_SET_ACK_ERROR_MASK){
			//printf("ACK error exists\n");
			/*test with abortion*/
			//canfd_reigister_set_bit(priv,TCMD,CAN_FD_SET_TPA_MASK);

			count++;
		}

		/*CRC error interrupt*/
		if(koer == CAN_FD_SET_CRC_ERROR_MASK){
			//printf("CRC error exists\n");
			count++;
		}
		count++;
	}

}

uint32_t canfd_interrupt(CAN_AL9000_TypeDef *CANX){
	uint8_t isr,eir;
	uint8_t isr_handled, eir_handled;
	/*read the value of interrupt status register and error interrupt register*/
	isr = GET_BITS(CANX -> RTIE_RTIF_ERRINT_LIMIT,0,7);
	eir = GET_BITS(CANX -> RTIE_RTIF_ERRINT_LIMIT,0,7);


	isr_handled = 0;
	eir_handled = 0;

	/* Check for Tx interrupt and Processing it */
	if (isr & (CAN_FD_SET_TPIF_MASK|CAN_FD_SET_TSIF_MASK)){
		canfd_tx_interrupt(CANX);
		isr_handled |= (CAN_FD_SET_TPIF_MASK|CAN_FD_SET_TSIF_MASK);
	}

	if (isr & (CAN_FD_SET_RAFIF_MASK|CAN_FD_SET_RFIF_MASK) ) {
		canfd_rxfull_interrupt(CANX,isr);
		isr_handled |= (CAN_FD_SET_RAFIF_MASK|CAN_FD_SET_RFIF_MASK);
	}

	/*	Check Rx interrupt and Processing the receive interrupt routine*/
	if (isr & CAN_FD_SET_RIF_MASK){
		CANX ->RTIE_RTIF_ERRINT_LIMIT &=CAN_FD_OFF_RIE_MASK;
		CANX ->RTIE_RTIF_ERRINT_LIMIT |=CAN_FD_SET_RIF_MASK;
		isr_handled |= CAN_FD_SET_RIF_MASK;

	}
	//test
	//printf("RTIE before error routine is 0x%02x\n",GET_BITS(CANX ->RTIE,0,7));
	// if (~( (isr&CAN_FD_SET_EIF_MASK) || (eir & CAN_FD_SET_EPIF_MASK ) || (eir & CAN_FD_SET_BEIF_MASK))) {
	// 		return IRQ_NONE;
	// 	}
	if ( (isr & CAN_FD_SET_EIF_MASK) | (eir & (CAN_FD_SET_EPIF_MASK | CAN_FD_SET_BEIF_MASK))){
	//if (isr & CAN_FD_SET_EIF_MASK){
		//printf("Error Error Error\n");
		//canfd_reigister_off_bit(priv,ERRINT,CAN_FD_OFF_EPIE_MASK&CAN_FD_OFF_BEIE_MASK);
		//canfd_reigister_off_bit(priv,RTIE,CAN_FD_OFF_EIE_MASK);

		/*reset EPIF and BEIF. Reset EIF. What if during error interrupt, new interrupt appears?*/
		CANX ->RTIE_RTIF_ERRINT_LIMIT |=eir & (CAN_FD_SET_EPIF_MASK|CAN_FD_SET_BEIF_MASK);
		CANX ->RTIE_RTIF_ERRINT_LIMIT|= isr & CAN_FD_SET_EIF_MASK;

		canfd_error_interrupt(CANX);
		isr_handled |= CAN_FD_SET_EIF_MASK;
		eir_handled |=(CAN_FD_SET_EPIF_MASK | CAN_FD_SET_BEIF_MASK);
	}
	if ((isr_handled == 0) && (eir_handled == 0)) {
		printf("unhandled interrupt!");
		return 0;
	}
	//printf("ISR: %02X/%02X\n", isr, isr_handled);
	//printf("EIR: %02X/%02X\n", eir, eir_handled);
	//printf("canfd error counter: RECNT is 0x%02x, TECNT is 0x%02x\n",GET_BITS(CANX->RECNT,0,7),GET_BITS(CANX->TECNT,0,7));
	return 0;
}
#if 0
int canfd_chip_start(CAN_AL9000_TypeDef *CANX,enum baud_rate rate){
	int temp;
	uint8_t ret;

	//printf( "can starts to work here\n");

	temp=set_reset_mode(CANX);
	if (temp < 0){
		//printf( "set reset mod failed\n");
		return temp;
    }
	temp=canfd_device_driver_bittime_configuration(CANX,rate );/*set S_Seg_1 S_Seg_2 S_SJW S_PRESC */
	if (temp < 0){
		//printf( "bittime setting failed\n");
		return temp;
    }
	/*set Almost Full Warning Limit*/
	CANX ->LIMIT |=CAN_FD_SET_AFWL_MASK;
	/*Programmable Error Warning Limit = (EWL+1)*8. Set EWL=11->Error Warning=96 */
	CANX ->LIMIT |=CAN_FD_SET_EWL_MASK;
	/*the start state*/
	CANX ->RBUF_ID_PARM.RBUF_ESI = ACTIVE;
	/*interrupts enable*/
	CANX ->RTIE |=CAN_FD_INTR_ALL_MASK;
	/*Error Interrupts enable(Error Passive and Bus Error)*/
	CANX ->ERRINT |=CAN_FD_SET_EPIE_MASK;
	CANX ->ERRINT |=CAN_FD_SET_BEIE_MASK;
	/*check the mode,later on*/

	/*set mode*/
	ret=GET_BITS(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,0,7);
	//ret|=CAN_FD_LBMEMOD_MASK;	/*set loopback externel mode*/
	ret&=~CAN_FD_LBMEMOD_MASK;	/*unset loopback externel mode*/
	CANX ->CFG_STAT_TCMD_TCTRL_RCTRL = ret;
	return 0;

}
#endif
uint8_t CAN_RX_INTERRUPT_ENABLE(CAN_AL9000_TypeDef *CANX)
{
	/*receive interrupt enable */
	CANX ->RTIE_RTIF_ERRINT_LIMIT |= CAN_FD_SET_RIE_MASK;
	/*receive buffer overrun interrupt enable*/
	CANX ->RTIE_RTIF_ERRINT_LIMIT |= CAN_FD_SET_ROE_MASK;
	/*receive buffer full interrupt enable*/
	CANX ->RTIE_RTIF_ERRINT_LIMIT |= CAN_FD_SET_RFIE_MASK;
	/*receive buffer almost full interrupt enable*/
	CANX ->RTIE_RTIF_ERRINT_LIMIT |= CAN_FD_SET_RAFIE_MASK;
}
uint8_t CAN_TransmitStatus(CAN_AL9000_TypeDef *CANX){
	uint8_t state = 0;
	uint8_t can_cfg_state;
	can_cfg_state = GET_BITS(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,11,12);
	switch(can_cfg_state){
	case 0:
	break;
	case 1:
		printf("please set  STB");
	break;
	case 2:
		if(GET_BIT(CANX ->RTIE_RTIF_ERRINT_LIMIT,10) == CAN_TxStatus_Ok)
			state = CAN_TxStatus_Ok;
		else
			state = CAN_TxStatus_Failed;
	break;
	case 3:
		if(GET_BIT(CANX ->RTIE_RTIF_ERRINT_LIMIT,10)||GET_BIT(CANX ->RTIE_RTIF_ERRINT_LIMIT,11))
				state = CAN_TxStatus_Ok;
			else
				state = CAN_TxStatus_Failed;
	break;
	default:
		state = CAN_TxStatus_Failed;
	break;
	}
	return state;
}
uint8_t CAN_MessagePending(CAN_AL9000_TypeDef *CANX){
	volatile uint8_t state = 0;
	volatile uint8_t can_cfg_state;
	can_cfg_state = GET_BITS(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,24,25);
	switch(can_cfg_state){
	case 0:
		state = CAN_RxStatus_Failed;
	break;
	case 1:
		state = CAN_RxStatus_Ok;
	break;
	case 2:
		state = CAN_RxStatus_Ok;
	break;
	case 3:
		state = CAN_RxStatus_Ok;
		printf("RX buffer is full");
		while(3 == GET_BITS(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,24,25));
	break;
	default:
		state = CAN_RxStatus_Failed;
	break;
	}
	while(GET_BIT(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,29));
	//printf("state =  %d \r\n",state);
	return state;
}
uint8_t can_TX_length(enum data_length len){
	CanTxMsg TxMessage;
	switch(len){
	case data_length_0 :
		TxMessage.DLC = 0x00;
		break;
	case data_length_1 :
		TxMessage.DLC = 0x01;
		break;
	case data_length_2 :
		TxMessage.DLC = 0x02;
		break;
	case data_length_3 :
		TxMessage.DLC= 0x03;
		break;
	case data_length_4 :
		TxMessage.DLC = 0x04;
		break;
	case data_length_5:
		TxMessage.DLC = 0x05;
		break;
	case data_length_6 :
		TxMessage.DLC = 0x06;
		break;
	case data_length_7 :
		TxMessage.DLC = 0x07;
		break;
	case data_length_8 :
		TxMessage.DLC = 0x08;
		break;
	case data_length_12 :
		TxMessage.DLC = 0x09;
		break;
	case data_length_16 :
		TxMessage.DLC = 0x0A;
		break;
	case data_length_20 :
		TxMessage.DLC = 0x0B;
		break;
	case data_length_24 :
		TxMessage.DLC = 0x0C;
		break;
	case data_length_32 :
		TxMessage.DLC = 0x0D;
		break;
	case data_length_48 :
		TxMessage.DLC = 0x0E;
		break;
	case data_length_64 :
		TxMessage.DLC = 0x0F;
		break;
	default :
		TxMessage.DLC = 0x08;
		break;
	}
	return TxMessage.DLC;
}
uint8_t Can_Send_Msg(CAN_AL9000_TypeDef *CANX,uint8_t* msg,enum can_tx_mode tx_mode,enum data_length len ,uint32_t ID)
{
	volatile uint8_t mbox;
	volatile uint8_t isr;
	volatile uint16_t i=0;
	volatile uint8_t count = 0;
	CanTxMsg TxMessage;
	TxMessage.StdId=ID;			// 标准标识符
	TxMessage.ExtId=0x12;			// 设置扩展标示符
	TxMessage.IDE=CAN_Id_Standard; 	// 标准帧
	TxMessage.RTR=0;//CAN_RTR_Data;		// 数据帧
	TxMessage.FDF = 1;
	TxMessage.BRS = 1;
	TxMessage.DLC = can_TX_length(len);
	TxMessage.TTSEN = 0x00000000;            //time stamp off = 0x00000000/time stamp on 0x80000000
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];
	mbox= canfd_driver_start_xmit(CANX,&TxMessage,tx_mode,len);
	isr = GET_BITS(CANX ->RTIE_RTIF_ERRINT_LIMIT,0,7);
	while(GET_BIT(CANX ->RTIE_RTIF_ERRINT_LIMIT,11) == PASSIVE);
	//while(GET_BIT(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,1) == ACTIVE);
	if(GET_BIT(CANX ->RTIE_RTIF_ERRINT_LIMIT,11) == ACTIVE){
		i=0;
		CANX ->RTIE_RTIF_ERRINT_LIMIT |= CAN_FD_SET_TPIF_MASK;
		//while(GET_BIT(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,1) == ACTIVE);//等待发送完成
		i++;	//等待发送结束
		if(i>=0XFFF)return 1;
	}
	else{
		//printf("can is not active");
		//CANX -> CFG_STAT_TCMD_TCTRL_RCTRL |= CAN_FD_SET_TPA_MASK;
		//count++;
	}
	i=0;
	return 0;
}
//can口接收数据查询
//buf:数据缓存区;
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
uint8_t Can_Receive_Msg(CAN_AL9000_TypeDef *CANX,uint8_t *buf,enum data_length len)
{
 	volatile uint32_t i;
	CanRxMsg RxMessage;
	//if(CAN_MessagePending(CANX) == 0) return 0;//没有接收到数据,直接退出
	/*if(GET_BIT(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,2) == PASSIVE){
	      printf("can_rx is not active\r\n");
	      while(0 == GET_BIT(CANX ->CFG_STAT_TCMD_TCTRL_RCTRL,2));
	      return 0;
	 }*/
	if(CAN_MessagePending(CANX) == 0) return 0;//没有接收到数据,直接退出
	else {
		  if(CAN_MessagePending(CANX) == 0) return 0;//没有接收到数据,直接退出
		  can_rx(CANX,&RxMessage,len);
		  for(i=0;i < len;i++)
		  buf[i]=RxMessage.Data[i];
		  //printf( "can_tx_mode %08x\n",CANX ->CFG_STAT_TCMD_TCTRL_RCTRL);
		  //printf("intr %08x\r\n",CANX ->EALCAP_TDC_RECNT_TECNT);
		  return RxMessage.DLC;
	      CANX ->RTIE_RTIF_ERRINT_LIMIT |= CAN_INTF_RIF;
	    }
	return 0;
}
void  CAN_TX_COMPLETE(CAN_AL9000_TypeDef *CANX)
{
	CANX -> CFG_STAT_TCMD_TCTRL_RCTRL |= CAN_FD_SET_TPA_MASK;
	CANX -> CFG_STAT_TCMD_TCTRL_RCTRL |= CAN_FD_SET_RST_MASK;
	CANX -> CFG_STAT_TCMD_TCTRL_RCTRL &=~CAN_FD_SET_TPA_MASK;
	CANX -> CFG_STAT_TCMD_TCTRL_RCTRL &=~CAN_FD_SET_RST_MASK;
}
void CAN_TEST_error(CAN_AL9000_TypeDef *CANX){
	if(GET_BITS(CANX ->EALCAP_TDC_RECNT_TECNT,24,31) == 0){
		printf("can trasmiate success\r\n");
	}
	else{printf("can trasmiate error\r\n");}
	if(GET_BITS(CANX ->EALCAP_TDC_RECNT_TECNT,16,23) == 0){
		printf("can receive success\r\n");
	}
	else{printf("can receive error\r\n");}
}
