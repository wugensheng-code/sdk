
/**
  * Copyright (c) 2019 Nuclei Limited. All rights reserved.
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  * Licensed under the Apache License, Version 2.0 (the License); you may
  * not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an AS IS BASIS, WITHOUT
  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */

/**
  * \file zx_xec.c
  * \brief source file for the XEC
  */

/* Includes ------------------------------------------------------------------*/
#include "demosoc.h"
#include "demosoc_xec.h"
#include "string.h"


ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB] __attribute__ ((aligned (4))); /* Ethernet Rx Descriptor */
ETH_DMADESCTypeDef  DMATxDscrTab[ETH_TXBUFNB] __attribute__ ((aligned (4))); /* Ethernet Tx Descriptor */
uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __attribute__ ((aligned (4))) = {0}; /* Ethernet Receive Buffer */
uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __attribute__ ((aligned (4))) = {0}; /* Ethernet Transmit Buffer */

__IO ETH_DMADESCTypeDef  *DMATxDescToSet;
__IO ETH_DMADESCTypeDef  *DMARxDescToSet;
uint32_t ETH_DMADESC_Index =  0;
uint32_t ETH_DMADESCRx_Index =  0;
//--------------------xec1---------------------------------------------------------------------------------------
ETH_DMADESCTypeDef  DMARx1DscrTab[ETH_RXBUFNB] __attribute__ ((aligned (4))); /* Ethernet Rx Descriptor */
ETH_DMADESCTypeDef  DMATx1DscrTab[ETH_TXBUFNB] __attribute__ ((aligned (4))); /* Ethernet Tx Descriptor */
uint8_t Rx_Buff1[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __attribute__ ((aligned (4))) = {0}; /* Ethernet Receive Buffer */
uint8_t Tx_Buff1[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __attribute__ ((aligned (4))) = {0}; /* Ethernet Transmit Buffer */

__IO ETH_DMADESCTypeDef  *DMATx1DescToSet;
__IO ETH_DMADESCTypeDef  *DMARx1DescToSet;
uint32_t ETH1_DMADESC_Index =  0;
uint32_t ETH1_DMADESCRx_Index =  0;





uint8_t DA[6]={0x00, 0x2b, 0x20, 0x21, 0x03, 0x23};

uint8_t DA_mask[6]={0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t broadcast[6]={0xff,0xff,0xff,0xff,0xff,0xff};
uint8_t broadcast_mask[6]={0x00,0x00,0x00,0x00,0x00,0x00};

#define PHY_DEVICE_ADDRESS  0x001

/**
  * @brief  Enables or disables the MAC transmission.
  * @param  NewState: new state of the MAC transmission.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ETH_MACTransmissionCmd(FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    /* Enable the MAC transmission */
	  XEC0_MAC->MAC_Configuration  = XEC0_MAC->MAC_Configuration | 0x1<<1;
	 // XEC0_MAC->MAC_Configuration  = XEC0_MAC->MAC_Configuration | 0x1<<1 | 0x1<<29;   //tx mac address insert
	// XEC0_MAC->MAC_Configuration  = XEC0_MAC->MAC_Configuration | 0x1<<1 | 0x1<<29 | 0x1<<1;   //tx mac address replacement
	//  XEC0_MAC->MAC_Configuration  = XEC0_MAC->MAC_Configuration | 0x1<<1 | 0x1<<16 | 0x1<<19;    //jumbo pkt
  }
  else
  {
    /* Disable the MAC transmission */
	  XEC0_MAC->MAC_Configuration =  XEC0_MAC->MAC_Configuration | 0x0<<0 ;
  }
}


/**
  * @brief  Enables or disables the MAC reception.
  * @param  NewState: new state of the MAC reception.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ETH_MACReceptionCmd(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the MAC reception */
	  XEC0_MAC->MAC_Configuration =XEC0_MAC->MAC_Configuration | 0x1<<0;
  }
  else
  {
    /* Disable the MAC reception */
	  XEC0_MAC->MAC_Configuration = XEC0_MAC->MAC_Configuration | 0x0<<0;
  }
}



