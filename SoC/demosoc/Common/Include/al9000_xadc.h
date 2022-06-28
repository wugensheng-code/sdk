/*
 * al9000_xadc.h
 *
 *  Created on: 2022年5月29日
 *      Author: jian.huang
 */

#ifndef _AL9000_XADC_H_
#define _AL9000_XADC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "demosoc.h"

/****  Bit definition for XADC_CFG_R register  ****/
#define ENABLE_ADC_PS_ACCESS	1

#define JTAG_W_FALL_EDGE 0	/*  */
#define JTAG_W_RISE_EDGE 1	/*  */

#define JTAG_R_FALL_EDGE 0	/*  */
#define JTAG_R_RISE_EDGE 1	/*  */

#define H_XADC_PCLK		0	/* 1/2 of xadc_ps_pclk clock frequency */
#define Q_XADC_PCLK  	1	/* 1/4 of xadc_ps_pclk clock frequency */
#define E_XADC_PCLK  	2	/* 1/8 of xadc_ps_pclk clock frequency */
#define QQ_XADC_PCLK  	3	/* 1/16 of xadc_ps_pclk clock frequency */



/****  Bit definition for XADC_INTR_R register  ****/


/****  Bit definition for XADC_INTR_MASK_R register  ****/

#define ENABLE_CMD_FIFO_GTH_INTER		0	/* Enable command FIFO Greater level threshold interrupt */
#define MASK_CMD_FIFO_GTH_INTER			1	/* Mask command FIFO Greater level threshold interrupt */

#define ENABLE_DATA_FIFO_GTH_INTER		0	/* Enable Data FIFO greater level threshold interrupt */
#define MASK_DATA_FIFO_GTH_INTER		1	/* Mask Data FIFO greater level threshold interrupt */

#define ENABLE_CMD_FIFO_LTH_INTER		0	/* Enable command FIFO less level threshold interrupt */
#define MASK_CMD_FIFO_LTH_INTER			1	/* Mask command FIFO less level threshold interrupt */

#define ENABLE_DAYA_FIFO_LTH_INTER		0	/* Enable Data FIFO less level threshold interrupt */
#define MASK_DAYA_FIFO_LTH_INTER		1	/* Mask Data FIFO less level threshold interrupt */

#define ENABLE_SAMPLE_INTER				0	/* Enable sample interrupt from XADC */
#define MASK_SAMPLE_INTER				1	/* Mask sample interrupt from XADC */


/****  Bit definition for XADC_MSTS_R register  ****/

/****  Bit definition for XADC_CMD_FIFO_R register  ****/

/****  Bit definition for XADC_DATA_FIFO_R register  ****/


/****  Bit definition for XADC_MCTRL_R register  ****/

#define FLUSH_DAYA_FIFO		1	/* Flush Data FIFO */

#define FLUSH_CMD_FIFO		1	/* Flush Cmd FIFO */







/* ADC IO Mux Select Offset */
#define OFFSET_1ST 		0
#define OFFSET_2ND 		8


/* ADC IO Mux Select */
#define IO_MUX_0		0
#define IO_MUX_1		1
#define IO_MUX_2		2
#define IO_MUX_3		3
#define IO_MUX_4		4
#define IO_MUX_5		5
#define IO_MUX_6		6
#define IO_MUX_7		7
#define IO_MUX_8		8
#define IO_MUX_9		9
#define IO_MUX_10		10
#define IO_MUX_11		11
#define IO_MUX_12		12
#define IO_MUX_13		13
#define IO_MUX_14		14
#define IO_MUX_15		15
#define IO_MUX_16		16

/*** ADC_CONFIG0 ***/

/* These bits indicate the gap between the channel select to start of convertion,
shoulde be set during PHY External MUX mode.
The value is related to the latency of external mux IO and analog input IO */
#define PHY_EXTERNAL_MUX_GAP_OFFSET	12



/* Using REG soc signal or PIB soc signal Offset */
#define REG_PIB_SEL_OFFSET	11

/* Using REG soc signal or PIB soc signal */
#define REG_SEL			0
#define PIB_SEL			1




