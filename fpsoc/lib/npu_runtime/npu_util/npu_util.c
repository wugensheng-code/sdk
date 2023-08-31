

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

#include "npu_util.h"
#include "npu_defines.h"
#include "file_ops.h"
#include "al_core.h"


#define LOG_TAG "NPU_UTIL"
#include "elog.h"

AL_VOID dump_tensor (rt_tensor_t* t) {

    char name[30];
    // sprintf(name, "debug/%s", t->name);
    sprintf(name, "%s", t->name);

    extern AL_U64* ts_vaddr_rec;
    AL_VOID* tensor_buf = ts_vaddr_rec[t->index] ;
    if (write_file(name, tensor_buf, (AL_S64)(t->param->size))) {
        log_e("write file: %s failed\n", name);
        return;
    }

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

unsigned long get_sys_counter_val(AL_VOID) {
    return AlSys_GetTimerTickCount();
}
