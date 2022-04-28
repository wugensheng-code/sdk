//-----------------------------------------------------------------------------
// The confidential and proprietary information contained in this file may
// only be used by a person authorised under and to the extent permitted
// by a subsisting licensing agreement from ARM Limited or its affiliates.
//
//            (C) COPYRIGHT 2013-2016 ARM Limited or its affiliates.
//                ALL RIGHTS RESERVED
//
// This entire notice must be reproduced on all copies of this file
// and copies of this file may only be made by a person if such person is
// permitted to do so under the terms of a subsisting license agreement
// from ARM Limited or its affiliates.
//
//      Release Information : MERCURY-r0p2-00rel0
//
//------------------------------------------------------------------------------
// Purpose:
//   Output a character to the tube
//------------------------------------------------------------------------------
// Description:  keyword
//
//
//
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "al9000_vfw_utils.h"

//-----------------------------------------------------------
//function declaration:
//-----------------------------------------------------------
void USB_BIT_CFG (unsigned reg_addr, unsigned bit_data, unsigned bit_mask, unsigned bit_shift);
unsigned USB_BIT_READ (unsigned reg_addr, unsigned bit_mask, unsigned bit_shift);
void USB_CTRL_INIT (unsigned dma_en, unsigned bst_len);
void USB_HOST_INIT ();
void USB_HOST_CHAN_INIT ();
//-----------------------------------------------------------

	unsigned int    membuffer[400] __attribute__ ((aligned (512)));
