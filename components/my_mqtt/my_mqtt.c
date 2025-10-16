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
esp_mqtt_client_handle_t mqtt_handle = NULL;   // MQTT 客戶端 handle，全域變數
static bool mqtt_connected = false;            // MQTT 連線狀態旗標
static bool mqtt_need_reconnect = false;       // 是否需要重連 MQTT

static char device_topic[64];                  // 儲存 WE1/{MAC} topic 字串
static char mqttset_topic[64];                 // 儲存 WE1/{MAC}/MQTTset topic 字串
static char wifiset_topic[64];                 // 儲存 WE1/{MAC}/WIFIset topic 字串
static char ota_status_topic[64];              // 儲存 WE1/{MAC}/OTA/status topic 字串
static const char *ota_set_topic = "wi485update"; // OTA 廣播 topic 字串

extern void wifi_reconnect_with_new_config(void); // 外部 WiFi 重新連線函式
extern void wifi_apply_ip_config(void);           // 外部 IP 設定函式
extern bool wifi_is_connected(void);              // 外部 WiFi 連線狀態查詢

// OTA 狀態回呼（MQTT 發佈）
static void ota_status_mqtt_callback(ota_status_t status, esp_err_t error)
{
    if (!mqtt_connected || !mqtt_handle) return; // 若未連線則不處理

    const char *status_str = "unknown";          // 預設狀態字串
    switch (status) {                            // 根據 OTA 狀態設定字串
        case OTA_STATUS_IN_PROGRESS: status_str = "ota_started"; break;
        case OTA_STATUS_SUCCESS:     status_str = "ota_success"; break;
        case OTA_STATUS_FAILED:      status_str = "ota_failed";  break;
        default: break;
    }

    char payload[256];                           // 準備發佈的 JSON 字串
    if (status == OTA_STATUS_FAILED) {           // 若失敗則帶錯誤訊息
        snprintf(payload, sizeof(payload),
                 "{\"mac\":\"%s\",\"status\":\"%s\",\"error\":\"%s\"}",
                 g_device_config.device_mac, status_str, esp_err_to_name(error));
    } else {                                     // 其他狀態只帶 mac 與狀態
        snprintf(payload, sizeof(payload),
                 "{\"mac\":\"%s\",\"status\":\"%s\"}",
                 g_device_config.device_mac, status_str);
    }

    esp_mqtt_client_publish(mqtt_handle, ota_status_topic, payload, 0, 1, 0); // 發佈狀態
    ESP_LOGI(TAG, "OTA status published: %s", payload);                       // 日誌
}

// 發佈 Wi-Fi 資訊
static void mqtt_publish_wifi_info(void)
{
    if (!mqtt_connected || !mqtt_handle) return; // 未連線不處理

    char payload[512];                           // JSON 資料緩衝區
    snprintf(payload, sizeof(payload),
             "{\"mac\":\"%s\",\"FWVer\":\"%s\",\"ip_mode\":\"%d\","
             "\"ip\":\"%s\",\"gateway\":\"%s\",\"subnet\":\"%s\"}",
             g_device_config.device_mac,
             g_device_config.fw_version,
             g_device_config.ip_mode,
             g_device_config.current_ip,
             g_device_config.gateway,
             g_device_config.subnet);

    esp_mqtt_client_publish(mqtt_handle, "WE1/WIFIinfo", payload, 0, 1, 0); // 發佈
    ESP_LOGI(TAG, "Published WiFi info");                                   // 日誌
}

// 發佈 MQTT 資訊
static void mqtt_publish_mqtt_info(void)
{
    if (!mqtt_connected || !mqtt_handle) return; // 未連線不處理

    char payload[512];                           // JSON 資料緩衝區
    snprintf(payload, sizeof(payload),
             "{\"mac\":\"%s\",\"ssid\":\"%s\",\"wifi_pwd\":\"%s\","
             "\"host\":\"%s\",\"port\":\"%d\",\"user\":\"%s\"}",
             g_device_config.device_mac,
             g_device_config.wifi_ssid,
             g_device_config.wifi_password,
             g_device_config.mqtt_host,
             g_device_config.mqtt_port,
             g_device_config.mqtt_username);

    esp_mqtt_client_publish(mqtt_handle, "WE1/MQTTinfo", payload, 0, 1, 0); // 發佈
    ESP_LOGI(TAG, "Published MQTT info");                                   // 日誌
}

