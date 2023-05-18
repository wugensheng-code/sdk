#ifndef __UIO_NPU_DRIVER_H__
#define __UIO_NPU_DRIVER_H__

#include <stdint.h>
#include "driver_api.h"
#include "npu_rt_graph.h"

#ifdef RUN_PLATFORM_LINUX
#include "hard_npu_uio.h"
#include "soft_npu_uio.h"
#else
#include "bare_metal_io.h"
#endif

#define HARD_NPU_MASK 0x63f00000
#define SOFT_NPU_MASK 0x91000000

#define HARD_NPU_MODE_LOAD_DATA_ADDR 0x50
#define HARD_NPU_IFM_BASE_ADDR 0x2000
#define HARD_NPU_FILTER_BASE_ADDR 0x2004
#define HARD_NPU_OFM_BASE_ADDR 0x6000

#define SOFT_NPU_FIRST_REG_ADDR 0x000
#define SOFT_NPU_OFM_BASE_ADDR 0x00c
#define SOFT_NPU_IFM0_BASE_ADDR 0x010
#define SOFT_NPU_IFM1_BASE_ADDR 0x014
#define SOFT_NPU_IFM2_BASE_ADDR 0x018
#define SOFT_NPU_IFM3_BASE_ADDR 0x01c

#define HARD_AXI_MASTER_TRANSFER_LIMIT_ADDR 0x3c
#define HARD_DMA_REG_BASE_ADDR 0x100c
#define HARD_DMA_LENGTH_ADDR 0x1010
#define HARD_DMA_READ_CFG_START_ADDR 0x1000

#define SOFT_DMA_REG_BASE_ADDR 0x04
#define SOFT_DMA_LENGTH_ADDR 0x08

#define HARDNPU__CCU__CLEAR_INTERRUPT__ADDR 0x101c
#define SOFTNPU__SOFT_SHARE__CFG_INIT_ADDR__ADDR 0x0
#define SOFTNPU__SOFT_YOLO__YOLO_C__ADDR 0x10c
#define SOFTNPU__SOFT_SHARE__CLEAR_INTERRUPT__ADDR 0xa4

#define SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR 0x200

int8_t run_npu_hard_normal_mode (hard_npu_pkg_info_t* pkg) ;
int8_t run_npu_hard_dma_mode (hard_npu_pkg_info_t* pkg) ;

int8_t run_npu_soft_normal_mode (soft_npu_pkg_info_t* pkg, rt_node_t* nd) ;
int8_t run_npu_soft_dma_mode (soft_npu_pkg_info_t* pkg, rt_node_t* nd) ;

int8_t run_npu_soft_yolo_normal_mode (yolo_pkg_info_t* pkg) ;
int8_t run_npu_soft_yolo_dma_mode (yolo_pkg_info_t* pkg) ;

#endif
