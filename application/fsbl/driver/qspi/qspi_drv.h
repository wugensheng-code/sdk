
#ifndef _AL9000_QSPI_DRV_H_
#define _AL9000_QSPI_DRV_H_

#include <stdio.h>
#include <stdint.h>

#define     __IOM           volatile

#define PERIPH_BASE         (0xF8000000UL)                      /*!< (Peripheral) Base Address */
#define QSPI0_BASE          (PERIPH_BASE + 0x0004E000)               /*!< (QSPI0) Base Address */

#define QSPI_READ_REG32(reg)  (*(__IOM uint32_t *)(reg))
#define QSPI_WRITE_REG32(reg,value) (*(__IOM uint32_t *)(reg) = (value))
#define QSPI_MODIFY_REG32(reg,clear_mask,set_mask) \
   QSPI_WRITE_REG32((reg), ((( QSPI_READ_REG32(reg)) & ~(clear_mask)) | (set_mask)) )
   
   
   /* bits definitions */
#define QSPI_EN             1   /*Enables DWC_ssi*/
#define QSPI_DIS            0   /*Disables DWC_ssi*/

#define DFS_BYTE            (7) // 7+1=8 bits=byte
#define DFS_HALFWORD        (15)// 15+1=16 bits=halfword
#define DFS_WORD            (31)// 31+1=32 bits=word

#define NDF_RCV_FLASHID_BYTE  2  // receive 3+1 data items

#define SPI_FRF_X1_MODE                     0
#define SPI_FRF_X2_MODE                     1
#define SPI_FRF_X4_MODE                     2
#define SPI_FRF_X8_MODE                     3

#define TMOD_TX_RX                          0
#define TMOD_TX_ONLY                        1
#define TMOD_RX_ONLY                        2
#define TMOD_EEPROM                         3

#define MOTOROLA_SPI_FMT                       0
#define TI_SSP_FMT                             1
#define NS_MICR_FMT                            2

#define CTRLR0_SSTE_EN                      1
#define CTRLR0_SSTE_DIS                     0

#define SLAVE_CS0_SEL                      1
#define SLAVE_CS1_SEL                      2
#define SLAVE_ALL_SEL                      2

#define SPI_TXFTLR_TXFTHR(x)                (((x) & 0xff) << 16)


#define QSPI_SR_TXFIFO_EMPTY                1
#define QSPI_SR_TXFIFO_NOTEMPTY             0


#define QSPI_SR_RXFIFO_NOTEMPTY             1
#define QSPI_SR_RXFIFO_EMPTY                0

#define QSPI_SR_BUSY                        1
#define QSPI_SR_IDLE                        0

#define     SPI_TRANSFER_TYPE0                        0
#define     SPI_TRANSFER_TYPE1                        1
#define     SPI_TRANSFER_TYPE2                        2

#define     SPI_NO_ADDR                               4
#define     SPI_FLASH_ADDR24                          6
#define     SPI_FLASH_ADDR32                          8


   
typedef struct {
    __IOM uint32_t CTRLR0;              /*offset 0x0*/
    __IOM uint32_t CTRLR1;              /*offset 0x4*/
    __IOM uint32_t SSIENR;              /*offset 0x8*/
    __IOM uint32_t MWCR;                /*offset 0xC*/
    __IOM uint32_t SER;                 /*offset 0x10*/
    __IOM uint32_t BAUDR;               /*offset 0x14*/
    __IOM uint32_t TXFTLR;              /*offset 0x18*/
    __IOM uint32_t RXFTLR;              /*offset 0x1c*/
    __IOM uint32_t TXFLR;               /*offset 0x20*/
    __IOM uint32_t RXFLR;               /*offset 0x24*/
    __IOM uint32_t SR;                  /*offset 0x28*/
    __IOM uint32_t IMR;                 /*offset 0x2C*/
    __IOM uint32_t ISR;                 /*offset 0x30*/
    __IOM uint32_t RISR;                /*offset 0x34*/
    __IOM uint32_t TXOICR;              /*offset 0x38*/
    __IOM uint32_t RXOICR;              /*offset 0x3c*/
    __IOM uint32_t RXUICR;              /*offset 0x40*/
    __IOM uint32_t MSTICR;              /*offset 0x44*/
    __IOM uint32_t ICR;                 /*offset 0x48*/
    __IOM uint32_t DMACR;               /*offset 0x4c*/
    __IOM uint32_t DMATDLR;             /*offset 0x50*/
    __IOM uint32_t DMARDLR;             /*offset 0x54*/
    __IOM uint32_t IDR;                 /*offset 0x58*/
    __IOM uint32_t SSIC_VERSION_ID;     /*offset 0x5c*/
    __IOM uint32_t DRx;             /*offset 0x60+i*0x4, i=[0..35]*/
    __IOM uint32_t DRxRev[35];             /*offset 0x60+i*0x4, i=[0..35]*/
    __IOM uint32_t RX_SAMPLE_DELAY;     /*offset 0xf0*/
    __IOM uint32_t SPI_CTRLR0;          /*offset 0xf4*/
    __IOM uint32_t DDR_DRIVE_EDGE;      /*offset 0xf8*/
    __IOM uint32_t XIP_MODE_BITS;       /*offset 0xfc*/
} QSPI_REGS;


