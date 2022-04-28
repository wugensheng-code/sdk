#include "usb_bsp.h"



//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//USB-BSP ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/21
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 
  
//USB OTG �ײ�IO��ʼ��
//pdev:USB OTG�ں˽ṹ��ָ��
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{//do nothing 
	//RCC->AHB1ENR|=1<<0;    		//ʹ��PORTAʱ��	    
 	//RCC->AHB2ENR|=1<<7;    		//ʹ��USB OTGʱ��	

	//printf("===>RCC->AHB1ENR  adr:0x%lx   data:0x%lx \n",&(RCC->AHB1ENR), RCC->AHB1ENR);
	//printf("===>RCC->AHB2ENR  adr:0x%lx   data:0x%lx \n",&(RCC->AHB2ENR), RCC->AHB2ENR);
	
	//GPIO_Set(GPIOA,3<<11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);	//PA11/12���ù������	
	//GPIO_Set(GPIOA,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);	//PA15�������	 LED
	//PCF8574_WriteBit(USB_PWR_IO,1);//����USB HOST��Դ����
	//GPIO_AF_Set(GPIOA,11,10);	//PA11,AF10(USB)
 	//GPIO_AF_Set(GPIOA,12,10);	//PA12,AF10(USB)
}

//USB OTG �ж�����,����USB FS�ж�
//pdev:USB OTG�ں˽ṹ��ָ��
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
   	//MY_NVIC_Init(0,3,OTG_FS_IRQn,2);	//���ȼ�����Ϊ��ռ0,�����ȼ�3����2	 
}

//USB OTG �ж�����,����USB FS�ж�
//pdev:USB OTG�ں˽ṹ��ָ��
void USB_OTG_BSP_DisableInterrupt(void)
{
}
//USB OTG �˿ڹ�������(������δ�õ�)
//pdev:USB OTG�ں˽ṹ��ָ��
//state:0,�ϵ�;1,�ϵ�
void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{
}
//USB_OTG �˿ڹ���IO����(������δ�õ�)
//pdev:USB OTG�ں˽ṹ��ָ��
void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{
}

#if 1

void usb_delay_us(int usec)
{
	int i,j;

	for(j = 1; j > 0; j--)
	{
		//__nop();
		__asm__ ("nop");
	}
}

void usb_delay_ms(int msec)
{
	int i,j;

	for(j = 100; j > 0; j--) //300
	{
		//__nop();
		__asm__ ("nop");
	}
}

#endif

//USB_OTG us����ʱ����
//�����̲���SYSTEM�ļ��е�delay.c�����delay_us����ʵ��
//�ٷ����̲��õ��Ƕ�ʱ��2��ʵ�ֵ�.
//usec:Ҫ��ʱ��us��.
void USB_OTG_BSP_uDelay (const uint32_t usec)
{ 
   	//delay_us(usec);
   	usb_delay_us(usec);
}
//USB_OTG ms����ʱ����
//�����̲���SYSTEM�ļ��е�delay.c�����delay_ms����ʵ��
//�ٷ����̲��õ��Ƕ�ʱ��2��ʵ�ֵ�.
//msec:Ҫ��ʱ��ms��.
void USB_OTG_BSP_mDelay (const uint32_t msec)
{  
	//printf("===> USB_OTG_BSP_mDelay 0 !!! \n");
	//delay_ms(msec);
	usb_delay_ms(msec);
}






















