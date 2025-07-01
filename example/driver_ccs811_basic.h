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
 * @file      driver_ccs811_basic.h
 * @brief     driver ccs811 basic header file
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

#ifndef DRIVER_CCS811_BASIC_H
#define DRIVER_CCS811_BASIC_H

#include "driver_ccs811_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ccs811_example_driver ccs811 example driver function
 * @brief    ccs811 example driver modules
 * @ingroup  ccs811_driver
 * @{
 */

/**
 * @brief ccs811 basic example default definition
 */
#define CCS811_BASIC_DEFAULT_MODE        CCS811_MODE_CONSTANT_POWER        /**< constant power */

/**
 * @brief     basic example init
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ccs811_basic_init(ccs811_address_t addr_pin);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ccs811_basic_deinit(void);

/**
 * @brief      basic example read
 * @param[out] *eco2_ppm pointer to an eco2 ppm buffer
 * @param[out] *tvoc_ppb pointer to a tvoc buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ccs811_basic_read(uint16_t *eco2_ppm, uint16_t *tvoc_ppb);

/**
 * @brief      basic example get hardware id
 * @param[out] *id pointer to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ccs811_basic_get_hardware_id(uint8_t *id);

/**
 * @brief      basic example get hardware version
 * @param[out] *version pointer to a version buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ccs811_basic_get_hardware_version(uint8_t *version);

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
uint8_t ccs811_basic_get_firmware_bootloader_version(uint8_t *major, uint8_t *minor, uint8_t *trivial);

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
uint8_t ccs811_basic_get_firmware_application_version(uint8_t *major, uint8_t *minor, uint8_t *trivial);

/**
 * @brief  basic example soft reset
 * @return status code
 *         - 0 success
 *         - 1 soft reset failed
 * @note   none
 */
uint8_t ccs811_basic_soft_reset(void);

/**
 * @brief  basic example hard reset
 * @return status code
 *         - 0 success
 *         - 1 hard reset failed
 * @note   none
 */
uint8_t ccs811_basic_hard_reset(void);

/**
 * @brief  basic example wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t ccs811_basic_wake_up(void);

/**
 * @brief  basic example power down
 * @return status code
 *         - 0 success
 *         - 1 power down failed
 * @note   none
 */
uint8_t ccs811_basic_power_down(void);

/**
 * @brief      basic example get status
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 * @note       none
 */
uint8_t ccs811_basic_get_status(uint8_t *status);

/**
 * @brief      basic example get raw data
 * @param[out] *ua pointer to a ua buffer
 * @param[out] *adc_v pointer to an adc converted data buffer in voltage
 * @return     status code
 *             - 0 success
 *             - 1 get raw data failed
 * @note       none
 */
uint8_t ccs811_basic_get_raw_data(uint8_t *ua, float *adc_v);

/**
 * @brief     basic example set environment data
 * @param[in] humidity humidity in percentage
 * @param[in] temperature temperature in degrees
 * @return    status code
 *            - 0 success
 *            - 1 set environment data failed
 * @note      none
 */
uint8_t ccs811_basic_set_environment_data(float humidity, float temperature);

/**
 * @brief     basic example set baseline
 * @param[in] baseline input baseline
 * @return    status code
 *            - 0 success
 *            - 1 set baseline failed
 * @note      none
 */
uint8_t ccs811_basic_set_baseline(uint16_t baseline);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
