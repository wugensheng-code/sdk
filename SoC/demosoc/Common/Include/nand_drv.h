/*
 * smc_nand_drv.h
 *
 *  Created on: 2022年6月6日
 *      Author: jian.huang
 */
#ifndef _SMC_NAND_DRV_H_
#define _SMC_NAND_DRV_H_

#include "smc_drv.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifndef NULL
#define NULL ((void *) 0)
#endif


#define SMC_MAX_PAGE_SIZE		4096	/* Max page size of NAND flash */
#define SMC_MAX_SPARE_SIZE		256		/* Max spare bytes of a NAND flash page */
#define SMC_MAX_PAGES_PER_BLOCK	512
#define SMC_MAX_BLOCKS			32768	/* Max number of Blocks */
#define SMC_MAX_LUNS		8	/**< Max number of LUNs */


#define NAND_ECC_BLOCK_SIZE		512		/* ECC block size */
#define NAND_ECC_BYTES			3		/* ECC bytes per ECC block */

#define NAND_PAGE_SIZE_512		512		/* Page size 512 */
#define NAND_PAGE_SIZE_1024		1024	/* Page size 1024 */
#define NAND_PAGE_SIZE_2048		2048	/* Page size 2048 */
#define NAND_PAGE_SIZE_4096		4096	/* Page size 4096 */
#define NAND_PAGE_SIZE_8192		8192	/* Page size 8192 */

#define NAND_SPARE_SIZE_8		8		/* Spare bytes size 8 */
#define NAND_SPARE_SIZE_16		16		/* Spare bytes size 16 */
#define NAND_SPARE_SIZE_32		32		/* Spare bytes size 32 */
#define NAND_SPARE_SIZE_64		64		/* Spare bytes size 64 */
#define NAND_SPARE_SIZE_128		128		/* Spare bytes size 128 */
#define NAND_SPARE_SIZE_256		256		/* Spare bytes size 256 */

#define NAND_FLASH_WIDTH_8		0		/* NAND Flash width 8-bit */
#define NAND_FLASH_WIDTH_16		1		/* NAND Flash width 16-bit */


#define ONFI_AXI_DATA_WIDTH			4		/* AXI bus width */

/* Smc Addr Config */
#define ONFI_END_CMD_NONE			0		/* No End command */

#define ONFI_END_CMD_INVALID		0		/* End command invalid */
#define ONFI_ENDIN_CMD_PHASE		1		/* End command in command phase */
#define ONFI_ENDIN_DATA_PHASE		2		/* End command in data phase */

#define ONFI_PAGE_NOT_VALID		-1		/* Page is not valid in command phase */
#define ONFI_COLUMN_NOT_VALID	-1		/* Column is not valid in command phase */


/*  command  */
#define ONFI_CMD_READ_PARAMETER1					0xEC	/* ONFI read parameter start command */
#define ONFI_CMD_READ_PARAMETER2					ONFI_END_CMD_NONE	/* ONFI read parameter end command */
#define ONFI_CMD_READ_PARAMETER_CYCLES				1		/* ONFI read parameter command send address total cycles*/
#define ONFI_CMD_READ_PARAMETER_END_TIMING			ONFI_END_CMD_INVALID /* read parameter end command invalid */

#define ONFI_CMD_SET_FEATURES1						0xEF	/* ONFI set features start command */
#define ONFI_CMD_SET_FEATURES2						ONFI_END_CMD_NONE	/* ONFI set features end command */
#define ONFI_CMD_SET_FEATURES_CYCLES				1		/* ONFI set features command address total cycles*/
#define ONFI_CMD_SET_FEATURES_END_TIMING			ONFI_END_CMD_INVALID /* set features end command invalid */

#define ONFI_CMD_GET_FEATURES1						0xEE	/* ONFI get features start command */
#define ONFI_CMD_GET_FEATURES2						ONFI_END_CMD_NONE	/* ONFI get features end command */
#define ONFI_CMD_GET_FEATURES_CYCLES				1		/* ONFI get features command address total cycles*/
#define ONFI_CMD_GET_FEATURES_END_TIMING			ONFI_END_CMD_INVALID /* get features end command invalid */

