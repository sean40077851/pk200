#include "config_manager.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "cJSON.h"
#include <string.h>

#define TAG "CONFIG"
#define NVS_NAMESPACE "device_config"
#define CONFIG_KEY "config_data"
#define FW_VERSION "PK200-wentai-v0.6.2"

// 全域設定變數
device_config_t g_device_config;

// 設定預設值
void config_set_default_values(void) {
    // 預設 WiFi 設定
    strcpy(g_device_config.wifi_ssid,"A100");
    //strcpy(g_device_config.wifi_password, "00000000");
    
    // 預設 IP 設定 (ip_mode=2 表示預設)
    g_device_config.ip_mode = 2;
    strcpy(g_device_config.static_ip, "192.168.2.123");
    strcpy(g_device_config.gateway, "192.168.2.254"); 
    strcpy(g_device_config.subnet, "255.255.255.0");
    strcpy(g_device_config.current_ip, "0.0.0.0");
    
    // 預設 MQTT 設定
    strcpy(g_device_config.mqtt_host, "27.105.113.156");
    g_device_config.mqtt_port = 1883;
    strcpy(g_device_config.mqtt_username, "test");
    strcpy(g_device_config.mqtt_password, "test123");
    
    // 裝置資訊
    config_get_mac_address(g_device_config.device_mac);
    strcpy(g_device_config.fw_version, FW_VERSION);
    
    g_device_config.config_loaded = false;
    ESP_LOGI(TAG, "Default config loaded, MAC: %s", g_device_config.device_mac);
    ESP_LOGI("WIFI", "WIFI SSID, MAC: %s", g_device_config.wifi_ssid);
    
}

// 取得 MAC 地址 (無冒號格式)
void config_get_mac_address(char* mac_str_no_colon) {
    uint8_t mac[6];
    esp_wifi_get_mac(ESP_IF_WIFI_STA, mac);
    snprintf(mac_str_no_colon, 13, "%02x%02x%02x%02x%02x%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// 設定目前 IP
void config_set_current_ip(const char* ip) {
    strncpy(g_device_config.current_ip, ip, sizeof(g_device_config.current_ip) - 1);
    g_device_config.current_ip[sizeof(g_device_config.current_ip) - 1] = '\0';
}

// 設定韌體版本
void config_set_firmware_version(const char* version) {
    strncpy(g_device_config.fw_version, version, sizeof(g_device_config.fw_version) - 1);
    g_device_config.fw_version[sizeof(g_device_config.fw_version) - 1] = '\0';
}

// 取得韌體版本
const char* config_get_firmware_version(void) {
    return g_device_config.fw_version;
}

// 初始化設定管理
esp_err_t config_init(void) {
    ESP_LOGI(TAG, "Initializing config manager");
    
    esp_err_t ret = config_load();
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Failed to load config from NVS, using defaults");
        config_set_default_values();
        config_save();
    }
        // 新增：檢查 SSID 是否為空
    if (strlen(g_device_config.wifi_ssid) == 0) {
        ESP_LOGW(TAG, "SSID is empty, resetting to defaults");
        config_set_default_values();
        config_save();
    }
        ESP_LOGI(TAG, "Config loaded from NVS successfully");
        g_device_config.config_loaded = true;
    
    
    return ESP_OK;
}

// 儲存設定到 NVS
esp_err_t config_save(void) {
    nvs_handle_t nvs_handle;
    esp_err_t ret;
    
    ret = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS handle: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ret = nvs_set_blob(nvs_handle, CONFIG_KEY, &g_device_config, sizeof(device_config_t));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error saving config: %s", esp_err_to_name(ret));
    } else {
        ret = nvs_commit(nvs_handle);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Config saved to NVS");
        }
    }
    
    nvs_close(nvs_handle);
    return ret;
}

