# AXI-QSPI

## 概述

axi-qspi example 例程提供如下测试功能：

- 中断/轮询测试
- flash 读写擦除测试
- Standard, Dual, Quad mode test

## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明

示例1: al_axi_qspi_x1_intr_example.c
- 中断模式下flash读写测试。

示例2: al_axi_qspi_x1_polled_example.c
- 轮询模式下flash读写测试。

示例3: al_axi_qspi_x1_3byte_unit_intr_example.c
- 中断模式下，使用3Byte地址进行flash读写测试。

示例4: al_axi_qspi_x1_3byte_unit_polled_example.c
- 轮询模式下，使用3Byte地址进行flash读写测试。

示例5: al_axi_qspi_x1_4byte_unit_intr_example.c
- 中断模式下，使用4Byte地址进行flash读写测试。

示例6: al_axi_qspi_x1_4byte_unit_polled_example.c
- 轮询模式下，使用4Byte地址进行flash读写测试。

示例7: al_axi_qspi_x2_3byte_unit_polled_example.c
- 轮询模式下，Dual Spi模式下，使用3Byte地址进行flash读写测试。

示例8: al_axi_qspi_x4_3byte_unit_polled_example.c
- 轮询模式下，Quad Spi模式下，使用3Byte地址进行flash读写测试。
