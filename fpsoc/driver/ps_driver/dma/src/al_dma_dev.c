/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include <stdint.h>
#include <string.h>
#include "al_dma_dev.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#ifdef ENABLE_MMU
#define IS_CACHELINE_ALIGNED(Addr) IS_ALIGN(Addr, CACHE_LINE_SIZE)
#else
#define IS_CACHELINE_ALIGNED(Addr) IS_ALIGN(Addr, 4)
#endif

/************************** Variable Definitions *****************************/
extern AL_DMA_HwConfigStruct AlDma_HwConfig[AL_DMA_NUM_INSTANCE];

/** @addtogroup Dev
 *  @{
 */

/** @defgroup DMA_How_To_Use How To Use
 *  @{

 The Dev driver can be Used as follows:

 - Invoke AlDma_Dev_Init to initialize Dev.
 - Invoke AlDma_Dev_RequestChannel to request a available dma Channel.
 - Invoke AlDma_Dev_Config to Config dma transfer Config.
 - Invoke AlDma_Dev_PrepDmaSingle/Cyclic for single/Cyclic transfer.
 - Invoke AlDma_Dev_Start to start a ready dma transfer.
 - Invoke AlDma_Dev_Stop to stop the dma Channel.
 - Invoke AlDma_Dev_ReleaseChannel to release the dma Channel.
 - Invoke AlDma_Dev_DeInit to deinitialize Dev.
 - More details refer to APIs' descriptions as below.

 @} */

/** @defgroup DMA_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#ifdef DMA_DEBUG_MCGEN
static AL_U32 DebugCmdLine;
#define DMA_DBGCMD_DUMP(Off, Format, ...)   do {                             \
                                                AL_LOG(AL_LOG_LEVEL_DEBUG, "%x:"Format, DebugCmdLine, ##__VA_ARGS__); \
                                                DebugCmdLine += Off;            \
                                            } while (0)
#define DMA_DBGMC_START(Addr)               (DebugCmdLine = Addr)
#else
#define DMA_DBGCMD_DUMP(Off, Format, ...)   do {                             \
                                            } while (0)
#define DMA_DBGMC_START(Addr)               do {                        \
                                            } while (0)
#endif

#ifndef DMA_MAX_WAIT
#define DMA_MAX_WAIT 4000
#endif

#define DMAC_MODE_NS BIT(0)

#define DS_ST_STOP      0x0
#define DS_ST_EXEC      0x1
#define DS_ST_CMISS     0x2
#define DS_ST_UPDTPC    0x3
#define DS_ST_WFE       0x4
#define DS_ST_ATBRR     0x5
#define DS_ST_QBUSY     0x6
#define DS_ST_WFP       0x7
#define DS_ST_KILL      0x8
#define DS_ST_CMPLT     0x9
#define DS_ST_FLTCMP    0xe
#define DS_ST_FAULT     0xf

#define CC_SRCINC           BIT(0)
#define CC_DSTINC           BIT(14)
#define CC_SRCPRI           BIT(8)
#define CC_DSTPRI           BIT(22)
#define CC_SRCNS            BIT(9)
#define CC_DSTNS            BIT(23)
#define CC_SRCIA            BIT(10)
#define CC_DSTIA            BIT(24)
#define CC_SRCBRSTLEN_SHFT  4
#define CC_DSTBRSTLEN_SHFT  18
#define CC_SRCBRSTSIZE_SHFT 1
#define CC_DSTBRSTSIZE_SHFT 15
#define CC_SRCCCTRL_SHFT    11
#define CC_SRCCCTRL_MASK    0x7
#define CC_DSTCCTRL_SHFT    25
#define CC_DRCCCTRL_MASK    0x7
#define CC_SWAP_SHFT        28

#define CR0_PERIPH_REQ_SET      BIT(0)
#define CR0_BOOT_EN_SET         BIT(1)
#define CR0_BOOT_MAN_NS         BIT(2)
#define CR0_NUM_CHANS_SHIFT     4
#define CR0_NUM_CHANS_MASK      0x7
#define CR0_NUM_PERIPH_SHIFT    12
#define CR0_NUM_PERIPH_MASK     0x1f
#define CR0_NUM_EVENTS_SHIFT    17
#define CR0_NUM_EVENTS_MASK     0x1f

#define CR1_ICACHE_LEN_SHIFT        0
#define CR1_ICACHE_LEN_MASK         0x7
#define CR1_NUM_ICACHELINES_SHIFT   4
#define CR1_NUM_ICACHELINES_MASK    0xf

#define CRD_DATA_WIDTH_SHIFT    0
#define CRD_DATA_WIDTH_MASK     0x7
#define CRD_WR_CAP_SHIFT        4
#define CRD_WR_CAP_MASK         0x7
#define CRD_WR_Q_DEP_SHIFT      8
#define CRD_WR_Q_DEP_MASK       0xf
#define CRD_RD_CAP_SHIFT        12
#define CRD_RD_CAP_MASK         0x7
#define CRD_RD_Q_DEP_SHIFT      16
#define CRD_RD_Q_DEP_MASK       0xf
#define CRD_DATA_BUFF_SHIFT     20
#define CRD_DATA_BUFF_MASK      0x3ff

#define PART            0x330
#define DESIGNER        0x41
#define REVISION        0x0
#define INTEG_CFG       0x0
#define PERIPH_ID_VAL   ((PART << 0) | (DESIGNER << 12))

#define DMA_DS_DMA_STATUS           0x0f
#define DMA_DS_DMA_STATUS_STOPPED   0x00
#define DMA_DBGSTATUS_BUSY          0x01
#define DMA_CS_ACTIVE_MASK          0x07
#define DMA_CR1_I_CACHE_LEN_MASK    0x07
#define DMA_INTCLR_ALL_MASK         0xff

#define CMD_DMAADDH     0x54
#define CMD_DMAEND      0x00
#define CMD_DMAFLUSHP   0x35
#define CMD_DMAGO       0xa0
#define CMD_DMALD       0x04
#define CMD_DMALDP      0x25
#define CMD_DMALP       0x20
#define CMD_DMALPEND    0x28
#define CMD_DMAKILL     0x01
#define CMD_DMAMOV      0xbc
#define CMD_DMANOP      0x18
#define CMD_DMARMB      0x12
#define CMD_DMASEV      0x34
#define CMD_DMAST       0x08
#define CMD_DMASTP      0x29
#define CMD_DMASTZ      0x0c
#define CMD_DMAWFE      0x36
#define CMD_DMAWFP      0x30
#define CMD_DMAWMB      0x13

#define SZ_DMAADDH      3
#define SZ_DMAEND       1
#define SZ_DMAFLUSHP    2
#define SZ_DMALD        1
#define SZ_DMALDP       2
#define SZ_DMALP        2
#define SZ_DMALPEND     2
#define SZ_DMAKILL      1
#define SZ_DMAMOV       6
#define SZ_DMANOP       1
#define SZ_DMARMB       1
#define SZ_DMASEV       2
#define SZ_DMAST        1
#define SZ_DMASTP       2
#define SZ_DMASTZ       1
#define SZ_DMAWFE       2
#define SZ_DMAWFP       2
#define SZ_DMAWMB       1
#define SZ_DMAGO        6

#define BRST_LEN(Ccr)       ((((Ccr) >> CC_SRCBRSTLEN_SHFT) & 0xf) + 1)
#define BRST_SIZE(Ccr)      (1 << (((Ccr) >> CC_SRCBRSTSIZE_SHFT) & 0x7))

#define BYTE_TO_BURST(b, Ccr)       ((b) / BRST_SIZE(Ccr) / BRST_LEN(Ccr))
#define BURST_TO_BYTE(c, Ccr)       ((c) * BRST_SIZE(Ccr) * BRST_LEN(Ccr))
#define BYTE_MOD_BURST_LEN(b, Ccr)  (((b) / BRST_SIZE(Ccr)) % BRST_LEN(Ccr))

/*
 * DMA_DBGINST0 - constructs the word for the Debug Instruction-0 Register.
 * @B1: Instruction byte 1
 * @B0: Instruction byte 0
 * @Ch: Channel number
 * @DbgTh: Debug Thread encoding: 0 = Dev manager Thread, 1 = Dev Channel
 */
#define DMA_DBGINST0(B1, B0, Ch, DbgTh) \
    (((B1) << 24) | ((B0) << 16) | (((Ch) & 0x7) << 8) | ((DbgTh & 0x1)))

/********************* Private Structure Definition **************************/
typedef enum {
    SAR = 0,
    CCR,
    DAR,
} DMA_MovEnum;

typedef enum {
    SRC = 0,
    DST,
} DMA_AddrEnum;

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/
/**
 * Checks if the DMA direction is from memory to device or device to memory.
 *
 * @param Direction The direction of the DMA transfer.
 * @return AL_BOOL True if the direction is either from memory to device or device to memory, otherwise false.
 */
__STATIC_INLINE AL_BOOL AlDma_Dev_IsSlaveDirection(AL_DMA_TransEnum Direction)
{
    return (Direction == DMA_MEM_TO_DEV) || (Direction == DMA_DEV_TO_MEM);
}

/**
 * Encodes the DMAADDH instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @param Addr The address register to be added (source or destination).
 * @param Val The value to be added to the address register.
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMAADDH(AL_U8 DryRun, AL_CHAR *Buf, DMA_AddrEnum Addr, AL_U16 Val)
{
    if (DryRun) {
        return SZ_DMAADDH;
    }

    *Buf = CMD_DMAADDH;
    *Buf |= (Addr << 1);
    *((AL_U16 *)(Buf + 1)) = Val;

    DMA_DBGCMD_DUMP(SZ_DMAADDH, "\tDMAADDH %s %u\r\n",
                      Addr == DST ? "DA" : "SA", Val);

    return SZ_DMAADDH;
}

/**
 * Encodes the DMAEND instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMAEND(AL_U8 DryRun, AL_CHAR *Buf)
{
    if (DryRun) {
        return SZ_DMAEND;
    }
    /*
     * DMAEND encoding:
     * 7 6 5 4 3 2 1 0
     * 0 0 0 0 0 0 0 0
     */
    *Buf = CMD_DMAEND;

    DMA_DBGCMD_DUMP(SZ_DMAEND, "\tDMAEND\r\n");

    return SZ_DMAEND;
}

/**
 * Copies 4 bytes from the source to the destination.
 *
 * @param Dst The destination buffer.
 * @param Src The source buffer.
 * @return None.
 */
__STATIC_INLINE void AlDma_Dev_Cpy4(AL_CHAR *Dst, AL_CHAR *Src)
{
    *Dst = *Src;
    *(Dst + 1) = *(Src + 1);
    *(Dst + 2) = *(Src + 2);
    *(Dst + 3) = *(Src + 3);
}

