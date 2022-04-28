/**
  ******************************************************************************
  * @file    usbd_req.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   This file provides the standard USB requests following chapter 9.
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
#include "usbd_req.h"
#include "usbd_ioreq.h"
#include "usbd_desc.h"


/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_REQ 
  * @brief USB standard requests module
  * @{
  */ 

/** @defgroup USBD_REQ_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_REQ_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBD_REQ_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_REQ_Private_Variables
  * @{
  */ 
extern __IO USB_OTG_DCTL_TypeDef SET_TEST_MODE;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */  
__ALIGN_BEGIN uint32_t USBD_ep_status __ALIGN_END  = 0; 

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint32_t  USBD_default_cfg __ALIGN_END  = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint32_t  USBD_cfg_status __ALIGN_END  = 0;  

/**
  * @}
  */ 


/** @defgroup USBD_REQ_Private_FunctionPrototypes
  * @{
  */ 
static void USBD_GetDescriptor(USB_OTG_CORE_HANDLE  *pdev, 
                               USB_SETUP_REQ *req);

static void USBD_SetAddress(USB_OTG_CORE_HANDLE  *pdev, 
                            USB_SETUP_REQ *req);

static void USBD_SetConfig(USB_OTG_CORE_HANDLE  *pdev, 
                           USB_SETUP_REQ *req);

static void USBD_GetConfig(USB_OTG_CORE_HANDLE  *pdev, 
                           USB_SETUP_REQ *req);

static void USBD_GetStatus(USB_OTG_CORE_HANDLE  *pdev, 
                           USB_SETUP_REQ *req);

static void USBD_SetFeature(USB_OTG_CORE_HANDLE  *pdev, 
                            USB_SETUP_REQ *req);

static void USBD_ClrFeature(USB_OTG_CORE_HANDLE  *pdev, 
                            USB_SETUP_REQ *req);

static uint8_t USBD_GetLen(uint8_t *buf);
/**
  * @}
  */ 


/** @defgroup USBD_REQ_Private_Functions
  * @{
  */ 


/**
* @brief  USBD_StdDevReq
*         Handle standard usb device requests
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
USBD_Status  USBD_StdDevReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req)
{
  USBD_Status ret = USBD_OK;  
  
  switch (req->bRequest) 
  {
  case USB_REQ_GET_DESCRIPTOR: 
	  printf("   ==> 0 doepint.b.setup   USBD_GetDescriptor 6 \r\n");
    USBD_GetDescriptor (pdev, req) ;
    break;
    
  case USB_REQ_SET_ADDRESS:
	  printf("   ==> 0 doepint.b.setup   USB_REQ_SET_ADDRESS 5 \r\n");
    USBD_SetAddress(pdev, req);
    break;
    
  case USB_REQ_SET_CONFIGURATION:
	  printf("   ==> 0 doepint.b.setup   USB_REQ_SET_CONFIGURATION 9 \r\n");
    USBD_SetConfig (pdev , req);
    break;
    
  case USB_REQ_GET_CONFIGURATION:
	  printf("   ==> 0 doepint.b.setup   USB_REQ_GET_CONFIGURATION 8 \r\n");
    USBD_GetConfig (pdev , req);
    break;
    
  case USB_REQ_GET_STATUS:
	  printf("   ==> 0 doepint.b.setup   USB_REQ_GET_STATUS 0 \r\n");
    USBD_GetStatus (pdev , req);
    break;
    
    
  case USB_REQ_SET_FEATURE:   
	  printf("   ==> 0 doepint.b.setup   USB_REQ_SET_FEATURE 3 \r\n");
    USBD_SetFeature (pdev , req);    
    break;
    
  case USB_REQ_CLEAR_FEATURE:
	  printf("   ==> 0 doepint.b.setup   USB_REQ_CLEAR_FEATURE 1 \r\n");
    USBD_ClrFeature (pdev , req);
    break;
    
  default:  
	  printf("   ==> 0 doepint.b.setup   USBD_CtlError 7 \r\n");
    USBD_CtlError(pdev , req);
    break;
  }
  
  return ret;
}
//Handle_USBAsynchXfer

/**
* @brief  USBD_StdItfReq
*         Handle standard usb interface requests
* @param  pdev: USB OTG device instance
* @param  req: usb request
* @retval status
*/
USBD_Status  USBD_StdItfReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req)
{
  USBD_Status ret = USBD_OK; 
  
  //printf(" USBD_StdItfReq device_status : %d\r\n", pdev->dev.device_status);
  switch (pdev->dev.device_status) 
  {
  case USB_OTG_CONFIGURED: //RUNING 3
    
    if (LOBYTE(req->wIndex) <= USBD_ITF_MAX_NUM) 
    {//goto usbd_cdc_Setup
      pdev->dev.class_cb->Setup (pdev, req); 
      //USB_OTG_EP0_OutStart(pdev); //hcx_add
      
      if((req->wLength == 0)&& (ret == USBD_OK))
      {//printf("USBD_StdItfReq USBD_CtlSendStatus \r\n");
         USBD_CtlSendStatus(pdev);
      }
    } 
    else 
    {  printf("USBD_StdItfReq 1 \r\n");
       USBD_CtlError(pdev , req);
    }
    break;
    
  default:
	  //printf("USBD_StdItfReq 2 \r\n");
     USBD_CtlError(pdev , req);
    break;
  }
  return ret;
}

