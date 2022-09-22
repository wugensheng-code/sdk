#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nuclei_sdk_soc.h"
#include "al9000_registers.h"
extern int PL330_Init();
#define TUBE_ADDRESS ((volatile char *) 0x80000000u)
#define REG_READ(reg_address)  *(unsigned int*)reg_address

#define DMAC_AXI_SECURE__DBGSTATUS__ADDR 0xF8418D00
#define DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR 0xF8418024
void Data_cheak(uint32_t Saddr, uint32_t Eaddr, uint32_t count) {
	volatile uint32_t i = 0;
	for (i = 0; i < count; i++) {
		if ((*(uint32_t*)(Saddr + i + 4)) == (*(uint32_t*)(Eaddr + i + 4)))
		{
			printf("START_DATA = %08x\r\n", *(uint32_t*)(Saddr + i + 4));
			printf("END_DATA = %08x\r\n", *(uint32_t*)(Eaddr + i + 4));
			printf("check data is right\r\n");
		}
		else {
			printf("START_DATA = %08x\r\n", *(uint32_t*)(Saddr + i + 4));
			printf("END_DATA = %08x\r\n", *(uint32_t*)(Eaddr + i + 4));
			printf("check data is fail please check you dma set data\r\n");
		}

		printf("\r\n");
	}
}
uint8_t Load_Inst2MEM(uint32_t* INST, uint32_t Count, uint32_t* MEMAddr) {
	volatile uint32_t i;
	if (Count < 0 || Count == 0) {
		return 0;
	}
	for (i = 0; i < Count; i++) {
		*MEMAddr++ = *INST++;
	}
	return 1;
}
uint8_t Load_data2MEM(uint32_t* data, uint32_t Count, uint32_t* MEMAddr) {
	volatile uint32_t i;
	if (Count < 0 || Count == 0) {
		return 0;
	}
	for (i = 0; i < Count; i++) {
		*MEMAddr++ = *data;
	}
	return 1;
}
uint8_t Set_Bootaddress(TOPCFG_TypeDef* TOP, uint32_t Channeladdr, uint32_t manageraddr) {
	TOP->CFG_CTRL0_DMAC_AXI = Channeladdr;
	TOP->CFG_CTRL1_DMAC_AXI = manageraddr;
	return 0;
}
void Reset_PL330() {
	*(uint32_t*)(0Xf8801074) = 0X00003270;
	*(uint32_t*)(0Xf8801074) = 0X00003370;
}
void Get_DmacStatus() {

}

static int Src[DMA_LENGTH] __attribute__ ((aligned (32)));
static int Dst[DMA_LENGTH] __attribute__ ((aligned (32)));
static int Mcode[DMA_LENGTH] __attribute__ ((aligned (32)));

int PL330_Init()
{
    uint32_t OCM__BASE1_ADDR = Mcode;
	uint32_t OCM__BASE2_ADDR = Src;
	uint32_t OCM__BASE3_ADDR = Dst;

	uint32_t OCM__BASE1H_ADDR = OCM__BASE1_ADDR >> 16;   //0x00006101
	uint32_t OCM__BASE2H_ADDR = OCM__BASE2_ADDR >> 16;   //0x00006102
	uint32_t OCM__BASE3H_ADDR = OCM__BASE3_ADDR >> 16;   //0x00006103
//set thread0 and start 61010020
	uint32_t DMAgo_0 = 0x002000A0; // secure, channel 0
	uint32_t DMAgo_1 = OCM__BASE1H_ADDR;//0x00006101
// real instructions
   // MOV CCR 0x00824209
   // MOV SAR OCM_BASE2_ADDR
   // MOV DAR OCM__BASE3_ADDR
   // LD[S]
   // RMB
   // ST[S]
   // SEV 2 ( set event[2] = 1)
   // END
	uint32_t DMA_inst0 = 0x420901bc;
	uint32_t DMA_inst1 = 0x00bc0082;
	uint32_t DMA_inst2 = OCM__BASE2_ADDR;
	uint32_t DMA_inst3 = 0x000002bc;
	uint32_t DMA_inst4 = 0x12050000 + OCM__BASE3H_ADDR;
	uint32_t DMA_inst5 = 0x10341309;
	uint32_t DMA_inst6 = 0x00000000;
	// write LD data
	uint32_t data[4] = { 0xa5a4f3f2,0x12345678,0xabcd2468,0xa1b2c3d4 };
	Load_data2MEM((uint32_t*)data, 4, (uint32_t*)OCM__BASE2_ADDR);
	//load instruction in boor address
	uint32_t Manager_inst[2] = { DMAgo_0,DMAgo_1 };
	uint32_t Channel_inst[7] = { DMA_inst0,DMA_inst1,DMA_inst2,DMA_inst3,DMA_inst4,DMA_inst5,DMA_inst6 };
	Load_Inst2MEM((uint32_t*)Manager_inst, 2, (uint32_t*)OCM__BASE1_ADDR);
	Load_Inst2MEM((uint32_t*)Channel_inst, 7, (uint32_t*)(OCM__BASE1_ADDR + 0x20));
	Set_Bootaddress(AL_TOP0, 0x00000001, OCM__BASE1_ADDR);
	// reset dmac
	Reset_PL330();
	uint32_t rdata0 = REG_READ(DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR);
	printf("** DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR = %x\n", rdata0);
	//if (rdata0 == 0x00000004) ;
	Data_cheak(OCM__BASE2_ADDR, OCM__BASE3_ADDR, 4);
}
int main(){
	PL330_Init();
	return 0;
}
