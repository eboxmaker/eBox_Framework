#ifndef USBMSD_SD_H
#define USBMSD_SD_H
 
#include "ebox.h"
#include "USBMSD.h"
#include "mmc_sd.h"
/** Use the SDcard as mass storage device using the USBMSD class
 *
 * @code
 * #include "ebox.h"
 * #include "USBMSD_SD.h"
 *
 * USBMSD_SD usb_sd;
 *
 * int main() {
 *   while(1);
 * }
 *
 * @endcode
 */
class USBMSD_SD : public USBMSD,public SD{
public:			
		USBMSD_SD(Gpio *cs, Spi *spi):SD(cs, spi)
		{
			 //no init
			SD::begin(3);
			_status = 0x01;
			connect();
		};
    
		/*
    * read one or more blocks on a storage chip
    *
    * @param data pointer where will be stored read data
    * @param block starting block number
    * @param count number of blocks to read
    * @returns 0 if successful
    */
    virtual int disk_read(uint8_t* data, uint64_t block, uint8_t count);

    /*
    * write one or more blocks on a storage chip
    *
    * @param data data to write
    * @param block starting block number
    * @param count number of blocks to write
    * @returns 0 if successful
    */
    virtual int disk_write(const uint8_t* data, uint64_t block, uint8_t count);

    /*
    * Disk initilization
    */
    virtual int disk_initialize();

    /*
    * Return the number of blocks
    *
    * @returns number of blocks
    */
    virtual uint64_t disk_sectors();

    /*
    * Return memory size
    *
    * @returns memory size
    */
    virtual uint64_t disk_size();


    /*
    * To check the status of the storage chip
    *
    * @returns status: 0: OK, 1: disk not initialized, 2: no medium in the drive, 4: write protected
    */
    virtual int disk_status();

private:
		uint64_t mass_memory_size;
		uint8_t _status;
};
 
#endif
