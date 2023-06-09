#ifndef __NPU_RUNTIME_FILE_OPS_H__
#define __NPU_RUNTIME_FILE_OPS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "al_type.h"

AL_S8 init_file_system ();

AL_S64 get_file_size(const char* file_path);

AL_S8 read_file(const char* file_path, AL_VOID* dst_addr, AL_S64 rd_len);

AL_S8 write_file(const char* file_path, AL_VOID* src_addr, AL_S64 wr_len);


/* for stbi lib or separate write */
AL_VOID* get_wr_fd(const char* file_path);

AL_VOID write_func(AL_VOID *context, AL_VOID *data, int size);

AL_S8 close_fp(AL_VOID* context);


#ifdef __cplusplus
}
#endif


#endif