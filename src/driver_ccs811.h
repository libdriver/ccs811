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
 * @file      driver_ccs811.h
 * @brief     driver ccs811 header file
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

#ifndef DRIVER_CCS811_H
#define DRIVER_CCS811_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ccs811_driver ccs811 driver function
 * @brief    ccs811 driver modules
 * @{
 */

/**
 * @addtogroup ccs811_base_driver
 * @{
 */

/**
 * @brief ccs811 address enumeration definition
 */
typedef enum
{
    CCS811_ADDRESS_GND = (0x5A << 1),        /**< addr pin connected to the GND */
    CCS811_ADDRESS_VCC = (0x5B << 1),        /**< addr pin connected to the VCC */
} ccs811_address_t;

/**
 * @brief ccs811 bool enumeration definition
 */
typedef enum
{
    CCS811_BOOL_FALSE = 0x00,        /**< disable */
    CCS811_BOOL_TRUE  = 0x01,        /**< true */
} ccs811_bool_t;

/**
 * @brief ccs811 mode enumeration definition
 */
typedef enum
{
    CCS811_MODE_IDLE                     = 0x00,        /**< idle */
    CCS811_MODE_CONSTANT_POWER           = 0x01,        /**< constant power */
    CCS811_MODE_PULSE_HEATING            = 0x02,        /**< pulse heating */
    CCS811_MODE_LOW_POWER_PULSE_HEATING  = 0x03,        /**< low power pulse heating */
    CCS811_MODE_HIGH_CONSTANT_POWER      = 0x04,        /**< high constant power */
} ccs811_mode_t;

/**
 * @brief ccs811 status enumeration definition
 */
typedef enum
{
    CCS811_STATUS_FW_MODE    = (1 << 7),        /**< fw mode */
    CCS811_STATUS_APP_VALID  = (1 << 4),        /**< app valid */
    CCS811_STATUS_DATA_READY = (1 << 3),        /**< data ready */
    CCS811_STATUS_ERROR      = (1 << 0),        /**< error */
} ccs811_status_t;

/**
 * @brief ccs811 interrupt enumeration definition
 */
typedef enum
{
    CCS811_INTERRUPT_READY     = 0x03,        /**< ready */
    CCS811_INTERRUPT_THRESHOLD = 0x02,        /**< threshold */
} ccs811_interrupt;

/**
 * @brief ccs811 error enumeration definition
 */
typedef enum
{
    CCS811_ERROR_WRITE_REG_INVALID = (1 << 0),        /**< write register invalid */
    CCS811_ERROR_READ_REG_INVALID  = (1 << 1),        /**< read register invalid */
    CCS811_ERROR_MEASMODE_INVALID  = (1 << 2),        /**< meas mode invalid */
    CCS811_ERROR_MAX_RESISTANCE    = (1 << 3),        /**< max resistance */
    CCS811_ERROR_HEATER_FAULT      = (1 << 4),        /**< heater fault */
    CCS811_ERROR_HEATER_SUPPLY     = (1 << 5),        /**< heater supply */
} ccs811_error_t;

/**
 * @brief ccs811 handle structure definition
 */
typedef struct ccs811_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*gpio_wake_init)(void);                                                    /**< point to a gpio_wake_init function address */
    uint8_t (*gpio_wake_deinit)(void);                                                  /**< point to a gpio_wake_deinit function address */
    uint8_t (*gpio_wake_write)(uint8_t level);                                          /**< point to a gpio_wake_write function address */
    uint8_t (*gpio_reset_init)(void);                                                   /**< point to a gpio_reset_init function address */
    uint8_t (*gpio_reset_deinit)(void);                                                 /**< point to a gpio_reset_deinit function address */
    uint8_t (*gpio_reset_write)(uint8_t level);                                         /**< point to a gpio_reset_write function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    uint8_t (*bin_read_init)(char *name, uint32_t *size);                               /**< point to a bin_read_init function address */
    uint8_t (*bin_read)(uint32_t addr, uint16_t size, uint8_t *buffer);                 /**< point to a bin_read function address */
    uint8_t (*bin_read_deinit)(void);                                                   /**< point to a bin_read_deinit function address */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t iic_addr;                                                                   /**< iic address */
    uint8_t buf[256];                                                                   /**< inner buffer */
} ccs811_handle_t;

