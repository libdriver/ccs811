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
 * @file      driver_ccs811_update.h
 * @brief     driver ccs811 update header file
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

#ifndef DRIVER_CCS811_UPDATE_H
#define DRIVER_CCS811_UPDATE_H

#include "driver_ccs811_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup ccs811_example_driver
 * @{
 */

/**
 * @brief     update example init
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ccs811_update_init(ccs811_address_t addr_pin);

/**
 * @brief  update example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ccs811_update_deinit(void);

/**
 * @brief     update example firmware
 * @param[in] *path pointer a path buffer
 * @return    status code
 *            - 0 success
 *            - 1 update failed
 * @note      none
 */
uint8_t ccs811_update_firmware(char *path);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
