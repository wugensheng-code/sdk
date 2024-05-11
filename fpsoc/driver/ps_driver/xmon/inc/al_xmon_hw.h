/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_UART_HW_H_
#define __AL_UART_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

#define AL_XID_MAX 		0xFFFF
#define AL_XSIZE_MAX 		0x7
#define AL_XLEN_MAX 		0xFF
#define AL_XTYPE_MAX 		0x3
#define AL_XQOS_MAX 		0xF

// ///////////////////////////////////////////////////////
// // Register: XMON_GLB_WIN
// // 
// ///////////////////////////////////////////////////////

// #define CRP__XMON_GLB_WIN__OFFSET (CRG__BASE_ADDR + 0x4B8ULL)
// #define CRP__XMON_GLB_WIN__NUM  0x1

// #define CRP__XMON_GLB_WIN__XMON_GLB_WIN__SHIFT    0
// #define CRP__XMON_GLB_WIN__RESERVED_31_1__SHIFT    1

// #define CRP__XMON_GLB_WIN__XMON_GLB_WIN__MASK    0x00000001
// #define CRP__XMON_GLB_WIN__RESERVED_31_1__MASK    0xfffffffe

// #define CRP__XMON_GLB_WIN__XMON_GLB_WIN__POR_VALUE    0x0
// #define CRP__XMON_GLB_WIN__RESERVED_31_1__POR_VALUE    0x0

// ///////////////////////////////////////////////////////
// // Register: INTR_XMON_STATE
// // 
// ///////////////////////////////////////////////////////

// #define TOP_NS__INTR_XMON_STATE__OFFSET (TOP_NS__BASE_ADDR + 0x210ULL)
// #define TOP_NS__INTR_XMON_STATE__NUM  0x1

// #define TOP_NS__INTR_XMON_STATE__INTR_XMON_STATE__SHIFT    0
// #define TOP_NS__INTR_XMON_STATE__RESERVED_31_21__SHIFT    21

// #define TOP_NS__INTR_XMON_STATE__INTR_XMON_STATE__MASK    0x001fffff
// #define TOP_NS__INTR_XMON_STATE__RESERVED_31_21__MASK    0xffe00000

// #define TOP_NS__INTR_XMON_STATE__INTR_XMON_STATE__POR_VALUE    0x0
// #define TOP_NS__INTR_XMON_STATE__RESERVED_31_21__POR_VALUE    0x0

// #define XMON__ACP__BASE_ADDR 0xF8446800ULL

///////////////////////////////////////////////////////
// Register: QOS_RECORD
// 
///////////////////////////////////////////////////////

#define XMON__QOS_RECORD__OFFSET (0x000ULL)

#define XMON__QOS_RECORD__ARQOS_RECORD__SHIFT    0
#define XMON__QOS_RECORD__AWQOS_RECORD__SHIFT    4
#define XMON__QOS_RECORD__RESERVED_31_8__SHIFT    8

#define XMON__QOS_RECORD__ARQOS_RECORD__MASK    0x0000000f
#define XMON__QOS_RECORD__AWQOS_RECORD__MASK    0x000000f0
#define XMON__QOS_RECORD__RESERVED_31_8__MASK    0xffffff00

#define XMON__QOS_RECORD__ARQOS_RECORD__POR_VALUE    0x0
#define XMON__QOS_RECORD__AWQOS_RECORD__POR_VALUE    0x0
#define XMON__QOS_RECORD__RESERVED_31_8__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: QOS_CONFIG
// 
///////////////////////////////////////////////////////

#define XMON__QOS_CONFIG__OFFSET (0x004ULL)
#define XMON__QOS_CONFIG__NUM  0x1

#define XMON__QOS_CONFIG__QOS_CONFIG_VALUE__SHIFT    0
#define XMON__QOS_CONFIG__QOS_CONFIG_EN__SHIFT    4
#define XMON__QOS_CONFIG__RESERVED_31_6__SHIFT    6

#define XMON__QOS_CONFIG__QOS_CONFIG_VALUE__MASK    0x0000000f
#define XMON__QOS_CONFIG__QOS_CONFIG_EN__MASK    0x00000030
#define XMON__QOS_CONFIG__RESERVED_31_6__MASK    0xffffffc0

