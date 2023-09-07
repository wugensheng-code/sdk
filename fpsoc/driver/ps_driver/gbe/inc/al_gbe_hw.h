/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_GBE_HW_H_
#define __AL_GBE_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

///////////////////////////////////////////////////////
// Register: DMA_Mode
// Tx Packet Count Good
///////////////////////////////////////////////////////

#define GBE__DMA_MODE__OFFSET (0x1000ULL)
#define GBE__DMA_MODE__NUM  0x1

#define GBE__DMA_MODE__SWR__SHIFT    0
#define GBE__DMA_MODE__DA__SHIFT    1
#define GBE__DMA_MODE__RESERVED_4_2__SHIFT    2
#define GBE__DMA_MODE__RESERVED_7_5__SHIFT    5
#define GBE__DMA_MODE__RESERVED_8__SHIFT    8
#define GBE__DMA_MODE__ARBC__SHIFT    9
#define GBE__DMA_MODE__RESERVED_10__SHIFT    10
#define GBE__DMA_MODE__TXPR__SHIFT    11
#define GBE__DMA_MODE__PR__SHIFT    12
#define GBE__DMA_MODE__RESERVED_15__SHIFT    15
#define GBE__DMA_MODE__INTM__SHIFT    16
#define GBE__DMA_MODE__RESERVED_31_18__SHIFT    18

#define GBE__DMA_MODE__PR__SIZE    2

///////////////////////////////////////////////////////
// Register: DMA_SysBus_Mode
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_SYSBUS_MODE__OFFSET (0x1004ULL)
#define GBE__DMA_SYSBUS_MODE__NUM  0x1

#define GBE__DMA_SYSBUS_MODE__FB__SHIFT    0
#define GBE__DMA_SYSBUS_MODE__BLEN4__SHIFT    1
#define GBE__DMA_SYSBUS_MODE__BLEN8__SHIFT    2
#define GBE__DMA_SYSBUS_MODE__BLEN16__SHIFT    3
#define GBE__DMA_SYSBUS_MODE__BLEN32__SHIFT    4
#define GBE__DMA_SYSBUS_MODE__BLEN64__SHIFT    5
#define GBE__DMA_SYSBUS_MODE__BLEN128__SHIFT    6
#define GBE__DMA_SYSBUS_MODE__BLEN256__SHIFT    7
#define GBE__DMA_SYSBUS_MODE__RESERVED_11_8__SHIFT    8
#define GBE__DMA_SYSBUS_MODE__AAL__SHIFT    12
#define GBE__DMA_SYSBUS_MODE__RESERVED_13__SHIFT    13
#define GBE__DMA_SYSBUS_MODE__MB__SHIFT    14
#define GBE__DMA_SYSBUS_MODE__RB__SHIFT    15
#define GBE__DMA_SYSBUS_MODE__RESERVED_31_16__SHIFT    16

#define GBE__DMA_SYSBUS_MODE__BLEN__SIZE    7

///////////////////////////////////////////////////////
// Register: DMA_Interrupt_Status
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_INTERRUPT_STATUS__OFFSET (0x1008ULL)
#define GBE__DMA_INTERRUPT_STATUS__NUM  0x1

#define GBE__DMA_INTERRUPT_STATUS__DC0IS__SHIFT    0
#define GBE__DMA_INTERRUPT_STATUS__RESERVED_15_1__SHIFT    1
#define GBE__DMA_INTERRUPT_STATUS__MTLIS__SHIFT    16
#define GBE__DMA_INTERRUPT_STATUS__MACIS__SHIFT    17
#define GBE__DMA_INTERRUPT_STATUS__RESERVED_31_18__SHIFT    18

///////////////////////////////////////////////////////
// Register: DMA_Debug_Status0
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_DEBUG_STATUS0__OFFSET (0x100CULL)
#define GBE__DMA_DEBUG_STATUS0__NUM  0x1

#define GBE__DMA_DEBUG_STATUS0__AXWHSTS__SHIFT    0
#define GBE__DMA_DEBUG_STATUS0__RESERVED_7_1__SHIFT    1
#define GBE__DMA_DEBUG_STATUS0__RPS0__SHIFT    8
#define GBE__DMA_DEBUG_STATUS0__TPS0__SHIFT    12
#define GBE__DMA_DEBUG_STATUS0__RESERVED_31_16__SHIFT    16

///////////////////////////////////////////////////////
// Register: DMA_CH0_Control
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_CONTROL__OFFSET (0x1100ULL)
#define GBE__DMA_CH0_CONTROL__NUM  0x1

#define GBE__DMA_CH0_CONTROL__MSS__SHIFT    0
#define GBE__DMA_CH0_CONTROL__RESERVED_15_14__SHIFT    14
#define GBE__DMA_CH0_CONTROL__PBLX8__SHIFT    16
#define GBE__DMA_CH0_CONTROL__RESERVED_17__SHIFT    17
#define GBE__DMA_CH0_CONTROL__DSL__SHIFT    18
#define GBE__DMA_CH0_CONTROL__RESERVED_31_21__SHIFT    21

#define GBE__DMA_CH0_CONTROL__MSS__SIZE    14

#define GBE__DMA_CH0_CONTROL__DSL__SIZE    3

///////////////////////////////////////////////////////
// Register: DMA_CH0_Tx_Control
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_TX_CONTROL__OFFSET (0x1104ULL)
#define GBE__DMA_CH0_TX_CONTROL__NUM  0x1

#define GBE__DMA_CH0_TX_CONTROL__ST__SHIFT    0
#define GBE__DMA_CH0_TX_CONTROL__RESERVED_3_1__SHIFT    1
#define GBE__DMA_CH0_TX_CONTROL__OSF__SHIFT    4
#define GBE__DMA_CH0_TX_CONTROL__RESERVED_11_5__SHIFT    5
#define GBE__DMA_CH0_TX_CONTROL__TSE__SHIFT    12
#define GBE__DMA_CH0_TX_CONTROL__RESERVED_15_13__SHIFT    13
#define GBE__DMA_CH0_TX_CONTROL__TXPBL__SHIFT    16
#define GBE__DMA_CH0_TX_CONTROL__ETIC__SHIFT    22
#define GBE__DMA_CH0_TX_CONTROL__RESERVED_31_23__SHIFT    23

#define GBE__DMA_CH0_TX_CONTROL__TXPBL__SIZE    6

///////////////////////////////////////////////////////
// Register: DMA_CH0_Rx_Control
//  
// Rx Packet Flush.
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_RX_CONTROL__OFFSET (0x1108ULL)
#define GBE__DMA_CH0_RX_CONTROL__NUM  0x1

#define GBE__DMA_CH0_RX_CONTROL__SR__SHIFT    0
#define GBE__DMA_CH0_RX_CONTROL__RBSZ_X_0__SHIFT    1
#define GBE__DMA_CH0_RX_CONTROL__RBSZ_13_Y__SHIFT    3
#define GBE__DMA_CH0_RX_CONTROL__RESERVED_15__SHIFT    15
#define GBE__DMA_CH0_RX_CONTROL__RXPBL__SHIFT    16
#define GBE__DMA_CH0_RX_CONTROL__ERIC__SHIFT    22
#define GBE__DMA_CH0_RX_CONTROL__RESERVED_30_23__SHIFT    23
#define GBE__DMA_CH0_RX_CONTROL__RPF__SHIFT    31

#define GBE__DMA_CH0_RX_CONTROL__RXPBL__SIZE    6
#define GBE__DMA_CH0_RX_CONTROL__RBSZ_13_Y__SIZE    13

///////////////////////////////////////////////////////
// Register: DMA_CH0_TxDesc_List_Address
// Start of Transmit List
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_TXDESC_LIST_ADDRESS__OFFSET (0x1114ULL)
#define GBE__DMA_CH0_TXDESC_LIST_ADDRESS__NUM  0x1

#define GBE__DMA_CH0_TXDESC_LIST_ADDRESS__RESERVED_LSB__SHIFT    0
#define GBE__DMA_CH0_TXDESC_LIST_ADDRESS__TDESLA__SHIFT    2

#define GBE__DMA_CH0_TXDESC_LIST_ADDRESS__TDESLA__SIZE    30

///////////////////////////////////////////////////////
// Register: DMA_CH0_RxDesc_List_Address
// Start of Receive List
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_RXDESC_LIST_ADDRESS__OFFSET (0x111CULL)
#define GBE__DMA_CH0_RXDESC_LIST_ADDRESS__NUM  0x1

#define GBE__DMA_CH0_RXDESC_LIST_ADDRESS__RESERVED_LSB__SHIFT    0
#define GBE__DMA_CH0_RXDESC_LIST_ADDRESS__RDESLA__SHIFT    2

#define GBE__DMA_CH0_RXDESC_LIST_ADDRESS__RDESLA__SIZE    30

///////////////////////////////////////////////////////
// Register: DMA_CH0_TxDesc_Tail_Pointer
// Transmit Descriptor Tail Pointer
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_TXDESC_TAIL_POINTER__OFFSET (0x1120ULL)
#define GBE__DMA_CH0_TXDESC_TAIL_POINTER__NUM  0x1

#define GBE__DMA_CH0_TXDESC_TAIL_POINTER__RESERVED_LSB__SHIFT    0
#define GBE__DMA_CH0_TXDESC_TAIL_POINTER__TDTP__SHIFT    2

#define GBE__DMA_CH0_TXDESC_TAIL_POINTER__TDTP__SIZE    30

///////////////////////////////////////////////////////
// Register: DMA_CH0_RxDesc_Tail_Pointer
// Receive Descriptor Tail Pointer
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_RXDESC_TAIL_POINTER__OFFSET (0x1128ULL)
#define GBE__DMA_CH0_RXDESC_TAIL_POINTER__NUM  0x1

#define GBE__DMA_CH0_RXDESC_TAIL_POINTER__RESERVED_LSB__SHIFT    0
#define GBE__DMA_CH0_RXDESC_TAIL_POINTER__RDTP__SHIFT    2

#define GBE__DMA_CH0_RXDESC_TAIL_POINTER__RDTP__SIZE    30

///////////////////////////////////////////////////////
// Register: DMA_CH0_TxDesc_Ring_Length
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_TXDESC_RING_LENGTH__OFFSET (0x112CULL)
#define GBE__DMA_CH0_TXDESC_RING_LENGTH__NUM  0x1

#define GBE__DMA_CH0_TXDESC_RING_LENGTH__TDRL__SHIFT    0
#define GBE__DMA_CH0_TXDESC_RING_LENGTH__RESERVED_31_10__SHIFT    10

#define GBE__DMA_CH0_TXDESC_RING_LENGTH__TDRL__SIZE    10

///////////////////////////////////////////////////////
// Register: DMA_CH0_RxDesc_Ring_Length
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_RXDESC_RING_LENGTH__OFFSET (0x1130ULL)
#define GBE__DMA_CH0_RXDESC_RING_LENGTH__NUM  0x1

#define GBE__DMA_CH0_RXDESC_RING_LENGTH__RDRL__SHIFT    0
#define GBE__DMA_CH0_RXDESC_RING_LENGTH__RESERVED_31_10__SHIFT    10

#define GBE__DMA_CH0_RXDESC_RING_LENGTH__RDRL__SIZE    10

///////////////////////////////////////////////////////
// Register: DMA_CH0_Interrupt_Enable
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_INTERRUPT_ENABLE__OFFSET (0x1134ULL)
#define GBE__DMA_CH0_INTERRUPT_ENABLE__NUM  0x1

#define GBE__DMA_CH0_INTERRUPT_ENABLE__TIE__SHIFT    0
#define GBE__DMA_CH0_INTERRUPT_ENABLE__TXSE__SHIFT    1
#define GBE__DMA_CH0_INTERRUPT_ENABLE__TBUE__SHIFT    2
#define GBE__DMA_CH0_INTERRUPT_ENABLE__RESERVED_5_3__SHIFT    3
#define GBE__DMA_CH0_INTERRUPT_ENABLE__RIE__SHIFT    6
#define GBE__DMA_CH0_INTERRUPT_ENABLE__RBUE__SHIFT    7
#define GBE__DMA_CH0_INTERRUPT_ENABLE__RSE__SHIFT    8
#define GBE__DMA_CH0_INTERRUPT_ENABLE__RWTE__SHIFT    9
#define GBE__DMA_CH0_INTERRUPT_ENABLE__ETIE__SHIFT    10
#define GBE__DMA_CH0_INTERRUPT_ENABLE__ERIE__SHIFT    11
#define GBE__DMA_CH0_INTERRUPT_ENABLE__FBEE__SHIFT    12
#define GBE__DMA_CH0_INTERRUPT_ENABLE__CDEE__SHIFT    13
#define GBE__DMA_CH0_INTERRUPT_ENABLE__AIE__SHIFT    14
#define GBE__DMA_CH0_INTERRUPT_ENABLE__NIE__SHIFT    15
#define GBE__DMA_CH0_INTERRUPT_ENABLE__RESERVED_31_16__SHIFT    16