/**
 * Encodes the DMAGO instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @param Chan The channel number.
 * @param Imm The immediate value.
 * @param NonSecure Indicates if the operation is non-secure.
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMAGO(AL_U8 DryRun, AL_CHAR *Buf, AL_U8 Chan, AL_U32 Imm, AL_U8 NonSecure)
{
    if (DryRun) {
        return SZ_DMAGO;
    }
    /*
     * DMAGO encoding:
     * 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
     *  0  0  0  0  0 |cn[2:0]| 1  0  1  0  0  0 ns  0
     *
     * 47 ... 16
     *  imm[32:0]
     */
    *Buf = CMD_DMAGO | ((NonSecure << 1) & 0x02);

    *(Buf + 1) = (AL_U8)(Chan & 0x07);

    AlDma_Dev_Cpy4(Buf + 2, (AL_CHAR *)&Imm);

    return SZ_DMAGO;
}

/**
 * Encodes the DMALP instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @param LoopCntr The loop counter (0 or 1).
 * @param Loops The number of loops.
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMALP(AL_U8 DryRun, AL_CHAR *Buf, AL_U8 LoopCntr, AL_U16 Loops)
{
    if (DryRun) {
        return SZ_DMALP;
    }
    /*
     * DMALP encoding
     * 15   ...   8 7 6 5 4 3 2 1  0
     * | iter[7:0] |0 0 1 0 0 0 lc 0
     */
    *Buf = (AL_U8)(CMD_DMALP | ((LoopCntr & 1) << 1));
    *(Buf + 1) = (AL_U8)(Loops - 1);

    DMA_DBGCMD_DUMP(SZ_DMALP, "\tDMALP_%c %u\r\n", LoopCntr ? '1' : '0', Loops - 1);

    return SZ_DMALP;
}

/**
 * Encodes the DMAMOV instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @param RegDest The register destination (SAR, DAR, or CCR).
 * @param Imm The immediate value to be moved into the register.
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMAMOV(AL_U8 DryRun, AL_CHAR *Buf, DMA_MovEnum RegDest, AL_U32 Imm)
{
    if (DryRun) {
        return SZ_DMAMOV;
    }
    /*
     * DMAMOV encoding
     * 15 4 3 2 1 10 ... 8 7 6 5 4 3 2 1 0
     *  0 0 0 0 0 |rd[2:0]|1 0 1 1 1 1 0 0
     *
     * 47 ... 16
     *  imm[32:0]
     *
     * rd: b000 for SAR, b001 CCR, b010 DAR
     */
    *Buf = CMD_DMAMOV;
    *(Buf + 1) = RegDest & 0x7;
    AlDma_Dev_Cpy4(Buf + 2, (AL_CHAR *)&Imm);

    DMA_DBGCMD_DUMP(SZ_DMAMOV, "\tDMAMOV %s 0x%x\r\n",
                      RegDest == SAR ? "SAR" : (RegDest == DAR ? "DAR" : "CCR"), Imm);

    return SZ_DMAMOV;
}

/**
 * Encodes the DMANOP instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMANOP(AL_U8 DryRun, AL_CHAR *Buf)
{
    if (DryRun) {
        return SZ_DMANOP;
    }
    /*
     * DMANOP encoding
     * 7 6 5 4 3 2 1 0
     * 0 0 0 1 1 0 0 0
     */
    *Buf = CMD_DMANOP;

    DMA_DBGCMD_DUMP(SZ_DMANOP, "\tDMANOP\r\n");

    return SZ_DMANOP;
}

/**
 * Encodes the DMARMB instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMARMB(AL_U8 DryRun, AL_CHAR *Buf)
{
    if (DryRun) {
        return SZ_DMARMB;
    }
    /*
     * DMARMB encoding
     * 7 6 5 4 3 2 1 0
     * 0 0 0 1 0 0 1 0
     */
    *Buf = CMD_DMARMB;

    DMA_DBGCMD_DUMP(SZ_DMARMB, "\tDMARMB\r\n");

    return SZ_DMARMB;
}

/**
 * Encodes the DMASEV instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @param Event The event number.
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMASEV(AL_U8 DryRun, AL_CHAR *Buf, AL_U8 Event)
{
    if (DryRun) {
        return SZ_DMASEV;
    }
    /*
     * DMASEV encoding
     * 15 4 3 2 1  10 9 8 7 6 5 4 3 2 1 0
     * |event[4:0]| 0 0 0 0 0 1 1 0 1 0 0
     */
    *Buf = CMD_DMASEV;
    *(Buf + 1) = (AL_U8)(Event << 3);

    DMA_DBGCMD_DUMP(SZ_DMASEV, "\tDMASEV %u\r\n", Event);

    return SZ_DMASEV;
}

/**
 * Encodes the DMAWMB instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMAWMB(AL_U8 DryRun, AL_CHAR *Buf)
{
    if (DryRun) {
        return SZ_DMAWMB;
    }
    /*
     * DMAWMB encoding
     * 7 6 5 4 3 2 1 0
     * 0 0 0 1 0 0 1 0
     */
    *Buf = CMD_DMAWMB;

    DMA_DBGCMD_DUMP(SZ_DMAWMB, "\tDMAWMB\r\n");

    return SZ_DMAWMB;
}

/**
 * Encodes the DMAFLUSHP instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @param Peri The peripheral number.
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMAFLUSHP(AL_U8 DryRun, AL_CHAR *Buf, AL_U8 Peri)
{
    if (DryRun) {
        return SZ_DMAFLUSHP;
    }
    /*
     * DMAFLUSHP encoding
     * 15 4 3 2 1  10 9 8 7 6 5 4 3 2 1 0
     * |perip[4:0]| 0 0 0 0 0 1 1 0 1 0 1
     */
    *Buf = CMD_DMAFLUSHP;

    Peri &= 0x1f;
    Peri <<= 3;
    *(Buf + 1) = Peri;

    DMA_DBGCMD_DUMP(SZ_DMAFLUSHP, "\tDMAFLUSHP %u\r\n", Peri >> 3);

    return SZ_DMAFLUSHP;
}

/**
 * Encodes the DMALD instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @param Cond The condition for the load (single or burst).
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMALD(AL_U8 DryRun, AL_CHAR *Buf, AL_DMA_CondEnum Cond)
{
    if (DryRun) {
        return SZ_DMALD;
    }
    /*
     * DMALD encoding
     * 7 6 5 4 3 2 1  0
     * 0 0 0 0 0 1 bs Xfer
     */
    *Buf = CMD_DMALD;

    if (Cond == SINGLE) {
        *Buf |= (0 << 1) | (1 << 0);
    } else if (Cond == BURST) {
        *Buf |= (1 << 1) | (1 << 0);
    }

    DMA_DBGCMD_DUMP(SZ_DMALD, "\tDMALD%c\r\n",
                      Cond == SINGLE ? 'S' : (Cond == BURST ? 'B' : 'A'));

    return SZ_DMALD;
}

/**
 * Encodes the DMALDP instruction into the buffer.
 *
 * @param DryRun If non-zero, the function returns the size of the instruction without encoding it.
 * @param Buf The buffer where the instruction is encoded.
 * @param Cond The condition for the load (single or burst).
 * @param Peri The peripheral number.
 * @return AL_S32 The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMALDP(AL_U8 DryRun, AL_CHAR *Buf, AL_DMA_CondEnum Cond, AL_U8 Peri)
{
    if (DryRun) {
        return SZ_DMALDP;
    }
    /*
     * DMALDP encoding
     * 15 4 3 2 1  10 9 8 7 6 5 4 3 2 1  0
     * |perip[4:0]| 0 0 0 0 0 1 0 0 1 bs 1
     */
    *Buf = CMD_DMALDP;

    if (Cond == BURST) {
        *Buf |= (1 << 1);
    }

    Peri &= 0x1f;
    Peri <<= 3;
    *(Buf + 1) = Peri;

    DMA_DBGCMD_DUMP(SZ_DMALDP, "\tDMALDP%c %u\r\n", Cond == SINGLE ? 'S' : 'B',
                      Peri >> 3);

    return SZ_DMALDP;
}

/**
 * Encodes a DMALPEND instruction into the buffer.
 *
 * @param DryRun If set to non-zero, the function does not perform encoding but returns the size of the instruction.
 * @param Buf Pointer to the buffer where the instruction is to be encoded.
 * @param Cond Condition under which the loop operation is performed.
 * @param Forever Indicates whether the loop is infinite.
 * @param Loop Indicates whether looping is enabled.
 * @param BackJump The number of bytes to jump back at the end of the loop.
 * @return The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMALPEND(AL_U8 DryRun, AL_CHAR *Buf, AL_DMA_CondEnum Cond, AL_BOOL Forever,
                                          AL_U32 Loop, AL_U8 BackJump)
{
    if (DryRun) {
        return SZ_DMALPEND;
    }
    /*
     * DMALPEND encoding
     * 15       ...        8 7 6 5 4  3 2  1  0
     * | backward_jump[7:0] |0 0 1 nf 1 lc bs Xfer
     */
    *Buf = CMD_DMALPEND;

    if (Loop) {
        *Buf |= (1 << 2);
    }

    if (!Forever) {
        *Buf |= (1 << 4);
    }

    if (Cond == SINGLE) {
        *Buf |= (0 << 1) | (1 << 0);
    } else if (Cond == BURST) {
        *Buf |= (1 << 1) | (1 << 0);
    }

    *(Buf + 1) = BackJump;

    DMA_DBGCMD_DUMP(SZ_DMALPEND, "\tDMALP%s%c_%c bjmpto_%Xfer\r\n",
                      Forever ? "FE" : "END",
                      Cond == SINGLE ? 'S' : (Cond == BURST ? 'B' : 'A'),
                      Loop ? '1' : '0', BackJump);

    return SZ_DMALPEND;
}

/**
 * Encodes a DMAKILL instruction into the buffer.
 *
 * @param DryRun If set to non-zero, the function does not perform encoding but returns the size of the instruction.
 * @param Buf Pointer to the buffer where the instruction is to be encoded.
 * @return The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMAKILL(AL_U8 DryRun, AL_CHAR *Buf)
{
    if (DryRun) {
        return SZ_DMAKILL;
    }
    /*
     * DMAKILL encoding
     * 7 6 5 4 3 2 1 0
     * 0 0 0 0 0 0 0 1
     */
    *Buf = CMD_DMAKILL;

    return SZ_DMAKILL;
}

/**
 * Encodes a DMAST instruction into the buffer.
 *
 * @param DryRun If set to non-zero, the function does not perform encoding but returns the size of the instruction.
 * @param Buf Pointer to the buffer where the instruction is to be encoded.
 * @param Cond Condition under which the store operation is performed.
 * @return The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMAST(AL_U8 DryRun, AL_CHAR *Buf, AL_DMA_CondEnum Cond)
{
    if (DryRun) {
        return SZ_DMAST;
    }
    /*
     * DMAST encoding
     * 7 6 5 4 3 2 1  0
     * 0 0 0 0 1 0 bs Xfer
     */
    *Buf = CMD_DMAST;

    if (Cond == SINGLE) {
        *Buf |= (0 << 1) | (1 << 0);
    } else if (Cond == BURST) {
        *Buf |= (1 << 1) | (1 << 0);
    }

    DMA_DBGCMD_DUMP(SZ_DMAST, "\tDMAST%c\r\n",
                      Cond == SINGLE ? 'S' : (Cond == BURST ? 'B' : 'A'));

    return SZ_DMAST;
}

