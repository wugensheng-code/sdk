#include "al_core.h"

void Enablepinmux1(void)
{
     *(AL_U32 *)(0xf8803000u) =0x1;      //QSPI  MIO0
     *(AL_U32 *)(0xf8803004u) =0x1;
     *(AL_U32 *)(0xf8803008u) =0x1;
     *(AL_U32 *)(0xf880300cu) =0x1;
     *(AL_U32 *)(0xf8803010u) =0x1;
     *(AL_U32 *)(0xf8803014u) =0x1;
     *(AL_U32 *)(0xf8803018u) =0x1;

     *(AL_U32 *)(0xf880301cu) =0x4;    //GPIO MIO7

     *(AL_U32 *)(0xf8803020u) =0x1;    //CAN1 MIO8
     *(AL_U32 *)(0xf8803024u) =0x1;
     *(AL_U32 *)(0xf880340cu) =0x1;    //sel3

     *(AL_U32 *)(0xf8803028u) =0x4;    //GPIO MIO10
     *(AL_U32 *)(0xf880302cu) =0x4;    //GPIO MIO11

     *(AL_U32 *)(0xf8803030u) =0x0;    //I2C1 MIO12
     *(AL_U32 *)(0xf8803034u) =0x0;    //I2C1 MIO13
     *(AL_U32 *)(0xf8803404u) =0x1;


     *(AL_U32 *)(0xf8803038u) =0x4;    //GPIO MIO14
     *(AL_U32 *)(0xf880303cu) =0x4;    //GPIO MIO15

     *(AL_U32 *)(0xf8803040u) =0x9;    //spi0  MIO16
     *(AL_U32 *)(0xf8803044u) =0x9;    //spi0  MIO17
     *(AL_U32 *)(0xf8803048u) =0x9;    //spi0  MIO18
     *(AL_U32 *)(0xf880304cu) =0x4;    //GPIO MIO19
     *(AL_U32 *)(0xf8803050u) =0x4;    //GPIO MIO20
     *(AL_U32 *)(0xf8803054u) =0x4;
     //*(AL_U32 *)(0xf8803054u) =0x9;    //spi0  MIO21
     *(AL_U32 *)(0xf8803424u) =0x1;    //emio_sel9


    *(AL_U32 *)(0xf8803058u) =0x7;        //MIO23/24/25/22 jtag
    *(AL_U32 *)(0xf880305cu) =0x7;
    *(AL_U32 *)(0xf8803060u) =0x7;
    *(AL_U32 *)(0xf8803064u) =0x7;
                                         //

    *(AL_U32 *)(0xf8803068u) =0x3;    //uart0  MIO26/27
    *(AL_U32 *)(0xf880306cu) =0x3;
    *(AL_U32 *)(0xf8803410u) =0x1;

    *(AL_U32 *)(0xf8803070u) =0xe;     //rgmii1 mio 28-39
    *(AL_U32 *)(0xf8803074u) =0xe;
    *(AL_U32 *)(0xf8803078u) =0xe;
    *(AL_U32 *)(0xf880307cu) =0xe;
    *(AL_U32 *)(0xf8803080u) =0xe;
    *(AL_U32 *)(0xf8803084u) =0xe;
    *(AL_U32 *)(0xf8803088u) =0xe;
    *(AL_U32 *)(0xf880308cu) =0xe;
    *(AL_U32 *)(0xf8803090u) =0xe;
    *(AL_U32 *)(0xf8803094u) =0xe;
    *(AL_U32 *)(0xf8803098u) =0xe;
    *(AL_U32 *)(0xf880309cu) =0xe;


    *(AL_U32 *)(0xf88030a0u) =0xa;      //SDIO0 mio40-45
    *(AL_U32 *)(0xf88030a4u) =0xa;
    *(AL_U32 *)(0xf88030a8u) =0xa;
    *(AL_U32 *)(0xf88030acu) =0xa;
    *(AL_U32 *)(0xf88030b0u) =0xa;
    *(AL_U32 *)(0xf88030b4u) =0xa;
    *(AL_U32 *)(0xf880342cu) =0x1;     //emio_sel11

    *(AL_U32 *)(0xf88030b8u) =0x9;      //spi1 mio46-49
    *(AL_U32 *)(0xf88030bcu) =0x9;
    *(AL_U32 *)(0xf88030c0u) =0x9;
    *(AL_U32 *)(0xf88030c4u) =0x9;
    *(AL_U32 *)(0xf8803428u) =0x1;      //emio_sel9

    *(AL_U32 *)(0xf88030c8u) =0x0;      //i2c0 mio50-51
    *(AL_U32 *)(0xf88030ccu) =0x0;
    *(AL_U32 *)(0xf8803400u) =0x1;      //emio_sel12

    *(AL_U32 *)(0xf88030d0u) =0xf;      //mdc1   MIO52-53
    *(AL_U32 *)(0xf88030d4u) =0xf;      //mdio1
    *(AL_U32 *)(0xf8803438u) =0x1;      //emio_sel

#ifndef AL9000_BRINGUP
    *(AL_U32 *)(0xf840e004u) =0x0;      //close mpu ddr0
    *(AL_U32 *)(0xf840f004u) =0x0;      //close mpu ddr1
    *(AL_U32 *)(0xf8801074u) |=0x4000;  //reset release ddr
#endif

    *(AL_U32 *)(0xf88030c0u) =0x3; //uart0  MIO48
    *(AL_U32 *)(0xf88030c4u) =0x3; //uart1  MIO49
    *(AL_U32 *)(0xf8803414u) =0x1;
}

