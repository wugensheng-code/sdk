#include "delay.h"
#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 	 

int usb_delay_us(int usec)
{
	int i,j;
    i=0;
	for(j = 1; j > 0; j--) //300
	{
		//__nop();
		i= i+1;
		__asm__ ("nop");
	}

	return i;
}

int usb_delay_ms(int msec)
{
	int i,j;
    i=0;
	for(j = 100; j > 0; j--) //300
	{
		//__nop();
		i= i+1;
		__asm__ ("nop");
	}

	return i;
}


			   
//��ʼ���ӳٺ���
//��ʹ��OS��ʱ��,�˺������ʼ��OS��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init(u8 SYSCLK)
{

}								    


//��ʱnus
//nusΪҪ��ʱ��us��.	
//ע��:nus��ֵ,��Ҫ����798915us(���ֵ��2^24/fac_us@fac_us=21)
void delay_us(uint32_t nus)
{
	int i=0;

	for (i=0;i<nus; i++)
		usb_delay_us(i);
}

//��ʱnms 
//nms:0~65535
void delay_ms(uint32_t nms)
{	int i=0;
    for (i=0;i<nms*13; i++)
	    usb_delay_ms(i);
} 

/*
uint64_t fetch_rv_cycle(void)
{

    volatile uint32_t high0, low, high;
    uint64_t full;

    high0 = __RV_CSR_READ(CSR_MCYCLEH);
    low = __RV_CSR_READ(CSR_MCYCLE);
    high = __RV_CSR_READ(CSR_MCYCLEH);
    if (high0 != high) {
        low = __RV_CSR_READ(CSR_MCYCLE);
    }
    full = (((uint64_t)high) << 32) | low;
    return full;

}
*/
			 



































