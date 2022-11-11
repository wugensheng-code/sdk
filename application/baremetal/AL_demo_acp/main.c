/*
 * main.c
 *
 *  Created on: 2022年10月18日
 *      Author: wei.pang
 */


#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "gic.h"
#include "delay.h"
#include "io.h"
#include "type.h"
#include "timer.h"
#include "gic_v3.h"
#include "gic_v3_addr.h"
#include "mm.h"

#define VDMA_BASE_ADDR					0x8000c000
#define PL_BRAM_BASE_ADDR 				0x80010000
#define PL_BRAM_ADDR_DMAC 				0x10000000
#define AXI_DMAC_REG_CTL				0x400
#define AXI_DMAC_REG_FLAGS				0x40c
#define AXI_DMAC_REG_SRC_ADDRESS		0x414
#define AXI_DMAC_REG_DEST_ADDRESS		0x410
#define AXI_DMAC_REG_X_LENGTH			0x418
#define AXI_DMAC_REG_DEST_STRIDE		0x420
#define AXI_DMAC_REG_SRC_STRIDE			0x424
#define AXI_DMAC_REG_Y_LENGTH			0x41c
#define AXI_DMAC_REG_IRQ_MASK
#define AXI_DMAC_REG_TRANSFER_SUBMIT	0x408
#define AXI_DMAC_CTRL_ENABLE			BIT(0)
#define AXI_DMAC_CTRL_DISABLE			0X0
#define AXI_DMAC_TRANSFER_SUBMIT		BIT(0)

static volatile int nexterrirq_happened = 0;



void Al_writeReg(uint32_t Addr,uint32_t Value){
	uint32_t *LocalAddr = (volatile uint32_t *)Addr;
	*LocalAddr =Value;
}
extern unsigned long mmu_at_test(unsigned long addr);

static unsigned long parse_par_el1(unsigned long val)
{
	extern char _text_start[], _text_end[], _rodata_end[];
	extern char _data_start[], _sp[];

	int ret = (val & 1);
	//printf("Address Translation instruction %s \r\n", (ret == 0) ? "successfully" : "failed");
	if (ret)
		return -1;

	unsigned long pa = (val & PAGE_MASK) & 0xffffffffff;
	//printf("par_el1:0x%lx, PA: 0x%lx\r\n", val, pa);

	//printf("  NS=%u\r\n", (val >> 9) & 1);
	//printf("  SH=%u\r\n", (val >> 7) & 3);
	//printf("  ATTR=0x%x\r\n", val >> 56);

	return pa;
}

