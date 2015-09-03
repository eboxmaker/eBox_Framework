/*-----------------------------------------------------------------------
/  Low level disk interface modlue include file  R0.07   (C)ChaN, 2009
/-----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"{
#endif
#ifndef _DISKIO

#define _READONLY	0	/* 1: Read-only mode */
#define _USE_IOCTL	1

#include "integer.h"
#include "stm32f10x.h"


#ifndef __cplusplus
typedef enum {TRUE = 0, FALSE = !TRUE} bool;
#endif

/* Status of Disk Functions */
typedef BYTE  DSTATUS;

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Successful */
	RES_ERROR,		/* 1: R/W Error */
	RES_WRPRT,		/* 2: Write Protected */
	RES_NOTRDY,		/* 3: Not Ready */
	RES_PARERR		/* 4: Invalid Parameter */
} DRESULT;


/*---------------------------------------*/
/* Prototypes for disk control functions */


bool assign_drives (int argc, char *argv[]);
DSTATUS disk_initialize (BYTE);
DSTATUS disk_status (BYTE);
DRESULT disk_read (BYTE, BYTE*, DWORD, BYTE);
#if	_READONLY == 0
DRESULT disk_write (BYTE, const BYTE*, DWORD, BYTE);
#endif
DRESULT disk_ioctl (BYTE, BYTE, void*);

/*----------Low Layer I/O-------------*/
DSTATUS USB_disk_initialize(void);
DSTATUS ATA_disk_initialize(void);
DSTATUS SD_disk_initialize(void);

DSTATUS ATA_disk_status(void);
DSTATUS SD_disk_status(void);
DSTATUS USB_disk_status(void);

DRESULT ATA_disk_read(u32 sector, u8 *buff, u8 count);
DRESULT MMC_disk_read(u32 sector, u8 *buff, u8 count);
DRESULT USB_disk_read(u32 sector, u8 *buff, u8 count);

DRESULT ATA_disk_write(u32 sector, const u8 *buff, u8 count);
DRESULT MMC_disk_write(u32 sector, const u8 *buff, u8 count);
DRESULT USB_disk_write(u32 sector, const u8 *buff, u8 count);

DRESULT ATA_disk_ioctl(BYTE ctrl, void *buff);
DRESULT MMC_disk_ioctl(BYTE ctrl, void *buff);
DRESULT USB_disk_ioctl(BYTE ctrl, void *buff);

/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */


/* Command code for disk_ioctrl() */

/* Generic command */
#define CTRL_SYNC		0	/* Mandatory for write functions */
#define GET_SECTOR_COUNT	1	/* Mandatory for only f_mkfs() */
#define GET_SECTOR_SIZE		2
#define GET_BLOCK_SIZE		3	/* Mandatory for only f_mkfs() */
#define CTRL_POWER		4
#define CTRL_LOCK		5
#define CTRL_EJECT		6
/* MMC/SDC command */
#define MMC_GET_TYPE		10
#define MMC_GET_CSD		11
#define MMC_GET_CID	        12
#define MMC_GET_OCR		13
#define MMC_GET_SDSTAT		14
/* ATA/CF command */
#define ATA_GET_REV		20
#define ATA_GET_MODEL		21
#define ATA_GET_SN		22


#define _DISKIO
#endif
#ifdef __cplusplus
}
#endif
