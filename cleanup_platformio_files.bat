@echo off
echo === 清理 PlatformIO 遺留檔案腳本 ===

echo 正在清理 PlatformIO 相關檔案和目錄...

REM PlatformIO 配置檔案
if exist "platformio.ini" (
    del "platformio.ini"
    echo ✓ 刪除 platformio.ini
)

REM PlatformIO 目錄結構
if exist "src" (
    rmdir /s /q "src"
    echo ✓ 刪除 src 目錄 (已轉移到 main)
)

if exist "lib" (
    rmdir /s /q "lib"
    echo ✓ 刪除 lib 目錄
)

if exist "include" (
    rmdir /s /q "include"
    echo ✓ 刪除 include 目錄
)

if exist "test" (
    rmdir /s /q "test"
    echo ✓ 刪除 test 目錄
)

REM 舊的 README 檔案
if exist "README.md" (
    del "README.md"
    echo ✓ 刪除舊的 README.md
)

if exist "readme.txt" (
    del "readme.txt"
    echo ✓ 刪除 readme.txt
)

REM 其他 PlatformIO 相關檔案
if exist "filelist.txt" (
    del "filelist.txt"
    echo ✓ 刪除 filelist.txt
)

if exist "APP_16MB.csv" (
    del "APP_16MB.csv"
    echo ✓ 刪除 APP_16MB.csv
)

REM 清理不需要的庫檔案 (保留 LVGL 相關)
echo 清理不需要的庫檔案...

set "libraries_to_remove=AccelStepper Adafruit_BusIO Adafruit_GFX_Library Adafruit_ILI9341 Adafruit_ST7789_Library Adafruit_ST7796S Adafruit_seesaw_Library AnimatedGIF ArduinoJson ArduinoModbus ArduinoRS485 Arduino_ESP32_OTA Arduino_GT911_Library Arduino_GigaDisplayTouch Arduino_GigaDisplay_GFX Arduino_JSON BitBang_I2C Button2 ESP32_HUB75_LED_MATRIX_PANEL_DMA_Display FT6236 FT6336U FT81x_Arduino_Driver FastLED GFX_Library_for_Arduino GFX_Library_for_Arduino-1.5.0 GFX_Root GxEPD2 HUB75nano LittleFS_esp32-1.0.6 ModbusMaster PNGdec RunningAverage SD TAMC_FT62X6 TAMC_GT911 TFT_eSPI TJpg_Decoder Time U8g2 Vector WiFiManager lvgl lvgl_8.3.9"

for %%i in (%libraries_to_remove%) do (
    if exist "libraries\%%i" (
        rmdir /s /q "libraries\%%i"
        echo ✓ 刪除庫: %%i
    )
)

REM 清理 libraries 目錄中的其他檔案
if exist "libraries\lv_conf.h" (
    del "libraries\lv_conf.h"
    echo ✓ 刪除 libraries\lv_conf.h (已移到 components/lvgl)
)

REM 清理根目錄中散落的 UI 組件檔案 (已移到 components 目錄)
set "ui_files_to_remove=ui_comp.c ui_comp.h ui_comp_button.c ui_comp_button.h ui_comp_card.c ui_comp_card.h ui_comp_cardbg.c ui_comp_cardbg.h ui_comp_hook.c ui_comp_hook.h ui_comp_percent.c ui_comp_percent.h"

for %%i in (%ui_files_to_remove%) do (
    if exist "components\%%i" (
        del "components\%%i"
        echo ✓ 刪除重複的組件檔案: %%i
    )
)

echo.
echo === 清理完成 ===
echo.
echo 已保留的重要檔案和目錄：
echo ✓ CMakeLists.txt (ESP-IDF 主配置)
echo ✓ main/ (主程式目錄)
echo ✓ components/ (ESP-IDF 組件)
echo ✓ libraries/lvgl-release-v8.4/ (LVGL 庫)
echo ✓ screens/ (UI 畫面檔案)
echo ✓ images/ (UI 圖片檔案)
echo ✓ fonts/ (UI 字體檔案)
echo ✓ ui.c, ui.h, ui_helpers.c, ui_helpers.h (主要 UI 檔案)
echo ✓ sdkconfig.defaults (ESP-IDF 配置)
echo ✓ partitions.csv (分區表)
echo ✓ README_ESP_IDF.md (ESP-IDF 說明文件)
echo ✓ 各種建置腳本 (.bat 檔案)
echo.
echo 已刪除的 PlatformIO 相關檔案：
echo ✗ platformio.ini
echo ✗ src/ 目錄 (內容已轉移到 main/)
echo ✗ lib/, include/, test/ 目錄
echo ✗ 不需要的庫檔案
echo ✗ 重複的 UI 組件檔案
echo.

pause