typedef union _CTRLR0_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t DFS                :5;   /*[4:0]-Data Frame Size.*/
        uint32_t RSVD_CTRLR0_5      :1;   /*[5]-RSVD*/
        uint32_t FRF                :2;   /*[7:6]-Frame Format.0x0(SPI);0x1(SSP);0x2(MICROWIRE);default=0,SPI*/
        uint32_t SCPH               :1;   /*[8]-Serial Clock Phase.--fixed by HW*/
        uint32_t SCPOL              :1;   /*[9]-Serial Clock Polarity.--fixed by HW*/
        uint32_t TMOD               :2;   /*[11:10]-Transfer Mode. 0x0(TX_AND_RX);0x1(TX_ONLY);0x2(RX_ONLY);0x3(EEPROM_READ)*/
        uint32_t SLV_OE             :1;   /*[12]-Slave Output Enable.1-dis;0-en*/
        uint32_t SRL                :1;   /*[13]-Shift Register Loop.*/
        uint32_t SSTE               :1;   /*[14]-Slave Select Toggle Enable.1-en,0-dis*/
        uint32_t RSVD_CTRLR0_15     :1;   /*[15]-RSVD*/
        uint32_t CFS                :4;   /*[19:16]-Control Frame Size.--Microwire,,not use*/
        uint32_t RSVD_CTRLR0_20_21  :2;   /*[21:20]-RSVD */
        uint32_t SPI_FRF            :2;   /*[23:22]-SPI Frame Format.0(x1),1(x2),2(x4),3(x8) mode*/
        uint32_t SPI_HYPERBUS_EN    :1;   /*[24]-SPI Hyperbus Frame format enable. */
        uint32_t RSVD_CTRLR0_25_30  :6;   /*[30:25]-RSVD */
        uint32_t SSI_IS_MST         :1;   /*[31]-DWC_ssi =1,Master or =0,Slave mode; defualt=1;*/
   }
   b;
}CTRLR0_TypeDef;

typedef union _CTRLR1_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t NDF                :16;   /*[4:0]-Data Frame Size.*/
        uint32_t RSVD_CTRLR1        :16;   /*[5]-RSVD*/
   }
   b;
}CTRLR1_TypeDef;

typedef union _SSIENR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t SSIC_EN                :1;   /*[4:0]-Data Frame Size.*/
        uint32_t RSVD_CTRLR1            :31;   /*[5]-RSVD*/
   }
   b;
}SSIENR_TypeDef;

typedef union _MWCR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t MWMOD                  :1;   /*[4:0]-Data Frame Size.*/
        uint32_t MDD                    :1;   /*[5]-RSVD*/
        uint32_t MHS                    :1;   /*[5]-RSVD*/
        uint32_t RSVD_MWCR              :29;   /*[5]-RSVD*/
   }
   b;
}MWCR_TypeDef;


typedef union _SER_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t SER                    :2;   /*[4:0]-Data Frame Size.*/
        uint32_t RSVD_SER               :30;   /*[5]-RSVD*/
   }
   b;
}SER_TypeDef;

typedef union _BAUDR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t RSVD_BAUDR_0           :1;     /*[4:0]-Data Frame Size.*/
        uint32_t SCKDV                  :15;    /*[5]-RSVD*/
        uint32_t RSVD_BAUDR_1           :16;    /*[4:0]-Data Frame Size.*/
   }
   b;
}BAUDR_TypeDef;

