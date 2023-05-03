#ifndef _QSPI_TEST_H_
#define _QSPI_TEST_H_

#include "qspi_drv.h"
#include "qspi_flash_drv.h"



#define PAGE_PROGRAM        (0X02)
#define PAGE_PROGRAM_4ADDR  (0X12)

#define READ_STATUS_REG1    (0x05)
#define READ_STATUS_REG2    (0x35)
#define READ_STATUS_REG3    (0x15)

#define WRITE_STATUS_REG1    (0x01)
#define WRITE_STATUS_REG2    (0x31)
#define WRITE_STATUS_REG3    (0x11)

#define CHIP_ERASE          (0xC7)

#define WRITE_ENABLE        (0x06)




#define SW_DIE_SEL_CMD          0xC2
#define WRITE_STATUS1_REG_CMD   0x01

#define DEEP_POWER_DOWN_CMD     0xB9

#define RELEASE_POWER_DOWN_CMD  0xAB

#define ENABLE_QPI_CMD          0x38

#define ENABLE_4BYTE_CMD        0xB7
#define DISABLE_4BYTE_CMD       0xE9

#define FAST_READ_QUAD_OP          0x6B
#define FAST_READ_QUAD_OP_4ADDR    0x6C
#define FAST_READ_QUAD_IO          0xEB
#define FAST_READ_QUAD_IO_4ADDR    0xEC

#define FAST_READ_DUAL_OP          0x3B
#define FAST_READ_DUAL_OP_4ADDR    0x3C
#define FAST_READ_DUAL_IO          0xBB
#define FAST_READ_DUAL_IO_4ADDR    0xBC

#define READ_SFDP_CMD              0x5A















#define TIMER_OUT_ERR10 9
#define TIMER_OUT_ERR11 8
#define TIMER_OUT_ERR12 7











//uint32_t Test_QspiInit(QspiParams *params);
uint32_t Test_QspiProgram(u32 offset, u8* dest, u32 length);

uint8_t EnableWrite(void);


 void Nor_SendCmd(uint8_t cmd);


#endif 