void Enablepinmux1_mode2(void)
{
    *(AL_U32 *)(0xf8803068u) =0x3;    //uart0  MIO26/27
    *(AL_U32 *)(0xf880306cu) =0x3;
    *(AL_U32 *)(0xf8803410u) =0x1;
#if ENABLE_PINMUX_MODE_SD1 == 1
    *(AL_U32 *)(0xf88030b8u) =0xa;
    *(AL_U32 *)(0xf88030bcu) =0xa;
    *(AL_U32 *)(0xf88030c0u) =0xa;
    *(AL_U32 *)(0xf88030c4u) =0xa;
    *(AL_U32 *)(0xf88030c8u) =0xa;
    *(AL_U32 *)(0xf88030ccu) =0xa;

    *(AL_U32 *)(0xf8803430u) =0x1;     //emio_sel12
#else
    *(AL_U32 *)(0xf88030a0u) =0xb;      //SDIO0 mio40-49
    *(AL_U32 *)(0xf88030a4u) =0xb;
    *(AL_U32 *)(0xf88030a8u) =0xb;
    *(AL_U32 *)(0xf88030acu) =0xb;
    *(AL_U32 *)(0xf88030b0u) =0xb;
    *(AL_U32 *)(0xf88030b4u) =0xb;
    *(AL_U32 *)(0xf88030b8u) =0xb;
    *(AL_U32 *)(0xf88030bcu) =0xb;
    *(AL_U32 *)(0xf88030c0u) =0xb;
    *(AL_U32 *)(0xf88030c4u) =0xb;
    *(AL_U32 *)(0xf880342cu) =0x1;     //emio_sel11
#endif

    //USB mode2
    *(AL_U32 *)(0xf8803070u) =0xf;     //USB pinmux config
    *(AL_U32 *)(0xf8803074u) =0xf;
    *(AL_U32 *)(0xf8803078u) =0xf;
    *(AL_U32 *)(0xf880307cu) =0xf;
    *(AL_U32 *)(0xf8803080u) =0xf;
    *(AL_U32 *)(0xf8803084u) =0xf;
    *(AL_U32 *)(0xf8803088u) =0xf;
    *(AL_U32 *)(0xf880308cu) =0xf;
    *(AL_U32 *)(0xf8803090u) =0xf;
    *(AL_U32 *)(0xf8803094u) =0xf;
    *(AL_U32 *)(0xf8803098u) =0xf;
    *(AL_U32 *)(0xf880309cu) =0xf;
    //USB mode2

    *(AL_U32 *)(0xf8803040u) =0x7;
    *(AL_U32 *)(0xf8803044u) =0x7;
    *(AL_U32 *)(0xf880341cu) =0x1; //TTC SET

    *(AL_U32 *)(0xf8803048u) =0x0;
    *(AL_U32 *)(0xf880304cu) =0x0;
    *(AL_U32 *)(0xf8803400u) =0x1; //IIC0 SET

    *(AL_U32 *)(0xf88030d0u) =0x1;
    *(AL_U32 *)(0xf88030d4u) =0x1;
    *(AL_U32 *)(0xf880340cu) =0x1; //CAN1 SET

#ifndef AL9000_BRINGUP
    *(AL_U32 *)(0xf840e004u) =0x0;      //close mpu ddr0
    *(AL_U32 *)(0xf840f004u) =0x0;      //close mpu ddr1
    *(AL_U32 *)(0xf8801074u) |=0x4000;  //reset release ddr
#endif
}