/**
 * Encodes a DMASTP instruction into the buffer.
 *
 * @param DryRun If set to non-zero, the function does not perform encoding but returns the size of the instruction.
 * @param Buf Pointer to the buffer where the instruction is to be encoded.
 * @param Cond Condition under which the store operation is performed.
 * @param Peri Peripheral number associated with the store operation.
 * @return The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMASTP(AL_U8 DryRun, AL_CHAR *Buf, AL_DMA_CondEnum Cond, AL_U8 Peri)
{
    if (DryRun) {
        return SZ_DMASTP;
    }
    /*
     * DMASTP encoding
     * 15 4 3 2 1  10 9 8 7 6 5 4 3 2 1  0
     * |perip[4:0]| 0 0 0 0 0 1 0 1 0 bs 1
     */
    *Buf = CMD_DMASTP;

    if (Cond == BURST) {
        *Buf |= (1 << 1);
    }

    Peri &= 0x1f;
    Peri <<= 3;
    *(Buf + 1) = Peri;

    DMA_DBGCMD_DUMP(SZ_DMASTP, "\tDMASTP%c %u\r\n", Cond == SINGLE ? 'S' : 'B',
                      Peri >> 3);

    return SZ_DMASTP;
}

/**
 * Encodes a DMASTZ instruction into the buffer.
 *
 * @param DryRun If set to non-zero, the function does not perform encoding but returns the size of the instruction.
 * @param Buf Pointer to the buffer where the instruction is to be encoded.
 * @return The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMASTZ(AL_U8 DryRun, AL_CHAR *Buf)
{
    if (DryRun) {
        return SZ_DMASTZ;
    }
    /*
     * DMASTZ encoding
     * 7 6 5 4 3 2 1 0
     * 0 0 0 1 1 1 0 0
     */
    *Buf = CMD_DMASTZ;

    DMA_DBGCMD_DUMP(SZ_DMASTZ, "\tDMASTZ\r\n");

    return SZ_DMASTZ;
}

/**
 * Encodes a DMAWFE instruction into the buffer.
 *
 * @param DryRun If set to non-zero, the function does not perform encoding but returns the size of the instruction.
 * @param Buf Pointer to the buffer where the instruction is to be encoded.
 * @param Event Event number to wait for.
 * @param Invalidate Indicates whether to invalidate the event after waiting.
 * @return The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMAWFE(AL_U8 DryRun, AL_CHAR *Buf, AL_U8 Event, AL_U32 Invalidate)
{
    if (DryRun) {
        return SZ_DMAWFE;
    }
    /*
     * DMAWFE encoding
     * 15 4 3 2 1  10 9 8 7 6 5 4 3 2 1 0
     * |event[4:0]| 0 i 0 0 0 1 1 0 1 1 0
     */
    *Buf = CMD_DMAWFE;

    Event &= 0x1f;
    Event <<= 3;
    *(Buf + 1) = Event;

    if (Invalidate) {
        *(Buf + 1) |= (1 << 1);
    }

    DMA_DBGCMD_DUMP(SZ_DMAWFE, "\tDMAWFE %u%s\r\n", Event >> 3,
                      Invalidate ? ", I" : "");

    return SZ_DMAWFE;
}

/**
 * Encodes a DMAWFP instruction into the buffer.
 *
 * @param DryRun If set to non-zero, the function does not perform encoding but returns the size of the instruction.
 * @param Buf Pointer to the buffer where the instruction is to be encoded.
 * @param Cond Condition under which the wait for peripheral operation is performed.
 * @param Peri Peripheral number to wait for.
 * @return The size of the encoded instruction.
 */
__STATIC_INLINE AL_S32 DMA_Instr_DMAWFP(AL_U8 DryRun, AL_CHAR *Buf, AL_DMA_CondEnum Cond, AL_U8 Peri)
{
    if (DryRun) {
        return SZ_DMAWFP;
    }
    /*
     * DMAWFP encoding
     * 15 4 3 2 1  10 9 8 7 6 5 4 3 2 1  0
     * |perip[4:0]| 0 0 0 0 0 1 1 0 0 bs p
     */
    *Buf = CMD_DMAWFP;

    if (Cond == SINGLE) {
        *Buf |= (0 << 1) | (0 << 0);
    } else if (Cond == BURST) {
        *Buf |= (1 << 1) | (0 << 0);
    } else {
        *Buf |= (0 << 1) | (1 << 0);
    }

    Peri &= 0x1f;
    Peri <<= 3;
    *(Buf + 1) = Peri;

    DMA_DBGCMD_DUMP(SZ_DMAWFP, "\tDMAWFP%c %u\r\n",
                      Cond == SINGLE ? 'S' : (Cond == BURST ? 'B' : 'P'),
                      Peri >> 3);

    return SZ_DMAWFP;
}

/**
 * Loads and stores data from memory to memory in a loop.
 *
 * @param DryRun If set to non-zero, the function does not perform encoding but returns the total size of the instructions.
 * @param Buf Pointer to the buffer where the instructions are to be encoded.
 * @param Cycle Number of load/store cycles to perform.
 * @return The total size of the encoded instructions.
 */
static AL_S32 AlDma_Dev_LdStMemToMem(AL_U8 DryRun, AL_CHAR *Buf, AL_S32 Cycle)
{
    AL_S32 Off = 0;

    while (Cycle--) {
        Off += DMA_Instr_DMALD(DryRun, &Buf[Off], ALWAYS);
        Off += DMA_Instr_DMAST(DryRun, &Buf[Off], ALWAYS);
    }

    return Off;
}

/**
 * Loads from a device and stores to memory in a loop.
 *
 * @param DryRun If set to non-zero, the function does not perform encoding but returns the total size of the instructions.
 * @param Dev Pointer to the device structure.
 * @param Buf Pointer to the buffer where the instructions are to be encoded.
 * @param XferSpec Pointer to the transfer specification structure.
 * @param Cycle Number of load/store cycles to perform.
 * @return The total size of the encoded instructions.
 */
static AL_S32 AlDma_Dev_LdStDevToMem(AL_U8 DryRun, AL_DMA_DevStruct *Dev, AL_CHAR *Buf,
                                     AL_DMA_XferSpecStruct *XferSpec, AL_S32 Cycle)
{
    AL_S32 Off = 0;
    AL_DMA_CondEnum Cond = Dev->PeripReqType;

    while (Cycle--) {
        Off += DMA_Instr_DMAWFP(DryRun, &Buf[Off], Cond, XferSpec->Desc->Peri);
        Off += DMA_Instr_DMALDP(DryRun, &Buf[Off], Cond, XferSpec->Desc->Peri);
        Off += DMA_Instr_DMAST(DryRun, &Buf[Off], ALWAYS);
        Off += DMA_Instr_DMAFLUSHP(DryRun, &Buf[Off], XferSpec->Desc->Peri);
        if (XferSpec->Desc->DstInterlaceSize) {
            Off += DMA_Instr_DMAADDH(DryRun, &Buf[Off], DST,
                                       XferSpec->Desc->DstInterlaceSize);
        }
    }

    return Off;
}
/**
 * Loads from memory and stores to a device in a loop.
 *
 * @param DryRun If set to non-zero, the function does not perform encoding but returns the total size of the instructions.
 * @param Dev Pointer to the device structure.
 * @param Buf Pointer to the buffer where the instructions are to be encoded.
 * @param XferSpec Pointer to the transfer specification structure.
 * @param Cycle Number of load/store cycles to perform.
 * @return The total size of the encoded instructions.
 */
static AL_S32 AlDma_Dev_LdStMemToDev(AL_U8 DryRun, AL_DMA_DevStruct *Dev, AL_CHAR *Buf,
                                     AL_DMA_XferSpecStruct *XferSpec, AL_S32 Cycle)
{
    AL_S32 Off = 0;
    AL_DMA_CondEnum Cond = Dev->PeripReqType;

    while (Cycle--) {
        Off += DMA_Instr_DMAWFP(DryRun, &Buf[Off], Cond, XferSpec->Desc->Peri);
        Off += DMA_Instr_DMALD(DryRun, &Buf[Off], ALWAYS);
        Off += DMA_Instr_DMASTP(DryRun, &Buf[Off], Cond, XferSpec->Desc->Peri);
        Off += DMA_Instr_DMAFLUSHP(DryRun, &Buf[Off], XferSpec->Desc->Peri);
        if (XferSpec->Desc->SrcInterlaceSize) {
            Off += DMA_Instr_DMAADDH(DryRun, &Buf[Off], SRC,
                                       XferSpec->Desc->SrcInterlaceSize);
        }
    }

    return Off;
}

/**
 * Performs a burst transfer operation based on the direction specified in the transfer specification.
 *
 * @param DryRun If set to non-zero, the function does not perform encoding but returns the total size of the instructions.
 * @param Dev Pointer to the device structure.
 * @param Buf Pointer to the buffer where the instructions are to be encoded.
 * @param XferSpec Pointer to the transfer specification structure.
 * @param Cycle Number of burst cycles to perform.
 * @return The total size of the encoded instructions or an error code to scare off the client.
 */
static AL_S32 AlDma_Dev_Burst(AL_U8 DryRun, AL_DMA_DevStruct *Dev, AL_CHAR *Buf,
                              AL_DMA_XferSpecStruct *XferSpec, AL_S32 Cycle)
{
    AL_S32 Off = 0;

    switch (XferSpec->Desc->Dir) {
    case DMA_MEM_TO_DEV:
        Off += AlDma_Dev_LdStMemToDev(DryRun, Dev, &Buf[Off], XferSpec, Cycle);
        break;
    case DMA_DEV_TO_MEM:
        Off += AlDma_Dev_LdStDevToMem(DryRun, Dev, &Buf[Off], XferSpec, Cycle);
        break;
    case DMA_MEM_TO_MEM:
        Off += AlDma_Dev_LdStMemToMem(DryRun, &Buf[Off], Cycle);
        break;
    default:
        Off += 0x40000000; /* Scare Off the Client */
        break;
    }

    return Off;
}

/**
 * This function calculates the necessary loop counts for the DMA transfer based on the burst size.
 * It supports splitting the transfer into multiple loops if the burst size exceeds the maximum loop count limits.
 * The function handles both single and double loop scenarios and adjusts the burst size accordingly.
 *
 * @param DryRun If set to 1, the function performs a dry run without actual data transfer.
 * @param Dev Pointer to the DMA device structure.
 * @param Buf Pointer to the buffer where DMA instructions are stored.
 * @param BurstSize Pointer to the size of the burst. This value is updated based on the loop calculations.
 * @param XferSpec Pointer to the DMA transfer specification structure.
 * @return The offset in the buffer after adding the DMA instructions.
 *
 *
 */
