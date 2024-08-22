/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gbe_hal.h"
#include "al_gbe_phy.h"

#define PHY_SSR_REG                         0x11

#define PHY_BCTL_RESET_MASK                 0x8000
#define PHY_BCTL_AUTONEG_MASK               0x1000
#define PHY_BCTL_RESTART_AUTONEG_MASK       0x0200

#define PHY_BSTATUS_AUTONEGCOMP_MASK        0x0020

#define PHY_ASYMMETRIC_PAUSE_MASK           0x0800
#define PHY_PAUSE_MASK                      0x0400

#define PHY_ADVERTISE_10HALF_MASK           0x0020  /* Try for 10mbps half-duplex  */
#define PHY_ADVERTISE_10FULL_MASK           0x0040  /* Try for 10mbps full-duplex  */
#define PHY_ADVERTISE_100HALF_MASK          0x0080  /* Try for 100mbps half-duplex */
#define PHY_ADVERTISE_100FULL_MASK          0x0100  /* Try for 100mbps full-duplex */
#define PHY_ADVERTISE_100BASE4_MASK         0x0200  /* Try for 100mbps 4k packets  */

#define PHY_1000BASET_FULLDUPLEX_MASK       0x0200
#define PHY_1000BASET_HALFDUPLEX_MASK       0x0100

#define PHY_ADVERTISE_100_AND_10_MASK       (PHY_ADVERTISE_10FULL_MASK | PHY_ADVERTISE_100FULL_MASK | \
                                             PHY_ADVERTISE_10HALF_MASK | PHY_ADVERTISE_100HALF_MASK)
#define PHY_ADVERTISE_100_MASK              (PHY_ADVERTISE_100FULL_MASK | PHY_ADVERTISE_100HALF_MASK)
#define PHY_ADVERTISE_10_MASK               (PHY_ADVERTISE_10FULL_MASK | PHY_ADVERTISE_10HALF_MASK)

#define PHY_ADVERTISE_1000_MASK             (PHY_1000BASET_FULLDUPLEX_MASK | PHY_1000BASET_HALFDUPLEX_MASK)

static AL_S32 AlPhy_YT8531Init(AL_PHY_DevStruct *Phy)
{
    AL_S32 Ret = 0;
    AL_U16 RegValue = 0;
    AL_U16 TmpRegValue = 0;

    AL_GBE_DevStruct *Gbe = Phy->DevObj;
    AL_U8 PHYAddress = Phy->PhyAddr;

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

    if (Gbe->MacDmaConfig.Speed == AL_GBE_SPEED_AUTONEG) {
        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_AUTONEG_ADVERTISE_REG, &RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        RegValue |= PHY_ASYMMETRIC_PAUSE_MASK;
        RegValue |= PHY_PAUSE_MASK;
        RegValue |= PHY_ADVERTISE_100_MASK;
        RegValue |= PHY_ADVERTISE_10_MASK;
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_AUTONEG_ADVERTISE_REG, RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_1000BASET_CONTROL_REG, &RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        } 

        RegValue |= PHY_ADVERTISE_1000_MASK;
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_1000BASET_CONTROL_REG, RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

    } else {

        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_AUTONEG_ADVERTISE_REG, &RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        }
        RegValue |= PHY_ASYMMETRIC_PAUSE_MASK;
        RegValue |= PHY_PAUSE_MASK;

        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_1000BASET_CONTROL_REG, &TmpRegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        if (Gbe->MacDmaConfig.Speed == AL_GBE_SPEED_100M) {
            RegValue |= PHY_ADVERTISE_100_MASK;
            RegValue &= ~(PHY_ADVERTISE_10_MASK);

            TmpRegValue &= ~(PHY_ADVERTISE_1000_MASK);
        } else if (Gbe->MacDmaConfig.Speed == AL_GBE_SPEED_10M) {
            RegValue &= ~(PHY_ADVERTISE_100_MASK);
            RegValue |= PHY_ADVERTISE_10_MASK;

            TmpRegValue &= ~(PHY_ADVERTISE_1000_MASK);
        } else {
            RegValue &= ~(PHY_ADVERTISE_100_MASK);
            RegValue &= ~(PHY_ADVERTISE_10_MASK);

            TmpRegValue |= PHY_ADVERTISE_1000_MASK;
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
    RegValue |= PHY_BCTL_AUTONEG_MASK;
    RegValue |= PHY_BCTL_RESTART_AUTONEG_MASK;
    Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_BCR_REG, RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BCR_REG, &RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    RegValue |= PHY_BCTL_RESET_MASK;
    Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_BCR_REG, RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    while (1) {
        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BCR_REG, &RegValue);
        if ((Ret != AL_OK) || (RegValue & PHY_BCTL_RESET_MASK))
            continue;
        else
         break;
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

static AL_S32 AlPhy_Yt8531GetLinkStatus(AL_PHY_DevStruct *Phy, AL_BOOL *Status)
{
    AL_GBE_DevStruct *Gbe = Phy->DevObj;
    AL_U8 PHYAddress = Phy->PhyAddr;
    AL_U16 RegValue;

    AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_SSR_REG, &RegValue);

    RegValue = RegValue & 0x400;

    if (RegValue) {
        *Status = AL_TRUE;
    } else {
        *Status = AL_FALSE;
    }

    return AL_OK;
}

static AL_S32 AlPhy_Yt8531GetLinkSpeedDuplex(AL_PHY_DevStruct *Phy, AL_U8 *Speed, AL_U8 *Duplex)
{
    AL_GBE_DevStruct *Gbe = Phy->DevObj;
    AL_U8 PHYAddress = Phy->PhyAddr;
    AL_U16 RegValue;

     AL_U16 TempSpeed;

     AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_SSR_REG, &RegValue);
     TempSpeed = RegValue >> 14;

     switch (TempSpeed)
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

     if ((RegValue & 0x2000) != 0U) {
         *Duplex = PHY_FULL_DUPLEX;
     } else {
         *Duplex = PHY_HALF_DUPLEX;
     }

    return AL_OK;
}

struct AL_PHY_DevStruct PhyYt8531dh = {
    .PhyName = "YT8531DH",
    .PhyId = PHY_ID_YT8531,
    .Init = AlPhy_YT8531Init,
    .GetLinkStatus = AlPhy_Yt8531GetLinkStatus,
    .GetLinkSpeedDuplex = AlPhy_Yt8531GetLinkSpeedDuplex,
};