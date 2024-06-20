# MMC

## 概述

MMC 示例工程演示了不同 DMA 类型下的数据读写测试方法。

## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明

示例1:al_mmc_adma2_blocked.c
- MMC adma2 模式 数据读写测试，读写测试完成后校验数据是否正确。

示例2:al_mmc_non_dma_blocked.c
- MMC no dma 模式 数据读写测试，读写测试完成后校验数据是否正确。

示例3:al_mmc_sdma_blocked.c
- MMC sdma 模式 数据读写测试，读写测试完成后校验数据是否正确。
 