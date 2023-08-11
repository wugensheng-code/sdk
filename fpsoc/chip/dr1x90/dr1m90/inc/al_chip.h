#ifndef __AL_CHIP_H__
#define __AL_CHIP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"
#include "al_hwcfg.h"
#include "platform_init.h"

AL_VOID AlChip_Dr1m90Init(AL_VOID);

#define AlChip_Init AlChip_Dr1m90Init

/*----------------------------------------------------------------------------
* Define Interrupt Number
*----------------------------------------------------------------------------*/
typedef enum {
    SOC_REQ0_IRQn        =  43,
    SOC_ACK0_IRQn        =  44,
    SOC_REQ1_IRQn        =  45,
    SOC_ACK1_IRQn        =  46,
    SOC_REQ2_IRQn        =  47,
    SOC_ACK2_IRQn        =  48,
    SOC_REQ3_IRQn        =  49,
    SOC_ACK3_IRQn        =  50,
    SOC_DMACAHB_IRQn     =  78,
    SOC_SMC_IRQn         =  79,
    SOC_QSPI_IRQn        =  80,
    GPIO0_IRQn           =  84,
    SOC_USB0_IRQn        =  88,
    SOC_USB1_IRQn        =  89,
    SOC_GBE0_IRQn        =  90,
    SOC_GBE1_IRQn        =  91,
    SOC_MMC0_IRQn        =  94,
    SOC_MMC1_IRQn        =  95,
    SOC_IIC0_IRQn        =  96,
    SOC_IIC1_IRQn        =  97,
    SOC_SPI0_MST_IRQn    =  98,
    SOC_SPI0_SLV_IRQn    =  99,
    SOC_SPI1_MST_IRQn    =  100,
    SOC_SPI1_SLV_IRQn    =  101,
    SOC_INT130_IRQn      =  143,
    SOC_UART0_IRQn       =  102,
    SOC_UART1_IRQn       =  103,
    SOC_CAN0_IRQn        =  104,
    SOC_CAN1_IRQn        =  105,
    SOC_INT_MAX          =  256,
    NPU_HARD_IRQn        =  144,
    NPU_SOFT_IRQn        =  129,
} AL_IrqNumEnum;

#ifdef __cplusplus
}
#endif
#endif /* __AL_CHIP_H__ */
