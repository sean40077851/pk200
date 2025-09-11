# PlatformIO 檔案清理指南

## 🗂️ 可以安全刪除的 PlatformIO 檔案

### 📋 配置檔案
- `platformio.ini` - PlatformIO 專案配置檔案

### 📁 目錄結構
- `src/` - 原始碼目錄 (內容已轉移到 `main/`)
- `lib/` - PlatformIO 庫目錄
- `include/` - 標頭檔目錄
- `test/` - 測試目錄

### 📚 不需要的庫檔案 (在 `libraries/` 目錄中)
- `AccelStepper` - 步進馬達庫
- `Adafruit_BusIO` - Adafruit 匯流排庫
- `Adafruit_GFX_Library` - Adafruit 圖形庫
- `Adafruit_ILI9341` - ILI9341 顯示驅動
- `Adafruit_ST7789_Library` - ST7789 顯示驅動
- `Adafruit_ST7796S` - ST7796S 顯示驅動
- `Adafruit_seesaw_Library` - Adafruit seesaw 庫
- `AnimatedGIF` - 動畫 GIF 庫
- `ArduinoJson` - JSON 處理庫
- `ArduinoModbus` - Modbus 通訊庫
- `ArduinoRS485` - RS485 通訊庫
- `Arduino_ESP32_OTA` - OTA 更新庫
- `Arduino_GT911_Library` - GT911 觸控庫
- `Arduino_GigaDisplayTouch` - Arduino Giga 觸控庫
- `Arduino_GigaDisplay_GFX` - Arduino Giga 顯示庫
- `Arduino_JSON` - Arduino JSON 庫
- `BitBang_I2C` - 軟體 I2C 庫
- `Button2` - 按鈕處理庫
- `ESP32_HUB75_LED_MATRIX_PANEL_DMA_Display` - LED 矩陣顯示庫
- `FT6236` - FT6236 觸控庫
- `FT6336U` - FT6336U 觸控庫 (已有自定義驅動)
- `FT81x_Arduino_Driver` - FT81x 驅動
- `FastLED` - LED 控制庫
- `GFX_Library_for_Arduino` - Arduino GFX 庫
- `GFX_Library_for_Arduino-1.5.0` - Arduino GFX 庫舊版本
- `GFX_Root` - GFX 根庫
- `GxEPD2` - 電子紙顯示庫
- `HUB75nano` - HUB75 顯示庫
- `LittleFS_esp32-1.0.6` - LittleFS 檔案系統
- `ModbusMaster` - Modbus 主機庫
- `PNGdec` - PNG 解碼庫
- `RunningAverage` - 移動平均庫
- `SD` - SD 卡庫
- `TAMC_FT62X6` - FT62X6 觸控庫
- `TAMC_GT911` - GT911 觸控庫
- `TFT_eSPI` - TFT 顯示庫
- `TJpg_Decoder` - JPEG 解碼庫
- `Time` - 時間處理庫
- `U8g2` - U8g2 顯示庫
- `Vector` - 向量庫
- `WiFiManager` - WiFi 管理庫
- `lvgl` - 舊版 LVGL (保留 `lvgl-release-v8.4`)
- `lvgl_8.3.9` - LVGL 8.3.9 版本
- `lv_conf.h` - LVGL 配置檔案 (已移到 `components/lvgl/`)

### 📄 其他檔案
- `README.md` - 舊的 README 檔案
- `readme.txt` - 文字說明檔案
- `filelist.txt` - 檔案清單
- `APP_16MB.csv` - 應用程式配置檔案

### 🔄 重複的 UI 組件檔案 (在 `components/` 目錄中)
這些檔案已經整合到 `components/ui_components/` 中：
- `ui_comp.c` / `ui_comp.h`
- `ui_comp_button.c` / `ui_comp_button.h`
- `ui_comp_card.c` / `ui_comp_card.h`
- `ui_comp_cardbg.c` / `ui_comp_cardbg.h`
- `ui_comp_hook.c` / `ui_comp_hook.h`
- `ui_comp_percent.c` / `ui_comp_percent.h`

## ✅ 必須保留的檔案

### 🏗️ ESP-IDF 專案結構
- `CMakeLists.txt` - ESP-IDF 主配置檔案
- `sdkconfig.defaults` - ESP-IDF 預設配置
- `partitions.csv` - 分區表配置
- `main/` - 主程式目錄
- `components/` - ESP-IDF 組件目錄

### 📚 必要的庫
- `libraries/lvgl-release-v8.4/` - LVGL v8.4 圖形庫

### 🎨 UI 相關檔案
- `screens/` - UI 畫面檔案
- `images/` - UI 圖片檔案
- `fonts/` - UI 字體檔案
- `ui.c` / `ui.h` - 主要 UI 檔案
- `ui_helpers.c` / `ui_helpers.h` - UI 輔助函數
- `ui_events.h` - UI 事件定義

### 🔧 建置腳本
- `*.bat` - 各種建置和測試腳本
- `README_ESP_IDF.md` - ESP-IDF 專案說明

## 🚀 清理方法

### 方法 1：使用自動化腳本
```cmd
safe_cleanup_platformio.bat
```

### 方法 2：手動清理
按照上述清單手動刪除不需要的檔案和目錄。

### 方法 3：選擇性清理
只刪除您確定不需要的特定檔案或目錄。

## ⚠️ 注意事項

1. **建議先備份**：清理前建議備份整個專案
2. **保留 LVGL**：確保保留 `libraries/lvgl-release-v8.4/`
3. **保留 UI 檔案**：所有 `ui_` 開頭的檔案都要保留
4. **測試編譯**：清理後執行 `final_fix_and_build.bat` 測試編譯

## 📊 清理後的專案大小

清理前：約 500+ MB (包含所有 Arduino 庫)
清理後：約 50-100 MB (只保留必要檔案)

清理後您的專案將是一個純淨的 ESP-IDF 專案，沒有 PlatformIO 的遺留檔案。