#define  _AARCH_64
int main()
{
    uint32_t midr;
    #ifdef _AARCH_64
    uint64_t cpunum = 0;
    uint64_t mpidr;

   // asm volatile("mrs %0, midr_el1":"=r"(midr)::"memory");
    //asm volatile("mrs %0, mpidr_el1":"=r"(mpidr)::"memory");
    #endif
    #ifdef _AARCH_32
    uint32_t cpunum = 0;
    uint32_t mpidr;
    asm volatile("mrc p15,0,%0,c0,c0,0":"=r"(midr)::"memory");
    asm volatile("mrc p15,0,%0,c0,c0,5":"=r"(mpidr)::"memory");
    #endif
	unsigned MEM_BASE_ADDR;

    volatile unsigned rdata0_0;
    volatile unsigned rdata0_1;
    volatile unsigned rdata1;
   // volatile unsigned MEM_BASE_ADDR = 0x6103FC00;  //memory base address used for DMA-test, need 256-byte*4 space
    //test var
    volatile unsigned regaddr0;  //register addr
    volatile unsigned wdata0;    //register write data

    //cpunum = mpidr & 0x00ff;
	cpunum =0;
    MEM_BASE_ADDR = (unsigned int)membuffer;
    if (cpunum == 0) {
    //-----------------------------------------------------------
    //function description:
    //-----------------------------------------------------------
    //USB_BIT_CFG: set usb register some bits
    //USB_BIT_READ: read usb register some bits, and start at bit[0]
    //
    //USB_CTRL_INIT: usb controller initialization
    //USB_HOST_INIT: usb host mode initialization
    //USB_HOST_CHAN_INIT: usb host mode channel initialization
    //-----------------------------------------------------------


    //-----------------------------------------------------------
    //case: it_usb_004
    //1. host control transfer in descriptor-DMA mode
    //2. host bulk transfer in descriptor-DMA mode(IN and OUT)
    //3. host interrupt transfer in descriptor-DMA mode(IN and OUT)
    //4. host isochronous transfer in descriptor-DMA mode(IN and OUT)
    //-----------------------------------------------------------


    //-----------------------------------------------------------
    //write the datas into Memory at first for next test
    //-----------------------------------------------------------
    //SETUP test --- from MEM_BASE_ADDR to MEM_BASE_ADDR+0x7
    //-----------------------------------------------------------
    //wValue(9-bit+7-bit deviceAddr) + bRequest(8-bit) bmRequestType(8-bit)
    wdata0 = (0x0<<23) | (0x11<<16) | (0x5<<8) | (0x0);
    REG_WRITE(MEM_BASE_ADDR,wdata0);
    //wLength(16-bit) + wIndex(16-bit)
    wdata0 = (0x0<<16) | (0x0);
    REG_WRITE(MEM_BASE_ADDR+0x4,wdata0);
    //-----------------------------------------------------------

    //-----------------------------------------------------------
    //Bulk-out test --- from MEM_BASE_ADDR+0x10 to MEM_BASE_ADDR+0x1f
    //-----------------------------------------------------------
    //the value should be the same as the data_ram in usb_connect.sv for vip test
    REG_WRITE(MEM_BASE_ADDR+0x10,0x10111213);
    REG_WRITE(MEM_BASE_ADDR+0x14,0x20212223);
    REG_WRITE(MEM_BASE_ADDR+0x18,0x30313233);
    REG_WRITE(MEM_BASE_ADDR+0x1c,0x40414243);
    //-----------------------------------------------------------

    //-----------------------------------------------------------
    //Intr-out test --- from MEM_BASE_ADDR+0x30 to MEM_BASE_ADDR+0x3f
    //-----------------------------------------------------------
    //the value should be the same as the data_ram in usb_connect.sv for vip test
    REG_WRITE(MEM_BASE_ADDR+0x30,0x10111213);
    REG_WRITE(MEM_BASE_ADDR+0x34,0x20212223);
    REG_WRITE(MEM_BASE_ADDR+0x38,0x30313233);
    REG_WRITE(MEM_BASE_ADDR+0x3c,0x40414243);
    //-----------------------------------------------------------

    //-----------------------------------------------------------
    //Isoc-out test --- from MEM_BASE_ADDR+0x50 to MEM_BASE_ADDR+0x5f
    //-----------------------------------------------------------
    //the value should be the same as the data_ram in usb_connect.sv for vip test
    REG_WRITE(MEM_BASE_ADDR+0x50,0x10111213);
    REG_WRITE(MEM_BASE_ADDR+0x54,0x20212223);
    REG_WRITE(MEM_BASE_ADDR+0x58,0x30313233);
    REG_WRITE(MEM_BASE_ADDR+0x5c,0x40414243);
    //-----------------------------------------------------------

    //-----------------------------------------------------------
    //Bulk-in test --- from MEM_BASE_ADDR+0x80 to MEM_BASE_ADDR+0x8f
    //Intr-in test --- from MEM_BASE_ADDR+0xa0 to MEM_BASE_ADDR+0xaf
    //Isoc-in test --- from MEM_BASE_ADDR+0xc0 to MEM_BASE_ADDR+0xcf
    //-----------------------------------------------------------


    //-----------------------------------------------------------
    //ss_scaledown_mode set as 2'b11
    //to reduce the simulation time
    //-----------------------------------------------------------
    REG_WRITE(TOP_NS__CFG_CTRL_USB0__ADDR,0x3);
    //-----------------------------------------------------------


    //-----------------------------------------------------------
    //test0:
    //1. host control transfer in descriptor-DMA mode
    //2. host bulk transfer in descriptor-DMA mode(IN and OUT)
    //3. host interrupt transfer in descriptor-DMA mode(IN and OUT)
    //4. host isochronous transfer in descriptor-DMA mode(IN and OUT)
    //-----------------------------------------------------------
    if(1) {
    	printf("##PRINT##: test0 start  %d",0);
        
        //usb controller initialization
        USB_CTRL_INIT(1, 1);  //enable dma
        //usb host initialization
        USB_HOST_INIT();
        //usb host channel initialization
        USB_HOST_CHAN_INIT();

        //to enable descriptor dma mode
        //HCFG config
        USB_OTG__HCFG__ACC_T        usb0_h400_reg;
        usb0_h400_reg.reg = REG_READ(USB_OTG__USB0__HCFG__ADDR);
        usb0_h400_reg.descdma = 1;  //enable descriptor dma
        REG_WRITE(USB_OTG__USB0__HCFG__ADDR,usb0_h400_reg.reg);


        //-----------------------------------------------------------
        //run testcase --- host SETUP test(control xfer) in descriptor-DMA mode
        //-----------------------------------------------------------
        printf("##PRINT##: SET_ADDRRESS xfer in descriptor-DMA mode Start  %d",0);

        //descriptor for Ctrl-OUT --- from MEM_BASE_ADDR+0x200 to MEM_BASE_ADDR+0x207
        wdata0 = 0x0;
        wdata0 |= (1<<31);  //bit[31]: active bit
        wdata0 |= (0x0<<28);  //bit[29:28]: tx_sts
        wdata0 |= (1<<26);  //bit[26]: eol
        wdata0 |= (1<<25);  //bit[25]: ioc
        wdata0 |= (1<<24);  //bit[24]: sup
        wdata0 |= 8;  //bit[16:0]: tx_bytes
        REG_WRITE(MEM_BASE_ADDR+0x200,wdata0);
        REG_WRITE(MEM_BASE_ADDR+0x204,MEM_BASE_ADDR);  //data address in mem

        //HCDMA0 config
        USB_OTG__HCDMA0__ACC_T        usb0_h514_reg;
        usb0_h514_reg.reg = REG_READ(USB_OTG__USB0__HCDMA0__ADDR);
        usb0_h514_reg.dmaaddr = MEM_BASE_ADDR+0x200;  //descriptor start address
        REG_WRITE(USB_OTG__USB0__HCDMA0__ADDR,usb0_h514_reg.reg);

        //HCTSIZ0 config
        USB_OTG__HCTSIZ0__ACC_T        usb0_h510_reg;
        usb0_h510_reg.reg = 0;
        REG_WRITE(USB_OTG__USB0__HCTSIZ0__ADDR,usb0_h510_reg.reg);
        usb0_h510_reg.reg = REG_READ(USB_OTG__USB0__HCTSIZ0__ADDR);
        usb0_h510_reg.pid = 3;  //MDATA(non-control)/SETUP(control)
        usb0_h510_reg.xfersize = (0<<8);  //1 descriptor
        REG_WRITE(USB_OTG__USB0__HCTSIZ0__ADDR,usb0_h510_reg.reg);

        //HCCHAR0 config
        USB_OTG__HCCHAR0__ACC_T        usb0_h500_reg;
        usb0_h500_reg.reg = REG_READ(USB_OTG__USB0__HCCHAR0__ADDR);
        usb0_h500_reg.eptype = 0;  //control
        usb0_h500_reg.epdir = 0;  //out
        usb0_h500_reg.mps = 8;  //max packet is 8
        usb0_h500_reg.epnum = 0;  //vip endpoint number
        usb0_h500_reg.chena = 1;  //channel enable
        REG_WRITE(USB_OTG__USB0__HCCHAR0__ADDR,usb0_h500_reg.reg);

        //poll HCINT0.XferCompl
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__HCINT0__ADDR);
        } while (((rdata0_0>>0)&0x1) == 0);  //loop when XferCompl is 0
        REG_WRITE(USB_OTG__USB0__HCINT0__ADDR,(0x1<<0));  //clear XferCompl
        REG_WRITE(USB_OTG__USB0__HCINT0__ADDR,(0x1<<1));  //clear ChHltd
        printf("##PRINT##: HCINT0.XferCompl Detected & to be cleared  %d",0);
        printf("##PRINT##: SET_ADDRESS xfer in descriptor-DMA mode is completed  %d",0);

        //check the descriptor writeback data --- active bit and Tx-Sts bits
        rdata0_0 = REG_READ(MEM_BASE_ADDR+0x200);
        if ((rdata0_0>>28) != 0) {
        	printf("##PRINT##: ERROR: Descriptor writeback value is error: %x",rdata0_0);
         //   FAIL();
        }
        for(int i = 0; i < 500; i++) {asm volatile("nop");}
        //-----------------------------------------------------------


        //-----------------------------------------------------------
        //cfg_ctrl_usb[30] set as 1'b1 to start Bulk-OUT test in vip
        //-----------------------------------------------------------
        rdata0_0 = REG_READ(TOP_NS__CFG_CTRL_USB0__ADDR);
        rdata0_0 = (rdata0_0&0xbfffffff) | (0x1<<30);
        REG_WRITE(TOP_NS__CFG_CTRL_USB0__ADDR,rdata0_0);
        //-----------------------------------------------------------


        //-----------------------------------------------------------
        //run testcase --- host Bulk-OUT test in Descriptor-DMA mode
        //-----------------------------------------------------------
        printf("##PRINT##: Bulk-OUT Xfer in descriptor-DMA mode Start  %d",0);

        //descriptor for Bulk-OUT --- from MEM_BASE_ADDR+0x200 to MEM_BASE_ADDR+0x207
        wdata0 = 0x0;
        wdata0 |= (1<<31);  //bit[31]: active bit
        wdata0 |= (0x0<<28);  //bit[29:28]: tx_sts
        wdata0 |= (1<<26);  //bit[26]: eol
        wdata0 |= (1<<25);  //bit[25]: ioc
        wdata0 |= (0<<24);  //bit[24]: sup
        wdata0 |= 16;  //bit[16:0]: tx_bytes
        REG_WRITE(MEM_BASE_ADDR+0x200,wdata0);
        REG_WRITE(MEM_BASE_ADDR+0x204,MEM_BASE_ADDR+0x10);  //data address in mem

        //Initialize channel 12
        //HCDMA12 config
        USB_OTG__HCDMA1__ACC_T        usb0_h534_reg;
        usb0_h534_reg.reg = REG_READ(USB_OTG__USB0__HCDMA0__ADDR+12*0x20);
        usb0_h534_reg.dmaaddr = MEM_BASE_ADDR+0x200;  //descriptor start address
        REG_WRITE(USB_OTG__USB0__HCDMA0__ADDR+12*0x20,usb0_h534_reg.reg);

        //HCTSIZ12 config
        USB_OTG__HCTSIZ1__ACC_T        usb0_h530_reg;
        usb0_h530_reg.reg = 0;
        REG_WRITE(USB_OTG__USB0__HCTSIZ0__ADDR+12*0x20,usb0_h530_reg.reg);
        usb0_h530_reg.reg = REG_READ(USB_OTG__USB0__HCTSIZ0__ADDR+12*0x20);
        usb0_h530_reg.pid = 0;  //DATA0
        usb0_h530_reg.xfersize = (0<<8);  //1 descriptor
        REG_WRITE(USB_OTG__USB0__HCTSIZ0__ADDR+12*0x20,usb0_h530_reg.reg);

        //HCCHAR12 config
        USB_OTG__HCCHAR1__ACC_T        usb0_h520_reg;
        usb0_h520_reg.reg = REG_READ(USB_OTG__USB0__HCCHAR0__ADDR+12*0x20);
        usb0_h520_reg.eptype = 2;  //bulk
        usb0_h520_reg.epdir = 0;  //out
        usb0_h520_reg.mps = 16;  //max packet is 16
        usb0_h520_reg.devaddr = 0x11;  //device address
        usb0_h520_reg.epnum = 1;  //vip endpoint number
        usb0_h520_reg.chena = 1;  //channel enable
        REG_WRITE(USB_OTG__USB0__HCCHAR0__ADDR+12*0x20,usb0_h520_reg.reg);

        //poll HCINT12.XferCompl
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__HCINT0__ADDR+12*0x20);
        } while (((rdata0_0>>0)&0x1) == 0);  //loop when XferCompl is 0
        REG_WRITE(USB_OTG__USB0__HCINT0__ADDR+12*0x20,(0x1<<0));  //clear XferCompl
        REG_WRITE(USB_OTG__USB0__HCINT0__ADDR+12*0x20,(0x1<<1));  //clear ChHltd
        printf("##PRINT##: HCINT12.XferCompl Detected & to be cleared  %d",0);
        printf("##PRINT##: Bulk-OUT xfer in descriptor-DMA mode is completed  %d",0);

        //check the descriptor writeback data --- active bit and Tx-Sts bits
        rdata0_0 = REG_READ(MEM_BASE_ADDR+0x200);
        if ((rdata0_0>>28) != 0) {
        	printf("##PRINT##: ERROR: Descriptor writeback value is error: %x",rdata0_0);
           // FAIL();
        }
        for(int i = 0; i < 500; i++) {asm volatile("nop");}
        //-----------------------------------------------------------


        //-----------------------------------------------------------
        //cfg_ctrl_usb[30] clear as 1'b0 to start Bulk-IN test in vip
        //-----------------------------------------------------------
        rdata0_0 = REG_READ(TOP_NS__CFG_CTRL_USB0__ADDR);
        rdata0_0 = (rdata0_0&0xbfffffff) | (0x0<<30);
        REG_WRITE(TOP_NS__CFG_CTRL_USB0__ADDR,rdata0_0);
        //-----------------------------------------------------------


        //-----------------------------------------------------------
        //run testcase --- host Bulk-IN test in Descriptor-DMA mode
        //-----------------------------------------------------------
        printf("##PRINT##: Bulk-IN Xfer in Descriptor-DMA mode Start  %d",0);

        //descriptor for Bulk-IN --- from MEM_BASE_ADDR+0x200 to MEM_BASE_ADDR+0x207
        wdata0 = 0x0;
        wdata0 |= (1<<31);  //bit[31]: active bit
        wdata0 |= (0x0<<28);  //bit[29:28]: rx_sts
        wdata0 |= (1<<26);  //bit[26]: eol
        wdata0 |= (1<<25);  //bit[25]: ioc
        wdata0 |= (0<<23);  //bit[23]: AQTD_VALID
        wdata0 |= (0<<17);  //bit[22:17]: AQTD_VALUE
        wdata0 |= 16;  //bit[16:0]: rx_bytes
        REG_WRITE(MEM_BASE_ADDR+0x200,wdata0);
        REG_WRITE(MEM_BASE_ADDR+0x204,MEM_BASE_ADDR+0x80);  //data address in mem

        //Initialize channel 13
        //HCDMA13 config
        usb0_h534_reg.reg = REG_READ(USB_OTG__USB0__HCDMA0__ADDR+13*0x20);
        usb0_h534_reg.dmaaddr = MEM_BASE_ADDR + 0x200;  //descriptor start address
        REG_WRITE(USB_OTG__USB0__HCDMA0__ADDR+13*0x20,usb0_h534_reg.reg);

        //HCTSIZ13 config
        usb0_h530_reg.reg = 0;
        REG_WRITE(USB_OTG__USB0__HCTSIZ0__ADDR+13*0x20,usb0_h530_reg.reg);
        usb0_h530_reg.reg = REG_READ(USB_OTG__USB0__HCTSIZ0__ADDR+13*0x20);
        usb0_h530_reg.pid = 0;  //DATA0
        usb0_h530_reg.xfersize = (0<<8);  //1 descriptor
        REG_WRITE(USB_OTG__USB0__HCTSIZ0__ADDR+13*0x20,usb0_h530_reg.reg);

        //HCCHAR13 config
        usb0_h520_reg.reg = REG_READ(USB_OTG__USB0__HCCHAR0__ADDR+13*0x20);
        usb0_h520_reg.eptype = 2;  //bulk
        usb0_h520_reg.epdir = 1;  //in
        usb0_h520_reg.mps = 16;  //max packet is 16
        usb0_h520_reg.devaddr = 0x11;  //device address
        usb0_h520_reg.epnum = 1;  //vip endpoint number
        usb0_h520_reg.chena = 1;  //channel enable
        REG_WRITE(USB_OTG__USB0__HCCHAR0__ADDR+13*0x20,usb0_h520_reg.reg);

        //poll HCINT13.ChHltd
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__HCINT0__ADDR+13*0x20);
        } while (((rdata0_0>>1)&0x1) == 0);  //loop when ChHltd is 0
        REG_WRITE(USB_OTG__USB0__HCINT0__ADDR+13*0x20,(0x1<<1));  //clear ChHltd

        //poll HCINT13.XferCompl
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__HCINT0__ADDR+13*0x20);
        } while (((rdata0_0>>0)&0x1) == 0);  //loop when XferCompl is 0
        REG_WRITE(USB_OTG__USB0__HCINT0__ADDR+13*0x20,(0x1<<0));  //clear XferCompl
        printf("##PRINT##: HCINT13.XferCompl Detected & to be cleared  %d",0);

        //Bulk IN 1st data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0x80);
        printf("##PRINT##: Bulk IN 1st data is %x",rdata0_0);
        if (rdata0_0 != 0xaaabacad) {
        	printf("##PRINT##: ERROR: Bulk IN 1st data is mismatch  %d",0);
           // FAIL();
        }
        //Bulk IN 2nd data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0x84);
        printf("##PRINT##: Bulk IN 2nd data is %x",rdata0_0);
        if (rdata0_0 != 0xaeafbabb) {
        	printf("##PRINT##: ERROR: Bulk IN 2nd data is mismatch  %d",0);
           // FAIL();
        }
        //Bulk IN 3th data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0x88);
        printf("##PRINT##: Bulk IN 3th data is %x",rdata0_0);
        if (rdata0_0 != 0xbcbdbebf) {
        	printf("##PRINT##: ERROR: Bulk IN 3th data is mismatch  %d",0);
           // FAIL();
        }
        //Bulk IN 4th data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0x8c);
        printf("##PRINT##: Bulk IN 4th data is %x",rdata0_0);
        if (rdata0_0 != 0xcacbcccd) {
        	printf("##PRINT##: ERROR: Bulk IN 4th data is mismatch  %d",0);
          //  FAIL();
        }
        printf("##PRINT##: Bulk-IN xfer in Descriptor-DMA mode is completed  %d",0);

        //check the descriptor writeback data --- active bit and rx-Sts bits
        rdata0_0 = REG_READ(MEM_BASE_ADDR+0x200);
        if ((rdata0_0>>28) != 0) {
        	printf("##PRINT##: ERROR: Descriptor writeback value is error: %x",rdata0_0);
            //FAIL();
        }
        for(int i = 0; i < 500; i++) {asm volatile("nop");}
        //-----------------------------------------------------------


        //-----------------------------------------------------------
        //cfg_ctrl_usb[30] set as 1'b1 to start Intr-OUT test in vip
        //-----------------------------------------------------------
        rdata0_0 = REG_READ(TOP_NS__CFG_CTRL_USB0__ADDR);
        rdata0_0 = (rdata0_0&0xbfffffff) | (0x1<<30);
        REG_WRITE(TOP_NS__CFG_CTRL_USB0__ADDR,rdata0_0);
        //-----------------------------------------------------------


        //-----------------------------------------------------------
        //run testcase --- host Intr-OUT test in Descriptor-DMA mode
        //-----------------------------------------------------------
        printf("##PRINT##: Intr-OUT Xfer in Descriptor-DMA mode Start  %d",0);

        //descriptor for Intr-OUT --- from MEM_BASE_ADDR+0x200 to MEM_BASE_ADDR+0x207
        wdata0 = 0x0;
        wdata0 |= (1<<31);  //bit[31]: active bit
        wdata0 |= (0x0<<28);  //bit[29:28]: tx_sts
        wdata0 |= (1<<26);  //bit[26]: eol
        wdata0 |= (1<<25);  //bit[25]: ioc
        wdata0 |= (0<<24);  //bit[24]: sup
        wdata0 |= 16;  //bit[16:0]: tx_bytes
        REG_WRITE(MEM_BASE_ADDR+0x200,wdata0);
        REG_WRITE(MEM_BASE_ADDR+0x204,MEM_BASE_ADDR+0x30);  //data address in mem

        //Initialize channel 14
        //HCDMA14 config
        usb0_h534_reg.reg = REG_READ(USB_OTG__USB0__HCDMA0__ADDR+14*0x20);
        usb0_h534_reg.dmaaddr = MEM_BASE_ADDR + 0x200;  //descriptor start address
        REG_WRITE(USB_OTG__USB0__HCDMA0__ADDR+14*0x20,usb0_h534_reg.reg);

        //HCTSIZ14 config
        usb0_h530_reg.reg = 0;
        REG_WRITE(USB_OTG__USB0__HCTSIZ0__ADDR+14*0x20,usb0_h530_reg.reg);
        usb0_h530_reg.reg = REG_READ(USB_OTG__USB0__HCTSIZ0__ADDR+14*0x20);
        usb0_h530_reg.pid = 0;  //DATA0
        usb0_h530_reg.xfersize = 0 | (0<<8) | 0x4;  //1 descriptor and the 3rd sched_info
        REG_WRITE(USB_OTG__USB0__HCTSIZ0__ADDR+14*0x20,usb0_h530_reg.reg);

        //HCCHAR14 config
        usb0_h520_reg.reg = REG_READ(USB_OTG__USB0__HCCHAR0__ADDR+14*0x20);
        usb0_h520_reg.eptype = 3;  //intr
        usb0_h520_reg.epdir = 0;  //out
        usb0_h520_reg.mps = 16;  //max packet is 16
        usb0_h520_reg.ec = 0x1;  //1 transaction
        usb0_h520_reg.devaddr = 0x11;  //device address
        usb0_h520_reg.epnum = 2;  //vip endpoint number
        usb0_h520_reg.chena = 1;  //channel enable
        REG_WRITE(USB_OTG__USB0__HCCHAR0__ADDR+14*0x20,usb0_h520_reg.reg);

        //poll HCINT14.XferCompl
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__HCINT0__ADDR+14*0x20);
        } while (((rdata0_0>>0)&0x1) == 0);  //loop when XferCompl is 0
        REG_WRITE(USB_OTG__USB0__HCINT0__ADDR+14*0x20,(0x1<<0));  //clear XferCompl
        REG_WRITE(USB_OTG__USB0__HCINT0__ADDR+14*0x20,(0x1<<1));  //clear ChHltd
        printf("##PRINT##: HCINT14.XferCompl Detected & to be cleared  %d",0);

        //check the descriptor writeback data --- active bit and tx-Sts bits
        rdata0_0 = REG_READ(MEM_BASE_ADDR+0x200);
        if ((rdata0_0>>28) != 0) {
        	printf("##PRINT##: ERROR: Descriptor writeback value is error: %x",rdata0_0);
            //FAIL();
        }
        printf("##PRINT##: Intr-OUT xfer in Descriptor-DMA mode is completed  %d",0);
        for(int i = 0; i < 500; i++) {asm volatile("nop");}
        //-----------------------------------------------------------


        //-----------------------------------------------------------
        //cfg_ctrl_usb[30] clear as 1'b0 to start Intr-IN test in vip
        //-----------------------------------------------------------
        rdata0_0 = REG_READ(TOP_NS__CFG_CTRL_USB0__ADDR);
        rdata0_0 = (rdata0_0&0xbfffffff) | (0x0<<30);
        REG_WRITE(TOP_NS__CFG_CTRL_USB0__ADDR,rdata0_0);
        //-----------------------------------------------------------


        //-----------------------------------------------------------
        //run testcase --- host Intr-IN test in Descriptor-DMA mode
        //-----------------------------------------------------------
        printf("##PRINT##: Intr-IN Xfer in Descriptor-DMA mode Start  %d",0);

        //program frame list into memory
        for (int i=0; i<64; i++) {
            REG_WRITE(MEM_BASE_ADDR+0x100+i*4,0x1e);
        }

        //descriptor for Intr-IN --- from MEM_BASE_ADDR+0x200 to MEM_BASE_ADDR+0x207
        wdata0 = 0x0;
        wdata0 |= (1<<31);  //bit[31]: active bit
        wdata0 |= (0x0<<28);  //bit[29:28]: rx_sts
        wdata0 |= (1<<26);  //bit[26]: eol
        wdata0 |= (1<<25);  //bit[25]: ioc
        wdata0 |= (0<<23);  //bit[23]: AQTD_VALID
        wdata0 |= (0<<17);  //bit[22:17]: AQTD_VALUE
        wdata0 |= 16;  //bit[16:0]: rx_bytes
        REG_WRITE(MEM_BASE_ADDR+0x200,wdata0);
        REG_WRITE(MEM_BASE_ADDR+0x204,MEM_BASE_ADDR+0xa0);  //data address in mem

        //HFLBAddr config
        USB_OTG__HFLBADDR__ACC_T        usb0_h41c_reg;
        usb0_h41c_reg.reg = REG_READ(USB_OTG__USB0__HFLBADDR__ADDR);
        usb0_h41c_reg.hflbaddr = MEM_BASE_ADDR+0x100;  //frame list start address
        REG_WRITE(USB_OTG__USB0__HFLBADDR__ADDR,usb0_h41c_reg.reg);

        //HCFG config
        usb0_h400_reg.reg = REG_READ(USB_OTG__USB0__HCFG__ADDR);
        usb0_h400_reg.descdma = 1;  //enable descriptor dma
        usb0_h400_reg.perschedena = 1;  //enable periodic scheduling
        usb0_h400_reg.frlisten = 0;  //frame list entries
        REG_WRITE(USB_OTG__USB0__HCFG__ADDR,usb0_h400_reg.reg);

        //Initialize channel 15
        //HCDMA15 config
        usb0_h534_reg.reg = REG_READ(USB_OTG__USB0__HCDMA0__ADDR+15*0x20);
        usb0_h534_reg.dmaaddr = MEM_BASE_ADDR + 0x200;  //descriptor start address
        REG_WRITE(USB_OTG__USB0__HCDMA1__ADDR+15*0x20,usb0_h534_reg.reg);

        //HCTSIZ15 config
        usb0_h530_reg.reg = 0;
        REG_WRITE(USB_OTG__USB0__HCTSIZ0__ADDR+15*0x20,usb0_h530_reg.reg);
        usb0_h530_reg.reg = REG_READ(USB_OTG__USB0__HCTSIZ0__ADDR+15*0x20);
        usb0_h530_reg.pid = 0;  //DATA0
        usb0_h530_reg.xfersize = 0 | (0<<8) | 0x2;  //1 descriptor and the 3rd sched_info
        REG_WRITE(USB_OTG__USB0__HCTSIZ0__ADDR+15*0x20,usb0_h530_reg.reg);

        //HCCHAR15 config
        usb0_h520_reg.reg = REG_READ(USB_OTG__USB0__HCCHAR0__ADDR+15*0x20);
        usb0_h520_reg.eptype = 3;  //intr
        usb0_h520_reg.epdir = 1;  //in
        usb0_h520_reg.mps = 16;  //max packet is 16
        usb0_h520_reg.ec = 0x1;  //1 transaction
        usb0_h520_reg.devaddr = 0x11;  //device address
        usb0_h520_reg.epnum = 2;  //vip endpoint number
        usb0_h520_reg.chena = 1;  //channel enable
        REG_WRITE(USB_OTG__USB0__HCCHAR0__ADDR+15*0x20,usb0_h520_reg.reg);

        //poll HCINT15.ChHltd
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__HCINT0__ADDR+15*0x20);
        } while (((rdata0_0>>1)&0x1) == 0);  //loop when ChHltd is 0
        REG_WRITE(USB_OTG__USB0__HCINT0__ADDR+15*0x20,(0x1<<1));  //clear ChHltd

        //poll HCINT15.XferCompl
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__HCINT0__ADDR+15*0x20);
        } while (((rdata0_0>>0)&0x1) == 0);  //loop when XferCompl is 0
        REG_WRITE(USB_OTG__USB0__HCINT0__ADDR+15*0x20,(0x1<<0));  //clear XferCompl
        printf("##PRINT##: HCINT15.XferCompl Detected & to be cleared  %d",0);

        //Intr IN 1st data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0xa0);
        printf("##PRINT##: Intr IN 1st data is %x",rdata0_0);
        if (rdata0_0 != 0xaaabacad) {
        	printf("##PRINT##: ERROR: Intr IN 1st data is mismatch  %d",0);
          //  FAIL();
        }
        //Intr IN 2nd data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0xa4);
        printf("##PRINT##: Intr IN 2nd data is %x",rdata0_0);
        if (rdata0_0 != 0xaeafbabb) {
        	printf("##PRINT##: ERROR: Intr IN 2nd data is mismatch  %d",0);
         //   FAIL();
        }
        //Intr IN 3th data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0xa8);
        printf("##PRINT##: Intr IN 3th data is %x",rdata0_0);
        if (rdata0_0 != 0xbcbdbebf) {
        	printf("##PRINT##: ERROR: Intr IN 3th data is mismatch  %d",0);
           // FAIL();
        }
        //Intr IN 4th data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0xac);
        printf("##PRINT##: Intr IN 4th data is %x",rdata0_0);
        if (rdata0_0 != 0xcacbcccd) {
        	printf("##PRINT##: ERROR: Intr IN 4th data is mismatch  %d",0);
          //  FAIL();
        }

        //check the descriptor writeback data --- active bit and rx-Sts bits
        rdata0_0 = REG_READ(MEM_BASE_ADDR+0x200);
        if ((rdata0_0>>28) != 0) {
        	printf("##PRINT##: ERROR: Descriptor writeback value is error: %x",rdata0_0);
          //  FAIL();
        }
        printf("##PRINT##: Intr-IN xfer in writeback-DMA mode is completed  %d",0);
        for(int i = 0; i < 500; i++) {asm volatile("nop");}
        //-----------------------------------------------------------

