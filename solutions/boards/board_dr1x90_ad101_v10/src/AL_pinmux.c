/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_core.h"
#include "dr1x90_pinctrl.h"

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
    //DEBUG UART1
    *(AL_U32 *)(0xf88030c0u) = 0x3; //PS_UART_TX    PS_IO48 uart1_tx__emio_o_5 (out)    Function_3
    *(AL_U32 *)(0xf88030c4u) = 0x3; //PS_UART_RX    PS_IO49 uart1_rx__emio_i_5 (in)     Function_3
    *(AL_U32 *)(0xf8803414u) = 0x1; //EMIOSEL_5     1:MIO 0:EMIO

    //USB0
    *(AL_U32 *)(0xf8803070u) = 0xf; //PS_USB_DATA4  PS_IO28 usb0_data[4] (inout)    Function_15
    *(AL_U32 *)(0xf8803074u) = 0xf; //PS_USB_DIR    PS_IO29 usb0_dir (in)           Function_15
    *(AL_U32 *)(0xf8803078u) = 0xf; //PS_USB_STP    PS_IO30 usb0_stp (out)          Function_15
    *(AL_U32 *)(0xf880307cu) = 0xf; //PS_USB_NXT    PS_IO31 usb0_nxt (in)           Function_15
    *(AL_U32 *)(0xf8803080u) = 0xf; //PS_USB_DATA0  PS_IO32 usb0_data[0] (inout)    Function_15
    *(AL_U32 *)(0xf8803084u) = 0xf; //PS_USB_DATA1  PS_IO33 usb0_data[1] (inout)    Function_15
    *(AL_U32 *)(0xf8803088u) = 0xf; //PS_USB_DATA2  PS_IO34 usb0_data[2] (inout)    Function_15
    *(AL_U32 *)(0xf880308cu) = 0xf; //PS_USB_DATA3  PS_IO35 usb0_data[3] (inout)    Function_15
    *(AL_U32 *)(0xf8803090u) = 0xf; //PS_USB_CLKOUT PS_IO36 usb0_clk (in)           Function_15
    *(AL_U32 *)(0xf8803094u) = 0xf; //PS_USB_DATA5  PS_IO37 usb0_data[5] (inout)    Function_15
    *(AL_U32 *)(0xf8803098u) = 0xf; //PS_USB_DATA6  PS_IO38 usb0_data[6] (inout)    Function_15
    *(AL_U32 *)(0xf880309cu) = 0xf; //PS_USB_DATA7  PS_IO39 usb0_data[7] (inout)    Function_15

    //ETH0
    *(AL_U32 *)(0xf8803040u) = 0xe; //PS_ETH_TXC     PS_IO16 rgmii_rmii_0_tx_clk (out)    Function_14
    *(AL_U32 *)(0xf8803044u) = 0xe; //PS_ETH_TXD0    PS_IO17 rgmii_rmii_0_txd[0] (out)    Function_14
    *(AL_U32 *)(0xf8803048u) = 0xe; //PS_ETH_TXD1    PS_IO18 rgmii_rmii_0_txd[1] (out)    Function_14
    *(AL_U32 *)(0xf880304cu) = 0xe; //PS_ETH_TXD2    PS_IO19 rgmii0_txd[2] (out)          Function_14
    *(AL_U32 *)(0xf8803050u) = 0xe; //PS_ETH_TXD3    PS_IO20 rgmii0_txd[3] (out)          Function_14
    *(AL_U32 *)(0xf8803054u) = 0xe; //PS_ETH_TXCTL   PS_IO21 rgmii_rmii_0_tx_ctl (out)    Function_14
    *(AL_U32 *)(0xf8803058u) = 0xe; //PS_ETH_RXC     PS_IO22 rgmii_rmii_0_rx_clk (in)     Function_14
    *(AL_U32 *)(0xf880305cu) = 0xe; //PS_ETH_RXD0    PS_IO23 rgmii_rmii_0_rxd[0] (in)     Function_14
    *(AL_U32 *)(0xf8803060u) = 0xe; //PS_ETH_RXD1    PS_IO24 rgmii_rmii_0_rxd[1] (in)     Function_14
    *(AL_U32 *)(0xf8803064u) = 0xe; //PS_ETH_RXD2    PS_IO25 rgmii0_rxd[2] (in)           Function_14
    *(AL_U32 *)(0xf8803068u) = 0xe; //PS_ETH_RXD3    PS_IO26 rgmii0_rxd[3] (in)           Function_14
    *(AL_U32 *)(0xf880306cu) = 0xe; //PS_ETH_RXCTL   PS_IO27 rgmii_rmii_0_rx_ctl (in)     Function_14

    *(AL_U32 *)(0xf88030d0u) = 0xe; //PS_ETH_MDC     PS_IO52 mdc0__emio_o_13 (out)        Function_14
    *(AL_U32 *)(0xf88030d4u) = 0xe; //PS_ETH_MDIO    PS_IO53 mdio0__emio_io_13 (inout)    Function_14
    *(AL_U32 *)(0xf8803434u) = 0x1; //EMIOSEL_13     1:MIO 0:EMIO

    //I2C0
    *(AL_U32 *)(0xf88030c8u) = 0x0; //PS_IIC_SCL_MAIN     PS_IO50    i2c0_scl__emio_io_0 (inout)    Function_0
    *(AL_U32 *)(0xf88030ccu) = 0x0; //PS_IIC_SDA_MAIN     PS_IO51    i2c0_sda__emio_io_0 (inout)    Function_0
    *(AL_U32 *)(0xf880303cu) = 0x4; //PS_IIC_MUX_RESET    PS_IO15    GPIO15                         Function_4
    *(AL_U32 *)(0xf8803400u) = 0x1; //EMIOSEL_0           1:MIO 0:EMIO

    //CAN0
    *(AL_U32 *)(0xf88030b8u) = 0x1; //PS_CAN_RXD     PS_IO46    can0_rx__emio_i_2 (in)     Function_1
    *(AL_U32 *)(0xf88030bcu) = 0x1; //PS_CAN_TXD     PS_IO47    can0_tx__emio_o_2 (out)    Function_1
    *(AL_U32 *)(0xf8803024u) = 0x4; //PS_CAN_STBY    PS_IO9     GPIO9                      Function_4
    *(AL_U32 *)(0xf8803408u) = 0x1; //EMIOSEL_2      1:MIO 0:EMIO

    //LED1
    *(AL_U32 *)(0xf8803038u) = 0x4; //PS_LED_1     PS_IO14    GPIO14    Function_4

    //SD0
    *(AL_U32 *)(0xf88030a0u) = 0xa; //PS_SD_CLK    PS_IO40    sd0_clk__emio_io_11 (inout)        Function_10
    *(AL_U32 *)(0xf88030a4u) = 0xa; //PS_SD_CMD    PS_IO41    sd0_cmd__emio_io_11 (inout)        Function_10
    *(AL_U32 *)(0xf88030a8u) = 0xa; //PS_SD_D0     PS_IO42    sd0_data[0]__emio_io_11 (inout)    Function_10
    *(AL_U32 *)(0xf88030acu) = 0xa; //PS_SD_D1     PS_IO43    sd0_data[1]__emio_io_11 (inout)    Function_10
    *(AL_U32 *)(0xf88030b0u) = 0xa; //PS_SD_D2     PS_IO44    sd0_data[2]__emio_io_11 (inout)    Function_10
    *(AL_U32 *)(0xf88030b4u) = 0xa; //PS_SD_D3     PS_IO45    sd0_data[3]__emio_io_11 (inout)    Function_10
    *(AL_U32 *)(0xf8803000u) = 0xc; //PS_SD_DET    PS_IO0     sd0_cd__emio_i_11 (in)             Function_12
    *(AL_U32 *)(0xf880342cu) = 0x1; //EMIOSEL_11   1:MIO 0:EMIO

    //QSPI NOR FLASH 0
    *(AL_U32 *)(0xf8803004u) = 0x0; //PS_FLASH_CS_B    PS_IO1    qspi0_ss_b  (out)      Function_0
    *(AL_U32 *)(0xf8803008u) = 0x0; //PS_FLASH_DQ0     PS_IO2    qspi0_io[0] (inout)    Function_0
    *(AL_U32 *)(0xf880300cu) = 0x0; //PS_FLASH_DQ1     PS_IO3    qspi0_io[1] (inout)    Function_0
    *(AL_U32 *)(0xf8803010u) = 0x0; //PS_FLASH_DQ2     PS_IO4    qspi0_io[2] (inout)    Function_0
    *(AL_U32 *)(0xf8803014u) = 0x0; //PS_FLASH_DQ3     PS_IO5    qspi0_io[3] (inout)    Function_0
    *(AL_U32 *)(0xf8803018u) = 0x0; //PS_FLASH_CLK     PS_IO6    qspi0_sclk  (out)      Function_0

    /* Set Mio 10K up pull to enable qspi */
    mio_pad_ctrl0(0, MIO_SPEED_FAST, MIO_PULL_10K_EN, MIO_PULL_DIS);
    mio_pad_ctrl0(1, MIO_SPEED_FAST, MIO_PULL_10K_EN, MIO_PULL_DIS);
    mio_pad_ctrl0(2, MIO_SPEED_FAST, MIO_PULL_10K_EN, MIO_PULL_DIS);
    mio_pad_ctrl0(3, MIO_SPEED_FAST, MIO_PULL_10K_EN, MIO_PULL_DIS);
    mio_pad_ctrl0(4, MIO_SPEED_FAST, MIO_PULL_10K_EN, MIO_PULL_DIS);
    mio_pad_ctrl0(5, MIO_SPEED_FAST, MIO_PULL_10K_EN, MIO_PULL_DIS);
    mio_pad_ctrl0(6, MIO_SPEED_FAST, MIO_PULL_10K_EN, MIO_PULL_DIS);

    /* GBE Configuration drive capability */
    *((volatile AL_U32 *)(0xf8803880))=0xc8000007;
    *((volatile AL_U32 *)(0xf8803888))=0xc8000007;
    *((volatile AL_U32 *)(0xf8803890))=0xc8000007;
    *((volatile AL_U32 *)(0xf8803898))=0xc8000007;
    *((volatile AL_U32 *)(0xf88038a0))=0xc8000007;
    *((volatile AL_U32 *)(0xf88038a8))=0xc8000007;
    *((volatile AL_U32 *)(0xf88038b0))=0x88000000;
    *((volatile AL_U32 *)(0xf88038b8))=0x88000000;
    *((volatile AL_U32 *)(0xf88038c0))=0x88000000;
    *((volatile AL_U32 *)(0xf88038c8))=0x88000000;
    *((volatile AL_U32 *)(0xf88038d0))=0x88000000;
    *((volatile AL_U32 *)(0xf88038d8))=0x88000000;
    *((volatile AL_U32 *)(0xf8803884))=0x121702;
    *((volatile AL_U32 *)(0xf880388c))=0x121702;
    *((volatile AL_U32 *)(0xf8803894))=0x121702;
    *((volatile AL_U32 *)(0xf880389c))=0x121702;
    *((volatile AL_U32 *)(0xf88038a4))=0x121702;
    *((volatile AL_U32 *)(0xf88038ac))=0x121702;
    *((volatile AL_U32 *)(0xf88038b4))=0x1E0602;
    *((volatile AL_U32 *)(0xf88038bc))=0x1E0602;
    *((volatile AL_U32 *)(0xf88038c4))=0x1E0602;
    *((volatile AL_U32 *)(0xf88038cc))=0x1E0602;
    *((volatile AL_U32 *)(0xf88038d4))=0x1E0602;
    *((volatile AL_U32 *)(0xf88038dc))=0x1E0602;

    /*USB0 Electrical Characteristics*/
    /*DATA[7_0]:pull-dn  STP:pull-up other:HiZ */
    /*1V8_1V5 20MA*/
    *((volatile AL_U32 *)(0xf88038e0)) = 0x48000007;
    *((volatile AL_U32 *)(0xf8803900)) = 0x48000007;
    *((volatile AL_U32 *)(0xf8803908)) = 0x48000007;
    *((volatile AL_U32 *)(0xf8803910)) = 0x48000007;
    *((volatile AL_U32 *)(0xf8803918)) = 0x48000007;
    *((volatile AL_U32 *)(0xf8803928)) = 0x48000007;
    *((volatile AL_U32 *)(0xf8803930)) = 0x48000007;
    *((volatile AL_U32 *)(0xf8803938)) = 0x48000007;
    *((volatile AL_U32 *)(0xf88038f0)) = 0x88000007;
    *((volatile AL_U32 *)(0xf88038e8)) = 0xc8000007;
    *((volatile AL_U32 *)(0xf88038f8)) = 0xc8000007;
    *((volatile AL_U32 *)(0xf8803920)) = 0xc8000007;
    *((volatile AL_U32 *)(0xf88038e4)) = 0x121702;
    *((volatile AL_U32 *)(0xf88038ec)) = 0x121702;
    *((volatile AL_U32 *)(0xf88038f4)) = 0x121702;
    *((volatile AL_U32 *)(0xf88038fc)) = 0x121702;
    *((volatile AL_U32 *)(0xf8803904)) = 0x121702;
    *((volatile AL_U32 *)(0xf880390c)) = 0x121702;
    *((volatile AL_U32 *)(0xf8803914)) = 0x121702;
    *((volatile AL_U32 *)(0xf880391c)) = 0x121702;
    *((volatile AL_U32 *)(0xf8803924)) = 0x121702;
    *((volatile AL_U32 *)(0xf880392c)) = 0x121702;
    *((volatile AL_U32 *)(0xf8803934)) = 0x121702;
    *((volatile AL_U32 *)(0xf880393c)) = 0x121702;
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




