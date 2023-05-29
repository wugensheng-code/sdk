#ifndef AL_UART_HW_H
#define AL_UART_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/


#define AL_UART_NUM_INSTANCE    (2)

#define UART0__BASE_ADDR 0xF8400000UL
#define UART1__BASE_ADDR 0xF8401000UL

#define CRP__BASE_ADDR 0xF8801000UL

#define UART_CLOCK  40000000

///////////////////////////////////////////////////////
// Register: SRST_CTRL3
///////////////////////////////////////////////////////
#define CRP__SRST_CTRL3__OFFSET                        0x07c
#define CRP__SRST_CTRL3__UART0__SRST__N__SHIFT         12
#define CRP__SRST_CTRL3__UART1__SRST__N__SHIFT         13

///////////////////////////////////////////////////////
// Register: RBR__THR__DLL
// A combined register:
// (RBR): Receive Buffer Register.
//
// (THR): Transmit Holding Register.
//
// (DLL): Divisor Latch (Low).
// If UART_16550_COMPATIBLE = No, then this register can be accessed only when the DLAB bit (LCR[7]) is set and the UART is not busy - that is, USR[0] is 0; otherwise this register can be accessed only when the DLAB bit (LCR[7]) is set.
///////////////////////////////////////////////////////
#define UART__RBR__THR__DLL__OFFSET                        0x0
#define UART__RBR__THR__DLL__NUM                           0x1
#define UART__RBR__THR__DLL__WIDTH                         8

#define UART__RBR__THR__DLL__RBR__THR__DLL__SHIFT          0
#define UART__RBR__THR__DLL__RSVD_RBR__THR__DLL__SHIFT     8

#define UART__RBR__THR__DLL__RBR__THR__DLL__MASK           0x000000ff
#define UART__RBR__THR__DLL__RSVD_RBR__THR__DLL__MASK      0xffffff00

#define UART__RBR__THR__DLL__RBR__THR__DLL__POR_VALUE      0x0
#define UART__RBR__THR__DLL__RSVD_RBR__THR__DLL__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: IER_DLH
// A combined register:
// (IER): Interrupt Enable Register.
//
// (DLH): Divisor Latch High (DLH) Register.If UART_16550_COMPATIBLE = No, then this register can be accessed only when the DLAB bit (LCR[7]) is set and the UART is not busy, that is, USR[0] is 0; otherwise this register can be accessed only when the DLAB bit (LCR[7]) is set.
///////////////////////////////////////////////////////
#define UART__IER_DLH__OFFSET       0x4
#define UART__IER_DLH__NUM          0x1
#define UART__IER_DLH__dlh__SHIFT   0
#define UART__IER_DLH__dlh__SIZE    8

#define UART__IER_DLH__ERBFI__DLH__SHIFT           0
#define UART__IER_DLH__ETBEI__DLH__SHIFT           1
#define UART__IER_DLH__ELSI__DLH__SHIFT            2
#define UART__IER_DLH__EDSSI__DLH__SHIFT           3
#define UART__IER_DLH__ELCOLR__DLH__SHIFT          4
#define UART__IER_DLH__RSVD_IER__DLH_6TO5__SHIFT   5
#define UART__IER_DLH__PTIME__DLH__SHIFT           7
#define UART__IER_DLH__RSVD_IER__DLH__SHIFT        8

#define UART__IER_DLH__ERBFI__DLH__MASK             0x00000001
#define UART__IER_DLH__ETBEI__DLH__MASK             0x00000002
#define UART__IER_DLH__ELSI__DLH__MASK              0x00000004
#define UART__IER_DLH__EDSSI__DLH__MASK             0x00000008
#define UART__IER_DLH__ELCOLR__DLH__MASK            0x00000010
#define UART__IER_DLH__RSVD_IER__DLH_6TO5__MASK     0x00000060
#define UART__IER_DLH__PTIME__DLH__MASK             0x00000080
#define UART__IER_DLH__RSVD_IER__DLH__MASK          0xffffff00