/*
        //-----------------------------------------------------------
        //cfg_ctrl_usb[30] set as 1'b1 to start Isoc-OUT test in vip
        //-----------------------------------------------------------
        rdata0_0 = REG_READ(TOP_NS__CFG_CTRL_USB0__ADDR);
        rdata0_0 = (rdata0_0&0xbfffffff) | (0x1<<30);
        REG_WRITE(TOP_NS__CFG_CTRL_USB0__ADDR,rdata0_0);
        //-----------------------------------------------------------


        //-----------------------------------------------------------
        //run testcase --- host Isoc-OUT test in buffer-DMA mode
        //-----------------------------------------------------------
        vfwp("##PRINT##: Isoc-OUT Xfer in buffer-DMA mode Start  %d",0);
        //clear SOF interrupt at first
        REG_WRITE(USB_OTG__USB0__GINTSTS__ADDR,(0x1<<3));  //clear Sof
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__GINTSTS__ADDR);
        } while (((rdata0_0>>3)&0x1) == 1);  //loop when Sof is 1

        //poll GINTSTS.Sof
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__GINTSTS__ADDR);
        } while (((rdata0_0>>3)&0x1) == 0);  //loop when Sof is 0
        REG_WRITE(USB_OTG__USB0__GINTSTS__ADDR,(0x1<<3));  //clear Sof

        //read the frame num reg
        wdata0 = REG_READ(USB_OTG__USB0__HFNUM__ADDR);
        if ((wdata0&0x1) == 0) {
            wdata0 = 1;  //even frame number
        } else {
            wdata0 = 0;  //odd frame number
        }

        //Initialize channel 1
        //HCDMA1 config
        usb0_h534_reg.reg = REG_READ(USB_OTG__USB0__HCDMA1__ADDR);
        usb0_h534_reg.dmaaddr = MEM_BASE_ADDR + 0x28;  //mem start address
        REG_WRITE(USB_OTG__USB0__HCDMA1__ADDR,usb0_h534_reg.reg);

        //HCTSIZ1 config
        usb0_h530_reg.reg = 0;
        REG_WRITE(USB_OTG__USB0__HCTSIZ1__ADDR,usb0_h530_reg.reg);
        usb0_h530_reg.reg = REG_READ(USB_OTG__USB0__HCTSIZ1__ADDR);
        usb0_h530_reg.pid = 0;  //DATA0
        usb0_h530_reg.pktcnt = 1;
        usb0_h530_reg.xfersize = 16;
        REG_WRITE(USB_OTG__USB0__HCTSIZ1__ADDR,usb0_h530_reg.reg);

        //HCCHAR1 config
        usb0_h520_reg.reg = REG_READ(USB_OTG__USB0__HCCHAR1__ADDR);
        usb0_h520_reg.eptype = 1;  //isoc
        usb0_h520_reg.epdir = 0;  //out
        usb0_h520_reg.mps = 16;  //max packet is 16
        usb0_h520_reg.ec = 0x1;  //1 transaction
        usb0_h520_reg.devaddr = 0x11;  //device address
        usb0_h520_reg.epnum = 0x3;  //vip endpoint number
        usb0_h520_reg.oddfrm = ((wdata0==0) ? 1 : 0);  //odd frame set
        usb0_h520_reg.chena = 1;  //channel enable
        REG_WRITE(USB_OTG__USB0__HCCHAR1__ADDR,usb0_h520_reg.reg);

        //poll HCINT1.XferCompl
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__HCINT1__ADDR);
        } while (((rdata0_0>>0)&0x1) == 0);  //loop when XferCompl is 0
        REG_WRITE(USB_OTG__USB0__HCINT1__ADDR,(0x1<<0));  //clear XferCompl
        REG_WRITE(USB_OTG__USB0__HCINT1__ADDR,(0x1<<1));  //clear ChHltd
        vfwp("##PRINT##: HCINT1.XferCompl Detected & to be cleared  %d",0);

        //reset HCCHAR1.OddFrm
        usb0_h520_reg.reg = REG_READ(USB_OTG__USB0__HCCHAR1__ADDR);
        usb0_h520_reg.oddfrm = 0;  //odd frame set
        REG_WRITE(USB_OTG__USB0__HCCHAR1__ADDR,usb0_h520_reg.reg);

        vfwp("##PRINT##: Isoc-OUT xfer in buffer-DMA mode is completed  %d",0);
        for(int i = 0; i < 500; i++) {asm volatile("nop");}
        //-----------------------------------------------------------


        //-----------------------------------------------------------
        //cfg_ctrl_usb[30] clear as 1'b0 to start Isoc-OUT test in vip
        //-----------------------------------------------------------
        rdata0_0 = REG_READ(TOP_NS__CFG_CTRL_USB0__ADDR);
        rdata0_0 = (rdata0_0&0xbfffffff) | (0x0<<30);
        REG_WRITE(TOP_NS__CFG_CTRL_USB0__ADDR,rdata0_0);
        //-----------------------------------------------------------


        //-----------------------------------------------------------
        //run testcase --- host Isoc-IN test in buffer-DMA mode
        //-----------------------------------------------------------
        vfwp("##PRINT##: Isoc-IN Xfer in buffer-DMA mode Start  %d",0);
        //clear SOF interrupt at first
        REG_WRITE(USB_OTG__USB0__GINTSTS__ADDR,(0x1<<3));  //clear Sof
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__GINTSTS__ADDR);
        } while (((rdata0_0>>3)&0x1) == 1);  //loop when Sof is 1

        //poll GINTSTS.Sof
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__GINTSTS__ADDR);
        } while (((rdata0_0>>3)&0x1) == 0);  //loop when Sof is 0
        REG_WRITE(USB_OTG__USB0__GINTSTS__ADDR,(0x1<<3));  //clear Sof

        //read the frame num reg
        wdata0 = REG_READ(USB_OTG__USB0__HFNUM__ADDR);
        if ((wdata0&0x1) == 0) {
            wdata0 = 1;  //even frame number
        } else {
            wdata0 = 0;  //odd frame number
        }

        //Initialize channel 1
        //HCDMA1 config
        usb0_h534_reg.reg = REG_READ(USB_OTG__USB0__HCDMA1__ADDR);
        usb0_h534_reg.dmaaddr = MEM_BASE_ADDR + 0x100 + 0x20;  //mem start address
        REG_WRITE(USB_OTG__USB0__HCDMA1__ADDR,usb0_h534_reg.reg);

        //HCTSIZ1 config
        usb0_h530_reg.reg = 0;
        REG_WRITE(USB_OTG__USB0__HCTSIZ1__ADDR,usb0_h530_reg.reg);
        usb0_h530_reg.reg = REG_READ(USB_OTG__USB0__HCTSIZ1__ADDR);
        usb0_h530_reg.pid = 0;  //DATA0
        usb0_h530_reg.pktcnt = 1;
        usb0_h530_reg.xfersize = 16;
        REG_WRITE(USB_OTG__USB0__HCTSIZ1__ADDR,usb0_h530_reg.reg);

        //HCCHAR1 config
        usb0_h520_reg.reg = REG_READ(USB_OTG__USB0__HCCHAR1__ADDR);
        usb0_h520_reg.eptype = 1;  //isoc
        usb0_h520_reg.epdir = 1;  //in
        usb0_h520_reg.mps = 16;  //max packet is 16
        usb0_h520_reg.ec = 0x1;  //1 transaction
        usb0_h520_reg.devaddr = 0x11;  //device address
        usb0_h520_reg.epnum = 0x3;  //vip endpoint number
        usb0_h520_reg.oddfrm = ((wdata0==0) ? 1 : 0);  //odd frame set
        usb0_h520_reg.chena = 1;  //channel enable
        REG_WRITE(USB_OTG__USB0__HCCHAR1__ADDR,usb0_h520_reg.reg);

        //poll HCINT1.ChHltd
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__HCINT1__ADDR);
        } while (((rdata0_0>>1)&0x1) == 0);  //loop when ChHltd is 0
        REG_WRITE(USB_OTG__USB0__HCINT1__ADDR,(0x1<<1));  //clear ChHltd

        //poll HCINT1.XferCompl
        do {
            rdata0_0 = REG_READ(USB_OTG__USB0__HCINT1__ADDR);
        } while (((rdata0_0>>0)&0x1) == 0);  //loop when XferCompl is 0
        REG_WRITE(USB_OTG__USB0__HCINT1__ADDR,(0x1<<0));  //clear XferCompl
        vfwp("##PRINT##: HCINT1.XferCompl Detected & to be cleared  %d",0);

        //reset HCCHAR1.OddFrm
        usb0_h520_reg.reg = REG_READ(USB_OTG__USB0__HCCHAR1__ADDR);
        usb0_h520_reg.oddfrm = 0;  //odd frame set
        REG_WRITE(USB_OTG__USB0__HCCHAR1__ADDR,usb0_h520_reg.reg);

        //Isoc IN 1st data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0x100 + 0x20);
        vfwp("##PRINT##: Isoc IN 1st data is %x",rdata0_0);
        if (rdata0_0 != 0xaaabacad) {
            vfwp("##PRINT##: ERROR: Isoc IN 1st data is mismatch  %d",0);
            FAIL();
        }
        //Isoc IN 2nd data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0x100 + 0x24);
        vfwp("##PRINT##: Isoc IN 2nd data is %x",rdata0_0);
        if (rdata0_0 != 0xaeafbabb) {
            vfwp("##PRINT##: ERROR: Isoc IN 2nd data is mismatch  %d",0);
            FAIL();
        }
        //Isoc IN 3th data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0x100 + 0x28);
        vfwp("##PRINT##: Isoc IN 3th data is %x",rdata0_0);
        if (rdata0_0 != 0xbcbdbebf) {
            vfwp("##PRINT##: ERROR: Isoc IN 3th data is mismatch  %d",0);
            FAIL();
        }
        //Isoc IN 4th data compared accrording to usb_connect.sv
        rdata0_0 = REG_READ(MEM_BASE_ADDR + 0x100 + 0x2c);
        vfwp("##PRINT##: Isoc IN 4th data is %x",rdata0_0);
        if (rdata0_0 != 0xcacbcccd) {
            vfwp("##PRINT##: ERROR: Isoc IN 4th data is mismatch  %d",0);
            FAIL();
        }

        vfwp("##PRINT##: Isoc-IN xfer in buffer-DMA mode is completed  %d",0);
        for(int i = 0; i < 500; i++) {asm volatile("nop");}
        //-----------------------------------------------------------
*/

        //-----------------------------------------------------------
        //cfg_ctrl_usb[31] set as 1'b1 to indicate the test is completed
        //-----------------------------------------------------------
        rdata0_0 = REG_READ(TOP_NS__CFG_CTRL_USB0__ADDR);
        rdata0_0 = (rdata0_0&0x7fffffff) | (0x1<<31);
        REG_WRITE(TOP_NS__CFG_CTRL_USB0__ADDR,rdata0_0);
        for(int i = 0; i < 500; i++) {asm volatile("nop");}
        //-----------------------------------------------------------

        printf("##PRINT##: test0 finished, result is PASS  %d",0);
    }  //if(1)
    //-----------------------------------------------------------











    for(int i = 0; i < 100; i++) {asm volatile("nop");}
   // SUCCESS();   // cpu votes with thumb up
    }  //if (cpunum == 0)



