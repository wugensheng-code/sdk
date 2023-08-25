/*
 * alfsbl_psu_init_default.c
 *
 *  Created on: Dec 10, 2022
 *      Author: xuqingsong
 */

#include <stdio.h>

int __attribute__((weak)) Soc_PlatInit(void)
{
	printf("Default ps init\r\n");
	return 0;
}
