#include <mm.h>

#define NR_PAGES (10)

static unsigned short mem_map[NR_PAGES] = {0,};

static unsigned long phy_start_address;

char pagetable[4096*NR_PAGES] __attribute__ ((aligned(4096))) = {0};

void mem_init(unsigned long start_mem, unsigned long end_mem)
{
	unsigned long nr_free_pages = 0;
	unsigned long free;

	start_mem = PAGE_ALIGN(start_mem);
	phy_start_address = start_mem;
	end_mem &= PAGE_MASK;
	free = end_mem - start_mem;

	while (start_mem < end_mem) {
		nr_free_pages++;
		start_mem += PAGE_SIZE;
	}

	printf("Memory: %uKB available, %u free pages\n", free/1024, nr_free_pages);
}

extern void main_success(void);

unsigned long get_free_page(void)
{
	int i;

	for (i = 0; i < NR_PAGES; i++) {
		if (mem_map[i] == 0) {
			mem_map[i] = 1;
			return (unsigned long)pagetable + i * PAGE_SIZE;
		}
	}

	main_success();
	return 0;
}

void free_page(unsigned long p)
{
	mem_map[(p - (unsigned long)pagetable)/PAGE_SIZE] = 0;
}
