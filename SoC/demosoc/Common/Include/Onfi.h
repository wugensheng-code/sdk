
#ifndef _AL9000_ONFI_H_
#define _AL9000_ONFI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "smc_drv.h"
#include "nand_drv.h"

#define FAILED			1	/* return failed flag */
#define SUCCESS			0	/* return success flag */


#define ONFI_CMD_PROGRAM_PAGE1						0x80	/* ONFI Read ID Start command */
#define ONFI_CMD_PROGRAM_PAGE2						0x10	/* ONFI Read ID End command */
#define ONFI_CMD_PROGRAM_PAGE_CYCLES				5		/* ONFI Read ID command total address cycles*/
#define ONFI_CMD_PROGRAM_PAGE_END_TIMING 			ONFI_ENDIN_DATA_PHASE/* READ_ID End Cmd Invalid */


#define ONFI_CMD_ERASE_BLOCK1						0x60	/* ONFI Read ID command Start */
#define ONFI_CMD_ERASE_BLOCK2						0xD0	/* ONFI Read ID command End */
#define ONFI_CMD_ERASE_BLOCK_CYCLES					3		/* ONFI Read ID command total address cycles*/
#define ONFI_CMD_ERASE_BLOCK_END_TIMING				ONFI_ENDIN_CMD_PHASE /* READ_ID End Cmd Invalid */


#define ONFI_CMD_PROGRAM_CACHE1						0X80	/* ONFI Read Cache command Start */
#define ONFI_CMD_PROGRAM_CACHE2						0X15	/* ONFI Read Cache command End */
#define ONFI_CMD_PROGRAM_CACHE_CYCLES				5		/* ONFI Read Cache command total address cycles*/
#define ONFI_CMD_PROGRAM_CACHE_END_TIMING			ONFI_ENDIN_DATA_PHASE /* READ_CACHE End Cmd Invalid */




uint8_t Onfi_CmdProgramPage(uint32_t Page, uint32_t Column);
uint8_t Onfi_CmdProgramCachePage(uint32_t Page, uint32_t Column);

uint8_t Onfi_CmdBlockErase(uint32_t Page);

uint8_t Nand_ProgramPage(uint32_t Page, uint32_t Column, uint8_t *Buf, Nand_TypeDef *nandSize);
uint8_t Nand_ProgramPage_EOob(uint32_t Page, uint32_t Column, uint8_t *Buf, Nand_TypeDef *nandSize);
uint8_t Nand_ProgramPage_HwEcc(uint32_t Page, uint32_t Column, uint8_t *Buf, Nand_TypeDef *nandSize);







#ifdef __cplusplus
}
#endif

#endif
