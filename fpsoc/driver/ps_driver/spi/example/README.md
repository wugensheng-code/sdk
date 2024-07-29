# SPI

## 概述

 SPI 例程提供如下测试功能：

- SPI master 收发数据
- SPI slave 收发数据
- SPI master 对norflash 读取ID、擦除、写入、读取等功能测试


## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明

示例1:al_spi_dma_nor.c
- SPI master 使用dma模式，读取nor flash ID，以及数据擦除，读写

示例2:al_spi_nor.c
- SPI master 使用中断模式，读取nor flash ID，以及数据擦除，读写

示例3:al_spi_master.c
- SPI master 数据收发

示例4:al_spi_slave.c
- SPI slave 数据收发
