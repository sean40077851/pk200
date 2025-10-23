#ifndef WIFI_H
#define WIFI_H

#include <stdbool.h>
#include "esp_event_base.h"
// WiFi 初始化
void wifi_init(void);

// WiFi 重新連線（使用新設定）
void wifi_reconnect_with_new_config(void);

void restore_time_from_nvs(void);
void nvs_time_init(void);

// 檢查 WiFi 連線狀態
bool wifi_is_connected(void);

// 取得目前 IP 地址
char* wifi_get_current_ip(void);

// 套用 IP 設定 (DHCP 或固定IP)
void wifi_apply_ip_config(void);

// 手動套用新的 IP 設定（由 MQTT 呼叫）
void wifi_apply_new_ip_config(void);
const char *wifi_get_connected_ssid(void);

// WiFi 事件處理函式
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

#endif // WIFI_H