void ETH_Init(void)
{

  //  printf("ETH_Init\r\n");
    uint32_t ID_val=0 , CR_val=0, SR_val=0 ,reg0_val=0;
    XEC0_MACAddr->MAC_Address0_Low =0x2b000801;
    XEC0_MACAddr->MAC_Address0_High =0x2220;
    XEC0_MACAddr->MAC_Address1_Low =0x2b000801;
    XEC0_MACAddr->MAC_Address1_High =0x2220;

    XEC0_MAC->MAC_Packet_Filter = 0x1<<31|                           //
    		                      // 0x0<<31|
                                  // 1<<16|                              //vlan filter
                                   0<<15|
                                   0<<8|
                                   0<<7|
                                   0<<6|
                                   0x0<<4|
                                   0x0<<2|
								  // 0x0<<0;                         //filter
                                   0x1<<0;                           //f804_0008  pass all incoming pkt irrespective sa/da
                        
    XEC0_Interrupt->MAC_Interrupt_Enable = 0x0<<0|
                           0x0<<2|
                           0x0<<5|
                           0x0<<4|
                           0x0<<9|
                           0x1<<14;                                  //f804_00b4

    //XEC0_MAC->MAC_VLAN_Tag = 0x1<<27  | 0x1<<16 | 0x1<<20 | 0x1<<2;              //vlan filter right setting
  //  XEC0_MAC->MAC_VLAN_Tag_Data =0x1<<27 | 0x1<<16 | 0x1<<2;              //vlan filter
    
    //XEC0_MAC->MAC_Configuration = XEC0_MAC->MAC_Configuration | 0x1 <<12;   //internal loopback
   // ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x6000);  //f804_0200  LOOPBACK 100M
   // ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x4000);  //f804_0200  phy LOOPBACK 10M
    ETH_MACTransmissionCmd(ENABLE);
    ETH_MACReceptionCmd(ENABLE);           

    XEC0_EQOS_MTL_Q0->MTL_RxQ0_Operation_Mode =XEC0_EQOS_MTL_Q0->MTL_RxQ0_Operation_Mode | 0x20; //F804_0d30
    XEC0_EQOS_MTL_Q0->MTL_TxQ0_Operation_Mode=XEC0_EQOS_MTL_Q0->MTL_TxQ0_Operation_Mode | 0x1<<1;
//    XEC1_IOC->IOC_Tx_config = 0<<8|
//                            (ETH_TXBUFNB-1)<<0;

//    XEC1_IOC->IOC_Rx_config = ETH_MTU<<16|
//                            0<<8|
//                            (ETH_RXBUFNB-1)<<0;
//    XEC1_IOC->IOC_Sys_Wr_config =0x1f;
  //  ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x1<<15);  //f804_0200
  //  ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x1<<14);  //f804_0200  LOOPBACK
 //   reg0_val=ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
//    printf("%04x\r\n",reg0_val);
   /* do {
        reg0_val = ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
    }while(reg0_val & 0x1<<15);

     reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
     ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,(reg0_val | (0x1<<12)));
   //ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,(reg0_val & ~(0x1<<12)));
    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
   //ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,(reg0_val | (0x1<<14)));
    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
   // ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,(reg0_val & ~(0x1<<6) &~(0x1<<13)  |(0x1<<8)));
     do {
        reg0_val = ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x1A);
    }while(!(reg0_val & 0x1<<2));
*/
  //ian fang  printf("init finish\r\n");
   // ETH_CreatLUT(0,DA,DA_mask,NULL,NULL,0);
   // ETH_CreatLUT(1,broadcast,broadcast_mask,NULL,NULL,0);
}

/*void ETH_Init1(void)
{

  //  printf("ETH_Init\r\n");
    uint32_t ID_val=0 , CR_val=0, SR_val=0 ,reg0_val=0;
    XEC0_MACAddr->MAC_Address0_Low =0x2b000801;
    XEC0_MACAddr->MAC_Address0_High =0x2220;
    XEC0_MACAddr->MAC_Address1_Low =0x13010001;
    XEC0_MACAddr->MAC_Address1_High =0xdf001030;

    XEC0_MAC->MAC_Packet_Filter = //0x1<<31|                           //
    		                       0x0<<31|
                                   0<<16|
                                   0<<15|
								   1<<9|                               //mac sa filter
                                   0<<8|
                                   0<<7|
                                   0<<6|
                                   0x0<<4|
                                   0x0<<2|
								   0x0<<0;
                                   //0x1<<0;                           //f804_0008  pass all incoming pkt irrespective sa/da

    XEC0_Interrupt->MAC_Interrupt_Enable = 0x0<<0|
                           0x0<<2|
                           0x0<<5|
                           0x0<<4|
                           0x0<<9|
                           0x1<<14;                                  //f804_00b4

  //  XEC0_MAC->MAC_Configuration = XEC0_MAC->MAC_Configuration | 0x1 <<12;   //internal loopback
   // ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x6000);  //f804_0200  LOOPBACK 100M
 //   ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x4000);  //f804_0200  phy LOOPBACK 10M
    ETH_MACTransmissionCmd(ENABLE);
    ETH_MACReceptionCmd(ENABLE);

    XEC0_EQOS_MTL_Q0->MTL_RxQ0_Operation_Mode =XEC0_EQOS_MTL_Q0->MTL_RxQ0_Operation_Mode | 0x20; //F804_0d30
    XEC0_EQOS_MTL_Q0->MTL_TxQ0_Operation_Mode=XEC0_EQOS_MTL_Q0->MTL_TxQ0_Operation_Mode | 0x1<<1;

}

*/