/* Returns bytes consumed and updates bursts */
static AL_S32 AlDma_Dev_Loop(AL_U8 DryRun, AL_DMA_DevStruct *Dev, AL_CHAR *Buf, AL_UINTPTR *BurstSize,
                             AL_DMA_XferSpecStruct *XferSpec)
{
    AL_S32 Cycle, CycleMax, SizeLp, SizeLpend, SizeBurst, Off;
    AL_U32 LoopCnt0, LoopCnt1, LoopJump0, LoopJump1;

    if (*BurstSize == 1) {
        return AlDma_Dev_Burst(DryRun, Dev, Buf, XferSpec, 1);
    }

    /* Max iterations possible in DMALP is 256 */
    if (*BurstSize >= 256 * 256) {
        LoopCnt1 = 256;
        LoopCnt0 = 256;
        Cycle = *BurstSize / LoopCnt1 / LoopCnt0;
    } else if (*BurstSize > 256) {
        LoopCnt1 = 256;
        LoopCnt0 = *BurstSize / LoopCnt1;
        Cycle = 1;
    } else {
        LoopCnt1 = *BurstSize;
        LoopCnt0 = 0;
        Cycle = 1;
    }

    SizeLp = DMA_Instr_DMALP(1, Buf, 0, 0);
    SizeBurst = AlDma_Dev_Burst(1, Dev, Buf, XferSpec, 1);
    SizeLpend = DMA_Instr_DMALPEND(1, Buf, ALWAYS, AL_FALSE, 0, 0);

    if (LoopCnt0) {
        SizeLp *= 2;
        SizeLpend *= 2;
    }

    /*
     * Max bursts that we can unroll due to limit on the
     * size of backward jump that can be encoded in DMALPEND
     * which is 8-bits and hence 255
     */
    CycleMax = (255 - (SizeLp + SizeLpend)) / SizeBurst;
    Cycle = (CycleMax < Cycle) ? CycleMax : Cycle;
    Off = 0;

    if (LoopCnt0) {
        Off += DMA_Instr_DMALP(DryRun, &Buf[Off], 0, LoopCnt0);
        LoopJump0 = Off;
    }

    Off += DMA_Instr_DMALP(DryRun, &Buf[Off], 1, LoopCnt1);
    LoopJump1 = Off;

    Off += AlDma_Dev_Burst(DryRun, Dev, &Buf[Off], XferSpec, Cycle);

    Off +=
        DMA_Instr_DMALPEND(DryRun, &Buf[Off], ALWAYS, AL_FALSE, 1, Off - LoopJump1);

    if (LoopCnt0) {
        Off += DMA_Instr_DMALPEND(DryRun, &Buf[Off], ALWAYS, AL_FALSE, 0,
                                    Off - LoopJump0);
    }
    *BurstSize = LoopCnt1 * Cycle;
    if (LoopCnt0) {
        *BurstSize *= LoopCnt0;
    }

    return Off;
}

/**
 * This function is responsible for generating the DMA instructions for a periodic transfer.
 * It calculates the loop count based on the burst size and handles any remainder by adding additional instructions.
 * The function also adjusts the control register for burst length if necessary.
 *
 * @param DryRun If set to 1, performs a dry run without actual data transfer.
 * @param Dev Pointer to the DMA device structure.
 * @param Buf Pointer to the buffer where DMA instructions are stored.
 * @param BurstSize The burst size for the transfer.
 * @param XferSpec Pointer to the DMA transfer specification structure.
 * @param Event The event number to signal upon completion.
 * @return The offset in the buffer after adding the DMA instructions.
 */
static AL_S32 AlDma_Dev_Period(AL_U8 DryRun, AL_DMA_DevStruct *Dev, AL_CHAR *Buf, AL_UINTPTR BurstSize,
                               AL_DMA_XferSpecStruct *XferSpec, AL_S32 Event)
{
    AL_U32 LoopCnt1, LoopJump1;
    AL_S32 Cycle, Off = 0;
    AL_DMA_XferStruct *Xfer = &XferSpec->Desc->Xfer;

    if (BurstSize > 256) {
        LoopCnt1 = 256;
        Cycle = BurstSize / 256;
    } else {
        LoopCnt1 = BurstSize;
        Cycle = 1;
    }

    /* loop1 */
    Off += DMA_Instr_DMALP(DryRun, &Buf[Off], 1, LoopCnt1);
    LoopJump1 = Off;
    Off += AlDma_Dev_Burst(DryRun, Dev, &Buf[Off], XferSpec, Cycle);
    Off +=
        DMA_Instr_DMALPEND(DryRun, &Buf[Off], ALWAYS, AL_FALSE, 1, Off - LoopJump1);

    /* remainder */
    LoopCnt1 = BurstSize - (LoopCnt1 * Cycle);

    if (LoopCnt1) {
        Off += DMA_Instr_DMALP(DryRun, &Buf[Off], 1, LoopCnt1);
        LoopJump1 = Off;
        Off += AlDma_Dev_Burst(DryRun, Dev, &Buf[Off], XferSpec, 1);
        Off += DMA_Instr_DMALPEND(DryRun, &Buf[Off], ALWAYS, AL_FALSE, 1,
                                    Off - LoopJump1);
    }

    if (!XferSpec->Desc->SrcInterlaceSize &&
        !XferSpec->Desc->DstInterlaceSize &&
        Dev->PeripReqType == BURST) {
        AL_U32 Ccr = XferSpec->Ccr;
        AL_UINTPTR c = 0;

        c = BYTE_MOD_BURST_LEN(Xfer->length, XferSpec->Ccr);

        if (c) {
            Ccr &= ~(0xf << CC_SRCBRSTLEN_SHFT);
            Ccr &= ~(0xf << CC_DSTBRSTLEN_SHFT);
            Off += DMA_Instr_DMAMOV(DryRun, &Buf[Off], CCR, Ccr);
            Off += DMA_Instr_DMALP(DryRun, &Buf[Off], 1, c);
            LoopJump1 = Off;
            Off += AlDma_Dev_Burst(DryRun, Dev, &Buf[Off], XferSpec, 1);
            Off += DMA_Instr_DMALPEND(DryRun, &Buf[Off], ALWAYS, AL_FALSE, 1,
                                        Off - LoopJump1);
            Off += DMA_Instr_DMAMOV(DryRun, &Buf[Off], CCR, XferSpec->Ccr);
        }
    }

    Off += DMA_Instr_DMASEV(DryRun, &Buf[Off], Event);

    return Off;
}

/**
 * This function configures the DMA for cyclic transfers by dividing the total transfer into periods.
 * It calculates the number of periods and any residue outside of these periods.
 * The function generates loops for each period and handles the residue with additional instructions.
 *
 * @param DryRun If set to 1, performs a dry run without actual data transfer.
 * @param Dev Pointer to the DMA device structure.
 * @param Buf Pointer to the buffer where DMA instructions are stored.
 * @param XferSpec Pointer to the DMA transfer specification structure.
 * @return The offset in the buffer after setting up the DMA loops.
 *
 * @return None.
 */
static AL_S32 AlDma_Dev_LoopCyclic(AL_U8 DryRun, AL_DMA_DevStruct *Dev, AL_CHAR *Buf, AL_UINTPTR BurstSize,
                                   AL_DMA_XferSpecStruct *XferSpec, AL_S32 Event)
{
    AL_S32 Off, Periods, Residue, i;
    AL_U32 LoopCnt0, LoopJump0, ljmpfe;
    AL_DMA_XferStruct *Xfer = &XferSpec->Desc->Xfer;

    Off = 0;
    ljmpfe = Off;
    LoopCnt0 = XferSpec->Desc->NumPeriods;
    Periods = 1;

    while (LoopCnt0 > 256) {
        Periods++;
        LoopCnt0 = XferSpec->Desc->NumPeriods / Periods;
    }

    Residue = XferSpec->Desc->NumPeriods % Periods;

    /* forever loop */
    Off += DMA_Instr_DMAMOV(DryRun, &Buf[Off], SAR, Xfer->SrcAddr);
    Off += DMA_Instr_DMAMOV(DryRun, &Buf[Off], DAR, Xfer->DstAddr);
    Off += DMA_Instr_DMAFLUSHP(DryRun, &Buf[Off], XferSpec->Desc->Peri);
    /* loop0 */
    Off += DMA_Instr_DMALP(DryRun, &Buf[Off], 0, LoopCnt0);
    LoopJump0 = Off;

    for (i = 0; i < Periods; i++) {
        Off += AlDma_Dev_Period(DryRun, Dev, &Buf[Off], BurstSize, XferSpec, Event);
    }

    Off +=
        DMA_Instr_DMALPEND(DryRun, &Buf[Off], ALWAYS, AL_FALSE, 0, Off - LoopJump0);

    for (i = 0; i < Residue; i++) {
        Off += AlDma_Dev_Period(DryRun, Dev, &Buf[Off], BurstSize, XferSpec, Event);
    }

    Off +=
        DMA_Instr_DMALPEND(DryRun, &Buf[Off], ALWAYS, AL_TRUE, 1, Off - ljmpfe);

    return Off;
}

/**
 *
 * This function calculates the burst size based on the transfer length and control register settings.
 * It then calls AlDma_Dev_Loop to handle the actual loop setup for the transfer.
 * The function adjusts for different transfer directions and interlace sizes.
 *
 * @param DryRun Indicates whether the operation is a dry run (for calculation purposes) or an actual execution.
 * @param Dev Pointer to the DMA device structure.
 * @param Buf Pointer to the buffer where DMA instructions are stored.
 * @param BurstSize The burst size of the transfer.
 * @param XferSpec Pointer to the DMA transfer specification structure.
 * @param Event The event associated with the transfer.
 * @return The offset within the buffer after adding the DMA instructions.
 */
static AL_S32 AlDma_Dev_SetupLoops(AL_U8 DryRun, AL_DMA_DevStruct *Dev, AL_CHAR *Buf, AL_DMA_XferSpecStruct *XferSpec)
{
    AL_DMA_XferStruct *Xfer = &XferSpec->Desc->Xfer;
    AL_U32 Ccr = XferSpec->Ccr;
    AL_UINTPTR c, BurstSize = BYTE_TO_BURST(Xfer->length, Ccr);
    AL_S32 Off = 0;

    if (AlDma_Dev_IsSlaveDirection(XferSpec->Desc->Dir)) {
        Off += DMA_Instr_DMAFLUSHP(DryRun, &Buf[Off], XferSpec->Desc->Peri);
    }

    if (XferSpec->Desc->Dir == DMA_DEV_TO_MEM) {
        BurstSize = Xfer->length / (BRST_SIZE(Ccr) * BRST_LEN(Ccr) +
                              XferSpec->Desc->DstInterlaceSize);
    } else if (XferSpec->Desc->Dir == DMA_MEM_TO_DEV) {
        BurstSize = Xfer->length / (BRST_SIZE(Ccr) * BRST_LEN(Ccr) +
                              XferSpec->Desc->SrcInterlaceSize);
    }

    while (BurstSize) {
        c = BurstSize;
        Off += AlDma_Dev_Loop(DryRun, Dev, &Buf[Off], &c, XferSpec);
        BurstSize -= c;
    }

    return Off;
}

/**
 * This function initializes the DMA transfer by setting the source and destination addresses using DMAMOV instructions.
 * It then calls AlDma_Dev_SetupLoops to configure the necessary loops for the transfer. If the transfer does not require
 * interlacing and is of type BURST, it adjusts the CCR (Channel Control Register) for burst length and sets up additional loops if needed.
 *
 * @param DryRun Indicates whether the operation is a dry run (for calculation purposes) or an actual execution.
 * @param Dev Pointer to the DMA device structure.
 * @param Buf Pointer to the buffer where DMA instructions are stored.
 * @param XferSpec Pointer to the DMA transfer specification structure.
 * @return The offset within the buffer after adding the DMA instructions.
 *
 */
