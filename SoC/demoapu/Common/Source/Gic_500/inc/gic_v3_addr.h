#ifndef __ALSOC_GIC_V3_AADR_H
#define __ALSOC_GIC_V3_ADDR_H


/* GICD_* */
#define GICD_BASE              ((void *)0xdd000000UL)
#define GICD_CTLR              (GICD_BASE + 0x0000)
#define GICD_TYPER             (GICD_BASE + 0x0004)
#define GICD_IGROUPR		   (GICD_BASE + 0x0080)
#define GICD_ISENABLER         (GICD_BASE + 0x0100)
#define GICD_ICENABLER         (GICD_BASE + 0x0180)
#define GICD_ISPENDR           (GICD_BASE + 0x0200)
#define GICD_ICPENDR           (GICD_BASE + 0x0280)
#define GICD_ACTIVE_SET        (GICD_BASE + 0x0300)
#define GICD_ACTIVE_CLEAR      (GICD_BASE + 0x0380)
#define GICD_IPRIORITYR        (GICD_BASE + 0x0400)
#define GICD_ICFGR             (GICD_BASE + 0x0c00)
#define GICD_IGRPMODR          (GICD_BASE + 0x0d00)
#define GICD_CPENDSGIR         (GICD_BASE + 0x0f10)
#define GICD_IROUTER		   (GICD_BASE + 0x6000)

/* GICR_* */
#define GICR_RD_BASE           ((void *)0xdd040000UL)
/* GICR_WEAKER */
#define GICR_WAKER             (GICR_RD_BASE + 0x14)

#define GICR_SGI_BASE          (GICR_RD_BASE + 0x10000)
#define GICR_SGI_IGROUPR0      (GICR_SGI_BASE + 0x0080)
#define GICR_SGI_ACTIVE_CLEAR  (GICR_SGI_BASE + 0x0380)
#define GICR_SGI_ENABLE        (GICR_SGI_BASE + 0x0100)
#define GICR_SGI_ENABLE_CLEAR  (GICR_SGI_BASE + 0x0180)
#define GICR_SGI_PRI           (GICR_SGI_BASE + 0X0400)
#define GICR_SGI_IGRPMOD0      (GICR_SGI_BASE + 0x0d00)
#define GICR_SGI_ICFGR0        (GICR_SGI_BASE + 0x0c00)
#define GICR_SGI_ICFGR1        (GICR_SGI_BASE + 0x0c04)

/* GICC_* */
#define GICC_BASE              ((void *)0xf8c00000UL)
#define GICC_CTRL              (GICC_BASE + 0x0000)
#define GICC_PMR               (GICC_BASE + 0X0004)
#define GICC_BPR               (GICC_BASE + 0X0008)
#define GICC_IAR               (GICC_BASE + 0X000C)
#define GICC_EOIR              (GICC_BASE + 0X0010)
#define GICC_RPR               (GICC_BASE + 0X0014)
#define GICC_HPPIR             (GICC_BASE + 0X0018)
#define GICC_APR               (GICC_BASE + 0X00D0)
#define GICC_NSAPR             (GICC_BASE + 0X00e0)
#define GICC_IIDR              (GICC_BASE + 0X00FC)
#define GICC_DIR               (GICC_BASE + 0X1000)



/* ICC_SRE_ELn */
#define ICC_SRE_EL1_SRE         (1 << 0)
#define ICC_SRE_EL1_DFB         (1 << 1)
#define ICC_SRE_EL1_DIB         (1 << 2)


#endif /* __ALSOC_GIC_V3_H */