#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "cma_mem_init.h"

#define LOG_TAG "CMA_MEM_INIT"
#include "elog.h"

AL_S8 cma_mem_init(AL_U32* cma_phy_start,AL_VOID** cma_virt_start) {
    char name_buf[30] = "/dev/cma_mem";
    int fd = -1;
    AL_VOID* access_address = 0;
    AL_U64 block_size = getpagesize() * (1024/4) * 102;

    fd = open(name_buf, O_RDWR);
    if (fd < 0) {
        log_e("open device: %s failed\n", name_buf);  
        return -1;  
    }

    access_address = mmap(NULL, block_size, PROT_READ|PROT_WRITE,  
                    MAP_SHARED, fd, 0);  
    if ( access_address == (AL_VOID*) -1) {  
        log_e("mmap error for device: %s\n", name_buf);  
        return -1;
    }  

    (*cma_virt_start) = access_address;
    log_d("block_virt_start: %lx\n", cma_virt_start);

    AL_U32 phy_addr = 0;
    read(fd, &phy_addr, 4);

    (*cma_phy_start) = phy_addr;
    log_d("block_phy_start: %lx\n", cma_phy_start);

    close(fd);
    
    return 0;
}