//    if (cpunum == 1) {
//
//    // Ensure all writes to system registers have taken place
//    //asm volatile("dsb     sy");
//    //asm volatile("isb     sy");
//    //asm volatile("dmb     sy");
//
//    char  p[] = "** CPU1 TEST PASSED OK **\n";
//    char* c   = p;
//
//    while (*c)
//    {
//      *TUBE_ADDRESS = *c;
//      c++;
//    }
//
//
//    }
    while(1);

    return 0;
}



//-----------------------------------------------------------
//USB_BIT_CFG: set usb register some bits
//-----------------------------------------------------------
//reg_addr: register address
//bit_data: bits data need to set
//bit_mask: bits need to set
//bit_shift: bits need to shift
//-----------------------------------------------------------
void USB_BIT_CFG (unsigned reg_addr, unsigned bit_data, unsigned bit_mask, unsigned bit_shift)
{
    unsigned rdata;      //register read data

    rdata = REG_READ(reg_addr);  //reg read

    //set usb register bits
    rdata = (~bit_mask & rdata) | (bit_mask & (bit_data << bit_shift));  //only set the masked bits
    REG_WRITE(reg_addr, rdata);  //reg write
}  //USB_BIT_CFG
//-----------------------------------------------------------

//-----------------------------------------------------------
//USB_BIT_READ: read usb register some bits, and start at bit[0]
//-----------------------------------------------------------
//reg_addr: register address
//bit_mask: bits need to read
//bit_shift: bits need to shift
//-----------------------------------------------------------
//return: read bits
//-----------------------------------------------------------
unsigned USB_BIT_READ (unsigned reg_addr, unsigned bit_mask, unsigned bit_shift)
{
    unsigned rdata;      //register read bits data

    rdata = REG_READ(reg_addr);  //reg read

    //catch usb register read bits
    rdata &= bit_mask;  //only read the masked bits
    rdata = (rdata >> bit_shift);  //shift bits to start at bit[0]
    return rdata;
}  //USB_BIT_READ
//-----------------------------------------------------------

