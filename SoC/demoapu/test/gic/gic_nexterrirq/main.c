/*
 *
 */

#include <stdio.h>
#include "config.h"
#include "gic.h"
#include "delay.h"
#include "io.h"
#include "type.h"
#include "timer.h"
#include "gic_v3.h"
#include "gic_v3_addr.h"
#include "mm.h"

static volatile int nexterrirq_happened = 0;

extern unsigned long mmu_at_test(unsigned long addr);

static unsigned long parse_par_el1(unsigned long val)
{
	extern char _text_start[], _text_end[], _rodata_end[];
	extern char _data_start[], _sp[];

	int ret = (val & 1);
	printf("Address Translation instruction %s \r\n", (ret == 0) ? "successfully" : "failed");
	if (ret)
		return -1;

	unsigned long pa = (val & PAGE_MASK) & 0xffffffffff;
	printf("par_el1:0x%lx, PA: 0x%lx\r\n", val, pa);

	printf("  NS=%u\r\n", (val >> 9) & 1);
	printf("  SH=%u\r\n", (val >> 7) & 3);
	printf("  ATTR=0x%x\r\n", val >> 56);

	return pa;
}

void nexterrirq_handler(void)
{
	nexterrirq_happened = 1;

	printf("nexterrirq_handler happened \r\n");
	printf("nexterrirq test pass\r\n");

	while(1);
}

void main_success(void)
{

	#define TOP_NS__BASE_ADDR			(0xF8800000UL)
	#define TOP_NS__RWL0_RESERVED__ADDR (TOP_NS__BASE_ADDR + 0x010UL)

	#define SUCCESS 					(0x600d600d)

	*(volatile unsigned int *)TOP_NS__RWL0_RESERVED__ADDR = SUCCESS;
}

/*
 * test pass with bitfile: soc_stage7_MIO_tag0728_20220818_apu_jtag_MODE_1_release
 * NPU is not default enabled; enable it;
*/
void mpu_enable(void)
{
	*(volatile unsigned int *)0xF840E004 = 0x01;
	*(volatile unsigned int *)0xF840E010 = 0x01;
	*(volatile unsigned int *)0xF840E018 = 0x09;
}


int main()
{
	printf("test nexterrupt :::: \r\n");
	
	mpu_enable();
	
	request_irq(37, nexterrirq_handler);

	irq_enable();

	paging_init();

	for (int k = 0 ; k < 0x10; k++)
	{
		*(volatile unsigned int *)0x00000000 = k;
		printf("0x%x\r\n", *(volatile unsigned int *)0x0);
	}

	while(1);

	return 0;
}
