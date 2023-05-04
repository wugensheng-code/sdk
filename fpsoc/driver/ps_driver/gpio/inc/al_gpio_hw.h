#ifndef AL_GPIO_HW_H
#define AL_GPIO_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"


#define AL_GPIO_NUM_INSTANCE	 (1)
#define GPIO_BASE_ADDR           0xF8411000UL
 




/** 
 *  Register offsets for the GPIO. 
 */
// Register: GPIO_SWPORTA_DR_BLK0 方向output时写入该寄存器
#define GPIO__SWPORTA__DR__BLK0__OFFSET 0x0

// Register: GPIO_SWPORTA_DDR_BLK0 控制数据位方向 0:input; 1:output
#define GPIO__SWPORTA__DDR__BLK0__OFFSET 0x4

// Register: GPIO_SWPORTA_DR_CLR_BLK0  1:clear data
#define GPIO__SWPORTA__DR__CLR__BLK0__OFFSET 0x8

// Register: GPIO_SWPORTA_DDR_CLR_BLK0  1:clear data direction
#define GPIO__SWPORTA__DDR__CLR__BLK0__OFFSET 0xc

// Register: GPIO_INTEN_CLR__BLK0   1:clear interrupt enable
#define GPIO__INTEN__CLR__BLK0__OFFSET 0x10

// Register: GPIO_INTMASK_CLR_BLK0   1: 屏蔽该信号的中断生成能力 0:中断允许通过
#define GPIO__INTMASK__CLR__BLK0__OFFSET 0x14

// Register: GPIO_INTTYPE_LEVEL_CLR_BLK0  
#define GPIO__INTTYPE__LEVEL__CLR__BLK0__OFFSET 0x18

// Register: GPIO_INT_POLARITY_CLR_BLK0
#define GPIO__INT__POLARITY__CLR__BLK0__OFFSET 0x1c

// Register: GPIO_DEBOUNCE_CLR_BLK0
#define GPIO__DEBOUNCE__CLR__BLK0__OFFSET 0x20

// Register: GPIO_INT_BOTHEDGE_CLR_BLK0
#define GPIO__INT__BOTHEDGE__CLR__BLK0__OFFSET 0x24

// Register: GPIO_INTEN_BLK0  中断使能
#define GPIO__INTEN__BLK0__OFFSET 0x30

// Register: GPIO_INTMASK_BLK0  1: 屏蔽该信号的中断生成能力 0:中断允许通过
#define GPIO__INTMASK__BLK0__OFFSET 0x34

// Register: GPIO_INTTYPE_LEVEL_BLK0  中断类型 0:level电平触发; 1:edge边沿触发
#define GPIO__INTTYPE__LEVEL__BLK0__OFFSET 0x38

// Register: GPIO_INT_POLARITY_BLK0  0：低电平或下降沿有效；1：高电平或上升沿有效
#define GPIO__INT__POLARITY__BLK0__OFFSET 0x3c

// Register: GPIO_INTSTATUS_BLK0   mask中断状态 0：禁止；1：允许
#define GPIO__INTSTATUS__BLK0__OFFSET 0x40

// Register: GPIO_RAW_INTSTATUS_BLK0   raw中断状态 0：禁止；1：允许
#define GPIO__RAW__INTSTATUS_BLK0__OFFSET 0x44

// Register: GPIO_DEBOUNCE_BLK0  1:启用去抖动 synopysys家的GPIO ip
#define GPIO__DEBOUNCE__BLK0__OFFSET 0x48

// Register: GPIO_PORTA_EOI_BLK0  1：中断被清除
#define GPIO__PORTA__EOI__BLK0__OFFSET 0x4c

// Register: GPIO_EXT_PORTA_BLK0   读到的数据
#define GPIO__EXT__PORTA__BLK0__OFFSET 0x50

// Register: GPIO_LS_SYNC_BLK0  1：所有电平敏感型中断同步到 pclk_intr
#define GPIO__LS__SYNC__BLK0__OFFSET 0x60

// Register: GPIO_ID_CODE_BLK0
#define GPIO__ID__CODE__BLK0__OFFSET 0x64

// Register: GPIO_INT_BOTHEDGE_BLK0  双边沿触发
#define GPIO__INT__BOTHEDGE__BLK0__OFFSET 0x68

// Register: GPIO_VER_ID_CODE_BLK0
#define GPIO__VER__ID__CODE__BLK0__OFFSET 0x6c

// Register: GPIO_CONFIG_REG2_BLK0
#define GPIO__CONFIG__REG2__BLK0__OFFSET 0x70

// Register: GPIO_CONFIG_REG1_BLK0
#define GPIO__CONFIG__REG1__BLK0__OFFSET 0x74




typedef struct {
	AL_U32                  DeviceId;
	AL_U32                  BaseAddress;
} AL_GPIO_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
