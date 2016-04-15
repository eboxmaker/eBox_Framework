#include "ebox.h"
#include "mmc_sd.h"
#include "wrapperdiskio.h"
SD* sdCard;

int	MMC_disk_status(void)
{
	return 0;
};

void attach_sd_to_fat(void* sd)
{
	sdCard = (SD*)sd;
}

u8 MMC_disk_initialize(void)
{
	return(sdCard->init());
}


u8 MMC_disk_read( u8 *buffer, u32 sector, u8 count)
{
	if(count == 1)
	//if(1)
	{
		return(sdCard->read_single_block(sector,buffer));
	}
	else
	{
		return(sdCard->read_multi_block(sector,buffer,count));		
	}

}

u8 MMC_disk_write( const u8 *data, u32 sector, u8 count)
{
	if(count == 1)
	//if(1)
	{
		return(sdCard->write_single_block(sector,data));
	}
	else
	{
		return(sdCard->write_multi_block(sector,data,count));
	}
}


uint64_t MMC_get_capacity(void)
{
		return(sdCard->get_capacity());

}
