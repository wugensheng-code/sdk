# GBE

## 概述

gbe  例程提供如下测试功能：

- GBE 外设的内回环测试
- GBE 轮询模式 ping reply 测试
- GBE 中断模式 ping reply 测试

## 硬件设置

无特殊设置

## 已知问题

无已知问题

## 运行说明

示例1:al_gbe_internal_loopback.c
- GBE 内回环测试，需要接上网线

示例2:al_gbe_ping_reply_polling.c
- GBE 轮询模式ping reply测试，运行后，用一台主机ping开发板，开发板回应ping

示例3:al_gbe_ping_reply_intr.c
- GBE 中断模式ping reply测试，运行后，用一台主机ping开发板，开发板回应ping