/**
 * @brief ccs811 information structure definition
 */
typedef struct ccs811_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ccs811_info_t;

/**
 * @}
 */

/**
 * @defgroup ccs811_link_driver ccs811 link driver function
 * @brief    ccs811 link driver modules
 * @ingroup  ccs811_driver
 * @{
 */

/**
 * @brief     initialize ccs811_handle_t structure
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] STRUCTURE ccs811_handle_t
 * @note      none
 */
#define DRIVER_CCS811_LINK_INIT(HANDLE, STRUCTURE)            memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_IIC_INIT(HANDLE, FUC)             (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_IIC_DEINIT(HANDLE, FUC)           (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to an iic_write function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_IIC_WRITE(HANDLE, FUC)            (HANDLE)->iic_write = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to an iic_read function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_IIC_READ(HANDLE, FUC)             (HANDLE)->iic_read = FUC

/**
 * @brief     link gpio_wake_init function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to a gpio_wake_init function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_GPIO_WAKE_INIT(HANDLE, FUC)       (HANDLE)->gpio_wake_init = FUC

/**
 * @brief     link gpio_wake_deinit function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to a gpio_wake_deinit function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_GPIO_WAKE_DEINIT(HANDLE, FUC)     (HANDLE)->gpio_wake_deinit = FUC

/**
 * @brief     link gpio_wake_write function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to a gpio_wake_write function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_GPIO_WAKE_WRITE(HANDLE, FUC)      (HANDLE)->gpio_wake_write = FUC

/**
 * @brief     link gpio_reset_init function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to a gpio_reset_init function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_GPIO_RESET_INIT(HANDLE, FUC)      (HANDLE)->gpio_reset_init = FUC

/**
 * @brief     link gpio_reset_deinit function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to a gpio_reset_deinit function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_GPIO_RESET_DEINIT(HANDLE, FUC)    (HANDLE)->gpio_reset_deinit = FUC

/**
 * @brief     link gpio_reset_write function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to a gpio_reset_write function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_GPIO_RESET_WRITE(HANDLE, FUC)     (HANDLE)->gpio_reset_write = FUC

/**
 * @brief     link bin_read_init function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to a bin_read_init function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_BIN_READ_INIT(HANDLE, FUC)        (HANDLE)->bin_read_init = FUC

/**
 * @brief     link bin_read_deinit function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to a bin_read_deinit function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_BIN_READ_DEINIT(HANDLE, FUC)      (HANDLE)->bin_read_deinit = FUC

/**
 * @brief     link bin_read function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to a bin_read function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_BIN_READ(HANDLE, FUC)             (HANDLE)->bin_read = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_DELAY_MS(HANDLE, FUC)             (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to a ccs811 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_CCS811_LINK_DEBUG_PRINT(HANDLE, FUC)          (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup ccs811_base_driver ccs811 base driver function
 * @brief    ccs811 base driver modules
 * @ingroup  ccs811_driver
 * @{
 */

/**
 * @brief      get chip information
 * @param[out] *info pointer to a ccs811 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ccs811_info(ccs811_info_t *info);

/**
 * @brief     set address pin
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ccs811_set_addr_pin(ccs811_handle_t *handle, ccs811_address_t addr_pin);

/**
 * @brief      get address pin
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ccs811_get_addr_pin(ccs811_handle_t *handle, ccs811_address_t *addr_pin);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to a ccs811 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or gpio initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is error
 *            - 5 firmware loaded failed
 * @note      none
 */