typedef union _TXFTLR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t TFT                :8;     /*Transmit FIFO Threshold*/
        uint32_t RSVD_TXFTLR_0      :8;   /*[5]-RSVD*/
        uint32_t TXFTHR             :8;   /*[4:0]-Data Frame Size.*/
        uint32_t RSVD_TXFTLR_1      :8;   /*[4:0]-Data Frame Size.*/
   }
   b;
}TXFTLR_TypeDef;


typedef union _RXFTLR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t RFT                :8;     /*[4:0]-Data Frame Size.*/
        uint32_t RSVD_RXFTLR_0      :24;   /*[5]-RSVD*/
   }
   b;
}RXFTLR_TypeDef;

typedef union _TXFLR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t TXTFL              :9;     /*[4:0]-Data Frame Size.*/
        uint32_t RSVD_TXFLR         :23;   /*[5]-RSVD*/
   }
   b;
}TXFLR_TypeDef;


typedef union _RXTFL_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t RXTFL              :9;     /*[4:0]-Data Frame Size.*/
        uint32_t RSVD_RXTFL         :23;   /*[5]-RSVD*/
   }
   b;
}RXTFL_TypeDef;

typedef union _SR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t BUSY               :1;     /*SSI Busy Flag.*/
        uint32_t TFNF               :1;     /*Transmit FIFO Not Full*/
        uint32_t TFE                :1;     /*Transmit FIFO Empty*/
        uint32_t RFNE               :1;     /*Receive FIFO Not Empty*/
        uint32_t RFF                :1;     /*Receive FIFO Full.*/
        uint32_t TXE                :1;     /*Transmission Error*/
        uint32_t DCOL               :1;     /*Data Collision Error*/
        uint32_t RSVD_SR            :25;    
   }
   b;
}SR_TypeDef;

typedef union _IMR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t TXEIM              :1;     /*Transmit FIFO Empty Interrupt Mask.*/
        uint32_t TXOIM              :1;     /*Transmit FIFO Overflow Interrupt Mask*/
        uint32_t RXUIM              :1;     /*Receive FIFO Underflow Interrupt Mask*/
        uint32_t RXOIM              :1;     /*Receive FIFO Overflow Interrupt Mask*/
        uint32_t RXFIM              :1;     /*Receive FIFO Full Interrupt Mask 0.*/
        uint32_t MSTIM              :1;     /*Multi-Master Contention Interrupt Mask*/
        uint32_t XRXOIM             :1;     /*XIP Receive FIFO Overflow Interrupt Mask*/
        uint32_t RSVD_IMR            :25;    
   }
   b;
}IMR_TypeDef;

typedef union _ISR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t TXEIS              :1;     /*Transmit FIFO Empty Interrupt Status.*/
        uint32_t TXOIS              :1;     /*Transmit FIFO Overflow Interrupt Status*/
        uint32_t RXUIS              :1;     /*Receive FIFO Underflow Interrupt Status*/
        uint32_t RXOIS              :1;     /*Receive FIFO Overflow Interrupt Status*/
        uint32_t RXFIS              :1;     /*Receive FIFO Full Interrupt Status*/
        uint32_t MSTIS              :1;     /*Multi-Master Contention Interrupt Status*/
        uint32_t XRXOIS             :1;     /*XIP Receive FIFO Overflow Interrupt Status*/
        uint32_t RSVD_ISR            :25;    
   }
   b;
}ISR_TypeDef;


typedef union _RISR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t TXEIR              :1;     /*Transmit FIFO Empty Raw Interrupt Status.*/
        uint32_t TXOIR              :1;     /*Transmit FIFO Overflow Raw Interrupt Status*/
        uint32_t RXUIR              :1;     /*Receive FIFO Underflow Raw Interrupt Status*/
        uint32_t RXOIR              :1;     /*Receive FIFO Overflow Raw Interrupt Status*/
        uint32_t RXFIR              :1;     /*Receive FIFO Full Raw Interrupt Status*/
        uint32_t MSTIR              :1;     /*Multi-Master Contention Raw Interrupt Status*/
        uint32_t XRXOIR             :1;     /*XIP Receive FIFO Overflow Raw Interrupt Status*/
        uint32_t RSVD_RISR          :25;    
   }
   b;
}RISR_TypeDef;

typedef union _TXOICR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t TXOICR              :1;     /*Clear Transmit FIFO Overflow Interrupt.*/
        uint32_t RSVD_TXOICR        :31;     /*Reserved bits*/
   }
   b;
}TXOICR_TypeDef;

