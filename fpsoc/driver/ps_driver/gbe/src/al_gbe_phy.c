/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gbe_hal.h"
#include "al_gbe_phy.h"

#define AL_GBE_SUPPORT_PHY_NUM      2

static const AL_GBE_PhyInfoStruct AL_GBE_PhyInfoList[AL_GBE_SUPPORT_PHY_NUM] = {
    {
        .PhyName = "RTL8211F",
        .PhyID = PHY_ID_RTL8211F,
    },

    {
        .PhyName = "YT8531DH",
        .PhyID = PHY_ID_YT8531,
    },
};

/**
 *
 * This function writes a specified value to a PHY register. It checks if the GMII interface is
 * busy before proceeding with the write operation. It prepares the MDIO Address Register value,
 * writes the value to the MII data register, and then writes the result into the MII Address
 * register. It waits for the GMII interface to become available before completing.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param PHYAddr The PHY device address.
 * @param PHYReg The PHY register address.
 * @param RegValue The value to write to the PHY register.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_WritePhyRegister(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddr, AL_U32 PHYReg, AL_U16 RegValue)
{
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);
    AL_U32 TmpReg;

    /* Check for the Busy flag */
    if (AlGbe_ll_IsGmiiBusy(GbeBaseAddr)) {
        return AL_GBE_ERR_PHY_BUSY;
    }

    /* Get the  MACMDIOAR value */
    TmpReg = AlGbe_ll_ReadMdioAddr(GbeBaseAddr);

    /*
       Prepare the MDIO Address Register value
     - Set the PHY device address
     - Set the PHY register address
     - Set the write mode
     - Set the MII Busy bit
    */
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__PA__SHIFT, GBE__MAC_MDIO_ADDRESS__PA__SIZE, PHYAddr);
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__RDA__SHIFT, GBE__MAC_MDIO_ADDRESS__RDA__SIZE, PHYReg);
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__GOC_0__SHIFT, GBE__MAC_MDIO_ADDRESS__GOC__SIZE, AL_GBE_GMII_CMD_WRITE);
    AL_REG32_SET_BIT(&TmpReg, GBE__MAC_MDIO_ADDRESS__GB__SHIFT, AL_GBE_FUNC_ENABLE);

    /* Give the value to the MII data register */
    AlGbe_ll_WriteMdioGmiiData(GbeBaseAddr, (AL_U16)RegValue);

    /* Write the result value into the MII Address register */
    AlGbe_ll_WriteMdioAddr(GbeBaseAddr, TmpReg);

    /* Wait for the Busy flag */
    while (AlGbe_ll_IsGmiiBusy(GbeBaseAddr)) {
        //ToDo: TimeOut
    }

  return AL_OK;
}

/**
 *
 * This function reads a value from a specified PHY register. It checks if the GMII interface is
 * busy before proceeding with the read operation. It prepares the MDIO Address Register value,
 * writes the result into the MII Address register, and waits for the GMII interface to become
 * available. It then reads the value from the MII data register.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param PHYAddr The PHY device address.
 * @param PHYReg The PHY register address.
 * @param RegValue Pointer to store the read value from the PHY register.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_ReadPhyRegister(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddr, AL_U32 PHYReg, AL_U16 *RegValue)
{
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);
    AL_U32 TmpReg;

    /* Check for the Busy flag */
    if (AlGbe_ll_IsGmiiBusy(GbeBaseAddr)) {
        return AL_GBE_ERR_PHY_BUSY;
    }

    /* Get the  MACMDIOAR value */
    TmpReg = AlGbe_ll_ReadMdioAddr(GbeBaseAddr);

    /*
       Prepare the MDIO Address Register value
     - Set the PHY device address
     - Set the PHY register address
     - Set the read mode
     - Set the MII Busy bit
    */
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__PA__SHIFT, GBE__MAC_MDIO_ADDRESS__PA__SIZE, PHYAddr);
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__RDA__SHIFT, GBE__MAC_MDIO_ADDRESS__RDA__SIZE, PHYReg);
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__GOC_0__SHIFT, GBE__MAC_MDIO_ADDRESS__GOC__SIZE, AL_GBE_GMII_CMD_READ);
    AL_REG32_SET_BIT(&TmpReg, GBE__MAC_MDIO_ADDRESS__GB__SHIFT, AL_GBE_FUNC_ENABLE);

    /* Write the result value into the MII Address register */
    AlGbe_ll_WriteMdioAddr(GbeBaseAddr, TmpReg);

    /* Wait for the Busy flag */
    while (AlGbe_ll_IsGmiiBusy(GbeBaseAddr)) {
        //ToDo: TimeOut
    }

    /* Get MACMIIDR value */
    *RegValue = (AL_U16)AlGbe_ll_ReadMdioGmiiData(GbeBaseAddr);

    return AL_OK;
}