FrameTypeDef ETH_Rx_Packet(void)
{
    uint32_t framelength=0;
    uint32_t Rx_Tail;
    uint32_t rx_status,reg0_val;
    FrameTypeDef frame={0,0};
    uint8_t * buffer;

    //if(*(uint32_t *)(0x10070170) & BIT(0))
    //{
    //    XEC1_PhyPort->PRT_INTR_STS =0x1;
    //    printf("FATL!!!!!!!!\r\n");
   // }
    //if(*(uint32_t *)(0x10070170) & BIT(1))
    //{
    //    *(uint32_t *)(0x10070170) =0x2;
    //}
    //XEC0_DMA_CH->DMA_CH0_Rx_Control = 0x17ff9;//XEC0_DMA_CH->DMA_CH0_Rx_Control | 0x1;  //1108
    //rx_status =  DMARxDescToSet->DTYPE_SPEC_1 ;
    //if((rx_status & RECE_NO_ERROR) == RESET )
    //{
    //    if((rx_status & BIT(2)))
    //    {
            frame.length = DMATxDescToSet->DTYPE_SPEC_2 & 0x7fff  ;
            frame.buffer = (uint32_t)&Rx_Buff[ETH_DMADESCRx_Index];   //(uint32_t)
            frame.descriptor =  DMARxDescToSet;
        buffer = (uint8_t *)frame.buffer;
      // }
   // }else
   // {
   //     printf("ETH_Rx_Packet fail or discard\r\n");
   // }

 //   DMARxDescToSet->DTYPE_SPEC_0 = (uint32_t)&Rx_Buff[ETH_DMADESCRx_Index];
 //   DMARxDescToSet->COMM = 0xb18005f8;//SUMM_BUF0V |DINFO_HW | DINFO_INT;

 //   Rx_Tail = XEC0_DMA_CH->DMA_CH0_RxDesc_Tail_Pointer;
 //   if(Rx_Tail < (RX_RING_LEN-1)*(0x10))
 //   {
 //   	XEC0_DMA_CH->DMA_CH0_RxDesc_Tail_Pointer = Rx_Tail + 0x10;
 //   }else
 //   {
 //   	XEC0_DMA_CH->DMA_CH0_RxDesc_Tail_Pointer = 0x0;
 //   }

    if ((ETH_DMADESCRx_Index+1)== RX_RING_LEN)
    {
        ETH_DMADESCRx_Index = 0;
    }else
    {
        ETH_DMADESCRx_Index++;
    }

    DMARxDescToSet = (ETH_DMADESCTypeDef*) &(DMARxDscrTab[ETH_DMADESCRx_Index]);

    return frame;
}

uint8_t ETH_Tx_Packet(uint32_t FrameLength)
{
    static uint8_t count=0;
    
   // while((DMATxDescToSet->COMM & DINFO_HW)== SET);
 //ian fang   printf("ETH_Tx_Packet successfully\r\n");
                   //0xf804_1120
    XEC0_DMA_CH->DMA_CH0_Tx_Control = XEC0_DMA_CH->DMA_CH0_Tx_Control | 0x1 | 0x1<<4;    //DMA transmit    start                              //f804_1104
   //ETH_IOCTxCmd(ENABLE);

    if ((ETH_DMADESC_Index+1)== TX_RING_LEN)
    {
        ETH_DMADESC_Index = 0;
    }else
    {
        ETH_DMADESC_Index++;
    }
    
    DMATxDescToSet = (ETH_DMADESCTypeDef*) &(DMATxDscrTab[ETH_DMADESC_Index]);

}

