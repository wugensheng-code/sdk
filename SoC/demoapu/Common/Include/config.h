#ifndef __CONFIG_H
#define __CONFIG_H

//#include "autoconf.h"

/* print debug infomation */
#define DEBUG
/* switch el1 from el3 */
#define SWITCH_TO_EL1_EL0_FROM_EL3

/* apu system frequency */
#define COUNTER_FREQUENCY 50000000
/* test or not */
// #define TEST

/* */
#define ENABLE_MMU
#define ENABLE_IRQ

/* gic */
//#define GIC_BASE    0xdd000000
//#define GIC_SIZE    0x400000

// #define CONFIG_GICV2
//#define GICD_BASE   GIC_BASE
//#define GICC_BASE   0xdd010000

/*|---------- SP BUS 0 ----------------|
 * -------------------------------------
 * name     start       end         size
 * -----    --------    --------    ----
 * uart0	F8400000	F8400FFF	4KB
 * uart1	F8401000	F8401FFF	4KB
 * spi1	    F8405000	F8405FFF	4KB
 * can1	    F8409000	F8409FFF	4KB
 * ttc0	    F840C000	F840CFFF	4KB
 * ttc1	    F840D000	F840DFFF	4KB
 * gpio	    F8411000	F8411FFF	4KB
*/
#define SPBUS0_BASE 0xf8400000
#define SPBUS0_SIZE (0x1000 * 7)

/*|---------- AHB BUS -----------------|  
 *|------------------------------------|
 * name     start       end         size
 * -----    --------    --------    ----
 * sdio0	F8049000	F8049FFF	4KB
 * sdio1	F804A000	F804AFFF	4KB
 * hp0	    F804B000	F804BFFF	4KB
 * hp1	    F804C000	F804CFFF	4KB
 * dmac_ahb	F804D000	F804DFFF	4KB
 * qspi	    F804E000	F804EFFF	4KB
 * gbe0	    F8100000	F810FFFF	64KB
 * gbe1	    F8110000	F811FFFF	64KB
 * usb0	    F8180000	F81BFFFF	256KB
 * usb1	    F81C0000	F81FFFFF	256KB
 * csu_sysctrl	F8080000	F80BFFFF	256KB
 */
#define AHBBUS_BASE 0xf8049000
#define AHBBUS_SIZE (0x1000 * 230)

#endif
