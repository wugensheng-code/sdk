/*
 * alfsbl_qspi.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#include <stdio.h>

#include "demosoc.h"

#include "alfsbl_qspi.h"
#include "alfsbl_misc.h"

#include "driver/qspi/qspi_drv.h"
#include "driver/qspi/qspi_flash_drv.h"


uint32_t AlFsbl_Qspi24Init(void)
{
	QspiParams qspi_params;
	Csu_QspiInit(&qspi_params);

	return 0;
}


uint32_t AlFsbl_Qspi24Copy(PTRSIZE SrcAddress, PTRSIZE DestAddress, uint32_t Length)
{
	uint32_t ret;
	ret = Csu_QspiRead(SrcAddress - IMAGE_FLASH_OFFSET, (uint8_t *)(DestAddress), Length);

	return ret;
}


uint32_t AlFsbl_Qspi24Release(void)
{
	return 0;
}
