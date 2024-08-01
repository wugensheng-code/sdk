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

typedef struct {
    const AL_U8 *PhyName;
    AL_U32 PhyID;
} AL_GBE_PhyInfoStruct;

/* Define phy register */
#define PHY_ID_RTL8211F                     0x001cc916
#define PHY_RTL8211F_PHASE_100M             0x14a1
#define PHY_RTL8211F_PHASE_1000M            0x1461

#define PHY_ID_YT8531                       0x4f51e91b
#define PHY_YT8531_PHASE_100M               0x361
#define PHY_YT8531_PHASE_1000M              0x621

#define PHY_ID_DEFAULT                      PHY_ID_RTL8211F
#define PHY_DEFAULT_PHASE_100M              PHY_RTL8211F_PHASE_100M
#define PHY_DEFAULT_PHASE_1000M             PHY_RTL8211F_PHASE_1000M

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

/* RGMII TXC delay register */
#define PHY_RGMII_TX_DELAY_REG              0x11

/* RGMII RXC delay register */
#define PHY_RGMII_RX_DELAY_REG              0x15

/* Specific Mode Status Register */
#define PHY_SSR_REG                         0x1A

/* PHY page select register. */
#define PHY_PAGE_SELECT_REG                 0x1F

/* PHY_BCR register descriptions define */
#define PHY_BCTL_SPEED1_MASK                0x0040
#define PHY_BCTL_ISOLATE_MASK               0x0400
#define PHY_BCTL_DUPLEX_MASK                0x0100
#define PHY_BCTL_RESTART_AUTONEG_MASK       0x0200
#define PHY_BCTL_AUTONEG_MASK               0x1000
#define PHY_BCTL_SPEED0_MASK                0x2000
#define PHY_BCTL_LOOP_MASK                  0x4000
#define PHY_BCTL_RESET_MASK                 0x8000

/* PHY_BSR_REG register descriptions define */
#define PHY_BSTATUS_LINKSTATUS_MASK         0x0004
#define PHY_BSTATUS_AUTONEGABLE_MASK        0x0008
#define PHY_BSTATUS_SPEEDUPLX_MASK          0x001C
#define PHY_BSTATUS_AUTONEGCOMP_MASK        0x0020

/* PHY_BSR_REG register descriptions define */
#define PHY_100BaseT4_ABILITY_MASK          0x0200
#define PHY_100BASETX_FULLDUPLEX_MASK       0x0100
#define PHY_100BASETX_HALFDUPLEX_MASK       0x0080
#define PHY_10BASETX_FULLDUPLEX_MASK        0x0040
#define PHY_10BASETX_HALFDUPLEX_MASK        0x0020
#define PHY_IEEE802_3_SELECTOR_MASK         0x0001

/* PHY_1000BASET_CONTROL_REG register descriptions define */
#define PHY_1000BASET_FULLDUPLEX_MASK       0x0200
#define PHY_1000BASET_HALFDUPLEX_MASK       0x0100

/* PHY_SSR_REG register descriptions define */
#define PHY_SSTATUS_LINKSTATUS_MASK         0x0004
#define PHY_SSTATUS_LINKSPEED_MASK          0x0030
#define PHY_SSTATUS_LINKDUPLEX_MASK         0x0008
#define PHY_SSTATUS_LINKSPEED_SHIFT         0x4

#define PHY_PAGE_RGMII_TXRX_DELAY_ADDR      0x0D08
#define PHY_RGMII_TX_DELAY_MASK             0x0100
#define PHY_RGMII_RX_DELAY_MASK             0x0008

/* Defines the PHY link speed. This is not align with the speed for MAC */
#define PHY_SPEED_10M                       0
#define PHY_SPEED_100M                      1
#define PHY_SPEED_1000M                     2

/* Defines the PHY link duplex. */
#define PHY_HALF_DUPLEX                     0
#define PHY_FULL_DUPLEX                     1

