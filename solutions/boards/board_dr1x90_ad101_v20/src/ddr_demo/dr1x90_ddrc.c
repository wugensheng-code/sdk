#include <stdio.h>
#include "al_barrier.h"
#include "dr1x90_ddrc.h"

void dr1x90_reg_write(u32 addr, u32 data)
{
    uintptr_t addr_phy = DDRC_ADDR_BASE + addr;
    *(volatile u32*)(addr_phy) = data;
    DSB();
}

u32 dr1x90_reg_read(u32 addr)
{
    uintptr_t addr_phy = DDRC_ADDR_BASE + addr;
    u32 data = *(volatile u32*)(addr_phy);
    DSB();
    return data;
}

void dr1x90_reg_update(u32 addr, u32 oldData, u32 newData)
{
    if (oldData != newData) {
        dr1x90_reg_write(addr, newData);
    }
}

void dr1x90_field_write(u32 addr, u32 offset, u32 mask, u32 data)
{
    u32 RegData;
    u32 RegDataNew;
    u32 dataMask ;

    dataMask = (data << offset) & mask ;

    RegData = dr1x90_reg_read(addr);

    RegDataNew = (RegData & (~mask)) | dataMask ;

    dr1x90_reg_write(addr, RegDataNew);
}

u32 dr1x90_field_read(u32 addr, u32 offset, u32 mask)
{
    u32 data ;
    u32 RegData ;

    RegData = dr1x90_reg_read(addr);
    data = (RegData & mask) >> offset ;

    return data ;
}

u32 dr1x90_field_set(u32 addr, u32 offset, u32 mask, u32 data, u32 oldData)
{
    u32 dataMask ;
    u32 newData  ;

    dataMask = (data << offset) & mask ;

    newData = (oldData & (~mask)) | dataMask ;

    return newData ;
}

int dr1x90_field_wait(u32 addr, u32 offset, u32 mask, u32 expect, u32 timeout)
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

void dr1x90_dram_write(uintptr_t addr, u32 data)
{
    *(volatile u32*)(addr) = data;
    DSB();
}

u32 dr1x90_dram_read(uintptr_t addr)
{
    u32 data = *(volatile u32*)(addr);
    DSB();
    return data;
}

void dr1x90_dram_field_write(uintptr_t addr, u32 offset, u32 mask, u32 data)
{
    u32 RegData;
    u32 RegDataNew;
    u32 dataMask;

    dataMask = (data << offset) & mask;

    RegData = dr1x90_dram_read(addr);

    RegDataNew = (RegData & (~mask)) | dataMask;

    dr1x90_dram_write(addr, RegDataNew);
}
