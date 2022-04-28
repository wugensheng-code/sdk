/**
  ******************************************************************************
  * @file    usbd_core.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   This file provides all the USBD core functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_req.h"
#include "usbd_ioreq.h"
#include "usb_dcd_int.h"
#include "usb_bsp.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
* @{
*/


/** @defgroup USBD_CORE 
* @brief usbd core module
* @{
*/ 

/** @defgroup USBD_CORE_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBD_CORE_Private_Defines
* @{
*/ 

/**
* @}
*/ 


/** @defgroup USBD_CORE_Private_Macros
* @{
*/ 
/**
* @}
*/ 




/** @defgroup USBD_CORE_Private_FunctionPrototypes
* @{
*/ 
static uint8_t USBD_SetupStage(USB_OTG_CORE_HANDLE *pdev);
static uint8_t USBD_DataOutStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
static uint8_t USBD_DataInStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
static uint8_t USBD_SOF(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Reset(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Suspend(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Resume(USB_OTG_CORE_HANDLE  *pdev);
#ifdef VBUS_SENSING_ENABLED
static uint8_t USBD_DevConnected(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_DevDisconnected(USB_OTG_CORE_HANDLE  *pdev);
#endif
static uint8_t USBD_IsoINIncomplete(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t  USBD_RunTestMode (USB_OTG_CORE_HANDLE  *pdev) ;
/**
* @}
*/ 

/** @defgroup USBD_CORE_Private_Variables
* @{
*/ 

__IO USB_OTG_DCTL_TypeDef SET_TEST_MODE;

USBD_DCD_INT_cb_TypeDef USBD_DCD_INT_cb = 
{
  USBD_DataOutStage,
  USBD_DataInStage,
  USBD_SetupStage,
  USBD_SOF,
  USBD_Reset,
  USBD_Suspend,
  USBD_Resume,
  USBD_IsoINIncomplete,
  USBD_IsoOUTIncomplete,
#ifdef VBUS_SENSING_ENABLED
  USBD_DevConnected, 
  USBD_DevDisconnected,    
#endif  
};

USBD_DCD_INT_cb_TypeDef  *USBD_DCD_INT_fops = &USBD_DCD_INT_cb;
/**
* @}
*/ 

/** @defgroup USBD_CORE_Private_Functions
* @{
*/ 

/**
* @brief  USBD_Init
*         Initializes the device stack and load the class driver
* @param  pdev: device instance
* @param  core_address: USB OTG core ID
* @param  class_cb: Class callback structure address
* @param  usr_cb: User callback structure address
* @retval None
*/
void USBD_Init(USB_OTG_CORE_HANDLE *pdev,
               USB_OTG_CORE_ID_TypeDef coreID,
               USBD_DEVICE *pDevice,                  
               USBD_Class_cb_TypeDef *class_cb, 
               USBD_Usr_cb_TypeDef *usr_cb)
{
  /* Hardware Init */
  //USB_OTG_BSP_Init(pdev);  //do nothing hcx_add
  
  //USBD_DeInit(pdev); //do nothing hcx_add
  
  /*Register class and user callbacks */
  pdev->dev.class_cb = class_cb;
  pdev->dev.usr_cb = usr_cb;
  pdev->dev.usr_device = pDevice;
  /* set USB OTG core params */
  DCD_Init(pdev , coreID);
  /* Upon Init call usr callback */
  pdev->dev.usr_cb->Init();
  /* Enable Interrupts */
  USB_OTG_BSP_EnableInterrupt(pdev);
}

/**
* @brief  USBD_DeInit 
*         Re-Initialize the device library
* @param  pdev: device instance
* @retval status: status
*/
USBD_Status USBD_DeInit(USB_OTG_CORE_HANDLE *pdev)
{
  /* Software Init */
  
  return USBD_OK;
}

/**
* @brief  USBD_SetupStage 
*         Handle the setup stage
* @param  pdev: device instance
* @retval status
*/
static uint8_t USBD_SetupStage(USB_OTG_CORE_HANDLE *pdev)
{
  USB_SETUP_REQ req;
  
  USBD_ParseSetupRequest(pdev , &req);

#if 1
	int i=0;
	printf("8 { ");
  		for (i=0; i<8; i++)
  			printf("%02x ", pdev->dev.setup_packet[i]);
	printf("}0x%x \r\n", req.bmRequest & 0x1F);

	//printf("req.bmRequest : 0x%x   0x%x \r\n",req.bmRequest, req.bmRequest & 0x1F);
#endif

	//printf(" USBD_SetupStage req.bmRequest & 0x1F : 0x%x \r\n", req.bmRequest & 0x1F);
  switch (req.bmRequest & 0x1F)
  {
  case USB_REQ_RECIPIENT_DEVICE: //0x00
	  printf("  ==> 0 doepint.b.setup USB_REQ_RECIPIENT_DEVICE 0x00 \r\n");
    USBD_StdDevReq (pdev, &req);
    break;
    
  case USB_REQ_RECIPIENT_INTERFACE: //0x01
	  printf("  ==> 0 doepint.b.setup USB_REQ_RECIPIENT_INTERFACE 0x011 \r\n");
    USBD_StdItfReq(pdev, &req);
    break;
    
  case USB_REQ_RECIPIENT_ENDPOINT: //0x02
	  printf("  ==> 0 doepint.b.setup USB_REQ_RECIPIENT_ENDPOINT 0x02 \r\n");
    USBD_StdEPReq(pdev, &req);   
    break;
    
  default:
	  printf("  ==> 0 doepint.b.setup default 3 \r\n");
    DCD_EP_Stall(pdev , req.bmRequest & 0x80);
    break;
  }  
  return USBD_OK;
}

/**
* @brief  USBD_DataOutStage 
*         Handle data out stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
static uint8_t USBD_DataOutStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum)
{
  USB_OTG_EP *ep;
  //printf("==> USBD_DataOutStage 0 \r\n");
  if(epnum == 0) 
  {//printf("==> USBD_DataOutStage 00 \r\n"); //running null
    ep = &pdev->dev.out_ep[0];
    if ( pdev->dev.device_state == USB_OTG_EP0_DATA_OUT)
    {//printf("==> USBD_DataOutStage 000 \r\n");
      if(ep->rem_data_len > ep->maxpacket)
      {//printf("==> USBD_DataOutStage 1 \r\n");
        ep->rem_data_len -=  ep->maxpacket;
        
        if(pdev->cfg.dma_enable == 1)
        {
          /* in slave mode this, is handled by the RxSTSQLvl ISR */
          ep->xfer_buff += ep->maxpacket; 
        }
        //printf("==> USBD_DataOutStage 2 \r\n");
        USBD_CtlContinueRx (pdev, 
                            ep->xfer_buff,
                            MIN(ep->rem_data_len ,ep->maxpacket));
      }
      else
      {//printf("==> USBD_DataOutStage 3 \r\n");
        if((pdev->dev.class_cb->EP0_RxReady != NULL)&&
           (pdev->dev.device_status == USB_OTG_CONFIGURED))
        {//printf("==> USBD_DataOutStage 4 \r\n");
          pdev->dev.class_cb->EP0_RxReady(pdev); 
        }
        printf("==> USBD_DataOutStage USBD_CtlSendStatus \r\n");
        //while(1);
        USBD_CtlSendStatus(pdev);

        //printf("==> USBD_DataOutStage DOEPDMA:0x%08lx\r\n", pdev->regs.OUTEP_REGS[0]->DOEPDMA);
      }
    }
  }
  else if((pdev->dev.class_cb->DataOut != NULL)&&
          (pdev->dev.device_status == USB_OTG_CONFIGURED))
  {//goto usbd_cdc_DataOut
	  //printf("==> USBD_DataOutStage 6 \r\n");
    pdev->dev.class_cb->DataOut(pdev, epnum); 
  }
  else
  {
	  //printf("==> USBD_DataOutStage do nothing \r\n");
    /* Do Nothing */
  }

  //printf("==> USBD_DataOutStage DOEPDMA:0x%08lx\r\n", pdev->regs.OUTEP_REGS[0]->DOEPDMA);
  return USBD_OK;
}