static AL_S32 AlDma_Dev_SetupXfer(AL_U8 DryRun, AL_DMA_DevStruct *Dev, AL_CHAR *Buf, AL_DMA_XferSpecStruct *XferSpec)
{
    AL_DMA_XferStruct *Xfer = &XferSpec->Desc->Xfer;
    AL_S32 Off = 0;

    /* DMAMOV SAR, Xfer->src_addr */
    Off += DMA_Instr_DMAMOV(DryRun, &Buf[Off], SAR, Xfer->SrcAddr);
    /* DMAMOV DAR, Xfer->dst_addr */
    Off += DMA_Instr_DMAMOV(DryRun, &Buf[Off], DAR, Xfer->DstAddr);

    /* Setup Loop(s) */
    Off += AlDma_Dev_SetupLoops(DryRun, Dev, &Buf[Off], XferSpec);

    if (!XferSpec->Desc->SrcInterlaceSize &&
        !XferSpec->Desc->DstInterlaceSize &&
        Dev->PeripReqType == BURST) {
        AL_U32 Ccr = XferSpec->Ccr;
        AL_UINTPTR c = 0;

        c = BYTE_MOD_BURST_LEN(Xfer->length, XferSpec->Ccr);

        if (c) {
            Ccr &= ~(0xf << CC_SRCBRSTLEN_SHFT);
            Ccr &= ~(0xf << CC_DSTBRSTLEN_SHFT);
            Off += DMA_Instr_DMAMOV(DryRun, &Buf[Off], CCR, Ccr);
            Off += AlDma_Dev_Loop(DryRun, Dev, &Buf[Off], &c, XferSpec);
        }
    }

    return Off;
}

/**
 * This function calculates the burst size for a cyclic DMA transfer based on the transfer length, burst size, burst length,
 * and interlace size. It then configures the transfer loops using AlDma_Dev_LoopCyclic.
 *
 * @param DryRun Indicates whether the operation is a dry run (for calculation purposes) or an actual execution.
 * @param Dev Pointer to the DMA device structure.
 * @param Buf Pointer to the buffer where DMA instructions are stored.
 * @param XferSpec Pointer to the DMA transfer specification structure.
 * @param Event The event associated with the transfer.
 * @return The offset within the buffer after adding the DMA instructions.
 *
 */
static AL_S32 AlDma_Dev_SetupXferCyclic(AL_U8 DryRun, AL_DMA_DevStruct *Dev,
                              AL_CHAR *Buf, AL_DMA_XferSpecStruct *XferSpec, AL_S32 Event)
{
    AL_DMA_XferStruct *Xfer = &XferSpec->Desc->Xfer;
    AL_U32 Ccr = XferSpec->Ccr;
    AL_UINTPTR BurstSize = BYTE_TO_BURST(Xfer->length, Ccr);
    AL_S32 Off = 0;

    if (XferSpec->Desc->Dir == DMA_DEV_TO_MEM) {
        BurstSize = Xfer->length / (BRST_SIZE(Ccr) * BRST_LEN(Ccr)
                              + XferSpec->Desc->DstInterlaceSize);
    } else if (XferSpec->Desc->Dir == DMA_MEM_TO_DEV) {
        BurstSize = Xfer->length / (BRST_SIZE(Ccr) * BRST_LEN(Ccr)
                              + XferSpec->Desc->SrcInterlaceSize);
    }

    /* Setup Loop(s) */
    Off += AlDma_Dev_LoopCyclic(DryRun, Dev, &Buf[Off], BurstSize, XferSpec, Event);

    return Off;
}

/**
 * This function maps an endian swap size (0, 8, 16, 32, 64, 128) to its corresponding control bits (0 to 4) used in DMA configuration.
 * If the endian swap size does not match any of the predefined sizes, it returns 0.
 *
 * @param EndianSwapSize The endian swap size in bits.
 * @return The control bits corresponding to the endian swap size.
 *
 */
__STATIC_INLINE AL_U32 AlDma_Dev_ToendianSwapSizeBits(AL_U32 EndianSwapSize)
{
    switch (EndianSwapSize) {
    case 0:
    case 8:

        return 0;
    case 16:

        return 1;
    case 32:

        return 2;
    case 64:

        return 3;
    case 128:

        return 4;
    default:

        return 0;
    }
}

/**
 * This function maps a burst size (1, 2, 4, 8, 16, 32, 64, 128) to its corresponding control bits (0 to 7) used in DMA configuration.
 * If the burst size does not match any of the predefined sizes, it returns 0.
 *
 * @param BurstSize The burst size.
 * @return The control bits corresponding to the burst size.
 *
 */
static AL_U32 AlDma_Dev_ToBurstSizeBits(AL_U32 BurstSize)
{
    switch (BurstSize) {
    case 1:

        return 0;
    case 2:

        return 1;
    case 4:

        return 2;
    case 8:

        return 3;
    case 16:

        return 4;
    case 32:

        return 5;
    case 64:

        return 6;
    case 128:

        return 7;
    default:

        return 0;
    }
}

/**
 * Prepares the Control Configuration Register (CCR) value based on the DMA request configuration.
 *
 * @param ReqCfg Pointer to the DMA request configuration structure.
 * @return The configured CCR value.
 */
static AL_U32 AlDma_Dev_PrepareCCR(AL_DMA_ReqCfgStruct *ReqCfg)
{
    AL_U32 Ccr = 0;

    if (ReqCfg->SrcInc) {
        Ccr |= CC_SRCINC;
    }

    if (ReqCfg->DstInc) {
        Ccr |= CC_DSTINC;
    }

    /* We set same protection levels for Src and DST for now */
    if (ReqCfg->Privileged) {
        Ccr |= CC_SRCPRI | CC_DSTPRI;
    }
    if (ReqCfg->NonSecure) {
        Ccr |= CC_SRCNS | CC_DSTNS;
    }
    if (ReqCfg->InsnAccess) {
        Ccr |= CC_SRCIA | CC_DSTIA;
    }

    Ccr |= (((ReqCfg->BurstLen - 1) & 0xf) << CC_SRCBRSTLEN_SHFT);
    Ccr |= (((ReqCfg->BurstLen - 1) & 0xf) << CC_DSTBRSTLEN_SHFT);

    Ccr |= (ReqCfg->BurstSize << CC_SRCBRSTSIZE_SHFT);
    Ccr |= (ReqCfg->BurstSize << CC_DSTBRSTSIZE_SHFT);

    Ccr |= (ReqCfg->SrcCacheCtl << CC_SRCCCTRL_SHFT);
    Ccr |= (ReqCfg->DstCacheCtl << CC_DSTCCTRL_SHFT);

    Ccr |= (ReqCfg->Swap << CC_SWAP_SHFT);

    return Ccr;
}

/**
 * Calculates the burst length for a DMA transfer based on the device's IP configuration and the transfer length.
 *
 * @param Desc Pointer to the DMA descriptor structure.
 * @param Dev Pointer to the DMA device structure.
 * @param Len The length of the data to be transferred.
 * @return The calculated burst length. Returns -1 if the burst length calculation fails.
 */
/* Call after fixing burst size */
static AL_S32 AlDma_Dev_GetBurstLen(AL_DMA_DescStruct *Desc, AL_DMA_DevStruct *Dev, AL_U32 Len)
{
    AL_S32 burstLen;

    burstLen = Dev->IpCfgInfo.DataBusWidth / 8;
    burstLen *= Dev->IpCfgInfo.DataBufDep / Dev->IpCfgInfo.NumChan * 2;
    burstLen >>= Desc->ReqCfg.BurstSize;

    /* Src/dst_burst_len can't be more than 16 */
    if (burstLen > 16) {
        burstLen = 16;
    }

    while (burstLen > 1) {
        if (!(Len % (burstLen << Desc->ReqCfg.BurstSize))) {
            break;
        }
        burstLen--;
    }

    return burstLen;
}

/**
 * Executes the DMAKILL instruction for a specific channel and thread.
 *
 * @param Reg Pointer to the DMA control register structure.
 * @param Channel The DMA channel number.
 * @param Thread The DMA thread number.
 * @return 0 on success, AL_DMA_ERR_TIMEOUT if the operation times out.
 */
static AL_S32 DMA_Exec_DMAKILL(AL_DMA_DmaCtrlRegStruct *Reg, AL_U32 Channel, AL_U32 Thread)
{
    AL_U32 DbgInst0;
    AL_S32 WaitCount;

    DbgInst0 = DMA_DBGINST0(0, 0x01, Channel, Thread);

    /* wait while debug status is busy */
    WaitCount = 0;
    while ((AL_REG32_READ(&Reg->DBGSTATUS) & DMA_DBGSTATUS_BUSY) &&
           (WaitCount < DMA_MAX_WAIT)) {
        WaitCount++;
    }

    if (WaitCount >= DMA_MAX_WAIT) {
        /* wait time out */
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dev device at %p debug status busy time out\n", Reg);
        return AL_DMA_ERR_TIMEOUT;
    }

    /* write debug instruction 0 */
    AL_REG32_WRITE(&Reg->DBGINST[0], DbgInst0);
    AL_REG32_WRITE(&Reg->DBGINST[1], 0);

    /* run the command in DbgInst0 and DbgInst1 */
    AL_REG32_WRITE(&Reg->DBGCMD, 0);

    return 0;
}

/**
 * Reads and updates the DMA device configuration based on the device's control registers.
 *
 * @param Dev Pointer to the DMA device structure.
 * @return None.
 */
static void AlDma_Dev_ReadConfig(AL_DMA_DevStruct *Dev)
{
    AL_U32 Val, Crdn, Cr;
    AL_DMA_IpCfgInfoStruct *IpCfgInfo = &Dev->IpCfgInfo;
    AL_DMA_DmaCtrlRegStruct *Reg = Dev->CtrlReg;

    Crdn = AL_REG32_READ(&Reg->CRDN);
    Cr = AL_REG32_READ(&Reg->CR[0]);

    Val = (Crdn >> CRD_DATA_WIDTH_SHIFT) & CRD_DATA_WIDTH_MASK;
    IpCfgInfo->DataBusWidth = 8 * (1 << Val);

    IpCfgInfo->RdCap = ((Crdn >> CRD_RD_CAP_SHIFT) & CRD_RD_CAP_MASK) + 1;
    IpCfgInfo->WrCap = ((Crdn >> CRD_WR_CAP_SHIFT) & CRD_WR_CAP_MASK) + 1;

    IpCfgInfo->DataBufDep = ((Crdn >> CRD_DATA_BUFF_SHIFT) & CRD_DATA_BUFF_MASK) + 1;
    IpCfgInfo->NumChan = ((Cr >> CR0_NUM_CHANS_SHIFT) & CR0_NUM_CHANS_MASK) + 1;

    if (Cr & CR0_PERIPH_REQ_SET) {
        IpCfgInfo->NumPeri = ((Cr >> CR0_NUM_PERIPH_SHIFT) & CR0_NUM_PERIPH_MASK) + 1;
        IpCfgInfo->PeriNs = AL_REG32_READ(&Reg->CR[4]);
    } else {
        IpCfgInfo->NumPeri = 0;
    }

    if (Cr & CR0_BOOT_MAN_NS) {
        IpCfgInfo->SecureMode |= DMAC_MODE_NS;
    } else {
        IpCfgInfo->SecureMode &= ~DMAC_MODE_NS;
    }

    IpCfgInfo->NumEvents = ((Cr >> CR0_NUM_EVENTS_SHIFT) & CR0_NUM_EVENTS_MASK) + 1;
    IpCfgInfo->IrqNs = AL_REG32_READ(&Reg->CR[3]);
}

