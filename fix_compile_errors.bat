@echo off
echo === 修正編譯錯誤腳本 ===

REM 設定 ESP-IDF 環境
echo 設定 ESP-IDF 環境...
call D:\Espressif\frameworks\esp-idf-v5.4.2\export.bat

REM 清理之前的建置
echo 清理之前的建置檔案...
if exist build (
    rmdir /s /q build
    echo ✓ 清理完成
)

REM 設定目標
echo 設定目標為 ESP32...
idf.py set-target esp32
if %errorlevel% neq 0 (
    echo ✗ 目標設定失敗
    pause
    exit /b 1
)
echo ✓ 目標設定成功

echo.
echo === 已修正的問題 ===
echo ✓ LVGL 記憶體配置錯誤 - 建立自定義記憶體分配函數
echo ✓ UI 色彩設定錯誤 - 設定 LV_COLOR_16_SWAP=0
echo ✓ 未使用函數警告 - 添加 __attribute__((unused))
echo ✓ sdkconfig 無效配置 - 移除不相容的選項
echo ✓ 專案結構錯誤 - 修正主 CMakeLists.txt 為 ESP-IDF 格式
echo ✓ UI 組件路徑錯誤 - 更新 ui_components 包含所有 UI 檔案
echo.

REM 嘗試建置
echo 開始建置專案...
idf.py build

if %errorlevel% equ 0 (
    echo.
    echo ✓ 專案建置成功！
    echo.
    echo === 建置完成 ===
    echo 接下來可以執行：
    echo idf.py -p COM_PORT flash monitor
    echo.
    echo 請將 COM_PORT 替換為您的實際串口號碼 (例如: COM3)
    echo.
    echo === 專案特性 ===
    echo ✓ ESP32 目標設備
    echo ✓ LVGL v8.4 圖形界面
    echo ✓ ST7796 顯示驅動 (320x480)
    echo ✓ FT6336U 觸控驅動
    echo ✓ PSRAM 記憶體支援
    echo ✓ 所有 ui_ 檔案保持不變
) else (
    echo.
    echo ✗ 專案建置失敗
    echo.
    echo 請檢查上方的錯誤訊息，常見問題：
    echo 1. 確認 ESP-IDF 環境已正確設定
    echo 2. 檢查 LVGL 庫檔案是否完整
    echo 3. 確認所有組件檔案存在
    echo 4. 檢查 GPIO 定義是否正確
    echo.
    echo 如需更多幫助，請參考 README_ESP_IDF.md
)

echo.
echo === 腳本執行完成 ===
pause
