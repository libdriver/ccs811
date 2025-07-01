[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver CCS811

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ccs811/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

CCS811是一种超低功耗数字气体传感器解决方案，它集成了一个金属氧化物（MOX）气体传感器，用于检测各种挥发性有机化合物（VOC），用于室内空气质量监测，并配有一个微控制器单元（MCU），其中包括一个模数转换器（ADC）和一个I²C接口。CCS811基于艾迈斯半导体独特的微热板技术，为气体传感器提供了高度可靠的解决方案，循环时间非常快，平均功耗显著降低。集成MCU管理传感器驱动模式和检测VOC时测量的原始传感器数据。I²C数字接口大大简化了硬件和软件设计，缩短了上市时间。CCS811支持智能算法处理原始传感器测量值，以输出TVOC值或等效的CO2（eCO2）水平。CCS811支持多种测量模式，这些模式在活动传感器测量期间针对低功耗进行了优化，并在便携式应用中延长了电池寿命。CCS811采用10引脚2.7mm x 4.0mm x 1.1mm、间距0.6mm的LGA封装。

LibDriver CCS811是LibDriver推出的CCS811的全功能驱动，该驱动提供TVOC、eCO2读取、温湿度矫正等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example update](#example-update)
    - [example interrupt](#example-interrupt)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver CCS811的源文件。

/interface目录包含了LibDriver CCS811与平台无关的IIC总线模板。

/test目录包含了LibDriver CCS811驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver CCS811编程范例。

/doc目录包含了LibDriver CCS811离线文档。

/datasheet目录包含了CCS811数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_ccs811_basic.h"

uint8_t res;
uint8_t id;
uint8_t version;
uint8_t major;
uint8_t minor;
uint8_t trivial;
uint8_t status;
uint32_t i;
uint16_t baseline = 0;
float temperature = 25.0f;
float humidity = 50.0f;

/* init */
res = ccs811_basic_init(CCS811_ADDRESS_GND);
if (res != 0)
{
    return 1;
}

/* delay 5000ms */
ccs811_interface_delay_ms(5000);

...
    
/* loop */
for (i = 0; i < 3; i++)
{
    uint16_t eco2_ppm;
    uint16_t tvoc_ppb;

    /* read data */
    res = ccs811_basic_read((uint16_t *)&eco2_ppm, (uint16_t *)&tvoc_ppb);
    if (res != 0)
    {
        (void)ccs811_basic_deinit();

        return 1;
    }

    /* output */
    ccs811_interface_debug_print("ccs811: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)3);
    ccs811_interface_debug_print("ccs811: co2 eq is %d ppm.\n", eco2_ppm);
    ccs811_interface_debug_print("ccs811: tvoc is %d ppb.\n", tvoc_ppb);

    /* delay 1000ms */
    ccs811_interface_delay_ms(1000);
    
    ...
}

...

/* loop */
for (i = 0; i < 3; i++)
{
    uint8_t ua;
    float adc_v;

    /* read data */
    res = ccs811_basic_get_raw_data(&ua, &adc_v);
    if (res != 0)
    {
        (void)ccs811_basic_deinit();

        return 1;
    }

    /* output */
    ccs811_interface_debug_print("ccs811: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)3);
    ccs811_interface_debug_print("ccs811: ua is %duA.\n", ua);
    ccs811_interface_debug_print("ccs811: adc is %.02fV.\n", adc_v);

    /* delay 1000ms */
    ccs811_interface_delay_ms(1000);
    
    ...
}

...

/* set environment data */
res = ccs811_basic_set_environment_data(humidity, temperature);
if (res != 0)
{
    (void)ccs811_basic_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: set humidity %.02f%%.\n", humidity);
ccs811_interface_debug_print("ccs811: set temperature %.02fC.\n", temperature); 

...

/* set baseline */
res = ccs811_basic_set_baseline(baseline);
if (res != 0)
{
    (void)ccs811_basic_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: set baseline %d.\n", baseline);

...

/* get hardware id */
res = ccs811_basic_get_hardware_id(&id);
if (res != 0)
{
    (void)ccs811_basic_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: hardware id is 0x%02X.\n", id);

...

/* get hardware version */
res = ccs811_basic_get_hardware_version(&version);
if (res != 0)
{
    (void)ccs811_basic_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: hardware version is 0x%02X.\n", version); 

...
 
/* get firmware bootloader version */
res = ccs811_basic_get_firmware_bootloader_version(&major, &minor, &trivial);
if (res != 0)
{
    (void)ccs811_basic_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: firmware bootloader version major is 0x%02X.\n", major);
ccs811_interface_debug_print("ccs811: firmware bootloader version minor is 0x%02X.\n", minor);
ccs811_interface_debug_print("ccs811: firmware bootloader version trivial is 0x%02X.\n", trivial); 

...

/* get firmware application version */
res = ccs811_basic_get_firmware_application_version(&major, &minor, &trivial);
if (res != 0)
{
    (void)ccs811_basic_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: firmware application version major is 0x%02X.\n", major);
ccs811_interface_debug_print("ccs811: firmware application version minor is 0x%02X.\n", minor);
ccs811_interface_debug_print("ccs811: firmware application version trivial is 0x%02X.\n", trivial);

...

/* get status */
res = ccs811_basic_get_status(&status);
if (res != 0)
{
    (void)ccs811_basic_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: status is 0x%02X.\n", status); 

...
    
/* soft reset */
res = ccs811_basic_soft_reset();
if (res != 0)
{
    (void)ccs811_basic_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: soft reset.\n");

...

/* hard reset */
res = ccs811_basic_hard_reset();
if (res != 0)
{
    (void)ccs811_basic_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: hard reset.\n");

...

/* wake up */
res = ccs811_basic_wake_up();
if (res != 0)
{
    (void)ccs811_basic_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: wake up.\n");

...

/* power down */
res = ccs811_basic_power_down();
if (res != 0)
{
    (void)ccs811_basic_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: power down.\n");

...
    
/* deinit */
(void)ccs811_basic_deinit();

return 0;
```

#### example update

```C
#include "driver_ccs811_update.h"

uint8_t res;
char path[33] = "update.bin";

/* init */
res = ccs811_update_init(CCS811_ADDRESS_GND);
if (res != 0)
{
    return 1;
}

...
    
/* output */
ccs811_interface_debug_print("ccs811: start updating.\n");

/* update */
res = ccs811_update_firmware(path);
if (res != 0)
{
    (void)ccs811_update_deinit();

    return 1;
}

/* output */
ccs811_interface_debug_print("ccs811: end updating.\n");

...
    
/* deinit */
(void)ccs811_update_deinit();

return 0;
```
#### example interrupt

```C
#include "driver_ccs811_interrupt.h"

uint8_t res;
uint32_t i;
uint8_t hysteresis = 2;
uint16_t high = 500;
uint16_t low = 200;
uint8_t (*g_gpio_irq)(void) = NULL;

static uint8_t a_callback(void)
{
    ccs811_interface_debug_print("ccs811: threshold irq.\n");
    
    return 0;
}


/* init */
gpio_interrupt_init();
g_gpio_irq = a_callback;
res = ccs811_interrupt_init(CCS811_ADDRESS_GND, low, high, hysteresis);
if (res != 0)
{
    g_gpio_irq = NULL;
    (void)gpio_interrupt_deinit();

    return 1;
}

/* delay 5000ms */
ccs811_interface_delay_ms(5000);

...
    
/* loop */
for (i = 0; i < 3; i++)
{
    uint16_t eco2_ppm;
    uint16_t tvoc_ppb;

    /* read data */
    res = ccs811_interrupt_read((uint16_t *)&eco2_ppm, (uint16_t *)&tvoc_ppb);
    if (res != 0)
    {
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
        (void)ccs811_interrupt_deinit();

        return 1;
    }

    /* output */
    ccs811_interface_debug_print("ccs811: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)3);
    ccs811_interface_debug_print("ccs811: co2 eq is %d ppm.\n", eco2_ppm);
    ccs811_interface_debug_print("ccs811: tvoc is %d ppb.\n", tvoc_ppb);

    /* delay 1000ms */
    ccs811_interface_delay_ms(1000);
    
    ...
}

...
    
/* deinit */
g_gpio_irq = NULL;
(void)gpio_interrupt_deinit();
(void)ccs811_interrupt_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/ccs811/index.html](https://www.libdriver.com/docs/ccs811/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。