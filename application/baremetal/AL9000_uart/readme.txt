This file will tell you some detials about uart files

Edition:		2021/9/24
			Only have an uartport : al9000_uart0



Function_file:		AL9000_UART.H(this file is function's headfile) 
			AL9000_UART.C(this file is driver's file)
			{
				Mode_type:autoflow.
			}
			uart_write() (this file is test file,include setting buad rate,stopbit,datebits.ect)


Modify:			demosoc.h         
			#define al9000_uart's registers
			#define base_address of al9000_uart
			#define al9000_uart's




Key modification:	#define AL9000_UART0_BASE	(0xF8400000)
			#define AL9000_UART_REG(offset) 



some question:		function redefine     eum_type_error not clear


Edition:		2021/9/26
			Only have an uartport : al9000_uart0



Function_file:		AL9000_UART.H(this file is function's headfile) 
			AL9000_UART.C(this file is driver's file)
			uart_write() (this file is test file,include setting buad rate,stopbit,datebits.ect)


Modify:			demosoc.h         
			#define al9000_uart's registers
			#define base_address of al9000_uart
			#define al9000_uart's
			
			Revising function of the AL9000_uart_init()	file:al9000_uart.c
			DLH: uart->DLH = ((uint16_t)SystemCoreClock / baudrate) >> 8 ; history edition(2021/9/24):DLH = ((uint16_t)SystemCoreClock / baudrate) >> 16 
			DLL: uart->DLL = ((uint8_t)SystemCoreClock / baudrate) ;       history edition(2021/9/24):DLL = ((uint16_t)SystemCoreClock / baudrate) >> 16
			
			Revising function of the AL9000_uart_init()	file:uart_write.c
 			Add two new functions compare history edition(2021/9/24)
			AL9000_uart_config_stopbit(AL9000_UART0,1);
			AL9000_uart_fifo_enable(AL9000_UART0);

			

Key modification:	#define AL9000_UART0_BASE	(0xF8400000)
			#define AL9000_UART_REG(offset) 



some question:		function redefine     eum_type_error not clear



Edition:		2021/9/27
			Only have an uartport : al9000_uart0



Function_file:		AL9000_UART.H(this file is function's headfile) 
			AL9000_UART.C(this file is driver's file)
			uart_write() (this file is test file,include setting buad rate,stopbit,datebits.ect)
						


Modify:			demosoc.h         
			#define al9000_uart's registers
			#define base_address of al9000_uart
			#define al9000_uart's
			
			Revising function of the AL9000_uart_init()	file:al9000_uart.c
			DLH: uart->DLH = ((uint16_t)SystemCoreClock / baudrate) >> 8 ; history edition(2021/9/24):DLH = ((uint16_t)SystemCoreClock / baudrate) >> 16 
			DLL: uart->DLL = ((uint8_t)SystemCoreClock / baudrate) ;       history edition(2021/9/24):DLL = ((uint16_t)SystemCoreClock / baudrate) >> 16
			
			Revising function of the AL9000_uart_init()	file:uart_write.c
			
			AL9000_uart_config_stopbit(AL9000_UART0,AL9000_UART_STOP_BIT_1); 
			AL9000_uart_fifo_enable(AL9000_UART0);
			
			

			

Key modification:	#define AL9000_UART0_BASE	(0xF8400000)
			#define AL9000_UART_REG(offset) 
			
			file "demosoc.h"
			address 
			#define ONCHIP_ILM_BASE         (0xc0000000UL)			history edition(2021/9/26)//(0x80000000UL)      /*!< (ILM       ) Base Address */
			#define ONCHIP_DLM_BASE         (0xc0040000UL)			history edition(2021/9/26)//(0x90000000UL)      /*!< (DLM       ) Base Address */
			#define DEMOSOC_PERIPH_BASE     (0x10000000UL)			history edition(2021/9/26)//(0x10000000UL)      /*!< (Peripheral) Base Address */


			#define __ECLIC_BASEADDR          0xD4000000UL			history edition(2021/9/26)//0x0C000000UL          /*!< Set to ECLIC baseaddr of your device 
			#define __ECLIC_INTNUM            159				history edition(2021/9/26)//51                    	/*!< Set to 1 - 1024, total interrupt number of ECLIC Unit */
			#define __SYSTIMER_BASEADDR       0xD4020000UL			history edition(2021/9/26)//0x02000000UL          /*!< Set to SysTimer baseaddr of your device */

			#define RTC_FREQ                  10000000			history edition(2021/9/26)//32768

some question:		function redefine     eum_type_error not clear

