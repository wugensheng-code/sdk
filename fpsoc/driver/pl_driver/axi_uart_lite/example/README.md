# AXI-UART-LITE
## 概述

AXI-UART-LITE 示例工程展示了如何使用AXI-UART-LITE（通用异步收发器）外设进行阻塞式和非阻塞式通信。

## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明

示例1:al_axi_uart_lite_intr_block.c
- 本示例演示如何使用 AXI-UART-LITE（通用异步收发器）外设进行中断阻塞式通信。示例初始化AXI-UART-LITE模块，配置参数并进行中断阻塞式数据接收和发送。
- 在终端或其他设备上发送少于指定字节数的数据，软件将接收并回传数据。

示例2:al_axi_uart_lite_poll_block.c
- 本示例演示如何使用Anlogic AXI-UART-LITE（通用异步收发器）外设进行轮询阻塞式通信。示例初始化AXI-UART-LITE模块，配置参数并进行轮询阻塞式数据接收和发送。
- 在终端或其他设备上发送少于指定字节数的数据，软件将接收并回传相同数据。