/**
* @brief  USBD_DataInStage 
*         Handle data in stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/ //DataInStage
static uint8_t USBD_DataInStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum)
{
  USB_OTG_EP *ep;
  int i=0;
  if(epnum == 0) 
  {
    ep = &pdev->dev.in_ep[0];
    if ( pdev->dev.device_state == USB_OTG_EP0_DATA_IN)//2
    {//printf(" USBD_DataInStage = 00 \r\n");
      if(ep->rem_data_len > ep->maxpacket)
      {
        ep->rem_data_len -=  ep->maxpacket;
        if(pdev->cfg.dma_enable == 1)
        {
          /* in slave mode this, is handled by the TxFifoEmpty ISR */
          ep->xfer_buff += ep->maxpacket;
        }
        //printf("--> USBD_DataInStage 01 \r\n");
#if 0
	//int i=0;
	printf("USBD_CtlContinueSendData %d{ ", ep->rem_data_len);
  		for (i=0; i<ep->rem_data_len; i++)
  			printf("%02x ", ep->xfer_buff[i]);
	printf("}\r\n");
#endif
        USBD_CtlContinueSendData (pdev, 
                                  ep->xfer_buff, 
                                  ep->rem_data_len);
        
        /* Start the transfer */  
        //printf("--> USBD_DataInStage 1 \n");
        DCD_EP_PrepareRx (pdev,
                          0,
                          NULL,
                          0);
        //printf("--> USBD_DataInStage 2 \r\n");
      }
      else
      { /* last packet is MPS multiple, so send ZLP packet */
    	  //printf(" USBD_DataInStage = MPS \r\n");
        if((ep->total_data_len % ep->maxpacket == 0) &&
           (ep->total_data_len >= ep->maxpacket) &&
             (ep->total_data_len < ep->ctl_data_len ))
        {
        	//printf(" last packet is MPS multiple, so send ZLP packet  \r\n");
          USBD_CtlContinueSendData(pdev , NULL, 0);
          ep->ctl_data_len = 0;
          
          /* Start the transfer */  
          DCD_EP_PrepareRx (pdev,
                            0,
                            NULL,
                            0);
        }
        else
        {//printf("--> USBD_DataInStage USBD_CtlReceiveStatus 0 \r\n");
          if((pdev->dev.class_cb->EP0_TxSent != NULL)&&
             (pdev->dev.device_status == USB_OTG_CONFIGURED))
          {//printf("--> USBD_DataInStage USBD_CtlReceiveStatus 1 \r\n");
            pdev->dev.class_cb->EP0_TxSent(pdev); 
          }          
          USBD_CtlReceiveStatus(pdev);
        }
      }
    }
    if (pdev->dev.test_mode == 1)
    {//printf("--> USBD_DataInStage 5 \r\n");
      USBD_RunTestMode(pdev); 
      pdev->dev.test_mode = 0;
    }
  }
  else if((pdev->dev.class_cb->DataIn != NULL)&& 
          (pdev->dev.device_status == USB_OTG_CONFIGURED))
  {//printf("--> USBD_DataInStage 6 \r\n");
  //goto usbd_cdc_DataIn
    pdev->dev.class_cb->DataIn(pdev, epnum); 
    //printf("--> USBD_DataInStage 7 \n");
  }
  
  else
  {//printf("--> USBD_DataInStage 8 do nothing\r \n");
    /* Do Nothing */
  }
  return USBD_OK;
}




