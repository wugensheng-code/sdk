#ifndef __ALSOC_IO_H
#define __ALSOC_IO_H

#include "type.h"
#include "barriers.h"

/*
 * Generic IO read/write.  These perform native-endian accesses.
 */
#define __raw_writeb __raw_writeb
static inline void __raw_writeb(u8 val, volatile void __iomem *addr)
{
	asm volatile("strb %w0, [%1]" : : "rZ" (val), "r" (addr));
}

#define __raw_writew __raw_writew
static inline void __raw_writew(u16 val, volatile void __iomem *addr)
{
	asm volatile("strh %w0, [%1]" : : "rZ" (val), "r" (addr));
}

#define __raw_writel __raw_writel
static __always_inline inline void __raw_writel(u32 val, volatile void __iomem *addr)
{
	asm volatile("str %w0, [%1]" : : "rZ" (val), "r" (addr));
}

#define __raw_writeq __raw_writeq
static inline void __raw_writeq(AL_U64 val, volatile void __iomem *addr)
{
	asm volatile("str %x0, [%1]" : : "rZ" (val), "r" (addr));
}

#define __raw_readb __raw_readb
static inline u8 __raw_readb(const volatile void __iomem *addr)
{
	u8 val;
	asm volatile("ldrb %w0, [%1]" \
		: "=r" (val) \
		: "r" (addr));
	return val;
}

#define __raw_readw __raw_readw
static inline u16 __raw_readw(const volatile void __iomem *addr)
{
	u16 val;

	asm volatile("ldrh %w0, [%1]" \
		     : "=r" (val) : "r" (addr));
	return val;
}

#define __raw_readl __raw_readl
static __always_inline inline u32 __raw_readl(const volatile void __iomem *addr)
{
	u32 val;
	asm volatile("ldr %w0, [%1]" \
		     : "=r" (val) : "r" (addr));
	return val;
}

#define __raw_readq __raw_readq
static inline AL_U64 __raw_readq(const volatile void __iomem *addr)
{
	AL_U64 val;
	asm volatile("ldr %0, [%1]" \
		     : "=r" (val) : "r" (addr));
	return val;
}

/* IO barriers */
#define __iormb(v)							\
({									\
	unsigned long tmp;						\
									\
	dma_rmb();								\
									\
	/*								\
	 * Create a dummy control dependency from the IO read to any	\
	 * later instructions. This ensures that a subsequent call to	\
	 * udelay() will be ordered due to the ISB in get_cycles().	\
	 */								\
	asm volatile("eor	%0, %1, %1\n"				\
		     "cbnz	%0, ."					\
		     : "=r" (tmp) : "r" ((unsigned long)(v))		\
		     : "memory");					\
})

#define __io_par(v)	__iormb(v)
#define __iowmb() dma_wmb()
#define __iomb()  dma_mb()


/*
 * Relaxed I/O memory access primitives. These follow the Device memory
 * ordering rules but do not guarantee any ordering relative to Normal memory
 * accesses.
 */
#define readb_relaxed(addr)	({ u8  __r = __raw_readb(addr); __r; })
#define readw_relaxed(addr)	({ u16 __r = __raw_readw(addr); __r; })
#define readl_relaxed(addr)	({ u32 __r = __raw_readl(addr); __r; })
#define readq_relaxed(addr)	({ AL_U64 __r = __raw_readq(addr); __r; })

#define writeb_relaxed(val,addr)	((void)__raw_writeb((val),(addr)))
#define writew_relaxed(val,addr)	((void)__raw_writew((val),(addr)))
#define writel_relaxed(val,addr)	((void)__raw_writel((val),(addr)))
#define writeq_relaxed(val,addr)	((void)__raw_writeq((val),(addr)))

/*
 * I/O memory access primitives. Reads are ordered relative to any
 * following Normal memory access. Writes are ordered relative to any prior
 * Normal memory access.
 */
#define readb(addr)		({ u8  __v = readb_relaxed(addr); __iormb(__v); __v; })
#define readw(addr)		({ u16 __v = readw_relaxed(addr); __iormb(__v); __v; })
#define readl(addr)		({ u32 __v = readl_relaxed(addr); __iormb(__v); __v; })
#define readq(addr)		({ AL_U64 __v = readq_relaxed(addr); __iormb(__v); __v; })

#define writeb(val,addr)		({ __iowmb(); writeb_relaxed((val),(addr)); })
#define writew(val,addr)		({ __iowmb(); writew_relaxed((val),(addr)); })
#define writel(val,addr)		({ __iowmb(); writel_relaxed((val),(addr)); })
#define writeq(val,addr)		({ __iowmb(); writeq_relaxed((val),(addr)); })

#endif /* __IO_H */
