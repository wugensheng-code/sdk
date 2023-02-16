#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "nuclei_sdk_soc.h"
#include "usbd_cdc_vcp.h"
#include "usbd_usr.h"
#include "usbd_desc.h"

USB_OTG_CORE_HANDLE    USB_OTG_dev;
extern u8 bDeviceState;
extern void OTG_FS_IRQHandler(void);

int main(void)
{
    u8 led0sta=1;
    u16 t;
    u16 len;
    u16 times=0;
    u8 usbstatus=0;


    ECLIC_Register_IRQ(SOC_INT75_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,OTG_FS_IRQHandler);
       __enable_irq();

    USBD_Init(&USB_OTG_dev,USB_OTG_HS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);

    while(1)
    {

        if(usbstatus!=bDeviceState)
        {
            usbstatus=bDeviceState;
            if(usbstatus==1)
            {
                printf("bule\n");

            }else
            {
            	 printf("red\n");

            }
        }
        if(USB_USART_RX_STA&0x8000)
        {
            len=USB_USART_RX_STA&0x3FFF;

            for(t=0;t<len;t++)
            {
                VCP_DataTx(USB_USART_RX_BUF[t]);
            }
            printf("\r\n\r\n");
            USB_USART_RX_STA=0;
        }else
        {
            times++;
            if(times%5000==0)
            {
            	//printf("\r\n ***virtual com test %d*****\r\n",times);
               // usb_printf("����ԭ��@ALIENTEK\r\n\r\n");
            }
           // if(times%200==0)usb_printf("����������,�Իس�������\r\n");
           // if(times%30==0)LED0(led0sta^=1);//��˸LED,��ʾϵͳ��������.
            USB_OTG_BSP_mDelay(10);
        }

        USB_OTG_BSP_mDelay(10);
    }


}

















