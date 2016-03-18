
#include "ebox.h"
#include "w5500.h"
#include "mmc_sd.h"
#include "ff.h"
#include "wrapperdiskio.h"
#include "udp.h"
extern void attachSDCardToFat(SD *sd);


static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
FRESULT res;
DIR DirObject;       //目录结构
DWORD free_clust;//空簇，空扇区大小

u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*定义Mac变量*/
u8 lip[4] = {192, 168, 1, 111}; /*定义lp变量*/
u8 sub[4] = {255, 255, 255, 0}; /*定义subnet变量*/
u8 gw[4] = {192, 168, 1, 1}; /*定义gateway变量*/

u8 ip[6];

W5500 w5500(PC13, SPI2, PB13, PB14, PB15, PC14, PC15);
SD sd(PB12, SPI2, PB13, PB14, PB15);

UDP udp1;

uint32_t ret;

u8 buf[1024];
u32 rl;
float x;
void setup()
{
    ebox_init();
    uart1.begin(9600);
    ret = sd.begin();
    if(!ret)
        uart1.printf("\r\nsdcard init ok!");
    f_mount(0, &fs);

    w5500.begin(mac, lip, sub, gw);

    attachEthToSocket(&w5500);
    attachSDCardToFat(&sd);

    w5500.getMAC (ip);
    uart1.printf("\r\nmac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", ip[0], ip[1], ip[2], ip[3], ip[4], ip[5]);
    w5500.getIP (ip);
    uart1.printf("IP : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    w5500.getSubnet(ip);
    uart1.printf("mask : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    w5500.getGateway(ip);
    uart1.printf("GW : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    uart1.printf("Network is ready.\r\n");

    if(udp1.begin(0, 30000) == 0)
        uart1.printf("\r\nudp1 server creat ok! listen on 30000");


    uart1.printf("\r\nret = %d", ret);
    ret = sd.getCID(buf);
    uart1.printf("\r\nret = %d", ret);
    uart1.printf((const char *)buf);

    ret = sd.getCSD(buf);
    uart1.printf("\r\nret = %d", ret);

    rl = sd.getCapacity();
    x = (float)rl;
    uart1.printf("\r\n容量 = %f", x / 1024 / 1024);

    res = f_getfree("/", &free_clust, &fss);
    uart1.printf("该分区所有扇区数为：%d\r\n", (fss->max_clust - 2) * (fss->csize));
    if(res == FR_OK)
    {
        uart1.printf("该分区所有扇区数为：%d\r\n", (fss->max_clust - 2) * (fss->csize));
        uart1.printf("该分区大小为：%dM\r\n", (fss->max_clust - 2) * (fss->csize) / 2048);
        uart1.printf("该分区空簇数为：%d\r\n", free_clust);
        uart1.printf("该分区空扇区数为：%d\r\n", free_clust * (fss->csize));
    }
    else
        uart1.printf("获取分区空簇失败\r\n,res = %d", res);
    uart1.printf("\r\n");

}
u32 count;
int main(void)
{
    setup();
    while(1)
    {
        res = f_opendir(&DirObject, "/123"); //打开目录
        if(res == FR_OK)
        {
            uart1.printf("\r\n打开目录成功 !\r\n");
            uart1.printf("该目录所在簇号：%d\r\n", DirObject.clust);
            uart1.printf("该目录所在扇区号：%d\r\n", DirObject.sect);
        }
        else if(res == FR_NO_PATH)
            uart1.printf("所找目录路径不存在\r\n");
        else
            uart1.printf("打开目录失败~~~~(>_<)~~~~ \r\n");
        if(udp1.recv(buf))
        {
            uart1.printf("\r\n============================");
            uart1.printf("\r\n本地端口:%d", udp1.localPort );
            uart1.printf("\r\n消息来源:%d.%d.%d.%d:%d", udp1.remoteIP[0], udp1.remoteIP[1], udp1.remoteIP[2], udp1.remoteIP[3], udp1.remotePort);
            uart1.printf("\r\n数据内容:");
            uart1.printf((const char *)buf);
            uart1.printf("\r\n============================");
            udp1.sendto(udp1.remoteIP, udp1.remotePort, buf, 100);
        }
        delay_ms(1000);
    }


}