/**
* @brief  USBD_StdEPReq
*         Handle standard usb endpoint requests
* @param  pdev: USB OTG device instance
* @param  req: usb request
* @retval status
*/
USBD_Status  USBD_StdEPReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req)
{
  
  uint8_t   ep_addr;
  int i=0;
  USBD_Status ret = USBD_OK; 
  
  ep_addr  = LOBYTE(req->wIndex);   

  /* Check the class specific requests before going to standard request */
  if ((req->bmRequest & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_CLASS)
  {
    pdev->dev.class_cb->Setup (pdev, req);
    return ret;
  }

  switch (req->bRequest) 
  {  
  case USB_REQ_SET_FEATURE :
    
    switch (pdev->dev.device_status) 
    {
    case USB_OTG_ADDRESSED:          
      if ((ep_addr != 0x00) && (ep_addr != 0x80)) 
      {
        DCD_EP_Stall(pdev , ep_addr);
      }
      break;	
      
    case USB_OTG_CONFIGURED:   
      if (req->wValue == USB_FEATURE_EP_HALT)
      {
        if ((ep_addr != 0x00) && (ep_addr != 0x80)) 
        { 
          DCD_EP_Stall(pdev , ep_addr);
          
        }
      }
      pdev->dev.class_cb->Setup (pdev, req);   
      USBD_CtlSendStatus(pdev);
      
      break;
      
    default:                         
      USBD_CtlError(pdev , req);
      break;    
    }
    break;
    
  case USB_REQ_CLEAR_FEATURE :
    
    switch (pdev->dev.device_status) 
    {
    case USB_OTG_ADDRESSED:          
      if ((ep_addr != 0x00) && (ep_addr != 0x80)) 
      {
        DCD_EP_Stall(pdev , ep_addr);
      }
      break;	
      
    case USB_OTG_CONFIGURED:   
      if (req->wValue == USB_FEATURE_EP_HALT)
      {
        if ((ep_addr != 0x00) && (ep_addr != 0x80)) 
        {        
          DCD_EP_ClrStall(pdev , ep_addr);
          pdev->dev.class_cb->Setup (pdev, req);
        }
        USBD_CtlSendStatus(pdev);
      }
      break;
      
    default:                         
       USBD_CtlError(pdev , req);
      break;    
    }
    break;
    
  case USB_REQ_GET_STATUS:                  
    switch (pdev->dev.device_status) 
    {
    case USB_OTG_ADDRESSED:          
      if ((ep_addr != 0x00) && (ep_addr != 0x80)) 
      {
        DCD_EP_Stall(pdev , ep_addr);
      }
      break;	
      
    case USB_OTG_CONFIGURED:         
      
      
      if ((ep_addr & 0x80)== 0x80)
      {
        if(pdev->dev.in_ep[ep_addr & 0x7F].is_stall)
        {
          USBD_ep_status = 0x0001;     
        }
        else
        {
          USBD_ep_status = 0x0000;  
        }
      }
      else if ((ep_addr & 0x80)== 0x00)
      {
        if(pdev->dev.out_ep[ep_addr].is_stall)
        {
          USBD_ep_status = 0x0001;     
        }
        
        else 
        {
          USBD_ep_status = 0x0000;     
        }      
      }
      
      else
      {
        /* Do Nothing */
      }
      
	printf("\r\nUSBD_StdEPReq USBD_CtlSendData USBD_ep_status len:2\r\n");

      USBD_CtlSendData (pdev,
                        (uint8_t *)&USBD_ep_status,
                        2);
      break;
      
    default:                         
       USBD_CtlError(pdev , req);
      break;
    }
    break;
    
  default:
    break;
  }
  return ret;
}
/**
* @brief  USBD_GetDescriptor
*         Handle Get Descriptor requests
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
static void USBD_GetDescriptor(USB_OTG_CORE_HANDLE  *pdev, 
                               USB_SETUP_REQ *req)
{
  uint16_t len;
  uint8_t *pbuf;
  len = req->wLength ;
    
  switch (req->wValue >> 8)
  {

#if (USBD_LPM_ENABLED == 1)
  case USB_DESC_TYPE_BOS:
    pbuf = pdev->pDesc->GetBOSDescriptor(pdev->dev_speed, &len);
    break;
#endif

  case USB_DESC_TYPE_DEVICE:
	  printf("    ==> 0 doepint.b.setup   USB_DESC_TYPE_DEVICE  0 \r\n");
    pbuf = pdev->dev.usr_device->GetDeviceDescriptor(pdev->cfg.speed, &len);
    break;
    
  case USB_DESC_TYPE_CONFIGURATION:
	  printf("    ==> 0 doepint.b.setup   USB_DESC_TYPE_CONFIGURATION  1 \r\n");
      pbuf   = (uint8_t *)pdev->dev.class_cb->GetConfigDescriptor(pdev->cfg.speed, &len);
#ifdef USB_OTG_HS_CORE
    if((pdev->cfg.speed == USB_OTG_SPEED_FULL )&&
       (pdev->cfg.phy_itface  == USB_OTG_ULPI_PHY))
    {
      pbuf   = (uint8_t *)pdev->dev.class_cb->GetOtherConfigDescriptor(pdev->cfg.speed, &len);
    }
#endif  
    pbuf[1] = USB_DESC_TYPE_CONFIGURATION;
    pdev->dev.pConfig_descriptor = pbuf;
    //len = 32;
    break;
    
  case USB_DESC_TYPE_STRING:
    switch ((uint8_t)(req->wValue))
    {
    case USBD_IDX_LANGID_STR:
    	printf("    ==> 0 doepint.b.setup   USBD_IDX_LANGID_STR  2 \r\n");
     pbuf = pdev->dev.usr_device->GetLangIDStrDescriptor(pdev->cfg.speed, &len);        
      break;
      
    case USBD_IDX_MFC_STR:
    	printf("    ==> 0 doepint.b.setup   USBD_IDX_MFC_STR  3  \r\n");
      pbuf = pdev->dev.usr_device->GetManufacturerStrDescriptor(pdev->cfg.speed, &len);
      break;
      
    case USBD_IDX_PRODUCT_STR:
    	printf("    ==> 0 doepint.b.setup   USBD_IDX_PRODUCT_STR  4 \r\n");
      pbuf = pdev->dev.usr_device->GetProductStrDescriptor(pdev->cfg.speed, &len);
      break;
      
    case USBD_IDX_SERIAL_STR:
    	printf("    ==> 0 doepint.b.setup   USBD_IDX_SERIAL_STR  5 start len:%d \r\n", len);
      pbuf = pdev->dev.usr_device->GetSerialStrDescriptor(pdev->cfg.speed, &len);
      //goto USBD_USR_SerialStrDescriptor
      printf("    ==> 0 doepint.b.setup   USBD_IDX_CONFIG_STR  5 end \r\n");
      break;
      
    case USBD_IDX_CONFIG_STR:
    	printf("    ==> 0 doepint.b.setup   USBD_IDX_CONFIG_STR  6 \r\n");
      pbuf = pdev->dev.usr_device->GetConfigurationStrDescriptor(pdev->cfg.speed, &len);

      break;
      
    case USBD_IDX_INTERFACE_STR:
    	printf("    ==> 0 doepint.b.setup   USBD_IDX_INTERFACE_STR  7 \r\n");
      pbuf = pdev->dev.usr_device->GetInterfaceStrDescriptor(pdev->cfg.speed, &len);
      break;
      
    default:
#ifdef USB_SUPPORT_USER_STRING_DESC
      pbuf = pdev->dev.class_cb->GetUsrStrDescriptor(pdev->cfg.speed, (req->wValue) , &len);
      break;
#else      
       USBD_CtlError(pdev , req);
       printf("    ==> 0 doepint.b.setup   USBD_CtlError  8 \r\n");
      return;
#endif /* USBD_CtlError(pdev , req)*/      
    }
    break;
  case USB_DESC_TYPE_DEVICE_QUALIFIER:                   
