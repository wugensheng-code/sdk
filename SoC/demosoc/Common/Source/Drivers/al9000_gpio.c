#include "demosoc.h"
#include "al9000_gpio.h"

//int32_t gpio_iof_config(GPIO_TypeDef* gpio, uint32_t mask, IOF_FUNC func)
//{
//    if (__RARELY(gpio == NULL)) {
//        return -1;
//    }
//    switch (func) {
//        case IOF_SEL_GPIO:
//            gpio->IOF_EN &= ~mask;
//            break;
//        case IOF_SEL_0:
//            gpio->IOF_SEL &= ~mask;
//            gpio->IOF_EN |= mask;
//            break;
//        case IOF_SEL_1:
//            gpio->IOF_SEL |= mask;
//            gpio->IOF_EN |= mask;
//            break;
//        default:
//            break;
//    }
//    return 0;
//}

//int32_t gpio_enable_output(GPIO_TypeDef* gpio, uint32_t mask)
//{
//    if (__RARELY(gpio == NULL)) {
//        return -1;
//    }
//    gpio->OUTPUT_EN |= mask;
//    gpio->INPUT_EN &= ~mask;
//    return 0;
//}

//int32_t gpio_enable_input(GPIO_TypeDef* gpio, uint32_t mask)
//{
//    if (__RARELY(gpio == NULL)) {
//        return -1;
//    }
//    gpio->INPUT_EN |= mask;
//    gpio->OUTPUT_EN &= ~mask;
//    return 0;
//}

int32_t gpio_enable_output(GPIO_TypeDef* gpio, uint32_t mask)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
    gpio->SWPORTA_DDR |= mask;
    return 0;
}

int32_t gpio_enable_input(GPIO_TypeDef* gpio, uint32_t mask)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
    gpio->SWPORTA_DDR_CLR |= mask;
    return 0;
}

//int32_t gpio_write(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value)
//{
//    if (__RARELY(gpio == NULL)) {
//        return -1;
//    }
//    // If value != 0, mean set gpio pin high, otherwise set pin low
//    if (value) {
//        gpio->OUTPUT_VAL |= (mask);
//    } else {
//        gpio->OUTPUT_VAL &= ~(mask);
//    }
//    return 0;
//}

int32_t gpio_write(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
    // If value != 0, mean set gpio pin high, otherwise set pin low
    if (value) {
        gpio->SWPORTA_DR |= (mask);
    } else {
        gpio->SWPORTA_DR_CLR |= (mask);
    }
    return 0;
}

//int32_t gpio_toggle(GPIO_TypeDef* gpio, uint32_t mask)
//{
//    if (__RARELY(gpio == NULL)) {
//        return -1;
//    }
//    gpio->OUTPUT_VAL = (mask ^ gpio->OUTPUT_VAL);
//    return 0;
//}

int32_t gpio_toggle(GPIO_TypeDef* gpio, uint32_t mask)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
    uint32_t value;
    value = (mask & gpio->SWPORTA_DR);
    // If value != 0, mean set gpio pin high, otherwise set pin low
	if (value) {
		gpio->SWPORTA_DR_CLR |= (mask);
	} else {
		gpio->SWPORTA_DR |= (mask);
	}
    return 0;
}

int32_t gpio_output_read(GPIO_TypeDef* gpio, uint32_t mask)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
    return gpio->SWPORTA_DR & mask;
}

int32_t gpio_input_read(GPIO_TypeDef* gpio, uint32_t mask)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
    return gpio->EXT_PORTA & mask;
}

//int32_t gpio_set_pue(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value)
//{
//    if (__RARELY(gpio == NULL)) {
//        return -1;
//    }
//    mask = gpio->PULLUP_EN & (~mask);
//    gpio->PULLUP_EN = (mask | value);
//    return 0;
//}

//int32_t gpio_set_ds(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value)
//{
//    if (__RARELY(gpio == NULL)) {
//        return -1;
//    }
//    mask = gpio->DRIVE & (~mask);
//    gpio->DRIVE = (mask | value);
//    return 0;
//}

