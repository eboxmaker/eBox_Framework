#include "ebox.h"
#include "mmc_sd.h"
#include "wrapperdiskio.h"
SD* sdCard;

int	MMC_disk_status(void)
{
	return 0;
};

void attachSDCardToFat(SD* sd)
{
	sdCard = sd;
}

u8 MMC_disk_initialize(void)
{
	return(sdCard->init());
}


u8 MMC_disk_read( u8 *buffer, u32 sector, u8 count)
{
	return(sdCard->readMultiBlock(sector,buffer,count));
}

u8 MMC_disk_write( const u8 *data, u32 sector, u8 count)
{
	return(sdCard->writeMultiBlock(sector,data,count));
}