#define XMON__QOS_CONFIG__QOS_CONFIG_VALUE__POR_VALUE    0x0
#define XMON__QOS_CONFIG__QOS_CONFIG_EN__POR_VALUE    0x0
#define XMON__QOS_CONFIG__RESERVED_31_6__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: QOS_THRESHOLD
// 
///////////////////////////////////////////////////////

#define XMON__QOS_THRESHOLD__OFFSET (0x008ULL)
#define XMON__QOS_THRESHOLD__NUM  0x1

#define XMON__QOS_THRESHOLD__QOS_MIN__SHIFT    0
#define XMON__QOS_THRESHOLD__QOS_MAX__SHIFT    4
#define XMON__QOS_THRESHOLD__RESERVED_31_8__SHIFT    8

#define XMON__QOS_THRESHOLD__QOS_MIN__MASK    0x0000000f
#define XMON__QOS_THRESHOLD__QOS_MAX__MASK    0x000000f0
#define XMON__QOS_THRESHOLD__RESERVED_31_8__MASK    0xffffff00

#define XMON__QOS_THRESHOLD__QOS_MIN__POR_VALUE    0x1
#define XMON__QOS_THRESHOLD__QOS_MAX__POR_VALUE    0xf
#define XMON__QOS_THRESHOLD__RESERVED_31_8__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: RESPTIME_CFG
// 
///////////////////////////////////////////////////////

#define XMON__RESPTIME_CFG__OFFSET (0x010ULL)
#define XMON__RESPTIME_CFG__NUM  0x1

#define XMON__RESPTIME_CFG__RESPTIME_ENABLE__SHIFT    0
#define XMON__RESPTIME_CFG__RESPTIME_CLEAN__SHIFT    1
#define XMON__RESPTIME_CFG__RESERVED_31_2__SHIFT    2

#define XMON__RESPTIME_CFG__RESPTIME_ENABLE__MASK    0x00000001
#define XMON__RESPTIME_CFG__RESPTIME_CLEAN__MASK    0x00000002
#define XMON__RESPTIME_CFG__RESERVED_31_2__MASK    0xfffffffc

#define XMON__RESPTIME_CFG__RESPTIME_ENABLE__POR_VALUE    0x0
#define XMON__RESPTIME_CFG__RESPTIME_CLEAN__POR_VALUE    0x0
#define XMON__RESPTIME_CFG__RESERVED_31_2__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: AWRESPTIME_MAX
// 
///////////////////////////////////////////////////////

#define XMON__AWRESPTIME_MAX__OFFSET (0x014ULL)
#define XMON__AWRESPTIME_MAX__NUM  0x1

#define XMON__AWRESPTIME_MAX__AWRESPTIME_MAX__SHIFT    0

#define XMON__AWRESPTIME_MAX__AWRESPTIME_MAX__MASK    0xffffffff

#define XMON__AWRESPTIME_MAX__AWRESPTIME_MAX__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: AWRESPTIME_ACC_H
// 
///////////////////////////////////////////////////////

#define XMON__AWRESPTIME_ACC_H__OFFSET (0x018ULL)
#define XMON__AWRESPTIME_ACC_H__NUM  0x1

#define XMON__AWRESPTIME_ACC_H__AWRESPTIME_ACC_H__SHIFT    0

#define XMON__AWRESPTIME_ACC_H__AWRESPTIME_ACC_H__MASK    0xffffffff

#define XMON__AWRESPTIME_ACC_H__AWRESPTIME_ACC_H__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: AWRESPTIME_ACC_L
// 
///////////////////////////////////////////////////////

#define XMON__AWRESPTIME_ACC_L__OFFSET (0x01CULL)
#define XMON__AWRESPTIME_ACC_L__NUM  0x1

#define XMON__AWRESPTIME_ACC_L__AWRESPTIME_ACC_L__SHIFT    0

#define XMON__AWRESPTIME_ACC_L__AWRESPTIME_ACC_L__MASK    0xffffffff

#define XMON__AWRESPTIME_ACC_L__AWRESPTIME_ACC_L__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: AWRESPTIME_CNT
// 
///////////////////////////////////////////////////////

#define XMON__AWRESPTIME_CNT__OFFSET (0x020ULL)
#define XMON__AWRESPTIME_CNT__NUM  0x1

#define XMON__AWRESPTIME_CNT__AWRESPTIME_CNT__SHIFT    0

#define XMON__AWRESPTIME_CNT__AWRESPTIME_CNT__MASK    0xffffffff

