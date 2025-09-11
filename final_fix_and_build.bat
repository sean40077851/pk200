@echo off
echo === 最終修正和編譯腳本 ===

REM 設定 ESP-IDF 環境
echo 設定 ESP-IDF 環境...
call D:\Espressif\frameworks\esp-idf-v5.4.2\export.bat

REM 清理建置目錄
echo 清理建置目錄...
if exist build (
    rmdir /s /q build
    echo 建置目錄已清理
)

echo.
echo === 已修正的問題清單 ===
echo ✓ LVGL 記憶體配置錯誤 - 建立自定義記憶體分配函數
echo ✓ UI 色彩設定錯誤 - 設定 LV_COLOR_16_SWAP=0
echo ✓ 未使用函數警告 - 添加 __attribute__((unused))
echo ✓ sdkconfig 無效配置 - 移除不相容的選項
echo ✓ 專案結構錯誤 - 修正主 CMakeLists.txt 為 ESP-IDF 格式
echo ✓ UI 組件路徑錯誤 - 更新 ui_components 包含所有 UI 檔案
echo ✓ size_t 類型錯誤 - 在 lv_conf.h 中添加 #include ^<stddef.h^>
echo.

REM 設定目標
echo 設定目標為 ESP32...
idf.py set-target esp32
if %errorlevel% neq 0 (
    echo ✗ 目標設定失敗
    pause
    exit /b 1
)
echo ✓ 目標設定成功

REM 開始編譯
echo 開始編譯專案...
idf.py build

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo ✓ 專案編譯成功！
    echo ========================================
    echo.
    echo 專案特性：
    echo - ESP32 目標設備
    echo - LVGL v8.4 圖形界面
    echo - ST7796 顯示驅動 (320x480)
    echo - FT6336U 觸控驅動
    echo - PSRAM 記憶體支援
    echo - 完整的充電樁 UI (7個畫面)
    echo - 所有 ui_ 檔案保持不變
    echo.
    echo 接下來可以執行：
    echo idf.py -p COM_PORT flash monitor
    echo.
    echo 請將 COM_PORT 替換為您的實際串口號碼
    echo ========================================
) else (
    echo.
    echo ✗ 專案編譯失敗
    echo.
    echo 請檢查上方的錯誤訊息
    echo 如果還有問題，請提供具體的錯誤訊息以便進一步協助
)

echo.
echo === 腳本執行完成 ===
pause
