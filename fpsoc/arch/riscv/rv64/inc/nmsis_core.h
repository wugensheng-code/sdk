/*
 * Copyright (c) 2009-2019 Arm Limited. All rights reserved.
 * -- Adaptable modifications made for Nuclei Processors. --
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __NMSIS_CORE_H__
#define __NMSIS_CORE_H__

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif


#include "al_compiler.h"     /* NMSIS compiler specific defines */

/* === Include Nuclei Core Related Headers === */
/* Include core base feature header file */
#include "core_feature_base.h"


/* Include core pmp feature header file */
//#include "core_feature_pmp.h"


#ifdef __cplusplus
}
#endif
#endif /* __NMSIS_CORE_H__ */
