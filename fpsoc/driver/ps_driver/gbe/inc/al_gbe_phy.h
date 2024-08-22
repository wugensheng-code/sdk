/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_GBE_PHY_H_
#define __AL_GBE_PHY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
  The following PHY registers are defined by the IEEE802.3 standard,
  and the same register and bit represent the same or similar functions.
 */
/* Basic mode control register */
#define PHY_BCR_REG                         0x0

/* Basic Mode Status Register */
#define PHY_BSR_REG                         0x1

/* PHY ID one register. */
#define PHY_ID1_REG                         0x02

/* PHY ID two register. */
#define PHY_ID2_REG                         0x03

/* PHY auto-negotiate advertise register */
#define PHY_AUTONEG_ADVERTISE_REG           0x04

/* PHY auto negotiation link partner ability register */
#define PHY_AUTONEG_LINKPARTNER_REG         0x05

/* PHY auto negotiation expansion register */
#define PHY_AUTONEG_EXPANSION_REG           0x06

/* PHY 1000BASE-T control register */
#define PHY_1000BASET_CONTROL_REG           0x09

/* PHY MMD access control register */
#define PHY_MMD_ACCESS_CONTROL_REG          0x0D

/* PHY MMD access data register */
#define PHY_MMD_ACCESS_DATA_REG             0x0E

/* PHY extend status register */
#define PHY_EXTEND_STATUS_REG               0x0F

/* Defines the PHY link speed. This is not align with the speed for MAC */
#define PHY_SPEED_10M                       0
#define PHY_SPEED_100M                      1
#define PHY_SPEED_1000M                     2

/* Defines the PHY link duplex. */
#define PHY_HALF_DUPLEX                     0
#define PHY_FULL_DUPLEX                     1

#define AL_GBE_PHY_READ_TIMEOUT_MS          5000

/* Define phy ID and PHY phase here */
#define PHY_ID_RTL8211F                     0x001cc916
#define PHY_RTL8211F_PHASE_100M             0x14a1
#define PHY_RTL8211F_PHASE_1000M            0x1461

#define PHY_ID_YT8531                       0x4f51e91b
#define PHY_YT8531_PHASE_100M               0x361
#define PHY_YT8531_PHASE_1000M              0x621

#define PHY_ID_DEFAULT                      PHY_ID_RTL8211F
#define PHY_DEFAULT_PHASE_100M              PHY_RTL8211F_PHASE_100M
#define PHY_DEFAULT_PHASE_1000M             PHY_RTL8211F_PHASE_1000M

typedef struct AL_PHY_DevStruct
{
    AL_U8 PhyAddr;
    const AL_U8 *PhyName;
    AL_U32 PhyId;
    AL_S32 (*Init)(struct AL_PHY_DevStruct *Phy);
    AL_S32 (*GetLinkStatus)(struct AL_PHY_DevStruct *Phy, AL_BOOL *Status);
    AL_S32 (*GetLinkSpeedDuplex)(struct AL_PHY_DevStruct *Phy, AL_U32 *Speed, AL_U32 *Duplex);
    AL_VOID *DevObj;
} AL_PHY_DevStruct;

typedef struct {
    const AL_U8 *PhyName;
    AL_PHY_DevStruct *PhyDev;
} AL_GBE_PhyInfoStruct;


#ifdef __cplusplus
}
#endif

#endif /* __AL_GBE_PHY_H_ */