// set 指令處理
static void handle_set_command(int set_value)
{
    ESP_LOGI(TAG, "Received set command: %d", set_value); // 日誌

    switch (set_value) {
        case 3: // 重啟
            ESP_LOGI(TAG, "Restarting device...");
            if (mqtt_connected && mqtt_handle) {
                char ack[128];
                snprintf(ack, sizeof(ack), "{\"status\":\"restarting\",\"mac\":\"%s\"}",
                         g_device_config.device_mac);
                esp_mqtt_client_publish(mqtt_handle, device_topic, ack, 0, 0, 0); // 發佈重啟通知
            }
            vTaskDelay(pdMS_TO_TICKS(2000)); // 等待 2 秒
            esp_restart();                   // 重新啟動
            break;

        case 7: // 進入 MQTT 設定模式
            ESP_LOGI(TAG, "MQTT setup mode - waiting for MQTTset data");
            break;

        case 8: // 發送 WiFi & MQTT info
            ESP_LOGI(TAG, "Sending WiFi & MQTT info...");
            vTaskDelay(pdMS_TO_TICKS(100));  // 短暫延遲避免訊息擁塞
            mqtt_publish_wifi_info();        // 發佈 WiFi info
            vTaskDelay(pdMS_TO_TICKS(100));
            mqtt_publish_mqtt_info();        // 發佈 MQTT info
            break;

        case 9: // 進入 WiFi IP 設定模式
            ESP_LOGI(TAG, "WiFi IP setup mode - waiting for WIFIset data");
            break;

        default: // 其他未知指令
            ESP_LOGW(TAG, "Unknown set command: %d", set_value);
            break;
    }
}

// 處理 MQTTset 訊息
static void handle_mqttset_command(const char* data, int data_len) {
    char json_buffer[512];                        // JSON 緩衝區
    
    if (data_len >= sizeof(json_buffer)) {        // 檢查資料長度
        ESP_LOGE(TAG, "MQTTset data too large");
        return;
    }
    
    memcpy(json_buffer, data, data_len);          // 複製資料
    json_buffer[data_len] = '\0';                 // 補字串結尾
    
    ESP_LOGI(TAG, "Received MQTTset: %s", json_buffer); // 日誌
    
    // 更新 MQTT 設定
    esp_err_t ret = config_update_mqtt_from_json(json_buffer);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "MQTT configuration updated successfully");
        
        // 檢查設定是否真的有改變
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
    char json_buffer[512];                        // JSON 緩衝區
    
    if (data_len >= sizeof(json_buffer)) {        // 檢查資料長度
        ESP_LOGE(TAG, "WIFIset data too large");
        return;
    }
    
    memcpy(json_buffer, data, data_len);          // 複製資料
    json_buffer[data_len] = '\0';                 // 補字串結尾
    
    ESP_LOGI(TAG, "Received WIFIset: %s", json_buffer); // 日誌
    
    // 更新 WiFi IP 設定
    esp_err_t ret = config_update_wifi_from_json(json_buffer);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "WiFi IP configuration updated successfully");
        ESP_LOGI(TAG, "IP config will be applied on next connection");
    } else {
        ESP_LOGE(TAG, "Failed to update WiFi IP configuration");
    }
}