///////////////////////////////////////////////////////
// Register: DMA_CH0_Rx_Interrupt_Watchdog_Timer
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__OFFSET (0x1138ULL)
#define GBE__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__NUM  0x1

#define GBE__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RWT__SHIFT    0
#define GBE__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RESERVED_15_8__SHIFT    8
#define GBE__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RWTU__SHIFT    16
#define GBE__DMA_CH0_RX_INTERRUPT_WATCHDOG_TIMER__RESERVED_31_18__SHIFT    18

///////////////////////////////////////////////////////
// Register: DMA_CH0_Current_App_TxDesc
// Application Transmit Descriptor OFFSETess Pointer
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_CURRENT_APP_TXDESC__OFFSET (0x1144ULL)
#define GBE__DMA_CH0_CURRENT_APP_TXDESC__NUM  0x1

#define GBE__DMA_CH0_CURRENT_APP_TXDESC__CURTDESAPTR__SHIFT    0

///////////////////////////////////////////////////////
// Register: DMA_CH0_Current_App_RxDesc
// Application Receive Descriptor OFFSETess Pointer
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_CURRENT_APP_RXDESC__OFFSET (0x114CULL)
#define GBE__DMA_CH0_CURRENT_APP_RXDESC__NUM  0x1

#define GBE__DMA_CH0_CURRENT_APP_RXDESC__CURRDESAPTR__SHIFT    0

///////////////////////////////////////////////////////
// Register: DMA_CH0_Current_App_TxBuffer
// Application Transmit Buffer OFFSETess Pointer
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_CURRENT_APP_TXBUFFER__OFFSET (0x1154ULL)
#define GBE__DMA_CH0_CURRENT_APP_TXBUFFER__NUM  0x1

#define GBE__DMA_CH0_CURRENT_APP_TXBUFFER__CURTBUFAPTR__SHIFT    0

///////////////////////////////////////////////////////
// Register: DMA_CH0_Current_App_RxBuffer
// Application Receive Buffer OFFSETess Pointer
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_CURRENT_APP_RXBUFFER__OFFSET (0x115CULL)
#define GBE__DMA_CH0_CURRENT_APP_RXBUFFER__NUM  0x1

#define GBE__DMA_CH0_CURRENT_APP_RXBUFFER__CURRBUFAPTR__SHIFT    0

///////////////////////////////////////////////////////
// Register: DMA_CH0_Status
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_STATUS__OFFSET (0x1160ULL)
#define GBE__DMA_CH0_STATUS__NUM  0x1

#define GBE__DMA_CH0_STATUS__TI__SHIFT    0
#define GBE__DMA_CH0_STATUS__TPS__SHIFT    1
#define GBE__DMA_CH0_STATUS__TBU__SHIFT    2
#define GBE__DMA_CH0_STATUS__RESERVED_5_3__SHIFT    3
#define GBE__DMA_CH0_STATUS__RI__SHIFT    6
#define GBE__DMA_CH0_STATUS__RBU__SHIFT    7
#define GBE__DMA_CH0_STATUS__RPS__SHIFT    8
#define GBE__DMA_CH0_STATUS__RWT__SHIFT    9
#define GBE__DMA_CH0_STATUS__ETI__SHIFT    10
#define GBE__DMA_CH0_STATUS__ERI__SHIFT    11
#define GBE__DMA_CH0_STATUS__FBE__SHIFT    12
#define GBE__DMA_CH0_STATUS__CDE__SHIFT    13
#define GBE__DMA_CH0_STATUS__AIS__SHIFT    14
#define GBE__DMA_CH0_STATUS__NIS__SHIFT    15
#define GBE__DMA_CH0_STATUS__TEB__SHIFT    16
#define GBE__DMA_CH0_STATUS__REB__SHIFT    19
#define GBE__DMA_CH0_STATUS__RESERVED_31_22__SHIFT    22

///////////////////////////////////////////////////////
// Register: DMA_CH0_Miss_Frame_Cnt
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_MISS_FRAME_CNT__OFFSET (0x1164ULL)
#define GBE__DMA_CH0_MISS_FRAME_CNT__NUM  0x1

#define GBE__DMA_CH0_MISS_FRAME_CNT__MFC__SHIFT    0
#define GBE__DMA_CH0_MISS_FRAME_CNT__RESERVED_14_11__SHIFT    11
#define GBE__DMA_CH0_MISS_FRAME_CNT__MFCO__SHIFT    15
#define GBE__DMA_CH0_MISS_FRAME_CNT__RESERVED_31_16__SHIFT    16

///////////////////////////////////////////////////////
// Register: DMA_CH0_RX_ERI_Cnt
// Reserved.
///////////////////////////////////////////////////////

#define GBE__DMA_CH0_RX_ERI_CNT__OFFSET (0x1168ULL)
#define GBE__DMA_CH0_RX_ERI_CNT__NUM  0x1

#define GBE__DMA_CH0_RX_ERI_CNT__ECNT__SHIFT    0
#define GBE__DMA_CH0_RX_ERI_CNT__RESERVED_31_12__SHIFT    12

///////////////////////////////////////////////////////
// Register: MAC_Configuration
// ARP Offload Enable
///////////////////////////////////////////////////////

#define GBE__MAC_CONFIGURATION__OFFSET (0x0ULL)
#define GBE__MAC_CONFIGURATION__NUM  0x1

#define GBE__MAC_CONFIGURATION__RE__SHIFT    0
#define GBE__MAC_CONFIGURATION__TE__SHIFT    1
#define GBE__MAC_CONFIGURATION__PRELEN__SHIFT    2
#define GBE__MAC_CONFIGURATION__RESERVED_10_4__SHIFT    4
#define GBE__MAC_CONFIGURATION__ECRSFD__SHIFT    11
#define GBE__MAC_CONFIGURATION__LM__SHIFT    12
#define GBE__MAC_CONFIGURATION__DM__SHIFT    13
#define GBE__MAC_CONFIGURATION__FES__SHIFT    14
#define GBE__MAC_CONFIGURATION__PS__SHIFT    15
#define GBE__MAC_CONFIGURATION__JE__SHIFT    16
#define GBE__MAC_CONFIGURATION__JD__SHIFT    17
#define GBE__MAC_CONFIGURATION__RESERVED_18__SHIFT    18
#define GBE__MAC_CONFIGURATION__WD__SHIFT    19
#define GBE__MAC_CONFIGURATION__ACS__SHIFT    20
#define GBE__MAC_CONFIGURATION__CST__SHIFT    21
#define GBE__MAC_CONFIGURATION__S2KP__SHIFT    22
#define GBE__MAC_CONFIGURATION__GPSLCE__SHIFT    23
#define GBE__MAC_CONFIGURATION__IPG__SHIFT    24
#define GBE__MAC_CONFIGURATION__IPC__SHIFT    27
#define GBE__MAC_CONFIGURATION__RESERVED_30_28__SHIFT    28
#define GBE__MAC_CONFIGURATION__ARPEN__SHIFT    31

#define GBE__MAC_CONFIGURATION__PRELEN__SIZE    2
#define GBE__MAC_CONFIGURATION__PORT_SPEED__SIZE    2
#define GBE__MAC_CONFIGURATION__IPG__SIZE    3

///////////////////////////////////////////////////////
// Register: MAC_Ext_Configuration
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_EXT_CONFIGURATION__OFFSET (0x4ULL)
#define GBE__MAC_EXT_CONFIGURATION__NUM  0x1

#define GBE__MAC_EXT_CONFIGURATION__GPSL__SHIFT    0
#define GBE__MAC_EXT_CONFIGURATION__RESERVED_15_14__SHIFT    14
#define GBE__MAC_EXT_CONFIGURATION__DCRCC__SHIFT    16
#define GBE__MAC_EXT_CONFIGURATION__SPEN__SHIFT    17
#define GBE__MAC_EXT_CONFIGURATION__USP__SHIFT    18
#define GBE__MAC_EXT_CONFIGURATION__RESERVED_23_19__SHIFT    19
#define GBE__MAC_EXT_CONFIGURATION__EIPGEN__SHIFT    24
#define GBE__MAC_EXT_CONFIGURATION__EIPG__SHIFT    25
#define GBE__MAC_EXT_CONFIGURATION__RESERVED_31_30__SHIFT    30


#define GBE__MAC_EXT_CONFIGURATION__GPSL__SIZE    14
#define GBE__MAC_EXT_CONFIGURATION__EIPG__SIZE    5

///////////////////////////////////////////////////////
// Register: MAC_Packet_Filter
// Receive All
///////////////////////////////////////////////////////

#define GBE__MAC_PACKET_FILTER__OFFSET (0x8ULL)
#define GBE__MAC_PACKET_FILTER__NUM  0x1

#define GBE__MAC_PACKET_FILTER__PR__SHIFT    0
#define GBE__MAC_PACKET_FILTER__HUC__SHIFT    1
#define GBE__MAC_PACKET_FILTER__HMC__SHIFT    2
#define GBE__MAC_PACKET_FILTER__DAIF__SHIFT    3
#define GBE__MAC_PACKET_FILTER__PM__SHIFT    4
#define GBE__MAC_PACKET_FILTER__DBF__SHIFT    5
#define GBE__MAC_PACKET_FILTER__PCF__SHIFT    6

#define GBE__MAC_PACKET_FILTER__SAIF__SHIFT    8
#define GBE__MAC_PACKET_FILTER__SAF__SHIFT    9
#define GBE__MAC_PACKET_FILTER__HPF__SHIFT    10
#define GBE__MAC_PACKET_FILTER__RESERVED_15_11__SHIFT    11
#define GBE__MAC_PACKET_FILTER__VTFE__SHIFT    16
#define GBE__MAC_PACKET_FILTER__RESERVED_30_17__SHIFT    17
#define GBE__MAC_PACKET_FILTER__RA__SHIFT    31

#define GBE__MAC_PACKET_FILTER__PCF__SIZE    2

///////////////////////////////////////////////////////
// Register: MAC_Watchdog_Timeout
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_WATCHDOG_TIMEOUT__OFFSET (0xCULL)
#define GBE__MAC_WATCHDOG_TIMEOUT__NUM  0x1

#define GBE__MAC_WATCHDOG_TIMEOUT__WTO__SHIFT    0
#define GBE__MAC_WATCHDOG_TIMEOUT__RESERVED_7_4__SHIFT    4
#define GBE__MAC_WATCHDOG_TIMEOUT__PWE__SHIFT    8
#define GBE__MAC_WATCHDOG_TIMEOUT__RESERVED_31_9__SHIFT    9

#define GBE__MAC_WATCHDOG_TIMEOUT__WTO__SIZE    4

///////////////////////////////////////////////////////
// Register: MAC_VLAN_Tag
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_VLAN_TAG__OFFSET (0x50ULL)
#define GBE__MAC_VLAN_TAG__NUM  0x1

#define GBE__MAC_VLAN_TAG__VL__SHIFT    0
#define GBE__MAC_VLAN_TAG__ETV__SHIFT    16
#define GBE__MAC_VLAN_TAG__VTIM__SHIFT    17
#define GBE__MAC_VLAN_TAG__ESVL__SHIFT    18
#define GBE__MAC_VLAN_TAG__ERSVLM__SHIFT    19
#define GBE__MAC_VLAN_TAG__DOVLTC__SHIFT    20
#define GBE__MAC_VLAN_TAG__EVLS__SHIFT    21
#define GBE__MAC_VLAN_TAG__RESERVED_23__SHIFT    23
#define GBE__MAC_VLAN_TAG__EVLRXS__SHIFT    24
#define GBE__MAC_VLAN_TAG__RESERVED_31_25__SHIFT    25

///////////////////////////////////////////////////////
// Register: MAC_Q0_Tx_Flow_Ctrl
// Pause Time
///////////////////////////////////////////////////////

#define GBE__MAC_Q0_TX_FLOW_CTRL__OFFSET (0x70ULL)
#define GBE__MAC_Q0_TX_FLOW_CTRL__NUM  0x1

