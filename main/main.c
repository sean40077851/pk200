#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "driver/gpio.h"

// LVGL includes
#include "lvgl.h"
#include "display_driver.h"
#include "touch_driver.h"
#include "wifi.h"
// UI includes
#include "ui/ui.h"

static const char *TAG = "PK200_LCD";

// LVGL task - Optimized for better performance
void lvgl_task(void *parameter) {
    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));  // 5ms delay for ~200Hz task rate, better responsiveness
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Starting PK200 LCD application");

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "NVS initialized successfully");
    wifi_init();
    ESP_LOGI(TAG, "WiFi initialized");
    // Initialize LVGL
    lv_init();
    ESP_LOGI(TAG, "LVGL initialized");

    // Initialize display driver
    display_init();
    ESP_LOGI(TAG, "Display driver initialized");

    
    // Initialize touch driver
    touch_init(GPIO_NUM_10, GPIO_NUM_9, GPIO_NUM_18, GPIO_NUM_8);  // SDA, SCL, RST, INT pins
    ESP_LOGI(TAG, "Touch driver initialized");

    // Initialize UI
    ui_init();
    ESP_LOGI(TAG, "UI initialized");

    // Create LVGL task with higher priority and larger stack for better performance
    xTaskCreate(lvgl_task, "lvgl_task", 8192, NULL, 10, NULL);  // Higher priority (10) and larger stack
    ESP_LOGI(TAG, "LVGL task created with high priority");

    ESP_LOGI(TAG, "Application started successfully");
}
