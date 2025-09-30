#include <stdio.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_sntp.h"
#include "esp_netif.h"
#include "config_manager.h"
#include <time.h>
#include <string.h>

#define TAG "WIFI"
static esp_netif_t *sta_netif = NULL;
static bool wifi_connected = false;
static int retry_count = 0;
static const int max_retry = 5;

// 外部函數聲明
extern void mqtt_init(void);
extern void mqtt_reconnect_with_new_config(void);

void sntp_init_time(void) {
    if (esp_sntp_enabled()) {   // 已經在跑，就不要重設
            ESP_LOGI(TAG, "SNTP already initialized, skip");
            return;
        }

        ESP_LOGI(TAG, "Initializing SNTP...");
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, "pool.ntp.org");
        sntp_init();
        setenv("TZ", "CST-8", 1);
        tzset();
}

// 套用 IP 設定 (DHCP 或固定IP)
void wifi_apply_ip_config(void) {
    if (!sta_netif) {
        ESP_LOGE(TAG, "STA netif not initialized");
        return;
    }
    
    ESP_LOGI(TAG, "Applying IP config, mode: %d", g_device_config.ip_mode);
    
    if (g_device_config.ip_mode == 0) {
        // DHCP 模式
        ESP_LOGI(TAG, "Setting DHCP mode");
        esp_netif_dhcpc_start(sta_netif);
        
    } else if (g_device_config.ip_mode == 1) {
        // 固定IP模式
        ESP_LOGI(TAG, "Setting static IP: %s", g_device_config.static_ip);
        
        // 停止 DHCP
        esp_netif_dhcpc_stop(sta_netif);
        
        // 設定固定IP
        esp_netif_ip_info_t ip_info;
        memset(&ip_info, 0, sizeof(esp_netif_ip_info_t));
        
        ip_info.ip.addr = esp_ip4addr_aton(g_device_config.static_ip);
        ip_info.gw.addr = esp_ip4addr_aton(g_device_config.gateway);
        ip_info.netmask.addr = esp_ip4addr_aton(g_device_config.subnet);
        
        esp_err_t ret = esp_netif_set_ip_info(sta_netif, &ip_info);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to set static IP: %s", esp_err_to_name(ret));
        } else {
            ESP_LOGI(TAG, "Static IP set successfully");
            config_set_current_ip(g_device_config.static_ip);
        }
    } else {
        // ip_mode == 2: 預設模式，使用 DHCP
        ESP_LOGI(TAG, "Using default DHCP mode");
        esp_netif_dhcpc_start(sta_netif);
    }
}

// WiFi 重新連線（使用新設定）
void wifi_reconnect_with_new_config(void) {
    ESP_LOGI(TAG, "=== Reconnecting WiFi with new configuration ===");
    
    wifi_connected = false;
    retry_count = 0;
    
    // 先優雅地斷開連線
    ESP_LOGI(TAG, "Gracefully disconnecting from current AP...");
    esp_wifi_disconnect();
    vTaskDelay(pdMS_TO_TICKS(1000));  // 等待斷線完成
    
    // 停止 WiFi
    ESP_LOGI(TAG, "Stopping WiFi driver...");
    esp_wifi_stop();
    vTaskDelay(pdMS_TO_TICKS(2000));  // 等待停止完成
    
    // 使用新設定重新啟動
    ESP_LOGI(TAG, "Configuring WiFi with new settings...");
    wifi_config_t wifi_config = {0};
    strcpy((char*)wifi_config.sta.ssid, g_device_config.wifi_ssid);
    strcpy((char*)wifi_config.sta.password, g_device_config.wifi_password);
    
    // 根據密碼設定驗證模式
    if (strlen(g_device_config.wifi_password) == 0) {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_OPEN;
        ESP_LOGI(TAG, "Using OPEN authentication (no password)");
    } else {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        ESP_LOGI(TAG, "Using WPA2_PSK authentication");
    }
    
    wifi_config.sta.pmf_cfg.capable = true;
    wifi_config.sta.pmf_cfg.required = false;
    
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    
    // 套用 IP 設定
    wifi_apply_ip_config();
    
    // 啟動 WiFi
    ESP_LOGI(TAG, "Starting WiFi with SSID: %s", g_device_config.wifi_ssid);
    ESP_ERROR_CHECK(esp_wifi_start());
    
    ESP_LOGI(TAG, "=== WiFi reconnection initiated ===");
}

