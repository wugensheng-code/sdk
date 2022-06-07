#include "usb_bsp.h"
#include "sys.h"  
#include "delay.h" 

void phy_power_pulse(USB_OTG_CORE_HANDLE *pdev);
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{

}

//USB OTG interrupt setting,enable USB FS interrupt
//pdev:USB OTG core struct point
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{

}

//USB OTG interrupt setting,disable USB FS interrupt
//pdev:USB OTG core struct point
void USB_OTG_BSP_DisableInterrupt(void)
{ 
}
//USB OTG power supply 
//pdev:USB OTG core struct point
//state:0,turn of;1,turn on
void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{ 
	//phy_power_pulse(pdev);
}
//USB_OTG power supply 
//pdev:USB OTG core struct point
void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{ 
} 
//USB_OTG us����ʱ����
//�����̲���SYSTEM�ļ��е�delay.c�����delay_us����ʵ��
//�ٷ����̲��õ��Ƕ�ʱ��2��ʵ�ֵ�.
//usec:Ҫ��ʱ��us��.
void USB_OTG_BSP_uDelay (const uint32_t usec)
{ 
    delay_us(usec);
}
//USB_OTG ms����ʱ����
//�����̲���SYSTEM�ļ��е�delay.c�����delay_ms����ʵ��
//�ٷ����̲��õ��Ƕ�ʱ��2��ʵ�ֵ�.
//msec:Ҫ��ʱ��ms��.
void USB_OTG_BSP_mDelay (const uint32_t msec)
{  
    delay_ms(msec);
}






















