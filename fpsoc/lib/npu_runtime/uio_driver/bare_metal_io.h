#ifndef __BARE_METAL_IO_H__
#define __BARE_METAL_IO_H__

void hard_npu_write(uint32_t offset, uint32_t value);

uint32_t hard_npu_read(uint32_t offset);

void soft_npu_write(uint32_t offset, uint32_t value);

uint32_t soft_npu_read(uint32_t offset);

#endif
