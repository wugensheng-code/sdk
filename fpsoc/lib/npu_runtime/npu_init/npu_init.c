#include <unistd.h>
#include <string.h>

#include "image.h"
#include "npu_util.h"
#include "fifo.h"
#include "npu_rt_graph.h"
#include "rgb_deserializer.h"
#include "npu_init.h"
#include "npu_defines.h"
#include "buddy_allocator.h"
#include "fm_mem_alloc.h"
#include "al_intr.h"

#ifdef RUN_PLATFORM_LINUX
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>

#include "hard_npu_uio.h"
#include "soft_npu_uio.h"
#endif

#ifdef RUN_PLATFORM_BM
#include "ff.h"
#include "gic.h"
#endif

#define READY_QUEUE_SIZE 350


extern void soft_node_irq_handler(void) ;
extern void hard_node_irq_handler(void) ;


rt_graph_t* load_network (char* rt_graph_fn, char* filter_fn) {

#ifdef RUN_PLATFORM_BM

    /* mount sd card */
    TCHAR  *Path = "0:/";
    static FATFS fatfs;
    FRESULT Res = f_mount(&fatfs, Path, 0);
	if (Res != FR_OK) {
		printf("f_mount failed\n") ;
        return NULL;
    }

#endif

    rt_graph_t* graph = load_rt_graph (rt_graph_fn);
    // dump_rt_graph (graph) ;

#ifdef RUN_PLATFORM_LINUX
    /* --------------- init hard/soft_npu driver -------------------------------------------- */
    extern int hard_irq_fd;
    extern int soft_irq_fd;

    char dev_dir_name[15] = "";
    char dev_name_dir_name[38] = "";
    char dev_size_dir_name[38] = "";
    int dev_dir_fd = -1, dev_name_fd = -1, dev_size_fd = -1;
    char dev_name[16] = "";
    uint8_t find_hard_npu = 0;
    uint8_t find_soft_npu = 0;

    int uio_cnt = get_uio_dev_count("/dev");
    printf("get uio_cnt: %d\n", uio_cnt);
    for (int i = 0; i < uio_cnt; i++) {
        sprintf(dev_dir_name, "/dev/uio%d", i);
        sprintf(dev_name_dir_name, "/sys/class/uio/uio%d/name", i);
        sprintf(dev_size_dir_name, "/sys/class/uio/uio%d/maps/map0/size", i);

        dev_name_fd = open(dev_name_dir_name, O_RDONLY);
        if (dev_name_fd < 0) {
            printf("fail to open: %s\n", dev_name_dir_name);
            continue;
        }

        read(dev_name_fd, dev_name, 15); // the last char is \n.
        printf("read dev_name: %s\n", dev_name);
        if (strcmp(dev_name, "hard_npu_device") == 0) {
            find_hard_npu = 1;

            int init_res = hard_npu_init(dev_dir_name, dev_size_dir_name);
            if(init_res < 0) {
                printf("npu_driver uio init failed\n") ;
                return NULL ;
            }

            hard_irq_fd = open(dev_dir_name, O_RDWR);
            if (hard_irq_fd < 0) {
                printf("hard_npu device open failed, did you forget to sudo?\n");
                return NULL;
            }
        } else if (strcmp(dev_name, "soft_npu_device") == 0) {
            find_soft_npu = 1;

            int init_res = soft_npu_init(dev_dir_name, dev_size_dir_name);
            if(init_res < 0) {
                printf("npu_driver uio init failed\n") ;
                return NULL ;
            }

            soft_irq_fd = open(dev_dir_name, O_RDWR);
            if (soft_irq_fd < 0) {
                printf("hard_npu device open failed, did you forget to sudo?\n");
                return NULL;
            }
        }

        if (find_hard_npu && find_soft_npu) {
            break;
        }
    }
    if(!(find_hard_npu && find_soft_npu)) {
        printf("fail to find hard_npu and soft_npu driver\n");
        return NULL;
    }
    /* --------------- end of init hard/soft_npu driver -------------------------------------------- */
#endif

    extern int16_t* ts_usage_rec;
    ts_usage_rec = (int16_t*) malloc (sizeof(int16_t) * graph->tensor_num) ;
    if (ts_usage_rec == NULL) {
        printf("malloc failed for ts_usage_rec\n") ;
    }
    // init ts_usage_rec ;
    for (int i = 0; i < graph->tensor_num; i++) {
        ts_usage_rec[i] = 0 ;
    }

    extern int16_t* sched_nd_rec;
    sched_nd_rec = (int16_t*) malloc (sizeof(int16_t) * graph->scheduler_node_num) ;
    if (sched_nd_rec == NULL) {
        printf("malloc failed for sched_nd_rec\n") ;
    }
    // init sched_nd_rec ;
    for (int i = 0; i < graph->scheduler_node_num; i++) {
        sched_nd_rec[i] = 0 ;
    }

    extern uint64_t* ts_vaddr_rec;
    ts_vaddr_rec = (int64_t*) malloc (sizeof(uint64_t) * graph->tensor_num) ;
    if (ts_vaddr_rec == NULL) {
        printf("malloc failed for ts_usage_rec\n") ;
    }
    // init ts_usage_rec ;
    for (int i = 0; i < graph->tensor_num; i++) {
        ts_vaddr_rec[i] = 0 ;
    }


    extern fifo_t* hard_ready_nodes ;
    hard_ready_nodes = create_fifo (sizeof(void*), READY_QUEUE_SIZE + 1) ;
    extern fifo_t* soft_ready_nodes ;
    soft_ready_nodes = create_fifo (sizeof(void*), READY_QUEUE_SIZE + 1) ;
    extern fifo_t* hard_running_nodes ;
    hard_running_nodes = create_fifo (sizeof(void*), READY_QUEUE_SIZE + 1) ;
    extern fifo_t* soft_running_nodes ;
    soft_running_nodes = create_fifo (sizeof(void*), READY_QUEUE_SIZE + 1) ;


    extern uint32_t fm_start_addr ; // filter data is stored before fm, after init, fm_start_addr is defined
    extern uint32_t fm_current_addr ; // record the start addr of next alloc
    extern uint32_t cma_phy_start ; // record the phy_addr of the start of cma mem block
    extern void* cma_virt_start ; // record the vir_addr of the start of cma mem block
    extern struct buddy* buddy_allocator ;

    /* --------------- init cma_mem -------------------------------------------- */

#ifdef RUN_PLATFORM_LINUX

    char name_buf[30] = "/dev/cma_mem";
    int fd = -1;
    void* access_address = 0;
    size_t block_size = getpagesize() * (1024/4) * 102;

    fd = open(name_buf, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open device: %s failed\n", name_buf);
        return NULL;
    }

    access_address = mmap(NULL, block_size, PROT_READ|PROT_WRITE,
                    MAP_SHARED, fd, 0);
    if ( access_address == (void*) -1) {
        fprintf(stderr, "mmap error for device: %s\n", name_buf);
        return NULL;
    }

    cma_virt_start = access_address;
    printf("block_virt_start: %lx\n", cma_virt_start);

    uint32_t phy_addr = 0;
    read(fd, &phy_addr, 4);

    cma_phy_start = phy_addr;
    printf("block_phy_start: %lx\n", cma_phy_start);

    close(fd);

#else

    size_t cma_size = 102 * 1024 *1024 ; // 102MB
    cma_virt_start = malloc(cma_size);
    if(cma_virt_start == NULL) {
        printf("Error: alloc cma mem failed\n");
        return NULL;
    }
    cma_phy_start = cma_virt_start;

#endif

    fm_start_addr = 0;
    fm_current_addr = 0;

    /* --------------- end of init npu_mem -------------------------------------------- */

    /* --------------- read in weight file -------------------------------------------- */
    size_t file_len = -1 ;

#ifdef RUN_PLATFORM_LINUX

    struct stat stat;
    fd = open(filter_fn, O_RDONLY);
    if (fd < 0) {
        printf("cannot open file %s\n", filter_fn);
        return NULL;
    }

    // move filter data to mem block && update all filter addr && update all pkg_addr
    fstat(fd, &stat);
    file_len = stat.st_size;

    size_t ret = read(fd, cma_virt_start, file_len) ;
    if (ret != file_len) {
        printf("read failed, read num: %lu expected num: %lu \n", ret, file_len);
        return NULL;
    }

    close(fd);

#else

    FILINFO fno ;
	FIL fd ;
    int ret = f_open(&fd, filter_fn, FA_READ) ;
    if (ret) {
        printf("Fail to open file %s\n", filter_fn) ;
        return NULL ;
    }
    f_stat(filter_fn, &fno) ;
    file_len = fno.fsize ;

    UINT br = 0 ;
    ret = f_read(&fd, cma_virt_start, file_len, &br) ;
    if (ret) {
        printf("Read filter data file failed: %s, err no: %d\n", filter_fn, ret) ;
        return NULL ;
    }
    if (file_len != br) {
		printf("Fail to read expected length from file %s, expected length: %lu, actual length: %lu\n", filter_fn, file_len, (size_t)br) ;
        return NULL ;
	}

	ret = f_close(&fd) ;
    if (ret) {
        printf("Fail to close file %s, err no: %d\n", filter_fn, ret) ;
        return 0 ;
    }

#endif

    if (file_len > WEIGHT_DATA_LENGTH) {
        printf("file_len: %lx of weight.bin is more than reserved value: %lx\n", file_len, WEIGHT_DATA_LENGTH);
        return NULL;
    }
    fm_start_addr = WEIGHT_DATA_LENGTH ;

    for (int i = 0; i < graph->tensor_num; i++) {
        rt_tensor_t* t = get_rt_graph_tensor(graph, i) ;
        if (t->tensor_type == RT_TENSOR_CONST) {
            t->param->addr += cma_phy_start;
        }
    }

    extern int hard_node_num ;
    extern int soft_node_num ;
    hard_node_num = 0 ;
    soft_node_num = 0 ;

    for (int i = 0; i < graph->node_num; i++) {
        rt_node_t* nd = get_rt_graph_node(graph, i) ;
        uint8_t node_type = nd->node_type ;
        switch (node_type) {
            case RT_NPU_HARD :
            {
                hard_npu_pkg_info_t* hpi = (hard_npu_pkg_info_t*) nd->param ;
                hpi->pkg_info.virt_addr = hpi->pkg_info.addr + cma_virt_start ;
                hpi->pkg_info.addr += cma_phy_start ;
                hard_node_num++ ;
                break ;
            }
            case RT_POOLING:
            case RT_CONCAT:
            case RT_UPSAMPLE:
            case RT_PRELU:
            case RT_ELTWISE:
            case RT_INTERP:
            {
                soft_npu_pkg_info_t* spi = (soft_npu_pkg_info_t*) nd->param ;
                spi->pkg_info.virt_addr = spi->pkg_info.addr + cma_virt_start ;
                spi->pkg_info.addr += cma_phy_start ;
                soft_node_num++ ;
                break ;
            }

            case RT_YOLO:
            {
                yolo_pkg_info_t** ypi = (yolo_pkg_info_t**) nd->param ;

                for (unsigned int i = 0; i < nd->input_num; i++) {
                    ypi[i]->pkg_info.virt_addr = ypi[i]->pkg_info.addr + cma_virt_start ;
                    ypi[i]->pkg_info.addr += cma_phy_start ;
                    soft_node_num++ ;
                }

                break ;
            }
            case RT_INPUT:
            default:
            {
                break;
            }
        }
    }

    fm_current_addr = fm_start_addr;

    buddy_allocator = buddy_new(BUDDY_LEVEL) ;

    // exclude weight data in the first buddy
    buddy_alloc(buddy_allocator, fm_start_addr) ;

#ifdef RUN_PLATFORM_BM


    /* register irq handler */
    AlIntr_RegHandler(129 - 13, NULL, soft_node_irq_handler, NULL);
    AlIntr_RegHandler(144 - 13, NULL, hard_node_irq_handler, NULL);

#if 0
    /* enable soft node irq */
    gicv3_enable_irq(129 - 13);

    /* enable hard node irq */
    gicv3_enable_irq(144 - 13);
#endif

#endif

    return graph ;
}