//-----------------------------------------------------------
//USB_CTRL_INIT: usb controller initialization
//-----------------------------------------------------------
//dma_en: "1" for DMA enable, "0" for DMA disable
//bst_len: DMA burst length, "0" for Single, "1" for INCR, "3" for INCR4, "5" for INCR8, "7" for INCR16
//-----------------------------------------------------------
void USB_CTRL_INIT (unsigned dma_en, unsigned bst_len)
{
    unsigned rdata;

    //check dma_en and bst_len
    if ((dma_en != 0) && (dma_en != 1)) {
    	printf("##PRINT##: ERROR: dma_en Input Error, Please Input 0 or 1 for dma_en  %d",0);
       // FAIL();
    }
    if ((bst_len != 0) && (bst_len != 1) && (bst_len != 3) && (bst_len != 5) && (bst_len != 7)) {
    	printf("##PRINT##: ERROR: bst_len Input Error, Please Input 0 or 1 or 3 or 5 or 7 for bst_len  %d",0);
        //FAIL();
    }

    printf("##PRINT##: The Control initialization is Started  %d",0);

    //GAHBCFG config
    USB_OTG__GAHBCFG__ACC_T        usb0_h008_reg;
    usb0_h008_reg.reg = 0x0;
    usb0_h008_reg.ptxfemplvl = 1;  //completed empty
    usb0_h008_reg.nptxfemplvl = 1;  //completed empty
    usb0_h008_reg.glblintrmsk = 1;  //unmasked
    usb0_h008_reg.dmaen = dma_en;  //DMA enable or disable
    usb0_h008_reg.hbstlen = bst_len;  //DMA burst length
    REG_WRITE(USB_OTG__USB0__GAHBCFG__ADDR,usb0_h008_reg.reg);

    //GINTMSK config
    USB_OTG__GINTMSK__ACC_T        usb0_h018_reg;
    usb0_h018_reg.reg = REG_READ(USB_OTG__USB0__GINTMSK__ADDR);
    usb0_h018_reg.rxflvlmsk = 0;  //masked
    REG_WRITE(USB_OTG__USB0__GINTMSK__ADDR,usb0_h018_reg.reg);

    //GUSBCFG config
    USB_OTG__GUSBCFG__ACC_T        usb0_h00c_reg;
    usb0_h00c_reg.reg = REG_READ(USB_OTG__USB0__GUSBCFG__ADDR);
    usb0_h00c_reg.phyif = 0;  //8-bit ULPI
    usb0_h00c_reg.ulpi_utmi_sel = 1;  //ULPI intf
    usb0_h00c_reg.usbtrdtim = 9;  //8-bit UTMI+ intf
    usb0_h00c_reg.toutcal = 0;  //<--- need some refs

    usb0_h00c_reg.forcehstmode =1;
    usb0_h00c_reg.forcedevmode = 0;
   /*

    set = host ? GUSBCFG_FORCEHOSTMODE : GUSBCFG_FORCEDEVMODE;
        clear = host ? GUSBCFG_FORCEDEVMODE : GUSBCFG_FORCEHOSTMODE;

        usbcfg.d32 &= ~clear;
        usbcfg.d32 |= set;
        */

    REG_WRITE(USB_OTG__USB0__GUSBCFG__ADDR,usb0_h00c_reg.reg);

    //GINTMSK config
    usb0_h018_reg.reg = REG_READ(USB_OTG__USB0__GINTMSK__ADDR);
    usb0_h018_reg.otgintmsk = 1;  //unmasked
    usb0_h018_reg.modemismsk = 1;  //unmasked
    REG_WRITE(USB_OTG__USB0__GINTMSK__ADDR,usb0_h018_reg.reg);

    //GOTGCTL config
    USB_OTG__GOTGCTL__ACC_T        usb0_h000_reg;
    usb0_h000_reg.reg = REG_READ(USB_OTG__USB0__GOTGCTL__ADDR);
    usb0_h000_reg.otgver = 1;  //version 2.0
    REG_WRITE(USB_OTG__USB0__GOTGCTL__ADDR,usb0_h000_reg.reg);
    //for(int i = 0; i < 500; i++) {asm volatile("nop");}

    //check current mode
    rdata = REG_READ(USB_OTG__USB0__GINTSTS__ADDR);
    if ((rdata&0x1) == 0) {
    	printf("##PRINT##: The current mode is Device Mode  %d",0);
    } else {
    	printf("##PRINT##: The current mode is Host Mode  %d",0);
    }

    printf("##PRINT##: The Control initialization is Completed  %d",0);
}  //USB_CTRL_INIT
//-----------------------------------------------------------

