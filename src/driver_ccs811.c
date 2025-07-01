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
 * @file      driver_ccs811.c
 * @brief     driver ccs811 source file
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

#include "driver_ccs811.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "AMS CCS811"        /**< chip name */
#define MANUFACTURER_NAME         "AMS"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.8f                /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                /**< chip max supply voltage */
#define MAX_CURRENT               54.0f               /**< chip max current */
#define TEMPERATURE_MIN           -40.0f              /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f               /**< chip max operating temperature */
#define DRIVER_VERSION            1000                /**< driver version */

/**
 * @brief chip register definition
 */
#define CCS811_REG_STATUS                  0x00        /**< status register */
#define CCS811_REG_MEAS_MODE               0x01        /**< meas mode register */
#define CCS811_REG_ALG_RESULT_DATA         0x02        /**< alg result data register */
#define CCS811_REG_RAW_DATA                0x03        /**< raw data register */
#define CCS811_REG_ENV_DATA                0x05        /**< env data register */
#define CCS811_REG_THRESHOLDS              0x10        /**< thresholds register */
#define CCS811_REG_BASELINE                0x11        /**< baseline register */
#define CCS811_REG_APP_ERASE               0xF1        /**< app erase register */
#define CCS811_REG_APP_DATA                0xF2        /**< app data register */
#define CCS811_REG_APP_VERIFY              0xF3        /**< app verify register */
#define CCS811_REG_APP_START               0xF4        /**< app start register */
#define CCS811_REG_HW_ID                   0x20        /**< hw id register */
#define CCS811_REG_HW_VERSION              0x21        /**< hw version register */
#define CCS811_REG_FW_BOOT_VERSION         0x23        /**< fw boot version register */
#define CCS811_REG_FW_APP_VERSION          0x24        /**< fw app version register */
#define CCS811_REG_ERROR_ID                0xE0        /**< error id register */
#define CCS811_REG_SW_RESET                0xFF        /**< sw reset register */