// 從 NVS 載入設定
esp_err_t config_load(void) {
    nvs_handle_t nvs_handle;
    esp_err_t ret;
    size_t required_size = sizeof(device_config_t);
    
    ret = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (ret != ESP_OK) {
        return ret;
    }
    
    ret = nvs_get_blob(nvs_handle, CONFIG_KEY, &g_device_config, &required_size);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Config not found in NVS: %s", esp_err_to_name(ret));
    } else if (required_size != sizeof(device_config_t)) {
        ESP_LOGW(TAG, "Config size mismatch");
        ret = ESP_ERR_INVALID_SIZE;
    }
    
    nvs_close(nvs_handle);
    return ret;
}

// 重設為預設值
esp_err_t config_reset_to_default(void) {
    ESP_LOGI(TAG, "Resetting config to defaults");

    // 清空整個結構體，避免殘留舊資料
    memset(&g_device_config, 0, sizeof(g_device_config));

    // 設定預設值 (SSID、密碼、MQTT host 等)
    config_set_default_values();

    // 標記狀態
    g_device_config.config_loaded = false;  // 表示目前是預設設定
    // 儲存到 NVS
    return config_save();
}

// 從 JSON 更新 MQTT 設定
esp_err_t config_update_mqtt_from_json(const char* json_str) {
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL) {
        ESP_LOGE(TAG, "Invalid JSON format for MQTT config");
        return ESP_ERR_INVALID_ARG;
    }
    
    ESP_LOGI(TAG, "Updating MQTT config from JSON");
    
    cJSON *ssid = cJSON_GetObjectItem(json, "ssid");
    if (cJSON_IsString(ssid) && (ssid->valuestring != NULL)) {
        strncpy(g_device_config.wifi_ssid, ssid->valuestring, sizeof(g_device_config.wifi_ssid) - 1);
        g_device_config.wifi_ssid[sizeof(g_device_config.wifi_ssid) - 1] = '\0';
        ESP_LOGI(TAG, "Updated WiFi SSID: %s", g_device_config.wifi_ssid);
    }
    
    cJSON *wifi_pwd = cJSON_GetObjectItem(json, "wifi_pwd");
    if (cJSON_IsString(wifi_pwd)) {
        if (wifi_pwd->valuestring && strlen(wifi_pwd->valuestring) > 0) {
            strncpy(g_device_config.wifi_password, wifi_pwd->valuestring,
                    sizeof(g_device_config.wifi_password) - 1);
        } else {
            // 確保清空密碼
            g_device_config.wifi_password[0] = '\0';
        }
        g_device_config.wifi_password[sizeof(g_device_config.wifi_password) - 1] = '\0';
        ESP_LOGI(TAG, "Updated WiFi password");
    }
    
    cJSON *host = cJSON_GetObjectItem(json, "host");
    if (cJSON_IsString(host) && (host->valuestring != NULL)) {
        strncpy(g_device_config.mqtt_host, host->valuestring, sizeof(g_device_config.mqtt_host) - 1);
        g_device_config.mqtt_host[sizeof(g_device_config.mqtt_host) - 1] = '\0';
        ESP_LOGI(TAG, "Updated MQTT host: %s", g_device_config.mqtt_host);
    }
    
    cJSON *port = cJSON_GetObjectItem(json, "port");
    if (cJSON_IsString(port) && (port->valuestring != NULL)) {
        g_device_config.mqtt_port = (uint16_t)atoi(port->valuestring);
        ESP_LOGI(TAG, "Updated MQTT port: %d", g_device_config.mqtt_port);
    }
    
    cJSON *user = cJSON_GetObjectItem(json, "user");
    if (cJSON_IsString(user) && (user->valuestring != NULL)) {
        strncpy(g_device_config.mqtt_username, user->valuestring, sizeof(g_device_config.mqtt_username) - 1);
        g_device_config.mqtt_username[sizeof(g_device_config.mqtt_username) - 1] = '\0';
        ESP_LOGI(TAG, "Updated MQTT username: %s", g_device_config.mqtt_username);
    }
    
    cJSON *mqtt_pwd = cJSON_GetObjectItem(json, "mqtt_pwd");
    if (cJSON_IsString(mqtt_pwd) && (mqtt_pwd->valuestring != NULL)) {
        strncpy(g_device_config.mqtt_password, mqtt_pwd->valuestring, sizeof(g_device_config.mqtt_password) - 1);
        g_device_config.mqtt_password[sizeof(g_device_config.mqtt_password) - 1] = '\0';
        ESP_LOGI(TAG, "Updated MQTT password");
    }
    
    strcpy(g_device_config.fw_version, FW_VERSION);
    cJSON_Delete(json);
    
    // 儲存更新的設定
    esp_err_t ret = config_save();
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "MQTT config updated and saved successfully");
    }
    
    return ret;
}

