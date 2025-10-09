#ifndef MY_MQTT_H
#define MY_MQTT_H
#include "mqtt_client.h"
#include <stdbool.h>
#include "ota.h"
extern esp_mqtt_client_handle_t mqtt_handle;
// MQTT 初始化
void mqtt_init(void);

// MQTT 重新連線（使用新設定）
void mqtt_reconnect_with_new_config(void);

// 檢查 MQTT 連線狀態
bool mqtt_is_connected(void);

// 發送狀態訊息
void mqtt_publish_status(const char* status_msg);

// 手動發佈裝置資訊
void mqtt_publish_device_info(void);

// 停止 MQTT 客戶端
void mqtt_stop(void);

// 事件回調函式
void mqtt_event_callback(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

void mqtt_reconnect_task(void *pvParameters);

#endif // MY_MQTT_H
