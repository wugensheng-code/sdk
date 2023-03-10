/*
 * main.c
 *
 *  Created on: Oct.27.2021
 *      Author: haitao.liu
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"

#define SPI0_SALVE ((SPI_TypeDef *)(SPI0_BASE + 0x100))
#define SPI1_SALVE ((SPI_TypeDef *)(SPI1_BASE + 0x100))


void Spi0_Mst_IRQHandler(void)
{
	//  printf("ISR_MST:0x%x\n\r",*(volatile uint32_t*)(0xf8404000 + 0x30));
	// printf("RISR_MST:0x%x\n\r",*(volatile uint32_t*)(0xf8404000 + 0x34));

	printf("Spi0000000000_Mst_IRQHandler!\n\r");
	spi_dwc_ssi_disable(SPI0);
	spi_dwc_ssi_enable(SPI1);
}

void Spi0_Slv_IRQHandler(void)
{
	//  printf("ISR_MST:0x%x\n\r",*(volatile uint32_t*)(0xf8404000 + 0x30));
	// printf("RISR_MST:0x%x\n\r",*(volatile uint32_t*)(0xf8404000 + 0x34));

	spi_dwc_ssi_disable(SPI0_SALVE);

	spi_dwc_ssi_enable(SPI1_SALVE);

	/*enable spi0 slave*/
 *(uint32_t volatile *)(0xf8405000 + 0x10) = 0x1;

	printf("Spi000000000000_    Slv_IRQHandler!\n\r");
}

void Spi1_Mst_IRQHandler(void)
{
    // printf("ISR_MST:0x%x\n\r",*(volatile uint32_t*)(0xf8405000 + 0x30));
	// printf("RISR_MST:0x%x\n\r",*(volatile uint32_t*)(0xf8405000 + 0x34));

	printf("Spi11111111                  Mst_IRQHandler!\n\r");
	spi_dwc_ssi_disable(SPI1);

	// spi_dwc_ssi_enable(SPI0);



	/*enable spi0 spi1 to slave*/
  *(uint32_t volatile *)(0xf8800000 + 0x160) = 0x1 + (0x1 << 8);

	/*enable spi0 slave*/
 *(uint32_t volatile *)(0xf8404000 + 0x10) = 0x1; 

	 spi_dwc_ssi_enable(SPI0_SALVE);

}

void Spi1_Slv_IRQHandler(void)
{
	//     printf("ISR_MST:0x%x\n\r",*(volatile uint32_t*)(0xf8405000 + 0x30));
	// printf("RISR_MST:0x%x\n\r",*(volatile uint32_t*)(0xf8405000 + 0x34));

	printf("Spi1111111111111_    Slv_IRQHandler!\n\r");

	/*disable spi0 spi1 to slave*/
  *(uint32_t volatile *)(0xf8800000 + 0x160) = 0;
}



int main()
{
   /***********************AL9000***************************/
    printf("test start\r\n");

     ECLIC_Register_IRQ(SOC_INT85_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,Spi0_Mst_IRQHandler);
	 ECLIC_Register_IRQ(SOC_INT86_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,Spi0_Slv_IRQHandler);
	 ECLIC_Register_IRQ(SOC_INT87_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,Spi1_Mst_IRQHandler);
	 ECLIC_Register_IRQ(SOC_INT88_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,Spi1_Slv_IRQHandler);
       __enable_irq();

 spi_dwc_ssi_enable(SPI0);

 while(1);
}