#define UART__IER_DLH__ERBFI__DLH__POR_VALUE            0x0
#define UART__IER_DLH__ETBEI__DLH__POR_VALUE            0x0
#define UART__IER_DLH__ELSI__DLH__POR_VALUE             0x0
#define UART__IER_DLH__EDSSI__DLH__POR_VALUE            0x0
#define UART__IER_DLH__ELCOLR__DLH__POR_VALUE           0x0
#define UART__IER_DLH__RSVD_IER__DLH_6TO5__POR_VALUE    0x0
#define UART__IER_DLH__PTIME__DLH__POR_VALUE            0x0
#define UART__IER_DLH__RSVD_IER__DLH__POR_VALUE         0x0


///////////////////////////////////////////////////////
// Register: FCR__IIR
// A combined register:
// (FCR): This register is only valid when the DW_apb_uart is configured to have FIFO's implemented (FIFO_MODE != NONE). If FIFO's are not implemented, this register does not exist and writing to this register address will have no effect.
//
// (IIR): Interrupt Identification Register
///////////////////////////////////////////////////////
#define UART__FCR__IIR__OFFSET  0x8
#define UART__FCR__IIR__NUM  0x1

#define UART__FCR__IIR__FIFOE__IID__SHIFT       0
#define UART__FCR__IIR__FIFOE__IID__SIZE        4
#define UART__FCR__IIR__RFIFOR__IID__SHIFT      1
#define UART__FCR__IIR__XFIFOR__IID__SHIFT      2
#define UART__FCR__IIR__DMAM__IID__SHIFT        3
#define UART__FCR__IIR__TET__RSVD_IIR__SHIFT    4
#define UART__FCR__IIR__TET__RSVD_IIR__SIZE     2
#define UART__FCR__IIR__RT__FIFOSE__SHIFT       6
#define UART__FCR__IIR__RT__FIFOSE__SSIZE       2
#define UART__FCR__IIR__RSVD_FCR__IIR__SHIFT    8

#define UART__FCR__IIR__FIFOE__IID__MASK        0x00000001
#define UART__FCR__IIR__RFIFOR__IID__MASK       0x00000002
#define UART__FCR__IIR__XFIFOR__IID__MASK       0x00000004
#define UART__FCR__IIR__DMAM__IID__MASK         0x00000008
#define UART__FCR__IIR__TET__RSVD_IIR__MASK     0x00000030
#define UART__FCR__IIR__RT__FIFOSE__MASK        0x000000c0
#define UART__FCR__IIR__RSVD_FCR__IIR__MASK     0xffffff00

///////////////////////////////////////////////////////
// Register: LCR
// Line Control Register
///////////////////////////////////////////////////////
#define UART__LCR__OFFSET 0xC
#define UART__LCR__DLS__SHIFT     (0)
#define UART__LCR__DLS__SIZE      (2)
#define UART__LCR__STOP__SHIFT    (2)
#define UART__LCR__STOP__SIZE     (1)
#define UART__LCR__PARITY__SHIFT  (3)
#define UART__LCR__PARITY__SIZE   (3)
#define UART__LCR__DLAB__SHIFT    (7)
#define UART__LCR__DLAB__SIZE     (1)


///////////////////////////////////////////////////////
// Register: MCR
// Modem Control Register
///////////////////////////////////////////////////////
#define UART__MCR__ADDR_OFFSET  0x10
#define UART__MCR__NUM  0x1

#define UART__MCR__DTR__SHIFT    0
#define UART__MCR__RTS__SHIFT    1
#define UART__MCR__OUT1__SHIFT    2
#define UART__MCR__OUT2__SHIFT    3
#define UART__MCR__LOOPBACK__SHIFT    4
#define UART__MCR__AFCE__SHIFT    5
#define UART__MCR__SIRE__SHIFT    6
#define UART__MCR__RSVD_MCR_31TO7__SHIFT    7

