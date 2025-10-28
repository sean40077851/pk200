#ifndef __SDCARD_DRIVER_H__
#define __SDCARD_DRIVER_H__

#include "esp_err.h"

#define MOUNT_POINT "/sdcard"
#define SDIO_FILELINE_MAX_CHAR_SIZE 1024

#ifdef __cplusplus
extern "C" {
#endif

// === 初始化 SD 卡 ===
// 掛載 FAT 檔案系統並檢查卡片狀態
void sdcard_init(void);

// === 寫入資料到 SD 卡 ===
// path: 檔案路徑 (例如 "/sdcard/test.txt")
// data: 要寫入的字串
esp_err_t sdio_write_file(const char* path, const char* data);

// === 讀取資料從 SD 卡 ===
// path: 檔案路徑
// buffer: 用來存放讀取結果的緩衝區
// max_len: 緩衝區最大長度
esp_err_t sdio_read_file(const char* path, char* buffer, size_t max_len);

// === 測試 SD 卡寫入與讀取 ===
void sdcard_test(void);

#ifdef __cplusplus
}
#endif

#endif // __SDCARD_DRIVER_H__