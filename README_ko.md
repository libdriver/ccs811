[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver CCS811

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ccs811/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

CCS811은 실내 공기질 모니터링을 위해 다양한 휘발성 유기 화합물(VOC)을 감지하는 금속 산화물(MOX) 가스 센서를 통합한 초저전력 디지털 가스 센서 솔루션입니다. 이 센서는 아날로그-디지털 변환기(ADC)와 I²C 인터페이스를 포함하는 마이크로컨트롤러 유닛(MCU)을 통해 제공됩니다. CCS811은 ams의 독보적인 마이크로 핫플레이트 기술을 기반으로 가스 센서를 위한 고신뢰성 솔루션, 매우 빠른 사이클 타임, 그리고 평균 전력 소비의 상당한 절감을 구현합니다. 통합 MCU는 VOC 감지 중에 측정된 센서 구동 모드와 원시 센서 데이터를 관리합니다. I²C 디지털 인터페이스는 하드웨어 및 소프트웨어 설계를 크게 간소화하여 제품 출시 기간을 단축합니다. CCS811은 VOC의 주요 원인이 사람인 경우, 원시 센서 측정값을 처리하여 TVOC 값 또는 이에 상응하는 CO2(eCO2) 수치를 출력하는 지능형 알고리즘을 지원합니다. CCS811은 활성 센서 측정 및 유휴 모드 시 저전력 소비에 최적화된 다양한 측정 모드를 지원하여 휴대용 애플리케이션의 배터리 수명을 연장합니다. CCS811은 10리드 2.7mm x 4.0mm x 1.1mm, 0.6mm 피치 LGA 패키지로 제공됩니다.

LibDriver CCS811은 LibDriver에서 출시한 CCS811의 전체 기능 드라이버입니다. TVOC, eCO2판독, 온도 및 습도 보정 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example update](#example-update)
    - [example interrupt](#example-interrupt)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver CCS811의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver CCS811용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver CCS811드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver CCS811프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver CCS811오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 CCS811데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인IIC 버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/ccs811/index.html](https://www.libdriver.com/docs/ccs811/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.