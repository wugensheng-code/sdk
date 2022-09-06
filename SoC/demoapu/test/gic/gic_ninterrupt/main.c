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


static volatile int ninterrupt_happened = 0;

extern unsigned long mmu_at_test(unsigned long addr);

static unsigned long parse_par_el1(unsigned long val)
{
	extern char _text_start[], _text_end[], _rodata_end[];
	extern char _data_start[], _sp[];

	int ret = val & 1;
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

void ninterrupt_handler(void)
{
	ninterrupt_happened = 1;

	printf("ninterrupt_handler happened \r\n");
	printf("ninterrupt test pass\r\n");
	while(1);
}

void main_success(void)
{

	#define TOP_NS__BASE_ADDR			(0xF8800000UL)
	#define TOP_NS__RWL0_RESERVED__ADDR (TOP_NS__BASE_ADDR + 0x010UL)

	#define SUCCESS (0x600d600d)

	*(volatile unsigned int *)TOP_NS__RWL0_RESERVED__ADDR = SUCCESS;
}

int main()
{
	printf("test ninterrupt :::: \r\n");

	request_irq(36, ninterrupt_handler);

	irq_enable();

	paging_init();

	unsigned long v = mmu_at_test((unsigned long)0x38300240);
	unsigned long pa = parse_par_el1(v);

	//trigger address is 0x38300240 in ninterrupt_trigger
	ninterrupt_trigger(0x38300240);

	while (!ninterrupt_happened);

	printf("ninterrupt test pass\r\n");

	return 0;
}