//-----------------------------------------------------------
//USB_HOST_INIT: usb host mode initialization
//-----------------------------------------------------------
void USB_HOST_INIT ()
{
    unsigned rdata;

    printf("##PRINT##: The Host Mode initialization is Started  %d",0);

    //GINTMSK config
    USB_OTG__GINTMSK__ACC_T        usb0_h018_reg;
    usb0_h018_reg.reg = REG_READ(USB_OTG__USB0__GINTMSK__ADDR);
    usb0_h018_reg.prtintmsk = 1;  //unmasked
    REG_WRITE(USB_OTG__USB0__GINTMSK__ADDR,usb0_h018_reg.reg);

    //HCFG config
    USB_OTG__HCFG__ACC_T        usb0_h400_reg;
    usb0_h400_reg.reg = REG_READ(USB_OTG__USB0__HCFG__ADDR);
    //usb0_h400_reg.fslssupp = 1;  //FS/LS only
    //usb0_h400_reg.fslspclksel = 1;  //48 MHz selected
    usb0_h400_reg.fslssupp = 0;  //HS/FS/LS
    REG_WRITE(USB_OTG__USB0__HCFG__ADDR,usb0_h400_reg.reg);

    //HPRT config
    USB_OTG__HPRT__ACC_T        usb0_h440_reg;
    usb0_h440_reg.reg = REG_READ(USB_OTG__USB0__HPRT__ADDR);
    usb0_h440_reg.prtpwr = 1;  //port power on
    usb0_h440_reg.prtconndet = 0;  //no connection
    REG_WRITE(USB_OTG__USB0__HPRT__ADDR,usb0_h440_reg.reg);

    //poll HPRT.PrtConnDet
    do {
        rdata = REG_READ(USB_OTG__USB0__HPRT__ADDR);
    } while (((rdata>>1)&0x1) == 0);  //loop when PrtConnDet is not set
    printf("##PRINT##: The Host Port Connection is detected  %d",0);

    //Program the HPRT.PrtRst bit to 1'b1 to start the reset process
    usb0_h440_reg.reg = REG_READ(USB_OTG__USB0__HPRT__ADDR);
    usb0_h440_reg.prtrst = 1;  //port reset
    REG_WRITE(USB_OTG__USB0__HPRT__ADDR,usb0_h440_reg.reg);
    printf("##PRINT##: The Host Reset Process is Start  %d",0);

    //wait for reset completed --- at least 50us for simulation(10ms for emulaiton)
    for(int i = 0; i < 2000; i++) {asm volatile("nop");}

    //Program the HPRT.PrtRst bit reset to 1'b0
    usb0_h440_reg.reg = REG_READ(USB_OTG__USB0__HPRT__ADDR);
    usb0_h440_reg.prtrst = 0;
    REG_WRITE(USB_OTG__USB0__HPRT__ADDR,usb0_h440_reg.reg);

    //poll HPRT.PrtEnChng
    do {
        rdata = REG_READ(USB_OTG__USB0__HPRT__ADDR);
    } while (((rdata>>3)&0x1) == 0);  //loop when PrtEnChng is not set
    printf("##PRINT##: The Host Port Enable Has Changed  %d",0);

    //poll HPRT.PrtEna
    do {
        rdata = REG_READ(USB_OTG__USB0__HPRT__ADDR);
    } while (((rdata>>2)&0x1) == 0);  //loop when PrtEna is not set

    //Read the HPRT.PrtSpd field to get the enumerated speed.
    rdata = REG_READ(USB_OTG__USB0__HPRT__ADDR);
    if (((rdata>>17)&0x3) == 0x0) {
    	printf("##PRINT##: The Enumerated Speed is High-Speed  %d",0);
        //config HFIR
        rdata = REG_READ(USB_OTG__USB0__HFIR__ADDR);
        rdata = (rdata&0xffff0000) | (0x1d4c&0xffff);  //HS 125us Sof interval
        REG_WRITE(USB_OTG__USB0__HFIR__ADDR,rdata);
    } else if (((rdata>>17)&0x3) == 0x1) {
    	printf("##PRINT##: The Enumerated Speed is Full-Speed  %d",0);
        //config HFIR
        rdata = REG_READ(USB_OTG__USB0__HFIR__ADDR);
        rdata = (rdata&0xffff0000) | (0xea60&0xffff);  //FS 1ms Sof interval
        REG_WRITE(USB_OTG__USB0__HFIR__ADDR,rdata);
    } else if (((rdata>>17)&0x3) == 0x2) {
    	printf("##PRINT##: The Enumerated Speed is Low-Speed  %d",0);
        //config HFIR
        rdata = REG_READ(USB_OTG__USB0__HFIR__ADDR);
        rdata = (rdata&0xffff0000) | (0xea60&0xffff);  //LS 1ms Sof interval
        REG_WRITE(USB_OTG__USB0__HFIR__ADDR,rdata);
    }

    printf("##PRINT##: The Host Mode initialization is Completed  %d",0);
}  //USB_HOST_INIT
//-----------------------------------------------------------