void nexterrirq_handler(void)
{
	nexterrirq_happened = 1;

	//printf("nexterrirq_handler happened \r\n");
	//printf("nexterrirq test pass\r\n");

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
void PS_To_PL(uint32_t* pSnAddr, uint32_t Count, uint32_t* pDnAddr)
{
    volatile uint32_t i;
    if (Count < 0 || Count == 0) return;
    for (i = 0; i < Count; i++)
    {
        *pDnAddr++ = *pSnAddr++;
    }
}
//static uint32_t __attribute__((aligned(0x1000))) PL_DATA_BUFFER[200];
int main(){
	//uint32_t *DmaProgBuf = 0x61001400;
	//*DmaProgBuf = PL_DATA_BUFFER;
	*(uint32_t *)0xf8800080  = 0x0;
	*(uint32_t *)0xf8802004  = 0x0;
	printf("ACP test  :::: 2022/10/19 \r\n");
	paging_init();
	printf("release GP0 reset  :::: 2022/10/19 \r\n");
	printf("INIT_GP0_RELASE_RESET = %08x\r\n",*(uint32_t*) (0xf8800080));
	printf("INIT_ACP_ENABLE 		 = %08x\r\n",*(uint32_t*) (0xf8802004));
	printf("INIT_ACP_RELASE_RESET = %08x\r\n",*(uint32_t*) (0xf8801070));
	uint32_t Bram_Init[256];
	uint32_t Clear_buffer[256];
	for (uint32_t i = 0 ; i < 128 ; i ++){
		if(i%2 == 0){
			Bram_Init[i] = 0x123456 ;
		}
		else
		{
			Bram_Init[i] = 0x1133bbdd;
		}
	}
	for (uint32_t i = 0 ; i < 256 ; i ++){
		 Clear_buffer[i] = 1;
	}
	PS_To_PL((uint32_t*) Bram_Init, 128, (uint32_t*) PL_BRAM_BASE_ADDR);
	printf("\r\n");
	printf("PS_INIT_PL_BRAM has been completed\r\n");
	printf("PS_INIT_PL_AXI_DMAC  :::: 2022/10/19 \r\n");

	//*(uint32_t *)0xf88060B0 |= 0xc;
	*(uint32_t *)0xf8801070 |= 0x100;
	*(uint32_t *)0xf8801078  = 0x1133;
	*(uint32_t *)0xf8801074  = 0x73f0;
	*(uint32_t *)0xf88061b0  = 0x133;
	*(uint32_t *)0xf8800010  = 0x7;

	//*(uint32_t *)0x61030000 = 222222;
#if 1
	printf("GP0_RELASE_RESET = %08x\r\n",*(uint32_t*) (0xf8800080));
	printf("ACP_ENABLE 		 = %08x\r\n",*(uint32_t*) (0xf8802004));
	printf("ACP_RELASE_RESET = %08x\r\n",*(uint32_t*) (0xf8801070));
	//printf("addr_pl%08x\r\n",&PL_DATA_BUFFER[0]);
	printf("PS_INIT_PL_BRAM .....\r\n");
	//PS_To_PL((uint32_t*) Clear_buffer, 256, (uint32_t*) PL_DATA_BUFFER);
	//PS_To_PL((uint32_t*) Clear_buffer, 64, (uint32_t*) &PL_DATA_BUFFER[0]);

	/*for (uint32_t i = 0 ; i < 1024 ; i ++){
		printf("Bram_Init_data = %d",Bram_Init[i]);
	}*/

	Al_writeReg(VDMA_BASE_ADDR+AXI_DMAC_REG_CTL,AXI_DMAC_CTRL_DISABLE);
	//setting OCM cachable and sharable(iner or outer is same )
	Al_writeReg(VDMA_BASE_ADDR+AXI_DMAC_REG_CTL				,	AXI_DMAC_CTRL_DISABLE	);
	Al_writeReg(VDMA_BASE_ADDR+AXI_DMAC_REG_CTL				,	AXI_DMAC_CTRL_ENABLE	);
	Al_writeReg(VDMA_BASE_ADDR+AXI_DMAC_REG_FLAGS			,	0x2				 		);
	Al_writeReg(VDMA_BASE_ADDR+AXI_DMAC_REG_SRC_ADDRESS		,	0x10000000			 	);
	Al_writeReg(VDMA_BASE_ADDR+AXI_DMAC_REG_DEST_ADDRESS	,	0x6103d000				);
	//Al_writeReg(VDMA_BASE_ADDR+AXI_DMAC_REG_DEST_ADDRESS	,	0x00					);
	Al_writeReg(VDMA_BASE_ADDR+AXI_DMAC_REG_SRC_STRIDE		,	0x0						);
	Al_writeReg(VDMA_BASE_ADDR+AXI_DMAC_REG_X_LENGTH		,	1024					 );
	//Al_writeReg(VDMA_BASE_ADDR+AXI_DMAC_REG_Y_LENGTH		,	0					 	);
	Al_writeReg(VDMA_BASE_ADDR+AXI_DMAC_REG_TRANSFER_SUBMIT		,	AXI_DMAC_TRANSFER_SUBMIT);
	printf("DEST_ADDR = %08x",*(uint32_t*) (0x8000c410));
	printf("DONE_STAT = %08x",*(uint32_t*) (0x8000c428));
	printf("\r\n");
	for (uint32_t i = 0 ; i < 128 ; i ++){
		if((*(uint32_t*) (0x6103D000 + (i*4))) == (*(uint32_t*) (PL_BRAM_BASE_ADDR + (i*4))))
		{
		printf("OCM_BUFFER_data = %08x",*(uint32_t*) (PL_BRAM_BASE_ADDR + (i*4)));
		printf("    ");
		printf("ACP_BUFFER_data = %08x",*(uint32_t*) (0x6103D000 + (i*4)));
		printf("    ");
		printf("addr = %08x",0x6103D000 + (i*4));
		printf("    ");
		printf("__________________________DATA SAME\r\n");
		}
		else{
			printf("OCM_BUFFER_data = %08x",*(uint32_t*) (PL_BRAM_BASE_ADDR + (i*4)));
			printf("ACP_BUFFER_data = %08x",*(uint32_t*) (0x6103D000 + (i*4)));
			printf("addr = %08x",0x6103D000 + (i*4));
			printf("__________________________fail!!!!\r\n");
		}
	}
	/*for (uint32_t i = 0 ; i < 128 ; i ++){
		if((*(uint32_t*) (PL_DATA_BUFFER + i)) == (*(uint32_t*) (PL_BRAM_BASE_ADDR + (i*4)))){
			printf("PL_BRAM_BUFFER_data = %08x",*(uint32_t*) (PL_BRAM_BASE_ADDR + (i*4)));
			//printf("ACP_BUFFER_data = %08x",*(uint32_t*) (0x61001400 + (i*4)));
			printf("ACP_BUFFER_data = %08x",*(uint32_t*) (PL_DATA_BUFFER + i));
			printf("__________________________DATA SAME\r\n");
		}
		else{
			printf("PL_BRAM_BUFFER_data = %08x",*(uint32_t*) (PL_BRAM_BASE_ADDR + (i*4)));
			printf("ACP_BUFFER_data = %08x",*(uint32_t*) (PL_DATA_BUFFER + i));
			printf("__________________________fail!!!!\r\n");
		}
	}*/
	printf("\r\n");
	/*for (uint32_t i = 0 ; i < 128 ; i ++){
			//printf("PL_BRAM_BUFFER_data = %08x",*(uint32_t*) (PL_BRAM_BASE_ADDR + (i*4)));
			//printf("ACP_BUFFER_data = %08x",*(uint32_t*) (0x61001400 + (i*4)));
			printf("ACP_BUFFER_data = %08x",*(uint32_t*) (0x61010100 + (i)));
			printf("__________________________fail!!!!\r\n");

	}*/
	printf("DONE_STAT = %08x\r\n",*(uint32_t*) (0x8000c428));
#endif
	printf("test end");
	return 0;
}