void ETH_DMARxDescChainInit()
{
	uint32_t tempreg = 0;
	 uint32_t reg0_val = 0;
//ian	 reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
//	 printf("%04x\r\n",reg0_val);
//	    ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x1<<13);
//	    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
//	    printf("%04x\r\n",reg0_val);
//-------------------------------------------------------------------------------------
	   // uint32_t i = 0;
	   //for (i = 0x0; i <= 0xff; i++) {
	   //	  reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x3);
	   // 	  if(reg0_val==0xc916)
    	//	  printf("..");
	  // 	  else
	  // 		  printf("%04x\r\n",reg0_val);
	  // 	}
//--------------------------------------------------------------------------------------

//	    ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x19,0x1<<11 | 0x1<<0);
//	    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x19);
//	   	    printf("reg19 %04x\r\n",reg0_val);


	   // ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x4,0x0);
	   // reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x4);
	   // printf("%04x\r\n",reg0_val);

	   // reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x9);
	   //	    reg0_val=reg0_val | 0x300;
	  	//    ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x9,reg0_val);
	  	//    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x9);
	  	//    	   printf("%04x\r\n",reg0_val);

//	  	    	 ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x1<<9 |0x1<<8);
//	  	    		  	    		    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x1);
//	  	    		  	    		    printf("reg1 %04x\r\n",reg0_val);

	  	//    		  	   	   	 reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x1);
	  	  //  		  	   	   	 	  	    			    printf("reg01 %04x\r\n",reg0_val);



	  	 //   	 ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x1<<12 );
	  	 //   		    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
	  	 //   		    printf("%04x\r\n",reg0_val);

//	  	   		  ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x1<<9);
//	  	    			    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
//	  	    			    printf("%04x\r\n",reg0_val);


 	    //		  	   	   	 reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x1);
 	    //		  	   	   	 	  	    			    printf("reg01 %04x\r\n",reg0_val);



	  //  ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x1<<15);
	  // 	    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
	  // 	    printf("%04x\r\n",reg0_val);

	  // 	 reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x1);
	  // 	 	  	    			    printf("%04x\r\n",reg0_val);

	 //  	 	  	    			 reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x1);
	 //  	 	  	    				   	 	  	    			    printf("%04x\r\n",reg0_val);

//------------------------------force------------------------------------------------------------------------------------
	 //  	 ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x140);
	 //    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
	 //    printf("%04x\r\n",reg0_val);
	 //    ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x1<<11);
	 //    ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x1040);
	 //    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
	 //  	     printf("%04x\r\n",reg0_val);


	 //   reg0_val = reg0_val | 0x1<<9 | 0x1<<12;
	 //   ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,reg0_val);
//ian	    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
	 //   reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x9);
	 //   reg0_val=reg0_val | 0x300;
	 //   ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x9,reg0_val);
//	    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x9);
//	    	   printf("%04x\r\n",reg0_val);

	//   reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x0);
	//   printf("%04x\r\n",reg0_val);
//	   reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x1);
//	  	   printf("%04x\r\n",reg0_val);
//	    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x2);
//	    printf("%04x\r\n",reg0_val);

//ian	    reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x3);
//	  	    printf("%04x\r\n",reg0_val);
//	  	  reg0_val =  ETH_ReadPHYRegister(PHY_DEVICE_ADDRESS,0x1A);
//	  	 	  	    printf("%04x\r\n",reg0_val);

	//  	  ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x1<<11);
	//      ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x1040);
		tempreg = XEC0_DMA-> DMA_Mode;                         //0xf810_1000
		XEC0_DMA-> DMA_Mode = tempreg | (uint32_t)0x1;
		do
		  {
			tempreg = XEC0_DMA-> DMA_Mode;
		  } while (tempreg & 0x1);
		XEC0_MAC->MAC_Configuration = XEC0_MAC->MAC_Configuration | 0x1<<15 | 0x1<<14 | 0x1 <<13 ; //100Mbps  bit13 1 full-duplex 0:half-duplex
	//	XEC0_MAC->MAC_Configuration = XEC0_MAC->MAC_Configuration | 0x1<<15 | 0x1 <<13; //10Mbps
	    XEC0_DMA->DMA_SysBus_Mode = XEC0_DMA->DMA_SysBus_Mode | 0x1<<12;   //AAL   0xf804_1004
	    XEC0_EQOS_MTL_Q0->MTL_RxQ0_Operation_Mode = XEC0_EQOS_MTL_Q0->MTL_RxQ0_Operation_Mode | 0x1<<1;
		//XEC0_DMA->DMA_SysBus_Mode = XEC0_DMA->DMA_SysBus_Mode | 0x1 <<31;
		XEC0_DMA_CH->DMA_CH0_RxDesc_Ring_Length = (uint32_t)0x10;       //       0xf804_1130
		 memset(DMATxDscrTab,0,sizeof(ETH_DMADESCTypeDef) * ETH_TXBUFNB);
		 memset(DMARxDscrTab,0,sizeof(ETH_DMADESCTypeDef) * ETH_RXBUFNB);
	ETH_DMADESCRx_Index=0;
    for(int i=0;i<ETH_RXBUFNB;i++)
    {
        DMARxDscrTab[i].DTYPE_SPEC_0 = (uint32_t)&Rx_Buff[i];
        DMARxDscrTab[i].COMM = DMARxDscrTab[i].COMM | 0x1<<31 | 0x1<<24 | 0x1<<30;// bit30:enable interrupt
    }
    DMARxDescToSet = (ETH_DMADESCTypeDef*) &(DMARxDscrTab[ETH_DMADESCRx_Index]);
    XEC0_DMA_CH->DMA_CH0_RxDesc_List_Address = (uint32_t) DMARxDscrTab;                                       //0xf804_111c   0xc101_0198     0xc101_0254
    XEC0_DMA_CH->DMA_CH0_RxDesc_Tail_Pointer = XEC0_DMA_CH->DMA_CH0_RxDesc_List_Address + (uint32_t)0x40;     //0xf804_1128

	//XEC0_DMA_CH->DMA_CH0_TxDesc_List_Address = 0x0;//XEC0_DMA_CH->DMA_CH0_TxDesc_List_Address | 0x1 <<10;
                                                              //
	XEC0_DMA_CH->DMA_CH0_Control = XEC0_DMA_CH->DMA_CH0_Control | 0x1<<16;                                  //0xf804_1100
	XEC0_DMA_CH->DMA_CH0_Interrupt_Enable |= 0x1<<15 | 0x1<<6;    //DMA_CH(#i)_Interrupt_Enable  15:NIE 6:RIE
	XEC0_DMA_CH->DMA_CH0_Rx_Control = 0x17ff9;                                                              //0xf804_1108  start
	//XEC0_DMA_CH->DMA_CH0_Rx_Control = XEC0_DMA_CH->DMA_CH0_Rx_Control |  0x1 <<18;                        //TXPBL     0xf804_1108
    //XEC0_DMA_CH->DMA_CH0_Rx_Control = XEC0_DMA_CH->DMA_CH0_Rx_Control | 0x1;

			//tempreg = XEC0_DMA-> DMA_Mode;
    //XEC1_IOC_CHX->IOC_CHX_RX_TAIL_POINTER = (uint32_t)0x10;
}

