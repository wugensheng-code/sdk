/*
 * alfsbl_partition_load.h
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#ifndef ALFSBL_PARTITION_LOAD_H_
#define ALFSBL_PARTITION_LOAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_data.h"
#include "alfsbl_hw.h"
#include "alfsbl_err_code.h"
#include "al9000_secure.h"




uint32_t AlFsbl_PartitionLoad(AlFsblInfo *FsblInstancePtr, uint32_t PartitionIdx);


#ifdef __cplusplus
}
#endif


#endif /* ALFSBL_PARTITION_LOAD_H_ */
