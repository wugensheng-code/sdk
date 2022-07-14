/*
 * al_pinmux.c
 *
 *  Created on: June.10.2022
 *      Author: kefu.fang
 */
#include "demosoc.h"
#include "nuclei_sdk_soc.h"
/**************************function definition******************************/
/*!
    \brief     Enable pinmux
    \retval
*/

 void Enablepinmux1(void)
{ 
     *(uint32_t *)(0xf8803000u) =0x1;	  //QSPI  MIO0
     *(uint32_t *)(0xf8803004u) =0x1;
     *(uint32_t *)(0xf8803008u) =0x1;
     *(uint32_t *)(0xf880300cu) =0x1;
     *(uint32_t *)(0xf8803010u) =0x1;
     *(uint32_t *)(0xf8803014u) =0x1;
     *(uint32_t *)(0xf8803018u) =0x1;   

     *(uint32_t *)(0xf880301cu) =0x4;    //GPIO MIO7
	 
     *(uint32_t *)(0xf8803020u) =0x1;    //CAN1 MIO8
     *(uint32_t *)(0xf8803024u) =0x1;
	 *(uint32_t *)(0xf880340cu) =0x1;    //sel3

     *(uint32_t *)(0xf8803028u) =0x4;    //GPIO MIO10
     *(uint32_t *)(0xf880302cu) =0x4;    //GPIO MIO11

     *(uint32_t *)(0xf8803030u) =0x0;    //I2C1 MIO12
     *(uint32_t *)(0xf8803034u) =0x0;    //I2C1 MIO13

     *(uint32_t *)(0xf8803038u) =0x4;    //GPIO MIO14
     *(uint32_t *)(0xf880303cu) =0x4;    //GPIO MIO15

     *(uint32_t *)(0xf8803040u) =0x9;    //spi0  MIO16
     *(uint32_t *)(0xf8803044u) =0x9;    //spi0  MIO17
     *(uint32_t *)(0xf8803048u) =0x9;    //spi0  MIO18
     *(uint32_t *)(0xf880304cu) =0x4;    //GPIO MIO19
     *(uint32_t *)(0xf8803050u) =0x4;    //GPIO MIO20
     *(uint32_t *)(0xf8803054u) =0x9;    //spi0  MIO21
     *(uint32_t *)(0xf8803424u) =0x1;    //emio_sel9


     *(uint32_t *)(0xf8803058u) =0x7        //MIO23/24/25/22 jtag
     *(uint32_t *)(0xf880305cu) =0x7 
     *(uint32_t *)(0xf8803060u) =0x7 
     *(uint32_t *)(0xf8803064u) =0x7 
                                         //
	 
	 *(uint32_t *)(0xf8803068u) =0x3;    //uart0  MIO26/27
	 *(uint32_t *)(0xf880306cu) =0x3;
	 *(uint32_t *)(0xf8803410u) =0x1;

	*(uint32_t *)(0xf8803070u) =0xe;     //rgmii1 mio 28-39
	*(uint32_t *)(0xf8803074u) =0xe;
    *(uint32_t *)(0xf8803078u) =0xe;
    *(uint32_t *)(0xf880307cu) =0xe;
    *(uint32_t *)(0xf8803080u) =0xe;
    *(uint32_t *)(0xf8803084u) =0xe;
    *(uint32_t *)(0xf8803088u) =0xe;
    *(uint32_t *)(0xf880308cu) =0xe;
    *(uint32_t *)(0xf8803090u) =0xe;
    *(uint32_t *)(0xf8803094u) =0xe;
    *(uint32_t *)(0xf8803098u) =0xe;
    *(uint32_t *)(0xf880309cu) =0xe;
  

    *(uint32_t *)(0xf88030a0u) =0xa;      //SDIO0 mio40-45
    *(uint32_t *)(0xf88030a4u) =0xa;
    *(uint32_t *)(0xf88030a8u) =0xa;
    *(uint32_t *)(0xf88030acu) =0xa;
    *(uint32_t *)(0xf88030b0u) =0xa;
    *(uint32_t *)(0xf88030b4u) =0xa; 
    *(uint32_t *)(0xf880342cu) =0x1;     //emio_sel11

    *(uint32_t *)(0xf88030b8u) =0x9;      //spi1 mio46-49
    *(uint32_t *)(0xf88030bcu) =0x9;
    *(uint32_t *)(0xf88030c0u) =0x9;
    *(uint32_t *)(0xf88030c4u) =0x9;
    *(uint32_t *)(0xf8803428u) =0x1;      //emio_sel9

    *(uint32_t *)(0xf88030c8u) =0x0;      //i2c0 mio50-51
    *(uint32_t *)(0xf88030ccu) =0x0;
    *(uint32_t *)(0xf8803400u) =0x1;      //emio_sel12
    
    *(uint32_t *)(0xf88030d0u) =0xf;      //mdc1   MIO52-53
    *(uint32_t *)(0xf88030d4u) =0xf;      //mdio1
    *(uint32_t *)(0xf8803438u) =0x1;      //emio_sel

}



    

