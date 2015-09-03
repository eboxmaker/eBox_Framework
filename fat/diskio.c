/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "stm32f10x.h"
#include "wrapperdiskio.h"

//extern u8 SD_Init();

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */

#define MMC		0
#define ATA		1
#define USB		2




/*-----------------------------------------------------------------------*/
/* 存储器初始化                                                    */

DSTATUS disk_initialize (
	                  BYTE drv  /* Physical drive nmuber (0..) */
                        )
{
  DSTATUS result;
  switch (drv) 
  {
    case ATA :result = ATA_disk_initialize();return result;
    case MMC :result = SD_disk_initialize();return result;
    case USB :result = USB_disk_initialize();return result;
  }
  return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* 存储器状态                                                    */

DSTATUS disk_status (
	              BYTE drv		/* Physical drive nmuber (0..) */
                    )
{
  DSTATUS result;
   
  switch (drv) 
  {
    case ATA :result = ATA_disk_status();return result;
    case MMC :result = SD_disk_status();return result;
    case USB :result = USB_disk_status();return result;
  }
  return STA_NOINIT;
}



/*-------------------------------------*/
/* 读扇区（或多个扇区）数据 */

DRESULT disk_read (
	            BYTE drv,	/* Physical drive nmuber (0..) */
	            BYTE *buff,	/* Data buffer to store read data */
	            DWORD sector,	/* Sector address (LBA) */
	            BYTE count	/* Number of sectors to read (1..255) */
                  )
{
  DRESULT result;

  switch (drv)
  {
    case ATA :result = ATA_disk_read(sector,  buff,  count);return result;
    case MMC :result = MMC_disk_read(sector,  buff,  count);return result;
    case USB :result = USB_disk_read(sector,  buff,  count);return result;
  }
  return RES_PARERR;
}



/*--------------------------------------------*/
/* 读扇区（或多个扇区）数据 */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,		/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count		/* Number of sectors to write (1..255) */
)
{
  DRESULT result;
  switch (drv) 
  {
    case ATA :result = ATA_disk_write(sector, buff, count);return result;
    case MMC :result = MMC_disk_write(sector, buff, count);return result;
    case USB :result = USB_disk_write(sector, buff, count);return result;
  }
  return RES_PARERR;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	              BYTE drv,		/* Physical drive nmuber (0..) */
	              BYTE ctrl,	/* Control code */
	              void *buff	/* Buffer to send/receive control data */
                    )
{
  DRESULT result;
  switch (drv) 
  {
    case ATA :result = ATA_disk_ioctl(ctrl, buff);return result;
    case MMC :result = MMC_disk_ioctl(ctrl, buff);return result;
    case USB :result = USB_disk_ioctl(ctrl, buff);return result;
  }
  return RES_PARERR;
}

DSTATUS ATA_disk_initialize(void)
{
  return STA_NOINIT;
}

DSTATUS SD_disk_initialize(void)
{
  return(SD_Init());
}

DSTATUS USB_disk_initialize(void)
{
  return STA_NOINIT;
}

DSTATUS ATA_disk_status(void)//默认状态正常
{
  return 0;
}

DSTATUS SD_disk_status(void)//默认状态正常
{
  return 0;
}

DSTATUS USB_disk_status(void)//默认状态正常
{
  return 0;
}

DRESULT ATA_disk_read(u32 sector, u8 *buff, u8 count)
{
  return RES_OK;
}

DRESULT MMC_disk_read(u32 sector, u8 *buff, u8 count)
{
  return ((DRESULT)(SD_ReadMultiBlock(sector,buff,count)));
}
DRESULT USB_disk_read(u32 sector, u8 *buff, u8 count)
{
  return RES_OK;
}

DRESULT ATA_disk_write(u32 sector, const u8 *buff, u8 count)
{
  return RES_OK;
}
DRESULT MMC_disk_write(u32 sector, const u8 *buff, u8 count)
{
  return ((DRESULT)(SD_WriteMultiBlock(sector,buff,count)));
}

DRESULT USB_disk_write(u32 sector, const u8 *buff, u8 count)
{
  return RES_OK;
}

DRESULT ATA_disk_ioctl(BYTE ctrl, void *buff)
{
  return RES_OK;
}
DRESULT MMC_disk_ioctl(BYTE ctrl, void *buff)
{
  return RES_OK;
}
DRESULT USB_disk_ioctl(BYTE ctrl, void *buff)
{
  return RES_OK;
}

