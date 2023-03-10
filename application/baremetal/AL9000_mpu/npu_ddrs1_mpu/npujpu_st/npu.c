/*
 * @Author: Chengsen Ma
 * @Date: 2022-04-20
 * @LastEditors: Guangxiang Rao
 * @LastEditTime: 2022-09-22 16:23:50
 * @Description: driver for npu
 * @FilePath: /master_sdk/SoC/demoapu/Common/Source/NPU/npu.c
 */

#include "npu.h"
#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include <io.h>

//#include "gic.h"

int done;
void alsoc_hardnpu_init(void) {
    //writel(NPU_ENABLE, ALSOC_NPU_ADDR(NPU_CTRL));
    //writel(NPU_OK, ALSOC_NPU_ADDR(NPU_STATUS));
    //gicv3_enable_irq(ALSOC_HARDNPU_IRQ);
}
void alsoc_softnpu_init(void) {
    //writel(NPU_ENABLE, ALSOC_NPU_ADDR(NPU_CTRL));
    //writel(NPU_OK, ALSOC_NPU_ADDR(NPU_STATUS));
    //gicv3_enable_irq(ALSOC_SOFTNPU_IRQ);
}

unsigned int alsoc_npu_test_reg(unsigned int addr , unsigned int data) {
    unsigned int res ;
    printf("npu w addr: %x\n",(addr));
    writel(data,ALSOC_NPU_ADDR(addr));
    printf("npu w addr: %x\n",(addr));
    res = readl(ALSOC_NPU_ADDR(addr));
    return res ;
}
void reg_write(unsigned int addr , unsigned int data){
    //printf("npu w addr: %x , npu w data: %x\n",(ALSOC_NPU_BASE+addr),(data));
    writel(data,ALSOC_NPU_BASE+addr);
    //printf("npu w addr: %x , npu w data: %x\n",(addr),(data));
    //unsigned int res ;
    //res = readl(ALSOC_NPU_BASE+addr);
    //printf("npu r addr: %x , npu r data: %x\n",(addr),(res));
}
void mem_write(unsigned int addr , unsigned int data){
    writel(data,addr);
    //printf("npu w addr: %x , npu w data: %x\n",(addr),(data));
}
unsigned int mem_read(unsigned int addr){
    unsigned int res ;
    res = readl(addr);
    //printf("npu r addr: %x , npu r data: %x\n",(addr),(res));
    return res ;
}
void mem_write_64(unsigned long long  addr , unsigned long long  data){
    writeq(data,addr);
    //printf("npu w addr: %x , npu w data: %x\n",(addr),(data));
}
unsigned long long  mem_read_64(unsigned long long addr){
    unsigned long long res ;
    res = readq(addr);
    //printf("npu r addr: %x , npu r data: %x\n",(addr),(res));
    return res ;
}

unsigned int reg_read(unsigned int addr){
    unsigned int res ;
    res = readl(ALSOC_NPU_BASE+addr);
    //printf("npu r addr: %x , npu r data: %x\n",(addr),(res));
    return res ;
}
void soft_reg_write(unsigned int addr , unsigned int data){
    //printf("npu w addr: %x , npu w data: %x\n",(addr),(data));
    writel(data,SOFT_NPU_BASE+addr);
    //printf("npu w addr: %x , npu w data: %x\n",(SOFT_NPU_BASE+addr),(data));
    //unsigned int res ;
    //res = readl(SOFT_NPU_BASE+addr); 
    //printf("npu r addr: %x , npu r data: %x\n",(SOFT_NPU_BASE+addr),(res));
}
unsigned int soft_reg_read(unsigned int addr){
    unsigned int res ;
    res = readl(SOFT_NPU_BASE+addr);
    //printf("npu r addr: %x , npu r data: %x\n",(addr),(res));
    return res ;
}
void alsoc_hardnpu_irq_handler(void) {
    //printf("in irq status!!!!!\n");
    done = 1 ;
    unsigned int status = 0;
    //unsigned int res = 0;
    hard_reg_write(HARDNPU__CCU__CLEAR_INTERRUPT__ADDR,0x1) ;
    //status = readl(ALSOC_NPU_ADDR((Hard_NPU__CCU__Working_status__ADDR>>2)));
    /*printf("npu status: %d\n",status);
    if (status == NPU_IDLE) {
        //res = readl(ALSOC_NPU_ADDR(NPU_RES));
        // clear interrupt and cmd status 
        writel(0x1001000, ALSOC_NPU_ADDR((Hard_NPU__Share__Filter_size__ADDR>>2)));
        res = readl(ALSOC_NPU_ADDR((Hard_NPU__Share__Filter_size__ADDR>>2)));
        //writel(NPU_OK, ALSOC_NPU_ADDR(NPU_CMD));
        printf("npu res: %d\n",res);
    }*/
}
void alsoc_softnpu_irq_handler(void) {
    //printf("in soft irq status!!!!!\n");
    done = 1 ;
    //unsigned int status = 0;
    //unsigned int res = 0;
    soft_reg_write(SOFTNPU__SOFT_SHARE__CLEAR_INTERRUPT__ADDR,0x1) ;

}