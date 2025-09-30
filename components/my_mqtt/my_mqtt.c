#include <stdio.h>
#include "mqtt_client.h"
#include "esp_log.h"
#include "my_mqtt.h"
#include "config_manager.h"
#include "esp_system.h"
#include <string.h>

#define TAG "MQTT"

esp_mqtt_client_handle_t mqtt_handle = NULL;
static bool mqtt_connected = false;
static bool mqtt_need_reconnect = false;  // 新增：重連標記
static char device_topic[64];        // WE1/{MAC}
static char mqttset_topic[64];       // WE1/{MAC}/MQTTset  
static char wifiset_topic[64];       // WE1/{MAC}/WIFIset

// 外部函數聲明
extern void wifi_reconnect_with_new_config(void);
extern void wifi_apply_ip_config(void);
extern bool wifi_is_connected(void);

// 發佈 WiFi 資訊 (協議要求)
static void mqtt_publish_wifi_info(void) {
    if (!mqtt_connected || !mqtt_handle) return;
    
    char payload[512];
    snprintf(payload, sizeof(payload),
             "{"
             "\"mac\":\"%s\","
             "\"FWVer\":\"%s\","
             "\"ip_mode\":\"%d\","
             "\"ip\":\"%s\","
             "\"gateway\":\"%s\","
             "\"subnet\":\"%s\""
             "}",
             g_device_config.device_mac,
             g_device_config.fw_version,
             g_device_config.ip_mode,
             g_device_config.current_ip,
             g_device_config.gateway,
             g_device_config.subnet);
    
    esp_mqtt_client_publish(mqtt_handle, "WE1/WIFIinfo", payload, 0, 1, 0);
    ESP_LOGI(TAG, "Published WiFi info: %s", payload);
}

// 發佈 MQTT 資訊 (協議要求)
static void mqtt_publish_mqtt_info(void) {
    if (!mqtt_connected || !mqtt_handle) return;
    
    char payload[512];
    snprintf(payload, sizeof(payload),
             "{"
             "\"mac\":\"%s\","
             "\"ssid\":\"%s\","
             "\"wifi_pwd\":\"%s\","
             "\"host\":\"%s\","
             "\"port\":\"%d\","
             "\"user\":\"%s\""
             "}",
             g_device_config.device_mac,
             g_device_config.wifi_ssid,
             g_device_config.wifi_password,
             g_device_config.mqtt_host,
             g_device_config.mqtt_port,
             g_device_config.mqtt_username);
    
    esp_mqtt_client_publish(mqtt_handle, "WE1/MQTTinfo", payload, 0, 1, 0);
    ESP_LOGI(TAG, "Published MQTT info");
}

// 處理設定指令
static void handle_set_command(int set_value) {
    ESP_LOGI(TAG, "Received set command: %d", set_value);
    
    switch (set_value) {
        case 3:  // 裝置重開指令
            ESP_LOGI(TAG, "Device restart command received");
            vTaskDelay(pdMS_TO_TICKS(1000)); // 延遲1秒讓回應送出
            esp_restart();
            break;
            
        case 7:  // 設定 MQTT (會配合 MQTTset topic)
            ESP_LOGI(TAG, "MQTT setup mode activated");
            break;
            
        case 8:  // 讀取 WiFi 及 MQTT 資訊
            ESP_LOGI(TAG, "Sending WiFi and MQTT info");
            mqtt_publish_wifi_info();
            mqtt_publish_mqtt_info();
            break;
            
        case 9:  // 設定 IP (會配合 WIFIset topic)
            ESP_LOGI(TAG, "WiFi IP setup mode activated");
            break;
            
        default:
            ESP_LOGW(TAG, "Unknown set command: %d", set_value);
            break;
    }
}

// 處理 MQTTset 訊息
static void handle_mqttset_command(const char* data, int data_len) {
    char json_buffer[512];
    
    if (data_len >= sizeof(json_buffer)) {
        ESP_LOGE(TAG, "MQTTset data too large");
        return;
    }
    
    memcpy(json_buffer, data, data_len);
    json_buffer[data_len] = '\0';
    
    ESP_LOGI(TAG, "Received MQTTset: %s", json_buffer);
    
    // 更新 MQTT 設定
    esp_err_t ret = config_update_mqtt_from_json(json_buffer);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "MQTT configuration updated successfully");
        ESP_LOGI(TAG, "Device will reconnect with new settings...");
        
        // 設定重連標記，讓獨立任務處理
        mqtt_need_reconnect = true;
        
    } else {
        ESP_LOGE(TAG, "Failed to update MQTT configuration");
    }
}

// 處理 WIFIset 訊息  
static void handle_wifiset_command(const char* data, int data_len) {
    char json_buffer[512];
    
    if (data_len >= sizeof(json_buffer)) {
        ESP_LOGE(TAG, "WIFIset data too large");
        return;
    }
    
    memcpy(json_buffer, data, data_len);
    json_buffer[data_len] = '\0';
    
    ESP_LOGI(TAG, "Received WIFIset: %s", json_buffer);
    
    // 更新 WiFi IP 設定
    esp_err_t ret = config_update_wifi_from_json(json_buffer);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "WiFi IP configuration updated successfully");
        
        // 套用新的 IP 設定
        wifi_apply_ip_config();
        
    } else {
        ESP_LOGE(TAG, "Failed to update WiFi IP configuration");
    }
}

