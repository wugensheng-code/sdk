
#if 0
/*
 * main.c
 *
 *  Created on: 2021年11月10日
 *      Author: wei.pang
 */
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_uart.h"
#include "al9000_dmac.h"
//#include "anl_printf.h"
#include <time.h>
#include "demosoc.h"
/*
 *test
 *al9000_dmac_uart
 *
 *
 *
*/
#define LLP_BASE_ADDR      0x61030200
#define LLP_1_ADDR         0x61030200
#define LLP_2_ADDR         0x61030300
#define LLP_3_ADDR         0x61030400
#define LLP_4_ADDR         0x61030500
volatile uint16_t count = 0;

void write_To_OCM(uint32_t *pSnAddr,uint32_t Count,uint32_t *pDnAddr)
{
	volatile uint32_t i;
	if(Count < 0 || Count== 0) return;
	for(i = 0 ; i < Count ; i++)
     {
        *pDnAddr++ = *pSnAddr++;
     }
}
void SOC_DMA_AHB_HANDLER(void)
{
    //printf("enter_irq_sucess\n\r");
    //printf("DMAC_MEM2MEM_LLP_MODE has been completed\n\r");
    for(int i = 0 ; i< 5 ; i++)
    {

    	//printf("ocm_data LLP_1_ADDR = %.2x\r\n",*(uint32_t*)(LLP_1_ADDR + i*4));
    	//printf("ocm_data LLP_2_ADDR = %.2x\r\n",*(uint32_t*)(LLP_2_ADDR + i*4));
    	//printf("ocm_data LLP_3_ADDR = %.2x\r\n",*(uint32_t*)(LLP_3_ADDR + i*4));
    	//printf("ocm_data LLP_4_ADDR = %.2x\r\n",*(uint32_t*)(LLP_4_ADDR + i*4));
    	//printf("ocm_data MEM_BASE1_ADDR = %.2x\r\n",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
    	//printf("ocm_data MEM_BASE2_ADDR = %.2x\r\n",*(uint32_t*)(AL9000_UART0_BASE ));

    	if((*(uint32_t*)(MEM_BASE1_ADDR + i*4)) == (*(uint32_t*)(AL9000_UART0_BASE)))
    	{
    		printf("dmac_transmissiom_pass\r\n");
    	}
    	else
    	{
    		printf("dmac_transmissiom_fail\r\n");
    	}
    	//printf("very good");
    }
    while(1);
#if 0
      count ++;
     if(count == 10){
       dw_dmac_setSingle(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst);
       dw_dmac_clearIrq(AL9000_DMAC,dw_dmac_channel_num_1,tfr_l);
       dw_dmac_enable(AL9000_DMAC);
       //count = 0;
       //printf("complete_DMAC_transfer\n\r");
    }
    else{
           dw_dmac_clearIrq(AL9000_DMAC,dw_dmac_channel_num_1,block_1);
         }
#endif
}
    int main(void){
    	__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
	/*printf("anlogic uart_dma test");
	printf("anlogic uart_dma test");
	printf("anlogic uart_dma test");
	printf("anlogic uart_dma test");
	printf("anlogic uart_dma test");
	printf("anlogic uart_dma test");
	printf("anlogic uart_dma test");
	printf("anlogic uart_dma test");*/

    		/*mem2men mode
    		 *If you want to use interrupts to count the number of blocks
    	 	 *You can open this part
    	 	 *But we do not recommend this
    	 	 *interrupts_count_blocks_mode will stay in interrupt fo a long time,reduce DMA efficiency
    	 	 *
    	 	 */
    	        ECLIC_Register_IRQ(SOC_INT65_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_DMA_AHB_HANDLER);
    		__enable_irq();
    		/*
    	 	 *dw_dmac_enableChannelIrq()
    	 	 */
    	         dw_dmac_enableChannelIrq(AL9000_DMAC_channel_0,dw_dmac_channel_num_1);
    		/*
    		 *dw_dmac_unmaskIrq()
    		 */
    	         dw_dmac_unmaskIrq(AL9000_DMAC,dw_dmac_channel_num_1,tfr_l);
	/*
	 * dw_dmac_setAddress
	 *
	 */
	 dw_dmac_setAddress(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src,MEM_BASE1_ADDR);
		/*
		 * dw_dmac_setAddress
		 */
         dw_dmac_setAddress(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_dst,AL9000_UART0_BASE);

	/*
	 * dw_dmac_setChannelConfig
	 */
	 dw_dmac_setChannelConfig(AL9000_DMAC_channel_0,dw_dmac_channel_num_1);
	 /*
	  * dw_dmac_setTransferType
	  */
	 dw_dmac_setTransferType(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_transfer_row6,Dmac_src_dst);
	 /*
	  * dw_dmac_setMemPeriphFlowCtl
	  */
	 dw_dmac_setMemPeriphFlowCtl(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,MEM2PER);
	 /*
	  * dw_dmac_setTransWidth
	  * you can set the Transwidth :8/16/32/64/256 MAX_WIDTH=256
	  */
	 dw_dmac_setTransWidth(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,WIDTH_32);
	 /*
	  * dw_dmac_setBlockTransSize
	  * you can set the number of burst in one of the block
	  * range MIN:MAX = 1:4096
	  */
	 dw_dmac_setBlockTransSize(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,64);
	 /*
	  * dw_dmac_setBurstTransLength
	  */
	 dw_dmac_setBurstTransLength(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,DW_DMA_MSIZE_32);
	 /*
	  *  dw_dmac_setMstSelect
	  */
	 dw_dmac_setMstSelect(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,SMS_master1,DMS_master1);
	 /*
	  * dw_dmac_setAddressInc
	  */
	 dw_dmac_setAddressInc(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src,Increments);
	 dw_dmac_setAddressInc(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_dst,FIX);
	 /*
	  * dw_dmac_setHandshakingMode
	  */
	 dw_dmac_setHandshakingMode(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,Hardware_handshaking);
	 /*
	  * dw_dmac_setHsPolarity
	  */
	 dw_dmac_setHsPolarity(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,ACTIVE_HIGH);
	 /*
	  * dw_dmac_setFifoMode
	  */
	 dw_dmac_setFifoMode(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,FIFO_MODE_0);

	 /*
	  * dw_dmac_setFlowCtlMode
	  */
	 dw_dmac_setFlowCtlMode(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,fc_mode_0);
	 /*
	  * dw_dmac_checkChannelBusy
	  */
	 dw_dmac_checkChannelBusy(AL9000_DMAC,dw_dmac_channel_num_1);
	 AL9000_uart_setTxTrigger(AL9000_UART0,FIFO_1_2full);
	 dw_dmac_setListPointerAddress(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,LMS_master1,LLP_BASE_ADDR);
	 //dw_dmac_setLlpEnable(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src);
	 dw_dmac_setReload(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst);
	 AL9000_uart_setDmaMode(AL9000_UART0);
     volatile uint32_t buffer[10];
	 volatile uint32_t buffer_list_parm[4][5]={ {0x61030000,AL9000_UART0_BASE,0x61030300,0x08012025,0x0000000F},
						    {0x61030020,AL9000_UART0_BASE,0x61030400,0x08012025,0x0000000F},
						    {0x61030040,AL9000_UART0_BASE,0x61030500,0x08012025,0x0000000F},
						    {0x610300c0,AL9000_UART0_BASE,0x61030600,0x08012025,0x0000000F}};
	 for (volatile uint32_t i = 0 ; i < 100 ; i++)
	 {
		 buffer[i] = i;
	 }
	 write_To_OCM((uint32_t*)buffer,100,(uint32_t*)MEM_BASE1_ADDR);
	 /*static volatile uint32_t *p=(uint32_t*)MEM_BASE1_ADDR;    //指定q的地址为MEM_BASE1_ADDR;
			          *p=1;*/

        for (volatile uint32_t i = 0 ; i < 4 ; i++)
	{
		for (volatile uint32_t j = 0 ; j < 5 ; j++)
		{
 			buffer[j] = (uint32_t)buffer_list_parm[i][j];
 		}
              switch(i)
		{
		case 0:
		      write_To_OCM((uint32_t*)buffer,5,(uint32_t*)LLP_1_ADDR );
		break;
		case 1:
		      write_To_OCM((uint32_t*)buffer,5,(uint32_t*)LLP_2_ADDR);
		break;
		case 2:
			  write_To_OCM((uint32_t*)buffer,5,(uint32_t*)LLP_3_ADDR);
				break;
		default:
		      write_To_OCM((uint32_t*)buffer,5,(uint32_t*)LLP_4_ADDR);
		break;
		}
	}
  	 /*
	  * dw_dmac_enable
	  */
	 dw_dmac_enable(AL9000_DMAC);
	 /*
	  * dw_dmac_enableChannel
	  */
	 dw_dmac_enableChannel(AL9000_DMAC,dw_dmac_channel_num_1);

	return 0;
}
#endif
#if 0
    /*
     * main.c
     *
     *  Created on: 2021年11月10日
     *      Author: wei.pang
     */

    #include <stdio.h>
    #include <stdlib.h>
    #include "nuclei_sdk_soc.h"
    #include "al9000_uart.h"
    #include "al9000_dmac.h"
    #include <time.h>
    /*
     *instruction
     *test_mode		:reload mode
     *importannt parameter	:
     *
     *
     *
    */
    volatile uint16_t count = 0;

   /* void write_To_OCM(uint32_t *pSnAddr,uint32_t Count,uint32_t *pDnAddr)
    {
    	volatile uint32_t i;
    	if(Count < 0 || Count== 0) return;
    	for(i = 0 ; i < Count ; i++)
         {
            *pDnAddr++ = *pSnAddr++;
         }
    }*/
    /*void SOC_DMA_AHB_HANDLER(void)
    {
        //printf("enter_irq_number%d\n\r",count);
        printf("enter_irq_sucess\n\r");
        printf("DMAC_MEM2MEM_SINGLE_MODE has been completed\n\r");
    	 for(int i = 0 ; i<10; i++)
    	 {
    	 	printf("ocm_data1 = %.2x\r\n",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
    	 	printf("ocm_data2 = %.2x\r\n",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
        	if((*(uint32_t*)(MEM_BASE1_ADDR + i*4)) == (*(uint32_t*)(MEM_BASE2_ADDR + i*4)))
        	{
        		printf("dmac_transmissiom_pass\r\n");
        	}
        	else
        	{
        		printf("dmac_transmissiom_fail\r\n");
        	}
    	 }
    	 while(1);
    	 /*
          count ++;
         if(count == 10){
           dw_dmac_setSingle(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst);
           dw_dmac_clearIrq(AL9000_DMAC,dw_dmac_channel_num_1,tfr_l);
           dw_dmac_enable(AL9000_DMAC);
           //count = 0;
           //printf("complete_DMAC_transfer\n\r");
        }
        else{
               dw_dmac_clearIrq(AL9000_DMAC,dw_dmac_channel_num_1,block_1);
             }
    }*/
        int main(void){
    	/*__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);

    	/*mem2men mode
    	 *If you want to use interrupts to count the number of blocks
     	 *You can open this part
     	 *But we do not recommend this
     	 *interrupts_count_blocks_mode will stay in interruptttttt fo a long time,reduce DMA efficiency
     	 *
     	 */
#if 0
            ECLIC_Register_IRQ(SOC_INT65_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_DMA_AHB_HANDLER);
            __enable_irq();
    	/*
     	 *dw_dmac_enableChannelIrq()
     	 */
             dw_dmac_enableChannelIrq(AL9000_DMAC_channel_0,dw_dmac_channel_num_1);
    	/*
    	 *dw_dmac_unmaskIrq()
    	 */
             dw_dmac_unmaskIrq(AL9000_DMAC,dw_dmac_channel_num_1,tfr_l);
#endif
    	/*
    	 * dw_dmac_setAddress
    	 *
    	 */
    	 dw_dmac_setAddress(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src,MEM_BASE1_ADDR);
    		/*
    		 * dw_dmac_setAddress
    		 */
         dw_dmac_setAddress(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_dst,AL9000_UART0_BASE);

    	/*
    	 * dw_dmac_setChannelConfig
    	 */
    	 dw_dmac_setChannelConfig(AL9000_DMAC_channel_0,dw_dmac_channel_num_1);
    	 /*
    	  * dw_dmac_setTransferType
    	  */
    	 //dw_dmac_setTransferType(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_transfer_row1,Dmac_src);
    	 dw_dmac_setReload(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src);
    	 dw_dmac_setReload(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_dst);
    	 //dw_dmac_setTransferType(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_transfer_row1,Dmac_dst);
    	 /*
    	  * dw_dmac_setMemPeriphFlowCtl
    	  */
    	 dw_dmac_setMemPeriphFlowCtl(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,MEM2PER);
    	 /*
    	  * dw_dmac_setTransWidth
    	  * you can set the Transwidth :8/16/32/64/256 MAX_WIDTH=256
    	  */
    	 dw_dmac_setTransWidth(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,WIDTH_8);
    	 /*
    	  * dw_dmac_setBlockTransSize
    	  * you can set the number of burst in one of the block
    	  * range MIN:MAX = 1:4096
    	  */
    	 dw_dmac_setBlockTransSize(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,64);
    	 /*
    	  * dw_dmac_setBurstTransLength
    	  */
    	 dw_dmac_setBurstTransLength(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,DW_DMA_MSIZE_8);
    	 dw_dmac_setAddressInc(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_dst,FIX);

              /*volatile uint32_t buffer[128];
    	 for (volatile uint32_t i = 0 ; i < 10 ; i++)
    	 {
    		 buffer[i] = i;
    	 }
    	 write_To_OCM((uint32_t*)buffer,128,(uint32_t*)MEM_BASE1_ADDR);*/
    	 AL9000_uart_setTxTrigger(AL9000_UART0,FIFO_empty);
    	// dw_dmac_setListPointerAddress(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,LMS_master1,LLP_BASE_ADDR);
    	 //dw_dmac_setLlpEnable(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src);
    	 //dw_dmac_setReload(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst);
    	 AL9000_uart_setDmaMode(AL9000_UART0);
      	 /*
    	  * dw_dmac_enable
    	  */
    	 dw_dmac_enable(AL9000_DMAC);
    	 /*
    	  * dw_dmac_enableChannel
    	  */
    	 dw_dmac_enableChannel(AL9000_DMAC,dw_dmac_channel_num_1);

    	 /*for(int i = 0 ; i< 130; i++)
    	 {
    	 	printf("ocm_data ");
    	 }*/
    	 //printf("ocm_data\r\n");

    	return 0;
    }




