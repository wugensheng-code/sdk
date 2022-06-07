/*
 * test.h
 *
 *  Created on: 2022年5月5日
 *      Author: Administrator
 */

#ifndef AL9000_TEST_H
#define AL9000_TEST_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "demosoc.h"
#include "string.h"
#include "sys.h"
#include "delay.h"
#include "usbh_usr.h"

int test_i2c_e2prom(I2C_TypeDef *i2c);
void iic_test();
void can_test();
uint32_t test_SReadID(void);
void test_flash_write_enable();
uint32_t test_flash_wait_wip(void);
uint32_t qspi_test();
void dma_test();
void DMA_SOC_DMA_AHB_HANDLER(void);
void test_XEC1_IRQHandler(void);
void test_XEC0_IRQHandler(void);
void gbe_test(void);
uint32_t plps_test();
int gpio_test(void);
void gpio_config(void);
int timer_test(void);
void setup_timer();
void mtimer_sw_irq_handler(void);
void mtimer_irq_handler(void);
void wait_seconds(size_t n);
uint32_t benchmark_test();
int usb_test(void);
int spi_test();
#ifdef __cplusplus
}
#endif



#endif /* SOC_DEMOSOC_COMMON_INCLUDE_AL9000_CAN_H_ */
