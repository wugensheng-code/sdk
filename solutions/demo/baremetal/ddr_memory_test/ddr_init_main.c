#include <stdio.h>
#include <stdint.h>
#include "fd_ddr_init.h"
#include "al_core.h"

#define KB (1024UL)
#define MB (1024UL * KB)

#define ECC_STR (FD_PARA_ECC == DDR_ECC_SIDEBAND) ? "SideBand ECC" : \
                (FD_PARA_ECC == DDR_ECC_INLINE)   ? "Inline ECC" : "without ECC"

int write_read_compare(volatile uint64_t* wptr, volatile uint64_t* rptr, int size, uint64_t seed, const char* tag);

int main()
{

    int err = 0;
    printf("Compile Time: %s %s\r\n", __DATE__, __TIME__);
    printf("DDR Memory Test\r\n");

    err = fd_ddr_init();

    // Total Size in MB
    size_t total_size = FD_PARA_DRAM_DENSITY / 8 * FD_PARA_DQ_WIDTH / FD_PARA_DRAM_WIDTH;
    if (FD_PARA_ECC == DDR_ECC_INLINE)
        total_size = total_size * 7 / 8;
    printf("DDR %s, Available Size = %ld MB\r\n", ECC_STR, total_size);

    err += write_read_compare((void*)(1 * MB), (void*)(1 * MB), 32 * KB / sizeof(uint64_t), 0x114514UL, "1MB offset, 32KB Read Back");
    err += write_read_compare((void*)(4 * MB), (void*)(4 * MB),  8 * MB / sizeof(uint64_t), 0x66CCFFUL, "4MB offset, 8MB Read Back");
    for (int i = 0; i < 1; ++i) {
        err += write_read_compare((void*)(0UL), (void*)(0UL), total_size * MB / sizeof(uint64_t), 0x1145141919810UL, "Full Space Scan");
    }

    printf("DDR Memory Test %s\r\n", err == 0 ? "PASS" : "FAIL");

    return 0;
}

static uint64_t random_val;

void ddr_test_seed(uint64_t v)
{
    random_val = v;
}

uint64_t ddr_test_rand()
{
    random_val = random_val * 25214903917ULL + 11ULL;
    return random_val;
}

int write_read_compare(volatile uint64_t* wptr, volatile uint64_t* rptr, int size, uint64_t seed, const char* tag)
{
    ddr_test_seed(seed);
    for (int i = 0; i < size; i += 1)
    {
        uint64_t val = ddr_test_rand();
        wptr[i] = val;
    }

    int err = 0;
    ddr_test_seed(seed);
    for (int i = 0; i < size; i += 1)
    {
        uint64_t gold = ddr_test_rand();
        uint64_t val  = rptr[i];
        if (gold != val)
        {
            ++err;
            printf("#%d [0x%p] == 0x%lx, Exp = 0x%lx, Xor = 0x%lx\r\n", i, rptr + i, val, gold, val ^ gold);
        }
    }

    if (err == 0) {
        printf("<%s> PASS\r\n", tag);
    }
    else {
        printf("<%s> FAIL: %d in %d\r\n", tag, err, size);
    }
    return err;
}