#define ONFI_CMD_READ_PAGE1							0x00	/* ONFI Read ID command Start */
#define ONFI_CMD_READ_PAGE2							0x30	/* ONFI Read ID command End */
#define ONFI_CMD_READ_PAGE_CYCLES					5		/* ONFI Read ID command total address cycles*/
#define ONFI_CMD_READ_PAGE_END_TIMING				ONFI_ENDIN_CMD_PHASE /* READ_ID End Cmd Invalid */

#define ONFI_CMD_PROGRAM_PAGE1						0x80	/* ONFI Read ID Start command */
#define ONFI_CMD_PROGRAM_PAGE2						0x10	/* ONFI Read ID End command */
#define ONFI_CMD_PROGRAM_PAGE_CYCLES				5		/* ONFI Read ID command total address cycles*/
#define ONFI_CMD_PROGRAM_PAGE_END_TIMING 			ONFI_ENDIN_DATA_PHASE/* READ_ID End Cmd Invalid */

#define ONFI_CMD_RESET1								0xFF	/* ONFI Read ID command Start */
#define ONFI_CMD_RESET2								ONFI_END_CMD_NONE	/* ONFI Read ID command End */
#define ONFI_CMD_RESET_CYCLES						0		/* ONFI Read ID command total address cycles*/
#define ONFI_CMD_RESET_END_TIMING					ONFI_END_CMD_INVALID /* READ_ID End Cmd Invalid */

#define ONFI_CMD_READ_ID1							0x90	/* ONFI Read ID command Start */
#define ONFI_CMD_READ_ID2							ONFI_END_CMD_NONE	/* ONFI Read ID command End */
#define ONFI_CMD_READ_ID_CYCLES						1		/* ONFI Read ID command total address cycles*/
#define ONFI_CMD_READ_ID_END_TIMING					ONFI_END_CMD_INVALID /* READ_ID End Cmd Invalid */

#define ONFI_CMD_READ_STATUS1						0x70	/* ONFI Read Status command Start */
#define ONFI_CMD_READ_STATUS2						ONFI_END_CMD_NONE	/* ONFI Read Status command End */
#define ONFI_CMD_READ_STATUS_CYCLES					0		/* ONFI Read Status command total address cycles*/
#define ONFI_CMD_READ_STATUS_END_TIMING				ONFI_END_CMD_INVALID /* READ_STATUS End Cmd Invalid */

#define ONFI_CMD_ECC_READ_STATUS1						0x7A	/* ONFI Read Status command Start */
#define ONFI_CMD_ECC_READ_STATUS2						ONFI_END_CMD_NONE	/* ONFI Read Status command End */
#define ONFI_CMD_ECC_READ_STATUS_CYCLES					0		/* ONFI Read Status command total address cycles*/
#define ONFI_CMD_ECC_READ_STATUS_END_TIMING				ONFI_END_CMD_INVALID /* READ_STATUS End Cmd Invalid */

/* ONFI Status Register Mask */
#define ONFI_STATUS_FAIL			0x01	/* Status Register : FAIL */
#define ONFI_STATUS_FAILC			0x02	/* Status Register : FAILC */
#define ONFI_STATUS_ARDY			0x20	/* Status Register : ARDY */
#define ONFI_STATUS_RDY				0x40	/* Status Register : RDY */
#define ONFI_STATUS_WP				0x80	/* Status Register : WR */

#define ONFI_STATUS_PAGE_UNCORRECT		0x10	/* SkyHigh Status Register: Page Uncorrectable */

/*GD Ecc Status Table */
#define IO0 0
#define IO3 3
#define IO4 4

#define ZERO_BITS_ERR 			((0 << IO4)|(0 << IO3)|(0 << IO0))
#define ONE_OR_TWO_BITS_ERR		((0 << IO4)|(1 << IO3)|(0 << IO0))
#define TRI_BITS_ERR			((1 << IO4)|(0 << IO3)|(0 << IO0))
#define FOUR_BITS_ERR			((1 << IO4)|(1 << IO3)|(0 << IO0))
#define MUL_BITS_ERR			((0 << IO4)|(0 << IO3)|(1 << IO0))

#define NAND_ECC_STATUS_MASK		(BIT(4) | BIT(3) | BIT(0))
#define NAND_ECC_STATUS_UNCORRECTABLE	BIT(0)
#define NAND_ECC_STATUS_4_6_CORRECTED	BIT(3)
#define NAND_ECC_STATUS_1_3_CORRECTED	BIT(4)
#define NAND_ECC_STATUS_7_8_CORRECTED	(BIT(4) | BIT(3))


