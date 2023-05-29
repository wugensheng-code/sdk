#ifndef _AL9000_QSPI_FLASH_H_
#define _AL9000_QSPI_FLASH_H_

#include "al_types.h"

#define QSPI_ENHANCE_MODE_INCLUDE   1


#define XQSPIPSU_SELECT_MODE_SPI 1

#define XQSPIPSU_MSG_FLAG_TX  1
#define XQSPIPSU_MSG_FLAG_RX  2


//error code define
#define TIMER_OUT_ERR0 10
#define TIMER_OUT_ERR1 11
#define TIMER_OUT_ERR2 12
#define TIMER_OUT_ERR3 13
#define TIMER_OUT_ERR4 14
#define TIMER_OUT_ERR5 15
#define TIMER_OUT_ERR6 16
#define TIMER_OUT_ERR7 17
#define TIMER_OUT_ERR8 18
#define TIMER_OUT_ERR9 19


#define DATA_XFER_ERR0 20
#define DATA_XFER_ERR1 21
#define DATA_XFER_ERR2 22
#define DATA_XFER_ERR3 23
#define DATA_XFER_ERR4 24
#define DATA_XFER_ERR5 25

#define FLASH_ID_ERR0  1
#define FLASH_ID_ERR1  2
#define FLASH_ID_ERR2  3
#define FLASH_ID_ERR3  4
#define FLASH_ID_ERR4  5


#define FLASH_READ_ERR0 30
#define FLASH_READ_ERR1 31

#define QSPI_MAX_SIZE 0X10000  //MAX 64K ,ctrlr0 +1 = qspi max size
#define QSPI_FIFO_MAX_SIZE  128

/*
 * The following constants define the commands which may be sent to the Flash
 * device.
 */
#define WRITE_STATUS_CMD    0x01
#define WRITE_CMD           0x02
#define READ_CMD            0x03
#define WRITE_DISABLE_CMD   0x04
#define READ_STATUS_CMD     0x05
#define WRITE_ENABLE_CMD    0x06
#define FAST_READ_CMD       0x0B
#define DUAL_READ_CMD       0x3B
#define QUAD_READ_CMD       0x6B
#define BULK_ERASE_CMD      0xC7
#define SEC_ERASE_CMD       0xD8
#define READ_ID             0x9F
#define READ_CONFIG_CMD     0x35
#define WRITE_CONFIG_CMD    0x01
#define BANK_REG_RD         0x16
#define BANK_REG_WR         0x17
/* Bank register is called Extended Address Register in Micron */
#define EXTADD_REG_RD       0xC8
#define EXTADD_REG_WR       0xC5
#define DIE_ERASE_CMD       0xC4
#define READ_FLAG_STATUS_CMD    0x70

#define EN4B_MACRONIX_CMD   0xB7
#define QSPI_MACRONIX_EN_CMD 0XF5
#define QSPI_MACRONIX_DIS_CMD 0X35


/*
 * The following constants define the commands which may be sent to the FLASH
 * device.
 */
#define READ_ID_CMD             (0x9F)

#define BASE_READ_CMD_24BIT     (0x03)
#define FAST_READ_CMD_24BIT     (0x0B)
#define DUAL_READ_CMD_24BIT     (0x3BU)
#define QUAD_READ_CMD_24BIT     (0x6BU)

#define BASE_READ_CMD_32BIT     (0x13U)
#define FAST_READ_CMD_32BIT     (0x0CU)
#define DUAL_READ_CMD_32BIT     (0x3CU)
#define QUAD_READ_CMD_32BIT     (0x6CU)
#define QUAD_READ_CMD_24BIT2    (0xEBU)


#define BANK_REG_RD_CMD     (0x16U)
#define BANK_REG_WR_CMD     (0x17U)
/* Bank register is called Extended Address Reg in Micron */
#define EXTADD_REG_RD_CMD   (0xC8U)
#define EXTADD_REG_WR_CMD   (0xC5U)

#define COMMAND_OFFSET      (0U) /* FLASH instruction */
#define ADDRESS_1_OFFSET    (1U) /* MSB byte of address to read or write */
#define ADDRESS_2_OFFSET    (2U) /* Middle byte of address to read or write */
#define ADDRESS_3_OFFSET    (3U) /* Middle byte of address to read or write */
#define ADDRESS_4_OFFSET    (4U) /* LSB byte of address to read or write */
#define DATA_OFFSET         (4U) /* Start of Data for Read/Write */
#define DUMMY_OFFSET        (4U) /* Dummy byte offset for fast, dual and quad
                     reads */
#define DUMMY_SIZE          (1U) /* Number of dummy bytes for fast, dual and
                     quad reads */
#define DUMMY_CLOCKS        8 /* Number of dummy bytes for fast, dual and
                     quad reads */
#define DUMMY_CLOCKS_MACRONIX   6 /* For 4-4-4 mode in Macronix dummy cycles are
                    default to 6*/
