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

// Network includes
#include "config_manager.h"  // 新增：設定管理
#include "wifi.h"
#include "my_mqtt.h"

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

// 系統狀態監控任務
void system_monitor_task(void *pvParameters) {
    int heartbeat_counter = 0;
    
    while (1) {
        if (wifi_is_connected()) {
            if (!mqtt_is_connected()) {
                // WiFi 連上但 MQTT 沒連上，MQTT 會自動重試
                if (heartbeat_counter % 10 == 0) {  // 每10秒提示一次
                    ESP_LOGW(TAG, "WiFi connected but MQTT disconnected");
                }
            } else {
                // 系統正常運行 - 每60秒發送一次心跳
                if (heartbeat_counter >= 60) {
                    mqtt_publish_device_info();
                    heartbeat_counter = 0;
                    ESP_LOGI(TAG, "System heartbeat sent - WiFi: OK, MQTT: OK");
                }
            }
        } else {
            // WiFi 斷線
            if (heartbeat_counter % 15 == 0) {  // 每15秒提示一次
                ESP_LOGW(TAG, "WiFi disconnected, waiting for reconnection...");
            }
        }
        
        heartbeat_counter++;
        vTaskDelay(pdMS_TO_TICKS(1000)); // 每秒檢查一次
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "=== Starting PK200 LCD Application ===");

    // Initialize NVS (移到最前面，因為設定管理需要)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "NVS initialized successfully");

    // Initialize WiFi (包含設定管理初始化)
    wifi_init();
    ESP_LOGI(TAG, "WiFi initialized with SSID: %s", g_device_config.wifi_ssid);
    ESP_LOGI(TAG, "Device MAC: %s", g_device_config.device_mac);

    // Initialize LVGL
    lv_init();
    ESP_LOGI(TAG, "LVGL initialized");

    // Initialize display driver
    display_init();
    ESP_LOGI(TAG, "Display driver initialized");

    // Initialize touch driver
    touch_init(GPIO_NUM_10, GPIO_NUM_9, GPIO_NUM_18, GPIO_NUM_8);  // SDA, SCL, RST, INT pins
    ESP_LOGI(TAG, "Touch driver initialized");

    // MQTT 初始化會在 WiFi 連線成功後自動觸發，這裡不需要手動呼叫
    ESP_LOGI(TAG, "MQTT will auto-initialize when WiFi connects");

    // Initialize UI
    ui_init();
    ESP_LOGI(TAG, "UI initialized");

    // Create LVGL task with higher priority and larger stack for better performance
    xTaskCreate(lvgl_task, "lvgl_task", 8192, NULL, 10, NULL);  // Higher priority (10) and larger stack
    ESP_LOGI(TAG, "LVGL task created with high priority");

    // Create system monitor task
    xTaskCreate(system_monitor_task, "sys_monitor", 4096, NULL, 5, NULL);
    ESP_LOGI(TAG, "System monitor task created");
    
    // Create MQTT reconnect handler task
    extern void mqtt_reconnect_task(void *pvParameters);
    xTaskCreate(mqtt_reconnect_task, "mqtt_reconnect", 4096, NULL, 5, NULL);
    ESP_LOGI(TAG, "MQTT reconnect task created");

    // 顯示設備配置資訊
    ESP_LOGI(TAG, "=== Device Configuration ===");
    ESP_LOGI(TAG, "Firmware: %s", g_device_config.fw_version);
    ESP_LOGI(TAG, "MQTT Host: %s:%d", g_device_config.mqtt_host, g_device_config.mqtt_port);
    ESP_LOGI(TAG, "MQTT User: %s", g_device_config.mqtt_username);
    ESP_LOGI(TAG, "IP Mode: %d (0=DHCP, 1=Static, 2=Default)", g_device_config.ip_mode);
    
    // 顯示 MQTT 控制 Topics
    char topic_buffer[128];
    config_get_device_topic(topic_buffer, sizeof(topic_buffer));
    ESP_LOGI(TAG, "Control Topic: %s", topic_buffer);
    config_get_mqttset_topic(topic_buffer, sizeof(topic_buffer));
    ESP_LOGI(TAG, "Config Topic: %s", topic_buffer);
    
    ESP_LOGI(TAG, "=== Application Started Successfully ===");
    ESP_LOGI(TAG, "System ready for MQTT Setting Tool control");
}