#define GBE__MAC_Q0_TX_FLOW_CTRL__FCB_BPA__SHIFT    0
#define GBE__MAC_Q0_TX_FLOW_CTRL__TFE__SHIFT    1
#define GBE__MAC_Q0_TX_FLOW_CTRL__RESERVED_3_2__SHIFT    2
#define GBE__MAC_Q0_TX_FLOW_CTRL__PLT__SHIFT    4
#define GBE__MAC_Q0_TX_FLOW_CTRL__DZPQ__SHIFT    7
#define GBE__MAC_Q0_TX_FLOW_CTRL__RESERVED_15_8__SHIFT    8
#define GBE__MAC_Q0_TX_FLOW_CTRL__PT__SHIFT    16

#define GBE__MAC_Q0_TX_FLOW_CTRL__PLT__SIZE    3
#define GBE__MAC_Q0_TX_FLOW_CTRL__PT__SIZE    16

///////////////////////////////////////////////////////
// Register: MAC_Rx_Flow_Ctrl
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_RX_FLOW_CTRL__OFFSET (0x90ULL)
#define GBE__MAC_RX_FLOW_CTRL__NUM  0x1

#define GBE__MAC_RX_FLOW_CTRL__RFE__SHIFT    0
#define GBE__MAC_RX_FLOW_CTRL__UP__SHIFT    1
#define GBE__MAC_RX_FLOW_CTRL__RESERVED_31_2__SHIFT    2

///////////////////////////////////////////////////////
// Register: MAC_Interrupt_Status
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_INTERRUPT_STATUS__OFFSET (0xB0ULL)
#define GBE__MAC_INTERRUPT_STATUS__NUM  0x1

#define GBE__MAC_INTERRUPT_STATUS__RGSMIIIS__SHIFT    0
#define GBE__MAC_INTERRUPT_STATUS__RESERVED_2_1__SHIFT    1
#define GBE__MAC_INTERRUPT_STATUS__PHYIS__SHIFT    3
#define GBE__MAC_INTERRUPT_STATUS__RESERVED_7_4__SHIFT    4
#define GBE__MAC_INTERRUPT_STATUS__MMCIS__SHIFT    8
#define GBE__MAC_INTERRUPT_STATUS__MMCRXIS__SHIFT    9
#define GBE__MAC_INTERRUPT_STATUS__MMCTXIS__SHIFT    10
#define GBE__MAC_INTERRUPT_STATUS__RESERVED_11__SHIFT    11
#define GBE__MAC_INTERRUPT_STATUS__TSIS__SHIFT    12
#define GBE__MAC_INTERRUPT_STATUS__TXSTSIS__SHIFT    13
#define GBE__MAC_INTERRUPT_STATUS__RXSTSIS__SHIFT    14
#define GBE__MAC_INTERRUPT_STATUS__RESERVED_17_15__SHIFT    15
#define GBE__MAC_INTERRUPT_STATUS__MDIOIS__SHIFT    18
#define GBE__MAC_INTERRUPT_STATUS__RESERVED_31_19__SHIFT    19

///////////////////////////////////////////////////////
// Register: MAC_Interrupt_Enable
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_INTERRUPT_ENABLE__OFFSET (0xB4ULL)
#define GBE__MAC_INTERRUPT_ENABLE__NUM  0x1

#define GBE__MAC_INTERRUPT_ENABLE__RGSMIIIE__SHIFT    0
#define GBE__MAC_INTERRUPT_ENABLE__RESERVED_2_1__SHIFT    1
#define GBE__MAC_INTERRUPT_ENABLE__PHYIE__SHIFT    3
#define GBE__MAC_INTERRUPT_ENABLE__RESERVED_11_4__SHIFT    4
#define GBE__MAC_INTERRUPT_ENABLE__TSIE__SHIFT    12
#define GBE__MAC_INTERRUPT_ENABLE__TXSTSIE__SHIFT    13
#define GBE__MAC_INTERRUPT_ENABLE__RXSTSIE__SHIFT    14
#define GBE__MAC_INTERRUPT_ENABLE__RESERVED_17_15__SHIFT    15
#define GBE__MAC_INTERRUPT_ENABLE__MDIOIE__SHIFT    18
#define GBE__MAC_INTERRUPT_ENABLE__RESERVED_31_19__SHIFT    19

///////////////////////////////////////////////////////
// Register: MAC_Rx_Tx_Status
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_RX_TX_STATUS__OFFSET (0xB8ULL)
#define GBE__MAC_RX_TX_STATUS__NUM  0x1

#define GBE__MAC_RX_TX_STATUS__TJT__SHIFT    0
#define GBE__MAC_RX_TX_STATUS__RESERVED_7_1__SHIFT    1
#define GBE__MAC_RX_TX_STATUS__RWT__SHIFT    8
#define GBE__MAC_RX_TX_STATUS__RESERVED_31_9__SHIFT    9

///////////////////////////////////////////////////////
// Register: MAC_PHYIF_Control_Status
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_PHYIF_CONTROL_STATUS__OFFSET (0xF8ULL)
#define GBE__MAC_PHYIF_CONTROL_STATUS__NUM  0x1

#define GBE__MAC_PHYIF_CONTROL_STATUS__TC__SHIFT    0
#define GBE__MAC_PHYIF_CONTROL_STATUS__LUD__SHIFT    1
#define GBE__MAC_PHYIF_CONTROL_STATUS__RESERVED_15_2__SHIFT    2
#define GBE__MAC_PHYIF_CONTROL_STATUS__LNKMOD__SHIFT    16
#define GBE__MAC_PHYIF_CONTROL_STATUS__LNKSPEED__SHIFT    17
#define GBE__MAC_PHYIF_CONTROL_STATUS__LNKSTS__SHIFT    19
#define GBE__MAC_PHYIF_CONTROL_STATUS__RESERVED_31_20__SHIFT    20

///////////////////////////////////////////////////////
// Register: MAC_Version
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_VERSION__OFFSET (0x110ULL)
#define GBE__MAC_VERSION__NUM  0x1

#define GBE__MAC_VERSION__SNPSVER__SHIFT    0
#define GBE__MAC_VERSION__USERVER__SHIFT    8
#define GBE__MAC_VERSION__RESERVED_31_16__SHIFT    16

///////////////////////////////////////////////////////
// Register: MAC_Debug
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_DEBUG__OFFSET (0x114ULL)
#define GBE__MAC_DEBUG__NUM  0x1

#define GBE__MAC_DEBUG__RPESTS__SHIFT    0
#define GBE__MAC_DEBUG__RFCFCSTS__SHIFT    1
#define GBE__MAC_DEBUG__RESERVED_15_3__SHIFT    3
#define GBE__MAC_DEBUG__TPESTS__SHIFT    16
#define GBE__MAC_DEBUG__TFCSTS__SHIFT    17
#define GBE__MAC_DEBUG__RESERVED_31_19__SHIFT    19

///////////////////////////////////////////////////////
// Register: MAC_HW_Feature0
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_HW_FEATURE0__OFFSET (0x11CULL)
#define GBE__MAC_HW_FEATURE0__NUM  0x1

#define GBE__MAC_HW_FEATURE0__MIISEL__SHIFT    0
#define GBE__MAC_HW_FEATURE0__GMIISEL__SHIFT    1
#define GBE__MAC_HW_FEATURE0__HDSEL__SHIFT    2
#define GBE__MAC_HW_FEATURE0__PCSSEL__SHIFT    3
#define GBE__MAC_HW_FEATURE0__VLHASH__SHIFT    4
#define GBE__MAC_HW_FEATURE0__SMASEL__SHIFT    5
#define GBE__MAC_HW_FEATURE0__RWKSEL__SHIFT    6
#define GBE__MAC_HW_FEATURE0__MGKSEL__SHIFT    7
#define GBE__MAC_HW_FEATURE0__MMCSEL__SHIFT    8
#define GBE__MAC_HW_FEATURE0__ARPOFFSEL__SHIFT    9
#define GBE__MAC_HW_FEATURE0__RESERVED_11_10__SHIFT    10
#define GBE__MAC_HW_FEATURE0__TSSEL__SHIFT    12
#define GBE__MAC_HW_FEATURE0__EEESEL__SHIFT    13
#define GBE__MAC_HW_FEATURE0__TXCOESEL__SHIFT    14
#define GBE__MAC_HW_FEATURE0__RESERVED_15__SHIFT    15
#define GBE__MAC_HW_FEATURE0__RXCOESEL__SHIFT    16
#define GBE__MAC_HW_FEATURE0__RESERVED_17__SHIFT    17
#define GBE__MAC_HW_FEATURE0__ADDMACADRSEL__SHIFT    18
#define GBE__MAC_HW_FEATURE0__MACADR32SEL__SHIFT    23
#define GBE__MAC_HW_FEATURE0__MACADR64SEL__SHIFT    24
#define GBE__MAC_HW_FEATURE0__TSSTSSEL__SHIFT    25
#define GBE__MAC_HW_FEATURE0__SAVLANINS__SHIFT    27
#define GBE__MAC_HW_FEATURE0__ACTPHYSEL__SHIFT    28
#define GBE__MAC_HW_FEATURE0__RESERVED_31__SHIFT    31

///////////////////////////////////////////////////////
// Register: MAC_HW_Feature1
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_HW_FEATURE1__OFFSET (0x120ULL)
#define GBE__MAC_HW_FEATURE1__NUM  0x1

#define GBE__MAC_HW_FEATURE1__RXFIFOSIZE__SHIFT    0
#define GBE__MAC_HW_FEATURE1__SPRAM__SHIFT    5
#define GBE__MAC_HW_FEATURE1__TXFIFOSIZE__SHIFT    6
#define GBE__MAC_HW_FEATURE1__OSTEN__SHIFT    11
#define GBE__MAC_HW_FEATURE1__PTOEN__SHIFT    12
#define GBE__MAC_HW_FEATURE1__ADVTHWORD__SHIFT    13
#define GBE__MAC_HW_FEATURE1__OFFSET64__SHIFT    14
#define GBE__MAC_HW_FEATURE1__DCBEN__SHIFT    16
#define GBE__MAC_HW_FEATURE1__SPHEN__SHIFT    17
#define GBE__MAC_HW_FEATURE1__TSOEN__SHIFT    18
#define GBE__MAC_HW_FEATURE1__DBGMEMA__SHIFT    19
#define GBE__MAC_HW_FEATURE1__AVSEL__SHIFT    20
#define GBE__MAC_HW_FEATURE1__RAVSEL__SHIFT    21
#define GBE__MAC_HW_FEATURE1__RESERVED_22__SHIFT    22
#define GBE__MAC_HW_FEATURE1__POUOST__SHIFT    23
#define GBE__MAC_HW_FEATURE1__HASHTBLSZ__SHIFT    24
#define GBE__MAC_HW_FEATURE1__RESERVED_26__SHIFT    26
#define GBE__MAC_HW_FEATURE1__L3L4FNUM__SHIFT    27
#define GBE__MAC_HW_FEATURE1__RESERVED_31__SHIFT    31

///////////////////////////////////////////////////////
// Register: MAC_HW_Feature2
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_HW_FEATURE2__OFFSET (0x124ULL)
#define GBE__MAC_HW_FEATURE2__NUM  0x1

#define GBE__MAC_HW_FEATURE2__RXQCNT__SHIFT    0
#define GBE__MAC_HW_FEATURE2__RESERVED_5_4__SHIFT    4
#define GBE__MAC_HW_FEATURE2__TXQCNT__SHIFT    6
#define GBE__MAC_HW_FEATURE2__RESERVED_11_10__SHIFT    10
#define GBE__MAC_HW_FEATURE2__RXCHCNT__SHIFT    12
#define GBE__MAC_HW_FEATURE2__RESERVED_17_16__SHIFT    16
#define GBE__MAC_HW_FEATURE2__TXCHCNT__SHIFT    18
#define GBE__MAC_HW_FEATURE2__RESERVED_23_22__SHIFT    22
#define GBE__MAC_HW_FEATURE2__PPSOUTNUM__SHIFT    24
#define GBE__MAC_HW_FEATURE2__RESERVED_27__SHIFT    27
#define GBE__MAC_HW_FEATURE2__AUXSNAPNUM__SHIFT    28
#define GBE__MAC_HW_FEATURE2__RESERVED_31__SHIFT    31

///////////////////////////////////////////////////////
// Register: MAC_HW_Feature3
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_HW_FEATURE3__OFFSET (0x128ULL)
#define GBE__MAC_HW_FEATURE3__NUM  0x1