#define ADVERTISE_10HALF            0x0020  /* Try for 10mbps half-duplex  */
#define ADVERTISE_1000XFULL         0x0020  /* Try for 1000BASE-X full-duplex */
#define ADVERTISE_10FULL            0x0040  /* Try for 10mbps full-duplex  */
#define ADVERTISE_1000XHALF         0x0040  /* Try for 1000BASE-X half-duplex */
#define ADVERTISE_100HALF           0x0080  /* Try for 100mbps half-duplex */
#define ADVERTISE_1000XPAUSE        0x0080  /* Try for 1000BASE-X pause    */
#define ADVERTISE_100FULL           0x0100  /* Try for 100mbps full-duplex */
#define ADVERTISE_1000XPSE_ASYM     0x0100  /* Try for 1000BASE-X asym pause */
#define ADVERTISE_100BASE4          0x0200  /* Try for 100mbps 4k packets  */


#define ADVERTISE_100_AND_10        (ADVERTISE_10FULL | ADVERTISE_100FULL | \
					ADVERTISE_10HALF | ADVERTISE_100HALF)
#define ADVERTISE_100               (ADVERTISE_100FULL | ADVERTISE_100HALF)
#define ADVERTISE_10                (ADVERTISE_10FULL | ADVERTISE_10HALF)

#define ADVERTISE_1000              0x0300

#define IEEE_CONTROL_REG_OFFSET                    0
#define IEEE_STATUS_REG_OFFSET                     1
#define IEEE_AUTONEGO_ADVERTISE_REG                4
#define IEEE_PARTNER_ABILITIES_1_REG_OFFSET        5
#define IEEE_PARTNER_ABILITIES_2_REG_OFFSET        8
#define IEEE_PARTNER_ABILITIES_3_REG_OFFSET        10
#define IEEE_1000_ADVERTISE_REG_OFFSET             9
#define IEEE_MMD_ACCESS_CONTROL_REG                13
#define IEEE_MMD_ACCESS_ADDRESS_DATA_REG           14
#define IEEE_COPPER_SPECIFIC_CONTROL_REG           16
#define IEEE_SPECIFIC_STATUS_REG                   17
#define IEEE_COPPER_SPECIFIC_STATUS_REG_2          19
#define IEEE_EXT_PHY_SPECIFIC_CONTROL_REG          20
#define IEEE_CONTROL_REG_MAC                       21
#define IEEE_PAGE_ADDRESS_REGISTER                 22

#define IEEE_CTRL_1GBPS_LINKSPEED_MASK             0x2040
#define IEEE_CTRL_LINKSPEED_MASK                   0x0040
#define IEEE_CTRL_LINKSPEED_1000M                  0x0040
#define IEEE_CTRL_LINKSPEED_100M                   0x2000
#define IEEE_CTRL_LINKSPEED_10M                    0x0000
#define IEEE_CTRL_FULL_DUPLEX                      0x100
#define IEEE_CTRL_RESET_MASK                       0x8000
#define IEEE_CTRL_AUTONEGOTIATE_ENABLE             0x1000
#define IEEE_STAT_AUTONEGOTIATE_CAPABLE            0x0008
#define IEEE_STAT_AUTONEGOTIATE_COMPLETE           0x0020
#define IEEE_STAT_AUTONEGOTIATE_RESTART            0x0200
#define IEEE_STAT_LINK_STATUS                      0x0004
#define IEEE_STAT_1GBPS_EXTENSIONS                 0x0100
#define IEEE_AN1_ABILITY_MASK                      0x1FE0
#define IEEE_AN3_ABILITY_MASK_1GBPS                0x0C00
#define IEEE_AN1_ABILITY_MASK_100MBPS              0x0380
#define IEEE_AN1_ABILITY_MASK_10MBPS               0x0060
#define IEEE_RGMII_TXRX_CLOCK_DELAYED_MASK         0x0030

#define IEEE_SPEED_MASK                            0xC000
#define IEEE_SPEED_1000                            0x8000
#define IEEE_SPEED_100                             0x4000

#define IEEE_ASYMMETRIC_PAUSE_MASK                 0x0800
#define IEEE_PAUSE_MASK                            0x0400
#define IEEE_AUTONEG_ERROR_MASK                    0x8000

#define IEEE_MMD_ACCESS_CTRL_DEVAD_MASK            0x1F
#define IEEE_MMD_ACCESS_CTRL_PIDEVAD_MASK          0x801F
#define IEEE_MMD_ACCESS_CTRL_NOPIDEVAD_MASK        0x401F

#define AL_GBE_PHY_READ_TIMEOUT_MS                 5000

#ifdef __cplusplus
}
#endif

#endif /* __AL_GBE_PHY_H_ */
