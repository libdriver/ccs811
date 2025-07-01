[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver CCS811
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ccs811/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der CCS811 ist eine digitale Gassensorlösung mit extrem niedrigem Stromverbrauch. Er integriert einen Metalloxid-Gassensor (MOX) zur Erkennung einer Vielzahl flüchtiger organischer Verbindungen (VOCs) zur Überwachung der Raumluftqualität. Er verfügt über eine Mikrocontroller-Einheit (MCU), die einen Analog-Digital-Wandler (ADC) und eine I²C-Schnittstelle umfasst. CCS811 basiert auf der einzigartigen Micro-Hotplate-Technologie von ams, die eine hochzuverlässige Lösung für Gassensoren, sehr schnelle Zykluszeiten und eine deutliche Reduzierung des durchschnittlichen Stromverbrauchs ermöglicht. Die integrierte MCU verwaltet die Sensor-Ansteuerungsmodi und die während der VOC-Erkennung gemessenen Sensor-Rohdaten. Die digitale I²C-Schnittstelle vereinfacht das Hard- und Software-Design erheblich und ermöglicht so eine schnellere Markteinführung. CCS811 unterstützt intelligente Algorithmen zur Verarbeitung der Sensor-Rohmesswerte und gibt einen TVOC-Wert oder äquivalente CO2-Werte (eCO2) aus, wobei die Hauptursache für VOCs der Mensch ist. CCS811 unterstützt mehrere Messmodi, die für einen niedrigen Stromverbrauch während einer aktiven Sensormessung und im Leerlaufmodus optimiert sind und so die Batterielebensdauer in tragbaren Anwendungen verlängern. CCS811 ist in einem 10-poligen LGA-Gehäuse mit den Abmessungen 2,7 mm x 4,0 mm x 1,1 mm und einem Rastermaß von 0,6 mm erhältlich.

LibDriver CCS811 ist ein voll funktionsfähiger Treiber von CCS811, der von LibDriver gestartet wurde. Er bietet TVOC, eCO2 Messung, Temperatur- und Feuchtigkeitskorrektur und andere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example update](#example-update)
    - [example interrupt](#example-interrupt)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver CCS811-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver CCS811 IIC.

/test enthält den Testcode des LibDriver CCS811-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver CCS811-Beispielcode.

/doc enthält das LibDriver CCS811-Offlinedokument.

/Datenblatt enthält CCS811-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/ccs811/index.html](https://www.libdriver.com/docs/ccs811/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.