//int32_t gpio_set_outxor(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value)
//{
//    if (__RARELY(gpio == NULL)) {
//        return -1;
//    }
//    mask = gpio->OUTPUT_XOR & (~mask);
//    gpio->OUTPUT_XOR = (mask | value);
//    return 0;
//}

int32_t gpio_int_enable(GPIO_TypeDef* gpio, uint32_t mask)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
    gpio->INTEN |= mask;
    return 0;
}

int32_t gpio_int_disable(GPIO_TypeDef* gpio, uint32_t mask)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
	gpio->INTEN_CLR |= mask;
    return 0;
}

int32_t gpio_intmask_cfg(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
    if (value) {
    	gpio->INTMASK |= mask;
	} else {
		gpio->INTMASK_CLR |= mask;
	}
    return 0;
}

//int32_t gpio_enable_interrupt(GPIO_TypeDef* gpio, uint32_t mask, GPIO_INT_TYPE type)
//{
//    if (__RARELY(gpio == NULL)) {
//        return -1;
//    }
//    switch (type) {
//        case GPIO_INT_RISE:
//            gpio->RISE_IE |= mask;
//            break;
//        case GPIO_INT_FALL:
//            gpio->FALL_IE |= mask;
//            break;
//        case GPIO_INT_HIGH:
//            gpio->HIGH_IE |= mask;
//            break;
//        case GPIO_INT_LOW:
//            gpio->LOW_IE |= mask;
//            break;
//        default:
//            break;
//    }
//    return 0;
//}

int32_t gpio_inttype_cfg(GPIO_TypeDef* gpio, uint32_t mask, GPIO_INT_TYPE type)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
    switch (type) {
        case GPIO_INT_RISE:
            gpio->INTTYPE_LEVEL |= mask;
            gpio->INT_POLARITY |= mask;
            break;
        case GPIO_INT_FALL:
        	gpio->INTTYPE_LEVEL |= mask;
        	gpio->INT_POLARITY_CLR |= mask;
            break;
        case GPIO_INT_HIGH:
        	gpio->INTTYPE_LEVEL_CLR |= mask;
        	gpio->INT_POLARITY |= mask;
            break;
        case GPIO_INT_LOW:
        	gpio->INTTYPE_LEVEL_CLR |= mask;
        	gpio->INT_POLARITY_CLR |= mask;
            break;
        default:
            break;
    }
    return 0;
}

//int32_t gpio_disable_interrupt(GPIO_TypeDef* gpio, uint32_t mask, GPIO_INT_TYPE type)
//{
//    if (__RARELY(gpio == NULL)) {
//        return -1;
//    }
//    switch (type) {
//        case GPIO_INT_RISE:
//            gpio->RISE_IE &= ~mask;
//            break;
//        case GPIO_INT_FALL:
//            gpio->FALL_IE &= ~mask;
//            break;
//        case GPIO_INT_HIGH:
//            gpio->HIGH_IE &= ~mask;
//            break;
//        case GPIO_INT_LOW:
//            gpio->LOW_IE &= ~mask;
//            break;
//        default:
//            break;
//    }
//    return 0;
//}

//int32_t gpio_clear_interrupt(GPIO_TypeDef* gpio, uint32_t mask, GPIO_INT_TYPE type)
//{
//    if (__RARELY(gpio == NULL)) {
//        return -1;
//    }
//    switch (type) {
//        case GPIO_INT_RISE:
//            gpio->RISE_IP |= mask;
//            break;
//        case GPIO_INT_FALL:
//            gpio->FALL_IP |= mask;
//            break;
//        case GPIO_INT_HIGH:
//            gpio->HIGH_IP |= mask;
//            break;
//        case GPIO_INT_LOW:
//            gpio->LOW_IP |= mask;
//            break;
//        default:
//            break;
//    }
//    return 0;
//}

int32_t gpio_int_status(GPIO_TypeDef* gpio, uint32_t mask)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
    return gpio->INTSTATUS & mask;
}

int32_t gpio_debounce_cfg(GPIO_TypeDef* gpio, uint32_t mask, uint32_t value)
{
    if (__RARELY(gpio == NULL)) {
        return -1;
    }
    if (value) {
		gpio->DEBOUNCE |= mask;
	} else {
		gpio->DEBOUNCE_CLR |= mask;
	}
    return 0;
}

