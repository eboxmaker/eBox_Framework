# SFUD (Serial Flash Universal Driver) 串行 Flash 通用驱动库

---

## 0、SFUD 是什么

[SFUD](https://github.com/armink/SFUD) 是一款开源的串行 SPI Flash 通用驱动库。由于现有市面的串行 Flash 种类居多，各个 Flash 的规格及命令存在差异， SFUD 就是为了解决这些 Flash 的差异现状而设计，让我们的产品能够支持不同品牌及规格的 Flash，提高了涉及到 Flash 功能的软件的可重用性及可扩展性，同时也可以规避 Flash 缺货或停产给产品所带来的风险。

- 主要特点：面向对象（同时支持多个 Flash 对象）、可灵活裁剪、扩展性强、支持 4 字节地址
- 资源占用
  - 标准占用：RAM:0.2KB ROM:5.5KB
  - 最小占用：RAM:0.1KB ROM:3.6KB
- 设计思路：这里要首先跟大家介绍一个标准： **SFDP** ，它是 JEDEC （固态技术协会）制定的串行 Flash 功能的参数表标准，最新版 V1.6B （[点击这里查看](https://www.jedec.org/standards-documents/docs/jesd216b)）。该标准规定了，每个 Flash 中会存在一个参数表，该表中会存放 Flash 容量、写粗粒度、擦除命令、地址模式等 Flash 规格参数。目前，除了部分厂家旧款 Flash 型号会不支持该标准，其他绝大多数新出厂的 Flash 均已支持 SFDP 标准。所以该库在初始化时会优先读取 SFDP 表参数，如果该 Flash 不支持 SFDP，则查询配置文件 ( `/sfud/inc/sfud_flash_def.h` ) 中提供的 **Flash 参数信息表** 中是否支持该款 Flash。如果不支持，则可以在配置文件中添加该款 Flash 的参数信息（添加方法详细见 [2.5 添加库目前不支持的 Flash](#25-添加库目前不支持的-flash)）。获取到了 Flash 的规格参数后，就可以实现对 Flash 的全部操作。

## 1、为什么选择 SFUD

- 避免项目因 Flash 缺货、Flash 停产或产品扩容而带来的风险；
- 越来越多的项目将固件存储到串行 Flash 中，例如：ESP8266 的固件、主板中的 BIOS 及其他常见电子产品中的固件等等，但是各种 Flash 规格及命令不统一。使用 SFUD 即可避免，在相同功能的软件平台基础下，无法适配不同 Flash 种类的硬件平台的问题，提高软件的可重用性；
- 简化软件流程，降低开发难度。现在只需要配置好 SPI 通信，即可畅快的开始玩串行 Flash 了；
- 可以用来制作 Flash 编程器/烧写器

## 2、SFUD 如何使用

### 2.1 已支持 Flash 

下表为所有在 Demo 平台上进行过真机测试的 Flash。目前 SFUD 提供的 **Flash 参数信息表** 只包括下表中 **不支持** SFDP 标准的 Flash，其他不支持 SFDP 标准的 Flash 需要大家以后 **共同来完善和维护**  **([Github](https://github.com/armink/SFUD)|[OSChina](http://git.oschina.net/armink/SFUD)|[Coding](https://coding.net/u/armink/p/SFUD/git))** 。如果觉得这个开源项目很赞，可以点击 [项目主页](https://github.com/armink/SFUD) 右上角的 **Star** ，同时把它推荐给更多有需要的朋友。

|型号|制造商|容量|最高速度|SFDP|备注|
|:--:|:----:|:--:|:--:|:--:|:--:|
|[W25Q40BV](http://microchip.ua/esp8266/W25Q40BV(EOL).pdf)|Winbond|4Mb|50Mhz|不支持|已停产|
|[W25Q80DV](http://www.winbond.com/resource-files/w25q80dv_revg_07212015.pdf)|Winbond|8Mb|104Mhz|支持||
|[W25Q16CV](http://www.winbond.com/resource-files/da00-w25q16cvf1.pdf)|Winbond|16Mb|104Mhz|支持||
|[W25Q32BV](http://www.winbond.com/resource-files/w25q32bv_revi_100413_wo_automotive.pdf)|Winbond|32Mb|104Mhz|支持||
|[W25Q64CV](http://www.winbond.com/resource-files/w25q64cv_revh_052214[2].pdf)|Winbond|64Mb|80Mhz|支持||
|[W25Q128BV](http://www.winbond.com/resource-files/w25q128bv_revh_100313_wo_automotive.pdf)|Winbond|128Mb|104Mhz|支持||
|[W25Q256FV](http://www.winbond.com/resource-files/w25q256fv%20revi%2002262016%20kms.pdf)|Winbond|256Mb|104Mhz|支持||
|[MX25L3206E](http://www.macronix.com/Lists/DataSheet/Attachments/3199/MX25L3206E,%203V,%2032Mb,%20v1.5.pdf)|Macronix|32Mb|86MHz|支持||
|[KH25L3206E](http://www.macronix.com.hk/Lists/Datasheet/Attachments/131/KH25L3206E.pdf)|Macronix|32Mb|86Mhz|支持||
|[SST25VF016B](http://ww1.microchip.com/downloads/en/DeviceDoc/20005044C.pdf)|Microchip|16Mb|50MHz|不支持| SST 已被 Microchip 收购|
|[M25P32](https://www.micron.com/~/media/documents/products/data-sheet/nor-flash/serial-nor/m25p/m25p32.pdf)|Micron|32Mb|75Mhz|不支持||
|[EN25Q32B](http://www.kean.com.au/oshw/WR703N/teardown/EN25Q32B%2032Mbit%20SPI%20Flash.pdf)|EON|32Mb|104MHz|不支持||
|[GD25Q64B](http://www.gigadevice.com/product/download/24.html)|GigaDevice|64Mb|120Mhz|不支持||
|[S25FL216K](http://www.cypress.com/file/197346/download)|Cypress|16Mb|65Mhz|不支持||
|[S25FL164K](http://www.cypress.com/file/196886/download)|Cypress|64Mb|108Mhz|支持||
|[A25LQ64](http://www.amictechnology.com/datasheets/A25LQ64.pdf)|AMIC|64Mb|104Mhz|支持||
|[A25L080](http://www.amictechnology.com/datasheets/A25L080.pdf)|AMIC|8Mb|100Mhz|不支持||
|[F25L004](http://www.esmt.com.tw/db/manager/upload/f25l004.pdf)|ESMT|4Mb|100Mhz|不支持||
|[PCT25VF016B](http://pctgroup.com.tw/attachments/files/files/248_25VF016B-P.pdf)|PCT|16Mb|80Mhz|不支持|SST 授权许可，会被识别为 SST25VF016B|
|[AT45DB161E](http://www.adestotech.com/wp-content/uploads/doc8782.pdf)|ADESTO|16Mb|85MHz|不支持|ADESTO 收购 Atmel 串行闪存产品线|

### 2.2 API 说明

#### 2.2.1 初始化 SFUD 库

```C
sfud_err sfud_init(void)
```

#### 2.2.2 获取 Flash 设备对象

在 SFUD 配置文件中会定义 Flash 设备表，负责存放所有将要使用的 Flash 设备对象，所以 SFUD 支持多个 Flash 设备同时驱动。设备表的配置在 `/sfud/inc/sfud_cfg.h` 中 `SFUD_FLASH_DEVICE_TABLE` 宏定义，详细配置方法参照 [2.3 配置方法 Flash](#23-配置方法)）。本方法通过 Flash 设备位于设备表中索引值来返回 Flash 设备对象，超出设备表范围返回 `NULL` 。

```C
sfud_flash *sfud_get_device(size_t index)
```

|参数                                    |描述|
|:-----                                  |:----|
|index                                   |Flash 设备位于 FLash 设备表中的索引值|

#### 2.2.3 获取 Flash 设备总数

返回 Flash 设备表的总长度。

```C
size_t sfud_get_device_num(void)
```

#### 2.2.4 获取 Flash 设备表

```C
const sfud_flash *sfud_get_device_table(void)
```

#### 2.2.5 读取 Flash 数据

```C
sfud_err sfud_read(const sfud_flash *flash, uint32_t addr, size_t size, uint8_t *data)
```

|参数                                    |描述|
|:-----                                  |:----|
|flash                                   |Flash 设备对象|
|addr                                    |起始地址|
|size                                    |读取数据的大小|
|data                                    |读取到的数据|

#### 2.2.6 擦除 Flash 数据

```C
sfud_err sfud_erase(const sfud_flash *flash, uint32_t addr, size_t size)
```

|参数                                    |描述|
|:-----                                  |:----|
|flash                                   |Flash 设备对象|
|addr                                    |起始地址|
|size                                    |擦除数据的大小|

#### 2.2.7 擦除 Flash 全部数据

```C
sfud_err sfud_chip_erase(const sfud_flash *flash)
```

|参数                                    |描述|
|:-----                                  |:----|
|flash                                   |Flash 设备对象|

#### 2.2.8 往 Flash 写数据

```C
sfud_err sfud_write(const sfud_flash *flash, uint32_t addr, size_t size, const uint8_t *data)
```

|参数                                    |描述|
|:-----                                  |:----|
|flash                                   |Flash 设备对象|
|addr                                    |起始地址|
|size                                    |写数据的大小|
|data                                    |待写入的数据|

#### 2.2.9 先擦除再往 Flash 写数据

```C
sfud_err sfud_erase_write(const sfud_flash *flash, uint32_t addr, size_t size, const uint8_t *data)
```

|参数                                    |描述|
|:-----                                  |:----|
|flash                                   |Flash 设备对象|
|addr                                    |起始地址|
|size                                    |写数据的大小|
|data                                    |待写入的数据|

#### 2.2.10 读取 Flash 状态

```C
sfud_err sfud_read_status(const sfud_flash *flash, uint8_t *status)
```

|参数                                    |描述|
|:-----                                  |:----|
|flash                                   |Flash 设备对象|
|status                                  |当前状态寄存器值|

#### 2.2.11 写（修改） Flash 状态

```C
sfud_err sfud_write_status(const sfud_flash *flash, bool is_volatile, uint8_t status)
```

|参数                                    |描述|
|:-----                                  |:----|
|flash                                   |Flash 设备对象|
|is_volatile                             |是否为易闪失的，true: 易闪失的，及断电后会丢失|
|status                                  |当前状态寄存器值|

### 2.3 配置方法

所有配置位于 `/sfud/inc/sfud_cfg.h` ，请参考下面的配置介绍，选择适合自己项目的配置。

#### 2.3.1 调试模式

打开/关闭 `SFUD_DEBUG_MODE` 宏定义

#### 2.3.2 是否使用 SFDP 参数功能

打开/关闭 `SFUD_USING_SFDP` 宏定义

> 注意：关闭后只会查询该库在  `/sfud/inc/sfud_flash_def.h` 中提供的 Flash 信息表。这样虽然会降低软件的适配性，但减少代码量。

#### 2.3.3 是否使用该库自带的 Flash 参数信息表

打开/关闭 `SFUD_USING_FLASH_INFO_TABLE` 宏定义

> 注意：关闭后该库只驱动支持 SFDP 规范的 Flash，也会适当的降低部分代码量。另外 2.3.2 及 2.3.3 这两个宏定义至少定义一种，也可以两种方式都选择。

#### 2.3.4 Flash 设备表

主要修改 `SFUD_FLASH_DEVICE_TABLE` 这个宏定义，示例如下：

```C
enum {
    SFUD_W25Q64CV_DEVICE_INDEX = 0,
    SFUD_GD25Q64B_DEVICE_INDEX = 1,
};

#define SFUD_FLASH_DEVICE_TABLE                                                \
{                                                                              \
    [SFUD_W25Q64CV_DEVICE_INDEX] = {.name = "W25Q64CV", .spi.name = "SPI1"},   \
    [SFUD_GD25Q64B_DEVICE_INDEX] = {.name = "GD25Q64B", .spi.name = "SPI3"},   \
}
```

上面定义了两个 Flash 设备（大部分产品一个足以），两个设备的名称为 `"W25Q64CV"` 及 `"GD25Q64B"` ，分别对应 `"SPI1"` 及 `"SPI3"` 这两个 SPI 设备名称（在移植 SPI 接口时会用到，位于 `/sfud/port/sfud_port.c` ）， `SFUD_W25Q16CV_DEVICE_INDEX` 与 `SFUD_GD25Q64B_DEVICE_INDEX` 这两个枚举定义了两个设备位于设备表中的索引，可以通过 `sfud_get_device_table()` 方法获取到设备表，再配合这个索引值来访问指定的设备。

### 2.4 移植说明

移植文件位于 `/sfud/port/sfud_port.c` ，文件中的 `sfud_err sfud_spi_port_init(sfud_flash *flash)` 方法是库提供的移植方法，在里面完成各个设备 SPI 读写驱动（必选）、重试次数（必选）、重试接口（可选）及 SPI 锁（可选）的配置。更加详细的移植内容，可以参考 demo 中的各个平台的移植文件。

### 2.5 添加库目前不支持的 Flash 

这里需要修改 `/sfud/inc/sfdu_flash_def.h` ，所有已经支持的 Flash 见 `SFUD_FLASH_CHIP_TABLE` 宏定义，需要提前准备的 Flash 参数内容分别为：| 名称 | 制造商 ID | 类型 ID | 容量 ID | 容量 | 写模式  | 擦除粗粒度（擦除的最小单位） | 擦除粗粒度对应的命令 | 。这里以添加 兆易创新 ( GigaDevice ) 的 `GD25Q64B` Flash 来举例。

此款 Flash 为兆易创新的早期生产的型号，所以不支持 SFDP 标准。首先需要下载其数据手册，找到 0x9F 命令返回的 3 种 ID， 这里需要最后面两字节 ID ，即 `type id` 及 `capacity id` 。 `GD25Q64B` 对应这两个 ID 分别为 `0x40` 及 `0x17` 。上面要求的其他 Flash 参数都可以在数据手册中找到，这里要重点说明下 **写模式** 这个参数，库本身提供的写模式共计有 4 种，详见文件顶部的 `sfud_write_mode` 枚举类型，同一款 Flash 可以同时支持多种写模式，视情况而定。对于 `GD25Q64B` 而言，其支持的写模式应该为 `SFUD_WM_PAGE_256B` ，即写 1-256 字节每页。结合上述 `GD25Q64B` 的 Flash 参数应如下：

```
    {"GD25Q64B", SFUD_MF_ID_GIGADEVICE, 0x40, 0x17, 8*1024*1024, SFUD_WM_PAGE_256B, 4096, 0x20},
```

再将其增加到 `SFUD_FLASH_CHIP_TABLE` 宏定义末尾，即可完成该库对 `GD25Q64B` 的支持。

### 2.6 Demo

目前已支持如下平台下的 Demo

|路径                             |平台描述|
|:-----                           |:----|
|[/demo/stm32f10x_non_os](https://github.com/armink/SFUD/tree/master/demo/stm32f10x_non_os) |STM32F10X 裸机平台|
|[/demo/stm32f2xx_rtt](https://github.com/armink/SFUD/tree/master/demo/stm32f2xx_rtt)  |STM32F2XX + [RT-Thread](http://www.rt-thread.org/) 操作系统平台|

### 2.7 许可

采用 MIT 开源协议，细节请阅读项目中的 LICENSE 文件内容。