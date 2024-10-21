/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_AXI_ETHLITE_HW_H_
#define __AL_AXI_ETHLITE_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/
#define AL_AXI_ETH_LITE_NUM_INSTANCE        1

#if (defined _AARCH_64 || defined __aarch64__ || defined __arm__)
#define AXI_ETH_LITE0_BASE_ADDR             0x1C0000000ULL
#else
#define AXI_ETH_LITE0_BASE_ADDR             0x1F0000000ULL
#endif

#define AXI_ETH_LITE_CLOCK (100*MHz)

///////////////////////////////////////////////////////
// Register: ELITE_ETXSTAT0
// Tx Ping buffer ctrl
///////////////////////////////////////////////////////
#define AXI_ETHLITE_TX_PING_BUFFER_PTR_OFFSET                  (0x000ULL)

#define AXI_ETHLITE_TX_PING_BUFFER_FRAME_LEN_SHIFT             0
#define AXI_ETHLITE_TX_PING_BUFFER_TX_ENABLE_SHIFT             30
#define AXI_ETHLITE_TX_PING_BUFFER_TX_BUSY_SHIFT               31

#define AXI_ETHLITE_TX_PING_BUFFER_FRAME_LEN_SIZE              12

#define AXI_ETHLITE_TX_PING_BUFFER_FRAME_LEN_TX_ENABLE_SIZE    31

#define AXI_ETHLITE_TX_PING_BUFFER_ADDR_OFFSET                 (0x2000ULL)


///////////////////////////////////////////////////////
// Register: ELITE_ERXSTAT0
// Rx Ping buffer ctrl
///////////////////////////////////////////////////////
#define AXI_ETHLITE_RX_PING_BUFFER_PTR_OFFSET                  (0x004ULL)

#define AXI_ETHLITE_RX_PING_BUFFER_RECV_FRAME_LEN_SHIFT        0
#define AXI_ETHLITE_RX_PING_BUFFER_BAD_FRAME_FLAG_SHIFT        29
#define AXI_ETHLITE_RX_PING_BUFFER_CLR_FRAME_FLAG_SHIFT        30
#define AXI_ETHLITE_RX_PING_BUFFER_RECV_FRAME_FLAG_SHIFT       31

#define AXI_ETHLITE_RX_PING_BUFFER_RECV_FRAME_LEN_SIZE         12

#define AXI_ETHLITE_RX_PING_BUFFER_ADDR_OFFSET                 (0x4000ULL)


///////////////////////////////////////////////////////
// Register: ELITE_ETXSTAT1
// Tx Pong buffer ctrl
///////////////////////////////////////////////////////
#define AXI_ETHLITE_TX_PONG_BUFFER_PTR_OFFSET                  (0x008ULL)

#define AXI_ETHLITE_TX_PONG_BUFFER_FRAME_LEN_SHIFT             0
#define AXI_ETHLITE_TX_PONG_BUFFER_TX_ENABLE_SHIFT             30
#define AXI_ETHLITE_TX_PONG_BUFFER_TX_BUSY_SHIFT               31

#define AXI_ETHLITE_TX_PONG_BUFFER_FRAME_LEN_SIZE              12

#define AXI_ETHLITE_TX_PONG_BUFFER_ADDR_OFFSET                 (0x3000ULL)


///////////////////////////////////////////////////////
// Register: ELITE_ERXSTAT1
// Rx Pong buffer ctrl
///////////////////////////////////////////////////////
#define AXI_ETHLITE_RX_PONG_BUFFER_PTR_OFFSET                  (0x00CULL)

#define AXI_ETHLITE_RX_PONG_BUFFER_RECV_FRAME_LEN_SHIFT        0
#define AXI_ETHLITE_RX_PONG_BUFFER_BAD_FRAME_FLAG_SHIFT        29
#define AXI_ETHLITE_RX_PONG_BUFFER_CLR_FRAME_FLAG_SHIFT        30
#define AXI_ETHLITE_RX_PONG_BUFFER_RECV_FRAME_FLAG_SHIFT       31

