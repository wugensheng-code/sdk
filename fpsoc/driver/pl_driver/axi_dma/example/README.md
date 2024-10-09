# PL AXI DMA

## 概述

AXI DMA 示例工程展示了如何使用 PL AXI DMA 进行 stream 数据搬运。

## 硬件设置

### Direct Mode

#### Disable Micro：

1. 传输数据量 <= (1 << buffer length) - 1

#### Enable Micro：

1. (Memory map Data Width / 8) * Burst Size / (Memory map Data Width / Stream data width) <= (1 << buffer length) - 1
2. 传输数据量 <= (Memory map Data Width / 8) * Burst Size / (Memory map Data Width / Stream data width)

### Scatter/Gather (SG) Mode

#### Disabled Micro：

1. 单个描述符数据量 <= (1 << buffer length) - 1
2. 所有描述符数据量 <= (1 << buffer length) - 1

#### Enable Micro：

1. (Memory map Data Width / 8) * Burst Size / (Memory map Data Width / Stream data width) <= (1 << buffer length) - 1
2. 单个描述符数据量 <= (Memory map Data Width / 8) * Burst Size / (Memory map Data Width / Stream data width)
3. 所有描述符数据量 <= (1 << buffer length) - 1

### Hardware

接在 PS 侧 GP 口，burst size 最大设置为 16。

## 已知问题

无已知问题

## 运行说明

示例1: axidma_direct_mode.c
- 本示例演示如何使用 PL AXI DMA direct 模式进行阻塞式/非阻塞数据搬运。需烧录 direct mode 对应的 bit。
- PL AXI DMA 内置一个数据收发器，用于将数据回环收发。

示例2: axidma_sg_mode.c
- 本示例演示如何使用 PL AXI DMA Scatter/Gather 模式进行阻塞式/非阻塞数据搬运。需烧录 Scatter/Gather mode 对应的 bit。
- PL AXI DMA 内置一个数据收发器，用于将数据回环收发。
