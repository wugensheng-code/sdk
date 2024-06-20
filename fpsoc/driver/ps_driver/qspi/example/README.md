# QSPI

## 概述

QSPI 示例工程演示了1线与4线模式下的 norflash 读写测试。

## 硬件设置

需要硬件配置兼容的 norflash。

## 已知问题

无已知问题

## 运行说明

示例1:al_qspi_dma_nor.c
- QSPI master 使用dma方法进行 nor flash 数据擦除，读写。

示例2:al_qspi_x1_nor.c
- QSPI master 1线 进行 nor flash 数据擦除，读写。

示例3:al_qspi_x4_nor.c
- QSPI master 4线 进行 nor flash 数据擦除，读写。
