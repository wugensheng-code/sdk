/*****************************************************************************/
/**
*
* @file xdmaps_hw.h
*
* This header file contains the hardware interface of an XDmaPs device.
******************************************************************************/

#ifndef XDMAPS_HW_H		/* prevent circular inclusions */
#define XDMAPS_HW_H		/* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>



#define INLINE __inline

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef uintptr_t UINTPTR;
typedef int32_t s32;
typedef char char8;

#ifndef NULL
#define NULL		0U
#endif


/** @name Register Map
 *
 * Register offsets for the DMAC.
 * @{
 */

#define XDMAPS_DS_OFFSET	0x000 /* DMA Status Register */
#define XDMAPS_DPC_OFFSET	0x004 /* DMA Program Counter Rregister */
#define XDMAPS_INTEN_OFFSET	0X020 /* DMA Interrupt Enable Register */
#define XDMAPS_ES_OFFSET		0x024 /* DMA Event Status Register */
#define XDMAPS_INTSTATUS_OFFSET	0x028 /* DMA Interrupt Status Register
					       */
#define XDMAPS_INTCLR_OFFSET	0x02c /* DMA Interrupt Clear Register */
#define XDMAPS_FSM_OFFSET 	0x030 /* DMA Fault Status DMA Manager
				       * Register
				       */
#define XDMAPS_FSC_OFFSET	0x034 /* DMA Fault Status DMA Chanel Register
				       */
#define XDMAPS_FTM_OFFSET	0x038 /* DMA Fault Type DMA Manager Register */

#define XDMAPS_FTC0_OFFSET	0x040 /* DMA Fault Type for DMA Channel 0 */
/*
 * The offset for the rest of the FTC registers is calculated as
 * FTC0 + dev_chan_num * 4
 */
#define XDmaPs_FTCn_OFFSET(ch)	(XDMAPS_FTC0_OFFSET + (ch) * 4)

#define XDMAPS_CS0_OFFSET	0x100 /* Channel Status for DMA Channel 0 */
/*
 * The offset for the rest of the CS registers is calculated as
 * CS0 + * dev_chan_num * 0x08
 */
#define XDmaPs_CSn_OFFSET(ch)	(XDMAPS_CS0_OFFSET + (ch) * 8)

#define XDMAPS_CPC0_OFFSET	0x104 /* Channel Program Counter for DMA
				       * Channel 0
				       */
/*
 * The offset for the rest of the CPC registers is calculated as
 * CPC0 + dev_chan_num * 0x08
 */
#define XDmaPs_CPCn_OFFSET(ch)	(XDMAPS_CPC0_OFFSET + (ch) * 8)

#define XDMAPS_SA_0_OFFSET	0x400 /* Source Address Register for DMA
				       * Channel 0
				       */
/* The offset for the rest of the SA registers is calculated as
 * SA_0 + dev_chan_num * 0x20
 */
#define XDmaPs_SA_n_OFFSET(ch)	(XDMAPS_SA_0_OFFSET + (ch) * 0x20)

#define XDMAPS_DA_0_OFFSET	0x404 /* Destination Address Register for
				       * DMA Channel 0
				       */
/* The offset for the rest of the DA registers is calculated as
 * DA_0 + dev_chan_num * 0x20
 */
#define XDmaPs_DA_n_OFFSET(ch)	(XDMAPS_DA_0_OFFSET + (ch) * 0x20)

#define XDMAPS_CC_0_OFFSET	0x408 /* Channel Control Register for
				       * DMA Channel 0
				       */
/*
 * The offset for the rest of the CC registers is calculated as
 * CC_0 + dev_chan_num * 0x20
 */
#define XDmaPs_CC_n_OFFSET(ch)	(XDMAPS_CC_0_OFFSET + (ch) * 0x20)

#define XDMAPS_LC0_0_OFFSET	0x40C /* Loop Counter 0 for DMA Channel 0 */
/*
 * The offset for the rest of the LC0 registers is calculated as
 * LC_0 + dev_chan_num * 0x20
 */
#define XDmaPs_LC0_n_OFFSET(ch)	(XDMAPS_LC0_0_OFFSET + (ch) * 0x20)
#define XDMAPS_LC1_0_OFFSET	0x410 /* Loop Counter 1 for DMA Channel 0 */
/*
 * The offset for the rest of the LC1 registers is calculated as
 * LC_0 + dev_chan_num * 0x20
 */
#define XDmaPs_LC1_n_OFFSET(ch)	(XDMAPS_LC1_0_OFFSET + (ch) * 0x20)

