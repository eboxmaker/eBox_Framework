#include "ebox.h"
#include "mmc_sd.h"
#include "wrapperdiskio.h"
SD* sdCard;

void attachSDCardToFat(SD* sd)
{
	sdCard = sd;
}

u8 SD_Init(void)
{
	return(sdCard->init());
}

u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count)
{
	return(sdCard->readMultiBlock(sector,buffer,count));
}
u8 SD_WriteMultiBlock(u32 sector,  const u8 *data, u8 count)
{
	return(sdCard->writeMultiBlock(sector,data,count));
}


