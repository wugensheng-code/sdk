
#include "config.h"
#include "npu.h"
#include "delay.h"
#include "io.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "nuclei_sdk_soc.h"
//#define FPGA_TEST
//#include "./frame0_layer1_grid0_setting_normal.c"
//#include "./frame0_layer1_grid1_setting_normal.c"
//#include "./frame0_layer1_grid2_setting_normal.c"
extern void frame0_layer1_grid0_setting_normal();
extern void frame0_layer1_grid1_setting_normal();
extern void frame0_layer1_grid2_setting_normal();

extern int done ;

int frame0_layer1_grid0_fpga_normal_test() {
    int res ;
    // printf("\n layer1 grid0 initial start\n");
    frame0_layer1_grid0_setting_normal();
     res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     while(res!=0x1){
         res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     }
    //while(done==0);
    //done = 0 ;
    // printf("completed layer1 grid0 npu work\n");
}
int frame0_layer1_grid1_fpga_normal_test() {
    int res ;
    // printf("\n layer1 grid1 initial start\n");
    frame0_layer1_grid1_setting_normal();
     res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     while(res!=0x1){
       res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     }
    //while(done==0);
    //done = 0 ;
    // printf("completed layer1 grid1 npu work\n");
}
int frame0_layer1_grid2_fpga_normal_test() {
    int res ;
    // printf("\n layer1 grid2 initial start\n");
    frame0_layer1_grid2_setting_normal();
     res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     while(res!=0x1){
       res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     }
    //while(done==0);
    //done = 0 ;
    // printf("completed layer1 grid2 npu work\n");
}

int frame0_layer1_fpga_normal_test() {
    int res ;
    frame0_layer1_grid0_fpga_normal_test() ;
     res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     //printf("interrupt status is %d\n",res);
     res = hard_reg_read(HARDNPU__CCU__WORKING_STATUS__ADDR);
     //printf("working status is %d\n",res);
     hard_reg_write(HARDNPU__CCU__CLEAR_INTERRUPT__ADDR,0x1) ;
     //printf("grid0 completed\n");
     res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     //printf("interrupt status is %d\n",res);
     res = hard_reg_read(HARDNPU__CCU__WORKING_STATUS__ADDR);
     //printf("working status is %d\n",res);
    frame0_layer1_grid1_fpga_normal_test() ;
     res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     //printf("interrupt status is %d\n",res);
     res = hard_reg_read(HARDNPU__CCU__WORKING_STATUS__ADDR);
     //printf("working status is %d\n",res);
     hard_reg_write(HARDNPU__CCU__CLEAR_INTERRUPT__ADDR,0x1) ;
     //printf("grid1 completed\n");
     res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     //printf("interrupt status is %d\n",res);
     res = hard_reg_read(HARDNPU__CCU__WORKING_STATUS__ADDR);
     //printf("working status is %d\n",res);
    frame0_layer1_grid2_fpga_normal_test() ;
     res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     //printf("interrupt status is %d\n",res);
     res = hard_reg_read(HARDNPU__CCU__WORKING_STATUS__ADDR);
     //printf("working status is %d\n",res);
     hard_reg_write(HARDNPU__CCU__CLEAR_INTERRUPT__ADDR,0x1) ;
     //printf("grid2 completed\n");
     res = hard_reg_read(HARDNPU__CCU__INTERRUPT_STATUS__ADDR) ;
     //printf("interrupt status is %d\n",res);
     res = hard_reg_read(HARDNPU__CCU__WORKING_STATUS__ADDR);
     //printf("working status is %d\n",res);


    
    int fail = 0 ;
    char* ofm_golden = (char *)0x30000000 + 0x7ec00;
    char* ofm_sim = (char *)0x10000000 + 0x7ec00;
    printf("layer1 frame0 data in compare!!\n");
    for(int i = 0 ; i < 692224; i++){
        if(ofm_sim[i] != ofm_golden[i]){
            printf("layer1 frame0 wb data fail, sim: %x golden: %x, at addr %d\n",ofm_sim[i],ofm_golden[i],i);
            fail = 1 ;
            break ;
        }
        if(fail)
            break ;
    }
    if(fail) {
        printf("layer1 frame0 test fail!!!\n");
    } else {
        printf("layer1 frame0 test pass!!!\n");
    }
    return fail ;
}