/**
 * @brief      read bytes
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[in]  reg iic register address
 * @param[out] *data pointer to a data buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_ccs811_iic_read(ccs811_handle_t *handle, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (handle->iic_read(handle->iic_addr, reg, data, len) != 0)        /* read data */
    {
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

/**
 * @brief     write bytes
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] reg iic register address
 * @param[in] *data pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ccs811_iic_write(ccs811_handle_t *handle, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (handle->iic_write(handle->iic_addr, reg, data, len) != 0)        /* write data */
    {
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief     set address pin
 * @param[in] *handle pointer to a ccs811 handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ccs811_set_addr_pin(ccs811_handle_t *handle, ccs811_address_t addr_pin)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;        /* set iic address */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get address pin
 * @param[in]  *handle pointer to a ccs811 handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ccs811_get_addr_pin(ccs811_handle_t *handle, ccs811_address_t *addr_pin)
{
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    
    *addr_pin = (ccs811_address_t)(handle->iic_addr);       /* set iic address */
    
    return 0;                                               /* success return 0 */
}

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
uint8_t ccs811_get_status(ccs811_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_STATUS, status, 1);        /* get status */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("ccs811: get status failed.\n");              /* get status failed */
       
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t ccs811_set_mode(ccs811_handle_t *handle, ccs811_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_MEAS_MODE, &prev, 1);        /* get meas mode */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("ccs811: get meas mode failed.\n");             /* get meas mode failed */
        
        return 1;                                                           /* return error */
    }
    prev &= ~(7 << 4);                                                      /* clear settings */
    prev |= mode << 4;                                                      /* set mode */
    res = a_ccs811_iic_write(handle, CCS811_REG_MEAS_MODE, &prev, 1);       /* set meas mode */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("ccs811: set meas mode failed.\n");             /* set meas mode failed */
        
        return 1;                                                           /* return error */
    }
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t ccs811_get_mode(ccs811_handle_t *handle, ccs811_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_MEAS_MODE, &prev, 1);        /* get meas mode */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("ccs811: get meas mode failed.\n");             /* get meas mode failed */
        
        return 1;                                                           /* return error */
    }
    *mode = (ccs811_mode_t)((prev >> 4) & 0x07);                            /* set mode */
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t ccs811_set_interrupt_data_ready(ccs811_handle_t *handle, ccs811_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_MEAS_MODE, &prev, 1);        /* get meas mode */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("ccs811: get meas mode failed.\n");             /* get meas mode failed */
        
        return 1;                                                           /* return error */
    }
    prev &= ~(1 << 3);                                                      /* clear settings */
    prev |= enable << 3;                                                    /* set bool */
    res = a_ccs811_iic_write(handle, CCS811_REG_MEAS_MODE, &prev, 1);       /* set meas mode */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("ccs811: set meas mode failed.\n");             /* set meas mode failed */
        
        return 1;                                                           /* return error */
    }
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t ccs811_get_interrupt_data_ready(ccs811_handle_t *handle, ccs811_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_MEAS_MODE, &prev, 1);        /* get meas mode */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("ccs811: get meas mode failed.\n");             /* get meas mode failed */
        
        return 1;                                                           /* return error */
    }
    *enable = (ccs811_bool_t)((prev >> 3) & 0x01);                          /* set bool */
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t ccs811_set_interrupt_threshold(ccs811_handle_t *handle, ccs811_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_MEAS_MODE, &prev, 1);        /* get meas mode */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("ccs811: get meas mode failed.\n");             /* get meas mode failed */
        
        return 1;                                                           /* return error */
    }
    prev &= ~(1 << 2);                                                      /* clear settings */
    prev |= enable << 2;                                                    /* set bool */
    res = a_ccs811_iic_write(handle, CCS811_REG_MEAS_MODE, &prev, 1);       /* set meas mode */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("ccs811: set meas mode failed.\n");             /* set meas mode failed */
        
        return 1;                                                           /* return error */
    }
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t ccs811_get_interrupt_threshold(ccs811_handle_t *handle, ccs811_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_MEAS_MODE, &prev, 1);        /* get meas mode */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("ccs811: get meas mode failed.\n");             /* get meas mode failed */
        
        return 1;                                                           /* return error */
    }
    *enable = (ccs811_bool_t)((prev >> 2) & 0x01);                          /* set bool */
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t ccs811_read(ccs811_handle_t *handle, uint16_t *eco2_ppm, uint16_t *tvoc_ppb, uint16_t *raw)
{
    uint8_t res;
    uint8_t buf[8];
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_ALG_RESULT_DATA, buf, 8);        /* read data */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ccs811: read failed.\n");                          /* read failed */
        
        return 1;                                                               /* return error */
    }
    if ((buf[4] & CCS811_STATUS_ERROR) != 0)                                    /* check error */
    {
        handle->debug_print("ccs811: find error.\n");                           /* find error */
        
        return 4;                                                               /* return error */
    }
    if ((buf[4] & CCS811_STATUS_DATA_READY) != 0)                               /* check data ready */
    {
        *eco2_ppm = (((uint16_t)buf[0]) << 8) | buf[1];                         /* save eco2 */
        *tvoc_ppb = (((uint16_t)buf[2]) << 8) | buf[3];                         /* save tvoc */
        *raw = (((uint16_t)buf[6]) << 8) | buf[7];                              /* save raw */
    }
    else
    {
        handle->debug_print("ccs811: data is not ready.\n");                    /* data is not ready */
        
        return 5;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ccs811_get_raw_data(ccs811_handle_t *handle, uint16_t *raw, uint8_t *ua, uint16_t *adc_raw, float *adc_v)
{
    uint8_t res;
    uint8_t status;
    uint8_t buf[2];
    
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_STATUS, &status, 1);              /* get status */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("ccs811: get status failed.\n");                     /* get status failed */
        
        return 1;                                                                /* return error */
    }
    if ((status & CCS811_STATUS_ERROR) != 0)                                     /* check error */
    {
        handle->debug_print("ccs811: find error.\n");                            /* find error */
        
        return 4;                                                                /* return error */
    }
    if ((status & CCS811_STATUS_DATA_READY) != 0)                                /* check data ready */
    {
        res = a_ccs811_iic_read(handle, CCS811_REG_RAW_DATA, buf, 2);            /* read data */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ccs811: read failed.\n");                       /* read failed */
            
            return 1;                                                            /* return error */
        }
        *raw = (((uint16_t)buf[0]) << 8) | buf[1];                               /* save raw */
        *ua = buf[0] >> 2;                                                       /* set ua */
        *adc_raw = (uint16_t)(((uint16_t)(buf[0] & 0x03)) << 8) | buf[1];        /* set adc raw */
        *adc_v = (*adc_raw) * 1.65f / 1023.0f;                                   /* set adc voltage */
    }
    else
    {
        handle->debug_print("ccs811: data is not ready.\n");                     /* data is not ready */
        
        return 5;                                                                /* return error */
    }
    
    return 0;                                                                    /* success return 0 */
}

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
uint8_t ccs811_set_environment_data(ccs811_handle_t *handle, uint16_t humidity_raw, uint16_t temperature_raw)
{
    uint8_t res;
    uint8_t buf[4];
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    buf[0] = (humidity_raw >> 8) & 0xFF;                                      /* set humidity raw high */
    buf[1] = (humidity_raw >> 0) & 0xFF;                                      /* set humidity raw low */
    buf[2] = (temperature_raw >> 8) & 0xFF;                                   /* set temperature raw high */
    buf[3] = (temperature_raw >> 0) & 0xFF;                                   /* set temperature raw low */
    res = a_ccs811_iic_write(handle, CCS811_REG_ENV_DATA, buf, 4);            /* set environment data */
    if (res != 0)                                                             /* check result */
    {
        handle->debug_print("ccs811: set environment data failed.\n");        /* set environment data failed */
        
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t ccs811_set_eco2_threshold(ccs811_handle_t *handle, uint16_t low_medium_ppm, uint16_t medium_high_ppm, uint8_t hysteresis)
{
    uint8_t res;
    uint8_t buf[5];
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    buf[0] = (low_medium_ppm >> 8) & 0xFF;                                  /* set low medium high */
    buf[1] = (low_medium_ppm >> 0) & 0xFF;                                  /* set low medium low */
    buf[2] = (medium_high_ppm >> 8) & 0xFF;                                 /* set medium high ppm high */
    buf[3] = (medium_high_ppm >> 0) & 0xFF;                                 /* set medium high ppm low */
    buf[4] = hysteresis;                                                    /* set hysteresis */
    res = a_ccs811_iic_write(handle, CCS811_REG_THRESHOLDS, buf, 5);        /* set threshold data */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("ccs811: set threshold failed.\n");             /* set threshold failed */
        
        return 1;                                                           /* return error */
    }
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t ccs811_set_baseline(ccs811_handle_t *handle, uint16_t baseline)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    buf[0] = (baseline >> 8) & 0xFF;                                      /* set baseline high */
    buf[1] = (baseline >> 0) & 0xFF;                                      /* set baseline low */
    res = a_ccs811_iic_write(handle, CCS811_REG_BASELINE, buf, 2);        /* set baseline data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("ccs811: set baseline failed.\n");            /* set baseline failed */
        
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t ccs811_get_baseline(ccs811_handle_t *handle, uint16_t *baseline)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_BASELINE, buf, 2);        /* get baseline data */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("ccs811: get baseline failed.\n");           /* get baseline failed */
        
        return 1;                                                        /* return error */
    }
    *baseline = ((uint16_t)buf[0]) << 8 | buf[1];                        /* save baseline */
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t ccs811_get_hardware_id(ccs811_handle_t *handle, uint8_t *id)
{
    uint8_t res;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_HW_ID, id, 1);            /* get hardware id data */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("ccs811: get hardware id failed.\n");        /* get hardware id failed */
        
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t ccs811_get_hardware_version(ccs811_handle_t *handle, uint8_t *version)
{
    uint8_t res;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_HW_VERSION, version, 1);        /* get hardware version data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ccs811: get hardware version failed.\n");         /* get hardware version failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t ccs811_get_firmware_bootloader_version(ccs811_handle_t *handle, uint8_t *major, uint8_t *minor, uint8_t *trivial)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_FW_BOOT_VERSION, buf, 2);        /* get version data */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ccs811: get version failed.\n");                   /* get version failed */
        
        return 1;                                                               /* return error */
    }
    *major = (buf[0] >> 4) & 0xFF;                                              /* set major */
    *minor = (buf[0] >> 0) & 0xFF;                                              /* set minor */
    *trivial = buf[1];                                                          /* set trivial */
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ccs811_get_firmware_application_version(ccs811_handle_t *handle, uint8_t *major, uint8_t *minor, uint8_t *trivial)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_FW_APP_VERSION, buf, 2);        /* get version data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ccs811: get version failed.\n");                  /* get version failed */
        
        return 1;                                                              /* return error */
    }
    *major = (buf[0] >> 4) & 0xFF;                                             /* set major */
    *minor = (buf[0] >> 0) & 0xFF;                                             /* set minor */
    *trivial = buf[1];                                                         /* set trivial */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t ccs811_get_error_id(ccs811_handle_t *handle, uint8_t *id)
{
    uint8_t res;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_ccs811_iic_read(handle, CCS811_REG_ERROR_ID, id, 1);        /* get error id */
    if (res != 0)                                                       /* check result */
    {
        handle->debug_print("ccs811: get error id failed.\n");          /* get error id failed */
        
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t ccs811_soft_reset(ccs811_handle_t *handle)
{
    uint8_t res;
    uint8_t cmd[] = {0x11, 0xE5, 0x72, 0x8A};
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_ccs811_iic_write(handle, CCS811_REG_SW_RESET, cmd, 4);        /* soft reset */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("ccs811: soft reset failed.\n");              /* soft reset failed */
        
        return 1;                                                         /* return error */
    }
    handle->delay_ms(20);                                                 /* wait 20 ms */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t ccs811_app_erase(ccs811_handle_t *handle)
{
    uint8_t res;
    uint8_t status;
    uint32_t i;
    uint32_t timeout_ms = 5000;
    uint8_t cmd[] = {0xE7, 0xA7, 0xE6, 0x09};
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ccs811_iic_write(handle, CCS811_REG_APP_ERASE, cmd, 4);            /* app erase */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ccs811: app erase failed.\n");                    /* app erase failed */
        
        return 1;                                                              /* return error */
    }
    handle->delay_ms(500);                                                     /* delay 500ms */
    for (i = 0; i < timeout_ms; i++)                                           /* check status */
    {
        res = a_ccs811_iic_read(handle, CCS811_REG_STATUS, &status, 1);        /* get status */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("ccs811: get status failed.\n");               /* get status failed */
            
            return 1;                                                          /* return error */
        }
        if ((status & (1 << 6)) != 0)                                          /* check flag */
        {
            return 0;                                                          /* success return 0 */
        }
        handle->delay_ms(1);                                                   /* delay 1ms */
    }
    handle->debug_print("ccs811: erase timeout.\n");                           /* erase timeout */
    
    return 4;                                                                  /* timeout */
}

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
uint8_t ccs811_app_verify(ccs811_handle_t *handle)
{
    uint8_t res;
    uint8_t status;
    uint32_t i;
    uint32_t timeout_ms = 5000;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ccs811_iic_write(handle, CCS811_REG_APP_VERIFY, NULL, 0);          /* app verify */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ccs811: app verify failed.\n");                   /* app verify failed */
        
        return 1;                                                              /* return error */
    }
    handle->delay_ms(100);                                                     /* delay 100ms */
    for (i = 0; i < timeout_ms; i++)                                           /* check status */
    {
        res = a_ccs811_iic_read(handle, CCS811_REG_STATUS, &status, 1);        /* get status */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("ccs811: get status failed.\n");               /* get status failed */
            
            return 1;                                                          /* return error */
        }
        if ((status & (1 << 5)) != 0)                                          /* check flag */
        {
            return 0;                                                          /* success return 0 */
        }
        handle->delay_ms(1);                                                   /* delay 1ms */
    }
    handle->debug_print("ccs811: verify timeout.\n");                          /* verify timeout */
    
    return 4;                                                                  /* timeout */
}

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
uint8_t ccs811_app_start(ccs811_handle_t *handle)
{
    uint8_t res;
    uint8_t status;
    uint32_t i;
    uint32_t timeout_ms = 1000;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ccs811_iic_write(handle, CCS811_REG_APP_START, NULL, 0);           /* app start */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ccs811: app start failed.\n");                    /* app start failed */
        
        return 1;                                                              /* return error */
    }
    handle->delay_ms(10);                                                      /* delay 10ms */
    for (i = 0; i < timeout_ms; i++)                                           /* check status */
    {
        res = a_ccs811_iic_read(handle, CCS811_REG_STATUS, &status, 1);        /* get status */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("ccs811: get status failed.\n");               /* get status failed */
            
            return 1;                                                          /* return error */
        }
        if ((status & (1 << 4)) != 0)                                          /* check flag */
        {
            return 0;                                                          /* success return 0 */
        }
        handle->delay_ms(1);                                                   /* delay 1ms */
    }
    handle->debug_print("ccs811: start timeout.\n");                           /* start timeout */
    
    return 4;                                                                  /* timeout */
}

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
uint8_t ccs811_app_program(ccs811_handle_t *handle, uint8_t *rom, uint32_t len)
{
    uint8_t res;
    uint32_t i;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    if ((len % 8) != 0)                                                           /* check length */
    {
        handle->debug_print("ccs811: len is invalid.\n");                         /* len is invalid */
        
        return 4;                                                                 /* return error */
    }
    
    for (i = 0; i < len; i += 8)
    {
        res = a_ccs811_iic_write(handle, CCS811_REG_APP_DATA, rom + i, 8);        /* app program */
        if (res != 0)                                                             /* check result */
        {
            handle->debug_print("ccs811: app program failed.\n");                 /* app program failed */
            
            return 1;                                                             /* return error */
        }
        handle->delay_ms(50);                                                     /* delay 50ms */
    }
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t ccs811_wake_up(ccs811_handle_t *handle, ccs811_bool_t enable)
{
    uint8_t res;
    
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }
    
    if (enable != 0)                                              /* enable */
    {
        res = handle->gpio_wake_write(0);                         /* set low */
        if (res != 0)                                             /* check result */
        {
            handle->debug_print("ccs811: wake failed.\n");        /* wake failed */
            
            return 1;                                             /* return error */
        }
    }
    else                                                          /* disable */
    {
        res = handle->gpio_wake_write(1);                         /* set high */
        if (res != 0)                                             /* check result */
        {
            handle->debug_print("ccs811: wake failed.\n");        /* wake failed */
            
            return 1;                                             /* return error */
        }
    }
    
    return 0;                                                     /* success return 0 */
}

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
uint8_t ccs811_hard_reset(ccs811_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }
    if (handle->inited != 1)                                   /* check handle initialization */
    {
        return 3;                                              /* return error */
    }
    
    res = handle->gpio_reset_write(0);                         /* set low */
    if (res != 0)                                              /* check result */
    {
        handle->debug_print("ccs811: reset failed.\n");        /* reset failed */
        
        return 1;                                              /* return error */
    }
    handle->delay_ms(20);                                      /* delay 20ms */
    res = handle->gpio_reset_write(1);                         /* set high */
    if (res != 0)                                              /* check result */
    {
        handle->debug_print("ccs811: reset failed.\n");        /* reset failed */
        
        return 1;                                              /* return error */
    }
    
    return 0;                                                  /* success return 0 */
}

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
uint8_t ccs811_environment_humidity_convert_to_register(ccs811_handle_t *handle, float humidity, uint16_t *reg)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    if (handle->inited != 1)                     /* check handle initialization */
    {
        return 3;                                /* return error */
    }
    
    *reg = (uint16_t)(humidity * 512.0f);        /* convert real data to register data */
    
    return 0;                                    /* success return 0 */
}

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
uint8_t ccs811_environment_humidity_convert_to_data(ccs811_handle_t *handle, uint16_t reg, float *humidity)
{
    if (handle == NULL)                       /* check handle */
    {
        return 2;                             /* return error */
    }
    if (handle->inited != 1)                  /* check handle initialization */
    {
        return 3;                             /* return error */
    }
    
    *humidity = (float)(reg) / 512.0f;        /* convert raw data to real data */
    
    return 0;                                 /* success return 0 */
}

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
uint8_t ccs811_environment_temperature_convert_to_register(ccs811_handle_t *handle, float temperature, uint16_t *reg)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (handle->inited != 1)                                  /* check handle initialization */
    {
        return 3;                                             /* return error */
    }
    
    *reg = (uint16_t)((temperature + 25.0f) * 512.0f);        /* convert real data to register data */
    
    return 0;                                                 /* success return 0 */
}

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
uint8_t ccs811_environment_temperature_convert_to_data(ccs811_handle_t *handle, uint16_t reg, float *temperature)
{
    if (handle == NULL)                                  /* check handle */
    {
        return 2;                                        /* return error */
    }
    if (handle->inited != 1)                             /* check handle initialization */
    {
        return 3;                                        /* return error */
    }
    
    *temperature = (float)(reg) / 512.0f - 25.0f;        /* convert raw data to real data */
    
    return 0;                                            /* success return 0 */
}

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
uint8_t ccs811_app_firmware_update(ccs811_handle_t *handle, char *path)
{
    uint8_t res;
    uint8_t done;
    uint8_t status;
    uint32_t i;
    uint32_t j;
    uint32_t timeout_ms;
    uint32_t size;
    uint16_t len;
    uint8_t cmd_erase[] = {0xE7, 0xA7, 0xE6, 0x09};
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = handle->bin_read_init(path, &size);                                                 /* bin read init */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("ccs811: bin init failed.\n");                                    /* bin init failed */
        
        return 1;                                                                             /* return error */
    }
    if ((size % 8) != 0)                                                                      /* check size */
    {
        handle->debug_print("ccs811: size is invalid.\n");                                    /* size is invalid */
        (void)handle->bin_read_deinit();                                                      /* bin read deinit */
        
        return 4;                                                                             /* return error */
    }
    
    timeout_ms = 5000;                                                                        /* set timeout */
    done= 0;                                                                                  /* init 0 */
    res = a_ccs811_iic_write(handle, CCS811_REG_APP_ERASE, cmd_erase, 4);                     /* app erase */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("ccs811: app erase failed.\n");                                   /* app erase failed */
        (void)handle->bin_read_deinit();                                                      /* bin read deinit */
        
        return 1;                                                                             /* return error */
    }
    handle->delay_ms(500);                                                                    /* delay 500ms */
    for (i = 0; i < timeout_ms; i++)                                                          /* check status */
    {
        res = a_ccs811_iic_read(handle, CCS811_REG_STATUS, &status, 1);                       /* get status */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("ccs811: get status failed.\n");                              /* get status failed */
            (void)handle->bin_read_deinit();                                                  /* bin read deinit */
            
            return 1;                                                                         /* return error */
        }
        if ((status & (1 << 6)) != 0)                                                         /* check flag */
        {
            done = 1;                                                                         /* set done */
            
            break;                                                                            /* break */
        }
        handle->delay_ms(1);                                                                  /* delay 1ms */
    }
    if (done == 0)                                                                            /* check done */
    {
        handle->debug_print("ccs811: erase timeout.\n");                                      /* erase timeout */
        (void)handle->bin_read_deinit();                                                      /* bin read deinit */
        
        return 5;                                                                             /* timeout */
    }
    
    for (i = 0; i < size; i += 256)                                                           /* loop */
    {
        len = ((size - i) >= 256) ? 256 : (size - i);                                         /* get length */
        res = handle->bin_read(i, len, handle->buf);                                          /* bin read */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("ccs811: bin read failed.\n");                                /* bin read failed */
            (void)handle->bin_read_deinit();                                                  /* bin read deinit */
            
            return 1;                                                                         /* return error */
        }
        
        for (j = 0; j < len; j += 8)                                                          /* program */
        {
            res = a_ccs811_iic_write(handle, CCS811_REG_APP_DATA, handle->buf + j, 8);        /* app program */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("ccs811: app program failed.\n");                         /* app program failed */
                (void)handle->bin_read_deinit();                                              /* bin read deinit */
                
                return 1;                                                                     /* return error */
            }
            handle->delay_ms(50);                                                             /* delay 50ms */
        }
    }
    
    timeout_ms = 5000;                                                                        /* set timeout */
    done= 0;                                                                                  /* init 0 */
    res = a_ccs811_iic_write(handle, CCS811_REG_APP_VERIFY, NULL, 0);                         /* app verify */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("ccs811: app verify failed.\n");                                  /* app verify failed */
        (void)handle->bin_read_deinit();                                                      /* bin read deinit */
        
        return 1;                                                                             /* return error */
    }
    handle->delay_ms(100);                                                                    /* delay 100ms */
    for (i = 0; i < timeout_ms; i++)                                                          /* check status */
    {
        res = a_ccs811_iic_read(handle, CCS811_REG_STATUS, &status, 1);                       /* get status */
        if(res != 0)                                                                          /* check result */
        {
            handle->debug_print("ccs811: get status failed.\n");                              /* get status failed */
            (void)handle->bin_read_deinit();                                                  /* bin read deinit */
            
            return 1;                                                                         /* return error */
        }
        if ((status & (1 << 5)) != 0)                                                         /* check flag */
        {
            done = 1;                                                                         /* set done */
            
            break;                                                                            /* break */
        }
        handle->delay_ms(1);                                                                  /* delay 1ms */
    }
    if (done == 0)                                                                            /* check done */
    {
        handle->debug_print("ccs811: erase timeout.\n");                                      /* erase timeout */
        (void)handle->bin_read_deinit();                                                      /* bin read deinit */
        
        return 5;                                                                             /* timeout */
    }
    res = handle->bin_read_deinit();                                                          /* bin read deinit */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("ccs811: bin deinit failed.\n");                                  /* bin deinit failed */
        
        return 1;                                                                             /* return error */
    }
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t ccs811_init(ccs811_handle_t *handle)
{
    uint8_t res;
    uint8_t id;
    uint8_t status;
    uint16_t i;
    uint8_t cmd_reset[] = {0x11, 0xE5, 0x72, 0x8A};
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->iic_init == NULL)                                            /* check iic_init */
    {
        handle->debug_print("ccs811: iic_init is null.\n");                  /* iic_init is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_deinit == NULL)                                          /* check iic_deinit */
    {
        handle->debug_print("ccs811: iic_deinit is null.\n");                /* iic_deinit is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_write == NULL)                                           /* check iic_write */
    {
        handle->debug_print("ccs811: iic_write is null.\n");                 /* iic_write is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_read == NULL)                                            /* check iic_read */
    {
        handle->debug_print("ccs811: iic_read is null.\n");                  /* iic_read is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_wake_init == NULL)                                      /* check gpio_wake_init */
    {
        handle->debug_print("ccs811: gpio_wake_init is null.\n");            /* gpio_wake_init is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_wake_deinit == NULL)                                    /* check gpio_wake_deinit */
    {
        handle->debug_print("ccs811: gpio_wake_deinit is null.\n");          /* gpio_wake_deinit is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_wake_write == NULL)                                     /* check gpio_wake_write */
    {
        handle->debug_print("ccs811: gpio_wake_write is null.\n");           /* gpio_wake_write is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_reset_init == NULL)                                     /* check gpio_reset_init */
    {
        handle->debug_print("ccs811: gpio_reset_init is null.\n");           /* gpio_reset_init is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_reset_deinit == NULL)                                   /* check gpio_reset_deinit */
    {
        handle->debug_print("ccs811: gpio_reset_deinit is null.\n");         /* gpio_reset_deinit is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_reset_write == NULL)                                    /* check gpio_reset_write */
    {
        handle->debug_print("ccs811: gpio_reset_write is null.\n");          /* gpio_reset_write is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->bin_read_init == NULL)                                       /* check bin_read_init */
    {
        handle->debug_print("ccs811: bin_read_init is null.\n");             /* bin_read_init is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->bin_read == NULL)                                            /* check bin_read */
    {
        handle->debug_print("ccs811: bin_read is null.\n");                  /* bin_read is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->bin_read_deinit == NULL)                                     /* check bin_read_deinit */
    {
        handle->debug_print("ccs811: bin_read_deinit is null.\n");           /* bin_read_deinit is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("ccs811: delay_ms is null.\n");                  /* delay_ms is null */
    
        return 3;                                                            /* return error */
    }
    
    if (handle->iic_init() != 0)                                             /* iic init */
    {
        handle->debug_print("ccs811: iic init failed.\n");                   /* iic init failed */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_wake_init() != 0)                                       /* gpio wake init */
    {
        handle->debug_print("ccs811: gpio wake init failed.\n");             /* gpio wake init failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        
        return 3;                                                            /* return error */
    }
    if (handle->gpio_reset_init() != 0)                                      /* gpio reset init */
    {
        handle->debug_print("ccs811: gpio reset init failed.\n");            /* gpio reset init failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        
        return 3;                                                            /* return error */
    }
    res = handle->gpio_wake_write(0);                                        /* wake up */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ccs811: wake up failed.\n");                    /* wake up failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 3;                                                            /* return error */
    }
    handle->delay_ms(5);                                                     /* delay 5ms */
    
    res = handle->gpio_reset_write(0);                                       /* set low */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ccs811: reset failed.\n");                      /* reset failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 3;                                                            /* return error */
    }
    handle->delay_ms(20);                                                    /* delay 20ms */
    res = handle->gpio_reset_write(1);                                       /* set high */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ccs811: reset failed.\n");                      /* reset failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 3;                                                            /* return error */
    }
    handle->delay_ms(5);                                                     /* delay 5ms */
    
    res = a_ccs811_iic_read(handle, CCS811_REG_HW_ID, &id, 1);               /* get hardware id data */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ccs811: get hardware id failed.\n");            /* get hardware id failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 3;                                                            /* return error */
    }
    if (id != 0x81)                                                          /* check id */
    {
        handle->debug_print("ccs811: id is error.\n");                       /* id is error */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 4;                                                            /* return error */
    }
    
    res = a_ccs811_iic_write(handle, CCS811_REG_SW_RESET, cmd_reset, 4);     /* soft reset */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ccs811: soft reset failed.\n");                 /* soft reset failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 1;                                                            /* return error */
    }
    handle->delay_ms(20);                                                    /* wait 20 ms */
    for (i = 0; i < 1000; i++)                                               /* check status */
    {
        res = a_ccs811_iic_read(handle, CCS811_REG_STATUS, &status, 1);      /* get status */
        if (res != 0)                                                        /* check result */
        {
            handle->debug_print("ccs811: get status failed.\n");             /* get status failed */
            (void)handle->iic_deinit();                                      /* iic deinit */
            (void)handle->gpio_wake_deinit();                                /* gpio wake deinit */
            (void)handle->gpio_reset_deinit();                               /* gpio reset deinit */
            
            return 1;                                                        /* return error */
        }
        if ((status & (1 << 4)) != 0)                                        /* check flag */
        {
            break;                                                           /* break */
        }
        handle->delay_ms(1);                                                 /* delay 1ms */
    }
    if (i == 1000)                                                           /* check timeout */
    {
        handle->debug_print("ccs811: firmware loaded failed.\n");            /* firmware loaded failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 5;                                                            /* return error */
    }
    handle->inited = 1;                                                      /* flag finish initialization */
  
    return 0;                                                                /* success return 0 */
}

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
uint8_t ccs811_app_firmware_init(ccs811_handle_t *handle)
{
    uint8_t res;
    uint8_t id;
    uint8_t cmd_reset[] = {0x11, 0xE5, 0x72, 0x8A};
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->iic_init == NULL)                                            /* check iic_init */
    {
        handle->debug_print("ccs811: iic_init is null.\n");                  /* iic_init is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_deinit == NULL)                                          /* check iic_deinit */
    {
        handle->debug_print("ccs811: iic_deinit is null.\n");                /* iic_deinit is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_write == NULL)                                           /* check iic_write */
    {
        handle->debug_print("ccs811: iic_write is null.\n");                 /* iic_write is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_read == NULL)                                            /* check iic_read */
    {
        handle->debug_print("ccs811: iic_read is null.\n");                  /* iic_read is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_wake_init == NULL)                                      /* check gpio_wake_init */
    {
        handle->debug_print("ccs811: gpio_wake_init is null.\n");            /* gpio_wake_init is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_wake_deinit == NULL)                                    /* check gpio_wake_deinit */
    {
        handle->debug_print("ccs811: gpio_wake_deinit is null.\n");          /* gpio_wake_deinit is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_wake_write == NULL)                                     /* check gpio_wake_write */
    {
        handle->debug_print("ccs811: gpio_wake_write is null.\n");           /* gpio_wake_write is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_reset_init == NULL)                                     /* check gpio_reset_init */
    {
        handle->debug_print("ccs811: gpio_reset_init is null.\n");           /* gpio_reset_init is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_reset_deinit == NULL)                                   /* check gpio_reset_deinit */
    {
        handle->debug_print("ccs811: gpio_reset_deinit is null.\n");         /* gpio_reset_deinit is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_reset_write == NULL)                                    /* check gpio_reset_write */
    {
        handle->debug_print("ccs811: gpio_reset_write is null.\n");          /* gpio_reset_write is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->bin_read_init == NULL)                                       /* check bin_read_init */
    {
        handle->debug_print("ccs811: bin_read_init is null.\n");             /* bin_read_init is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->bin_read == NULL)                                            /* check bin_read */
    {
        handle->debug_print("ccs811: bin_read is null.\n");                  /* bin_read is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->bin_read_deinit == NULL)                                     /* check bin_read_deinit */
    {
        handle->debug_print("ccs811: bin_read_deinit is null.\n");           /* bin_read_deinit is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("ccs811: delay_ms is null.\n");                  /* delay_ms is null */
    
        return 3;                                                            /* return error */
    }
    
    if (handle->iic_init() != 0)                                             /* iic init */
    {
        handle->debug_print("ccs811: iic init failed.\n");                   /* iic init failed */
    
        return 3;                                                            /* return error */
    }
    if (handle->gpio_wake_init() != 0)                                       /* gpio wake init */
    {
        handle->debug_print("ccs811: gpio wake init failed.\n");             /* gpio wake init failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        
        return 3;                                                            /* return error */
    }
    if (handle->gpio_reset_init() != 0)                                      /* gpio reset init */
    {
        handle->debug_print("ccs811: gpio reset init failed.\n");            /* gpio reset init failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        
        return 3;                                                            /* return error */
    }
    res = handle->gpio_wake_write(0);                                        /* wake up */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ccs811: wake up failed.\n");                    /* wake up failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 3;                                                            /* return error */
    }
    handle->delay_ms(5);                                                     /* delay 5ms */
    
    res = handle->gpio_reset_write(0);                                       /* set low */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ccs811: reset failed.\n");                      /* reset failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 3;                                                            /* return error */
    }
    handle->delay_ms(20);                                                    /* delay 20ms */
    res = handle->gpio_reset_write(1);                                       /* set high */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ccs811: reset failed.\n");                      /* reset failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 3;                                                            /* return error */
    }
    handle->delay_ms(5);                                                     /* delay 5ms */
    
    res = a_ccs811_iic_read(handle, CCS811_REG_HW_ID, &id, 1);               /* get hardware id data */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ccs811: get hardware id failed.\n");            /* get hardware id failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 3;                                                            /* return error */
    }
    if (id != 0x81)                                                          /* check id */
    {
        handle->debug_print("ccs811: id is error.\n");                       /* id is error */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 4;                                                            /* return error */
    }
    
    res = a_ccs811_iic_write(handle, CCS811_REG_SW_RESET, cmd_reset, 4);     /* soft reset */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ccs811: soft reset failed.\n");                 /* soft reset failed */
        (void)handle->iic_deinit();                                          /* iic deinit */
        (void)handle->gpio_wake_deinit();                                    /* gpio wake deinit */
        (void)handle->gpio_reset_deinit();                                   /* gpio reset deinit */
        
        return 1;                                                            /* return error */
    }
    handle->delay_ms(20);                                                    /* wait 20 ms */
    handle->inited = 1;                                                      /* flag finish initialization */
  
    return 0;                                                                /* success return 0 */
}

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
uint8_t ccs811_deinit(ccs811_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = handle->gpio_reset_write(0);                                     /* set low */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("ccs811: reset failed.\n");                    /* reset failed */
        
        return 4;                                                          /* return error */
    }
    handle->delay_ms(20);                                                  /* delay 20ms */
    res = handle->gpio_reset_write(1);                                     /* set high */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("ccs811: reset failed.\n");                    /* reset failed */
        
        return 4;                                                          /* return error */
    }
    if (handle->gpio_reset_deinit() != 0)                                  /* gpio reset deinit */
    {
        handle->debug_print("ccs811: gpio reset deinit failed.\n");        /* gpio reset deinit failed */
    
        return 1;                                                          /* return error */
    }
    if (handle->gpio_wake_deinit() != 0)                                   /* gpio wake deinit */
    {
        handle->debug_print("ccs811: gpio wake deinit failed.\n");         /* gpio wake deinit failed */
    
        return 1;                                                          /* return error */
    }
    if (handle->iic_deinit() != 0)                                         /* iic deinit */
    {
        handle->debug_print("ccs811: iic close failed.\n");                /* iic close failed */
    
        return 1;                                                          /* return error */
    }
    handle->inited = 0;                                                    /* flag close initialization */
  
    return 0;                                                              /* success return 0 */
}

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
uint8_t ccs811_set_reg(ccs811_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                      /* check handle */
    {
        return 2;                                            /* return error */
    }
    if (handle->inited != 1)                                 /* check handle initialization */
    {
        return 3;                                            /* return error */
    }
  
    return a_ccs811_iic_write(handle, reg, buf, len);        /* write data */
}

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
uint8_t ccs811_get_reg(ccs811_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    if (handle->inited != 1)                                /* check handle initialization */
    {
        return 3;                                           /* return error */
    }
  
    return a_ccs811_iic_read(handle, reg, buf, len);        /* read data */
}

/**
 * @brief      get chip information
 * @param[out] *info pointer to a ccs811 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ccs811_info(ccs811_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ccs811_info_t));                         /* initialize ccs811 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
