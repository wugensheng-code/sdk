#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

/* RT-Thread Kernel */

#define RT_NAME_MAX                 #{{RT_NAME_MAX}}
#define RT_ALIGN_SIZE               #{{RT_ALIGN_SIZE}}
#define RT_THREAD_PRIORITY_32           
#define RT_THREAD_PRIORITY_MAX      #{{RT_THREAD_PRIORITY_MAX}}
#define RT_TICK_PER_SECOND          #{{RT_TICK_PER_SECOND}}
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_HOOK_USING_FUNC_PTR
#define RT_USING_IDLE_HOOK
#define RT_IDLE_HOOK_LIST_SIZE      #{{RT_IDLE_HOOK_LIST_SIZE}}
#define IDLE_THREAD_STACK_SIZE      #{{IDLE_THREAD_STACK_SIZE}}
#define RT_USING_TIMER_SOFT
#define RT_TIMER_THREAD_PRIO        #{{RT_TIMER_THREAD_PRIO}}
#define RT_TIMER_THREAD_STACK_SIZE  #{{RT_TIMER_THREAD_STACK_SIZE}}

/* kservice optimization */

#define RT_DEBUG

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_SMALL_MEM
#define RT_USING_SMALL_MEM_AS_HEAP
#define RT_USING_HEAP

/* Kernel Device Object */
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 256
#define RT_CONSOLE_DEVICE_NAME "uart0"  
#define RT_VER_NUM                      0x40101
#define ARCH_CPU_64BIT
#define ARCH_ARMV8

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE       #{{RT_MAIN_THREAD_STACK_SIZE}}
#define RT_MAIN_THREAD_PRIORITY         #{{RT_MAIN_THREAD_PRIORITY}}     
#define RT_USING_MSH
#define RT_USING_FINSH
#define FINSH_USING_MSH
#define FINSH_THREAD_NAME               #{{FINSH_THREAD_NAME}}
#define FINSH_THREAD_PRIORITY           #{{FINSH_THREAD_PRIORITY}}
#define FINSH_THREAD_STACK_SIZE         #{{FINSH_THREAD_STACK_SIZE}}
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES             #{{FINSH_HISTORY_LINES}}
#define FINSH_USING_SYMTAB
#define FINSH_CMD_SIZE                  #{{FINSH_CMD_SIZE}}
#define MSH_USING_BUILT_IN_COMMANDS
#define FINSH_USING_DESCRIPTION
#define FINSH_ARG_MAX                   #{{FINSH_ARG_MAX}}

/* Device Drivers */


/* Using USB */


/* C/C++ and POSIX layer */

#define RT_LIBC_DEFAULT_TIMEZONE        #{{RT_LIBC_DEFAULT_TIMEZONE}}

/* POSIX (Portable Operating System Interface) layer */


/* Interprocess Communication (IPC) */


/* Socket is in the 'Network' category */


/* Network */


/* Utilities */

#define RT_USING_UTEST
#define UTEST_THR_STACK_SIZE            #{{UTEST_THR_STACK_SIZE}}
#define UTEST_THR_PRIORITY              #{{UTEST_THR_PRIORITY}}

/* RT-Thread online packages */

/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* IoT Cloud */


/* security packages */


/* language packages */

/* JSON: JavaScript Object Notation, a lightweight data-interchange format */


/* XML: Extensible Markup Language */


/* multimedia packages */

/* LVGL: powerful and easy-to-use embedded GUI library */


/* u8g2: a monochrome graphic library */


/* tools packages */


/* system packages */

/* enhanced kernel services */


/* acceleration: Assembly language or algorithmic acceleration packages */


/* CMSIS: ARM Cortex-M Microcontroller Software Interface Standard */


/* Micrium: Micrium software products porting for RT-Thread */


/* peripheral libraries and drivers */

/* sensors drivers */


/* touch drivers */


/* Kendryte SDK */


/* AI packages */


/* Signal Processing and Control Algorithm Packages */


/* miscellaneous packages */

/* project laboratory */

/* samples: kernel and components samples */


/* entertainment: terminal games and other interesting software packages */


/* Arduino libraries */


/* Projects and Demos */


/* Sensors */


/* Display */


/* Timing */


/* Data Processing */


/* Data Storage */

/* Communication */


/* Device Control */


/* Other */


/* Signal IO */


/* Uncategorized */

#define AARCH64_CORE

/* Hardware Drivers Config */

#define SOC_HUMMINGBIRD

/* Onboard Peripheral Drivers */


/* On-chip Peripheral Drivers */


/* Board extended module Drivers */

/* Soc Config */

#define USE_ARM_CORE
#define BSP_USING_GIC
#define BSP_USING_GICV3

/* RT-Thread Utestcases */

#define RT_USING_UTESTCASES

/* Utest Self Testcase */


/* Kernel Testcase */

#define UTEST_THREAD_TC

/* CPP11 Testcase */


/* Utest Serial Testcase */


/* RTT Posix Testcase */


#endif