#define XDMAPS_DBGSTATUS_OFFSET	0xD00 /* Debug Status Register */
#define XDMAPS_DBGCMD_OFFSET	0xD04 /* Debug Command Register */
#define XDMAPS_DBGINST0_OFFSET	0xD08 /* Debug Instruction 0 Register */
#define XDMAPS_DBGINST1_OFFSET	0xD0C /* Debug Instruction 1 Register */

#define XDMAPS_CR0_OFFSET	0xE00 /* Configuration Register 0 */
#define XDMAPS_CR1_OFFSET	0xE04 /* Configuration Register 1 */
#define XDMAPS_CR2_OFFSET	0xE08 /* Configuration Register 2 */
#define XDMAPS_CR3_OFFSET	0xE0C /* Configuration Register 3 */
#define XDMAPS_CR4_OFFSET	0xE10 /* Configuration Register 4 */
#define XDMAPS_CRDN_OFFSET	0xE14 /* Configuration Register Dn */

#define XDMAPS_PERIPH_ID_0_OFFSET	0xFE0 /* Peripheral Identification
					       * Register 0
					       */
#define XDMAPS_PERIPH_ID_1_OFFSET	0xFE4 /* Peripheral Identification
					       * Register 1
					       */
#define XDMAPS_PERIPH_ID_2_OFFSET	0xFE8 /* Peripheral Identification
					       * Register 2
					       */
#define XDMAPS_PERIPH_ID_3_OFFSET	0xFEC /* Peripheral Identification
					       * Register 3
					       */
#define XDMAPS_PCELL_ID_0_OFFSET	0xFF0 /* PrimeCell Identification
				       * Register 0
				       */
#define XDMAPS_PCELL_ID_1_OFFSET	0xFF4 /* PrimeCell Identification
				       * Register 1
				       */
#define XDMAPS_PCELL_ID_2_OFFSET	0xFF8 /* PrimeCell Identification
				       * Register 2
				       */
#define XDMAPS_PCELL_ID_3_OFFSET	0xFFC /* PrimeCell Identification
				       * Register 3
				       */

/*
 * Some useful register masks
 */
#define XDMAPS_DS_DMA_STATUS		0x0F /* DMA status mask */
#define XDMAPS_DS_DMA_STATUS_STOPPED	0x00 /* debug status busy mask */

#define XDMAPS_DBGSTATUS_BUSY		0x01 /* debug status busy mask */

#define XDMAPS_CS_ACTIVE_MASK		0x07 /* channel status active mask,
					      * llast 3 bits of CS register
					      */

#define XDMAPS_CR1_I_CACHE_LEN_MASK	0x07 /* i_cache_len mask */


/*
 * XDMAPS_DBGINST0 - constructs the word for the Debug Instruction-0 Register.
 * @b1: Instruction byte 1
 * @b0: Instruction byte 0
 * @ch: Channel number
 * @dbg_th: Debug thread encoding: 0 = DMA manager thread, 1 = DMA channel
 */
#define XDmaPs_DBGINST0(b1, b0, ch, dbg_th) \
	(((b1) << 24) | ((b0) << 16) | (((ch) & 0x7) << 8) | ((dbg_th & 0x1)))

#define XDMAPS_CHANNELS_PER_DEV		8

#define XDMAPS_INTCLR_ALL_MASK		0xFF

/*****************************************************************************/
/**
*
* @brief    Performs an input operation for a memory location by
*           reading from the specified address and returning the 32 bit Value
*           read  from that address.
*
* @param	Addr: contains the address to perform the input operation
*
* @return	The 32 bit Value read from the specified input address.
*
******************************************************************************/
static INLINE u32 Xil_In32(UINTPTR Addr)
{
	return *(volatile u32 *) Addr;
}

/*****************************************************************************/
/**
*
* @brief    Performs an output operation for a memory location by writing the
*           32 bit Value to the the specified address.
*
* @param	Addr contains the address to perform the output operation
* @param	Value contains the 32 bit Value to be written at the specified
*           address.
*
* @return	None.
*
******************************************************************************/
static INLINE void Xil_Out32(UINTPTR Addr, u32 Value)
{
	/* write 32 bit value to specified address */
	volatile u32 *LocalAddr = (volatile u32 *)Addr;
	*LocalAddr = Value;
}

#define XDmaPs_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))

#define XDmaPs_WriteReg(BaseAddress, RegOffset, RegisterValue) \
    Xil_Out32((BaseAddress) + (RegOffset), (RegisterValue))

/*
 * Perform reset operation to the dmaps interface
 */
void XDmaPs_ResetHw(u32 BaseAddress);
#ifdef __cplusplus
}
#endif

#endif