typedef union _RXOICR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t RXOICR              :1;     /*Clear Receive FIFO Overflow Interrupt.*/
        uint32_t RSVD_RXOICR        :31;     /*Reserved bits*/
   }
   b;
}RXOICR_TypeDef;

typedef union _RXUICR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t RXUICR              :1;     /*Clear Receive FIFO Underflow Interrupt.*/
        uint32_t RSVD_RXUICR        :31;     /*Reserved bits*/
   }
   b;
}RXUICR_TypeDef;

typedef union _MSTICR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t MSTICR              :1;     /*Multi-Master Interrupt Clear Register.*/
        uint32_t RSVD_MSTICR        :31;     /*Reserved bits*/
   }
   b;
}MSTICR_TypeDef;


typedef union _DMACR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t RDMAE              :1;     /*Receive DMA Enable*/
        uint32_t TDMAE              :1;     /*Transmit DMA Enable*/
        uint32_t RSVD_DMACR         :30;     /*Reserved bits*/
   }
   b;
}DMACR_TypeDef;

typedef union _DMATDLR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t DMATDL              :8;     /*Transmit Data Level*/
        uint32_t RSVD_DMATDLR        :24;     /*Reserved bits*/
   }
   b;
}DMATDLR_TypeDef;

typedef union _DMARDLR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t DMARDL              :8;     /*Transmit Data Level*/
        uint32_t RSVD_DMARDLR        :24;     /*Reserved bits*/
   }
   b;
}DMARDLR_TypeDef;

typedef union _IDR_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t IDCODE              :32;     /*Identification Register*/
   }
   b;
}IDR_TypeDef;

typedef union _VERSION_ID_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t VERSION_ID  :32;     /*VERSION_ID*/
   }
   b;
}VERSION_ID_TypeDef;

typedef union _DRX_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t DR0  :32;     /*Data Register.*/
   }
   b;
}DRX_TypeDef;

typedef union _RX_SAMPLE_DELAY_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t RSD        :8;     /*Receive Data (rxd) Sample Delay.*/
        uint32_t RSVD0      :8;     /*Reserved bits.*/
        uint32_t SE         :1;     /*Receive Data (rxd) Sampling Edge*/
        uint32_t RSVD1      :15;     /*Reserved bits.*/
   }
   b;
}RX_SAMPLE_DELAY_TypeDef;


typedef union _SPI_CTRLR0_TypeDef
{
    uint32_t d32;
    struct
    {
        uint32_t TRANS_TYPE         :2;   /*[1:0]-Address and instruction transfer format.*/
        uint32_t ADDR_L             :4;   /*[5:2]-This bit defines Length of Address to be transmitted.*/
        uint32_t RSVD_6             :1;   /*[6]-RSVD*/
        uint32_t XIP_MD_BIT_EN      :1;   /*[7]-Mode bits enable in XIP mode.*/
        uint32_t INST_L             :2;   /*[9:8]-Dual/Quad/Octal mode instruction length in bits.*/
        uint32_t RSVD_10            :1;   /*[10]-RSVD*/
        uint32_t WAIT_CYCLES        :5;   /*[15:11]-Wait cycles in Dual/Quad/Octal mode between control frames transmit and data reception.*/
        uint32_t SPI_DDR_EN         :1;   /*[16]-SPI DDR Enable bit.*/
        uint32_t INST_DDR_EN        :1;   /*[17]-Instruction DDR Enable bit.*/
        uint32_t SPI_RXDS_EN        :1;   /*[18]-Read data strobe enable bit.*/
        uint32_t XIP_DFS_HC         :1;   /*[19]-Fix DFS for XIP transfers.*/
        uint32_t XIP_INST_EN        :1;   /*[20]-XIP instruction enable bit.*/
        uint32_t SSIC_XIP_CONT_XFER_EN  :1; /*[21]-Enable continuous transfer in XIP mode.*/
        uint32_t RSVD_23_22         :2;   /*[23:22]-RSVD*/
        uint32_t SPI_DM_EN          :1;   /*[24]-SPI data mask enable bit.*/
        uint32_t SPI_RXDS_SIG_EN    :1;   /*[25]-Enable rxds signaling during address and command phase of Hypebus transfer.*/
        uint32_t XIP_MBL            :2;   /*[27:26]-XIP Mode bits length.*/
        uint32_t RSVD_28            :1;   /*[28]-RSVD*/
        uint32_t XIP_PREFETCH_EN    :1;   /*[29]-Enables XIP pre-fetch functionality in DWC_ssi. */
        uint32_t CLK_STRETCH_EN     :1;   /*[30]-Enables clock stretching capability in SPI transfers.*/
        uint32_t RSVD_31            :1;   /*[31]-RSVD*/
    }
    b;
}SPI_CTRLR0_TypeDef;

