#ifndef AL_SPINOR_H
#define AL_SPINOR_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BIT
#define BIT(nr)		(1UL << (nr))
#endif

/* Flash opcodes. */
#define NOR_OP_WRDI		0x04	/* Write disable */
#define NOR_OP_WREN		0x06	/* Write enable */
#define NOR_OP_RDSR		0x05	/* Read status register */
#define NOR_OP_WRSR		0x01	/* Write status register 1 byte */
#define NOR_OP_RDSR2		0x3f	/* Read status register 2 */
#define NOR_OP_WRSR2		0x3e	/* Write status register 2 */
#define NOR_OP_READ		0x03	/* Read data bytes (low frequency) */
#define NOR_OP_READ_FAST	0x0b	/* Read data bytes (high frequency) */
#define NOR_OP_READ_1_1_2	0x3b	/* Read data bytes (Dual Output ) */
#define NOR_OP_READ_1_2_2	0xbb	/* Read data bytes (Dual I/O ) */
#define NOR_OP_READ_1_1_4	0x6b	/* Read data bytes (Quad Output ) */
#define NOR_OP_READ_1_4_4	0xeb	/* Read data bytes (Quad I/O ) */
#define NOR_OP_READ_1_1_8	0x8b	/* Read data bytes (Octal Output ) */
#define NOR_OP_READ_1_8_8	0xcb	/* Read data bytes (Octal I/O ) */
#define NOR_OP_PP		0x02	/* Page program (up to 256 bytes) */
#define NOR_OP_PP_1_1_4	0x32	/* Quad page program */
#define NOR_OP_PP_1_4_4	0x38	/* Quad page program */
#define NOR_OP_PP_1_1_8	0x82	/* Octal page program */
#define NOR_OP_PP_1_8_8	0xc2	/* Octal page program */
#define NOR_OP_BE_4K		0x20	/* Erase 4KiB block */
#define NOR_OP_BE_4K_PMC	0xd7	/* Erase 4KiB block on PMC chips */
#define NOR_OP_BE_32K	0x52	/* Erase 32KiB block */
#define NOR_OP_CHIP_ERASE	0xc7	/* Erase whole flash chip */
#define NOR_OP_SE		0xd8	/* Sector erase (usually 64KiB) */
#define NOR_OP_RDID		0x9f	/* Read JEDEC ID */
#define NOR_OP_RDSFDP	0x5a	/* Read SFDP */
#define NOR_OP_RDCR		0x35	/* Read configuration register */
#define NOR_OP_SRSTEN	0x66	/* Software Reset Enable */
#define NOR_OP_SRST		0x99	/* Software Reset */
#define NOR_OP_INFINEON_SRST	0xf0	/* Infineon Software Reset */
#define NOR_OP_GBULK		0x98    /* Global Block Unlock */

/* 4-byte address opcodes - used on Spansion and some Macronix flashes. */
#define NOR_OP_READ_4B	        0x13	/* Read data bytes (low frequency) */
#define NOR_OP_READ_FAST_4B	    0x0c	/* Read data bytes (high frequency) */
#define NOR_OP_READ_1_1_2_4B	0x3c	/* Read data bytes (Dual Output ) */
#define NOR_OP_READ_1_2_2_4B	0xbc	/* Read data bytes (Dual I/O ) */
#define NOR_OP_READ_1_1_4_4B	0x6c	/* Read data bytes (Quad Output ) */
#define NOR_OP_READ_1_4_4_4B	0xec	/* Read data bytes (Quad I/O ) */
#define NOR_OP_READ_1_1_8_4B	0x7c	/* Read data bytes (Octal Output ) */
#define NOR_OP_READ_1_8_8_4B	0xcc	/* Read data bytes (Octal I/O ) */
#define NOR_OP_PP_4B		    0x12	/* Page program (up to 256 bytes) */
#define NOR_OP_PP_1_1_4_4B	    0x34	/* Quad page program */
#define NOR_OP_PP_1_4_4_4B	    0x3e	/* Quad page program */
#define NOR_OP_PP_1_1_8_4B	    0x84	/* Octal page program */
#define NOR_OP_PP_1_8_8_4B	    0x8e	/* Octal page program */
#define NOR_OP_BE_4K_4B	        0x21	/* Erase 4KiB block */
#define NOR_OP_BE_32K_4B	    0x5c	/* Erase 32KiB block */
#define NOR_OP_SE_4B		    0xdc	/* Sector erase (usually 64KiB) */
#define NOR_OP_CE	            0x60	/* Chip Erase */

/* Double Transfer Rate opcodes - defined in JEDEC JESD216B. */
#define NOR_OP_READ_1_1_1_DTR	0x0d
#define NOR_OP_READ_1_2_2_DTR	0xbd
#define NOR_OP_READ_1_4_4_DTR	0xed

#define NOR_OP_READ_1_1_1_DTR_4B	0x0e
#define NOR_OP_READ_1_2_2_DTR_4B	0xbe
#define NOR_OP_READ_1_4_4_DTR_4B	0xee

/* Used for SST flashes only. */
#define NOR_OP_BP		0x02	/* Byte program */
#define NOR_OP_AAI_WP	0xad	/* Auto address increment word program */

/* Used for Macronix and Winbond flashes. */
#define NOR_OP_EN4B		0xb7	/* Enter 4-byte mode */
#define NOR_OP_EX4B		0xe9	/* Exit 4-byte mode */

/* Used for Spansion flashes only. */
#define NOR_OP_BRWR		0x17	/* Bank register write */

/* Used for Micron flashes only. */
#define NOR_OP_RD_EVCR      0x65    /* Read EVCR register */
#define NOR_OP_WD_EVCR      0x61    /* Write EVCR register */

/* Used for GigaDevices and Winbond flashes. */
#define NOR_OP_ESECR		0x44	/* Erase Security registers */
#define NOR_OP_PSECR		0x42	/* Program Security registers */
#define NOR_OP_RSECR		0x48	/* Read Security registers */

/* Status Register bits. */
#define SR_WIP			BIT(0)	/* Write in progress */
#define SR_WEL			BIT(1)	/* Write enable latch */
/* meaning of other SR_* bits may differ between vendors */
#define SR_BP0			BIT(2)	/* Block protect 0 */
#define SR_BP1			BIT(3)	/* Block protect 1 */
#define SR_BP2			BIT(4)	/* Block protect 2 */
#define SR_BP3			BIT(5)	/* Block protect 3 */
#define SR_TB_BIT5		BIT(5)	/* Top/Bottom protect */
#define SR_BP3_BIT6		BIT(6)	/* Block protect 3 */
#define SR_TB_BIT6		BIT(6)	/* Top/Bottom protect */
#define SR_SRWD			BIT(7)	/* SR write protect */
/* Spansion/Cypress specific status bits */
#define SR_E_ERR		BIT(5)
#define SR_P_ERR		BIT(6)

#define SR1_QUAD_EN_BIT6	BIT(6)

#define SR_BP_SHIFT		2

/* Enhanced Volatile Configuration Register bits */
#define EVCR_QUAD_EN_MICRON	BIT(7)	/* Micron Quad I/O */

/* Status Register 2 bits. */
#define SR2_QUAD_EN_BIT1	BIT(1)
#define SR2_LB1			BIT(3)	/* Security Register Lock Bit 1 */
#define SR2_LB2			BIT(4)	/* Security Register Lock Bit 2 */
#define SR2_LB3			BIT(5)	/* Security Register Lock Bit 3 */
#define SR2_QUAD_EN_BIT7	BIT(7)

#ifdef __cplusplus
}
#endif

#endif