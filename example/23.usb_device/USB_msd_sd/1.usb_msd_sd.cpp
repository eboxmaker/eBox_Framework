#include "ebox.h"
#include "usb_msd_sd.h"

/*
* Disk initilization
*/
int USBMSD_SD::disk_initialize() {
	
	int ret;
	ret = SD::init();
	mass_memory_size = SD::get_capacity();
	_status = 0;	
	return ret;
}

int USBMSD_SD::disk_write(const uint8_t* data, uint64_t block, uint8_t count) {
	
	if(count == 1)
	{
		return(SD::write_single_block(block,data));
	}
	else
	{
		return(SD::write_multi_block(block,data,count));
	}	
}


int USBMSD_SD::disk_read(uint8_t* data, uint64_t block, uint8_t count){

	if(count == 1)
	{
		return(SD::read_single_block(block,data));
	}
	else
	{
		return(SD::read_multi_block(block,data,count));		
	}	
}

int USBMSD_SD::disk_status(){
	
		return _status;
	
}

uint64_t USBMSD_SD::disk_size(){

	return mass_memory_size;

}

uint64_t USBMSD_SD::disk_sectors(){
	
	return mass_memory_size/512;
}
