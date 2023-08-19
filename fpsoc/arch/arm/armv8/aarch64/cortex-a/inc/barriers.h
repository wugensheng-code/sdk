
#ifndef __ALSOC_BARRIERS_H__
#define __ALSOC_BARRIERS_H__

#ifndef __ASSEMBLY__
#define __nops(n)	".rept	" #n "\nnop\n.endr\n"
#define nops(n)		asm volatile(__nops(n))

#define sev()		asm volatile("sev" : : : "memory")
#define wfe()		asm volatile("wfe" : : : "memory")
#define wfi()		asm volatile("wfi" : : : "memory")

#define isb()		asm volatile("isb" : : : "memory")
#define dmb(opt)	asm volatile("dmb " #opt : : : "memory")
#define dsb(opt)	asm volatile("dsb " #opt : : : "memory")

#define psb_csync()	asm volatile("hint #17" : : : "memory")
#define csdb()		asm volatile("hint #20" : : : "memory")

#define spec_bar()	asm volatile("nop\n")

#define pmr_sync()	do {} while (0)

#define mb()		dsb(sy)
#define rmb()		dsb(ld)
#define wmb()		dsb(st)

#define dsbsy()     dsb(sy)
#define dsbish()    dsb(ish)
#define dsbosh()    dsb(osh)
#define dsbnsh()    dsb(nsh)
#define dsbishst()  dsb(ishst)
#define dsboshst()  dsb(oshst)

#define dma_mb()	dmb(osh)
#define dma_rmb()	dmb(oshld)
#define dma_wmb()	dmb(oshst)

#define dmbsy()     dmb(sy)
#define dmbst()     dmb(st)
#define dmbld()     dmb(ld)
#define dmboshld()  dmb(oshld)
#define dmboshst()  dmb(oshst)
#define dmboshld()  dmb(oshld)
#define dmboshst()  dmb(oshst)
#define dmbosh()    dmb(osh)
#define dmbnshld()  dmb(nshld)
#define dmbnshst()  dmb(nshst)
#define dmbnsh()    dmb(nsh)
#define dmbishld()  dmb(ishld)
#define dmbishst()  dmb(ishst)
#define dmbish()    dmb(ish)

#endif

#endif /* __ASM_BARRIERS_H__ */