/* MUX ENABLE Offset */
#define MUX_ENBALE_OFFSET	10

/* enable/disable Mux */
#define MUX_DISABLE			0
#define MUX_ENABLE			1

/* phy-external multiplexer mode Offset */
#define PHY_EXTERNAL_M_OFFSET	9

/* enable/disable phy-external multiplexer mode */
#define NORMAL_MODE			0
#define PHY_EXTERNAL_MODE	1


/* Single/Differential mode Offset */
#define INPUT_M_OFFSET	8

/* Single/Differential mode Select */
#define SINGLE_MODE			0
#define DIFF_MODE			1


/* resolution selection Offset */
#define RES_OFFSET 6

/* resolution selection */
#define RES_6BIT	0
#define RES_8BIT	1
#define RES_10BIT	2
#define RES_12BIT	3

/* reference voltag selection Offset */
#define REF_OFFSET 5

/* reference voltag selection */
#define REF_VREF		0
#define REF_INTER_REF	1

/* bandgap selection Offset */
#define BAG_SEL_OFFSET 4

/* bandgap selection */
#define BAG_POWERDOWN		0
#define BAG_ACTIVE			1

/* ADC Software Reset Offset */
#define SW_RESET_OFFSET 0

/* ADC Software Reset */
#define ADC_SW_RESET	1
#define ADC_RELEASE		0


/*** ADC_CONFIG1 ***/
#define INTR_ERROR_OFFSET 		11

#define INTR_LTH_OFFSET			10

#define INTR_GTH_OFFSET			9

#define INTR_DONE_OFFSET		8

#define INTR_ERROR_MASK_OFFSET	3

#define INTR_LTH_MASK_OFFSET	2

#define INTR_GTH_MASK_OFFSET	1

#define INTR_DONE_MASK_OFFSET	0

/*** ADC_CONFIG2 ***/
#define ADC_CLK_DIV_OFFSET		8

#define ADC_CLK_DIV_2	2
#define ADC_CLK_DIV_3	3
#define ADC_CLK_DIV_4	4
#define ADC_CLK_DIV_5	5
#define ADC_CLK_DIV_6	6
#define ADC_CLK_DIV_7	7

#define ADC_CLK_GATE_OFFSET		6

#define ADC_CLK_SEL_OFFSET		4

#define OSC_CLK_44M			0
#define ADC_PS_CLK_50M		1
#define ADC_PL_CLK_70M		2


#define ADC_DISLVL_OFFSET	1

#define ADC_ENABLE_OFFSET	0


/*** ADC_CONFIG3 ***/
#define REG_ADC_SOC_OFFSET		12

#define START_CONVER	1
#define NO_OPERATION	0


#define EXTERNAL_CH_SEL_OFFSET	8

#define CH_SEL_OFFSET			4

#define MODE_SEL_OFFSET			0

#define SIGNAL_CH_MODE			0
#define SIGNAL_PASS_MODE		1
#define CONTINUE_MODE			2

/* ADC PL Cmd  */
#define READ_CMD	0x78
#define WRITE_CMD	0x77


/* ADC PL Addr  */

#define		ADC_VC0		0x40
#define		ADC_VC1		0x42
#define		ADC_VC2		0x44
#define		ADC_VC3		0x46
#define		ADC_VC4		0x48
#define		ADC_VC5		0x4a
#define		ADC_VC6		0x4c
#define		ADC_VC7		0x4e


#define		ADC_VER0		0x50
#define		ADC_VER1		0x52
#define		ADC_FLAG		0x5E

#define		ADC_CHANNEL_0_1		0x60
#define		ADC_CHANNEL_2_3		0x62
#define		ADC_CHANNEL_4_5		0x64
#define		ADC_CHANNEL_6_7		0x66

#define		ADC_CONFIG0		0x68
#define		ADC_CONFIG1		0x6A
#define		ADC_CONFIG2		0x6C
#define		ADC_CONFIG3		0x6E

