# PK200 LCD ESP-IDF 專案

這個專案已經從 Arduino 框架轉換為 ESP-IDF 框架。

## 專案結構

```
PK200_LCD_ESP-IDF/
├── CMakeLists.txt              # 主要 CMake 配置檔案
├── sdkconfig.defaults          # ESP-IDF 預設配置
├── partitions.csv              # 分區表配置
├── main/                       # 主程式目錄
│   ├── CMakeLists.txt
│   └── main.c                  # 主程式檔案 (從 Main.cpp 轉換)
├── components/                 # ESP-IDF 組件目錄
│   ├── lvgl/                   # LVGL 圖形庫組件
│   │   ├── CMakeLists.txt
│   │   └── lv_conf.h          # LVGL 配置檔案
│   ├── display_driver/         # 顯示驅動組件 (ST7796)
│   │   ├── CMakeLists.txt
│   │   ├── include/
│   │   └── display_driver.c
│   ├── touch_driver/           # 觸控驅動組件 (FT6336U)
│   │   ├── CMakeLists.txt
│   │   ├── include/
│   │   └── touch_driver.c
│   └── ui_components/          # UI 組件 (LVGL UI 檔案)
│       ├── CMakeLists.txt
│       └── include/
├── screens/                    # UI 畫面檔案 (保持不變)
│   ├── ui_Home.c
│   ├── ui_Step1.c
│   ├── ui_Step2.c
│   ├── ui_Connect.c
│   ├── ui_Welcome.c
│   ├── ui_Charging.c
│   └── ui_End.c
├── components/                 # UI 組件檔案 (保持不變)
│   ├── ui_comp_button.c
│   ├── ui_comp_card.c
│   ├── ui_comp_cardbg.c
│   ├── ui_comp_percent.c
│   ├── ui_comp.c
│   └── ui_comp_hook.c
├── images/                     # UI 圖片檔案 (保持不變)
│   ├── ui_img_bg1_png.c
│   ├── ui_img_bg2_png.c
│   ├── ui_img_bg3_png.c
│   └── ... (其他圖片檔案)
├── fonts/                      # UI 字體檔案 (保持不變)
├── ui.c                        # 主 UI 檔案 (保持不變)
├── ui.h                        # UI 標頭檔 (保持不變)
└── ui_helpers.c                # UI 輔助函數 (保持不變)
└── libraries/                  # 原始庫檔案 (保持不變)
```

## 主要轉換內容

### 1. 主程式轉換 (Main.cpp → main.c)
- 移除 Arduino 特定的標頭檔案和函數
- 使用 ESP-IDF 的 GPIO、SPI、I2C、PWM 驅動
- 替換 `millis()` 為 `esp_timer_get_time()`
- 替換 `analogWrite()` 為 LEDC PWM
- 使用 ESP-IDF 的記憶體管理和日誌系統

### 2. 顯示驅動 (Arduino_GFX → display_driver)
- 實作 ST7796 顯示控制器驅動
- 使用 ESP-IDF SPI 驅動
- 支援 320x480 解析度
- 16-bit 色彩格式

### 3. 觸控驅動 (FT6336U → touch_driver)
- 實作 FT6336U 觸控控制器驅動
- 使用 ESP-IDF I2C 驅動
- 支援單點觸控

### 4. LVGL 整合
- 使用 LVGL v8.4
- 配置為使用 PSRAM 作為顯示緩衝區
- 自定義記憶體分配器
- 16-bit 色彩深度，支援色彩交換

### 5. UI 檔案整合
- 所有 `ui_` 開頭的檔案保持不變
- 透過 ui_components 組件整合到專案中

## 硬體配置

### 顯示器 (ST7796)
- MOSI: GPIO 5
- CLK: GPIO 6
- CS: GPIO 7
- DC: GPIO 15
- RST: GPIO 16
- 背光: GPIO 4

### 觸控 (FT6336U)
- SDA: GPIO 10
- SCL: GPIO 9
- RST: GPIO 18
- INT: GPIO 8

## 編譯和燒錄

### Windows 環境：

1. **設定 ESP-IDF 環境**：
```cmd
D:\Espressif\frameworks\esp-idf-v5.4.2\export.bat
```

2. **使用提供的批次檔進行測試**：
```cmd
build_test.bat
```

