# ebox_stm32
# ebox_stm32特点
1.在STM32的库文件的基础上封装一层类似于Arduino的API。

2.使得STM32也可以使用arduino的大部分驱动。驱动程序从github上下载后稍作修改就可以使用。

3.快速实现底层驱动代码，减少STM32开发人员编写、调试器件驱动的工作量，提高驱动的重复利用率。

# 支持芯片

1、STM32F0系列

2、STM32F1系列

3、STM32F4系列

# 如何编译
本项目使用MDK 5编译

# 固件库支持

* MCU芯片的全部外设（Gpio，Exti，Timer，Uart，Pwm，Spi，I2c，Adc，Wdg等等）
* 传感器，执行器的驱动文件
* 网络（w5500,ESP8266）
* 文件系统（Fatfs）
* 操作系统（FreeRTOS,uCOS，ebox_os）
* 数字滤波器（高通，低通，带通）
* PID控制器
* IO事件管理器（IO事件驱动）
* 色彩管理器（HSL,HSV,RGB）
* 万年历
* 线性回归（主要用于数据采集器的校准）
* JSON
* FreeModbus
* gui



# 论坛支持
bbs.eboxmaker.com
