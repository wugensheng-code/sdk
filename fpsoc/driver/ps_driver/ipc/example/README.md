# IPC

## 概述

IPC示例工程演示了三种核间通信的交互方式。

## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明

示例1:al_ipc_intr.c
- 通过 request/acknowledgement 中断对进行核间通信。

示例2:al_ipc_mailbox.c
- 通过核间的mailbox数据交换进行通信。

示例2:al_ipc_spinlock.c
- 通过对 spin lock的争夺和状态查询进行核间通信。

### 注意事项
通过中断对进行核间通信时，需要打开 APU 和 RPU 两个宏，APU & RPU 各编译一个elf , 先load RPU 的elf ,再load APU 的elf。