3. **手動編譯步驟**：
```cmd
idf.py set-target esp32
idf.py build
idf.py -p COM_PORT flash monitor
```

### Linux/Mac 環境：

1. **設定 ESP-IDF 環境**：
```bash
. $HOME/esp/esp-idf/export.sh
```

2. **使用提供的腳本進行測試**：
```bash
chmod +x build_test.sh
./build_test.sh
```

3. **手動編譯步驟**：
```bash
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

## 功能特性

- ✅ LVGL 圖形界面
- ✅ 觸控輸入
- ✅ 顯示輸出
- ✅ 背光控制 (10分鐘無操作自動降低亮度)
- ✅ 記憶體監控
- ✅ FreeRTOS 任務管理
- ✅ PSRAM 支援

## 注意事項

1. 確保 ESP32-S3 設備有足夠的 PSRAM (建議 8MB 或以上)
2. 所有 UI 相關的檔案 (`ui_*`) 保持原樣，無需修改
3. 如需修改硬體配置，請修改對應組件中的 GPIO 定義
4. LVGL 配置可在 `components/lvgl/lv_conf.h` 中調整

## 故障排除

### 常見編譯錯誤及解決方法：

#### 1. "Can not find Python" 錯誤
**解決方法**：
- Windows: 執行 `D:\Espressif\frameworks\esp-idf-v5.4.2\export.bat`
- Linux/Mac: 執行 `. $HOME/esp/esp-idf/export.sh`

#### 2. LVGL 記憶體配置錯誤
**錯誤訊息**：`too few arguments to function 'heap_caps_malloc'`
**解決方法**：
- 建立了自定義記憶體分配函數 `lv_port_mem.c`
- 修正了 `components/lvgl/lv_conf.h` 中的記憶體配置宏定義
- 優先使用 PSRAM，失敗時回退到內部 RAM

#### 3. GPIO 定義錯誤
**解決方法**：確保所有 GPIO 定義使用 `GPIO_NUM_X` 格式而不是直接的數字。

#### 4. UI 色彩設定錯誤
**錯誤訊息**：`LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings`
**解決方法**：修正 `components/lvgl/lv_conf.h` 中的 `LV_COLOR_16_SWAP` 設定為 0。

#### 5. 專案結構錯誤
**錯誤訊息**：`fatal error: lvgl.h: No such file or directory`
**解決方法**：
- 修正主 CMakeLists.txt 從 SquareLine Studio 格式轉換為 ESP-IDF 格式
- 更新 ui_components 組件包含所有 UI 檔案（screens、components、images）
- 正確設定包含路徑

#### 6. size_t 類型未定義錯誤
**錯誤訊息**：`error: unknown type name 'size_t'`
**解決方法**：
- 在 `components/lvgl/lv_conf.h` 中添加 `#include <stddef.h>` 標頭檔
- 將自定義記憶體函數宣告移到 `#endif` 之外，確保在正確的作用域中

#### 7. UI 組件檔案缺失錯誤
**錯誤訊息**：`missing and no known rule to make it`
**解決方法**：從 `components/ui_components/CMakeLists.txt` 中移除不存在的 UI 組件檔案引用，只保留實際存在的檔案。

#### 7. sdkconfig 警告
**警告訊息**：`unknown kconfig symbol`
**解決方法**：已移除不相容的配置選項，使用 `idf.py menuconfig` 進行進一步配置。

#### 5. 目標設備不匹配
**注意**：目前配置為 ESP32，如果使用 ESP32-S3，請修改：
- `sdkconfig.defaults` 中的 `CONFIG_IDF_TARGET="esp32s3"`
- 執行 `idf.py set-target esp32s3`

### 一般故障排除步驟：

1. **檢查 ESP-IDF 版本** (建議 v5.0 或以上)
2. **確認所有組件的路徑正確**
3. **檢查 LVGL 庫檔案是否完整**
4. **確認分區表配置適合您的 Flash 大小**
5. **清理並重新建置**：
   ```cmd
   idf.py fullclean
   idf.py build
   ```

### 如果仍有問題：

1. 檢查終端機輸出的具體錯誤訊息
2. 確認硬體連接正確
3. 檢查 USB 驅動程式是否正確安裝
4. 嘗試使用不同的 COM 埠或 USB 線
