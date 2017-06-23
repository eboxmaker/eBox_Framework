
1.在STM32的3.5版本的库的基础上封装一层类似于Arduino的API。

2.使得STM32也可以使用arduino的大部分驱动。驱动程序从github上下载后稍作修改就可以使用。

3.减少STM32开发人员编写、调试器件驱动的工作量，提高驱动的重复利用率。

#如何编译 本项目使用MDK 515编译

#最新动态 1.更新操作系统api

2.命名操作系统为ebox_os

3.添加GPIO的方法 实现了PB8 = 0;PB8 = !PB8;的操作。

4.增加了ucos

5.增加了freertos

#论坛支持 www.eboxmaker.cn