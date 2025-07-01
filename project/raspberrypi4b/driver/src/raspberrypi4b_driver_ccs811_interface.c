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
 * @file      raspberrypi4b_driver_ccs811_interface.c
 * @brief     raspberrypi4b driver ccs811 interface source file
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

#include "driver_ccs811_interface.h"
#include "iic.h"
#include "wire.h"
#include <stdarg.h>

/**
 * @brief iic device name definition
 */
#define IIC_DEVICE_NAME "/dev/i2c-1"        /**< iic device name */

/**
 * @brief iic device handle definition
 */
static int gs_fd;                           /**< iic handle */

/**
 * @brief fp handle definition
 */
static FILE *gs_fp = NULL;                  /**< fp handle */

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t ccs811_interface_iic_init(void)
{
    return iic_init(IIC_DEVICE_NAME, &gs_fd);
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t ccs811_interface_iic_deinit(void)
{
    return iic_deinit(gs_fd);
}

/**
 * @brief     interface iic bus write
 * @param[in] addr iic device write address
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t ccs811_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return iic_write(gs_fd, addr, reg, buf, len);
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr iic device write address
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ccs811_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return iic_read(gs_fd, addr, reg, buf, len);
}

/**
 * @brief interface gpio wake init
 * @note  none
 */
uint8_t ccs811_interface_gpio_wake_init(void)
{
    return wire_init();
}

/**
 * @brief interface gpio wake deinit
 * @note  none
 */
uint8_t ccs811_interface_gpio_wake_deinit(void)
{
    return wire_deinit();
}

/**
 * @brief     interface gpio wake write
 * @param[in] level input level
 * @note      none
 */
uint8_t ccs811_interface_gpio_wake_write(uint8_t level)
{
    return wire_write(level);
}

/**
 * @brief interface gpio reset init
 * @note  none
 */
uint8_t ccs811_interface_gpio_reset_init(void)
{
    return wire_clock_init();
}

/**
 * @brief interface gpio reset deinit
 * @note  none
 */
uint8_t ccs811_interface_gpio_reset_deinit(void)
{
    return wire_clock_deinit();
}

/**
 * @brief     interface gpio reset write
 * @param[in] level input level
 * @note      none
 */
uint8_t ccs811_interface_gpio_reset_write(uint8_t level)
{
    return wire_clock_write(level);
}

/**
 * @brief      interface bin read init
 * @param[in]  *name pointer to a name buffer
 * @param[out] *size pointer to a size buffer
 * @return     status code
 *             - 0 success
 *             - 1 init failed
 * @note       none
 */
uint8_t ccs811_interface_bin_read_init(char *name, uint32_t *size)
{
    gs_fp = fopen(name, "r");
    if (gs_fp == NULL)
    {
        return 1;
    }
    if (fseek(gs_fp, 0, SEEK_END) != 0)
    {
        return 1;
    }
    *size = ftell(gs_fp);
    
    return 0;
}

/**
 * @brief      interface bin read
 * @param[in]  addr read address
 * @param[in]  size read size
 * @param[out] *buffer pointer to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ccs811_interface_bin_read(uint32_t addr, uint16_t size, uint8_t *buffer)
{
    if (fseek(gs_fp, addr, SEEK_SET) != 0)
    {
        return 1;
    }
    if (fread((void *)buffer, 1, size, gs_fp) < 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief  interface bin read deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ccs811_interface_bin_read_deinit(void)
{
    if (fclose(gs_fp) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void ccs811_interface_delay_ms(uint32_t ms)
{
    usleep(1000 * ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void ccs811_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    (void)printf((uint8_t *)str);
}
