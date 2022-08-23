//------------------------------------------------------------------------------
// dmac registers read and write test
//  (1) read and write by secure APB interface
//  (2) read and write by non-secure APB interface
//------------------------------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "al9000_registers.h"
#include "xdmaps.h"

#define vfwp printf
#define TOP_S__CFG_CTRL2_DMAC_AXI__ADDR 0xF88060A0u
#define TUBE_ADDRESS ((volatile char *) 0xF8800010u)

int main()
{

volatile unsigned rdata0;
volatile unsigned rdata1;
volatile unsigned rdata2;
volatile unsigned rdata3;
volatile unsigned rdata4;
volatile unsigned rdata5;
volatile unsigned rdata6;
volatile unsigned rdata7;
volatile unsigned rdata8;
volatile unsigned rdata9;
volatile unsigned rdata10;
volatile unsigned rdata11;
volatile unsigned rdata12;
volatile unsigned rdata13;
volatile unsigned rdata14;
volatile unsigned rdata15;
volatile unsigned rdata16;
volatile unsigned rdata17;
volatile unsigned rdata18;
volatile unsigned rdata19;
volatile unsigned rdata20;
volatile unsigned rdata21;
volatile unsigned rdata22;
volatile unsigned rdata23;
volatile unsigned rdata24;
volatile unsigned rdata25;
volatile unsigned rdata26;
volatile unsigned rdata27;
volatile unsigned rdata28;
volatile unsigned rdata29;
volatile unsigned rdata30;
volatile unsigned rdata31;
volatile unsigned rdata32;
volatile unsigned rdata33;
volatile unsigned rdata34;
volatile unsigned rdata35;
volatile unsigned rdata36;
volatile unsigned rdata37;
volatile unsigned rdata38;
volatile unsigned rdata39;
volatile unsigned rdata40;
volatile unsigned rdata41;
volatile unsigned rdata42;
volatile unsigned rdata43;
volatile unsigned rdata44;
volatile unsigned rdata45;
volatile unsigned rdata46;
volatile unsigned rdata47;
volatile unsigned rdata48;
volatile unsigned rdata49;
volatile unsigned rdata50;
volatile unsigned rdata51;
volatile unsigned rdata52;
volatile unsigned rdata53;
volatile unsigned rdata54;
volatile unsigned rdata55;
volatile unsigned rdata56;
volatile unsigned rdata57;
volatile unsigned rdata58;
volatile unsigned rdata59;
volatile unsigned rdata60;
volatile unsigned rdata61;
volatile unsigned rdata62;
volatile unsigned rdata63;
volatile unsigned rdata64;
volatile unsigned rdata65;
volatile unsigned rdata66;
volatile unsigned rdata67;
volatile unsigned rdata68;
volatile unsigned rdata69;
volatile unsigned rdata70;
volatile unsigned rdata71;
volatile unsigned rdata72;
volatile unsigned rdata73;
volatile unsigned rdata74;
volatile unsigned rdata75;
volatile unsigned rdata76;
volatile unsigned rdata77;
volatile unsigned rdata78;
volatile unsigned rdata79;
volatile unsigned rdata80;
volatile unsigned rdata81;
volatile unsigned rdata82;
volatile unsigned rdata83;
volatile unsigned rdata84;
volatile unsigned rdata85;
volatile unsigned rdata86;
volatile unsigned rdata87;
volatile unsigned rdata88;
volatile unsigned rdata89;
volatile unsigned rdata90;
volatile unsigned rdata91;
volatile unsigned rdata92;
volatile unsigned rdata93;
volatile unsigned rdata94;
volatile unsigned rdata95;
volatile unsigned rdata96;
volatile unsigned rdata97;
volatile unsigned rdata98;
volatile unsigned rdata99;
volatile unsigned rdata100;
volatile unsigned rdata101;
volatile unsigned rdata102;
volatile unsigned rdata103;
volatile unsigned rdata104;
volatile unsigned rdata105;
volatile unsigned rdata106;
volatile unsigned rdata107;
volatile unsigned rdata108;
volatile unsigned rdata109;
volatile unsigned rdata110;
volatile unsigned rdata111;
volatile unsigned rdata112;
volatile unsigned rdata113;
volatile unsigned rdata114;
volatile unsigned rdata115;
volatile unsigned rdata116;
volatile unsigned rdata117;
volatile unsigned rdata118;
volatile unsigned rdata119;
volatile unsigned rdata120;
volatile unsigned rdata121;
volatile unsigned rdata122;
volatile unsigned rdata123;
volatile unsigned rdata124;
volatile unsigned rdata125;
volatile unsigned rdata126;
volatile unsigned rdata127;
volatile unsigned rdata128;
volatile unsigned rdata129;
volatile unsigned rdata130;
volatile unsigned rdata131;
volatile unsigned rdata132;
volatile unsigned rdata133;
volatile unsigned rdata134;
volatile unsigned rdata135;
volatile unsigned rdata136;
volatile unsigned rdata137;
volatile unsigned rdata138;
volatile unsigned rdata139;
volatile unsigned rdata140;
volatile unsigned rdata141;
volatile unsigned rdata142;
volatile unsigned rdata143;
volatile unsigned rdata144;
volatile unsigned rdata145;
volatile unsigned rdata146;
volatile unsigned rdata147;
volatile unsigned rdata148;
volatile unsigned rdata149;
volatile unsigned rdata150;
volatile unsigned rdata151;
volatile unsigned rdata152;
volatile unsigned rdata153;
volatile unsigned rdata154;
volatile unsigned rdata155;
volatile unsigned rdata156;
volatile unsigned rdata157;
volatile unsigned rdata158;
volatile unsigned rdata159;
volatile unsigned rdata160;
volatile unsigned rdata161;
volatile unsigned rdata162;
volatile unsigned rdata163;
volatile unsigned rdata164;
volatile unsigned rdata165;
volatile unsigned rdata166;
volatile unsigned rdata167;
volatile unsigned rdata168;
volatile unsigned rdata169;
volatile unsigned rdata170;
volatile unsigned rdata171;
volatile unsigned rdata172;
volatile unsigned rdata173;
volatile unsigned rdata174;
volatile unsigned rdata175;
volatile unsigned rdata176;
volatile unsigned rdata177;
volatile unsigned rdata178;
volatile unsigned rdata179;
volatile unsigned rdata180;
volatile unsigned rdata181;
volatile unsigned rdata182;
volatile unsigned rdata183;

volatile unsigned fail_flag;

//##### STEP2 #####
//1 change to non_secure mode
      WriteReg(TOP_S__CFG_CTRL2_DMAC_AXI__ADDR, 0x001FFFFF);
      WriteReg(CRP__SRST_CTRL1__ADDR,0xfffffeff);//soft reset
      WriteReg(CRP__SRST_CTRL1__ADDR,0xffffffff);//soft reset release

     WriteReg(DMAC_AXI_NON_SECURE__DSR__ADDR,0xffffffff);
     rdata0 = ReadReg(DMAC_AXI_NON_SECURE__DSR__ADDR);//0x000

     WriteReg(DMAC_AXI_NON_SECURE__DPC__ADDR,0xffffffff);
     rdata1 = ReadReg(DMAC_AXI_NON_SECURE__DPC__ADDR);//0x004

     WriteReg(DMAC_AXI_NON_SECURE__INTEN__ADDR,0xffffffff);
     rdata2 = ReadReg(DMAC_AXI_NON_SECURE__INTEN__ADDR);//0x020

     WriteReg(DMAC_AXI_NON_SECURE__INT_EVENT_RIS__ADDR,0xffffffff);
     rdata3 = ReadReg(DMAC_AXI_NON_SECURE__INT_EVENT_RIS__ADDR);//0x024

     WriteReg(DMAC_AXI_NON_SECURE__INTMIS__ADDR,0xffffffff);
     rdata4 = ReadReg(DMAC_AXI_NON_SECURE__INTMIS__ADDR);//0x028

     WriteReg(DMAC_AXI_NON_SECURE__INTCLR__ADDR,0xffffffff);
     rdata5 = ReadReg(DMAC_AXI_NON_SECURE__INTCLR__ADDR);//0x02C

     WriteReg(DMAC_AXI_NON_SECURE__FSRD__ADDR,0xffffffff);
     rdata6 = ReadReg(DMAC_AXI_NON_SECURE__FSRD__ADDR);//0x030

     WriteReg(DMAC_AXI_NON_SECURE__FSRC__ADDR,0xffffffff);
     rdata7 = ReadReg(DMAC_AXI_NON_SECURE__FSRC__ADDR);//0x034

     WriteReg(DMAC_AXI_NON_SECURE__FTRD__ADDR,0xffffffff);
     rdata8 = ReadReg(DMAC_AXI_NON_SECURE__FTRD__ADDR);//0x038

     WriteReg(DMAC_AXI_NON_SECURE__FTR0__ADDR,0xffffffff);
     rdata9 = ReadReg(DMAC_AXI_NON_SECURE__FTR0__ADDR);//0x040

     WriteReg(DMAC_AXI_NON_SECURE__FTR1__ADDR,0xffffffff);
     rdata10 = ReadReg(DMAC_AXI_NON_SECURE__FTR1__ADDR);//0x044

     WriteReg(DMAC_AXI_NON_SECURE__FTR2__ADDR,0xffffffff);
     rdata11 = ReadReg(DMAC_AXI_NON_SECURE__FTR2__ADDR);//0x048

     WriteReg(DMAC_AXI_NON_SECURE__FTR3__ADDR,0xffffffff);
     rdata12 = ReadReg(DMAC_AXI_NON_SECURE__FTR3__ADDR);//0x04C

     WriteReg(DMAC_AXI_NON_SECURE__FTR4__ADDR,0xffffffff);
     rdata13 = ReadReg(DMAC_AXI_NON_SECURE__FTR4__ADDR);//0x050

     WriteReg(DMAC_AXI_NON_SECURE__FTR5__ADDR,0xffffffff);
     rdata14 = ReadReg(DMAC_AXI_NON_SECURE__FTR5__ADDR);//0x054

     WriteReg(DMAC_AXI_NON_SECURE__FTR6__ADDR,0xffffffff);
     rdata15 = ReadReg(DMAC_AXI_NON_SECURE__FTR6__ADDR);//0x058

     WriteReg(DMAC_AXI_NON_SECURE__FTR7__ADDR,0xffffffff);
     rdata16 = ReadReg(DMAC_AXI_NON_SECURE__FTR7__ADDR);//0x05C

     WriteReg(DMAC_AXI_NON_SECURE__CSR0__ADDR,0xffffffff);
     rdata17 = ReadReg(DMAC_AXI_NON_SECURE__CSR0__ADDR);//0x100

     WriteReg(DMAC_AXI_NON_SECURE__CPC0__ADDR,0xffffffff);
     rdata18 = ReadReg(DMAC_AXI_NON_SECURE__CPC0__ADDR);//0x104

     WriteReg(DMAC_AXI_NON_SECURE__CSR1__ADDR,0xffffffff);
     rdata19 = ReadReg(DMAC_AXI_NON_SECURE__CSR1__ADDR);//0x108

     WriteReg(DMAC_AXI_NON_SECURE__CPC1__ADDR,0xffffffff);
     rdata20 = ReadReg(DMAC_AXI_NON_SECURE__CPC1__ADDR);//0x10C

     WriteReg(DMAC_AXI_NON_SECURE__CSR2__ADDR,0xffffffff);
     rdata21 = ReadReg(DMAC_AXI_NON_SECURE__CSR2__ADDR);//0x110

     WriteReg(DMAC_AXI_NON_SECURE__CPC2__ADDR,0xffffffff);
     rdata22 = ReadReg(DMAC_AXI_NON_SECURE__CPC2__ADDR);//0x114

     WriteReg(DMAC_AXI_NON_SECURE__CSR3__ADDR,0xffffffff);
     rdata23 = ReadReg(DMAC_AXI_NON_SECURE__CSR3__ADDR);//0x118

     WriteReg(DMAC_AXI_NON_SECURE__CPC3__ADDR,0xffffffff);
     rdata24 = ReadReg(DMAC_AXI_NON_SECURE__CPC3__ADDR);//0x11C

     WriteReg(DMAC_AXI_NON_SECURE__CSR4__ADDR,0xffffffff);
     rdata25 = ReadReg(DMAC_AXI_NON_SECURE__CSR4__ADDR);//0x120

     WriteReg(DMAC_AXI_NON_SECURE__CPC4__ADDR,0xffffffff);
     rdata26 = ReadReg(DMAC_AXI_NON_SECURE__CPC4__ADDR);//0x124

     WriteReg(DMAC_AXI_NON_SECURE__CSR5__ADDR,0xffffffff);
     rdata27 = ReadReg(DMAC_AXI_NON_SECURE__CSR5__ADDR);//0x128

     WriteReg(DMAC_AXI_NON_SECURE__CPC5__ADDR,0xffffffff);
     rdata28 = ReadReg(DMAC_AXI_NON_SECURE__CPC5__ADDR);//0x12C

     WriteReg(DMAC_AXI_NON_SECURE__CSR6__ADDR,0xffffffff);
     rdata29 = ReadReg(DMAC_AXI_NON_SECURE__CSR6__ADDR);//0x130

     WriteReg(DMAC_AXI_NON_SECURE__CPC6__ADDR,0xffffffff);
     rdata30 = ReadReg(DMAC_AXI_NON_SECURE__CPC6__ADDR);//0x134

     WriteReg(DMAC_AXI_NON_SECURE__CSR7__ADDR,0xffffffff);
     rdata31 = ReadReg(DMAC_AXI_NON_SECURE__CSR7__ADDR);//0x138

     WriteReg(DMAC_AXI_NON_SECURE__CPC7__ADDR,0xffffffff);
     rdata32 = ReadReg(DMAC_AXI_NON_SECURE__CPC7__ADDR);//0x13C

     WriteReg(DMAC_AXI_NON_SECURE__SAR0__ADDR,0xffffffff);
     rdata33 = ReadReg(DMAC_AXI_NON_SECURE__SAR0__ADDR);//0x400

     WriteReg(DMAC_AXI_NON_SECURE__DAR0__ADDR,0xffffffff);
     rdata34 = ReadReg(DMAC_AXI_NON_SECURE__DAR0__ADDR);//0x404

     WriteReg(DMAC_AXI_NON_SECURE__CCR0__ADDR,0xffffffff);
     rdata35 = ReadReg(DMAC_AXI_NON_SECURE__CCR0__ADDR);//0x408

     WriteReg(DMAC_AXI_NON_SECURE__LC0_0__ADDR,0xffffffff);
     rdata36 = ReadReg(DMAC_AXI_NON_SECURE__LC0_0__ADDR);//0x40C

     WriteReg(DMAC_AXI_NON_SECURE__LC1_0__ADDR,0xffffffff);
     rdata37 = ReadReg(DMAC_AXI_NON_SECURE__LC1_0__ADDR);//0x410

     WriteReg(DMAC_AXI_NON_SECURE__SAR1__ADDR,0xffffffff);
     rdata38 = ReadReg(DMAC_AXI_NON_SECURE__SAR1__ADDR);//0x420

     WriteReg(DMAC_AXI_NON_SECURE__DAR1__ADDR,0xffffffff);
     rdata39 = ReadReg(DMAC_AXI_NON_SECURE__DAR1__ADDR);//0x424

     WriteReg(DMAC_AXI_NON_SECURE__CCR1__ADDR,0xffffffff);
     rdata40 = ReadReg(DMAC_AXI_NON_SECURE__CCR1__ADDR);//0x428

     WriteReg(DMAC_AXI_NON_SECURE__LC0_1__ADDR,0xffffffff);
     rdata41 = ReadReg(DMAC_AXI_NON_SECURE__LC0_1__ADDR);//0x42C

     WriteReg(DMAC_AXI_NON_SECURE__LC1_1__ADDR,0xffffffff);
     rdata42 = ReadReg(DMAC_AXI_NON_SECURE__LC1_1__ADDR);//0x430

     WriteReg(DMAC_AXI_NON_SECURE__SAR2__ADDR,0xffffffff);
     rdata43 = ReadReg(DMAC_AXI_NON_SECURE__SAR2__ADDR);//0x440

     WriteReg(DMAC_AXI_NON_SECURE__DAR2__ADDR,0xffffffff);
     rdata44 = ReadReg(DMAC_AXI_NON_SECURE__DAR2__ADDR);//0x444

     WriteReg(DMAC_AXI_NON_SECURE__CCR2__ADDR,0xffffffff);
     rdata45 = ReadReg(DMAC_AXI_NON_SECURE__CCR2__ADDR);//0x448

     WriteReg(DMAC_AXI_NON_SECURE__LC0_2__ADDR,0xffffffff);
     rdata46 = ReadReg(DMAC_AXI_NON_SECURE__LC0_2__ADDR);//0x44C

     WriteReg(DMAC_AXI_NON_SECURE__LC1_2__ADDR,0xffffffff);
     rdata47 = ReadReg(DMAC_AXI_NON_SECURE__LC1_2__ADDR);//0x450

     WriteReg(DMAC_AXI_NON_SECURE__SAR3__ADDR,0xffffffff);
     rdata48 = ReadReg(DMAC_AXI_NON_SECURE__SAR3__ADDR);//0x460

     WriteReg(DMAC_AXI_NON_SECURE__DAR3__ADDR,0xffffffff);
     rdata49 = ReadReg(DMAC_AXI_NON_SECURE__DAR3__ADDR);//0x464

     WriteReg(DMAC_AXI_NON_SECURE__CCR3__ADDR,0xffffffff);
     rdata50 = ReadReg(DMAC_AXI_NON_SECURE__CCR3__ADDR);//0x468

     WriteReg(DMAC_AXI_NON_SECURE__LC0_3__ADDR,0xffffffff);
     rdata51 = ReadReg(DMAC_AXI_NON_SECURE__LC0_3__ADDR);//0x46C

     WriteReg(DMAC_AXI_NON_SECURE__LC1_3__ADDR,0xffffffff);
     rdata52 = ReadReg(DMAC_AXI_NON_SECURE__LC1_3__ADDR);//0x470

     WriteReg(DMAC_AXI_NON_SECURE__SAR4__ADDR,0xffffffff);
     rdata53 = ReadReg(DMAC_AXI_NON_SECURE__SAR4__ADDR);//0x480

     WriteReg(DMAC_AXI_NON_SECURE__DAR4__ADDR,0xffffffff);
     rdata54 = ReadReg(DMAC_AXI_NON_SECURE__DAR4__ADDR);//0x484

     WriteReg(DMAC_AXI_NON_SECURE__CCR4__ADDR,0xffffffff);
     rdata55 = ReadReg(DMAC_AXI_NON_SECURE__CCR4__ADDR);//0x488

     WriteReg(DMAC_AXI_NON_SECURE__LC0_4__ADDR,0xffffffff);
     rdata56 = ReadReg(DMAC_AXI_NON_SECURE__LC0_4__ADDR);//0x48c

     WriteReg(DMAC_AXI_NON_SECURE__LC1_4__ADDR,0xffffffff);
     rdata57 = ReadReg(DMAC_AXI_NON_SECURE__LC1_4__ADDR);//0x490

     WriteReg(DMAC_AXI_NON_SECURE__SAR5__ADDR,0xffffffff);
     rdata58 = ReadReg(DMAC_AXI_NON_SECURE__SAR5__ADDR);//0x4A0

     WriteReg(DMAC_AXI_NON_SECURE__DAR5__ADDR,0xffffffff);
     rdata59 = ReadReg(DMAC_AXI_NON_SECURE__DAR5__ADDR);//0x4A4

     WriteReg(DMAC_AXI_NON_SECURE__CCR5__ADDR,0xffffffff);
     rdata60 = ReadReg(DMAC_AXI_NON_SECURE__CCR5__ADDR);//0x4A8

     WriteReg(DMAC_AXI_NON_SECURE__LC0_5__ADDR,0xffffffff);
     rdata61 = ReadReg(DMAC_AXI_NON_SECURE__LC0_5__ADDR);//0x4AC

     WriteReg(DMAC_AXI_NON_SECURE__LC1_5__ADDR,0xffffffff);
     rdata62 = ReadReg(DMAC_AXI_NON_SECURE__LC1_5__ADDR);//0x4B0

     WriteReg(DMAC_AXI_NON_SECURE__SAR6__ADDR,0xffffffff);
     rdata63 = ReadReg(DMAC_AXI_NON_SECURE__SAR6__ADDR);//0x4C0

     WriteReg(DMAC_AXI_NON_SECURE__DAR6__ADDR,0xffffffff);
     rdata64 = ReadReg(DMAC_AXI_NON_SECURE__DAR6__ADDR);//0x4C4

     WriteReg(DMAC_AXI_NON_SECURE__CCR6__ADDR,0xffffffff);
     rdata65 = ReadReg(DMAC_AXI_NON_SECURE__CCR6__ADDR);//0x4C8

     WriteReg(DMAC_AXI_NON_SECURE__LC0_6__ADDR,0xffffffff);
     rdata66 = ReadReg(DMAC_AXI_NON_SECURE__LC0_6__ADDR);//0x4CC

     WriteReg(DMAC_AXI_NON_SECURE__LC1_6__ADDR,0xffffffff);
     rdata67 = ReadReg(DMAC_AXI_NON_SECURE__LC1_6__ADDR);//0x4D0

     WriteReg(DMAC_AXI_NON_SECURE__SAR7__ADDR,0xffffffff);
     rdata68 = ReadReg(DMAC_AXI_NON_SECURE__SAR7__ADDR);//0x4E0

     WriteReg(DMAC_AXI_NON_SECURE__DAR7__ADDR,0xffffffff);
     rdata69 = ReadReg(DMAC_AXI_NON_SECURE__DAR7__ADDR);//0x4E4

     WriteReg(DMAC_AXI_NON_SECURE__CCR7__ADDR,0xffffffff);
     rdata70 = ReadReg(DMAC_AXI_NON_SECURE__CCR7__ADDR);//0x4E8

     WriteReg(DMAC_AXI_NON_SECURE__LC0_7__ADDR,0xffffffff);
     rdata71 = ReadReg(DMAC_AXI_NON_SECURE__LC0_7__ADDR);//0x4EC

     WriteReg(DMAC_AXI_NON_SECURE__LC1_7__ADDR,0xffffffff);
     rdata72 = ReadReg(DMAC_AXI_NON_SECURE__LC1_7__ADDR);//0x4F0

     WriteReg(DMAC_AXI_NON_SECURE__DBGSTATUS__ADDR,0xffffffff);
     rdata73 = ReadReg(DMAC_AXI_NON_SECURE__DBGSTATUS__ADDR);//0xD00

     WriteReg(DMAC_AXI_NON_SECURE__DBGCMD__ADDR,0xffffffff);
     rdata74 = ReadReg(DMAC_AXI_NON_SECURE__DBGCMD__ADDR);//0xD04

     WriteReg(DMAC_AXI_NON_SECURE__DBGINST0__ADDR,0xffffffff);
     rdata75 = ReadReg(DMAC_AXI_NON_SECURE__DBGINST0__ADDR);//0xD08

     WriteReg(DMAC_AXI_NON_SECURE__DBGINST1__ADDR,0xffffffff);
     rdata76 = ReadReg(DMAC_AXI_NON_SECURE__DBGINST1__ADDR);//0xD0C

     WriteReg(DMAC_AXI_NON_SECURE__CR0__ADDR,0xffffffff);
     rdata77 = ReadReg(DMAC_AXI_NON_SECURE__CR0__ADDR);//0xE00

     WriteReg(DMAC_AXI_NON_SECURE__CR1__ADDR,0xffffffff);
     rdata78 = ReadReg(DMAC_AXI_NON_SECURE__CR1__ADDR);//0xE04

     WriteReg(DMAC_AXI_NON_SECURE__CR2__ADDR,0xffffffff);
     rdata79 = ReadReg(DMAC_AXI_NON_SECURE__CR2__ADDR);//0xE08

     WriteReg(DMAC_AXI_NON_SECURE__CR3__ADDR,0xffffffff);
     rdata80 = ReadReg(DMAC_AXI_NON_SECURE__CR3__ADDR);//0xE0C

     WriteReg(DMAC_AXI_NON_SECURE__CR4__ADDR,0xffffffff);
     rdata81 = ReadReg(DMAC_AXI_NON_SECURE__CR4__ADDR);//0xE10

     WriteReg(DMAC_AXI_NON_SECURE__CRD__ADDR,0xffffffff);
     rdata82 = ReadReg(DMAC_AXI_NON_SECURE__CRD__ADDR);//0xE14

     WriteReg(DMAC_AXI_NON_SECURE__WD__ADDR,0xffffffff);
     rdata83 = ReadReg(DMAC_AXI_NON_SECURE__WD__ADDR);//0xE80

     WriteReg(DMAC_AXI_NON_SECURE__PERIPH_ID_0__ADDR,0xffffffff);
     rdata84 = ReadReg(DMAC_AXI_NON_SECURE__PERIPH_ID_0__ADDR);//0xFE0

     WriteReg(DMAC_AXI_NON_SECURE__PERIPH_ID_1__ADDR,0xffffffff);
     rdata85 = ReadReg(DMAC_AXI_NON_SECURE__PERIPH_ID_1__ADDR);//0xFE4

     WriteReg(DMAC_AXI_NON_SECURE__PERIPH_ID_2__ADDR,0xffffffff);
     rdata86 = ReadReg(DMAC_AXI_NON_SECURE__PERIPH_ID_2__ADDR);//0xFE8

     WriteReg(DMAC_AXI_NON_SECURE__PERIPH_ID_3__ADDR,0xffffffff);
     rdata87 = ReadReg(DMAC_AXI_NON_SECURE__PERIPH_ID_3__ADDR);//0xFEC

     WriteReg(DMAC_AXI_NON_SECURE__PCELL_ID_0__ADDR,0xffffffff);
     rdata88 = ReadReg(DMAC_AXI_NON_SECURE__PCELL_ID_0__ADDR);//0xFF0

     WriteReg(DMAC_AXI_NON_SECURE__PCELL_ID_1__ADDR,0xffffffff);
     rdata89 = ReadReg(DMAC_AXI_NON_SECURE__PCELL_ID_1__ADDR);//0xFF4

     WriteReg(DMAC_AXI_NON_SECURE__PCELL_ID_2__ADDR,0xffffffff);
     rdata90 = ReadReg(DMAC_AXI_NON_SECURE__PCELL_ID_2__ADDR);//0xFF8

     WriteReg(DMAC_AXI_NON_SECURE__PCELL_ID_3__ADDR,0xffffffff);
     rdata91 = ReadReg(DMAC_AXI_NON_SECURE__PCELL_ID_3__ADDR);//0xFFC
// 2 change to secure mode
      WriteReg(TOP_S__CFG_CTRL2_DMAC_AXI__ADDR, 0x000FFFFF);
      WriteReg(CRP__SRST_CTRL1__ADDR,0xfffffeff);//soft reset
      WriteReg(CRP__SRST_CTRL1__ADDR,0xffffffff);//soft reset release

     WriteReg(DMAC_AXI_SECURE__DSR__ADDR,0xffffffff);
     rdata92 = ReadReg(DMAC_AXI_SECURE__DSR__ADDR);//0x000

     WriteReg(DMAC_AXI_SECURE__DPC__ADDR,0xffffffff);
     rdata93 = ReadReg(DMAC_AXI_SECURE__DPC__ADDR);//0x004

     WriteReg(DMAC_AXI_SECURE__INTEN__ADDR,0xffffffff);
     rdata94 = ReadReg(DMAC_AXI_SECURE__INTEN__ADDR);//0x020

     WriteReg(DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR,0xffffffff);
     rdata95 = ReadReg(DMAC_AXI_SECURE__INT_EVENT_RIS__ADDR);//0x024

     WriteReg(DMAC_AXI_SECURE__INTMIS__ADDR,0xffffffff);
     rdata96 = ReadReg(DMAC_AXI_SECURE__INTMIS__ADDR);//0x028

     WriteReg(DMAC_AXI_SECURE__INTCLR__ADDR,0xffffffff);
     rdata97 = ReadReg(DMAC_AXI_SECURE__INTCLR__ADDR);//0x02C

     WriteReg(DMAC_AXI_SECURE__FSRD__ADDR,0xffffffff);
     rdata98 = ReadReg(DMAC_AXI_SECURE__FSRD__ADDR);//0x030

     WriteReg(DMAC_AXI_SECURE__FSRC__ADDR,0xffffffff);
     rdata99 = ReadReg(DMAC_AXI_SECURE__FSRC__ADDR);//0x034

     WriteReg(DMAC_AXI_SECURE__FTRD__ADDR,0xffffffff);
     rdata100 = ReadReg(DMAC_AXI_SECURE__FTRD__ADDR);//0x038

     WriteReg(DMAC_AXI_SECURE__FTR0__ADDR,0xffffffff);
     rdata101 = ReadReg(DMAC_AXI_SECURE__FTR0__ADDR);//0x040

     WriteReg(DMAC_AXI_SECURE__FTR1__ADDR,0xffffffff);
     rdata102 = ReadReg(DMAC_AXI_SECURE__FTR1__ADDR);//0x044

     WriteReg(DMAC_AXI_SECURE__FTR2__ADDR,0xffffffff);
     rdata103 = ReadReg(DMAC_AXI_SECURE__FTR2__ADDR);//0x048

     WriteReg(DMAC_AXI_SECURE__FTR3__ADDR,0xffffffff);
     rdata104 = ReadReg(DMAC_AXI_SECURE__FTR3__ADDR);//0x04C

     WriteReg(DMAC_AXI_SECURE__FTR4__ADDR,0xffffffff);
     rdata105 = ReadReg(DMAC_AXI_SECURE__FTR4__ADDR);//0x050

     WriteReg(DMAC_AXI_SECURE__FTR5__ADDR,0xffffffff);
     rdata106 = ReadReg(DMAC_AXI_SECURE__FTR5__ADDR);//0x054

     WriteReg(DMAC_AXI_SECURE__FTR6__ADDR,0xffffffff);
     rdata107 = ReadReg(DMAC_AXI_SECURE__FTR6__ADDR);//0x058

     WriteReg(DMAC_AXI_SECURE__FTR7__ADDR,0xffffffff);
     rdata108 = ReadReg(DMAC_AXI_SECURE__FTR7__ADDR);//0x05C

     WriteReg(DMAC_AXI_SECURE__CSR0__ADDR,0xffffffff);
     rdata109 = ReadReg(DMAC_AXI_SECURE__CSR0__ADDR);//0x100

     WriteReg(DMAC_AXI_SECURE__CPC0__ADDR,0xffffffff);
     rdata110 = ReadReg(DMAC_AXI_SECURE__CPC0__ADDR);//0x104

     WriteReg(DMAC_AXI_SECURE__CSR1__ADDR,0xffffffff);
     rdata111 = ReadReg(DMAC_AXI_SECURE__CSR1__ADDR);//0x108

     WriteReg(DMAC_AXI_SECURE__CPC1__ADDR,0xffffffff);
     rdata112 = ReadReg(DMAC_AXI_SECURE__CPC1__ADDR);//0x10C

     WriteReg(DMAC_AXI_SECURE__CSR2__ADDR,0xffffffff);
     rdata113 = ReadReg(DMAC_AXI_SECURE__CSR2__ADDR);//0x110

     WriteReg(DMAC_AXI_SECURE__CPC2__ADDR,0xffffffff);
     rdata114 = ReadReg(DMAC_AXI_SECURE__CPC2__ADDR);//0x114

     WriteReg(DMAC_AXI_SECURE__CSR3__ADDR,0xffffffff);
     rdata115 = ReadReg(DMAC_AXI_SECURE__CSR3__ADDR);//0x118

     WriteReg(DMAC_AXI_SECURE__CPC3__ADDR,0xffffffff);
     rdata116 = ReadReg(DMAC_AXI_SECURE__CPC3__ADDR);//0x11C

     WriteReg(DMAC_AXI_SECURE__CSR4__ADDR,0xffffffff);
     rdata117 = ReadReg(DMAC_AXI_SECURE__CSR4__ADDR);//0x120

     WriteReg(DMAC_AXI_SECURE__CPC4__ADDR,0xffffffff);
     rdata118 = ReadReg(DMAC_AXI_SECURE__CPC4__ADDR);//0x124

     WriteReg(DMAC_AXI_SECURE__CSR5__ADDR,0xffffffff);
     rdata119 = ReadReg(DMAC_AXI_SECURE__CSR5__ADDR);//0x128

     WriteReg(DMAC_AXI_SECURE__CPC5__ADDR,0xffffffff);
     rdata120 = ReadReg(DMAC_AXI_SECURE__CPC5__ADDR);//0x12C

     WriteReg(DMAC_AXI_SECURE__CSR6__ADDR,0xffffffff);
     rdata121 = ReadReg(DMAC_AXI_SECURE__CSR6__ADDR);//0x130

     WriteReg(DMAC_AXI_SECURE__CPC6__ADDR,0xffffffff);
     rdata122 = ReadReg(DMAC_AXI_SECURE__CPC6__ADDR);//0x134

     WriteReg(DMAC_AXI_SECURE__CSR7__ADDR,0xffffffff);
     rdata123 = ReadReg(DMAC_AXI_SECURE__CSR7__ADDR);//0x138

     WriteReg(DMAC_AXI_SECURE__CPC7__ADDR,0xffffffff);
     rdata124 = ReadReg(DMAC_AXI_SECURE__CPC7__ADDR);//0x13C

     WriteReg(DMAC_AXI_SECURE__SAR0__ADDR,0xffffffff);
     rdata125 = ReadReg(DMAC_AXI_SECURE__SAR0__ADDR);//0x400

     WriteReg(DMAC_AXI_SECURE__DAR0__ADDR,0xffffffff);
     rdata126 = ReadReg(DMAC_AXI_SECURE__DAR0__ADDR);//0x404

     WriteReg(DMAC_AXI_SECURE__CCR0__ADDR,0xffffffff);
     rdata127 = ReadReg(DMAC_AXI_SECURE__CCR0__ADDR);//0x408

     WriteReg(DMAC_AXI_SECURE__LC0_0__ADDR,0xffffffff);
     rdata128 = ReadReg(DMAC_AXI_SECURE__LC0_0__ADDR);//0x40C

     WriteReg(DMAC_AXI_SECURE__LC1_0__ADDR,0xffffffff);
     rdata129 = ReadReg(DMAC_AXI_SECURE__LC1_0__ADDR);//0x410

     WriteReg(DMAC_AXI_SECURE__SAR1__ADDR,0xffffffff);
     rdata130 = ReadReg(DMAC_AXI_SECURE__SAR1__ADDR);//0x420

     WriteReg(DMAC_AXI_SECURE__DAR1__ADDR,0xffffffff);
     rdata131 = ReadReg(DMAC_AXI_SECURE__DAR1__ADDR);//0x424

     WriteReg(DMAC_AXI_SECURE__CCR1__ADDR,0xffffffff);
     rdata132 = ReadReg(DMAC_AXI_SECURE__CCR1__ADDR);//0x428

     WriteReg(DMAC_AXI_SECURE__LC0_1__ADDR,0xffffffff);
     rdata133 = ReadReg(DMAC_AXI_SECURE__LC0_1__ADDR);//0x42C

     WriteReg(DMAC_AXI_SECURE__LC1_1__ADDR,0xffffffff);
     rdata134 = ReadReg(DMAC_AXI_SECURE__LC1_1__ADDR);//0x430

     WriteReg(DMAC_AXI_SECURE__SAR2__ADDR,0xffffffff);
     rdata135 = ReadReg(DMAC_AXI_SECURE__SAR2__ADDR);//0x440

     WriteReg(DMAC_AXI_SECURE__DAR2__ADDR,0xffffffff);
     rdata136 = ReadReg(DMAC_AXI_SECURE__DAR2__ADDR);//0x444

     WriteReg(DMAC_AXI_SECURE__CCR2__ADDR,0xffffffff);
     rdata137 = ReadReg(DMAC_AXI_SECURE__CCR2__ADDR);//0x448

     WriteReg(DMAC_AXI_SECURE__LC0_2__ADDR,0xffffffff);
     rdata138 = ReadReg(DMAC_AXI_SECURE__LC0_2__ADDR);//0x44C

     WriteReg(DMAC_AXI_SECURE__LC1_2__ADDR,0xffffffff);
     rdata139 = ReadReg(DMAC_AXI_SECURE__LC1_2__ADDR);//0x450

     WriteReg(DMAC_AXI_SECURE__SAR3__ADDR,0xffffffff);
     rdata140 = ReadReg(DMAC_AXI_SECURE__SAR3__ADDR);//0x460

     WriteReg(DMAC_AXI_SECURE__DAR3__ADDR,0xffffffff);
     rdata141 = ReadReg(DMAC_AXI_SECURE__DAR3__ADDR);//0x464

     WriteReg(DMAC_AXI_SECURE__CCR3__ADDR,0xffffffff);
     rdata142 = ReadReg(DMAC_AXI_SECURE__CCR3__ADDR);//0x468

     WriteReg(DMAC_AXI_SECURE__LC0_3__ADDR,0xffffffff);
     rdata143 = ReadReg(DMAC_AXI_SECURE__LC0_3__ADDR);//0x46C

     WriteReg(DMAC_AXI_SECURE__LC1_3__ADDR,0xffffffff);
     rdata144 = ReadReg(DMAC_AXI_SECURE__LC1_3__ADDR);//0x470

     WriteReg(DMAC_AXI_SECURE__SAR4__ADDR,0xffffffff);
     rdata145 = ReadReg(DMAC_AXI_SECURE__SAR4__ADDR);//0x480

     WriteReg(DMAC_AXI_SECURE__DAR4__ADDR,0xffffffff);
     rdata146 = ReadReg(DMAC_AXI_SECURE__DAR4__ADDR);//0x484

     WriteReg(DMAC_AXI_SECURE__CCR4__ADDR,0xffffffff);
     rdata147 = ReadReg(DMAC_AXI_SECURE__CCR4__ADDR);//0x488

     WriteReg(DMAC_AXI_SECURE__LC0_4__ADDR,0xffffffff);
     rdata148 = ReadReg(DMAC_AXI_SECURE__LC0_4__ADDR);//0x48c

     WriteReg(DMAC_AXI_SECURE__LC1_4__ADDR,0xffffffff);
     rdata149 = ReadReg(DMAC_AXI_SECURE__LC1_4__ADDR);//0x490

     WriteReg(DMAC_AXI_SECURE__SAR5__ADDR,0xffffffff);
     rdata150 = ReadReg(DMAC_AXI_SECURE__SAR5__ADDR);//0x4A0

     WriteReg(DMAC_AXI_SECURE__DAR5__ADDR,0xffffffff);
     rdata151 = ReadReg(DMAC_AXI_SECURE__DAR5__ADDR);//0x4A4

     WriteReg(DMAC_AXI_SECURE__CCR5__ADDR,0xffffffff);
     rdata152 = ReadReg(DMAC_AXI_SECURE__CCR5__ADDR);//0x4A8

     WriteReg(DMAC_AXI_SECURE__LC0_5__ADDR,0xffffffff);
     rdata153 = ReadReg(DMAC_AXI_SECURE__LC0_5__ADDR);//0x4AC

     WriteReg(DMAC_AXI_SECURE__LC1_5__ADDR,0xffffffff);
     rdata154 = ReadReg(DMAC_AXI_SECURE__LC1_5__ADDR);//0x4B0

     WriteReg(DMAC_AXI_SECURE__SAR6__ADDR,0xffffffff);
     rdata155 = ReadReg(DMAC_AXI_SECURE__SAR6__ADDR);//0x4C0

     WriteReg(DMAC_AXI_SECURE__DAR6__ADDR,0xffffffff);
     rdata156 = ReadReg(DMAC_AXI_SECURE__DAR6__ADDR);//0x4C4

     WriteReg(DMAC_AXI_SECURE__CCR6__ADDR,0xffffffff);
     rdata157 = ReadReg(DMAC_AXI_SECURE__CCR6__ADDR);//0x4C8

     WriteReg(DMAC_AXI_SECURE__LC0_6__ADDR,0xffffffff);
     rdata158 = ReadReg(DMAC_AXI_SECURE__LC0_6__ADDR);//0x4CC

     WriteReg(DMAC_AXI_SECURE__LC1_6__ADDR,0xffffffff);
     rdata159 = ReadReg(DMAC_AXI_SECURE__LC1_6__ADDR);//0x4D0

     WriteReg(DMAC_AXI_SECURE__SAR7__ADDR,0xffffffff);
     rdata160 = ReadReg(DMAC_AXI_SECURE__SAR7__ADDR);//0x4E0

     WriteReg(DMAC_AXI_SECURE__DAR7__ADDR,0xffffffff);
     rdata161 = ReadReg(DMAC_AXI_SECURE__DAR7__ADDR);//0x4E4

     WriteReg(DMAC_AXI_SECURE__CCR7__ADDR,0xffffffff);
     rdata162 = ReadReg(DMAC_AXI_SECURE__CCR7__ADDR);//0x4E8

     WriteReg(DMAC_AXI_SECURE__LC0_7__ADDR,0xffffffff);
     rdata163 = ReadReg(DMAC_AXI_SECURE__LC0_7__ADDR);//0x4EC

     WriteReg(DMAC_AXI_SECURE__LC1_7__ADDR,0xffffffff);
     rdata164 = ReadReg(DMAC_AXI_SECURE__LC1_7__ADDR);//0x4F0

     WriteReg(DMAC_AXI_SECURE__DBGSTATUS__ADDR,0xffffffff);
     rdata165 = ReadReg(DMAC_AXI_SECURE__DBGSTATUS__ADDR);//0xD00

     WriteReg(DMAC_AXI_SECURE__DBGCMD__ADDR,0xffffffff);
     rdata166 = ReadReg(DMAC_AXI_SECURE__DBGCMD__ADDR);//0xD04

     WriteReg(DMAC_AXI_SECURE__DBGINST0__ADDR,0xffffffff);
     rdata167 = ReadReg(DMAC_AXI_SECURE__DBGINST0__ADDR);//0xD08

     WriteReg(DMAC_AXI_SECURE__DBGINST1__ADDR,0xffffffff);
     rdata168 = ReadReg(DMAC_AXI_SECURE__DBGINST1__ADDR);//0xD0C

     WriteReg(DMAC_AXI_SECURE__CR0__ADDR,0xffffffff);
     rdata169 = ReadReg(DMAC_AXI_SECURE__CR0__ADDR);//0xE00

     WriteReg(DMAC_AXI_SECURE__CR1__ADDR,0xffffffff);
     rdata170 = ReadReg(DMAC_AXI_SECURE__CR1__ADDR);//0xE04

     WriteReg(DMAC_AXI_SECURE__CR2__ADDR,0xffffffff);
     rdata171 = ReadReg(DMAC_AXI_SECURE__CR2__ADDR);//0xE08

     WriteReg(DMAC_AXI_SECURE__CR3__ADDR,0xffffffff);
     rdata172 = ReadReg(DMAC_AXI_SECURE__CR3__ADDR);//0xE0C

     WriteReg(DMAC_AXI_SECURE__CR4__ADDR,0xffffffff);
     rdata173 = ReadReg(DMAC_AXI_SECURE__CR4__ADDR);//0xE10

     WriteReg(DMAC_AXI_SECURE__CRD__ADDR,0xffffffff);
     rdata174 = ReadReg(DMAC_AXI_SECURE__CRD__ADDR);//0xE14

     WriteReg(DMAC_AXI_SECURE__WD__ADDR,0xffffffff);
     rdata175 = ReadReg(DMAC_AXI_SECURE__WD__ADDR);//0xE80

     WriteReg(DMAC_AXI_SECURE__PERIPH_ID_0__ADDR,0xffffffff);
     rdata176 = ReadReg(DMAC_AXI_SECURE__PERIPH_ID_0__ADDR);//0xFE0

     WriteReg(DMAC_AXI_SECURE__PERIPH_ID_1__ADDR,0xffffffff);
     rdata177 = ReadReg(DMAC_AXI_SECURE__PERIPH_ID_1__ADDR);//0xFE4

     WriteReg(DMAC_AXI_SECURE__PERIPH_ID_2__ADDR,0xffffffff);
     rdata178 = ReadReg(DMAC_AXI_SECURE__PERIPH_ID_2__ADDR);//0xFE8

     WriteReg(DMAC_AXI_SECURE__PERIPH_ID_3__ADDR,0xffffffff);
     rdata179 = ReadReg(DMAC_AXI_SECURE__PERIPH_ID_3__ADDR);//0xFEC

     WriteReg(DMAC_AXI_SECURE__PCELL_ID_0__ADDR,0xffffffff);
     rdata180 = ReadReg(DMAC_AXI_SECURE__PCELL_ID_0__ADDR);//0xFF0

     WriteReg(DMAC_AXI_SECURE__PCELL_ID_1__ADDR,0xffffffff);
     rdata181 = ReadReg(DMAC_AXI_SECURE__PCELL_ID_1__ADDR);//0xFF4

     WriteReg(DMAC_AXI_SECURE__PCELL_ID_2__ADDR,0xffffffff);
     rdata182 = ReadReg(DMAC_AXI_SECURE__PCELL_ID_2__ADDR);//0xFF8

     WriteReg(DMAC_AXI_SECURE__PCELL_ID_3__ADDR,0xffffffff);
     rdata183 = ReadReg(DMAC_AXI_SECURE__PCELL_ID_3__ADDR);//0xFFC

    fail_flag = 0;
    if (rdata0 != 0x00000200 ) {//DSR default 0x0000_0000
      vfwp("**DSR(rdata0) WRITE FAIL **%x: ",0x000);
      vfwp("%x\n", rdata0);
      fail_flag = 1;
     }
    if (rdata1 != 0x00000000 ) {//0x004
      vfwp("** CPU0 TEST FAIL **%x ",0x004);
	vfwp("%x\n", rdata1);
      fail_flag = 1;
     }
    if (rdata2 != 0x0000ffff ) {//0x020
      vfwp("** rdata2 TEST FAIL **%x: ",0x020);
	vfwp("%x\n", rdata2);
      fail_flag = 1;
     }
    if (rdata3 != 0x00000000 ) {//0x024
      vfwp("** CPU0 TEST FAIL **%x ",0x024);
      vfwp("%x\n", rdata3);
	fail_flag = 1;
     }
    if (rdata4 != 0x00000000 ) {//0x028
      vfwp("** CPU0 TEST FAIL **%x ",0x028);
	vfwp("%x\n", rdata4);
      fail_flag = 1;
     }
    if (rdata5 != 0x00000000 ) {//0x02C
      vfwp("** CPU0 TEST FAIL **%x ",0x02C);
	vfwp("%x\n", rdata5);
      fail_flag = 1;
     }
    if (rdata6 != 0x00000000 ) {//0x030
      vfwp("** CPU0 TEST FAIL **%x ",0x030);
	vfwp("%x\n", rdata6);
      fail_flag = 1;
     }
    if (rdata7 != 0x00000000 ) {//0x034
      vfwp("** CPU0 TEST FAIL **%x ",0x034);
	vfwp("%x\n", rdata7);
      fail_flag = 1;
     }
    if (rdata8 != 0x00000000 ) {//0x038
      vfwp("** CPU0 TEST FAIL **%x ",0x038);
	vfwp("%x\n", rdata8);
      fail_flag = 1;
     }
    if (rdata9 != 0x00000000 ) {//0x040
      vfwp("** CPU0 TEST FAIL **%x ",0x040);
	vfwp("%x\n", rdata9);
      fail_flag = 1;
     }
    if (rdata10 != 0x00000000 ) {//0x044
      vfwp("** CPU0 TEST FAIL **%x ",0x044);
	vfwp("%x\n", rdata10);
	fail_flag = 1;
     }
    if (rdata11 != 0x00000000 ) {//0x048
      vfwp("** CPU0 TEST FAIL **%x ",0x048);
      	vfwp("%x\n", rdata11);
	fail_flag = 1;
     }
    if (rdata12 != 0x00000000 ) {//0x04C
      vfwp("** CPU0 TEST FAIL **%x",0x04C);
      vfwp("%x\n", rdata12);
	fail_flag = 1;
     }
    if (rdata13 != 0x00000000 ) {//0x050
      vfwp("** CPU0 TEST FAIL **%x",0x050);
      vfwp("%x\n", rdata13);
	fail_flag = 1;
     }
    if (rdata14 != 0x00000000 ) {//0x054
      vfwp("** CPU0 TEST FAIL **%x",0x054);
      vfwp("%x\n", rdata14);
	fail_flag = 1;
     }
    if (rdata15 != 0x00000000 ) {//0x058
      vfwp("** CPU0 TEST FAIL **%x",0x058);
	vfwp("%x\n", rdata15);
	fail_flag = 1;
     }
    if (rdata16 != 0x00000000 ) {//0x05C
      vfwp("** CPU0 TEST FAIL **%x",0x05C);
      vfwp("%x\n", rdata16);
	fail_flag = 1;
     }
    if (rdata17 != 0x00000000 ) {//0x100
      vfwp("** CPU0 TEST FAIL **%x",0x100);
      vfwp("%x\n", rdata17);
	fail_flag = 1;
     }
    if (rdata18 != 0x00000000 ) {//0x104
      vfwp("** CPU0 TEST FAIL **%x",0x104);
      vfwp("%x\n", rdata18);
	fail_flag = 1;
     }
    if (rdata19 != 0x00000000 ) {//0x108
      vfwp("** CPU0 TEST FAIL **%x",0x108);
      vfwp("%x\n", rdata19);
	fail_flag = 1;
     }
    if (rdata20 != 0x00000000 ) {//0x10C
      vfwp("** CPU0 TEST FAIL **%x",0x10C);
      vfwp("%x\n", rdata20);
	fail_flag = 1;
     }
    if (rdata21 != 0x00000000 ) {//0x110
      vfwp("** CPU0 TEST FAIL **%x",0x110);
      vfwp("%x\n", rdata21);
	fail_flag = 1;
     }
    if (rdata22 != 0x00000000 ) {//0x114
      vfwp("** CPU0 TEST FAIL **%x",0x114);
      vfwp("%x\n", rdata22);
	fail_flag = 1;
     }
    if (rdata23 != 0x00000000 ) {//0x118
      vfwp("** CPU0 TEST FAIL **%x",0x118);
      vfwp("%x\n", rdata23);
	fail_flag = 1;
     }
    if (rdata24 != 0x00000000 ) {//0x11C
      vfwp("** CPU0 TEST FAIL **%x",0x11C);
      vfwp("%x\n", rdata24);
	fail_flag = 1;
     }
    if (rdata25 != 0x00000000 ) {//0x120
      vfwp("** CPU0 TEST FAIL **%x",0x120);
      fail_flag = 1;
     }
    if (rdata26 != 0x00000000 ) {//0x124
      vfwp("** CPU0 TEST FAIL **%x",0x124);
      fail_flag = 1;
     }
    if (rdata27 != 0x00000000 ) {//0x128
      vfwp("** CPU0 TEST FAIL **%x",0x128);
      fail_flag = 1;
     }
    if (rdata28 != 0x00000000 ) {//0x12C
      vfwp("** CPU0 TEST FAIL **%x",0x12C);
      fail_flag = 1;
     }
    if (rdata29 != 0x00000000 ) {//0x130
      vfwp("** CPU0 TEST FAIL **%x",0x130);
      fail_flag = 1;
     }
    if (rdata30 != 0x00000000 ) {//0x134
      vfwp("** CPU0 TEST FAIL **%x",0x134);
      fail_flag = 1;
     }
    if (rdata31 != 0x00000000 ) {//0x138
      vfwp("** CPU0 TEST FAIL **%x",0x138);
      fail_flag = 1;
     }
    if (rdata32 != 0x00000000 ) {//0x13C
      vfwp("** CPU0 TEST FAIL **%x",0x13C);
      fail_flag = 1;
     }
    if (rdata33 != 0x00000000 ) {//0x400
      vfwp("** CPU0 TEST FAIL **%x",0x400);
      fail_flag = 1;
     }
    if (rdata34 != 0x00000000 ) {//0x404
      vfwp("** CPU0 TEST FAIL **%x",0x404);
      fail_flag = 1;
     }
    if (rdata35 != 0x00000000 ) {//0x408
      vfwp("** CPU0 TEST FAIL **%x",0x408);
      fail_flag = 1;
     }
    if (rdata36 != 0x00000000 ) {//0x40C
      vfwp("** CPU0 TEST FAIL **%x",0x40C);
      fail_flag = 1;
     }
    if (rdata37 != 0x00000000 ) {//0x410
      vfwp("** CPU0 TEST FAIL **%x",0x410);
      fail_flag = 1;
     }
    if (rdata38 != 0x00000000 ) {//0x420
      vfwp("** CPU0 TEST FAIL **%x",0x420);
      fail_flag = 1;
     }
    if (rdata39 != 0x00000000 ) {//0x424
      vfwp("** CPU0 TEST FAIL **%x",0x424);
      fail_flag = 1;
     }
    if (rdata40 != 0x00000000 ) {//0x428
      vfwp("** CPU0 TEST FAIL **%x",0x428);
      fail_flag = 1;
     }
    if (rdata41 != 0x00000000 ) {//0x42C
      vfwp("** CPU0 TEST FAIL **%x",0x42C);
      fail_flag = 1;
     }
    if (rdata42 != 0x00000000 ) {//0x430
      vfwp("** CPU0 TEST FAIL **%x",0x430);
      fail_flag = 1;
     }
    if (rdata43 != 0x00000000 ) {//0x440
      vfwp("** CPU0 TEST FAIL **%x",0x440);
      fail_flag = 1;
     }
    if (rdata44 != 0x00000000 ) {//0x444
      vfwp("** CPU0 TEST FAIL **%x",0x444);
      fail_flag = 1;
     }
    if (rdata45 != 0x00000000 ) {//0x448
      vfwp("** CPU0 TEST FAIL **%x",0x448);
      fail_flag = 1;
     }
    if (rdata46 != 0x00000000 ) {//0x44C
      vfwp("** CPU0 TEST FAIL **%x",0x44C);
      fail_flag = 1;
     }
    if (rdata47 != 0x00000000 ) {//0x450
      vfwp("** CPU0 TEST FAIL **%x",0x450);
      fail_flag = 1;
     }
    if (rdata48 != 0x00000000 ) {//0x460
      vfwp("** CPU0 TEST FAIL **%x",0x460);
      fail_flag = 1;
     }
    if (rdata49 != 0x00000000 ) {//0x464
      vfwp("** CPU0 TEST FAIL **%x",0x464);
      fail_flag = 1;
     }
    if (rdata50 != 0x00000000 ) {//0x468
      vfwp("** CPU0 TEST FAIL **%x",0x468);
      fail_flag = 1;
     }
    if (rdata51 != 0x00000000 ) {//0x46C
      vfwp("** CPU0 TEST FAIL **%x",0x46C);
      fail_flag = 1;
     }
    if (rdata52 != 0x00000000 ) {//0x470
      vfwp("** CPU0 TEST FAIL **%x",0x470);
      fail_flag = 1;
     }
    if (rdata53 != 0x00000000 ) {//0x480
      vfwp("** CPU0 TEST FAIL **%x",0x480);
      fail_flag = 1;
     }
    if (rdata54 != 0x00000000 ) {//0x484
      vfwp("** CPU0 TEST FAIL **%x",0x484);
      fail_flag = 1;
     }
    if (rdata55 != 0x00000000 ) {//0x488
      vfwp("** CPU0 TEST FAIL **%x",0x488);
      fail_flag = 1;
     }
    if (rdata56 != 0x00000000 ) {//0x48c
      vfwp("** CPU0 TEST FAIL **%x",0x48c);
      fail_flag = 1;
     }
    if (rdata57 != 0x00000000 ) {//0x490
      vfwp("** CPU0 TEST FAIL **%x",0x490);
      fail_flag = 1;
     }
    if (rdata58 != 0x00000000 ) {//0x4A0
      vfwp("** CPU0 TEST FAIL **%x",0x4A0);
      fail_flag = 1;
     }
    if (rdata59 != 0x00000000 ) {//0x4A4
      vfwp("** CPU0 TEST FAIL **%x",0x4A4);
      fail_flag = 1;
     }
    if (rdata60 != 0x00000000 ) {//0x4A8
      vfwp("** CPU0 TEST FAIL **%x",0x4A8);
      fail_flag = 1;
     }
    if (rdata61 != 0x00000000 ) {//0x4AC
      vfwp("** CPU0 TEST FAIL **%x",0x4AC);
      fail_flag = 1;
     }
    if (rdata62 != 0x00000000 ) {//0x4B0
      vfwp("** CPU0 TEST FAIL **%x",0x4B0);
      fail_flag = 1;
     }
    if (rdata63 != 0x00000000 ) {//0x4C0
      vfwp("** CPU0 TEST FAIL **%x",0x4C0);
      fail_flag = 1;
     }
    if (rdata64 != 0x00000000 ) {//0x4C4
      vfwp("** CPU0 TEST FAIL **%x",0x4C4);
      fail_flag = 1;
     }
    if (rdata65 != 0x00000000 ) {//0x4C8
      vfwp("** CPU0 TEST FAIL **%x",0x4C8);
      fail_flag = 1;
     }
    if (rdata66 != 0x00000000 ) {//0x4CC
      vfwp("** CPU0 TEST FAIL **%x",0x4CC);
      fail_flag = 1;
     }
    if (rdata67 != 0x00000000 ) {//0x4D0
      vfwp("** CPU0 TEST FAIL **%x",0x4D0);
      fail_flag = 1;
     }
    if (rdata68 != 0x00000000 ) {//0x4E0
      vfwp("** CPU0 TEST FAIL **%x",0x4E0);
      fail_flag = 1;
     }
    if (rdata69 != 0x00000000 ) {//0x4E4
      vfwp("** CPU0 TEST FAIL **%x",0x4E4);
      fail_flag = 1;
     }
    if (rdata70 != 0x00000000 ) {//0x4E8
      vfwp("** CPU0 TEST FAIL **%x",0x4E8);
      fail_flag = 1;
     }
    if (rdata71 != 0x00000000 ) {//0x4EC
      vfwp("** CPU0 TEST FAIL **%x",0x4EC);
      fail_flag = 1;
     }
    if (rdata72 != 0x00000000 ) {//0x4F0
      vfwp("** CPU0 TEST FAIL **%x",0x4F0);
      fail_flag = 1;
     }
    if (rdata73 != 0x00000000 ) {//0xD00
      vfwp("** CPU0 TEST FAIL **%x",0xD00);
      fail_flag = 1;
     }
    if (rdata74 != 0x00000000 ) {//0xD04
      vfwp("** CPU0 TEST FAIL **%x",0xD04);
      fail_flag = 1;
     }
    if (rdata75 != 0x00000000 ) {//0xD08
      vfwp("** CPU0 TEST FAIL **%x",0xD08);
      fail_flag = 1;
     }
    if (rdata76 != 0x00000000 ) {//0xD0C
      vfwp("** CPU0 TEST FAIL **%x",0xD0C);
      fail_flag = 1;
     }
    if (rdata77 != 0x001e3075 ) {//0xE00 CR0 default 0x0000_0000
      vfwp("** CR0(rdata77) TEST FAIL **%x: ",0xE00);
      vfwp("%x\n", rdata77);
	fail_flag = 1;
     }
    if (rdata78 != 0x000000f5 ) {//0xE04
      vfwp("** 0xE04(rdata78) TEST FAIL **%x: ",0xE04);
      vfwp("%x\n", rdata78);
	fail_flag = 1;
     }
    if (rdata79 != 0x00000000 ) {//0xE08
      vfwp("** CPU0 TEST FAIL **%x",0xE08);
      fail_flag = 1;
     }
    if (rdata80 != 0x0000ffff ) {//0xE0C
      vfwp("** rdata80 TEST FAIL **%x: ",0xE0C);
      vfwp("%x\n", rdata80);
      fail_flag = 1;
     }
    if (rdata81 != 0x0000000f ) {//0xE10
      vfwp("** rdata81 TEST FAIL **%x: ",0xE10);
      vfwp("%x\n", rdata81);
      fail_flag = 1;
     }
    if (rdata82 != 0x00ff7f74 ) {//0xE14
      vfwp("** CPU0 TEST FAIL **%x: ",0xE14);
      vfwp("%x\n", rdata82);
	fail_flag = 1;
     }
    if (rdata83 != 0x00000001 ) {//0xE80
      vfwp("** Watchdog reg(rdata83) TEST FAIL **%x: ",0xE80);
      vfwp("%x\n", rdata83);
      fail_flag = 1;
     }
    if (rdata84 != 0x00000030 ) {//0xFE0
      vfwp("** CPU0 TEST FAIL **%x: ",0xFE0);
      vfwp("%x\n", rdata84);
	fail_flag = 1;
     }
    if (rdata85 != 0x00000013 ) {//0xFE4
      vfwp("** CPU0 TEST FAIL **%x: ",0xFE4);
      vfwp("%x\n", rdata85);
	fail_flag = 1;
     }
    if (rdata86 != 0x00000034 ) {//0xFE8
      vfwp("** CPU0 TEST FAIL **%x: ",0xFE8);
      vfwp("%x\n", rdata86);
	fail_flag = 1;
     }
    if (rdata87 != 0x00000000 ) {//0xFEC
      vfwp("** CPU0 TEST FAIL **%x",0xFEC);
      fail_flag = 1;
     }
    if (rdata88 != 0x0000000d ) {//0xFF0
      vfwp("** CPU0 TEST FAIL **%x: ",0xFF0);
      vfwp("%x\n", rdata88);
	fail_flag = 1;
     }
    if (rdata89 != 0x000000f0 ) {//0xFF4
      vfwp("** CPU0 TEST FAIL **%x: ",0xFF4);
      vfwp("%x\n", rdata89);
	fail_flag = 1;
     }
    if (rdata90 != 0x00000005 ) {//0xFF8
      vfwp("** CPU0 TEST FAIL **%x: ",0xFF8);
	vfwp("%x\n", rdata90);
	fail_flag = 1;
     }
    if (rdata91 != 0x000000b1 ) {//0xFFC
      vfwp("** CPU0 TEST FAIL **%x: ",0xFFC);
      vfwp("%x\n", rdata91);
	fail_flag = 1;
     }
    if (rdata92 != 0x00000000 ) {//0x000
      vfwp("** CPU0 TEST FAIL **%x",0x000);
      fail_flag = 1;
     }
    if (rdata93 != 0x00000000 ) {//0x004
      vfwp("** CPU0 TEST FAIL **%x",0x004);
      fail_flag = 1;
     }
    if (rdata94 != 0x0000ffff ) {//0x020
      vfwp("** rdata94 TEST FAIL **%x: ",0x020);
      vfwp("%x\n", rdata94);
      fail_flag = 1;
     }
    if (rdata95 != 0x00000000 ) {//0x024
      vfwp("** CPU0 TEST FAIL **%x",0x024);
      fail_flag = 1;
     }
    if (rdata96 != 0x00000000 ) {//0x028
      vfwp("** CPU0 TEST FAIL **%x",0x028);
      fail_flag = 1;
     }
    if (rdata97 != 0x00000000 ) {//0x02C
      vfwp("** CPU0 TEST FAIL **%x",0x02C);
      fail_flag = 1;
     }
    if (rdata98 != 0x00000000 ) {//0x030
      vfwp("** CPU0 TEST FAIL **%x",0x030);
      fail_flag = 1;
     }
    if (rdata99 != 0x00000000 ) {//0x034
      vfwp("** CPU0 TEST FAIL **%x",0x034);
      fail_flag = 1;
     }
    if (rdata100 != 0x00000000 ) {//0x038
      vfwp("** CPU0 TEST FAIL **%x",0x038);
      fail_flag = 1;
     }
    if (rdata101 != 0x00000000 ) {//0x040
      vfwp("** CPU0 TEST FAIL **%x",0x040);
      fail_flag = 1;
     }
    if (rdata102 != 0x00000000 ) {//0x044
      vfwp("** CPU0 TEST FAIL **%x",0x044);
      fail_flag = 1;
     }
    if (rdata103 != 0x00000000 ) {//0x048
      vfwp("** CPU0 TEST FAIL **%x",0x048);
      fail_flag = 1;
     }
    if (rdata104 != 0x00000000 ) {//0x04C
      vfwp("** CPU0 TEST FAIL **%x",0x04C);
      fail_flag = 1;
     }
    if (rdata105 != 0x00000000 ) {//0x050
      vfwp("** CPU0 TEST FAIL **%x",0x050);
      fail_flag = 1;
     }
    if (rdata106 != 0x00000000 ) {//0x054
      vfwp("** CPU0 TEST FAIL **%x",0x054);
      fail_flag = 1;
     }
    if (rdata107 != 0x00000000 ) {//0x058
      vfwp("** CPU0 TEST FAIL **%x",0x058);
      fail_flag = 1;
     }
    if (rdata108 != 0x00000000 ) {//0x05C
      vfwp("** CPU0 TEST FAIL **%x",0x05C);
      fail_flag = 1;
     }
    if (rdata109 != 0x00000000 ) {//0x100
      vfwp("** CPU0 TEST FAIL **%x",0x100);
      fail_flag = 1;
     }
    if (rdata110 != 0x00000000 ) {//0x104
      vfwp("** CPU0 TEST FAIL **%x",0x104);
      fail_flag = 1;
     }
    if (rdata111 != 0x00000000 ) {//0x108
      vfwp("** CPU0 TEST FAIL **%x",0x108);
      fail_flag = 1;
     }
    if (rdata112 != 0x00000000 ) {//0x10C
      vfwp("** CPU0 TEST FAIL **%x",0x10C);
      fail_flag = 1;
     }
    if (rdata113 != 0x00000000 ) {//0x110
      vfwp("** CPU0 TEST FAIL **%x",0x110);
      fail_flag = 1;
     }
    if (rdata114 != 0x00000000 ) {//0x114
      vfwp("** CPU0 TEST FAIL **%x",0x114);
      fail_flag = 1;
     }
    if (rdata115 != 0x00000000 ) {//0x118
      vfwp("** CPU0 TEST FAIL **%x",0x118);
      fail_flag = 1;
     }
    if (rdata116 != 0x00000000 ) {//0x11C
      vfwp("** CPU0 TEST FAIL **%x",0x11C);
      fail_flag = 1;
     }
    if (rdata117 != 0x00000000 ) {//0x120
      vfwp("** CPU0 TEST FAIL **%x",0x120);
      fail_flag = 1;
     }
    if (rdata118 != 0x00000000 ) {//0x124
      vfwp("** CPU0 TEST FAIL **%x",0x124);
      fail_flag = 1;
     }
    if (rdata119 != 0x00000000 ) {//0x128
      vfwp("** CPU0 TEST FAIL **%x",0x128);
      fail_flag = 1;
     }
    if (rdata120 != 0x00000000 ) {//0x12C
      vfwp("** CPU0 TEST FAIL **%x",0x12C);
      fail_flag = 1;
     }
    if (rdata121 != 0x00000000 ) {//0x130
      vfwp("** CPU0 TEST FAIL **%x",0x130);
      fail_flag = 1;
     }
    if (rdata122 != 0x00000000 ) {//0x134
      vfwp("** CPU0 TEST FAIL **%x",0x134);
      fail_flag = 1;
     }
    if (rdata123 != 0x00000000 ) {//0x138
      vfwp("** CPU0 TEST FAIL **%x",0x138);
      fail_flag = 1;
     }
    if (rdata124 != 0x00000000 ) {//0x13C
      vfwp("** CPU0 TEST FAIL **%x",0x13C);
      fail_flag = 1;
     }
    if (rdata125 != 0x00000000 ) {//0x400
      vfwp("** CPU0 TEST FAIL **%x",0x400);
      fail_flag = 1;
     }
    if (rdata126 != 0x00000000 ) {//0x404
      vfwp("** CPU0 TEST FAIL **%x",0x404);
      fail_flag = 1;
     }
    if (rdata127 != 0x00800200 ) {//0x408
      vfwp("** CPU0 TEST FAIL **%x",0x408);
      fail_flag = 1;
     }
    if (rdata128 != 0x00000000 ) {//0x40C
      vfwp("** CPU0 TEST FAIL **%x",0x40C);
      fail_flag = 1;
     }
    if (rdata129 != 0x00000000 ) {//0x410
      vfwp("** CPU0 TEST FAIL **%x",0x410);
      fail_flag = 1;
     }
    if (rdata130 != 0x00000000 ) {//0x420
      vfwp("** CPU0 TEST FAIL **%x",0x420);
      fail_flag = 1;
     }
    if (rdata131 != 0x00000000 ) {//0x424
      vfwp("** CPU0 TEST FAIL **%x",0x424);
      fail_flag = 1;
     }
    if (rdata132 != 0x00800200 ) {//0x428
      vfwp("** CPU0 TEST FAIL **%x",0x428);
      fail_flag = 1;
     }
    if (rdata133 != 0x00000000 ) {//0x42C
      vfwp("** CPU0 TEST FAIL **%x",0x42C);
      fail_flag = 1;
     }
    if (rdata134 != 0x00000000 ) {//0x430
      vfwp("** CPU0 TEST FAIL **%x",0x430);
      fail_flag = 1;
     }
    if (rdata135 != 0x00000000 ) {//0x440
      vfwp("** CPU0 TEST FAIL **%x",0x440);
      fail_flag = 1;
     }
    if (rdata136 != 0x00000000 ) {//0x444
      vfwp("** CPU0 TEST FAIL **%x",0x444);
      fail_flag = 1;
     }
    if (rdata137 != 0x00800200 ) {//0x448
      vfwp("** CPU0 TEST FAIL **%x",0x448);
      fail_flag = 1;
     }
    if (rdata138 != 0x00000000 ) {//0x44C
      vfwp("** CPU0 TEST FAIL **%x",0x44C);
      fail_flag = 1;
     }
    if (rdata139 != 0x00000000 ) {//0x450
      vfwp("** CPU0 TEST FAIL **%x",0x450);
      fail_flag = 1;
     }
    if (rdata140 != 0x00000000 ) {//0x460
      vfwp("** CPU0 TEST FAIL **%x",0x460);
      fail_flag = 1;
     }
    if (rdata141 != 0x00000000 ) {//0x464
      vfwp("** CPU0 TEST FAIL **%x",0x464);
      fail_flag = 1;
     }
    if (rdata142 != 0x00800200 ) {//0x468
      vfwp("** CPU0 TEST FAIL **%x",0x468);
      fail_flag = 1;
     }
    if (rdata143 != 0x00000000 ) {//0x46C
      vfwp("** CPU0 TEST FAIL **%x",0x46C);
      fail_flag = 1;
     }
    if (rdata144 != 0x00000000 ) {//0x470
      vfwp("** CPU0 TEST FAIL **%x",0x470);
      fail_flag = 1;
     }
    if (rdata145 != 0x00000000 ) {//0x480
      vfwp("** CPU0 TEST FAIL **%x",0x480);
      fail_flag = 1;
     }
    if (rdata146 != 0x00000000 ) {//0x484
      vfwp("** CPU0 TEST FAIL **%x",0x484);
      fail_flag = 1;
     }
    if (rdata147 != 0x00800200 ) {//0x488
      vfwp("** CPU0 TEST FAIL **%x",0x488);
      fail_flag = 1;
     }
    if (rdata148 != 0x00000000 ) {//0x48c
      vfwp("** CPU0 TEST FAIL **%x",0x48c);
      fail_flag = 1;
     }
    if (rdata149 != 0x00000000 ) {//0x490
      vfwp("** CPU0 TEST FAIL **%x",0x490);
      fail_flag = 1;
     }
    if (rdata150 != 0x00000000 ) {//0x4A0
      vfwp("** CPU0 TEST FAIL **%x",0x4A0);
      fail_flag = 1;
     }
    if (rdata151 != 0x00000000 ) {//0x4A4
      vfwp("** CPU0 TEST FAIL **%x",0x4A4);
      fail_flag = 1;
     }
    if (rdata152 != 0x00800200 ) {//0x4A8
      vfwp("** CPU0 TEST FAIL **%x",0x4A8);
      fail_flag = 1;
     }
    if (rdata153 != 0x00000000 ) {//0x4AC
      vfwp("** CPU0 TEST FAIL **%x",0x4AC);
      fail_flag = 1;
     }
    if (rdata154 != 0x00000000 ) {//0x4B0
      vfwp("** CPU0 TEST FAIL **%x",0x4B0);
      fail_flag = 1;
     }
    if (rdata155 != 0x00000000 ) {//0x4C0
      vfwp("** CPU0 TEST FAIL **%x",0x4C0);
      fail_flag = 1;
     }
    if (rdata156 != 0x00000000 ) {//0x4C4
      vfwp("** CPU0 TEST FAIL **%x",0x4C4);
      fail_flag = 1;
     }
    if (rdata157 != 0x00800200 ) {//0x4C8
      vfwp("** CPU0 TEST FAIL **%x",0x4C8);
      fail_flag = 1;
     }
    if (rdata158 != 0x00000000 ) {//0x4CC
      vfwp("** CPU0 TEST FAIL **%x",0x4CC);
      fail_flag = 1;
     }
    if (rdata159 != 0x00000000 ) {//0x4D0
      vfwp("** CPU0 TEST FAIL **%x",0x4D0);
      fail_flag = 1;
     }
    if (rdata160 != 0x00000000 ) {//0x4E0
      vfwp("** CPU0 TEST FAIL **%x",0x4E0);
      fail_flag = 1;
     }
    if (rdata161 != 0x00000000 ) {//0x4E4
      vfwp("** CPU0 TEST FAIL **%x",0x4E4);
      fail_flag = 1;
     }
    if (rdata162 != 0x00800200 ) {//0x4E8
      vfwp("** CPU0 TEST FAIL **%x",0x4E8);
      fail_flag = 1;
     }
    if (rdata163 != 0x00000000 ) {//0x4EC
      vfwp("** CPU0 TEST FAIL **%x",0x4EC);
      fail_flag = 1;
     }
    if (rdata164 != 0x00000000 ) {//0x4F0
      vfwp("** CPU0 TEST FAIL **%x",0x4F0);
      fail_flag = 1;
     }
    if (rdata165 != 0x00000000 ) {//0xD00
      vfwp("** CPU0 TEST FAIL **%x",0xD00);
      fail_flag = 1;
     }
    if (rdata166 != 0x00000000 ) {//0xD04
      vfwp("** CPU0 TEST FAIL **%x",0xD04);
      fail_flag = 1;
     }
    if (rdata167 != 0x00000000 ) {//0xD08
      vfwp("** CPU0 TEST FAIL **%x",0xD08);
      fail_flag = 1;
     }
    if (rdata168 != 0x00000000 ) {//0xD0C
      vfwp("** CPU0 TEST FAIL **%x",0xD0C);
      fail_flag = 1;
     }
    if (rdata169 != 0x001e3071 ) {//0xE00
      vfwp("** CPU0 TEST FAIL **%x",0xE00);
      fail_flag = 1;
     }
    if (rdata170 != 0x000000f5 ) {//0xE04
      vfwp("** 0xE04(rdata170) TEST FAIL **%x",0xE04);
      fail_flag = 1;
     }
    if (rdata171 != 0x00000000 ) {//0xE08
      vfwp("** CPU0 TEST FAIL **%x",0xE08);
      fail_flag = 1;
     }
    if (rdata172 != 0x0000ffff) {//0xE0C
      vfwp("** rdata172 TEST FAIL **%x: ",0xE0C);
	vfwp("%x\n", rdata172);
      fail_flag = 1;
     }
    if (rdata173 != 0x0000000f ) {//0xE10
      vfwp("** rdata173 TEST FAIL **%x: ",0xE10);
	vfwp("%x\n", rdata173);
      fail_flag = 1;
     }
    if (rdata174 != 0x00ff7f74 ) {//0xE14
      vfwp("** 0xE14(rdata174) TEST FAIL **%x",0xE14);
      fail_flag = 1;
     }
    if (rdata175 != 0x00000001 ) {//0xE80
      vfwp("** rdata175 TEST FAIL **%x: ",0xE80);
      vfwp("%x\n", rdata175);
      fail_flag = 1;
     }
    if (rdata176 != 0x00000030 ) {//0xFE0
      vfwp("** CPU0 TEST FAIL **%x",0xFE0);
      fail_flag = 1;
     }
    if (rdata177 != 0x00000013 ) {//0xFE4
      vfwp("** CPU0 TEST FAIL **%x",0xFE4);
      fail_flag = 1;
     }
    if (rdata178 != 0x00000034 ) {//0xFE8
      vfwp("** CPU0 TEST FAIL **%x",0xFE8);
      fail_flag = 1;
     }
    if (rdata179 != 0x00000000 ) {//0xFEC
      vfwp("** CPU0 TEST FAIL **%x",0xFEC);
      fail_flag = 1;
     }
    if (rdata180 != 0x0000000d ) {//0xFF0
      vfwp("** CPU0 TEST FAIL **%x",0xFF0);
      fail_flag = 1;
     }
    if (rdata181 != 0x000000f0 ) {//0xFF4
      vfwp("** CPU0 TEST FAIL **%x: ",0xFF4);
      vfwp("%x\n", rdata181);
      fail_flag = 1;
     }
    if (rdata182 != 0x00000005 ) {//0xFF8
      vfwp("** CPU0 TEST FAIL **%x: ",0xFF8);
      vfwp("%x\n", rdata182);
      fail_flag = 1;
     }
    if (rdata183 != 0x000000b1 ) {//0xFFC
      vfwp("** CPU0 TEST FAIL **%x: ",0xFFC);
      vfwp("%x\n", rdata183);
      fail_flag = 1;
     }
    if (fail_flag != 1 ) {
      vfwp("** REG WRITE and READ test SUCCESS!!!");
     }
    else{
      vfwp("** REG WRITE and READ test FAIL !!! %x\n",0);
     }

    return 0;
}