#define GBE__MAC_HW_FEATURE3__NRVF__SHIFT    0
#define GBE__MAC_HW_FEATURE3__RESERVED_3__SHIFT    3
#define GBE__MAC_HW_FEATURE3__CBTISEL__SHIFT    4
#define GBE__MAC_HW_FEATURE3__DVLAN__SHIFT    5
#define GBE__MAC_HW_FEATURE3__RESERVED_8_6__SHIFT    6
#define GBE__MAC_HW_FEATURE3__PDUPSEL__SHIFT    9
#define GBE__MAC_HW_FEATURE3__FRPSEL__SHIFT    10
#define GBE__MAC_HW_FEATURE3__FRPBS__SHIFT    11
#define GBE__MAC_HW_FEATURE3__FRPES__SHIFT    13
#define GBE__MAC_HW_FEATURE3__RESERVED_15__SHIFT    15
#define GBE__MAC_HW_FEATURE3__ESTSEL__SHIFT    16
#define GBE__MAC_HW_FEATURE3__ESTDEP__SHIFT    17
#define GBE__MAC_HW_FEATURE3__ESTWID__SHIFT    20
#define GBE__MAC_HW_FEATURE3__RESERVED_25_22__SHIFT    22
#define GBE__MAC_HW_FEATURE3__FPESEL__SHIFT    26
#define GBE__MAC_HW_FEATURE3__TBSSEL__SHIFT    27
#define GBE__MAC_HW_FEATURE3__ASP__SHIFT    28
#define GBE__MAC_HW_FEATURE3__RESERVED_31_30__SHIFT    30

///////////////////////////////////////////////////////
// Register: MAC_MDIO_OFFSETess
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_MDIO_ADDRESS__OFFSET (0x200ULL)
#define GBE__MAC_MDIO_ADDRESS__NUM  0x1

#define GBE__MAC_MDIO_ADDRESS__GB__SHIFT    0
#define GBE__MAC_MDIO_ADDRESS__C45E__SHIFT    1
#define GBE__MAC_MDIO_ADDRESS__GOC_0__SHIFT    2
#define GBE__MAC_MDIO_ADDRESS__GOC_1__SHIFT    3
#define GBE__MAC_MDIO_ADDRESS__SKAP__SHIFT    4
#define GBE__MAC_MDIO_ADDRESS__RESERVED_7_5__SHIFT    5
#define GBE__MAC_MDIO_ADDRESS__CR__SHIFT    8
#define GBE__MAC_MDIO_ADDRESS__NTC__SHIFT    12
#define GBE__MAC_MDIO_ADDRESS__RESERVED_15__SHIFT    15
#define GBE__MAC_MDIO_ADDRESS__RDA__SHIFT    16
#define GBE__MAC_MDIO_ADDRESS__PA__SHIFT    21
#define GBE__MAC_MDIO_ADDRESS__BTB__SHIFT    26
#define GBE__MAC_MDIO_ADDRESS__PSE__SHIFT    27
#define GBE__MAC_MDIO_ADDRESS__RESERVED_31_28__SHIFT    28

#define GBE__MAC_MDIO_ADDRESS__GOC__SIZE    2
#define GBE__MAC_MDIO_ADDRESS__CR__SIZE    4
#define GBE__MAC_MDIO_ADDRESS__RDA__SIZE    5
#define GBE__MAC_MDIO_ADDRESS__PA__SIZE    5

///////////////////////////////////////////////////////
// Register: MAC_MDIO_Data
// Register OFFSETess
///////////////////////////////////////////////////////

#define GBE__MAC_MDIO_DATA__OFFSET (0x204ULL)
#define GBE__MAC_MDIO_DATA__NUM  0x1

#define GBE__MAC_MDIO_DATA__GD__SHIFT    0
#define GBE__MAC_MDIO_DATA__RA__SHIFT    16

#define GBE__MAC_MDIO_DATA__GD__SIZE    16

///////////////////////////////////////////////////////
// Register: MAC_ARP_OFFSETess
//  
// ARP Protocol OFFSETess
///////////////////////////////////////////////////////

#define GBE__MAC_ARP_OFFSETESS__OFFSET (0x210ULL)
#define GBE__MAC_ARP_OFFSETESS__NUM  0x1

#define GBE__MAC_ARP_OFFSETESS__ARPPA__SHIFT    0

///////////////////////////////////////////////////////
// Register: MAC_CSR_SW_Ctrl
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_CSR_SW_CTRL__OFFSET (0x230ULL)
#define GBE__MAC_CSR_SW_CTRL__NUM  0x1

#define GBE__MAC_CSR_SW_CTRL__RCWE__SHIFT    0
#define GBE__MAC_CSR_SW_CTRL__RESERVED_7_1__SHIFT    1
#define GBE__MAC_CSR_SW_CTRL__SEEN__SHIFT    8
#define GBE__MAC_CSR_SW_CTRL__RESERVED_31_9__SHIFT    9

///////////////////////////////////////////////////////
// Register: MAC_Address0_High
// Address Enable
///////////////////////////////////////////////////////

#define GBE_MAC_ADDRESS0_HIGH__OFFSET (0x300ULL)
#define GBE_MAC_ADDRESS0_HIGH__NUM  0x1

#define GBE_MAC_ADDRESS0_HIGH__ADDRHI__SHIFT    0
#define GBE_MAC_ADDRESS0_HIGH__RESERVED_30_16__SHIFT    16
#define GBE_MAC_ADDRESS0_HIGH__AE__SHIFT    31

#define GBE_MAC_ADDRESS0_HIGH__ADDRHI__SIZE    16

///////////////////////////////////////////////////////
// Register: MAC_Address0_Low
// MAC Address0[31:0]
///////////////////////////////////////////////////////

#define GBE_MAC_ADDRESS0_LOW__OFFSET (0x304ULL)
#define GBE_MAC_ADDRESS0_LOW__NUM  0x1

#define GBE_MAC_ADDRESS0_LOW__ADDRLO__SHIFT    0


///////////////////////////////////////////////////////
// Register: MMC_Control
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MMC_CONTROL__OFFSET (0x700ULL)
#define GBE__MMC_CONTROL__NUM  0x1

#define GBE__MMC_CONTROL__CNTRST__SHIFT    0
#define GBE__MMC_CONTROL__CNTSTOPRO__SHIFT    1
#define GBE__MMC_CONTROL__RSTONRD__SHIFT    2
#define GBE__MMC_CONTROL__CNTFREEZ__SHIFT    3
#define GBE__MMC_CONTROL__CNTPRST__SHIFT    4
#define GBE__MMC_CONTROL__CNTPRSTLVL__SHIFT    5
#define GBE__MMC_CONTROL__RESERVED_7_6__SHIFT    6
#define GBE__MMC_CONTROL__UCDBC__SHIFT    8
#define GBE__MMC_CONTROL__RESERVED_31_9__SHIFT    9

///////////////////////////////////////////////////////
// Register: MMC_Rx_Interrupt
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MMC_RX_INTERRUPT__OFFSET (0x704ULL)
#define GBE__MMC_RX_INTERRUPT__NUM  0x1

#define GBE__MMC_RX_INTERRUPT__RXGBPKTIS__SHIFT    0
#define GBE__MMC_RX_INTERRUPT__RXGBOCTIS__SHIFT    1
#define GBE__MMC_RX_INTERRUPT__RXGOCTIS__SHIFT    2
#define GBE__MMC_RX_INTERRUPT__RXBCGPIS__SHIFT    3
#define GBE__MMC_RX_INTERRUPT__RXMCGPIS__SHIFT    4
#define GBE__MMC_RX_INTERRUPT__RXCRCERPIS__SHIFT    5
#define GBE__MMC_RX_INTERRUPT__RXALGNERPIS__SHIFT    6
#define GBE__MMC_RX_INTERRUPT__RXRUNTPIS__SHIFT    7
#define GBE__MMC_RX_INTERRUPT__RXJABERPIS__SHIFT    8
#define GBE__MMC_RX_INTERRUPT__RXUSIZEGPIS__SHIFT    9
#define GBE__MMC_RX_INTERRUPT__RXOSIZEGPIS__SHIFT    10
#define GBE__MMC_RX_INTERRUPT__RX64OCTGBPIS__SHIFT    11
#define GBE__MMC_RX_INTERRUPT__RX65T127OCTGBPIS__SHIFT    12
#define GBE__MMC_RX_INTERRUPT__RX128T255OCTGBPIS__SHIFT    13
#define GBE__MMC_RX_INTERRUPT__RX256T511OCTGBPIS__SHIFT    14
#define GBE__MMC_RX_INTERRUPT__RX512T1023OCTGBPIS__SHIFT    15
#define GBE__MMC_RX_INTERRUPT__RX1024TMAXOCTGBPIS__SHIFT    16
#define GBE__MMC_RX_INTERRUPT__RXUCGPIS__SHIFT    17
#define GBE__MMC_RX_INTERRUPT__RXLENERPIS__SHIFT    18
#define GBE__MMC_RX_INTERRUPT__RXORANGEPIS__SHIFT    19
#define GBE__MMC_RX_INTERRUPT__RXPAUSPIS__SHIFT    20
#define GBE__MMC_RX_INTERRUPT__RXFOVPIS__SHIFT    21
#define GBE__MMC_RX_INTERRUPT__RXVLANGBPIS__SHIFT    22
#define GBE__MMC_RX_INTERRUPT__RXWDOGPIS__SHIFT    23
#define GBE__MMC_RX_INTERRUPT__RXRCVERRPIS__SHIFT    24
#define GBE__MMC_RX_INTERRUPT__RXCTRLPIS__SHIFT    25
#define GBE__MMC_RX_INTERRUPT__RESERVED_31_26__SHIFT    26

///////////////////////////////////////////////////////
// Register: MMC_Tx_Interrupt
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MMC_TX_INTERRUPT__OFFSET (0x708ULL)
#define GBE__MMC_TX_INTERRUPT__NUM  0x1

#define GBE__MMC_TX_INTERRUPT__TXGBOCTIS__SHIFT    0
#define GBE__MMC_TX_INTERRUPT__TXGBPKTIS__SHIFT    1
#define GBE__MMC_TX_INTERRUPT__TXBCGPIS__SHIFT    2
#define GBE__MMC_TX_INTERRUPT__TXMCGPIS__SHIFT    3
#define GBE__MMC_TX_INTERRUPT__TX64OCTGBPIS__SHIFT    4
#define GBE__MMC_TX_INTERRUPT__TX65T127OCTGBPIS__SHIFT    5
#define GBE__MMC_TX_INTERRUPT__TX128T255OCTGBPIS__SHIFT    6
#define GBE__MMC_TX_INTERRUPT__TX256T511OCTGBPIS__SHIFT    7
#define GBE__MMC_TX_INTERRUPT__TX512T1023OCTGBPIS__SHIFT    8
#define GBE__MMC_TX_INTERRUPT__TX1024TMAXOCTGBPIS__SHIFT    9
#define GBE__MMC_TX_INTERRUPT__TXUCGBPIS__SHIFT    10
#define GBE__MMC_TX_INTERRUPT__TXMCGBPIS__SHIFT    11
#define GBE__MMC_TX_INTERRUPT__TXBCGBPIS__SHIFT    12
#define GBE__MMC_TX_INTERRUPT__TXUFLOWERPIS__SHIFT    13
#define GBE__MMC_TX_INTERRUPT__RESERVED_19_14__SHIFT    14
#define GBE__MMC_TX_INTERRUPT__TXGOCTIS__SHIFT    20
#define GBE__MMC_TX_INTERRUPT__TXGPKTIS__SHIFT    21
#define GBE__MMC_TX_INTERRUPT__RESERVED_22__SHIFT    22
#define GBE__MMC_TX_INTERRUPT__TXPAUSPIS__SHIFT    23
#define GBE__MMC_TX_INTERRUPT__TXVLANGPIS__SHIFT    24
#define GBE__MMC_TX_INTERRUPT__TXOSIZEGPIS__SHIFT    25
#define GBE__MMC_TX_INTERRUPT__RESERVED_31_26__SHIFT    26

///////////////////////////////////////////////////////
// Register: MMC_Rx_Interrupt_Mask
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MMC_RX_INTERRUPT_MASK__OFFSET (0x70CULL)
#define GBE__MMC_RX_INTERRUPT_MASK__NUM  0x1

