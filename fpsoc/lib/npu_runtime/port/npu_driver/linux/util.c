#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#include "util.h"

#define LOG_TAG "NPU_DRIVER_UTIL"
#include "elog.h"

#define MAX_LEN 1024
int get_uio_dev_count(char *root)
{
    DIR *dir;
    struct dirent * ptr;
    int total = 0;
    char path[MAX_LEN];
    char key[4] = "";
    dir = opendir(root); 
    if(dir == NULL)
    {
        log_e("fail to open dir: %s\n", root);
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
                log_d("%s/%s\n",root,ptr->d_name);
            }
        }
    }

    if(errno != 0)
    {
        log_e("fail to get_uio_dev_count\n");  
        return -1;
    }
    closedir(dir);
    return total;
}