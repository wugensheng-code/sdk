/***************************** Include Files *********************************/
#include "al_gpio_hw.h"
#include "al_gpio_ll.h"
#include "al_gpio_dev.h"
#include "al_gpio_hal.h"
#include "al_errno.h"
#include "nuclei_sdk_soc.h"
#include "al_intr.h"

/************************** Variable Definitions *****************************/
static AL_GPIO_DevStruct AL_GPIO_DevInstance[AL_GPIO_NUM_INSTANCE];

#if 0
/**
 * @brief  This function sets the status callback function. The callback function is called by the
 *         AL_GPIO_IntrHandler when an interrupt occurs.
 * @param  Gpio is a pointer to the AL_GPIO instance.
 * @param  CallBackRef
 * @param  FunPointer is the pointer to the callback function.
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_IntrCallbackHandler(AL_GPIO_HalStruct *Handle, AL_VOID *CallBackRef, Gpio_Handler FunPointer)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    if (FunPointer == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    Handle->Dev->Handler = FunPointer;
    Handle->Dev->EventCallBackRef = CallBackRef;

    return AL_OK;
}

/**
 * @brief  This function is the interrupt handler for GPIO interrupts.It checks the interrupt status registers
 * of all the banks to determine the actual bank in which interrupt have been triggered. It then calls the
 * upper layer callback handler set by the function AlGpio_Hal_IntrCallbackHandler.
 * @param  Gpio is a pointer to the AL_GPIO instance.
 * @param  Pin is the pin number to which the Data is to be written.
 * @param  IntrType is the IRQ type for GPIO Pin.
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_IntrHandler(AL_GPIO_HalStruct *Handle)
{
    AL_U8 Bank;
	AL_U32 IntrStatus;
	AL_U32 IntrEnable;
    AL_U32 IntrReg;
	AL_U8 PinNumber;

    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    for(Bank = 0U; Bank < Handle->Dev->HwConfig.MaxBanks; Bank++) {
        IntrStatus = AlGpio_Dev_IntrGetStatus(Handle, Bank);
        IntrEnable = AlGpio_Dev_IntrGetEnable(Handle, Bank);
        printf("IntrStatus %d is %x\r\n", Bank, IntrStatus);
        printf("IntrEnable %d is %x\r\n", Bank, IntrEnable);
        if((IntrStatus & IntrEnable) != (AL_U32)0) {
            AlGpio_Dev_IntrClr(Handle, Bank, IntrStatus & IntrEnable);
            Handle->Dev->Handler(Handle->Dev->EventCallBackRef, Bank, (IntrStatus & IntrEnable));
        }
    }

    return AL_OK;
}
#endif

/**************************** Function Prototypes ******************************/
/**
 * @brief  This function initializes a AL_GPIO driver.
 * @param  Handle is a pointer to the AL_GPIO instance.
 * @param  DevId
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_Init(AL_GPIO_HalStruct *Handle, AL_U32 DevId)
{
    AL_S32 ret = AL_OK;
    AL_GPIO_HwConfigStruct *HwConfig;
    AL_GPIO_DevStruct *Dev;

    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    /* 1. look up hardware config */
    HwConfig = AlGpio_Dev_LookupConfig(DevId);
    if(HwConfig != AL_NULL) {
        Handle->Dev = &AL_GPIO_DevInstance[DevId];
    }

    /* 2. Init */
    ret = AlGpio_Dev_Init(Handle->Dev, HwConfig);
    if(ret != AL_OK) {
        //
    }

    /* 3. register intr */
<<<<<<< HEAD
    AL_INTR_HandlerStruct IntrHandler = {
            .Func  = AlGpio_Dev_IntrHandler,
            .Param = (AL_VOID *)(Handle->Dev)
        };
    AL_DEFAULT_ATTR(Attr);
    AlIntr_RegHandler(GPIO0_IRQn, &Attr, &IntrHandler);
=======
    (AL_VOID)AlIntr_RegHandler(GPIO0_IRQn, NULL, AlGpio_Dev_IntrHandler, (AL_VOID *)(Handle->Dev));
>>>>>>> a3b660865b58eae3a8e924a2f7c0965f0b439df9
    __enable_irq();

    printf("config BaseAddress is %x\r\n", Handle->Dev->HwConfig.BaseAddress);
    return ret;
}

/**
 * @brief  Write data to the specified pin.
 * @param  Handle is a pointer to the AL_GPIO instance.
 * @param  Pin is the pin number to which the Data is to be written.
 * @param  Data is the data to be written to the specified pin (0 or 1).
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_WritePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U32 Data)
{
    AL_U8 i;
    AL_S32 Ret = AL_OK;
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_Dev_WritePin(Handle->Dev, Pin, Data);

    return AL_OK;
}

/**
 * @brief  Read Data from the specified pin in DR register.
 * @param  Handle is a pointer to the AL_GPIO instance.
 * @param  Pin is the pin number to which the Data is to be written.
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_OutputReadPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_Dev_OutputReadPin(Handle->Dev, Pin);
}

/**
 * @brief  Read Data from the specified pin in EXT register.
 * @param  Handle is a pointer to the AL_GPIO instance.
 * @param  Pin is the pin number to which the Data is to be written.
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_InputReadPin(AL_GPIO_HalStruct *Handle, AL_U8 Pin)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_Dev_SetDirectionPin(Handle->Dev, Pin, GPIO_INPUT);
    AlGpio_Dev_InputReadPin(Handle->Dev, Pin);
}

#if 0
// ReadEXTRegisterOut
AL_S32 AlGpio_Hal_ReadEXTRegisterOut(AL_GPIO_HalStruct *Handle, AL_U8 Pin)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_Dev_SetDirectionPin(Handle->Dev, Pin, GPIO_OUTPUT);
    AlGpio_Dev_InputReadPin(Handle->Dev, Pin);
}
#endif

/**
 * @brief  This function configure the interrupt, including direction, type and enable register.
 * @param  Handle
 * @param  Pin
 * @param  IntrType
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_IntrCfg(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U8 IntrType)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_Dev_SetDirectionPin(Handle->Dev, Pin, GPIO_INPUT);
    AlGpio_Dev_IntrSetTypePin(Handle->Dev, Pin, IntrType);
    AlGpio_Dev_IntrEnablePin(Handle->Dev, Pin);

    //AlGpio_Dev_IntrGetStatus(Handle->Dev, 0);
    printf("IntrStatus %d is %x\r\n", 0, AlGpio_Dev_IntrGetStatus(Handle->Dev, 0));
    printf("IntrEnable %d is %x\r\n", 0, AlGpio_Dev_IntrGetEnable(Handle->Dev, 0));

    return AL_OK;
}
