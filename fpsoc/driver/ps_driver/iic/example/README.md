# IIC

## 概述

IIC 例程提供如下测试功能：

- DMA 模式收发数据测试
- EEPROM 读写测试
- 7/10 bit 地址模式，standard/fast 模式下收发测试

## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明

示例1:al_iic_eeprom_block.c
- IIC 中断模式下 EEPROM 读写测试

示例2:al_iic_master_10bit_block.c
- 先运行slave，再运行master
- IIC master 模式下 标准速度模式 10bit地址 数据收发测试

示例3:al_iic_master_block.c
- 先运行slave，再运行master
- IIC master 模式下 标准速度模式 7bit地址  数据收发测试

示例4:al_iic_master_dma.c
- 先运行slave，再运行master
- IIC master 模式下通过 dma 方式收发数据

示例5:al_iic_master_fast_7bit_block.c
- 先运行slave，再运行master
- IIC master 模式下 fast 模式 7bit地址  数据收发测试

示例6:al_iic_master_fast_10bit_block.c
- 先运行slave，再运行master
- IIC master 模式下 fast 模式 10bit地址  数据收发测试
