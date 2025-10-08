#include <stdio.h>                          // 標準輸入輸出函式庫
#include "nvs_flash.h"                      // NVS（非易失性儲存）相關函式
#include "esp_wifi.h"                       // ESP-IDF WiFi 控制函式
#include "esp_event.h"                      // ESP-IDF 事件系統
#include "esp_log.h"                        // ESP-IDF 日誌系統
#include "esp_err.h"                        // ESP-IDF 錯誤碼
#include "esp_sntp.h"                       // SNTP（網路時間同步）函式
#include "esp_netif.h"                      // 網路介面函式
#include "config_manager.h"                 // 設定管理模組
#include <time.h>                           // 標準時間函式
#include <string.h>                         // 字串處理函式

#define TAG "WIFI"                          // 日誌標籤
static esp_netif_t *sta_netif = NULL;       // STA（Station）網路介面指標
static bool wifi_connected = false;         // WiFi 連線狀態旗標
static char current_connected_ssid[33] = "";// 目前連線的 SSID（最多32字元+結尾字元）
static int retry_count = 0;                 // 連線重試次數
static const int max_retry = 3;             // 最大重試次數

// 外部函數聲明
extern void mqtt_init(void);                // MQTT 初始化函式
extern void mqtt_reconnect_with_new_config(void); // MQTT 重新連線函式

// SNTP 初始化（同步網路時間）
void sntp_init_time(void) {
    if (esp_sntp_enabled()) {               // 如果 SNTP 已啟用
        ESP_LOGI(TAG, "SNTP already initialized, skip"); // 輸出日誌
        return;                             // 不重複初始化
    }
    ESP_LOGI(TAG, "Initializing SNTP...");  // 輸出日誌
    sntp_setoperatingmode(SNTP_OPMODE_POLL);// 設定 SNTP 為輪詢模式
    sntp_setservername(0, "pool.ntp.org");  // 設定 NTP 伺服器
    sntp_init();                            // 啟動 SNTP
    setenv("TZ", "CST-8", 1);               // 設定時區（中國標準時間）
    tzset();                                // 套用時區設定
}

// 套用 IP 設定（DHCP 或固定 IP）
void wifi_apply_ip_config(void) {
    if (!sta_netif) {                       // 檢查 STA 網路介面是否初始化
        ESP_LOGE(TAG, "STA netif not initialized"); // 輸出錯誤
        return;
    }
    ESP_LOGI(TAG, "Applying IP config, mode: %d", g_device_config.ip_mode); // 輸出目前 IP 模式
    if (g_device_config.ip_mode == 0) {     // 0: DHCP 模式
        ESP_LOGI(TAG, "Setting DHCP mode");
        esp_netif_dhcpc_start(sta_netif);   // 啟動 DHCP 客戶端
    } else if (g_device_config.ip_mode == 1) { // 1: 固定 IP 模式
        ESP_LOGI(TAG, "Setting static IP: %s", g_device_config.static_ip);
        esp_netif_dhcpc_stop(sta_netif);    // 停止 DHCP
        esp_netif_ip_info_t ip_info;
        memset(&ip_info, 0, sizeof(esp_netif_ip_info_t)); // 清空結構體
        ip_info.ip.addr = esp_ip4addr_aton(g_device_config.static_ip); // 設定 IP
        ip_info.gw.addr = esp_ip4addr_aton(g_device_config.gateway);   // 設定閘道
        ip_info.netmask.addr = esp_ip4addr_aton(g_device_config.subnet); // 設定子網
        esp_err_t ret = esp_netif_set_ip_info(sta_netif, &ip_info); // 套用設定
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to set static IP: %s", esp_err_to_name(ret)); // 設定失敗
        } else {
            ESP_LOGI(TAG, "Static IP set successfully"); // 設定成功
            config_set_current_ip(g_device_config.static_ip); // 更新目前 IP
        }
    } else {                                // 2: 預設模式（DHCP）
        ESP_LOGI(TAG, "Using default DHCP mode");
        esp_netif_dhcpc_start(sta_netif);
    }
}