#define XMON__AWRESPTIME_CNT__AWRESPTIME_CNT__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: ARRESPTIME_MAX
// 
///////////////////////////////////////////////////////

#define XMON__ARRESPTIME_MAX__OFFSET (0x024ULL)
#define XMON__ARRESPTIME_MAX__NUM  0x1

#define XMON__ARRESPTIME_MAX__ARRESPTIME_MAX__SHIFT    0

#define XMON__ARRESPTIME_MAX__ARRESPTIME_MAX__MASK    0xffffffff

#define XMON__ARRESPTIME_MAX__ARRESPTIME_MAX__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: ARRESPTIME_ACC_H
// 
///////////////////////////////////////////////////////

#define XMON__ARRESPTIME_ACC_H__OFFSET (0x028ULL)
#define XMON__ARRESPTIME_ACC_H__NUM  0x1

#define XMON__ARRESPTIME_ACC_H__ARRESPTIME_ACC_H__SHIFT    0

#define XMON__ARRESPTIME_ACC_H__ARRESPTIME_ACC_H__MASK    0xffffffff

#define XMON__ARRESPTIME_ACC_H__ARRESPTIME_ACC_H__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: ARRESPTIME_ACC_L
// 
///////////////////////////////////////////////////////

#define XMON__ARRESPTIME_ACC_L__OFFSET (0x02CULL)
#define XMON__ARRESPTIME_ACC_L__NUM  0x1

#define XMON__ARRESPTIME_ACC_L__ARRESPTIME_ACC_L__SHIFT    0

#define XMON__ARRESPTIME_ACC_L__ARRESPTIME_ACC_L__MASK    0xffffffff

#define XMON__ARRESPTIME_ACC_L__ARRESPTIME_ACC_L__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: ARRESPTIME_CNT
// 
///////////////////////////////////////////////////////

#define XMON__ARRESPTIME_CNT__OFFSET (0x030ULL)
#define XMON__ARRESPTIME_CNT__NUM  0x1

#define XMON__ARRESPTIME_CNT__ARRESPTIME_CNT__SHIFT    0

#define XMON__ARRESPTIME_CNT__ARRESPTIME_CNT__MASK    0xffffffff

#define XMON__ARRESPTIME_CNT__ARRESPTIME_CNT__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: BANDWIDTH_TMWD
// 
///////////////////////////////////////////////////////

#define XMON__BANDWIDTH_TMWD__OFFSET (0x034ULL)
#define XMON__BANDWIDTH_TMWD__NUM  0x1

#define XMON__BANDWIDTH_TMWD__BANDWIDTH_TMWD__SHIFT    0

#define XMON__BANDWIDTH_TMWD__BANDWIDTH_TMWD__MASK    0xffffffff

#define XMON__BANDWIDTH_TMWD__BANDWIDTH_TMWD__POR_VALUE    0x400


///////////////////////////////////////////////////////
// Register: BANDWIDTH_WR
// 
///////////////////////////////////////////////////////

#define XMON__BANDWIDTH_WR__OFFSET (0x038ULL)
#define XMON__BANDWIDTH_WR__NUM  0x1

#define XMON__BANDWIDTH_WR__BANDWIDTH_WR__SHIFT    0

#define XMON__BANDWIDTH_WR__BANDWIDTH_WR__MASK    0xffffffff

#define XMON__BANDWIDTH_WR__BANDWIDTH_WR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: BANDWIDTH_RD
// 
///////////////////////////////////////////////////////

#define XMON__BANDWIDTH_RD__OFFSET (0x03CULL)
#define XMON__BANDWIDTH_RD__NUM  0x1

#define XMON__BANDWIDTH_RD__BANDWIDTH_RD__SHIFT    0

#define XMON__BANDWIDTH_RD__BANDWIDTH_RD__MASK    0xffffffff

#define XMON__BANDWIDTH_RD__BANDWIDTH_RD__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: SUBPARA_CONFIG
// 
///////////////////////////////////////////////////////

#define XMON__SUBPARA_CONFIG__OFFSET (0x040ULL)
#define XMON__SUBPARA_CONFIG__NUM  0x1

#define XMON__SUBPARA_CONFIG__SUBPARA_ENABLE__SHIFT    0
#define XMON__SUBPARA_CONFIG__SUBPARA_CLEAN__SHIFT    1
#define XMON__SUBPARA_CONFIG__RESERVED_31_24__SHIFT    2

