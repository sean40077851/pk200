#include <stdio.h>
#include <string.h>
#include "mqtt_client.h"
#include "esp_log.h"
#include "esp_system.h"
#include "my_mqtt.h"
#include "config_manager.h"
#include "wifi.h"
#include "ota.h"

#define TAG "MQTT"
esp_mqtt_client_handle_t mqtt_handle = NULL;
static bool mqtt_connected = false;
static bool mqtt_need_reconnect = false;

static char device_topic[64];    // WE1/{MAC}
static char mqttset_topic[64];   // WE1/{MAC}/MQTTset
static char wifiset_topic[64];   // WE1/{MAC}/WIFIset
static char ota_status_topic[64]; // WE1/{MAC}/OTA/status
static const char *ota_set_topic = "wi485update"; // 共用 OTA 訂閱 topic

extern void wifi_reconnect_with_new_config(void);
extern void wifi_apply_ip_config(void);
extern bool wifi_is_connected(void);

// OTA 狀態回呼（MQTT 發佈）
static void ota_status_mqtt_callback(ota_status_t status, esp_err_t error)
{
    if (!mqtt_connected || !mqtt_handle) return;

    const char *status_str = "unknown";
    switch (status) {
        case OTA_STATUS_IN_PROGRESS: status_str = "ota_started"; break;
        case OTA_STATUS_SUCCESS:     status_str = "ota_success"; break;
        case OTA_STATUS_FAILED:      status_str = "ota_failed";  break;
        default: break;
    }

    char payload[256];
    if (status == OTA_STATUS_FAILED) {
        snprintf(payload, sizeof(payload),
                 "{\"mac\":\"%s\",\"status\":\"%s\",\"error\":\"%s\"}",
                 g_device_config.device_mac, status_str, esp_err_to_name(error));
    } else {
        snprintf(payload, sizeof(payload),
                 "{\"mac\":\"%s\",\"status\":\"%s\"}",
                 g_device_config.device_mac, status_str);
    }

    esp_mqtt_client_publish(mqtt_handle, ota_status_topic, payload, 0, 1, 0);
    ESP_LOGI(TAG, "OTA status published: %s", payload);
}

// 發佈 Wi-Fi 資訊
static void mqtt_publish_wifi_info(void)
{
    if (!mqtt_connected || !mqtt_handle) return;

    char payload[512];
    snprintf(payload, sizeof(payload),
             "{\"mac\":\"%s\",\"FWVer\":\"%s\",\"ip_mode\":\"%d\","
             "\"ip\":\"%s\",\"gateway\":\"%s\",\"subnet\":\"%s\"}",
             g_device_config.device_mac,
             g_device_config.fw_version,
             g_device_config.ip_mode,
             g_device_config.current_ip,
             g_device_config.gateway,
             g_device_config.subnet);

    esp_mqtt_client_publish(mqtt_handle, "WE1/WIFIinfo", payload, 0, 1, 0);
    ESP_LOGI(TAG, "Published WiFi info");
}

// 發佈 MQTT 資訊
static void mqtt_publish_mqtt_info(void)
{
    if (!mqtt_connected || !mqtt_handle) return;

    char payload[512];
    snprintf(payload, sizeof(payload),
             "{\"mac\":\"%s\",\"ssid\":\"%s\",\"wifi_pwd\":\"%s\","
             "\"host\":\"%s\",\"port\":\"%d\",\"user\":\"%s\"}",
             g_device_config.device_mac,
             g_device_config.wifi_ssid,
             g_device_config.wifi_password,
             g_device_config.mqtt_host,
             g_device_config.mqtt_port,
             g_device_config.mqtt_username);

    esp_mqtt_client_publish(mqtt_handle, "WE1/MQTTinfo", payload, 0, 1, 0);
    ESP_LOGI(TAG, "Published MQTT info");
}

