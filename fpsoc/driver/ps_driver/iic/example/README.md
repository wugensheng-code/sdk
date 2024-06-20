# IIC

## 概述

I2C example 例程提供如下测试功能：

- dma 模式收发数据测试
- EEPROM 读写测试
- 不同设备地址模式收发测试

## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明

示例1:al_iic_eeprom_block.c
- I2C 中断模式下 EEPROM 读写测试

示例2:al_iic_master_10bit_block.c
- 先运行slave，再运行master
- I2C master 模式下 标准速度模式 10bit地址 数据收发测试

示例3:al_iic_master_block.c
- 先运行slave，再运行master
- I2C master 模式下 标准速度模式 7bit地址  数据收发测试

示例4:al_iic_master_dma.c
- 先运行slave，再运行master
- I2C master 模式下通过 dma 方式收发数据

示例5:al_iic_master_fast_7bit_block.c
- 先运行slave，再运行master
- I2C master 模式下 fast 模式 7bit地址  数据收发测试

示例6:al_iic_master_fast_10bit_block.c
- 先运行slave，再运行master
- I2C master 模式下 fast 模式 10bit地址  数据收发测试
