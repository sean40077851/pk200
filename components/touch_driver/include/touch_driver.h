#ifndef TOUCH_DRIVER_H
#define TOUCH_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the touch driver
 * 
 * @param sda_pin I2C SDA pin
 * @param scl_pin I2C SCL pin
 * @param rst_pin Reset pin
 * @param int_pin Interrupt pin
 * @return esp_err_t ESP_OK on success
 */
esp_err_t touch_init(gpio_num_t sda_pin, gpio_num_t scl_pin, gpio_num_t rst_pin, gpio_num_t int_pin);

/**
 * @brief Read touch coordinates
 * 
 * @param x Pointer to store X coordinate
 * @param y Pointer to store Y coordinate
 * @return true if touch detected, false otherwise
 */
bool touch_read(uint16_t *x, uint16_t *y);

/**
 * @brief Get touch status
 * 
 * @return uint8_t Number of touch points detected
 */
uint8_t touch_get_status(void);

#ifdef __cplusplus
}
#endif

#endif /* TOUCH_DRIVER_H */
