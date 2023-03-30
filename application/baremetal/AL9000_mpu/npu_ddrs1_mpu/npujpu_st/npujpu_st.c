/*
* @Author: Tianyi Wang
* @Date:   2022-03-20 10:28:30
* @Last Modified by:   Tianyi Wang
* @Last Modified time: 2022-03-29 09:12:38
*/


#include "nuclei_sdk_soc.h"
#include "npu.h"

extern void jpu_test_case_020(void);
extern int frame0_layer1_fpga_normal_test();

int main_npujpu() {
    /* init UART*/
    unsigned int n = 0;
    //uart_init(0);

    
    //open uart enable
    mem_write( 0xf8803068u , 0x3);
    mem_write( 0xf880306cu , 0x3);
    mem_write( 0xf8803410u , 0x1);

    //open use enable
    //int res = mem_read(PLS_PROT) ;
    //printf("GP PLS_PROT regsiter is %x\n",res);
    //mem_write(PLS_PROT,0x0); // enable gp port acess to pl
    //res = mem_read(PLS_PROT) ;
    //printf("GP PLS_PROT regsiter is %x\n",res);
//
    //res = mem_read(SRST_CTRL2) ;
    //printf("read reset regsiter is %x\n",res);
    //mem_write(SRST_CTRL2,res|0x3); // release hp port reset
    //res = mem_read(SRST_CTRL2) ;
    //printf("read reset regsiter is %x\n",res);


    printf("this is jpu send begin\n");
    /*
       JPU output address
       Before invoking the jpu interface, clear the data of this address,
       because there may be invalid data from the last time. Even if the
       jpu fails to access ddrs1, the test may fail because of the data from the last time.
     */
    memset((char *)0x00500000, 0, (416*416*3/2));

   	jpu_test_case_020();//jpu case
    //jpu result compare
    int com_jpu_out_num = 0 ;
    char* JPU_OFM = (char *)0x00500000 ;
    char* YUV_GLD = (char *)0x18500000 ;
    int fail_jpu = 0 ;
    for(com_jpu_out_num=0;com_jpu_out_num<(416*416*3/2);com_jpu_out_num++){
        if(JPU_OFM[com_jpu_out_num]!=YUV_GLD[com_jpu_out_num]){
//            printf("fail at %d , sim is %x , golden is %x\n",com_jpu_out_num,JPU_OFM[com_jpu_out_num],YUV_GLD[com_jpu_out_num]) ;
            fail_jpu = 1 ;
        }
    }
    if(fail_jpu==1){
        printf("TEST_JPEG2YUV FAIL!\n") ;
    }else{
        printf("TEST_JPEG2YUV PASS!\n") ;
    }

    frame0_layer1_fpga_normal_test() ;//npu yolov3-tiny uint8 mode layer1 test
    return 0;
}
