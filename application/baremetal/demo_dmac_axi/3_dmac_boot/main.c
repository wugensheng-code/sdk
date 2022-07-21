#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nuclei_sdk_soc.h"
#include "al9000_registers.h"

#define TUBE_ADDRESS ((volatile char *) 0x80000000u)

#define vfwp printf
#define REG_WRITE(reg_address, reg_wdata)  *(unsigned int*)(reg_address) = reg_wdata
#define REG_READ(reg_address)  *(unsigned int*)reg_address

#define DMAC_AXI_SECURE__DBGSTATUS__ADDR 0xF8418D00
#define DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR 0xF8418024

int main()
{
		uint32_t midr;
		uint64_t cpunum = 0;
		uint64_t mpidr;

		#ifdef _AARCH_64
		uint64_t sctlr_el3;

		asm volatile("mrs %0, midr_el1":"=r"(midr)::"memory");
		asm volatile("mrs %0, mpidr_el1":"=r"(mpidr)::"memory");
		#endif

		#ifdef _AARCH_32
		uint32_t cpunum = 0;
		uint32_t mpidr;
		asm volatile("mrc p15,0,%0,c0,c0,0":"=r"(midr)::"memory");
		asm volatile("mrc p15,0,%0,c0,c0,5":"=r"(mpidr)::"memory");
		#endif

    cpunum = mpidr & 0x00ff;
    vfwp("%d",cpunum);
    if (1) {
volatile unsigned rdata0;
volatile unsigned rdata1;
volatile unsigned rdata2;
volatile unsigned fail_flag;

    fail_flag =0 ;

// OCM 61000000 - 6103FFFF
//
// 
// OCM 61000000 DV use
// OCM 61010000 DMA Instruction
// OCM 61020000 DMA LD Data
// OCM 61030000 DMA ST Data
//
//
//OCM_L 0x61000000
     #define OCM_L 0x61000000
     int OCM__BASE1_ADDR = OCM_L+0x00010000;
     int OCM__BASE2_ADDR = OCM_L+0x00020000;
     int OCM__BASE3_ADDR = OCM_L+0x00030000;

     int OCM__BASE1H_ADDR = OCM__BASE1_ADDR>>16;   //0x00006101
     int OCM__BASE2H_ADDR = OCM__BASE2_ADDR>>16;   //0x00006102
     int OCM__BASE3H_ADDR = OCM__BASE3_ADDR>>16;   //0x00006103


//set thread0 and start 61010020
   int DMAgo_0 = 0x002000A0; // secure, channel 0
   int DMAgo_1 = OCM__BASE1H_ADDR;//0x00006101


// real instructions
   // MOV CCR 0x00824209
   // MOV SAR OCM_BASE2_ADDR
   // MOV DAR OCM__BASE3_ADDR
   // LD[S]
   // RMB
   // ST[S]
   // SEV 2 ( set event[2] = 1)
   // END
   int DMA_inst0 = 0x420901bc;
   int DMA_inst1 = 0x00bc0082;
   int DMA_inst2 = OCM__BASE2_ADDR;
   int DMA_inst3 = 0x000002bc;
   int DMA_inst4 = 0x12050000+OCM__BASE3H_ADDR;
   int DMA_inst5 = 0x10341309;
   int DMA_inst6 = 0x00000000;


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
    // rdata0 = REG_READ(0Xf8801074);
    // vfwp("** damc_SOFT_RESET = %x\n",rdata0);
     REG_WRITE(CRP__SRST_CTRL1__ADDR, 0X00003370);
     //rdata0 = REG_READ(0Xf8801074);
     //vfwp("** damc_SOFT_RESET = %x\n",rdata0);

     // view CPC0
     rdata0 = REG_READ(0xF8418104);
     vfwp("** CPC0 = %x \n",rdata0);
     // view DPC
     rdata0 = REG_READ(0xF8418004);
     vfwp("** DPC = %x \n",rdata0);
     // view CSR0
    // while (REG_READ(0xF8418100)=0){
     //}
    // rdata0 = REG_READ(0xF8418100);
     //vfwp("** CSR0 = %x \n",rdata0);

    // wait for dmac process until event status = 4
    while(1){
    rdata0 = REG_READ(DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR);
    vfwp("** DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR = %x\n",rdata0);
    if(rdata0 == 0x00000004) break;
    }


     //check the store data ,compare to the load data
     rdata1 = REG_READ(OCM__BASE2_ADDR);
     rdata2 = REG_READ(OCM__BASE3_ADDR);
     vfwp("** OCM_ld_data 0x0 = %x \n",rdata1);
     vfwp("** OCM_st_data 0x0 = %x\n",rdata2);

     if (rdata2 == rdata1) {
       //fail_flag = 0;
     }
     else{
       fail_flag = 1;
     }

     rdata1 = REG_READ(OCM__BASE2_ADDR+0x4);
     rdata2 = REG_READ(OCM__BASE3_ADDR+0x4);
     vfwp("** OCM_ld_data 0x4 = %x\n",rdata1);
     vfwp("** OCM_st_data 0x4 = %x\n",rdata2);

     if (rdata2 == rdata1) {
       //fail_flag = 0;
     }
     else{
       fail_flag = 1;
     }

     rdata1 = REG_READ(OCM__BASE2_ADDR+0x8);
     rdata2 = REG_READ(OCM__BASE3_ADDR+0x8);
     vfwp("** OCM_ld_data 0x8 = %x\n",rdata1);
     vfwp("** OCM_st_data 0x8 = %x\n",rdata2);

     if (rdata2 == rdata1) {
       //fail_flag = 0;
     }
     else{
       fail_flag = 1;
     }

     rdata1 = REG_READ(OCM__BASE2_ADDR+0xC);
     rdata2 = REG_READ(OCM__BASE3_ADDR+0xC);
     vfwp("** OCM_ld_data 0xC = %x\n",rdata1);
     vfwp("** OCM_st_data 0xC = %x\n",rdata2);

     if (rdata2 == rdata1) {
       //fail_flag = 0;
     }
     else{
       fail_flag = 1;
     }

     if (fail_flag == 0) {
        vfwp("** SUCCESS !!! %x",0);
     }
     else{
        vfwp("** FAIL !!! %x\n",0);
     }
   }
  }

}