#define GBE__MMC_RX_INTERRUPT_MASK__RXGBPKTIM__SHIFT    0
#define GBE__MMC_RX_INTERRUPT_MASK__RXGBOCTIM__SHIFT    1
#define GBE__MMC_RX_INTERRUPT_MASK__RXGOCTIM__SHIFT    2
#define GBE__MMC_RX_INTERRUPT_MASK__RXBCGPIM__SHIFT    3
#define GBE__MMC_RX_INTERRUPT_MASK__RXMCGPIM__SHIFT    4
#define GBE__MMC_RX_INTERRUPT_MASK__RXCRCERPIM__SHIFT    5
#define GBE__MMC_RX_INTERRUPT_MASK__RXALGNERPIM__SHIFT    6
#define GBE__MMC_RX_INTERRUPT_MASK__RXRUNTPIM__SHIFT    7
#define GBE__MMC_RX_INTERRUPT_MASK__RXJABERPIM__SHIFT    8
#define GBE__MMC_RX_INTERRUPT_MASK__RXUSIZEGPIM__SHIFT    9
#define GBE__MMC_RX_INTERRUPT_MASK__RXOSIZEGPIM__SHIFT    10
#define GBE__MMC_RX_INTERRUPT_MASK__RX64OCTGBPIM__SHIFT    11
#define GBE__MMC_RX_INTERRUPT_MASK__RX65T127OCTGBPIM__SHIFT    12
#define GBE__MMC_RX_INTERRUPT_MASK__RX128T255OCTGBPIM__SHIFT    13
#define GBE__MMC_RX_INTERRUPT_MASK__RX256T511OCTGBPIM__SHIFT    14
#define GBE__MMC_RX_INTERRUPT_MASK__RX512T1023OCTGBPIM__SHIFT    15
#define GBE__MMC_RX_INTERRUPT_MASK__RX1024TMAXOCTGBPIM__SHIFT    16
#define GBE__MMC_RX_INTERRUPT_MASK__RXUCGPIM__SHIFT    17
#define GBE__MMC_RX_INTERRUPT_MASK__RXLENERPIM__SHIFT    18
#define GBE__MMC_RX_INTERRUPT_MASK__RXORANGEPIM__SHIFT    19
#define GBE__MMC_RX_INTERRUPT_MASK__RXPAUSPIM__SHIFT    20
#define GBE__MMC_RX_INTERRUPT_MASK__RXFOVPIM__SHIFT    21
#define GBE__MMC_RX_INTERRUPT_MASK__RXVLANGBPIM__SHIFT    22
#define GBE__MMC_RX_INTERRUPT_MASK__RXWDOGPIM__SHIFT    23
#define GBE__MMC_RX_INTERRUPT_MASK__RXRCVERRPIM__SHIFT    24
#define GBE__MMC_RX_INTERRUPT_MASK__RXCTRLPIM__SHIFT    25
#define GBE__MMC_RX_INTERRUPT_MASK__RESERVED_31_26__SHIFT    26

///////////////////////////////////////////////////////
// Register: MMC_Tx_Interrupt_Mask
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MMC_TX_INTERRUPT_MASK__OFFSET (0x710ULL)
#define GBE__MMC_TX_INTERRUPT_MASK__NUM  0x1

#define GBE__MMC_TX_INTERRUPT_MASK__TXGBOCTIM__SHIFT    0
#define GBE__MMC_TX_INTERRUPT_MASK__TXGBPKTIM__SHIFT    1
#define GBE__MMC_TX_INTERRUPT_MASK__TXBCGPIM__SHIFT    2
#define GBE__MMC_TX_INTERRUPT_MASK__TXMCGPIM__SHIFT    3
#define GBE__MMC_TX_INTERRUPT_MASK__TX64OCTGBPIM__SHIFT    4
#define GBE__MMC_TX_INTERRUPT_MASK__TX65T127OCTGBPIM__SHIFT    5
#define GBE__MMC_TX_INTERRUPT_MASK__TX128T255OCTGBPIM__SHIFT    6
#define GBE__MMC_TX_INTERRUPT_MASK__TX256T511OCTGBPIM__SHIFT    7
#define GBE__MMC_TX_INTERRUPT_MASK__TX512T1023OCTGBPIM__SHIFT    8
#define GBE__MMC_TX_INTERRUPT_MASK__TX1024TMAXOCTGBPIM__SHIFT    9
#define GBE__MMC_TX_INTERRUPT_MASK__TXUCGBPIM__SHIFT    10
#define GBE__MMC_TX_INTERRUPT_MASK__TXMCGBPIM__SHIFT    11
#define GBE__MMC_TX_INTERRUPT_MASK__TXBCGBPIM__SHIFT    12
#define GBE__MMC_TX_INTERRUPT_MASK__TXUFLOWERPIM__SHIFT    13
#define GBE__MMC_TX_INTERRUPT_MASK__RESERVED_19_14__SHIFT    14
#define GBE__MMC_TX_INTERRUPT_MASK__TXGOCTIM__SHIFT    20
#define GBE__MMC_TX_INTERRUPT_MASK__TXGPKTIM__SHIFT    21
#define GBE__MMC_TX_INTERRUPT_MASK__RESERVED_22__SHIFT    22
#define GBE__MMC_TX_INTERRUPT_MASK__TXPAUSPIM__SHIFT    23
#define GBE__MMC_TX_INTERRUPT_MASK__TXVLANGPIM__SHIFT    24
#define GBE__MMC_TX_INTERRUPT_MASK__TXOSIZEGPIM__SHIFT    25
#define GBE__MMC_TX_INTERRUPT_MASK__RESERVED_31_26__SHIFT    26

///////////////////////////////////////////////////////
// Register: Tx_Octet_Count_Good_Bad
// Tx Octet Count Good Bad
///////////////////////////////////////////////////////

#define GBE__TX_OCTET_COUNT_GOOD_BAD__OFFSET (0x714ULL)
#define GBE__TX_OCTET_COUNT_GOOD_BAD__NUM  0x1

#define GBE__TX_OCTET_COUNT_GOOD_BAD__TXOCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_Packet_Count_Good_Bad
// Tx Packet Count Good Bad
///////////////////////////////////////////////////////

#define GBE__TX_PACKET_COUNT_GOOD_BAD__OFFSET (0x718ULL)
#define GBE__TX_PACKET_COUNT_GOOD_BAD__NUM  0x1

#define GBE__TX_PACKET_COUNT_GOOD_BAD__TXPKTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_Broadcast_Packets_Good
// Tx Broadcast Packets Good
///////////////////////////////////////////////////////

#define GBE__TX_BROADCAST_PACKETS_GOOD__OFFSET (0x71CULL)
#define GBE__TX_BROADCAST_PACKETS_GOOD__NUM  0x1

#define GBE__TX_BROADCAST_PACKETS_GOOD__TXBCASTG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_Multicast_Packets_Good
// Tx Multicast Packets Good
///////////////////////////////////////////////////////

#define GBE__TX_MULTICAST_PACKETS_GOOD__OFFSET (0x720ULL)
#define GBE__TX_MULTICAST_PACKETS_GOOD__NUM  0x1

#define GBE__TX_MULTICAST_PACKETS_GOOD__TXMCASTG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_64Octets_Packets_Good_Bad
// Tx 64Octets Packets Good_Bad
///////////////////////////////////////////////////////

#define GBE__TX_64OCTETS_PACKETS_GOOD_BAD__OFFSET (0x724ULL)
#define GBE__TX_64OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__TX_64OCTETS_PACKETS_GOOD_BAD__TX64OCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_65To127Octets_Packets_Good_Bad
// Tx 65To127Octets Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__TX_65TO127OCTETS_PACKETS_GOOD_BAD__OFFSET (0x728ULL)
#define GBE__TX_65TO127OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__TX_65TO127OCTETS_PACKETS_GOOD_BAD__TX65_127OCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_128To255Octets_Packets_Good_Bad
// Tx 128To255Octets Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__TX_128TO255OCTETS_PACKETS_GOOD_BAD__OFFSET (0x72CULL)
#define GBE__TX_128TO255OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__TX_128TO255OCTETS_PACKETS_GOOD_BAD__TX128_255OCTGB__SHIFT    0


///////////////////////////////////////////////////////
// Register: Tx_256To511Octets_Packets_Good_Bad
// Tx 256To511Octets Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__TX_256TO511OCTETS_PACKETS_GOOD_BAD__OFFSET (0x730ULL)
#define GBE__TX_256TO511OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__TX_256TO511OCTETS_PACKETS_GOOD_BAD__TX256_511OCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_512To1023Octets_Packets_Good_Bad
//  
// Tx 512To1023Octets Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__TX_512TO1023OCTETS_PACKETS_GOOD_BAD__OFFSET (0x734ULL)
#define GBE__TX_512TO1023OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__TX_512TO1023OCTETS_PACKETS_GOOD_BAD__TX512_1023OCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_1024ToMaxOctets_Packets_Good_Bad
// Tx 1024ToMaxOctets Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__TX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__OFFSET (0x738ULL)
#define GBE__TX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__TX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__TX1024_MAXOCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_Unicast_Packets_Good_Bad
//  
// Tx Unicast Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__TX_UNICAST_PACKETS_GOOD_BAD__OFFSET (0x73CULL)
#define GBE__TX_UNICAST_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__TX_UNICAST_PACKETS_GOOD_BAD__TXUCASTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_Multicast_Packets_Good_Bad
//  
// Tx Multicast Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__TX_MULTICAST_PACKETS_GOOD_BAD__OFFSET (0x740ULL)
#define GBE__TX_MULTICAST_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__TX_MULTICAST_PACKETS_GOOD_BAD__TXMCASTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_Broadcast_Packets_Good_Bad
// Tx Broadcast Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__TX_BROADCAST_PACKETS_GOOD_BAD__OFFSET (0x744ULL)
#define GBE__TX_BROADCAST_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__TX_BROADCAST_PACKETS_GOOD_BAD__TXBCASTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_Underflow_Error_Packets
// Tx Underflow Error Packets
///////////////////////////////////////////////////////

#define GBE__TX_UNDERFLOW_ERROR_PACKETS__OFFSET (0x748ULL)
#define GBE__TX_UNDERFLOW_ERROR_PACKETS__NUM  0x1

#define GBE__TX_UNDERFLOW_ERROR_PACKETS__TXUNDRFLW__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_Octet_Count_Good
// Tx Octet Count Good
///////////////////////////////////////////////////////

#define GBE__TX_OCTET_COUNT_GOOD__OFFSET (0x764ULL)
#define GBE__TX_OCTET_COUNT_GOOD__NUM  0x1

#define GBE__TX_OCTET_COUNT_GOOD__TXOCTG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_Packet_Count_Good
//  
// Tx Packet Count Good
///////////////////////////////////////////////////////

#define GBE__TX_PACKET_COUNT_GOOD__OFFSET (0x768ULL)
#define GBE__TX_PACKET_COUNT_GOOD__NUM  0x1

#define GBE__TX_PACKET_COUNT_GOOD__TXPKTG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_Pause_Packets
// Tx Pause Packets
///////////////////////////////////////////////////////

#define GBE__TX_PAUSE_PACKETS__OFFSET (0x770ULL)
#define GBE__TX_PAUSE_PACKETS__NUM  0x1

#define GBE__TX_PAUSE_PACKETS__TXPAUSE__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_VLAN_Packets_Good
// Tx VLAN Packets Good
///////////////////////////////////////////////////////

#define GBE__TX_VLAN_PACKETS_GOOD__OFFSET (0x774ULL)
#define GBE__TX_VLAN_PACKETS_GOOD__NUM  0x1

#define GBE__TX_VLAN_PACKETS_GOOD__TXVLANG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Tx_OSize_Packets_Good
// Tx OSize Packets Good
///////////////////////////////////////////////////////

#define GBE__TX_OSIZE_PACKETS_GOOD__OFFSET (0x778ULL)
#define GBE__TX_OSIZE_PACKETS_GOOD__NUM  0x1

#define GBE__TX_OSIZE_PACKETS_GOOD__TXOSIZG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Packets_Count_Good_Bad
// Rx Packets Count Good Bad
///////////////////////////////////////////////////////

#define GBE__RX_PACKETS_COUNT_GOOD_BAD__OFFSET (0x780ULL)
#define GBE__RX_PACKETS_COUNT_GOOD_BAD__NUM  0x1

#define GBE__RX_PACKETS_COUNT_GOOD_BAD__RXPKTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Octet_Count_Good_Bad
//  
// Rx Octet Count Good Bad
///////////////////////////////////////////////////////

