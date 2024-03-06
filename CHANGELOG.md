# CHANGELOG

## BM_SDK_2023.3_ES1.0 (2024-3-6)

### (d52893591d0136e91da41267faa4d5c9ff398cb2)

### Fix

- 修复rtt中cli的线程阻塞问题。

- 完善 bspconfig。

- 修复adc再fd中存在的部分问题。

- 完善gbe example 中的log。

### Feature

- 添加 ipc，xmon 驱动和相应 example。

- 添加 gbe ptp example。

- 添加FD中的链接脚本模板。

- 完善ddr驱动的v3版本。

- 添加 openamp proxy rpc demo。

- 添加 dma-mem-cpy-performance case in FD。

## BM_2024.01_Beta1.2 (2024-1-12)

### (75b7cf3cd9136bec266042952bb3d07485625a64)

### Feat

- 增加对1588的支持到v2版本。

- 更新ddr到v3版本。

### Fix

- 修复smc中read nand paramater crc错误。

- 修复qspi中4 addr mode错误。

- 修复riscv fp寄存器问题。


## BM_2023.11_Bate1.0 (2023-11-28)

### (e2f479612ee0df223c092cd26b407bd8831508a6)

### Fix

- 清理以下模块中存在的警告:

  gic, cipher, osal, uart, usb, spi, qspi, smc, ttc, gpio, can, ahb-dma, axi-dma, fatfs, plat。

- 修复uart轮询发送中存在的问题。

- 修复freertos（arm）中中断向量表未对齐的问题。

- 修复驱动中寄存器bit读写接口存在的问题。

- 修复ps-pl port 初始化的问题。

- 修复depend.json部分问题。

### Feature

- 添加bsp_settings的json，为后续的图形界面配置做准备。

- openamp中收到shutdown消息后回复ack消息。

- 添加neon的example。


## BM_2023.11_Alpha1.2 (2023-11-16)

### Fix

- 在 aarch64 启动的过程中使能同步异常。

- 移除 uart 初始化过程中的延时操作，以避免 openamp uart 没有打印。

- 移除 rt-thread 中原生的 gic 代码，统一为 baremetal 的 gic 初始化操作，以避免 gic 初始化后的状态混乱。

- 增大 rt-thread 中 cli thread 的栈空间，以避免栈溢出。

- 修复 CAN 驱动中存在的 DMA 传输问题：

- 将 Arm 核的启动符号固定到起始地址。

- 修复 gpio 驱动的中断服务函数中的部分逻辑混乱的问题。

### Feature

- 添加 openamp 在 freertos 上运行的 demo。

- 添加 openamp 在 rt-thread 上运行的 demo。

- 添加 readme