void mqtt_event_callback(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
    
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            mqtt_connected = true;
            
            // 訂閱控制 topics
            esp_mqtt_client_subscribe(mqtt_handle, device_topic, 1);
            esp_mqtt_client_subscribe(mqtt_handle, mqttset_topic, 1);
            esp_mqtt_client_subscribe(mqtt_handle, wifiset_topic, 1);
            
            ESP_LOGI(TAG, "Subscribed to topics:");
            ESP_LOGI(TAG, "  - %s", device_topic);
            ESP_LOGI(TAG, "  - %s", mqttset_topic);
            ESP_LOGI(TAG, "  - %s", wifiset_topic);
            
            // 發佈初始資訊 (協議要求)
            mqtt_publish_wifi_info();
            mqtt_publish_mqtt_info();
            break;
            
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            mqtt_connected = false;
            break;
            
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
            
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
            
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
            
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            
            // 處理主控制 topic (WE1/{MAC})
            if (strncmp(event->topic, device_topic, event->topic_len) == 0) {
                // 解析 set 指令
                char json_buffer[128];
                if (event->data_len < sizeof(json_buffer)) {
                    memcpy(json_buffer, event->data, event->data_len);
                    json_buffer[event->data_len] = '\0';
                    
                    // 簡單的 JSON 解析找 set 值
                    char* set_ptr = strstr(json_buffer, "\"set\":");
                    if (set_ptr) {
                        int set_value = atoi(set_ptr + 6);
                        handle_set_command(set_value);
                    }
                }
            }
            // 處理 MQTTset topic
            else if (strncmp(event->topic, mqttset_topic, event->topic_len) == 0) {
                handle_mqttset_command(event->data, event->data_len);
            }
            // 處理 WIFIset topic  
            else if (strncmp(event->topic, wifiset_topic, event->topic_len) == 0) {
                handle_wifiset_command(event->data, event->data_len);
            }
            break;
            
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
            
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}

void mqtt_init(void) {
    if (!wifi_is_connected()) {
        ESP_LOGW(TAG, "WiFi not connected, skipping MQTT initialization");
        return;
    }
    
    // 生成 topics
    config_get_device_topic(device_topic, sizeof(device_topic));
    config_get_mqttset_topic(mqttset_topic, sizeof(mqttset_topic));
    config_get_wifiset_topic(wifiset_topic, sizeof(wifiset_topic));
    
    ESP_LOGI(TAG, "Initializing MQTT with host: %s:%d", 
             g_device_config.mqtt_host, g_device_config.mqtt_port);
    ESP_LOGI(TAG, "Device MAC: %s", g_device_config.device_mac);
    
    // 建立 MQTT URI
    char mqtt_uri[256];
    snprintf(mqtt_uri, sizeof(mqtt_uri), "mqtt://%s", g_device_config.mqtt_host);
    
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = mqtt_uri,
        .broker.address.port = g_device_config.mqtt_port,
        .credentials.username = g_device_config.mqtt_username,
        .credentials.authentication.password = g_device_config.mqtt_password,
        .credentials.client_id = g_device_config.device_mac,  // 使用 MAC 作為 client ID
    };
    
    mqtt_handle = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_handle == NULL) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return;
    }
    
    esp_mqtt_client_register_event(mqtt_handle, ESP_EVENT_ANY_ID, mqtt_event_callback, NULL);
    
    esp_err_t ret = esp_mqtt_client_start(mqtt_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start MQTT client: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "MQTT client started successfully");
    }
}

// 重新連線 MQTT（使用新設定）
void mqtt_reconnect_with_new_config(void) {
    if (mqtt_handle) {
        ESP_LOGI(TAG, "Reconnecting MQTT with new configuration");
        
        esp_mqtt_client_stop(mqtt_handle);
        esp_mqtt_client_destroy(mqtt_handle);
        mqtt_handle = NULL;
        mqtt_connected = false;
    }
    
    mqtt_init();
}

// 檢查 MQTT 連線狀態
bool mqtt_is_connected(void) {
    return mqtt_connected;
}

// 發送狀態訊息
void mqtt_publish_status(const char* status_msg) {
    if (mqtt_connected && mqtt_handle) {
        esp_mqtt_client_publish(mqtt_handle, device_topic, status_msg, 0, 1, 0);
    }
}

// 停止 MQTT 客戶端
void mqtt_stop(void) {
    if (mqtt_handle) {
        esp_mqtt_client_stop(mqtt_handle);
        mqtt_connected = false;
        ESP_LOGI(TAG, "MQTT client stopped");
    }
}

// 手動發佈裝置資訊
void mqtt_publish_device_info(void) {
    if (mqtt_connected && mqtt_handle) {
        mqtt_publish_wifi_info();
        mqtt_publish_mqtt_info();
        ESP_LOGI(TAG, "Published device info manually");
    }
}

// MQTT 重連處理任務（獨立任務，避免在回調中操作）
void mqtt_reconnect_task(void *pvParameters) {
    while (1) {
        if (mqtt_need_reconnect) {
            ESP_LOGI(TAG, "Reconnect task: Starting reconnection...");
            
            // 停止 MQTT
            if (mqtt_handle) {
                mqtt_connected = false;
                esp_mqtt_client_stop(mqtt_handle);
                esp_mqtt_client_destroy(mqtt_handle);
                mqtt_handle = NULL;
            }
            
            // 等待一下
            vTaskDelay(pdMS_TO_TICKS(2000));
            
            // 重新連線 WiFi
            wifi_reconnect_with_new_config();
            
            // 清除標記
            mqtt_need_reconnect = false;
            
            ESP_LOGI(TAG, "Reconnect task: Reconnection initiated");
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// 檢查是否需要重連
bool mqtt_check_reconnect_needed(void) {
    return mqtt_need_reconnect;
}