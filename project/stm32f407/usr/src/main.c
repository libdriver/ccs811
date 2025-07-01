/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2025-06-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2025/06/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ccs811_basic.h"
#include "driver_ccs811_update.h"
#include "driver_ccs811_interrupt.h"
#include "driver_ccs811_register_test.h"
#include "driver_ccs811_read_test.h"
#include "driver_ccs811_update_test.h"
#include "driver_ccs811_interrupt_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "gpio.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        if (g_gpio_irq != NULL)
        {
            g_gpio_irq();
        }
    }
}

/**
 * @brief  callback
 * @return status code
 *         - 0 success
 * @note   none
 */
static uint8_t a_callback(void)
{
    ccs811_interface_debug_print("ccs811: threshold irq.\n");
    
    return 0;
}

/**
 * @brief     ccs811 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ccs811(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"baseline", required_argument, NULL, 2},
        {"bin", required_argument, NULL, 3},
        {"high", required_argument, NULL, 4},
        {"hysteresis", required_argument, NULL, 5},
        {"humidity", required_argument, NULL, 6},
        {"low", required_argument, NULL, 7},
        {"temperature", required_argument, NULL, 8},
        {"times", required_argument, NULL, 9},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    char path[33] = "update.bin";
    uint8_t hysteresis = 2;
    uint16_t baseline = 0;
    uint32_t times = 3;
    uint16_t high = 500;
    uint16_t low = 200;
    float temperature = 25.0f;
    float humidity = 50.0f;
    ccs811_address_t addr_pin = CCS811_ADDRESS_GND;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* addr */
            case 1 :
            {
                if (strcmp(optarg, "0") == 0)
                {
                    addr_pin = CCS811_ADDRESS_GND;
                }
                else if (strcmp(optarg, "1") == 0)
                {
                    addr_pin = CCS811_ADDRESS_VCC;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* baseline */
            case 2 :
            {
                /* set the baseline */
                baseline = (uint16_t)atol(optarg);
                
                break;
            }
            
            /* bin */
            case 3 :
            {
                strcpy(path, optarg);
                
                break;
            }
             
            /* high */
            case 4 :
            {
                high = (uint16_t)atol(optarg);
                
                break;
            }
            
            /* hysteresis */
            case 5 :
            {
                /* set the hysteresis */
                hysteresis = (uint8_t)atol(optarg);
                
                break;
            }
            
            /* humidity */
            case 6 :
            {
                /* set the humidity */
                humidity = (float)atof(optarg);
                
                break;
            }
            
            /* low */
            case 7 :
            {
                /* set the low */
                low = (uint16_t)atol(optarg);
                
                break;
            }
            
            /* temperature */
            case 8 :
            {
                /* set the temperature */
                temperature = (float)atof(optarg);
                
                break;
            }
            
            /* times */
            case 9 :
            {
                /* set the times */
                times = (uint32_t)atol(optarg);
                
                break;
            }
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (ccs811_register_test(addr_pin) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (ccs811_read_test(addr_pin, times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_update", type) == 0)
    {
        /* run update test */
        if (ccs811_update_test(addr_pin, path) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_int", type) == 0)
    {
        /* run int test */
        if (ccs811_interrupt_test(addr_pin, low, high, hysteresis, times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 5000ms */
        ccs811_interface_delay_ms(5000);
        
        /* loop */
        for (i = 0; i < times; i++)
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
            ccs811_interface_debug_print("ccs811: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            ccs811_interface_debug_print("ccs811: co2 eq is %d ppm.\n", eco2_ppm);
            ccs811_interface_debug_print("ccs811: tvoc is %d ppb.\n", tvoc_ppb);
            
            /* delay 1000ms */
            ccs811_interface_delay_ms(1000);
        }
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_raw-data", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 5000ms */
        ccs811_interface_delay_ms(5000);
        
        /* loop */
        for (i = 0; i < times; i++)
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
            ccs811_interface_debug_print("ccs811: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            ccs811_interface_debug_print("ccs811: ua is %duA.\n", ua);
            ccs811_interface_debug_print("ccs811: adc is %.02fV.\n", adc_v);
            
            /* delay 1000ms */
            ccs811_interface_delay_ms(1000);
        }
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_env", type) == 0)
    {
        uint8_t res;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
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
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_baseline", type) == 0)
    {
        uint8_t res;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* set baseline */
        res = ccs811_basic_set_baseline(baseline);
        if (res != 0)
        {
            (void)ccs811_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ccs811_interface_debug_print("ccs811: set baseline %d.\n", baseline);
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_hardware-id", type) == 0)
    {
        uint8_t res;
        uint8_t id;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* get hardware id */
        res = ccs811_basic_get_hardware_id(&id);
        if (res != 0)
        {
            (void)ccs811_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ccs811_interface_debug_print("ccs811: hardware id is 0x%02X.\n", id);
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_hardware-version", type) == 0)
    {
        uint8_t res;
        uint8_t version;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* get hardware version */
        res = ccs811_basic_get_hardware_version(&version);
        if (res != 0)
        {
            (void)ccs811_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ccs811_interface_debug_print("ccs811: hardware version is 0x%02X.\n", version);
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_bootloader-version", type) == 0)
    {
        uint8_t res;
        uint8_t major;
        uint8_t minor;
        uint8_t trivial;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
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
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_application-version", type) == 0)
    {
        uint8_t res;
        uint8_t major;
        uint8_t minor;
        uint8_t trivial;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
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
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_soft-reset", type) == 0)
    {
        uint8_t res;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* soft reset */
        res = ccs811_basic_soft_reset();
        if (res != 0)
        {
            (void)ccs811_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ccs811_interface_debug_print("ccs811: soft reset.\n");
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_hard-reset", type) == 0)
    {
        uint8_t res;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* hard reset */
        res = ccs811_basic_hard_reset();
        if (res != 0)
        {
            (void)ccs811_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ccs811_interface_debug_print("ccs811: hard reset.\n");
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_wake-up", type) == 0)
    {
        uint8_t res;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* wake up */
        res = ccs811_basic_wake_up();
        if (res != 0)
        {
            (void)ccs811_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ccs811_interface_debug_print("ccs811: wake up.\n");
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_power-down", type) == 0)
    {
        uint8_t res;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* power down */
        res = ccs811_basic_power_down();
        if (res != 0)
        {
            (void)ccs811_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ccs811_interface_debug_print("ccs811: power down.\n");
        
        return 0;
    }
    else if (strcmp("e_status", type) == 0)
    {
        uint8_t res;
        uint8_t status;
        
        /* init */
        res = ccs811_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* get status */
        res = ccs811_basic_get_status(&status);
        if (res != 0)
        {
            (void)ccs811_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ccs811_interface_debug_print("ccs811: status is 0x%02X.\n", status);
        
        /* deinit */
        (void)ccs811_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_update", type) == 0)
    {
        uint8_t res;
        
        /* init */
        res = ccs811_update_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
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
        
        /* deinit */
        (void)ccs811_update_deinit();
        
        return 0;
    }
    else if (strcmp("e_int", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* init */
        gpio_interrupt_init();
        g_gpio_irq = a_callback;
        res = ccs811_interrupt_init(addr_pin, low, high, hysteresis);
        if (res != 0)
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* delay 5000ms */
        ccs811_interface_delay_ms(5000);
        
        /* loop */
        for (i = 0; i < times; i++)
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
            ccs811_interface_debug_print("ccs811: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            ccs811_interface_debug_print("ccs811: co2 eq is %d ppm.\n", eco2_ppm);
            ccs811_interface_debug_print("ccs811: tvoc is %d ppb.\n", tvoc_ppb);
            
            /* delay 1000ms */
            ccs811_interface_delay_ms(1000);
        }
        
        /* deinit */
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
        (void)ccs811_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ccs811_interface_debug_print("Usage:\n");
        ccs811_interface_debug_print("  ccs811 (-i | --information)\n");
        ccs811_interface_debug_print("  ccs811 (-h | --help)\n");
        ccs811_interface_debug_print("  ccs811 (-p | --port)\n");
        ccs811_interface_debug_print("  ccs811 (-t reg | --test=reg) [--addr=<0 | 1>]\n");
        ccs811_interface_debug_print("  ccs811 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]\n");
        ccs811_interface_debug_print("  ccs811 (-t update | --test=update) [--addr=<0 | 1>] [--bin=<path>]\n");
        ccs811_interface_debug_print("  ccs811 (-t int | --test=int) [--addr=<0 | 1>] [--low=<low-threshold>]\n"); 
        ccs811_interface_debug_print("         [--high=<high-threshold>] [--hysteresis=<hysteresis>] [--times=<num>]\n");
        ccs811_interface_debug_print("  ccs811 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]\n");
        ccs811_interface_debug_print("  ccs811 (-e raw-data | --example=raw-data) [--addr=<0 | 1>] [--times=<num>]\n");
        ccs811_interface_debug_print("  ccs811 (-e env | --example=env) [--addr=<0 | 1>] [--humidity=<rh>] [--temperature=<degrees>]\n");
        ccs811_interface_debug_print("  ccs811 (-e baseline | --example=baseline) [--addr=<0 | 1>] [--baseline=<baseline>]\n");
        ccs811_interface_debug_print("  ccs811 (-e hardware-id | --example=hardware-id) [--addr=<0 | 1>]\n");
        ccs811_interface_debug_print("  ccs811 (-e hardware-version | --example=hardware-version) [--addr=<0 | 1>]\n");
        ccs811_interface_debug_print("  ccs811 (-e bootloader-version | --example=bootloader-version) [--addr=<0 | 1>]\n");
        ccs811_interface_debug_print("  ccs811 (-e application-version | --example=application-version) [--addr=<0 | 1>]\n");
        ccs811_interface_debug_print("  ccs811 (-e soft-reset | --example=soft-reset) [--addr=<0 | 1>]\n");
        ccs811_interface_debug_print("  ccs811 (-e hard-reset | --example=hard-reset) [--addr=<0 | 1>]\n");
        ccs811_interface_debug_print("  ccs811 (-e wake-up | --example=wake-up) [--addr=<0 | 1>]\n");
        ccs811_interface_debug_print("  ccs811 (-e power-down | --example=power-down) [--addr=<0 | 1>]\n");
        ccs811_interface_debug_print("  ccs811 (-e status | --example=status) [--addr=<0 | 1>]\n");
        ccs811_interface_debug_print("  ccs811 (-e update | --example=update) [--addr=<0 | 1>] [--bin=<path>]\n");
        ccs811_interface_debug_print("  ccs811 (-e int | --example=int) [--addr=<0 | 1>] [--low=<low-threshold>]\n");
        ccs811_interface_debug_print("         [--high=<high-threshold>] [--hysteresis=<hysteresis>] [--times=<num>]\n");
        ccs811_interface_debug_print("\n");
        ccs811_interface_debug_print("Options:\n");
        ccs811_interface_debug_print("      --addr=<0 | 1>                 Set the chip address.([default: 0])\n");
        ccs811_interface_debug_print("      --baseline=<baseline>          Set the baseline value.([default: 0])\n");
        ccs811_interface_debug_print("      --bin=<path>                   Set the firmware binary file path.([default: update.bin])\n");
        ccs811_interface_debug_print("  -e <read | raw-data | env | baseline | hardware-id | hardware-version | bootloader-version |\n"); 
        ccs811_interface_debug_print("      application-version | soft-reset | hard-reset | wake-up | power-down | status | update | int>,\n"); 
        ccs811_interface_debug_print("      --example=<read | raw-data | env | baseline | hardware-id | hardware-version | bootloader-version |\n"); 
        ccs811_interface_debug_print("      application-version | soft-reset | hard-reset | wake-up | power-down | status | update | int>\n");
        ccs811_interface_debug_print("                                     Run the driver example.\n");
        ccs811_interface_debug_print("  -h, --help                         Show the help.\n");
        ccs811_interface_debug_print("      --high=<high-threshold>        Set the high threshold.([default: 500])\n");
        ccs811_interface_debug_print("      --hysteresis=<hysteresis>      Set the hysteresis.([default: 2])\n");
        ccs811_interface_debug_print("      --humidity=<rh>                Set the relative humidity.([default: 50])\n");
        ccs811_interface_debug_print("  -i, --information                  Show the chip information.\n");
        ccs811_interface_debug_print("      --low=<low-threshold>          Set the low threshold.([default: 200])\n");
        ccs811_interface_debug_print("  -p, --port                         Display the pin connections of the current board.\n");
        ccs811_interface_debug_print("  -t <reg | read | update | int>, --test=<reg | read | update | int>\n");
        ccs811_interface_debug_print("                                     Run the driver test.\n");
        ccs811_interface_debug_print("      --temperature=<degrees>        Set the temperature in degrees celsius.([default: 25])\n");
        ccs811_interface_debug_print("      --times=<num>                  Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ccs811_info_t info;
        
        /* print ccs811 info */
        ccs811_info(&info);
        ccs811_interface_debug_print("ccs811: chip is %s.\n", info.chip_name);
        ccs811_interface_debug_print("ccs811: manufacturer is %s.\n", info.manufacturer_name);
        ccs811_interface_debug_print("ccs811: interface is %s.\n", info.interface);
        ccs811_interface_debug_print("ccs811: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ccs811_interface_debug_print("ccs811: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ccs811_interface_debug_print("ccs811: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ccs811_interface_debug_print("ccs811: max current is %0.2fmA.\n", info.max_current_ma);
        ccs811_interface_debug_print("ccs811: max temperature is %0.1fC.\n", info.temperature_max);
        ccs811_interface_debug_print("ccs811: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ccs811_interface_debug_print("ccs811: SCL connected to GPIOB PIN8.\n");
        ccs811_interface_debug_print("ccs811: SDA connected to GPIOB PIN9.\n");
        ccs811_interface_debug_print("ccs811: RST connected to GPIOA PIN0.\n");
        ccs811_interface_debug_print("ccs811: WAKE connected to GPIOA PIN8.\n");
        ccs811_interface_debug_print("ccs811: INT connected to GPIOB PIN0.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register ccs811 function */
    shell_init();
    shell_register("ccs811", ccs811);
    uart_print("ccs811: welcome to libdriver ccs811.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("ccs811: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("ccs811: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("ccs811: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("ccs811: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("ccs811: param is invalid.\n");
            }
            else
            {
                uart_print("ccs811: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
