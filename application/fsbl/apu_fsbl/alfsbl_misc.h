/*
 * alfsbl_misc.h
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#ifndef ALFSBL_MISC_H_
#define ALFSBL_MISC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void *AlFsbl_MemCpy(void * DestPtr, const void * SrcPtr, uint32_t Len);



#ifdef __cplusplus
}
#endif

#endif /* ALFSBL_MISC_H_ */