#define XMON__SUBPARA_CONFIG__SUBPARA_ENABLE__MASK    0x00000001
#define XMON__SUBPARA_CONFIG__SUBPARA_CLEAN__MASK    0x00000002
#define XMON__SUBPARA_CONFIG__RESERVED_31_24__MASK    0xfffffffc

#define XMON__SUBPARA_CONFIG__SUBPARA_ENABLE__POR_VALUE    0x0
#define XMON__SUBPARA_CONFIG__SUBPARA_CLEAN__POR_VALUE    0x0
#define XMON__SUBPARA_CONFIG__RESERVED_31_24__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: SUBPARA_AWID
// 
///////////////////////////////////////////////////////

#define XMON__SUBPARA_AWID__OFFSET (0x044ULL)
#define XMON__SUBPARA_AWID__NUM  0x1

#define XMON__SUBPARA_AWID__SUBPARA_AWID_MIN__SHIFT    0
#define XMON__SUBPARA_AWID__SUBPARA_AWID_MAX__SHIFT    16

#define XMON__SUBPARA_AWID__SUBPARA_AWID_MIN__MASK    0x0000ffff
#define XMON__SUBPARA_AWID__SUBPARA_AWID_MAX__MASK    0xffff0000

#define XMON__SUBPARA_AWID__SUBPARA_AWID_MIN__POR_VALUE    0x0
#define XMON__SUBPARA_AWID__SUBPARA_AWID_MAX__POR_VALUE    0xffff


///////////////////////////////////////////////////////
// Register: SUBPARA_GRPID
// 
///////////////////////////////////////////////////////

#define XMON__SUBPARA_GRPID__OFFSET (0x048ULL)
#define XMON__SUBPARA_GRPID__NUM  0x1

#define XMON__SUBPARA_GRPID__SUBPARA_ARGID_MIN__SHIFT    0
#define XMON__SUBPARA_GRPID__SUBPARA_AWGID_MIN__SHIFT    8
#define XMON__SUBPARA_GRPID__SUBPARA_ARGID_MAX__SHIFT    16
#define XMON__SUBPARA_GRPID__SUBPARA_AWGID_MAX__SHIFT    24

#define XMON__SUBPARA_GRPID__SUBPARA_ARGID_MIN__MASK    0x000000ff
#define XMON__SUBPARA_GRPID__SUBPARA_AWGID_MIN__MASK    0x0000ff00
#define XMON__SUBPARA_GRPID__SUBPARA_ARGID_MAX__MASK    0x00ff0000
#define XMON__SUBPARA_GRPID__SUBPARA_AWGID_MAX__MASK    0xff000000

#define XMON__SUBPARA_GRPID__SUBPARA_ARGID_MIN__POR_VALUE    0x0
#define XMON__SUBPARA_GRPID__SUBPARA_AWGID_MIN__POR_VALUE    0x0
#define XMON__SUBPARA_GRPID__SUBPARA_ARGID_MAX__POR_VALUE    0x0
#define XMON__SUBPARA_GRPID__SUBPARA_AWGID_MAX__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: SUBPARA_AWBURST
// 
///////////////////////////////////////////////////////

#define XMON__SUBPARA_AWBURST__OFFSET (0x04CULL)
#define XMON__SUBPARA_AWBURST__NUM  0x1

#define XMON__SUBPARA_AWBURST__SUBPARA_AWBLEN_MIN__SHIFT    0
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBSIZE_MIN__SHIFT    8
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBTYPE_MIN__SHIFT    11
#define XMON__SUBPARA_AWBURST__RESERVED_15_13__SHIFT    13
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBLEN_MAX__SHIFT    16
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBSIZE_MAX__SHIFT    24
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBTYPE_MAX__SHIFT    27
#define XMON__SUBPARA_AWBURST__RESERVED_31_29__SHIFT    29

#define XMON__SUBPARA_AWBURST__SUBPARA_AWBLEN_MIN__MASK    0x000000ff
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBSIZE_MIN__MASK    0x00000700
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBTYPE_MIN__MASK    0x00001800
#define XMON__SUBPARA_AWBURST__RESERVED_15_13__MASK    0x0000e000
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBLEN_MAX__MASK    0x00ff0000
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBSIZE_MAX__MASK    0x07000000
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBTYPE_MAX__MASK    0x18000000
#define XMON__SUBPARA_AWBURST__RESERVED_31_29__MASK    0xe0000000