// set 指令處理
static void handle_set_command(int set_value)
{
    ESP_LOGI(TAG, "Received set command: %d", set_value);

    switch (set_value) {
        case 3:
            ESP_LOGI(TAG, "Restarting device...");
            if (mqtt_connected && mqtt_handle) {
                char ack[128];
                snprintf(ack, sizeof(ack), "{\"status\":\"restarting\",\"mac\":\"%s\"}",
                         g_device_config.device_mac);
                esp_mqtt_client_publish(mqtt_handle, device_topic, ack, 0, 0, 0);
            }
            vTaskDelay(pdMS_TO_TICKS(2000));
            esp_restart();
            break;

        case 7:
            ESP_LOGI(TAG, "MQTT setup mode - waiting for MQTTset data");
            break;

        case 8:
            ESP_LOGI(TAG, "Sending WiFi & MQTT info...");
            vTaskDelay(pdMS_TO_TICKS(100));  // 短暫延遲避免訊息擁塞
            mqtt_publish_wifi_info();
            vTaskDelay(pdMS_TO_TICKS(100));
            mqtt_publish_mqtt_info();
            break;

        case 9:
            ESP_LOGI(TAG, "WiFi IP setup mode - waiting for WIFIset data");
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
        
        // 檢查設定是否真的有改變
        // 如果 SSID 和 MQTT host 都沒變，就不需要重連
        // 這可以避免重複收到相同設定時被 AP 踢掉
        static char last_ssid[64] = {0};
        static char last_host[128] = {0};
        
        bool need_reconnect = false;
        if (strcmp(last_ssid, g_device_config.wifi_ssid) != 0 ||
            strcmp(last_host, g_device_config.mqtt_host) != 0) {
            need_reconnect = true;
            strcpy(last_ssid, g_device_config.wifi_ssid);
            strcpy(last_host, g_device_config.mqtt_host);
        }
        
        if (need_reconnect) {
            ESP_LOGI(TAG, "Configuration changed, device will reconnect...");
            mqtt_need_reconnect = true;
        } else {
            ESP_LOGI(TAG, "Configuration unchanged, no reconnection needed");
        }
        
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
        
        // 如果是 IP 模式變更，需要重連
        // 只有從固定IP切到DHCP或反之才需要重連
        // 這裡先套用設定，不立即重連（避免被 AP 踢掉）
        ESP_LOGI(TAG, "IP config will be applied on next connection");
        
    } else {
        ESP_LOGE(TAG, "Failed to update WiFi IP configuration");
    }
}

