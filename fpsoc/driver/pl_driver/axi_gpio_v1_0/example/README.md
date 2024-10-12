# GPIO

## 概述

GPIO示例工程演示了GPIO口读写功能和中断功能。

## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明
以下示例中用到的MIO/EMIO口均需要配置pinmux
示例1:axi_gpio_output_led.c
- 以CHANNEL的方式写寄存器，支持单/双通道
- 以BIT的方式写寄存器，位宽可编程

示例2:axi_gpio_input_key.c
- 支持单通道中断，支持双通道同时中断，中断位可选
