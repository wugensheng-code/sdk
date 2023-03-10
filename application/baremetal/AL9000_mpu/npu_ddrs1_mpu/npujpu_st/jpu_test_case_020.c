/*
* @Author: Tianyi Wang
* @Date:   2022-03-23 15:21:34
* @Last Modified by:   Tianyi Wang
* @Last Modified time: 2022-04-22 12:33:27
*/

#include "jpu.h"
//#include "io.h"
//#include "gic.h"
//#include "config.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "npu.h"


#define TUBE_ADDRESS ((volatile char *) 0xF8800010u)
void SUCCESS()
{
    printf("TEST SUCCESS\n");
}

void FAIL()
{
    printf("TEST FAIL\n");
}

void jpu_test_case_020(void)
{
    uint32_t midr;
    uint64_t cpunum = 0;
    volatile unsigned rdata, addr;
    volatile unsigned fail_flag ;
    if (cpunum == 0) {
    // Ensure all writes to system registers have taken place
    #ifdef _AARCH_64
    asm volatile("dsb     sy");
    asm volatile("isb     sy");
    asm volatile("dmb     sy");
    #endif // MPU_APU_RUN

// #if 1
    if(1)
    {
        fail_flag = 0;
        //revise cfg_ctrl_jpu in top register to set addr_32 to 1.
        // mem_write(0xF88001b4,0x00000001);

        mem_write(JPU__MJPEG_PIC_START_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_PIC_STATUS_REG_1__ADDR,0x00000000);
        rdata = mem_read(JPU__MJPEG_INST_CTRL_START_REG__ADDR);
        mem_write(JPU__MJPEG_INST_CTRL_START_REG__ADDR,0x00000004);
        rdata = mem_read(JPU__MJPEG_INST_CTRL_STATUS_REG__ADDR);
        mem_write(JPU__MJPEG_INST_CTRL_START_REG__ADDR,0x00000000);
        mem_write(JPU__MJPEG_INST_CTRL_START_REG__ADDR,0x00000001);
        rdata = mem_read(JPU__MJPEG_VERSION_INFO_REG_1__ADDR);
        mem_write(JPU__MJPEG_INTR_MASK_REG_1__ADDR,0x000003f8);
        mem_write(JPU__MJPEG_SLICE_INFO_REG_1__ADDR,0x000001a0);
        mem_write(JPU__MJPEG_SLICE_DPB_POS_REG_1__ADDR,0x000001a0);
        mem_write(JPU__MJPEG_SLICE_ENC_POS_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_PIC_SETMB_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_BBC_WR_PTR_REG_1__ADDR,0x000151a0);

        mem_write(JPU__MJPEG_BBC_ENDADDR_REG_1__ADDR,0x00015200);
        mem_write(JPU__MJPEG_BBC_BAS_ADDR_REG_1__ADDR,0x00000000);

        // mem_write(JPU__MJPEG_BBC_ENDADDR_REG_1__ADDR,0xf0002800);
        // mem_write(JPU__MJPEG_BBC_BAS_ADDR_REG_1__ADDR,0xf0000000);

        mem_write(JPU__MJPEG_GBU_TT_CNT_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_GBU_TT_CNT_P4_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_PIC_ERRMB_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_PIC_CTRL_REG_1__ADDR,0x00006180);
        mem_write(JPU__MJPEG_PIC_SIZE_REG_1__ADDR,0x01a001a0);
        mem_write(JPU__MJPEG_OP_INFO_REG_1__ADDR,0x00000004);
        mem_write(JPU__MJPEG_MCU_INFO_REG_1__ADDR,0x000cd255);
        mem_write(JPU__MJPEG_DPB_CONFIG_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_RST_INTERVAL_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_SCL_INFO_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_QMAT_CTRL_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000001);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000001);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000001);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000001);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000005);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000006);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000004);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000004);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000005);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000007);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000006);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000007);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000007);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000007);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000006);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000007);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000007);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000008);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000009);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000b);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000009);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000008);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000008);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000a);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000008);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000007);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000007);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000a);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000d);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000a);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000a);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000b);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000007);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000009);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000e);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000f);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000d);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000e);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000b);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_CTRL_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_QMAT_CTRL_REG_1__ADDR,0x00000043);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000006);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000006);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000008);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000007);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000008);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_CTRL_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_QMAT_CTRL_REG_1__ADDR,0x00000083);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000002);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000006);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000006);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000008);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000007);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x00000008);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_DATA_REG_1__ADDR,0x0000000c);
        mem_write(JPU__MJPEG_QMAT_CTRL_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_BBC_CUR_POS_REG_1__ADDR,0x00000023);

        mem_write(JPU__MJPEG_BBC_EXT_ADDR_REG_1__ADDR,0x00002300);
        mem_write(JPU__MJPEG_BBC_INT_ADDR_REG_1__ADDR,0x00000040);

        mem_write(JPU__MJPEG_BBC_DATA_CNT_REG_1__ADDR,0x00000040);
        mem_write(JPU__MJPEG_BBC_COMMAND_REG_1__ADDR,0x00000000);

        //while read 224 to get 0
        //mem_read(0xF8460224,0x00000000);

        rdata = mem_read(JPU__MJPEG_BBC_BUSY_REG_1__ADDR);
        while (rdata != 0x0)
        {
            for (int i=0; i<100; i++){
                //end_cnt++;
                #ifdef _AARCH_64
                asm volatile("dsb     sy");
                asm volatile("isb     sy");
                asm volatile("dmb     sy");
                #endif
            }
            rdata = mem_read(JPU__MJPEG_BBC_BUSY_REG_1__ADDR);
        
        }

        mem_write(JPU__MJPEG_BBC_CUR_POS_REG_1__ADDR,0x00000024);
        mem_write(JPU__MJPEG_BBC_EXT_ADDR_REG_1__ADDR,0x00002400);
        mem_write(JPU__MJPEG_BBC_INT_ADDR_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_BBC_DATA_CNT_REG_1__ADDR,0x00000040);
        mem_write(JPU__MJPEG_BBC_COMMAND_REG_1__ADDR,0x00000000);

        rdata = mem_read(JPU__MJPEG_BBC_BUSY_REG_1__ADDR);
        while (rdata != 0x0)
        {
            for (int i=0; i<100; i++){
                //end_cnt++;
                #ifdef _AARCH_64
                asm volatile("dsb     sy");
                asm volatile("isb     sy");
                asm volatile("dmb     sy");
                #endif
            }
            rdata = mem_read(JPU__MJPEG_BBC_BUSY_REG_1__ADDR);
        }

        mem_write(JPU__MJPEG_BBC_CUR_POS_REG_1__ADDR,0x00000025);
        mem_write(JPU__MJPEG_BBC_CTRL_REG_1__ADDR,0x00000001);
        mem_write(JPU__MJPEG_GBU_WD_PTR_REG_1__ADDR,0x00000040);
        mem_write(JPU__MJPEG_GBU_BBSR_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_GBU_BBER_REG_1__ADDR,0x0000007f);
        mem_write(JPU__MJPEG_GBU_BBIR_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_GBU_BBHR_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_GBU_CTRL_REG_1__ADDR,0x00000004);
        mem_write(JPU__MJPEG_GBU_FF_RPTR_REG_1__ADDR,0x00000008);
        rdata = mem_read(JPU__MJPEG_BBC_STRM_CTRL_REG_1__ADDR);
        mem_write(JPU__MJPEG_BBC_STRM_CTRL_REG_1__ADDR,0x80000152);
        mem_write(JPU__MJPEG_RST_INDEX_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_RST_COUNT_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_DPCM_DIFF_Y_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_DPCM_DIFF_CB_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_DPCM_DIFF_CR_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_GBU_FF_RPTR_REG_1__ADDR,0x00000008);
        mem_write(JPU__MJPEG_GBU_CTRL_REG_1__ADDR,0x00000003);
        mem_write(JPU__MJPEG_ROT_INFO_REG_1__ADDR,0x00000000);

        mem_write(JPU__MJPEG_DPB_BASE00_REG_1__ADDR,0x00500000);
        mem_write(JPU__MJPEG_DPB_BASE01_REG_1__ADDR,0x0052a400);
        mem_write(JPU__MJPEG_DPB_BASE02_REG_1__ADDR,0x00534d00);
        mem_write(JPU__MJPEG_DPB_YSTRIDE_REG_1__ADDR,0x000001a0);
        mem_write(JPU__MJPEG_DPB_CSTRIDE_REG_1__ADDR,0x000000d0);


        mem_write(JPU__MJPEG_CLP_INFO_REG_1__ADDR,0x00000000);
        mem_write(JPU__MJPEG_PIC_STATUS_REG_1__ADDR,0x000003ff);
        mem_write(JPU__MJPEG_PIC_START_REG_1__ADDR,0x00000010);

        #ifdef _AARCH_64
        asm volatile("dsb     sy");
        asm volatile("isb     sy");
        asm volatile("dmb     sy");
        #endif
        }
    
        rdata = mem_read(JPU__MJPEG_PIC_STATUS_REG_1__ADDR);
        while (rdata != 0x1)
        {
            for (int i=0; i<100; i++){
                //end_cnt++;
                #ifdef _AARCH_64
                asm volatile("dsb     sy");
                asm volatile("isb     sy");
                asm volatile("dmb     sy");
                #endif
            }
            rdata = mem_read(JPU__MJPEG_PIC_STATUS_REG_1__ADDR);
        
        }

        mem_write(JPU__MJPEG_PIC_STATUS_REG_1__ADDR,0x00000001);
//mem_read(00c8, 0x00000000);
        mem_write(JPU__MJPEG_PIC_START_REG_1__ADDR,0x00000000);

 //mem_read(0c04, 0x00000000);
        mem_write(JPU__MJPEG_INST_CTRL_STATUS_REG__ADDR,0x00000050);
        mem_write(JPU__MJPEG_INST_CTRL_START_REG__ADDR,0x00000000);


        if (fail_flag != 1) {
            // vfwp("** REG SCAN SUCCESS CPU**%x",0);
            SUCCESS();
        }
        else{
            FAIL();
        }
// #endif
    }
}

