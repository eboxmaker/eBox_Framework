/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "w25xxx.h"
#include "mmc_sd.h"
#include "bsp_ebox.h"
#include "fatstructs.h"
#include "interface/storage/filesystem/dir.h"
#include "interface/storage/filesystem/file.h"
//#include "interface/storage/filesystem/littlefs/LittleFileSystem.h"
#include "interface/storage/filesystem/fat/FatFileSystem.h"

using namespace ebox;

SD sd(&PB12, &spi2);

FATFileSystem fs("fs");


#define MAX_BUFFER_SIZE 128

Dir dir;
File file;

DIR *dd;
FILE *fd;

struct dirent ent;
struct dirent *ed;
size_t size;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t rbuffer[MAX_BUFFER_SIZE];
uint8_t wbuffer[MAX_BUFFER_SIZE];

//int LittleFileSystem::format(BlockDevice *bd,
//                             lfs_size_t read_size, lfs_size_t prog_size,
//                             lfs_size_t block_size, lfs_size_t lookahead)

void test_directory_creation()
{
    int res = sd.init();
    

    res = fs.mount(&sd);
    if(res == FR_OK)
    {
        uart1.printf("mount ok:%d\r\n",res);
    }
    else
    {
        uart1.printf("mount err :%d\r\n",res);
    }
    
    
    res = fs.format(&sd,);
    if(res == FR_OK)
    {
        uart1.printf("format ok:%d\r\n",res);
    }
    else
    {
        uart1.printf("format err :%d\r\n",res);
    }
    while(1);


    res = fs.mkdir("0:potato2", 0777);
    if(res == FR_OK)
    {
        uart1.printf("mkdir ok:%d\r\n",res);
    }
    else
    {
        uart1.printf("mkdir err :%d\r\n",res);
    }

    res = fs.unmount();
    if(res == FR_OK)
    {
        uart1.printf("unmount ok:%d\r\n",res);
    }
    else
    {
            uart1.printf("unmount err :%d\r\n",res);
    }

    res = sd.deinit();
    
}

void test_root_directory()
{
    int res = sd.init();
    res = fs.mount(&sd);
//    res = fs.mkdir("/potato2", 0777);
//    res = fs.mkdir("/potato4", 0777);
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
    res = sd.deinit();
}

void test_simple_file_test()
{
    int res = sd.init();

    res = fs.mount(&sd);
    if(res == 0)
    {
        uart1.printf("file mount ok\r\n");
    }
    else
    {
        uart1.printf("mount£º[%d]\r\n",res);
    }    
    res = file.open(&fs, "/he.txt", O_WRONLY | O_CREAT);
    if(res == 0)
    {
        uart1.printf("creat ok\r\n");
    }
    else
    {
        uart1.printf("creat err£º[%d]\r\n",res);
    }

    size = strlen("Hello World!\n");
    memcpy(wbuffer, "Hello World!\n", size);
    res = file.write(wbuffer, size);
    uart1.printf("write size:%d\r\n",res);
 
    res = file.close();
    
    
    res = file.open(&fs, "/he.txt", O_RDONLY);
    if(res == 0)
    {
        uart1.printf("open ok\r\n");
    }
    else
    {
        uart1.printf("open err£º[%d]\r\n",res);
    }

    uart1.flush();
    size = strlen("Hello World!\n");
    res = file.read(rbuffer, size);
    uart1.printf("read size:%d[%s]\r\n",res,rbuffer);

    res = file.close();
    res = fs.unmount();

    res = sd.deinit();
    uart1.flush();
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

//        uart1.printf("\r\nrunning£¡");
        delay_ms(1000);
    }


}




