/*
 * alfsbl_main.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */


#include <stdio.h>
#include <stdint.h>


int main(void)
{

    *(volatile uint32_t *)(0xf88030c0) = 3;
    *(volatile uint32_t *)(0xf88030c4) = 3;
    *(volatile uint32_t *)(0xf8803414) = 1;

	printf("fpsoc branch hello world\r\n");


	return 0;
}




