#ifndef OTA_H
#define OTA_H

#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief OTA 更新狀態。
 */
typedef enum {
    OTA_STATUS_IDLE = 0,
    OTA_STATUS_IN_PROGRESS,
    OTA_STATUS_SUCCESS,
    OTA_STATUS_FAILED
} ota_status_t;

/**
 * @brief OTA 狀態回呼函式型別。
 *
 * @param status 目前狀態。
 * @param error  當狀態為 OTA_STATUS_FAILED 時帶入的錯誤碼，其他狀態為 ESP_OK。
 */
typedef void (*ota_status_callback_t)(ota_status_t status, esp_err_t error);

/**
 * @brief 初始化 OTA 模組。在 app_main 啟動時呼叫。
 */
void ota_init(void);

/**
 * @brief 註冊 OTA 狀態回呼，用於取得 OTA 進度與結果。
 *
 * @param callback 回呼函式指標，若為 NULL 則取消註冊。
 */
void ota_register_status_callback(ota_status_callback_t callback);

/**
 * @brief 判斷是否正在執行 OTA。
 */
bool ota_is_in_progress(void);

/**
 * @brief 啟動 OTA 更新（非同步）。
 *
 * @param url 固件下載連結（HTTP/HTTPS）。
 * @return ESP_OK 代表已成功建立 OTA 任務，其他值代表失敗原因。
 */
esp_err_t ota_start_async(const char *url);

#ifdef __cplusplus
}
#endif

#endif // OTA_H
