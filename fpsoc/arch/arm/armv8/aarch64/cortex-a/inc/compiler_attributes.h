#ifndef __COMPILER_ATTRIBUTES_H__
#define __COMPILER_ATTRIBUTES_H__

#ifdef __CHECKER__

#define __iomem    __attribute__((noderef, address_space(2)))
#define __force	   __attribute__((force))


#else

#define __iomem
#define __force

#endif


#define cacheline_aligned __attribute__((__aligned__(64)))

#if __has_attribute(__fallthrough__)
# define fallthrough                    __attribute__((__fallthrough__))
#else
# define fallthrough                    do {} while (0)  /* fallthrough */
#endif

#ifndef __always_inline
#define __always_inline	inline __attribute__((always_inline))
#endif

#endif /* __COMPILER_ATTRIBUTES_H__ */
