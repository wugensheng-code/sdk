#ifndef __GICV3_VALUE_H
#define __GICV3_VALUE_H

#define GICD_ENABLE			        0x1
#define GICD_DISABLE			    0x0
#define GICD_INT_ACTLOW_LVLTRIG		0x0
#define GICD_INT_DEF_PRI		    0xa0
#define GICD_INT_DEF_PRI_X4		    ((GICD_INT_DEF_PRI << 24) |\
					                 (GICD_INT_DEF_PRI << 16) |\
					                 (GICD_INT_DEF_PRI << 8) |\
					                  GICD_INT_DEF_PRI)

#define GICD_INT_EN_CLR_X32		0xffffffff
#define GICD_INT_EN_SET_SGI		0x0000ffff
#define GICD_INT_EN_CLR_PPI		0xffff0000

#define GICD_CTLR_RWP                   (1U << 31)
#define GICD_CTLR_nASSGIreq             (1U << 8)
#define GICD_CTLR_DS                    (1U << 6)
#define GICD_CTLR_ARE_NS                (1U << 4)
#define GICD_CTLR_ENABLE_G1A            (1U << 1)
#define GICD_CTLR_ENABLE_G1             (1U << 0)

#define GICR_SGI_EDGE_TRIG      0xaaaaaaaa
#define GICR_SGI_LEVEL_TRIG     0X0

#define MPIDR_LEVEL_BITS_SHIFT	3
#define MPIDR_LEVEL_BITS	(1 << MPIDR_LEVEL_BITS_SHIFT)
#define MPIDR_LEVEL_MASK	((1 << MPIDR_LEVEL_BITS) - 1)

#define MPIDR_LEVEL_SHIFT(level) \
	(((1 << level) >> 1) << MPIDR_LEVEL_BITS_SHIFT)

#define MPIDR_AFFINITY_LEVEL(mpidr, level) \
	((mpidr >> MPIDR_LEVEL_SHIFT(level)) & MPIDR_LEVEL_MASK)



#define GICR_WAKER_ProcessorSleep	(1U << 1)
#define GICR_WAKER_ChildrenAsleep	(1U << 2)

/*
 * CPU interface registers
 */
#define ICC_CTLR_EL1_EOImode_SHIFT	(1)
#define ICC_CTLR_EL1_EOImode_drop_dir	(0U << ICC_CTLR_EL1_EOImode_SHIFT)
#define ICC_CTLR_EL1_EOImode_drop	(1U << ICC_CTLR_EL1_EOImode_SHIFT)
#define ICC_CTLR_EL1_EOImode_MASK	(1 << ICC_CTLR_EL1_EOImode_SHIFT)
#define ICC_CTLR_EL1_CBPR_SHIFT		0
#define ICC_CTLR_EL1_CBPR_MASK		(1 << ICC_CTLR_EL1_CBPR_SHIFT)
#define ICC_CTLR_EL1_PMHE_SHIFT		6
#define ICC_CTLR_EL1_PMHE_MASK		(1 << ICC_CTLR_EL1_PMHE_SHIFT)
#define ICC_CTLR_EL1_PRI_BITS_SHIFT	8
#define ICC_CTLR_EL1_PRI_BITS_MASK	(0x7 << ICC_CTLR_EL1_PRI_BITS_SHIFT)
#define ICC_CTLR_EL1_ID_BITS_SHIFT	11
#define ICC_CTLR_EL1_ID_BITS_MASK	(0x7 << ICC_CTLR_EL1_ID_BITS_SHIFT)
#define ICC_CTLR_EL1_SEIS_SHIFT		14
#define ICC_CTLR_EL1_SEIS_MASK		(0x1 << ICC_CTLR_EL1_SEIS_SHIFT)
#define ICC_CTLR_EL1_A3V_SHIFT		15
#define ICC_CTLR_EL1_A3V_MASK		(0x1 << ICC_CTLR_EL1_A3V_SHIFT)
#define ICC_CTLR_EL1_RSS		(0x1 << 18)
#define ICC_CTLR_EL1_ExtRange		(0x1 << 19)
#define ICC_PMR_EL1_SHIFT		0
#define ICC_PMR_EL1_MASK		(0xff << ICC_PMR_EL1_SHIFT)
#define ICC_BPR0_EL1_SHIFT		0
#define ICC_BPR0_EL1_MASK		(0x7 << ICC_BPR0_EL1_SHIFT)
#define ICC_BPR1_EL1_SHIFT		0
#define ICC_BPR1_EL1_MASK		(0x7 << ICC_BPR1_EL1_SHIFT)
#define ICC_IGRPEN0_EL1_SHIFT		0
#define ICC_IGRPEN0_EL1_MASK		(1 << ICC_IGRPEN0_EL1_SHIFT)
#define ICC_IGRPEN1_EL1_SHIFT		0
#define ICC_IGRPEN1_EL1_MASK		(1 << ICC_IGRPEN1_EL1_SHIFT)

//#define ICC_SRE_EL1_DIB			(1U << 2)
//#define ICC_SRE_EL1_DFB			(1U << 1)
//#define ICC_SRE_EL1_SRE			(1U << 0)

#endif /* __GICV3_VALUE_H */