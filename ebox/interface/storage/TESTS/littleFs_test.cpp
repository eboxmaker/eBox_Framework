/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "mmc_sd.h"
#include "w25xxx.h"
#include "bsp_ebox.h"
#include "fatstructs.h"
#include "interface/storage/filesystem/dir.h"
#include "interface/storage/filesystem/file.h"
#include "interface/storage/filesystem/littlefs/LittleFileSystem.h"
#include "interface/storage/filesystem/fat/FatFileSystem.h"


using namespace ebox;

//W25xxx flash(&PA15, &spi1);
SD flash(&PB12, &spi2);

LittleFileSystem fs("fs");


#define MBED_TEST_BUFFER 1024

#define MBED_TEST_TIMEOUT 480






Dir dir;

File file;

DIR *dd[4];
FILE *fd[4];

//DIR *dd[MBED_TEST_DIRS];
//FILE *fd[MBED_TEST_FILES];

struct dirent ent;
struct dirent *ed;
size_t size;
uint8_t buffer[MBED_TEST_BUFFER];
uint8_t rbuffer[MBED_TEST_BUFFER];
uint8_t wbuffer[MBED_TEST_BUFFER];

//int LittleFileSystem::format(BlockDevice *bd,
//                             lfs_size_t read_size, lfs_size_t prog_size,
//                             lfs_size_t block_size, lfs_size_t lookahead)

void test_directory_creation()
{
    int res = flash.init();
    
    fs.format(&flash,flash.get_read_size(),flash.get_program_size(),
        flash.get_erase_size(),128);  
    
    res = fs.mount(&flash);
    if (res) {

    }

    res = fs.mkdir("potato1", 0777);

    res = fs.unmount();
    
    
    {
        uart1.printf("init :%d\r\n",res);
    }

    res = flash.deinit();
    
}

void test_root_directory()
{
    int res = flash.init();
    res = fs.mount(&flash);
    res = fs.mkdir("/potato2", 0777);
    res = fs.mkdir("/potato4", 0777);
    res = dir.open(&fs, "/");
    struct dirent x;
    int i = 0;
    while(1)
    {
        
//        dir.seek(i++);
        res = dir.read(&x);
        if(res)
        {
            uart1.printf("dir read:[%s]\r\n", x.d_name);
        }
        else
        {
            uart1.printf("dir err:[%d]\r\n",(res));
            break;
        }
    }

    res = dir.close();
    
    
    res = fs.unmount();
    res = flash.deinit();
}

void test_simple_file_test()
{
    int res = flash.init();

    res = fs.mount(&flash);
    res = file.open(&fs, "hello.txt", O_WRONLY | O_CREAT);
    size = strlen("Hello World!\n");
    memcpy(wbuffer, "Hello World!\n", size);
    res = file.write(wbuffer, size);
    res = file.close();
    res = file.open(&fs, "hello.txt", O_RDONLY);
    size = strlen("Hello World!\n");
    res = file.read(rbuffer, size);
    uart1.printf("read £º[%s]\r\n",rbuffer);
//    res = memcmp(rbuffer, wbuffer, size);
    res = file.close();
    res = fs.unmount();

    res = flash.deinit();
}


void setup()
{
    u8 ret;
    ebox_init();
    uart1.begin(115200);
    print_log();
    test_directory_creation();
test_root_directory();
    test_simple_file_test();
}


int main(void)
{
    setup();
    while(1)
    {

        //uart1.printf("\r\nrunning£¡");
        delay_ms(1000);
    }


}




