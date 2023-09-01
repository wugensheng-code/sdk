// #include "ee_printf.h"
#include "al9000_ddrc.h"

u32 ftcHandle = 0;

u32 al9000_ddr_reg_write(u32* useless, u32 addr, u32 data)
{
    *(volatile u32*)(addr) = data;
    // printf("[WR] *0x%08x <= 0x%08x\r\n", addr, data);
    return 0;
}

u32 al9000_ddr_reg_read(u32* useless, u32 addr, u32* data)
{
    *data = *(volatile u32*)(addr);
    // printf("[RD] *0x%08x == 0x%08x\r\n", addr, data);
    return 0;
}

void al9000_reg_write(u16 addr, u32 data)
{
    u32 ftcStatus;
    unsigned int addr_phy = 0xf8420000 | addr;
    ftcStatus = al9000_ddr_reg_write(&ftcHandle, addr_phy, data);

    #ifdef DEBUG_REG_self
        printf("write : 0x%08x = 0x%08x\r\n", addr, data);
    #endif

}
u32  al9000_reg_read(u16 addr)
{
    u32 ftcStatus;
    unsigned int reg_data = 0;
    unsigned int addr_phy = 0xf8420000 | addr;
    ftcStatus = al9000_ddr_reg_read(&ftcHandle, addr_phy, &reg_data);

    return reg_data;
}


void al9000_reg_update( u16 addr, u32 oldData, u32 newData )
{
    if (oldData != newData) {
        al9000_reg_write(addr, newData);
    }
}

void al9000_field_write(u16 addr, u8 offset, u32 mask, u32 data)
{
    u32 RegData;
    u32 RegDataNew;
    u32 dataMask ;

    dataMask = (data << offset) & mask ;

    RegData = al9000_reg_read(addr);

    RegDataNew = (RegData & (~mask)) | dataMask ;

    al9000_reg_write(addr, RegDataNew);
}

u32 al9000_field_read(u16 addr, u8 offset, u32 mask)
{
    u32 data ;
    u32 RegData ;

    RegData = al9000_reg_read(addr);
    data = (RegData & mask) >> offset ;

    return data ;
}

u32 al9000_field_set(u16 addr, u8 offset, u32 mask, u32 data, u32 oldData)
{
    u32 dataMask ;
    u32 newData  ;

    dataMask = (data << offset) & mask ;

    newData = (oldData & (~mask)) | dataMask ;

    return newData ;
}

int al9000_field_wait(u16 addr, u8 offset, u32 mask, u32 expect, u32 timeout)
{
    u8  exitFlag ;
    u32 count ;

    u32 curval ;

    count = 0 ;
    exitFlag = 1;
    while(exitFlag) {
        curval = al9000_field_read(addr, offset, mask);
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

