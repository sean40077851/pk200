@echo off
echo === VS Code ESP-IDF 編譯腳本 ===

REM 設定 ESP-IDF 環境
echo 設定 ESP-IDF 環境...
call D:\Espressif\frameworks\esp-idf-v5.4.2\export.bat

REM 設定目標
echo 設定目標為 ESP32...
idf.py set-target esp32
if %errorlevel% neq 0 (
    echo ✗ 目標設定失敗
    pause
    exit /b 1
)

REM 開始編譯
echo 開始編譯專案...
idf.py build

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo ✓ 專案編譯成功！
    echo ========================================
    echo.
    echo 現在可以在 VS Code 中：
    echo 1. 使用 Ctrl+Shift+P 開啟命令面板
    echo 2. 搜尋 "ESP-IDF: Build project"
    echo 3. 或使用 "ESP-IDF: Flash device" 燒錄
    echo 4. 或使用 "ESP-IDF: Monitor device" 監控
    echo.
) else (
    echo.
    echo ✗ 專案編譯失敗
    echo.
    echo 請檢查上方的錯誤訊息
    echo 常見問題：
    echo 1. 檢查 ESP-IDF 環境是否正確設定
    echo 2. 檢查 VS Code ESP-IDF 擴展是否已安裝
    echo 3. 檢查專案路徑是否包含中文或特殊字符
)

echo.
pause