#define XMON__SUBPARA_AWBURST__SUBPARA_AWBLEN_MIN__POR_VALUE    0x0
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBSIZE_MIN__POR_VALUE    0x0
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBTYPE_MIN__POR_VALUE    0x0
#define XMON__SUBPARA_AWBURST__RESERVED_15_13__POR_VALUE    0x0
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBLEN_MAX__POR_VALUE    0xff
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBSIZE_MAX__POR_VALUE    0x7
#define XMON__SUBPARA_AWBURST__SUBPARA_AWBTYPE_MAX__POR_VALUE    0x3
#define XMON__SUBPARA_AWBURST__RESERVED_31_29__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: SUBPARA_ARID
// 
///////////////////////////////////////////////////////

#define XMON__SUBPARA_ARID__OFFSET (0x050ULL)
#define XMON__SUBPARA_ARID__NUM  0x1

#define XMON__SUBPARA_ARID__SUBPARA_ARID_MIN__SHIFT    0
#define XMON__SUBPARA_ARID__SUBPARA_ARID_MAX__SHIFT    16

#define XMON__SUBPARA_ARID__SUBPARA_ARID_MIN__MASK    0x0000ffff
#define XMON__SUBPARA_ARID__SUBPARA_ARID_MAX__MASK    0xffff0000

#define XMON__SUBPARA_ARID__SUBPARA_ARID_MIN__POR_VALUE    0x0
#define XMON__SUBPARA_ARID__SUBPARA_ARID_MAX__POR_VALUE    0xffff


///////////////////////////////////////////////////////
// Register: SUBPARA_ARBURST
// 
///////////////////////////////////////////////////////

#define XMON__SUBPARA_ARBURST__OFFSET (0x054ULL)
#define XMON__SUBPARA_ARBURST__NUM  0x1

#define XMON__SUBPARA_ARBURST__SUBPARA_ARBLEN_MIN__SHIFT    0
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBSIZE_MIN__SHIFT    8
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBTYPE_MIN__SHIFT    11
#define XMON__SUBPARA_ARBURST__RESERVED_15_13__SHIFT    13
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBLEN_MAX__SHIFT    16
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBSIZE_MAX__SHIFT    24
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBTYPE_MAX__SHIFT    27
#define XMON__SUBPARA_ARBURST__RESERVED_31_29__SHIFT    29

#define XMON__SUBPARA_ARBURST__SUBPARA_ARBLEN_MIN__MASK    0x000000ff
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBSIZE_MIN__MASK    0x00000700
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBTYPE_MIN__MASK    0x00001800
#define XMON__SUBPARA_ARBURST__RESERVED_15_13__MASK    0x0000e000
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBLEN_MAX__MASK    0x00ff0000
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBSIZE_MAX__MASK    0x07000000
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBTYPE_MAX__MASK    0x18000000
#define XMON__SUBPARA_ARBURST__RESERVED_31_29__MASK    0xe0000000

#define XMON__SUBPARA_ARBURST__SUBPARA_ARBLEN_MIN__POR_VALUE    0x0
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBSIZE_MIN__POR_VALUE    0x0
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBTYPE_MIN__POR_VALUE    0x0
#define XMON__SUBPARA_ARBURST__RESERVED_15_13__POR_VALUE    0x0
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBLEN_MAX__POR_VALUE    0xff
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBSIZE_MAX__POR_VALUE    0x7
#define XMON__SUBPARA_ARBURST__SUBPARA_ARBTYPE_MAX__POR_VALUE    0x3
#define XMON__SUBPARA_ARBURST__RESERVED_31_29__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_AWCMD
// 
///////////////////////////////////////////////////////

#define XMON__CNT_AWCMD__OFFSET (0x060ULL)
#define XMON__CNT_AWCMD__NUM  0x1

#define XMON__CNT_AWCMD__CNT_AWCMD__SHIFT    0

#define XMON__CNT_AWCMD__CNT_AWCMD__MASK    0xffffffff

#define XMON__CNT_AWCMD__CNT_AWCMD__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_AWBTYPE
// 
///////////////////////////////////////////////////////

#define XMON__CNT_AWBTYPE__OFFSET (0x064ULL)
#define XMON__CNT_AWBTYPE__NUM  0x1

