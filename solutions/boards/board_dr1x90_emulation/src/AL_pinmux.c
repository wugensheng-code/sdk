/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

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
    //  *(AL_U32 *)(0xf8803054u) =0x4;
     *(AL_U32 *)(0xf8803054u) =0x9;    //spi0  MIO21
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

#ifndef AL_DCBOARD_TTC
    *(AL_U32 *)(0xF8803078u) =0x7;    //ttc0
    *(AL_U32 *)(0xF880307cu) =0x7;
    *(AL_U32 *)(0xf8803070u) =0x7;    //ttc1
    *(AL_U32 *)(0xf8803074u) =0x7;
#endif

#ifndef AL_BRINGUP
    *(AL_U32 *)(0xf840e004u) =0x0;      //close mpu ddr0
    *(AL_U32 *)(0xf840f004u) =0x0;      //close mpu ddr1
    *(AL_U32 *)(0xf8801074u) |=0x4000;  //reset release ddr
#endif

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

#define PIN_MUX_BASE    (0xF8803000UL)

#define PS_MIO0         (0x00+PIN_MUX_BASE)
#define PS_MIO1         (0x04+PIN_MUX_BASE)
#define PS_MIO2         (0x08+PIN_MUX_BASE)
#define PS_MIO3         (0x0c+PIN_MUX_BASE)
#define PS_MIO4         (0x10+PIN_MUX_BASE)
#define PS_MIO5         (0x14+PIN_MUX_BASE)
#define PS_MIO6         (0x18+PIN_MUX_BASE)
#define PS_MIO7         (0x1c+PIN_MUX_BASE)
#define PS_MIO8         (0x20+PIN_MUX_BASE)
#define PS_MIO9         (0x24+PIN_MUX_BASE)
#define PS_MIO10        (0x28+PIN_MUX_BASE)
#define PS_MIO11        (0x2c+PIN_MUX_BASE)
#define PS_MIO12        (0x30+PIN_MUX_BASE)
#define PS_MIO13        (0x34+PIN_MUX_BASE)
#define PS_MIO14        (0x38+PIN_MUX_BASE)
#define PS_MIO15        (0x3c+PIN_MUX_BASE)
#define PS_MIO16        (0x40+PIN_MUX_BASE)
#define PS_MIO17        (0x44+PIN_MUX_BASE)
#define PS_MIO18        (0x48+PIN_MUX_BASE)
#define PS_MIO19        (0x4c+PIN_MUX_BASE)
#define PS_MIO20        (0x50+PIN_MUX_BASE)
#define PS_MIO21        (0x54+PIN_MUX_BASE)
#define PS_MIO22        (0x58+PIN_MUX_BASE)
#define PS_MIO23        (0x5c+PIN_MUX_BASE)
#define PS_MIO24        (0x60+PIN_MUX_BASE)
#define PS_MIO25        (0x64+PIN_MUX_BASE)
#define PS_MIO26        (0x68+PIN_MUX_BASE)
#define PS_MIO27        (0x6c+PIN_MUX_BASE)
#define PS_MIO28        (0x70+PIN_MUX_BASE)

    AL_REG32_WRITE(PS_MIO0, 0x02);
    //AL_REG32_WRITE(PS_MIO1, 0x02);
    AL_REG32_WRITE(PS_MIO2, 0x02);
    AL_REG32_WRITE(PS_MIO3, 0x02);
    AL_REG32_WRITE(PS_MIO4, 0x02);
    AL_REG32_WRITE(PS_MIO5, 0x02);
    AL_REG32_WRITE(PS_MIO6, 0x02);
    AL_REG32_WRITE(PS_MIO7, 0x02);
    AL_REG32_WRITE(PS_MIO8, 0x02);
    AL_REG32_WRITE(PS_MIO9, 0x02);

    AL_REG32_WRITE(PS_MIO10, 0x02);
    AL_REG32_WRITE(PS_MIO11, 0x02);
    AL_REG32_WRITE(PS_MIO12, 0x02);
    AL_REG32_WRITE(PS_MIO13, 0x02);
    AL_REG32_WRITE(PS_MIO14, 0x02);

#ifndef AL_BRINGUP
    *(AL_U32 *)(0xf840e004u) =0x0;      //close mpu ddr0
    *(AL_U32 *)(0xf840f004u) =0x0;      //close mpu ddr1
    *(AL_U32 *)(0xf8801074u) |=0x4000;  //reset release ddr
#endif
}