typedef union _DDR_DRIVE_EDGE_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t TDE        :8;     /*TXD Drive edge register which decided the driving edge of transmit data*/
        uint32_t RSVD0      :24;     /*Reserved bits.*/
   }
   b;
}DDR_DRIVE_EDGE_TypeDef;

typedef union _XIP_MODE_BITS_TypeDef
{  uint32_t d32;
   struct 
   {
        uint32_t XIP_MD_BITS    :16;     /*TXD Drive edge register which decided the driving edge of transmit data*/
        uint32_t RSVD0          :16;     /*Reserved bits.*/
   }
   b;
}XIP_MODE_BITS_TypeDef;

typedef struct _QSPI_core_cfg
{
  uint32_t      flashID;
  uint32_t      flashSize;
  uint8_t       is_enable;
  uint8_t       width;
  uint8_t       dfs;
  uint8_t       mode;
  uint8_t       enhanced;
  uint8_t       addr_mode;
  uint8_t       speed;
}
QSPI_CORE_CFGS, *PQSPI_CORE_CFGS;

typedef struct _QSPI_handle
{
  QSPI_CORE_CFGS        cfg;
  QSPI_REGS            *regs;
}QSPI_CORE_HANDLE , *PQSPI_CORE_HANDLE;

int32_t Qspi_Init(QSPI_CORE_HANDLE *pdev);
int32_t Qspi_Disable(QSPI_CORE_HANDLE *pdev);
int32_t Qspi_Enable(QSPI_CORE_HANDLE *pdev);
int32_t Qspi_Width(QSPI_CORE_HANDLE *pdev, uint32_t spi_width);
int32_t Qspi_Mode(QSPI_CORE_HANDLE *pdev, uint32_t mode);
#ifdef   QSPI_EXTENSION_FUNCTION
int32_t Qspi_Sste(QSPI_CORE_HANDLE *pdev, uint32_t sste_mode);
void Qspi_ClkStretchEn(QSPI_CORE_HANDLE *pdev, uint32_t val);
#endif
int32_t Qspi_TransType(QSPI_CORE_HANDLE *pdev, uint32_t type);
int32_t Qspi_Dfs(QSPI_CORE_HANDLE *pdev, uint32_t len);
int32_t Qspi_Ser(QSPI_CORE_HANDLE *pdev, uint32_t slv);
int32_t Qspi_SckdivCfg(QSPI_CORE_HANDLE *pdev, uint32_t div);
int32_t Qspi_TxftlTft(QSPI_CORE_HANDLE *pdev, uint32_t Threshold);
int32_t Qspi_TxfifoStartLevelSet(QSPI_CORE_HANDLE *pdev, uint32_t start_level);
int32_t Qspi_RxftlCfg(QSPI_CORE_HANDLE *pdev, uint32_t Threshold);
int32_t Qspi_DataTransmit(QSPI_CORE_HANDLE *pdev, uint32_t data);
uint32_t Qspi_DataRead(QSPI_CORE_HANDLE *pdev);
uint32_t Qspi_TxfifoEmpty(QSPI_CORE_HANDLE *pdev);
uint32_t Qspi_Busy(QSPI_CORE_HANDLE *pdev);
uint32_t Qspi_RxNotempty(QSPI_CORE_HANDLE *pdev);
void Qspi_Ctrl1Ndf(QSPI_CORE_HANDLE *pdev, uint32_t num);
void Qspi_AddrMode(QSPI_CORE_HANDLE *pdev, uint32_t val);
void Qspi_InstructionLength(QSPI_CORE_HANDLE *pdev, uint32_t val);
void Qspi_WaitCycles(QSPI_CORE_HANDLE *pdev, uint32_t val);


#ifdef QSPI_DEBUG
void Qspi_Info(QSPI_CORE_HANDLE *pdev);
#endif


#endif /* _AL9000_QSPI_DRV_H_ */
