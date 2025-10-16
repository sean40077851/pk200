#include <stdio.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_sntp.h"
#include <time.h>
static int current_ap = 0;
void sntp_init_time(void)
{
    // 設定 SNTP 為輪詢模式（每隔一段時間自動同步）
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

    // 設定 NTP 伺服器位址（這裡用 pool.ntp.org，可根據需求更換）
    sntp_setservername(0, "pool.ntp.org"); 

    // 啟動 SNTP 功能，開始與 NTP 伺服器同步時間
    sntp_init();

    // 設定時區為台灣（CST-8），讓 localtime 取得正確本地時間
    setenv("TZ", "CST-8", 1);
    tzset();
}
// WiFi 事件處理函式
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    // 當 WiFi STA 啟動時，自動連線
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }
    // 當 WiFi STA 斷線時，自動重連
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGW("WIFI", "Disconnected from SSID %s", 
                 (current_ap == 0) ? "A100P" : "A100");

<<<<<<< Updated upstream
        if (current_ap == 0) {
            // 如果目前是 A100P，切換到 A100
            wifi_config_t wifi_config = {
                .sta = {
                    .ssid = "A100",
                    .threshold.authmode = WIFI_AUTH_OPEN,
                },
            };
            esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
            current_ap = 1;
            ESP_LOGI("WIFI", "Switching to backup AP: A100");
=======
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
            
            ESP_LOGE(TAG, "Max retry reached. Please check WiFi settings:");
            ESP_LOGE(TAG, "  SSID: %s", g_device_config.wifi_ssid);
            ESP_LOGE(TAG, "  Password: %s", strlen(g_device_config.wifi_password) > 0 ? "***SET***" : "***EMPTY***");
            ESP_LOGE(TAG, "Waiting 10 seconds before next attempt...");

            // 將 Wi-Fi 設定重置為預設值（例如出廠狀態的 Wi-Fi 名稱與密碼）
            config_reset_to_default();

            // 使用預設設定重新初始化 Wi-Fi 並重新連線
            wifi_reconnect_with_new_config();

            // 暫停 10 秒再嘗試下一次連線（避免過於頻繁重連）
            vTaskDelay(pdMS_TO_TICKS(10000));

            retry_count = 0; // 重設重試次數
            
>>>>>>> Stashed changes
        }
        esp_wifi_connect(); // 嘗試重連
    }
    // 當 WiFi STA 獲得 IP 時，顯示 IP
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI("WIFI", "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        sntp_init_time();
    }
}

// WiFi 初始化主流程
void wifi_init(void) {
    // 1. 初始化 NVS（非揮發性儲存，WiFi 驗證資訊會用到）
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // 如果 NVS 空間不足或版本不符，先清除再初始化
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 2. 初始化 TCP/IP 網路堆疊
    ESP_ERROR_CHECK(esp_netif_init());

    // 3. 建立系統事件迴圈（WiFi 事件、IP 事件都靠這個分派）
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // 4. 建立預設的 WiFi STA（Station）網路介面
    esp_netif_create_default_wifi_sta();

    // 5. 設定 WiFi 初始化參數（使用預設值）
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 6. 註冊 WiFi 事件與 IP 事件的處理函式
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);

    // 7. 設定 WiFi 連線參數（SSID、加密模式等）
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "A100P",
            .password = "12345678",
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,                 // 支援 PMF（Protected Management Frames）
                .required = false                // 不強制要求 PMF
            },
        },
    };
    ESP_LOGI("WIFI", "Currently connected to SSID: %s", (current_ap == 0) ? "A100P" : "A100");
    // 8. 設定 WiFi 為 STA（連線到 AP）模式
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // 9. 設定 WiFi 連線參數
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));

    // 10. 啟動 WiFi
    ESP_ERROR_CHECK(esp_wifi_start());
    // 11. 顯示初始化完成訊息
    ESP_LOGI("WIFI", "WiFi initialized in STA mode");

} 