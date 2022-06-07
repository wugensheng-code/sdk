/*
 * al9000_WDT.H
 *
 *  Created on: 2022.02.15
 *      Author: jian.huang
 */

#include "demosoc.h"

#ifndef _AL9000_WDT_H_
#define _AL9000_WDT_H_

#ifdef __cplusplus
extern "C" {
#endif






/****  Bit definition for WDT_REG_CR register  ****/

//Peripheral_RegName_RegBitsFeature_Value

/* Reset pulse length */
#define WDT_CR_RPL_FIELD		(2)

#define WDT_CR_RPL_0  			(0 << WDT_CR_RPL_FIELD) /* 2 plck cycles */
#define WDT_CR_RPL_1  			(1 << WDT_CR_RPL_FIELD) /* 4 plck cycles */
#define WDT_CR_RPL_2  			(2 << WDT_CR_RPL_FIELD) /* 8 plck cycles */
#define WDT_CR_RPL_3  			(3 << WDT_CR_RPL_FIELD) /* 16 plck cycles */
#define WDT_CR_RPL_4  			(4 << WDT_CR_RPL_FIELD) /* 32 plck cycles */
#define WDT_CR_RPL_5  			(5 << WDT_CR_RPL_FIELD) /* 64 plck cycles */
#define WDT_CR_RPL_6  			(6 << WDT_CR_RPL_FIELD) /* 128 plck cycles */
#define WDT_CR_RPL_7  			(7 << WDT_CR_RPL_FIELD) /* 256 plck cycles */

/* Response mode */
#define WDT_CR_RMOD_RESET_FIELD	(1)
#define WDT_CR_RMOD_RESET   		(0 << WDT_CR_RMOD_RESET_FIELD) /* Generate a system reset */
#define WDT_CR_RMOD_INTERRUPT   	(1 << WDT_CR_RMOD_RESET_FIELD) /* First generate an interupt.Second timeout a system reset */

/* WDT enable */
#define WDT_CR_WDT_EN_FIELD		(0)
#define WDT_CR_WDT_EN   		(1 << WDT_CR_WDT_EN_FIELD)  /* WDT Enable */

/****  Bit definition for WDT_REG_TORR register  ****/

/* Timeout period for initialization. */
#define WDT_TORR_TOP_INIT_FIELD (4)
/* read only */

/* Timeout period */
#define WDT_TORR_TOP_FIELD 		(0)

#define WDT_TORR_TOP_10737M   			((uint8_t)0xf << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_5368M   			((uint8_t)0xe << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_2684M			((uint8_t)0xd << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_1342M   			((uint8_t)0xc << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_671M   			((uint8_t)0xb << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_335M   			((uint8_t)0xa << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_167M   			((uint8_t)0x9 << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_83M   			((uint8_t)0x8 << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_41M   			((uint8_t)0x7 << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_20M   			((uint8_t)0x6 << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_10M   			((uint8_t)0x5 << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_5M    			((uint8_t)0x4 << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_2621U  			((uint8_t)0x3 << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_1310U   			((uint8_t)0x2 << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_655U   			((uint8_t)0x1 << WDT_TORR_TOP_FIELD)
#define WDT_TORR_TOP_327U   			((uint8_t)0x0 << WDT_TORR_TOP_FIELD) /* time value set 327us */


/****  Bit definition for WDT_REG_CCVR register  ****/
/***		Current Counter Value Register 		***/

/* WDT Current Counter Value Register */
/* Read Only */

/****  Bit definition for WDT_REG_CRR register  ****/

/* Counter Restart Register */
#define WDT_CCVR_VALUE_FIELD	(0)
#define WDT_CCVR_VALUE   		((uint16_t)0x76 << WDT_CCVR_VALUE_FIELD)

/****  Bit definition for WDT_REG_STAT register  ****/

/* Interrupt Status Register */
#define WDT_STAT_INTER_ENABLE_FIELD	(0)
#define WDT_STAT_INTER_ENABLE   	(1 << WDT_STAT_INTER_ENABLE_FIELD)

/****  Bit definition for WDT_REG_EOI register  ****/
/* Read Only */

/****  Bit definition for WDT_REG_PROT_LEVEL register  ****/

/* {(HC_PROT_LEVEL == 1) ? "read-only" : "read-write" */


/****  Bit definition for WDT_REG_COMP_PARAMS_5 register  ****/
/* Read Only */

/****  Bit definition for WDT_REG_COMP_PARAMS_4 register  ****/
/* Read Only */

/****  Bit definition for WDT_REG_COMP_PARAMS_3 register  ****/
/* Read Only */

/****  Bit definition for WDT_REG_COMP_PARAMS_2 register  ****/
/* Read Only */

/****  Bit definition for WDT_REG_COMP_PARAMS_1 register  ****/
/* Read Only */

/****  Bit definition for WDT_REG_COMP_VERSION register  ****/
/* Read Only */

/****  Bit definition for WDT_REG_COMP_TYPE register  ****/
/* Read Only */

#define TORR_CLR_MASK  	0xfffffff0
#define CR_CLR_MASK  	0xffffffc0


typedef struct {
	uint16_t WDT_PuaseLength;	/* WDT puase length */
	uint16_t WDT_Mode;		/* WDT response mode */
	uint16_t WDT_TimeOutValue;	/* WDT time out period */
} WDT_InitTypeDef;




/**
 * \brief  This function initialize wdt
 * \param  wdt: wdt regs struct
 * \param  WDT_InitTypeDef: contains the configuration for the specified WDT peripheral
 * \retval if wdt==null or WDT_InitStruct==NULL ,return -1 ; otherwise 0;
 */
int AL9000_wdt_init(WDT_TypeDef *wdt,WDT_InitTypeDef *WDT_InitStruct);


#ifdef __cplusplus
}
#endif

#endif
