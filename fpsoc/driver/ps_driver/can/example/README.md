# CAN

## 概述

can example 例程提供如下测试功能：

- CAN 外设的内/外回环测试
- CANFD 相关测试
- CAN-DMA 相关测试

## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明

示例1:al_can_ex_loopback_blocked.c
- CAN 外回环测试: ptb 传输模式，中断模式，CAN2.0标准。

示例2:al_can_fd_dma_llp_nonblock.c
- CAN 正常模式:ptb 传输模式，dma中断llp测试，CANFD标准。

示例3:al_can_fd_dma_nonblock.c
- CAN 正常模式:ptb 传输模式，dma中断，CANFD标准。

示例4:al_can_fd_stb_fifo_blocked.c
- CAN 正常模式: stb 传输模式，中断模式，CANFD标准。

示例5:al_can_in_loopback_blocked.c
- CAN 内回环测试: ptb 传输模式，中断模式，CAN2.0标准。

示例6:al_can_std_ptb_blocked.c
-CAN 正常模式: ptb 传输模式，中断模式，CAN2.0标准。
