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
 * @file      driver_ccs811_update.c
 * @brief     driver ccs811 update source file
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

#include "driver_ccs811_update.h"

static ccs811_handle_t gs_handle;        /**< ccs811 handle */

/**
 * @brief     update example init
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ccs811_update_init(ccs811_address_t addr_pin)
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
    
    /* app firmware init */
    res = ccs811_app_firmware_init(&gs_handle);
    if (res != 0)
    {
        ccs811_interface_debug_print("ccs811: app firmware init failed.\n");
    
        return 1;
    }
    
    return 0;
}

/**
 * @brief  update example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ccs811_update_deinit(void)
{
    /* close ccs811 */
    if (ccs811_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     update example firmware
 * @param[in] *path pointer a path buffer
 * @return    status code
 *            - 0 success
 *            - 1 update failed
 * @note      none
 */
uint8_t ccs811_update_firmware(char *path)
{
    /* update firmware */
    if (ccs811_app_firmware_update(&gs_handle, path) != 0)
    {
        return 1;
    }
    
    return 0;
}