AL_S32 AlGbe_Dev_PhyRTL8211Init(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddress)
{
    AL_S32 Ret = 0;
    AL_U16 RegValue;
    AL_U16 TmpRegValue;

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

AL_S32 AlGbe_Dev_PhyYT8531Init(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddress)
{
    AL_S32 Ret = 0;
    AL_U16 RegValue = 0;
    AL_U16 TmpRegValue = 0;

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
        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_AUTONEGO_ADVERTISE_REG, &RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        RegValue |= IEEE_ASYMMETRIC_PAUSE_MASK;
        RegValue |= IEEE_PAUSE_MASK;
        RegValue |= ADVERTISE_100;
        RegValue |= ADVERTISE_10;
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, IEEE_AUTONEGO_ADVERTISE_REG, RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_1000_ADVERTISE_REG_OFFSET, &RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        } 

        RegValue |= ADVERTISE_1000;
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, IEEE_1000_ADVERTISE_REG_OFFSET, RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

    } else {

        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_AUTONEGO_ADVERTISE_REG, &RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        }
        RegValue |= IEEE_ASYMMETRIC_PAUSE_MASK;
        RegValue |= IEEE_PAUSE_MASK;

        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_1000_ADVERTISE_REG_OFFSET, &TmpRegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        if (Gbe->MacDmaConfig.Speed == AL_GBE_SPEED_100M) {
            RegValue |= ADVERTISE_100;
            RegValue &= ~(ADVERTISE_10);

            TmpRegValue &= ~(ADVERTISE_1000);
        } else if (Gbe->MacDmaConfig.Speed == AL_GBE_SPEED_10M) {
            RegValue &= ~(ADVERTISE_100);
            RegValue |= ADVERTISE_10;

            TmpRegValue &= ~(ADVERTISE_1000);
        } else {
            RegValue &= ~(ADVERTISE_100);
            RegValue &= ~(ADVERTISE_10);

            TmpRegValue |= ADVERTISE_1000;
        }

        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, IEEE_AUTONEGO_ADVERTISE_REG, RegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, IEEE_1000_ADVERTISE_REG_OFFSET, TmpRegValue);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }
    }

    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_CONTROL_REG_OFFSET, &RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }
    RegValue |= IEEE_CTRL_AUTONEGOTIATE_ENABLE;
    RegValue |= IEEE_STAT_AUTONEGOTIATE_RESTART;
    Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, IEEE_CONTROL_REG_OFFSET, RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_CONTROL_REG_OFFSET, &RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    RegValue |= IEEE_CTRL_RESET_MASK;
    Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, IEEE_CONTROL_REG_OFFSET, RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    while (1) {
        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_CONTROL_REG_OFFSET, &RegValue);
        if ((Ret != AL_OK) || (RegValue & IEEE_CTRL_RESET_MASK))
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

        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_STATUS_REG_OFFSET, &RegValue);
    } while ((Ret != AL_OK) || (!(RegValue & IEEE_STAT_AUTONEGOTIATE_COMPLETE)));

    return AL_OK;
}