#define		ADC_CH0_LTH		0x80
#define		ADC_CH1_LTH		0x82
#define		ADC_CH2_LTH		0x84
#define		ADC_CH3_LTH		0x86
#define		ADC_CH4_LTH		0x88
#define		ADC_CH5_LTH		0x8A
#define		ADC_CH6_LTH		0x8C
#define		ADC_CH7_LTH		0x8E

#define		ADC_CH0_GTH		0x90
#define		ADC_CH1_GTH		0x92
#define		ADC_CH2_GTH		0x94
#define		ADC_CH3_GTH		0x96
#define		ADC_CH4_GTH		0x98
#define		ADC_CH5_GTH		0x9A
#define		ADC_CH6_GTH		0x9C
#define		ADC_CH7_GTH		0x9E

#if 1
/**
  * @brief XADC-AL9000
  */
typedef struct {
	__IOM  	uint32_t 		CFG;
	__IOM  	uint32_t 		INTR;
	__IOM  	uint32_t 		INTR_MASK;
	__IM  	uint32_t 		STATUS;
	__OM  	uint32_t 		CMD;
	__IM  	uint32_t 		DATA;
	__IOM  	uint32_t 		CTRL;
} XADC_TypeDef;

typedef union
{
	uint32_t reg;
	struct {
		uint32_t ENABLE_PS_ADC	:1;
		uint32_t 				:2;
		uint32_t READ_IGAP		:5;
		uint32_t CMD_FIFOTH		:4;
		uint32_t DATA_FIFOOTH	:4;
		uint32_t 				:2;
		uint32_t JTAG_WEDGE		:1;
		uint32_t JTAG_REDGE		:1;
		uint32_t 				:2;
		uint32_t TCK_RATE		:2;
		uint32_t 				:3;
		uint32_t IGAP			:5;
	} bitfiled;
}CFG_TypeDef;


typedef union
{
	uint32_t reg;
	struct {
		uint32_t 				:20;
		uint32_t CMD_FIFO_GTH	:1;
		uint32_t DATA_FIFO_LTH	:1;
		uint32_t CMD_FIFO_LTH	:1;
		uint32_t DATA_FIFO_GTH	:1;
		uint32_t 				:7;
		uint32_t INTR			:1;
	} bitfiled;
}INTR_TypeDef;



typedef union
{
	uint32_t reg;
	struct {
		uint32_t 					:20;
		uint32_t MASK_CMD_FIFO_GTH	:1;
		uint32_t MASK_DATA_FIFO_LTH	:1;
		uint32_t MASK_CMD_FIFO_LTH	:1;
		uint32_t MASK_DATA_FIFO_GTH	:1;
		uint32_t 					:7;
		uint32_t MASK_INTR			:1;
	} bitfiled;
}INTR_MASK_TypeDef;


typedef union
{
	uint32_t reg;
	struct {
		uint32_t 				:12;
		uint32_t CMD_FIFO_LVL	:4;
		uint32_t DATA_FIFO_LVL	:4;
		uint32_t CMD_FIFO_F		:1;
		uint32_t CMD_FIFO_E		:1;
		uint32_t DATA_FIFO_F	:1;
		uint32_t DATA_FIFO_E	:1;
		uint32_t 				:7;
		uint32_t MASK_INTR		:1;
	} bitfiled;
}STATUS_TypeDef;




typedef union
{
	uint32_t reg;
	struct {
		uint32_t 					:27;
		uint32_t RESET				:1;
		uint32_t 					:2;
		uint32_t DATA_FIFO_FLUSE	:1;
		uint32_t CMD_FIFO_FLUSE		:1;
	} bitfiled;
}CTRL_TypeDef;


#define WriteReg(reg,value)	\
do{		\
	*(volatile uint32_t *)reg = value;	\
}while(0)


#define ReadReg(reg)	 (*(volatile uint32_t *)reg)


uint16_t XadcRead(uint8_t Addr);

void XadcWrite(uint8_t Addr, uint16_t Data);

uint8_t XadcInit(void);

#endif



#ifdef __cplusplus
}
#endif

#endif /* _AL9000_XADC_H_ */
