@echo off
echo === 最終編譯測試腳本 ===

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
echo === 最新修正的問題 ===
echo ✓ LVGL size_t 類型錯誤 - 移動函數宣告到正確位置
echo ✓ UI 組件檔案缺失錯誤 - 移除不存在的組件檔案引用
echo ✓ 保留所有實際存在的 UI 檔案 (screens, images)
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
    echo - 7個 UI 畫面 (Home, Step1, Step2, Connect, Welcome, Charging, End)
    echo - 19個圖片資源
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