uint8_t ccs811_init(ccs811_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to a ccs811 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 reset failed
 * @note      none
 */
uint8_t ccs811_deinit(ccs811_handle_t *handle);

/**
 * @brief      read data
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *eco2_ppm pointer to an eco2 ppm buffer
 * @param[out] *tvoc_ppb pointer to a tvoc ppb buffer
 * @param[out] *raw pointer to a raw buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 find error
 *             - 5 data is not ready
 * @note       none
 */
uint8_t ccs811_read(ccs811_handle_t *handle, uint16_t *eco2_ppm, uint16_t *tvoc_ppb, uint16_t *raw);

/**
 * @brief      get raw data
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *raw pointer to a raw buffer
 * @param[out] *ua pointer to a ua buffer
 * @param[out] *adc_raw pointer to an adc raw buffer
 * @param[out] *adc_v pointer to an adc converted data buffer in voltage
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 find error
 *             - 5 data is not ready
 * @note       none
 */
uint8_t ccs811_get_raw_data(ccs811_handle_t *handle, uint16_t *raw, uint8_t *ua, uint16_t *adc_raw, float *adc_v);

/**
 * @brief      get status
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_get_status(ccs811_handle_t *handle, uint8_t *status);

/**
 * @brief     set mode
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] mode chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ccs811_set_mode(ccs811_handle_t *handle, ccs811_mode_t mode);

/**
 * @brief      get mode
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *mode pointer to a mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_get_mode(ccs811_handle_t *handle, ccs811_mode_t *mode);

/**
 * @brief     enable or disable interrupt data ready
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt data ready failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ccs811_set_interrupt_data_ready(ccs811_handle_t *handle, ccs811_bool_t enable);

/**
 * @brief      get interrupt data ready status
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *enable pointer to a bool value
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt data ready failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_get_interrupt_data_ready(ccs811_handle_t *handle, ccs811_bool_t *enable);

/**
 * @brief     enable or disable interrupt threshold
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ccs811_set_interrupt_threshold(ccs811_handle_t *handle, ccs811_bool_t enable);

/**
 * @brief      get interrupt threshold status
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *enable pointer to a bool value
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_get_interrupt_threshold(ccs811_handle_t *handle, ccs811_bool_t *enable);

/**
 * @brief     set environment data
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] humidity_raw input humidity raw
 * @param[in] temperature_raw input temperature raw
 * @return    status code
 *            - 0 success
 *            - 1 set environment data failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ccs811_set_environment_data(ccs811_handle_t *handle, uint16_t humidity_raw, uint16_t temperature_raw);

/**
 * @brief     set eco2 threshold
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] low_medium_ppm low to medium threshold in ppm
 * @param[in] medium_high_ppm medium to high threshold in ppm
 * @param[in] hysteresis input hysteresis
 * @return    status code
 *            - 0 success
 *            - 1 set eco2 threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ccs811_set_eco2_threshold(ccs811_handle_t *handle, uint16_t low_medium_ppm, uint16_t medium_high_ppm, uint8_t hysteresis);

/**
 * @brief     set baseline
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] baseline input baseline
 * @return    status code
 *            - 0 success
 *            - 1 set baseline failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ccs811_set_baseline(ccs811_handle_t *handle, uint16_t baseline);

/**
 * @brief      get baseline
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *baseline pointer to a baseline buffer
 * @return     status code
 *             - 0 success
 *             - 1 get baseline failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_get_baseline(ccs811_handle_t *handle, uint16_t *baseline);

/**
 * @brief      get error id
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *id pointer to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get error id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_get_error_id(ccs811_handle_t *handle, uint8_t *id);

/**
 * @brief      get hardware id
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *id pointer to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get hardware id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_get_hardware_id(ccs811_handle_t *handle, uint8_t *id);

/**
 * @brief      get hardware version
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *version pointer to a version buffer
 * @return     status code
 *             - 0 success
 *             - 1 get hardware version failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_get_hardware_version(ccs811_handle_t *handle, uint8_t *version);

/**
 * @brief      get firmware bootloader version
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *major pointer to a major buffer
 * @param[out] *minor pointer to a minor buffer
 * @param[out] *trivial pointer to a trivial buffer
 * @return     status code
 *             - 0 success
 *             - 1 get firmware bootloader version failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_get_firmware_bootloader_version(ccs811_handle_t *handle, uint8_t *major, uint8_t *minor, uint8_t *trivial);

/**
 * @brief      get firmware application version
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *major pointer to a major buffer
 * @param[out] *minor pointer to a minor buffer
 * @param[out] *trivial pointer to a trivial buffer
 * @return     status code
 *             - 0 success
 *             - 1 get firmware application version failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_get_firmware_application_version(ccs811_handle_t *handle, uint8_t *major, uint8_t *minor, uint8_t *trivial);

/**
 * @brief     soft reset
 * @param[in] *handle pointer to a ccs811 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ccs811_soft_reset(ccs811_handle_t *handle);

/**
 * @brief     wake up
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 wake up failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ccs811_wake_up(ccs811_handle_t *handle, ccs811_bool_t enable);

/**
 * @brief     hard reset
 * @param[in] *handle pointer to a ccs811 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 hard reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ccs811_hard_reset(ccs811_handle_t *handle);

/**
 * @brief      convert real data format to register data format
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[in]  humidity humidity in percentage
 * @param[out] *reg pointer to a converted data buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_environment_humidity_convert_to_register(ccs811_handle_t *handle, float humidity, uint16_t *reg);

/**
 * @brief      convert register data to real data format
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[in]  reg register data 
 * @param[out] *humidity pointer to a real data buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_environment_humidity_convert_to_data(ccs811_handle_t *handle, uint16_t reg, float *humidity);

/**
 * @brief      convert real data format to register data format
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[in]  temperature temperature in degrees
 * @param[out] *reg pointer to a converted data buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_environment_temperature_convert_to_register(ccs811_handle_t *handle, float temperature, uint16_t *reg);

/**
 * @brief      convert register data to real data format
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[in]  reg register data 
 * @param[out] *temperature pointer to a real data buffer in degrees
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_environment_temperature_convert_to_data(ccs811_handle_t *handle, uint16_t reg, float *temperature);

/**
 * @}
 */

/**
 * @defgroup ccs811_app_driver ccs811 app driver function
 * @brief    ccs811 app driver modules
 * @ingroup  ccs811_driver
 * @{
 */

/**
 * @brief     app erase
 * @param[in] *handle pointer to a ccs811 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 app erase failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 erase timeout
 * @note      none
 */
uint8_t ccs811_app_erase(ccs811_handle_t *handle);

/**
 * @brief     app verify
 * @param[in] *handle pointer to a ccs811 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 app verify failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 verify timeout
 * @note      none
 */
uint8_t ccs811_app_verify(ccs811_handle_t *handle);

/**
 * @brief     app start
 * @param[in] *handle pointer to a ccs811 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 app start failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start timeout
 * @note      none
 */
uint8_t ccs811_app_start(ccs811_handle_t *handle);

/**
 * @brief     app program
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] *rom pointer to a rom buffer
 * @param[in] len rom length
 * @return    status code
 *            - 0 success
 *            - 1 app program failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len is invalid
 * @note      none
 */
uint8_t ccs811_app_program(ccs811_handle_t *handle, uint8_t *rom, uint32_t len);

/**
 * @brief     app firmware initialize
 * @param[in] *handle pointer to a ccs811 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or gpio initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is error
 * @note      none
 */
uint8_t ccs811_app_firmware_init(ccs811_handle_t *handle);

/**
 * @brief     app firmware update
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] *path pointer a path buffer
 * @return    status code
 *            - 0 success
 *            - 1 update failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 size is invalid
 *            - 5 update timeout
 * @note      none
 */
uint8_t ccs811_app_firmware_update(ccs811_handle_t *handle, char *path);

/**
 * @}
 */

/**
 * @defgroup ccs811_extern_driver ccs811 extern driver function
 * @brief    ccs811 extern driver modules
 * @ingroup  ccs811_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ccs811_set_reg(ccs811_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ccs811_get_reg(ccs811_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
