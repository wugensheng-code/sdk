// See LICENSE for license details.
#ifndef _AL_GPIO_H
#define _AL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

//#define GPIO_INPUT_VAL      (0x00)
//#define GPIO_INPUT_EN       (0x04)
//#define GPIO_OUTPUT_EN      (0x08)
//#define GPIO_OUTPUT_VAL     (0x0C)
//#define GPIO_PULLUP_EN      (0x10)
//#define GPIO_DRIVE          (0x14)
//#define GPIO_RISE_IE        (0x18)
//#define GPIO_RISE_IP        (0x1C)
//#define GPIO_FALL_IE        (0x20)
//#define GPIO_FALL_IP        (0x24)
//#define GPIO_HIGH_IE        (0x28)
//#define GPIO_HIGH_IP        (0x2C)
//#define GPIO_LOW_IE         (0x30)
//#define GPIO_LOW_IP         (0x34)
//#define GPIO_IOF_EN         (0x38)
//#define GPIO_IOF_SEL        (0x3C)
//#define GPIO_OUTPUT_XOR     (0x40)


//typedef enum iof_func {
//    IOF_SEL_GPIO = 0,
//    IOF_SEL_0 = 1,
//    IOF_SEL_1 = 2
//} IOF_FUNC;

typedef enum gpio_int_type {
    GPIO_INT_RISE = 0,
    GPIO_INT_FALL = 1,
    GPIO_INT_HIGH = 2,
    GPIO_INT_LOW = 3
} GPIO_INT_TYPE;

//int32_t gpio_iof_config(GPIO_TypeDef* gpio, uint32_t mask, IOF_FUNC func);
int32_t AlGpio_EnableOutput(GPIO_TypeDef* gpio, uint32_t mask);
int32_t AlGpio_EnableInput(GPIO_TypeDef* gpio, uint32_t mask);
int32_t AlGpio_Write(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value);
int32_t AlGpio_Toggle(GPIO_TypeDef* gpio, uint32_t mask);
int32_t AlGpio_OutputRead(GPIO_TypeDef* gpio, uint32_t mask);
int32_t AlGpio_InputRead(GPIO_TypeDef* gpio, uint32_t mask);
//int32_t gpio_set_pue(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value);
//int32_t gpio_set_ds(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value);
//int32_t gpio_set_outxor(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value);
int32_t AlGpio_IntEnable(GPIO_TypeDef* gpio, uint32_t mask);
int32_t AlGpio_IntDisable(GPIO_TypeDef* gpio, uint32_t mask);
int32_t AlGpio_IntMaskCfg(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value);
int32_t AlGpio_IntTypeCfg(GPIO_TypeDef* gpio, uint32_t mask, GPIO_INT_TYPE type);
//int32_t gpio_disable_interrupt(GPIO_TypeDef* gpio, uint32_t mask, GPIO_INT_TYPE type);
//int32_t gpio_clear_interrupt(GPIO_TypeDef* gpio, uint32_t mask, GPIO_INT_TYPE type);
int32_t AlGpio_IntStatus(GPIO_TypeDef* gpio, uint32_t mask);
int32_t AlGpio_DebounceCfg(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value);

#ifdef __cplusplus
}
#endif
#endif /* _DEMOSOC_GPIO_H */
