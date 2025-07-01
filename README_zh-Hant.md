[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver CCS811

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ccs811/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

CCS811是一種超低功耗數位氣體感測器解決方案，它集成了一個金屬氧化物（MOX）氣體感測器，用於檢測各種揮發性有機化合物（VOC），用於室內空氣品質監測，並配有一個微控制器單元（MCU），其中包括一個模數轉換器（ADC）和一個I²C介面。 CCS811基於艾邁斯電晶體獨特的微熱板科技，為氣體感測器提供了高度可靠的解決方案，迴圈時間非常快，平均功耗顯著降低。 集成MCU管理感測器驅動模式和檢測VOC時量測的原始感測器數據。 I²C數位介面大大簡化了硬體和軟體設計，縮短了上市時間。 CCS811支持智慧算灋處理原始感測器測量值，以輸出TVOC值或等效的CO2（eCO2）水准。 CCS811支持多種量測模式，這些模式在活動感測器量測期間針對低功耗進行了優化，並在可擕式應用中延長了電池壽命。 CCS811採用10引脚2.7mm x 4.0mm x 1.1mm、間距0.6mm的LGA封裝。

LibDriver CCS811是LibDriver推出的CCS811的全功能驅動，該驅動提供TVOC、eCO2讀取、溫濕度矯正等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example update](#example-update)
    - [example interrupt](#example-interrupt)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver CCS811的源文件。

/interface目錄包含了LibDriver CCS811與平台無關的IIC總線模板。

/test目錄包含了LibDriver CCS811驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver CCS811編程範例。

/doc目錄包含了LibDriver CCS811離線文檔。

/datasheet目錄包含了CCS811數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/ccs811/index.html](https://www.libdriver.com/docs/ccs811/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。