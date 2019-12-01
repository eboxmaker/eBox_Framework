#ifndef __EBOX_BLOCK_H
#define __EBOX_BLOCK_H


#include "ebox_core.h"


class Block
{

    public:
        Block(){};
        virtual int init() = 0;
        virtual int read_sector(uint8_t *buffer, uint32_t sector, uint8_t count) = 0;
        virtual int erase_sector(uint32_t sector) = 0;
        virtual int write_sector(uint8_t *data, uint32_t sector, uint8_t count) = 0;
        virtual uint32_t get_sector_count(){return sector_count;};
        virtual uint32_t get_sector_size(){return sector_size;};
        virtual uint32_t get_capacity(){return capacity;};

    protected:
        uint16_t sector_size;
        uint32_t sector_count;
        uint32_t capacity;

//        uint16_t blocks;
//        uint16_t segments;
};


#endif