#ifdef USB_OTG_HS_CORE
    if(pdev->cfg.speed == USB_OTG_SPEED_HIGH  )   
    {
      
      pbuf   = (uint8_t *)pdev->dev.class_cb->GetConfigDescriptor(pdev->cfg.speed, &len);
            
      USBD_DeviceQualifierDesc[4]= pbuf[14];
      USBD_DeviceQualifierDesc[5]= pbuf[15];
      USBD_DeviceQualifierDesc[6]= pbuf[16];
      
      pbuf = USBD_DeviceQualifierDesc;
      len  = USB_LEN_DEV_QUALIFIER_DESC;
      break;
    }
    else
    {
      USBD_CtlError(pdev , req);
      return;
    }
#else
    printf("    ==> 0 doepint.b.setup   USBD_CtlError  9 \r\n");
      USBD_CtlError(pdev , req);
      return;
#endif    

  case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION:
#ifdef USB_OTG_HS_CORE   
    
    if(pdev->cfg.speed == USB_OTG_SPEED_HIGH  )   
    {
      pbuf   = (uint8_t *)pdev->dev.class_cb->GetOtherConfigDescriptor(pdev->cfg.speed, &len);
      pbuf[1] = USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION;
      break; 
    }
    else
    {
      USBD_CtlError(pdev , req);
      return;
    }
