/****************************************************************************/
/**
*
* @file xdmaps_hw.c
*
* This file contains the implementation of the interface reset functionality 
*	for XDmaPs driver.
*****************************************************************************/
#include "xdmaps_hw.h"
#include "xdmaps.h"

#ifndef XDMAPS_MAX_WAIT
#define XDMAPS_MAX_WAIT 4000
#endif

//#define __NONSECURE  // non-secure addr

/*****************************************************************************/
/**
* This function perform the reset sequence to the given dmaps interface by 
* configuring the appropriate control bits in the dmaps specifc registers
* the dmaps reset squence involves the following steps
*	Disable all the interuupts 
*	Clear the pending interrupts
*	Kill all the active channel threads
*	Kill the manager thread
*
* @param   BaseAddress of the interface
*
* @return N/A
*
* @note 
* This function will not modify the slcr registers that are relavant for 
* dmaps controller
******************************************************************************/
void XDmaPs_ResetHw(u32 BaseAddress)
{
	u32 DbgInst;
	u32 WaitCount = 0;
	u32 ChanIndex;

	/* Disable all the interrupts */
	XDmaPs_WriteReg(BaseAddress, XDMAPS_INTEN_OFFSET, 0x00);
	/* Clear the interrupts */
	XDmaPs_WriteReg(BaseAddress, XDMAPS_INTCLR_OFFSET, XDMAPS_INTCLR_ALL_MASK);
	/* Kill the dma channel threads */
	for (ChanIndex=0; ChanIndex < XDMAPS_CHANNELS_PER_DEV; ChanIndex++) {
		while ((XDmaPs_ReadReg(BaseAddress, XDMAPS_DBGSTATUS_OFFSET)
				& XDMAPS_DBGSTATUS_BUSY)
				&& (WaitCount < XDMAPS_MAX_WAIT))
				WaitCount++;

		DbgInst = XDmaPs_DBGINST0(0, 0x01, ChanIndex, 1);	
		XDmaPs_WriteReg(BaseAddress, XDMAPS_DBGINST0_OFFSET, DbgInst);
		XDmaPs_WriteReg(BaseAddress, XDMAPS_DBGINST1_OFFSET, 0x0);	
		XDmaPs_WriteReg(BaseAddress, XDMAPS_DBGCMD_OFFSET, 0x0);
	}	
	/* Kill the manager thread	*/
	DbgInst = XDmaPs_DBGINST0(0, 0x01, 0, 0);	
	XDmaPs_WriteReg(BaseAddress, XDMAPS_DBGINST0_OFFSET, DbgInst);
	XDmaPs_WriteReg(BaseAddress, XDMAPS_DBGINST1_OFFSET, 0x0);	
	XDmaPs_WriteReg(BaseAddress, XDMAPS_DBGCMD_OFFSET, 0x0);	
}


#define XPAR_XDMAPS_0_DEVICE_ID (0)

//secure: 0xF8418000
//non-secure: 0xF8419000

#ifdef __NONSECURE
#define XPAR_XDMAPS_0_BASEADDR  0xF8419000 //NS
#else
#define XPAR_XDMAPS_0_BASEADDR  (0xF8418000) //secure
#endif

/**
 * Each XDmaPs device in the system has an entry in this table.
 */
XDmaPs_Config XDmaPs_ConfigTable[] = {
	{
		XPAR_XDMAPS_0_DEVICE_ID,
		XPAR_XDMAPS_0_BASEADDR,
	},
};

/****************************************************************************/
/**
*
* Looks up the device configuration based on the unique device ID. The table
* contains the configuration info for each device in the system.
*
*  The implementation of the XDmaPs driver's static initialzation
*   functionality.
*
* @param DeviceId contains the ID of the device
*
* @return
*
* A pointer to the configuration structure or NULL if the specified device
* is not in the system.
*
******************************************************************************/
#define XPAR_XDMAPS_NUM_INSTANCES (1)
XDmaPs_Config *XDmaPs_LookupConfig(u16 DeviceId)
{
	XDmaPs_Config *CfgPtr = NULL;

	int i;

	for (i = 0; i < XPAR_XDMAPS_NUM_INSTANCES; i++) {
		if (XDmaPs_ConfigTable[i].DeviceId == DeviceId) {
			CfgPtr = &XDmaPs_ConfigTable[i];
			break;
		}
	}

	return CfgPtr;
}
