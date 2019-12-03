#include "ebox.h"
#include "mmc_sd.h"
#include "wrapperdiskio.h"
#include "ebox_block.h"
#include "diskio.h"

#define MAX_PHYSICAL 1

#define SPI_FLASH		0	/* Example: Map MMC/SD card to physical drive 1 */
//#define ATA		1	/* Example: Map ATA harddisk to physical drive 0 */
//#define USB		2	/* Example: Map USB MSD to physical drive 2 */


Block *block[MAX_PHYSICAL];


DRESULT attach_sd_to_fat(uint8_t pdrv,void *handler)
{
    if(pdrv >= MAX_PHYSICAL) return RES_PARERR;
    block[pdrv] = (Block *)handler;
}

DSTATUS	my_disk_status(uint8_t pdrv)
{
    if(pdrv >= MAX_PHYSICAL) return STA_NOINIT;
    return 0;
};


DSTATUS my_disk_initialize(uint8_t pdrv)
{
    if(pdrv >= MAX_PHYSICAL) return RES_PARERR;
    block[pdrv]->begin();
    return 0;
}


DRESULT my_disk_read( uint8_t pdrv, uint8_t *buffer, uint32_t sector, uint8_t count)
{
    if(pdrv >= MAX_PHYSICAL) return RES_PARERR;
    block[pdrv]->read_sector(buffer,sector, count);

    return RES_OK;
}

DRESULT my_disk_write(uint8_t pdrv, const uint8_t *data, uint32_t sector, uint8_t count)
{
    if(pdrv >= MAX_PHYSICAL) return RES_PARERR;
    block[pdrv]->write_sector((uint8_t *)data,sector, count);
    for(int i = 0; i < count; i++)
    {
        uart1.printf("write:sector %d\r\n",sector++);
    }
    return RES_OK;
}

DRESULT  my_disk_ioctl (
   uint8_t pdrv,
    uint8_t cmd,		/* Control code */
    void *buff		/* Buffer to send/receive control data */
)
{
    if(pdrv >= MAX_PHYSICAL) return RES_PARERR;
    switch (cmd) 
    {
     case CTRL_SYNC:
         return RES_OK;

     /* 扇区数量 1024*1024*1024 =4 (MB) */

     case GET_SECTOR_COUNT:

         *(DWORD * )buff = block[pdrv]->get_sector_count();//W25Q32 有1024个大小为4k bytes 的扇区
         return RES_OK;

     /* 扇区大小 */

     case GET_SECTOR_SIZE :

         *(WORD * )buff = block[pdrv]->get_sector_size();//spi flash的扇区大小是 4K Bytes
         return RES_OK;

     /*块大小 */

     case GET_BLOCK_SIZE :
         *(DWORD * )buff = 4;
         return RES_OK;
         
     default:
         return RES_PARERR;
     }
    return RES_PARERR;
}
uint64_t my_et_capacity(uint8_t pdrv)
{
    if(pdrv >= MAX_PHYSICAL) return RES_PARERR;
    return(block[pdrv]->get_capacity());
}



