# SPI

## 概述

 SPI 示例工程演示了 norflash 读取ID、擦除、写入、读取等功能测试。

## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明

示例1:al_spi_dma_nor.c
- SPI master 使用dma方法进行 nor flash 数据擦除，读写。

示例2:al_spi_nor.c
- SPI master 进行 nor flash 数据擦除，读写。

示例3:al_spi_slave.c
- SPI slave 进行数据接收。