#define AXI_ETHLITE_RX_PONG_BUFFER_RECV_FRAME_LEN_SIZE         12

#define AXI_ETHLITE_RX_PONG_BUFFER_ADDR_OFFSET                 (0x5000ULL)


///////////////////////////////////////////////////////
// Register: ELITE_RX_FRAME_LOSS_COUNT
// 
///////////////////////////////////////////////////////
#define AXI_ETHLITE_RX_FRAME_LOSS_COUNT_OFFSET                 (0x0010ULL)

#define AXI_ETHLITE_RX_FRAME_LOSS_CLR_ENABLE                   31


///////////////////////////////////////////////////////
// Register: ELITE_INTR_ENABLE
// 
///////////////////////////////////////////////////////
#define AXI_ETHLITE_INTR_ENABLE_OFFSET                         (0x0018ULL)

#define AXI_ETHLITE_INTR_RX_VALID_SHIFT                        0
#define AXI_ETHLITE_INTR_TX_IDLE_SHIFT                         1
#define AXI_ETHLITE_INTR_TX_DONE_SHIFT                         2
#define AXI_ETHLITE_INTR_RX_FRAME_LOSS_SHIFT                   6
#define AXI_ETHLITE_INTR_RX_BAD_FRAME_SHIFT                    7


///////////////////////////////////////////////////////
// Register: ELITE_INTR_EVENT
// 
///////////////////////////////////////////////////////
#define AXI_ETHLITE_INTR_EVENT_OFFSET                          (0x001CULL)

#define AXI_ETHLITE_INTR_EVENT_RX_VALID_SHIFT                  0
#define AXI_ETHLITE_INTR_EVENT_TX_IDLE_SHIFT                   1
#define AXI_ETHLITE_INTR_EVENT_TX_DONE_SHIFT                   2
#define AXI_ETHLITE_INTR_EVENT_RX_FRAME_LOSS_SHIFT             6
#define AXI_ETHLITE_INTR_EVENT_RX_BAD_FRAME_SHIFT              7


///////////////////////////////////////////////////////
// Register: ELITE_INTR_PENDING
// 
///////////////////////////////////////////////////////
#define AXI_ETHLITE_INTR_PENDING_OFFSET                        (0x0020ULL)

#define AXI_ETHLITE_INTR_PENDING_RX_VALID_SHIFT                0
#define AXI_ETHLITE_INTR_PENDING_TX_IDLE_SHIFT                 1
#define AXI_ETHLITE_INTR_PENDING_TX_DONE_SHIFT                 2
#define AXI_ETHLITE_INTR_PENDING_RX_FRAME_LOSS_SHIFT           6
#define AXI_ETHLITE_INTR_PENDING_RX_BAD_FRAME_SHIFT            7


///////////////////////////////////////////////////////
// Register: MDIO_CONFIG
// 
///////////////////////////////////////////////////////
#define AXI_ETHLITE_MDIO_CONFIG_OFFSET                         (0xD0ULL)

#define AXI_ETHLITE_MDIO_CLOCK_DIV_SHIFT                       0
#define AXI_ETHLITE_MDIO_ENABLE_SHIFT                          31

#define AXI_ETHLITE_MDIO_CLOCK_DIV_SIZE                        6


///////////////////////////////////////////////////////
// Register: MDIO_OPERATION
// 
///////////////////////////////////////////////////////
#define AXI_ETHLITE_MDIO_OPERATION_OFFSET                      (0xD4ULL)

#define AXI_ETHLITE_MDIO_OPERATION_TRANS_ENABLE_SHIFT          0
#define AXI_ETHLITE_MDIO_OPERATION_READ_WRITE_SHIFT            1
#define AXI_ETHLITE_MDIO_OPERATION_PHY_ADDR_SHIFT              3
#define AXI_ETHLITE_MDIO_OPERATION_PHY_REG_SHIFT               8