/**
 * Builds the DMA program for a transfer, optionally performing a dry run to calculate the program length.
 *
 * @param DryRun If 1, performs a dry run without actually configuring the device. If 0, configures the device.
 * @param Dev Pointer to the DMA device structure.
 * @param XferSpec Pointer to the DMA transfer specification structure.
 * @param Channel The DMA channel number.
 * @return The offset within the microcode buffer after the program is built. Returns a negative value on failure.
 */
static AL_S32 AlDma_Dev_BuildDmaProg(AL_U8 DryRun, AL_DMA_DevStruct *Dev, AL_DMA_XferSpecStruct *XferSpec,
                                     AL_U32 Channel)
{
    AL_CHAR *Buf = (AL_CHAR *)XferSpec->Desc->McBuf;
    AL_DMA_XferStruct *Xfer = &XferSpec->Desc->Xfer;
    AL_S32 Off = 0;

    DMA_DBGMC_START((AL_UINTPTR)Buf);

    /* DMAMOV CCR, Ccr */
    Off += DMA_Instr_DMAMOV(DryRun, &Buf[Off], CCR, XferSpec->Ccr);

    if (Dev->PeripReqType != BURST) {
        /* Error if xfer length is not aligned at burst size */
        if (Xfer->length % (BRST_SIZE(XferSpec->Ccr) * BRST_LEN(XferSpec->Ccr))) {
            return AL_DMA_ERR_FAIL;
        }
    }

    if (!XferSpec->Desc->Cyclic) {
        Off += AlDma_Dev_SetupXfer(DryRun, Dev, &Buf[Off], XferSpec);
        /* DMASEV peripheral/event */
        Off += DMA_Instr_DMASEV(DryRun, &Buf[Off], Channel);
        /* DMAEND */
        Off += DMA_Instr_DMAEND(DryRun, &Buf[Off]);
    } else {
        Off += AlDma_Dev_SetupXferCyclic(DryRun, Dev, &Buf[Off], XferSpec, Channel);
    }

    /* make sure the Buf and bufsize is cache line aligned. */
    if (!DryRun) {
        AL_ASSERT(IS_CACHELINE_ALIGNED((AL_UINTPTR)Buf), AL_DMA_ERR_BUFF_NOT_ALIGN);
        AlCache_FlushDcacheRange((AL_UINTPTR)Buf, (AL_UINTPTR)(Buf + DMA_CHAN_BUF_LEN));
    }

    return Off;
}

/**
 * Generates the DMA program for a transfer and writes it to the device's microcode buffer.
 *
 * @param Dev Pointer to the DMA device structure.
 * @param XferSpec Pointer to the DMA transfer specification structure.
 * @param Channel The DMA channel number.
 * @return AL_OK on success, AL_DMA_ERR_FAIL if the program length exceeds the buffer size or other failures occur.
 */
static AL_S32 AlDma_Dev_GenDmaProg(AL_DMA_DevStruct *Dev, AL_DMA_XferSpecStruct *XferSpec,
                                   AL_U32 Channel)
{
    AL_DMA_DescStruct *Desc = XferSpec->Desc;
    AL_S32 Len;

    AL_ASSERT(Dev != AL_NULL, AL_DMA_ERR_NULL_PTR);
    AL_ASSERT(XferSpec != AL_NULL, AL_DMA_ERR_NULL_PTR);
    AL_ASSERT(Desc != AL_NULL, AL_DMA_ERR_NULL_PTR);
    AL_ASSERT(Desc->McBuf != AL_NULL, AL_DMA_ERR_NULL_PTR);

    Len = AlDma_Dev_BuildDmaProg(1, Dev, XferSpec, Channel);
    if (Len < 0 || Len > DMA_CHAN_BUF_LEN) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "xfer size is too large, try to increase mc size\n");

        return AL_DMA_ERR_FAIL;
    }

    AlDma_Dev_BuildDmaProg(0, Dev, XferSpec, Channel);

    return AL_OK;
}

/**
 * Executes the DMAGO instruction to start a DMA transfer on a specific channel.
 *
 * @param Reg Pointer to the DMA control register structure.
 * @param Channel The DMA channel number.
 * @param Addr The address of the DMA program in memory.
 * @return AL_OK on success, AL_DMA_ERR_TIMEOUT if the operation times out.
 */
static AL_S32 DMA_Exec_DMAGO(AL_DMA_DmaCtrlRegStruct *Reg, AL_U32 Channel, AL_U32 Addr)
{
    AL_CHAR Buf[8];
    AL_U32 DbgInst0;
    AL_U32 DbgInst1;
    AL_S32 WaitCount;

    DMA_Instr_DMAGO(0, Buf, Channel, Addr, 0);

    DbgInst0 = DMA_DBGINST0(Buf[1], Buf[0], 0, 0);
    DbgInst1 = (AL_U32)Addr;

    /* wait while debug status is busy */
    WaitCount = 0;
    while ((AL_REG32_READ(&Reg->DBGSTATUS) & DMA_DBGSTATUS_BUSY) &&
           (WaitCount < DMA_MAX_WAIT)) {
        WaitCount++;
    }

    if (WaitCount >= DMA_MAX_WAIT) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dev device at %p debug status busy time out\r\n", Reg);
        return AL_DMA_ERR_TIMEOUT;
    }

    /* write debug instruction 0 */
    AL_REG32_WRITE(&Reg->DBGINST[0], DbgInst0);
    /* write debug instruction 1 */
    AL_REG32_WRITE(&Reg->DBGINST[1], DbgInst1);

    /* wait while the Dev Manager is busy */
    WaitCount = 0;
    while ((AL_REG32_READ(&Reg->DSR) & DMA_DS_DMA_STATUS) !=
           DMA_DS_DMA_STATUS_STOPPED &&
           WaitCount <= DMA_MAX_WAIT) {
        WaitCount++;
    }

    if (WaitCount >= DMA_MAX_WAIT) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dev device at %p debug status busy time out\r\n", Reg);
        return AL_DMA_ERR_TIMEOUT;
    }

    /* run the command in DbgInst0 and DbgInst1 */
    AL_REG32_WRITE(&Reg->DBGCMD, 0);

    return AL_OK;
}

/**
 * Cleans and invalidates the data buffer associated with a DMA descriptor to ensure data coherency.
 *
 * @param Desc Pointer to the DMA descriptor structure.
 * @return AL_OK on success, AL_DMA_ERR_BUFF_NOT_ALIGN if the buffer or buffer size is not cache line aligned.
 */
