/*
 * Copyright (c) 2014, Mentor Graphics Corporation
 * All rights reserved.
 * Copyright (c) 2015-2022 Xilinx, Inc. All rights reserved.
 * Copyright (c) 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
 * Copyright (c) 2023 Anlogic, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * This file populates resource table for BM remote
 * for use by the Linux host
 */

#include <openamp/open_amp.h>
#include "rsc_table.h"
#include "platform_info.h"

/* Place resource table in special ELF section */
/* Redefine __section for section name with token */
#define __section_t(S)          __attribute__((__section__(#S)))
#define __resource              __section_t(.resource_table)

#define RPMSG_VDEV_DFEATURES        (1 << VIRTIO_RPMSG_F_NS)

/* VirtIO rpmsg device id */
#define VIRTIO_ID_RPMSG_             7

/* Resource table entries */
#define NUM_VRINGS                  0x02
#define VRING_ALIGN                 0x1000

#if BOARD_SEL == 0
/* board_dr1x90_emulation */
#define RING_TX                     0x61030000
#define RING_RX                     0x61034000
#else
/* board_dr1x90_ad101_v10 */
#define RING_TX                     0xf030000
#define RING_RX                     0xf034000
#endif

#define VRING_BD_NUM                128 /* FROM linux DTS, num * PAGE_ALIGN(vring_size)*/

#define NUM_TABLE_ENTRIES           1

struct remote_resource_table __resource resources = {
	/* Version */
	1,

	/* NUmber of table entries */
	NUM_TABLE_ENTRIES,
	/* reserved fields */
	{0, 0,},

	/* Offsets of rsc entries */
	{
	 offsetof(struct remote_resource_table, rpmsg_vdev),
	},

	/* Virtio device entry */
	{
	 RSC_VDEV, VIRTIO_ID_RPMSG_, 31, RPMSG_VDEV_DFEATURES, 0, 0, 0,
	 NUM_VRINGS, {0, 0},
	},

	/* Vring rsc entry - part of vdev rsc entry */
	{RING_TX, VRING_ALIGN, VRING_BD_NUM, 1, 0},
	{RING_RX, VRING_ALIGN, VRING_BD_NUM, 2, 0},
};

void *get_resource_table (int rsc_id, int *len)
{
	(void) rsc_id;
	*len = sizeof(resources);
	return &resources;
}