#define AXI_ETHLITE_MDIO_OPERATION_READ_WRITE_SIZE             2
#define AXI_ETHLITE_MDIO_OPERATION_PHY_ADDR_SIZE               5
#define AXI_ETHLITE_MDIO_OPERATION_PHY_REG_SIZE                8


///////////////////////////////////////////////////////
// Register: MDIO_READ_DATA
// 
///////////////////////////////////////////////////////
#define AXI_ETHLITE_MDIO_READ_DATA_OFFSET                      (0xD8ULL)

#define AXI_ETHLITE_MDIO_READ_DATA_SHIFT                       0


///////////////////////////////////////////////////////
// Register: MDIO_WRITE_DATA
// 
///////////////////////////////////////////////////////
#define AXI_ETHLITE_MDIO_WRITE_DATA_OFFSET                     (0xDCULL)

#define AXI_ETHLITE_MDIO_WRITE_DATA_SHIFT                      0


///////////////////////////////////////////////////////
// Register: MAC_CONFIG
// 
///////////////////////////////////////////////////////
#define AXI_ETHLITE_MAC_CONFIG_OFFSET                          (0x0E0ULL)

#define AXI_ETHLITE_TX_HALF_DUMPLE_ENABLE_SHIFT                0
#define AXI_ETHLITE_TX_VLAN_ENABLE_SHIFT                       1
#define AXI_ETHLITE_TX_FCS_ENABLE_SHIFT                        2
#define AXI_ETHLITE_TX_JUMBO_FRAME_ENABLE_SHIFT                3
#define AXI_ETHLITE_TX_INTER_FRAME_GAP_ADJUST_ENABLE_SHIFT     5
#define AXI_ETHLITE_TX_FLOW_CONTROL_ENABLE_SHIFT               6
#define AXI_ETHLITE_TX_ENABLE_SHIFT                            7
#define AXI_ETHLITE_RX_HALF_DUMPLE_ENABLE_SHIFT                8
#define AXI_ETHLITE_RX_VLAN_ENABLE_SHIFT                       9
#define AXI_ETHLITE_RX_FCS_ENABLE_SHIFT                        10
#define AXI_ETHLITE_RX_JUMBO_FRAME_ENABLE_SHIFT                11
#define AXI_ETHLITE_RX_CONTROL_FRAME_LEN_CHECK_ENABLE_SHIFT    12
#define AXI_ETHLITE_RX_FRAME_LEN_TYPE_CHECK_ENABLE_SHIFT       13
#define AXI_ETHLITE_RX_FLOW_CONTROL_ENABLE_SHIFT               14
#define AXI_ETHLITE_RX_ENABLE_SHIFT                            15
#define AXI_ETHLITE_TX_INTER_FRAME_GAP_VALUE_SHIFT             16
#define AXI_ETHLITE_SPEED_SHIFT                                30


#define AXI_ETHLITE_TX_INTER_FRAME_GAP_VALUE_SIZE              8
#define AXI_ETHLITE_SPEED_SIZE                                 2


///////////////////////////////////////////////////////
// Register: MAC_RESET
// 
///////////////////////////////////////////////////////
#define AXI_ETHLITE_RESET_OFFSET                               (0x0F0ULL)


///////////////////////////////////////////////////////
// Register: MAC_IP_INFO
// 
///////////////////////////////////////////////////////
#define AXI_ETHLITE_IP_INFO_OFFSET                             (0x0FCULL)

#define AXI_ETHLITE_TX_BUFFER_DEPTH_SHIFT                      0
#define AXI_ETHLITE_RX_BUFFER_DEPTH_SHIFT                      14
#define AXI_ETHLITE_MDIO_EXIST_SHIFT                           28

#define AXI_ETHLITE_TX_BUFFER_DEPTH_SIZE                       14
#define AXI_ETHLITE_RX_BUFFER_DEPTH_SIZE                       14


typedef struct {
    AL_U32      DeviceId;
    AL_UINTPTR  BaseAddress;
    AL_U32      InputClk;
    AL_U32      IntrId;
} AlAxiEthLite_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif /* __AL_AXI_ETHLITE_LITE_HW_H_ */