#define GBE__RX_OCTET_COUNT_GOOD_BAD__OFFSET (0x784ULL)
#define GBE__RX_OCTET_COUNT_GOOD_BAD__NUM  0x1

#define GBE__RX_OCTET_COUNT_GOOD_BAD__RXOCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Octet_Count_Good
// Rx Octet Count Good
///////////////////////////////////////////////////////

#define GBE__RX_OCTET_COUNT_GOOD__OFFSET (0x788ULL)
#define GBE__RX_OCTET_COUNT_GOOD__NUM  0x1

#define GBE__RX_OCTET_COUNT_GOOD__RXOCTG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Broadcast_Packets_Good
// Rx Broadcast Packets Good
///////////////////////////////////////////////////////

#define GBE__RX_BROADCAST_PACKETS_GOOD__OFFSET (0x78CULL)
#define GBE__RX_BROADCAST_PACKETS_GOOD__NUM  0x1

#define GBE__RX_BROADCAST_PACKETS_GOOD__RXBCASTG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Multicast_Packets_Good
//  
// Rx Multicast Packets Good
///////////////////////////////////////////////////////

#define GBE__RX_MULTICAST_PACKETS_GOOD__OFFSET (0x790ULL)
#define GBE__RX_MULTICAST_PACKETS_GOOD__NUM  0x1

#define GBE__RX_MULTICAST_PACKETS_GOOD__RXMCASTG__SHIFT    0

#define GBE__RX_MULTICAST_PACKETS_GOOD__RXMCASTG__MASK    0xffffffff

#define GBE__RX_MULTICAST_PACKETS_GOOD__RXMCASTG__POR_VALUE    0x0

///////////////////////////////////////////////////////
// Register: Rx_CRC_Error_Packets
// Rx CRC Error Packets
///////////////////////////////////////////////////////

#define GBE__RX_CRC_ERROR_PACKETS__OFFSET (0x794ULL)
#define GBE__RX_CRC_ERROR_PACKETS__NUM  0x1

#define GBE__RX_CRC_ERROR_PACKETS__RXCRCERR__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Alignment_Error_Packets
// Rx Alignment Error Packets
///////////////////////////////////////////////////////

#define GBE__RX_ALIGNMENT_ERROR_PACKETS__OFFSET (0x798ULL)
#define GBE__RX_ALIGNMENT_ERROR_PACKETS__NUM  0x1

#define GBE__RX_ALIGNMENT_ERROR_PACKETS__RXALGNERR__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Runt_Error_Packets
// Rx Runt Error Packets
///////////////////////////////////////////////////////

#define GBE__RX_RUNT_ERROR_PACKETS__OFFSET (0x79CULL)
#define GBE__RX_RUNT_ERROR_PACKETS__NUM  0x1

#define GBE__RX_RUNT_ERROR_PACKETS__RXRUNTERR__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Jabber_Error_Packets
// Rx Jabber Error Packets
///////////////////////////////////////////////////////

#define GBE__RX_JABBER_ERROR_PACKETS__OFFSET (0x7A0ULL)
#define GBE__RX_JABBER_ERROR_PACKETS__NUM  0x1

#define GBE__RX_JABBER_ERROR_PACKETS__RXJABERR__SHIFT    0


///////////////////////////////////////////////////////
// Register: Rx_Undersize_Packets_Good
// Rx Undersize Packets Good
///////////////////////////////////////////////////////

#define GBE__RX_UNDERSIZE_PACKETS_GOOD__OFFSET (0x7A4ULL)
#define GBE__RX_UNDERSIZE_PACKETS_GOOD__NUM  0x1

#define GBE__RX_UNDERSIZE_PACKETS_GOOD__RXUNDERSZG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Oversize_Packets_Good
// Rx Oversize Packets Good
///////////////////////////////////////////////////////

#define GBE__RX_OVERSIZE_PACKETS_GOOD__OFFSET (0x7A8ULL)
#define GBE__RX_OVERSIZE_PACKETS_GOOD__NUM  0x1

#define GBE__RX_OVERSIZE_PACKETS_GOOD__RXOVERSZG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_64Octets_Packets_Good_Bad
// Rx 64 Octets Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__RX_64OCTETS_PACKETS_GOOD_BAD__OFFSET (0x7ACULL)
#define GBE__RX_64OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__RX_64OCTETS_PACKETS_GOOD_BAD__RX64OCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_65To127Octets_Packets_Good_Bad
//  
// Rx 65-127 Octets Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__RX_65TO127OCTETS_PACKETS_GOOD_BAD__OFFSET (0x7B0ULL)
#define GBE__RX_65TO127OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__RX_65TO127OCTETS_PACKETS_GOOD_BAD__RX65_127OCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_128To255Octets_Packets_Good_Bad
//  
// Rx 128-255 Octets Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__RX_128TO255OCTETS_PACKETS_GOOD_BAD__OFFSET (0x7B4ULL)
#define GBE__RX_128TO255OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__RX_128TO255OCTETS_PACKETS_GOOD_BAD__RX128_255OCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_256To511Octets_Packets_Good_Bad
// Rx 256-511 Octets Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__RX_256TO511OCTETS_PACKETS_GOOD_BAD__OFFSET (0x7B8ULL)
#define GBE__RX_256TO511OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__RX_256TO511OCTETS_PACKETS_GOOD_BAD__RX256_511OCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_512To1023Octets_Packets_Good_Bad
// RX 512-1023 Octets Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__RX_512TO1023OCTETS_PACKETS_GOOD_BAD__OFFSET (0x7BCULL)
#define GBE__RX_512TO1023OCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__RX_512TO1023OCTETS_PACKETS_GOOD_BAD__RX512_1023OCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_1024ToMaxOctets_Packets_Good_Bad
// Rx 1024-Max Octets Good Bad
///////////////////////////////////////////////////////

#define GBE__RX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__OFFSET (0x7C0ULL)
#define GBE__RX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__RX_1024TOMAXOCTETS_PACKETS_GOOD_BAD__RX1024_MAXOCTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Unicast_Packets_Good
// Rx Unicast Packets Good
///////////////////////////////////////////////////////

#define GBE__RX_UNICAST_PACKETS_GOOD__OFFSET (0x7C4ULL)
#define GBE__RX_UNICAST_PACKETS_GOOD__NUM  0x1

#define GBE__RX_UNICAST_PACKETS_GOOD__RXUCASTG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Length_Error_Packets
// Rx Length Error Packets
///////////////////////////////////////////////////////

#define GBE__RX_LENGTH_ERROR_PACKETS__OFFSET (0x7C8ULL)
#define GBE__RX_LENGTH_ERROR_PACKETS__NUM  0x1

#define GBE__RX_LENGTH_ERROR_PACKETS__RXLENERR__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Out_Of_Range_Type_Packets
// Rx Out of Range Type Packet
///////////////////////////////////////////////////////

#define GBE__RX_OUT_OF_RANGE_TYPE_PACKETS__OFFSET (0x7CCULL)
#define GBE__RX_OUT_OF_RANGE_TYPE_PACKETS__NUM  0x1

#define GBE__RX_OUT_OF_RANGE_TYPE_PACKETS__RXOUTOFRNG__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Pause_Packets
// Rx Pause Packets
///////////////////////////////////////////////////////

#define GBE__RX_PAUSE_PACKETS__OFFSET (0x7D0ULL)
#define GBE__RX_PAUSE_PACKETS__NUM  0x1

#define GBE__RX_PAUSE_PACKETS__RXPAUSEPKT__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_FIFO_Overflow_Packets
// Rx FIFO Overflow Packets
///////////////////////////////////////////////////////

#define GBE__RX_FIFO_OVERFLOW_PACKETS__OFFSET (0x7D4ULL)
#define GBE__RX_FIFO_OVERFLOW_PACKETS__NUM  0x1

#define GBE__RX_FIFO_OVERFLOW_PACKETS__RXFIFOOVFL__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_VLAN_Packets_Good_Bad
//  
// Rx VLAN Packets Good Bad
///////////////////////////////////////////////////////

#define GBE__RX_VLAN_PACKETS_GOOD_BAD__OFFSET (0x7D8ULL)
#define GBE__RX_VLAN_PACKETS_GOOD_BAD__NUM  0x1

#define GBE__RX_VLAN_PACKETS_GOOD_BAD__RXVLANPKTGB__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Watchdog_Error_Packets
// Rx Watchdog Error Packets
///////////////////////////////////////////////////////

#define GBE__RX_WATCHDOG_ERROR_PACKETS__OFFSET (0x7DCULL)
#define GBE__RX_WATCHDOG_ERROR_PACKETS__NUM  0x1

#define GBE__RX_WATCHDOG_ERROR_PACKETS__RXWDGERR__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Receive_Error_Packets
// Rx Receive Error Packets
///////////////////////////////////////////////////////

#define GBE__RX_RECEIVE_ERROR_PACKETS__OFFSET (0x7E0ULL)
#define GBE__RX_RECEIVE_ERROR_PACKETS__NUM  0x1

#define GBE__RX_RECEIVE_ERROR_PACKETS__RXRCVERR__SHIFT    0

///////////////////////////////////////////////////////
// Register: Rx_Control_Packets_Good
// Rx Control Packets Good
///////////////////////////////////////////////////////

#define GBE__RX_CONTROL_PACKETS_GOOD__OFFSET (0x7E4ULL)
#define GBE__RX_CONTROL_PACKETS_GOOD__NUM  0x1

#define GBE__RX_CONTROL_PACKETS_GOOD__RXCTRLG__SHIFT    0

///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Control
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_TIMESTAMP_CONTROL__OFFSET (0xB00ULL)
#define GBE__MAC_TIMESTAMP_CONTROL__NUM  0x1

#define GBE__MAC_TIMESTAMP_CONTROL__TSENA__SHIFT    0
#define GBE__MAC_TIMESTAMP_CONTROL__TSCFUPDT__SHIFT    1
#define GBE__MAC_TIMESTAMP_CONTROL__TSINIT__SHIFT    2
#define GBE__MAC_TIMESTAMP_CONTROL__TSUPDT__SHIFT    3
#define GBE__MAC_TIMESTAMP_CONTROL__TSTRIG__SHIFT    4
#define GBE__MAC_TIMESTAMP_CONTROL__TSOFFSETEG__SHIFT    5
#define GBE__MAC_TIMESTAMP_CONTROL__RESERVED_7_6__SHIFT    6
#define GBE__MAC_TIMESTAMP_CONTROL__TSENALL__SHIFT    8
#define GBE__MAC_TIMESTAMP_CONTROL__TSCTRLSSR__SHIFT    9
#define GBE__MAC_TIMESTAMP_CONTROL__TSVER2ENA__SHIFT    10
#define GBE__MAC_TIMESTAMP_CONTROL__TSIPENA__SHIFT    11
#define GBE__MAC_TIMESTAMP_CONTROL__TSIPV6ENA__SHIFT    12
#define GBE__MAC_TIMESTAMP_CONTROL__TSIPV4ENA__SHIFT    13
#define GBE__MAC_TIMESTAMP_CONTROL__TSEVNTENA__SHIFT    14
#define GBE__MAC_TIMESTAMP_CONTROL__TSMSTRENA__SHIFT    15
#define GBE__MAC_TIMESTAMP_CONTROL__SNAPTYPSEL__SHIFT    16
#define GBE__MAC_TIMESTAMP_CONTROL__TSENMACOFFSET__SHIFT    18
#define GBE__MAC_TIMESTAMP_CONTROL__CSC__SHIFT    19
#define GBE__MAC_TIMESTAMP_CONTROL__ESTI__SHIFT    20
#define GBE__MAC_TIMESTAMP_CONTROL__RESERVED_23_21__SHIFT    21
#define GBE__MAC_TIMESTAMP_CONTROL__TXTSSTSM__SHIFT    24
#define GBE__MAC_TIMESTAMP_CONTROL__RESERVED_27_25__SHIFT    25
#define GBE__MAC_TIMESTAMP_CONTROL__AV8021ASMEN__SHIFT    28
#define GBE__MAC_TIMESTAMP_CONTROL__RESERVED_31_29__SHIFT    29

///////////////////////////////////////////////////////
// Register: MAC_Sub_Second_Increment
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_SUB_SECOND_INCREMENT__OFFSET (0xB04ULL)
#define GBE__MAC_SUB_SECOND_INCREMENT__NUM  0x1

