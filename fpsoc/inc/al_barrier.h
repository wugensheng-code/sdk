#ifndef AL_BARRIER_H
#define AL_BARRIER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if (defined _AARCH_64 || defined __aarch64__)

#define __nops(n)      ".rept    " #n "\nnop\n.endr\n"
#define nops(n)        asm volatile(__nops(n))

#define sev()          asm volatile("sev" : : : "memory")
#define wfe()          asm volatile("wfe" : : : "memory")
#define wfi()          asm volatile("wfi" : : : "memory")

#define isb()          asm volatile("isb" : : : "memory")
#define dmb(opt)       asm volatile("dmb " #opt : : : "memory")
#define dsb(opt)       asm volatile("dsb " #opt : : : "memory")

#define psb_csync()    asm volatile("hint #17" : : : "memory")
#define csdb()         asm volatile("hint #20" : : : "memory")

#define spec_bar()     asm volatile("nop\n")

#define pmr_sync()     do {} while (0)

#define mb()           dsb(sy)
#define rmb()          dsb(ld)
#define wmb()          dsb(st)

#define dma_mb()      dmb(osh)
#define dma_rmb()     dmb(oshld)
#define dma_wmb()     dmb(oshst)

#else


#ifndef RISCV_FENCE
#define RISCV_FENCE(p, s) \
    __asm__ __volatile__ ("fence " #p "," #s : : : "memory")
#endif

#define isb() RISCV_FENCE(iorw, iorw)

#define dsb() RISCV_FENCE(iorw, iorw)

#define dmb() RISCV_FENCE(iorw, iorw)

#endif // _AARCH_64, __aarch64__


#ifdef __cplusplus
}

#endif /* __cplusplus */

#endif /* AL_BARRIER_H */