/**
 * This function performs several operations to initialize the PHY device, including reading the PHY ID,
 * resetting the PHY, configuring RGMII delays, setting up auto-negotiation, and enabling or disabling
 * 1000BaseT full duplex mode based on the MAC DMA configuration. It handles different PHY models by checking
 * the PHY ID and applies specific initialization sequences for each. The function also ensures that the PHY
 * is reset properly and waits for auto-negotiation to complete if applicable.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param PHYAddress The address of the PHY device to initialize.
 * @return AL_OK on success, or an error code on failure.
 *
 * @retval AL_GBE_ERR_ILLEGAL_PARAM Indicates an illegal parameter error.
 * @retval AL_GBE_ERR_PHY_RESET_FAILED Indicates that resetting the PHY device failed.
 */
AL_S32 AlGbe_Dev_PhyInit(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddress)
{
    AL_S32 Ret = AL_OK;
    AL_U32 Index = 0;
    AL_U16 PhyId1, PhyId2;

    AL_ASSERT((Gbe != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_ID1_REG, &PhyId1);
    if (Ret != AL_OK ) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return AL_GBE_ERR_PHY_RESET_FAILED;
    }

    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_ID2_REG, &PhyId2);
    if (Ret != AL_OK ) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return AL_GBE_ERR_PHY_RESET_FAILED;
    }

    Gbe->PhyId = ((PhyId1 & 0xffff) << 16) | (PhyId2 & 0xffff);
    for (Index = 0; Index < AL_GBE_SUPPORT_PHY_NUM; Index++) {
        if (AL_GBE_PhyInfoList[Index].PhyID == Gbe->PhyId) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Phy detected, Phy is %s\r\n", AL_GBE_PhyInfoList[Index].PhyName);
            break;
        }
    }
    if (Index == AL_GBE_SUPPORT_PHY_NUM) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Unsupport PHY, Phy ID:%x\r\n", Gbe->PhyId);
        return AL_GBE_ERR_PHY_UNSUPPORT;
    }

    if (Gbe->PhyId == PHY_ID_RTL8211F) {
        Ret = AlGbe_Dev_PhyRTL8211Init(Gbe, PHYAddress);
    } else if (Gbe->PhyId == PHY_ID_YT8531) {
        Ret = AlGbe_Dev_PhyYT8531Init(Gbe, PHYAddress);
    }

    return Ret;
}

/**
 * This function queries the PHY link status, including speed and duplex mode, for a specified PHY address.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param PHYAddress The address of the PHY to query.
 * @param Speed Pointer to a variable where the link speed will be stored.
 * @param Duplex Pointer to a variable where the duplex mode will be stored.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlGbe_Dev_GetPhyLinkStatus(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddress, AL_U8 *Speed, AL_U8 *Duplex)
{
    AL_S32 Ret;
    AL_U16 RegValue;

    AL_ASSERT((Gbe != AL_NULL) && (Speed != AL_NULL) && (Duplex != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    if (Gbe->PhyId == PHY_ID_RTL8211F) {
        do {
            Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_SSR_REG, &RegValue);
            if (Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
                return Ret;
            }

            //Todo: Timeout
        } while(((PHY_SSTATUS_LINKSTATUS_MASK & RegValue) == 0));

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
    } else if (Gbe->PhyId == PHY_ID_YT8531) {
        AL_U16 TempSpeed;

        AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress,0X11,&RegValue);
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
    }

    return AL_OK;
}

/**
 * Initializes the PHY.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param PhyAddr PHY address.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_PhyInit(AL_GBE_HalStruct *Handle, AL_U8 PhyAddr)
{
    AL_S32 Ret;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    Ret = AlGbe_Dev_PhyInit(&Handle->Dev, PhyAddr);

    return Ret;
}

/**
 * Gets the PHY link status.
 *
 * @param Handle Pointer to the GBE HAL structure.
 * @param PhyAddr PHY address.
 * @param Speed Pointer to store the speed.
 * @param Duplex Pointer to store the duplex mode.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Hal_GetPhyLinkStatus(AL_GBE_HalStruct *Handle, AL_U32 PhyAddr, AL_U8 *Speed, AL_U8 *Duplex)
{
    AL_S32 Ret;

    AL_ASSERT((Handle != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    Ret = AlGbe_Dev_GetPhyLinkStatus(&Handle->Dev, PhyAddr, Speed, Duplex);

    return Ret;
}