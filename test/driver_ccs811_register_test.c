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
 * @file      driver_ccs811_register_test.c
 * @brief     driver ccs811 register test source file
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

#include "driver_ccs811_register_test.h"
#include <stdlib.h>

static ccs811_handle_t gs_handle;        /**< ccs811 handle */

/**
 * @brief     register test
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ccs811_register_test(ccs811_address_t addr_pin)
{
    uint8_t res;
    uint8_t status;
    uint8_t raw8;
    uint8_t id;
    uint8_t version;
    uint8_t major;
    uint8_t minor;
    uint8_t trivial;
    uint16_t raw;
    uint16_t raw_check;
    float f;
    float f_check;
    ccs811_mode_t mode;
    ccs811_address_t addr_pin_check;
    ccs811_bool_t enable;
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
    
    /* start register test */
    ccs811_interface_debug_print("ccs811: start register test.\n");
    
    /* ccs811_set_addr_pin/ccs811_get_addr_pin test */
    ccs811_interface_debug_print("ccs811: ccs811_set_addr_pin/ccs811_get_addr_pin test.\n");
    
    /* set addr pin gnd */
    res = ccs811_set_addr_pin(&gs_handle, CCS811_ADDRESS_GND);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set addr pin failed.\n");
       
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set addr pin gnd.\n");
    res = ccs811_get_addr_pin(&gs_handle, &addr_pin_check);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get addr pin failed.\n");
       
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check addr pin %s.\n", (addr_pin_check == CCS811_ADDRESS_GND) ? "ok" : "error");
    
    /* set addr pin vcc */
    res = ccs811_set_addr_pin(&gs_handle, CCS811_ADDRESS_VCC);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set addr pin failed.\n");
       
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set addr pin vcc.\n");
    res = ccs811_get_addr_pin(&gs_handle, &addr_pin_check);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get addr pin failed.\n");
       
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check addr pin %s.\n", (addr_pin_check == CCS811_ADDRESS_VCC) ? "ok" : "error");
    
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
    
    /* ccs811_get_status test */
    ccs811_interface_debug_print("ccs811: ccs811_get_status test.\n");
    
    /* get status */
    res = ccs811_get_status(&gs_handle, &status);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get status failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: status is 0x%02X.\n", status);
    
    /* ccs811_set_mode/ccs811_get_mode test */
    ccs811_interface_debug_print("ccs811: ccs811_set_mode/ccs811_get_mode test.\n");
    
    /* set high constant power */
    res = ccs811_set_mode(&gs_handle, CCS811_MODE_HIGH_CONSTANT_POWER);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set mode failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set high constant power.\n");
    res = ccs811_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get mode failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check mode %s.\n", (mode == CCS811_MODE_HIGH_CONSTANT_POWER) ? "ok" : "error");
    
    /* set low power pulse heating */
    res = ccs811_set_mode(&gs_handle, CCS811_MODE_LOW_POWER_PULSE_HEATING);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set mode failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set low power pulse heating.\n");
    res = ccs811_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get mode failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check mode %s.\n", (mode == CCS811_MODE_LOW_POWER_PULSE_HEATING) ? "ok" : "error");
    
    /* set pulse heating */
    res = ccs811_set_mode(&gs_handle, CCS811_MODE_PULSE_HEATING);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set mode failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set pulse heating.\n");
    res = ccs811_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get mode failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check mode %s.\n", (mode == CCS811_MODE_PULSE_HEATING) ? "ok" : "error");
    
    /* set constant power */
    res = ccs811_set_mode(&gs_handle, CCS811_MODE_CONSTANT_POWER);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set mode failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set constant power.\n");
    res = ccs811_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get mode failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check mode %s.\n", (mode == CCS811_MODE_CONSTANT_POWER) ? "ok" : "error");
    
    /* set idle */
    res = ccs811_set_mode(&gs_handle, CCS811_MODE_IDLE);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set mode failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set idle.\n");
    res = ccs811_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get mode failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check mode %s.\n", (mode == CCS811_MODE_IDLE) ? "ok" : "error");
    
    /* ccs811_set_interrupt_data_ready/ccs811_get_interrupt_data_ready test */
    ccs811_interface_debug_print("ccs811: ccs811_set_interrupt_data_ready/ccs811_get_interrupt_data_ready test.\n");
    
    /* enable interrupt data ready */
    res = ccs811_set_interrupt_data_ready(&gs_handle, CCS811_BOOL_TRUE);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set interrupt data ready failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: enable interrupt data ready.\n");
    res = ccs811_get_interrupt_data_ready(&gs_handle, &enable);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get interrupt data ready failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check interrupt data ready %s.\n", (enable == CCS811_BOOL_TRUE) ? "ok" : "error");
    
    /* disable interrupt data ready */
    res = ccs811_set_interrupt_data_ready(&gs_handle, CCS811_BOOL_FALSE);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set interrupt data ready failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: disable interrupt data ready.\n");
    res = ccs811_get_interrupt_data_ready(&gs_handle, &enable);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get interrupt data ready failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check interrupt data ready %s.\n", (enable == CCS811_BOOL_FALSE) ? "ok" : "error");
    
    /* ccs811_set_interrupt_threshold/ccs811_get_interrupt_threshold test */
    ccs811_interface_debug_print("ccs811: ccs811_set_interrupt_threshold/ccs811_get_interrupt_threshold test.\n");
    
    /* enable interrupt threshold */
    res = ccs811_set_interrupt_threshold(&gs_handle, CCS811_BOOL_TRUE);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set interrupt threshold failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: enable interrupt threshold.\n");
    res = ccs811_get_interrupt_threshold(&gs_handle, &enable);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get interrupt threshold failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check interrupt threshold %s.\n", (enable == CCS811_BOOL_TRUE) ? "ok" : "error");
    
    /* disable interrupt threshold */
    res = ccs811_set_interrupt_threshold(&gs_handle, CCS811_BOOL_FALSE);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set interrupt threshold failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: disable interrupt threshold.\n");
    res = ccs811_get_interrupt_threshold(&gs_handle, &enable);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get interrupt threshold failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check interrupt threshold %s.\n", (enable == CCS811_BOOL_FALSE) ? "ok" : "error");
    
    /* ccs811_set_environment_data test */
    ccs811_interface_debug_print("ccs811: ccs811_set_environment_data test.\n");
    
    raw = rand() % 0xFFFFU;
    res = ccs811_set_environment_data(&gs_handle, raw, raw);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set environment data failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set raw data 0x%04X.\n", raw);
    ccs811_interface_debug_print("ccs811: check environment data %s.\n", (res == 0) ? "ok" : "error");
    
    /* ccs811_set_eco2_threshold test */
    ccs811_interface_debug_print("ccs811: ccs811_set_eco2_threshold test.\n");
    
    raw = rand() % 0xFFFFU;
    raw8 = rand() % 0xFF;
    res = ccs811_set_eco2_threshold(&gs_handle, raw, raw, raw8);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set eco2 threshold failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set raw data 0x%04X.\n", raw);
    ccs811_interface_debug_print("ccs811: set hysteresis 0x%02X.\n", raw8);
    ccs811_interface_debug_print("ccs811: check eco2 threshold %s.\n", (res == 0) ? "ok" : "error");
    
    /* ccs811_set_baseline/ccs811_get_baseline test */
    ccs811_interface_debug_print("ccs811: ccs811_set_baseline/ccs811_get_baseline test.\n");
    
    raw = 0xFFFFU;
    res = ccs811_set_baseline(&gs_handle, raw);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set baseline failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set baseline 0x%04X.\n", raw);
    res = ccs811_get_baseline(&gs_handle, &raw_check);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get baseline failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check baseline %s.\n", (raw == raw_check) ? "ok" : "error");
    
    /* ccs811_get_hardware_id test */
    ccs811_interface_debug_print("ccs811: ccs811_get_hardware_id test.\n");
    
    /* get hardware id */
    res = ccs811_get_hardware_id(&gs_handle, &id);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get hardware id failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: hardware id is 0x%02X.\n", id);
    
    /* ccs811_get_hardware_version test */
    ccs811_interface_debug_print("ccs811: ccs811_get_hardware_version test.\n");
    
    /* get hardware version */
    res = ccs811_get_hardware_version(&gs_handle, &version);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get hardware version failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: hardware version is 0x%02X.\n", version);
    
    /* ccs811_get_firmware_bootloader_version test */
    ccs811_interface_debug_print("ccs811: ccs811_get_firmware_bootloader_version test.\n");
    
    /* get firmware bootloader version */
    res = ccs811_get_firmware_bootloader_version(&gs_handle, &major, &minor, &trivial);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get firmware bootloader version failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: firmware bootloader version major is 0x%02X.\n", major);
    ccs811_interface_debug_print("ccs811: firmware bootloader version minor is 0x%02X.\n", minor);
    ccs811_interface_debug_print("ccs811: firmware bootloader version trivial is 0x%02X.\n", trivial);
    
    /* ccs811_get_firmware_application_version test */
    ccs811_interface_debug_print("ccs811: ccs811_get_firmware_application_version test.\n");
    
    /* get firmware application version */
    res = ccs811_get_firmware_application_version(&gs_handle, &major, &minor, &trivial);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: get firmware application version failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: firmware application version major is 0x%02X.\n", major);
    ccs811_interface_debug_print("ccs811: firmware application version minor is 0x%02X.\n", minor);
    ccs811_interface_debug_print("ccs811: firmware application version trivial is 0x%02X.\n", trivial);
    
    /* ccs811_soft_reset test */
    ccs811_interface_debug_print("ccs811: ccs811_soft_reset test.\n");
    
    /* soft reset */
    res = ccs811_soft_reset(&gs_handle);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: soft reset failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check soft reset %s.\n", (res == 0) ? "ok" : "error");
    
    /* ccs811_wake_up test */
    ccs811_interface_debug_print("ccs811: ccs811_wake_up test.\n");
    
    /* disable wake up */
    res = ccs811_wake_up(&gs_handle, CCS811_BOOL_FALSE);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: wake up failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: disable wake up.\n");
    ccs811_interface_debug_print("ccs811: check wake up %s.\n", (res == 0) ? "ok" : "error");
    
    /* enable wake up */
    res = ccs811_wake_up(&gs_handle, CCS811_BOOL_TRUE);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: wake up failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: enable wake up.\n");
    ccs811_interface_debug_print("ccs811: check wake up %s.\n", (res == 0) ? "ok" : "error");
    
    /* ccs811_hard_reset test */
    ccs811_interface_debug_print("ccs811: ccs811_hard_reset test.\n");
    
    /* hard reset */
    res = ccs811_hard_reset(&gs_handle);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: hard reset failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check hard reset %s.\n", (res == 0) ? "ok" : "error");
    
    /* ccs811_environment_humidity_convert_to_register/ccs811_environment_humidity_convert_to_data test */
    ccs811_interface_debug_print("ccs811: ccs811_environment_humidity_convert_to_register/ccs811_environment_humidity_convert_to_data test.\n");
    
    f = (float)(rand() % 1000) / 10.0f;
    res = ccs811_environment_humidity_convert_to_register(&gs_handle, f, &raw);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: environment humidity convert to register failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set environment humidity %.02f%%.\n", f);
    res = ccs811_environment_humidity_convert_to_data(&gs_handle, raw, &f_check);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: environment humidity convert to data failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check environment humidity %.02f%%.\n", f_check);
    
    /* ccs811_environment_temperature_convert_to_register/ccs811_environment_temperature_convert_to_data test */
    ccs811_interface_debug_print("ccs811: ccs811_environment_temperature_convert_to_register/ccs811_environment_temperature_convert_to_data test.\n");
    
    f = (float)(rand() % 500) / 10.0f;
    res = ccs811_environment_temperature_convert_to_register(&gs_handle, f, &raw);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: environment temperature convert to register failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: set environment temperature %.02fC.\n", f);
    res = ccs811_environment_temperature_convert_to_data(&gs_handle, raw, &f_check);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: environment temperature convert to data failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    ccs811_interface_debug_print("ccs811: check environment temperature %.02fC.\n", f_check);
    
    /* finish register test */
    ccs811_interface_debug_print("ccs811: finish register test.\n");
    (void)ccs811_deinit(&gs_handle);
    
    return 0;
}