#define NAND_COMMAND_PHASE_FLAG	0x00000000		/* Command phase flag */
#define NAND_DATA_PHASE_FLAG	0x00080000		/* Data phase flag */

#define NAND_PAGE_NOT_VALID		-1	/* Page is not valid in command phase */
#define NAND_COLUMN_NOT_VALID	-1	/* Column is not valid in command phase */

#define NANDP_BB_FLAG			0xFF	/* Bad block flag */


#define NAND_WRITE_PROTECTED 2	/* return BlockErase fail because nand write protected */

//#define GOOD_BLOCK 1	/* return BlockErase fail because nand write protected */
//#define BAD_BLOCK  2	/* return BlockErase fail because nand write protected */

#define NO_CLEAR_CS			0	/* return failed flag */
#define CLEAR_CS			1	/* return success flag */

#define NO_ECC_LAST			0	/* return failed flag */
#define ECC_LAST			1	/* return success flag */



#define SMC_WriteReg(reg, value)		REG32_WRITE((reg), (value))
#define SMC_ReadReg(reg)	 				REG32_READ((reg))
#define SMC_Read8BitReg(reg)	 	(*(volatile uint8_t *)(reg))


 /* OneHot is used to check if one and only one bit is set. */
#define OneHot(Value)	(!((Value) & (Value - 1)))


/* */
#define CRC16_INIT			0x4F4E		/* CRC16 initial constant */
#define CRC16_POLYNOM		0x8005		/* CRC16 polynomial */
#define CRC16_LEN			254			/* Onfi CRC16 length */

#define ONFI_PARAM_LEN		256			/* Onfi Parameter Page Length */
#define ONFI_SIGNATURE_LEN	4			/* Onfi Signature Length */

/* */
#define NAND_FEAT_POS		6
#define DATA_PER_PAGE_POS	80
#define SPARE_PER_PAGE_POS	84
#define PAGE_PER_BLOCK_POS	92
#define BLOCKS_PER_UINT_POS	96
#define TOTAL_UINT_POS		100
#define ECC_NUM_POS			112

#define ZoneBitError	0
#define OneBitError		1








enum SMC_ERROR_CODE{
	SmcSuccess = 0,				/* 返回成功 */
	SmcResetErr,				/* 复位失败错误 */ 
	SmcParaBusyStatusErr,		/* Read parameter data happen status reg error */
	SmcCrcErr,					/* Parameter page三次crc都失败出现的错误 */
	SmcParameterOver,			/* 读Parameter Page发现读取出来的数据，表示的page datasize大小远远超出了正常大小 */
	SmcSamsungParamOver,
	SmcHwInitSizeErr,			/* Smc ecc最多能支持的page size的大小为2048.当使用Smc的ecc功能并且nandflash大小超过时出现此错误。*/
	SmcHwInitEccBusyErr,
	SmcHwDisEccBusyErr,
	SmcFeatBusyErr,
	SmcWriteEccFeatErr,			/* 使能On Die Ecc但是使能失败产生的错误 */
	SmcSpareStatusErr,			/* Read spare data happen status reg error */
	SmcBadBlock,				/* 检测到了坏块 */
	SmcReadCmdTimeOutErr,
	SmcHwReadSizeOver,
	SmcCalEccBusyErr,
	SmcEccDataInvalidErr,			/* 读取Smc ecc数据，但是ecc数据被标记为无效 */
	SmcTwoBitsErr,
	SmcMultipleBitsErr,


#if 0
	SmcEccBusyErr0,				/* Nand_EccHwDisable，SMC Ecc busy时间超过5ms产生的错误 */
	SmcEccBusyErr1,				/* Nand_EccHwInit，SMC Ecc busy时间超过5ms产生的错误 */
	SmcFeatBusyErr,				/* GetFeature/SetFeature 5ms超时，发生busy错误 */

	SmcSizeOverErr,
	SmcReadAlignErr1,			/* Nand_ReadPage_HwEcc传递的参数出现错误。未4byte对齐，或者Length=0. */
	SmcBusyTimeOutErr,			/* 调用ReadPage Cmd中判断nandflash为busy并且持续5ms时间。*/
	SmcOnfiStatusErr,			/* ReadPage Cmd之后发送读取状态寄存器的命令 */
	SmcEccBusyErr2,				/* 在数据传输读取结束之后，读取Smc ecc数据，但是ecc busy持续了5ms */

