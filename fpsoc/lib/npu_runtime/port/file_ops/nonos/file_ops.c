#include <stdio.h>
#include "file_ops.h"
#include "ff.h"

FIL global_fd ;

AL_S8 init_file_system () {

    /* mount sd card */
    TCHAR  *Path = "0:/";
    static FATFS fatfs;
    FRESULT Res = f_mount(&fatfs, Path, 0);
	if (Res != FR_OK) {
		printf("ERROR: f_mount failed\n");
        return -1;
    }

    return 0;

}

AL_S64 get_file_size(const char* file_path) {

    FILINFO fno;
	FIL fd;
	int ret = f_open(&fd, file_path, FA_READ);
	if (ret) {
		printf("ERROR: Fail to open file %s, err code: %d\n", file_path, ret);
		return -1;
	}

	f_stat(file_path, &fno);
	AL_S64 file_len = fno.fsize;

    ret = f_close(&fd);
	if (ret) {
		printf("ERROR: Fail to close file %s, err no: %d\n", file_path, ret);
		return -1;
	}

    return file_len;

}

AL_S8 read_file(const char* file_path, AL_VOID* dst_addr, AL_S64 rd_len) {
	FIL fd;
	int ret = f_open(&fd, file_path, FA_READ);
	if (ret) {
		printf("ERROR: Fail to open file %s, err code: %d\n", file_path, ret);
		return -1;
	}

    UINT br = 0;
    ret = f_read(&fd, dst_addr, rd_len, &br);
	if (ret) {
		printf("ERROR: Read file failed: %s, err no: %d\n", file_path, ret);
		return -1;
	}
	if (rd_len != br) {
		printf("ERROR: Fail to read expected length from file %s, expected length: %lu, actual length: %lu\n", file_path, rd_len, (AL_U64)br);
		return -1;
	}

	ret = f_close(&fd);
	if (ret) {
		printf("ERROR: Fail to close file %s, err no: %d\n", file_path, ret);
		return -1;
	}

    return 0;
}

AL_S8 write_file(const char* file_path, AL_VOID* src_addr, AL_S64 wr_len) {
	FIL fd;
	int ret = f_open(&fd, file_path, FA_CREATE_ALWAYS | FA_WRITE);
	if (ret) {
		printf("ERROR: Fail to open file %s, err code: %d\n", file_path, ret);
		return -1;
	}

    int num_wr = 0;
    f_write (&fd, src_addr, wr_len, &num_wr);

    if(num_wr != wr_len) {
       printf("ERROR: write failed: size: %d\t num_wr: %d\n", wr_len, num_wr);
       return -1;
    }

    ret = f_close(&fd);
	if (ret) {
		printf("ERROR: Fail to close file %s, err no: %d\n", file_path, ret);
		return -1;
	}

    return 0;

}


AL_VOID* get_wr_fd(const char* file_path) {
	int ret = f_open(&global_fd, file_path, FA_CREATE_ALWAYS | FA_WRITE);
	if (ret) {
		printf("ERROR: Fail to open file %s, err code: %d\n", file_path, ret);
		return NULL;
	}

    return (AL_VOID*)(&global_fd);
}

AL_VOID write_func(AL_VOID *context, AL_VOID *data, int size) {
    FIL* fp = (FIL*)(context) ;
    int num_wr = 0 ;
    FRESULT res = f_write(fp, data, size, &num_wr) ;
    if (size != num_wr) {
        printf("ERROR: Write failed in stbi_write_func_sd, size: %d num_wr: %d\n", size, num_wr) ;
    }

    return;
}

AL_S8 close_fp(AL_VOID* context) {
    FIL* fp = (FIL*)(context) ;
    AL_S8 ret = f_close(fp) ;
    if (ret) {
        printf("ERROR: cannot close fp\n") ;
        return -1 ;
    }

    return 0;
}