#else
    printf("    ==> 0 doepint.b.setup   USBD_CtlError  10 \r\n");
      USBD_CtlError(pdev , req);
      return;
#endif     
  default: 
	  printf("    ==> 0 doepint.b.setup   USBD_CtlError  11 \r\n");
     USBD_CtlError(pdev , req);
    return;
  }
  
  printf("    ==> 0 doepint.b.setup 8  len:%d req->wLength:%d  \r\n", len, req->wLength);
int i=0;
  if((len != 0)&& (req->wLength != 0))
  {

    len = MIN(len , req->wLength);
    //if(len > 64) len = 63;

#if 1
	//int i=0;
	printf("USBD_CtlSendData Descriptor %d{ ", len);
  		for (i=0; i<len; i++)
  			printf("%02x ", pbuf[i]);
	printf("}\r\n");
#endif

    USBD_CtlSendData (pdev, 
                      pbuf,
                      len);
  }
  
}

/**
* @brief  USBD_SetAddress
*         Set device address
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
static void USBD_SetAddress(USB_OTG_CORE_HANDLE  *pdev, 
                            USB_SETUP_REQ *req)
{
  uint8_t  dev_addr; 
  
  if ((req->wIndex == 0) && (req->wLength == 0)) 
  {
    dev_addr = (uint8_t)(req->wValue) & 0x7F;     
    
    if (pdev->dev.device_status == USB_OTG_CONFIGURED) 
    {
      USBD_CtlError(pdev , req);
    } 
    else 
    {
      pdev->dev.device_address = dev_addr;
      DCD_EP_SetAddress(pdev, dev_addr);               
      USBD_CtlSendStatus(pdev);                         
      
      if (dev_addr != 0) 
      {
        pdev->dev.device_status  = USB_OTG_ADDRESSED;
      } 
      else 
      {
        pdev->dev.device_status  = USB_OTG_DEFAULT; 
      }
    }
  } 
  else 
  {
     USBD_CtlError(pdev , req);                        
  } 
}

/**
* @brief  USBD_SetConfig
*         Handle Set device configuration request
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
static void USBD_SetConfig(USB_OTG_CORE_HANDLE  *pdev, 
                           USB_SETUP_REQ *req)
{
  
  static uint8_t  cfgidx;
  
  cfgidx = (uint8_t)(req->wValue);                 
  
  if (cfgidx > USBD_CFG_MAX_NUM ) 
  {            
     USBD_CtlError(pdev , req);                              
  } 
  else 
  {
    switch (pdev->dev.device_status) 
    {
    case USB_OTG_ADDRESSED:
      if (cfgidx) 
      {                                			   							   							   				
        pdev->dev.device_config = cfgidx;
        pdev->dev.device_status = USB_OTG_CONFIGURED;
        USBD_SetCfg(pdev , cfgidx);
        USBD_CtlSendStatus(pdev);
      }
      else 
      {
         USBD_CtlSendStatus(pdev);
      }
      break;
      
    case USB_OTG_CONFIGURED:
      if (cfgidx == 0) 
      {                           
        pdev->dev.device_status = USB_OTG_ADDRESSED;
        pdev->dev.device_config = cfgidx;          
        USBD_ClrCfg(pdev , cfgidx);
        USBD_CtlSendStatus(pdev);
        
      } 
      else  if (cfgidx != pdev->dev.device_config) 
      {
        /* Clear old configuration */
        USBD_ClrCfg(pdev , pdev->dev.device_config);
        
        /* set new configuration */
        pdev->dev.device_config = cfgidx;
        USBD_SetCfg(pdev , cfgidx);
        USBD_CtlSendStatus(pdev);
      }
      else
      {
        USBD_CtlSendStatus(pdev);
      }
      break;
      
    default:					
       USBD_CtlError(pdev , req);                     
      break;
    }
  }
}