#define RD_ID_SIZE          (4U) /* Read ID command + 3 bytes ID response */
#define BANK_SEL_SIZE       (2U) /* BRWR or EARWR command + 1 byte bank value */
#define WRITE_ENABLE_CMD_SIZE   (1U) /* WE command */
/*
 * The following constants specify the extra bytes which are sent to the
 * FLASH on the QSPI interface, that are not data, but control information
 * which includes the command and address
 */
#define OVERHEAD_SIZE       (4U)


/**
 * Flash connection type as defined in PCW
 */
#define FLASH_SIZE_16MB         (0x1000000U)
#define BANKSIZE            (FLASH_SIZE_16MB)
#define SINGLEBANKSIZE          BANKSIZE
#define BANKSIZE_64MB           (0x4000000U)
#define BANKMASK_64MB           (~(BANKSIZE_64MB - 1U))

/*
 * Bank mask
 */
#define BANKMASK            (0xFFFFFFFFU & ~(BANKSIZE - 1))
#define SINGLEBANKMASK          BANKMASK

/*
 * Identification of Flash
 * Micron:
 * Byte 0 is Manufacturer ID;
 * Byte 1 is first byte of Device ID - 0xBB or 0xBA
 * Byte 2 is second byte of Device ID describes flash size:
 * 128Mbit : 0x18; 256Mbit : 0x19; 512Mbit : 0x20
 * Spansion:
 * Byte 0 is Manufacturer ID;
 * Byte 1 is Device ID - Memory Interface type - 0x20 or 0x02
 * Byte 2 is second byte of Device ID describes flash size:
 * 128Mbit : 0x18; 256Mbit : 0x19; 512Mbit : 0x20
 */

#define MICRON_ID       (0x20U)
#define SPANSION_ID     (0x01U)
#define WINBOND_ID      (0xEFU)
#define MACRONIX_ID     (0xC2U)
#define ISSI_ID         (0x9DU)

#define PUYA_ID         (0x85U)
#define GIGA_ID         (0xc8U)

#define FLASH_SIZE_ID_2M        (0x12U)
#define FLASH_SIZE_ID_4M        (0x13U)
#define FLASH_SIZE_ID_8M        (0x14U)
#define FLASH_SIZE_ID_16M       (0x15U)
#define FLASH_SIZE_ID_32M       (0x16U)
#define FLASH_SIZE_ID_64M       (0x17U)
#define FLASH_SIZE_ID_128M      (0x18U)
#define FLASH_SIZE_ID_256M      (0x19U)
#define FLASH_SIZE_ID_512M      (0x20U)
#define FLASH_SIZE_ID_1G        (0x21U)
#define FLASH_SIZE_ID_2G        (0x22U)
/* Macronix size constants are different for 512M and 1G */
#define MACRONIX_FLASH_SIZE_ID_512M     (0x1AU)
#define MACRONIX_FLASH_SIZE_ID_1G       (0x1BU)
#define MACRONIX_FLASH_SIZE_ID_2G       (0x1CU)
#define MACRONIX_FLASH_1_8_V_SIZE_ID_2G     (0x3CU)
#define MACRONIX_FLASH_1_8_V_SIZE_ID_1G     (0x3BU)
#define MACRONIX_FLASH_1_8_V_MX25_ID_256    (0x39U)
#define MACRONIX_FLASH_1_8_V_MX66_ID_512    (0x3AU)


/*
 * Size in bit
 */
 
#define FLASH_SIZE_2M           (0x40000U)
#define FLASH_SIZE_4M           (0x80000U)
#define FLASH_SIZE_8M           (0x100000U)
#define FLASH_SIZE_16M          (0x200000U)
#define FLASH_SIZE_32M          (0x400000U)
#define FLASH_SIZE_64M          (0x800000U)
#define FLASH_SIZE_128M         (0x1000000U)
#define FLASH_SIZE_256M         (0x2000000U)
#define FLASH_SIZE_512M         (0x4000000U)
#define FLASH_SIZE_1G           (0x8000000U)
#define FLASH_SIZE_2G           (0x10000000U)

/* Macronix */
#define DISABLE_QPI     0x0U
#define ENABLE_QPI      0x1U

#define QSPI_WIDTH_X1   0x0
#define QSPI_WIDTH_X2   0x1
#define QSPI_WIDTH_X4   0x2


#define  QSPI_ADDR_24    0x1
#define  QSPI_ADDR_32    0x2



/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

typedef struct {
    u8 *TxBfrPtr;
    u8 *RxBfrPtr;
    u32 txByteCount;
    u32 rxByteCount;
    u32 BusWidth;
    u32 Flags;
    /*
    u8  PollData;
    u32 PollTimeout;
    u8 PollStatusCmd;
    u8 PollBusMask;
    */
} QspiFlash_Msg;

typedef struct _QspiParams
{
  uint32_t      flashID;
  uint32_t      flashSize;
}QspiParams;

#define  COREN205_MTIMER_COUNTER64_LOW (0x68000)
#define  COREN205_MTIMER_COUNTER64_HI (0x68004)

u32 Csu_QspiRead(u32 offset, u8* dest, u32 length);
u32 Csu_QspiSetMode(u32 Mode, u32 AddrMode);
u32 Csu_QspiInit(QspiParams *params);

#endif /* _AL9000_SPI_H_ */
