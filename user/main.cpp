/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "bsp_ebox.h"
#include "mmc_sd.h"
#include "w25xxx.h"
#include "interface/storage/filesystem/FileSystem.h"
#include "interface/storage/filesystem/FAT/FATFileSystem.h"
#include "interface/storage/filesystem/dir.h"
#include "rtcmillis.h"
FATFileSystem fs("sd");

W25xxx flash(&PA15, &spi1);
SD sd(&PB12, &spi2);
RtcMillis LocalTime;

void dir_create()
{
    int ret;
    UART.printf("mkdir...\n");
    for(int i = 0; i < 10; i++)
    {
        String path("/ebox_");
        path += i;
        ret = fs.mkdir(path.c_str(),O_CREAT);
        if(ret)
        {
            UART.printf("mkdir[%s]:%d\n",path.c_str(), ret);
        }
        else
        {
            UART.printf("mkdir:[%s]...ok\n", path.c_str());
        }
        UART.flush();
    }
    
    for(int i = 0; i < 10; i++)
    {
        String path("/ebox_0/ebox2_");
        path += i;
        ret = fs.mkdir(path.c_str(),O_CREAT);
        if(ret)
        {
            UART.printf("mkdir[%s]:%d\n",path.c_str(), ret);
        }
        else
        {
            UART.printf("mkdir:[%s]...ok\n", path.c_str());
        }
        UART.flush();
    }
}
#include "ebox_mem.h"
int leve = 0;
void dir_tree(const char *path)
{
    int ret;
    Dir dir;
    ret = dir.open(&fs,path);
    if(ret)
    {
        UART.printf("open dir err:%d\n",ret);
    }
    else
    {
        UART.printf("open dir ok:%s,%d\n",path,ebox_get_free());
        leve++;
    }
    UART.flush();
    struct dirent de;
    while(1)
    {
        String str = "";
        ret = dir.read(&de);
        if(ret)
        {
            for(int i = 1; i < leve; i++)
                UART.printf("\t|");
            UART.printf("%s\n", &(de.d_name)[0]);
            if(de.d_type == DT_DIR)
            {
                str += path;
                str += "/";
                str += de.d_name;
//                UART.printf("new:%s\n", str.c_str());
                dir_tree(str.c_str());
            }

        }
        else
        {
//            UART.printf("path:%s;err:%d\n",str.c_str(), ret);
            break;
        }
        UART.flush();
    } 
    leve--;
}
void dirOpt()
{
    int ret;
    UART.printf("mkdir...\n");
    for(int i = 0; i < 10; i++)
    {
        String path("/ebox_");
        path += i;
        ret = fs.mkdir(path.c_str(),O_CREAT);
        if(ret)
        {
            UART.printf("mkdir[%s]:%d\n",path.c_str(), ret);
        }
        else
        {
            UART.printf("mkdir:[%s]...ok\n", path.c_str());
        }
        UART.flush();
    }
    
    
    Dir dir;
    ret = dir.open(&fs,"/");
    if(ret)
    {
        UART.printf("open dir err:%d\n",ret);
    }
    else
    {
        UART.printf("open dir ok\n");
    }
    UART.flush();
    struct dirent de;
   
    UART.printf("read dir...\n");
    UART.flush();
    
    while(1)
    {
        ret = dir.read(&de);
        if(ret)
        {
            UART.printf("pwd:[/%s]\n", &(de.d_name)[0]);
        }
        else
        {
            UART.printf("err:%d\n", ret);
            break;
        }
        UART.flush();
    }
    UART.printf("delet dir...\n");
    for(int i = 0; i < 10; i++)
    {
        String path("/ebox_");
        path += i;
        ret = fs.remove(path.c_str());
        if(ret)
        {
            UART.printf("remove[%s]:%d\n",path.c_str(), ret);
        }
        else
        {
            UART.printf("remove:[%s]\n", path.c_str());
        }
        UART.flush();
    }   
}
void setup()
{
    u8 ret;
    ebox_init();
    LocalTime.begin();
    LocalTime.dateTime.parse(String(__DATE__),String(__TIME__),8);

    UART.begin(115200);
    print_log();
    ret = fs.mount(&sd);
    if(ret)
    {
        UART.printf("mount err:%d\n",ret);
    }
    else
    {
        UART.printf("mount ok\n");
    }
    dir_create();
    dir_tree("");
    //dirOpt();
}
int main(void)
{
    setup();
    while(1)
    {
        LocalTime.update();
        LocalTime.print(UART);
        delay_ms(1000);
    }


}




