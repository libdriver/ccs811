[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver CCS811

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ccs811/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

CCS811は、超低消費電力デジタルガスセンサソリューションです。金属酸化物（MOX）ガスセンサを統合し、マイクロコントローラユニット（MCU）を使用して屋内空気質モニタリング用の幅広い揮発性有機化合物（VOC）を検出します。アナログ-デジタルコンバータ（ADC）とI²Cインターフェースを備えています。CCS811は、ams独自のマイクロホットプレート技術に基づいており、ガスセンサ向けの信頼性の高いソリューション、非常に高速なサイクルタイム、平均消費電力の大幅な削減を実現します。統合型MCUは、センサの駆動モードと、VOC検出中に測定された生のセンサデータを管理します。I²Cデジタルインターフェースはハードウェアとソフトウェアの設計を大幅に簡素化し、市場投入までの時間を短縮します。CCS811は、生のセンサ測定値を処理してTVOC値または同等のCO2（eCO2）レベルを出力するインテリジェントアルゴリズムをサポートしています。VOCの主な原因は人間に起因します。CCS811は、アクティブセンサ測定中の低消費電力と、ポータブルアプリケーションでのバッテリ寿命を延ばすアイドルモードに最適化された複数の測定モードをサポートしています。 CCS811 は、10 リード 2.7mm x 4.0mm x 1.1mm、0.6mm ピッチ LGA パッケージで提供されます。

LibDriver CCS811は、LibDriverによって起動されたCCS811の全機能ドライバーであり、TVOC、eCO2読み取り、温度および湿度補正、およびその他の機能を提供します。LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example update](#example-update)
    - [example interrupt](#example-interrupt)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver CCS811のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver CCS811用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver CCS811ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver CCS811プログラミング例が含まれています。

/ docディレクトリには、LibDriver CCS811オフラインドキュメントが含まれています。

/ datasheetディレクトリには、CCS811データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/ccs811/index.html](https://www.libdriver.com/docs/ccs811/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。