// MQTT 訊息事件
void mqtt_event_callback(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data; // 取得事件資料

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:                // 連線成功
            mqtt_connected = true;
            ESP_LOGI(TAG, "MQTT connected!");

            esp_mqtt_client_subscribe(mqtt_handle, device_topic, 1);    // 訂閱主 topic
            esp_mqtt_client_subscribe(mqtt_handle, mqttset_topic, 1);   // 訂閱 MQTTset
            esp_mqtt_client_subscribe(mqtt_handle, wifiset_topic, 1);   // 訂閱 WIFIset
            esp_mqtt_client_subscribe(mqtt_handle, ota_set_topic, 1);   // 訂閱 OTA 廣播

            mqtt_publish_wifi_info();                                   // 發佈 WiFi info
            mqtt_publish_mqtt_info();                                   // 發佈 MQTT info

            ESP_LOGI(TAG, "Subscribed: %s, %s, %s, %s",
                     device_topic, mqttset_topic, wifiset_topic, ota_set_topic);
            check_firmware_version();

            break;

        case MQTT_EVENT_DISCONNECTED:             // 斷線
            mqtt_connected = false;
            ESP_LOGI(TAG, "MQTT disconnected");
            break;

        case MQTT_EVENT_DATA:                     // 收到資料
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic); // 印出 topic
            printf("DATA=%.*s\r\n", event->data_len, event->data);    // 印出資料

            // 處理 OTA 廣播 topic
            if (strncmp(event->topic, ota_set_topic, event->topic_len) == 0) {
                char json_buffer[256]; // 宣告一個緩衝區存放 JSON 字串
                memcpy(json_buffer, event->data, event->data_len); // 將收到的資料複製到緩衝區
                json_buffer[event->data_len] = '\0'; // 補上字串結尾符號，確保是合法 C 字串
                ESP_LOGI(TAG, "OTA JSON received: %s", json_buffer); // 印出收到的 JSON

                // 在 JSON 字串中尋找 "url" 欄位
                char *url_start = strstr(json_buffer, "\"url\"");
                if (url_start) { // 找到 "url" 欄位
                    url_start = strchr(url_start, ':'); // 找到冒號（:）的位置
                    if (url_start) {
                        // 跳過冒號、空白與引號，直到真正的 URL 開頭
                        while (*url_start && (*url_start == ':' || *url_start == ' ' || *url_start == '\"'))
                            url_start++;
                        // 找到 URL 結尾的引號
                        char *url_end = strchr(url_start, '\"');
                        if (url_end) {
                            char ota_url[256]; // 宣告一個緩衝區存放 URL
                            int len = url_end - url_start; // 計算 URL 長度
                            if (len >= sizeof(ota_url))
                                len = sizeof(ota_url) - 1; // 避免溢位
                            strncpy(ota_url, url_start, len); // 複製 URL 到 ota_url
                            ota_url[len] = '\0'; // 補上字串結尾
                            ESP_LOGI(TAG, "Starting OTA from: %s", ota_url); // 印出要下載的 URL
                            
                            // 啟動 OTA 非同步下載與更新
                            esp_err_t ret = ota_start_async(ota_url);
                            if (ret == ESP_OK)
                                ESP_LOGI(TAG, "OTA started successfully"); // 成功啟動 OTA
                            else
                                ESP_LOGE(TAG, "OTA start failed: %s", esp_err_to_name(ret)); // 啟動失敗
                        }
                    }
                }
            }
            // 處理主控制 topic (WE1/{MAC})
            else if (strncmp(event->topic, device_topic, event->topic_len) == 0) {
                char json_buffer[128]; // 宣告緩衝區存放資料
                if (event->data_len < sizeof(json_buffer)) { // 檢查資料長度
                    memcpy(json_buffer, event->data, event->data_len); // 複製資料
                    json_buffer[event->data_len] = '\0'; // 補上字串結尾
                    char* set_ptr = strstr(json_buffer, "\"set\":"); // 尋找 "set" 欄位
                    if (set_ptr) {
                        int set_value = atoi(set_ptr + 6); // 取得 set 欄位的數值
                        handle_set_command(set_value); // 執行對應的 set 指令
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
    if (!wifi_is_connected()) {                      // 若 WiFi 未連線則不初始化
        ESP_LOGW(TAG, "WiFi not connected, skip MQTT init");
        return;
    }

    config_get_device_topic(device_topic, sizeof(device_topic));   // 取得主 topic
    config_get_mqttset_topic(mqttset_topic, sizeof(mqttset_topic));// 取得 MQTTset topic
    config_get_wifiset_topic(wifiset_topic, sizeof(wifiset_topic));// 取得 WIFIset topic
    snprintf(ota_status_topic, sizeof(ota_status_topic), "WE1/%s/OTA/status", g_device_config.device_mac); // OTA 狀態 topic

    ota_register_status_callback(ota_status_mqtt_callback);        // 註冊 OTA 狀態回呼

    char mqtt_uri[256];
    snprintf(mqtt_uri, sizeof(mqtt_uri), "mqtt://%s", g_device_config.mqtt_host); // 組合 MQTT URI

    esp_mqtt_client_config_t cfg = {                               // 設定 MQTT 參數
        .broker.address.uri = mqtt_uri,
        .broker.address.port = g_device_config.mqtt_port,
        .credentials.username = g_device_config.mqtt_username,
        .credentials.authentication.password = g_device_config.mqtt_password,
        .credentials.client_id = g_device_config.device_mac,
    };

    mqtt_handle = esp_mqtt_client_init(&cfg);                     // 初始化 MQTT 客戶端
    esp_mqtt_client_register_event(mqtt_handle, ESP_EVENT_ANY_ID, mqtt_event_callback, NULL); // 註冊事件
    esp_mqtt_client_start(mqtt_handle);                           // 啟動 MQTT
    ESP_LOGI(TAG, "MQTT started: %s:%d", g_device_config.mqtt_host, g_device_config.mqtt_port); // 日誌
}

// 重新連線 MQTT
void mqtt_reconnect_with_new_config(void) {
    if (mqtt_handle) {
        ESP_LOGI(TAG, "Reconnecting MQTT with new configuration");
        esp_mqtt_client_stop(mqtt_handle);        // 停止 MQTT
        esp_mqtt_client_destroy(mqtt_handle);     // 銷毀 MQTT
        mqtt_handle = NULL;
        mqtt_connected = false;
    }
    mqtt_init();                                  // 重新初始化
}

// 檢查 MQTT 連線狀態
bool mqtt_is_connected(void) {
    return mqtt_connected;                        // 回傳連線狀態
}

// 發送狀態訊息
void mqtt_publish_status(const char* status_msg) {
    if (mqtt_connected && mqtt_handle) {
        esp_mqtt_client_publish(mqtt_handle, device_topic, status_msg, 0, 1, 0); // 發佈狀態
    }
}

// 停止 MQTT 客戶端
void mqtt_stop(void) {
    if (mqtt_handle) {
        esp_mqtt_client_stop(mqtt_handle);        // 停止 MQTT
        mqtt_connected = false;
        ESP_LOGI(TAG, "MQTT client stopped");     // 日誌
    }
}

// 手動發佈裝置資訊
void mqtt_publish_device_info(void) {
    if (mqtt_connected && mqtt_handle) {
        mqtt_publish_wifi_info();                 // 發佈 WiFi info
        mqtt_publish_mqtt_info();                 // 發佈 MQTT info
        ESP_LOGI(TAG, "Published device info manually"); // 日誌
    }
}

// MQTT 重連處理任務（獨立任務，避免在回調中操作）
void mqtt_reconnect_task(void *pvParameters) {
    while (1) {
        if (mqtt_need_reconnect) {
            ESP_LOGI(TAG, "Reconnect task: Starting reconnection in 3 seconds...");
            vTaskDelay(pdMS_TO_TICKS(3000));      // 等待 3 秒
            if (mqtt_handle) {
                mqtt_connected = false;
                ESP_LOGI(TAG, "Stopping MQTT client...");
                esp_mqtt_client_stop(mqtt_handle);    // 停止 MQTT
                vTaskDelay(pdMS_TO_TICKS(1000));      // 等待 1 秒
                esp_mqtt_client_destroy(mqtt_handle); // 銷毀 MQTT
                mqtt_handle = NULL;
                ESP_LOGI(TAG, "MQTT client stopped");
            }
            vTaskDelay(pdMS_TO_TICKS(1000));         // 再等 1 秒
            ESP_LOGI(TAG, "Starting WiFi reconnection...");
            wifi_reconnect_with_new_config();         // 重新連 WiFi
            mqtt_need_reconnect = false;              // 清除重連旗標
            ESP_LOGI(TAG, "Reconnect task: Reconnection process completed");
        }
        vTaskDelay(pdMS_TO_TICKS(100));              // 每 100ms 檢查一次
    }
}

// 檢查是否需要重連
bool mqtt_check_reconnect_needed(void) {
    return mqtt_need_reconnect;                      // 回傳重連旗標
}