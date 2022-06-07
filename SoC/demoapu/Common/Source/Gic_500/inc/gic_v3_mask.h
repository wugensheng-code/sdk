
#ifndef __GICV3_MASK_H
#define __GICV3_MASK_H

#define GICD_CTLR_EnableGrp0       (1 << 0 )
#define GICD_CTLR_EnableGrp1NS     (1 << 1 )
#define GICD_CTLR_EnableGrp1S      (1 << 2 )
#define GICD_CTLR_ARE_S            (1 << 4 )
// #define GICD_CTLR_ARE_NS           (1 << 5 )
//#define GICD_CTLR_DS               (1 << 6 )
#define GICD_CTLR_E1NWF            (1 << 7 )

#define GICD_ITLines_Numbers       (0x1f   )
#define GICR_WEAKER_ProcessorSleep (0 << 1)
#define GICR_WEAKER_ChildrenAsleep (1 << 2)

#endif /* __GICV3_MASK_H */