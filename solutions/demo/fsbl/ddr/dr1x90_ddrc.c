/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// #include "ee_printf.h"
#include "dr1x90_ddrc.h"

u32 ftcHandle = 0;

u32 dr1x90_ddr_reg_write(u32* useless, u32 addr, u32 data)
{
    *(volatile u32*)(addr) = data;
    // printf("[WR] *0x%08x <= 0x%08x\r\n", addr, data);
    return 0;
}

u32 dr1x90_ddr_reg_read(u32* useless, u32 addr, u32* data)
{
    *data = *(volatile u32*)(addr);
    // printf("[RD] *0x%08x == 0x%08x\r\n", addr, data);
    return 0;
}

void dr1x90_reg_write(u16 addr, u32 data)
{
    u32 ftcStatus;
    unsigned int addr_phy = 0xf8420000 | addr;
    ftcStatus = dr1x90_ddr_reg_write(&ftcHandle, addr_phy, data);

    #ifdef DEBUG_REG_self
        printf("write : 0x%08x = 0x%08x\r\n", addr, data);
    #endif

}
u32  dr1x90_reg_read(u16 addr)
{
    u32 ftcStatus;
    unsigned int reg_data = 0;
    unsigned int addr_phy = 0xf8420000 | addr;
    ftcStatus = dr1x90_ddr_reg_read(&ftcHandle, addr_phy, &reg_data);

    return reg_data;
}


void dr1x90_reg_update( u16 addr, u32 oldData, u32 newData )
{
    if (oldData != newData) {
        dr1x90_reg_write(addr, newData);
    }
}

void dr1x90_field_write(u16 addr, u8 offset, u32 mask, u32 data)
{
    u32 RegData;
    u32 RegDataNew;
    u32 dataMask ;

    dataMask = (data << offset) & mask ;

    RegData = dr1x90_reg_read(addr);

    RegDataNew = (RegData & (~mask)) | dataMask ;

    dr1x90_reg_write(addr, RegDataNew);
}

u32 dr1x90_field_read(u16 addr, u8 offset, u32 mask)
{
    u32 data ;
    u32 RegData ;

    RegData = dr1x90_reg_read(addr);
    data = (RegData & mask) >> offset ;

    return data ;
}

u32 dr1x90_field_set(u16 addr, u8 offset, u32 mask, u32 data, u32 oldData)
{
    u32 dataMask ;
    u32 newData  ;

    dataMask = (data << offset) & mask ;

    newData = (oldData & (~mask)) | dataMask ;

    return newData ;
}

int dr1x90_field_wait(u16 addr, u8 offset, u32 mask, u32 expect, u32 timeout)
{
    u8  exitFlag ;
    u32 count ;

    u32 curval ;

    count = 0 ;
    exitFlag = 1;
    while(exitFlag) {
        curval = dr1x90_field_read(addr, offset, mask);
        if (curval == expect) {
            exitFlag = 0;
        }

        if (count == timeout) {
            exitFlag = 0;  // if timeout occurs
        }

        count++;
    }

    if (count >= timeout) return -1;
    else return 0 ; // if success
}