// 從 JSON 更新 WiFi IP 設定
esp_err_t config_update_wifi_from_json(const char* json_str) {
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL) {
        ESP_LOGE(TAG, "Invalid JSON format for WiFi config");
        return ESP_ERR_INVALID_ARG;
    }
    
    ESP_LOGI(TAG, "Updating WiFi IP config from JSON");
    
    cJSON *ip_mode = cJSON_GetObjectItem(json, "ip_mode");
    if (cJSON_IsString(ip_mode) && (ip_mode->valuestring != NULL)) {
        g_device_config.ip_mode = (uint8_t)atoi(ip_mode->valuestring);
        ESP_LOGI(TAG, "Updated IP mode: %d", g_device_config.ip_mode);
    }
    
    cJSON *fixip = cJSON_GetObjectItem(json, "fixip");
    if (cJSON_IsString(fixip) && (fixip->valuestring != NULL)) {
        strncpy(g_device_config.static_ip, fixip->valuestring, sizeof(g_device_config.static_ip) - 1);
        g_device_config.static_ip[sizeof(g_device_config.static_ip) - 1] = '\0';
        ESP_LOGI(TAG, "Updated static IP: %s", g_device_config.static_ip);
    }
    
    cJSON *gateway = cJSON_GetObjectItem(json, "gateway");
    if (cJSON_IsString(gateway) && (gateway->valuestring != NULL)) {
        strncpy(g_device_config.gateway, gateway->valuestring, sizeof(g_device_config.gateway) - 1);
        g_device_config.gateway[sizeof(g_device_config.gateway) - 1] = '\0';
        ESP_LOGI(TAG, "Updated gateway: %s", g_device_config.gateway);
    }
    
    cJSON *netmask = cJSON_GetObjectItem(json, "netmask");
    if (cJSON_IsString(netmask) && (netmask->valuestring != NULL)) {
        strncpy(g_device_config.subnet, netmask->valuestring, sizeof(g_device_config.subnet) - 1);
        g_device_config.subnet[sizeof(g_device_config.subnet) - 1] = '\0';
        ESP_LOGI(TAG, "Updated netmask: %s", g_device_config.subnet);
    }
    strcpy(g_device_config.fw_version, FW_VERSION);
    cJSON_Delete(json);
    
    // 儲存更新的設定
    esp_err_t ret = config_save();
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "WiFi IP config updated and saved successfully");
    }
    
    return ret;
}

// 生成裝置主 Topic
void config_get_device_topic(char* topic_buffer, size_t buffer_size) {
    snprintf(topic_buffer, buffer_size, "WE1/%s", g_device_config.device_mac);
}

// 生成 MQTT 設定 Topic
void config_get_mqttset_topic(char* topic_buffer, size_t buffer_size) {
    snprintf(topic_buffer, buffer_size, "WE1/%s/MQTTset", g_device_config.device_mac);
}

// 生成 WiFi 設定 Topic
void config_get_wifiset_topic(char* topic_buffer, size_t buffer_size) {
    snprintf(topic_buffer, buffer_size, "WE1/%s/WIFIset", g_device_config.device_mac);
}
