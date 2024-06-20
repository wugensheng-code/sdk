#  DMA

## 概述

dma example 例程提供如下测试功能：

- 申请/释放DMA资源
- 配置DMA通道
- 使用相应DMA资源的中断

## DMA Manager使用提示

- DMA Manager组件内部已经内置了中断处理函数，用户程序中不应当再声明中断处理函数


## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明

示例1:al_dma_mem_cpy_blocked.c
- mem-to-mem dma 数据传输测试

示例2:al_dma_mem_cpy_performance.c
- mem-to-mem dma 性能测试

示例3:al_dma_mem_cpy.c
- mem-to-mem dma 数据传输测试

示例4:al_dma_per_cpy_cycle.c
- I2S-DMA mem-to-dev & dev-to-mem 数据循环传输测试

示例5:al_dma_per_cpy_single.c
- I2S-DMA mem-to-dev & dev-to-mem 数据单次传输测试