#define UART__MCR__DTR__MASK    0x00000001
#define UART__MCR__RTS__MASK    0x00000002
#define UART__MCR__OUT1__MASK    0x00000004
#define UART__MCR__OUT2__MASK    0x00000008
#define UART__MCR__LOOPBACK__MASK    0x00000010
#define UART__MCR__AFCE__MASK    0x00000020
#define UART__MCR__SIRE__MASK    0x00000040
#define UART__MCR__RSVD_MCR_31TO7__MASK    0xffffff80

#define UART__MCR__DTR__POR_VALUE    0x0
#define UART__MCR__RTS__POR_VALUE    0x0
#define UART__MCR__OUT1__POR_VALUE    0x0
#define UART__MCR__OUT2__POR_VALUE    0x0
#define UART__MCR__LOOPBACK__POR_VALUE    0x0
#define UART__MCR__AFCE__POR_VALUE    0x0
#define UART__MCR__SIRE__POR_VALUE    0x0
#define UART__MCR__RSVD_MCR_31TO7__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: LSR
// Line Status Register
///////////////////////////////////////////////////////
#define UART__LSR__OFFSET  0x14
#define UART__LSR__NUM  0x1

#define UART__LSR__DR__SHIFT    0
#define UART__LSR__OE__SHIFT    1
#define UART__LSR__PE__SHIFT    2
#define UART__LSR__FE__SHIFT    3
#define UART__LSR__BI__SHIFT    4
#define UART__LSR__THRE__SHIFT    5
#define UART__LSR__TEMT__SHIFT    6
#define UART__LSR__RFE__SHIFT    7
#define UART__LSR__RSVD_ADDR_RCVD__SHIFT    8
#define UART__LSR__RSVD_LSR_31TO9__SHIFT    9

#define UART__LSR__DR__MASK    0x00000001
#define UART__LSR__OE__MASK    0x00000002
#define UART__LSR__PE__MASK    0x00000004
#define UART__LSR__FE__MASK    0x00000008
#define UART__LSR__BI__MASK    0x00000010
#define UART__LSR__THRE__MASK    0x00000020
#define UART__LSR__TEMT__MASK    0x00000040
#define UART__LSR__RFE__MASK    0x00000080
#define UART__LSR__RSVD_ADDR_RCVD__MASK    0x00000100
#define UART__LSR__RSVD_LSR_31TO9__MASK    0xfffffe00

#define UART__LSR__DR__POR_VALUE    0x0
#define UART__LSR__OE__POR_VALUE    0x0
#define UART__LSR__PE__POR_VALUE    0x0
#define UART__LSR__FE__POR_VALUE    0x0
#define UART__LSR__BI__POR_VALUE    0x0
#define UART__LSR__THRE__POR_VALUE    0x1
#define UART__LSR__TEMT__POR_VALUE    0x1
#define UART__LSR__RFE__POR_VALUE    0x0
#define UART__LSR__RSVD_ADDR_RCVD__POR_VALUE    0x0
#define UART__LSR__RSVD_LSR_31TO9__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MSR
// Whenever bits 0, 1, 2 or 3 is set to logic one, to indicate a change on the modem control inputs, a modem status interrupt will be generated if enabled via the IER regardless of when the change occurred. The bits (bits 0, 1, 3) can be set after a reset-even tthough their respective modem signals are inactive-because the synchronized version of the modem signals have a reset value of 0 and change to value 1 after reset. To prevent unwanted interrupts due to this change, a read of the MSR register can be performed after reset.
///////////////////////////////////////////////////////
#define UART__MSR__NUM  0x1

#define UART__MSR__DCTS__SHIFT    0
#define UART__MSR__DDSR__SHIFT    1
#define UART__MSR__TERI__SHIFT    2
#define UART__MSR__DDCD__SHIFT    3
#define UART__MSR__CTS__SHIFT    4
#define UART__MSR__DSR__SHIFT    5
#define UART__MSR__RI__SHIFT    6
#define UART__MSR__DCD__SHIFT    7
#define UART__MSR__RSVD_MSR_31TO8__SHIFT    8

