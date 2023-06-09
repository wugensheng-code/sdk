#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#include "al_type.h"
#include "npu_driver_io.h"
#include "npu_driver_init.h"

#define LOG_TAG "SOFT_NPU_DRIVER"
#include "elog.h"

/*********************************** macros ***********************************/
#define UIO_EXAMPLE_ERROR -1  // error return value
#define UIO_EXAMPLE_SUCCESS 0 // success return value
#define MMAP_OFFSET 0

#define OPEN_ERROR -1                // open() returns -1 on error

/********************************** globals ***********************************/
static int f;     // this is a file descriptor that describes an open UIO device
static char *ptr; // this is the virtual address of the UIO device registers
static int mmap_size; // this is the size of the uio mem

/********************************* functions **********************************/
// initializes the uio driver, returns -1 if failed in error, 0 otherwise
static AL_S32 soft_npu_init(char devDevice[], char sizeDevice[]) {

  // open the device
  f = open(devDevice, O_RDWR);
  if (f == OPEN_ERROR) {
    log_e("uio device open failed, did you forget to sudo?\n");
    return UIO_EXAMPLE_ERROR;
  }

  char uio_size_buf[18];
  int size_fd = open(sizeDevice, O_RDONLY);	
  if (size_fd < 0) {
    log_e("uio size file open failed\n");
	  return UIO_EXAMPLE_ERROR;
  }
  read(size_fd, uio_size_buf, sizeof(uio_size_buf));
  mmap_size = (int)strtol(uio_size_buf, NULL, 0);
  log_d("the size of the mem in uio device %s is %d\n", devDevice, mmap_size);

  // memory map the physical address of the software into virtual address space
  ptr = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, f,
             MMAP_OFFSET);
  if (ptr == MAP_FAILED) {
    log_e("soft npu driver map failed\n");
    return UIO_EXAMPLE_ERROR;
  }

  close(f) ;
  /* put hardware setup here */

  return UIO_EXAMPLE_SUCCESS;
}

// close the UIO device
//	this function must be called after all read/write operations are done
//	to properly unmap the memory and close the file descriptor
static void soft_npu_exit() {
  munmap(ptr, mmap_size);
  close(f);
}


// write to a register of the UIO device
AL_VOID soft_npu_write(AL_U32 offset, AL_U32 value) {
  // the address is cast as a pointer so it can be dereferenced
  *((volatile AL_U32 *)(ptr + offset)) = value;
}

// read from a register of the UIO device
AL_U32 soft_npu_read(AL_U32 offset) {
  return *((volatile AL_U32 *)(ptr + offset));
}

AL_S8 soft_npu_driver_init() {
    extern int soft_irq_fd;

    char dev_dir_name[15] = "";
    char dev_name_dir_name[38] = "";
    char dev_size_dir_name[38] = "";
    int dev_dir_fd = -1, dev_name_fd = -1, dev_size_fd = -1;
    char dev_name[16] = "";
    AL_U8 find_soft_npu = 0;

    int uio_cnt = get_uio_dev_count("/dev");
    log_d("get uio_cnt: %d\n", uio_cnt);
    for (int i = 0; i < uio_cnt; i++) {
        sprintf(dev_dir_name, "/dev/uio%d", i);
        sprintf(dev_name_dir_name, "/sys/class/uio/uio%d/name", i);
        sprintf(dev_size_dir_name, "/sys/class/uio/uio%d/maps/map0/size", i);

        dev_name_fd = open(dev_name_dir_name, O_RDONLY);
        if (dev_name_fd < 0) {
            log_w("fail to open: %s\n", dev_name_dir_name);
            continue;
        }
        
        read(dev_name_fd, dev_name, 15); // the last char is \n.
        log_d("read dev_name: %s\n", dev_name);
        if (strcmp(dev_name, "soft_npu_device") == 0) {
            find_soft_npu = 1;

            int init_res = soft_npu_init(dev_dir_name, dev_size_dir_name);
            if(init_res < 0) {
                log_e("soft npu driver uio init failed\n") ;
                return -1;
            }
            
            soft_irq_fd = open(dev_dir_name, O_RDWR);
            if (soft_irq_fd < 0) {
                log_e("soft_npu device open failed, did you forget to sudo?\n");
                return -1;
            }
        }

        if (find_soft_npu) {
            break;
        }
    }
    if(!find_soft_npu) {
        log_e("fail to init soft_npu driver\n");
        return -1;
    }
}

AL_S8 soft_npu_driver_exit() {
  soft_npu_exit();
  return 0;
}