#define GBE__MAC_SUB_SECOND_INCREMENT__RESERVED_7_0__SHIFT    0
#define GBE__MAC_SUB_SECOND_INCREMENT__SNSINC__SHIFT    8
#define GBE__MAC_SUB_SECOND_INCREMENT__SSINC__SHIFT    16
#define GBE__MAC_SUB_SECOND_INCREMENT__RESERVED_31_24__SHIFT    24

///////////////////////////////////////////////////////
// Register: MAC_System_Time_Seconds
//  
// Timestamp Second
///////////////////////////////////////////////////////

#define GBE__MAC_SYSTEM_TIME_SECONDS__OFFSET (0xB08ULL)
#define GBE__MAC_SYSTEM_TIME_SECONDS__NUM  0x1

#define GBE__MAC_SYSTEM_TIME_SECONDS__TSS__SHIFT    0

///////////////////////////////////////////////////////
// Register: MAC_System_Time_Nanoseconds
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_SYSTEM_TIME_NANOSECONDS__OFFSET (0xB0CULL)
#define GBE__MAC_SYSTEM_TIME_NANOSECONDS__NUM  0x1

#define GBE__MAC_SYSTEM_TIME_NANOSECONDS__TSSS__SHIFT    0
#define GBE__MAC_SYSTEM_TIME_NANOSECONDS__RESERVED_31__SHIFT    31

///////////////////////////////////////////////////////
// Register: MAC_System_Time_Seconds_Update
//  
// Timestamp Seconds
///////////////////////////////////////////////////////

#define GBE__MAC_SYSTEM_TIME_SECONDS_UPDATE__OFFSET (0xB10ULL)
#define GBE__MAC_SYSTEM_TIME_SECONDS_UPDATE__NUM  0x1

#define GBE__MAC_SYSTEM_TIME_SECONDS_UPDATE__TSS__SHIFT    0

///////////////////////////////////////////////////////
// Register: MAC_System_Time_Nanoseconds_Update
// Add or Subtract Time
///////////////////////////////////////////////////////

#define GBE__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__OFFSET (0xB14ULL)
#define GBE__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__NUM  0x1

#define GBE__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__TSSS__SHIFT    0
#define GBE__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__ADDSUB__SHIFT    31

///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Addend
// Timestamp Addend Register
///////////////////////////////////////////////////////

#define GBE__MAC_TIMESTAMP_ADDEND__OFFSET (0xB18ULL)
#define GBE__MAC_TIMESTAMP_ADDEND__NUM  0x1

#define GBE__MAC_TIMESTAMP_ADDEND__TSAR__SHIFT    0

///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Status
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_TIMESTAMP_STATUS__OFFSET (0xB20ULL)
#define GBE__MAC_TIMESTAMP_STATUS__NUM  0x1

#define GBE__MAC_TIMESTAMP_STATUS__TSSOVF__SHIFT    0
#define GBE__MAC_TIMESTAMP_STATUS__TSTARGT0__SHIFT    1
#define GBE__MAC_TIMESTAMP_STATUS__AUXTSTRIG__SHIFT    2
#define GBE__MAC_TIMESTAMP_STATUS__TSTRGTERR0__SHIFT    3
#define GBE__MAC_TIMESTAMP_STATUS__RESERVED_14_4__SHIFT    4
#define GBE__MAC_TIMESTAMP_STATUS__TXTSSIS__SHIFT    15
#define GBE__MAC_TIMESTAMP_STATUS__RESERVED_23_16__SHIFT    16
#define GBE__MAC_TIMESTAMP_STATUS__ATSSTM__SHIFT    24
#define GBE__MAC_TIMESTAMP_STATUS__ATSNS__SHIFT    25
#define GBE__MAC_TIMESTAMP_STATUS__RESERVED_31_30__SHIFT    30

///////////////////////////////////////////////////////
// Register: MAC_Tx_Timestamp_Status_Nanoseconds
// Transmit Timestamp Status Missed
///////////////////////////////////////////////////////

#define GBE__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__OFFSET (0xB30ULL)
#define GBE__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__NUM  0x1

#define GBE__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__TXTSSLO__SHIFT    0
#define GBE__MAC_TX_TIMESTAMP_STATUS_NANOSECONDS__TXTSSMIS__SHIFT    31

///////////////////////////////////////////////////////
// Register: MAC_Tx_Timestamp_Status_Seconds
// Transmit Timestamp Status High
///////////////////////////////////////////////////////

#define GBE__MAC_TX_TIMESTAMP_STATUS_SECONDS__OFFSET (0xB34ULL)
#define GBE__MAC_TX_TIMESTAMP_STATUS_SECONDS__NUM  0x1

#define GBE__MAC_TX_TIMESTAMP_STATUS_SECONDS__TXTSSHI__SHIFT    0

///////////////////////////////////////////////////////
// Register: MAC_Auxiliary_Control
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_AUXILIARY_CONTROL__OFFSET (0xB40ULL)
#define GBE__MAC_AUXILIARY_CONTROL__NUM  0x1

#define GBE__MAC_AUXILIARY_CONTROL__ATSFC__SHIFT    0
#define GBE__MAC_AUXILIARY_CONTROL__RESERVED_3_1__SHIFT    1
#define GBE__MAC_AUXILIARY_CONTROL__ATSEN0__SHIFT    4
#define GBE__MAC_AUXILIARY_CONTROL__RESERVED_31_5__SHIFT    5

///////////////////////////////////////////////////////
// Register: MAC_Auxiliary_Timestamp_Nanoseconds
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__OFFSET (0xB48ULL)
#define GBE__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__NUM  0x1

#define GBE__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__AUXTSLO__SHIFT    0
#define GBE__MAC_AUXILIARY_TIMESTAMP_NANOSECONDS__RESERVED_31__SHIFT    31

///////////////////////////////////////////////////////
// Register: MAC_Auxiliary_Timestamp_Seconds
// Auxiliary Timestamp
///////////////////////////////////////////////////////

#define GBE__MAC_AUXILIARY_TIMESTAMP_SECONDS__OFFSET (0xB4CULL)
#define GBE__MAC_AUXILIARY_TIMESTAMP_SECONDS__NUM  0x1

#define GBE__MAC_AUXILIARY_TIMESTAMP_SECONDS__AUXTSHI__SHIFT    0

///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Ingress_Asym_Corr
//  
// One-Step Timestamp Ingress Asymmetry Correction
///////////////////////////////////////////////////////

#define GBE__MAC_TIMESTAMP_INGRESS_ASYM_CORR__OFFSET (0xB50ULL)
#define GBE__MAC_TIMESTAMP_INGRESS_ASYM_CORR__NUM  0x1

#define GBE__MAC_TIMESTAMP_INGRESS_ASYM_CORR__OSTIAC__SHIFT    0

///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Egress_Asym_Corr
//  
// One-Step Timestamp Ingress Asymmetry Correction
///////////////////////////////////////////////////////

#define GBE__MAC_TIMESTAMP_EGRESS_ASYM_CORR__OFFSET (0xB54ULL)
#define GBE__MAC_TIMESTAMP_EGRESS_ASYM_CORR__NUM  0x1

#define GBE__MAC_TIMESTAMP_EGRESS_ASYM_CORR__OSTEAC__SHIFT    0

///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Ingress_Corr_Nanosecond
// Timestamp Egress Correction
///////////////////////////////////////////////////////

#define GBE__MAC_TIMESTAMP_INGRESS_CORR_NANOSECOND__OFFSET (0xB58ULL)
#define GBE__MAC_TIMESTAMP_INGRESS_CORR_NANOSECOND__NUM  0x1

#define GBE__MAC_TIMESTAMP_INGRESS_CORR_NANOSECOND__TSIC__SHIFT    0

///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Ingress_Corr_Subnanosec
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__OFFSET (0xB60ULL)
#define GBE__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__NUM  0x1

#define GBE__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__RESERVED_7_0__SHIFT    0
#define GBE__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__TSICSNS__SHIFT    8
#define GBE__MAC_TIMESTAMP_INGRESS_CORR_SUBNANOSEC__RESERVED_31_16__SHIFT    16

///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Egress_Corr_Subnanosec
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__OFFSET (0xB64ULL)
#define GBE__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__NUM  0x1

#define GBE__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__RESERVED_7_0__SHIFT    0
#define GBE__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__TSECSNS__SHIFT    8
#define GBE__MAC_TIMESTAMP_EGRESS_CORR_SUBNANOSEC__RESERVED_31_16__SHIFT    16

///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Ingress_Latency
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_TIMESTAMP_INGRESS_LATENCY__OFFSET (0xB68ULL)
#define GBE__MAC_TIMESTAMP_INGRESS_LATENCY__NUM  0x1

#define GBE__MAC_TIMESTAMP_INGRESS_LATENCY__RESERVED_7_0__SHIFT    0
#define GBE__MAC_TIMESTAMP_INGRESS_LATENCY__ITLSNS__SHIFT    8
#define GBE__MAC_TIMESTAMP_INGRESS_LATENCY__ITLNS__SHIFT    16
#define GBE__MAC_TIMESTAMP_INGRESS_LATENCY__RESERVED_31_28__SHIFT    28

///////////////////////////////////////////////////////
// Register: MAC_Timestamp_Egress_Latency
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_TIMESTAMP_EGRESS_LATENCY__OFFSET (0xB6CULL)
#define GBE__MAC_TIMESTAMP_EGRESS_LATENCY__NUM  0x1

#define GBE__MAC_TIMESTAMP_EGRESS_LATENCY__RESERVED_7_0__SHIFT    0
#define GBE__MAC_TIMESTAMP_EGRESS_LATENCY__ETLSNS__SHIFT    8
#define GBE__MAC_TIMESTAMP_EGRESS_LATENCY__ETLNS__SHIFT    16
#define GBE__MAC_TIMESTAMP_EGRESS_LATENCY__RESERVED_31_28__SHIFT    28

///////////////////////////////////////////////////////
// Register: MAC_PPS_Control
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_PPS_CONTROL__OFFSET (0xB70ULL)
#define GBE__MAC_PPS_CONTROL__NUM  0x1

#define GBE__MAC_PPS_CONTROL__PPSCTRL_PPSCMD__SHIFT    0
#define GBE__MAC_PPS_CONTROL__RESERVED_31_4__SHIFT    4

///////////////////////////////////////////////////////
// Register: MAC_PPS0_Target_Time_Seconds
// PPS Target Time Seconds Register
///////////////////////////////////////////////////////

#define GBE__MAC_PPS0_TARGET_TIME_SECONDS__OFFSET (0x0B80ULL)
#define GBE__MAC_PPS0_TARGET_TIME_SECONDS__NUM  0x1

#define GBE__MAC_PPS0_TARGET_TIME_SECONDS__TSTRH0__SHIFT    0

///////////////////////////////////////////////////////
// Register: MAC_PPS0_Target_Time_Nanoseconds
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MAC_PPS0_TARGET_TIME_NANOSECONDS__OFFSET (0x0B84ULL)
#define GBE__MAC_PPS0_TARGET_TIME_NANOSECONDS__NUM  0x1

#define GBE__MAC_PPS0_TARGET_TIME_NANOSECONDS__TTSL0__SHIFT    0
#define GBE__MAC_PPS0_TARGET_TIME_NANOSECONDS__RESERVED_31__SHIFT    31

///////////////////////////////////////////////////////
// Register: MTL_Operation_Mode
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MTL_OPERATION_MODE__OFFSET (0xC00ULL)
#define GBE__MTL_OPERATION_MODE__NUM  0x1

#define GBE__MTL_OPERATION_MODE__RESERVED_0__SHIFT    0
#define GBE__MTL_OPERATION_MODE__DTXSTS__SHIFT    1
#define GBE__MTL_OPERATION_MODE__RESERVED_7_2__SHIFT    2
#define GBE__MTL_OPERATION_MODE__CNTPRST__SHIFT    8
#define GBE__MTL_OPERATION_MODE__CNTCLR__SHIFT    9
#define GBE__MTL_OPERATION_MODE__RESERVED_31_10__SHIFT    10

///////////////////////////////////////////////////////
// Register: MTL_Interrupt_Status
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MTL_INTERRUPT_STATUS__OFFSET (0xC20ULL)
#define GBE__MTL_INTERRUPT_STATUS__NUM  0x1

#define GBE__MTL_INTERRUPT_STATUS__Q0IS__SHIFT    0
#define GBE__MTL_INTERRUPT_STATUS__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: MTL_TxQ0_Operation_Mode
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MTL_TXQ0_OPERATION_MODE__OFFSET (0xD00ULL)
#define GBE__MTL_TXQ0_OPERATION_MODE__NUM  0x1