//-----------------------------------------------------------
//USB_HOST_CHAN_INIT: usb host mode channel initialization
//-----------------------------------------------------------
void USB_HOST_CHAN_INIT ()
{
    unsigned wdata;
    unsigned fifo_start_addr;  //fifo start address

    printf("##PRINT##: The Host Mode Channel initialization is Started  %d",0);

    //GINTMSK config
    USB_OTG__GINTMSK__ACC_T        usb0_h018_reg;
    usb0_h018_reg.reg = REG_READ(USB_OTG__USB0__GINTMSK__ADDR);
    usb0_h018_reg.rxflvlmsk = 1;
    usb0_h018_reg.ptxfempmsk = 1;  //unmasked
    usb0_h018_reg.nptxfempmsk = 1;  //unmasked
    usb0_h018_reg.sofmsk = 1;
    usb0_h018_reg.prtintmsk = 1;
    usb0_h018_reg.hchintmsk = 1;
    REG_WRITE(USB_OTG__USB0__GINTMSK__ADDR,usb0_h018_reg.reg);

    //HAINTMSK config
    USB_OTG__HAINTMSK__ACC_T        usb0_h418_reg;
    usb0_h418_reg.reg = REG_READ(USB_OTG__USB0__HAINTMSK__ADDR);
    usb0_h418_reg.reg |= 0xffffffff;  //unmasked all host channel interrupt
    REG_WRITE(USB_OTG__USB0__HAINTMSK__ADDR,usb0_h418_reg.reg);

    //HCINTMSKi config
    wdata = 0xffffffff;  //unmasked the transaction related interrupts
    for(int i = 0; i < 16; i++) {
        REG_WRITE(USB_OTG__USB0__HCINTMSK0__ADDR+i*0x20,wdata);
    }

    //GRXFSIZ config
    USB_OTG__GRXFSIZ__ACC_T        usb0_h024_reg;
    usb0_h024_reg.reg = REG_READ(USB_OTG__USB0__GRXFSIZ__ADDR);
    if ((usb0_h024_reg.reg&0xffff) > 300) {
        usb0_h024_reg.reg = 300;  //depth
        fifo_start_addr = 300;  //start address of GNPTXFSIZ
        REG_WRITE(USB_OTG__USB0__GRXFSIZ__ADDR,usb0_h024_reg.reg);
    } else {
        fifo_start_addr = (usb0_h024_reg.reg&0xffff);
    }

    //GNPTXFSIZ config
    USB_OTG__GNPTXFSIZ__ACC_T        usb0_h028_reg;
    usb0_h028_reg.reg = REG_READ(USB_OTG__USB0__GNPTXFSIZ__ADDR);
    if (((usb0_h028_reg.reg>>16)&0xffff) > 256) {
        usb0_h028_reg.reg = (256<<16);  //depth
        usb0_h028_reg.reg |= fifo_start_addr;  //start address
        REG_WRITE(USB_OTG__USB0__GNPTXFSIZ__ADDR,usb0_h028_reg.reg);
        fifo_start_addr += 256;  //start address of HPTXFSIZ
    } else {
        usb0_h028_reg.reg = (usb0_h028_reg.reg&0xffff0000) | fifo_start_addr;
        REG_WRITE(USB_OTG__USB0__GNPTXFSIZ__ADDR,usb0_h028_reg.reg);
        fifo_start_addr += (usb0_h028_reg.reg>>16);  //start address of HPTXFSIZ
    }

    //HPTXFSIZ config
    USB_OTG__HPTXFSIZ__ACC_T        usb0_h100_reg;
    usb0_h100_reg.reg = REG_READ(USB_OTG__USB0__HPTXFSIZ__ADDR);
    if (((usb0_h100_reg.reg>>16)&0xffff) > 512) {
        usb0_h100_reg.reg = (512<<16);  //depth
        usb0_h100_reg.reg |= fifo_start_addr;  //start address
        REG_WRITE(USB_OTG__USB0__HPTXFSIZ__ADDR,usb0_h100_reg.reg);
        fifo_start_addr += 512;
    } else {
        usb0_h100_reg.reg = (usb0_h100_reg.reg&0xffff0000) | fifo_start_addr;
        REG_WRITE(USB_OTG__USB0__HPTXFSIZ__ADDR,usb0_h100_reg.reg);
        fifo_start_addr += (usb0_h100_reg.reg>>16);
    }
    for(int i = 0; i < 500; i++) {asm volatile("nop");}

    printf("##PRINT##: The Host Mode Channel initialization is Completed  %d",0);
}  //USB_HOST_CHAN_INIT
//-----------------------------------------------------------



