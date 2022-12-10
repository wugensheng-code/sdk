/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include <stdio.h>
#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"
#include "string.h"
#include "al_sd.h"
#include "al_sd_write.h"
#include "al_emmc.h"
#include "al_emmc_write.h"


/* Ϊÿ���豸����һ�������� */
#define ATA			           0     // SD��
#define EMMC		       1     // Ԥ���ⲿSPI Flashʹ��

//#define SD_BLOCKSIZE     512

/*-----------------------------------------------------------------------*/
/* ��ȡ�豸״̬                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* ������ */
)
{
	DSTATUS status = STA_NOINIT;
	
	switch (pdrv) {
		case ATA:	/* SD CARD */
			status &= ~STA_NOINIT;
			break;
    
		case EMMC:        /* SPI Flash */   
            status &= ~STA_NOINIT;
			break;

		default:
			status = STA_NOINIT;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* �豸��ʼ��                                                            */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* ������ */
)
{
	DSTATUS status = STA_NOINIT;	
	switch (pdrv) {
		case ATA:	         /* SD CARD */
			if(AlSd_Init() == MMC_SUCCESS)
			{
				//Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
				status &= ~STA_NOINIT;
			}
			else 
			{
				status = STA_NOINIT;
			}
		
			break;
    
		case EMMC:    /* EMMC */ 
            if(AlEmmc_Init() == MMC_SUCCESS)
			{
				//Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
				status &= ~STA_NOINIT;
			}
			else 
			{
				status = STA_NOINIT;
			}
			break;
      
		default:
			status = STA_NOINIT;
	}
	return status;
}


/*-----------------------------------------------------------------------*/
/* ����������ȡ�������ݵ�ָ���洢��                                              */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* �豸������(0..) */
	BYTE *buff,		/* ���ݻ����� */
	DWORD sector,	/* �����׵�ַ */
	UINT count		/* ��������(1..128) */
)
{
	DRESULT status = RES_PARERR;
	uint32_t blockcount = 0;
	
	switch (pdrv) {
		case ATA:	/* SD CARD */
			for(blockcount = 0; blockcount < count; blockcount++){
				status = AlSd_ReadSingleBlock((uint8_t *)(buff+SDCardInfo.CardBlockSize*blockcount),sector+blockcount,SDCardInfo.CardBlockSize);
				if(status != RES_OK)
				{
					/* Check if the Transfer is finished */
					status = RES_PARERR;
					return status;
				}
			}
			break;   
			
		case EMMC:
            for(blockcount = 0; blockcount < count; blockcount++){
				status = AlEmmc_ReadSingleBlock((uint8_t *)(buff+EmmcCardInfo.CardBlockSize*blockcount),sector+blockcount,EmmcCardInfo.CardBlockSize);
				if(status != RES_OK)
				{
					/* Check if the Transfer is finished */
					status = RES_PARERR;
					return status;
				}
			}
			break;
		default:
			status = RES_PARERR;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* д������������д��ָ�������ռ���                                      */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			  /* �豸������(0..) */
	const BYTE *buff,	/* ��д�����ݵĻ����� */
	DWORD sector,		  /* �����׵�ַ */
	UINT count			  /* ��������(1..128) */
)
{
	DRESULT status = RES_OK;
	uint32_t blockcount = 0;
	
	if (!count) {
		return RES_PARERR;		/* Check parameter */
	}

	switch (pdrv) {
		case ATA:	/* SD CARD */  
			for(blockcount = 0; blockcount < count; blockcount++){
				status = AlSd_WriteSingleBlock((uint8_t *)(buff+SDCardInfo.CardBlockSize*blockcount),sector+blockcount,SDCardInfo.CardBlockSize);
				if(status != RES_OK)
				{
					/* Check if the Transfer is finished */
					status = RES_PARERR;
					return status;
				}
			}
			break;
		case EMMC:
            for(blockcount = 0; blockcount < count; blockcount++){
				status = AlEmmc_WriteSingleBlock((uint8_t *)(buff+EmmcCardInfo.CardBlockSize*blockcount),sector+blockcount,EmmcCardInfo.CardBlockSize);
				if(status != RES_OK)
				{
					/* Check if the Transfer is finished */
					status = RES_PARERR;
					return status;
				}
			}
			break;
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif


/*-----------------------------------------------------------------------*/
/* ��������                                                              */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* ������ */
	BYTE cmd,		  /* ����ָ�� */
	void *buff		/* д����߶�ȡ���ݵ�ַָ�� */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			switch (cmd) 
			{
				// Get R/W sector size (WORD) 
				case GET_SECTOR_SIZE :    
					*(WORD * )buff = SDCardInfo.CardBlockSize;
				break;
				// Get erase block size in unit of sector (DWORD)
				case GET_BLOCK_SIZE :      
					*(DWORD * )buff = 1;
				break;

				case GET_SECTOR_COUNT:
					//*(DWORD * )buff = 1000;
					*(DWORD * )buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
					break;
				case CTRL_SYNC :
				break;
			}
			status = RES_OK;
			break;
    
		case EMMC:
            switch (cmd) 
			{
				// Get R/W sector size (WORD) 
				case GET_SECTOR_SIZE :    
					*(WORD * )buff = EmmcCardInfo.CardBlockSize;
				break;
				// Get erase block size in unit of sector (DWORD)
				case GET_BLOCK_SIZE :      
					*(DWORD * )buff = 1;
				break;

				case GET_SECTOR_COUNT:
					//*(DWORD * )buff = 1000;
					*(DWORD * )buff = EmmcCardInfo.CardCapacity/EmmcCardInfo.CardBlockSize;
					break;
				case CTRL_SYNC :
				break;
			}
			status = RES_OK;
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif
//typedef unsigned long	DWORD;
#if 1
DWORD get_fattime(void) {
	/* ���ص�ǰʱ��� */
	return	  ((DWORD)(2015 - 1980) << 25)	/* Year 2015 */
			| ((DWORD)1 << 21)				/* Month 1 */
			| ((DWORD)1 << 16)				/* Mday 1 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				  /* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}
#endif