/**
* @brief  USBD_RunTestMode 
*         Launch test mode process
* @param  pdev: device instance
* @retval status
*/
static uint8_t  USBD_RunTestMode (USB_OTG_CORE_HANDLE  *pdev) 
{
  USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DCTL, SET_TEST_MODE.d32);
  return USBD_OK;  
}

/**
* @brief  USBD_Reset 
*         Handle Reset event
* @param  pdev: device instance
* @retval status
*/

static uint8_t USBD_Reset(USB_OTG_CORE_HANDLE  *pdev)
{
  /* Open EP0 OUT */
  DCD_EP_Open(pdev,
              0x00,
              USB_OTG_MAX_EP0_SIZE,
              EP_TYPE_CTRL);
  
  /* Open EP0 IN */
  DCD_EP_Open(pdev,
              0x80,
              USB_OTG_MAX_EP0_SIZE,
              EP_TYPE_CTRL);
  
  /* Upon Reset call usr call back */
  pdev->dev.device_status = USB_OTG_DEFAULT;
  //goto USBD_USR_DeviceReset
  pdev->dev.usr_cb->DeviceReset(pdev->cfg.speed);
  
  return USBD_OK;
}

/**
* @brief  USBD_Resume 
*         Handle Resume event
* @param  pdev: device instance
* @retval status
*/