#define GBE__MTL_TXQ0_OPERATION_MODE__FTQ__SHIFT    0
#define GBE__MTL_TXQ0_OPERATION_MODE__TSF__SHIFT    1
#define GBE__MTL_TXQ0_OPERATION_MODE__RESERVED_3_2__SHIFT    2
#define GBE__MTL_TXQ0_OPERATION_MODE__TTC__SHIFT    4
#define GBE__MTL_TXQ0_OPERATION_MODE__RESERVED_31_7__SHIFT    7

///////////////////////////////////////////////////////
// Register: MTL_TxQ0_Underflow
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MTL_TXQ0_UNDERFLOW__OFFSET (0xD04ULL)
#define GBE__MTL_TXQ0_UNDERFLOW__NUM  0x1

#define GBE__MTL_TXQ0_UNDERFLOW__UFFRMCNT__SHIFT    0
#define GBE__MTL_TXQ0_UNDERFLOW__UFCNTOVF__SHIFT    11
#define GBE__MTL_TXQ0_UNDERFLOW__RESERVED_31_12__SHIFT    12

///////////////////////////////////////////////////////
// Register: MTL_TxQ0_Debug
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MTL_TXQ0_DEBUG__OFFSET (0xD08ULL)
#define GBE__MTL_TXQ0_DEBUG__NUM  0x1

#define GBE__MTL_TXQ0_DEBUG__TXQPAUSED__SHIFT    0
#define GBE__MTL_TXQ0_DEBUG__TRCSTS__SHIFT    1
#define GBE__MTL_TXQ0_DEBUG__TWCSTS__SHIFT    3
#define GBE__MTL_TXQ0_DEBUG__TXQSTS__SHIFT    4
#define GBE__MTL_TXQ0_DEBUG__TXSTSFSTS__SHIFT    5
#define GBE__MTL_TXQ0_DEBUG__RESERVED_15_6__SHIFT    6
#define GBE__MTL_TXQ0_DEBUG__PTXQ__SHIFT    16
#define GBE__MTL_TXQ0_DEBUG__RESERVED_19__SHIFT    19
#define GBE__MTL_TXQ0_DEBUG__STXSTSF__SHIFT    20
#define GBE__MTL_TXQ0_DEBUG__RESERVED_31_23__SHIFT    23

///////////////////////////////////////////////////////
// Register: MTL_Q0_Interrupt_Control_Status
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MTL_Q0_INTERRUPT_CONTROL_STATUS__OFFSET (0xD2CULL)
#define GBE__MTL_Q0_INTERRUPT_CONTROL_STATUS__NUM  0x1

#define GBE__MTL_Q0_INTERRUPT_CONTROL_STATUS__TXUNFIS__SHIFT    0
#define GBE__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_7_1__SHIFT    1
#define GBE__MTL_Q0_INTERRUPT_CONTROL_STATUS__TXUIE__SHIFT    8
#define GBE__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_15_9__SHIFT    9
#define GBE__MTL_Q0_INTERRUPT_CONTROL_STATUS__RXOVFIS__SHIFT    16
#define GBE__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_23_17__SHIFT    17
#define GBE__MTL_Q0_INTERRUPT_CONTROL_STATUS__RXOIE__SHIFT    24
#define GBE__MTL_Q0_INTERRUPT_CONTROL_STATUS__RESERVED_31_25__SHIFT    25

///////////////////////////////////////////////////////
// Register: MTL_RxQ0_Operation_Mode
// Reserved.
///////////////////////////////////////////////////////
#define GBE__MTL_RXQ0_OPERATION_MODE__OFFSET (0xD30ULL)
#define GBE__MTL_RXQ0_OPERATION_MODE__NUM  0x1

#define GBE__MTL_RXQ0_OPERATION_MODE__RTC__SHIFT    0
#define GBE__MTL_RXQ0_OPERATION_MODE__RESERVED_2__SHIFT    2
#define GBE__MTL_RXQ0_OPERATION_MODE__FUP__SHIFT    3
#define GBE__MTL_RXQ0_OPERATION_MODE__FEP__SHIFT    4
#define GBE__MTL_RXQ0_OPERATION_MODE__RSF__SHIFT    5
#define GBE__MTL_RXQ0_OPERATION_MODE__DIS_TCP_EF__SHIFT    6
#define GBE__MTL_RXQ0_OPERATION_MODE__EHFC__SHIFT    7
#define GBE__MTL_RXQ0_OPERATION_MODE__RFA__SHIFT    8
#define GBE__MTL_RXQ0_OPERATION_MODE__RESERVED_13__SHIFT    13
#define GBE__MTL_RXQ0_OPERATION_MODE__RFD__SHIFT    14
#define GBE__MTL_RXQ0_OPERATION_MODE__RESERVED_31_19__SHIFT    19

///////////////////////////////////////////////////////
// Register: MTL_RxQ0_Missed_Packet_Overflow_Cnt
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__OFFSET (0xD34ULL)
#define GBE__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__NUM  0x1

#define GBE__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__OVFPKTCNT__SHIFT    0
#define GBE__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__OVFCNTOVF__SHIFT    11
#define GBE__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__RESERVED_15_12__SHIFT    12
#define GBE__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__MISPKTCNT__SHIFT    16
#define GBE__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__MISCNTOVF__SHIFT    27
#define GBE__MTL_RXQ0_MISSED_PACKET_OVERFLOW_CNT__RESERVED_31_28__SHIFT    28

///////////////////////////////////////////////////////
// Register: MTL_RxQ0_Debug
// Reserved.
///////////////////////////////////////////////////////

#define GBE__MTL_RXQ0_DEBUG__OFFSET (0xD38ULL)
#define GBE__MTL_RXQ0_DEBUG__NUM  0x1

#define GBE__MTL_RXQ0_DEBUG__RWCSTS__SHIFT    0
#define GBE__MTL_RXQ0_DEBUG__RRCSTS__SHIFT    1
#define GBE__MTL_RXQ0_DEBUG__RESERVED_3__SHIFT    3
#define GBE__MTL_RXQ0_DEBUG__RXQSTS__SHIFT    4
#define GBE__MTL_RXQ0_DEBUG__RESERVED_15_6__SHIFT    6
#define GBE__MTL_RXQ0_DEBUG__PRXQ__SHIFT    16
#define GBE__MTL_RXQ0_DEBUG__RESERVED_31_30__SHIFT    30

///////////////////////////////////////////////////////
// TDES0 Normal Descriptor
// Reserved.
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// TDES1 Normal Descriptor
// Reserved.
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// TDES2 Normal Descriptor
// Reserved.
///////////////////////////////////////////////////////
#define GBE__DESC_TX_DESC2__HL_B1L__SHIFT    0
#define GBE__DESC_TX_DESC2__VTIR__SHIFT    14
#define GBE__DESC_TX_DESC2__B2L__SHIFT    16
#define GBE__DESC_TX_DESC2__TTSE_TMWD__SHIFT    30
#define GBE__DESC_TX_DESC2__IOC__SHIFT    31

#define GBE__DESC_TX_DESC2__B1L__SIZE    14
#define GBE__DESC_TX_DESC2__B2L__SIZE    14

///////////////////////////////////////////////////////
// TDES3 Normal Descriptor
// Reserved.
///////////////////////////////////////////////////////
#define GBE__DESC_TX_DESC3__FL_TPL__SHIFT    0
#define GBE__DESC_TX_DESC3__TPL__SHIFT    15
#define GBE__DESC_TX_DESC3__CIC_TPL__SHIFT    16
#define GBE__DESC_TX_DESC3__TSE__SHIFT    18
#define GBE__DESC_TX_DESC3__SLOTNUM_THL__SHIFT    19
#define GBE__DESC_TX_DESC3__SAIC__SHIFT    23
#define GBE__DESC_TX_DESC3__CPC__SHIFT    26
#define GBE__DESC_TX_DESC3__LD__SHIFT    28
#define GBE__DESC_TX_DESC3__FD__SHIFT    29
#define GBE__DESC_TX_DESC3__CTXT__SHIFT    30
#define GBE__DESC_TX_DESC3__OWN__SHIFT    31

#define GBE__DESC_TX_DESC3__FL_TPL__SIZE    15

#define GBE__DESC_TX_DESC3__CIC_TPL__SIZE   2

/* Write-back Descriptor */
#define GBE__DESC_WRITE_BACK_TX_DESC3__IHE__SHIFT    0
#define GBE__DESC_WRITE_BACK_TX_DESC3__DB__SHIFT    1
#define GBE__DESC_WRITE_BACK_TX_DESC3__UF__SHIFT    2
#define GBE__DESC_WRITE_BACK_TX_DESC3__ED__SHIFT    3
#define GBE__DESC_WRITE_BACK_TX_DESC3__CC__SHIFT    4
#define GBE__DESC_WRITE_BACK_TX_DESC3__EC__SHIFT    8
#define GBE__DESC_WRITE_BACK_TX_DESC3__LC__SHIFT    9
#define GBE__DESC_WRITE_BACK_TX_DESC3__NC__SHIFT    10
#define GBE__DESC_WRITE_BACK_TX_DESC3__LOC__SHIFT    11
#define GBE__DESC_WRITE_BACK_TX_DESC3__PCE__SHIFT    12
#define GBE__DESC_WRITE_BACK_TX_DESC3__FF__SHIFT    13
#define GBE__DESC_WRITE_BACK_TX_DESC3__JT__SHIFT    14
#define GBE__DESC_WRITE_BACK_TX_DESC3__ES__SHIFT    15
#define GBE__DESC_WRITE_BACK_TX_DESC3__EUE__SHIFT    16
#define GBE__DESC_WRITE_BACK_TX_DESC3__TTSS__SHIFT    17
#define GBE__DESC_WRITE_BACK_TX_DESC3__DE__SHIFT    23
#define GBE__DESC_WRITE_BACK_TX_DESC3__LD__SHIFT    28
#define GBE__DESC_WRITE_BACK_TX_DESC3__FD__SHIFT    29
#define GBE__DESC_WRITE_BACK_TX_DESC3__CTXT__SHIFT    30
#define GBE__DESC_WRITE_BACK_TX_DESC3__OWN__SHIFT    31

///////////////////////////////////////////////////////
// RDES3 Descriptor
// Reserved.
///////////////////////////////////////////////////////

/* Normal Descriptor */
#define GBE__DESC_RX_DESC3__BUF2AP__SHIFT    0
#define GBE__DESC_RX_DESC3__BUF1V__SHIFT    24
#define GBE__DESC_RX_DESC3__BUF2V__SHIFT    25
#define GBE__DESC_RX_DESC3__IOC__SHIFT    30
#define GBE__DESC_RX_DESC3__OWN__SHIFT    31

/* Write-back Descriptor */
#define GBE__DESC_WRITE_BACK_RX_DESC3__PL__SHIFT    0
#define GBE__DESC_WRITE_BACK_RX_DESC3__ES__SHIFT    15
#define GBE__DESC_WRITE_BACK_RX_DESC3__LT__SHIFT    26
#define GBE__DESC_WRITE_BACK_RX_DESC3__DE__SHIFT    19
#define GBE__DESC_WRITE_BACK_RX_DESC3__RE__SHIFT    20
#define GBE__DESC_WRITE_BACK_RX_DESC3__OE__SHIFT    21
#define GBE__DESC_WRITE_BACK_RX_DESC3__RWT__SHIFT    22
#define GBE__DESC_WRITE_BACK_RX_DESC3__GP__SHIFT    23
#define GBE__DESC_WRITE_BACK_RX_DESC3__CE__SHIFT    24
#define GBE__DESC_WRITE_BACK_RX_DESC3__RS0V__SHIFT    25
#define GBE__DESC_WRITE_BACK_RX_DESC3__RS1V__SHIFT    26
#define GBE__DESC_WRITE_BACK_RX_DESC3__RS2V__SHIFT    27
#define GBE__DESC_WRITE_BACK_RX_DESC3__LD__SHIFT    28
#define GBE__DESC_WRITE_BACK_RX_DESC3__FD__SHIFT    29
#define GBE__DESC_WRITE_BACK_RX_DESC3__CTXT__SHIFT    30
#define GBE__DESC_WRITE_BACK_RX_DESC3__OWN__SHIFT    31

#define GBE__DESC_WRITE_BACK_RX_DESC3__PL__SIZE    15

typedef struct
{
    AL_U32                  DeviceId;
    AL_U32                  BaseAddress;
    AL_U32                  InputClockHz;
    AL_U32                  IntrNum;
} AL_GBE_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif