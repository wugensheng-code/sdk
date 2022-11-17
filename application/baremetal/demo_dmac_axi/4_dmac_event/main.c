#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nuclei_sdk_soc.h"
#include "al9000_registers.h"

#define vfwp printf
#define REG_WRITE(reg_address, reg_wdata)  *(unsigned int*)(reg_address) = reg_wdata
#define REG_READ(reg_address)  *(unsigned int*)reg_address

#define DMAC_AXI_SECURE__DBGSTATUS__ADDR 0xF8418D00
#define DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR 0xF8418024
#define DMA_LENGTH 4096

static int Src[DMA_LENGTH] __attribute__ ((aligned (32)));
static int Dst[DMA_LENGTH] __attribute__ ((aligned (32)));
static int Mcode[DMA_LENGTH] __attribute__ ((aligned (32)));

int main()
{
 {
volatile unsigned rdata0;
volatile unsigned rdata1;
volatile unsigned rdata2;
volatile unsigned rdata_cpc0;
volatile unsigned fail_flag;

    fail_flag =0 ;

// OCM 61000000 - 6103FFFF
//
// OCM 61000000 DV use
// OCM 61010000 DMA Instruction
// OCM 61020000 DMA LD Data
// OCM 61030000 DMA ST Data
//
//
//OCM_L 0x61000000
     #define OCM_L 0x61000000
	uint32_t OCM__BASE1_ADDR = Mcode;
    uint32_t OCM__BASE2_ADDR = Src;
	uint32_t OCM__BASE3_ADDR = Dst;


	uint32_t OCM__BASE1H_ADDR = OCM__BASE1_ADDR >> 16;   //0x00006101
	uint32_t OCM__BASE2H_ADDR = OCM__BASE2_ADDR >> 16;   //0x00006102
	uint32_t OCM__BASE3H_ADDR = OCM__BASE3_ADDR >> 16;   //0x00006103


//set thread0 and start 61010020
 	uint32_t DMAgo_0 = ((uint32_t)Mcode << 16) | 0x2000A0; // secure, channel 0
 	uint32_t DMAgo_1 = OCM__BASE1H_ADDR;//0x00006101


// real instructions
   // while(event_number < 16){
   // 	DMASEV event_number
   //   event_number = event_number + 1;
   //  }
   int DMA_inst0 = 0x08340034;
   int DMA_inst1 = 0x18341034;
   int DMA_inst2 = 0x28342034;
   int DMA_inst3 = 0x38343034;
   int DMA_inst4 = 0x48344034;
   int DMA_inst5 = 0x58345034;
   int DMA_inst6 = 0x68346034;
   int DMA_inst7 = 0x78347034;

     // write LD data
     REG_WRITE(OCM__BASE2_ADDR+0x00, 0xa5a4f3f2);
     REG_WRITE(OCM__BASE2_ADDR+0x04, 0x12345678);
     REG_WRITE(OCM__BASE2_ADDR+0x08, 0xabcd2468);
     REG_WRITE(OCM__BASE2_ADDR+0x0C, 0xa1b2c3d4);

    if(1){
    // write instructions 
      REG_WRITE(OCM__BASE1_ADDR+0x0 , DMAgo_0);
      REG_WRITE(OCM__BASE1_ADDR+0x4 , DMAgo_1);
      REG_WRITE(OCM__BASE1_ADDR+0x20, DMA_inst0);
      REG_WRITE(OCM__BASE1_ADDR+0x24, DMA_inst1);
      REG_WRITE(OCM__BASE1_ADDR+0x28, DMA_inst2);
      REG_WRITE(OCM__BASE1_ADDR+0x2C, DMA_inst3);
      REG_WRITE(OCM__BASE1_ADDR+0x30, DMA_inst4);
      REG_WRITE(OCM__BASE1_ADDR+0x34, DMA_inst5);
      REG_WRITE(OCM__BASE1_ADDR+0X38, DMA_inst6);
      REG_WRITE(OCM__BASE1_ADDR+0X3C, DMA_inst7);

    rdata0 = REG_READ(DMAC_AXI_SECURE__DBGSTATUS__ADDR);
    vfwp("** DBGSTATUS = %x \n",rdata0);
    vfwp("** Before \n");
    rdata0 = REG_READ(TOP_NS__CFG_CTRL1_DMAC_AXI__ADDR);
    vfwp("** CFG_CTRL1_DMAC_AXI__ADDR = %x \n",rdata0);
    rdata0 = REG_READ(TOP_NS__CFG_CTRL0_DMAC_AXI__ADDR);
    vfwp("** CFG_CTRL0_DMAC_AXI__ADDR = %x \n",rdata0);
    rdata0 = REG_READ(0xF88000A0ULL);
    vfwp("** CFG_CTRL2_DMAC_AXI__ADDR = %x \n",rdata0);

    vfwp("** After \n");
     // set boot_addr
     REG_WRITE(TOP_NS__CFG_CTRL1_DMAC_AXI__ADDR, OCM__BASE1_ADDR);
    //REG_WRITE(TOP_NS__CFG_CTRL1_DMAC_AXI__ADDR, 0);
     rdata0 = REG_READ(TOP_NS__CFG_CTRL1_DMAC_AXI__ADDR);
     vfwp("** CFG_CTRL1_DMAC_AXI__ADDR = %x \n",rdata0);
     // set boot
     REG_WRITE(TOP_NS__CFG_CTRL0_DMAC_AXI__ADDR, 0x00000001);
     rdata0 = REG_READ(TOP_NS__CFG_CTRL0_DMAC_AXI__ADDR);
     vfwp("** CFG_CTRL0_DMAC_AXI__ADDR = %x \n",rdata0);
     
     // reset dmac
     REG_WRITE(CRP__SRST_CTRL1__ADDR, 0X00003270);
     rdata0 = REG_READ(0Xf8801074);
     //vfwp("** damc_SOFT_RESET = %x\n",rdata0);
     REG_WRITE(CRP__SRST_CTRL1__ADDR, 0X00003370);
     rdata0 = REG_READ(0Xf8801074);
    // vfwp("** damc_SOFT_RESET = %x\n",rdata0);

     // view CPC0
     rdata_cpc0 = REG_READ(0xF8418104);
     vfwp("** CPC0 = %x \n",rdata_cpc0);
     // view DPC
     rdata0 = REG_READ(0xF8418004);
     vfwp("** DPC = %x \n",rdata0);
     // view CSR0
    // while (REG_READ(0xF8418100)=0){
     //}
     rdata0 = REG_READ(0xF8418100);
     vfwp("** CSR0 = %x \n",rdata0);

    // wait for dmac process until event status = FFFF
    while(1){
    rdata0 = REG_READ(DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR);
    vfwp("** DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR = %x\n",rdata0);
    if(rdata0 == 0x0000FFFF) break;
    }

     if (rdata_cpc0 == (OCM__BASE1_ADDR+0X3C + 0X4) ) { // OCM__BASE1_ADDR+0X3C + 0X4
        vfwp("** SUCCESS !!! %x",0);
     }
   }
  }

}