#define UART__MSR__DCTS__MASK    0x00000001
#define UART__MSR__DDSR__MASK    0x00000002
#define UART__MSR__TERI__MASK    0x00000004
#define UART__MSR__DDCD__MASK    0x00000008
#define UART__MSR__CTS__MASK    0x00000010
#define UART__MSR__DSR__MASK    0x00000020
#define UART__MSR__RI__MASK    0x00000040
#define UART__MSR__DCD__MASK    0x00000080
#define UART__MSR__RSVD_MSR_31TO8__MASK    0xffffff00

#define UART__MSR__DCTS__POR_VALUE    0x0
#define UART__MSR__DDSR__POR_VALUE    0x0
#define UART__MSR__TERI__POR_VALUE    0x0
#define UART__MSR__DDCD__POR_VALUE    0x0
#define UART__MSR__CTS__POR_VALUE    0x0
#define UART__MSR__DSR__POR_VALUE    0x0
#define UART__MSR__RI__POR_VALUE    0x0
#define UART__MSR__DCD__POR_VALUE    0x0
#define UART__MSR__RSVD_MSR_31TO8__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: SCR
// Scratchpad Register.
///////////////////////////////////////////////////////
#define UART__SCR__NUM  0x1

#define UART__SCR__SCR__SHIFT    0
#define UART__SCR__RSVD_SCR_31TO8__SHIFT    8

#define UART__SCR__SCR__MASK    0x000000ff
#define UART__SCR__RSVD_SCR_31TO8__MASK    0xffffff00

#define UART__SCR__SCR__POR_VALUE    0x0
#define UART__SCR__RSVD_SCR_31TO8__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: FAR
// FIFO Access Register
///////////////////////////////////////////////////////
#define UART__FAR__OFFSET  0x70
#define UART__FAR__NUM  0x1

#define UART__FAR__FAR__SHIFT    0
#define UART__FAR__RSVD_FAR_31TO1__SHIFT    1

#define UART__FAR__FAR__MASK    0x00000001
#define UART__FAR__RSVD_FAR_31TO1__MASK    0xfffffffe

#define UART__FAR__FAR__POR_VALUE    0x0
#define UART__FAR__RSVD_FAR_31TO1__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: TFR
// This register is valid only when the DW_apb_uart is configured to have the FIFO access test mode available (FIFO_ACCESS = YES). If not configured, this register does not exist and reading from this register address returns 0.
///////////////////////////////////////////////////////
#define UART__TFR__OFFSET 0x74
#define UART__TFR__NUM  0x1

#define UART__TFR__TFR__SHIFT    0
#define UART__TFR__RSVD_TFR_31TO8__SHIFT    8

#define UART__TFR__TFR__MASK    0x000000ff
#define UART__TFR__RSVD_TFR_31TO8__MASK    0xffffff00

#define UART__TFR__TFR__POR_VALUE    0x0
#define UART__TFR__RSVD_TFR_31TO8__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: RFW
// This register is valid only when the DW_apb_uart is configured to have the FIFO access test mode available (FIFO_ACCESS = YES). If not configured, this register does not exist and reading from this register address returns 0.
///////////////////////////////////////////////////////
#define UART__RFW__OFFSET  0x78
#define UART__RFW__NUM  0x1

#define UART__RFW__RFWD__SHIFT    0
#define UART__RFW__RFPE__SHIFT    8
#define UART__RFW__RFFE__SHIFT    9
#define UART__RFW__RSVD_RFW_31TO10__SHIFT    10

#define UART__RFW__RFWD__MASK    0x000000ff
#define UART__RFW__RFPE__MASK    0x00000100
#define UART__RFW__RFFE__MASK    0x00000200
#define UART__RFW__RSVD_RFW_31TO10__MASK    0xfffffc00