static uint8_t USBD_Resume(USB_OTG_CORE_HANDLE  *pdev)
{
  /* Upon Resume call usr call back */
  pdev->dev.usr_cb->DeviceResumed(); 
  pdev->dev.device_status = pdev->dev.device_old_status;  
  pdev->dev.device_status = USB_OTG_CONFIGURED;  
  return USBD_OK;
}


/**
* @brief  USBD_Suspend 
*         Handle Suspend event
* @param  pdev: device instance
* @retval status
*/

static uint8_t USBD_Suspend(USB_OTG_CORE_HANDLE  *pdev)
{
  pdev->dev.device_old_status = pdev->dev.device_status;
  pdev->dev.device_status  = USB_OTG_SUSPENDED;
  /* Upon Resume call usr call back */
  pdev->dev.usr_cb->DeviceSuspended(); 
  return USBD_OK;
}


/**
* @brief  USBD_SOF 
*         Handle SOF event
* @param  pdev: device instance
* @retval status
*/

static uint8_t USBD_SOF(USB_OTG_CORE_HANDLE  *pdev)
{
  if(pdev->dev.class_cb->SOF)
  {
	  //goto usbd_cdc_SOF
    pdev->dev.class_cb->SOF(pdev); 
  }
  return USBD_OK;
}
/**
* @brief  USBD_SetCfg 
*        Configure device and start the interface
* @param  pdev: device instance
* @param  cfgidx: configuration index
* @retval status
*/

USBD_Status USBD_SetCfg(USB_OTG_CORE_HANDLE  *pdev, uint8_t cfgidx)
{
  pdev->dev.class_cb->Init(pdev, cfgidx); 
  
  /* Upon set config call usr call back */
  pdev->dev.usr_cb->DeviceConfigured();
  return USBD_OK; 
}

/**
* @brief  USBD_ClrCfg 
*         Clear current configuration
* @param  pdev: device instance
* @param  cfgidx: configuration index
* @retval status: USBD_Status
*/
USBD_Status USBD_ClrCfg(USB_OTG_CORE_HANDLE  *pdev, uint8_t cfgidx)
{
  pdev->dev.class_cb->DeInit(pdev, cfgidx);   
  return USBD_OK;
}

/**
* @brief  USBD_IsoINIncomplete 
*         Handle iso in incomplete event
* @param  pdev: device instance
* @retval status
*/
static uint8_t USBD_IsoINIncomplete(USB_OTG_CORE_HANDLE  *pdev)
{
	//goto NULL
  pdev->dev.class_cb->IsoINIncomplete(pdev);   
  return USBD_OK;
}

/**
* @brief  USBD_IsoOUTIncomplete 
*         Handle iso out incomplete event
* @param  pdev: device instance
* @retval status
*/
static uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE  *pdev)
{
	//goto NULL
  pdev->dev.class_cb->IsoOUTIncomplete(pdev);   
  return USBD_OK;
}

#ifdef VBUS_SENSING_ENABLED
/**
* @brief  USBD_DevConnected 
*         Handle device connection event
* @param  pdev: device instance
* @retval status
*/
static uint8_t USBD_DevConnected(USB_OTG_CORE_HANDLE  *pdev)
{
  pdev->dev.usr_cb->DeviceConnected();
  pdev->dev.connection_status = 1;  
  return USBD_OK;
}

/**
* @brief  USBD_DevDisconnected 
*         Handle device disconnection event
* @param  pdev: device instance
* @retval status
*/
static uint8_t USBD_DevDisconnected(USB_OTG_CORE_HANDLE  *pdev)
{
  pdev->dev.usr_cb->DeviceDisconnected();
  pdev->dev.class_cb->DeInit(pdev, 0);
  pdev->dev.connection_status = 0;    
  return USBD_OK;
}
#endif
/**
* @}
*/ 


/**
* @}
*/ 


/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

