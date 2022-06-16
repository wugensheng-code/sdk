/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"
//#include "sdio/bsp_sdio_sd.h"
#include "string.h"
#include "sdio_test.h"
#include <stdio.h>

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
			if(SD_Init() == XST_SUCCESS)
			{
				status &= ~STA_NOINIT;
			}
			else 
			{
				status = STA_NOINIT;
			}
		
			break;
    
		case EMMC:    /* EMMC */ 
            if(EMMC_Init() == XST_SUCCESS)
			{
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
	u32 SD_state = XST_SUCCESS;
	
	switch (pdrv) {
		case ATA:	/* SD CARD */
#if 0
		  if((DWORD)buff&3)
		 {
				DRESULT res = RES_OK;
				DWORD scratch[SDCardInfo.CardBlockSize / 4];

				while (count--) 
				{
					res = disk_read(ATA,(void *)scratch, sector++, 1);

					if (res != RES_OK) 
					{
						break;
					}
					memcpy(buff, scratch, SDCardInfo.CardBlockSize);
					buff += SDCardInfo.CardBlockSize;
		        }
		        return res;
		}
#endif

		SD_state=SD_ReadMultiBlocks((uint8_t *)buff,sector,SDCardInfo.CardBlockSize,count);
		if(SD_state == XST_SUCCESS)
		{
			/* Check if the Transfer is finished */
			//SD_state=SD_WaitReadOperation();
		}
		if(SD_state != XST_SUCCESS)
			status = RES_PARERR;
		else
			status = RES_OK;	
		break;   
			
		case EMMC:
            SD_state=EMMC_ReadMultiBlocks((uint8_t *)buff,sector,SDCardInfo.CardBlockSize,count);
            if(SD_state == XST_SUCCESS)
		    {
			/* Check if the Transfer is finished */
			//SD_state=SD_WaitReadOperation();
		    }
		    if(SD_state != XST_SUCCESS)
			    status = RES_PARERR;
		    else
			    status = RES_OK;	
            
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
	DRESULT status = RES_PARERR;
    u32 SD_state = XST_SUCCESS;
	
	if (!count) {
		return RES_PARERR;		/* Check parameter */
	}

	switch (pdrv) {
		case ATA:	/* SD CARD */  
#if 0
			if((DWORD)buff&3)
			{
				DRESULT res = RES_OK;
				DWORD scratch[SDCardInfo.CardBlockSize / 4];

				while (count--) 
				{
					memcpy( scratch,buff,SDCardInfo.CardBlockSize);
					res = disk_write(ATA,(void *)scratch, sector++, 1);
					if (res != RES_OK) 
					{
						break;
					}					
					buff += SDCardInfo.CardBlockSize;
		    }
		    return res;
			}		
#endif
			SD_state=SD_WriteMultiBlocks((uint8_t *)buff,sector,SDCardInfo.CardBlockSize,count);
			if(SD_state == XST_SUCCESS)
			{
				/* Check if the Transfer is finished */
			}
			if(SD_state != XST_SUCCESS)
				status = RES_PARERR;
		    else
			  status = RES_OK;	
		break;

		case EMMC:
            SD_state=EMMC_WriteMultiBlocks((uint8_t *)buff,sector,SDCardInfo.CardBlockSize,count);
			if(SD_state == XST_SUCCESS)
			{
				/* Check if the Transfer is finished */
			}
			if(SD_state != XST_SUCCESS)
				status = RES_PARERR;
		    else
			  status = RES_OK;	
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
    
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif
typedef unsigned long	DWORD;
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
