#include "utils.h"

double what_time_is_it_now(){}
int *read_intlist(char *gpu_list, int *ngpus, int d){}
int *read_map(char *filename){}
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections){}
void shuffle(void *arr, size_t n, size_t size){}
int *random_index_order(int min, int max){}
void del_arg(int argc, char **argv, int index){}
int find_arg(int argc, char* argv[], char *arg){}
int find_int_arg(int argc, char **argv, char *arg, int def){}
float find_float_arg(int argc, char **argv, char *arg, float def){}
char *find_char_arg(int argc, char **argv, char *arg, char *def){}
char *basecfg(char *cfgfile){}
int alphanum_to_int(char c){}
char int_to_alphanum(int i){}
void pm(int M, int N, float *A){}
void find_replace(char *str, char *orig, char *rep, char *output){}
float sec(clock_t clocks){}
void top_k(float *a, int n, int k, int *index){}
void error(const char *s){}
unsigned char *read_file(char *filename){}
void malloc_error(){}
void file_error(char *s){}
list *split_str(char *s, char delim){}
void strip(char *s){}
void strip_char(char *s, char bad){}
void free_ptrs(void **ptrs, int n){}
char *fgetl(FILE *fp){}
int read_int(int fd){}
void write_int(int fd, int n){}
int read_all_fail(int fd, char *buffer, size_t bytes){}
int write_all_fail(int fd, char *buffer, size_t bytes){}
void read_all(int fd, char *buffer, size_t bytes){}
void write_all(int fd, char *buffer, size_t bytes){}
char *copy_string(char *s){}
list *parse_csv_line(char *line){}
int count_fields(char *line){}
float *parse_fields(char *line, int n){}
float sum_array(float *a, int n){}
float mean_array(float *a, int n){}
void mean_arrays(float **a, int n, int els, float *avg){}
void print_statistics(float *a, int n){}
float variance_array(float *a, int n){}
int constrain_int(int a, int min, int max){}
float constrain(float min, float max, float a){}
float dist_array(float *a, float *b, int n, int sub){}
float mse_array(float *a, int n){}
void normalize_array(float *a, int n){}
void translate_array(float *a, int n, float s){}
float mag_array(float *a, int n){}
void scale_array(float *a, int n, float s){}
int sample_array(float *a, int n){}
int max_int_index(int *a, int n){}
int max_index(float *a, int n){}
int int_index(int *a, int val, int n){}
int rand_int(int min, int max){}
float rand_normal(){}
size_t rand_size_t(){}
float rand_uniform(float min, float max){}
float rand_scale(float s){}
float **one_hot_encode(float *a, int n, int k){}
unsigned char * stbi_load(char* filename, int* w, int* h, int* c, int channels);