static AL_S32 AlDma_Dev_CleanInvalidateDataBuf(AL_DMA_DescStruct *Desc)
{
    /* make sure the Buf and bufsize cache line aligned. */
    if (Desc->ReqCfg.SrcInc) {
        AL_ASSERT(IS_CACHELINE_ALIGNED(Desc->Xfer.SrcAddr), AL_DMA_ERR_BUFF_NOT_ALIGN);
        AL_ASSERT(IS_CACHELINE_ALIGNED(Desc->Xfer.length), AL_DMA_ERR_BUFF_NOT_ALIGN);
        AlCache_FlushDcacheRange(Desc->Xfer.SrcAddr, Desc->Xfer.SrcAddr + Desc->Xfer.length);
    }

    if (Desc->ReqCfg.DstInc) {
        AL_ASSERT(IS_CACHELINE_ALIGNED(Desc->Xfer.DstAddr), AL_DMA_ERR_BUFF_NOT_ALIGN);
        AL_ASSERT(IS_CACHELINE_ALIGNED(Desc->Xfer.length), AL_DMA_ERR_BUFF_NOT_ALIGN);
        AlCache_InvalidateDcacheRange(Desc->Xfer.DstAddr, Desc->Xfer.DstAddr + Desc->Xfer.length);
    }

    return AL_OK;
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup DMA_Exported_Functions_Group2 State and Errors Functions

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/**
 * Gets the raw interrupt status from the DMA device.
 *
 * @param Dev Pointer to the DMA device structure.
 * @return Raw interrupt status.
 */
AL_U32 AlDma_Dev_GetRawIrqStatus(AL_DMA_DevStruct *Dev)
{
    AL_DMA_DmaCtrlRegStruct *Reg = Dev->CtrlReg;

    return AL_REG32_READ(&Reg->EVENT_RIS);
}

/**
 * Clears a specified interrupt for the DMA device.
 *
 * @param Dev Pointer to the DMA device structure.
 * @param Irq The interrupt number to clear.
 * @return Status code (AL_OK if successful).
 */
AL_S32 AlDma_Dev_ClearIrq(AL_DMA_DevStruct *Dev, AL_U32 Irq)
{
    AL_DMA_DmaCtrlRegStruct *Reg = Dev->CtrlReg;

    AL_REG32_WRITE(&Reg->INTCLR, 1 << Irq);

    return AL_OK;
}

/**
 * Retrieves the current position of the DMA transfer for a specific channel.
 *
 * @param Chan Pointer to the DMA channel structure.
 * @return The number of bytes transferred.
 */
AL_S32 AlDma_Dev_GetPosition(AL_DMA_ChStruct *Chan)
{
    AL_U32 Val = 0, Addr = 0;
    AL_DMA_DevStruct *Dev = Chan->Dev;
    AL_DMA_DescStruct *Desc = &Chan->Desc;
    AL_DMA_DmaCtrlRegStruct *Reg = Dev->CtrlReg;
    AL_S32 Transferred;

    if (Desc->ReqCfg.SrcInc) {
        Val = AL_REG32_READ(&Reg->CHAN_CFG[Chan->ChanId].SAR);
        Addr = Desc->Xfer.SrcAddr;
    } else {
        Val = AL_REG32_READ(&Reg->CHAN_CFG[Chan->ChanId].DAR);
        Addr = Desc->Xfer.DstAddr;
    }

    Transferred = Val - Addr;

    return Transferred;
}

/** @} */

/** @defgroup DMA_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */
/**
 * Resets the DMA device.
 *
 * @return None.
 */
AL_VOID AlDma_Dev_Reset(AL_VOID)
{
    AL_REG32_WRITE(0xF88060A0, 0x00000000);
    AL_REG16_SET_BIT(0xF8801074, 8, 0);
    AlSys_UDelay(1);
    AL_REG16_SET_BIT(0xF8801074, 8, 1);
}

/**
 * Initializes the DMA device and its channels.
 *
 * @param Dev Pointer to the DMA device structure.
 * @return Status code (AL_OK if successful).
 */
AL_S32 AlDma_Dev_Init(AL_DMA_DevStruct *Dev)
{
    AL_DMA_ChStruct *Chan;
    AL_U8 Channel;

    AL_ASSERT(Dev != AL_NULL, AL_DMA_ERR_NULL_PTR);

    memset(Dev->Chans, 0, sizeof(*Chan) * AL_DMA_CHAN_COUNT);

    for (Channel = 0; Channel < AL_DMA_CHAN_COUNT; Channel++) {
        Chan = Dev->Chans + Channel;
        Chan->ChanId = Channel;
    }

    AlDma_Dev_ReadConfig(Dev);

    return AL_OK;
}

/**
 * De-initializes the DMA device.
 *
 * @param Dev Pointer to the DMA device structure.
 * @return Status code (AL_OK if successful).
 */
AL_S32 AlDma_Dev_DeInit(AL_DMA_DevStruct *Dev)
{
    AL_U32 DbgInst;
    AL_U32 WaitCount = 0;
    AL_U32 i;
    AL_DMA_DmaCtrlRegStruct *Reg = Dev->CtrlReg;

    /* Disable all the interrupts */
    AL_REG32_WRITE(&Reg->INTEN, 0x00);
    /* Clear the interrupts */
    AL_REG32_WRITE(&Reg->INTCLR, DMA_INTCLR_ALL_MASK);
    /* Kill the dma Channel threads */
    for (i = 0; i < AL_DMA_CHAN_COUNT; i++) {
        while ((AL_REG32_READ(&Reg->DBGSTATUS) & DMA_DBGSTATUS_BUSY) &&
               (WaitCount < DMA_MAX_WAIT)) {
            WaitCount++;
        }

        DbgInst = DMA_DBGINST0(0, 0x01, i, 1);
        AL_REG32_WRITE(&Reg->DBGINST[0], DbgInst);
        AL_REG32_WRITE(&Reg->DBGINST[1], 0x0);
        AL_REG32_WRITE(&Reg->DBGCMD, 0x0);
    }
    /* Kill the manager Thread    */
    DbgInst = DMA_DBGINST0(0, 0x01, 0, 0);
    AL_REG32_WRITE(&Reg->DBGINST[0], DbgInst);
    AL_REG32_WRITE(&Reg->DBGINST[1], 0x0);
    AL_REG32_WRITE(&Reg->DBGCMD, 0x0);

    return AL_OK;
}

/** @} */

/** @defgroup DMA_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */
/**
 * Starts a DMA transfer on a specific channel.
 *
 * @param Chan Pointer to the DMA channel structure.
 * @return Status code (AL_OK if successful, AL_DMA_ERR_FAIL if not).
 */
AL_S32 AlDma_Dev_Start(AL_DMA_ChStruct *Chan)
{
    AL_S32 ret = AL_OK;
    AL_U32 Ccr;
    AL_DMA_XferSpecStruct XferSpec;
    AL_U32 Channel = Chan->ChanId;
    AL_DMA_DevStruct *Dev = Chan->Dev;
    AL_DMA_DescStruct *Desc = &Chan->Desc;
    AL_DMA_DmaCtrlRegStruct *Reg = Dev->CtrlReg;

    AL_ASSERT(Dev != AL_NULL, AL_DMA_ERR_NULL_PTR);
    AL_ASSERT(Chan->McBuf != AL_NULL, AL_DMA_ERR_NULL_PTR);

    if (Dev->IpCfgInfo.SecureMode & DMAC_MODE_NS) {
        Desc->ReqCfg.NonSecure = 1;
    } else {
        Desc->ReqCfg.NonSecure = 0;
    }

    Ccr = AlDma_Dev_PrepareCCR(&Desc->ReqCfg);

    XferSpec.Ccr = Ccr;
    XferSpec.Desc = Desc;
    Desc->McBuf = Chan->McBuf;

    ret = AlDma_Dev_GenDmaProg(Dev, &XferSpec, Channel);
    if (ret) {
        return AL_DMA_ERR_FAIL;
    }

    /* Lock for one Channel */
    while (!AlDma_Dev_FetchLock(Dev->HwCfg->LockAddr));
    /* enable the interrupt */
    AL_REG32_SET_BIT(&Reg->INTEN, Channel, AL_TRUE);

    ret = DMA_Exec_DMAGO(Dev->CtrlReg, Channel, (AL_UINTPTR)Desc->McBuf);

    AlDma_Dev_ReleaseLock(Chan->Dev->HwCfg->LockAddr);

    return ret;
}

/**
 * Stops a DMA transfer on a specific channel.
 *
 * @param Chan Pointer to the DMA channel structure.
 * @return Status code (AL_OK if successful).
 */
AL_S32 AlDma_Dev_Stop(AL_DMA_ChStruct *Chan)
{
    AL_U32 IntEn = AL_REG32_READ(&Chan->Dev->CtrlReg->INTEN);

    AL_ASSERT(Chan != AL_NULL, AL_DMA_ERR_NULL_PTR);

    DMA_Exec_DMAKILL(Chan->Dev->CtrlReg, Chan->ChanId, 1);

    if (IntEn & (1 << Chan->ChanId)) {
        AL_REG32_WRITE(&Chan->Dev->CtrlReg->INTCLR, 1 << Chan->ChanId);
    }

    AL_REG32_WRITE(&Chan->Dev->CtrlReg->INTEN, IntEn & ~(1 << Chan->ChanId));

    return AL_OK;
}

/**
 * Registers an event callback function for a specific DMA channel.
 *
 * @param Channel Pointer to the DMA channel structure.
 * @param CallBack The callback function to register.
 * @param CallBackRef User-defined reference passed to the callback function.
 * @return Status code (AL_OK if successful).
 */
AL_S32 AlDma_Dev_RegisterEventCallBack(AL_DMA_ChStruct *Channel, AL_DMA_EventCallBack CallBack, AL_VOID *CallBackRef)
{
   AL_ASSERT(Channel != AL_NULL, AL_ERR_NULL_PTR);

    if (Channel->EventCallBack != AL_NULL) {
        AL_LOG(AL_LOG_LEVEL_WARNING, "Dma=%p duplicate register callback: replace old:%p with New: %p\r\n",
               Channel, Channel->EventCallBack, CallBack);
    }

    Channel->EventCallBack      = CallBack;
    Channel->EventCallBackRef   = CallBackRef;

    return AL_OK;
}

/**
 * Unregisters the event callback function for a specific DMA channel.
 *
 * @param Channel Pointer to the DMA channel structure.
 * @return Status code (AL_OK if successful).
 */
AL_S32 AlDma_Dev_UnRegisterEventCallBack(AL_DMA_ChStruct *Channel)
{
   AL_ASSERT(Channel != AL_NULL, AL_ERR_NULL_PTR);

    Channel->EventCallBack      = (AL_DMA_EventCallBack)AL_NULL;
    Channel->EventCallBackRef   = AL_NULL;

    return AL_OK;
}

/**
 * Handles IRQ for a specific DMA channel.
 *
 * @param Channel Pointer to the DMA channel structure.
 * @return None.
 */
AL_VOID AlDma_Dev_IrqHandler(AL_VOID *Channel)
{
    AL_DMA_DevStruct *Dev = ((AL_DMA_ChStruct *)Channel)->Dev;
    AL_DMA_DmaCtrlRegStruct *Reg = Dev->CtrlReg;
    AL_U32 Val, IntEn;
    AL_U32 Ch, i = 0;

    Val = AL_REG32_READ(&Reg->FSRD) & 0x1;
    if (Val) {
        /*
         * if Dev manager is fault
         */
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Fault Type: 0x%x\n", AL_REG32_READ(&Reg->FTRD));
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Fault PC 0x%x\n", AL_REG32_READ(&Reg->DPC));
        /* kill the Dev manager Thread */
        /* Should we disable interrupt?*/
        DMA_Exec_DMAKILL(Reg, 0, 0);
    }

    Val = AL_REG32_READ(&Reg->FSRC) & ((1 << Dev->IpCfgInfo.NumChan) - 1);
    if (Val) {
        while (i < Dev->IpCfgInfo.NumChan) {
            if (Val & (1 << i)) {
                AL_LOG(AL_LOG_LEVEL_DEBUG, "Reset Channel-%d\t CS-%x\n",
                        i, AL_REG32_READ(&Reg->CHAN_STS[i].CSR));
                AL_LOG(AL_LOG_LEVEL_DEBUG, "Reset Channel-%d\t FTC-%x\n",
                        i, AL_REG32_READ(&Reg->FTR[i]));
                /* kill the Channel Thread */
                /* Should we disable interrupt? */
                DMA_Exec_DMAKILL(Dev->CtrlReg, i, 1);
            }
            i++;
        }
    }

    /* Check which event happened i.e, Thread notified */
    Val = AL_REG32_READ(&Reg->EVENT_RIS);
    IntEn = AL_REG32_READ(&Reg->INTEN);

    for (Ch = 0; Ch < AL_DMA_CHAN_COUNT; Ch++) {
        if (Val & (1 << Ch)) { /* Event occurred */
            AL_DMA_EventStruct Event = {
                .EventId = AL_DMA_EVENT_TRANS_DONE,
                .EventData = 0
            };
            Dev->Chans[Ch].EventCallBack(&Event, Dev->Chans[Ch].EventCallBackRef);
            /* Clear the event */
            if (IntEn & (1 << Ch)) {
                AL_REG32_WRITE(&Reg->INTCLR, 1 << Ch);
            }
        }
    }
}

#if 0
/**
 * Sets the microcontroller buffer for a DMA channel.
 *
 * @param Chan Pointer to the DMA channel structure.
 * @param Buf Address of the microcontroller buffer.
 * @return Status code (AL_OK if successful).
 */
AL_S32 AlDma_Dev_SetMcBuf(AL_DMA_ChStruct *Chan, void *Buf)
{
    AL_ASSERT(Chan);
    AL_ASSERT(Buf);

    Chan->McBuf = Buf;

    return AL_OK;
}

#endif

/**
 * Gets the microcontroller buffer for a DMA channel.
 *
 * @param Chan Pointer to the DMA channel structure.
 * @return Pointer to the microcontroller buffer.
 */
void *AlDma_Dev_GetMcBuf(AL_DMA_ChStruct *Chan)
{
    return Chan->McBuf;
}

/**
 * Retrieves the descriptor for a DMA channel.
 *
 * @param Chan Pointer to the DMA channel structure.
 * @return Pointer to the DMA descriptor.
 */
const AL_DMA_DescStruct *AlDma_Dev_GetDesc(AL_DMA_ChStruct *Chan)
{
    return &Chan->Desc;
}

/**
 * Requests a DMA channel from the DMA device.
 *
 * @param Dev Pointer to the DMA device structure.
 * @return Pointer to the allocated DMA channel structure, or NULL if no channel is available.
 */
AL_DMA_ChStruct *AlDma_Dev_RequestChannel(AL_DMA_DevStruct *Dev)
{
    AL_S32 i = 0;
    AL_DMA_ChStruct *Chan = AL_NULL;

    for (i = 0; i < AL_DMA_CHAN_COUNT; i++) {
        if (Dev->Chans[i].Used) {
            continue;
        }
        Chan        = &Dev->Chans[i];
        Chan->Used  = AL_TRUE;
        Chan->Dev = Dev;

        AL_LOG(AL_LOG_LEVEL_DEBUG, "Dev = %p \r\n", Dev);
        break;
    }

    return Chan;
}

/**
 * Releases a DMA channel.
 *
 * @param Chan Pointer to the DMA channel structure.
 * @return AL_OK on success, error code otherwise.
 * None
 */
AL_S32 AlDma_Dev_ReleaseChannel(AL_DMA_ChStruct *Chan)
{
    AL_ASSERT(Chan != AL_NULL, AL_DMA_ERR_NULL_PTR);

    Chan->PeriId = 0;
    Chan->Used = AL_FALSE;

    return AL_OK;
}

/**
 * Configures a DMA channel with specific settings.
 *
 * @param Chan Pointer to the DMA channel structure to configure.
 * @param Config Pointer to the configuration structure containing the new settings.
 * @return AL_OK on success, error code otherwise.
 * None
 */
AL_S32 AlDma_Dev_Config(AL_DMA_ChStruct *Chan, AL_DMA_ChCfgStruct *Config)
{
    AL_ASSERT(Chan != AL_NULL, AL_DMA_ERR_NULL_PTR);

    Chan->Dir = Config->Direction;

    if (Config->Direction == DMA_MEM_TO_DEV) {
        if (Config->DstAddr) {
            Chan->FifoAddr = Config->DstAddr;
        }
        if (Config->DstAddrWidth) {
            Chan->BurstSize = AlDma_Dev_ToBurstSizeBits(Config->DstAddrWidth);
        }
        if (Config->DstMaxBurst) {
            Chan->BurstLen = Config->DstMaxBurst;
        }
        if (Config->SrcInterlaceSize) {
            Chan->SrcInterlaceSize = Config->SrcInterlaceSize;
        }
    } else if (Config->Direction == DMA_DEV_TO_MEM) {
        if (Config->SrcAddr) {
            Chan->FifoAddr = Config->SrcAddr;
        }
        if (Config->SrcAddrWidth) {
            Chan->BurstSize = AlDma_Dev_ToBurstSizeBits(Config->SrcAddrWidth);
        }
        if (Config->SrcMaxBurst) {
            Chan->BurstLen = Config->SrcMaxBurst;
        }
        if (Config->DstInterlaceSize) {
            Chan->DstInterlaceSize = Config->DstInterlaceSize;
        }
    }

    return AL_OK;
}

/**
 * Prepares a DMA channel for cyclic DMA operations.
 *
 * @param Chan Pointer to the DMA channel structure.
 * @param MemAddr Memory address for the DMA operation.
 * @param Len Length of the data to be transferred.
 * @param PeriodLen Length of one period in the cyclic transfer.
 * @param Direction Direction of the DMA transfer (memory to device or device to memory).
 * @return AL_OK on success, error code otherwise.
 * None
 */
AL_S32 AlDma_Dev_PrepDmaCyclic(AL_DMA_ChStruct *Chan, AL_U32 MemAddr, AL_U32 Len, AL_U32 PeriodLen,
                               AL_DMA_TransEnum Direction)
{
    AL_DMA_DevStruct *Dev = Chan->Dev;
    AL_DMA_DescStruct *Desc = &Chan->Desc;
    AL_U32 Src, Dst;

    AL_ASSERT(Dev != AL_NULL, AL_DMA_ERR_NULL_PTR);
    AL_ASSERT(Len % PeriodLen == 0, AL_DMA_ERR_ILLEGAL_LENGTH);
    AL_ASSERT(Direction == DMA_MEM_TO_DEV || Direction == DMA_DEV_TO_MEM, AL_DMA_ERR_ILLEGAL_PARAM);

    memset(Desc, 0x0, sizeof(*Desc));

    switch (Direction) {
    case DMA_MEM_TO_DEV:
        Desc->ReqCfg.SrcInc = 1;
        Desc->ReqCfg.DstInc = 0;
        Src = MemAddr;
        Dst = Chan->FifoAddr;
        break;
    case DMA_DEV_TO_MEM:
        Desc->ReqCfg.SrcInc = 0;
        Desc->ReqCfg.DstInc = 1;
        Src = Chan->FifoAddr;
        Dst = MemAddr;
        break;
    default:

        return AL_DMA_ERR_FAIL;
    }

    Desc->Peri = Chan->PeriId;
    Desc->Dir = Direction;
    Desc->ReqCfg.BurstSize = Chan->BurstSize;

    if (Dev->PeripReqType == BURST) {
        Desc->ReqCfg.BurstLen = Chan->BurstLen;
    } else {
        Desc->ReqCfg.BurstLen = 1;
    }

    Desc->BytesReq = Len;
    Desc->Xfer.SrcAddr = Src;
    Desc->Xfer.DstAddr = Dst;
    Desc->Xfer.length = PeriodLen;

    Desc->Cyclic = AL_TRUE;
    Desc->NumPeriods = Len / PeriodLen;

    Desc->SrcInterlaceSize = Chan->SrcInterlaceSize;
    Desc->DstInterlaceSize = Chan->DstInterlaceSize;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "%s: SrcInterlaceSize: %d, DstInterlaceSize: %d\r\n", __func__,
                                Desc->SrcInterlaceSize, Desc->DstInterlaceSize);

    return AlDma_Dev_CleanInvalidateDataBuf(Desc);
}

