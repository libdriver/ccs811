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
 * @file      driver_ccs811_interrupt_test.c
 * @brief     driver ccs811 interrupt test source file
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

#include "driver_ccs811_interrupt_test.h"

static ccs811_handle_t gs_handle;        /**< ccs811 handle */

/**
 * @brief     interrupt test
 * @param[in] addr_pin address pin
 * @param[in] low_medium_ppm low to medium threshold in ppm
 * @param[in] medium_high_ppm medium to high threshold in ppm
 * @param[in] hysteresis input hysteresis
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ccs811_interrupt_test(ccs811_address_t addr_pin, uint16_t low_medium_ppm, 
                              uint16_t medium_high_ppm, uint8_t hysteresis, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    uint16_t eco2_ppm;
    uint16_t tvoc_ppb;
    ccs811_info_t info;
    
    /* link functions */
    DRIVER_CCS811_LINK_INIT(&gs_handle, ccs811_handle_t);
    DRIVER_CCS811_LINK_IIC_INIT(&gs_handle, ccs811_interface_iic_init);
    DRIVER_CCS811_LINK_IIC_DEINIT(&gs_handle, ccs811_interface_iic_deinit);
    DRIVER_CCS811_LINK_IIC_WRITE(&gs_handle, ccs811_interface_iic_write);
    DRIVER_CCS811_LINK_IIC_READ(&gs_handle, ccs811_interface_iic_read);
    DRIVER_CCS811_LINK_GPIO_WAKE_INIT(&gs_handle, ccs811_interface_gpio_wake_init);
    DRIVER_CCS811_LINK_GPIO_WAKE_DEINIT(&gs_handle, ccs811_interface_gpio_wake_deinit);
    DRIVER_CCS811_LINK_GPIO_WAKE_WRITE(&gs_handle, ccs811_interface_gpio_wake_write);
    DRIVER_CCS811_LINK_GPIO_RESET_INIT(&gs_handle, ccs811_interface_gpio_reset_init);
    DRIVER_CCS811_LINK_GPIO_RESET_DEINIT(&gs_handle, ccs811_interface_gpio_reset_deinit);
    DRIVER_CCS811_LINK_GPIO_RESET_WRITE(&gs_handle, ccs811_interface_gpio_reset_write);
    DRIVER_CCS811_LINK_BIN_READ_INIT(&gs_handle, ccs811_interface_bin_read_init);
    DRIVER_CCS811_LINK_BIN_READ_DEINIT(&gs_handle, ccs811_interface_bin_read_deinit);
    DRIVER_CCS811_LINK_BIN_READ(&gs_handle, ccs811_interface_bin_read);
    DRIVER_CCS811_LINK_DELAY_MS(&gs_handle, ccs811_interface_delay_ms);
    DRIVER_CCS811_LINK_DEBUG_PRINT(&gs_handle, ccs811_interface_debug_print);
    
    /* ccs811 info */
    res = ccs811_info(&info);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        ccs811_interface_debug_print("ccs811: chip is %s.\n", info.chip_name);
        ccs811_interface_debug_print("ccs811: manufacturer is %s.\n", info.manufacturer_name);
        ccs811_interface_debug_print("ccs811: interface is %s.\n", info.interface);
        ccs811_interface_debug_print("ccs811: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ccs811_interface_debug_print("ccs811: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ccs811_interface_debug_print("ccs811: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ccs811_interface_debug_print("ccs811: max current is %0.2fmA.\n", info.max_current_ma);
        ccs811_interface_debug_print("ccs811: max temperature is %0.1fC.\n", info.temperature_max);
        ccs811_interface_debug_print("ccs811: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start interrupt test */
    ccs811_interface_debug_print("ccs811: start interrupt test.\n");
    
    /* set addr pin */
    res = ccs811_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set addr pin failed.\n");
       
        return 1;
    }
    
    /* ccs811 init */
    res = ccs811_init(&gs_handle);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: init failed.\n");
    
        return 1;
    }
    
    /* app start */
    res = ccs811_app_start(&gs_handle);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: start failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set high constant power */
    res = ccs811_set_mode(&gs_handle, CCS811_MODE_HIGH_CONSTANT_POWER);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set mode failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable interrupt data ready */
    res = ccs811_set_interrupt_data_ready(&gs_handle, CCS811_BOOL_FALSE);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set interrupt data ready failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set eco2 threshold */
    res = ccs811_set_eco2_threshold(&gs_handle, low_medium_ppm, medium_high_ppm, hysteresis);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set eco2 threshold failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable interrupt threshold */
    res = ccs811_set_interrupt_threshold(&gs_handle, CCS811_BOOL_TRUE);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set interrupt threshold failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 1s */
    ccs811_interface_delay_ms(1000);
    
    for (i = 0; i < times; i++)
    {
        uint16_t raw;
        
        /* read data */
        res = ccs811_read(&gs_handle, &eco2_ppm, &tvoc_ppb, &raw);
        if (res != 0)
        {
            ccs811_interface_debug_print("ccs811: read failed.\n");
            (void)ccs811_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ccs811_interface_debug_print("ccs811: eco2 ppm is %d ppm, please check int gpio level.\n", eco2_ppm);
        
        /* delay 1s */
        ccs811_interface_delay_ms(1000);
    }
    
    /* finish interrupt test */
    ccs811_interface_debug_print("ccs811: finish interrupt test.\n");
    (void)ccs811_deinit(&gs_handle);
    
    return 0;
}
