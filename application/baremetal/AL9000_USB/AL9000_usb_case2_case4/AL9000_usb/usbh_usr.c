#include "usbh_usr.h" 
#include "usb_hcd_int.h"
#include "delay.h"  
#include "string.h" 


static u8 AppState; 
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;

//USB OTG interrupt handler
//process all interrupt
void OTG_FS_IRQHandler(void)
{
    USBH_OTG_ISR_Handler(&USB_OTG_Core);
} 
//USB HOST user callback function
USBH_Usr_cb_TypeDef USR_cb =
{
    USBH_USR_Init,
    USBH_USR_DeInit,
    USBH_USR_DeviceAttached,
    USBH_USR_ResetDevice,
    USBH_USR_DeviceDisconnected,
    USBH_USR_OverCurrentDetected,
    USBH_USR_DeviceSpeedDetected,
    USBH_USR_Device_DescAvailable,
    USBH_USR_DeviceAddressAssigned,
    USBH_USR_Configuration_DescAvailable,
    USBH_USR_Manufacturer_String,
    USBH_USR_Product_String,
    USBH_USR_SerialNum_String,
    USBH_USR_EnumerationDone,
    USBH_USR_UserInput,
    USBH_USR_MSC_Application,
    USBH_USR_DeviceNotSupported,
    USBH_USR_UnrecoveredError 
}; 
/////////////////////////////////////////////////////////////////////////////////
//user callback.


//USB HOST Initliation 
void USBH_USR_Init(void)
{
    printf("USB OTG HS MSC Host\r\n");
    printf("> USB Host library started.\r\n");
    printf("  USB Host Library v2.1.0\r\n\r\n");
    
}
//detect udisk attach
void USBH_USR_DeviceAttached(void)
{
    printf("Detect udisk insert!\r\n");
}
//Detect udidk remove
void USBH_USR_DeviceDisconnected (void)
{

    printf("USB udisk removed!\r\n");
}  
//
void USBH_USR_ResetDevice(void)
{
    printf("Reset USB...\r\n");
}
//Detect devie speed
//DeviceSpeed:
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
    if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
    {
        printf("(HS) USB!\r\n");
    }  
    else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
    {
        printf("(FS) USB!\r\n"); 
    }
    else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
    {
        printf("(LS) USB!\r\n");  
    }
    else
    {
        printf("USB Speed ERR!\r\n");  
    }
}
//Detect device descriptor
//DeviceDesc:
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
    USBH_DevDesc_TypeDef *hs;
    hs=DeviceDesc;   
    printf("VID: %04Xh\r\n" , (uint32_t)(*hs).idVendor); 
    printf("PID: %04Xh\r\n" , (uint32_t)(*hs).idProduct); 
}
//device address setting
void USBH_USR_DeviceAddressAssigned(void)
{
    printf("Device address setting!\r\n");   
}

void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
    USBH_InterfaceDesc_TypeDef *id; 
    id = itfDesc;   
    if((*id).bInterfaceClass==0x08)
    {
        printf("MSC device configuration descriptor fetch!\r\n"); 
    }else if((*id).bInterfaceClass==0x03)
    {
        printf("HID device configuration descriptor fetch!\r\n"); 
    }    
}
//Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
    printf("Manufacturer: %s\r\n",(char *)ManufacturerString);
}
//Product String 
void USBH_USR_Product_String(void *ProductString)
{
    printf("Product: %s\r\n",(char *)ProductString);  
}
//SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{
    printf("Serial Number: %s\r\n",(char *)SerialNumString);    
} 
//USB enumeration completely
void USBH_USR_EnumerationDone(void)
{ 
    printf("USB enumeration completely!\r\n\r\n");    
} 
//Unkown USB Device
void USBH_USR_DeviceNotSupported(void)
{
    printf("Unkown USB Device!\r\n\r\n");    
}  
//waitting user input
USBH_USR_Status USBH_USR_UserInput(void)
{ 
    printf("Skip user input step!\r\n");
    return USBH_USR_RESP_OK;
} 
//USB over current
void USBH_USR_OverCurrentDetected (void)
{
    printf("USB over current!!!\r\n");
} 

extern u8 USH_User_App(void);       //user main test app
//USB HOST MSC user application
int USBH_USR_MSC_Application(void)
{
    u8 res=0;
    switch(AppState)
    {
        case USH_USR_FS_INIT:  //Initliation file system
            printf("Begin user app!!!\r\n");
            AppState=USH_USR_FS_TEST;
            break;
        case USH_USR_FS_TEST:   //exec USB OTG test app
            res=USH_User_App(); //user app
            if(res)AppState=USH_USR_FS_INIT;
            break;
        default:break;
    } 
    return res;
}
//user reinit 
void USBH_USR_DeInit(void)
{
   // AppState=USH_USR_FS_INIT;
    AppState=USH_USR_FS_INIT;
}

void USBH_USR_UnrecoveredError (void)
{
    printf("Unrostore error!!!\r\n\r\n");   
}
////////////////////////////////////////////////////////////////////////////////////////

extern USBH_HOST              USB_Host;


u8 USBH_UDISK_Status(void)
{
    return HCD_IsDeviceConnected(&USB_OTG_Core);//����U��״̬
}


u8 USBH_UDISK_Read(u8* buf,u32 sector,u32 cnt)
{
    u8 res=1;
    if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//���ӻ�����,����APP����״̬
    {           
        do
        {
            res=USBH_MSC_Read10(&USB_OTG_Core,buf,sector,512*cnt);
            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);             
            if(!HCD_IsDeviceConnected(&USB_OTG_Core))
            {
                res=1;//��д����
                break;
            };   
        }while(res==USBH_MSC_BUSY);
    }else res=1;          
    if(res==USBH_MSC_OK)res=0;  
    return res;
}


u8 USBH_UDISK_Write(u8* buf,u32 sector,u32 cnt)
{
    u8 res=1;
    if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//���ӻ�����,����APP����״̬
    {           
        do
        {
            res=USBH_MSC_Write10(&USB_OTG_Core,buf,sector,512*cnt); 
            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);             
            if(!HCD_IsDeviceConnected(&USB_OTG_Core))
            {
                res=1;//��д����
                break;
            };   
        }while(res==USBH_MSC_BUSY);
    }else res=1;          
    if(res==USBH_MSC_OK)res=0;  
    return res;
}