void ETH_DMATxDescChainInit()
{
	XEC0_DMA_CH->DMA_CH0_TxDesc_Ring_Length = (uint32_t)0x8;                            //0xf804_112c
	//XEC0_DMA_CH->DMA_CH0_RxDesc_List_Address= 0x0;

    ETH_DMADESC_Index=0;
    for(int i=0;i<ETH_TXBUFNB;i++)
    {
        DMATxDscrTab[i].DTYPE_SPEC_0 = (uint32_t)&Tx_Buff[i];
        DMATxDscrTab[i].DTYPE_SPEC_1 =  0;
        DMATxDscrTab[i].DTYPE_SPEC_2 = 0x97;// 0xc00;//0x97;//FrameLength-1;   //0x40 crc pad
        DMATxDscrTab[i].COMM = 0xb0000097;//0xb0000c00;//0xb0000097;//(FrameLength-1);//0x1 <<31;;//DMATxDscrTab[i].COMM |  0x1 <<31 | 0x1 <<28 | 0x1 <<29 | (ETH_TX_BUF_SIZE) <<0;   //0X5F8=1528

      //  DMATxDescToSet->DTYPE_SPEC_2    =  FrameLength-1;
      //     DMATxDescToSet->DTYPE_SPEC_0 = (uint32_t)&Tx_Buff[ETH_DMADESC_Index];
                //DMATxDescToSet->COMM           |=  (SUMM_BUF0V|SUMM_FD|SUMM_LD | PKTOP_PAD_EN |0x1<<2);
      //     DMATxDescToSet->COMM         =  0xb1000000 | (FrameLength-1);//0x1 <<31;

    }

    DMATxDescToSet = (ETH_DMADESCTypeDef*) &(DMATxDscrTab[ETH_DMADESC_Index]);
    XEC0_DMA_CH->DMA_CH0_TxDesc_List_Address = (uint32_t)DMATxDscrTab;                                          //0xf804_1114       c101_0218
                                                     //0xf804_112c
    XEC0_DMA_CH->DMA_CH0_TxDesc_Tail_Pointer = XEC0_DMA_CH->DMA_CH0_TxDesc_List_Address + 0x40;//ETH_DMADESC_Index < (TX_RING_LEN-1) ? (ETH_DMADESC_Index+1) * (0x10) : 0;
    XEC0_DMA_CH->DMA_CH0_TxDesc_Ring_Length = (uint32_t)0x8;
   // XEC0_DMA_CH->DMA_CH0_Tx_Control = XEC0_DMA_CH->DMA_CH0_Tx_Control | 0x1<<4 | 0x1;   //RBSZ 0X11 BIT0 BIT4     0xf804_1104
   // XEC0_DMA_CH->DMA_CH0_Tx_Control = XEC0_DMA_CH->DMA_CH0_Tx_Control | 0x1;

}

//--------------------------------------xec1 DMA RX-----------------------------------------------------------
void ETH1_MACTransmissionCmd(FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    /* Enable the MAC transmission */
	  XEC1_MAC->MAC_Configuration  = XEC1_MAC->MAC_Configuration | 0x1<<1;
	  XEC1_MAC->MAC_Configuration  = XEC1_MAC->MAC_Configuration | 0x1<<1 | 0x1<<29 | 0x1<<1;   //tx mac address replacement
	  XEC1_MAC->MAC_Configuration  = XEC1_MAC->MAC_Configuration | 0x1<<1 | 0x1<<16 | 0x1<<19;    //jumbo pkt

  }
  else
  {
    /* Disable the MAC transmission */
	  XEC1_MAC->MAC_Configuration =  XEC1_MAC->MAC_Configuration | 0x0<<0 ;
  }
}

/**
  * @brief  Enables or disables the MAC reception.
  * @param  NewState: new state of the MAC reception.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ETH1_MACReceptionCmd(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the MAC reception */
	  XEC1_MAC->MAC_Configuration =XEC1_MAC->MAC_Configuration | 0x1<<0;
  }
  else
  {
    /* Disable the MAC reception */
	  XEC1_MAC->MAC_Configuration = XEC1_MAC->MAC_Configuration | 0x0<<0;
  }
}


