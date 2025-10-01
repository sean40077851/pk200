#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H
#include "esp_err.h"
#include <stdbool.h>

// 網路設定結構
typedef struct {
    // WiFi 設定
    char wifi_ssid[64];
    char wifi_password[128];
    
    // IP 設定  
    uint8_t ip_mode;        // 0=DHCP, 1=固定IP, 2=預設
    char static_ip[16];     // 固定IP
    char gateway[16];       // 閘道
    char subnet[16];        // 子網路遮罩
    char current_ip[16];    // 目前IP
    
    // MQTT 設定
    char mqtt_host[128];
    uint16_t mqtt_port;
    char mqtt_username[64];
    char mqtt_password[128];
    
    // 裝置資訊
    char device_mac[13];    // MAC地址(無冒號)，例如: 40915149e492
    char fw_version[64];    // 韌體版本
    
    bool config_loaded;     // 設定是否已載入
} device_config_t;

// 全域設定變數
extern device_config_t g_device_config;

// 設定管理函式
esp_err_t config_init(void);
esp_err_t config_save(void);
esp_err_t config_load(void);
esp_err_t config_reset_to_default(void);
void config_set_default_values(void);
// 設定更新函式
esp_err_t config_update_wifi_from_json(const char* json_str);
esp_err_t config_update_ip_from_json(const char* json_str);
esp_err_t config_update_mqtt_from_json(const char* json_str);

// MAC 地址處理
void config_get_mac_address(char* mac_str_no_colon);
void config_set_current_ip(const char* ip);

// 裝置資訊
void config_set_firmware_version(const char* version);
const char* config_get_firmware_version(void);

// Topic 生成器
void config_get_device_topic(char* topic_buffer, size_t buffer_size);
void config_get_mqttset_topic(char* topic_buffer, size_t buffer_size);
void config_get_wifiset_topic(char* topic_buffer, size_t buffer_size);

#endif // CONFIG_MANAGER_Hs