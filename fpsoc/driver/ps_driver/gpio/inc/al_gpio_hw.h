#ifndef AL_GPIO_HW_H
#define AL_GPIO_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"


#define AL_GPIO_NUM_INSTANCE	 (1)
#define GPIO_BASE_ADDR           0xF8411000UL
#define GPIO_MAX_BANKS		     (AL_U8)4
#define GPIO_BANK_MAX_PINS		 (AL_U32)32
#define GPIO_MAX_PIN_NUM	     (AL_U32)118

/** 
 *  Register offsets for the GPIO. 
 */
#define GPIO__SWPORTA__DR__BLK0__OFFSET 0x0
#define GPIO__SWPORTA__DDR__BLK0__OFFSET 0x4
#define GPIO__SWPORTA__DR__CLR__BLK0__OFFSET 0x8
#define GPIO__SWPORTA__DDR__CLR__BLK0__OFFSET 0xc
#define GPIO__EXT__PORTA__BLK0__OFFSET 0x50
#define GPIO__LS__SYNC__BLK0__OFFSET 0x60

/** 
 *  Register for interrupt. 
 */
#define GPIO__INTEN__CLR__BLK0__OFFSET 0x10
#define GPIO__INTMASK__CLR__BLK0__OFFSET 0x14
#define GPIO__INTTYPE__LEVEL__CLR__BLK0__OFFSET 0x18
#define GPIO__INT__POLARITY__CLR__BLK0__OFFSET 0x1c
#define GPIO__DEBOUNCE__CLR__BLK0__OFFSET 0x20
#define GPIO__INT__BOTHEDGE__CLR__BLK0__OFFSET 0x24
#define GPIO__INTEN__BLK0__OFFSET 0x30
#define GPIO__INTMASK__BLK0__OFFSET 0x34
#define GPIO__INTTYPE__LEVEL__BLK0__OFFSET 0x38
#define GPIO__INT__POLARITY__BLK0__OFFSET 0x3c
#define GPIO__INTSTATUS__BLK0__OFFSET 0x40
#define GPIO__RAW__INTSTATUS_BLK0__OFFSET 0x44
#define GPIO__DEBOUNCE__BLK0__OFFSET 0x48
#define GPIO__PORTA__EOI__BLK0__OFFSET 0x4c
#define GPIO__INT__BOTHEDGE__BLK0__OFFSET 0x68

#define GPIO__ID__CODE__BLK0__OFFSET 0x64
#define GPIO__VER__ID__CODE__BLK0__OFFSET 0x6c
#define GPIO__CONFIG__REG2__BLK0__OFFSET 0x70
#define GPIO__CONFIG__REG1__BLK0__OFFSET 0x74


typedef struct {
	AL_U32                  DeviceId;
	AL_U32                  BaseAddress;
	AL_U32                  BankMaxPins;
	AL_U32                  MaxBanks;
} AL_GPIO_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