void ETH1_Init(void)
{

  //  printf("ETH_Init\r\n");
    uint32_t ID_val=0 , CR_val=0, SR_val=0 ,reg0_val=0;
    XEC1_MACAddr->MAC_Address0_Low =0x2b000801;
    XEC1_MACAddr->MAC_Address0_High =0x2220;
    XEC1_MACAddr->MAC_Address1_Low =0x2b000801;
    XEC1_MACAddr->MAC_Address1_High =0x2221;

    XEC1_MAC->MAC_Packet_Filter = 0x1<<31|                           //
    		                      // 0x0<<31|
                                  // 1<<16|                              //vlan filter
                                   0<<15|
                                   0<<8|
                                   0<<7|
                                   0<<6|
                                   0x0<<4|
                                   0x0<<2|
								  // 0x0<<0;                         //filter
                                   0x1<<0;                           //f811_0008  pass all incoming pkt irrespective sa/da

    XEC1_Interrupt->MAC_Interrupt_Enable = 0x0<<0|
                           0x0<<2|
                           0x0<<5|
                           0x0<<4|
                           0x0<<9|
                           0x1<<14;                                  //f811_00b4

    //XEC0_MAC->MAC_VLAN_Tag = 0x1<<27  | 0x1<<16 | 0x1<<20 | 0x1<<2;              //vlan filter right setting
  //  XEC0_MAC->MAC_VLAN_Tag_Data =0x1<<27 | 0x1<<16 | 0x1<<2;              //vlan filter

  //  XEC1_MAC->MAC_Configuration = XEC1_MAC->MAC_Configuration | 0x1 <<12;   //internal loopback
  //   ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x6000);  //f804_0200  LOOPBACK 100M
   // ETH_WritePHYRegister(PHY_DEVICE_ADDRESS,0x0,0x4000);  //f804_0200  phy LOOPBACK 10M
    ETH1_MACTransmissionCmd(ENABLE);
    ETH1_MACReceptionCmd(ENABLE);

    XEC1_EQOS_MTL_Q0->MTL_RxQ0_Operation_Mode =XEC0_EQOS_MTL_Q0->MTL_RxQ0_Operation_Mode | 0x20; //F804_0d30
    XEC1_EQOS_MTL_Q0->MTL_TxQ0_Operation_Mode=XEC0_EQOS_MTL_Q0->MTL_TxQ0_Operation_Mode | 0x1<<1;

}



void ETH1_DMARxDescChainInit()
{
	uint32_t tempreg = 0;
	 uint32_t reg0_val = 0;
		tempreg = XEC1_DMA-> DMA_Mode;                         //0xf811_1000
		XEC1_DMA-> DMA_Mode = tempreg | (uint32_t)0x1;
		do
		  {
			tempreg = XEC1_DMA-> DMA_Mode;
		  } while (tempreg & 0x1);
		XEC1_MAC->MAC_Configuration = XEC1_MAC->MAC_Configuration | 0x1<<15 | 0x1<<14 | 0x1 <<13 ; //100Mbps
	//	XEC1_MAC->MAC_Configuration = XEC1_MAC->MAC_Configuration | 0x1<<15 ; //10Mbps
	    XEC1_DMA->DMA_SysBus_Mode = XEC1_DMA->DMA_SysBus_Mode | 0x1<<12;   //AAL   0xf810_1004
	    XEC1_EQOS_MTL_Q0->MTL_RxQ0_Operation_Mode = XEC1_EQOS_MTL_Q0->MTL_RxQ0_Operation_Mode | 0x1<<1;
		//XEC0_DMA->DMA_SysBus_Mode = XEC0_DMA->DMA_SysBus_Mode | 0x1 <<31;
		XEC1_DMA_CH->DMA_CH0_RxDesc_Ring_Length = (uint32_t)0x10;       //       0xf810_1130
		 memset(DMATx1DscrTab,0,sizeof(ETH_DMADESCTypeDef) * ETH_TXBUFNB);
		 memset(DMARx1DscrTab,0,sizeof(ETH_DMADESCTypeDef) * ETH_RXBUFNB);
	ETH1_DMADESCRx_Index=0;
    for(int i=0;i<ETH_RXBUFNB;i++)
    {
        DMARx1DscrTab[i].DTYPE_SPEC_0 = (uint32_t)&Rx_Buff1[i];
        DMARx1DscrTab[i].COMM = DMARx1DscrTab[i].COMM | 0x1<<31 | 0x1<<24 | 0x1<<30;// bit30:enable interrupt
    }
    DMARx1DescToSet = (ETH_DMADESCTypeDef*) &(DMARx1DscrTab[ETH1_DMADESCRx_Index]);
    XEC1_DMA_CH->DMA_CH0_RxDesc_List_Address = (uint32_t) DMARx1DscrTab;                                       //0xf811_111c   0xc101_0198     0xc101_0254
    XEC1_DMA_CH->DMA_CH0_RxDesc_Tail_Pointer = XEC1_DMA_CH->DMA_CH0_RxDesc_List_Address + (uint32_t)0x40;     //0xf811_1128

	//XEC0_DMA_CH->DMA_CH0_TxDesc_List_Address = 0x0;//XEC0_DMA_CH->DMA_CH0_TxDesc_List_Address | 0x1 <<10;
                                                              //
	XEC1_DMA_CH->DMA_CH0_Control = XEC1_DMA_CH->DMA_CH0_Control | 0x1<<16;                                  //0xf811_1100
	XEC1_DMA_CH->DMA_CH0_Interrupt_Enable |= 0x1<<15 | 0x1<<6;    //DMA_CH(#i)_Interrupt_Enable  15:NIE 6:RIE
	XEC1_DMA_CH->DMA_CH0_Rx_Control = 0x17ff9;                                                              //0xf811_1108  start

}

