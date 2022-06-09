#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <time.h>
#include "nuclei_sdk_soc.h"
#include "AL_uart.h"
#include "string.h"
#include "sys.h"
#include "delay.h" 
#include "usbh_usr.h"


USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

extern void OTG_FS_IRQHandler(void);
void USB_SPD_Get(USB_OTG_CORE_HANDLE *pdev);


//user test app
//0 normal
//       1,error
u8 USH_User_App(void)
{  
    u8 led1sta=1;
    u32 total,free;
    u8 res=0;
    printf("device connect ok!.");
    //f_mount(fs[3],"3:",1);  //remount udisk
    //res=exf_getfree("3:",&total,&free);
    /*
    if(res==0)
    {
        POINT_COLOR=BLUE;//��������Ϊ��ɫ     
        LCD_ShowString(30,160,200,16,16,"FATFS OK!");   
        LCD_ShowString(30,180,200,16,16,"U Disk Total Size:     MB");    
        LCD_ShowString(30,200,200,16,16,"U Disk  Free Size:     MB");       
        LCD_ShowNum(174,180,total>>10,5,16);//��ʾU�������� MB
        LCD_ShowNum(174,200,free>>10,5,16); 
    } 
    */
    while(HCD_IsDeviceConnected(&USB_OTG_Core))//device connect ok
    {   
       
        delay_ms(200);
    }
   
    //f_mount(0,"3:",1);  //unmount udisk
    return res;
} 

int usb_test(void)
{   
    u8 led0sta=1;
    u8 t; 

   // f_mount(fs[0],"0:",1);      //mount sd card
   // f_mount(fs[1],"1:",1);      //mount FLASH.
   // f_mount(fs[2],"2:",1);      //mount NAND FLASH.

   // __RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
    printf("Anlogic usb msc udisk test\n\r");
    printf("Device connecting...");
    
    ECLIC_Register_IRQ(SOC_INT75_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,OTG_FS_IRQHandler);
    __enable_irq();
           
    //Initliation USB host
    USBH_Init(&USB_OTG_Core,USB_OTG_HS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);


    while(1)
    {

        USBH_Process(&USB_OTG_Core, &USB_Host);
        delay_ms(1);
        t++;
        if(t==200)
        {

            t=0;
        } 
    }   
}

#if __riscv
uint64_t fetch_rv_cycle(void)
{


    volatile uint64_t full;


    full = __RV_CSR_READ(CSR_MCYCLE);

;
    return full;

}
#endif



