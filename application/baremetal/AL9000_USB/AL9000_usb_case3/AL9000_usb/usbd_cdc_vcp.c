#include "usbd_cdc_vcp.h" 
#include "string.h"	
#include "stdarg.h"		 
#include "stdio.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//usb vcp��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/2/24
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//USB���⴮��������ò���
LINE_CODING linecoding =
{
	115200,		//������
	0x00,   	//ֹͣλ,Ĭ��1λ
	0x00,   	//У��λ,Ĭ����
	0x08    	//����λ,Ĭ��8λ
}; 

u8  USART_PRINTF_Buffer[USB_USART_REC_LEN];	//usb_printf���ͻ�����

//�����ƴ���1�������ݵķ���,������USB���⴮�ڽ��յ�������.
u8 USB_USART_RX_BUF[USB_USART_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USB_USART_RX_STA=0;       				//����״̬���	 

 
extern uint8_t  APP_Rx_Buffer [];			//���⴮�ڷ��ͻ�����(��������) 
extern uint32_t APP_Rx_ptr_in;   			//���⴮�ڽ��ջ�����(�������Ե��Ե�����)

//���⴮�����ú���(��USB�ں˵���)
CDC_IF_Prop_TypeDef VCP_fops = 
{
	VCP_Init,
	VCP_DeInit,
	VCP_Ctrl,
	VCP_DataTx,
	VCP_DataRx
}; 

//��ʼ��VCP
//����ֵ:USBD_OK
uint16_t VCP_Init(void)
{ 
	return USBD_OK;
} 
//��λVCP
//����ֵ:USBD_OK
uint16_t VCP_DeInit(void)
{ 
	return USBD_OK;
} 
//����VCP������
//buf:�������ݻ�����/�������滺����
//len:���ݳ���
//����ֵ:USBD_OK
uint16_t VCP_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{ 
	switch (Cmd)
	{
		case SEND_ENCAPSULATED_COMMAND:break;   
		case GET_ENCAPSULATED_RESPONSE:break;  
		case SET_COMM_FEATURE:break;  
		case GET_COMM_FEATURE:break;  
 		case CLEAR_COMM_FEATURE:break;  
		case SET_LINE_CODING: //0x20
			linecoding.bitrate = (uint32_t)(Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
			linecoding.format = Buf[4];
			linecoding.paritytype = Buf[5];
			linecoding.datatype = Buf[6]; 
			//��ӡ���ò���
#if 0
			printf("linecoding.format:%d\r\n",linecoding.format);
			printf("linecoding.paritytype:%d\r\n",linecoding.paritytype);
			printf("linecoding.datatype:%d\r\n",linecoding.datatype);
			printf("linecoding.bitrate:%d\r\n",linecoding.bitrate);
#endif
			break; 
		case GET_LINE_CODING: //0x21
			Buf[0] = (uint8_t)(linecoding.bitrate);
			Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
			Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
			Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
			Buf[4] = linecoding.format;
			Buf[5] = linecoding.paritytype;
			Buf[6] = linecoding.datatype; 
			break; 
		case SET_CONTROL_LINE_STATE:break;//0x22
		case SEND_BREAK:break;   
		default:break;  
	} 
	return USBD_OK;
}


//����һ���ֽڸ����⴮��(��������)
//data:Ҫ���͵�����
//����ֵ:USBD_OK
uint16_t VCP_DataTx (uint8_t data)
{  
	APP_Rx_Buffer[APP_Rx_ptr_in]=data;	//д�뷢��buf
	APP_Rx_ptr_in++;  					//дλ�ü�1
	//if(APP_Rx_ptr_in == APP_RX_DATA_SIZE)	//����buf��С��,����.
	if(APP_Rx_ptr_in >= APP_RX_DATA_SIZE)
	{
		APP_Rx_ptr_in = 0;
	}
	return USBD_OK;
} 
//�����USB���⴮�ڽ��յ�������
//databuffer:���ݻ�����
//Nb_bytes:���յ����ֽ���.
//����ֵ:USBD_OK
uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len)
{
	u8 i;
	u8 res;
	//printf("VCP_DataRx \r\n");
	for(i=0;i<Len;i++)
	{  
		res=Buf[i]; 
		if((USB_USART_RX_STA&0x8000)==0)		//����δ���
		{
			if(USB_USART_RX_STA&0x4000)			//���յ���0x0d
			{
				if(res!=0x0a)USB_USART_RX_STA=0;//���մ���,���¿�ʼ
				else USB_USART_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)USB_USART_RX_STA|=0x4000;
				else
				{
					USB_USART_RX_BUF[USB_USART_RX_STA&0X3FFF]=res;
					USB_USART_RX_STA++;
					if(USB_USART_RX_STA>(USB_USART_REC_LEN-1))USB_USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	
				}					
			}
		}   
	}  
	return USBD_OK;
}
//usb���⴮��,printf ����
//ȷ��һ�η������ݲ���USB_USART_REC_LEN�ֽ�
void usb_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART_PRINTF_Buffer,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART_PRINTF_Buffer);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
		VCP_DataTx(USART_PRINTF_Buffer[j]); 
	}
} 



