void ETH1_DMATxDescChainInit()
{
	XEC1_DMA_CH->DMA_CH0_TxDesc_Ring_Length = (uint32_t)0x8;                            //0xf811_112c
	//XEC0_DMA_CH->DMA_CH0_RxDesc_List_Address= 0x0;

    ETH1_DMADESC_Index=0;
    for(int i=0;i<ETH_TXBUFNB;i++)
    {
        DMATx1DscrTab[i].DTYPE_SPEC_0 = (uint32_t)&Tx_Buff1[i];
        DMATx1DscrTab[i].DTYPE_SPEC_1 =  0;
        DMATx1DscrTab[i].DTYPE_SPEC_2 = 0x28;// 0xc00;//0x97;//FrameLength-1;   //0x40 crc pad
        DMATx1DscrTab[i].COMM = 0xb0000028;//0xb0000097;//(FrameLength-1);//0x1 <<31;;//DMATxDscrTab[i].COMM |  0x1 <<31 | 0x1 <<28 | 0x1 <<29 | (ETH_TX_BUF_SIZE) <<0;   //0X5F8=1528


    }

    DMATx1DescToSet = (ETH_DMADESCTypeDef*) &(DMATx1DscrTab[ETH_DMADESC_Index]);
    XEC1_DMA_CH->DMA_CH0_TxDesc_List_Address = (uint32_t)DMATx1DscrTab;                                          //0xf804_1114       c101_0218
                                                     //0xf804_112c
    XEC1_DMA_CH->DMA_CH0_TxDesc_Tail_Pointer = XEC1_DMA_CH->DMA_CH0_TxDesc_List_Address + 0x40;//ETH_DMADESC_Index < (TX_RING_LEN-1) ? (ETH_DMADESC_Index+1) * (0x10) : 0;
    XEC1_DMA_CH->DMA_CH0_TxDesc_Ring_Length = (uint32_t)0x8;

}

FrameTypeDef ETH1_Rx_Packet(void)
{
    uint32_t framelength=0;
    uint32_t Rx_Tail;
    uint32_t rx_status,reg0_val;
    FrameTypeDef frame={0,0};
    uint8_t * buffer;

            frame.length = DMATx1DescToSet->DTYPE_SPEC_2 & 0x7fff  ;
            frame.buffer = (uint32_t)&Rx_Buff1[ETH1_DMADESCRx_Index];   //(uint32_t)
            frame.descriptor =  DMARx1DescToSet;
        buffer = (uint8_t *)frame.buffer;

    if ((ETH1_DMADESCRx_Index+1)== RX_RING_LEN)
    {
        ETH1_DMADESCRx_Index = 0;
    }else
    {
        ETH1_DMADESCRx_Index++;
    }

    DMARx1DescToSet = (ETH_DMADESCTypeDef*) &(DMARx1DscrTab[ETH1_DMADESCRx_Index]);

    return frame;
}



uint8_t ETH1_Tx_Packet(uint32_t FrameLength)
{
    static uint8_t count=0;

   // while((DMATxDescToSet->COMM & DINFO_HW)== SET);
 //ian fang   printf("ETH_Tx_Packet successfully\r\n");
                   //0xf804_1120
    XEC1_DMA_CH->DMA_CH0_Tx_Control = XEC1_DMA_CH->DMA_CH0_Tx_Control | 0x1 | 0x1<<4;    //DMA transmit    start                              //f811_1104
   //ETH_IOCTxCmd(ENABLE);

    if ((ETH1_DMADESC_Index+1)== TX_RING_LEN)
    {
        ETH1_DMADESC_Index = 0;
    }else
    {
        ETH1_DMADESC_Index++;
    }

    DMATx1DescToSet = (ETH_DMADESCTypeDef*) &(DMATx1DscrTab[ETH1_DMADESC_Index]);

}
//--------------------------------------------------------------------------------------------------------



//void IOC_CHx_Interrupt_Enable( ETH_IOC_CHXTypeDef * IOC_CHx ,uint32_t IT)
//{
 //   IOC_CHx->IOC_CHX_IT_ENABLE |= IT;
//}

/******************************************************************************/
/*                                PHY functions                               */
/******************************************************************************/

/**
  * @brief  Read a PHY register
  * @param PHYAddress: PHY device address, is the index of one of supported 32 PHY devices.
  *   This parameter can be one of the following values: 0,..,31
  * @param PHYReg: PHY register address, is the index of one of the 32 PHY register.
  *   This parameter can be one of the following values:
  *     @arg PHY_BCR: Transceiver Basic Control Register
  *     @arg PHY_BSR: Transceiver Basic Status Register
  *     @arg PHY_SR : Transceiver Status Register
  *     @arg More PHY register could be read depending on the used PHY
  * @retval ETH_ERROR: in case of timeout
  *         MAC MIIDR register value: Data read from the selected PHY register (correct read )
  */
