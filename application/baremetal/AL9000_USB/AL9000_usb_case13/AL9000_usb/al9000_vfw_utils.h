
#include "al9000_registers.h"


// define macros for verification firmware printing
#define VFW_BRIDGE_INST_OFFSET0 TOP_NS__RWL0_RESERVED__ADDR
#define VFW_BRIDGE_DATA_OFFSET0 TOP_NS__RWL1_RESERVED__ADDR
#define VFW_BRIDGE_INST_OFFSET1 TOP_NS__RWH_RESERVED__ADDR
#define VFW_BRIDGE_DATA_OFFSET1 TOP_NS__RO_RESERVED__ADDR

#define VFW_BRIDGE_INST_CODE_PRINT_START 0x00101230
#define VFW_BRIDGE_INST_CODE_PRINT_END   0x1100abcf
#define VFW_BRIDGE_INST_CODE_SUCCESS     0x600d600d
#define VFW_BRIDGE_INST_CODE_FAIL        0xbad00bad

//#define vfwp(...) vfw_printf(__LINE__, __VA_ARGS__)
//#define vfwp(...)

#define REG_WRITE(reg_address, reg_wdata)  *(unsigned int*)(reg_address) = reg_wdata

#define REG_READ(reg_address)  *(unsigned int*)reg_address