// WiFi 事件處理函式 - 加強錯誤處理
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "WiFi STA started, connecting to SSID: %s", g_device_config.wifi_ssid);
        esp_wifi_connect();
        
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        wifi_event_sta_disconnected_t* disconnected = (wifi_event_sta_disconnected_t*) event_data;
        wifi_connected = false;
        
        ESP_LOGW(TAG, "WiFi disconnected, reason: %d", disconnected->reason);
        
        // 根據斷線原因給出具體資訊
        switch (disconnected->reason) {
            case WIFI_REASON_NO_AP_FOUND:
                ESP_LOGE(TAG, "AP not found - Check if SSID '%s' exists", g_device_config.wifi_ssid);
                break;
            case WIFI_REASON_AUTH_FAIL:
                ESP_LOGE(TAG, "Authentication failed - Check WiFi password");
                break;
            case WIFI_REASON_ASSOC_LEAVE:
                ESP_LOGW(TAG, "Disconnected by AP");
                break;
            case WIFI_REASON_CONNECTION_FAIL:
                ESP_LOGE(TAG, "Connection failed - Check AP settings");
                break;
            default:
                ESP_LOGW(TAG, "Other disconnect reason: %d", disconnected->reason);
                break;
        }
        
        // 重試連線邏輯
        if (retry_count < max_retry) {
            retry_count++;
            ESP_LOGI(TAG, "Retry connecting (%d/%d)...", retry_count, max_retry);
            vTaskDelay(pdMS_TO_TICKS(5000));  // 等待5秒再重試
            esp_wifi_connect();
        } else {
            ESP_LOGE(TAG, "Max retry reached. Please check WiFi settings:");
            ESP_LOGE(TAG, "  SSID: %s", g_device_config.wifi_ssid);
            ESP_LOGE(TAG, "  Password: %s", strlen(g_device_config.wifi_password) > 0 ? "***SET***" : "***EMPTY***");
            ESP_LOGE(TAG, "Waiting 30 seconds before next attempt...");
            vTaskDelay(pdMS_TO_TICKS(30000));
            retry_count = 0;  // 重設重試計數
            esp_wifi_connect();
        }
        
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "✅ WiFi connected successfully!");
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI(TAG, "Gateway: " IPSTR, IP2STR(&event->ip_info.gw));
        ESP_LOGI(TAG, "Netmask: " IPSTR, IP2STR(&event->ip_info.netmask));
        
        // 更新目前IP到設定中
        char ip_str[16];
        snprintf(ip_str, sizeof(ip_str), IPSTR, IP2STR(&event->ip_info.ip));
        config_set_current_ip(ip_str);
        
        wifi_connected = true;
        retry_count = 0;  // 重設重試計數
        
        // 初始化 SNTP
        sntp_init_time();
        
        // 啟動 MQTT（延遲一點讓網路穩定）
        vTaskDelay(pdMS_TO_TICKS(3000));
        mqtt_init();
        
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_LOST_IP) {
        ESP_LOGW(TAG, "Lost IP address");
        wifi_connected = false;
    }
}

// WiFi 初始化主流程 - 加強除錯資訊
void wifi_init(void) {
    ESP_LOGI(TAG, "=== WiFi Initialization Starting ===");
    
    // 1. 初始化 NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "NVS initialized");
    
    // 2. 初始化設定管理
    ESP_ERROR_CHECK(config_init());
    ESP_LOGI(TAG, "Config manager initialized");
    
    // 3. 顯示設定資訊用於除錯
    ESP_LOGI(TAG, "=== Current WiFi Configuration ===");
    ESP_LOGI(TAG, "SSID: '%s'", g_device_config.wifi_ssid);
    ESP_LOGI(TAG, "Password length: %d", strlen(g_device_config.wifi_password));
    ESP_LOGI(TAG, "Device MAC: %s", g_device_config.device_mac);
    ESP_LOGI(TAG, "IP Mode: %d", g_device_config.ip_mode);
    
    // 4. 初始化 TCP/IP 網路堆疊
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_LOGI(TAG, "Network interface initialized");
    
    // 5. 建立系統事件迴圈
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_LOGI(TAG, "Event loop created");
    
    // 6. 建立預設的 WiFi STA 網路介面
    sta_netif = esp_netif_create_default_wifi_sta();
    ESP_LOGI(TAG, "WiFi STA interface created");
    
    // 7. 設定 WiFi 初始化參數
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_LOGI(TAG, "WiFi driver initialized");
    
    // 8. 註冊事件處理函式
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_LOST_IP, &wifi_event_handler, NULL));
    ESP_LOGI(TAG, "Event handlers registered");
    
    // 9. 設定 WiFi 連線參數
    wifi_config_t wifi_config = {0};
    strcpy((char*)wifi_config.sta.ssid, g_device_config.wifi_ssid);
    strcpy((char*)wifi_config.sta.password, g_device_config.wifi_password);
    
    // 根據密碼設定驗證模式
    if (strlen(g_device_config.wifi_password) == 0) {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_OPEN;
        ESP_LOGI(TAG, "Authentication: OPEN (no password)");
    } else {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        ESP_LOGI(TAG, "Authentication: WPA2_PSK");
    }
    
    wifi_config.sta.pmf_cfg.capable = true;
    wifi_config.sta.pmf_cfg.required = false;
    
    // 10. 設定 WiFi 為 STA 模式
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_LOGI(TAG, "WiFi mode set to STA");
    
    // 11. 設定 WiFi 連線參數
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_LOGI(TAG, "WiFi configuration set");
    
    // 12. 套用IP設定（DHCP或固定IP）
    wifi_apply_ip_config();
    
    // 13. 啟動 WiFi
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "WiFi started - attempting connection to '%s'", g_device_config.wifi_ssid);
    
    ESP_LOGI(TAG, "=== WiFi Initialization Complete ===");
}

// 檢查 WiFi 連線狀態
bool wifi_is_connected(void) {
    return wifi_connected;
}

// 取得目前 IP 地址
char* wifi_get_current_ip(void) {
    return g_device_config.current_ip;
}

// 手動套用新的 IP 設定
void wifi_apply_new_ip_config(void) {
    if (wifi_connected) {
        ESP_LOGI(TAG, "Applying new IP configuration");
        wifi_apply_ip_config();
        
        if (g_device_config.ip_mode == 1) {
            vTaskDelay(pdMS_TO_TICKS(1000));
            wifi_reconnect_with_new_config();
        }
    }
}