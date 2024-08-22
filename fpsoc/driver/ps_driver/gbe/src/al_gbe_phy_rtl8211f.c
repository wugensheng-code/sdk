/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gbe_hal.h"
#include "al_gbe_phy.h"

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

static AL_S32 AlPhy_RTL8211fInit(AL_PHY_DevStruct *Phy)
{
    AL_S32 Ret = 0;
    AL_U16 RegValue;
    AL_U16 TmpRegValue;
    AL_U8 PHYAddress;

    AL_GBE_DevStruct *Gbe = Phy->DevObj;
    PHYAddress = Phy->PhyAddr;

    /* Reset the Phy */
    Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_BCR_REG, PHY_BCTL_RESET_MASK);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    /* Delay at least 10 ms */
    AlSys_MDelay(500);

    /* Check reset status complete */
    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BCR_REG, &RegValue);
    if ((Ret != AL_OK) || (RegValue & PHY_BCTL_RESET_MASK)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return AL_GBE_ERR_PHY_RESET_FAILED;
    }

    /* The RGMII specifies output TXC/RXC and TXD/RXD without any clock skew. Need to add skew on clock line
    to make sure the other side sample right data. This can also be done in PCB traces. */
    Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_PAGE_SELECT_REG, PHY_PAGE_RGMII_TXRX_DELAY_ADDR);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    /* Set Tx Delay */
    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_RGMII_TX_DELAY_REG, &RegValue);
    if (Ret == AL_OK) {
        RegValue = 0x9; //Todo: There needs to be a reasonable explanation
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_RGMII_TX_DELAY_REG, RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    /* Set Rx Delay */
    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_RGMII_RX_DELAY_REG, &RegValue);
    if (Ret == AL_OK) {
        RegValue |= PHY_RGMII_RX_DELAY_MASK;
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_RGMII_RX_DELAY_REG, RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    /* Restore to default page 0 */
    Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_PAGE_SELECT_REG, 0x0);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    if (Gbe->MacDmaConfig.Speed == AL_GBE_SPEED_AUTONEG) {
        /* Set the auto-negotiation. */
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_AUTONEG_ADVERTISE_REG,
                                PHY_100BASETX_FULLDUPLEX_MASK | PHY_100BASETX_HALFDUPLEX_MASK |
                                PHY_10BASETX_FULLDUPLEX_MASK | PHY_10BASETX_HALFDUPLEX_MASK |
                                PHY_IEEE802_3_SELECTOR_MASK);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_1000BASET_CONTROL_REG, PHY_1000BASET_FULLDUPLEX_MASK);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

    } else {
        RegValue = 0;
        TmpRegValue = 0;
        RegValue |= (PHY_AUTONEG_ADVERTISE_REG | PHY_IEEE802_3_SELECTOR_MASK);

        if (Gbe->MacDmaConfig.Speed == AL_GBE_SPEED_100M) {
            RegValue |= (PHY_100BASETX_FULLDUPLEX_MASK | PHY_100BASETX_HALFDUPLEX_MASK);

        } else if (Gbe->MacDmaConfig.Speed == AL_GBE_SPEED_10M) {
            RegValue |= (PHY_10BASETX_FULLDUPLEX_MASK | PHY_10BASETX_HALFDUPLEX_MASK);

        } else {
            TmpRegValue |= PHY_1000BASET_FULLDUPLEX_MASK;

        }

        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_AUTONEG_ADVERTISE_REG, RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_1000BASET_CONTROL_REG, TmpRegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }
    }

    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BCR_REG, &RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }
    Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_BCR_REG,
                             (RegValue | PHY_BCTL_AUTONEG_MASK | PHY_BCTL_RESTART_AUTONEG_MASK));
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    AL_U64 StartTick = AlSys_GetTimerTickCount();
    AL_U64 CurrentTick = 0;

    do {
        CurrentTick = AlSys_GetTimerTickCount();
        if (((CurrentTick - StartTick) * 1000) / AlSys_GetTimerFreq() > AL_GBE_PHY_READ_TIMEOUT_MS) {
            return AL_GBE_ERR_TIMEOUT;
        }

        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BSR_REG, &RegValue);
    } while ((Ret != AL_OK) || (!(RegValue & PHY_BSTATUS_AUTONEGCOMP_MASK)));


    return AL_OK;
}

static AL_S32 AlPhy_RTL8211fGetLinkStatus(AL_PHY_DevStruct *Phy, AL_BOOL *Status)
{
    AL_S32 Ret;
    AL_GBE_DevStruct *Gbe = Phy->DevObj;
    AL_U8 PHYAddress = Phy->PhyAddr;
    AL_U16 RegValue;

    do {
        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_SSR_REG, &RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        }

            //Todo: Timeout
    } while(((PHY_SSTATUS_LINKSTATUS_MASK & RegValue) == 0));

    *Status = AL_TRUE;

    return AL_OK;
}


static AL_S32 AlPhy_RTL8211fGetLinkSpeedDuplex(AL_PHY_DevStruct *Phy, AL_U8 *Speed, AL_U8 *Duplex)
{
    AL_S32 Ret;
    AL_GBE_DevStruct *Gbe = Phy->DevObj;
    AL_U8 PHYAddress = Phy->PhyAddr;
    AL_U16 RegValue;

    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_SSR_REG, &RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }

     switch ((RegValue & PHY_SSTATUS_LINKSPEED_MASK) >> PHY_SSTATUS_LINKSPEED_SHIFT)
     {
     case (AL_U32)PHY_SPEED_10M:
         *Speed = PHY_SPEED_10M;
         break;
     case (AL_U32)PHY_SPEED_100M:
        *Speed = PHY_SPEED_100M;
         break;
     case (AL_U32)PHY_SPEED_1000M:
         *Speed = PHY_SPEED_1000M;
         break;
     default:
         *Speed = PHY_SPEED_10M;
         break;
     }

     if ((RegValue & PHY_SSTATUS_LINKDUPLEX_MASK) != 0U) {
         *Duplex = PHY_FULL_DUPLEX;
     } else {
         *Duplex = PHY_HALF_DUPLEX;
     }

     return AL_OK;
}

struct AL_PHY_DevStruct PhyRTL8211f = {
    .PhyName = "RTL8211F",
    .PhyId = PHY_ID_RTL8211F,
    .Init = AlPhy_RTL8211fInit,
    .GetLinkStatus = AlPhy_RTL8211fGetLinkStatus,
    .GetLinkSpeedDuplex = AlPhy_RTL8211fGetLinkSpeedDuplex,
};