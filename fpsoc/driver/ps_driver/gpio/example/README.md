# GPIO

## 概述

GPIO示例工程演示了GPIO口读写功能和中断功能。

## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明
以下示例中用到的MIO/EMIO口均需要配置pinmux
示例1:al_gpio_ctl_LED.c
- 以BANK的方式控制LED闪烁
- 以PIN的方式用KEY控制LED闪烁

示例2:al_gpio_intr.c
- 支持边沿触发以及电平触发模式，支持不同BANK中的PIN同时触发中断。本示例中配置两个MIO口和一个EMIO口边沿触发。