#define XMON__CNT_AWBTYPE__CNT_AWBTYPE__SHIFT    0

#define XMON__CNT_AWBTYPE__CNT_AWBTYPE__MASK    0xffffffff

#define XMON__CNT_AWBTYPE__CNT_AWBTYPE__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_AWBLEN
// 
///////////////////////////////////////////////////////

#define XMON__CNT_AWBLEN__OFFSET (0x068ULL)
#define XMON__CNT_AWBLEN__NUM  0x1

#define XMON__CNT_AWBLEN__CNT_AWBLEN__SHIFT    0

#define XMON__CNT_AWBLEN__CNT_AWBLEN__MASK    0xffffffff

#define XMON__CNT_AWBLEN__CNT_AWBLEN__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_AWBSIZE
// 
///////////////////////////////////////////////////////

#define XMON__CNT_AWBSIZE__OFFSET (0x06CULL)
#define XMON__CNT_AWBSIZE__NUM  0x1

#define XMON__CNT_AWBSIZE__CNT_AWBSIZE__SHIFT    0

#define XMON__CNT_AWBSIZE__CNT_AWBSIZE__MASK    0xffffffff

#define XMON__CNT_AWBSIZE__CNT_AWBSIZE__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_AWID
// 
///////////////////////////////////////////////////////

#define XMON__CNT_AWID__OFFSET (0x070ULL)
#define XMON__CNT_AWID__NUM  0x1

#define XMON__CNT_AWID__CNT_AWID__SHIFT    0

#define XMON__CNT_AWID__CNT_AWID__MASK    0xffffffff

#define XMON__CNT_AWID__CNT_AWID__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_AWGID
// 
///////////////////////////////////////////////////////

#define XMON__CNT_AWGID__OFFSET (0x074ULL)
#define XMON__CNT_AWGID__NUM  0x1

#define XMON__CNT_AWGID__CNT_AWGID__SHIFT    0

#define XMON__CNT_AWGID__CNT_AWGID__MASK    0xffffffff

#define XMON__CNT_AWGID__CNT_AWGID__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_ARCMD
// 
///////////////////////////////////////////////////////

#define XMON__CNT_ARCMD__OFFSET (0x078ULL)
#define XMON__CNT_ARCMD__NUM  0x1

#define XMON__CNT_ARCMD__CNT_ARCMD__SHIFT    0

#define XMON__CNT_ARCMD__CNT_ARCMD__MASK    0xffffffff

#define XMON__CNT_ARCMD__CNT_ARCMD__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_ARBTYPE
// 
///////////////////////////////////////////////////////

#define XMON__CNT_ARBTYPE__OFFSET (0x07CULL)
#define XMON__CNT_ARBTYPE__NUM  0x1

#define XMON__CNT_ARBTYPE__CNT_ARBTYPE__SHIFT    0

#define XMON__CNT_ARBTYPE__CNT_ARBTYPE__MASK    0xffffffff

#define XMON__CNT_ARBTYPE__CNT_ARBTYPE__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_ARBLEN
// 
///////////////////////////////////////////////////////

#define XMON__CNT_ARBLEN__OFFSET (0x080ULL)
#define XMON__CNT_ARBLEN__NUM  0x1

#define XMON__CNT_ARBLEN__CNT_ARBLEN__SHIFT    0

#define XMON__CNT_ARBLEN__CNT_ARBLEN__MASK    0xffffffff

#define XMON__CNT_ARBLEN__CNT_ARBLEN__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_ARBSIZE
// 
///////////////////////////////////////////////////////

#define XMON__CNT_ARBSIZE__OFFSET (0x084ULL)
#define XMON__CNT_ARBSIZE__NUM  0x1

#define XMON__CNT_ARBSIZE__CNT_ARBSIZE__SHIFT    0

#define XMON__CNT_ARBSIZE__CNT_ARBSIZE__MASK    0xffffffff

#define XMON__CNT_ARBSIZE__CNT_ARBSIZE__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_ARID
// 
///////////////////////////////////////////////////////

#define XMON__CNT_ARID__OFFSET (0x088ULL)
#define XMON__CNT_ARID__NUM  0x1

#define XMON__CNT_ARID__CNT_ARID__SHIFT    0

#define XMON__CNT_ARID__CNT_ARID__MASK    0xffffffff

#define XMON__CNT_ARID__CNT_ARID__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNT_ARGID
// 
///////////////////////////////////////////////////////