#define UART__RFW__RFWD__POR_VALUE    0x0
#define UART__RFW__RFPE__POR_VALUE    0x0
#define UART__RFW__RFFE__POR_VALUE    0x0
#define UART__RFW__RSVD_RFW_31TO10__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: USR
// UART Status register.
///////////////////////////////////////////////////////
#define UART__USR__OFFSET  0x7C
#define UART__USR__NUM   0x1

#define UART__USR__BUSY__SHIFT    0
#define UART__USR__RSVD_TFNF__SHIFT    1
#define UART__USR__RSVD_TFE__SHIFT    2
#define UART__USR__RSVD_RFNE__SHIFT    3
#define UART__USR__RSVD_RFF__SHIFT    4
#define UART__USR__RSVD_USR_31TO5__SHIFT    5

#define UART__USR__BUSY__MASK    0x00000001
#define UART__USR__RSVD_TFNF__MASK    0x00000002
#define UART__USR__RSVD_TFE__MASK    0x00000004
#define UART__USR__RSVD_RFNE__MASK    0x00000008
#define UART__USR__RSVD_RFF__MASK    0x00000010
#define UART__USR__RSVD_USR_31TO5__MASK    0xffffffe0

#define UART__USR__BUSY__POR_VALUE    0x0
#define UART__USR__RSVD_TFNF__POR_VALUE    0x1
#define UART__USR__RSVD_TFE__POR_VALUE    0x1
#define UART__USR__RSVD_RFNE__POR_VALUE    0x0
#define UART__USR__RSVD_RFF__POR_VALUE    0x0
#define UART__USR__RSVD_USR_31TO5__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: HTX
// Halt TX
///////////////////////////////////////////////////////
#define UART__HTX__OFFSET  0xA4
#define UART__HTX__NUM  0x1

#define UART__HTX__HTX__SHIFT    0
#define UART__HTX__RSVD_HTX_31TO1__SHIFT    1

#define UART__HTX__HTX__MASK    0x00000001
#define UART__HTX__RSVD_HTX_31TO1__MASK    0xfffffffe

#define UART__HTX__HTX__POR_VALUE    0x0
#define UART__HTX__RSVD_HTX_31TO1__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMASA
// DMA Software Acknowledge Register
///////////////////////////////////////////////////////
#define UART__DMASA__OFFSET  0xA8
#define UART__DMASA__NUM  0x1

#define UART__DMASA__DMASA__SHIFT    0
#define UART__DMASA__RSVD_DMASA_31TO1__SHIFT    1

#define UART__DMASA__DMASA__MASK    0x00000001
#define UART__DMASA__RSVD_DMASA_31TO1__MASK    0xfffffffe

#define UART__DMASA__DMASA__POR_VALUE    0x0
#define UART__DMASA__RSVD_DMASA_31TO1__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: TCR
// This register is used to enable or disable RS485 mode and also control the polarity values for Driven enable (de) and Receiver Enable (re) signals.
///////////////////////////////////////////////////////
#define UART__TCR__OFFSET  0xAC
#define UART__TCR__NUM  0x1

#define UART__TCR__RS485_EN__SHIFT    0
#define UART__TCR__RE_POL__SHIFT    1
#define UART__TCR__DE_POL__SHIFT    2
#define UART__TCR__XFER_MODE__SHIFT    3
#define UART__TCR__RSVD_TCR_31TO5__SHIFT    5

#define UART__TCR__RS485_EN__MASK    0x00000001
#define UART__TCR__RE_POL__MASK    0x00000002
#define UART__TCR__DE_POL__MASK    0x00000004
#define UART__TCR__XFER_MODE__MASK    0x00000018
#define UART__TCR__RSVD_TCR_31TO5__MASK    0xffffffe0