// WiFi 重新連線（使用新設定）
void wifi_reconnect_with_new_config(void) {
    ESP_LOGI(TAG, "=== Reconnecting WiFi with new configuration ===");
    wifi_connected = false;                 // 清除連線狀態
    retry_count = 0;                        // 重設重試次數
    ESP_LOGI(TAG, "Gracefully disconnecting from current AP...");
    esp_wifi_disconnect();                  // 斷開 WiFi
    vTaskDelay(pdMS_TO_TICKS(1000));        // 等待 1 秒
    ESP_LOGI(TAG, "Stopping WiFi driver...");
    esp_wifi_stop();                        // 停止 WiFi
    vTaskDelay(pdMS_TO_TICKS(2000));        // 等待 2 秒
    ESP_LOGI(TAG, "Configuring WiFi with new settings...");
    wifi_config_t wifi_config = {0};        // 建立 WiFi 設定結構
    strcpy((char*)wifi_config.sta.ssid, g_device_config.wifi_ssid); // 設定 SSID
    strcpy((char*)wifi_config.sta.password, g_device_config.wifi_password); // 設定密碼
    if (strlen(g_device_config.wifi_password) == 0) { // 無密碼
        wifi_config.sta.threshold.authmode = WIFI_AUTH_OPEN;
        ESP_LOGI(TAG, "Using OPEN authentication (no password)");
    } else {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        ESP_LOGI(TAG, "Using WPA2_PSK authentication");
    }
    wifi_config.sta.pmf_cfg.capable = true; // 啟用 PMF
    wifi_config.sta.pmf_cfg.required = false;
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config)); // 套用 WiFi 設定
    wifi_apply_ip_config();                  // 套用 IP 設定
    ESP_LOGI(TAG, "Starting WiFi with SSID: %s", g_device_config.wifi_ssid);
    ESP_ERROR_CHECK(esp_wifi_start());       // 啟動 WiFi
    ESP_LOGI(TAG, "=== WiFi reconnection initiated ===");
}