#define XMON__CNT_ARGID__OFFSET (0x08CULL)
#define XMON__CNT_ARGID__NUM  0x1

#define XMON__CNT_ARGID__CNT_ARGID__SHIFT    0

#define XMON__CNT_ARGID__CNT_ARGID__MASK    0xffffffff

#define XMON__CNT_ARGID__CNT_ARGID__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DEBUG_FLAGINFO
// 
///////////////////////////////////////////////////////

#define XMON__DEBUG_FLAGINFO__OFFSET (0x0A0ULL)
#define XMON__DEBUG_FLAGINFO__NUM  0x1

#define XMON__DEBUG_FLAGINFO__DEBUG_ADDR_H__SHIFT    0
#define XMON__DEBUG_FLAGINFO__DEBUG_BURST_TYPE__SHIFT    8
#define XMON__DEBUG_FLAGINFO__DEBUG_BURST_LENGTH__SHIFT    10
#define XMON__DEBUG_FLAGINFO__DEBUG_BURST_SIZE__SHIFT    18
#define XMON__DEBUG_FLAGINFO__DEBUG_ACCESS_TYPE__SHIFT    21
#define XMON__DEBUG_FLAGINFO__DEBUG_CLEAR__SHIFT    22
#define XMON__DEBUG_FLAGINFO__RESERVED_31_24__SHIFT    23

#define XMON__DEBUG_FLAGINFO__DEBUG_ADDR_H__MASK    0x000000ff
#define XMON__DEBUG_FLAGINFO__DEBUG_BURST_TYPE__MASK    0x00000300
#define XMON__DEBUG_FLAGINFO__DEBUG_BURST_LENGTH__MASK    0x0003fc00
#define XMON__DEBUG_FLAGINFO__DEBUG_BURST_SIZE__MASK    0x001c0000
#define XMON__DEBUG_FLAGINFO__DEBUG_ACCESS_TYPE__MASK    0x00200000
#define XMON__DEBUG_FLAGINFO__DEBUG_CLEAR__MASK    0x00400000
#define XMON__DEBUG_FLAGINFO__RESERVED_31_24__MASK    0xff800000

#define XMON__DEBUG_FLAGINFO__DEBUG_ADDR_H__POR_VALUE    0x0
#define XMON__DEBUG_FLAGINFO__DEBUG_BURST_TYPE__POR_VALUE    0x0
#define XMON__DEBUG_FLAGINFO__DEBUG_BURST_LENGTH__POR_VALUE    0x0
#define XMON__DEBUG_FLAGINFO__DEBUG_BURST_SIZE__POR_VALUE    0x0
#define XMON__DEBUG_FLAGINFO__DEBUG_ACCESS_TYPE__POR_VALUE    0x0
#define XMON__DEBUG_FLAGINFO__DEBUG_CLEAR__POR_VALUE    0x0
#define XMON__DEBUG_FLAGINFO__RESERVED_31_24__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DEBUG_ADDR_L
// 
///////////////////////////////////////////////////////

#define XMON__DEBUG_ADDR_L__OFFSET (0x0A4ULL)
#define XMON__DEBUG_ADDR_L__NUM  0x1

#define XMON__DEBUG_ADDR_L__DEBUG_ADDR_L__SHIFT    0

#define XMON__DEBUG_ADDR_L__DEBUG_ADDR_L__MASK    0xffffffff

#define XMON__DEBUG_ADDR_L__DEBUG_ADDR_L__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DEBUG_ID
// 
///////////////////////////////////////////////////////

#define XMON__DEBUG_ID__OFFSET (0x0A8ULL)
#define XMON__DEBUG_ID__NUM  0x1

#define XMON__DEBUG_ID__DEBUG_ID__SHIFT    0

#define XMON__DEBUG_ID__DEBUG_ID__MASK    0xffffffff

#define XMON__DEBUG_ID__DEBUG_ID__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DEBUG_TIMEOUT_FLAG
// 
///////////////////////////////////////////////////////

#define XMON__DEBUG_TIMEOUT_FLAG__OFFSET (0x0ACULL)
#define XMON__DEBUG_TIMEOUT_FLAG__NUM  0x1

