
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

#include "npu_util.h"
#include "npu_defines.h"

#ifdef RUN_PLATFORM_LINUX
#include <dirent.h>
#include <errno.h>
#endif

#ifdef RUN_PLATFORM_BM
#include "ff.h"
#endif

void dump_tensor (rt_tensor_t* t) {

#ifdef RUN_PLATFORM_LINUX

    int fd = open(t->name, O_CREAT|O_WRONLY, S_IRWXU|S_IRWXO);
    if (fd < 0)
    {
        printf("cannot open file %s\n", t->name);
        return;
    }

    extern uint64_t* ts_vaddr_rec;
    void* tenser_buf = ts_vaddr_rec[t->index] ;

    int ret = write(fd, tenser_buf, t->param->size) ;
    if (ret != t->param->size) {
        printf("write file %s failed, file size: %lu, write len: %d\n", t->name, t->param->size, ret) ;
        return;
    }

    close(fd) ;

#else 

    char name[30];
    sprintf(name, "mqye/debug/%s", t->name);
//    sprintf(name, "%s", t->name);

    // FATFS fatfs;
    // TCHAR  *Path = "0:/";
    // FRESULT Res = f_mount(&fatfs, Path, 0);
    // if (Res != FR_OK) {
    //     printf("f_mount failed\n");
    // }

    FIL   fil;
    FRESULT Res = f_open(&fil, name, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
    if (Res) {
       printf("open file: %s failed\n", name);
       printf("res num: %d\n", Res) ;
    }

    void* buf = (void*)t->param->addr ;
    int size = t->param->size ;
    int num_wr = 0 ;
    f_write (&fil, buf, size, &num_wr) ;

    if(num_wr != size) {
       printf("write failed: size: %d\t num_wr: %d\n", size, num_wr) ;
    }

    f_close (&fil) ;

#endif

return ;
}

char* strdup(const char* src) {
    if (src == NULL) {
        return NULL ;
    }
    int n = strlen(src) ;
    char* new_str = (char*) malloc(n + 1) ;
    if (new_str == NULL) {
        return NULL ;
    }
    memcpy(new_str, src, n + 1) ;
    return new_str;
}

// int8_t process_input_image (char* pic_fn, npu_input_param_t* input_param, uint8_t run_platform) {
//     /* hardcode parameters */
//     int img_h = input_param->input_h;
//     int img_w = input_param->input_w;
//     int img_c = input_param->input_c;
//     const float mean[3] = {input_param->mean[0], input_param->mean[1], input_param->mean[2]};
//     const float scale[3] = {input_param->scale[0], input_param->scale[1], input_param->scale[2]};
//     float input_scale = input_param->scale_q;
//     int input_zero_point = input_param->zero_point;

//    // FILINFO fno ;
//    // FIL fd ;
//    // int ret = f_open(&fd, pic_fn, FA_READ) ;
//    // if (ret) {
//    //     printf("Fail to open file %s, err no: %d\n", pic_fn, ret) ;
//    //     return NULL ;
//    // }
//    // f_stat(pic_fn, &fno) ;
//    // int file_len = fno.fsize ;
//    // printf("start read input pic\n") ;

//    // #define IMAGE_START_ADDR 0x40000000
//    // char* image_start_addr = IMAGE_START_ADDR ;
//    // UINT br = 0 ;
//    // ret = f_read(&fd, (void*)image_start_addr, file_len, &br) ;
//    // if (ret) {
//    //     printf("Fail to read file: %s, err no: %d\n", pic_fn, ret) ;
//    //     return NULL ;
//    // }
//    // if (file_len != br) {
//    // 	printf("Fail to read expected length from file %s, expected length: %d, actual length: %d\n", pic_fn, file_len, br) ;
//    //     return NULL ;
//    // }

//    // ret = f_close(&fd) ;
//    // if (ret) {
//    //     printf("Fail to close file %s, err no: %d\n", pic_fn, ret) ;
//    //     return 0 ;
//    // }

//    // image img_input ;
//    // 
//    // if (input_param->letterbox_rows == 0 && input_param->letterbox_cols == 0) {
//    //     img_input = load_image_from_memory(image_start_addr,file_len,img_w,img_h,3) ;
//    //     printf("finish load_image_from_memory\n") ;
//    // } else {
//    //     // process letterbox
//    //     img_input = load_image_from_memory(image_start_addr,file_len, 0, 0, 3) ;
//    //     img_w = input_param->letterbox_cols ;
//    //     img_h = input_param->letterbox_rows ;
//    //     img_c = 3 ; // hardcode here, maybe need one more parameter from user.

//    //     /* letterbox process to support different letterbox size */
//    //     float scale_letterbox = 0 ;
//    //     int resize_rows = 0 ;
//    //     int resize_cols = 0 ;
//    //     if ((input_param->letterbox_rows * 1.0 / img_input.h) < (input_param->letterbox_cols * 1.0 / img_input.w)) {
//    //         scale_letterbox = input_param->letterbox_rows * 1.0 / img_input.h ;
//    //     } else {
//    //         scale_letterbox = input_param->letterbox_cols * 1.0 / img_input.w ;
//    //     }
//    //     resize_cols = (int)(scale_letterbox * img_input.w);
//    //     resize_rows = (int)(scale_letterbox * img_input.h);

//    //     /* resize image */
//    //     image resized = resize_image(img_input, resize_cols, resize_rows);
//    //     free_image(img_input);
//    //     img_input = resized;
//    // 
//    //     // Generate a black image for letterbox 
//    //     image img_new = make_image(img_w, img_h, img_c);
//    //     int top = (img_h - resize_rows) / 2;
//    //     int bot = (img_h - resize_rows + 1) / 2;
//    //     int left = (img_w - resize_cols) / 2;
//    //     int right = (img_w - resize_cols + 1) / 2;
//    //     // Letterbox filling
//    //     for (int c = 0; c < img_c; c++) {
//    //         for (int h = 0; h < img_h; h++) {
//    //             for (int w = 0; w < img_w; w++) {
//    //                 if (!(w < left || h < top || w > (left + resize_cols) || h > (top + resize_rows))) {
//    //                     img_new.data[c*img_h*img_w + h*img_w + w] = img_input.data[c*resize_rows*resize_cols + (h-top)*resize_cols + (w-left)] ;
//    //                 }
//    //             }
//    //         }
//    //     }
//    //     free_image(img_input);
//    //     img_input = img_new;
//    // }
//    // 
//    // float* img_data = img_input.data;
//    // uint8_t* input_data = input_param->output_tensor->addr + FM_BASE_L_ADDR;
//    // if (NULL == input_data) {
//    // 	fprintf(stderr, "Fail to malloc mem for input_data\n") ;
//    // 	return ;
//    // }

//    // if (1 == input_param->focus) {
//    //     // process focus
//    //     for (int i = 0; i < 2; i++) // corresponding to rows
//    //     {
//    //         for (int g = 0; g < 2; g++) // corresponding to cols
//    //         {
//    //             for (int c = 0; c < img_c; c++)
//    //             {
//    //                 for (int h = 0; h < img_h/2; h++)
//    //                 {
//    //                     for (int w = 0; w < img_w/2; w++)
//    //                     {
//    //                         int in_index  = i + g * img_w + c * img_w * img_h +
//    //                                         h * 2 * img_w + w * 2;
//    //                         int out_index = i * 2 * img_c * (img_w/2) * (img_h/2) +
//    //                                         g * img_c * (img_w/2) * (img_h/2) +
//    //                                         c * (img_w/2) * (img_h/2) +
//    //                                         h * (img_w/2) +
//    //                                         w;

//    //                         float input_fp32 = (img_input.data[in_index]*255 - mean[c]) * scale[c];

//    //                         /* quant to int8/uint8 */
//    //                         int udata = (round)(input_fp32 / input_scale + (float)input_zero_point);

//    //                         if (input_param->output_tensor->data_type == NPU_DATA_UINT8) {
//    //                             if (udata > 255)
//    //                                 udata = 255;
//    //                             else if (udata < 0)
//    //                                 udata = 0;
//    //                         } else if (input_param->output_tensor->data_type == NPU_DATA_INT8) {
//    //                             if (udata > 127)
//    //                                 udata = 127;
//    //                             else if (udata < -128)
//    //                                 udata = -128;
//    //                         } else {
//    //                             printf("unsupported data type for input_op: %d", input_param->output_tensor->data_type) ;
//    //                             return -1 ;
//    //                         }

//    //                         input_data[out_index] = udata;
//    //                     }
//    //                 }
//    //             }
//    //         }
//    //     }
//    // } else {
//    //     /* quant data */
//    //     printf("start quant_data\n") ;
//    //     for (int c = 0; c < img_c; c++)
//    //     {
//    //         for (int h = 0; h < img_h; h++)
//    //         {
//    //             for (int w = 0; w < img_w; w++)
//    //             {
//    //                 int idx = c * img_h * img_w + h * img_w + w;
//    //                 float input_fp32 = (img_data[idx]*255 - mean[c]) * scale[c];

//    //                 /* quant to int8/uint8 */
//    //                 int udata = (round)(input_fp32 / input_scale + (float)input_zero_point);
//    //                 if (input_param->output_tensor->data_type == NPU_DATA_UINT8) {
//    //                     if (udata > 255)
//    //                         udata = 255;
//    //                     else if (udata < 0)
//    //                         udata = 0;

//    //                     input_data[idx] = udata;
//    //                 } else if (input_param->output_tensor->data_type == NPU_DATA_INT8) {
//    //                     if (udata > 127)
//    //                         udata = 127;
//    //                     else if (udata < -128)
//    //                         udata = -128;

//    //                     input_data[idx] = udata;
//    //                 } else {
//    //                     printf("unsupported data type for input_op: %d", input_param->output_tensor->data_type) ;
//    //                     return -1 ;
//    //                 }
//    //             }
//    //         }
//    //     }
//    //     printf("finish quant_data\n") ;
//    // }

//    // printf("free img_input") ;
//    // free_image(img_input) ;

//     printf("finish gen_input_data\n") ;

//     return 0 ;

// }

// int8_t load_input_data (npu_input_param_t* input_param) {
    
//     struct stat stat;
//     int fd = open("input_0.bin", O_RDONLY);

//     if (fd < 0)
//     {
//         printf("cannot open file %s\n", "input_0.bin");
//         return -1;
//     }

//     fstat(fd, &stat);

//     int file_len = stat.st_size;

//     extern uint64_t* ts_vaddr_rec;
//     void* mem_base = (void*) ts_vaddr_rec[0];
//     // void* mem_base = (void*) input_param->output_tensor->addr ;
//     // void* mem_base = (void*)malloc(file_len) ;

//     int ret = read(fd, mem_base, file_len);
//     if (ret != file_len) {
//         printf("Read file %s failed, err no: %d\n", "input_0.bin", ret) ;
//         return -1 ;
//     }

//     close(fd) ;
//     return 0 ;
// }

void mem_addr(unsigned long vaddr, unsigned long *paddr)
{

#ifdef RUN_PLATFORM_LINUX

    int pageSize = getpagesize();//调用此函数获取系统设定的页面大小

    unsigned long v_pageIndex = vaddr / pageSize;//计算此虚拟地址相对于0x0的经过的页面数
    unsigned long v_offset = v_pageIndex * sizeof(uint64_t);//计算在/proc/pid/page_map文件中的偏移量
    unsigned long page_offset = vaddr % pageSize;//计算虚拟地址在页面中的偏移量
    uint64_t item = 0;//存储对应项的值

    int fd = open("/proc/self/pagemap", O_RDONLY);
    if(fd == -1)//判断是否打开失败
    {
        printf("open /proc/self/pagemap error\n");
        return;
    }

    if(lseek(fd, v_offset, SEEK_SET) == -1)//将游标移动到相应位置，即对应项的起始地址且判断是否移动失败
    {
        printf("sleek error\n");
        return; 
    }

    if(read(fd, &item, sizeof(uint64_t)) != sizeof(uint64_t))//读取对应项的值，并存入item中，且判断读取数据位数是否正确
    {
        printf("read item error\n");
        return;
    }

    if((((uint64_t)1 << 63) & item) == 0)//判断present是否为0
    {
        printf("page present is 0\n");
        return ;
    }

    uint64_t phy_pageIndex = (((uint64_t)1 << 55) - 1) & item;//计算物理页号，即取item的bit0-54

    *paddr = (phy_pageIndex * pageSize) + page_offset;//再加上页内偏移量就得到了物理地址

#endif

return ;
}


#define MAX 1024

int get_uio_dev_count(char *root)
{
#ifdef RUN_PLATFORM_LINUX
    DIR *dir;
    struct dirent * ptr;
    int total = 0;
    char path[MAX];
    char key[4] = "";
    dir = opendir(root); 
    if(dir == NULL)
    {
        perror("fail to open dir");
        return -1;
    }
    

    errno = 0;
    while((ptr = readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name,"..") == 0)
        {
            continue;
        }
    
        /* ommit dir in root path */
        // if(ptr->d_type == DT_DIR)
        // {
        //     sprintf(path,"%s%s/",root,ptr->d_name);
        //     //printf("%s/n",path);
        //     total += get_file_count(path);
        // }
        
        if(ptr->d_type == DT_CHR)
        {
            strncpy(key,ptr->d_name,3);
            if(strcmp(key, "uio") == 0) {
                total++;
                printf("%s/%s\n",root,ptr->d_name);
            }
        }
    }

    if(errno != 0)
    {
        printf("fail to read dir\n");  
        exit(1);
    }
    closedir(dir);
    return total;

#else

return 0 ;

#endif

}

unsigned long get_sys_counter_val(void) {
    unsigned int sys_cnt_val;
	asm volatile(
		"mrs %0, cntpct_el0"
		: "=r" (sys_cnt_val)
		:
		: "memory");
	return sys_cnt_val;
}
