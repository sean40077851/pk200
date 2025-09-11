#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <stdint.h>
#include "esp_err.h"

// LCD physical dimensions (Portrait)
#define LCD_WIDTH  320
#define LCD_HEIGHT 480

// LVGL logical dimensions (Landscape - will be rotated to fit LCD)
#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the display driver
 * 
 * @return esp_err_t ESP_OK on success
 */
esp_err_t display_init(void);

/**
 * @brief Flush display buffer to screen
 * 
 * @param x1 Start X coordinate
 * @param y1 Start Y coordinate  
 * @param x2 End X coordinate
 * @param y2 End Y coordinate
 * @param color_data Pointer to color data buffer
 */
void display_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t *color_data);

/**
 * @brief Get display width
 * 
 * @return uint16_t Display width in pixels
 */
uint16_t display_get_width(void);

/**
 * @brief Get display height
 * 
 * @return uint16_t Display height in pixels
 */
uint16_t display_get_height(void);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_DRIVER_H */