// MQTT 訊息事件
void mqtt_event_callback(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            mqtt_connected = true;
            ESP_LOGI(TAG, "MQTT connected!");

            esp_mqtt_client_subscribe(mqtt_handle, device_topic, 1);
            esp_mqtt_client_subscribe(mqtt_handle, mqttset_topic, 1);
            esp_mqtt_client_subscribe(mqtt_handle, wifiset_topic, 1);
            esp_mqtt_client_subscribe(mqtt_handle, ota_set_topic, 1);

            mqtt_publish_wifi_info();
            mqtt_publish_mqtt_info();

            ESP_LOGI(TAG, "Subscribed: %s, %s, %s, %s",
                     device_topic, mqttset_topic, wifiset_topic, ota_set_topic);
            check_firmware_version();
            break;

        case MQTT_EVENT_DISCONNECTED:
            mqtt_connected = false;
            ESP_LOGI(TAG, "MQTT disconnected");
            break;

        case MQTT_EVENT_DATA:
    ESP_LOGI(TAG, "MQTT_EVENT_DATA");
    printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
    printf("DATA=%.*s\r\n", event->data_len, event->data);

    // 先處理共用 OTA topic
    if (strncmp(event->topic,ota_set_topic, event->topic_len) == 0) {
    char json_buffer[256];
    memcpy(json_buffer, event->data, event->data_len);
    json_buffer[event->data_len] = '\0';
    ESP_LOGI(TAG, "OTA JSON received: %s", json_buffer);

    char *url_start = strstr(json_buffer, "\"url\"");
    if (url_start) {
        url_start = strchr(url_start, ':');
        if (url_start) {
            // 跳過 ": " 和引號
            while (*url_start && (*url_start == ':' || *url_start == ' ' || *url_start == '\"'))
                url_start++;
            char *url_end = strchr(url_start, '\"');
            if (url_end) {
                char ota_url[256];
                int len = url_end - url_start;
                if (len >= sizeof(ota_url))
                    len = sizeof(ota_url) - 1;
                strncpy(ota_url, url_start, len);
                ota_url[len] = '\0';
                ESP_LOGI(TAG, "Starting OTA from: %s", ota_url);

                esp_err_t ret = ota_start_async(ota_url);
                if (ret == ESP_OK)
                    ESP_LOGI(TAG, "OTA started successfully");
                else
                    ESP_LOGE(TAG, "OTA start failed: %s", esp_err_to_name(ret));
              }
         }
        }
    }
    // 處理主控制 topic (WE1/{MAC})1
    else if (strncmp(event->topic, device_topic, event->topic_len) == 0) {
        char json_buffer[128];
        if (event->data_len < sizeof(json_buffer)) {
            memcpy(json_buffer, event->data, event->data_len);
            json_buffer[event->data_len] = '\0';
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
    default:
            break;
    }
}

// 初始化 MQTT
void mqtt_init(void)
{
    if (!wifi_is_connected()) {
        ESP_LOGW(TAG, "WiFi not connected, skip MQTT init");
        return;
    }

    config_get_device_topic(device_topic, sizeof(device_topic));
    config_get_mqttset_topic(mqttset_topic, sizeof(mqttset_topic));
    config_get_wifiset_topic(wifiset_topic, sizeof(wifiset_topic));
    snprintf(ota_status_topic, sizeof(ota_status_topic), "WE1/%s/OTA/status", g_device_config.device_mac);

    ota_register_status_callback(ota_status_mqtt_callback);

    char mqtt_uri[256];
    snprintf(mqtt_uri, sizeof(mqtt_uri), "mqtt://%s", g_device_config.mqtt_host);

    esp_mqtt_client_config_t cfg = {
        .broker.address.uri = mqtt_uri,
        .broker.address.port = g_device_config.mqtt_port,
        .credentials.username = g_device_config.mqtt_username,
        .credentials.authentication.password = g_device_config.mqtt_password,
        .credentials.client_id = g_device_config.device_mac,
    };

    mqtt_handle = esp_mqtt_client_init(&cfg);
    esp_mqtt_client_register_event(mqtt_handle, ESP_EVENT_ANY_ID, mqtt_event_callback, NULL);
    esp_mqtt_client_start(mqtt_handle);
    ESP_LOGI(TAG, "MQTT started: %s:%d", g_device_config.mqtt_host, g_device_config.mqtt_port);
}

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
            ESP_LOGI(TAG, "Reconnect task: Starting reconnection in 3 seconds...");
            
            // 等待3秒讓當前的 MQTT 訊息處理完成
            vTaskDelay(pdMS_TO_TICKS(3000));
            
            // 停止 MQTT
            if (mqtt_handle) {
                mqtt_connected = false;
                ESP_LOGI(TAG, "Stopping MQTT client...");
                esp_mqtt_client_stop(mqtt_handle);
                vTaskDelay(pdMS_TO_TICKS(1000));
                esp_mqtt_client_destroy(mqtt_handle);
                mqtt_handle = NULL;
                ESP_LOGI(TAG, "MQTT client stopped");
            }
            
            // 再等待1秒讓系統穩定
            vTaskDelay(pdMS_TO_TICKS(1000));
            
            // 重新連線 WiFi
            ESP_LOGI(TAG, "Starting WiFi reconnection...");
            wifi_reconnect_with_new_config();
            
            // 清除標記
            mqtt_need_reconnect = false;
            
            ESP_LOGI(TAG, "Reconnect task: Reconnection process completed");
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// 檢查是否需要重連
bool mqtt_check_reconnect_needed(void) {
    return mqtt_need_reconnect;
}