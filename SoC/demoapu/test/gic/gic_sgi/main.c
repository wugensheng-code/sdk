#include <stdio.h>
#include "config.h"
#include "gic.h"
#include "delay.h"
#include "type.h"
#include "gic_v3_addr.h"

volatile int gic_sgi_handler_0_happened = 0;
volatile int gic_sgi_handler_1_happened = 0;
volatile int gic_sgi_handler_2_happened = 0;
volatile int gic_sgi_handler_3_happened = 0;
volatile int gic_sgi_handler_4_happened = 0;
volatile int gic_sgi_handler_5_happened = 0;
volatile int gic_sgi_handler_6_happened = 0;
volatile int gic_sgi_handler_7_happened = 0;
volatile int gic_sgi_handler_8_happened = 0;
volatile int gic_sgi_handler_9_happened = 0;
volatile int gic_sgi_handler_10_happened = 0;
volatile int gic_sgi_handler_11_happened = 0;
volatile int gic_sgi_handler_12_happened = 0;
volatile int gic_sgi_handler_13_happened = 0;
volatile int gic_sgi_handler_14_happened = 0;
volatile int gic_sgi_handler_15_happened = 0;

void gic_sgi_handler_0(void)
{
	printf("sgi 0 happended \r\n");
	gic_sgi_handler_0_happened = 1;
}

void gic_sgi_handler_1(void)
{
	printf("sgi 1 happended \r\n");
	gic_sgi_handler_1_happened = 1;
}

void gic_sgi_handler_2(void)
{
	printf("sgi 2 happended \r\n");
	gic_sgi_handler_2_happened = 1;
}

void gic_sgi_handler_3(void)
{
	printf("sgi 3 happended \r\n");
	gic_sgi_handler_3_happened = 1;
}

void gic_sgi_handler_4(void)
{
	printf("sgi 4 happended \r\n");
	gic_sgi_handler_4_happened = 1;
}

void gic_sgi_handler_5(void)
{
	printf("sgi 5 happended \r\n");
	gic_sgi_handler_5_happened = 1;
}

void gic_sgi_handler_6(void)
{
	printf("sgi 6 happended \r\n");
	gic_sgi_handler_6_happened = 1;
}

void gic_sgi_handler_7(void)
{
	printf("sgi 7 happended \r\n");
	gic_sgi_handler_7_happened = 1;
}

void gic_sgi_handler_8(void)
{
	printf("sgi 8 happended \r\n");
	gic_sgi_handler_8_happened = 1;
}

void gic_sgi_handler_9(void)
{
	printf("sgi 9 happended \r\n");
	gic_sgi_handler_9_happened = 1;
}

void gic_sgi_handler_10(void)
{
	printf("sgi 10 happended \r\n");
	gic_sgi_handler_10_happened = 1;
}

void gic_sgi_handler_11(void)
{
	printf("sgi 11 happended \r\n");
	gic_sgi_handler_11_happened = 1;
}

void gic_sgi_handler_12(void)
{
	printf("sgi 12 happended \r\n");
	gic_sgi_handler_12_happened = 1;
}

void gic_sgi_handler_13(void)
{
	printf("sgi 13 happended \r\n");
	gic_sgi_handler_13_happened = 1;

}

void gic_sgi_handler_14(void)
{
	printf("sgi 14 happended \r\n");
	gic_sgi_handler_14_happened = 1;
}

void gic_sgi_handler_15(void)
{
	printf("sgi 15 happended \r\n");
	gic_sgi_handler_15_happened = 1;
}


int main()
{
	//trigger sgi-0
	request_irq(0, gic_sgi_handler_0);

	request_irq(1, gic_sgi_handler_1);

	request_irq(2, gic_sgi_handler_2);

	request_irq(3, gic_sgi_handler_3);

	request_irq(4, gic_sgi_handler_4);

	request_irq(5, gic_sgi_handler_5);

	request_irq(6, gic_sgi_handler_6);

	request_irq(7, gic_sgi_handler_7);

	request_irq(8, gic_sgi_handler_8);

	request_irq(9, gic_sgi_handler_9);

	request_irq(10, gic_sgi_handler_10);

	request_irq(11, gic_sgi_handler_11);

	request_irq(12, gic_sgi_handler_12);

	request_irq(13, gic_sgi_handler_13);

	request_irq(14, gic_sgi_handler_14);

	request_irq(15, gic_sgi_handler_15);

	irq_enable();

	//set ICC_SGI1R_EL1: 0x03
	asm volatile("ldr x0, =0x3; msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_0_happened);

	asm volatile("ldr x0, =(0x3 | (0x1 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_1_happened);

	asm volatile("ldr x0, =(0x3 | (0x2 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_2_happened);

	asm volatile("ldr x0, =(0x3 | (0x3 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_3_happened);

	asm volatile("ldr x0, =(0x3 | (0x4 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_4_happened);

	asm volatile("ldr x0, =(0x3 | (0x5 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_5_happened);

	asm volatile("ldr x0, =(0x3 | (0x6 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_6_happened);

	asm volatile("ldr x0, =(0x3 | (0x7 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_7_happened);

	asm volatile("ldr x0, =(0x3 | (0x8 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_8_happened);

	asm volatile("ldr x0, =(0x3 | (0x9 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_9_happened);

	asm volatile("ldr x0, =(0x3 | (0xa << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_10_happened);

	asm volatile("ldr x0, =(0x3 | (0xb << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_11_happened);

	asm volatile("ldr x0, =(0x3 | (12 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_12_happened);

	asm volatile("ldr x0, =(0x3 | (13 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_13_happened);

	asm volatile("ldr x0, =(0x3 | (14 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_14_happened);

	asm volatile("ldr x0, =(0x3 | (15 << 24)); msr s3_0_c12_c11_5, x0\r\n" ::: "x0");
	while (!gic_sgi_handler_15_happened);

    return 0;
}
