#ifndef _QSPI_TEST_H_
#define _QSPI_TEST_H_

#include "qspi_drv.h"
#include "qspi_flash_drv.h"



#define PAGE_PROGRAM        (0X02)
#define PAGE_PROGRAM_4ADDR  (0X12)

#define READ_STATUS_REG1    (0x05)
#define READ_STATUS_REG2    (0x35)
#define READ_STATUS_REG3    (0x15)

#define CHIP_ERASE          (0xC7)

#define WRITE_ENABLE        (0x06)



//uint32_t Test_QspiInit(QspiParams *params);
uint32_t Test_QspiProgram(u32 offset, u8* dest, u32 length);

#endif 