/**
 * Prepares a DMA channel for a single DMA operation.
 *
 * @param Chan Pointer to the DMA channel structure.
 * @param MemAddr Memory address for the DMA operation.
 * @param Len Length of the data to be transferred.
 * @param Direction Direction of the DMA transfer (memory to device or device to memory).
 * @return AL_OK on success, error code otherwise.
 * None
 */
AL_S32 AlDma_Dev_PrepDmaSingle(AL_DMA_ChStruct *Chan, AL_U32 MemAddr, AL_U32 Len, AL_DMA_TransEnum Direction)
{
    AL_DMA_DevStruct *Dev = Chan->Dev;
    AL_DMA_DescStruct *Desc = &Chan->Desc;
    AL_U32 Dst;
    AL_U32 Src;

    AL_ASSERT(Dev != AL_NULL, AL_DMA_ERR_NULL_PTR);
    AL_ASSERT(Direction == DMA_MEM_TO_DEV || Direction == DMA_DEV_TO_MEM, AL_DMA_ERR_ILLEGAL_PARAM);

    memset(Desc, 0x0, sizeof(*Desc));

    switch (Direction) {
    case DMA_MEM_TO_DEV:
        Desc->ReqCfg.SrcInc = 1;
        Desc->ReqCfg.DstInc = 0;
        Src = MemAddr;
        Dst = Chan->FifoAddr;
        break;
    case DMA_DEV_TO_MEM:
        Desc->ReqCfg.SrcInc = 0;
        Desc->ReqCfg.DstInc = 1;
        Src = Chan->FifoAddr;
        Dst = MemAddr;
        break;
    default:

        return AL_DMA_ERR_FAIL;
    }

    Desc->Peri = Chan->PeriId;
    Desc->Dir = Direction;
    Desc->ReqCfg.BurstSize = Chan->BurstSize;

    if (Dev->PeripReqType == BURST) {
        Desc->ReqCfg.BurstLen = Chan->BurstLen;
    } else {
        Desc->ReqCfg.BurstLen = 1;
    }

    Desc->BytesReq = Len;
    Desc->Xfer.SrcAddr = Src;
    Desc->Xfer.DstAddr = Dst;
    Desc->Xfer.length = Len;

    Desc->Cyclic = AL_FALSE;
    Desc->NumPeriods = 0;

    return AlDma_Dev_CleanInvalidateDataBuf(Desc);
}

/**
 * Prepares a DMA channel for a memcpy-like operation.
 *
 * @param Chan Pointer to the DMA channel structure.
 * @param Dst Destination memory address for the memcpy operation.
 * @param Src Source memory address for the memcpy operation.
 * @param Len Length of the data to be copied.
 * @return AL_OK on success, error code otherwise.
 * None
 */
AL_S32 AlDma_Dev_PrepDmaMemcpy(AL_DMA_ChStruct *Chan, AL_U32 Dst, AL_U32 Src, AL_U32 Len)
{
    AL_DMA_DevStruct *Dev = Chan->Dev;
    AL_DMA_DescStruct *Desc = &Chan->Desc;
    AL_S32 burst;

    AL_ASSERT(Dev != AL_NULL, AL_DMA_ERR_NULL_PTR);
    AL_ASSERT(Len > 0, AL_DMA_ERR_ILLEGAL_PARAM);

    // AL_LOG(AL_LOG_LEVEL_DEBUG, "%p %p \r\n", Dev, Dev->IpCfgInfo);

    memset(Desc, 0x0, sizeof(*Desc));

    Desc->Xfer.SrcAddr = Src;
    Desc->Xfer.DstAddr = Dst;
    Desc->Xfer.length = Len;
    Desc->ReqCfg.SrcInc = 1;
    Desc->ReqCfg.DstInc = 1;
    Desc->Dir = DMA_MEM_TO_MEM;

    /* Select max possible burst size */
    burst = Dev->IpCfgInfo.DataBusWidth / 8;

    /*
     * Make sure we use a burst size that aligns with all the memcpy
     * parameters because our Dev programming algorithm doesn't cope with
     * transfers which straddle an entry in the Dev device's MFIFO.
     */
    while ((Src | Dst | Len) & (burst - 1)) {
        burst /= 2;
    }

    Desc->ReqCfg.BurstSize = 0;
    while (burst != (1 << Desc->ReqCfg.BurstSize)) {
        Desc->ReqCfg.BurstSize++;
    }

    /*
     * If burst size is smaller than bus width then make sure we only
     * transfer one at a time to avoid a burst stradling an MFIFO entry.
     */
    if (Desc->ReqCfg.BurstSize * 8 < Dev->IpCfgInfo.DataBusWidth) {
        Desc->ReqCfg.BurstLen = 1;
    }

    Desc->ReqCfg.BurstLen = AlDma_Dev_GetBurstLen(Desc, Dev, Len);
    Desc->BytesReq = Len;

    return AlDma_Dev_CleanInvalidateDataBuf(Desc);
}

/**
 * Attempts to acquire a lock for a DMA operation.
 *
 * @param LockAddr Address of the lock register.
 * @return AL_TRUE if the lock was successfully acquired, AL_FALSE otherwise.
 * None
 */
AL_BOOL AlDma_Dev_FetchLock(AL_REG LockAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(LockAddr, 0);
}

/**
 * Releases a previously acquired lock.
 *
 * @param LockAddr Address of the lock register.
 * None
 */
AL_VOID AlDma_Dev_ReleaseLock(AL_REG LockAddr)
{
    AL_REG32_SET_BIT(LockAddr, 0, AL_FUNC_ENABLE);
}

/**
 * Looks up the hardware configuration for a given DMA device ID.
 *
 * @param DevId The device ID for which to lookup the configuration.
 * @return Pointer to the DMA hardware configuration structure if found, NULL otherwise.
 * None
 */
AL_DMA_HwConfigStruct *AlDma_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index = 0;
    AL_DMA_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_DMA_NUM_INSTANCE; Index++) {
        if (AlDma_HwConfig[Index].DeviceId == DevId) {
            ConfigPtr = &AlDma_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

/** @} */

/** @} */

/** @} */

