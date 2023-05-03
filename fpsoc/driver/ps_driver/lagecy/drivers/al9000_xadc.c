/*
 * al9000_xadc.c
 *
 *  Created on: 2022年5月29日
 *      Author: jian.huang
 */
#include "al9000_xadc.h"

/* XADC Read PL Reg */
uint16_t XadcRead(uint8_t Addr)
{
	uint32_t Cmd = READ_CMD << 24 |
				Addr << 16;

	XADC->CMD = Cmd;

	while((XADC->STATUS&0x100));

	return (uint16_t)XADC->DATA;
}

/* XADC Write PL Reg */
/* ??没写进去 */
void XadcWrite(uint8_t Addr, uint16_t Data)
{
	uint32_t Cmd = WRITE_CMD << 24 |
				Addr << 16|
				Data;

	XADC->CMD = Cmd;
}






