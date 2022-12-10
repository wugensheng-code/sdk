/*
 * alfsbl_psu_init_default.c
 *
 *  Created on: Dec 10, 2022
 *      Author: xuqingsong
 */

#include <stdio.h>
#include "psu_init.h"

uint32_t __attribute__((weak)) psu_init(void)
{
	printf("Default psu init\n");
	return 0;
}
