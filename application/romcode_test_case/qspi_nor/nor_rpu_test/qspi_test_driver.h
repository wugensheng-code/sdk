#ifndef _QSPI_TEST_H_
#define _QSPI_TEST_H_

#include "../../../fsbl/driver/qspi/qspi_drv.h"
#include "../../../fsbl/driver/qspi/qspi_flash_drv.h"


uint32_t Test_QspiInit(QspiParams *params);
uint32_t Test_QspiProgram(u32 offset, u8* dest, u32 length);

#endif 
