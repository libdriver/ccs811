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
 * @file      driver_ccs811_basic.c
 * @brief     driver ccs811 basic source file
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

static ccs811_handle_t gs_handle;        /**< ccs811 handle */

/**
 * @brief     basic example init
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ccs811_basic_init(ccs811_address_t addr_pin)
{
    uint8_t res;
    
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
    
    /* set default mode */
    res = ccs811_set_mode(&gs_handle, CCS811_BASIC_DEFAULT_MODE);
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
    
    /* disable interrupt threshold */
    res = ccs811_set_interrupt_threshold(&gs_handle, CCS811_BOOL_FALSE);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: set interrupt threshold failed.\n");
        (void)ccs811_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ccs811_basic_deinit(void)
{
    /* close ccs811 */
    if (ccs811_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *eco2_ppm pointer to an eco2 ppm buffer
 * @param[out] *tvoc_ppb pointer to a tvoc buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ccs811_basic_read(uint16_t *eco2_ppm, uint16_t *tvoc_ppb)
{
    uint16_t raw;
    
    /* read data */
    if (ccs811_read(&gs_handle, eco2_ppm, tvoc_ppb, &raw) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get status
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 * @note       none
 */
uint8_t ccs811_basic_get_status(uint8_t *status)
{
    if (ccs811_get_status(&gs_handle, status) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get raw data
 * @param[out] *ua pointer to a ua buffer
 * @param[out] *adc_v pointer to an adc converted data buffer in voltage
 * @return     status code
 *             - 0 success
 *             - 1 get raw data failed
 * @note       none
 */
uint8_t ccs811_basic_get_raw_data(uint8_t *ua, float *adc_v)
{
    uint16_t raw;
    uint16_t adc_raw;
    
    /* get raw data */
    if (ccs811_get_raw_data(&gs_handle, &raw, ua, &adc_raw, adc_v) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set environment data
 * @param[in] humidity humidity in percentage
 * @param[in] temperature temperature in degrees
 * @return    status code
 *            - 0 success
 *            - 1 set environment data failed
 * @note      none
 */
uint8_t ccs811_basic_set_environment_data(float humidity, float temperature)
{
    uint16_t humidity_raw;
    uint16_t temperature_raw;
    
    /* humidity convert */
    if (ccs811_environment_humidity_convert_to_register(&gs_handle, humidity, &humidity_raw) != 0)
    {
        return 1;
    }
    
    /* temperature convert */
    if (ccs811_environment_temperature_convert_to_register(&gs_handle, temperature, &temperature_raw) != 0)
    {
        return 1;
    }
    
    /* set environment data */
    if (ccs811_set_environment_data(&gs_handle, humidity_raw, temperature_raw) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set baseline
 * @param[in] baseline input baseline
 * @return    status code
 *            - 0 success
 *            - 1 set baseline failed
 * @note      none
 */
uint8_t ccs811_basic_set_baseline(uint16_t baseline)
{
    if (ccs811_set_baseline(&gs_handle, baseline) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get hardware id
 * @param[out] *id pointer to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ccs811_basic_get_hardware_id(uint8_t *id)
{
    if (ccs811_get_hardware_id(&gs_handle, id) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get hardware version
 * @param[out] *version pointer to a version buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ccs811_basic_get_hardware_version(uint8_t *version)
{
    if (ccs811_get_hardware_version(&gs_handle, version) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get firmware bootloader version
 * @param[out] *major pointer to a major buffer
 * @param[out] *minor pointer to a minor buffer
 * @param[out] *trivial pointer to a trivial buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ccs811_basic_get_firmware_bootloader_version(uint8_t *major, uint8_t *minor, uint8_t *trivial)
{
    if (ccs811_get_firmware_bootloader_version(&gs_handle, major, minor, trivial) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get firmware application version
 * @param[out] *major pointer to a major buffer
 * @param[out] *minor pointer to a minor buffer
 * @param[out] *trivial pointer to a trivial buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ccs811_basic_get_firmware_application_version(uint8_t *major, uint8_t *minor, uint8_t *trivial)
{
    if (ccs811_get_firmware_application_version(&gs_handle, major, minor, trivial) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example soft reset
 * @return status code
 *         - 0 success
 *         - 1 soft reset failed
 * @note   none
 */
uint8_t ccs811_basic_soft_reset(void)
{
    if (ccs811_soft_reset(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example hard reset
 * @return status code
 *         - 0 success
 *         - 1 hard reset failed
 * @note   none
 */
uint8_t ccs811_basic_hard_reset(void)
{
    if (ccs811_hard_reset(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t ccs811_basic_wake_up(void)
{
    if (ccs811_wake_up(&gs_handle, CCS811_BOOL_TRUE) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example power down
 * @return status code
 *         - 0 success
 *         - 1 power down failed
 * @note   none
 */
uint8_t ccs811_basic_power_down(void)
{
    if (ccs811_wake_up(&gs_handle, CCS811_BOOL_FALSE) != 0)
    {
        return 1;
    }
    
    return 0;
}