#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_RTIMEOUT__SHIFT    0
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_ARTIMEOUT__SHIFT    1
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_BTIMEOUT__SHIFT    2
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_WTIMEOUT__SHIFT    3
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_AWTIMEOUT__SHIFT    4
#define XMON__DEBUG_TIMEOUT_FLAG__RESERVED_31_5__SHIFT    5

#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_RTIMEOUT__MASK    0x00000001
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_ARTIMEOUT__MASK    0x00000002
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_BTIMEOUT__MASK    0x00000004
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_WTIMEOUT__MASK    0x00000008
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_AWTIMEOUT__MASK    0x00000010
#define XMON__DEBUG_TIMEOUT_FLAG__RESERVED_31_5__MASK    0xffffffe0

#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_RTIMEOUT__POR_VALUE    0x0
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_ARTIMEOUT__POR_VALUE    0x0
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_BTIMEOUT__POR_VALUE    0x0
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_WTIMEOUT__POR_VALUE    0x0
#define XMON__DEBUG_TIMEOUT_FLAG__DEBUG_AWTIMEOUT__POR_VALUE    0x0
#define XMON__DEBUG_TIMEOUT_FLAG__RESERVED_31_5__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DEBUG_TIMEOUT_THD
// 
///////////////////////////////////////////////////////

#define XMON__DEBUG_TIMEOUT_THD__OFFSET (0x0B0ULL)
#define XMON__DEBUG_TIMEOUT_THD__NUM  0x1

#define XMON__DEBUG_TIMEOUT_THD__DEBUG_TIMEOUT_THD__SHIFT    0

#define XMON__DEBUG_TIMEOUT_THD__DEBUG_TIMEOUT_THD__MASK    0xffffffff

#define XMON__DEBUG_TIMEOUT_THD__DEBUG_TIMEOUT_THD__POR_VALUE    0xffffffff


///////////////////////////////////////////////////////
// Register: INTR
// 
///////////////////////////////////////////////////////

#define XMON__INTR__OFFSET (0x0F0ULL)
#define XMON__INTR__NUM  0x1

#define XMON__INTR__DEBUG_RESP__SHIFT    0
#define XMON__INTR__DEBUG_TIMEOUT__SHIFT    1
#define XMON__INTR__INTR_CLEAR__SHIFT    2
#define XMON__INTR__INTR_MASK__SHIFT    3
#define XMON__INTR__INTR_ENABLE__SHIFT    4
#define XMON__INTR__RESERVED_31_5__SHIFT    5

#define XMON__INTR__DEBUG_RESP__MASK    0x00000001
#define XMON__INTR__DEBUG_TIMEOUT__MASK    0x00000002
#define XMON__INTR__INTR_CLEAR__MASK    0x00000004
#define XMON__INTR__INTR_MASK__MASK    0x00000008
#define XMON__INTR__INTR_ENABLE__MASK    0x00000010
#define XMON__INTR__RESERVED_31_5__MASK    0xffffffe0

#define XMON__INTR__DEBUG_RESP__POR_VALUE    0x0
#define XMON__INTR__DEBUG_TIMEOUT__POR_VALUE    0x0
#define XMON__INTR__INTR_CLEAR__POR_VALUE    0x0
#define XMON__INTR__INTR_MASK__POR_VALUE    0x0
#define XMON__INTR__INTR_ENABLE__POR_VALUE    0x0
#define XMON__INTR__RESERVED_31_5__POR_VALUE    0x0

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**************************** Type Definitions ******************************/

typedef union {
    struct {
        unsigned ArQos_Record : 4;
        unsigned AwQos_Record : 4;
        unsigned Reserved_31_8 : 24;
    };
    AL_U32 Reg;
} AL_XMON_QosRecord;

typedef union {
    struct {
        unsigned AwQos_RecordL : 32;
        unsigned AwQos_RecordH : 32;
    };
    AL_U64 AwRespTimeAcc;
} AL_XMON_AwResptimeAcc;

typedef union {
    struct {
        unsigned ArQos_RecordL : 32;
        unsigned ArQos_RecordH : 32;
    };
    AL_U64 ArRespTimeAcc;
} AL_XMON_ArResptimeAcc;

typedef enum
{
    AL_QOS_Fixed                        = 0,
    AL_QOS_Master_Through_Bypass        = 1,
    AL_QOS_Ip_Inter_Dynamic             = 2,
} AL_XMON_QosMode;

typedef struct
{
    AL_U32                  DeviceId;
    AL_U32                  BaseAddress;
} AL_XMON_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