uint16_t ETH_ReadPHYRegister(uint16_t PHYAddress, uint16_t PHYReg)
{
  uint32_t tmpreg = 0;
  __IO uint32_t timeout = 0;

  /* Get the ETHERNET MACMIIAR value */
  tmpreg = XEC0_Mdio->MAC_MDIO_Address;
  tmpreg= tmpreg | 1<<9;
  /* Keep only the CSR Clock Range CR[2:0] bits value */
  tmpreg &= 0X0000ff00;//~MAC_MDIO_CS_MASK;
  /* Prepare the MII address register value */
  tmpreg |=((PHYAddress<<21) & MAC_MDIO_PA); /* Set the PHY device address */
  tmpreg |=((PHYReg<<16) & MAC_MDIO_RDA);      /* Set the PHY register address */
  //tmpreg |=((uint32_t)PHYReg<<16);
  //tmpreg &= ~MAC_MDIO_OP;                              /* Set the read mode */
  tmpreg |= MAC_MDIO_OP_READ;
 // tmpreg |=2<<29;                            /* PRE_SUP_SEL 2¡¯b10: 32bits Preamble bit */

  tmpreg |= MAC_MDIO_PRE_BUSY; /* Set the MII Busy bit */
  /* Write the result value into the MII Address register */

  XEC0_Mdio->MAC_MDIO_Address = tmpreg;
  /* Check for the Busy flag */
  do
  {
    timeout++;
    tmpreg =  XEC0_Mdio->MAC_MDIO_Address;
  } while ((tmpreg & MAC_MDIO_PRE_BUSY) && (timeout < (uint32_t)PHY_READ_TO));
  /* Return ERROR in case of timeout */
  if(timeout == PHY_READ_TO)
  {
	  return (uint16_t)(XEC0_Mdio->MAC_MDIO_Data) ;
	  //    return (uint16_t)ERROR;
  }

  /* Return data register value */
  return (uint16_t)(XEC0_Mdio->MAC_MDIO_Data) ;
}

//uint32_t ETH_Get_ITStatus(uint32_t status)
//{
//  return  XEC1_IOC_CHX->IOC_CHX_IT_STATUS & status;
//}

//uint32_t ETH_Clear_ITStatus(uint32_t status)
//{
//    XEC1_IOC_CHX->IOC_CHX_IT_STATUS &= ~status;
//}

/**
  * @brief  Write to a PHY register
  * @param PHYAddress: PHY device address, is the index of one of supported 32 PHY devices.
  *   This parameter can be one of the following values: 0,..,31
  * @param PHYReg: PHY register address, is the index of one of the 32 PHY register.
  *   This parameter can be one of the following values:
  *     @arg PHY_BCR    : Transceiver Control Register
  *     @arg More PHY register could be written depending on the used PHY
  * @param  PHYValue: the value to write
  * @retval ETH_ERROR: in case of timeout
  *         ETH_SUCCESS: for correct write
  */
uint32_t ETH_WritePHYRegister(uint16_t PHYAddress, uint16_t PHYReg, uint16_t PHYValue)
{
  uint32_t tmpreg = 0;
  __IO uint32_t timeout = 0;

  /* Get the ETHERNET MACMIIAR value */
  tmpreg = XEC0_Mdio->MAC_MDIO_Address;
  tmpreg= tmpreg | 1<<9;
  /* Keep only the CSR Clock Range CR[2:0] bits value */
  tmpreg &= 0X0000ff00; //tmpreg &= ~MAC_MDIO_CS_MASK;
  /* Prepare the MII register address value */
  tmpreg |=((uint32_t)PHYAddress<<21); /* Set the PHY device address */
  tmpreg |=((uint32_t)PHYReg<<16);      /* Set the PHY register address */
  //tmpreg &= ~MAC_MDIO_OP;
  tmpreg |= 0X1<<2;                            /* Set the write mode */
  //tmpreg |= 2<<29;                            /* PRE_SUP_SEL 2¡¯b10: 32bits Preamble bit */
  tmpreg |= MAC_MDIO_GB_BUSY;
  /* Give the value to the MII data register */
  XEC0_Mdio->MAC_MDIO_Data = PHYValue;
  /* Write the result value into the MII Address register */
  XEC0_Mdio->MAC_MDIO_Address= tmpreg;
  /* Check for the Busy flag */
  do
  {
    timeout++;
    tmpreg = XEC0_Mdio->MAC_MDIO_Address;
  } while ((tmpreg & MAC_MDIO_GB_BUSY) && (timeout < (uint32_t)PHY_WRITE_TO));
  /* Return ERROR in case of timeout */
 // if(timeout == PHY_WRITE_TO)
  //{
 //   printf("fail\r\n");
 //   return ERROR;
//  }

  /* Return SUCCESS */
  return SUCCESS;
}