/**
* @brief  USBD_GetConfig
*         Handle Get device configuration request
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
static void USBD_GetConfig(USB_OTG_CORE_HANDLE  *pdev, 
                           USB_SETUP_REQ *req)
{
	int i=0;
  if (req->wLength != 1) 
  {                   
     USBD_CtlError(pdev , req);
  }
  else 
  {
    switch (pdev->dev.device_status )  
    {
    case USB_OTG_ADDRESSED:

	printf("\r\nUSBD_GetConfig 0 USBD_CtlSendData USBD_default_cfg len:1\r\n");


      USBD_CtlSendData (pdev, 
                        (uint8_t *)&USBD_default_cfg,
                        1);
      break;
      
    case USB_OTG_CONFIGURED:

	printf("\r\nUSBD_GetConfig 1 USBD_CtlSendData device_config len:1 \r\n");

      USBD_CtlSendData (pdev, 
                        &pdev->dev.device_config,
                        1);
      break;
      
    default:
       USBD_CtlError(pdev , req);
      break;
    }
  }
}

/**
* @brief  USBD_GetStatus
*         Handle Get Status request
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
static void USBD_GetStatus(USB_OTG_CORE_HANDLE  *pdev, 
                           USB_SETUP_REQ *req)
{
  int i=0;
    
  switch (pdev->dev.device_status) 
  {
  case USB_OTG_ADDRESSED:
  case USB_OTG_CONFIGURED:
    
#ifdef USBD_SELF_POWERED
    USBD_cfg_status = USB_CONFIG_SELF_POWERED;                                    
#else
    USBD_cfg_status = 0x00;                                    
#endif
                      
    if (pdev->dev.DevRemoteWakeup) 
    {
      USBD_cfg_status |= USB_CONFIG_REMOTE_WAKEUP;                                
    }
    
	printf("\r\nUSBD_GetStatus USBD_CtlSendData USBD_cfg_status len:2\r\n");

    USBD_CtlSendData (pdev, 
                      (uint8_t *)&USBD_cfg_status,
                      2);
    break;
    
  default :
    USBD_CtlError(pdev , req);                        
    break;
  }
}


/**
* @brief  USBD_SetFeature
*         Handle Set device feature request
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
static void USBD_SetFeature(USB_OTG_CORE_HANDLE  *pdev, 
                            USB_SETUP_REQ *req)
{

  USB_OTG_DCTL_TypeDef     dctl;
  uint8_t test_mode = 0;
 
  if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
  {
    pdev->dev.DevRemoteWakeup = 1;  
    pdev->dev.class_cb->Setup (pdev, req);   
    USBD_CtlSendStatus(pdev);
  }

  else if ((req->wValue == USB_FEATURE_TEST_MODE) && 
           ((req->wIndex & 0xFF) == 0))
  {
    dctl.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DCTL);
    
    test_mode = req->wIndex >> 8;
    switch (test_mode) 
    {
    case 1: /* TEST_J */
      dctl.b.tstctl = 1;
      break;
      
    case 2: /* TEST_K */	
      dctl.b.tstctl = 2;
      break;
      
    case 3: /* TEST_SE0_NAK */
      dctl.b.tstctl = 3;
      break;
      
    case 4: /* TEST_PACKET */
      dctl.b.tstctl = 4;
      break;
      
    case 5: /* TEST_FORCE_ENABLE */
      dctl.b.tstctl = 5;
      break;
      
    default :
      dctl.b.tstctl = 1;
      break;
    }
    SET_TEST_MODE = dctl;
    pdev->dev.test_mode = 1;
    USBD_CtlSendStatus(pdev);
  }
  else
  {
    /* Do Nothing */
  }
}


