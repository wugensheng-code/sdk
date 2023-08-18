
# define SLL32                  sllw
# define STORE                  sd
# define LOAD                   ld
# define LWU                    lwu
# define LOG_REGBYTES           3

#define REGBYTES                (1 << LOG_REGBYTES)

#define __STR(s)                #s
#define STRINGIFY(s)            __STR(s)