	SmcTwoBitsErr,				/* ecc出现了2bit错误 */
	SmcMultipleBitsErr,			/* Ecc出现了多bit错误 */
	SmcReadAlignErr2,			/* Nand_ReadPage传递的参数出现错误。未4byte对齐，或者Length=0. */
	SmcDataEccErr,				/* On die ecc情况下出现了超出On die ecc纠错位数能力的错误 */
	SmcLongEccSectorErr,		/* samsung flash出现了超出On die ecc纠错位数能力的错误 */
	SmcBadBlock,
	SmcOffsetErr
#endif



};


/*
 * NAND Flash Manufacturer ID Codes
 */
//#define NAND_MFR_TOSHIBA	0x98
//#define NAND_MFR_SAMSUNG	0xec
//#define NAND_MFR_FUJITSU	0x04
//#define NAND_MFR_NATIONAL	0x8f
//#define NAND_MFR_RENESAS	0x07
//#define NAND_MFR_STMICRO	0x20
//#define NAND_MFR_HYNIX		0xad
//#define NAND_MFR_AMD		0x01
//#define NAND_MFR_MACRONIX	0xc2
//#define NAND_MFR_EON		0x92
//#define NAND_MFR_SANDISK	0x45
//#define NAND_MFR_INTEL		0x89
//#define NAND_MFR_ATO		0x9b
//
//#define NAND_MFR_GD			0xC8
//#define NAND_MFR_MICRON		0x2c

/* NAND Flash Manufacturer ID Codes */

//#define NAND_MFR_SAMSUNG	0xec
//#define NAND_MFR_FUJITSU	0x04
//#define NAND_MFR_NATIONAL	0x8f
//#define NAND_MFR_RENESAS	0x07
//#define NAND_MFR_STMICRO	0x20
//#define NAND_MFR_HYNIX		0xad
//#define NAND_MFR_AMD		0x01
//#define NAND_MFR_EON		0x92
//#define NAND_MFR_SANDISK	0x45
//#define NAND_MFR_INTEL		0x89
//#define NAND_MFR_ATO		0x9b
//
//
//#define NAND_MFR_TOSHIBA	0x98
//#define NAND_MFR_MICRON		0x2c
//#define NAND_MFR_WINBOND 	0xEF
//#define NAND_MFR_GIGODEVICE 0xC8
//#define NAND_MFR_MXIC		0xC2



#define NAND_MFR_FORESEE	0xcd

#define NAND_MFR_AMD		0x01
#define NAND_MFR_ATO		0x9b
#define NAND_MFR_ISSI		0x9d
#define NAND_MFR_EON		0x92
#define NAND_MFR_ESMT		0xc8	/* GD == ESMT == ISSI */
#define NAND_MFR_FUJITSU	0x04
#define NAND_MFR_HYNIX		0xad
#define NAND_MFR_INTEL		0x89
#define NAND_MFR_MACRONIX	0xc2
#define NAND_MFR_MICRON		0x2c
#define NAND_MFR_NATIONAL	0x8f
#define NAND_MFR_RENESAS	0x07
#define NAND_MFR_SAMSUNG	0xec
#define NAND_MFR_SANDISK	0x45
#define NAND_MFR_STMICRO	0x20

/* Kioxia is new name of Toshiba memory. */
#define NAND_MFR_TOSHIBA	0x98
#define NAND_MFR_WINBOND	0xef



#define NAND_START_CMD_SHIFT		3	/**< Start command shift */
#define NAND_END_CMD_SHIFT			11	/**< End command shift */
#define NAND_END_CMD_VALID_SHIFT	20	/**< End command valid shift */
#define NAND_ADDR_CYCLES_SHIFT		21	/**< Address cycles shift */
#define NAND_CHIP_ADDR_SHIFT		24	/**< Chip address shift */
#define NAND_ECC_LAST_SHIFT			10	/**< Ecc last shift */
#define NAND_CLEAR_CS_SHIFT			21	/**< clear chip select shift */

#define NAND_BUSY	0
#define NAND_READY	1




uint32_t Csu_NandInit(Nand_TypeDef *nand);
uint32_t Csu_NandRead(uint32_t offset, uint8_t* dest, uint32_t length, Nand_TypeDef *nand);


#ifdef __cplusplus
}
#endif



#endif /* _SMC_NAND_DRV_H_ */