// WiFi 事件處理函式
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {

    // -------------------------- //
    //  STA 模式啟動事件處理   //
    // -------------------------- //
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        // 代表 Wi-Fi Station 模式啟動完成，準備進行連線。
        ESP_LOGI(TAG, "WiFi STA started, connecting to SSID: %s", g_device_config.wifi_ssid);

        // 立刻嘗試連線至設定中的 SSID。
        esp_wifi_connect();                  
    }

    // -------------------------- //
    // 成功連上 AP 事件處理   //
    // -------------------------- //
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
        // 當 Wi-Fi 成功連上 AP 時會觸發此事件。
        wifi_event_sta_connected_t *connected = (wifi_event_sta_connected_t *) event_data; // 取得事件資料（包含 SSID 名稱與長度）

        size_t copy_len = connected->ssid_len;  // 取得 SSID 的實際長度
        if (copy_len >= sizeof(current_connected_ssid)) {
            copy_len = sizeof(current_connected_ssid) - 1; // 避免超過緩衝區大小（防止溢位）
        }

        // 將 SSID 複製到全域變數中保存（方便後續 UI 顯示或狀態查詢）
        memcpy(current_connected_ssid, connected->ssid, copy_len);
        current_connected_ssid[copy_len] = '\0';  // 加上字串結尾符號，確保為合法字串
        ESP_LOGI(TAG, "Connected to AP: %s", current_connected_ssid);
    }

    // -------------------------- //
    //  Wi-Fi 斷線事件處理     //
    // -------------------------- //
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        // 當 Wi-Fi 失去連線時觸發（例如 AP 關機、訊號太弱、密碼錯誤等）
        wifi_event_sta_disconnected_t* disconnected = (wifi_event_sta_disconnected_t*) event_data;

        wifi_connected = false;           // 標記目前為未連線狀態
        current_connected_ssid[0] = '\0'; // 清空已保存的 SSID（避免顯示錯誤的舊值）

        ESP_LOGW(TAG, "WiFi disconnected, reason: %d", disconnected->reason); // 印出斷線原因代碼（整數）

        // 根據具體的斷線原因顯示詳細提示
        switch (disconnected->reason) {
            case WIFI_REASON_NO_AP_FOUND:
                // 找不到指定的 SSID（AP 可能不存在或超出範圍）
                ESP_LOGE(TAG, "AP not found - Check if SSID '%s' exists", g_device_config.wifi_ssid);
                break;
            case WIFI_REASON_AUTH_FAIL:
                // 驗證失敗（可能是密碼錯誤）
                ESP_LOGE(TAG, "Authentication failed - Check WiFi password");
                break;
            case WIFI_REASON_ASSOC_LEAVE:
                // AP 主動斷開連線（例如路由器設定斷開閒置設備）
                ESP_LOGW(TAG, "Disconnected by AP");
                break;
            case WIFI_REASON_CONNECTION_FAIL:
                // 連線失敗，可能是信號干擾或設定錯誤
                ESP_LOGE(TAG, "Connection failed - Check AP settings");
                break;
            default:
                // 其他不常見的斷線代碼
                ESP_LOGW(TAG, "Other disconnect reason: %d", disconnected->reason);
                break;
        }

        // ----------- 自動重試機制 ----------- //
        if (retry_count < max_retry) {
            // 若尚未超過最大重試次數（例如 max_retry = 3）
            retry_count++; // 增加重試計數
            ESP_LOGI(TAG, "Retry connecting (%d/%d)...", retry_count, max_retry);

            // 等待 3秒再重新嘗試連線（給 Wi-Fi driver 與網路時間恢復）
            vTaskDelay(pdMS_TO_TICKS(3000));
            esp_wifi_connect(); // 再次呼叫連線
        } else {
            // 若重試次數達上限，進入錯誤保護機制
            ESP_LOGE(TAG, "Max retry reached. Please check WiFi settings:");
            ESP_LOGE(TAG, "  SSID: %s", g_device_config.wifi_ssid);
            ESP_LOGE(TAG, "  Password: %s", strlen(g_device_config.wifi_password) > 0 ? "***SET***" : "***EMPTY***");
            ESP_LOGE(TAG, "Waiting 30 seconds before next attempt...");

            // 將 Wi-Fi 設定重置為預設值（例如出廠狀態的 Wi-Fi 名稱與密碼）
            config_reset_to_default();

            // 使用預設設定重新初始化 Wi-Fi 並重新連線
            wifi_reconnect_with_new_config();

            // 暫停 10 秒再嘗試下一次連線（避免過於頻繁重連）
            vTaskDelay(pdMS_TO_TICKS(10000));

            retry_count = 0; // 重設重試次數
            esp_wifi_connect(); // 再次發起連線
        }
    }

    // -------------------------- //
    // [4] 成功取得 IP 事件處理   //
    // -------------------------- //
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        // 當 Wi-Fi 成功取得 IP 位址時觸發
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;

        // 顯示獲得的 IP、Gateway、Netmask
        ESP_LOGI(TAG, "WiFi connected successfully!");
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI(TAG, "Gateway: " IPSTR, IP2STR(&event->ip_info.gw));
        ESP_LOGI(TAG, "Netmask: " IPSTR, IP2STR(&event->ip_info.netmask));

        // 將 IP 位址轉成字串格式以儲存或顯示
        char ip_str[16];
        snprintf(ip_str, sizeof(ip_str), IPSTR, IP2STR(&event->ip_info.ip));

        // 儲存目前 IP 到設定中（讓其他模組或 UI 可以取得）
        config_set_current_ip(ip_str);

        // 標記已連線成功
        wifi_connected = true;

        // 取得 AP 資訊以確認目前連線的 SSID（可防止事件順序錯亂造成錯誤顯示）
        wifi_ap_record_t ap_info;
        if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
            size_t copy_len = strnlen((const char *) ap_info.ssid, sizeof(ap_info.ssid));
            if (copy_len >= sizeof(current_connected_ssid)) {
                copy_len = sizeof(current_connected_ssid) - 1;
            }
            memcpy(current_connected_ssid, ap_info.ssid, copy_len);
            current_connected_ssid[copy_len] = '\0';
            ESP_LOGI(TAG, "Confirmed AP SSID: %s", current_connected_ssid);
        }

        // 重設重試計數（避免下一次連線時誤判為多次失敗）
        retry_count = 0;

        // 啟動 SNTP 同步時間（確保設備時間正確，用於 MQTT、日誌、TLS 驗證）
        sntp_init_time();

        // 等待 3 秒讓網路穩定再啟動 MQTT 連線
        vTaskDelay(pdMS_TO_TICKS(3000));
        mqtt_init(); // 啟動 MQTT client（連線 broker）
    }

    // -------------------------- //
    // [5] IP 遺失事件處理        //
    // -------------------------- //
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_LOST_IP) {
        // 表示網路連線雖然還在，但 DHCP 租約或 IP 已失效
        ESP_LOGW(TAG, "Lost IP address");

        // 標記為未連線狀態
        wifi_connected = false;

        // 清空已記錄的 SSID（避免後續顯示錯誤資訊）
        current_connected_ssid[0] = '\0';
    }
}
// WiFi 初始化主流程
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
    // 3. 顯示設定資訊
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
    return wifi_connected;                   // 回傳連線狀態
}

// 取得目前連線的 SSID
const char *wifi_get_connected_ssid(void) {
    return current_connected_ssid;           // 回傳目前 SSID 字串指標
}

// 取得目前 IP 地址
char* wifi_get_current_ip(void) {
    return g_device_config.current_ip;       // 回傳目前 IP 字串指標
}

// 手動套用新的 IP 設定
void wifi_apply_new_ip_config(void) {
    if (wifi_connected) {                    // 僅在已連線時執行
        ESP_LOGI(TAG, "Applying new IP configuration");
        wifi_apply_ip_config();              // 套用 IP 設定
        if (g_device_config.ip_mode == 1) {  // 若為固定 IP
            vTaskDelay(pdMS_TO_TICKS(1000));
            wifi_reconnect_with_new_config();// 重新連線
        }
    }
}
