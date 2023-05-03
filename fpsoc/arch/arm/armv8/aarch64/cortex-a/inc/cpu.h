#ifndef __ARM64_CPU_H  
#define __ARM64_CPU_H

#ifndef __ASSEMBLY__

static inline void cpu_relax(void)
{
	asm volatile("yield" ::: "memory");
}

#define IS_ALIGNED(x, a)		(((x) & ((typeof(x))(a) - 1)) == 0)

#else

#endif /* __ASSEMBLY__ */

#endif /* __ARM64_CPU_H */