#endif
/*
 * main.c
 *
 *  Created on: 2021�?11�?10�?
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_uart.h"
#include "al9000_dmac.h"
#include <time.h>
/*
 *instruction
 *test_mode		:reload mode
 *importannt parameter	:
 *
 *
 *
*/
volatile uint16_t count = 0;
void SOC_DMA_AHB_HANDLER(void)
{
    //printf("enter_irq_number%d\n\r",count);        
      count ++;
     if(count == 10){
       dw_dmac_setSingle(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst);
       dw_dmac_clearIrq(AL9000_DMAC,dw_dmac_channel_num_1,tfr_l);
       dw_dmac_enable(AL9000_DMAC);
       //count = 0;
       //printf("complete_DMAC_transfer\n\r");    
    }
    else{
           dw_dmac_clearIrq(AL9000_DMAC,dw_dmac_channel_num_1,block_1);
         }  
}
    int main(void){
	 dw_dmac_setChannelConfig(AL9000_DMAC_channel_0,dw_dmac_channel_num_1);	
	 dw_dmac_setAddress(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src,MEM_BASE1_ADDR);
         dw_dmac_setAddress(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_dst,AL9000_UART0_BASE);
	 dw_dmac_setAddressInc(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_dst,FIX);
	 //dw_dmac_setChannelConfig(AL9000_DMAC_channel_0,dw_dmac_channel_num_1);
	 dw_dmac_setTransferType(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_transfer_row1,Dmac_src);
	 dw_dmac_setMemPeriphFlowCtl(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,MEM2PER);
	 dw_dmac_setTransWidth(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,WIDTH_16);
	 dw_dmac_setBlockTransSize(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,50);
	 dw_dmac_setBurstTransLength(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,DW_DMA_MSIZE_32);
          volatile uint32_t buffer[128];
	 for (volatile uint32_t i = 0 ; i < 100 ; i++)
	 {
		 buffer[i] = i;
	 }
	 write_To_OCM((uint32_t*)buffer,128,(uint32_t*)MEM_BASE1_ADDR);
  	 /*
	  * dw_dmac_enable
	  */
	 AL9000_uart_setTxTrigger(AL9000_UART0,FIFO_1_2full);
	 AL9000_uart_setDmaMode(AL9000_UART0);
	 dw_dmac_enable(AL9000_DMAC);
	 /*
	  * dw_dmac_enableChannel
	  */
	 dw_dmac_enableChannel(AL9000_DMAC,dw_dmac_channel_num_1);	
	return 0;
}