/**
* @brief  USBD_ClrFeature
*         Handle clear device feature request
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
static void USBD_ClrFeature(USB_OTG_CORE_HANDLE  *pdev, 
                            USB_SETUP_REQ *req)
{
  switch (pdev->dev.device_status)
  {
  case USB_OTG_ADDRESSED:
  case USB_OTG_CONFIGURED:
    if (req->wValue == USB_FEATURE_REMOTE_WAKEUP) 
    {
      pdev->dev.DevRemoteWakeup = 0; 
      pdev->dev.class_cb->Setup (pdev, req);   
      USBD_CtlSendStatus(pdev);
    }
    break;
    
  default :
     USBD_CtlError(pdev , req);
    break;
  }
}

/**
* @brief  USBD_ParseSetupRequest 
*         Copy buffer into setup structure
* @param  pdev: device instance
* @param  req: usb request
* @retval None
*/

void USBD_ParseSetupRequest( USB_OTG_CORE_HANDLE  *pdev,
                            USB_SETUP_REQ *req)
{
  req->bmRequest     = *(uint8_t *)  (pdev->dev.setup_packet);
  req->bRequest      = *(uint8_t *)  (pdev->dev.setup_packet +  1);
  req->wValue        = SWAPBYTE      (pdev->dev.setup_packet +  2);
  req->wIndex        = SWAPBYTE      (pdev->dev.setup_packet +  4);
  req->wLength       = SWAPBYTE      (pdev->dev.setup_packet +  6);
  
  pdev->dev.in_ep[0].ctl_data_len = req->wLength  ;
  pdev->dev.device_state = USB_OTG_EP0_SETUP;

  printf("\n******* setup Request receive******\n bmRequest:%x bRequest:%x wValue:%x wIndex:%x wLength:%x\n", req->bmRequest,req->bRequest,req->wValue,req->wIndex,req->wLength);
}

/**
* @brief  USBD_CtlError 
*         Handle USB low level Error
* @param  pdev: device instance
* @param  req: usb request
* @retval None
*/

void USBD_CtlError( USB_OTG_CORE_HANDLE  *pdev,
                            USB_SETUP_REQ *req)
{
	//printf("-》USBD_CtlError\r\n");
  DCD_EP_Stall(pdev , 0x80);
  DCD_EP_Stall(pdev , 0);

  //printf("-out- USBD_CtlError %p \r\n", (uint32_t)&pdev->dev.setup_packet);
  USB_OTG_EP0_OutStart(pdev);  
}


/**
  * @brief  USBD_GetString
  *         Convert Ascii string into unicode one
  * @param  desc : descriptor buffer
  * @param  unicode : Formatted string buffer (unicode)
  * @param  len : descriptor length
  * @retval None
  */
void USBD_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len)
{
  uint8_t idx = 0;
  
  if (desc != NULL) 
  {
    *len =  USBD_GetLen(desc) * 2 + 2;    
    unicode[idx++] = *len;
    unicode[idx++] =  USB_DESC_TYPE_STRING;
    
    while (*desc != NULL) 
    {
      unicode[idx++] = *desc++;
      unicode[idx++] =  0x00;
    }
  } 
}

/**
  * @brief  USBD_GetLen
  *         return the string length
   * @param  buf : pointer to the ascii string buffer
  * @retval string length
  */
static uint8_t USBD_GetLen(uint8_t *buf)
{
    uint8_t  len = 0;

    while (*buf != NULL) 
    {
        len++;
        buf++;
    }

    return len;
}
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
