/*****************************************************************************
*****************************************************************************/

#ifndef AL_UTILS_DEF_H
#define AL_UTILS_DEF_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define ARRAY_SIZE(a)         (sizeof(a) / sizeof((a)[0]))

#define BIT_32(nr)            ((1U) << (nr))
#define BIT_64(nr)            ((1ULL) << (nr))

#define BITS_32(start, size)  ((0xFFFFFFFFU << (start)) & (0xFFFFFFFFU >> (32U - (start) - (size))))
#define BITS_64(start, size)  (((~0x0UL) << (start)) & ((~0x0UL) >> (64U - (start) - (size))))

#if (defined __aarch64__ || defined _AARCH_64 || defined __riscv64)
#define BIT_X                 BIT_64
#define BITS_X                BITS_64
#else
#define BIT_X                 BIT_32
#define BITS_X                BITS_32
#endif

#define DIV_ROUND_UP(val, div) __extension__ ({    \
    __typeof__(div) _div = (div);        \
    ((val) + _div - (__typeof__(div)) 1) / _div;        \
})

#define MIN(x, y) __extension__ ({    \
    __typeof__(x) _x = (x);        \
    __typeof__(y) _y = (y);        \
    (void)(&_x == &_y);        \
    _x < _y ? _x : _y;        \
})

#define MAX(x, y) __extension__ ({    \
    __typeof__(x) _x = (x);        \
    __typeof__(y) _y = (y);        \
    (void)(&_x == &_y);        \
    _x > _y ? _x : _y;        \
})

#define CLAMP(x, min, max) __extension__ ({ \
    __typeof__(x) _x = (x); \
    __typeof__(min) _min = (min); \
    __typeof__(max) _max = (max); \
    (void)(&_x == &_min); \
    (void)(&_x == &_max); \
    (_x > _max ? _max : (_x < _min ? _min : _x)); \
})

/*
 * The ROUND_BOUNDARY() macro rounds up a value to the given boundary.
 * The boundary must be a power of two.
 * ROUND_DOWN() is similar but rounds the value down instead.
 */
#define ROUND_BOUNDARY(value, boundary)        \
    ((__typeof__(value))((boundary) - 1))

#define ROUND_UP(value, boundary)        \
    ((((value) - 1) | ROUND_BOUNDARY(value, boundary)) + 1)

#define ROUND_DOWN(value, boundary)        \
    ((value) & ~ROUND_BOUNDARY(value, boundary))

#define AL_LOG(level, format, ...) \
	do {  \
		if(level >= AL_ERR_LEVEL_DEBUG) \
		printf(format, ##__VA_ARGS__); \
	} while (0);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* AL_UTILS_DEF_H */