#define UART__TCR__RS485_EN__POR_VALUE    0x0
#define UART__TCR__RE_POL__POR_VALUE    0x1
#define UART__TCR__DE_POL__POR_VALUE    0x1
#define UART__TCR__XFER_MODE__POR_VALUE    0x0
#define UART__TCR__RSVD_TCR_31TO5__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DE_EN
// The Driver Output Enable Register (DE_EN) is used to control the assertion and de-assertion of the DE signal.
///////////////////////////////////////////////////////
#define UART__DE_EN__OFFSET  0xB0
#define UART__DE_EN__NUM  0x1

#define UART__DE_EN__DE_ENABLE__SHIFT    0
#define UART__DE_EN__RSVD_DE_EN_31TO1__SHIFT    1

#define UART__DE_EN__DE_ENABLE__MASK    0x00000001
#define UART__DE_EN__RSVD_DE_EN_31TO1__MASK    0xfffffffe

#define UART__DE_EN__DE_ENABLE__POR_VALUE    0x0
#define UART__DE_EN__RSVD_DE_EN_31TO1__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: RE_EN
// The Receiver Output Enable Register (RE_EN) is used to control the assertion and de-assertion of the RE signal.
///////////////////////////////////////////////////////
#define UART__RE_EN__OFFSET  0xB4
#define UART__RE_EN__NUM  0x1

#define UART__RE_EN__RE_ENABLE__SHIFT    0
#define UART__RE_EN__RSVD_RE_EN_31TO1__SHIFT    1

#define UART__RE_EN__RE_ENABLE__MASK    0x00000001
#define UART__RE_EN__RSVD_RE_EN_31TO1__MASK    0xfffffffe

#define UART__RE_EN__RE_ENABLE__POR_VALUE    0x0
#define UART__RE_EN__RSVD_RE_EN_31TO1__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DET
// The Driver Output Enable Timing Register (DET) is used to control the DE assertion and de-assertion timings of 'de' signal.
///////////////////////////////////////////////////////
#define UART__DET__OFFSET  0xB8
#define UART__DET__NUM  0x1

#define UART__DET__DE_ASSERTION_TIME__SHIFT    0
#define UART__DET__RSVD_DE_AT_15TO8__SHIFT    8
#define UART__DET__DE_DEASSERTION_TIME__SHIFT    16
#define UART__DET__RSVD_DE_DEAT_31TO24__SHIFT    24

#define UART__DET__DE_ASSERTION_TIME__MASK    0x000000ff
#define UART__DET__RSVD_DE_AT_15TO8__MASK    0x0000ff00
#define UART__DET__DE_DEASSERTION_TIME__MASK    0x00ff0000
#define UART__DET__RSVD_DE_DEAT_31TO24__MASK    0xff000000

#define UART__DET__DE_ASSERTION_TIME__POR_VALUE    0x0
#define UART__DET__RSVD_DE_AT_15TO8__POR_VALUE    0x0
#define UART__DET__DE_DEASSERTION_TIME__POR_VALUE    0x0
#define UART__DET__RSVD_DE_DEAT_31TO24__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: TAT
// The TurnAround Timing Register (TAT) is used to hold the turnaround time between switching of 're' and 'de' signals.
///////////////////////////////////////////////////////
#define UART__TAT__OFFSET 0xBC
#define UART__TAT__NUM  0x1

#define UART__TAT__DE_TO_RE__SHIFT    0
#define UART__TAT__RE_TO_DE__SHIFT    16

#define UART__TAT__DE_TO_RE__MASK    0x0000ffff
#define UART__TAT__RE_TO_DE__MASK    0xffff0000

#define UART__TAT__DE_TO_RE__POR_VALUE    0x0
#define UART__TAT__RE_TO_DE__POR_VALUE    0x0


/**************************** Type Definitions ******************************/
typedef struct
{
    AL_U32                  DeviceId;
    AL_U32                  BaseAddress;
    AL_U32                  InputClockHz;
    AL_U32                  ModemPinsConnected;
    AL_U32                  IrqNum;
} AL_UART_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
