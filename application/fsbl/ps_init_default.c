/*
 * alfsbl_psu_init_default.c
 *
 *  Created on: Dec 10, 2022
 *      Author: xuqingsong
 */

#include <ps_init.h>
#include <stdio.h>

uint32_t __attribute__((weak)) ps_init(void)
{
	printf("Default ps init\r\n");
	return 0;
}
