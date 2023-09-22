/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include <stdio.h>
#include "diskio.h"
#include "ff.h"
#include "string.h"
#include "al_mmc_hal.h"
#include "al_hal.h"

#define SD      0
#define EMMC    1
#define USB     2


#ifdef HAVE_USBPS_DRIVER

extern int USB_disk_status(void);
extern int USB_disk_initialize(void);
extern int USB_disk_read(BYTE *buff, LBA_t sector, UINT count);
extern int USB_disk_write(const BYTE *buff, LBA_t sector, UINT count);
extern int USB_disk_ioctl(BYTE cmd, void *buff);

#endif

#define MMC_RD_WR_TIMEOUT_MS    10000

static AL_MMC_HalStruct *Handle;
static DevId = 0;
static AL_MMC_InitStruct InitConfig = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_SDMA,
    .FreqKhz            = AL_MMC_FREQ_KHZ_50000,
    .SpdMode            = AL_MMC_SPD_HS_SDR25,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_32K
};

DSTATUS disk_status(BYTE pdrv)
{
    DSTATUS status = STA_NOINIT;

    switch (pdrv) {
    case SD:
        status &= ~STA_NOINIT;
        break;
    case EMMC:
        status &= ~STA_NOINIT;
        break;
    case USB:
        status &= ~STA_NOINIT;
        break;
    default:
        status = STA_NOINIT;
        break;
    }

    return status;
}

DSTATUS disk_initialize (BYTE pdrv)
{
    DSTATUS status = STA_NOINIT;
    switch (pdrv) {
    case SD:
    case EMMC:
        status = AlMmc_Hal_Init(&Handle, DevId, &InitConfig, AL_NULL);
        if (status == RES_OK) {
            status = RES_OK;
        } else {
            status = STA_NOINIT;
        }
        break;
    case USB:
#ifdef HAVE_USBPS_DRIVER
        status = USB_disk_initialize();
#endif
        break;
    default:
        status = STA_NOINIT;
        break;
    }

    return status;
}

DRESULT disk_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    DRESULT status = RES_PARERR;
    switch (pdrv) {
    case SD:
    case EMMC:
        status = AlMmc_Hal_ReadBlocked(Handle, buff, sector, count, MMC_RD_WR_TIMEOUT_MS);
        if (status != RES_OK) {
            status = RES_ERROR;
        }
        break;
    case USB:
#ifdef HAVE_USBPS_DRIVER
        status = USB_disk_read(buff, sector, count);
#endif
        break;
    default:
        status = RES_PARERR;
        break;
    }

    return status;
}

#if _USE_WRITE
DRESULT disk_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    DRESULT status = RES_OK;
    if (!count) {
        return RES_PARERR;
    }

    switch (pdrv) {
    case SD:
    case EMMC:
        status = AlMmc_Hal_WriteBlocked(Handle, buff, sector, count, MMC_RD_WR_TIMEOUT_MS);
        if (status != RES_OK) {
            status = RES_ERROR;
        }
        break;
    case USB:
#ifdef HAVE_USBPS_DRIVER
        status = USB_disk_write(buff, sector, count);
#endif
        break;
    default:
        status = RES_PARERR;
        break;
    }
    return status;
}
#endif

#if _USE_IOCTL
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void *buff)
{
    DRESULT status = RES_PARERR;
    switch (pdrv) {
    case SD:
    case EMMC:
        switch (cmd)
        {
        case GET_SECTOR_SIZE :
            // Get R/W sector size (WORD)
            *(DWORD *)buff = Handle->Dev.CardInfo.BlkLen;
            break;
        case GET_BLOCK_SIZE :
            // Get erase block size in unit of sector (DWORD)
            *(DWORD *)buff = 1;
            break;
        case GET_SECTOR_COUNT:
            //*(DWORD * )buff = 1000;
            *(DWORD *)buff = Handle->Dev.CardInfo.CardCap * 1024 / Handle->Dev.CardInfo.BlkLen;
            break;
        case CTRL_SYNC :
            break;
        default:
            break;
        }
        status = RES_OK;
        break;
    case USB:
#ifdef HAVE_USBPS_DRIVER
        status = USB_disk_ioctl(cmd,buff);
#endif
        break;
    default:
        status = RES_PARERR;
        break;
    }
    return status;
}
#endif

DWORD get_fattime(void) {
    return ((DWORD)(2015 - 1980) << 25)    /* Year 2015 */
         | ((DWORD)1 << 21)                /* Month 1 */
         | ((DWORD)1 << 16)                /* Mday 1 */
         | ((DWORD)0 << 11)                /* Hour 0 */
         | ((DWORD)0 << 5)                  /* Min 0 */
         | ((DWORD)0 >> 1);                /* Sec 0 */
}
